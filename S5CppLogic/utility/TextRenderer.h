#pragma once

#include <shok/ui/s5_ui.h>
#include <shok/engine/s5_RWEngine.h>
#include <shok/engine/s5_RWE_2d.h>

namespace CppLogic {
    template<class T>
    class TextRenderer {
        enum class TextAlignment : int {
            Left, Center, Right,
        };

        shok::UIRenderer *Renderer;
        const T *Buff;
        RWE::P2D::Rt2dFont *Font;
        RWE::RwV2d Anchor;
        RWE::RwV2d PosTransform;
        float End;
        const float StartX;
        const float FontSize;
        const float LineDistance;
        bool ContainsAt = false;
        TextAlignment Alignment = TextAlignment::Left;
        const T *StrPos;
        T *LinePos;
        shok::UIRenderCustomColorContext *CustomColorData;
        const shok::Color DefaultColor;
        bool SkipWhitespaceActive = true;
        float ActiveTabSplit = -1.0f;

        static inline T Line[5001]{};

    public:
        TextRenderer(shok::UIRenderer *r, const T *txt, RWE::P2D::Rt2dFont *f, const RWE::RwV2d &anchor, float end,
                     float ldf, const RWE::RwV2d &posTransform, shok::UIRenderCustomColorContext *customcolordata,
                     shok::Color defaultcolor)
            : Renderer(r), Buff(txt), Font(f), Anchor(anchor), PosTransform(posTransform), End(end), StartX(anchor.x),
        FontSize(f->GetHeight()), LineDistance(FontSize * (ldf == 0.0f ? 1.0f : ldf)), StrPos(Buff), LinePos(Line),
        CustomColorData(customcolordata), DefaultColor(defaultcolor) {}

    private:
        static constexpr const T *txt_cr() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"cr";
            else
                return "cr";
        }

        static constexpr const T *txt_ra() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"ra";
            else
                return "ra";
        }

        static constexpr const T *txt_la() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"la";
            else
                return "la";
        }

        static constexpr const T *txt_center() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"center";
            else
                return "center";
        }

        static constexpr const T *txt_bs() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"bs";
            else
                return "bs";
        }

        static constexpr const T *txt_wsoff() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"skipWhitespace:off";
            else
                return "skipWhitespace:off";
        }

        static constexpr const T *txt_wson() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"skipWhitespace:on";
            else
                return "skipWhitespace:on";
        }

        static constexpr const T *txt_color() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"color:";
            else
                return "color:";
        }

        static constexpr const T *txt_customcolor() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"customcolor:";
            else
                return "customcolor:";
        }

        static constexpr const T *txt_defaultcolor() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"defaultcolor";
            else
                return "defaultcolor";
        }

        static constexpr const T *txt_icon() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"icon";
            else
                return "icon";
        }

        static constexpr const T *txt_tab() {
            if constexpr (std::same_as<T, wchar_t>)
                return L"tab:";
            else
                return "tab:";
        }

        static bool CharsMatch(const T *txt, const T *sear, bool checkend) {
            while (*sear) {
                if (*txt != *sear)
                    return false;
                ++txt;
                ++sear;
            }
            if (checkend) {
                return *txt == ' ' || *txt == '|';
            }
            return true;
        }

        static void SkipWhitespace(const T *&txt) {
            while (*txt == ' ') {
                ++txt;
            }
        }

        void PrepareLine() {
            T *lastwordpos = LinePos;
            const T *lastwordsourcepos = StrPos;
            ContainsAt = false;
            if (SkipWhitespaceActive)
                SkipWhitespace(StrPos);
            while (true) {
                if (!*StrPos) {
                    *LinePos = '\0';
                    if (End && LinePos != lastwordpos && lastwordpos != Line) {
                        if ((Font->GetStringWidth(Line, FontSize) + Anchor.x) >= End) {
                            LinePos = lastwordpos;
                            *LinePos = '\0';
                            StrPos = lastwordsourcepos;
                            break;
                        }
                    }
                    break;
                }
                if (*StrPos == '\r' || *StrPos == '\n') {
                    ++StrPos;
                    if (*StrPos == '\r' || *StrPos == '\n')
                        ++StrPos;
                    *LinePos = '\0';
                    break;
                }
                if (*StrPos == '@') {
                    if (StrPos[1] == '@') {
                        LinePos[0] = '@';
                        LinePos[1] = '@';
                        LinePos += 2;
                        StrPos += 2;
                        continue;
                    } else if (CharsMatch(&StrPos[1], txt_cr(), true)) {
                        StrPos += 3;
                        if (*StrPos == '|')
                            ++StrPos;
                        *LinePos = '\0';
                        break;
                    } else if (CharsMatch(&StrPos[1], txt_ra(), true)) {
                        Alignment = TextAlignment::Right;
                        StrPos += 3;
                    } else if (CharsMatch(&StrPos[1], txt_la(), true)) {
                        Alignment = TextAlignment::Left;
                        StrPos += 3;
                    } else if (CharsMatch(&StrPos[1], txt_center(), true)) {
                        Alignment = TextAlignment::Center;
                        StrPos += 7;
                    } else if (CharsMatch(&StrPos[1], txt_bs(), true)) {
                        if (LinePos == Line) {
                            StrPos += 4;
                            continue;
                        }
                        if (LinePos[-1] == ' ')
                            --LinePos;
                        T prev = *LinePos;
                        *LinePos = '\0';
                        if (End && (Font->GetStringWidth(Line, FontSize) + Anchor.x) >= End) {
                            if (LinePos != lastwordpos && lastwordpos != Line) {
                                LinePos = lastwordpos;
                                *LinePos = '\0';
                                StrPos = lastwordsourcepos;
                            }
                            break;
                        }
                        *LinePos = prev;
                        StrPos += 4;
                        lastwordpos = LinePos;
                        lastwordsourcepos = StrPos;
                        continue;
                    } else if (CharsMatch(&StrPos[1], txt_wsoff(), true)) {
                        SkipWhitespaceActive = false;
                        StrPos += 19;
                    } else if (CharsMatch(&StrPos[1], txt_wson(), true)) {
                        SkipWhitespaceActive = true;
                        StrPos += 18;
                    } else if (CharsMatch(&StrPos[1], txt_tab(), false)) {
                        StrPos += 5;
                        float newPos = ParseFloatParam(StrPos);
                        if (newPos < 1.0f) {
                            newPos *= End - StartX;
                        }
                        ActiveTabSplit = newPos;
                        if (*StrPos == '|')
                            ++StrPos;
                        *LinePos = '\0';
                        break;
                    } else {
                        ContainsAt = true;
                        while (true) {
                            *LinePos = *StrPos;
                            ++LinePos;
                            ++StrPos;
                            if (*StrPos == '\0')
                                break;
                            if (*StrPos == '|') {
                                *LinePos = '|';
                                ++LinePos;
                                ++StrPos;
                                if (SkipWhitespaceActive) {
                                    SkipWhitespace(StrPos);
                                }
                                break;
                            }
                            if (*StrPos == ' ') {
                                *LinePos = '|';
                                ++LinePos;
                                if (SkipWhitespaceActive) {
                                    ++StrPos;
                                    SkipWhitespace(StrPos);
                                }
                                break;
                            }
                        }
                        continue;
                    }
                    if (*StrPos == '|')
                        ++StrPos;
                    if (SkipWhitespaceActive)
                        SkipWhitespace(StrPos);
                    continue;
                }
                *LinePos = *StrPos;
                ++StrPos;
                if (*LinePos == ' ' && End && LinePos != Line) {
                    *LinePos = '\0';
                    if ((Font->GetStringWidth(Line, FontSize) + Anchor.x) >= End) {
                        if (LinePos != lastwordpos && lastwordpos != Line) {
                            LinePos = lastwordpos;
                            *LinePos = '\0';
                            StrPos = lastwordsourcepos;
                        }
                        break;
                    }
                    *LinePos = ' ';
                    lastwordpos = LinePos;
                    lastwordsourcepos = StrPos;
                    if (SkipWhitespaceActive)
                        SkipWhitespace(StrPos);
                }
                ++LinePos;
            }
        }

        void NextLine() {
            Anchor.x = StartX;
            Anchor.y -= LineDistance;
            ActiveTabSplit = -1.0f;
        }

        int ParseIntParam(T const *&plinepos, bool colorshort = false) {
            if (colorshort) {
                if (*plinepos == 'r' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1]
                                         == '\0')) {
                    ++plinepos;
                    return DefaultColor.R;
                }
                if (*plinepos == 'g' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1]
                                         == '\0')) {
                    ++plinepos;
                    return DefaultColor.G;
                }
                if (*plinepos == 'b' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1]
                                         == '\0')) {
                    ++plinepos;
                    return DefaultColor.B;
                }
                if (*plinepos == 'a' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1]
                                         == '\0')) {
                    ++plinepos;
                    return DefaultColor.A;
                }
            }
            int r;
            T* cont;
            if constexpr (std::same_as<T, wchar_t>) {
                r = std::wcstol(plinepos, &cont, 10);
            } else {
                r = std::strtol(plinepos, &cont, 10);
            }
            plinepos = cont;
            return r;
        }

        float ParseFloatParam(T const *&plinepos) {
            float r;
            T* cont;
            if constexpr (std::same_as<T, wchar_t>) {
                r = std::wcstof(plinepos, &cont);
            } else {
                r = std::strtof(plinepos, &cont);
            }
            plinepos = cont;
            return r;
        }

        static void ParseMaterialNameParam(T const *&plinepos, EGUIX::CMaterial &m) {
            char name[500]{};
            int off = 0;
            while (off < 500) {
                if (*plinepos == ',' || *plinepos == ' ' || *plinepos == '|' || *plinepos == '\0') {
                    break;
                }
                name[off] = static_cast<char>(*plinepos);
                ++plinepos;
                ++off;
            }
            name[off] = '\0';
            m.SetTexture(name);
        }

        static void SkipToEndOfAt(T const *&plinepos) {
            while (true) {
                if (*plinepos == '|') {
                    ++plinepos;
                    break;
                }
                if (*plinepos == ' ') {
                    break;
                }
                if (*plinepos == '\0')
                    break;
                ++plinepos;
            }
        }

        void RenderAtLine() {
            static T partialline[5001]{};
            T const *plinepos = Line;
            T *partlinepos = partialline;
            while (*plinepos) {
                while (true) {
                    if (!*plinepos) {
                        *partlinepos = '\0';
                        break;
                    }
                    if (*plinepos == '@') {
                        if (plinepos[1] == '@') {
                            *partlinepos = '@';
                            ++partlinepos;
                            plinepos += 2;
                            continue;
                        } else if (CharsMatch(&plinepos[1], txt_color(), false)) {
                            shok::Color c{};
                            plinepos += 7;
                            bool colorvalid = false;
                            c.R = static_cast<byte>(ParseIntParam(plinepos, true));
                            if (*plinepos == ',') {
                                ++plinepos;
                                c.G = static_cast<byte>(ParseIntParam(plinepos, true));
                                if (*plinepos == ',') {
                                    ++plinepos;
                                    c.B = static_cast<byte>(ParseIntParam(plinepos, true));
                                    if (*plinepos == ',') {
                                        ++plinepos;
                                        c.A = static_cast<byte>(ParseIntParam(plinepos, true));
                                    } else {
                                        c.A = 255;
                                    }
                                    colorvalid = true;
                                    if (*plinepos != '\0' && *plinepos != ' ')
                                        ++plinepos;
                                }
                            }
                            if (colorvalid) {
                                if (partlinepos != partialline) {
                                    *partlinepos = '\0';
                                    Font->RenderText(partialline, FontSize, &Anchor, Renderer->Brush);
                                    partlinepos = partialline;
                                }
                                Renderer->SetTextRenderColor(c);
                            } else {
                                SkipToEndOfAt(plinepos);
                            }
                            continue;
                        } else if (CharsMatch(&plinepos[1], txt_customcolor(), false)) {
                            plinepos += 13;
                            if (CustomColorData) {
                                int i = ParseIntParam(plinepos);
                                if (i) {
                                    const EGUIX::Color *c = CustomColorData->GetColorByInt(i);
                                    if (c) {
                                        *partlinepos = '\0';
                                        Font->RenderText(partialline, FontSize, &Anchor, Renderer->Brush);
                                        partlinepos = partialline;
                                        Renderer->SetTextRenderColor(c->ToShokColor());
                                    }
                                }
                                SkipToEndOfAt(plinepos);
                            }
                            continue;
                        } else if (CharsMatch(&plinepos[1], txt_defaultcolor(), true)) {
                            plinepos += 13;
                            if (*plinepos == '|')
                                ++plinepos;
                            *partlinepos = '\0';
                            Font->RenderText(partialline, FontSize, &Anchor, Renderer->Brush);
                            partlinepos = partialline;
                            Renderer->SetTextRenderColor(DefaultColor);
                            continue;
                        } else if (CharsMatch(&plinepos[1], txt_icon(), false)) {
                            plinepos += 5;
                            if (*plinepos == ':') {
                                ++plinepos;
                                EGUIX::CMaterial mat{};
                                ParseMaterialNameParam(plinepos, mat);
                                bool coordvalid = false;
                                if (*plinepos == ',') {
                                    ++plinepos;
                                    mat.TextureCoordinates.X = ParseFloatParam(plinepos);
                                    if (*plinepos == ',') {
                                        ++plinepos;
                                        mat.TextureCoordinates.Y = ParseFloatParam(plinepos);
                                        if (*plinepos == ',') {
                                            ++plinepos;
                                            mat.TextureCoordinates.W = ParseFloatParam(plinepos);
                                            if (*plinepos == ',') {
                                                ++plinepos;
                                                mat.TextureCoordinates.H = ParseFloatParam(plinepos);
                                                coordvalid = true;
                                            }
                                        }
                                    }
                                }
                                if (!coordvalid) {
                                    mat.TextureCoordinates = {0.0f, 0.0f, 1.0f, 1.0f};
                                }
                                shok::Position siz = mat.GetSize();
                                siz.X *= mat.TextureCoordinates.W;
                                siz.Y *= mat.TextureCoordinates.H;

                                bool colorvalid = false;
                                shok::Position smult = {1.0f, 1.0f};
                                if (*plinepos == ',') {
                                    ++plinepos;
                                    smult.X = ParseFloatParam(plinepos);
                                    if (*plinepos == ',') {
                                        ++plinepos;
                                        smult.Y = ParseFloatParam(plinepos);
                                        colorvalid = true;
                                    } else {
                                        smult.Y = smult.X;
                                    }
                                }

                                if (colorvalid && *plinepos == ',') {
                                    ++plinepos;
                                    colorvalid = false;
                                    mat.Color.Red = ParseIntParam(plinepos, true);
                                    if (*plinepos == ',') {
                                        ++plinepos;
                                        mat.Color.Green = ParseIntParam(plinepos, true);
                                        if (*plinepos == ',') {
                                            ++plinepos;
                                            mat.Color.Blue = ParseIntParam(plinepos, true);
                                            if (*plinepos == ',') {
                                                ++plinepos;
                                                mat.Color.Alpha = ParseIntParam(plinepos, true);
                                            } else {
                                                mat.Color.Alpha = 255;
                                            }
                                            colorvalid = true;
                                        }
                                    }
                                    if (!colorvalid) {
                                        mat.Color = EGUIX::Color{};
                                    }
                                }

                                *partlinepos = '\0';
                                Font->RenderText(partialline, FontSize, &Anchor, Renderer->Brush);
                                partlinepos = partialline;

                                float sf = FontSize * 0.75f;
                                // recover scaled screen coords where to put the icon
                                EGUIX::Rect pos{
                                    Anchor.x / PosTransform.x,
                                    (Anchor.y - 1.0f) * -shok::UIRenderer::ScaledScreenSize.Y - sf *
                                    shok::UIRenderer::ScaledScreenSize.Y,
                                    siz.X, siz.Y
                                };
                                // scale icon to match font size, keeping proportions
                                pos.W = pos.W / pos.H * sf / PosTransform.x * smult.X;
                                pos.H = sf * shok::UIRenderer::ScaledScreenSize.Y * smult.Y;
                                // adjust height
                                pos.Y += sf * shok::UIRenderer::ScaledScreenSize.Y * (1.0f - smult.Y);
                                Renderer->RenderMaterial(&mat, true, &pos);
                                // adjust position for next text
                                Anchor.x += pos.W * PosTransform.x;
                            }
                            SkipToEndOfAt(plinepos);
                            continue;
                        } else {
                            SkipToEndOfAt(plinepos);
                            continue;
                        }
                    }
                    *partlinepos = *plinepos;
                    ++partlinepos;
                    ++plinepos;
                }
                Font->RenderText(partialline, FontSize, &Anchor, Renderer->Brush);
                partlinepos = partialline;
            }
        }

    public:
        void MainRender() {
            while (*StrPos) {
                PrepareLine();
                float tabSplit = -1.0f;
                if (Alignment == TextAlignment::Right) {
                    float linelen = Font->GetStringWidth(Line, FontSize);
                    Anchor.x = End - linelen;
                } else if (Alignment == TextAlignment::Center) {
                    float linelen = Font->GetStringWidth(Line, FontSize);
                    if (End)
                        Anchor.x = (End + Anchor.x) / 2 - linelen / 2;
                    else
                        Anchor.x = Anchor.x - linelen / 2;
                } else if (ActiveTabSplit >= 0.0f) {
                    tabSplit = ActiveTabSplit + StartX;
                }
                if (ContainsAt)
                    RenderAtLine();
                else
                    Font->RenderText(Line, FontSize, &Anchor, Renderer->Brush);

                if (tabSplit >= 0.0f) {
                    ActiveTabSplit = -1.0f;
                    Anchor.x = std::max(Anchor.x, tabSplit);
                } else
                    NextLine();
                LinePos = Line;
            }
        }
    };
}

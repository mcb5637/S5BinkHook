#include "s5_ui.h"
#include <shok/globals/s5_mapdisplay.h>
#include <shok/ui/s5_guistates.h>
#include <shok/s5_defines.h>
#include <shok/ui/s5_widget.h>
#include <shok/engine/s5_RWE_2d.h>
#include <utility/entityiterator.h>
#include <utility/hooks.h>
#include <cstdlib>
#include <shok/entity/s5_tasklist.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/events/s5_netevents.h>

static inline void(__thiscall* const textset_load)(BB::CTextSet* th) = reinterpret_cast<void(__thiscall*)(BB::CTextSet*)>(0x723647);
void BB::CTextSet::Load()
{
	textset_load(this);
}
static inline void(__thiscall* const textset_merge)(BB::CTextSet* th, BB::CTextSet* o) = reinterpret_cast<void(__thiscall*)(BB::CTextSet*, BB::CTextSet*)>(0x723647);
void BB::CTextSet::Merge(CTextSet* other)
{
	textset_merge(this, other);
}

const char* (*BB::StringTableText::GetStringTableTextOverride)(const char* s) = nullptr;
const char* __stdcall BB::StringTableText::GetSTTOverride(const char* s)
{
	if (!BB::StringTableText::GetStringTableTextOverride)
		return nullptr;
	return BB::StringTableText::GetStringTableTextOverride(s);
}

void NAKED_DEF BB::StringTableText::GetSTTOverrideASM() {
	__asm {
		//int 3
		push[esp + 4];
		call BB::StringTableText::GetSTTOverride;
		test eax, eax;
		jz notfound;
		ret;

	notfound:
		mov eax, 0x894508;
		mov eax, [eax];
		push 0x556D33;
		ret;
	}
}
bool HookGetStringTableText_Hooked = false;
void BB::StringTableText::HookGetStringTableText()
{
	if (HookGetStringTableText_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		throw std::logic_error("HasSCELoader");
	HookGetStringTableText_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ BB::StringTableText::GetStringTableText , 0x20 };
	CppLogic::Hooks::WriteJump(BB::StringTableText::GetStringTableText, &GetSTTOverrideASM, reinterpret_cast<void*>(0x556D33));
}

static inline void(__thiscall* const uirender_rtext)(shok::UIRenderer* r, const char* txt, shok::FontId font, bool scale, float x, float y, float xend, const EGUIX::Color* color, shok::UIRenderCustomColorContext* ccc, float ldf) =
reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const char*, shok::FontId, bool, float, float, float, const EGUIX::Color*, shok::UIRenderCustomColorContext*, float)>(0x5577E1);
void shok::UIRenderer::RenderText(const char* txt, shok::FontId fontid, bool scale, float x, float y, float xend, const EGUIX::Color* color, float linedistancefactor)
{
	uirender_rtext(this, txt, fontid, scale, x, y, xend, color, nullptr, linedistancefactor);
}
void shok::UIRenderer::SetTextRenderColor(shok::Color c) const
{
	RWE::RwRGBA co{ c.R, c.G, c.B, c.A };
	Brush->SetRGBA(&co, &co, &co, &co);
}
static inline void(__thiscall* const uirender_rmaterial)(shok::UIRenderer* r, const EGUIX::CMaterial* m, bool s, const EGUIX::Rect* p) = reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const EGUIX::CMaterial*, bool, const EGUIX::Rect*)>(0x557167);
void shok::UIRenderer::RenderMaterial(const EGUIX::CMaterial* mat, bool scale, const EGUIX::Rect* pos)
{
	uirender_rmaterial(this, mat, scale, pos);
}
inline void(__thiscall* const uirender_rline)(shok::UIRenderer* r, const EGUIX::Color* c, bool scale, float x1, float y1, float x2, float y2) = reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const EGUIX::Color*, bool, float, float, float, float)>(0x557576);
void shok::UIRenderer::RenderLine(const EGUIX::Color* c, bool scale, float x1, float y1, float x2, float y2)
{
	uirender_rline(this, c, scale, x1, y1, x2, y2);
}
inline float(__thiscall* const uirender_textwidth)(shok::UIRenderer* r, const char* t, shok::FontId f) = reinterpret_cast<float(__thiscall*)(shok::UIRenderer*, const char*, shok::FontId)>(0x556FFF);
float shok::UIRenderer::GetTextWidth(const char* text, shok::FontId font)
{
	return uirender_textwidth(this, text, font);
}
float shok::UIRenderer::GetTextHeight(shok::FontId font)
{
	return EGUIX::FontManager::GlobalObj()->GetFontObj(font)->GetHeight();
}

static inline const EGUIX::Color* (__thiscall* const uircustomcolorcont_getcol)(shok::UIRenderCustomColorContext* th, int i) = reinterpret_cast<const EGUIX::Color * (__thiscall*)(shok::UIRenderCustomColorContext*, int)>(0x5577B6);
const EGUIX::Color* shok::UIRenderCustomColorContext::GetColorByInt(int i)
{
	return uircustomcolorcont_getcol(this, i);
}

float __cdecl printstr_getlength_override(RWE::P2D::Rt2dFont* f, const char* str, float multiply) {
	int charlen;
	unsigned char mask;
	int off;
	float* ld = f->GetLengthData();
	bool addlen = true;
	if (f->Flags & 2) {
		charlen = 2;
		mask = 0xFF;
		off = 1;
	}
	else {
		charlen = 1;
		mask = 0;
		off = 0;
	}
	float len = 0;
	bool lastat = false;
	unsigned int c = static_cast<unsigned int>(static_cast<unsigned char>(str[0])) | (static_cast<unsigned int>(static_cast<unsigned char>(mask & static_cast<unsigned char>(str[off]))) << 8);
	while (c > 0) {
		bool skipthis = false;
		if (addlen && c == '@') {
			addlen = false;
			lastat = true;

			str += charlen;
			c = static_cast<unsigned int>(static_cast<unsigned char>(str[0])) | (static_cast<unsigned int>(static_cast<unsigned char>(mask & static_cast<unsigned char>(str[off]))) << 8);
			continue;
		}
		else if (!addlen && c == '@' && lastat) { // NOLINT(*-branch-clone)
			addlen = true;
		}
		else if (!addlen && c == ' ') {
			addlen = true;
		}
		else if (!addlen && c == '|') {
			addlen = true;
			skipthis = true;
		}
		if (addlen && !skipthis) {
			int d;
			if (c < 128) {
				d = f->CharData[c];
			}
			else {
				c = c - f->ExtCharOffset;
				if (c < 0 || c > f->ExtCharLength) {
					d = -1;
				}
				else {
					d = f->ExtCharData[c];
				}
			}
			if (d >= 0) {
				len += ld[8 * d] + f->Spacing;
			}
		}

		lastat = false;
		str += charlen;
		c = static_cast<unsigned int>(static_cast<unsigned char>(str[0])) | (static_cast<unsigned int>(static_cast<unsigned char>(mask & static_cast<unsigned char>(str[off]))) << 8);
	}
	return len * multiply;
}
template<class T>
class TextRenderer {
	enum class TextAlignment : int {
		Left, Center, Right,
	};
	shok::UIRenderer* r;
	const T* buff;
	RWE::P2D::Rt2dFont* f;
	RWE::RwV2d anchor;
	RWE::RwV2d posTransform;
	float end;
	const float startx;
	const float fontsize;
	const float linedistance;
	bool containsat = false;
	TextAlignment ali = TextAlignment::Left;
	const T* strpos;
	T* linepos;
	shok::UIRenderCustomColorContext* customcolordata;
	const shok::Color defaultcolor;
	bool skipWhitespace = true;

	static inline T line[5001]{};
public:
	TextRenderer(shok::UIRenderer* r, const T* txt, RWE::P2D::Rt2dFont* f, const RWE::RwV2d& anchor, float end, float ldf, const RWE::RwV2d& posTransform, shok::UIRenderCustomColorContext* customcolordata,
		shok::Color defaultcolor)
		: startx(anchor.x), fontsize(f->GetHeight()), linedistance(fontsize* (ldf == 0.0f ? 1.0f : ldf))
		, defaultcolor(defaultcolor) {
		this->r = r;
		this->buff = txt;
		this->f = f;
		this->anchor = anchor;
		this->end = end;
		this->posTransform = posTransform;
		strpos = buff;
		linepos = line;
		this->customcolordata = customcolordata;
	}

private:
	constexpr const T* txt_cr() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"cr";
		else
			return "cr";
	}
	constexpr const T* txt_ra() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"ra";
		else
			return "ra";
	}
	constexpr const T* txt_la() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"la";
		else
			return "la";
	}
	constexpr const T* txt_center() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"center";
		else
			return "center";
	}
	constexpr const T* txt_bs() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"bs";
		else
			return "bs";
	}
	constexpr const T* txt_wsoff() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"skipWhitespace:off";
		else
			return "skipWhitespace:off";
	}
	constexpr const T* txt_wson() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"skipWhitespace:on";
		else
			return "skipWhitespace:on";
	}
	constexpr const T* txt_color() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"color:";
		else
			return "color:";
	}
	constexpr const T* txt_customcolor() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"customcolor:";
		else
			return "customcolor:";
	}
	constexpr const T* txt_defaultcolor() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"defaultcolor";
		else
			return "defaultcolor";
	}
	constexpr const T* txt_icon() {
		if constexpr (std::same_as<T, wchar_t>)
			return L"icon";
		else
			return "icon";
	}

	bool charsmatch(const T* txt, const T* sear, bool checkend) {
		while (*sear)
		{
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

	void SkipWhitespace(const T*& txt) {
		while (*txt == ' ') {
			++txt;
		}
	}

	void PrepareLine() {
		T* lastwordpos = linepos;
		const T* lastwordsourcepos = strpos;
		containsat = false;
		if (skipWhitespace)
			SkipWhitespace(strpos);
		while (true) {
			if (!*strpos) {
				*linepos = '\0';
				if (end && linepos != lastwordpos && lastwordpos != line) {
					if ((f->GetStringWidth(line, fontsize) + anchor.x) >= end) {
						linepos = lastwordpos;
						*linepos = '\0';
						strpos = lastwordsourcepos;
						break;
					}
				}
				break;
			}
			if (*strpos == '\r' || *strpos == '\n') {
				++strpos;
				if (*strpos == '\r' || *strpos == '\n')
					++strpos;
				*linepos = '\0';
				break;
			}
			if (*strpos == '@') {
				if (strpos[1] == '@') {
					linepos[0] = '@';
					linepos[1] = '@';
					linepos += 2;
					strpos += 2;
					continue;
				}
				else if (charsmatch(&strpos[1], txt_cr(), true)) {
					strpos += 3;
					if (*strpos == '|')
						++strpos;
					*linepos = '\0';
					break;
				}
				else if (charsmatch(&strpos[1], txt_ra(), true)) {
					ali = TextAlignment::Right;
					strpos += 3;
				}
				else if (charsmatch(&strpos[1], txt_la(), true)) {
					ali = TextAlignment::Left;
					strpos += 3;
				}
				else if (charsmatch(&strpos[1], txt_center(), true)) {
					ali = TextAlignment::Center;
					strpos += 7;
				}
				else if (charsmatch(&strpos[1], txt_bs(), true)) {
					if (linepos == line) {
						strpos += 4;
						continue;
					}
					if (linepos[-1] == ' ')
						--linepos;
					T prev = *linepos;
					*linepos = '\0';
					if (end && (f->GetStringWidth(line, fontsize) + anchor.x) >= end) {
						if (linepos != lastwordpos && lastwordpos != line) {
							linepos = lastwordpos;
							*linepos = '\0';
							strpos = lastwordsourcepos;
						}
						break;
					}
					*linepos = prev;
					strpos += 4;
					lastwordpos = linepos;
					lastwordsourcepos = strpos;
					continue;
				}
				else if (charsmatch(&strpos[1], txt_wsoff(), true)) {
					skipWhitespace = false;
					strpos += 19;
				}
				else if (charsmatch(&strpos[1], txt_wson(), true)) {
					skipWhitespace = true;
					strpos += 18;
				}
				else {
					containsat = true;
					while (true) {
						*linepos = *strpos;
						++linepos;
						++strpos;
						if (*strpos == '\0')
							break;
						if (*strpos == '|') {
							*linepos = '|';
							++linepos;
							++strpos;
							if (skipWhitespace) {
								SkipWhitespace(strpos);
							}
							break;
						}
						if (*strpos == ' ') {
							*linepos = '|';
							++linepos;
							if (skipWhitespace) {
								++strpos;
								SkipWhitespace(strpos);
							}
							break;
						}
					}
					continue;
				}
				if (*strpos == '|')
					++strpos;
				if (skipWhitespace)
					SkipWhitespace(strpos);
				continue;
			}
			*linepos = *strpos;
			++strpos;
			if (*linepos == ' ' && end && linepos != line) {
				*linepos = '\0';
				if ((f->GetStringWidth(line, fontsize) + anchor.x) >= end) {
					if (linepos != lastwordpos && lastwordpos != line) {
						linepos = lastwordpos;
						*linepos = '\0';
						strpos = lastwordsourcepos;
					}
					break;
				}
				*linepos = ' ';
				lastwordpos = linepos;
				lastwordsourcepos = strpos;
				if (skipWhitespace)
					SkipWhitespace(strpos);
			}
			++linepos;
		}
	}

	void NextLine() {
		anchor.x = startx;
		anchor.y -= linedistance;
	}

	int ParseIntParam(T*& plinepos, bool colorshort = false) {
		if (colorshort) {
			if (*plinepos == 'r' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1] == '\0')) {
				++plinepos;
				return defaultcolor.R;
			}
			if (*plinepos == 'g' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1] == '\0')) {
				++plinepos;
				return defaultcolor.G;
			}
			if (*plinepos == 'b' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1] == '\0')) {
				++plinepos;
				return defaultcolor.B;
			}
			if (*plinepos == 'a' && (plinepos[1] == ',' || plinepos[1] == ' ' || plinepos[1] == '|' || plinepos[1] == '\0')) {
				++plinepos;
				return defaultcolor.A;
			}
		}
		T* st = plinepos;
		while (true) {
			if (*plinepos == ',' || *plinepos == ' ' || *plinepos == '|' || *plinepos == '\0') {
				T prev = *plinepos;
				*plinepos = '\0';
				int r;
				if constexpr (std::same_as<T, wchar_t>) {
					r = _wtoi(st); // todo, fix for char if that is actually used somewhere
				}
				else {
					r = atoi(st);
				}
				*plinepos = prev;
				return r;
			}
			++plinepos;
		}
	}
	float ParseFloatParam(T*& plinepos) {
		T* st = plinepos;
		while (true) {
			if (*plinepos == ',' || *plinepos == ' ' || *plinepos == '|' || *plinepos == '\0') {
				T prev = *plinepos;
				*plinepos = '\0';
				double r;
				if constexpr (std::same_as<T, wchar_t>) {
					r = _wtof(st); // todo, fix for char if that is actually used somewhere
				}
				else {
					r = atof(st);
				}
				*plinepos = prev;
				return static_cast<float>(r);
			}
			++plinepos;
		}
	}
	void ParseMaterialNameParam(T*& plinepos, EGUIX::CMaterial& m) {
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

	void SkipToEndOfAt(T*& plinepos) {
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
		T* plinepos = line;
		T* partlinepos = partialline;
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
					}
					else if (charsmatch(&plinepos[1], txt_color(), false)) {
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
								}
								else {
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
								f->RenderText(partialline, fontsize, &anchor, r->Brush);
								partlinepos = partialline;
							}
							r->SetTextRenderColor(c);
						}
						else {
							SkipToEndOfAt(plinepos);
						}
						continue;
					}
					else if (charsmatch(&plinepos[1], txt_customcolor(), false)) {
						plinepos += 13;
						if (customcolordata) {
							int i = ParseIntParam(plinepos);
							if (i) {
								const EGUIX::Color* c = customcolordata->GetColorByInt(i);
								if (c) {
									*partlinepos = '\0';
									f->RenderText(partialline, fontsize, &anchor, r->Brush);
									partlinepos = partialline;
									r->SetTextRenderColor(c->ToShokColor());
								}
							}
							SkipToEndOfAt(plinepos);
						}
						continue;
					}
					else if (charsmatch(&plinepos[1], txt_defaultcolor(), true)) {
						plinepos += 13;
						if (*plinepos == '|')
							++plinepos;
						*partlinepos = '\0';
						f->RenderText(partialline, fontsize, &anchor, r->Brush);
						partlinepos = partialline;
						r->SetTextRenderColor(defaultcolor);
						continue;
					}
					else if (charsmatch(&plinepos[1], txt_icon(), false)) {
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
								mat.TextureCoordinates = { 0.0f, 0.0f, 1.0f, 1.0f };
							}
							shok::Position siz = mat.GetSize();
							siz.X *= mat.TextureCoordinates.W;
							siz.Y *= mat.TextureCoordinates.H;

							bool colorvalid = false;
							shok::Position smult = { 1.0f, 1.0f };
							if (*plinepos == ',') {
								++plinepos;
								smult.X = ParseFloatParam(plinepos);
								if (*plinepos == ',') {
									++plinepos;
									smult.Y = ParseFloatParam(plinepos);
									colorvalid = true;
								}
								else {
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
										}
										else {
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
							f->RenderText(partialline, fontsize, &anchor, r->Brush);
							partlinepos = partialline;

							float sf = fontsize * 0.75f;
							// recover scaled screen coords where to put the icon
							EGUIX::Rect pos{ anchor.x / posTransform.x, (anchor.y - 1.0f) * -shok::UIRenderer::ScaledScreenSize.Y - sf * shok::UIRenderer::ScaledScreenSize.Y, siz.X, siz.Y };
							// scale icon to match font size, keeping proportions
							pos.W = pos.W / pos.H * sf / posTransform.x * smult.X;
							pos.H = sf * shok::UIRenderer::ScaledScreenSize.Y * smult.Y;
							// adjust height
							pos.Y += sf * shok::UIRenderer::ScaledScreenSize.Y * (1.0f - smult.Y);
							r->RenderMaterial(&mat, true, &pos);
							// adjust position for next text
							anchor.x += pos.W * posTransform.x;
						}
						SkipToEndOfAt(plinepos);
						continue;
					}
					else {
						SkipToEndOfAt(plinepos);
						continue;
					}
				}
				*partlinepos = *plinepos;
				++partlinepos;
				++plinepos;
			}
			f->RenderText(partialline, fontsize, &anchor, r->Brush);
			partlinepos = partialline;
		}
	}

public:
	void MainRender() {
		while (*strpos) {
			PrepareLine();
			if (ali == TextAlignment::Right) {
				float linelen = f->GetStringWidth(line, fontsize);
				anchor.x = end - linelen;
			}
			else if (ali == TextAlignment::Center) {
				float linelen = f->GetStringWidth(line, fontsize);
				if (end)
					anchor.x = (end + anchor.x) / 2 - linelen / 2;
				else
					anchor.x = anchor.x - linelen / 2;
			}
			if (containsat)
				RenderAtLine();
			else
				f->RenderText(line, fontsize, &anchor, r->Brush);

			NextLine();
			linepos = line;
		}
	}
};

int __fastcall printstr_override(shok::UIRenderer* r, int _, const char* txt, shok::FontId font, bool uk, float x, float y, float xend, const EGUIX::Color* color, shok::UIRenderCustomColorContext* customcolordata, float ldf) {
	if (!r->ShouldRenderText)
		return 0;
	if (!txt || *txt == 0)
		return 1;
	auto* f = EGUIX::FontManager::GlobalObj()->GetFontObj(font);
	if (!f)
		return 0;
	// no idea what these funcs do
	RWE::P2D::CTM::Push();
	RWE::P2D::CTM::SetIdentity();
	r->Brush->SetTexture(nullptr);

	const shok::Color c = color ? color->ToShokColor() : shok::Color{};
	r->SetTextRenderColor(c);
	RWE::RwV2d anchor = { x,y };
	RWE::RwV2d posTransform = { 1.0f, 1.0f };
	float end = xend;
	if (!uk) {
		anchor.x = anchor.x * shok::UIRenderer::ScaledScreenSize.X / r->RenderSizeX;
		anchor.y = anchor.y * shok::UIRenderer::ScaledScreenSize.Y / r->RenderSizeY;
		end = end * shok::UIRenderer::ScaledScreenSize.X / r->RenderSizeX;
	}

	const float fontsize = f->GetHeight(); //0.036
	//const float linedistance = fontsize * (ldf == 0.0f ? 1.0f : ldf);

	RWE::RwV2d stepx;
	RWE::RwV2d stepy;
	RWE::RwV2d stepori;
	RWE::P2D::Device::GetStep(&stepx, &stepy, &stepori);
	anchor.x = anchor.x * stepx.x * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	posTransform.x = posTransform.x * stepx.x * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	end = end * stepx.x * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	if (!r->SomeTextBool) {
		RWE::P2D::CTM::Scale(shok::UIRenderer::ScaledScreenSize.X / r->RenderSizeX, shok::UIRenderer::ScaledScreenSize.Y / r->RenderSizeY);
		anchor.y = r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y - (fontsize * shok::UIRenderer::ScaledScreenSize.Y + anchor.y) / shok::UIRenderer::ScaledScreenSize.Y;
		posTransform.y = posTransform.y * r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y / shok::UIRenderer::ScaledScreenSize.Y;
	}
	else {
		anchor.y = 1.0f - (fontsize * shok::UIRenderer::ScaledScreenSize.Y + anchor.y) / shok::UIRenderer::ScaledScreenSize.Y;
		posTransform.y = posTransform.y * r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y / shok::UIRenderer::ScaledScreenSize.Y;
	}

	(*RWE::RwGlobals::GlobalObj)->dOpenDevice.SetTextureRaster(nullptr);
	// ReSharper disable once CppExpressionWithoutSideEffects
	(*RWE::RwGlobals::GlobalObj)->dOpenDevice.SetTextureFilterMode(RWE::RwTextureFilterMode::rwFILTERLINEAR);

	// i dont think this was a template when BB did this, but this is way nicer as one
	if (f->IsWchar()) {
		static wchar_t buff[5001]{}; // same size that is used in the original func
		shok::UIRenderer::MultibyteToWString(txt, buff, 5000);
		TextRenderer<wchar_t> rend{ r, buff, f, anchor, end, ldf, posTransform, customcolordata, c };
		rend.MainRender();
	}
	else {
		TextRenderer<char> rend{ r, txt, f, anchor, end, ldf, posTransform, customcolordata, c };
		rend.MainRender();
	}

	RWE::P2D::CTM::Pop();

	return 1;
}
bool HookTextPrinting_Hooked = false; // 1103
void shok::HookTextPrinting()
{
	if (HookTextPrinting_Hooked)
		return;
	HookTextPrinting_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {reinterpret_cast<void*>(0x5577E1),
		reinterpret_cast<void*>(0x708F00),
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5577E1), &printstr_override, reinterpret_cast<void*>(0x5577EB));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x708F00), &printstr_getlength_override, reinterpret_cast<void*>(0x708F0E));
}


void ERwTools::CRpClumpRenderable::SetModelData(const ED::ModelData* modeldata, float rotation)
{
	SetModelData(modeldata ? modeldata->Clump : nullptr, rotation);
}
void ERwTools::CRpClumpRenderable::SetPosition(const shok::Position& p, float z)
{
	SetPosition(p.X, p.Y, z);
}

void ERwTools::CRpClumpRenderable::SetBuildingRedColor(bool r)
{
	SetBuildingRedColorI(r ? 2 : 1);
}

static inline void(__thiscall* const c3dviewhandler_setguistate)(GGUI::C3DViewHandler* th, shok::ClassId id, const GGUI::SStateParameters* p) = reinterpret_cast<void(__thiscall*)(GGUI::C3DViewHandler*, shok::ClassId, const GGUI::SStateParameters*)>(0x52820C);
void GGUI::C3DViewHandler::SetGUIStateByIdentifier(shok::ClassId identifier, const GGUI::SStateParameters* p)
{
	c3dviewhandler_setguistate(this, identifier, p);
}
static inline void(__thiscall* const c3dviewhandler_setguistateonupdate)(GGUI::C3DViewHandler* th, shok::ClassId id) = reinterpret_cast<void(__thiscall*)(GGUI::C3DViewHandler*, shok::ClassId)>(0x5280DE);
void GGUI::C3DViewHandler::SetGUIStateByIdentfierOnNextUpdate(shok::ClassId identifier)
{
	c3dviewhandler_setguistateonupdate(this, identifier);
}

void ERwTools::CDefCameraBehaviour::HookEnableZoom(bool ena)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x52212B), 10 };
	*reinterpret_cast<byte*>(0x52212B) = ena ? 0x75 : 0xEB; // jnz : jmp
}

bool GGL::CGLGUIInterface::GetNearestFreePosForBuildingPlacement(shok::EntityTypeId ety, const shok::Position& inp, shok::Position& outp)
{
	return GetNearestFreePosForBuildingPlacement(ety, inp.X, inp.Y, &outp.X, &outp.Y, -1);
}


void NAKED_DEF GGL::CGLGUIInterface::GetMaxHPOverrideASM() {
	__asm {
		mov ecx, [ebp + 0xC];
		call EGL::CGLEEntity::GetMaxHPOverrideStatic;
		push eax;
		fild dword ptr [esp];
		pop eax;
		push 0x4BDEE0;
		ret;
	}
}
void GGL::CGLGUIInterface::HookFillDataHealth()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x16, {
		reinterpret_cast<void*>(0x4BDED8)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4BDED8), &GetMaxHPOverrideASM, reinterpret_cast<void*>(0x4BDEE0));
}

static inline void(__thiscall* const guimng_setpl)(GGUI::CManager* th, shok::PlayerId p) = reinterpret_cast<void(__thiscall*)(GGUI::CManager*, shok::PlayerId)>(0x52371C);
void GGUI::CManager::SetControlledPlayer(shok::PlayerId pl)
{
	guimng_setpl(this, pl);
}

inline bool(__thiscall* const guimng_sel_isselected)(const shok::Vector<GGUI::CManager::SelectionData>* th, shok::EntityId id) = reinterpret_cast<bool(__thiscall*)(const shok::Vector<GGUI::CManager::SelectionData>*, shok::EntityId)>(0x721E7B);
bool GGUI::CManager::IsEntitySelected(shok::EntityId id) const
{
	return guimng_sel_isselected(&SelectedEntities, id);
}
inline bool(__thiscall* const guimng_selectent)(GGUI::CManager* th, shok::EntityId id) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*, shok::EntityId)>(0x525828);
bool GGUI::CManager::SelectEntity(shok::EntityId id)
{
	return guimng_selectent(this, id);
}
inline void(__thiscall* const guimng_selectionchanged)(GGUI::CManager* th, int uk) = reinterpret_cast<void(__thiscall*)(GGUI::CManager*, int)>(0x5235BC);
void GGUI::CManager::OnSelectionChanged()
{
	guimng_selectionchanged(this, 0); // TODO check what that param is, only call with 1 is from event handler of FEEDBACK_EVENT_ENTITIES_EXCHANGED via 0x5235D4
}
inline bool(__thiscall* const guimng_deselect)(GGUI::CManager* th, shok::EntityId id) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*, shok::EntityId)>(0x524A98);
bool GGUI::CManager::DeselectEntity(shok::EntityId id)
{
	return guimng_deselect(this, id);
}
inline bool(__thiscall* const guimng_clearsel)(GGUI::CManager* th) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*)>(0x523590);
bool GGUI::CManager::ClearSelection()
{
	return guimng_clearsel(this);
}
inline bool(__thiscall* const guimng_isstatevalid)(GGUI::CManager* th, GGUI::CManager::StateIdData* s, shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata) = reinterpret_cast<bool(__thiscall*)(GGUI::CManager*, GGUI::CManager::StateIdData*, shok::EntityId, GGUI::CBasicState::TargetData*, GGUI::CBasicState::ExecuteData*)>(0x5237A2);
bool GGUI::CManager::IsCommandStateValid(StateIdData* s, shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata)
{
	return guimng_isstatevalid(this, s, entity, tdata, edata);
}
inline GGUI::CBasicState* (__thiscall* const guimng_getcmdstate)(GGUI::CManager* th, shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata) = reinterpret_cast<GGUI::CBasicState * (__thiscall*)(GGUI::CManager*, shok::EntityId, GGUI::CBasicState::TargetData*, GGUI::CBasicState::ExecuteData*)>(0x523FEC);
GGUI::CBasicState* GGUI::CManager::GetCommandStateFor(shok::EntityId entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata)
{
	return guimng_getcmdstate(this, entity, tdata, edata);
}
inline shok::EntityId(__thiscall* const guimng_lastselnonsol)(const shok::Vector<GGUI::CManager::SelectionData>* th) = reinterpret_cast<shok::EntityId(__thiscall*)(const shok::Vector<GGUI::CManager::SelectionData>*)>(0x721D9C);
shok::EntityId GGUI::CManager::GetLastSelectedNonSoldier() const
{
	return guimng_lastselnonsol(&SelectedEntities);
}

bool GGUI::CManager::CanSelectPlayer(shok::PlayerId p) const {
	auto* f = reinterpret_cast<bool(__thiscall*)(const GGUI::CManager*, shok::PlayerId)>(0x52376b);
	return f(this, p);
}

bool(*GGUI::CManager::PostEventCallback)(BB::CEvent* ev) = nullptr;
bool(*GGUI::CManager::PostEventCallback2)(BB::CEvent* ev) = nullptr;
// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
struct PostEventHook_GUIManager : public BB::IPostEvent {
	BB::IPostEvent* Base = nullptr;

	virtual void __stdcall PostEvent(BB::CEvent* ev) override {
		if (GGUI::CManager::PostEventCallback)
			if (GGUI::CManager::PostEventCallback(ev))
				return;
		if (GGUI::CManager::PostEventCallback2)
			if (GGUI::CManager::PostEventCallback2(ev))
				return;
		if (Base)
			Base->PostEvent(ev);
	}
};
PostEventHook_GUIManager PostEventHook_GUIManagerObj{};
void GGUI::CManager::HackPostEvent()
{
	if (PostGUIEvent == &PostEventHook_GUIManagerObj)
		return;
	PostEventHook_GUIManagerObj.Base = PostGUIEvent;
	PostGUIEvent = &PostEventHook_GUIManagerObj;
}

void GGUI::CManager::DisableSelectionLimit(bool disable)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x525959), 4 };
	*reinterpret_cast<byte*>(0x525959) = disable ? 0xEB : 0x7E; // jmp : jle
}

bool GGUI::CManager::IsModifierPressed(shok::Keys modif)
{
	return EGUIX::CEventManager::GlobalObj()->IsModifierPressed(modif);
}

bool CManager_HookExtraPlayers = false;
void GGUI::CManager::HookExtraPlayers() {

	if (CManager_HookExtraPlayers)
		return;
	CManager_HookExtraPlayers = true;
	CppLogic::Hooks::SaveVirtualProtect vp{0x100, {
		reinterpret_cast<void*>(0x52376b),
		reinterpret_cast<void*>(0x523721),
	}};
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x52376b), CppLogic::Hooks::MemberFuncPointerToVoid(&CManager::CanSelectPlayerExtra, 0), reinterpret_cast<void*>(0x523772));
	CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x523721), reinterpret_cast<void*>(0x52372b));
	CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x52372f), reinterpret_cast<void*>(0x52373f));
}

bool GGUI::CManager::CanSelectPlayerExtra(shok::PlayerId p) const {
	return p == ControlledPlayer;
}

void GGUI::SoundFeedback::SoundDatas::ReloadData(shok::FeedbackEventShortenedId id) {
	auto i = SoundData.find(id);
	if (i == SoundData.end())
		throw std::invalid_argument("SoundData not found");
	auto* data = i->second;
	for (auto* v : data->Sound)
		delete v;
	{
		auto v = data->Sound.SaveVector();
		v.Vector.clear();
	}
	auto file = std::format(R"(Data\Config\FeedbackEventSoundData\{}.xml)", CppLogic::GetIdManager<shok::FeedbackEventShortenedId>().GetNameByID(id));
	auto seri = BB::CXmlSerializer::CreateUnique();
	seri->Deserialize(file.c_str(), data, FeedbackEventSoundData::SerializationData);
}

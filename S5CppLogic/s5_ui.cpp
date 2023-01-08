#include "pch.h"
#include "s5_ui.h"
#include "s5_mapdisplay.h"
#include "s5_guistates.h"
#include "s5_defines.h"
#include "s5_widget.h"
#include "s5_RWE_2d.h"
#include "entityiterator.h"
#include "hooks.h"
#include <stdlib.h>

struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(BB::IPostEvent* th, BB::CEvent* ev);
};

struct shok_vtable_GGL_CGLGUIInterface {
	void(__thiscall* dtor)(GGL::CGLGUIInterface* th, bool free);
	PADDINGI(3);
	bool(__thiscall* GetNearestFreeBuildingPos)(GGL::CGLGUIInterface* th, int ety, float inx, float iny, float* outx, float* outy, float range);
};

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
void __declspec(naked) hooksttasm() {
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
		throw 0;
	HookGetStringTableText_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ BB::StringTableText::GetStringTableText , 0x20 };
	CppLogic::Hooks::WriteJump(BB::StringTableText::GetStringTableText, &hooksttasm, reinterpret_cast<void*>(0x556D33));
}

static inline void(__thiscall* const uirender_rtext)(shok::UIRenderer* r, const char* txt, int font, float uk, float x, float y, float xend, const EGUIX::Color* color, shok::UIRenderCustomColorContext* ccc, float ldf) =
reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const char*, int, float, float, float, float, const EGUIX::Color*, shok::UIRenderCustomColorContext*, float)>(0x5577E1);
void shok::UIRenderer::RenderText(const char* txt, int fontid, float x, float y, float xend, const EGUIX::Color* color, float linedistancefactor)
{
	uirender_rtext(this, txt, fontid, 1, x, y, xend, color, nullptr, linedistancefactor);
}
static inline void(__cdecl* const uirender_setcolor)(void* rd, shok::Color* c1, shok::Color* c2, shok::Color* c3, shok::Color* c4) = reinterpret_cast<void(__cdecl*)(void*, shok::Color*, shok::Color*, shok::Color*, shok::Color*)>(0x7077F0);
void shok::UIRenderer::SetTextRenderColor(shok::Color c)
{
	uirender_setcolor(TextRenderObj, &c, &c, &c, &c);
}
static inline void(__thiscall* const uirender_rmaterial)(shok::UIRenderer* r, const EGUIX::CMaterial* m, bool s, const EGUIX::Rect* p) = reinterpret_cast<void(__thiscall*)(shok::UIRenderer*, const EGUIX::CMaterial*, bool, const EGUIX::Rect*)>(0x557167);
void shok::UIRenderer::RenderMaterial(const EGUIX::CMaterial* mat, bool scale, const EGUIX::Rect* pos)
{
	uirender_rmaterial(this, mat, scale, pos);
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
		else if (!addlen && c == '@' && lastat) {
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
			if (*strpos == '\r ' || *strpos == '\n') {
				++strpos;
				if (*strpos == '\r ' || *strpos == '\n')
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
					if (skipWhitespace) {
						while (true) {
							*linepos = *strpos;
							++linepos;
							++strpos;
							if (*strpos == '\0')
								break;
							if (*strpos == '|' || *strpos == ' ') {
								*linepos = '|';
								++linepos;
								++strpos;
								SkipWhitespace(strpos);
								break;
							}
						}
						continue;
					}
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
								f->RenderText(partialline, fontsize, &anchor, r->TextRenderObj);
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
									f->RenderText(partialline, fontsize, &anchor, r->TextRenderObj);
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
						f->RenderText(partialline, fontsize, &anchor, r->TextRenderObj);
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
							f->RenderText(partialline, fontsize, &anchor, r->TextRenderObj);
							partlinepos = partialline;

							// recover scaled screen coords where to put the icon
							EGUIX::Rect pos{ anchor.x / posTransform.x, (anchor.y - 1.0f) * -shok::UIRenderer::ScaledScreenSize.Y - fontsize * shok::UIRenderer::ScaledScreenSize.Y, siz.X, siz.Y };
							// scale icon to match font size, keeping proportions
							pos.W = pos.W / pos.H * fontsize / posTransform.x * smult.X;
							pos.H = fontsize * shok::UIRenderer::ScaledScreenSize.Y * smult.Y;
							// adjust height
							pos.Y += fontsize * shok::UIRenderer::ScaledScreenSize.Y * (1.0f - smult.Y);
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
			f->RenderText(partialline, fontsize, &anchor, r->TextRenderObj);
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
				f->RenderText(line, fontsize, &anchor, r->TextRenderObj);

			NextLine();
			linepos = line;
		}
	}
};

int __fastcall printstr_override(shok::UIRenderer* r, int _, const char* txt, int font, bool uk, float x, float y, float xend, const EGUIX::Color* color, shok::UIRenderCustomColorContext* customcolordata, float ldf) {
	if (!r->ShouldRenderText)
		return 0;
	if (!txt || *txt == 0)
		return 1;
	auto* f = EGUIX::FontManager::GlobalObj()->GetFontObj(font);
	if (!f)
		return 0;
	// no idea what these funcs do
	reinterpret_cast<void(*)()>(0x707170)();
	reinterpret_cast<void(*)()>(0x706D60)();
	reinterpret_cast<void(__cdecl*)(void*, int)>(0x707A00)(r->TextRenderObj, 0);

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

	const float fontsize = f->GetHeight();
	const float linedistance = fontsize * (ldf == 0.0f ? 1.0f : ldf);

	float somesize[2];
	float unknownfa0[2];
	float unknownfa1[2];
	// also no idea what this exactly does
	reinterpret_cast<void(__cdecl*)(float*, float*, float*)>(0x707400)(somesize, unknownfa0, unknownfa1);
	anchor.x = anchor.x * somesize[0] * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	posTransform.x = posTransform.x * somesize[0] * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	end = end * somesize[0] * r->RenderSizeX / shok::UIRenderer::ScaledScreenSize.X;
	if (!r->SomeTextBool) {
		reinterpret_cast<void(__cdecl*)(float, float)>(0x707200)(shok::UIRenderer::ScaledScreenSize.X / r->RenderSizeX, shok::UIRenderer::ScaledScreenSize.Y / r->RenderSizeY);
		anchor.y = r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y - (fontsize * shok::UIRenderer::ScaledScreenSize.Y + anchor.y) / shok::UIRenderer::ScaledScreenSize.Y;
		posTransform.y = posTransform.y * r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y / shok::UIRenderer::ScaledScreenSize.Y;
	}
	else {
		anchor.y = 1.0f - (fontsize * shok::UIRenderer::ScaledScreenSize.Y + anchor.y) / shok::UIRenderer::ScaledScreenSize.Y;
		posTransform.y = posTransform.y * r->RenderSizeY / shok::UIRenderer::ScaledScreenSize.Y / shok::UIRenderer::ScaledScreenSize.Y;
	}
	
	reinterpret_cast<void(__cdecl*)(int, int)>(*reinterpret_cast<int*>(*reinterpret_cast<int*>(0x8501C8) + 32))(1, 0);
	reinterpret_cast<void(__cdecl*)(int, int)>(*reinterpret_cast<int*>(*reinterpret_cast<int*>(0x8501C8) + 32))(9, 2);

	// i dont think this was a template when BB did this, but this is way nicer as one
	if (f->Flags & 2) {
		static wchar_t buff[5001]{}; // same size that is used in the original func
		shok::UIRenderer::MultibyteToWString(txt, buff, 5000);
		TextRenderer<wchar_t> rend{ r, buff, f, anchor, end, ldf, posTransform, customcolordata, c };
		rend.MainRender();
	}
	else {
		TextRenderer<char> rend{ r, txt, f, anchor, end, ldf, posTransform, customcolordata, c };
		rend.MainRender();
	}

	reinterpret_cast<void(__cdecl*)()>(0x7071D0)();

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

static inline void(__thiscall*const c3dviewhandler_setguistate)(GGUI::C3DViewHandler* th, unsigned int id, const GGUI::SStateParameters* p) = reinterpret_cast<void(__thiscall*)(GGUI::C3DViewHandler*, unsigned int, const GGUI::SStateParameters*)>(0x52820C);
void GGUI::C3DViewHandler::SetGUIStateByIdentifier(unsigned int identifier, const GGUI::SStateParameters* p)
{
	c3dviewhandler_setguistate(this, identifier, p);
}
static inline void(__thiscall* const c3dviewhandler_setguistateonupdate)(GGUI::C3DViewHandler* th, unsigned int id) = reinterpret_cast<void(__thiscall*)(GGUI::C3DViewHandler*, unsigned int)>(0x5280DE);
void GGUI::C3DViewHandler::SetGUIStateByIdentfierOnNextUpdate(unsigned int identifier)
{
	c3dviewhandler_setguistateonupdate(this, identifier);
}

bool GGL::CGLGUIInterface::GetNearestFreePosForBuildingPlacement(int ety, const shok::Position& inp, shok::Position& outp)
{
	return GetNearestFreePosForBuildingPlacement(ety, inp.X, inp.Y, &outp.X, &outp.Y, -1);
}

void __declspec(naked) hookgetmaxhpui() {
	__asm {
		mov ecx, [ebp + 0xC];
		call EGL::CGLEEntity::GetMaxHPOverride;
		push eax;
		fild[esp];
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
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4BDED8), &hookgetmaxhpui, reinterpret_cast<void*>(0x4BDEE0));
}

static inline void(__thiscall* const guimng_setpl)(GGUI::CManager* th, int p) = reinterpret_cast<void(__thiscall*)(GGUI::CManager*, int)>(0x52371C);
void GGUI::CManager::SetControlledPlayer(int pl)
{
	guimng_setpl(this, pl);
}

void(__stdcall* PostEventOrig)(BB::IPostEvent* th, BB::CEvent* ev) = nullptr;
shok_vtable_BB_IPostEvent* BB_IPostEvent_vtableHooked = nullptr;
bool(*GGUI::CManager::PostEventCallback)(BB::CEvent* ev) = nullptr;
void __stdcall PostEventHook(BB::IPostEvent* th, BB::CEvent* ev) {
	if (GGUI::CManager::PostEventCallback)
		if (GGUI::CManager::PostEventCallback(ev))
			return;
	PostEventOrig(th, ev);
}
void GGUI::CManager::HackPostEvent()
{
	if (PostEventOrig) {
		CppLogic::Hooks::SaveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
		BB_IPostEvent_vtableHooked->PostEvent = PostEventOrig;
	}
	BB_IPostEvent_vtableHooked = reinterpret_cast<shok_vtable_BB_IPostEvent*>(CppLogic::GetVTable(PostEvent));
	CppLogic::Hooks::SaveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
	PostEventOrig = BB_IPostEvent_vtableHooked->PostEvent;
	BB_IPostEvent_vtableHooked->PostEvent = reinterpret_cast<void(__stdcall*)(BB::IPostEvent * th, BB::CEvent * ev)>(&PostEventHook);
}

static inline int* (* const modifpressed_getobj)() = reinterpret_cast<int* (*)()>(0x558C16);
static inline bool(__thiscall* const modifpressed_ispressed)(int* ob7, shok::Keys m) = reinterpret_cast<bool(__thiscall*)(int*, shok::Keys)>(0x558C1C);
bool GGUI::CManager::IsModifierPressed(shok::Keys modif)
{
	return modifpressed_ispressed(modifpressed_getobj() + 7, modif);
}

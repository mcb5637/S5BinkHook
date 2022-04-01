#include "pch.h"
#include "s5_ui.h"
#include "s5_mapdisplay.h"
#include "s5_guistates.h"
#include "s5_defines.h"
#include "s5_widget.h"
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


const char* (*shok::GetStringTableTextOverride)(const char* s) = nullptr;
const char* __stdcall hookstt(const char* s) {
	if (!shok::GetStringTableTextOverride)
		return nullptr;
	return shok::GetStringTableTextOverride(s);
}
void __declspec(naked) hooksttasm() {
	__asm {
		//int 3
		push[esp + 4];
		call hookstt;
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
void shok::HookGetStringTableText()
{
	if (HookGetStringTableText_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		throw 0;
	HookGetStringTableText_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ shok::GetStringTableText , 10 };
	CppLogic::Hooks::WriteJump(shok::GetStringTableText, &hooksttasm);
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

static inline const EGUIX::Color* (__thiscall* const uircustomcolorcont_getcol)(shok::UIRenderCustomColorContext* th, int i) = reinterpret_cast<const EGUIX::Color * (__thiscall*)(shok::UIRenderCustomColorContext*, int)>(0x5577B6);
const EGUIX::Color* shok::UIRenderCustomColorContext::GetColorByInt(int i)
{
	return uircustomcolorcont_getcol(this, i);
}

float __cdecl printstr_getlength_override(EGUIX::Font* f, const char* str, float multiply) {
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
	EGUIX::Font* f;
	float p[4];
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
	TextRenderer(shok::UIRenderer* r, const T* txt, EGUIX::Font* f, float p[4], float ldf, shok::UIRenderCustomColorContext* customcolordata,
		shok::Color defaultcolor)
		: startx(p[0]), fontsize(f->GetFontSize()), linedistance(fontsize* (ldf == 0.0f ? 1.0f : ldf))
		, defaultcolor(defaultcolor) {
		this->r = r;
		this->buff = txt;
		this->f = f;
		for (unsigned int i = 0; i < 4; ++i)
			this->p[i] = p[i];
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
				if (p[2] && linepos != lastwordpos && lastwordpos != line) {
					if ((f->GetTextLength(line, fontsize) + p[0]) >= p[2]) {
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
					if (p[2] && (f->GetTextLength(line, fontsize) + p[0]) >= p[2]) {
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
				if (skipWhitespace && linepos == line)
					SkipWhitespace(strpos);
				continue;
			}
			*linepos = *strpos;
			++strpos;
			if (*linepos == ' ' && p[2] && linepos != line) {
				*linepos = '\0';
				if ((f->GetTextLength(line, fontsize) + p[0]) >= p[2]) {
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
		p[0] = startx;
		p[1] -= linedistance;
	}

	int ParseIntParam(T*& plinepos) {
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
						c.R = static_cast<byte>(ParseIntParam(plinepos));
						if (*plinepos == ',') {
							++plinepos;
							c.G = static_cast<byte>(ParseIntParam(plinepos));
							if (*plinepos == ',') {
								++plinepos;
								c.B = static_cast<byte>(ParseIntParam(plinepos));
								if (*plinepos == ',') {
									++plinepos;
									c.A = static_cast<byte>(ParseIntParam(plinepos));
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
								f->RenderText(partialline, fontsize, p, r->TextRenderObj);
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
									f->RenderText(partialline, fontsize, p, r->TextRenderObj);
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
						f->RenderText(partialline, fontsize, p, r->TextRenderObj);
						partlinepos = partialline;
						r->SetTextRenderColor(defaultcolor);
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
			f->RenderText(partialline, fontsize, p, r->TextRenderObj);
			partlinepos = partialline;
		}
	}

public:
	void MainRender() {
		while (*strpos) {
			PrepareLine();
			if (ali == TextAlignment::Right) {
				float linelen = f->GetTextLength(line, fontsize);
				p[0] = p[2] - linelen;
			}
			else if (ali == TextAlignment::Center) {
				float linelen = f->GetTextLength(line, fontsize);
				if (p[2])
					p[0] = (p[2] + p[0]) / 2 - linelen / 2;
				else
					p[0] = p[0] - linelen / 2;
			}
			if (containsat)
				RenderAtLine();
			else
				f->RenderText(line, fontsize, p, r->TextRenderObj);

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
	EGUIX::Font* f = EGUIX::FontManager::GlobalObj()->GetFontObj(font);
	if (!f)
		return 0;
	// no idea what these funcs do
	reinterpret_cast<void(*)()>(0x707170)();
	reinterpret_cast<void(*)()>(0x706D60)();
	reinterpret_cast<void(__cdecl*)(void*, int)>(0x707A00)(r->TextRenderObj, 0);

	const shok::Color c = color ? color->ToShokColor() : shok::Color{};
	r->SetTextRenderColor(c);
	float p[4]{ x, y, xend, 0 };
	if (!uk) {
		p[0] = p[0] * 1024.0f / r->RenderSizeX;
		p[1] = p[1] * 768.0f / r->RenderSizeY;
		p[2] = p[2] * 1024.0f / r->RenderSizeX;
	}

	const float fontsize = f->GetFontSize();
	const float linedistance = fontsize * (ldf == 0.0f ? 1.0f : ldf);

	float somesize[2];
	float unknownfa0[2];
	float unknownfa1[2];
	// also no idea what this exactly does
	reinterpret_cast<void(__cdecl*)(float*, float*, float*)>(0x707400)(somesize, unknownfa0, unknownfa1);
	p[0] = p[0] * somesize[0] * r->RenderSizeX / 1024.0f;
	p[2] = p[2] * somesize[0] * r->RenderSizeX / 1024.0f;
	if (!r->SomeTextBool) {
		reinterpret_cast<void(__cdecl*)(float, float)>(0x707200)(1024.0f / r->RenderSizeX, 768.0f / r->RenderSizeY);
		p[1] = r->RenderSizeY / 768.0f - (fontsize * 768.0f + p[1]) / 768.0f;
	}
	else {
		p[1] = 1.0f - (fontsize * 768.0f + p[1]) / 768.0f;
	}
	
	reinterpret_cast<void(__cdecl*)(int, int)>(*reinterpret_cast<int*>(*reinterpret_cast<int*>(0x8501C8) + 32))(1, 0);
	reinterpret_cast<void(__cdecl*)(int, int)>(*reinterpret_cast<int*>(*reinterpret_cast<int*>(0x8501C8) + 32))(9, 2);

	// i dont think this was a template when BB did this, but this is way nicer as one
	if (f->Flags & 2) {
		static wchar_t buff[5001]{}; // same size that is used in the original func
		shok::UIRenderer::MultibyteToWString(txt, buff, 5000);
		TextRenderer<wchar_t> rend{ r, buff, f, p, ldf, customcolordata, c };
		rend.MainRender();
	}
	else {
		TextRenderer<char> rend{ r, txt, f, p, ldf, customcolordata, c };
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
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5577E1), 10 };
	CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x708F00), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5577E1), &printstr_override);
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x708F00), &printstr_getlength_override);
}


void ERwTools::CRpClumpRenderable::SetModelData(const ED::ModelData* modeldata, float rotation)
{
	SetModelData(modeldata ? *reinterpret_cast<void**>(const_cast<ED::ModelData*>(modeldata)) : nullptr, rotation);
}
void ERwTools::CRpClumpRenderable::SetPosition(const shok::Position& p, float z)
{
	SetPosition(p.X, p.Y, z);
}

static inline void(__thiscall*const c3dviewhandler_setguistate)(GGUI::C3DViewHandler* th, unsigned int id, GGUI::SStateParameters* p) = reinterpret_cast<void(__thiscall*)(GGUI::C3DViewHandler*, unsigned int, GGUI::SStateParameters*)>(0x52820C);
void GGUI::C3DViewHandler::SetGUIStateByIdentifier(unsigned int identifier)
{
	c3dviewhandler_setguistate(this, identifier, nullptr);
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
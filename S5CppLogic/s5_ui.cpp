#include "pch.h"
#include "s5_ui.h"
#include "s5_mapdisplay.h"
#include "s5_guistates.h"
#include "s5_defines.h"
#include "entityiterator.h"
#include "hooks.h"

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
		DEBUGGER_BREAK;
	HookGetStringTableText_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ shok::GetStringTableText , 10 };
	CppLogic::Hooks::WriteJump(shok::GetStringTableText, &hooksttasm);
}

void __declspec(naked) textprinting_getstringlen() {
	__asm {
		push 0;

	l_f60:
		cmp dword ptr[esp], 1;
		je l_skipOn;
		cmp ecx, 0x40; // @
		je l_turnSkipOn;
		jmp l_base;

	l_skipOn:
		cmp ecx, 0x20; // space
		je l_turnSkipOff;
		mov ecx, -1
			jmp l_f8e;

	l_turnSkipOn:
		mov dword ptr[esp], 1;
		jmp l_f8e;

	l_turnSkipOff:
		mov dword ptr[esp], 0;

	l_base:
		cmp ecx, 0x80; // base
		jl l_f89;
		sub ecx, [esi + 0x124];
		js l_f84;
		cmp ecx, [esi + 0x128];
		jge l_f84;
		mov eax, [esi + 0x12C];
		movsx ecx, word ptr[eax + ecx * 2];
		jmp l_f8e;
	l_f84:
		or ecx, -1;
		jmp l_f8e;
	l_f89:
		movsx ecx, word ptr[esi + ecx * 2 + 0x24];
	l_f8e:
		add edx, ebp; // next
		test ecx, ecx;
		jl l_fa3;
		mov eax, [esp + 0x14 + 8];
		shl ecx, 5;
		fld dword ptr[ecx + eax];
		fadd dword ptr[esi + 0xc];
		faddp st(1), st;
	l_fa3:
		movzx ecx, byte ptr[edx + edi];
		movzx eax, byte ptr[edx];
		and ecx, ebx;
		shl ecx, 8;
		or ecx, eax;
		jg l_f60;

		mov dword ptr[esp], 0x708FB3;
		ret;
	}
}
bool HookTextPrinting_Hooked = false;
void shok::HookTextPrinting()
{
	if (HookTextPrinting_Hooked)
		return;
	HookTextPrinting_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x557E47), 10 };
	CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x708F60), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x557E47), reinterpret_cast<void*>(0x557DAA)); // continue checking @ after center,... (redirecting an existing jmp, and removing a push for a previous parameter)
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x708F60), &textprinting_getstringlen);
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

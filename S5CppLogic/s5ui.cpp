#include "pch.h"
#include "s5data.h"

struct shok_vtable_GGUI_CState : shok_vtable_BB_IObject {
	void(__thiscall* OnMouseEvent)(shok_GGUI_CState* th, shok_BB_CEvent* ev);
	void(__thiscall* SetStateParameters)(shok_GGUI_CState* th, shok_GGUI_SStateParameters* p);
	void(__thiscall* Cancel)(shok_GGUI_CState* th);
	const char* (__thiscall* GetName)(shok_GGUI_CState* th);
	PADDINGI(1); // unknown, thiscall 1 param, cancel if called with 0
};
struct shok_vtable_GGUI_CBasicState : shok_vtable_GGUI_CState {
	bool(__thiscall* CheckCommandValid)(shok_GGUI_CBasicState* th, int* data, int zero); // data seems to be p to id, x, y,... ?
	void(__thiscall* ExecuteCommand)(shok_GGUI_CBasicState* th, int* target, int* selected); // both might ne p to data from above?
	int* (__thiscall* GetTargetData)(shok_GGUI_CBasicState* th, int* data, float x, float y); // creates data for above, has to return data
	void(__thiscall* OnMouseMove)(shok_GGUI_CBasicState* th, float x, float y);
	void(__thiscall* OnCancel)(shok_GGUI_CBasicState* th);
};
struct shok_vtable_GGUI_CPositionCommandState : shok_vtable_GGUI_CBasicState {
	void(__thiscall* ExecuteForPosAndEntity)(shok_GGUI_CPositionCommandState* th, int id, float x, float y, float r);
};

struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(shok_BB_IPostEvent* th, shok_BB_CEvent* ev);
};

static inline void(__thiscall*const c3dviewhandler_setguistate)(shok_GGUI_C3DViewHandler* th, unsigned int id, shok_GGUI_SStateParameters* p) = reinterpret_cast<void(__thiscall*)(shok_GGUI_C3DViewHandler*, unsigned int, shok_GGUI_SStateParameters*)>(0x52820C);
void shok_GGUI_C3DViewHandler::SetGUIStateByIdentifier(unsigned int identifier)
{
	c3dviewhandler_setguistate(this, identifier, nullptr);
}

void(__stdcall* PostEventOrig)(shok_BB_IPostEvent* th, shok_BB_CEvent* ev) = nullptr;
shok_vtable_BB_IPostEvent* BB_IPostEvent_vtableHooked = nullptr;
bool(*shok_GGUI_CManager::PostEventCallback)(shok_BB_CEvent* ev) = nullptr;
void __stdcall PostEventHook(shok_BB_IPostEvent* th, shok_BB_CEvent* ev) {
	if (shok_GGUI_CManager::PostEventCallback)
		if (shok_GGUI_CManager::PostEventCallback(ev))
			return;
	PostEventOrig(th, ev);
}
void shok_GGUI_CManager::HackPostEvent()
{
	if (PostEventOrig) {
		shok_saveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
		BB_IPostEvent_vtableHooked->PostEvent = PostEventOrig;
	}
	BB_IPostEvent_vtableHooked = reinterpret_cast<shok_vtable_BB_IPostEvent*>(PostEvent->vtable);
	shok_saveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
	PostEventOrig = BB_IPostEvent_vtableHooked->PostEvent;
	BB_IPostEvent_vtableHooked->PostEvent = reinterpret_cast<void(__stdcall*)(shok_BB_IPostEvent * th, shok_BB_CEvent * ev)>(&PostEventHook);
}


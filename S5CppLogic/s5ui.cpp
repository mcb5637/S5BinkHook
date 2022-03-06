#include "pch.h"
#include "s5data.h"
#include "entityiterator.h"

struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(BB::IPostEvent* th, BB::CEvent* ev);
};

struct shok_vtable_GGL_CGLGUIInterface {
	void(__thiscall* dtor)(GGL::CGLGUIInterface* th, bool free);
	PADDINGI(3);
	bool(__thiscall* GetNearestFreeBuildingPos)(GGL::CGLGUIInterface* th, int ety, float inx, float iny, float* outx, float* outy, float range);
};

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
		shok::SaveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
		BB_IPostEvent_vtableHooked->PostEvent = PostEventOrig;
	}
	BB_IPostEvent_vtableHooked = reinterpret_cast<shok_vtable_BB_IPostEvent*>(CppLogic::GetVTable(PostEvent));
	shok::SaveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
	PostEventOrig = BB_IPostEvent_vtableHooked->PostEvent;
	BB_IPostEvent_vtableHooked->PostEvent = reinterpret_cast<void(__stdcall*)(BB::IPostEvent * th, BB::CEvent * ev)>(&PostEventHook);
}

static inline int* (* const modifpressed_getobj)() = reinterpret_cast<int* (*)()>(0x558C16);
static inline bool(__thiscall* const modifpressed_ispressed)(int* ob7, shok::Keys m) = reinterpret_cast<bool(__thiscall*)(int*, shok::Keys)>(0x558C1C);
bool GGUI::CManager::IsModifierPressed(shok::Keys modif)
{
	return modifpressed_ispressed(modifpressed_getobj() + 7, modif);
}

#include "pch.h"
#include "s5data.h"
#include "entityiterator.h"

struct shok_vtable_GGUI_CState : shok_vtable_BB_IObject {
	bool(__thiscall* OnMouseEvent)(shok_GGUI_CState* th, BB::CEvent* ev);
	void(__thiscall* SetStateParameters)(shok_GGUI_CState* th, shok_GGUI_SStateParameters* p);
	int(__thiscall* Cancel)(shok_GGUI_CState* th);
	const char* (__thiscall* GetName)(shok_GGUI_CState* th);
	int(__thiscall* OnSelectionChanged)(shok_GGUI_CState*th, int z); // thiscall, on selection changed call with 0 -> cancel state
};
struct shok_vtable_GGUI_CBasicState : shok_vtable_GGUI_CState {
	bool(__thiscall* CheckCommandValid)(shok_GGUI_CBasicState* th, int* data, int zero); // data seems to be p to id, x, y,... ?
	void(__thiscall* ExecuteCommand)(shok_GGUI_CBasicState* th, int* target, int* selected); // both might ne p to data from above?
	int* (__thiscall* GetTargetData)(shok_GGUI_CBasicState* th, int* data, float x, float y); // creates data for above, has to return data
	void(__thiscall* OnMouseMove)(shok_GGUI_CBasicState* th, float x, float y);
	int(__thiscall* OnCancel)(shok_GGUI_CBasicState* th);
};
struct shok_vtable_GGUI_CPositionCommandState : shok_vtable_GGUI_CBasicState {
	void(__thiscall* ExecuteForPosAndEntity)(shok_GGUI_CPositionCommandState* th, int id, float x, float y, float r);
};

struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(shok_BB_IPostEvent* th, BB::CEvent* ev);
};

struct shok_vtable_ERwTools_CRpClumpRenderable {
	void(__thiscall* SetBuildingColoredRed)(shok_ERwTools_CRpClumpRenderable* th, int flag);
	PADDINGI(3);
	void(__thiscall* SetPosition)(shok_ERwTools_CRpClumpRenderable* th, float x, float y, float z);
	PADDINGI(1);
	void(__thiscall* SetModelData)(shok_ERwTools_CRpClumpRenderable* th, void* model, float rot);
};

struct shok_vtable_GGL_CGLGUIInterface {
	void(__thiscall* dtor)(shok_GGL_CGLGUIInterface* th, bool free);
	PADDINGI(3);
	bool(__thiscall* GetNearestFreeBuildingPos)(shok_GGL_CGLGUIInterface* th, int ety, float inx, float iny, float* outx, float* outy, float range);
};

void(__stdcall* const placebuildingstate_updatemodel)(shok_GGUI_CPlaceBuildingState* th) = reinterpret_cast<void(__stdcall*)(shok_GGUI_CPlaceBuildingState*)>(0x538C46);
void shok_GGUI_CPlaceBuildingState::UpdateModel()
{
	placebuildingstate_updatemodel(this);
}

float shok_GGUI_CPlaceBuildingState::PlacementRotation = 0;
void __declspec(naked) constructcommand_placebuilding() {
	__asm {
		fld[shok_GGUI_CPlaceBuildingState::PlacementRotation];
		mov eax, [eax + 0x3C];
		lea ecx, [ebp - 0x30];

		push 0x538FFC;
		ret;
	}
}
void __declspec(naked) constructcommand_checkposition() {
	__asm {
		sub esp, 0xC;
		fld[shok_GGUI_CPlaceBuildingState::PlacementRotation];
		mov ecx, esi;

		push 0x538A02;
		ret;
	}
}
float __stdcall constructcommand_getrotindeg() {
	return rad2deg(shok_GGUI_CPlaceBuildingState::PlacementRotation);
}
void __declspec(naked) constructcommand_setmodelrot() {
	__asm {
		push ecx; // gets replaced with float
		push eax;
		call constructcommand_getrotindeg; // for some reason i cant call rad2deg<float> directly here...
		fstp[esp+4]; // replacing
		mov ecx, edi;

		push 0x538B0A;
		ret;
	};
}
bool __stdcall constructcommand_getnearestpos(int ety, float x, float y, float* xo, float* yo) {
	shok::PositionRot p = shok_GGUI_CPlaceBuildingState::GetNearestPlacementPos(ety, { x, y, shok_GGUI_CPlaceBuildingState::PlacementRotation }, (*GGL::CLogicProperties::GlobalObj)->BuildingPlacementSnapDistance);
	if (p.X >= 0) {
		*xo = p.X;
		*yo = p.Y;
		shok_GGUI_CPlaceBuildingState::PlacementRotation = p.r;
		return true;
	}
	return false;
}
void __declspec(naked) constructcommand_checkpos() {
	__asm {
		lea eax, [ebp - 0x8];
		push eax;
		lea eax, [ebp - 0x4];
		push eax;
		push[ebp - 0x20];
		push[ebp - 0x24];
		push ebx;
		call constructcommand_getnearestpos;

		push 0x538C0E;
		ret;
	};
}
void(__thiscall* const constructcommand_updatemodelsetpos)(shok_GGUI_CPlaceBuildingState* th, int* p, int r) = reinterpret_cast<void(__thiscall*)(shok_GGUI_CPlaceBuildingState*, int*, int)>(0x5269FE);
void __fastcall constructcommand_updatemodelsetpos_over(shok_GGUI_CPlaceBuildingState* th, int _, int* p, int r) {
	th->C3DViewHandler->ClumpRenerable->Model->Rotate(rad2deg(shok_GGUI_CPlaceBuildingState::PlacementRotation), ED::ModelInstance::TransformOperation::Set);
	constructcommand_updatemodelsetpos(th, p, r);
}
bool HookConstructCommandRotation_Hooked = false;
void shok_GGUI_CPlaceBuildingState::HookPlacementRotation()
{
	if (HookConstructCommandRotation_Hooked)
		return;
	HookConstructCommandRotation_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x538FF4), 10 };
	WriteJump(reinterpret_cast<void*>(0x538FF4), &constructcommand_placebuilding);
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x5389FB), 10 };
	WriteJump(reinterpret_cast<void*>(0x5389FB), &constructcommand_checkposition);
	shok::SaveVirtualProtect vp3{ reinterpret_cast<void*>(0x538B01), 10 };
	WriteJump(reinterpret_cast<void*>(0x538B01), &constructcommand_setmodelrot);
	shok::SaveVirtualProtect vp4{ reinterpret_cast<void*>(0x538BDB), 10 };
	WriteJump(reinterpret_cast<void*>(0x538BDB), &constructcommand_checkpos);
	shok::SaveVirtualProtect vp5{ reinterpret_cast<void*>(0x538C8D), 10 };
	RedirectCall(reinterpret_cast<void*>(0x538C8D), &constructcommand_updatemodelsetpos_over);
}

shok::PositionRot shok_GGUI_CPlaceBuildingState::GetNearestPlacementPosBuildOn(int ety, const shok::Position& p, float range)
{
	const GGlue::CGlueEntityProps* e = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps);

	EntityIteratorPredicateOfPlayer predpl{ 0 };
	EntityIteratorPredicateAnyEntityType predety{ bp->BuildOn.data(), bp->BuildOn.size() };
	EntityIteratorPredicateInCircle predcir{ p, range };
	EntityIteratorPredicateFunc predfunc{ [](EGL::CGLEEntity* e) {
			return e->GetFirstAttachedToMe(shok::AttachmentType::BUILDING_BASE) == 0;
	} };
	EntityIteratorPredicate* preds[] = { &predpl, &predety, &predcir, &predfunc };
	EntityIteratorPredicateAnd predand{ preds, 4 };
	EntityIterator it{ &predand };
	auto* ent = it.GetNearest(nullptr);
	if (!ent)
		return { -1,-1,0 };
	return ent->Position;
}
shok::PositionRot shok_GGUI_CPlaceBuildingState::GetNearestPlacementPosFree(int ety, const shok::PositionRot& p, float range)
{
	const GGlue::CGlueEntityProps* e = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps);

	shok::Position r = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->BlockingData->GetFreeBuildingPlacementPos(bp, p, range);
	return { r.X, r.Y, p.r };
}
shok::PositionRot shok_GGUI_CPlaceBuildingState::GetNearestPlacementPos(int ety, const shok::PositionRot& p, float range)
{
	const GGlue::CGlueEntityProps* e = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps);
	if (bp->BuildOn.size() == 0)
		return GetNearestPlacementPosFree(ety, p, range);
	else
		return GetNearestPlacementPosBuildOn(ety, p, range);
}

void shok_ERwTools_CRpClumpRenderable::SetModelData(const ED::ModelData* modeldata, float rotation)
{
	reinterpret_cast<shok_vtable_ERwTools_CRpClumpRenderable*>(vtable)->SetModelData(this, modeldata ? *reinterpret_cast<void**>(const_cast<ED::ModelData*>(modeldata)) : nullptr, rotation);
}
void shok_ERwTools_CRpClumpRenderable::SetPosition(const shok::Position& p, float z)
{
	reinterpret_cast<shok_vtable_ERwTools_CRpClumpRenderable*>(vtable)->SetPosition(this, p.X, p.Y, z);
}
void shok_ERwTools_CRpClumpRenderable::SetBuildingRedColor(bool r)
{
	reinterpret_cast<shok_vtable_ERwTools_CRpClumpRenderable*>(vtable)->SetBuildingColoredRed(this, r ? 2 : 1);
}

static inline void(__thiscall*const c3dviewhandler_setguistate)(shok_GGUI_C3DViewHandler* th, unsigned int id, shok_GGUI_SStateParameters* p) = reinterpret_cast<void(__thiscall*)(shok_GGUI_C3DViewHandler*, unsigned int, shok_GGUI_SStateParameters*)>(0x52820C);
void shok_GGUI_C3DViewHandler::SetGUIStateByIdentifier(unsigned int identifier)
{
	c3dviewhandler_setguistate(this, identifier, nullptr);
}
static inline void(__thiscall* const c3dviewhandler_setguistateonupdate)(shok_GGUI_C3DViewHandler* th, unsigned int id) = reinterpret_cast<void(__thiscall*)(shok_GGUI_C3DViewHandler*, unsigned int)>(0x5280DE);
void shok_GGUI_C3DViewHandler::SetGUIStateByIdentfierOnNextUpdate(unsigned int identifier)
{
	c3dviewhandler_setguistateonupdate(this, identifier);
}

bool shok_GGL_CGLGUIInterface::GetNearestFreePosForBuildingPlacement(int ety, const shok::Position& inp, shok::Position& outp)
{
	return reinterpret_cast<shok_vtable_GGL_CGLGUIInterface*>(vtable)->GetNearestFreeBuildingPos(this, ety, inp.X, inp.Y, &outp.X, &outp.Y, -1);
}

void(__stdcall* PostEventOrig)(shok_BB_IPostEvent* th, BB::CEvent* ev) = nullptr;
shok_vtable_BB_IPostEvent* BB_IPostEvent_vtableHooked = nullptr;
bool(*shok_GGUI_CManager::PostEventCallback)(BB::CEvent* ev) = nullptr;
void __stdcall PostEventHook(shok_BB_IPostEvent* th, BB::CEvent* ev) {
	if (shok_GGUI_CManager::PostEventCallback)
		if (shok_GGUI_CManager::PostEventCallback(ev))
			return;
	PostEventOrig(th, ev);
}
void shok_GGUI_CManager::HackPostEvent()
{
	if (PostEventOrig) {
		shok::SaveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
		BB_IPostEvent_vtableHooked->PostEvent = PostEventOrig;
	}
	BB_IPostEvent_vtableHooked = reinterpret_cast<shok_vtable_BB_IPostEvent*>(PostEvent->vtable);
	shok::SaveVirtualProtect vp{ BB_IPostEvent_vtableHooked, 3 * 4 };
	PostEventOrig = BB_IPostEvent_vtableHooked->PostEvent;
	BB_IPostEvent_vtableHooked->PostEvent = reinterpret_cast<void(__stdcall*)(shok_BB_IPostEvent * th, BB::CEvent * ev)>(&PostEventHook);
}

static inline int* (* const modifpressed_getobj)() = reinterpret_cast<int* (*)()>(0x558C16);
static inline bool(__thiscall* const modifpressed_ispressed)(int* ob7, shok::Keys m) = reinterpret_cast<bool(__thiscall*)(int*, shok::Keys)>(0x558C1C);
bool shok_GGUI_CManager::IsModifierPressed(shok::Keys modif)
{
	return modifpressed_ispressed(modifpressed_getobj() + 7, modif);
}

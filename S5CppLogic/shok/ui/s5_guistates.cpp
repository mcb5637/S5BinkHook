#include "s5_guistates.h"
#include <shok/globals/s5_config.h>
#include <shok/ui/s5_ui.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/s5_defines.h>
#include <shok/entity/s5_entity.h>
#include <shok/engine/s5_RWEngine.h>
#include <utility/entityiterator.h>
#include <utility/hooks.h>

GGUI::SPlaceBuildingStateParameters::SPlaceBuildingStateParameters(shok::UpgradeCategoryId ucat)
{
	UCat = ucat;
}

inline void(__thiscall* const tdata_fillxyz)(GGUI::CBasicState::TargetData* th) = reinterpret_cast<void(__thiscall*)(GGUI::CBasicState::TargetData*)>(0x5432E6);
void GGUI::CBasicState::TargetData::FillPosWithZFromPos()
{
	tdata_fillxyz(this);
}

inline bool(__thiscall* const basicstate_mouseevent)(GGUI::CBasicState* th, BB::CEvent* ev) = reinterpret_cast<bool(__thiscall*)(GGUI::CBasicState*, BB::CEvent*)>(0x526BFE);
bool GGUI::CBasicState::OnMouseEvent(BB::CEvent* ev)
{
	return basicstate_mouseevent(this, ev);
}
void GGUI::CState::SetStateParameters(SStateParameters* p)
{
}
bool GGUI::CBasicState::Cancel()
{
	return OnCancel();
}
void GGUI::CState::OnSelectionChanged(int z)
{
	if (z == 0)
		Cancel();
}

void GGUI::CBasicState::OnMouseMove(int x, int y)
{
}

inline GGUI::CBasicState::TargetData* (__thiscall* const poscmdstate_gettar)(GGUI::CPositionCommandState* th, GGUI::CBasicState::TargetData* d, int x, int y) = reinterpret_cast<GGUI::CBasicState::TargetData * (__thiscall*)(GGUI::CPositionCommandState*, GGUI::CBasicState::TargetData*, int, int)>(0x526CB8);
GGUI::CBasicState::TargetData* GGUI::CPositionCommandState::GetTargetData(TargetData* d, int x, int y)
{
	return poscmdstate_gettar(this, d, x, y);
}
inline GGUI::CBasicState::TargetData* (__thiscall* const entcmdstate_gettar)(GGUI::CEntityCommandState* th, GGUI::CBasicState::TargetData* d, int x, int y) = reinterpret_cast<GGUI::CBasicState::TargetData * (__thiscall*)(GGUI::CEntityCommandState*, GGUI::CBasicState::TargetData*, int, int)>(0x526C9E);
GGUI::CBasicState::TargetData* GGUI::CEntityCommandState::GetTargetData(TargetData* d, int x, int y)
{
	return entcmdstate_gettar(this, d, x, y);
}

inline void(__thiscall* const basicstate_fillpos)(GGUI::CBasicState* th, GGUI::CBasicState::TargetData* d, int x, int y) = reinterpret_cast<void(__thiscall*)(GGUI::CBasicState*, GGUI::CBasicState::TargetData*, int, int)>(0x526B46);
void GGUI::CBasicState::FillPosData(TargetData* d, int x, int y)
{
	basicstate_fillpos(this, d, x, y);
}
inline void(__thiscall* const basicstate_fillent)(GGUI::CBasicState* th, GGUI::CBasicState::TargetData* d, int x, int y) = reinterpret_cast<void(__thiscall*)(GGUI::CBasicState*, GGUI::CBasicState::TargetData*, int, int)>(0x526A9B);
void GGUI::CBasicState::FillEntityData(TargetData* d, int x, int y)
{
	basicstate_fillent(this, d, x, y);
}

inline void(__stdcall* const placebuildingstate_updatemodel)(GGUI::CPlaceBuildingState* th) = reinterpret_cast<void(__stdcall*)(GGUI::CPlaceBuildingState*)>(0x538C46);
void GGUI::CPlaceBuildingState::UpdateModel()
{
	placebuildingstate_updatemodel(this);
}
inline void(__thiscall* const placebuild_setmodeltorender)(GGUI::CPlaceBuildingState* th) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*)>(0x538AC4);
void GGUI::CPlaceBuildingState::SetModelToRender()
{
	placebuild_setmodeltorender(this);
}
inline void(__thiscall* const placebuild_setmodeltorenderex)(GGUI::CPlaceBuildingState* th, RWE::RpClump* c, float r) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*, RWE::RpClump*, float)>(0x5269D5);
void GGUI::CPlaceBuildingState::SetModelToRender(RWE::RpClump* c, float r)
{
	placebuild_setmodeltorenderex(this, c, r);
}

inline void(__thiscall* const placebuild_ctor)(GGUI::CPlaceBuildingState* th) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*)>(0x538E97);
GGUI::CPlaceBuildingState::CPlaceBuildingState()
{
	placebuild_ctor(this);
}
inline void(__thiscall* const placebuild_dtor)(GGUI::CPlaceBuildingState* th) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*)>(0x538D56);
GGUI::CPlaceBuildingState::~CPlaceBuildingState()
{
	placebuild_dtor(this);
}
inline void(__thiscall* const placebuild_setparams)(GGUI::CPlaceBuildingState* th, GGUI::SStateParameters* p) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*, GGUI::SStateParameters*)>(0x538CE5);
void GGUI::CPlaceBuildingState::SetStateParameters(SStateParameters* p)
{
	placebuild_setparams(this, p);
}
inline void(__thiscall* const placebuld_onselchanged)(GGUI::CPlaceBuildingState* th, int z) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*, int)>(0x538971);
void GGUI::CPlaceBuildingState::OnSelectionChanged(int z)
{
	placebuld_onselchanged(this, z);
}
inline bool(__thiscall* const placebuild_checkcmd)(GGUI::CPlaceBuildingState* th, GGUI::CBasicState::TargetData* d, int z) = reinterpret_cast<bool(__thiscall*)(GGUI::CPlaceBuildingState*, GGUI::CBasicState::TargetData*, int)>(0x5389BD);
bool GGUI::CPlaceBuildingState::CheckCommandValid(TargetData* d, int z)
{
	return placebuild_checkcmd(this, d, z);
}
inline void(__thiscall* const placebuild_exec)(GGUI::CPlaceBuildingState* th, GGUI::CBasicState::TargetData* d, GGUI::CBasicState::ExecuteData* selectedID) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*, GGUI::CBasicState::TargetData*, GGUI::CBasicState::ExecuteData*)>(0x538F35);
void GGUI::CPlaceBuildingState::ExecuteCommand(TargetData* d, ExecuteData* selectedID)
{
	placebuild_exec(this, d, selectedID);
}
inline GGUI::CBasicState::TargetData* (__thiscall* const placebuild_getdata)(GGUI::CPlaceBuildingState* th, GGUI::CBasicState::TargetData* d, int x, int y) = reinterpret_cast<GGUI::CBasicState::TargetData * (__thiscall*)(GGUI::CPlaceBuildingState*, GGUI::CBasicState::TargetData*, int, int)>(0x538B11);
GGUI::CBasicState::TargetData* GGUI::CPlaceBuildingState::GetTargetData(TargetData* d, int x, int y)
{
	return placebuild_getdata(this, d, x, y);
}
inline void(__thiscall* const placebuild_mousemove)(GGUI::CPlaceBuildingState* th, int x, int y) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*, int, int)>(0x538D35);
void GGUI::CPlaceBuildingState::OnMouseMove(int x, int y)
{
	placebuild_mousemove(this, x, y);
}
inline bool(__thiscall* const placebuild_oncancel)(GGUI::CPlaceBuildingState* th) = reinterpret_cast<bool(__thiscall*)(GGUI::CPlaceBuildingState*)>(0x5388DA);
bool GGUI::CPlaceBuildingState::OnCancel()
{
	return placebuild_oncancel(this);
}


float GGUI::CPlaceBuildingState::PlacementRotation = 0;
void __declspec(naked) constructcommand_placebuilding() {
	__asm {
		fld dword ptr [GGUI::CPlaceBuildingState::PlacementRotation];
		mov eax, [eax + 0x3C];
		lea ecx, [ebp - 0x30];

		push 0x538FFC;
		ret;
	}
}
void __declspec(naked) constructcommand_checkposition() {
	__asm {
		sub esp, 0xC;
		fld dword ptr [GGUI::CPlaceBuildingState::PlacementRotation];
		mov ecx, esi;

		push 0x538A02;
		ret;
	}
}
float __stdcall constructcommand_getrotindeg() {
	return CppLogic::RadiansToDegrees(GGUI::CPlaceBuildingState::PlacementRotation);
}
void __declspec(naked) constructcommand_setmodelrot() {
	__asm {
		push ecx; // gets replaced with float
		push eax;
		call constructcommand_getrotindeg; // for some reason i cant call CppLogic::RadiansToDegrees<float> directly here...
		fstp dword ptr[esp + 4]; // replacing
		mov ecx, edi;

		push 0x538B0A;
		ret;
	};
}
bool __stdcall constructcommand_getnearestpos(shok::EntityTypeId ety, float x, float y, float* xo, float* yo) {
	shok::PositionRot p = GGUI::CPlaceBuildingState::GetNearestPlacementPos(ety, { x, y, GGUI::CPlaceBuildingState::PlacementRotation }, (*GGL::CLogicProperties::GlobalObj)->BuildingPlacementSnapDistance);
	if (p.X >= 0) {
		*xo = p.X;
		*yo = p.Y;
		GGUI::CPlaceBuildingState::PlacementRotation = p.r;
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
void(__thiscall* const constructcommand_updatemodelsetpos)(GGUI::CPlaceBuildingState* th, int* p, int r) = reinterpret_cast<void(__thiscall*)(GGUI::CPlaceBuildingState*, int*, int)>(0x5269FE);
void __fastcall constructcommand_updatemodelsetpos_over(GGUI::CPlaceBuildingState* th, int _, int* p, int r) {
	th->C3DViewHandler->ClumpRenerable->Model->GetFrame()->Rotate(CppLogic::RadiansToDegrees(GGUI::CPlaceBuildingState::PlacementRotation), RWE::RwOpCombineType::Replace);
	constructcommand_updatemodelsetpos(th, p, r);
}
bool HookConstructCommandRotation_Hooked = false;
void GGUI::CPlaceBuildingState::HookPlacementRotation()
{
	if (HookConstructCommandRotation_Hooked)
		return;
	HookConstructCommandRotation_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x538FF4),
		reinterpret_cast<void*>(0x5389FB),
		reinterpret_cast<void*>(0x538B01),
		reinterpret_cast<void*>(0x538BDB),
		reinterpret_cast<void*>(0x538C8D)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x538FF4), &constructcommand_placebuilding, reinterpret_cast<void*>(0x538FFC));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5389FB), &constructcommand_checkposition, reinterpret_cast<void*>(0x538A02));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x538B01), &constructcommand_setmodelrot, reinterpret_cast<void*>(0x538B0A));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x538BDB), &constructcommand_checkpos, reinterpret_cast<void*>(0x538C0E));
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x538C8D), &constructcommand_updatemodelsetpos_over);
}

shok::PositionRot GGUI::CPlaceBuildingState::GetNearestPlacementPosBuildOn(shok::EntityTypeId ety, const shok::Position& p, float range)
{
	const GGlue::CGlueEntityProps* e = CppLogic::GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps); // NOLINT(*-pro-type-static-cast-downcast)

	CppLogic::Iterator::EntityPredicateOfPlayer pl{ static_cast<shok::PlayerId>(0) };
	CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, range * range };
	CppLogic::Iterator::EntityPredicateOfAnyType pety{};
	pety.entityTypes.reserve(bp->BuildOn.size());
	for (auto t : bp->BuildOn)
		pety.entityTypes.push_back(t);
	auto fun = CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity>([](const EGL::CGLEEntity* e, float*, int*) {
		return e->GetFirstAttachedToMe(shok::AttachmentType::BUILDING_BASE) == static_cast<shok::EntityId>(0);
	});
	CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 4> pr{ &pl, &pety, &cir, &fun };
	CppLogic::Iterator::GlobalEntityIterator it{ &pr };
	if (auto* ent = it.GetNearest(nullptr))
		return ent->Position;
	return { -1,-1,0 };
}
shok::PositionRot GGUI::CPlaceBuildingState::GetNearestPlacementPosFree(shok::EntityTypeId ety, const shok::PositionRot& p, float range)
{
	const GGlue::CGlueEntityProps* e = CppLogic::GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps); // NOLINT(*-pro-type-static-cast-downcast)

	shok::Position r = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->BlockingData->GetFreeBuildingPlacementPos(bp, p, range);
	return { r.X, r.Y, p.r };
}
shok::PositionRot GGUI::CPlaceBuildingState::GetNearestPlacementPos(shok::EntityTypeId ety, const shok::PositionRot& p, float range)
{
	const GGlue::CGlueEntityProps* e = CppLogic::GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps); // NOLINT(*-pro-type-static-cast-downcast)
	if (bp->BuildOn.size() == 0)
		return GetNearestPlacementPosFree(ety, p, range);
	else
		return GetNearestPlacementPosBuildOn(ety, p, range);
}

void __stdcall selectionstate_getwindowsize(int* o) {
	RECT r;
	if (GetClientRect(*shok::MainWindowHandle, &r)) {
		CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
			reinterpret_cast<void*>(0x528DFF),
		} };
		o[0] = r.right;
		o[1] = r.bottom;
	}
}
void __declspec(naked) selectionstate_getwindowsize_asm() {
	__asm {
		push 0x300;
		push 0x400;

		push esp;
		call selectionstate_getwindowsize;

		push 0x528E09;
		ret;
	};
}
void GGUI::CSelectionState::HookFixDoubleClickSelection()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x528DFF),
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x528DFF), &selectionstate_getwindowsize_asm, reinterpret_cast<void*>(0x528E09));
}

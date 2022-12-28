#include "pch.h"
#include "s5_guistates.h"
#include "s5_config.h"
#include "s5_mapdisplay.h"
#include "s5_ui.h"
#include "s5_entitytype.h"
#include "s5_defines.h"
#include "s5_entity.h"
#include "s5_RWEngine.h"
#include "entityiterator.h"
#include "hooks.h"

GGUI::SPlaceBuildingStateParameters::SPlaceBuildingStateParameters(int ucat)
{
	UCat = ucat;
}

inline void(__thiscall* const tdata_fillxyz)(GGUI::CBasicState::TargetData* th) = reinterpret_cast<void(__thiscall*)(GGUI::CBasicState::TargetData*)>(0x5432E6);
void GGUI::CBasicState::TargetData::FillPosWithZFromPos()
{
	tdata_fillxyz(this);
}

inline bool (__thiscall* const basicstate_mouseevent)(GGUI::CBasicState* th, BB::CEvent* ev) = reinterpret_cast<bool (__thiscall*)(GGUI::CBasicState*, BB::CEvent*)>(0x526BFE);
bool GGUI::CBasicState::OnMouseEvent(BB::CEvent* ev)
{
	return basicstate_mouseevent(this, ev);
}
bool GGUI::CBasicState::Cancel()
{
	return OnCancel();
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
inline int(__thiscall* const placebuld_onselchanged)(GGUI::CPlaceBuildingState* th, int z) = reinterpret_cast<int(__thiscall*)(GGUI::CPlaceBuildingState*, int)>(0x538971);
int GGUI::CPlaceBuildingState::OnSelectionChanged(int z)
{
	return placebuld_onselchanged(this, z);
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
inline GGUI::CBasicState::TargetData* (__thiscall* const placebuild_getdata)(GGUI::CPlaceBuildingState* th, GGUI::CBasicState::TargetData* d, int x, int y) = reinterpret_cast<GGUI::CBasicState::TargetData* (__thiscall*)(GGUI::CPlaceBuildingState*, GGUI::CBasicState::TargetData*, int, int)>(0x538B11);
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
		fld[GGUI::CPlaceBuildingState::PlacementRotation];
		mov eax, [eax + 0x3C];
		lea ecx, [ebp - 0x30];

		push 0x538FFC;
		ret;
	}
}
void __declspec(naked) constructcommand_checkposition() {
	__asm {
		sub esp, 0xC;
		fld[GGUI::CPlaceBuildingState::PlacementRotation];
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
		fstp[esp + 4]; // replacing
		mov ecx, edi;

		push 0x538B0A;
		ret;
	};
}
bool __stdcall constructcommand_getnearestpos(int ety, float x, float y, float* xo, float* yo) {
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

shok::PositionRot GGUI::CPlaceBuildingState::GetNearestPlacementPosBuildOn(int ety, const shok::Position& p, float range)
{
	const GGlue::CGlueEntityProps* e = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps);

	CppLogic::Iterator::EntityPredicateOfPlayer pl{ 0 };
	CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, range * range };
	CppLogic::Iterator::EntityPredicateOfAnyType pety{};
	pety.entityTypes.reserve(bp->BuildOn.size());
	for (int t : bp->BuildOn)
		pety.entityTypes.push_back(t);
	CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity> fun{ [](const EGL::CGLEEntity* e, float*, int*) {
		return e->GetFirstAttachedToMe(shok::AttachmentType::BUILDING_BASE) == 0;
	} };
	CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 4> pr{ &pl, &pety, &cir, &fun };
	CppLogic::Iterator::GlobalEntityIterator it{ &pr };
	if (auto* ent = it.GetNearest(nullptr))
		return ent->Position;
	return { -1,-1,0 };
}
shok::PositionRot GGUI::CPlaceBuildingState::GetNearestPlacementPosFree(int ety, const shok::PositionRot& p, float range)
{
	const GGlue::CGlueEntityProps* e = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps);

	shok::Position r = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->BlockingData->GetFreeBuildingPlacementPos(bp, p, range);
	return { r.X, r.Y, p.r };
}
shok::PositionRot GGUI::CPlaceBuildingState::GetNearestPlacementPos(int ety, const shok::PositionRot& p, float range)
{
	const GGlue::CGlueEntityProps* e = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ety);
	const GGL::CGLBuildingProps* bp = static_cast<GGL::CGLBuildingProps*>(e->LogicProps);
	if (bp->BuildOn.size() == 0)
		return GetNearestPlacementPosFree(ety, p, range);
	else
		return GetNearestPlacementPosBuildOn(ety, p, range);
}

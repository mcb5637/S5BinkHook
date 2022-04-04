#include "pch.h"
#include "s5_guistates.h"
#include "s5_config.h"
#include "s5_mapdisplay.h"
#include "entityiterator.h"
#include "s5_ui.h"
#include "s5_entitytype.h"
#include "s5_defines.h"
#include "s5_entity.h"
#include "hooks.h"


void(__stdcall* const placebuildingstate_updatemodel)(GGUI::CPlaceBuildingState* th) = reinterpret_cast<void(__stdcall*)(GGUI::CPlaceBuildingState*)>(0x538C46);
void GGUI::CPlaceBuildingState::UpdateModel()
{
	placebuildingstate_updatemodel(this);
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
	th->C3DViewHandler->ClumpRenerable->Model->Rotate(CppLogic::RadiansToDegrees(GGUI::CPlaceBuildingState::PlacementRotation), ED::ModelInstance::TransformOperation::Set);
	constructcommand_updatemodelsetpos(th, p, r);
}
bool HookConstructCommandRotation_Hooked = false;
void GGUI::CPlaceBuildingState::HookPlacementRotation()
{
	if (HookConstructCommandRotation_Hooked)
		return;
	HookConstructCommandRotation_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x538FF4), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x538FF4), &constructcommand_placebuilding);
	CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x5389FB), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5389FB), &constructcommand_checkposition);
	CppLogic::Hooks::SaveVirtualProtect vp3{ reinterpret_cast<void*>(0x538B01), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x538B01), &constructcommand_setmodelrot);
	CppLogic::Hooks::SaveVirtualProtect vp4{ reinterpret_cast<void*>(0x538BDB), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x538BDB), &constructcommand_checkpos);
	CppLogic::Hooks::SaveVirtualProtect vp5{ reinterpret_cast<void*>(0x538C8D), 10 };
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

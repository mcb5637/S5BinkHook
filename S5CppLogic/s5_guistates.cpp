#include "pch.h"
#include "s5data.h"
#include "entityiterator.h"


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
	return rad2deg(GGUI::CPlaceBuildingState::PlacementRotation);
}
void __declspec(naked) constructcommand_setmodelrot() {
	__asm {
		push ecx; // gets replaced with float
		push eax;
		call constructcommand_getrotindeg; // for some reason i cant call rad2deg<float> directly here...
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
	th->C3DViewHandler->ClumpRenerable->Model->Rotate(rad2deg(GGUI::CPlaceBuildingState::PlacementRotation), ED::ModelInstance::TransformOperation::Set);
	constructcommand_updatemodelsetpos(th, p, r);
}
bool HookConstructCommandRotation_Hooked = false;
void GGUI::CPlaceBuildingState::HookPlacementRotation()
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

shok::PositionRot GGUI::CPlaceBuildingState::GetNearestPlacementPosBuildOn(int ety, const shok::Position& p, float range)
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

#include "pch.h"
#include "s5data.h"
#include <cmath>

static inline int(__thiscall* const plattracthandlerGetAttLimit)(shok_GGL_CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*)>(0x4C216F);
int shok_GGL_CPlayerAttractionHandler::GetAttractionLimit()
{
	return plattracthandlerGetAttLimit(this);
}

static inline int(__thiscall* const plattracthandlerGetAttUsage)(shok_GGL_CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*)>(0x4C278B);
int shok_GGL_CPlayerAttractionHandler::GetAttractionUsage()
{
	return plattracthandlerGetAttUsage(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getplayersettlerpayday)(shok_GGL_CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*)>(0x4C1CDE);
int shok_GGL_CPlayerAttractionHandler::GetWorkerPaydayIncome()
{
	return shok_playerattractionhandler_getplayersettlerpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumattractedworkers)(shok_GGL_CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*)>(0x4C1BAA);
int shok_GGL_CPlayerAttractionHandler::GetNumberOfAttractedWorkers()
{
	return shok_playerattractionhandler_getnumattractedworkers(this);
}
static inline float(__thiscall* const shok_playerattractionhandler_getplayerleaderpayday)(shok_GGL_CPlayerAttractionHandler* th) = reinterpret_cast<float(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*)>(0x4C1D1E);
float shok_GGL_CPlayerAttractionHandler::GetLeaderPaydayCost()
{
	return shok_playerattractionhandler_getplayerleaderpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumleaders)(shok_GGL_CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*)>(0x4C1B28);
int shok_GGL_CPlayerAttractionHandler::GetNumberOfLeaders()
{
	return shok_playerattractionhandler_getnumleaders(this);
}

void (*shok_GGL_CPlayerAttractionHandler::OnCheckPayDayCallback)(shok_GGL_CPlayerAttractionHandler* th) = nullptr;
void __fastcall hookedcheckpayday_call(shok_GGL_CPlayerAttractionHandler* th) {
	if (shok_GGL_CPlayerAttractionHandler::OnCheckPayDayCallback)
		shok_GGL_CPlayerAttractionHandler::OnCheckPayDayCallback(th);
}
void __declspec(naked) hookedcheckpayday() {
	__asm {
		mov ecx, esi;
		call hookedcheckpayday_call;

		mov esi, [esi + 4];
		mov[ebp + 0x18], 0x1300E;
		push 0x4C275E;
		ret;
	}
}
bool HookCheckPayday_Hooked = false;
void shok_GGL_CPlayerAttractionHandler::HookCheckPayday()
{
	if (HookCheckPayday_Hooked)
		return;
	if (HasSCELoader())
		DEBUGGER_BREAK;
	HookCheckPayday_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4C2754), 10 };
	WriteJump(reinterpret_cast<void*>(0x4C2754), &hookedcheckpayday);
}

static inline int(__thiscall* const upgrademanager_getucatbybuilding)(shok_GGL_CBuildingUpgradeManager* th, int id) = reinterpret_cast<int(__thiscall*)(shok_GGL_CBuildingUpgradeManager*, int)>(0x4B3CA6);
int shok_GGL_CBuildingUpgradeManager::GetUpgradeCategoryOfBuildingType(int etype)
{
	return upgrademanager_getucatbybuilding(this, etype);
}

static inline int(__thiscall* const settlerupmanager_getstybyucat)(shok_GGL_CSettlerUpgradeManager* th, int ucat) = reinterpret_cast<int(__thiscall*)(shok_GGL_CSettlerUpgradeManager*, int)>(0x4B3280);
int shok_GGL_CSettlerUpgradeManager::GetSettlerTypeByUCat(int ucat)
{
	return settlerupmanager_getstybyucat(this, ucat);
}

static inline shok_DiploState(__thiscall* const shok_GGL_CPlayerStatus_getDiploState)(int* d, int p) = reinterpret_cast<shok_DiploState(__thiscall*)(int* d, int p)>(0x4B4D5B);
shok_DiploState shok_GGL_CPlayerStatus::GetDiploStateTo(int p)
{
	return shok_GGL_CPlayerStatus_getDiploState(DiplomacyData, p);
}

static inline shok_TechState(__thiscall* const playerstatus_gettechstatus)(shok_GGL_CPlayerStatus_techData* th, int tech) = reinterpret_cast<shok_TechState(__thiscall*)(shok_GGL_CPlayerStatus_techData*, int)>(0x4A2A8D);
shok_TechState shok_GGL_CPlayerStatus::GetTechStatus(int tech)
{
	return playerstatus_gettechstatus(&TechnologyStates, tech);
}


bool ArePlayersHostile(int p1, int p2)
{
	return (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(p1)->GetDiploStateTo(p2) == shok_DiploState::Hostile;
}
bool ArePlayersFriendly(int p1, int p2)
{
	return (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(p1)->GetDiploStateTo(p2) == shok_DiploState::Friendly;
}

bool (*CanPlaceBuildingCallback)(int entitytype, int player, shok_position* pos, float rotation, int buildOnId) = nullptr;
int __stdcall canplacebuilding(int entitytype, int player, shok_position* pos, float rotation, int buildOnId) {
	if (CanPlaceBuildingCallback)
		return CanPlaceBuildingCallback(entitytype, player, pos, rotation, buildOnId);
	return true;
}
void __declspec(naked) canplacebuildingasm() {
	__asm {

		test bl, bl;
		jz retu;

		push[ebp + 0x18];
		push[ebp + 0x14];
		push[ebp + 0x10];
		push[ebp + 0xC];
		push[ebp + 0x8];
		call canplacebuilding;
		jmp modif;

	retu:
		mov al, bl;

	modif:
		mov ecx, [ebp + 0xC];
		pop ebx;
		push 0x4B45BF;
		ret;
	}
}
bool HookCanPlaceBuilding_Hooked = false;
void HookCanPlaceBuilding()
{
	if (HookCanPlaceBuilding_Hooked)
		return;
	HookCanPlaceBuilding_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4B45B9), 10 };
	WriteJump(reinterpret_cast<void*>(0x4B45B9), &canplacebuildingasm);
}

float ConstructBuildingRotation = 0.0f;
void __declspec(naked) constructcommandbuildingnetevent() {
	__asm {
		fld[ConstructBuildingRotation];
		mov eax, [eax + 0x3C];
		lea ecx, [ebp - 0x30];

		push 0x538FFC;
		ret;
	}
}
void __declspec(naked) constructcommandplacementpreviewcheck() {
	__asm {
		sub esp, 0xC;
		fld[ConstructBuildingRotation];
		mov ecx, esi;

		push 0x538A02;
		ret;
	}
}
void __fastcall constructcommandsetbuildingmodel_setrot(float***** p) { // TODO most of this are objects, further research needed
	if (p[1]) {
		p[1][5][2][1][4] = std::cos(ConstructBuildingRotation);
		p[1][5][2][1][5] = std::sin(ConstructBuildingRotation);
		p[1][5][2][1][8] = -std::sin(ConstructBuildingRotation);
		p[1][5][2][1][9] = std::cos(ConstructBuildingRotation);
	}
}
int __declspec(naked) constructcommandsetbuildingmodel() {
	__asm{
		push ecx;
		call constructcommandsetbuildingmodel_setrot;
		pop ecx;
		
		mov eax, 0x5269FE;
		call eax;

		push 0x538C92;
		ret;
	}
}
bool HookConstructCommandRotation_Hooked = false;
void HookConstructCommandRotation()
{
	if (HookConstructCommandRotation_Hooked)
		return;
	HookConstructCommandRotation_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x5389FB), 0x538FF4 - 0x5389FB + 10 };
	WriteJump(reinterpret_cast<void*>(0x538FF4), &constructcommandbuildingnetevent);
	WriteJump(reinterpret_cast<void*>(0x5389FB), &constructcommandplacementpreviewcheck);
	WriteJump(reinterpret_cast<void*>(0x538C8D), &constructcommandsetbuildingmodel);
}

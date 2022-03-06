#include "pch.h"
#include "s5data.h"
#include <cmath>

static inline int(__thiscall* const plattracthandlerGetAttLimit)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall*)(GGL::CPlayerAttractionHandler*)>(0x4C216F);
int GGL::CPlayerAttractionHandler::GetAttractionLimit()
{
	return plattracthandlerGetAttLimit(this);
}

static inline int(__thiscall* const plattracthandlerGetAttUsage)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall*)(GGL::CPlayerAttractionHandler*)>(0x4C278B);
int GGL::CPlayerAttractionHandler::GetAttractionUsage()
{
	return plattracthandlerGetAttUsage(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getplayersettlerpayday)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1CDE);
int GGL::CPlayerAttractionHandler::GetWorkerPaydayIncome()
{
	return shok_playerattractionhandler_getplayersettlerpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumattractedworkers)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1BAA);
int GGL::CPlayerAttractionHandler::GetNumberOfAttractedWorkers()
{
	return shok_playerattractionhandler_getnumattractedworkers(this);
}
static inline float(__thiscall* const shok_playerattractionhandler_getplayerleaderpayday)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<float(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1D1E);
float GGL::CPlayerAttractionHandler::GetLeaderPaydayCost()
{
	return shok_playerattractionhandler_getplayerleaderpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumleaders)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1B28);
int GGL::CPlayerAttractionHandler::GetNumberOfLeaders()
{
	return shok_playerattractionhandler_getnumleaders(this);
}

void (*GGL::CPlayerAttractionHandler::OnCheckPayDayCallback)(GGL::CPlayerAttractionHandler* th) = nullptr;
void __fastcall hookedcheckpayday_call(GGL::CPlayerAttractionHandler* th) {
	if (GGL::CPlayerAttractionHandler::OnCheckPayDayCallback)
		GGL::CPlayerAttractionHandler::OnCheckPayDayCallback(th);
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
void GGL::CPlayerAttractionHandler::HookCheckPayday()
{
	if (HookCheckPayday_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		DEBUGGER_BREAK;
	HookCheckPayday_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4C2754), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4C2754), &hookedcheckpayday);
}

static inline int(__thiscall* const upgrademanager_getucatbybuilding)(GGL::CUpgradeManager* th, int id) = reinterpret_cast<int(__thiscall*)(GGL::CUpgradeManager*, int)>(0x4B3CA6);
int GGL::CUpgradeManager::GetUpgradeCategoryOfEntityType(int etype)
{
	return upgrademanager_getucatbybuilding(this, etype);
}

static inline int(__thiscall* const settlerupmanager_getstybyucat)(GGL::CSettlerUpgradeManager* th, int ucat) = reinterpret_cast<int(__thiscall*)(GGL::CSettlerUpgradeManager*, int)>(0x4B3280);
int GGL::CSettlerUpgradeManager::GetSettlerTypeByUCat(int ucat)
{
	return settlerupmanager_getstybyucat(this, ucat);
}

unsigned int __stdcall GGL::CResourceStatistics::GetClassIdentifier() const
{
	return 0xC1488787;
}

unsigned int __stdcall GGL::CGameStatistics::GetClassIdentifier() const
{
	return 0x964BEA57;
}

static inline shok::DiploState(__thiscall* const shok_GGL_CPlayerStatus_getDiploState)(int* d, int p) = reinterpret_cast<shok::DiploState(__thiscall*)(int* d, int p)>(0x4B4D5B);
shok::DiploState GGL::CPlayerStatus::GetDiploStateTo(int p)
{
	return shok_GGL_CPlayerStatus_getDiploState(DiplomacyData, p);
}

static inline shok::TechState(__thiscall* const playerstatus_gettechstatus)(GGL::PlayerTechManager* th, int tech) = reinterpret_cast<shok::TechState(__thiscall*)(GGL::PlayerTechManager*, int)>(0x4A2A8D);
shok::TechState GGL::CPlayerStatus::GetTechStatus(int tech)
{
	return playerstatus_gettechstatus(&TechnologyStates, tech);
}


bool GGL::CPlayerStatus::ArePlayersHostile(int p1, int p2)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p1);
	if (!ps)
		return false;
	return ps->GetDiploStateTo(p2) == shok::DiploState::Hostile;
}
bool GGL::CPlayerStatus::ArePlayersFriendly(int p1, int p2)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p1);
	if (!ps)
		return false;
	return ps->GetDiploStateTo(p2) == shok::DiploState::Friendly;
}

bool (*GGL::CPlayerStatus::CanPlaceBuildingCallback)(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId) = nullptr;
int __stdcall canplacebuilding(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId) {
	if (GGL::CPlayerStatus::CanPlaceBuildingCallback)
		return GGL::CPlayerStatus::CanPlaceBuildingCallback(entitytype, player, pos, rotation, buildOnId);
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
void GGL::CPlayerStatus::HookCanPlaceBuilding()
{
	if (HookCanPlaceBuilding_Hooked)
		return;
	HookCanPlaceBuilding_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4B45B9), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4B45B9), &canplacebuildingasm);
}

#include "pch.h"
#include "s5data.h"

static inline int(__thiscall* const plattracthandlerGetAttLimit)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*))0x4C216F;
int shok_GGL_CPlayerAttractionHandler::GetAttractionLimit()
{
	return plattracthandlerGetAttLimit(this);
}

static inline int(__thiscall* const plattracthandlerGetAttUsage)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*))0x4C278B;
int shok_GGL_CPlayerAttractionHandler::GetAttractionUsage()
{
	return plattracthandlerGetAttUsage(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getplayersettlerpayday)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*))0x4C1CDE;
int shok_GGL_CPlayerAttractionHandler::GetWorkerPaydayIncome()
{
	return shok_playerattractionhandler_getplayersettlerpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumattractedworkers)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*))0x4C1BAA;
int shok_GGL_CPlayerAttractionHandler::GetNumberOfAttractedWorkers()
{
	return shok_playerattractionhandler_getnumattractedworkers(this);
}
static inline float(__thiscall* const shok_playerattractionhandler_getplayerleaderpayday)(shok_GGL_CPlayerAttractionHandler* th) = (float(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*))0x4C1D1E;
float shok_GGL_CPlayerAttractionHandler::GetLeaderPaydayCost()
{
	return shok_playerattractionhandler_getplayerleaderpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumleaders)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall* const)(shok_GGL_CPlayerAttractionHandler*))0x4C1B28;
int shok_GGL_CPlayerAttractionHandler::GetNumberOfLeaders()
{
	return shok_playerattractionhandler_getnumleaders(this);
}

void (*shok_GGL_CPlayerAttractionHandler_OnCheckPayDay)(shok_GGL_CPlayerAttractionHandler* th) = nullptr;
void __fastcall hookedcheckpayday_call(shok_GGL_CPlayerAttractionHandler* th) {
	if (shok_GGL_CPlayerAttractionHandler_OnCheckPayDay)
		shok_GGL_CPlayerAttractionHandler_OnCheckPayDay(th);
}
void __declspec(naked) hookedcheckpayday() {
	__asm {
		mov ecx, esi
		call hookedcheckpayday_call

		mov esi, [esi + 4]
		mov[ebp + 0x18], 0x1300E
		push 0x4C275E
		ret
	}
}
void shok_GGL_CPlayerAttractionHandler_HookCheckPayday()
{
	if (HasSCELoader())
		DEBUGGER_BREAK;
	WriteJump((void*)0x4C2754, &hookedcheckpayday);
}

static inline int(__thiscall* const upgrademanager_getucatbybuilding)(shok_GGL_CBuildingUpgradeManager* th, int id) = (int(__thiscall*)(shok_GGL_CBuildingUpgradeManager*, int))0x4B3CA6;
int shok_GGL_CBuildingUpgradeManager::GetUpgradeCategoryOfBuildingType(int etype)
{
	return upgrademanager_getucatbybuilding(this, etype);
}

static inline int(__thiscall* const settlerupmanager_getstybyucat)(shok_GGL_CSettlerUpgradeManager* th, int ucat) = (int(__thiscall*)(shok_GGL_CSettlerUpgradeManager*, int))0x4B3280;
int shok_GGL_CSettlerUpgradeManager::GetSettlerTypeByUCat(int ucat)
{
	return settlerupmanager_getstybyucat(this, ucat);
}

static inline int(__thiscall* const shok_GGL_CPlayerStatus_getDiploState)(int* d, int p) = (int(__thiscall*)(int* d, int p)) 0x4B4D5B;
int shok_GGL_CPlayerStatus::GetDiploStateTo(int p)
{
	return shok_GGL_CPlayerStatus_getDiploState(DiplomacyData, p);
}

static inline int(__thiscall* const playerstatus_gettechstatus)(shok_GGL_CPlayerStatus_techData* th, int tech) = (int(__thiscall*)(shok_GGL_CPlayerStatus_techData*, int))0x4A2A8D;
int shok_GGL_CPlayerStatus::GetTechStatus(int tech)
{
	return playerstatus_gettechstatus(&TechnologyStates, tech);
}


bool ArePlayersHostile(int p1, int p2)
{
	return (*shok_GGL_CGLGameLogicObj)->GetPlayer(p1)->GetDiploStateTo(p2) == shok_DiploState::Hostile;
}


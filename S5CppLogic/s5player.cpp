#include "pch.h"
#include "s5data.h"

static inline int(__thiscall* plattracthandlerGetAttLimit)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*))0x4C216F;
int shok_GGL_CPlayerAttractionHandler::GetAttractionLimit()
{
	return plattracthandlerGetAttLimit(this);
}

static inline int(__thiscall* plattracthandlerGetAttUsage)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*))0x4C278B;
int shok_GGL_CPlayerAttractionHandler::GetAttractionUsage()
{
	return plattracthandlerGetAttUsage(this);
}

static inline int(__thiscall* upgrademanager_getucatbybuilding)(shok_GGL_CBuildingUpgradeManager* th, int id) = (int(__thiscall*)(shok_GGL_CBuildingUpgradeManager*, int))0x4B3CA6;
int shok_GGL_CBuildingUpgradeManager::GetUpgradeCategoryOfBuildingType(int etype)
{
	return upgrademanager_getucatbybuilding(this, etype);
}

static inline int(__thiscall* settlerupmanager_getstybyucat)(shok_GGL_CSettlerUpgradeManager* th, int ucat) = (int(__thiscall*)(shok_GGL_CSettlerUpgradeManager*, int))0x4B3280;
int shok_GGL_CSettlerUpgradeManager::GetSettlerTypeByUCat(int ucat)
{
	return settlerupmanager_getstybyucat(this, ucat);
}

static inline int(__thiscall* shok_GGL_CPlayerStatus_getDiploState)(int* d, int p) = (int(__thiscall*)(int* d, int p)) 0x4B4D5B;
int shok_GGL_CPlayerStatus::GetDiploStateTo(int p)
{
	return shok_GGL_CPlayerStatus_getDiploState(&DiploData, p);
}

static inline int(__thiscall* playerstatus_gettechstatus)(shok_GGL_CPlayerStatus_techData* th, int tech) = (int(__thiscall*)(shok_GGL_CPlayerStatus_techData*, int))0x4A2A8D;
int shok_GGL_CPlayerStatus::GetTechStatus(int tech)
{
	return playerstatus_gettechstatus(&TechData, tech);
}


bool ArePlayersHostile(int p1, int p2)
{
	return (*shok_GGL_CGLGameLogicObj)->GetPlayer(p1)->GetDiploStateTo(p2) == shok_DIPLOSTATE_HOSTILE;
}


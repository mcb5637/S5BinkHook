#pragma once
#include "s5data.h"

struct shok_GGL_CPlayerAttractionHandler : shok_object {
	PADDINGI(1)
		byte PaydayStarted;
	PADDING(3)
		int PaydayStartTick;

	int GetAttractionLimit();
	int GetAttractionUsage();
};
#define shok_vtp_GGL_CBuildingUpgradeManager (void*)0x772948
struct shok_GGL_CBuildingUpgradeManager : shok_object {

	int GetUpgradeCategoryOfBuildingType(int etype);
};
struct shok_GGL_CSettlerUpgradeManager_UCatEntry {
	int UCat;
	int NumUpgrades;
	int FirstEntityType;
};
#define shok_vtp_GGL_CSettlerUpgradeManager (void*)0x772904
struct shok_GGL_CSettlerUpgradeManager : shok_object {
	PADDINGI(3)
		shok_set<shok_GGL_CSettlerUpgradeManager_UCatEntry> UpgradeCategories;

	int GetSettlerTypeByUCat(int ucat);
};

#define shok_vtp_GGL_CPlayerStatus (void*)0x76FA88
struct shok_GGL_CPlayerStatus : shok_object {
	int PlayerID;
	PADDINGI(21)
	shok_costInfo CurrentResources; // 23
	PADDINGI(1)
	int TaxLevel; // 42
	PADDINGI(3)
	byte AlarmActive;
	PADDING(3)
	int AlarmRechargeTime;
	int DiploData; // 48
	PADDINGI(33)
	int NumberOfSettlersKilled, NumberOfSettlersLost, NumberOfBuildingsKilled, NumberOfBuildingsLost;
	PADDINGI(111)
	shok_GGL_CPlayerAttractionHandler* PlayerAttractionHandler; // 197
	shok_GGL_CBuildingUpgradeManager* BuildingUpgradeManager;
	shok_GGL_CSettlerUpgradeManager* SettlerUpgradeManager;

	int GetDiploStateTo(int p);
};

static inline bool(_cdecl* shok_canPlaceBuilding)(int entitytype, int player, shok_position* pos, float rotation, int buildOnId) = (bool(_cdecl*)(int entitytype, int player, shok_position * pos, float rotation, int buildOnId)) 0x4B442C;


bool ArePlayersHostile(int p1, int p2);

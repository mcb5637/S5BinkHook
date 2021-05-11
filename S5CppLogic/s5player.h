#pragma once
#include "s5data.h"

struct shok_GGL_CPlayerAttractionHandler : shok_object {
	PADDINGI(1);
	byte PaydayStarted;
	PADDING(3);
	int PaydayStartTick;

	int GetAttractionLimit();
	int GetAttractionUsage();
};
struct shok_GGL_CBuildingUpgradeManager : shok_object {

	static inline constexpr int vtp = 0x772948;

	int GetUpgradeCategoryOfBuildingType(int etype);
};
struct shok_GGL_CSettlerUpgradeManager_UCatEntry {
	int UCat;
	int NumUpgrades;
	int FirstEntityType;
};
struct shok_GGL_CSettlerUpgradeManager : shok_object {
	PADDINGI(3);
	shok_set<shok_GGL_CSettlerUpgradeManager_UCatEntry> UpgradeCategories;

	static inline constexpr int vtp = 0x772904;

	int GetSettlerTypeByUCat(int ucat);
};

struct shok_GGL_CPlayerStatus_techData_tech {
	int TechStatus;
	int ResearchProgress;
	int StartTick;
	int ResearcherId;
};
struct shok_GGL_CPlayerStatus_techData : shok_object {
	vector_padding
	std::vector<shok_GGL_CPlayerStatus_techData_tech, shok_allocator<shok_GGL_CPlayerStatus_techData_tech>> TechList;
};
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
	PADDINGI(10)
	shok_GGL_CPlayerStatus_techData TechData; // 59
	PADDINGI(18)
	int NumberOfSettlersKilled, NumberOfSettlersLost, NumberOfBuildingsKilled, NumberOfBuildingsLost;
	PADDINGI(111)
	shok_GGL_CPlayerAttractionHandler* PlayerAttractionHandler; // 197
	shok_GGL_CBuildingUpgradeManager* BuildingUpgradeManager;
	shok_GGL_CSettlerUpgradeManager* SettlerUpgradeManager;

	static inline constexpr int vtp = 0x76FA88;

	int GetDiploStateTo(int p);
	int GetTechStatus(int tech);
};

static inline bool(_cdecl* const shok_canPlaceBuilding)(int entitytype, int player, shok_position* pos, float rotation, int buildOnId) = (bool(_cdecl*)(int entitytype, int player, shok_position * pos, float rotation, int buildOnId)) 0x4B442C;


bool ArePlayersHostile(int p1, int p2);

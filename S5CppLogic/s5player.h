#pragma once
#include "s5data.h"

struct shok_GGL_CSerfManager : shok_object {
	// seems to be just playerid and 2 unknown values

	static inline constexpr int vtp = 0x76EA2C;
};
struct shok_GGL_CPlayerAttractionHandler_entityinsystem {
	int EntityID, GameTurn;
};
struct shok_GGL_CPlayerAttractionHandler_typethatleft {
	int EntityType, GameTurn;
};
struct shok_GGL_CPlayerAttractionHandler_typecount {
	int EntityType, Count;
};
struct shok_GGL_CPlayerAttractionHandler : shok_object {
	int PlayerID;
	byte PaydayActive;
	PADDING(3);
	int PaydayFirstOccuraceGameTurn;
	byte AIPlayerFlag, PayLeaderFlag;
	PADDING(2);
	PADDINGI(2); // maybe char pointer? GUI_WorkersWithoutSleepPlace m_GUI_WorkersWithoutEatPlace
	vector_padding;
	std::vector<shok_GGL_CPlayerAttractionHandler_entityinsystem, shok_allocator<shok_GGL_CPlayerAttractionHandler_entityinsystem>> EntityInSystem;
	vector_padding;
	std::vector<int, shok_allocator<int>> HeadquarterArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> VillageCenterArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> WorkBuildingsArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> ResidenceBuildingArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> FarmBuildingArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> BarrackBuildingArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> FreeWorkerArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> EmployedWorkerArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> SoldierArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> LeaderArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> HeroArray;
	vector_padding;
	std::vector<int, shok_allocator<int>> SerfArray;
	vector_padding;
	std::vector<shok_GGL_CPlayerAttractionHandler_typethatleft, shok_allocator<shok_GGL_CPlayerAttractionHandler_typethatleft>> EntityTypeThatLeft;
	vector_padding;
	std::vector<shok_GGL_CPlayerAttractionHandler_typecount, shok_allocator<shok_GGL_CPlayerAttractionHandler_typecount>> EntityTypeCountMap;

	static inline constexpr int vtp = 0x770868;

	int GetAttractionLimit();
	int GetAttractionUsage();
	int GetWorkerPaydayIncome();
	int GetNumberOfAttractedWorkers();
	float GetLeaderPaydayCost();
	int GetNumberOfLeaders();

	// checkpayday 4C25FB thiscall
	static void HookCheckPayday();
	static void (*OnCheckPayDayCallback)(shok_GGL_CPlayerAttractionHandler* th);
};
//constexpr int i = offsetof(shok_GGL_CPlayerAttractionHandler, WorkBuildingsArray) / 4;

struct shok_GGL_CUpgradeManager_jobdata {
	int Category;
	shok_set<int[3]> UpgradeJobData; // UpgradeProgress, Category, UpgradeManager
};
struct shok_GGL_CUpgradeManager_UCatEntry {
	int UCat;
	int NumUpgrades;
	int FirstEntityType;
};
struct shok_GGL_CUpgradeManager : shok_object {
	shok_set<shok_GGL_CUpgradeManager_jobdata> JobData; // empty
	shok_set<shok_GGL_CUpgradeManager_UCatEntry> UpgradeCategories;
	int PlayerID;

	static inline constexpr int vtp = 0x7728CC;

	int GetUpgradeCategoryOfEntityType(int etype);
};
struct shok_GGL_CBuildingUpgradeManager : shok_GGL_CUpgradeManager {
	shok_set<int[3]> ScholarInfoElement; // Category, ScholarInfo { ?, CurrentAmount } ?

	static inline constexpr int vtp = 0x772948;
};
struct shok_GGL_CSettlerUpgradeManager : shok_GGL_CUpgradeManager {
	

	static inline constexpr int vtp = 0x772904;

	int GetSettlerTypeByUCat(int ucat);
};
struct shok_GGL_CTradeManager_data {
	int ResourceType;
	float CurrentPrice, CurrentInflation, CurrentDeflation;
	byte CanBeSold, CanBeBought;
};
struct shok_GGL_CTradeManager : shok_object {
	int PlayerID;
	vector_padding;
	std::vector<shok_GGL_CTradeManager_data, shok_allocator<shok_GGL_CTradeManager_data>> TradeData;

	static inline constexpr int vtp = 0x772860;
};

struct shok_GGL_CPlayerStatus_techData_tech {
	int TechStatus;
	int ResearchProgress;
	int StartTick;
	int ResearcherId;
};
struct shok_GGL_CPlayerStatus_techData { // size 12
	int PlayerID;
	vector_padding;
	std::vector<shok_GGL_CPlayerStatus_techData_tech, shok_allocator<shok_GGL_CPlayerStatus_techData_tech>> TechnologyState;
	PADDINGI(4); // vector<?> TechnologyInProcess
	PADDINGI(3);
};
static_assert(sizeof(shok_GGL_CPlayerStatus_techData) == 12 * 4);
struct shok_GGL_CPlayerStatus_tributeData_tribute {
	int UniqueTributeID;
	shok_costInfo Costs;
	int OwnerEntityID;
	int OfferingPlayerID;
	shok_string OfferStringTableKey;
};
struct shok_GGL_CPlayerStatus_tributeData {
	int PlayerID;
	vector_padding;
	std::vector<shok_GGL_CPlayerStatus_tributeData, shok_allocator<shok_GGL_CPlayerStatus_tributeData>> Tributes;
};
static_assert(sizeof(shok_GGL_CPlayerStatus_tributeData) == 5 * 4);
struct shok_GGL_CPlayerStatus_questData_quest {
	int UniqueQuestID;
	int QuestType;
	shok_string QuestNameStringTableKey, QuestDescriptionStringTableKey;
	int SubQuestDoneFlagArray[6]; // type?
};
struct shok_GGL_CPlayerStatus_questData {
	int PlayerID;
	vector_padding;
	std::list< shok_GGL_CPlayerStatus_questData_quest, shok_allocator< shok_GGL_CPlayerStatus_questData_quest>> Quests;
};
struct shok_statisticsTimeline {
	vector_padding;
	std::vector<int, shok_allocator<int>> Amounts;
	int PlayerID;
	int LastGatherTurn;
};
struct shok_GGL_CResourceStatistics_data {
	int ResourceType;
	int AmountMined;
	int AmountRefined;
	shok_statisticsTimeline TimeLine;
};
struct shok_GGL_CResourceStatistics : shok_object {
	vector_padding;
	std::vector<shok_GGL_CResourceStatistics_data, shok_allocator<shok_GGL_CResourceStatistics_data>> Data;
	PADDINGI(1);

	static inline constexpr int vtp = 0x76E0D8;
};
struct shok_statisticsTechResearched {
	int TechnologyType, Time;
};
struct shok_statisticsBuildingUpgraded {
	int BuildingType, Time;
};
struct shok_GGL_CGameStatistics : shok_object {
	int PlayerID;
	int NumberOfUnitsKilled, NumberOfUnitsDied, NumberOfBuildingsDestroyed, NumberOfBuildingsLost;
	shok_GGL_CResourceStatistics ResourceStatistics; //6
	shok_statisticsTimeline UnitsKilledTimeLine, UnitsDiedTimeLine, BuildingsDestroyedTimeLine, BuildingsLostTimeLine, // 12, 18
		SerfTimeLine, WorkerTimeLine, LeaderTimeLine, HeroTimeLine, SoldierTimeLine, FarmTimeLine, ResidenceTimeLine,
		WorkplaceTimeLine, MilitaryBuildingTimeLine, VillageCenterTimeLine, BuildingTimeLine, MotivationTimeLine; // la 102, 96, 90
	vector_padding;
	std::vector<shok_statisticsTechResearched, shok_allocator<shok_statisticsTechResearched>> ResearchedTechnologies;
	vector_padding;
	std::vector<shok_statisticsBuildingUpgraded, shok_allocator<shok_statisticsBuildingUpgraded>> UpgradedBuildings;

	static inline constexpr int vtp = 0x76E0E0;
};
static_assert(sizeof(shok_GGL_CGameStatistics) == 116 * 4);
//constexpr int i = offsetof(shok_GGL_CGameStatistics, VillageCenterTimeLine) / 4;
struct shok_GGL_CPlayerStatus : shok_object {
	int PlayerGameState;
	byte PlayerHasLeftGameFlag;
	PADDING(3);
	int PlayerGameStateChangeGameTurn;
	byte PlayerIsHumanFlag;
	PADDING(3);
	int PlayerColorR, PlayerColorG, PlayerColorB;
	int PlayerID;
	shok_string PlayerNameStringTableKey, PlayerNameStringRaw;
	shok_costInfo CurrentResources; // 23
	float TaxAmountFactor;
	int TaxLevel; // 42
	float CurrentMaxMotivation;
	int DurationOfWeatherChange; // seems to be always 0
	int NumberOfBuyableHeros;
	byte WorkerAlarmMode;
	PADDING(3);
	int AlarmRechargeTime;
	int DiplomacyData[11]; // 48
	shok_GGL_CPlayerStatus_techData TechnologyStates; // 59
	shok_GGL_CPlayerStatus_tributeData Tributes; // 71
	shok_GGL_CPlayerStatus_questData Quests; // 76
	shok_GGL_CGameStatistics Statistics; // 80
	shok_GGL_CSerfManager* SerfManager;
	shok_GGL_CPlayerAttractionHandler* PlayerAttractionHandler; // 197
	shok_GGL_CBuildingUpgradeManager* BuildingUpgradeManager;
	shok_GGL_CSettlerUpgradeManager* SettlerUpgradeManager;
	shok_GGL_CTradeManager* TradeManager;

	static inline constexpr int vtp = 0x76FA88;

	shok_DiploState GetDiploStateTo(int p);
	shok_TechState GetTechStatus(int tech);
};
//constexpr int i = offsetof(shok_GGL_CPlayerStatus, Statistics) / 4;

static inline bool(_cdecl* const shok_canPlaceBuilding)(int entitytype, int player, shok_position* pos, float rotation, int buildOnId) = reinterpret_cast<bool(_cdecl*)(int, int, shok_position*, float, int)>(0x4B442C);
static inline bool(_cdecl* const shok_canPlaceBuildingAtPos)(int entitytype, int player, shok_position* pos, float rotation) = reinterpret_cast<bool(_cdecl*)(int, int, shok_position *, float)>(0x4B45C8);


bool ArePlayersHostile(int p1, int p2);
bool ArePlayersFriendly(int p1, int p2);

extern bool (*CanPlaceBuildingCallback)(int entitytype, int player, shok_position* pos, float rotation, int buildOnId);
void HookCanPlaceBuilding();

extern float ConstructBuildingRotation;
void HookConstructCommandRotation();

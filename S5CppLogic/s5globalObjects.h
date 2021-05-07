#pragma once
#include "s5data.h"

#define shok_vtp_EGL_CGLEEntitiesProps (void*)0x788834
struct shok_EGL_CGLEEntitiesProps : shok_object {
private:
	int u[14];
public:
	vector_padding;
	std::vector<shok_GGlue_CGlueEntityProps, shok_allocator<shok_GGlue_CGlueEntityProps>> EntityTypes;

	shok_GGlue_CGlueEntityProps* GetEntityType(int i);
};
static inline shok_EGL_CGLEEntitiesProps** shok_EGL_CGLEEntitiesPropsObj = (shok_EGL_CGLEEntitiesProps**)0x895DB0;

// c manager
struct shok_ECS_CManager : shok_object {
	void ReloadCutscene(const char* path);
};
static inline shok_ECS_CManager*** shok_ECS_CManagerObject = (shok_ECS_CManager***)0x0A0344C;


// effect manager
struct shok_EGL_CGLEEffectManager : shok_object {
	bool IsEffectValid(int id);
	shok_EGL_CEffect* GetEffectById(int id);
};
static inline shok_EGL_CGLEEffectManager** shok_EGL_CGLEEffectManagerObject = (shok_EGL_CGLEEffectManager**)0x898144;

struct shok_BB_CIDManagerEx : shok_object {
	int GetAnimIdByName(const char* name);
};
static inline shok_BB_CIDManagerEx** shok_BB_CIDManagerExObj = (shok_BB_CIDManagerEx**)0xA0C838;


#define shok_vtp_EGL_CTerrainVertexColors (void*)0x7841F8
struct shok_EGL_CTerrainVertexColors : shok_object {
	int ArraySizeX, ArraySizeY;
	int* VertexColors;

	void ToTerrainCoord(shok_position& p, int* out);
	bool IsCoordValid(int* out);
	int GetTerrainVertexColor(shok_position& p);
	// set 580582 (this, x, y, *col) terraincoord
};
#define shok_vtp_EGL_CGLETerrainHiRes (void*)0x7837B0
struct shok_EGL_CGLETerrainHiRes : shok_object {
	vector_padding;
	std::vector<int16_t, shok_allocator<int16_t>> TerrainHeights;
	int MaxSizeX, MaxSizeY; // 5
	int ArraySizeX, ArraySizeY; // 7

	void ToTerrainCoord(shok_position& p, int* out);
	bool IsCoordValid(int* out);
	int GetTerrainHeight(shok_position& p);
	// set 591B53 (this, *out, int16 h)
};
#define shok_vtp_EGL_CGLETerrainLowRes (void*)0x7837C0
struct shok_EGL_CGLETerrainLowRes : shok_object {
	vector_padding
	std::vector<int, shok_allocator<int>> Data; // terrain type &0xFF, water type &0x3F00 >>8, water height &0x3FFFC000 >>14
	PADDINGI(4); // 1 vectors of int
	int MaxSizeX, MaxSizeY; // 9
	int ArraySizeX, ArraySizeY; // 11

	void ToQuadCoord(shok_position& p, int* out);
	bool IsCoordValid(int* out);
	int GetTerrainTypeAt(shok_position& p);
	int GetWaterTypeAt(shok_position& p);
	int GetWaterHeightAt(shok_position& p);
	// set water height 591B82 (this, *out, int)
	// set water type 591BB9 (this, *out, int)
	// set terrain type 58BBCE (this, *out, int)
};
#define shok_vtp_EGL_CGLELandscape (void*)0x783C38
struct shok_EGL_CGLELandscape : shok_object {
	PADDINGI(6);
	shok_EGL_CGLETerrainHiRes* HiRes;
	shok_EGL_CGLETerrainLowRes* LowRes;
	shok_EGL_CTerrainVertexColors* VertexColors;

	int GetSector(shok_position* p);
	bool GetNearestPositionInSector(shok_position* pIn, float range, int sector, shok_position* pOut);
};

// game logic
#define shok_vtp_EGL_CGLEGameLogic (void*)0x7839CC
struct shok_EGL_CGLEGameLogic : shok_object {
	PADDINGI(6);
	int* InGameTime; // 7
	PADDINGI(1);
	shok_EGL_CGLELandscape* Landscape; // 9

	int CreateEffect(shok_EGL_CGLEEffectCreator* data);
	int CreateEntity(shok_EGL_CGLEEntityCreator* cr);
	int GetTimeMS();
	int GetTick();

	void HookCreateEffect();
};
static inline shok_EGL_CGLEGameLogic** shok_EGL_CGLEGameLogicObject = (shok_EGL_CGLEGameLogic**)0x895DAC;
extern void(*CreateEffectHookCallback)(int id, void* ret);
constexpr int CreatEffectReturnBattleBehaviorAttack = 0x50C4B5;
constexpr int CreatEffectReturnAutoCannonBehaviorAttack = 0x5107a8;
constexpr int CreatEffectReturnCannonBallOnHit = 0x4ff55e;

static inline int* shok_mapsize = (int*)0x898B74;

#define shok_vtp_EGL_CRegionInfo (void*)0x783878
struct shok_EGL_CRegionInfo : shok_object {

};
#define shok_vtp_ED_CGlobalsLogicEx (void*)0x769F74
struct shok_ED_CGlobalsLogicEx : shok_object {
	PADDINGI(5);
	struct {
		int ArraySizeXY;
		byte* data;
	}* Blocking; // 6
	PADDINGI(2);
	shok_EGL_CRegionInfo* RegionInfo; // 9

	void ToTerrainCoord(shok_position& p, int* out);
	bool IsCoordValid(int* out);
	int GetBlocking(shok_position& p);
};
static inline shok_ED_CGlobalsLogicEx** shok_ED_CGlobalsLogicExObj = (shok_ED_CGlobalsLogicEx**)0x8581EC;

// entity manager
struct shok_EGL_CGLEEntityManager : shok_object {
public:
	int EntityCount;
private:
	int u[4];
	struct {
	private:
		int u;
	public:
		shok_EGL_CGLEEntity* entity;
	} Entities[1];

public:
	shok_EGL_CGLEEntity* GetEntityByNum(int num);
};
static inline shok_EGL_CGLEEntityManager** shok_EGL_CGLEEntityManagerObj = (shok_EGL_CGLEEntityManager**)0x897558;

#define shok_vtp_GGL_CDamageClassProps (void*)0x788978
struct shok_GGL_CDamageClassProps : shok_object {
	float BonusVsArmorClass[7];
};
struct shok_damageClassHolder {
	PADDINGI(1);
	vector_padding;
	std::vector<shok_GGL_CDamageClassProps*, shok_allocator<shok_GGL_CDamageClassProps*>> DamageClassList; // there is a damageclass 0, probably not working at all
};
static inline shok_damageClassHolder** shok_DamageClassHolderObj = (shok_damageClassHolder**)0x85A3DC;


#define shok_vtp_GGL_CLogicProperties_SBuildingUpgradeCategory (void*)0x76EF10
struct shok_GGL_CLogicProperties_SBuildingUpgradeCategory : shok_object {
	int Category, FirstBuilding;
	PADDINGI(1)
};
#define shok_vtp_GGL_CLogicProperties_SSettlerUpgradeCategory (void*)0x76EF18
struct shok_GGL_CLogicProperties_SSettlerUpgradeCategory : shok_object {
	int Category, FirstSettler;
};
#define shok_vtp_GGL_CLogicProperties_STaxationLevel (void*)0x76EF20
struct shok_GGL_CLogicProperties_STaxationLevel : shok_object {
	int RegularTax;
	float MotivationChange;
};
#define shok_vtp_GGL_CLogicProperties_STradeResource (void*)0x76EF28
struct shok_GGL_CLogicProperties_STradeResource : shok_object {
	int ResourceType;
	float BasePrice, MinPrice, MaxPrice, Inflation, Deflation, WorkAmount;
};
#define shok_vtp_GGL_CLogicProperties_SBlessCategory (void*)0x76EFC4
struct shok_GGL_CLogicProperties_SBlessCategory : shok_object {
	int Name;
	float RequiredFaith;
	vector_padding;
	std::vector<int, shok_allocator<int>> EntityTypes;
};
#define shok_vtp_GGL_CLogicProperties (void*)0x76EFCC
struct shok_GGL_CLogicProperties : shok_object {
	int CompensationOnBuildingSale;
	vector_padding;
	std::vector<shok_GGL_CLogicProperties_SBuildingUpgradeCategory, shok_allocator<shok_GGL_CLogicProperties_SBuildingUpgradeCategory>> BuildingUpgrades;
	vector_padding;
	std::vector<shok_GGL_CLogicProperties_SSettlerUpgradeCategory, shok_allocator<shok_GGL_CLogicProperties_SSettlerUpgradeCategory>> SettlerUpgrades;
	vector_padding;
	std::vector<shok_GGL_CLogicProperties_STaxationLevel, shok_allocator<shok_GGL_CLogicProperties_STaxationLevel>> TaxationLevels;
	vector_padding;
	std::vector<shok_GGL_CLogicProperties_STradeResource, shok_allocator<shok_GGL_CLogicProperties_STradeResource>> TradeResources;
	vector_padding;
	std::vector<shok_GGL_CLogicProperties_SBlessCategory, shok_allocator<shok_GGL_CLogicProperties_SBlessCategory>> BlessCategories; // 18
	float BuildingUnderConstructionExplorationFactor, BuildingPlacementSnapDistance, BuildingClosedHealthFactor, WeatherExplorationBuildingSnowFactor, WeatherExplorationSettlerSnowFactor;
	float WeatherMoveSpeedSnowFactor, WeatherExplorationBuildingRainFactor, WeatherExplorationSettlerRainFactor, WeatherMoveSpeedRainFactor;
	int WeatherMissChanceChangeRain, WeatherMissChanceChangeSnow;
	float AttackMoveRange; // 33
	int TaxAmount;
	float TaxPenalty;
	int InitialTaxLevel; // 36
	shok_costInfo BlessingCost;
	float BlessingBonus; // 55
	PADDINGI(1);
	int ForceToWorkPenalty;
	PADDINGI(1);
	int ResourceDoodadWarnAmount; // 59
	int FeedbackHandlerUpdateFrequency;
	int FeedbackHandlerTimeStaysInSystem;
	float MilitaryLeaderAutoAttackRange;
	float MilitaryBuildingAutoAttackRangeFactor;
	float MilitaryCivilianAutoAttackRangeFactor;
	float MilitaryDefendRange;
	int MilitaryRecentlyAttackedTime;
	int MilitaryMaxAttackersPerEntity;
	float CivilianDefenseRadius;
	float MotivationThresholdHappy; // 69
	float MotivationThresholdSad, MotivationThresholdAngry, MotivationThresholdLeave, AverageMotivationVillageCenterLockThreshold;
	int WorkTimeBase, WorkTimeThresholdWork, WorkTimeThresholdFarm, WorkTimeThresholdResidence, WorkTimeThresholdCampFire;
	float MotivationGameStartMaxMotivation;
	float MotivationAbsoluteMaxMotivation; // 80
	int MotivationMillisecondsWithoutJob;
	int HeroResurrectionTime, HeroResurrectionSlippage;
	float HeroResurrectionRadius, HeroResurrectionHealthFactor, HeroResurrectionActionPointFactor;
	int HeroComatoseExploration;
	int BattleSerfMaxSeconds;
	int ExpelEffectID, DefenderMSPerShot; // fi 89
	float DefenderMaxRange;
	int DefenderProjectileEffectType;
	int DefenderProjectileDamage;
	int DefenderProjectileDamageClass;
	int DefenderMissChance; // 95
	float WorkerFlightDistance;
	int MaxExperiencePoints;
	int BuildingRecentlyAttackedDuration;
	float EnergyRequiredForWeatherChange; // 99
	int DefaultDurationOfWeatherChange;
	float EnergyDeductedFromOtherPlayers;
	float ConstructionSiteHealthFactor;
	float MaxFaith; // 103
	float GuardMaxDistanceBuilding;
	float GuardMoveDistanceBuilding;
	float GuardMaxDistanceOther;
	float GuardMoveDistanceOther;
	float NPCInteractionDistance;
	int WeatherTowerAnim;
	int LeaderNudgeCount;
	float LeaderApproachRange;
	int AlarmRechargeTime;
	int OvertimeRechargeTimeInMs; // 113
};
static inline shok_GGL_CLogicProperties** shok_GGL_CLogicPropertiesObj = (shok_GGL_CLogicProperties**)0x85A3E0;

struct shok_GGL_CPlayerAttractionProps : shok_object {
	int AttractionFrequency, PaydayFrequency, EntityTypeBanTime, ReAttachWorkerFrequency, PlayerMoneyDispo;
	float MaximumDistanceWorkerToFarm, MaximumDistanceWorkerToResidence;
};
static inline shok_GGL_CPlayerAttractionProps** shok_GGL_CPlayerAttractionPropsObj = (shok_GGL_CPlayerAttractionProps**)0x866A80;

struct shok_BB_IPostEvent : shok_object {

};
#define shok_vtp_GGUI_CManager (void*)0x77B2F8
struct shok_GGUI_CManager : shok_object {
	PADDINGI(9)
private:
	shok_BB_IPostEvent* PostEvent;

public:
	void HackPostEvent();
};
static inline shok_GGUI_CManager* (*shok_GetGuiManager)() = (shok_GGUI_CManager * (*)()) 0x525622;
extern bool(*PostEventCallback)(shok_BB_CEvent* ev);

struct shok_GGL_CGLGameLogic_TechList {
	vector_padding
	std::vector<shok_technology*, shok_allocator<shok_technology*>> TechList;
};

struct shok_GGL_CWeatherHandler : shok_object {
	PADDINGI(12)
	int WeatherChangeActive;

	int GetNextWeatherState();
	int GetCurrentWeatherState();
	int GetTicksToNextPeriodicWeatherChange();
};

// gamelogic
#define shok_vtp_GGL_CGLGameLogic (void*)0x76E018
struct shok_GGL_CGLGameLogic : shok_object {
	PADDINGI(9)
public:
	shok_GGL_CPlayerStatus** players; // 10
	shok_GGL_CWeatherHandler* WeatherHandler;
	PADDINGI(1)
private:
	shok_GGL_CGLGameLogic_TechList* TechList;
public:

	shok_GGL_CPlayerStatus* GetPlayer(int i);
	shok_technology* GetTech(int i);
	void EnableAlarmForPlayer(int pl);
	void DisableAlarmForPlayer(int pl);
	void UpgradeSettlerCategory(int pl, int ucat);
	void PlayerActivateWeathermachine(int player, int weathertype);
	void PlayerBlessSettlers(int player, int blessCat);
};
static inline shok_GGL_CGLGameLogic** shok_GGL_CGLGameLogicObj = (shok_GGL_CGLGameLogic**)0x85A3A0;


#define shok_vtp_EScr_CScriptTriggerSystem (void*)0x78667C
struct shok_EScr_CScriptTriggerSystem : shok_object {
private:
	shok_BB_IPostEvent PostEvent;

public:
	void RunTrigger(shok_BB_CEvent* ev);
};
static inline shok_EScr_CScriptTriggerSystem** shok_EScr_CScriptTriggerSystemObj = (shok_EScr_CScriptTriggerSystem**)0x895DEC;

struct shok_BB_IFileSystem : shok_object {

};
#define shok_vtp_BB_CDirectoryFileSystem (void*)0x7803B4
struct shok_BB_CDirectoryFileSystem : shok_BB_IFileSystem {
	char* Path;
};
#define shok_vtp_BB_CBBArchiveFile (void*)0x77FABC
struct shok_BB_CBBArchiveFile : shok_BB_IFileSystem {
	PADDINGI(2) // BB::CFileStream 761C98, then probably file handle
	char* Path;
};
#define shok_vtp_BB_CFileSystemMgr (void*)0x77F794
struct shok_BB_CFileSystemMgr : shok_object {
	vector_padding
	std::vector<shok_BB_IFileSystem*, shok_allocator<shok_BB_IFileSystem*>> LoadOrder;

	void AddFolder(const char* path);
	void AddArchive(const char* path);
	void RemoveTopArchive();
};
static inline shok_BB_CFileSystemMgr** shok_BB_CFileSystemMgrObj = (shok_BB_CFileSystemMgr**)0x88F088;

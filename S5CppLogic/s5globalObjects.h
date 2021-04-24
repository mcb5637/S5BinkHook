#pragma once
#include "s5data.h"

#define shok_vtp_EGL_CGLEEntitiesProps (void*)0x788834
struct shok_EGL_CGLEEntitiesProps : shok_object {
private:
	int u[14];
public:
	vector_padding
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

struct shok_EGL_CGLELandscape : shok_object {


	int GetSector(shok_position* p);
	bool GetNearestPositionInSector(shok_position* pIn, float range, int sector, shok_position* pOut);
};

// game logic
#define shok_vtp_EGL_CGLEGameLogic (void*)0x7839CC
struct shok_EGL_CGLEGameLogic : shok_object {
	PADDINGI(6)
	int* InGameTime; // 7
	PADDINGI(1)
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
	PADDINGI(1)
		vector_padding
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
	vector_padding
		std::vector<int, shok_allocator<int>> EntityTypes;
};
#define shok_vtp_GGL_CLogicProperties (void*)0x76EFCC
struct shok_GGL_CLogicProperties : shok_object {
	int CompensationOnBuildingSale;
	vector_padding
	std::vector<shok_GGL_CLogicProperties_SBuildingUpgradeCategory, shok_allocator<shok_GGL_CLogicProperties_SBuildingUpgradeCategory>> BuildingUpgrades;
	vector_padding
	std::vector<shok_GGL_CLogicProperties_SSettlerUpgradeCategory, shok_allocator<shok_GGL_CLogicProperties_SSettlerUpgradeCategory>> SettlerUpgrades;
	vector_padding
	std::vector<shok_GGL_CLogicProperties_STaxationLevel, shok_allocator<shok_GGL_CLogicProperties_STaxationLevel>> TaxationLevels;
	vector_padding
	std::vector<shok_GGL_CLogicProperties_STradeResource, shok_allocator<shok_GGL_CLogicProperties_STradeResource>> TradeResources;
	vector_padding
	std::vector<shok_GGL_CLogicProperties_SBlessCategory, shok_allocator<shok_GGL_CLogicProperties_SBlessCategory>> BlessCategories;
	float BuildingUnderConstructionExplorationFactor, BuildingPlacementSnapDistance, BuildingClosedHealthFactor, WeatherExplorationBuildingSnowFactor, WeatherExplorationSettlerSnowFactor;
	float WeatherMoveSpeedSnowFactor, WeatherExplorationBuildingRainFactor, WeatherExplorationSettlerRainFactor, WeatherMoveSpeedRainFactor;
	int WeatherMissChanceChangeRain, WeatherMissChanceChangeSnow;
	float AttackMoveRange; // 33
	PADDINGI(21)
		float BlessingBonus; // 55
	PADDINGI(3)
		int ResourceDoodadWarnAmount; // 59
	PADDINGI(9)
		float MotivationThresholdHappy; // 69
	float MotivationThresholdSad, MotivationThresholdAngry, MotivationThresholdLeave, AverageMotivationVillageCenterLockThreshold;
	int WorkTimeBase, WorkTimeThresholdWork; // la 75
	PADDINGI(4)
		float MotivationAbsoluteMaxMotivation;
	PADDINGI(1)
		int HeroResurrectionTime, HeroResurrectionSlippage;
	PADDINGI(5)
		int ExpelEffectID, DefenderMSPerShot; // fi 89
	float DefenderMaxRange;
	PADDINGI(1)
		int DefenderProjectileDamage;
	PADDINGI(1)
		int DefenderMissChance; // 95
	PADDINGI(3)
		float EnergyRequiredForWeatherChange; // 99
	PADDINGI(3)
		float MaxFaith; // 103
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

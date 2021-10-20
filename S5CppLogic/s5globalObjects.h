#pragma once
#include "s5data.h"

struct shok_BB_CIDManagerEx_data {
	char* Name;
	unsigned int Hash;
};
struct shok_BB_CIDManagerEx : shok_object {
	static inline constexpr int vtp = 0x77F8C4;
	PADDINGI(1);
	vector_padding;
	std::vector<shok_BB_CIDManagerEx_data, shok_allocator<shok_BB_CIDManagerEx_data>> TypeNames;

	int GetIdByName(const char* name);
	const char* GetNameByID(int id);
	int GetIDByNameOrCreate(const char* name); // throws if id invalid
	int GetIDByNameOrCreate(const char* name, int newid); // sets id id >0, throws otherwise or if id does not match or already used
	void RemoveID(int id); // remove highest id first, cause that way the vector gets shrunk. ids get reused, use this only for cleanup
	void DumpManagerToLuaGlobal(lua_State* L, const char* global);

	static inline shok_BB_CIDManagerEx** const AnimManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0xA0C838);
	static inline shok_BB_CIDManagerEx** const EntityTypeManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x895DC0);
	static inline shok_BB_CIDManagerEx** const UpgradeCategoryManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0xA0C84C);
	static inline shok_BB_CIDManagerEx** const EntityCategoryManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3C4);
	static inline shok_BB_CIDManagerEx** const DamageClassManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3B8);
	static inline shok_BB_CIDManagerEx** const TechnologiesManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3BC);
	static inline shok_BB_CIDManagerEx** const TechnologyCategoryManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3C0);
	static inline shok_BB_CIDManagerEx** const AbilityManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3CC);
	static inline shok_BB_CIDManagerEx** const GoodsManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3B4); // i dont think they are actually used somewhere...
	static inline shok_BB_CIDManagerEx** const ArmorClassManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3C8); // you probably cannot add anything here, cause fixed array
	static inline shok_BB_CIDManagerEx** const AttachmentTypesManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x85A3D0); // 85A3D4 too
	static inline shok_BB_CIDManagerEx** const FeedBackEventManager = reinterpret_cast<shok_BB_CIDManagerEx**>(0x880BA0); // ids are not correct here
};
static_assert(sizeof(shok_BB_CIDManagerEx) == 6 * 4);
struct shok_BB_CIDManager : shok_object {
	PADDINGI(1);
	vector_padding;
	std::vector<shok_BB_CIDManagerEx_data, shok_allocator<shok_BB_CIDManagerEx_data>> TypeNames; // seems to be the same struct

	static inline constexpr int vtp = 0x76C028;

	int GetIDByName(const char* name); // throws if id invalid
	int GetIDByName(const char* name, int newid); // sets id id >0, throws otherwise or if id does not match or already used
};

struct shok_EGL_CGLEEntitiesProps : shok_object {
	PADDINGI(1); // float 240
	shok_BB_CIDManagerEx* EntityTypeManager;
	shok_BB_CIDManagerEx* EntityCategoryManager;
	shok_BB_CIDManagerEx* UpgradeCategoryManager;
	shok_BB_CIDManagerEx* BlessCategoryManager;
	vector_padding;
	std::vector<shok_EGL_CGLEEntityProps*, shok_allocator<shok_EGL_CGLEEntityProps*>> EntityTypesLogicProps; // 6
	shok_BB_CIDManagerEx* EntityTypeManagerAgain;
	vector_padding;
	std::vector<shok_ED_CDisplayEntityProps*, shok_allocator<shok_ED_CDisplayEntityProps*>> EntityTypesDisplayProps; // 11
	vector_padding;
	std::vector<shok_GGlue_CGlueEntityProps, shok_allocator<shok_GGlue_CGlueEntityProps>> EntityTypes; // 15

	static inline constexpr int vtp = 0x788834;

	shok_GGlue_CGlueEntityProps* GetEntityType(int i);

	static inline shok_EGL_CGLEEntitiesProps** const GlobalObj = reinterpret_cast<shok_EGL_CGLEEntitiesProps**>(0x895DB0);

	static const char* GetEntityTypeDisplayName(int i);
};
//constexpr int i = offsetof(shok_EGL_CGLEEntitiesProps, EntityTypesDisplayProps) / 4;

// c manager
struct shok_ECS_CManager : shok_object { // ECS::ICutsceneManager
	static inline constexpr int vtp = 0x7860B4;

	void ReloadCutscene(const char* path);

	static inline shok_ECS_CManager*** const GlobalObj = reinterpret_cast<shok_ECS_CManager***>(0x0A0344C);
};


// effect manager
struct shok_EGL_CGLEEffectManager : shok_object {
	static inline constexpr int vtp = 0x784528;

	bool IsEffectValid(int id);
	shok_EGL_CEffect* GetEffectById(int id);

	static inline shok_EGL_CGLEEffectManager** const GlobalObj = reinterpret_cast<shok_EGL_CGLEEffectManager**>(0x898144);
};


struct shok_EGL_CTerrainVertexColors : shok_object {
	int ArraySizeX, ArraySizeY;
	int* VertexColors;

	static inline constexpr int vtp = 0x7841F8;

	void ToTerrainCoord(shok_position& p, int* out);
	bool IsCoordValid(int* out);
	int GetTerrainVertexColor(shok_position& p);
	void SetTerrainVertexColor(shok_position& p, int col); // a,r,g,b each int8
};
struct shok_EGL_CGLETerrainHiRes : shok_BB_IObject {
	friend struct shok_EGL_CGLELandscape;
	vector_padding;
	std::vector<int16_t, shok_allocator<int16_t>> TerrainHeights;
	int MaxSizeX, MaxSizeY; // 5
	int ArraySizeX, ArraySizeY; // 7

	static inline constexpr int vtp = 0x7837B0;

	static void ToTerrainCoord(const shok_position& p, int* out);
	bool IsCoordValid(int* out);
	bool IsCoordValid(int x, int y);
	int GetTerrainHeight(const shok_position& p);
	void SetTerrainHeight(const shok_position& p, int h); // int16
private:
	int GetTerrainHeight(int x, int y);
};
struct shok_EGL_CGLETerrainLowRes : shok_BB_IObject {
	friend struct shok_EGL_CGLELandscape;
	std::vector<int, shok_allocator<int>> Data; // terrain type &0xFF, water type &0x3F00 >>8, water height &0x3FFFC000 >>14
	vector_padding;
	std::vector<int, shok_allocator<int>> BridgeHeights;
	int MaxSizeX, MaxSizeY; // 9
	int ArraySizeX, ArraySizeY; // 11

	static inline constexpr int vtp = 0x7837C0;

	static void ToQuadCoord(const shok_position& p, int* out);
	bool IsCoordValid(const int* out);
	bool IsCoordValid(int x, int y);
	int GetTerrainTypeAt(const shok_position& p);
	void SetTerrainTypeAt(const shok_position& p, int tty); // byte (int8)
	int GetWaterTypeAt(const shok_position& p);
	void SetWaterTypeAt(const shok_position& p, int wty); // int6
	int GetWaterHeightAt(const shok_position& p);
	void SetWaterHeightAt(const shok_position& p, int wh); // int16
	int GetBridgeHeight(const shok_position& p);
	void SetBridgeHeight(const shok_position& p, int bh);
private:
	inline int* GetBridgeHeightP(int x, int y);
	int GetWaterHeightAt(int x, int y);
};
struct shok_EGL_CTiling : shok_object {

	static inline constexpr int vtp = 0x783BAC;
};
struct shok_EGL_CGLELandscape_blockingData;
struct shok_EGL_CGLELandscape : shok_object {
	enum class BlockingMode : byte {
		None = 0x0,
		Blocked = 0x1,
		BridgeArea = 0x2,
		BuildBlock = 0x4,
		TerrainSlope = 0x8,
	};

	shok_EGL_CGLELandscape_blockingData* BlockingData;
	PADDINGI(1); //  0 array?
	shok_EGL_CTiling* Tiling;
	PADDINGI(3); // set/list of BB::TSlotEx1<EGL::CGLEGameLogic,EGL::C2DVector const &>
	shok_EGL_CGLETerrainHiRes* HiRes;
	shok_EGL_CGLETerrainLowRes* LowRes;
	shok_EGL_CTerrainVertexColors* VertexColors;

	static inline constexpr int vtp = 0x783C38;

	// only works with 90° rotation steps, cause behind the scenes its still an AA rect. maybe fix if needed.
	struct AdvancedAARectIterator {
		struct Coord {
			int x = 0, y = 0;
		};
		Coord Low, High;

		struct Iter {
		private:
			const AdvancedAARectIterator* I;
			Coord Curr;
		public:
			const Coord& operator*() const;
			bool operator==(const Iter& o) const;
			bool operator!=(const Iter& o) const;
			Iter& operator++();
			Iter operator++(int);
			Iter(const AdvancedAARectIterator& i, const Coord& c);
		};

		AdvancedAARectIterator(const shok_position& p, const shok_AARect& area, float rot, bool LowRes);
		bool HasNext(const Coord& Curr) const;
		void ToNext(Coord& Curr) const;
		Iter begin() const;
		Iter end() const;
	};

	int GetSector(const shok_position* p);
	bool GetNearestPositionInSector(const shok_position* pIn, float range, int sector, shok_position* pOut);
	shok_position GetNearestFreePos(const shok_position* p, float range);
	bool IsValidPos(shok_position* p);
	shok_position GetMapSize();
	bool IsPosBlockedInMode(const shok_position* p, BlockingMode mode);
	BlockingMode GetBlocking(const shok_position& p);
	void FlattenPosForBuilding(const shok_position& p, const shok_AARect& area, float rot);
	// block for vector of aarect: thiscall 577B07 (this, pos*, vector<aarect>*, float, byte*)
	// unblock for vector of aarect: thiscall 577C12 (this, pos*, vector<aarect>*, float, byte*)
	void ApplyBlocking(const shok_position& p, const shok_AARect& area, float rot, BlockingMode blockingmode);
	void RemoveBlocking(const shok_position& p, const shok_AARect& area, float rot, BlockingMode blockingmode);
	void AdvancedApplyBridgeHeight(const shok_position& p, const shok_AARect& area, float rot, int height);
	void UpdateBlocking(const shok_AARect& area);
	void AdvancedRemoveBridgeHeight(const shok_position& p, const shok_AARect& area, float rot);
	void AdvancedApplyBlocking(const shok_position& p, const shok_AARect& area, float rot, BlockingMode blockingmode);
	void AdvancedRemoveBlocking(const shok_position& p, const shok_AARect& area, float rot, BlockingMode blockingmode);
	bool IsAreaUnblockedInMode(const shok_position& p, const shok_AARect& area, float rot, BlockingMode mode);
	bool IsAreaNotUnderWater(const shok_position& p, const shok_AARect& area, float rot);
private:
	void RemoveSingleBlockingPoint(int x, int y, BlockingMode mode); // this probably got inlined by the compiler originally...
};
constexpr shok_EGL_CGLELandscape::BlockingMode operator&(shok_EGL_CGLELandscape::BlockingMode a, shok_EGL_CGLELandscape::BlockingMode b);
constexpr shok_EGL_CGLELandscape::BlockingMode operator|(shok_EGL_CGLELandscape::BlockingMode a, shok_EGL_CGLELandscape::BlockingMode b);
constexpr shok_EGL_CGLELandscape::BlockingMode operator^(shok_EGL_CGLELandscape::BlockingMode a, shok_EGL_CGLELandscape::BlockingMode b);
struct shok_EGL_CGLELandscape_blockingData {
	friend struct shok_EGL_CGLELandscape;
	int ArraySizeXY;
	byte* data;
private:
	inline shok_EGL_CGLELandscape::BlockingMode GetBlockingData(int x, int y);
};

struct shok_EGL_CPlayerExplorationHandler : shok_BB_IObject {
	PADDINGI(2);
	int SizeX, SizeY; // 3

	bool IsPositionExplored(const shok_position& p);
};

struct shok_somegamelogicstuff {

	shok_EGL_CPlayerExplorationHandler* GetExplorationHandlerByPlayer(int pl);
};

// game logic
struct shok_EGL_CGLEGameLogic : shok_object {
	PADDINGI(6);
	int* InGameTime; // 7
	PADDINGI(1);
	shok_EGL_CGLELandscape* Landscape; // 9
	PADDINGI(9);
	shok_somegamelogicstuff* SomeStuff;

	static inline constexpr int vtp = 0x7839CC;

	int CreateEffect(shok_EGL_CGLEEffectCreator* data);
	int CreateEntity(shok_EGL_CGLEEntityCreator* cr);
	int GetTimeMS();
	int GetTick();

	void HookCreateEffect(); // currently unused
	static void(*CreateEffectHookCallback)(int id, void* ret);

	static inline shok_EGL_CGLEGameLogic** const GlobalObj = reinterpret_cast<shok_EGL_CGLEGameLogic**>(0x895DAC); // also 85A3A4
	static inline int* const MapSize = reinterpret_cast<int*>(0x898B74);
};
//constexpr int i = offsetof(shok_EGL_CGLEGameLogic, SomeStuff) / 4;

struct shok_ED_CLandscape : shok_object {
	shok_EGL_CGLETerrainHiRes* TerrainHiRes;
	shok_EGL_CGLETerrainLowRes* TerrainLowRes;
	shok_EGL_CGLELandscape_blockingData* Blocking;
	int SizeDiv32X, SizeDiv32Y;
	int HiResSizeX, HiResSizeY;
	float WorldSizeX, WorldSizeY;

	static inline constexpr int vtp = 0x76A404;

	bool GetTerrainPosAtScreenCoords(shok_positionRot& outpos, int x, int y); // r in this case is the terrain height at the position
	float GetTerrainHeightAtPos(const shok_position& p);
	float GetWaterHeightAtPos(const shok_position& p);
};
static_assert(sizeof(shok_ED_CLandscape) == 10 * 4);

struct shok_EGL_CRegionInfo : shok_object {
	static inline constexpr int vtp = 0x783878;
};
struct shok_ED_CGlobalsLogicEx : shok_object {
	shok_EGL_CGLEGameLogic* GameLogic;
	PADDINGI(4); // p EGL::CGLEEntitiesDisplay, p EGL::CGLEEffectsDisplay, p EGL::CGLETerrainHiRes, p EGL::CGLETerrainLowRes
	shok_EGL_CGLELandscape_blockingData* Blocking; // 6
	PADDINGI(2); // p EGL::CGLELandscape, p EGL::CTerrainVertexColors
	shok_EGL_CRegionInfo* RegionInfo; // 9
	PADDINGI(1); // p EGL::CPlayerExplorationHandler
	shok_ED_CLandscape* Landscape;
	// p ED::CLandscapeFogOfWar, 
	PADDINGI(7);
	shok_ED_CVisibleEntityManager* VisibleEntityManager; // 19

	static inline constexpr int vtp = 0x769F74;

	void ToTerrainCoord(const shok_position& p, int* out);
	bool IsCoordValid(int* out);
	shok_EGL_CGLELandscape::BlockingMode GetBlocking(const shok_position& p);

	static inline shok_ED_CGlobalsLogicEx** const GlobalObj = reinterpret_cast<shok_ED_CGlobalsLogicEx**>(0x8581EC);
};
//constexpr int i = offsetof(shok_ED_CGlobalsLogicEx, VisibleEntityManager) / 4;

// entity manager
struct shok_EGL_CGLEEntityManager : shok_object {
	int EntityCount;
	PADDINGI(4);
private:
	struct {
		PADDINGI(1); // some type of flags
		shok_EGL_CGLEEntity* entity;
	} Entities[1];

public:
	shok_EGL_CGLEEntity* GetEntityByNum(int num);

	static inline constexpr int vtp = 0x783B70;

	static inline shok_EGL_CGLEEntityManager** const GlobalObj = reinterpret_cast<shok_EGL_CGLEEntityManager**>(0x897558);
};

struct shok_GGL_CDamageClassProps : shok_BB_IObject {
	float BonusVsArmorClass[7]; // remember to access with ArmorClass - 1

	static inline constexpr int vtp = 0x788978;
};
struct shok_damageClassHolder {
	shok_BB_CIDManagerEx* DamageClassManager;
	vector_padding;
	std::vector<shok_GGL_CDamageClassProps*, shok_allocator<shok_GGL_CDamageClassProps*>> DamageClassList; // there is a damageclass 0, probably not working at all

	static inline shok_damageClassHolder** const GlobalObj = reinterpret_cast<shok_damageClassHolder**>(0x85A3DC);
};


struct shok_GGL_CLogicProperties_SBuildingUpgradeCategory : shok_object {
	int Category, FirstBuilding;
	PADDINGI(1);

	static inline constexpr int vtp = 0x76EF10;
};
struct shok_GGL_CLogicProperties_SSettlerUpgradeCategory : shok_object {
	int Category, FirstSettler;

	static inline constexpr int vtp = 0x76EF18;
};
struct shok_GGL_CLogicProperties_STaxationLevel : shok_object {
	int RegularTax;
	float MotivationChange;

	static inline constexpr int vtp = 0x76EF20;
};
struct shok_GGL_CLogicProperties_STradeResource : shok_object {
	int ResourceType;
	float BasePrice, MinPrice, MaxPrice, Inflation, Deflation, WorkAmount;

	static inline constexpr int vtp = 0x76EF28;
};
struct shok_GGL_CLogicProperties_SBlessCategory : shok_object {
	int Name;
	float RequiredFaith;
	vector_padding;
	std::vector<int, shok_allocator<int>> EntityTypes;

	static inline constexpr int vtp = 0x76EFC4;
};
struct shok_GGL_CLogicProperties : shok_BB_IObject {
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

	static inline constexpr int vtp = 0x76EFCC;

	static inline shok_GGL_CLogicProperties** const GlobalObj = reinterpret_cast<shok_GGL_CLogicProperties**>(0x85A3E0);
};

struct shok_GGL_CPlayerAttractionProps : shok_BB_IObject {
	int AttractionFrequency, PaydayFrequency, EntityTypeBanTime, ReAttachWorkerFrequency, PlayerMoneyDispo;
	float MaximumDistanceWorkerToFarm, MaximumDistanceWorkerToResidence;

	static inline constexpr int vtp = 0x770834;

	static inline shok_GGL_CPlayerAttractionProps** const GlobalObj = reinterpret_cast<shok_GGL_CPlayerAttractionProps**>(0x866A80);
};


struct shok_GGL_CGLGameLogic_TechList {
	vector_padding;
	std::vector<shok_technology*, shok_allocator<shok_technology*>> TechList;
};

struct shok_GGL_CWeatherHandler_weatherElement {
	int StartTimeOffset = 0; // ticks, sec*10
	int Length = 0; // ticks, sec*10
	int State = 0;
	PADDINGI(1); // nonperiodic index?
	bool IsPeriodic = false;
	PADDING(3);
	int WeatherIndex = 0; //5
	int Forerun = 0;
	int GfxSet = 0;
	int Transition = 0;
};
struct shok_GGL_CWeatherHandler_KeyAndWeatherElement {
	int WeatherIndex;
	shok_GGL_CWeatherHandler_weatherElement WeatherElement;
};
struct shok_GGL_CWeatherHandler : shok_object {
	int CurrentWeatherState;
	int CurrentWeatherIndex;
	PADDINGI(1);// 3 next nonperiodic weather?
	int NextWeatherIndex;
	int CurrentWeatherOffset; // 5
	shok_set<shok_GGL_CWeatherHandler_KeyAndWeatherElement> Elements;
	int NextPeriodicWeatherStartTimeOffset;
	struct { // 10
		int CurrentWeatherGFXState; //0
		int FromGFXState;
		int StartedTimeOffset;
		int ToGFXState; //3 0 if no chnage
		int TransitionLength;
		int StateToChangeFrom;
		int StateToChangeTo;
		int WeatherStateChangeTimeOffset; //7
		int WIndexToChangeTo;
		int State; // 1 changing, 2 fix
	} WeatherChange;


	static inline constexpr int vtp = 0x770140;
	static inline constexpr int TypeDesc = 0x811DE8;

	int GetNextWeatherState();
	int GetCurrentWeatherState();
	int GetTicksToNextPeriodicWeatherChange();
	void AddWeatherElement(int state, int dur, bool peri, int forerun, int gfx, int transition); // all times in ticks
	void ClearQueue(int state, int dur, int forerun, int gfx, int transition);
};
//constexpr int i = offsetof(shok_GGL_CWeatherHandler, CurrentWeatherGFXState) / 4;

// gamelogic
struct shok_GGL_CGLGameLogic : shok_object {
	PADDINGI(9);
public:
	shok_GGL_CPlayerStatus** players; // 10
	shok_GGL_CWeatherHandler* WeatherHandler;
	PADDINGI(1);
private:
	shok_GGL_CGLGameLogic_TechList* TechList;
public:
	PADDINGI(1);
	bool GlobalInvulnerability;
	PADDING(3);

	static inline constexpr int vtp = 0x76E018;

	shok_GGL_CPlayerStatus* GetPlayer(int i);
	shok_technology* GetTech(int i);
	void EnableAlarmForPlayer(int pl);
	void DisableAlarmForPlayer(int pl);
	void UpgradeSettlerCategory(int pl, int ucat);
	void PlayerActivateWeathermachine(int player, int weathertype);
	void PlayerBlessSettlers(int player, int blessCat);

	static inline shok_GGL_CGLGameLogic** const GlobalObj = reinterpret_cast<shok_GGL_CGLGameLogic**>(0x85A3A0);
};
//constexpr int i = offsetof(shok_GGL_CGLGameLogic, WeatherHandler);


struct shok_EScr_CScriptTriggerSystem : shok_BB_IObject {
private:
	shok_BB_IPostEvent PostEvent;

public:
	static inline constexpr int vtp = 0x78667C;

	void RunTrigger(shok_BB_CEvent* ev);

	static inline shok_EScr_CScriptTriggerSystem** const GlobalObj = reinterpret_cast<shok_EScr_CScriptTriggerSystem**>(0x895DEC);
};

struct shok_BB_IFileSystem : shok_object {

};
struct shok_BB_CDirectoryFileSystem : shok_BB_IFileSystem {
	char* Path;

	static inline constexpr int vtp = 0x7803B4;
};
struct shok_BB_CBBArchiveFile : shok_BB_IFileSystem {
	PADDINGI(2); // BB::CFileStream 761C98, then probably file handle
	char* Path;

	static inline constexpr int vtp = 0x77FABC;
};
struct shok_BB_CFileSystemMgr : shok_object {
	vector_padding;
	std::vector<shok_BB_IFileSystem*, shok_allocator<shok_BB_IFileSystem*>> LoadOrder;

	static inline constexpr int vtp = 0x77F794;

	void AddFolder(const char* path);
	void AddArchive(const char* path);
	void RemoveTopArchive();
	// handle + size get set, use shok_BB_CFileSystemMgr::CloseHandle to close the file after you dont need it any more.
	// to read/write a file more easily, use shok_BB_CFileStreamEx.
	// remove data/ before usage, this func does not do that by itself.
	bool OpenFileAsHandle(const char* path, int& handle, size_t& size);
	static bool CloseHandle(int handle);

	static inline shok_BB_CFileSystemMgr** const GlobalObj = reinterpret_cast<shok_BB_CFileSystemMgr**>(0x88F088);
	static inline const char* (__cdecl* const PathGetExtension)(const char* path) = reinterpret_cast<const char* (__cdecl*)(const char*)>(0x40BAB3);
};

struct shok_ED_CDisplayProps : shok_object {
	byte ShadowBlur;
	PADDING(3);
	int ShadowRasterSize;
	float ShadowStrength;
	int InvalidPositionColorModulate;
	int InvalidPositionColorEmissive;
	vector_padding;
	std::vector<int, shok_allocator<int>> PlayerColor;
	vector_padding;
	std::vector<int, shok_allocator<int>> MiniMapColor;
	float SelectionRadiusScaleForModelsWithDecal;
	int FogOfWarNeverSeenLuminance;
	int FogOfWarSeenLuminance;
	float AuraRadius;
	float AuraHeight;
	shok_string AuraTexture;
	int CommandAcknowledgementModel;
	int CommandAcknowledgementDuration;

	static inline constexpr int vtp = 0x7AE630;
};

struct shok_ED_CModelsProps : shok_object {


	static inline constexpr int vtp = 0x7AE60C;
};

struct shok_ED_CCameraEx : shok_object {
	PADDINGI(1); //1
	void* SomeCameraData;

	static inline constexpr int vtp = 0x769E64;
};

struct shok_ED_CPlayerColors : shok_object {

	PADDINGI(63);
	int Colors[17]; // 8 player

	static inline constexpr int vtp = 0x76964C;

	int GetColorByIndex(int i);
	void SetColorByIndex(int i, int c);
	void RefreshPlayerColors();
};
struct shok_ED_CCommandAcknowledgements : shok_object {

	static inline constexpr int vtp = 0x7695C0;

	void ShowAck(const shok_position& pos);
};
struct shok_modelinstance {
	enum class TransformOperation : int {
		Set = 0, // sets to identity, then performs the operation
		Multiply = 1, // the new matrix is performed last
		ReverseMultiply = 2, // the new matrix is performed first
	};


	PADDINGI(1);
	struct {
		PADDINGI(4);
		float Matrix[4 * 4];
	}*Transform;

	void Register();
	void Destroy();
	void Rotate(float r, TransformOperation op);
	void Rotate(float r, float* axis, TransformOperation op);
	void Scale(float* s, TransformOperation op); // 3 coordinates (order?)
	void Scale(float s, TransformOperation op);
	void Translate(const shok_position& p, float height, TransformOperation op);
};
struct shok_modeldata {

	shok_modelinstance* Instanciate() const;
};
struct shok_ED_CResourceManager : shok_object {
	PADDINGI(1); // conat char**?
	void* ModelData; // most likely a vector
	PADDINGI(3);

	static inline constexpr int vtp = 0x769824;

	const shok_modeldata* GetModelData(int modelid);
};
struct shok_ED_CWorld : shok_object {
	void* SomeRenderObj;

	static inline constexpr int vtp = 0x769E94;
};
struct shok_ED_CGlobalsBaseEx : shok_object {
	shok_BB_CIDManagerEx* AnimManager;
	shok_BB_CIDManagerEx* ModelManager;
	shok_BB_CIDManagerEx* SpecialEffectManager; // lightning and snow textures?
	PADDINGI(1); // empty shok_BB_CIDManagerEx
	shok_ED_CDisplayProps* DisplayProps; // 5
	shok_BB_CIDManagerEx** EffectManager; // probably an object without vtable
	shok_BB_CIDManagerEx** EntityTypeManager; // probably an object without vtable
	shok_ED_CModelsProps* ModelProps;
	PADDINGI(1); // p to something terrain related?
	PADDINGI(1); // p to something water related? // 10
	shok_BBRw_CEngine* RWEngine; // p to BBRw::CEngine
	PADDINGI(1); // p to ED::CAuras
	shok_ED_CCameraEx* Camera;
	shok_ED_CCommandAcknowledgements* CommandAcks;
	PADDINGI(1); // p to ED::CEntitiesTypeFlags // 15
	PADDINGI(1); // p to ED::CGUIScene
	PADDINGI(1); // unknown
	PADDINGI(1); // p to ED::CLight
	PADDINGI(1); // unknown
	PADDINGI(1); // p to ED::COcclusionEffect
	PADDINGI(1); // p to ED::COrnamentalItems
	shok_ED_CPlayerColors* PlayerColors; // 22
	PADDINGI(3);
	shok_ED_CResourceManager* ResManager; // 26
	PADDINGI(7);
	shok_ED_CWorld* DisplayWorld;

	static inline constexpr int vtp = 0x769478;

	static inline shok_ED_CGlobalsBaseEx** const GlobalObj = reinterpret_cast<shok_ED_CGlobalsBaseEx**>(0x857E8C);
};
//constexpr int i = offsetof(shok_ED_CGlobalsBaseEx, DisplayWorld) / 4;

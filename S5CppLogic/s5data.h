#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <functional>

#define DEBUGGER_BREAK _asm int 3

#define SHOK_Import_LUA_OPEN 0x761284
#define SHOK_SEGMENTSTART 0x401000
#define SHOK_SEGMENTLENGTH 0x64B000

typedef uint8_t byte;

// allocator
extern void* (__cdecl* shok_malloc)(size_t t);
extern void* (__cdecl* shok_new)(size_t t);
extern void(__cdecl* shok_free)(void* p);
extern void (*shok_logString)(const char* format, const char* string);

struct shok_vector {
	int vtable, start, end, encCap;
};

template <class T>
struct shok_allocator {
	typedef T value_type;
	shok_allocator() = default;
	template <class U> constexpr shok_allocator(const shok_allocator <U>&) noexcept {}

	[[nodiscard]] T* allocate(size_t n) noexcept
	{
		//shok_logString("alloc: %s\n", std::to_string(n).c_str());
		void* p = shok_malloc(n * sizeof(T));
		//shok_logString("allocpoint: %s\n", std::to_string((int)p).c_str());
		return (T*)p;
	}
	void deallocate(T* p, size_t n) noexcept
	{
		//shok_logString("free: %s\n", std::to_string(n).c_str());
		//shok_logString("freepoint: %s\n", std::to_string((int)p).c_str());
		shok_free(p);
	}
};
template <class T, class U>
bool operator==(const shok_allocator <T>&, const shok_allocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const shok_allocator <T>&, const shok_allocator <U>&) { return false; }

template<class T>
inline void shok_saveVector(std::vector<T, shok_allocator<T>>* vec, std::function<void(std::vector<T, shok_allocator<T>> &s)> func) {
	std::vector<T, shok_allocator<T>> save = std::vector<T, shok_allocator<T>>();
	int* vecPoint = (int*)vec;
	int* savePoint = (int*)&save;
	int backu[3] = {};
	for (int i = 1; i < 4; i++) {
		backu[i - 1] = savePoint[i];
		savePoint[i] = vecPoint[i];
	}
	func(save);
	for (int i = 1; i < 4; i++) {
		vecPoint[i] = savePoint[i];
		savePoint[i] = backu[i - 1];
	}
}

// generic structs
struct shok_position {
	float X;
	float Y;
};

struct shok_positionRot : shok_position {
	float r;
};

struct shok_costInfo {
private:
	int u;
public:
	float Gold, GoldRaw, Silver, SilverRaw, Stone, StoneRaw, Iron, IronRaw, Sulfur, SulfurRaw, Clay, ClayRaw, Wood, WoodRaw, WeatherEnergy, Knowledge, Faith;
};

struct shok_modifyEntityProps {
	int MysteriousInt;
	std::vector<int, shok_allocator<int>> TechList;
};

struct shok_object {
public:
	void* vtable;
};


// behavior props
#define shok_vtp_EGL_CGLEBehaviorProps (void*)0x772A2C
struct shok_EGL_CGLEBehaviorProps : shok_object {
private:
	int u;
public:
	int BehaviorIndex, BehaviorClass;
};

#define shok_vtp_EGL_CMovementBehaviorProps (void*)0x784938
struct shok_EGL_CMovementBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float MovementSpeed, TurningSpeed;
	int MoveTaskList, MoveIdleAnim;
};

#define shok_vtp_GGL_CHeroAbilityProps (void*)0x773774
struct shok_GGL_CHeroAbilityProps : shok_EGL_CGLEBehaviorProps {
	int RechargeTimeSeconds;
};

#define shok_vtp_GGL_CCamouflageBehaviorProps (void*)0x7778D8
struct shok_GGL_CCamouflageBehaviorProps : shok_GGL_CHeroAbilityProps {
	int DurationSeconds;
	float DiscoveryRange;
};

#define shok_vtp_GGL_CHeroHawkBehaviorProps (void*)0x77672C
struct shok_GGL_CHeroHawkBehaviorProps : shok_GGL_CHeroAbilityProps {
	int HawkType;
	float HawkMaxRange;
};

#define shok_vtp_GGL_CInflictFearAbilityProps (void*)0x776674
struct shok_GGL_CInflictFearAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation, FlightDuration;
	float Range, FlightRange;
};

#define shok_vtp_GGL_CCannonBuilderBehaviorProps (void*)0x777510
struct shok_GGL_CCannonBuilderBehaviorProps : shok_GGL_CHeroAbilityProps {
	int TaskList;
};

#define shok_vtp_GGL_CRangedEffectAbilityProps (void*)0x774E9C
struct shok_GGL_CRangedEffectAbilityProps : shok_GGL_CHeroAbilityProps {
	byte AffectsOwn, AffectsFriends, AffectsNeutrals, AffectsHostiles, AffectsMilitaryOnly, AffectsLongRangeOnly;
private:
	byte u[2];
public:
	float Range;
	int DurationSeconds;
	float DamageFactor, ArmorFactor, HealthRecoveryFactor;
	int Effect, HealEffect;
};

#define shok_vtp_GGL_CCircularAttackProps (void*)0x7774A0
struct shok_GGL_CCircularAttackProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation, DamageClass, Damage;
	float Range;
};

#define shok_vtp_GGL_CSummonBehaviorProps (void*)0x773C50
struct shok_GGL_CSummonBehaviorProps : shok_GGL_CHeroAbilityProps {
	int SummonedEntityType, NumberOfSummonedEntities, SummonTaskList;
};

#define shok_vtp_GGL_CConvertSettlerAbilityProps (void*)0x7772D0
struct shok_GGL_CConvertSettlerAbilityProps : shok_GGL_CHeroAbilityProps {
	int ConversionTaskList;
	float HPToMSFactor, ConversionStartRange, ConversionMaxRange;
};

#define shok_vtp_GGL_CSniperAbilityProps (void*)0x7745E8
struct shok_GGL_CSniperAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range, DamageFactor;
};

#define shok_vtp_GGL_CMotivateWorkersAbilityProps (void*)0x775788
struct shok_GGL_CMotivateWorkersAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range;
	int WorkTimeBonus, Effect;
};

#define shok_vtp_GGL_CShurikenAbilityProps (void*)0x7746B4
struct shok_GGL_CShurikenAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range, MaxArcDegree;
	int NumberShuriken, ProjectileType;
	float ProjectileOffsetHeight, ProjectileOffsetFront, ProjectileOffsetRight;
	int DamageClass, DamageAmount;
};

#define shok_vtp_GGL_CSentinelBehaviorProps (void*)0x774BAC
struct shok_GGL_CSentinelBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float Range;
};

#define shok_vtp_GGL_CGLAnimationBehaviorExProps (void*)0x776C48
struct shok_GGL_CGLAnimationBehaviorExProps : shok_EGL_CGLEBehaviorProps {
	int SuspensionAnimation, AnimSet;
};

#define shok_vtp_GGL_CWorkerBehaviorProps (void*)0x772B90
struct shok_GGL_CWorkerBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int WorkTaskList, WorkIdleTaskList;
private:
	int u;
public:
	int WorkWaitUntil, EatTaskList, EatIdleTaskList, EatWait, RestTaskList, RestIdleTaskList, RestWait;
private:
	int u2;
public:
	int LeaveTaskList;
	float AmountResearched;
private:
	int u3;
public:
	float WorkTimeChangeFarm, WorkTimeChangeResidence, WorkTimeChangeCamp;
	int WorkTimeMaxCangeFarm, WorkTimeMaxChangeResidence;
	float ExhaustedWorkMotivationMalus;
	int TransportAmount, TransportModel, TransportAnim, ResourceToRefine;
	float WorkTimeChangeWork;
};

#define shok_vtp_GGL_CBattleBehaviorProps (void*)0x7731C0
struct shok_GGL_CBattleBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int BattleTaskList, NormalAttackAnim1, NormalAttackAnim2, CounterAttackAnim, FinishingMoveAnim, MissAttackAnim, BattleIdleAnim, BattleWalkAnim;
	int HitAnim, DamageClass, DamageAmount, MaxDamageRandomBonus;
	float DamageRange;
	int ProjectileEffectID;
	float ProjectileOffsetFront, ProjectileOffsetRight, ProjectileOffsetHeight;
	int BattleWaitUntil, MissChance;
	float MaxRange, MinRange;
};

#define shok_vtp_GGL_CLeaderBehaviorProps (void*)0x775FA4
struct shok_GGL_CLeaderBehaviorProps : shok_GGL_CBattleBehaviorProps {
	int SoldierType, BarrackUpgradeCategory;
	float HomeRadius;
	int HealingPoints, HealingSeconds;
	float AutoAttackRange, UpkeepCosts;
};

#define shok_vtp_GGL_CSoldierBehaviorProps (void*)0x773D10
struct shok_GGL_CSoldierBehaviorProps : shok_GGL_CBattleBehaviorProps {

};

#define shok_vtp_GGL_CSerfBattleBehaviorProps (void*)0x774B50
struct shok_GGL_CSerfBattleBehaviorProps : shok_GGL_CBattleBehaviorProps {

};

#define shok_vtp_GGL_CBattleSerfBehaviorProps (void*)0x77889C
struct shok_GGL_CBattleSerfBehaviorProps : shok_GGL_CLeaderBehaviorProps {

};

struct shok_serf_extractionInfo {
	int ResourceEntityType;
	float Delay;
	int Amount;
};
#define shok_vtp_GGL_CSerfBehaviorProps (void*)0x774A14
struct shok_GGL_CSerfBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int ResourceSearchRadius, ApproachConbstructionSiteTaskList, TurnIntoBattleSerfTaskList;
	std::vector<shok_serf_extractionInfo, shok_allocator<shok_serf_extractionInfo>> ExtractionInfo;
};

struct shok_limitedAttachmentProps {
private:
	int u;
public:
	char* Type;
private:
	int u2[4];
public:
	int Limit;
};
#define shok_vtp_GGL_CLimitedAttachmentBehaviorProperties (void*)0x775DE4
struct shok_GGL_CLimitedAttachmentBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	std::vector<shok_limitedAttachmentProps, shok_allocator<shok_limitedAttachmentProps>> Attachments;
};

struct shok_idleAnimProps {
	int AnimID, Frequency;
};
#define shok_vtp_GGL_CFormationBehaviorProperties (void*)0x776DE4
struct shok_GGL_CFormationBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	std::vector<shok_idleAnimProps, shok_allocator<shok_idleAnimProps>> IdleAnims;
};

#define shok_vtp_GGL_CCamperBehaviorProperties (void*)0x7777D4
struct shok_GGL_CCamperBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Range;
};

#define shok_vtp_GGL_CGLBehaviorPropsDying (void*)0x778634
struct shok_GGL_CGLBehaviorPropsDying : shok_EGL_CGLEBehaviorProps {
	int DyingTaskList;
};

#define shok_vtp_GGL_CHeroBehaviorProps (void*)0x7767F4
struct shok_GGL_CHeroBehaviorProps : shok_EGL_CGLEBehaviorProps {

};

#define shok_vtp_GGL_CBombBehaviorProperties (void*)0x7784A0
struct shok_GGL_CBombBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Radius, Delay;
	int Damage, ExplosionEffectID;
	std::vector<int, shok_allocator<int>> AffectedEntityTypes;
};

#define shok_vtp_GGL_CKegBehaviorProperties (void*)0x776558
struct shok_GGL_CKegBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Radius;
	int Damage;
	float Delay;
	int DamagePercent, ExplosionEffectID;
};

#define shok_vtp_GGL_CThiefBehaviorProperties (void*)0x7739E0
struct shok_GGL_CThiefBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int SecondsNeededToSteal, MinimumAmountToSteal, MaximumAmountToSteal, CarryingModelID, StealGoodsTaskList, SecureGoodsTaskList;
};

#define shok_vtp_GGL_CAutoCannonBehaviorProps (void*)0x778CD4
struct shok_GGL_CAutoCannonBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int NumberOfShots;
	float RotationSpeed;
	int CannonBallEffectType;
private:
	int u[3];
public:
	int ReloadTime;
	float MaxAttackRange;
private:
	int u2;
public:
	int DamageClass, DamageAmount;
	float DamageRange;
	int BattleTaskList;
};

#define shok_vtp_GGL_CResourceRefinerBehaviorProperties (void*)0x774C24
struct shok_GGL_CResourceRefinerBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int ResourceType;
	float InitialFactor;
private:
	int u[4];
public:
	int SupplierCategory;
};

#define shok_vtp_GGL_CAffectMotivationBehaviorProps (void*)0x7791D4
struct shok_GGL_CAffectMotivationBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float MotivationEffect;
};

#define shok_vtp_GGL_CLimitedLifespanBehaviorProps (void*)0x775DE4
struct shok_GGL_CLimitedLifespanBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int LifespanSeconds;
};

#define shok_vtp_GGL_CBarrackBehaviorProperties (void*)0x778B34
struct shok_GGL_CBarrackBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int TrainingTaskList1, TrainingTaskList2, TrainingTaskList3;
private:
	int MaxTrainingNumber;
public:
	int LeaveTaskList;
	float TrainingTime;
};

#define shok_vtp_GGL_CBuildingMerchantBehaviorProps (void*)0x7783B8
struct shok_GGL_CBuildingMerchantBehaviorProps : shok_EGL_CGLEBehaviorProps {

};

#define shok_vtp_GGL_CServiceBuildingBehaviorProperties (void*)0x774830
struct shok_GGL_CServiceBuildingBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	// initial amount at 4?
};


// entitytype

struct shok_AARect {
	shok_position low, high;
};
#define shok_vtp_EGL_CGLEEntityProps (void*)0x76E47C
struct shok_EGL_CGLEEntityProps : shok_object {
private:
	int u3;
public:
	int Class;
	std::vector<int, shok_allocator<int>> Categories;
	shok_positionRot ApproachPos;
private:
	int Race;
public:
	byte CanFloat, CanDrown, MapFileDontSave, DividesTwoSectors, ForceNoPlayer, AdjustWalkAnimSpeed, Visible, DoNotExecute;
	int MaxHealth;
private:
	int Models[5];
public:
	float Exploration;
	int ExperiencePoints, AccessCategory, NumBlockedPoints;
	float SnapTolerance;
	byte DeleteWhenBuiltOn, NeedsPlayer;
private:
	byte u[2];
	int u2[9];
public:
	std::vector<shok_AARect, shok_allocator<shok_AARect>> BlockingArea; // la37
};

#define shok_vtp_GGL_CEntityProperties (void*)0x776FEC
struct shok_GGL_CEntityProperties : shok_EGL_CGLEEntityProps {
	int ResourceEntity, ResourceAmount, SummerEffect, WinterEffect;
};

struct shok_upgradeInfo {
	float Time;
	shok_costInfo Cost;
	int Type, Category;
};
#define shok_vtp_GGL_CGLSettlerProps (void*)0x76E498
struct shok_GGL_CGLSettlerProps : shok_EGL_CGLEEntityProps {
private:
	int HeadSet, Hat;
public:
	shok_costInfo Cost;
	float BuildFactor, RepairFactor;
	int ArmorClass;
	int ArmorAmount;
	int DodgeChance;
	int IdleTaskList;
	shok_upgradeInfo Upgrade;
	byte Fearless, Convertible;
private:
	byte u[2];
public:
	shok_modifyEntityProps ModifyExploration, ModifyHitpoints, ModifySpeed, ModifyDamage, ModifyArmor, ModifyDodge, ModifyMaxRange, ModifyMinRange, ModifyDamageBonus, ModifyGroupLimit;
	int AttractionSlots;
};

#define shok_vtp_GGL_CGLAnimalProps (void*)0x779074
struct shok_GGL_CGLAnimalProps : shok_EGL_CGLEEntityProps {
	int DefaultTaskList;
	float TerritoryRadius, WanderRangeMin, WanderRangeMax, ShyRange, MaxBuildingPollution;
	int FleeTaskList;
};

#define shok_vtp_GGL_CBuildBlockProperties (void*)0x76EB38
struct shok_GGL_CBuildBlockProperties : shok_EGL_CGLEEntityProps {
	shok_AARect BuildBlockArea; // la 41
};

#define shok_vtp_GGL_CResourceDoodadProperties (void*)0x76FF68
struct shok_GGL_CResourceDoodadProperties : shok_GGL_CBuildBlockProperties {
	float Radius;
	shok_position Center, LineStart, LineEnd;
	int ExtractTaskList, Model1, Model2;
};

struct shok_constructionInfo {
private:
	int u;
public:
	std::vector<shok_positionRot, shok_allocator<shok_positionRot>> BuilderSlot;
	int Time;
	shok_costInfo Cost;
	int ConstructionSite;
};
#define shok_vtp_GGL_CGLBuildingProps (void*)0x76EC78
struct shok_GGL_CGLBuildingProps : shok_GGL_CBuildBlockProperties {
	int MaxWorkers, InitialMaxWorkers, NumberOfAttractableSettlers, Worker;
	shok_position DoorPos, LeavePos;
	shok_constructionInfo ConstructionInfo;
	std::vector<int, shok_allocator<int>> BuildOn;
	byte HideBase, CanBeSold, IsWall;
private:
	byte u;
public:
	shok_upgradeInfo Upgrade;
	int UpgradeSite, ArmorClass, ArmorAmount;
	std::vector<int, shok_allocator<int>> WorkTaskList;
private:
	int MilitaryInfo[4];
public:
	float CollapseTime;
	byte Convertible;
private:
	byte u2[3];
public:
	shok_modifyEntityProps ModifyExploration, ModifyArmor;
	float KegEffectFactor; // 124
};

#define shok_vtp_GGL_CBridgeProperties (void*)0x778148
struct shok_GGL_CBridgeProperties : shok_GGL_CGLBuildingProps {
	shok_AARect BridgeArea;
	int Height, ConstructionModel0, ConstructionModel1, ConstructionModel2;
};

#define shok_vtp_ED_CDisplayEntityProps (void*)0x788840
struct shok_ED_CDisplayEntityProps : shok_object {
	int DisplayClass;
	int Model[4];
	byte DrawPlayerColor, CastShadow, RenderInFoW, HighQualityOnly, MapEditor_Rotateable, MapEditor_Placeable;
private:
	byte u[2];
public:
	std::vector<int, shok_allocator<int>> AnimList;
};

#define shok_vtp_GGlue_CGlueEntityProps (void*)0x788824
struct shok_GGlue_CGlueEntityProps : shok_object {
private:
	int u;
public:
	shok_EGL_CGLEEntityProps* LogicProps;
	shok_ED_CDisplayEntityProps* DisplayProps;
	std::vector<shok_EGL_CGLEBehaviorProps*, shok_allocator<shok_EGL_CGLEBehaviorProps*>> BehaviorProps;


private:
	shok_EGL_CGLEBehaviorProps* SearchBehaviorProp(void** vts, int num);
	shok_EGL_CGLEBehaviorProps* SearchBehaviorProp(void* vt);

public:
	shok_GGL_CLeaderBehaviorProps* GetLeaderBehaviorProps();
	shok_GGL_CSoldierBehaviorProps* GetSoldierBehaviorProps();
	shok_GGL_CBattleBehaviorProps* GetBattleBehaviorProps();
	shok_GGL_CLimitedLifespanBehaviorProps* GetLimitedLifespanBehaviorProps();
	shok_GGL_CAffectMotivationBehaviorProps* GetAffectMotivationBehaviorProps();
	shok_GGL_CGLAnimationBehaviorExProps* GetAnimationExProps();
	shok_GGL_CAutoCannonBehaviorProps* GetAutoCannonProps();
	shok_GGL_CThiefBehaviorProperties* GetThiefBehaviorProps();
	shok_GGL_CCamouflageBehaviorProps* GetCamouflageBehaviorProps();
	shok_GGL_CCircularAttackProps* GetCircularAttackBehaviorProps();
	shok_GGL_CShurikenAbilityProps* GetShurikenBehaviorProps();
	shok_GGL_CSniperAbilityProps* GetSniperBehaviorProps();
	shok_GGL_CRangedEffectAbilityProps* GetRangedEffectBehaviorProps();
	shok_GGL_CWorkerBehaviorProps* GetWorkerBehaviorProps();

	bool IsSettlerType();
	bool IsBuildingType();
	bool IsOfCategory(int cat);
	bool IsCEntityProperties();
};

#define shok_vtp_EGL_CGLEEntitiesProps (void*)0x788834
struct shok_EGL_CGLEEntitiesProps : shok_object {
private:
	int u[14];
public:
	std::vector<shok_GGlue_CGlueEntityProps, shok_allocator<shok_GGlue_CGlueEntityProps>> EntityTypes;

	shok_GGlue_CGlueEntityProps* GetEntityType(int i);
};



// behaviors
struct shok_EGL_CGLEBehavior : shok_object {
	int BehaviorIndex, EntityId;
private:
	int PropPointer; // 3
};

#define shok_vtp_GGL_CBehaviorDefaultMovement (void*)0x7786AC
struct shok_GGL_CBehaviorDefaultMovement : shok_EGL_CGLEBehavior {
private:
	int Counter;
public:
	float MovementSpeed, TurningSpeed, SpeedFactor;
};

#define shok_vtp_GGL_CSettlerMovement (void*)0x77471C
struct shok_GGL_CSettlerMovement : shok_GGL_CBehaviorDefaultMovement {
private:
	int u;
public:
	shok_positionRot PositionHiRes, Position;
	byte BlockingFlag;
private:
	byte u2[3];
};

#define shok_vtp_GGL_CLeaderMovement (void*)0x775ED4
struct shok_GGL_CLeaderMovement : shok_GGL_CBehaviorDefaultMovement {
	int MoveTaskList;
	shok_positionRot NextWayPoint, LastTurnPos;
	byte IsPathingUsed, IsMoveFinished;
private:
	byte u[2];
};

#define shok_vtp_GGL_CSoldierMovement (void*)0x77438C
struct shok_GGL_CSoldierMovement : shok_GGL_CBehaviorDefaultMovement {

};

struct shok_GGL_CHeroAbility : shok_EGL_CGLEBehavior {
private:
	int u[2];
public:
	int AbilitySecondsCharged; // 5
};

#define shok_vtp_GGL_CCamouflageBehavior (void*)0x7738F4
struct shok_GGL_CCamouflageBehavior : shok_GGL_CHeroAbility {
	int InvisibilityRemaining;
};

#define shok_vtp_GGL_CThiefCamouflageBehavior (void*)0x773934
struct shok_GGL_CThiefCamouflageBehavior : shok_GGL_CCamouflageBehavior {
	int TimeToInvisibility;
};

#define shok_vtp_GGL_CHeroHawkBehavior (void*)0x7766F0
struct shok_GGL_CHeroHawkBehavior : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CInflictFearAbility (void*)0x776638
struct shok_GGL_CInflictFearAbility : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CBombPlacerBehavior (void*)0x7783D8
struct shok_GGL_CBombPlacerBehavior : shok_GGL_CHeroAbility {
	shok_position StartPosition, TargetPosition;
	byte PlacedBomb;
private:
	byte u[3];
};

#define shok_vtp_GGL_CCannonBuilderBehavior (void*)0x7774D4
struct shok_GGL_CCannonBuilderBehavior : shok_GGL_CHeroAbility {
private:
	int u;
public:
	shok_position StartPosition;
	int CannonType, FoundationType;
	byte PlacedCannon;
private:
	byte u2[3];
};

#define shok_vtp_GGL_CRangedEffectAbility (void*)0x774E54
struct shok_GGL_CRangedEffectAbility : shok_GGL_CHeroAbility {
private:
	int u;
public:
	int SecondsRemaining;
};

#define shok_vtp_GGL_CCircularAttack (void*)0x777464
struct shok_GGL_CCircularAttack : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CSummonBehavior (void*)0x773C10
struct shok_GGL_CSummonBehavior : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CConvertSettlerAbility (void*)0x777294
struct shok_GGL_CConvertSettlerAbility : shok_GGL_CHeroAbility {
private:
	int u;
public:
	int TimeToConvert;
};

#define shok_vtp_GGL_CSniperAbility (void*)0x7745AC
struct shok_GGL_CSniperAbility : shok_GGL_CHeroAbility {
private:
	int u;
public:
	int TargetId;
};

#define shok_vtp_GGL_CMotivateWorkersAbility (void*)0x77574C
struct shok_GGL_CMotivateWorkersAbility : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CShurikenAbility (void*)0x774658
struct shok_GGL_CShurikenAbility : shok_GGL_CHeroAbility {
private:
	int u;
public:
	int TargetId;
};

#define shok_vtp_GGL_CSentinelBehavior (void*)0x774B6C
struct shok_GGL_CSentinelBehavior : shok_EGL_CGLEBehavior {

};

#define shok_vtp_GGL_CGLBehaviorAnimationEx (void*)0x776B64
struct shok_GGL_CGLBehaviorAnimationEx : shok_EGL_CGLEBehavior {
	int Animation, AnimCategory, SuspendedAnimation, StartTurn, Duration;
	byte PlayBackwards;
private:
	byte u[3];
public:
	int TurnToWaitFor;
	float Speed;
};

#define shok_vtp_GGL_CBehaviorWalkCommand (void*)0x7736A4
struct shok_GGL_CBehaviorWalkCommand : shok_EGL_CGLEBehavior {
	shok_position TargetPosition;
};

#define shok_vtp_GGL_CWorkerBehavior (void*)0x772B30
struct shok_GGL_CWorkerBehavior : shok_EGL_CGLEBehavior {
	int WorkTimeRemaining, TargetWorkTime;
	float Motivation;
private:
	int BehaviorProps2;
public:
	int CycleIndex, TimesWorked, TimesNoWork, TimesNoFood, TimesNoRest, TimesNoPay, JoblessSinceTurn, CouldConsumeResource, IsLeaving;
	float TransportAmount;
};

#define shok_vtp_GGL_CBattleBehavior (void*)0x77313C
struct shok_GGL_CBattleBehavior : shok_EGL_CGLEBehavior {
	float SuccessDistance, FailureDistance;
	int TimeOutTime, StartTurn;
	shok_position TargetPosition;
	byte StartFollowing, StopFollowing;
private:
	byte u1[2];
public:
	int FollowStatus, LatestHitTurn;
private:
	int u2;
public:
	int LatestAttackerID, BattleStatus;
	byte NoMoveNecessary, NormalRangeCheckNecessary;
private:
	byte u3[2];
public:
	int Command;
	shok_position AttackMoveTarget;
private:
	int u4;
public:
	int MilliSecondsToWait, MSToPlayHitAnimation, HitPlayed; // la 23
};

#define shok_vtp_GGL_CLeaderBehavior (void*)0x7761E0
struct shok_GGL_CLeaderBehavior : shok_GGL_CBattleBehavior {
private:
	int u[3];
public:
	int TroopHealthCurrent, TroopHealthPerSoldier;
	shok_position TerritoryCenter;
	float TerritoryCenterRange;
	int Experience;
	std::vector<shok_position, shok_allocator<shok_position>> PatrolPoints;
private:
	int u2;
public:
	float DefendOrientation;
	int TrainingStartTurn;
private:
	int u3;
public:
	int SecondsSinceHPRefresh, NudgeCount, FormationType;
	shok_position StartBattlePosition;
};

#define shok_vtp_GGL_CSoldierBehavior (void*)0x773CC8
struct shok_GGL_CSoldierBehavior : shok_GGL_CBattleBehavior {
};

#define shok_vtp_GGL_CBattleSerfBehavior (void*)0x7788C4
struct shok_GGL_CBattleSerfBehavior : shok_GGL_CLeaderBehavior {

};

#define shok_vtp_GGL_CSerfBattleBehavior (void*)0x774A98
struct shok_GGL_CSerfBattleBehavior : shok_GGL_CBattleBehavior {

};

#define shok_vtp_GGL_CSerfBehavior (void*)0x774874
struct shok_GGL_CSerfBehavior : shok_EGL_CGLEBehavior {

};

#define shok_vtp_GGL_CLimitedAttachmentBehavior (void*)0x775E84
struct shok_GGL_CLimitedAttachmentBehavior : shok_EGL_CGLEBehavior {
	// Attachment1 at 6, 0
};

#define shok_vtp_GGL_CFormationBehavior (void*)0x776D60
struct shok_GGL_CFormationBehavior : shok_EGL_CGLEBehavior {
	int AnimStartTurn, AnimDuration;
};

#define shok_vtp_GGL_CCamperBehavior (void*)0x77777C
struct shok_GGL_CCamperBehavior : shok_EGL_CGLEBehavior {

};

#define shok_vtp_GGL_CGLBehaviorDying (void*)0x7785E4
struct shok_GGL_CGLBehaviorDying : shok_EGL_CGLEBehavior {

};

#define shok_vtp_GGL_CHeroBehavior (void*)0x77677C
struct shok_GGL_CHeroBehavior : shok_EGL_CGLEBehavior {
private:
	int u;
public:
	int ResurrectionTimePassed, SpawnTurn;
	byte FriendNear, EnemyNear;
private:
	byte u2[2];
};

#define shok_vtp_GGL_CBombBehavior (void*)0x778468
struct shok_GGL_CBombBehavior : shok_EGL_CGLEBehavior {
	int TimeToExplode;
};

#define shok_vtp_GGL_CKegBehavior (void*)0x7764D8
struct shok_GGL_CKegBehavior : shok_EGL_CGLEBehavior {
private:
	int u;
public:
	int TimeToExplode;
};

#define shok_vtp_GGL_CThiefBehavior (void*)0x7739B0
struct shok_GGL_CThiefBehavior : shok_EGL_CGLEBehavior {
	int Amount, ResourceType, StolenFromPlayer, TimeToSteal;
};

#define shok_vtp_GGL_CAutoCannonBehavior (void*)0x778CF0
struct shok_GGL_CAutoCannonBehavior : shok_EGL_CGLEBehavior {
private:
	int u[6];
public:
	int ShotsLeft;
};

#define shok_vtp_GGL_CResourceRefinerBehavior (void*)0x774BCC
struct shok_GGL_CResourceRefinerBehavior : shok_EGL_CGLEBehavior {

};

#define shok_vtp_GGL_CAffectMotivationBehavior (void*)0x77918C
struct shok_GGL_CAffectMotivationBehavior : shok_EGL_CGLEBehavior {

};

#define shok_vtp_GGL_CLimitedLifespanBehavior (void*)0x775D9C
struct shok_GGL_CLimitedLifespanBehavior : shok_EGL_CGLEBehavior {
private:
	int u;
public:
	int RemainingLifespanSeconds;
};

#define shok_vtp_GGL_CBarrackBehavior (void*)0x778A68
struct shok_GGL_CBarrackBehavior : shok_EGL_CGLEBehavior {
	byte AutoFillActive;
private:
	byte u[3];
};

#define shok_vtp_EGL_GLEBehaviorMultiSubAnims (void*)0x785EEC
struct shok_EGL_GLEBehaviorMultiSubAnims : shok_EGL_CGLEBehavior {
private:
	int u;
public:
	int LastUpdateTurn;
	struct {
		byte Active, PlayBackwards, IsLooped;
	private:
		byte u;
	public:
		int AnimID, StartTurn, Duration;
	private:
		int u2;
	public:
		float Speed;
	} AnimSlot[4];
};

#define shok_vtp_GGL_CBuildingMerchantBehavior_COffer (void*)0x7781B4
struct shok_GGL_CBuildingMerchantBehavior_COffer : shok_object {
	shok_costInfo ResourceCosts;
	int OffersRemaining;
};

#define shok_vtp_GGL_CBuildingTechTraderBehavior_CTechOffer (void*)0x7781C4
struct shok_GGL_CBuildingTechTraderBehavior_CTechOffer : shok_GGL_CBuildingMerchantBehavior_COffer {
	int OfferedTechnologyType;
};

#define shok_vtp_GGL_CBuildingMercenaryBehavior_CMercenaryOffer (void*)0x778284
struct shok_GGL_CBuildingMercenaryBehavior_CMercenaryOffer : shok_GGL_CBuildingMerchantBehavior_COffer {
	int OfferedEntityType;
};

#define shok_vtp_GGL_CBuildingMerchantBehavior (void*)0x778208
struct shok_GGL_CBuildingMerchantBehavior : shok_EGL_CGLEBehavior {
private:
	int u[2];
public:
	std::vector<shok_GGL_CBuildingMerchantBehavior_COffer*, shok_allocator<shok_GGL_CBuildingMerchantBehavior_COffer*>> ListOfOffers;
};

#define shok_vtp_GGL_CBuildingMercenaryBehavior (void*)0x7782C0
struct shok_GGL_CBuildingMercenaryBehavior : shok_GGL_CBuildingMerchantBehavior {

};

#define shok_vtp_GGL_CBuildingTechTraderBehavior (void*)0x77822C
struct shok_GGL_CBuildingTechTraderBehavior : shok_GGL_CBuildingMerchantBehavior {

};

#define shok_vtp_GGL_CMarketBehavior (void*)0x775CCC
struct shok_GGL_CMarketBehavior : shok_EGL_CGLEBehavior {
private:
	int u;
public:
	int SellResourceType, BuyResourceType;
	float BuyAmount, SellAmount, ProgressAmount; // prog max is buyam+sellam
};


// entities
#define shok_vtp_EGL_CGLEEntity (void*)0x783E74
struct shok_EGL_CGLEEntity : shok_object {
private:
	int u1;
public:
	int EntityId;
private:
	int u2;
public:
	int EntityType;
	int ModelOverride;
	int PlayerId;
private:
	int u3[15];
public:
	shok_positionRot Position; // 22
public:
	float Scale; // 25
private:
	int u4;
public:
	byte DefaultBehavourFlag, UserControlFlag, UnattackableFlag;
private:
	byte u20;
public:
	byte SelectableFlag, f1, VisibleFlag;
private:
	byte u21;
	int u22;
public:
	std::vector<shok_EGL_CGLEBehavior*, shok_allocator<shok_EGL_CGLEBehavior*>> BehaviorList; // 30, first field in 31
	int CurrentState, EntityStateBehaviors, TaskListId, TaskIndex; // la37
private:
	int u25[12]; // la49
public:
	int CurrentHealth;
	char* ScriptName;
	char* ScriptCommandLine;
	float Exploration;
	int TaskListStart; // 54
private:
	int u26[2];
public:
	int InvulnerabilityAndSuspended; // 57
	int SuspensionTurn;
	int ScriptingValue; // 59
private:
	int u27[3];
public:
	int StateChangeCounter; // 63
	int TaskListChangeCounter;
	int NumberOfAuraEffects; // 65

private:
	shok_EGL_CGLEBehavior* SearchBehavior(void* vt);
	shok_EGL_CGLEBehavior* SearchBehavior(void** vts, int num);
public:
	shok_GGL_CSoldierBehavior* GetSoldierBehavior();
	shok_GGL_CLeaderBehavior* GetLeaderBehavior();
	shok_GGL_CBehaviorDefaultMovement* GetMovementBehavior();
	shok_GGL_CBarrackBehavior* GetBarrackBehavior();
	shok_GGL_CCamouflageBehavior* GetCamoAbilityBehavior();
	shok_GGL_CThiefCamouflageBehavior* GetCamoThiefBehavior();
	shok_GGL_CHeroBehavior* GetHeroBehavior();
	shok_GGL_CLimitedLifespanBehavior* GetLimitedLifespanBehavior();
	shok_GGL_CWorkerBehavior* GetWorkerBehavior();
	shok_GGL_CMarketBehavior* GetMarketBehavior();
	shok_GGL_CThiefBehavior* GetThiefBehavior();

	bool IsEntityInCategory(int cat);
	bool IsMovingEntity();
	shok_GGlue_CGlueEntityProps* GetEntityType();

	int EventGetDamage();
	int EventGetArmor();
	int EventGetMaxWorktime();
	int EventGetById(int id);
};

#define shok_vtp_EGL_CMovingEntity (void*)0x783F84
struct shok_EGL_CMovingEntity : shok_EGL_CGLEEntity {
	shok_position TargetPosition; // la67
	byte TargetRotationValid;
private:
	byte u[3];
public:
	float TargetRotation;
	int MovementState; // 70
};

#define shok_vtp_GGL_CEvadingEntity (void*)0x770A7C
struct shok_GGL_CEvadingEntity : shok_EGL_CMovingEntity {};

#define shok_vtp_GGL_CSettler (void*)0x76E3CC
struct shok_GGL_CSettler : shok_GGL_CEvadingEntity {
private:
	int u[4];
public:
	int TimeToWait, HeadIndex, HeadParams; // la77
private:
	int u2[7];
public:
	float ExperiencePoints; // 85
	int ExperienceLevel;
private:
	int u3[2];
public:
	float MovingSpeed; //89
private:
	int u4;
public:
	float Damage;
private:
	int u5;
public:
	float Dodge;
private:
	int u6;
public:
	float Motivation;
private:
	int u7;
public:
	float Armor;
private:
	int u8;
public:
	float CurrentAmountSoldiers;
private:
	int u9;
public:
	float MaxAmountSoldiers;
private:
	int u10;
public:
	float DamageBonus;
private:
	int u11;
public:
	float ExplorationCache;
private:
	int u12;
public:
	float MaxAttackRange;
private:
	int u13;
public:
	float AutoAttackRange;
private:
	int u14;
public:
	float HealingPoints;
private:
	int u15;
public:
	float MissChance;
private:
	int u16;
public:
	float Healthbar; // 115
private:
	int u17[7];
public:
	float SomeIntRegardingTaskLists; // 123
private:
	int u18[3];
public:
	int LeaderId;
private:
	int u19[2];
public:
	int OverheadWidget;
	int ExperienceClass;
private:
	int u20[2];
public:
	int BlessBuff, NPCMarker, LeaveBuildingTurn; //la136
};

#define shok_vtp_GGL_CAnimal (void*)0x778F7C
struct shok_GGL_CAnimal : shok_EGL_CMovingEntity {

};

#define shok_vtp_GGL_CResourceDoodad (void*)0x76FEA4
struct shok_GGL_CResourceDoodad : shok_EGL_CGLEEntity {
	int ResourceType, ResourceAmount, ResourceAmountAdd;
};

#define shok_vtp_GGL_CBuilding (void*)0x76EB94
struct shok_GGL_CBuilding : shok_EGL_CGLEEntity {
	shok_position ApproachPosition, LeavePosition; // fi 66
	byte IsActive, IsRegistered, IsUpgrading, IsOvertimeActive, HQAlarmActive;
private:
	byte u[3];
public:
	int MaxNumWorkers, CurrentTechnology, LatestAttackTurn, MostRecentDepartureTurn;
	float BuildingHeight, Helathbar, UpgradeProgress; //la78
private:
	int u2[2];
public:
	int NumberOfRepairingSerfs;
private:
	int u3;
public:
	int ConstructionSiteType; // 83
};

#define shok_vtp_GGL_CBridgeEntity (void*)0x77805C
struct shok_GGL_CBridgeEntity : shok_GGL_CBuilding {};

#define shok_vtp_EGL_CAmbientSoundEntity (void*)0x78568C
struct shok_EGL_CAmbientSoundEntity : shok_EGL_CGLEEntity {
private:
	int u;
public:
	int AmbientSoundType;
};


// c manager
struct shok_ECS_CManager : shok_object {
	void ReloadCutscene(const char* path);
};



// effect manager
struct shok_effectCreatorData {
public:
	int CreatorType, EffectType;
private:
	int Zero1 = 0;
public:
	int PlayerID;
	shok_position StartPos, CurrentPos, TargetPos;
private:
	int Zero2 = 0;
public:
	int AttackerID, TargetID, Damage;
	float DamageRadius;
private:
	int Zero3 = 0, Zero4 = 0, Zero5 = 0;
};

struct shok_EGL_CGLEEffectManager : shok_object {
	bool IsEffectValid(int id);
};

struct shok_BB_CIDManagerEx : shok_object {
	int GetAnimIdByName(const char* name);
};


// game logic
struct shok_EGL_CGLEGameLogic : shok_object {
	int CreateEffect(shok_effectCreatorData* data);
};


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

// global funcs
extern shok_EGL_CGLEEntity* (_stdcall *shok_eid2obj)(int id);

extern void(_stdcall* shok_SetHighPrecFPU)();

extern void(* shok_entityHurtEntity)(shok_EGL_CGLEEntity* attackerObj, shok_EGL_CGLEEntity* targetObj, int damage);

extern int(*shok_getEntityIdByScriptName)(const char* str);

extern bool(_cdecl* shok_EntityIsSettler)(shok_EGL_CGLEEntity* e);

extern bool(_cdecl* shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e);

extern bool(_cdecl* shok_EntityIsResourceDoodad)(shok_EGL_CGLEEntity* e);

extern int(_cdecl* shok_EntityGetProvidedResourceByID)(int id);

// global objects
extern shok_ECS_CManager*** shok_ECS_CManagerObject;

extern shok_EGL_CGLEGameLogic** shok_EGL_CGLEGameLogicObject;

extern int shok_effectCreatorData_CreatorType_Projectile;

extern shok_EGL_CGLEEffectManager** shok_EGL_CGLEEffectManagerObject;

extern shok_BB_CIDManagerEx** shok_BB_CIDManagerExObj;

extern shok_EGL_CGLEEntityManager** shok_EGL_CGLEEntityManagerObj;

extern shok_EGL_CGLEEntitiesProps** shok_EGL_CGLEEntitiesPropsObj;

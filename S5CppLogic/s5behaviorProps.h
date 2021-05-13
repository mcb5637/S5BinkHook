#pragma once
#include "s5data.h"

struct shok_EGL_CGLEBehaviorProps : shok_object {
	PADDING(4);
	int BehaviorIndex, BehaviorClass;

	static inline constexpr int vtp = 0x772A2C;
};

struct shok_EGL_CMovementBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float MovementSpeed, TurningSpeed;
	int MoveTaskList, MoveIdleAnim;

	static inline constexpr int vtp = 0x784938;
};

struct shok_GGL_CHeroAbilityProps : shok_EGL_CGLEBehaviorProps {
	int RechargeTimeSeconds;

	static inline constexpr int vtp = 0x773774;
};

struct shok_GGL_CCamouflageBehaviorProps : shok_GGL_CHeroAbilityProps {
	int DurationSeconds;
	float DiscoveryRange;

	static inline constexpr int vtp = 0x7778D8;
};

struct shok_GGL_CHeroHawkBehaviorProps : shok_GGL_CHeroAbilityProps {
	int HawkType;
	float HawkMaxRange;

	static inline constexpr int vtp = 0x77672C;
};

struct shok_GGL_CInflictFearAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation, FlightDuration;
	float Range, FlightRange;

	static inline constexpr int vtp = 0x776674;
};

struct shok_GGL_CCannonBuilderBehaviorProps : shok_GGL_CHeroAbilityProps {
	int TaskList;

	static inline constexpr int vtp = 0x777510;
};

struct shok_GGL_CRangedEffectAbilityProps : shok_GGL_CHeroAbilityProps {
	byte AffectsOwn, AffectsFriends, AffectsNeutrals, AffectsHostiles, AffectsMilitaryOnly, AffectsLongRangeOnly;
private:
	byte u[2];
public:
	float Range;
	int DurationSeconds;
	float DamageFactor, ArmorFactor, HealthRecoveryFactor;
	int Effect, HealEffect;

	static inline constexpr int vtp = 0x774E9C;

	bool IsDefensive();
	bool IsAggressive();
	bool IsHeal();
};

struct shok_GGL_CCircularAttackProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation, DamageClass, Damage;
	float Range;

	static inline constexpr int vtp = 0x7774A0;
};

struct shok_GGL_CSummonBehaviorProps : shok_GGL_CHeroAbilityProps {
	int SummonedEntityType, NumberOfSummonedEntities, SummonTaskList;

	static inline constexpr int vtp = 0x773C50;
};

struct shok_GGL_CConvertSettlerAbilityProps : shok_GGL_CHeroAbilityProps {
	int ConversionTaskList;
	float HPToMSFactor, ConversionStartRange, ConversionMaxRange;

	static inline constexpr int vtp = 0x7772D0;
};

struct shok_GGL_CSniperAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range, DamageFactor;

	static inline constexpr int vtp = 0x7745E8;
};

struct shok_GGL_CMotivateWorkersAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range;
	int WorkTimeBonus, Effect;

	static inline constexpr int vtp = 0x775788;
};

struct shok_GGL_CShurikenAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range, MaxArcDegree;
	int NumberShuriken, ProjectileType;
	float ProjectileOffsetHeight, ProjectileOffsetFront, ProjectileOffsetRight;
	int DamageClass, DamageAmount;

	static inline constexpr int vtp = 0x7746B4;
};

struct shok_GGL_CKegPlacerBehaviorProperties : shok_GGL_CHeroAbilityProps {
	static inline constexpr int vtp = 0x7763B4;
};

struct shok_GGL_CAbilityScoutBinocularProps : shok_GGL_CHeroAbilityProps {
	static inline constexpr int vtp = 0x779254;
};

struct shok_GGL_CTorchPlacerBehaviorProperties : shok_GGL_CHeroAbilityProps {
	static inline constexpr int vtp = 0x773790;
};

struct shok_GGL_CPointToResourceBehaviorProperties : shok_GGL_CHeroAbilityProps {
	static inline constexpr int vtp = 0x774FEC;
};

struct shok_GGL_CSentinelBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float Range;

	static inline constexpr int vtp = 0x774BAC;
};

struct shok_GGL_CGLAnimationBehaviorExProps : shok_EGL_CGLEBehaviorProps {
	int SuspensionAnimation, AnimSet;

	static inline constexpr int vtp = 0x776C48;
};

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

	static inline constexpr int vtp = 0x772B90;
};

struct shok_GGL_CBattleBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int BattleTaskList, NormalAttackAnim1, NormalAttackAnim2, CounterAttackAnim, FinishingMoveAnim, MissAttackAnim, BattleIdleAnim, BattleWalkAnim;
	int HitAnim, DamageClass, DamageAmount, MaxDamageRandomBonus;
	float DamageRange;
	int ProjectileEffectID;
	float ProjectileOffsetFront, ProjectileOffsetRight, ProjectileOffsetHeight;
	int BattleWaitUntil, MissChance;
	float MaxRange, MinRange;

	static inline constexpr int vtp = 0x7731C0;
};

struct shok_GGL_CLeaderBehaviorProps : shok_GGL_CBattleBehaviorProps {
	int SoldierType, BarrackUpgradeCategory;
	float HomeRadius;
	int HealingPoints, HealingSeconds;
	float AutoAttackRange, UpkeepCosts;

	static inline constexpr int vtp = 0x775FA4;
};

struct shok_GGL_CSoldierBehaviorProps : shok_GGL_CBattleBehaviorProps {
	static inline constexpr int vtp = 0x773D10;
};

struct shok_GGL_CSerfBattleBehaviorProps : shok_GGL_CBattleBehaviorProps {
	static inline constexpr int vtp = 0x774B50;
};

struct shok_GGL_CBattleSerfBehaviorProps : shok_GGL_CLeaderBehaviorProps {
	static inline constexpr int vtp = 0x77889C;
};

struct shok_serf_extractionInfo {
	int ResourceEntityType;
	float Delay;
	int Amount;
};
struct shok_GGL_CSerfBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int ResourceSearchRadius, ApproachConbstructionSiteTaskList, TurnIntoBattleSerfTaskList;
	vector_padding;
	std::vector<shok_serf_extractionInfo, shok_allocator<shok_serf_extractionInfo>> ExtractionInfo;

	static inline constexpr int vtp = 0x774A14;
};

struct shok_limitedAttachmentProps {
	shok_string Type;
	int Limit;
	bool IsActive;
	PADDING(3);
};
struct shok_GGL_CLimitedAttachmentBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	vector_padding;
	std::vector<shok_limitedAttachmentProps, shok_allocator<shok_limitedAttachmentProps>> Attachments;

	static inline constexpr int vtp = 0x775DE4;
};

struct shok_idleAnimProps {
	int AnimID, Frequency;
};
struct shok_GGL_CFormationBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	vector_padding;
	std::vector<shok_idleAnimProps, shok_allocator<shok_idleAnimProps>> IdleAnims;

	static inline constexpr int vtp = 0x776DE4;
};

struct shok_GGL_CCamperBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Range;

	static inline constexpr int vtp = 0x7777D4;
};

struct shok_GGL_CGLBehaviorPropsDying : shok_EGL_CGLEBehaviorProps {
	int DyingTaskList;

	static inline constexpr int vtp = 0x778634;
};

struct shok_GGL_CHeroBehaviorProps : shok_EGL_CGLEBehaviorProps {
	static inline constexpr int vtp = 0x7767F4;
};

struct shok_GGL_CBombBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Radius, Delay;
	int Damage, ExplosionEffectID;
	vector_padding;
	std::vector<int, shok_allocator<int>> AffectedEntityTypes;

	static inline constexpr int vtp = 0x7784A0;
};

struct shok_GGL_CKegBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Radius;
	int Damage;
	float Delay;
	int DamagePercent, ExplosionEffectID;

	static inline constexpr int vtp = 0x776558;
};

struct shok_GGL_CThiefBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int SecondsNeededToSteal, MinimumAmountToSteal, MaximumAmountToSteal, CarryingModelID, StealGoodsTaskList, SecureGoodsTaskList;

	static inline constexpr int vtp = 0x7739E0;
};

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

	static inline constexpr int vtp = 0x778CD4;
};

struct shok_resourceRefinerEfficencyUpgrade {
	int Technology;
	float Factor;
};
struct shok_GGL_CResourceRefinerBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int ResourceType;
	float InitialFactor;
	vector_padding;
	std::vector<shok_resourceRefinerEfficencyUpgrade, shok_allocator<shok_resourceRefinerEfficencyUpgrade>> Efficiency;
	int SupplierCategory;

	static inline constexpr int vtp = 0x774C24;
};

struct shok_GGL_CAffectMotivationBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float MotivationEffect;

	static inline constexpr int vtp = 0x7791D4;
};

struct shok_GGL_CLimitedLifespanBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int LifespanSeconds;

	static inline constexpr int vtp = 0x775DE4;
};

struct shok_GGL_CBarrackBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int TrainingTaskList1, TrainingTaskList2, TrainingTaskList3;
private:
	int MaxTrainingNumber;
public:
	int LeaveTaskList;
	float TrainingTime;

	static inline constexpr int vtp = 0x778B34;
};

struct shok_GGL_CBuildingMerchantBehaviorProps : shok_EGL_CGLEBehaviorProps {
	static inline constexpr int vtp = 0x7783B8;
};

struct shok_GGL_CServiceBuildingBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	// initial amount at 4?
	static inline constexpr int vtp = 0x774830;
};

struct shok_GGL_CMineBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int AmountToMine;

	static inline constexpr int vtp = 0x77581C;
};

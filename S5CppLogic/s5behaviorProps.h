#pragma once
#include "s5data.h"

struct shok_EGL_CGLEBehaviorProps : shok_object {
	PADDING(4);
	int BehaviorIndex, BehaviorClass;

	static inline constexpr int vtp = 0x772A2C;
	static inline constexpr int TypeDesc = 0x813728;
};

struct shok_EGL_CMovementBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float MovementSpeed, TurningSpeed;
	int MoveTaskList, MoveIdleAnim;

	static inline constexpr int vtp = 0x784938;
	static inline constexpr int TypeDesc = 0x8373E4;
};

struct shok_GGL_CHeroAbilityProps : shok_EGL_CGLEBehaviorProps {
	int RechargeTimeSeconds; // 4

	static inline constexpr int vtp = 0x773774;
	static inline constexpr int TypeDesc = 0x816EA8;
};

struct shok_GGL_CCamouflageBehaviorProps : shok_GGL_CHeroAbilityProps {
	int DurationSeconds; // 5
	float DiscoveryRange;

	static inline constexpr int vtp = 0x7778D8;
	static inline constexpr int TypeDesc = 0x823D28;
};

struct shok_GGL_CHeroHawkBehaviorProps : shok_GGL_CHeroAbilityProps {
	int HawkType;
	float HawkMaxRange;

	static inline constexpr int vtp = 0x77672C;
	static inline constexpr int TypeDesc = 0x81FE58;
};

struct shok_GGL_CHawkBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int ExploreTaskList; //4
	float MaxCirclingDistance;

	static inline constexpr int vtp = 0x776A7C;
	static inline constexpr int TypeDesc = 0x82079C;
};

struct shok_GGL_CInflictFearAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation, FlightDuration;
	float Range, FlightRange;

	static inline constexpr int vtp = 0x776674;
	static inline constexpr int TypeDesc = 0x81FBAC;
};

struct shok_GGL_CCannonBuilderBehaviorProps : shok_GGL_CHeroAbilityProps {
	int TaskList;

	static inline constexpr int vtp = 0x777510;
	static inline constexpr int TypeDesc = 0x823224;
};

struct shok_GGL_CRangedEffectAbilityProps : shok_GGL_CHeroAbilityProps {
	byte AffectsOwn, AffectsFriends, AffectsNeutrals, AffectsHostiles, AffectsMilitaryOnly, AffectsLongRangeOnly;
	PADDING(2);
	float Range;
	int DurationSeconds; // 8
	float DamageFactor, ArmorFactor, HealthRecoveryFactor;
	int Effect, HealEffect;

	static inline constexpr int vtp = 0x774E9C;
	static inline constexpr int TypeDesc = 0x81B780;

	bool IsDefensive();
	bool IsAggressive();
	bool IsHeal();
};

struct shok_GGL_CCircularAttackProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation, DamageClass, Damage;
	float Range;

	static inline constexpr int vtp = 0x7774A0;
	static inline constexpr int TypeDesc = 0x82305C;
};

struct shok_GGL_CSummonBehaviorProps : shok_GGL_CHeroAbilityProps {
	int SummonedEntityType, NumberOfSummonedEntities, SummonTaskList;

	static inline constexpr int vtp = 0x773C50;
	static inline constexpr int TypeDesc = 0x817DE4;
};

struct shok_GGL_CSummonedBehaviorProps : shok_EGL_CGLEBehaviorProps {

	static inline constexpr int vtp = 0x773BF0;
	static inline constexpr int TypeDesc = 0x817D58;
};

struct shok_GGL_CConvertSettlerAbilityProps : shok_GGL_CHeroAbilityProps {
	int ConversionTaskList;
	float HPToMSFactor, ConversionStartRange, ConversionMaxRange;

	static inline constexpr int vtp = 0x7772D0;
	static inline constexpr int TypeDesc = 0x8227A8;
};

struct shok_GGL_CConvertBuildingAbilityProps : shok_GGL_CHeroAbilityProps {
	int ConversionTaskList;
	float HPToMSFactor;

	static inline constexpr int vtp = 0x7773D8;
	static inline constexpr int TypeDesc = 0x822B68;
};

struct shok_GGL_CSniperAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range, DamageFactor; // 7

	static inline constexpr int vtp = 0x7745E8;
	static inline constexpr int TypeDesc = 0x81901C;
};

struct shok_GGL_CMotivateWorkersAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range; // 7
	int WorkTimeBonus, Effect;

	static inline constexpr int vtp = 0x775788;
	static inline constexpr int TypeDesc = 0x81C3D8;
};

struct shok_GGL_CShurikenAbilityProps : shok_GGL_CHeroAbilityProps {
	int TaskList, Animation;
	float Range, MaxArcDegree; // 7
	int NumberShuriken, ProjectileType;
	float ProjectileOffsetHeight, ProjectileOffsetFront, ProjectileOffsetRight;
	int DamageClass, DamageAmount;

	static inline constexpr int vtp = 0x7746B4;
	static inline constexpr int TypeDesc = 0x8192E8;
};

struct shok_GGL_CKegPlacerBehaviorProperties : shok_GGL_CHeroAbilityProps {
	int SecondsNeededToArm; // 5
	int SecondsNeededToDisarm;
	int PlaceKegTaskList, DisarmKegTaskList, KegEntityType;

	static inline constexpr int vtp = 0x7763B4;
	static inline constexpr int TypeDesc = 0x81F0AC;
};

struct shok_GGL_CAbilityScoutBinocularProps : shok_GGL_CHeroAbilityProps {
	int NumberOfExplorationEntities; // 5
	float SpacingOfExplorationEntities; // int/float?
	int ExplorationRangeOfExplorationEntities; // int/float?
	int ExplorationEntityType, ExplorationTaskList;

	static inline constexpr int vtp = 0x779254;
	static inline constexpr int TypeDesc = 0x829218;
};

struct shok_GGL_CTorchPlacerBehaviorProperties : shok_GGL_CHeroAbilityProps {
	int TorchEntity; // 5
	int TaskList;

	static inline constexpr int vtp = 0x773790;
	static inline constexpr int TypeDesc = 0x816E74;
};

struct shok_GGL_CTorchBehaviorProperties : shok_EGL_CGLEBehaviorProps {

	static inline constexpr int vtp = 0x773880;
	static inline constexpr int TypeDesc = 0x817148;
};

struct shok_GGL_CPointToResourceBehaviorProperties : shok_GGL_CHeroAbilityProps {
	int TaskList; // 5
	float SearchRadius;

	static inline constexpr int vtp = 0x774FEC;
	static inline constexpr int TypeDesc = 0x81BBB0;
};

struct shok_GGL_CSentinelBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float Range;

	static inline constexpr int vtp = 0x774BAC;
	static inline constexpr int TypeDesc = 0x81AB94;
};

struct shok_GGL_CGLAnimationBehaviorExProps : shok_EGL_CGLEBehaviorProps { // there is EGL::CAnimationBehaviorProps, but its not used anywhere else vt 776BB4
	int SuspensionAnimation, AnimSet; // 4
	// list/set of {int Model, AnimSet; } ExtraSet

	static inline constexpr int vtp = 0x776C48;
	static inline constexpr int TypeDesc = 0x820B90;
};

struct shok_GGL_CWorkerBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int WorkTaskList, WorkIdleTaskList; // 4
	int WorkLeaveTaskList;
	int WorkWaitUntil, EatTaskList, EatIdleTaskList, EatWait, RestTaskList, RestIdleTaskList, RestWait;
	int IdleTaskList;
	int LeaveTaskList; // 15
	float AmountResearched;
	float WorkTimeChangeWork;
	float WorkTimeChangeFarm, WorkTimeChangeResidence, WorkTimeChangeCamp; // la20
	int WorkTimeMaxCangeFarm, WorkTimeMaxChangeResidence;
	float ExhaustedWorkMotivationMalus;
	int TransportAmount, TransportModel, TransportAnim, ResourceToRefine;
	float BridgeProgress, MaximumDistanceToBridge; // la 29

	static inline constexpr int vtp = 0x772B90;
	static inline constexpr int TypeDesc = 0x813B40;
};

struct shok_GGL_CBattleBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int BattleTaskList, NormalAttackAnim1, NormalAttackAnim2, CounterAttackAnim, FinishingMoveAnim, MissAttackAnim, BattleIdleAnim, BattleWalkAnim; // 4
	int HitAnim, DamageClass, DamageAmount, MaxDamageRandomBonus; // 12
	float DamageRange;
	int ProjectileEffectID;
	float ProjectileOffsetFront, ProjectileOffsetRight, ProjectileOffsetHeight;
	int BattleWaitUntil, MissChance;
	float MaxRange, MinRange;

	static inline constexpr int vtp = 0x7731C0;
	static inline constexpr int TypeDesc = 0x815F68;
};

struct shok_GGL_CLeaderBehaviorProps : shok_GGL_CBattleBehaviorProps {
	int SoldierType, BarrackUpgradeCategory;
	float HomeRadius;
	int HealingPoints, HealingSeconds; // 28
	float AutoAttackRange, UpkeepCosts;

	static inline constexpr int vtp = 0x775FA4;
	static inline constexpr int TypeDesc = 0x81D970;
};

struct shok_GGL_CSoldierBehaviorProps : shok_GGL_CBattleBehaviorProps {
	static inline constexpr int vtp = 0x773D10;
	static inline constexpr int TypeDesc = 0x817FE8;
};

struct shok_GGL_CSerfBattleBehaviorProps : shok_GGL_CBattleBehaviorProps {
	static inline constexpr int vtp = 0x774B50;
	static inline constexpr int TypeDesc = 0x81A4D0;
};

struct shok_GGL_CBattleSerfBehaviorProps : shok_GGL_CLeaderBehaviorProps {
	int TurnIntoSerfTaskList; //32

	static inline constexpr int vtp = 0x77889C;
	static inline constexpr int TypeDesc = 0x8268F0;
};

struct shok_serf_extractionInfo {
	int ResourceEntityType;
	float Delay;
	int Amount;
};
struct shok_GGL_CSerfBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int ResourceSearchRadius, ApproachConbstructionSiteTaskList, TurnIntoBattleSerfTaskList; //4
	vector_padding;
	std::vector<shok_serf_extractionInfo, shok_allocator<shok_serf_extractionInfo>> ExtractionInfo;

	static inline constexpr int vtp = 0x774A14;
	static inline constexpr int TypeDesc = 0x819B1C;
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
	static inline constexpr int TypeDesc = 0x81D080;
};

struct shok_idleAnimProps {
	int AnimID, Frequency;
};
struct shok_GGL_CFormationBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	vector_padding;
	std::vector<shok_idleAnimProps, shok_allocator<shok_idleAnimProps>> IdleAnims;

	static inline constexpr int vtp = 0x776DE4;
	static inline constexpr int TypeDesc = 0x8212CC;
};

struct shok_GGL_CCamperBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Range;

	static inline constexpr int vtp = 0x7777D4;
	static inline constexpr int TypeDesc = 0x8237CC;
};

struct shok_GGL_CCampBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	vector_padding;
	std::vector<shok_position> Slot;
	int RemoveDelay;

	static inline constexpr int vtp = 0x777854;
	static inline constexpr int TypeDesc = 0x823770;
};

struct shok_GGL_CGLBehaviorPropsDying : shok_EGL_CGLEBehaviorProps {
	int DyingTaskList;

	static inline constexpr int vtp = 0x778634;
	static inline constexpr int TypeDesc = 0x825F38;
};

struct shok_GGL_CHeroBehaviorProps : shok_EGL_CGLEBehaviorProps {
	static inline constexpr int vtp = 0x7767F4;
	static inline constexpr int TypeDesc = 0x820048;
};

struct shok_GGL_CBombBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Radius, Delay;
	int Damage, ExplosionEffectID;
	vector_padding;
	std::vector<int, shok_allocator<int>> AffectedEntityTypes;

	static inline constexpr int vtp = 0x7784A0;
	static inline constexpr int TypeDesc = 0x82587C;
};

struct shok_GGL_CKegBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float Radius; //4
	int Damage;
	float Delay;
	int DamagePercent, ExplosionEffectID;

	static inline constexpr int vtp = 0x776558;
	static inline constexpr int TypeDesc = 0x81F6DC;
};

struct shok_GGL_CThiefBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int SecondsNeededToSteal, MinimumAmountToSteal, MaximumAmountToSteal, CarryingModelID, StealGoodsTaskList, SecureGoodsTaskList;

	static inline constexpr int vtp = 0x7739E0;
	static inline constexpr int TypeDesc = 0x817400;
};

struct shok_GGL_CAutoCannonBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int NumberOfShots; //4
	float RotationSpeed;
	int CannonBallEffectType; // 6
	float ProjectileOffsetFront, ProjectileOffsetRight;
	int ImpactEffectType; // 9
	int ReloadTime;
	float MaxAttackRange, MinRange;
	int DamageClass, DamageAmount;
	float DamageRange;
	int BattleTaskList, SelfDestructTaskList; // 16


	static inline constexpr int vtp = 0x778CD4;
	static inline constexpr int TypeDesc = 0x828088;
};

struct shok_GGL_CFoundationBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int TopEntityType; //4
	float XOffset, YOffset; //int?

	static inline constexpr int vtp = 0x776D08;
	static inline constexpr int TypeDesc = 0x820F18;
};

struct shok_resourceRefinerEfficencyUpgrade {
	int Technology;
	float Factor;
};
struct shok_GGL_CResourceRefinerBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int ResourceType; // 4
	float InitialFactor;
	vector_padding;
	std::vector<shok_resourceRefinerEfficencyUpgrade, shok_allocator<shok_resourceRefinerEfficencyUpgrade>> Efficiency;
	int SupplierCategory;

	static inline constexpr int vtp = 0x774C24;
	static inline constexpr int TypeDesc = 0x81AD48;
};

struct shok_GGL_CAffectMotivationBehaviorProps : shok_EGL_CGLEBehaviorProps {
	float MotivationEffect; //4

	static inline constexpr int vtp = 0x7791D4;
	static inline constexpr int TypeDesc = 0x828FF0;
};

struct shok_GGL_CLimitedLifespanBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int LifespanSeconds;

	static inline constexpr int vtp = 0x775DE4;
	static inline constexpr int TypeDesc = 0x81D080;
};

struct shok_GGL_CBarrackBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int TrainingTaskList1, TrainingTaskList2, TrainingTaskList3; //4
private:
	int MaxTrainingNumber;
public:
	int LeaveTaskList;
	float TrainingTime; //9

	static inline constexpr int vtp = 0x778B34;
	static inline constexpr int TypeDesc = 0x827F48;
};

struct shok_GGL_CFoundryBehaviorProperties_CannonInfo {
	int Cannon;
	int TaskList;
};
struct shok_GGL_CFoundryBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	vector_padding;
	std::vector<shok_GGL_CFoundryBehaviorProperties_CannonInfo, shok_allocator<shok_GGL_CFoundryBehaviorProperties_CannonInfo>> CannonInfo;

	static inline constexpr int vtp = 0x778B80;
	static inline constexpr int TypeDesc = 0x827F78;
};

struct shok_GGL_CBuildingMerchantBehaviorProps : shok_EGL_CGLEBehaviorProps {
	static inline constexpr int vtp = 0x7783B8;
	static inline constexpr int TypeDesc = 0x8251B4;
};

struct shok_GGL_CSettlerMerchantBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int InitTaskList, MoveOutTaskList, MoveInTaskList; //4

	static inline constexpr int vtp = 0x7747DC;
	static inline constexpr int TypeDesc = 0x8196E0;
};

struct shok_GGL_CServiceBuildingBehaviorProperties_WorkModifier {
	int Technology;
	float WorkAmount;
};
struct shok_GGL_CServiceBuildingBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	float InitialWorkAmount; // int?
	vector_padding;
	std::vector<shok_GGL_CServiceBuildingBehaviorProperties_WorkModifier> Work;

	static inline constexpr int vtp = 0x774830;
	static inline constexpr int TypeDesc = 0x816C3C;
};

struct shok_GGL_CWorkerFleeBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int FlightTaskList; //4

	static inline constexpr int vtp = 0x772A60;
	static inline constexpr int TypeDesc = 0x81374C;
};

struct shok_GGL_CWorkerAlarmModeBehaviorProps : shok_EGL_CGLEBehaviorProps {
	int GoToDefendableBuildingTaskList; //4

	static inline constexpr int vtp = 0x773544;
	static inline constexpr int TypeDesc = 0x8164B4;
};

struct shok_GGL_CDefendableBuildingBehaviorProps : shok_EGL_CGLEBehaviorProps {

	static inline constexpr int vtp = 0x777234;
	static inline constexpr int TypeDesc = 0x8223F4;
};

struct shok_GGL_CGLResourceDoodadBehaviorProps : shok_EGL_CGLEBehaviorProps {
	struct {
		PADDINGI(1); // vtable?
		int Amount; //float?
		shok_ResourceType Good;
	} Resource; // if im unlucky this is a list or set or something similar stupid

	static inline constexpr int vtp = 0x774D40;
	static inline constexpr int TypeDesc = 0x81B170;
};

struct shok_GGL_CResourceDependentBuildingBehaviorProperties_data {
	int Resource, Type;
};
struct shok_GGL_CResourceDependentBuildingBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	vector_padding;
	std::vector<shok_GGL_CResourceDependentBuildingBehaviorProperties_data> Building;

	static inline constexpr int vtp = 0x774DCC;
	static inline constexpr int TypeDesc = 0x81B4FC;
};

struct shok_GGL_CReplaceableEntityBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int EntityType;

	static inline constexpr int vtp = 0x774E24;
	static inline constexpr int TypeDesc = 0x81B6A8;
};

struct shok_GGL_CMineBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int AmountToMine;

	static inline constexpr int vtp = 0x77581C;
	static inline constexpr int TypeDesc = 0x81C77C;
};

struct shok_GGL_CKeepBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	int NumSerfs; // used anywhere?

	static inline constexpr int vtp = 0x776600;
	static inline constexpr int TypeDesc = 0x81FA34;
};

struct shok_GGL_CNeutralBridgeBehaviorProperties : shok_EGL_CGLEBehaviorProps {
	//shok_constructionInfo ConstructionInfo;

	static inline constexpr int vtp = 0x779B9C;
	static inline constexpr int TypeDesc = 0x8298A4;
};

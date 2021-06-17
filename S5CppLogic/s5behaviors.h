#pragma once
#include "s5data.h"

struct shok_EGL_CGLEBehavior : shok_object { // no vtable
	int SlotIndex, EntityId;
private:
	int PropPointer; // 3

public:
	static inline constexpr int TypeDesc = 0x813778;
};

struct shok_GGL_CBehaviorDefaultMovement : shok_EGL_CGLEBehavior {
private:
	int Counter;
public:
	float MovementSpeed, TurningSpeed, SpeedFactor;

	static inline constexpr int vtp = 0x7786AC;
	static inline constexpr int TypeDesc = 0x818DF0;

	// defined states: 12
	// defined events: 1100D, 1100E, 11017

	float GetMovementSpeed();
};

struct shok_GGL_CSettlerMovement : shok_GGL_CBehaviorDefaultMovement {
	PADDINGI(1);
	shok_positionRot PositionHiRes, Position;
	byte BlockingFlag;
	PADDING(3);

	static inline constexpr int vtp = 0x77471C;
	static inline constexpr int TypeDesc = 0x81961C;
};

struct shok_GGL_CLeaderMovement : shok_GGL_CBehaviorDefaultMovement {
	int MoveTaskList;
	shok_positionRot NextWayPoint, LastTurnPos;
	byte IsPathingUsed, IsMoveFinished;
	PADDING(2);

	static inline constexpr int vtp = 0x775ED4;
	static inline constexpr int TypeDesc = 0x81D7E4;
};

struct shok_GGL_CSoldierMovement : shok_GGL_CBehaviorDefaultMovement {
	static inline constexpr int vtp = 0x77438C;
	static inline constexpr int TypeDesc = 0x818E1C;
};

struct shok_GGL_CHeroAbility : shok_EGL_CGLEBehavior { // no vtable
	PADDINGI(1);
	int SecondsCharged; // 5

	// defined events: HeroAbility_XXX, Behavior_Tick

	static inline constexpr int TypeDesc = 0x816E2C;
};

struct shok_GGL_CCamouflageBehavior : shok_GGL_CHeroAbility {
	PADDINGI(2);
	int InvisibilityRemaining; // 8

	// defined events: Behavior_Tick, Camouflage_XXX, 1504A and 16013 return isinvisible, AttackEntity 1500E camo reset

	static inline constexpr int vtp = 0x7738F4;
	static inline constexpr int TypeDesc = 0x8172E8;
};

struct shok_GGL_CThiefCamouflageBehavior : shok_GGL_CCamouflageBehavior {
	int TimeToInvisibility;

	// defined events: ThiefCamouflage_Reset

	static inline constexpr int vtp = 0x773934;
	static inline constexpr int TypeDesc = 0x817310;
};

struct shok_GGL_CHeroHawkBehavior : shok_GGL_CHeroAbility {
	// defined events: HeroHawk_XXX, HeroAbility_Reset, HeroAbility_StandUpOrInit

	static inline constexpr int vtp = 0x7766F0;
	static inline constexpr int TypeDesc = 0x81FE84;
};

struct shok_GGL_CInflictFearAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int SecondsFearRemaining; // 7

	// defined events: InflictFear_XXX, Behavior_Tick, HeroAbility_Reset
	// defined tasks: TASK_SET_SPECIAL_ATTACK_ANIM, TASK_PERFORM_SPECIAL_ATTACK

	static inline constexpr int vtp = 0x776638;
	static inline constexpr int TypeDesc = 0x81FBD8;
};

struct shok_GGL_CBombPlacerBehavior : shok_GGL_CHeroAbility {
	shok_position StartPosition, TargetPosition;
	byte PlacedBomb; // 10
	PADDING(3);

	// defined events: BombPlacer_XXX, HeroAbility_Cancel
	// defined tasks: TASK_PLACE_BOMB, TASK_GO_TO_BOMB_TARGET, TASK_RETREAT_FROM_BOMB_TARGET

	static inline constexpr int vtp = 0x7783D8;
	static inline constexpr int TypeDesc = 0x8255D0;
};

struct shok_GGL_CCannonBuilderBehavior : shok_GGL_CHeroAbility {
	PADDINGI(1);
	shok_position StartPosition;
	int CannonType, FoundationType;
	byte PlacedCannon; // 11
	PADDING(3);

	// defined events: CannonBuilder_XXX, HeroAbility_Cancel, HeroAbility_Reset, 1600B some form of stop?->12002
	// defined tasks: TASK_GO_TO_CANNON_POSITION, TASK_BUILD_CANNON

	static inline constexpr int vtp = 0x7774D4;
	static inline constexpr int TypeDesc = 0x823254;
};

struct shok_GGL_CRangedEffectAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int SecondsRemaining; // 7

	// defined events: HeroAbility_Reset, Behavior_Tick, RangedEffect_XXX

	static inline constexpr int vtp = 0x774E54;
	static inline constexpr int TypeDesc = 0x81B7AC;
};

struct shok_GGL_CCircularAttack : shok_GGL_CHeroAbility {

	// defined events: CircularAttack_ActivateCommand
	// defined tasks: TASK_SET_SPECIAL_ATTACK_ANIM, TASK_PERFORM_SPECIAL_ATTACK

	static inline constexpr int vtp = 0x777464;
	static inline constexpr int TypeDesc = 0x823038;
};

struct shok_GGL_CSummonBehavior : shok_GGL_CHeroAbility {

	// defined events: HeroAbility_Reset, Summon_ActivateCommand
	// defined tasks: TASK_SUMMON_ENTITIES

	static inline constexpr int vtp = 0x773C10;
	static inline constexpr int TypeDesc = 0x817E0C;
};

struct shok_GGL_CConvertSettlerAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int TimeToConvert;

	// defined states: ComvertSettler
	// defined tasks: TASK_MOVE_TO_SETTLER_TO_CONVERT, TASK_CONVERT_SETTLER
	// defined events: ComvertSettler_ActivateCommand, 1500E, 16028, HeroAbility_Cancel

	static inline constexpr int vtp = 0x777294;
	static inline constexpr int TypeDesc = 0x8227D8;
};

struct shok_GGL_CSniperAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int TargetId; // 7

	// defined tasks: TASK_SET_SNIPE_ANIM, TASK_SNIPE, TASK_TURN_TO_SNIPER_TARGET
	// defined events: Sniper_XXX

	static inline constexpr int vtp = 0x7745AC;
	static inline constexpr int TypeDesc = 0x819044;
};

struct shok_GGL_CMotivateWorkersAbility : shok_GGL_CHeroAbility {

	// defined tasks: TASK_SET_MOTIVATE_ANIM, TASK_PERFORM_MOTIVATION
	// defined events: MotivateVorkers_ActivateCommand

	static inline constexpr int vtp = 0x77574C;
	static inline constexpr int TypeDesc = 0x81C408;
};

struct shok_GGL_CShurikenAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int TargetId;

	// defined tasks: TASK_SET_THROW_SHURIKEN_ANIM, TASK_THROW_SHURIKEN, TASK_TURN_TO_SHURIKEN_TARGET
	// defined events: Shuriken_XXX

	static inline constexpr int vtp = 0x774658;
	static inline constexpr int TypeDesc = 0x819310;
};

struct shok_GGL_CKegPlacerBehavior : shok_GGL_CHeroAbility {
	shok_position StartPosition, TargetPosition;
	byte PlacedKeg; // 10
	PADDING(3);
	int TurnsUntilArmed, TurnsUntilDisarmed; // 11

	// defined states: ThiefSabotage, ThiefDisarm
	// defined tasks: TASK_PLACE_KEG, TASK_GO_TO_KEG_TARGET, TASK_GO_TO_KEG, TASK_CHECK_GO_TO_KEG_TARGET_SUCCESS, TASK_RETREAT_FROM_KEG_TARGET, TASK_DISARM_KEG
	// defined events: KegPlacer_XXX, HeroAbility_Cancel

	static inline constexpr int vtp = 0x776368;
	static inline constexpr int TypeDesc = 0x81F084;
};

struct shok_GGL_CAbilityScoutBinocular : shok_GGL_CHeroAbility {
	PADDINGI(1);
	byte Active; // 7
	PADDING(3);
	shok_position ExlorationPosition;

	// defined tasks: TASK_EXPLORE, TASK_TURN_TO_EXPLORE_POSITION
	// defined events: Binocular_XXX, 11002 1500E cancel

	static inline constexpr int vtp = 0x779218;
	static inline constexpr int TypeDesc = 0x829248;
};

struct shok_GGL_CTorchPlacerBehavior : shok_GGL_CHeroAbility {
	shok_position StartPosition, TargetPosition; // 5
	byte PlacedTorch;
	PADDING(3);

	// defined tasks: TASK_GO_TO_TORCH_DESTINATION, TASK_PLACE_TORCH
	// defined events: HeroAbility_Cancel, TorchPlacer_PlaceTorch

	static inline constexpr int vtp = 0x773738;
	static inline constexpr int TypeDesc = 0x816E4C;
};

struct shok_GGL_CPointToResourceBehavior : shok_GGL_CHeroAbility {

	// defined states: ScoutPointToRes (maybe unused)
	// defined tasks: TASK_POINT_TO_RESOURCE
	// defined events: HeroAbility_Cancel (null), PointToResources_Activate

	static inline constexpr int vtp = 0x774FB0;
	static inline constexpr int TypeDesc = 0x81BB84;
};

struct shok_GGL_CSentinelBehavior : shok_EGL_CGLEBehavior {
	PADDINGI(1);
	int Urgency; // 5 ???

	// defined events: Behavior_Tick, Sentinel_GetUrgency

	static inline constexpr int vtp = 0x774B6C;
	static inline constexpr int TypeDesc = 0x81ABC0;
};

struct shok_GGL_CGLBehaviorAnimationEx : shok_EGL_CGLEBehavior {
	int Animation, AnimCategory, SuspendedAnimation, StartTurn, Duration; // 4
	byte PlayBackwards;
	PADDING(3);
	int TurnToWaitFor;
	float SpeedModifier; // 11
	PADDINGI(1); // 12 p to EGL::CBehaviorAnimation::CSlotAnimation
	PADDINGI(1); // p to props
	struct {
		PADDINGI(1);
		int TaskType; // 15
		int AnimID;
		int Category;
		byte PlayBackwards;
		PADDING(3);
	} AnimToRestore;
	int AnimSet; // 19

	// defined states: WaitForAnim
	// defined tasks: TASK_SET_ANIM (x2), TASK_WAIT_FOR_ANIM, TASK_RANDOM_WAIT_FOR_ANIM
	// defined events: Animation_XXX

	static inline constexpr int vtp = 0x776B64;
	static inline constexpr int TypeDesc = 0x820BE8;
};

struct shok_GGL_CBehaviorWalkCommand : shok_EGL_CGLEBehavior {
	shok_position WalkTarget;

	// defined tasks: TASK_WALK -> goes to ev 20003
	// defined events: MoveCommand_Move

	static inline constexpr int vtp = 0x7736A4;
	static inline constexpr int TypeDesc = 0x816A78;
};

struct shok_GGL_CWorkerBehavior : shok_EGL_CGLEBehavior {
	int WorkTimeRemaining, TargetWorkTime; // 4
	float Motivation;
private:
	int BehaviorProps2; //7
public:
	int CycleIndex; // seems to be 0->working, 1->eating, 2->resting, 5->joining settlement
	int TimesWorked, TimesNoWork, TimesNoFood, TimesNoRest, TimesNoPay, JoblessSinceTurn;
	byte CouldConsumeResource, IsLeaving; // 15
	PADDING(2);
	PADDINGI(1);
	float CarriedResourceAmount; // 17

	// defined states: unknown 13,21
	// defined tasks: TASK_LEAVE_SETTLEMENT, TASK_GO_TO_POS, TASK_ADVANCE_IN_CYCLE, TASK_INCREASE_PLAYER_XXX,
	// 	   TASK_CHANGE_WORK_TIME_ABSOLUTE, TASK_CHANGE_WORK_TIME_RELATIVE, TASK_CHANGE_WORK_TIME_XXX, TASK_GO_TO_WORK_BUILDING,
	// 	   TASK_GO_TO_EAT_BUILDING, TASK_GO_TO_REST_BUILDING, TASK_GO_TO_LEAVE_BUILDING,
	// 	   TASK_CHECK_GO_TO_WORK_BUILDING_SUCCESS, TASK_CHECK_GO_TO_EAT_BUILDING_SUCCESS, TASK_CHECK_GO_TO_REST_BUILDING_SUCCESS, TASK_CHECK_GO_TO_VILLAGE_CENTER_SUCCESS,
	// 	   TASK_WORK_WAIT_UNTIL, TASK_EAT_WAIT, TASK_REST_WAIT, TASK_SET_ANIM_AT_WORKPLACE, TASK_SET_ANIM_AT_FARM, TASK_SET_ANIM_AT_RESIDENCE, TASK_SET_ANIM_AT_VILLAGE_CENTER,
	// 	   TASK_MINED_RESOURCE TASK_REFINE_RESOURCE TASK_DO_TRADE_STEP TASK_DO_RESEARCH_STEP (all the same),
	// 	   TASK_RETURN_TO_CYCLE, TASK_CHECK_FEAR, TASK_CREATE_CANNON, TASK_SET_CANNON_PROGRESS, TASK_DO_WORK_AT_FOUNDRY
	// 	   TASK_GO_TO_SUPPLIER, TASK_TAKE_FROM_STOCK, TASK_SET_CARRIER_MODEL, TASK_CHECK_GO_TO_SUPPLIER_SUCCESS, TASK_ACTIVATE_UVANIM,
	// 	   TASK_START_WORK_IF_AT_WORKPLACE, TASK_ACTIVATE_PARTICLE_EFFECT, TASK_DEACTIVATE_PARTICLE_EFFECT,TASK_GO_TO_NEAREST_NEUTRAL_BRIDGE, TASK_CHECK_GO_TO_BRIDGE_SUCCESS,
	// 	   TASK_INCREASE_BRIDGE_PROGRESS
	// defined events: Worker_XXX

	static inline constexpr int vtp = 0x772B30;
	static inline constexpr int TypeDesc = 0x813B1C;
};

struct shok_GGL_CBattleBehavior : shok_EGL_CGLEBehavior { // GGL::CBehaviorFollow in between, 776E40
	float SuccessDistance, FailureDistance; // 4
	int TimeOutTime, StartTurn;
	shok_position TargetPosition;
	byte StartFollowing, StopFollowing; // 10
	PADDING(2);
	int FollowStatus;
	PADDINGI(1); // 12 p to behprops
	int LatestHitTurn;
	int LatestAttackerID, BattleStatus; // la15
	byte NoMoveNecessary, NormalRangeCheckNecessary;
	PADDING(2);
	int Command;
	shok_position AttackMoveTarget;
	byte Helping;
	PADDING(3);
	int MilliSecondsToWait, MSToPlayHitAnimation;
	byte HitPlayed; // 23
	PADDING(3);
	int LatestAttackTurn;

	// defined sates: Follow, MoveToTarget, BattleWait
	// defined events: Follow_XXX, Animation_UnSuspend (x2) (updates startturn), Battle_XXX
	// defined tasks: TASK_MOVE_TO_TARGET, TASK_SET_ORIENTATION_TO_TARGET, TASK_SET_ATTACK_ANIM, TASK_HURT, TASK_CHECK_RANGE, TASK_FIRE_PROJECTILE, TASK_SET_BATTLE_IDLE_ANIM,
	//		TASK_BATTLE_WAIT_UNTIL, TASK_SET_LATEST_ATTACK_TURN, TASK_WAIT_FOR_LATEST_ATTACK, TASK_RESOLVE_BATTLE_COLLISION

	static inline constexpr int vtp = 0x77313C;
	static inline constexpr int TypeDesc = 0x815EEC;

	float GetMaxRange();
};

struct shok_GGL_CLeaderBehavior : shok_GGL_CBattleBehavior {
	PADDINGI(2);
	int TroopHealthCurrent, TroopHealthPerSoldier; // 27
	shok_position TerritoryCenter;
	float TerritoryCenterRange;
	int Experience; // 32
	vector_padding;
	std::vector<shok_position, shok_allocator<shok_position>> PatrolPoints;
	int NextPatrolPointIndex; // 37
	float DefendOrientation;
	int TrainingStartTurn;
	byte PatrolIndexCountingDown, UsingTargetOrientation, UsingTerritory; // 40
	PADDING(1);
	int SecondsSinceHPRefresh, NudgeCount, FormationType;
	shok_position StartBattlePosition;
	int IndexOfFirstChaoticPosition; //46

	// defined states: Train, LeaderGetCloseTotarget
	// defined events: Leader_XXX, MoveCommand_Move, Behavior_Tick (regen), HeroAbility_Reset
	// defined tasks: TASK_SET_DEFAULT_REACTION_TYPE, TASK_GO_TO_POS, TASK_ACTIVATE_UVANIM, TASK_LEAVE_BARRACKS, TASK_SET_ANIM_AT_BARRACKS, TASK_TRAIN,
	//		TASK_GET_CLOSE_TO_TARGET, TASK_ATTACK, TASK_CHECK_MIN_RADIUS

	static inline constexpr int vtp = 0x7761E0;
	static inline constexpr int TypeDesc = 0x81EF80;

	int GetTroopHealth();
	int GetTroopHealthPerSoldier();
};

struct shok_GGL_CSoldierBehavior : shok_GGL_CBattleBehavior {

	// defined states: 1, 24
	// defined events: Leader_Hurt, Leader_GetAttackTarget (forwards to leader), HeroAbility_Reset, Soldier_XXX
	// defined tasks: TASK_GO_TO_POS, TASK_LEAVE_BARRACKS

	static inline constexpr int vtp = 0x773CC8;
	static inline constexpr int TypeDesc = 0x817FC4;
};

struct shok_GGL_CBattleSerfBehavior : shok_GGL_CLeaderBehavior {
	PADDINGI(1); // 47 p to props
	int JobMemoryResourceID, TimeBeforeChangeback;

	// defined events: BattleSerf_XXX, Behavior_Tick (ticks down time if >0, reverts back if 0)
	// defined tasks: TASK_GO_TO_MAIN_HOUSE, TASK_TURN_INTO_SERF

	static inline constexpr int vtp = 0x7788C4;
	static inline constexpr int TypeDesc = 0x826BB4;
};

struct shok_GGL_CSerfBattleBehavior : shok_GGL_CBattleBehavior {
	// padding 2, shok_pos territory?

	// defined events: MoveCommand_Move, Leader_AttackEntity, Leader_GetAttackTarget, Behavior_Tick
	// defined tasks: TASK_SET_DEFAULT_REACTION_TYPE

	static inline constexpr int vtp = 0x774A98;
	static inline constexpr int TypeDesc = 0x81A4FC;
};

struct shok_GGL_CSerfBehavior : shok_EGL_CGLEBehavior {
	int ConstructionSiteSlotIndex; //4
	int ExtractionDelayCounter; //float?
	int LastResourceType, JobMemoryResourceID;
	int ResourceSlot;

	static inline constexpr int vtp = 0x774874;
	static inline constexpr int TypeDesc = 0x819AFC;
};

struct GGL_SSlotArgsLimitedAttachment {
	shok_AttachmentType AttachmentType;
	int Limit;
	int Event; // ?
};
struct shok_GGL_CLimitedAttachmentBehavior : shok_EGL_CGLEBehavior {
	int vtable_EGL_TSlot_GGL_SSlotArgsLimitedAttachment_331624813;
	shok_set<GGL_SSlotArgsLimitedAttachment> AttachmentLimits;

	static inline constexpr int vtp = 0x775E84;
	static inline constexpr int TypeDesc = 0x81D6DC;
};

struct shok_GGL_CFormationBehavior : shok_EGL_CGLEBehavior {
	int AnimStartTurn, AnimDuration;

	static inline constexpr int vtp = 0x776D60;
	static inline constexpr int TypeDesc = 0x8212A4;
};

struct shok_GGL_CCamperBehavior : shok_EGL_CGLEBehavior {
	static inline constexpr int vtp = 0x77777C;
	static inline constexpr int TypeDesc = 0x823720;
};

struct shok_GGL_CGLBehaviorDying : shok_EGL_CGLEBehavior {
	static inline constexpr int vtp = 0x7785E4;
	static inline constexpr int TypeDesc = 0x825F14;
};

struct shok_GGL_CHeroBehavior : shok_EGL_CGLEBehavior {
	PADDINGI(1);
	int ResurrectionTimePassed, SpawnTurn;
	byte FriendNear, EnemyNear;
	PADDING(2);

	static inline constexpr int vtp = 0x77677C;
	static inline constexpr int TypeDesc = 0x820070;
};

struct shok_GGL_CBombBehavior : shok_EGL_CGLEBehavior {
	int TimeToExplode;

	static inline constexpr int vtp = 0x778468;
	static inline constexpr int TypeDesc = 0x8258A8;
};

struct shok_GGL_CKegBehavior : shok_EGL_CGLEBehavior {
	PADDINGI(1);
	int TimeToExplode;

	static inline constexpr int vtp = 0x7764D8;
	static inline constexpr int TypeDesc = 0x81F6BC;
};

struct shok_GGL_CThiefBehavior : shok_EGL_CGLEBehavior {
	int Amount, ResourceType, StolenFromPlayer, TimeToSteal;

	static inline constexpr int vtp = 0x7739B0;
	static inline constexpr int TypeDesc = 0x8173DC;
};

struct shok_GGL_CAutoCannonBehavior : shok_EGL_CGLEBehavior {
	PADDINGI(6);
	int ShotsLeft;

	float GetMaxRange();

	static inline constexpr int vtp = 0x778CF0;
	static inline constexpr int TypeDesc = 0x8288A0;
};

struct shok_GGL_CResourceRefinerBehavior : shok_EGL_CGLEBehavior {
	static inline constexpr int vtp = 0x774BCC;
	static inline constexpr int TypeDesc = 0x81AD80;
};

struct shok_GGL_CAffectMotivationBehavior : shok_EGL_CGLEBehavior {
	static inline constexpr int vtp = 0x77918C;
	static inline constexpr int TypeDesc = 0x829024;
};

struct shok_GGL_CLimitedLifespanBehavior : shok_EGL_CGLEBehavior {
	PADDINGI(1);
	int RemainingLifespanSeconds;

	static inline constexpr int vtp = 0x775D9C;
	static inline constexpr int TypeDesc = 0x81D0B0;
};

struct shok_GGL_CBarrackBehavior : shok_EGL_CGLEBehavior {
	byte AutoFillActive;
	PADDING(3);

	static inline constexpr int vtp = 0x778A68;
	static inline constexpr int TypeDesc = 0x8278DC;
};

struct shok_EGL_GLEBehaviorMultiSubAnims : shok_EGL_CGLEBehavior {
	PADDINGI(1);
	int LastUpdateTurn;
	struct {
		byte Active, PlayBackwards, IsLooped;
		PADDING(1);
		int AnimID, StartTurn, Duration;
		PADDINGI(1);
		float Speed;
	} AnimSlot[4];

	static inline constexpr int vtp = 0x785EEC;
	static inline constexpr int TypeDesc = 0x83AE4C;
};

struct shok_GGL_CBuildingMerchantBehavior_COffer : shok_object {
	shok_costInfo ResourceCosts;
	int OffersRemaining;

	static inline constexpr int vtp = 0x7781B4;
	static inline constexpr int TypeDesc = 0x824BC8;
};

struct shok_GGL_CBuildingTechTraderBehavior_CTechOffer : shok_GGL_CBuildingMerchantBehavior_COffer {
	int OfferedTechnologyType;

	static inline constexpr int vtp = 0x7781C4;
	static inline constexpr int TypeDesc = 0x824BFC;
};

struct shok_GGL_CBuildingMercenaryBehavior_CMercenaryOffer : shok_GGL_CBuildingMerchantBehavior_COffer {
	int OfferedEntityType;

	static inline constexpr int vtp = 0x778284;
	static inline constexpr int TypeDesc = 0x824ED8;
};

struct shok_GGL_CBuildingMerchantBehavior : shok_EGL_CGLEBehavior {
	int CurrentPlayer;
	int TraderEntityID;
	vector_padding;
	std::vector<shok_GGL_CBuildingMerchantBehavior_COffer*, shok_allocator<shok_GGL_CBuildingMerchantBehavior_COffer*>> Offer;

	static inline constexpr int vtp = 0x778208;
	static inline constexpr int TypeDesc = 0x824DFC;
};

struct shok_GGL_CBuildingMercenaryBehavior : shok_GGL_CBuildingMerchantBehavior {
	static inline constexpr int vtp = 0x7782C0;
	static inline constexpr int TypeDesc = 0x8250E4;
};

struct shok_GGL_CBuildingTechTraderBehavior : shok_GGL_CBuildingMerchantBehavior {
	static inline constexpr int vtp = 0x77822C;
	static inline constexpr int TypeDesc = 0x824E28;
};

struct shok_GGL_CMarketBehavior : shok_EGL_CGLEBehavior {
	PADDINGI(1);
	int SellResourceType, BuyResourceType;
	float BuyAmount, SellAmount, ProgressAmount; // prog max is buyam+sellam

	static inline constexpr int vtp = 0x775CCC;
	static inline constexpr int TypeDesc = 0x81CE24;
};

struct shok_GGL_CFoundryBehavior : shok_EGL_CGLEBehavior {
	int CannonType, CannonProgress;

	static inline constexpr int vtp = 0x778A8C;
	static inline constexpr int TypeDesc = 0x827900;
};


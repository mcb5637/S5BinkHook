#pragma once
#include "s5data.h"

struct shok_EGL_CGLEBehavior : shok_object { // no vtable
	int BehaviorIndex, EntityId;
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
	int AbilitySecondsCharged; // 5

	static inline constexpr int TypeDesc = 0x816E2C;
};

struct shok_GGL_CCamouflageBehavior : shok_GGL_CHeroAbility {
	PADDINGI(2);
	int InvisibilityRemaining;

	static inline constexpr int vtp = 0x7738F4;
	static inline constexpr int TypeDesc = 0x8172E8;
};

struct shok_GGL_CThiefCamouflageBehavior : shok_GGL_CCamouflageBehavior {
	int TimeToInvisibility;

	static inline constexpr int vtp = 0x773934;
	static inline constexpr int TypeDesc = 0x817310;
};

struct shok_GGL_CHeroHawkBehavior : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x7766F0;
	static inline constexpr int TypeDesc = 0x81FE84;
};

struct shok_GGL_CInflictFearAbility : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x776638;
	static inline constexpr int TypeDesc = 0x81FBD8;
};

struct shok_GGL_CBombPlacerBehavior : shok_GGL_CHeroAbility {
	shok_position StartPosition, TargetPosition;
	byte PlacedBomb;
	PADDING(3);

	static inline constexpr int vtp = 0x7783D8;
	static inline constexpr int TypeDesc = 0x8255D0;
};

struct shok_GGL_CCannonBuilderBehavior : shok_GGL_CHeroAbility {
	PADDINGI(1);
	shok_position StartPosition;
	int CannonType, FoundationType;
	byte PlacedCannon;
	PADDINGI(3);

	static inline constexpr int vtp = 0x7774D4;
	static inline constexpr int TypeDesc = 0x823254;
};

struct shok_GGL_CRangedEffectAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int SecondsRemaining;

	static inline constexpr int vtp = 0x774E54;
	static inline constexpr int TypeDesc = 0x81B7AC;
};

struct shok_GGL_CCircularAttack : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x777464;
	static inline constexpr int TypeDesc = 0x823038;
};

struct shok_GGL_CSummonBehavior : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x773C10;
	static inline constexpr int TypeDesc = 0x817E0C;
};

struct shok_GGL_CConvertSettlerAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int TimeToConvert;

	static inline constexpr int vtp = 0x777294;
	static inline constexpr int TypeDesc = 0x8227D8;
};

struct shok_GGL_CSniperAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int TargetId;

	static inline constexpr int vtp = 0x7745AC;
	static inline constexpr int TypeDesc = 0x819044;
};

struct shok_GGL_CMotivateWorkersAbility : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x77574C;
	static inline constexpr int TypeDesc = 0x81C408;
};

struct shok_GGL_CShurikenAbility : shok_GGL_CHeroAbility {
	PADDINGI(1);
	int TargetId;

	static inline constexpr int vtp = 0x774658;
	static inline constexpr int TypeDesc = 0x819310;
};

struct shok_GGL_CKegPlacerBehavior : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x776368;
	static inline constexpr int TypeDesc = 0x81F084;
};

struct shok_GGL_CAbilityScoutBinocular : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x779218;
	static inline constexpr int TypeDesc = 0x829248;
};

struct shok_GGL_CTorchPlacerBehavior : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x773738;
	static inline constexpr int TypeDesc = 0x816E4C;
};

struct shok_GGL_CPointToResourceBehavior : shok_GGL_CHeroAbility {
	static inline constexpr int vtp = 0x774FB0;
	static inline constexpr int TypeDesc = 0x81BB84;
};

struct shok_GGL_CSentinelBehavior : shok_EGL_CGLEBehavior {
	static inline constexpr int vtp = 0x774B6C;
	static inline constexpr int TypeDesc = 0x81ABC0;
};

struct shok_GGL_CGLBehaviorAnimationEx : shok_EGL_CGLEBehavior {
	int Animation, AnimCategory, SuspendedAnimation, StartTurn, Duration;
	byte PlayBackwards;
	PADDING(3);
	int TurnToWaitFor;
	float Speed;

	static inline constexpr int vtp = 0x776B64;
	static inline constexpr int TypeDesc = 0x820BE8;
};

struct shok_GGL_CBehaviorWalkCommand : shok_EGL_CGLEBehavior {
	shok_position TargetPosition;

	static inline constexpr int vtp = 0x7736A4;
	static inline constexpr int TypeDesc = 0x816A78;
};

struct shok_GGL_CWorkerBehavior : shok_EGL_CGLEBehavior {
	int WorkTimeRemaining, TargetWorkTime;
	float Motivation;
private:
	int BehaviorProps2;
public:
	int CycleIndex, TimesWorked, TimesNoWork, TimesNoFood, TimesNoRest, TimesNoPay, JoblessSinceTurn, CouldConsumeResource, IsLeaving;
	float TransportAmount;

	static inline constexpr int vtp = 0x772B30;
	static inline constexpr int TypeDesc = 0x813B1C;
};

struct shok_GGL_CBattleBehavior : shok_EGL_CGLEBehavior {
	float SuccessDistance, FailureDistance;
	int TimeOutTime, StartTurn;
	shok_position TargetPosition;
	byte StartFollowing, StopFollowing;
	PADDING(2);
	int FollowStatus, LatestHitTurn;
	PADDINGI(1);
	int LatestAttackerID, BattleStatus;
	byte NoMoveNecessary, NormalRangeCheckNecessary;
	PADDING(2);
	int Command;
	shok_position AttackMoveTarget;
	PADDINGI(1);
	int MilliSecondsToWait, MSToPlayHitAnimation, HitPlayed; // la 23

	static inline constexpr int vtp = 0x77313C;
	static inline constexpr int TypeDesc = 0x815EEC;

	float GetMaxRange();
};

struct shok_GGL_CLeaderBehavior : shok_GGL_CBattleBehavior {
	PADDINGI(3);
	int TroopHealthCurrent, TroopHealthPerSoldier; // 27
	shok_position TerritoryCenter;
	float TerritoryCenterRange;
	int Experience;
	vector_padding;
	std::vector<shok_position, shok_allocator<shok_position>> PatrolPoints;
	PADDINGI(1);
	float DefendOrientation;
	int TrainingStartTurn;
	PADDINGI(1);
	int SecondsSinceHPRefresh, NudgeCount, FormationType;
	shok_position StartBattlePosition;

	static inline constexpr int vtp = 0x7761E0;
	static inline constexpr int TypeDesc = 0x81EF80;

	int GetTroopHealth();
	int GetTroopHealthPerSoldier();
};

struct shok_GGL_CSoldierBehavior : shok_GGL_CBattleBehavior {
	static inline constexpr int vtp = 0x773CC8;
	static inline constexpr int TypeDesc = 0x817FC4;
};

struct shok_GGL_CBattleSerfBehavior : shok_GGL_CLeaderBehavior {
	static inline constexpr int vtp = 0x7788C4;
	static inline constexpr int TypeDesc = 0x826BB4;
};

struct shok_GGL_CSerfBattleBehavior : shok_GGL_CBattleBehavior {
	static inline constexpr int vtp = 0x774A98;
	static inline constexpr int TypeDesc = 0x81A4FC;
};

struct shok_GGL_CSerfBehavior : shok_EGL_CGLEBehavior {
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


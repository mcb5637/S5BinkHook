#pragma once
#include "s5data.h"

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

	float GetMovementSpeed();
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
	PADDINGI(1)
		int AbilitySecondsCharged; // 5
};

#define shok_vtp_GGL_CCamouflageBehavior (void*)0x7738F4
struct shok_GGL_CCamouflageBehavior : shok_GGL_CHeroAbility {
	PADDINGI(2)
		int InvisibilityRemaining;

	bool IsThiefCamoBehavior();
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

#define shok_vtp_GGL_CKegPlacerBehavior (void*)0x776368
struct shok_GGL_CKegPlacerBehavior : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CAbilityScoutBinocular (void*)0x779218
struct shok_GGL_CAbilityScoutBinocular : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CTorchPlacerBehavior (void*)0x773738
struct shok_GGL_CTorchPlacerBehavior : shok_GGL_CHeroAbility {

};

#define shok_vtp_GGL_CPointToResourceBehavior (void*)0x774FB0
struct shok_GGL_CPointToResourceBehavior : shok_GGL_CHeroAbility {

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

	float GetMaxRange();
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
	vector_padding
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
	vector_padding
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

#define shok_vtp_GGL_CFoundryBehavior (void*)0x778A8C
struct shok_GGL_CFoundryBehavior : shok_EGL_CGLEBehavior {
	int CannonType, CannonProgress;
};


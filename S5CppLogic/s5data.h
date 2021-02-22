#pragma once
#include <cstdint>
#include <vector>

#define DEBUGGER_BREAK _asm int 3

#define SHOK_Import_LUA_OPEN 0x761284
#define SHOK_SEGMENTSTART 0x401000
#define SHOK_SEGMENTLENGTH 0x64B000

typedef uint8_t byte;

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

struct shok_object {
public:
	void* vtable;
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

#define shok_vtp_GGL_CSettlerMovement (void*)77471C
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
	std::vector<shok_position> PatrolPoints;
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
struct shok_GGL_CBattleSerfBehavior : shok_GGL_CBattleBehavior {

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
	std::vector<shok_GGL_CBuildingMerchantBehavior_COffer*> ListOfOffers;
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
	std::vector<shok_EGL_CGLEBehavior*> BehaviorList; // 30, first field in 31
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
};

#define shok_vtp_EGL_CMovingEntity (void*)0x783F84
struct shok_EGL_CMovingEntity : shok_EGL_CGLEEntity {
	shok_position TargetPosition; // la67
	int TargetRotationValid;
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
};

struct shok_vtable_ECS_CManager {
private:
	int u, u2;
public:
	void(__thiscall* ReloadCutscene)(shok_ECS_CManager* th, const char* path);
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

};

struct shok_BB_CIDManagerEx : shok_object {

};


// game logic
struct shok_EGL_CGLEGameLogic : shok_object {

};

struct shok_vtable_EGL_CGLEGameLogic {
private:
	int u[23];
public:
	int(__thiscall* CreateEffect)(shok_EGL_CGLEGameLogic* th, shok_effectCreatorData* data);
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

extern shok_EGL_CGLEEntity* (_stdcall *shok_eid2obj)(int id);

extern void(_stdcall* shok_SetHighPrecFPU)();

extern shok_ECS_CManager*** shok_ECS_CManagerObject;

extern shok_EGL_CGLEGameLogic** shok_EGL_CGLEGameLogicObject;

extern int shok_effectCreatorData_CreatorType_Projectile;

extern shok_EGL_CGLEEffectManager** shok_EGL_CGLEEffectManagerObject;

extern bool(__thiscall* shok_EGL_CGLEEffectManager_IsEffectValid)(shok_EGL_CGLEEffectManager* th, int id);

extern void(* shok_entityHurtEntity)(shok_EGL_CGLEEntity* attackerObj, shok_EGL_CGLEEntity* targetObj, int damage);

extern void (*shok_logString)(const char* format, const char* string);

extern shok_BB_CIDManagerEx** shok_BB_CIDManagerExObj;

extern int(__thiscall* shok_getAnimIdByName)(shok_BB_CIDManagerEx* th, const char* name);

extern shok_EGL_CGLEEntityManager** shok_EGL_CGLEEntityManagerObj;

extern int(*shok_getEntityIdByScriptName)(const char* str);

extern bool(_cdecl* shok_EntityIsSettler)(shok_EGL_CGLEEntity* e);

extern bool(_cdecl* shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e);

extern bool(__thiscall* shok_IsEntityInCategory)(shok_EGL_CGLEEntity* e, int cat);

extern bool(_cdecl* shok_EntityIsResourceDoodad)(shok_EGL_CGLEEntity* e);

extern int(_cdecl* shok_EntityGetProvidedResourceByID)(int id);

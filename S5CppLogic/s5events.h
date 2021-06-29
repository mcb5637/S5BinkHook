#pragma once
#include "s5data.h"

struct shok_event_data_EGL_CEventGetValue_int_1211121895 : shok_event_data {
	int vtable = 0x766CC4;
	int id = 0;
	int result = 0;
};

struct shok_event_data_EGL_CEventPosition : shok_event_data {
	int vtable = 0x766C70;
	int id = 0;
	shok_position pos = { 0,0 };
};
struct shok_event_data_EGL_CEventValue_bool_703333479 : shok_event_data {
	int vtable = 0x76E220;
	int id = 0;
	int b = 0;
};
struct shok_event_data_EGL_CEvent1Entity : shok_event_data {
	int vtable = 0x766C60;
	int id = 0;
	int vtable2 = 0x766C58; //EGL::CEvent1Entity ?
	int entityId = 0;
};
struct shok_event_data_BB_CEvent : shok_event_data {
	int vtable = 0x762114;
	int id = 0;
};
struct shok_event_data_GGL_CEventPositionAnd2EntityTypes : shok_event_data {
	int vtable = 0x766C70;
	int id = 0;
	shok_position pos = { 0,0 };
	int EntityType1 = 0, EntityType2 = 0;
};
struct shok_event_data_EGL_CEventValue_int_27574121 : shok_event_data {
	int vtable = 0x763130;
	int id = 0;
	int i = 0;
};
struct shok_event_data_GGL_CEventEntityIndex : shok_event_data {
	int vtable = 0x766C90;
	int id = 0;
	int entity = 0;
	int index = 0;
};
struct shok_event_data_EGL_CEventGetValue_float_1468983543 : shok_event_data {
	int vtable = 0x76E210;
	int id = 0;
	float f = 0.0f;
};
struct shok_event_data_GGL_CEventAttachmentTypeGetInteger : shok_event_data {
	int vtable = 0x766C80;
	int id = 0;
	shok_AttachmentType AttachmentType = static_cast<shok_AttachmentType>(0);
	int i = 0;
};
struct shok_event_data_GGL_CEventTransaction : shok_event_data {
	int vtable = 0x76D93C;
	int id = 0x1700A;
	shok_ResourceType SellType = static_cast<shok_ResourceType>(0), BuyType = static_cast<shok_ResourceType>(0);
	float BuyAmount = 0.0f;
};

// net events
struct shok_BB_CEvent : shok_object { // todo
	int EventTypeId;

	static inline constexpr int vtp = 0x762114;
};

struct shok_EGL_CNetEvent2Entities : shok_BB_CEvent {
	int ActorId, TargetId;

	static inline constexpr int vtp = 0x76DD60;
};

struct shok_EGL_CNetEventEntityAndPos : shok_BB_CEvent {
	int EntityId;
	shok_position Position;

	static inline constexpr int vtp = 0x76DD50;
};

struct shok_EGL_CNetEventEntityAndPosArray : shok_BB_CEvent {
	int EntityId;
	vector_padding;
	std::vector<shok_position, shok_allocator<shok_position>> Positions;
	float Rotation;

	static inline constexpr int vtp = 0x770704;
};

struct shok_GGL_CNetEventExtractResource : shok_BB_CEvent {
	int EntityId;
	int ResourceType;
	shok_position Position;

	static inline constexpr int vtp = 0x77061C;
};

struct shok_GGL_CNetEventTransaction : shok_BB_CEvent {
	int EntityId, SellType, BuyType, BuyAmount;

	static inline constexpr int vtp = 0x77062C;
};

struct shok_EGL_CNetEventEntityID : shok_BB_CEvent {
	int EntityId;

	static inline constexpr int vtp = 0x766C28;
};

struct shok_GGL_CNetEventCannonCreator : shok_EGL_CNetEventEntityID {
	int BottomType, TopType;
	shok_position Position;

	static inline constexpr int vtp = 0x7705EC;
};

struct shok_GGL_CNetEventEntityIDAndUpgradeCategory : shok_EGL_CNetEventEntityID {
	int UpgradeCategory;

	static inline constexpr int vtp = 0x77060C;
};

struct shok_EGL_CNetEventEntityIDAndInteger : shok_EGL_CNetEventEntityID {
	int Data;

	static inline constexpr int vtp = 0x766C48;
};

struct shok_GGL_CNetEventTechnologyAndEntityID : shok_EGL_CNetEventEntityID {
	int Technology;

	static inline constexpr int vtp = 0x7705FC;
};

struct shok_EGL_CNetEventPlayerID : shok_BB_CEvent {
	int PlayerId;

	static inline constexpr int vtp = 0x766C18;
};

struct shok_GGL_CNetEventBuildingCreator : shok_EGL_CNetEventPlayerID {
	int UpgradeCategory;
	shok_positionRot Position;
	vector_padding;
	std::vector<int, shok_allocator<int>> Serfs;

	static inline constexpr int vtp = 0x770714;
};

struct shok_EGL_CNetEventIntegerAndPlayerID : shok_EGL_CNetEventPlayerID {
	int Data;

	static inline constexpr int vtp = 0x7705BC;
};

struct shok_EGL_CNetEventPlayerIDAndInteger : shok_EGL_CNetEventPlayerID {
	int Data;

	static inline constexpr int vtp = 0x7705CC;
};

struct shok_EGL_CNetEventEntityIDAndPlayerID : shok_EGL_CNetEventPlayerID {
	int EntityId;

	static inline constexpr int vtp = 0x766C38;
};

struct shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType : shok_EGL_CNetEventEntityIDAndPlayerID {
	int EntityType;

	static inline constexpr int vtp = 0x77057C;
};

struct shok_GGL_CNetEventEntityIDPlayerIDAndInteger : shok_EGL_CNetEventEntityIDAndPlayerID {
	int Data;

	static inline constexpr int vtp = 0x77064C;
};

enum class shok_EventIDs : int {
	ResourceDoodad_RemoveOneRes = 0x10002, //BB::CEvent
	OnResourceDoodadDetach = 0x10003, //EGL::CEvent1Entity, also bserf
	Mine_DoWorkStep = 0x10004, //BB::CEvent
	Follow_GetFollowStatus = 0x10008, //EGL::CEventGetValue<int,1211121895>
	Mine_GetResourcesRemaining = 0x10007, //EGL::CEventGetValue<int,1211121895>
	Serf_ExtractResource = 0x10009, //EGL::CEvent1Entity, serfbattle same as 14003
	// 1000A serf something extract res get bool
	ResourceRefiner_Refine = 0x1000B, //BB::CEvent // todo check if this is generic work
	// 0x1000C res tree int?
	Tree_UpdateModel = 0x1000D, //BB::CEvent
	// 0x1000E res tree get bool

	Thief_StealFromCommand = 0x10102, //EGL::CEvent1Entity
	Thief_SecureGoodsCommand = 0x10103, //EGL::CEvent1Entity
	Thief_OnStealTargetDetach = 0x10104, //EGL::CEvent1Entity
	Thief_OnSecureTargetDetach = 0x10105, //EGL::CEvent1Entity
	Thief_IsCarryingSomething = 0x10106, //EGL::CEventGetValue<bool,1709081367>
	KegPlacer_SabotageCommand = 0x10107, //EGL::CEvent1Entity
	Keg_Setup = 0x10108, //GGL::CEventKegInfo {vt,evid,targetid}
	Keg_OnThiefDetach = 0x10109, //EGL::CEvent1Entity
	Keg_Arm = 0x1010A, //BB::CEvent
	Keg_Disarm = 0x1010B, //BB::CEvent
	KegPlacer_OnTargetDetach = 0x1010C, //EGL::CEvent1Entity
	Keg_CanBeDisarmed = 0x1010D, //EGL::CEventGetValue<bool,1709081367>
	KegPlacer_DefuseCommand = 0x1010E, //EGL::CEvent1Entity
	KegPlacer_OnDisarmDetach = 0x1010F, //EGL::CEvent1Entity
	Keg_GetTimeToExplode = 0x10110, //EGL::CEventGetValue<int,1211121895>
	Thief_GetStealProgress = 0x10111, //EGL::CEventGetValue<float,1468983543>
	KegPlacer_GetSabotageProgress = 0x10112, //EGL::CEventGetValue<float,1468983543>
	KegPlacer_GetDisarmProgress = 0x10113, //EGL::CEventGetValue<float,1468983543>
	Thief_GetAmountOfStolenResource = 0x10114, //EGL::CEventGetValue<int,1211121895>
	Thief_GetStolenResourceType = 0x10115, //EGL::CEventGetValue<int,1211121895>


	MoveCommand_Move = 0x11002, //EGL::CEventPosition
	Leader_SetTerritory = 0x11003, //EGL::CEventPosition
	Leader_SetTerritoryRange = 0x11004, //EGL::CEventValue<float,1278362727>

	Follow_TargetDestroyed = 0x11009, //EGL::CEvent1Entity
	Follow_StartFollowing = 0x1100A, //GGL::CEventFollowInfo
	Follow_CheckTargetStillAlive = 0x1100B, //BB::CEvent
	Follow_GetFollowBehavior = 0x1100C, //EGL::CEventGetValue<GGL::CBehaviorFollow *,-1301899769>
	Leader_ApproachPos = 0x1101A, //EGL::CEventPosition
	// 1101B follow semms to return -1, lots of calls
	Battle_SetBattleWalkAnim = 0x1101C, //BB::CEvent
	// 1101E formation get pos in formation?

	// 1100D, 1100E, 11017 something default movement
	// 11002 bino cancel
	// 11003 serfbattle, set territory?
	// 12002 stop?
	Animation_SetAnimSet = 0x12007, //EGL::CEventValue<int,-27574121
	// 12008 leader hurt?

	Binocular_ExploreCommand = 0x11102, //EGL::CEventPosition
	TorchPlacer_PlaceTorch = 0x11103, //EGL::CEventPosition
	PointToResources_Activate = 0x11104, //BB::CEvent

	Worker_WorkPlaceBuildingDestroyed = 0x13002, //EGL::CEvent1Entity serf probably construction/repair destr
	Worker_FarmBuildingDestroyed = 0x13003, //EGL::CEvent1Entity
	Worker_ResidencePlaceBuildingDestroyed = 0x13004, //EGL::CEvent1Entity
	Worker_GetMotivation = 0x13007, //EGL::CEventGetValue<float,1468983543>
	Worker_GetWorkWaitUntil = 0x13008, //EGL::CEventGetValue<int,1211121895>
	Worker_GetWorkTaskList = 0x13009, //EGL::CEventGetValue<int,1211121895> checks building, then props
	Worker_LevyTaxes = 0x1300A, //BB::CEvent
	Worker_ForceToWork = 0x1300B, //BB::CEvent
	// 1300C most likely worker leave
	// 13011 camper beh
	// 13012 camper beh forward to 12003 on camp
	// worker 13013 get int something cycleindex
	// worker 13014 empty EGL::CEventValue<int,-27574121>
	Worker_Bless = 0x13015, //BB::CEvent
	Worker_AdvanceInCycles = 0x13016, //BB::CEvent
	// worker 13017 CycleIndex=5
	Worker_ChangeMoti = 0x13019, //GGL::CEventChangeMotivation
	Worker_GetMaxWorkTime = 0x1301A, //EGL::CEventGetValue<int, 1211121895>
	Worker_GetWorkTimeRemaining = 0x1301B, //EGL::CEventGetValue<int, 1211121895>
	// 0x1301C foundry something progress?
	// 1301D worker something entity
	Worker_GetResourceToRefine = 0x1301E, //EGL::CEventGetValue<int, 1211121895>
	WorkerFlee_IsFleeing = 0x1301F, //EGL::CEventGetValue<bool,1709081367>
	// 13018 worker leave? EGL::CEvent1Entity
	// 13020 worker get some bool condidtionally forwards to 18007
	Worker_IsLeaving = 0x13021, //EGL::CEventGetValue<bool,1709081367>
	Worker_GetTransportModel = 0x13022, //EGL::CEventGetValue<int, 1211121895>
	SettlerMerchant_MoveInCommand = 0x13023, //BB::CEvent
	SettlerMerchant_MoveOutCommand = 0x13024, //BB::CEvent
	// 13025 worker emtpty EGL::CEvent1Entity
	Worker_SetWorkTimeRemaining = 0x13029, //EGL::CEventValue<int,-27574121>
	SettlerMerchant_GetBuildingId = 0x1302A, //EGL::CEventGetValue<int, 1211121895>

	Serf_Construct = 0x14003, // GGL::CEventEntityIndex serfbattle?
	Keep_BuySerfCommand = 0x14004, //BB::CEvent
	Serf_CommandTurnToBattleSerf = 0x14005, //BB::CEvent battleserf stop if toserf tl
	BattleSerf_CommandTurnToSerf = 0x14006, //BB::CEvent serf stop if toserf tl
	// 14007 serf get unknown float 9
	BattleSerf_GetTimeToChangeBack = 0x14008, //EGL::CEventGetValue<int, 1211121895>

	// 18003 get bool true

	Leader_AttackEntity = 0x15004, //EGL::CEvent1Entity
	// 15005 leader, serfbattle
	// 15007 soldier empty
	// 15008 leader empty
	// 15009 leader
	Leader_GetHealthPlusTroopHealth = 0x1500A, //EGL::CEventGetValue<int, 1211121895>
	Leader_GetMaxHealthPlusTroopHealth = 0x1500B, //EGL::CEventGetValue<int, 1211121895>
	// 1500C battle set target?, autocannon too?
	Leader_OnAttackTargetDetached = 0x1500D, //EGL::CEvent1Entity
	OnAttackedBy = 0x1500E, //EGL::CEvent1Entity
	Battle_GetBattleStatus = 0x15011, //EGL::CEventGetValue<int, 1211121895>
	Battle_SetBattleStatus = 0x15012, //EGL::CEventValue<int,-27574121>
	Battle_GetDamageClass = 0x15013, //EGL::CEventGetValue<int, 1211121895>
	Leader_GetCurrentNumSoldier = 0x15016, //EGL::CEventGetValue<int, 1211121895>
	Formation_AssumePosition = 0x15017, //BB::CEvent
	Leader_SetNudgeCount = 0x15018, //EGL::CEventValue<int,-27574121>
	Leader_GetNudgeCount = 0x15019, //EGL::CEventGetValue<int, 1211121895>
	// 1501C soldier some other entity
	// 1501D battle barracks attachment, set battle tl?, soldier simmilar, autocannon set battle tl
	// 1501E leader get some unknown int +100, soldier something other entity???
	Leader_GetMaxNumSoldier = 0x1501F, //EGL::CEventGetValue<int, 1211121895>
	Leader_GetAttackTarget = 0x15021, //EGL::CEventGetValue<int, 1211121895> soldier forwards to leader
	Leader_Hurt = 0x15023, //EGL::CEventValue<int,-27574121>
	Battle_GetLatestAttackerID = 0x15024, //EGL::CEventGetValue<int, 1211121895>
	Battle_GetLatestHitTurn = 0x15025, //EGL::CEventGetValue<int, 1211121895>
	Battle_GetBattleBehavior = 0x15026, //EGL::CEventGetValue<GGL::CBattleBehavior *,-646837913>
	Leader_GetLeaderBehavior = 0x15027, //EGL::CEventGetValue<GGL::CLeaderBehavior *,-1190255961>
	Soldier_GetSoldierBehavior = 0x15028, //EGL::CEventGetValue<GGL::CSoldierBehavior *,-806903129>
	SoldierGetLeaderID = 0x15029, //EGL::CEventGetValue<int, 1211121895>
	Leader_GetXP = 0x1502A, //EGL::CEventGetValue<int, 1211121895>
	Leader_SetXP = 0x1502B, //EGL::CEventValue<int,-27574121>
	// 1502C leader empty
	Leader_GetCommand = 0x1502D, //EGL::CEventGetValue<int, 1211121895>
	Leader_AttackMove = 0x1502E, //EGL::CEventPosition
	Leader_HoldPosition = 0x1502F, //BB::CEvent
	// 15030 leader disable territory?
	Leader_RunAwayFromFearEffect = 0x15031, //EGL::CEvent1Entity TODO validate
	Leader_Defend = 0x15032, //BB::CEvent
	BombPlacer_CommandPlaceBomb = 0x15033, //EGL::CEventPosition
	// 15034 also something with fear
	// 0x15035 rax buy leader?
	// 15036 leader some area check get
	Barracks_BuySoldierForLeader = 0x15037, //EGL::CEvent1Entity
	// 15038 leader? goes to defend after something else
	// 15039 leader something attack target?
	// 1503A leader
	Leader_IsUsingTargetOrientation = 0x1503B, //EGL::CEventGetValue<bool,1709081367>
	// 1503C serf some kind of stop?
	Barracks_GetTrainingTaskList = 0x1503F, //EGL::CEventGetValue<int, 1211121895>
	Barracks_GetTrainingTime = 0x15040, //EGL::CEventGetValue<float, 1468983543>
	// 0x15041 rax is training allowed?
	// 15042 leader set training tl?
	Barracks_GetLeaveTaskList = 0x15043, //EGL::CEventGetValue<int, 1211121895>
	// 15044 leader to 12002
	Leader_ExpellSoldier = 0x15046, //EGL::CEventValue<bool,703333479> bool seems to be use effect
	WorkerAlarmMode_Enable = 0x15047, //BB::CEvent
	WorkerAlarmMode_Disable = 0x15048, //BB::CEvent
	Battle_IsInBattleTL = 0x15049, //EGL::CEventGetValue<bool,1709081367>
	Leader_GetUpkeepCost = 0x1504C, //EGL::CEventGetValue<float, 1468983543>

	GetDamage = 0x1503D, //EGL::CEventGetValue<int, 1211121895>
	// 15045 battle maybe cancel?

	HeroHawk_SendHawk = 0x16002, //EGL::CEventPosition

	HeroAbility_IsAbilitySupported = 0x16008, //GGL::CEventHeroAbilityInteger
	HeroAbility_StandUpOrInit = 0x16009, //BB::CEvent
	CannonBuilder_BuildCannonCommand = 0x1600A, //GGL::CEventPositionAnd2EntityTypes
	// 1600B autocannon on foundation detach?
	HeroAbility_Cancel = 0x1600D, //BB::CEvent
	// 1500E convert cancel?
	// 0x1600F hawk destroy?
	HeroAbility_GetChargeCurrent = 0x16010, //GGL::CEventHeroAbilityInteger
	HeroAbility_GetChargeMax = 0x16011, //GGL::CEventHeroAbilityInteger
	HeroAbility_SetChargeCurrent = 0x16012, //GGL::CEventHeroAbilityInteger
	// 16013 ? fire projectile event???

	Camouflage_IsInvisible = 0x16013, //EGL::CEventGetValue<bool,1709081367>, 1504A same
	ThiefCamouflage_Reset = 0x16014, //BB::CEvent
	Camouflage_Activate = 0x16015, //BB::CEvent
	Camouflage_GetDurationMax = 0x16016, //EGL::CEventGetValue<int,1211121895>
	Camouflage_GetDurationCurrent = 0x16017, //EGL::CEventGetValue<int,1211121895>

	HeroBehavior_GetResurrectionTimePassed = 0x16018, //EGL::CEventGetValue<int,1211121895>

	Sentinel_GetUrgency = 0x16019, //EGL::CEventGetValue<int,1211121895>

	Summon_ActivateCommand = 0x1601A, //BB::CEvent
	Summoned_OnSummonerDetach = 0x1601B, //BB::CEvent
	RangedEffect_Activate = 0x1601C, //BB::CEvent
	RangedEffect_GetDamageFactor = 0x1601D, //EGL::CEventGetValue<float,1468983543>
	RangedEffect_GetArmorFactor = 0x1601E, //EGL::CEventGetValue<float,1468983543>
	RangedEffect_GetDurationMax = 0x1601F, //EGL::CEventGetValue<int,1211121895>
	RangedEffect_GetDurationCurrent = 0x16020, //EGL::CEventGetValue<int,1211121895>

	HeroBehavior_CommandNPCInteraction = 0x16021, //EGL::CEvent1Entity

	CircularAttack_ActivateCommand = 0x16022, //BB::CEvent

	ComvertSettler_ActivateCommand = 0x16027, //EGL::CEvent1Entity
	// 16028 convert cancel?
	LimitedLifespan_GetTimeRemaining = 0x16029, //EGL::CEventGetValue<int,1211121895>
	LimitedLifespan_GetTimeMax = 0x1602A, //EGL::CEventGetValue<int,1211121895>
	HeroBehavior_GetSpawnTurn = 0x1602B, //EGL::CEventGetValue<int,1211121895>

	MotivateVorkers_ActivateCommand = 0x1602C, //BB::CEvent
	Sniper_SnipeCommand = 0x1602D, //EGL::CEvent1Entity
	Sniper_GetRange = 0x1602E, //EGL::CEventGetValue<float,1468983543>
	Shuriken_ActivateCommand = 0x1602F, //EGL::CEvent1Entity
	Shuriken_GetRange = 0x16030, //EGL::CEventGetValue<float,1468983543>

	InflictFear_Activate = 0x16026, //BB::CEvent

	// 0x17005 affectmoti affect moti
	Market_WorkStep = 0x17007, //BB::CEvent
	University_ResearchStep = 0x17008, //BB::CEvent
	Market_GetProgress = 0x17009, //EGL::CEventGetValue<float,1468983543>
	AffectMotivation_GetEffect = 0x17010, //EGL::CEventGetValue<float,1468983543>
	Market_StartTrade = 0x1700A, //EGL::CEventGetValue<float,1468983543>
	Market_CancelTrade = 0x1700B, //BB::CEvent
	// 0x1700C university get tech progress?
	WorkerAlarmMode_OnBuildingDetach = 0x1700F, //EGL::CEvent1Entity
	// 0x17011 resourcerefiner get supplier?/can work?
	Worker_ResetTaskList = 0x17012, //BB::CEvent
	Foundry_GetProgress = 0x17014, //EGL::CEventGetValue<int,1211121895>
	Foundry_SetProgress = 0x17015, //EGL::CEventValue<int,-27574121>
	Foundry_BuildCannonCommand = 0x17016, //EGL::CEventValue<int,-27574121>
	Foundry_WorkStep = 0x17018, //BB::CEvent
	// 0x17019 affectmoti affect moti
	// 0x1701B mine detach res

	Barracks_ActivateAutoFill = 0x1701E, //BB::CEvent
	Barracks_DeActivateAutoFill = 0x1701F, //BB::CEvent
	Barracks_IsAutoFillActive = 0x17020, //EGL::CEventGetValue<bool,1709081367>
	BuildingMerchant_IsBlocked = 0x17021, //EGL::CEventPlayerGetBool
	BuildingMerchant_GetBlockingPlayer = 0x17022, //EGL::CEventGetValue<int,1211121895>
	BuildingMerchant_GetBuildingMerchantProps = 0x17023, //EGL::CEventGetValue<GGL::CBuildingMerchantBehaviorProps const *,-1872076829>
	BuildingMerchant_IsNotEmpty = 0x17024, //EGL::CEventGetValue<bool,1709081367>
	BuildingMercenary_AddOffer = 0x17025, //GGL::CEventMercenaryOffer
	BuildingMercenary_GetOffer = 0x17026, //GGL::CEventGetMercenaryOffer
	BuildingMercenary_BuyOffer = 0x17027, //GGL::CEventPlayerIDInteger
	// 0x17028 buildmerc ret true
	BuildingMerchant_GetNumberOfOffers = 0x17029, //EGL::CEventGetValue<int,1211121895>
	// 0x1702A buildingmerch ret true
	BuildingTechTrader_AddOffer = 0x1702B, //GGL::CEventTechOffer
	BuildingTechTrader_GetOffer = 0x1702C, //GGL::CEventGetTechOffer
	// 0x1702D builtechtrad ret true

	// 18002 serf ret true
	// 18004 leader ret true
	// 18005 soldier ret true
	// 18007 battle, autocannon ret true
	// 18006 herobeh ret true
	// 1800C resourcerefiner ret true
	// 1800D thefbeh ret true

	// 1A002 limitedattachment on attach?
	// 1A003 limitedattachment on detach?
	// 1A004 limitedattach maybe get is full?
	// 1A005 limitedattach get bool?
	// 1A006 limitedattach maybe get curr?
	LimitedAttachment_GetMax = 0x1A007, //GGL::CEventAttachmentTypeGetInteger
	// 1A008 limitedattachment maybe get left?
	// 1A009 limitedattach ?
	// 1A00A limitedattach ?

	// 20002 set walk target?
	// 20003 set walk target?
	// 20004 set building sub anim?

	MultiSubAnim_SetSubAnim = 0x20004, //EGL::CEventSubAnim
	Behavior_Tick = 0x20005, //BB::CEvent ticks every second
	Die = 0x2000A, ///BB::CEvent

	Animation_GetAnim = 0x20013, //EGL::CEventGetValue<int,1211121895>
	Animation_UnSuspend = 0x20014, //BB::CEvent may be swapped, check, autocannon EGL::CEventValue<int,-27574121>?
	Animation_Suspend = 0x20015, //EGL::CEventValue<int,-27574121> argument seems to be tick
	Animation_SetAnim = 0x2001D, //EGL::CEventAnimation
	Animation_ResetTaskType = 0x2001E, //BB::CEvent
	// worker 20024 get some int, leader get something barracks related, soldier simmilar, serf get terrainH < waterHeight
	ReplaceableEntity_Disable = 0x20025, //BB::CEvent
	// 20026 set uv anim?
	// 20027 BB::CEvent also suspend?
};

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
	MoveCommand_Move = 0x11002, //EGL::CEventPosition
	// 1100D, 1100E, 11017 something default movement
	// 11002 bino cancel
	// 12002 stop?
	Animation_SetAnimSet = 0x12007, //EGL::CEventValue<int,-27574121

	KegPlacer_Sabotage = 0x10107, //EGL::CEvent1Entity
	// 65804 -> 12002
	KegPpacer_Defuse = 0x1010E, //EGL::CEvent1Entity
	// 1010F -> 12002
	KegPlacer_GetSabotageProgress = 0x10112, //EGL::CEventGetValue<float,1468983543>
	KegPlacer_GetDisarmProgress = 0x10113, //EGL::CEventGetValue<float,1468983543>

	Binocular_ExploreCommand = 0x11102, //EGL::CEventPosition
	TorchPlacer_PlaceTorch = 0x11103, //EGL::CEventPosition
	PointToResources_Activate = 0x11104, //BB::CEvent

	// 13002, 13003, 13004 EGL::CEvent1Entity worker & some entity
	Worker_GetMotivation = 0x13007, //EGL::CEventGetValue<float,1468983543>
	Worker_GetWorkWaitUntil = 0x13008, //EGL::CEventGetValue<int,1211121895>
	Worker_GetWorkTaskList = 0x13009, //EGL::CEventGetValue<int,1211121895> checks building, then props
	Worker_LevyTaxes = 0x1300A, //BB::CEvent
	Worker_ForceToWork = 0x1300B, //BB::CEvent
	// 1300C most likely worker leave
	// worker 13013 get int something cycleindex
	// worker 13014 empty EGL::CEventValue<int,-27574121>
	Worker_Bless = 0x13015, //BB::CEvent
	Worker_AdvanceInCycles = 0x13016, //BB::CEvent
	// worker 13017 CycleIndex=5
	Worker_ChangeMoti = 0x13019, //GGL::CEventChangeMotivation
	Worker_GetMaxWorkTime = 0x1301A, //EGL::CEventGetValue<int, 1211121895>
	Worker_GetWorkTimeRemaining = 0x1301B, //EGL::CEventGetValue<int, 1211121895>
	// 1301D worker something entity
	Worker_GetResourceToRefine = 0x1301E, //EGL::CEventGetValue<int, 1211121895>
	// 13018 worker leave? EGL::CEvent1Entity
	// 13020 worker get some bool
	Worker_IsLeaving = 0x13021, //EGL::CEventGetValue<bool,1709081367>
	Worker_GetTransportModel = 0x13022, //EGL::CEventGetValue<int, 1211121895>
	// 13025 worker emtpty EGL::CEvent1Entity
	Worker_SetWorkTimeRemaining = 0x13029, //EGL::CEventValue<int,-27574121>
	// 18003 get bool true

	AttackEntity = 0x15004,
	// 1500E bino cancel
	BombPlacer_CommandPlaceBomb = 0x15033, //BB::CEvent,EGL::CEventPosition

	HeroHawk_SendHawk = 0x16002, //BB::CEvent,EGL::CEventPosition

	HeroAbility_IsAbilitySupported = 0x16008, //GGL::CEventHeroAbilityInteger
	HeroAbility_StandUpOrInit = 0x16009, //BB::CEvent
	CannonBuilder_BuildCannonCommand = 0x1600A, //GGL::CEventPositionAnd2EntityTypes
	HeroAbility_Cancel = 0x1600D, //BB::CEvent
	// 1500E convert cancel?
	HeroAbility_GetChargeCurrent = 0x16010, //GGL::CEventHeroAbilityInteger
	HeroAbility_GetChargeMax = 0x16011, //GGL::CEventHeroAbilityInteger
	HeroAbility_SetChargeCurrent = 0x16012, //GGL::CEventHeroAbilityInteger
	// 16013 ? fire projectile event???

	Camouflage_IsInvisible = 0x16013, //EGL::CEventGetValue<bool,1709081367>, 1504A same
	ThiefCamouflage_Reset = 0x16014, //BB::CEvent
	Camouflage_Activate = 0x16015, //BB::CEvent
	Camouflage_GetDurationMax = 0x16016, //EGL::CEventGetValue<int,1211121895>
	Camouflage_GetDurationCurrent = 0x16017, //EGL::CEventGetValue<int,1211121895>

	Sentinel_GetUrgency = 0x16019, //EGL::CEventGetValue<int,1211121895>

	Summon_ActivateCommand = 0x1601A, //BB::CEvent

	RangedEffect_Activate = 0x1601C, //BB::CEvent
	RangedEffect_GetDamageFactor = 0x1601D, //EGL::CEventGetValue<float,1468983543>
	RangedEffect_GetArmorFactor = 0x1601E, //EGL::CEventGetValue<float,1468983543>
	RangedEffect_GetDurationMax = 0x1601F, //EGL::CEventGetValue<int,1211121895>
	RangedEffect_GetDurationCurrent = 0x16020, //EGL::CEventGetValue<int,1211121895>

	CircularAttack_ActivateCommand = 0x16022, //BB::CEvent

	ComvertSettler_ActivateCommand = 0x16027, //EGL::CEvent1Entity
	// 16028 convert cancel?

	MotivateVorkers_ActivateCommand = 0x1602C, //BB::CEvent
	Sniper_SnipeCommand = 0x1602D, //EGL::CEvent1Entity
	Sniper_GetRange = 0x1602E, //EGL::CEventGetValue<float,1468983543>
	Shuriken_ActivateCommand = 0x1602F, //EGL::CEvent1Entity
	Shuriken_GetRange = 0x16030, //EGL::CEventGetValue<float,1468983543>

	InflictFear_Activate = 0x16026, //BB::CEvent

	Worker_ResetTaskList = 17012, //BB::CEvent

	// 20002 set walk target?
	// 20003 set walk target?

	Behavior_Tick = 0x20005, //BB::CEvent
	HeroAbility_Reset = 0x2000A, //GGL::CHeroAbility

	Animation_GetAnim = 0x20013, //EGL::CEventGetValue<int,1211121895>
	Animation_UnSuspend = 0x20014, //BB::CEvent may be swapped, check
	Animation_Suspend = 0x20015, //EGL::CEventValue<int,-27574121> argument seems to be tick
	Animation_SetAnim = 0x2001D, //EGL::CEventAnimation
	Animation_ResetTaskType = 0x2001E, //BB::CEvent
	// worker 20024 get some int
	// 20027 BB::CEvent also suspend?
};

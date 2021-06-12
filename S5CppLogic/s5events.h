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
	AttackEntity = 0x15004,

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

	Camouflage_IsInvisible = 0x16013, //EGL::CEventGetValue<bool,1709081367>, 1504A same
	Camouflage_Activate = 0x16015, //BB::CEvent
	Camouflage_GetDurationMax = 0x16016, //EGL::CEventGetValue<int,1211121895>
	Camouflage_GetDurationCurrent = 0x16017, //EGL::CEventGetValue<int,1211121895>

	Summon_ActivateCommand = 0x1601A, //BB::CEvent

	RangedEffect_Activate = 0x1601C, //BB::CEvent
	RangedEffect_GetDamageFactor = 0x1601D, //EGL::CEventGetValue<float,1468983543>
	RangedEffect_GetArmorFactor = 0x1601E, //EGL::CEventGetValue<float,1468983543>
	RangedEffect_GetDurationMax = 0x1601F, //EGL::CEventGetValue<int,1211121895>
	RangedEffect_GetDurationCurrent = 0x16020, //EGL::CEventGetValue<int,1211121895>

	CircularAttack_ActivateCommand = 0x16022, //BB::CEvent

	ComvertSettler_ActivateCommand = 0x16027, //EGL::CEvent1Entity
	// 16028 convert cancel?

	InflictFear_Activate = 0x16026, //BB::CEvent

	Behavior_Tick = 0x20005, //BB::CEvent
	HeroAbility_Reset = 0x2000A, //GGL::CHeroAbility

	// 1100D, 1100E, 11017 something default movement
	// 12002 stop?
};

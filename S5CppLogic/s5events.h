#pragma once
#include "s5data.h"

struct shok_event_data_EGL_CEventGetValue_int_1211121895 : shok_event_data {
	int vtable = 0x766CC4;
	int id = 0;
	int result = 0;
};
struct shok_vtable_EGL_CGLEEntity {
	PADDINGI(3);
	void(__thiscall* Destroy)(shok_EGL_CGLEEntity* th, int i);
	PADDINGI(12);
	void(__thiscall* FireEvent)(shok_EGL_CGLEEntity* th, shok_event_data* d);
	PADDINGI(11);
	float(__thiscall* GetExploration)(shok_EGL_CGLEEntity* e); // 28
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

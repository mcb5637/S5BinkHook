#pragma once
#include "s5data.h"

struct shok_BB_CEvent : shok_object {
	int EventTypeId;

	shok_BB_CEvent(int eventid);
	shok_BB_CEvent(shok_EventIDs eventid);
	shok_BB_CEvent(shok_NetEventIds eventid);
	shok_BB_CEvent(shok_FeedbackEventIds eventid);
	shok_BB_CEvent(shok_BB_CEvent&&) = default;
	shok_BB_CEvent(const shok_BB_CEvent&) = default;
	shok_BB_CEvent& operator=(shok_BB_CEvent&&) = default;
	shok_BB_CEvent& operator=(const shok_BB_CEvent&) = default;

	static inline constexpr int vtp = 0x762114;
	static inline constexpr int TypeDesc = 0x7FFE24;
};

// input events

struct shok_EGL_CEventValue_int : shok_BB_CEvent { // EGL::CEventValue<int,-27574121>
	int Data;

	shok_EGL_CEventValue_int(shok_EventIDs e, int d);
	shok_EGL_CEventValue_int(shok_EGL_CEventValue_int&&) = default;
	shok_EGL_CEventValue_int(const shok_EGL_CEventValue_int&) = default;
	shok_EGL_CEventValue_int& operator=(shok_EGL_CEventValue_int&&) = default;
	shok_EGL_CEventValue_int& operator=(const shok_EGL_CEventValue_int&) = default;

	static inline constexpr int vtp = 0x763130;
	static inline constexpr int TypeDesc = 0x801270;
};
struct shok_EGL_CEventValue_bool : shok_BB_CEvent { // EGL::CEventValue<bool,703333479>
	bool Data;

	shok_EGL_CEventValue_bool(shok_EventIDs e, bool d);
	shok_EGL_CEventValue_bool(shok_EGL_CEventValue_bool&&) = default;
	shok_EGL_CEventValue_bool(const shok_EGL_CEventValue_bool&) = default;
	shok_EGL_CEventValue_bool& operator=(shok_EGL_CEventValue_bool&&) = default;
	shok_EGL_CEventValue_bool& operator=(const shok_EGL_CEventValue_bool&) = default;

	static inline constexpr int vtp = 0x76E220;
	static inline constexpr int TypeDesc = 0x810ADC;
};
struct shok_EGL_CEventValue_float : shok_BB_CEvent { // EGL::CEventValue<float,1278362727>
	float Data;

	shok_EGL_CEventValue_float(shok_EventIDs e, float d);
	shok_EGL_CEventValue_float(shok_EGL_CEventValue_float&&) = default;
	shok_EGL_CEventValue_float(const shok_EGL_CEventValue_float&) = default;
	shok_EGL_CEventValue_float& operator=(shok_EGL_CEventValue_float&&) = default;
	shok_EGL_CEventValue_float& operator=(const shok_EGL_CEventValue_float&) = default;

	static inline constexpr int vtp = 0x76F8F4;
	static inline constexpr int TypeDesc = 0x8117B0;
};

struct shok_EGL_CEventGetValue_bool : shok_BB_CEvent { // EGL::CEventGetValue<bool,1709081367>
	bool Data = false;

	shok_EGL_CEventGetValue_bool(shok_EventIDs e);
	shok_EGL_CEventGetValue_bool(shok_EGL_CEventGetValue_bool&&) = default;
	shok_EGL_CEventGetValue_bool(const shok_EGL_CEventGetValue_bool&) = default;
	shok_EGL_CEventGetValue_bool& operator=(shok_EGL_CEventGetValue_bool&&) = default;
	shok_EGL_CEventGetValue_bool& operator=(const shok_EGL_CEventGetValue_bool&) = default;

	static inline constexpr int vtp = 0x766CB4;
	static inline constexpr int TypeDesc = 0x807A70;
};
struct shok_EGL_CEventGetValue_int : shok_BB_CEvent { // EGL::CEventGetValue<int,1211121895>
	int Data = 0;

	shok_EGL_CEventGetValue_int(shok_EventIDs e);
	shok_EGL_CEventGetValue_int(shok_EGL_CEventGetValue_int&&) = default;
	shok_EGL_CEventGetValue_int(const shok_EGL_CEventGetValue_int&) = default;
	shok_EGL_CEventGetValue_int& operator=(shok_EGL_CEventGetValue_int&&) = default;
	shok_EGL_CEventGetValue_int& operator=(const shok_EGL_CEventGetValue_int&) = default;

	static inline constexpr int vtp = 0x766CC4;
	static inline constexpr int TypeDesc = 0x807AA4;
};
struct shok_EGL_CEventGetValue_float : shok_BB_CEvent { // EGL::CEventGetValue<float,1468983543>
	float Data = 0.0f;

	shok_EGL_CEventGetValue_float(shok_EventIDs e);
	shok_EGL_CEventGetValue_float(shok_EGL_CEventGetValue_float&&) = default;
	shok_EGL_CEventGetValue_float(const shok_EGL_CEventGetValue_float&) = default;
	shok_EGL_CEventGetValue_float& operator=(shok_EGL_CEventGetValue_float&&) = default;
	shok_EGL_CEventGetValue_float& operator=(const shok_EGL_CEventGetValue_float&) = default;

	static inline constexpr int vtp = 0x76E210;
	static inline constexpr int TypeDesc = 0x810AAC;
};
// EGL::CEventGetValue<GGL::CBuildingMerchantBehaviorProps const *,-1872076829> 7704A4
// EGL::CEventGetValue<GGL::CNeutralBridgeBehavior *,1150290935>
// EGL::CEventGetValue<GGL::CBattleBehavior *,-646837913>
// EGL::CEventGetValue<GGL::CSoldierBehavior *,-806903129>
// EGL::CEventGetValue<GGL::CLeaderBehavior *,-1190255961>
// EGL::CEventGetValue<GGL::CBehaviorFollow *,-1301899769>
// EGL::CEventGetValue<EGL::CMovementBehavior *,212523703>

// GGL::CEventHeroAbilityInteger -> valueint
// GGL::CEventHeroAbilityGetInteger -> getvalueint


struct shok_EGL_CEvent1Entity : shok_BB_CEvent {
private:
	int vtable_EGL_IEventEntityID = 0x766C58;
public:
	int EntityID;

	shok_EGL_CEvent1Entity(shok_EventIDs e, int ent);
	shok_EGL_CEvent1Entity(shok_EGL_CEvent1Entity&&) = default;
	shok_EGL_CEvent1Entity(const shok_EGL_CEvent1Entity&) = default;
	shok_EGL_CEvent1Entity& operator=(shok_EGL_CEvent1Entity&&) = default;
	shok_EGL_CEvent1Entity& operator=(const shok_EGL_CEvent1Entity&) = default;

	static inline constexpr int vtp = 0x766C60;
	static inline constexpr int TypeDesc = 0x8079A8;
};

struct shok_EGL_CEventPosition : shok_BB_CEvent {
	shok_position Position;

	shok_EGL_CEventPosition(shok_EventIDs e, const shok_position& p);
	shok_EGL_CEventPosition(shok_EGL_CEventPosition&&) = default;
	shok_EGL_CEventPosition(const shok_EGL_CEventPosition&) = default;
	shok_EGL_CEventPosition& operator=(shok_EGL_CEventPosition&&) = default;
	shok_EGL_CEventPosition& operator=(const shok_EGL_CEventPosition&) = default;

	static inline constexpr int vtp = 0x766C70;
	static inline constexpr int TypeDesc = 0x8079C8;
};

struct shok_GGL_CEventAttachmentTypeGetInteger : shok_BB_CEvent {
	shok_AttachmentType AttachmentType;
	int Data = 0;

	shok_GGL_CEventAttachmentTypeGetInteger(shok_EventIDs e, shok_AttachmentType t);
	shok_GGL_CEventAttachmentTypeGetInteger(shok_GGL_CEventAttachmentTypeGetInteger&&) = default;
	shok_GGL_CEventAttachmentTypeGetInteger(const shok_GGL_CEventAttachmentTypeGetInteger&) = default;
	shok_GGL_CEventAttachmentTypeGetInteger& operator=(shok_GGL_CEventAttachmentTypeGetInteger&&) = default;
	shok_GGL_CEventAttachmentTypeGetInteger& operator=(const shok_GGL_CEventAttachmentTypeGetInteger&) = default;

	static inline constexpr int vtp = 0x766C80;
	static inline constexpr int TypeDesc = 0x8079EC;
};
struct shok_GGL_CEventAttachmentTypeInteger : shok_BB_CEvent {
	shok_AttachmentType AttachmentType;
	int Data = 0;

	shok_GGL_CEventAttachmentTypeInteger(shok_EventIDs e, shok_AttachmentType t, int d);
	shok_GGL_CEventAttachmentTypeInteger(shok_GGL_CEventAttachmentTypeInteger&&) = default;
	shok_GGL_CEventAttachmentTypeInteger(const shok_GGL_CEventAttachmentTypeInteger&) = default;
	shok_GGL_CEventAttachmentTypeInteger& operator=(shok_GGL_CEventAttachmentTypeInteger&&) = default;
	shok_GGL_CEventAttachmentTypeInteger& operator=(const shok_GGL_CEventAttachmentTypeInteger&) = default;

	static inline constexpr int vtp = 0x775E28;
	static inline constexpr int TypeDesc = 0x81D3A8;
};

struct shok_GGL_CEventEntityIndex : shok_BB_CEvent {
	int EntityId;
	int Index;

	shok_GGL_CEventEntityIndex(shok_EventIDs e, int eid, int ind);
	shok_GGL_CEventEntityIndex(shok_GGL_CEventEntityIndex&&) = default;
	shok_GGL_CEventEntityIndex(const shok_GGL_CEventEntityIndex&) = default;
	shok_GGL_CEventEntityIndex& operator=(shok_GGL_CEventEntityIndex&&) = default;
	shok_GGL_CEventEntityIndex& operator=(const shok_GGL_CEventEntityIndex&) = default;

	static inline constexpr int vtp = 0x766C90;
	static inline constexpr int TypeDesc = 0x807A20;
};

// ENetworkX::CEventBase probably never used, since servers are down

// EGL::CEventSoundPositionAndID

struct shok_EGL_CEventPlayerID : shok_BB_CEvent {
private:
	int vtable_EGL_IEventPlayerID = 0x76D8F8;
public:
	int PlayerID;

	shok_EGL_CEventPlayerID(shok_EventIDs e, int p);
	shok_EGL_CEventPlayerID(shok_EGL_CEventPlayerID&&) = default;
	shok_EGL_CEventPlayerID(const shok_EGL_CEventPlayerID&) = default;
	shok_EGL_CEventPlayerID& operator=(shok_EGL_CEventPlayerID&&) = default;
	shok_EGL_CEventPlayerID& operator=(const shok_EGL_CEventPlayerID&) = default;

	static inline constexpr int vtp = 0x76D900;
	static inline constexpr int TypeDesc = 0x80E078;
};

struct shok_EGL_CEventSubAnim : shok_BB_CEvent {
	int AnimId, SubAnimIndex;
	bool PlayBackwards, IsLooped;

	shok_EGL_CEventSubAnim(shok_EventIDs e, int an, int subind, bool back, bool loop);
	shok_EGL_CEventSubAnim(shok_EGL_CEventSubAnim&&) = default;
	shok_EGL_CEventSubAnim(const shok_EGL_CEventSubAnim&) = default;
	shok_EGL_CEventSubAnim& operator=(shok_EGL_CEventSubAnim&&) = default;
	shok_EGL_CEventSubAnim& operator=(const shok_EGL_CEventSubAnim&) = default;

	static inline constexpr int vtp = 0x76D910;
	static inline constexpr int TypeDesc = 0x80E09C;
};

struct shok_EGL_CEvent2Entities : shok_BB_CEvent {
private:
	int vtable_EGL_IEvent2Entities = 0x76D920;
public:
	int ActorId, TargetId;

	shok_EGL_CEvent2Entities(shok_EventIDs e, int aid, int tid);
	shok_EGL_CEvent2Entities(shok_EGL_CEvent2Entities&&) = default;
	shok_EGL_CEvent2Entities(const shok_EGL_CEvent2Entities&) = default;
	shok_EGL_CEvent2Entities& operator=(shok_EGL_CEvent2Entities&&) = default;
	shok_EGL_CEvent2Entities& operator=(const shok_EGL_CEvent2Entities&) = default;

	static inline constexpr int vtp = 0x76D92C;
	static inline constexpr int TypeDesc = 0x80E0BC;
};

struct shok_GGL_CEventTransaction : shok_BB_CEvent {
	shok_ResourceType SellType, BuyType;
	float BuyAmount;

	shok_GGL_CEventTransaction(shok_EventIDs e, shok_ResourceType sell, shok_ResourceType buy, float buyAm);
	shok_GGL_CEventTransaction(shok_GGL_CEventTransaction&&) = default;
	shok_GGL_CEventTransaction(const shok_GGL_CEventTransaction&) = default;
	shok_GGL_CEventTransaction& operator=(shok_GGL_CEventTransaction&&) = default;
	shok_GGL_CEventTransaction& operator=(const shok_GGL_CEventTransaction&) = default;

	static inline constexpr int vtp = 0x76D93C;
	static inline constexpr int TypeDesc = 0x80E0E0;
};

struct shok_GGL_CEventPositionAnd2EntityTypes : shok_EGL_CEventPosition {
	int Type1, Type2;

	shok_GGL_CEventPositionAnd2EntityTypes(shok_EventIDs e, const shok_position& p, int t1, int t2);
	shok_GGL_CEventPositionAnd2EntityTypes(shok_GGL_CEventPositionAnd2EntityTypes&&) = default;
	shok_GGL_CEventPositionAnd2EntityTypes(const shok_GGL_CEventPositionAnd2EntityTypes&) = default;
	shok_GGL_CEventPositionAnd2EntityTypes& operator=(shok_GGL_CEventPositionAnd2EntityTypes&&) = default;
	shok_GGL_CEventPositionAnd2EntityTypes& operator=(const shok_GGL_CEventPositionAnd2EntityTypes&) = default;

	static inline constexpr int vtp = 0x76D94C;
	static inline constexpr int TypeDesc = 0x80E104;
};

struct shok_GGL_CEventEntityAttachment : shok_BB_CEvent {
	shok_AttachmentType Type;
	int EntityId;
	shok_EventIDs DetachEvent;

	shok_GGL_CEventEntityAttachment(shok_EventIDs e, shok_AttachmentType ty, int eid, shok_EventIDs detach);
	shok_GGL_CEventEntityAttachment(shok_GGL_CEventEntityAttachment&&) = default;
	shok_GGL_CEventEntityAttachment(const shok_GGL_CEventEntityAttachment&) = default;
	shok_GGL_CEventEntityAttachment& operator=(shok_GGL_CEventEntityAttachment&&) = default;
	shok_GGL_CEventEntityAttachment& operator=(const shok_GGL_CEventEntityAttachment&) = default;

	static inline constexpr int vtp = 0x770844;
	static inline constexpr int TypeDesc = 0x8129E0;
};

// GGL::CEventPlayerIDInteger -> EGL::CEventPlayerID 76D964
// EGL::CEventPositionAndEntity 76E150
// GGL::CEventSourceTargetPlayerID 76F8BC
// GGL::CEventDiplomacyChanged -> GGL::CEventSourceTargetPlayerID 76F8E4
// GGL::CEventChangeMotivation -> EGL::CEventValue<float,1278362727> 76F904
// GGL::CEventWeatherStateChanged 770130
// GGL::CEventBridgeProgress -> EGL::CEventPlayerID 770494
// EGL::CEventEntityGetBool
// EGL::CEventPlayerGetBool
// GGL::CEventTributePaid
// GGL::CEventStartAlphaBlending
// GGL::CEventOfferBase
// GGL::CEventMercenaryOffer GGL::CEventGetMercenaryOffer
// GGL::CEventTechOffer GGL::CEventGetTechOffer
// EGL::CEventRowColumn GGL::CEventGetRowColumn
// ECore::CECoreEventInteger
// EGUIX::CEventUpdateTime
// EGUIX::CGroupEvent
// EGL::CEvent1Effect
// EGL::CEventIndexAndEffectType

// GGL::CEventEntityAttachment
// GGL::CEventAttachmentType
// GGL::CEventAttachmentTypeGetBool
// EGL::CEventUVAnim
// EGL::CEventGetPosition
// GGL::CEventGoodsTraded -> GGL::CEventTransaction
// EGL::CEventThousandthsGetInteger
// EGL::CEventPositionAndTaskState
// GGL::CEventFollowInfo
// GGL::CEventKegInfo
// GGL::CEventGetPositionFromID
// GGL::CEventIndex
// EGL::CEventAnimation
// EGL::CEventEntityInRangeOfEntity

// net events
struct shok_EGL_CNetEvent2Entities : shok_BB_CEvent {
	int ActorId, TargetId;

	shok_EGL_CNetEvent2Entities(shok_NetEventIds id, int actor, int tar);
	shok_EGL_CNetEvent2Entities(shok_EGL_CNetEvent2Entities&&) = default;
	shok_EGL_CNetEvent2Entities(const shok_EGL_CNetEvent2Entities&) = default;
	shok_EGL_CNetEvent2Entities& operator=(shok_EGL_CNetEvent2Entities&&) = default;
	shok_EGL_CNetEvent2Entities& operator=(const shok_EGL_CNetEvent2Entities&) = default;

	static inline constexpr int vtp = 0x76DD60;
	static inline constexpr int TypeDesc = 0x80FF1C;
};

struct shok_EGL_CNetEventEntityAndPos : shok_BB_CEvent {
	int EntityId;
	shok_position Position;

	shok_EGL_CNetEventEntityAndPos(shok_NetEventIds id, int ent, const shok_position& pos);
	shok_EGL_CNetEventEntityAndPos(shok_EGL_CNetEventEntityAndPos&&) = default;
	shok_EGL_CNetEventEntityAndPos(const shok_EGL_CNetEventEntityAndPos&) = default;
	shok_EGL_CNetEventEntityAndPos& operator=(shok_EGL_CNetEventEntityAndPos&&) = default;
	shok_EGL_CNetEventEntityAndPos& operator=(const shok_EGL_CNetEventEntityAndPos&) = default;

	static inline constexpr int vtp = 0x76DD50;
	static inline constexpr int TypeDesc = 0x80FEF4;
};

struct shok_EGL_CNetEventEntityAndPosArray : shok_BB_CEvent {
	int EntityId;
	vector_padding;
	std::vector<shok_position, shok_allocator<shok_position>> Positions;
	float Rotation;

	shok_EGL_CNetEventEntityAndPosArray(shok_NetEventIds id, int ent, float r);
	shok_EGL_CNetEventEntityAndPosArray(const shok_EGL_CNetEventEntityAndPosArray&) = default;
	shok_EGL_CNetEventEntityAndPosArray& operator=(const shok_EGL_CNetEventEntityAndPosArray&) = default;

	static inline constexpr int vtp = 0x770704;
	static inline constexpr int TypeDesc = 0x8125EC;
};

struct shok_GGL_CNetEventExtractResource : shok_BB_CEvent {
	int EntityId;
	shok_ResourceType ResourceType;
	shok_position Position;

	shok_GGL_CNetEventExtractResource(shok_NetEventIds id, int ent, shok_ResourceType resty, const shok_position& pos);
	shok_GGL_CNetEventExtractResource(shok_GGL_CNetEventExtractResource&&) = default;
	shok_GGL_CNetEventExtractResource(const shok_GGL_CNetEventExtractResource&) = default;
	shok_GGL_CNetEventExtractResource& operator=(shok_GGL_CNetEventExtractResource&&) = default;
	shok_GGL_CNetEventExtractResource& operator=(const shok_GGL_CNetEventExtractResource&) = default;

	static inline constexpr int vtp = 0x77061C;
	static inline constexpr int TypeDesc = 0x81252C;
};

struct shok_GGL_CNetEventTransaction : shok_BB_CEvent {
	int EntityId;
	shok_ResourceType SellType, BuyType;
	int BuyAmount;

	shok_GGL_CNetEventTransaction(shok_NetEventIds shok_NetEventIds, int ent, shok_ResourceType sell, shok_ResourceType buy, int buyam);
	shok_GGL_CNetEventTransaction(shok_GGL_CNetEventTransaction&&) = default;
	shok_GGL_CNetEventTransaction(const shok_GGL_CNetEventTransaction&) = default;
	shok_GGL_CNetEventTransaction& operator=(shok_GGL_CNetEventTransaction&&) = default;
	shok_GGL_CNetEventTransaction& operator=(const shok_GGL_CNetEventTransaction&) = default;

	static inline constexpr int vtp = 0x77062C;
	static inline constexpr int TypeDesc = 0x812558;
};

struct shok_EGL_CNetEventEntityID : shok_BB_CEvent {
	int EntityId;

	shok_EGL_CNetEventEntityID(shok_NetEventIds shok_NetEventIds, int ent);
	shok_EGL_CNetEventEntityID(shok_FeedbackEventIds shok_NetEventIds, int ent);
	shok_EGL_CNetEventEntityID(shok_EGL_CNetEventEntityID&&) = default;
	shok_EGL_CNetEventEntityID(const shok_EGL_CNetEventEntityID&) = default;
	shok_EGL_CNetEventEntityID& operator=(shok_EGL_CNetEventEntityID&&) = default;
	shok_EGL_CNetEventEntityID& operator=(const shok_EGL_CNetEventEntityID&) = default;

	static inline constexpr int vtp = 0x766C28;
	static inline constexpr int TypeDesc = 0x807924;
};

struct shok_GGL_CNetEventCannonCreator : shok_EGL_CNetEventEntityID {
	int BottomType, TopType;
	shok_position Position;

	shok_GGL_CNetEventCannonCreator(shok_NetEventIds id, int ent, int bty, int tty, const shok_position& pos);
	shok_GGL_CNetEventCannonCreator(shok_GGL_CNetEventCannonCreator&&) = default;
	shok_GGL_CNetEventCannonCreator(const shok_GGL_CNetEventCannonCreator&) = default;
	shok_GGL_CNetEventCannonCreator& operator=(shok_GGL_CNetEventCannonCreator&&) = default;
	shok_GGL_CNetEventCannonCreator& operator=(const shok_GGL_CNetEventCannonCreator&) = default;

	static inline constexpr int TypeDesc = 0x812494;
	static inline constexpr int vtp = 0x7705EC;
};

struct shok_GGL_CNetEventEntityIDAndUpgradeCategory : shok_EGL_CNetEventEntityID {
	int UpgradeCategory;

	shok_GGL_CNetEventEntityIDAndUpgradeCategory(shok_NetEventIds id, int ent, int ucat);
	shok_GGL_CNetEventEntityIDAndUpgradeCategory(shok_GGL_CNetEventEntityIDAndUpgradeCategory&&) = default;
	shok_GGL_CNetEventEntityIDAndUpgradeCategory(const shok_GGL_CNetEventEntityIDAndUpgradeCategory&) = default;
	shok_GGL_CNetEventEntityIDAndUpgradeCategory& operator=(shok_GGL_CNetEventEntityIDAndUpgradeCategory&&) = default;
	shok_GGL_CNetEventEntityIDAndUpgradeCategory& operator=(const shok_GGL_CNetEventEntityIDAndUpgradeCategory&) = default;

	static inline constexpr int TypeDesc = 0x8124F4;
	static inline constexpr int vtp = 0x77060C;
};

struct shok_EGL_CNetEventEntityIDAndInteger : shok_EGL_CNetEventEntityID {
	int Data;

	shok_EGL_CNetEventEntityIDAndInteger(shok_NetEventIds id, int ent, int d);
	shok_EGL_CNetEventEntityIDAndInteger(shok_FeedbackEventIds id, int ent, int d);
	shok_EGL_CNetEventEntityIDAndInteger(shok_EGL_CNetEventEntityIDAndInteger&&) = default;
	shok_EGL_CNetEventEntityIDAndInteger(const shok_EGL_CNetEventEntityIDAndInteger&) = default;
	shok_EGL_CNetEventEntityIDAndInteger& operator=(shok_EGL_CNetEventEntityIDAndInteger&&) = default;
	shok_EGL_CNetEventEntityIDAndInteger& operator=(const shok_EGL_CNetEventEntityIDAndInteger&) = default;

	static inline constexpr int TypeDesc = 0x807978;
	static inline constexpr int vtp = 0x766C48;
};

struct shok_GGL_CNetEventTechnologyAndEntityID : shok_EGL_CNetEventEntityID {
	int Technology;

	shok_GGL_CNetEventTechnologyAndEntityID(shok_NetEventIds id, int ent, int tech);
	shok_GGL_CNetEventTechnologyAndEntityID(shok_GGL_CNetEventTechnologyAndEntityID&&) = default;
	shok_GGL_CNetEventTechnologyAndEntityID(const shok_GGL_CNetEventTechnologyAndEntityID&) = default;
	shok_GGL_CNetEventTechnologyAndEntityID& operator=(shok_GGL_CNetEventTechnologyAndEntityID&&) = default;
	shok_GGL_CNetEventTechnologyAndEntityID& operator=(const shok_GGL_CNetEventTechnologyAndEntityID&) = default;

	static inline constexpr int TypeDesc = 0x8124C0;
	static inline constexpr int vtp = 0x7705FC;
};

struct shok_EGL_CNetEventPlayerID : shok_BB_CEvent {
	int PlayerId;

	shok_EGL_CNetEventPlayerID(shok_NetEventIds id, int pl);
	shok_EGL_CNetEventPlayerID(shok_FeedbackEventIds id, int pl);
	shok_EGL_CNetEventPlayerID(shok_EGL_CNetEventPlayerID&&) = default;
	shok_EGL_CNetEventPlayerID(const shok_EGL_CNetEventPlayerID&) = default;
	shok_EGL_CNetEventPlayerID& operator=(shok_EGL_CNetEventPlayerID&&) = default;
	shok_EGL_CNetEventPlayerID& operator=(const shok_EGL_CNetEventPlayerID&) = default;

	static inline constexpr int TypeDesc = 0x807900;
	static inline constexpr int vtp = 0x766C18;
};

struct shok_GGL_CNetEventBuildingCreator : shok_EGL_CNetEventPlayerID { // add at least one serf, or it crashes
	int UpgradeCategory;
	shok_positionRot Position;
	vector_padding;
	std::vector<int, shok_allocator<int>> Serfs;

	shok_GGL_CNetEventBuildingCreator(shok_NetEventIds id, int pl, int ucat, const shok_positionRot& p);
	shok_GGL_CNetEventBuildingCreator(shok_GGL_CNetEventBuildingCreator&&) = default;
	shok_GGL_CNetEventBuildingCreator(const shok_GGL_CNetEventBuildingCreator&) = default;
	shok_GGL_CNetEventBuildingCreator& operator=(shok_GGL_CNetEventBuildingCreator&&) = default;
	shok_GGL_CNetEventBuildingCreator& operator=(const shok_GGL_CNetEventBuildingCreator&) = default;

	static inline constexpr int TypeDesc = 0x81261C;
	static inline constexpr int vtp = 0x770714;
};

struct shok_EGL_CNetEventIntegerAndPlayerID : shok_EGL_CNetEventPlayerID {
	int Data;

	shok_EGL_CNetEventIntegerAndPlayerID(shok_NetEventIds id, int pl, int d);
	shok_EGL_CNetEventIntegerAndPlayerID(shok_EGL_CNetEventIntegerAndPlayerID&&) = default;
	shok_EGL_CNetEventIntegerAndPlayerID(const shok_EGL_CNetEventIntegerAndPlayerID&) = default;
	shok_EGL_CNetEventIntegerAndPlayerID& operator=(shok_EGL_CNetEventIntegerAndPlayerID&&) = default;
	shok_EGL_CNetEventIntegerAndPlayerID& operator=(const shok_EGL_CNetEventIntegerAndPlayerID&) = default;

	static inline constexpr int TypeDesc = 0x812404;
	static inline constexpr int vtp = 0x7705BC;
};

struct shok_EGL_CNetEventPlayerIDAndInteger : shok_EGL_CNetEventPlayerID {
	int Data;

	shok_EGL_CNetEventPlayerIDAndInteger(shok_NetEventIds id, int pl, int d);
	shok_EGL_CNetEventPlayerIDAndInteger(shok_EGL_CNetEventPlayerIDAndInteger&&) = default;
	shok_EGL_CNetEventPlayerIDAndInteger(const shok_EGL_CNetEventPlayerIDAndInteger&) = default;
	shok_EGL_CNetEventPlayerIDAndInteger& operator=(shok_EGL_CNetEventPlayerIDAndInteger&&) = default;
	shok_EGL_CNetEventPlayerIDAndInteger& operator=(const shok_EGL_CNetEventPlayerIDAndInteger&) = default;

	static inline constexpr int TypeDesc = 0x812434;
	static inline constexpr int vtp = 0x7705CC;
};

struct shok_EGL_CNetEventEntityIDAndPlayerID : shok_EGL_CNetEventPlayerID {
	int EntityId;

	shok_EGL_CNetEventEntityIDAndPlayerID(shok_NetEventIds id, int pl, int e);
	shok_EGL_CNetEventEntityIDAndPlayerID(shok_FeedbackEventIds id, int pl, int e);
	shok_EGL_CNetEventEntityIDAndPlayerID(shok_EGL_CNetEventEntityIDAndPlayerID&&) = default;
	shok_EGL_CNetEventEntityIDAndPlayerID(const shok_EGL_CNetEventEntityIDAndPlayerID&) = default;
	shok_EGL_CNetEventEntityIDAndPlayerID& operator=(shok_EGL_CNetEventEntityIDAndPlayerID&&) = default;
	shok_EGL_CNetEventEntityIDAndPlayerID& operator=(const shok_EGL_CNetEventEntityIDAndPlayerID&) = default;

	static inline constexpr int TypeDesc = 0x807948;
	static inline constexpr int vtp = 0x766C38;
};

struct shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType : shok_EGL_CNetEventEntityIDAndPlayerID {
	int EntityType;

	shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType(shok_NetEventIds id, int pl, int e, int ety);
	shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType(shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType&&) = default;
	shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType(const shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType&) = default;
	shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType& operator=(shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType&&) = default;
	shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType& operator=(const shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType&) = default;

	static inline constexpr int TypeDesc = 0x812340;
	static inline constexpr int vtp = 0x77057C;
};

struct shok_GGL_CNetEventEntityIDPlayerIDAndInteger : shok_EGL_CNetEventEntityIDAndPlayerID {
	int Data;

	shok_GGL_CNetEventEntityIDPlayerIDAndInteger(shok_NetEventIds id, int pl, int e, int d);
	shok_GGL_CNetEventEntityIDPlayerIDAndInteger(shok_GGL_CNetEventEntityIDPlayerIDAndInteger&&) = default;
	shok_GGL_CNetEventEntityIDPlayerIDAndInteger(const shok_GGL_CNetEventEntityIDPlayerIDAndInteger&) = default;
	shok_GGL_CNetEventEntityIDPlayerIDAndInteger& operator=(shok_GGL_CNetEventEntityIDPlayerIDAndInteger&&) = default;
	shok_GGL_CNetEventEntityIDPlayerIDAndInteger& operator=(const shok_GGL_CNetEventEntityIDPlayerIDAndInteger&) = default;

	static inline constexpr int TypeDesc = 0x8125B4;
	static inline constexpr int vtp = 0x77064C;
};

struct shok_GGL_CFeedbackEventBattling : shok_EGL_CNetEventEntityIDAndPlayerID {
	shok_position Pos;
	int TargetPlayer;

	shok_GGL_CFeedbackEventBattling(shok_FeedbackEventIds id, int eid, int p, const shok_position& pos, int tp);
	shok_GGL_CFeedbackEventBattling(shok_GGL_CFeedbackEventBattling&&) = default;
	shok_GGL_CFeedbackEventBattling(const shok_GGL_CFeedbackEventBattling&) = default;
	shok_GGL_CFeedbackEventBattling& operator=(shok_GGL_CFeedbackEventBattling&&) = default;
	shok_GGL_CFeedbackEventBattling& operator=(const shok_GGL_CFeedbackEventBattling&) = default;

	static inline constexpr int TypeDesc = 0x80FFE4;
	static inline constexpr int vtp = 0x76DDA0;
};

struct shok_Framework_CEventGameSpeed : shok_BB_CEvent {
	double SpeedFactor;

	shok_Framework_CEventGameSpeed(shok_NetEventIds id, double speed);
	shok_Framework_CEventGameSpeed(shok_Framework_CEventGameSpeed&&) = default;
	shok_Framework_CEventGameSpeed(const shok_Framework_CEventGameSpeed&) = default;
	shok_Framework_CEventGameSpeed& operator=(shok_Framework_CEventGameSpeed&&) = default;
	shok_Framework_CEventGameSpeed& operator=(const shok_Framework_CEventGameSpeed&) = default;

	static inline constexpr int TypeDesc = 0x800078;
	static inline constexpr int vtp = 0x762474;
};

struct shok_EGL_IGLEHandler_BB_CEvent_void : shok_object {
	void* Object;
	int(__thiscall* Func)(void* th, shok_BB_CEvent* ev);
};

enum class shok_EventIDs : int {
	NoDetachEvent = 0, // only use for attachmen detach event

	ResourceDoodad_RemoveOneRes = 0x10002, //BB::CEvent unused
	OnResourceDoodadDetach = 0x10003, //EGL::CEvent1Entity, also bserf
	Mine_DoWorkStep = 0x10004, //BB::CEvent
	Follow_GetFollowStatus = 0x10008, //EGL::CEventGetValue<int,1211121895>
	Mine_GetResourcesRemaining = 0x10007, //EGL::CEventGetValue<int,1211121895>
	Serf_ExtractResource = 0x10009, //EGL::CEvent1Entity, serfbattle same as 14003
	// 1000A serf something extract res get bool
	ResourceRefiner_Refine = 0x1000B, //BB::CEvent
	ResourceTree_Init = 0x1000C, //EGL::CEventGetValue<int,1211121895>
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

	// 0x11013 evadingent
	// 0x11014 evadingent to 0x11011 cevent
	// 0x11015 evadingent

	// 0x12002 stop?
	// 0x12003 camp detach worker?
	// 0x12004 settler wait? state 5 EGL::CEventValue<int,-27574121>
	Animation_SetAnimSet = 0x12007, //EGL::CEventValue<int,-27574121
	// 12008 leader hurt?

	Binocular_ExploreCommand = 0x11102, //EGL::CEventPosition
	TorchPlacer_PlaceTorch = 0x11103, //EGL::CEventPosition
	PointToResources_Activate = 0x11104, //BB::CEvent

	Worker_WorkPlaceBuildingDestroyed = 0x13002, //EGL::CEvent1Entity serf probably construction/repair destr
	Worker_FarmBuildingDestroyed = 0x13003, //EGL::CEvent1Entity
	Worker_ResidencePlaceBuildingDestroyed = 0x13004, //EGL::CEvent1Entity
	FeedbackEvent_BattleOnDamageDealt = 0x13005, //GGL::CFeedbackEventBattling
	Worker_GetMotivation = 0x13007, //EGL::CEventGetValue<float,1468983543>
	Worker_GetWorkWaitUntil = 0x13008, //EGL::CEventGetValue<int,1211121895>
	Worker_GetWorkTaskList = 0x13009, //EGL::CEventGetValue<int,1211121895> checks building, then props
	Worker_LevyTaxes = 0x1300A, //BB::CEvent
	Worker_ForceToWork = 0x1300B, //BB::CEvent
	// 1300C most likely worker leave
	// 0x1300D camp get num slots?
	// 0x1300E camp get pos from slot?
	// 0x1300F camp attach worker?
	// 0x13010 camp has free slot?
	// 0x13011 worker on camp detach?
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
	// 0x13026 neutralbridge progress?
	NeutralBridge_GetNeutralBridgeBehavior = 0x13027, //EGL::CEventGetValue<GGL::CNeutralBridgeBehavior *,1150290935>
	// 0x13028 neutralbridge get progress?
	Worker_SetWorkTimeRemaining = 0x13029, //EGL::CEventValue<int,-27574121>
	SettlerMerchant_GetBuildingId = 0x1302A, //EGL::CEventGetValue<int, 1211121895>

	// 0x14002 buildingbeh 1 ent
	Serf_Construct = 0x14003, // GGL::CEventEntityIndex serfbattle?
	Keep_BuySerfCommand = 0x14004, //BB::CEvent
	Serf_CommandTurnToBattleSerf = 0x14005, //BB::CEvent battleserf stop if toserf tl
	BattleSerf_CommandTurnToSerf = 0x14006, //BB::CEvent serf stop if toserf tl
	Serf_GetResourceAttachmentOffset = 0x14007, //EGL::CEventGetValue<float,1468983543> used for resouce finder
	BattleSerf_GetTimeToChangeBack = 0x14008, //EGL::CEventGetValue<int, 1211121895>

	// 18003 get bool true

	Leader_AttackEntity = 0x15004, //EGL::CEvent1Entity
	Leader_OnAttackCommandTargetDetach = 0x15005, //EGL::CEvent1Entity
	Soldier_OnLeaderDetach = 0x15007, //EGL::CEvent1Entity
	Leader_OnSoldierDetach = 0x15008, //EGL::CEvent1Entity
	Leader_AttachSoldier = 0x15009, //EGL::CEvent1Entity
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
	Battle_OnTargetDetached = 0x1501D, //EGL::CEvent1Entity
	// 1501E leader get some unknown int +100, soldier something other entity???
	Leader_GetMaxNumSoldier = 0x1501F, //EGL::CEventGetValue<int, 1211121895>
	GetArmorClass = 0x15020, //EGL::CEventGetValue<int,1211121895>
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
	Barracks_BuyLeader = 0x15035, //EGL::CEventValue<int,-27574121> ucat
	Leader_GetNearbyBarracks = 0x15036, //EGL::CEventGetValue<int, 1211121895>
	Barracks_BuySoldierForLeader = 0x15037, //EGL::CEvent1Entity
	// 15038 leader? goes to defend after something else
	Leader_OnGuardedAttackedBy = 15039, //EGL::CEvent1Entity
	// 1503A leader
	Leader_SetIsUsingTargetOrientation = 0x1503B, //EGL::CEventGetValue<bool,1709081367>
	// 1503C serf some kind of stop?
	GetArmor = 0x1503E, //EGL::CEventGetValue<int, 1211121895>
	Barracks_GetTrainingTaskList = 0x1503F, //EGL::CEventGetValue<int, 1211121895>
	Barracks_GetTrainingTime = 0x15040, //EGL::CEventGetValue<float, 1468983543>
	// 0x15041 rax is training allowed?
	Leader_SetTrainingTL = 0x15042, //EGL::CEventValue<int,-27574121>
	Barracks_GetLeaveTaskList = 0x15043, //EGL::CEventGetValue<int, 1211121895>
	Leader_OnBarracksDetach = 0x15044, //EGL::CEvent1Entity
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
	AutoCannon_OnFoundationDetach = 0x1600B, //EGL::CEvent1Entity
	Foundation_OnAutoCannonDetach = 0x1600C, //EGL::CEvent1Entity
	HeroAbility_Cancel = 0x1600D, //BB::CEvent
	// 0x1600E foundation detach builder & top then die, neutralbridge detach architects
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
	ConvertBuilding_ActivateCommand = 0x16023, //EGL::CEvent1Entity
	ConvertBuilding_OnTargetDetach = 0x16024, //EGL::CEvent1Entity
	Settler_IsFearless = 0x16025, //EGL::CEventGetValue<bool,1709081367>
	ConvertSettler_ActivateCommand = 0x16027, //EGL::CEvent1Entity
	ConvertSettler_OnTargetDetach = 0x16028, //EGL::CEvent1Entity
	LimitedLifespan_GetTimeRemaining = 0x16029, //EGL::CEventGetValue<int,1211121895>
	LimitedLifespan_GetTimeMax = 0x1602A, //EGL::CEventGetValue<int,1211121895>
	HeroBehavior_GetSpawnTurn = 0x1602B, //EGL::CEventGetValue<int,1211121895>

	MotivateVorkers_ActivateCommand = 0x1602C, //BB::CEvent
	Sniper_SnipeCommand = 0x1602D, //EGL::CEvent1Entity
	Sniper_GetRange = 0x1602E, //EGL::CEventGetValue<float,1468983543>
	Shuriken_ActivateCommand = 0x1602F, //EGL::CEvent1Entity
	Shuriken_GetRange = 0x16030, //EGL::CEventGetValue<float,1468983543>

	InflictFear_Activate = 0x16026, //BB::CEvent

	WorkPlace_OnWorkerAttach = 0x17003, //EGL::CEvent1Entity
	WorkPlace_OnWorkerDetach = 0x17004, //EGL::CEvent1Entity
	// 0x17005 affectmoti affect moti, foundation init, defendablebuil activate defendmode construction complete
	// 0x17006 buildingbeh worward to 0x17004
	Market_WorkStep = 0x17007, //BB::CEvent
	University_ResearchStep = 0x17008, //BB::CEvent
	Market_GetProgress = 0x17009, //EGL::CEventGetValue<float,1468983543>
	AffectMotivation_GetEffect = 0x17010, //EGL::CEventGetValue<float,1468983543>
	Market_StartTrade = 0x1700A, //EGL::CEventGetValue<float,1468983543>
	Market_CancelTrade = 0x1700B, //BB::CEvent
	// 0x1700C university get tech progress?
	// 0x1700D constructionsite complete
	// 0x1700E settler 1 entity ? building 0x20003
	WorkerAlarmMode_OnBuildingDetach = 0x1700F, //EGL::CEvent1Entity
	// 0x17011 resourcerefiner get supplier?/can work?
	Worker_ResetTaskList = 0x17012, //BB::CEvent
	Foundry_GetProgress = 0x17014, //EGL::CEventGetValue<int,1211121895>
	Foundry_SetProgress = 0x17015, //EGL::CEventValue<int,-27574121>
	Foundry_BuildCannonCommand = 0x17016, //EGL::CEventValue<int,-27574121>
	Foundry_WorkStep = 0x17018, //BB::CEvent
	// 0x17019 affectmoti affect moti, foundation init
	// 0x1701A foundation cleanup?
	// 0x1701B mine detach res
	// 0x1701C foundation init, save load?
	IsConvertible = 0x1701D, //EGL::CEventGetValue<bool,1709081367>

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

	IsSerf = 0x18002, //EGL::CEventGetValue<bool, 1709081367>
	IsWorker = 0x18003, //EGL::CEventGetValue<bool, 1709081367>
	IsLeader = 0x18004, //EGL::CEventGetValue<bool, 1709081367>
	IsSoldier = 0x18005, //EGL::CEventGetValue<bool, 1709081367>
	IsBattleOrAutocannon = 0x18007, //EGL::CEventGetValue<bool, 1709081367>
	IsSettlerOrBuilding = 0x18008, //EGL::CEventGetValue<bool, 1709081367>
	// 18006 herobeh ret true
	IsSerfOrWorker = 0x18009, //EGL::CEventGetValue<bool, 1709081367>
	DefendableBuilding_CanAlarmModeBeActivated = 0x1800A, // EGL::CEventGetValue<bool, 1709081367>
	// 0x1800B building get bool?
	// 1800C resourcerefiner ret true
	// 1800D thefbeh ret true

	LimitedAttachment_Attach = 0x1A002, //GGL::CEventEntityAttachment
	LimitedAttachment_Dettach = 0x1A003, //GGL::CEventEntityAttachment
	// 1A004 limitedattach maybe get is full? maybe get is limited?, is unused
	LimitedAttachment_IsActive = 0x1A005, //GGL::CEventAttachmentTypeGetBool
	LimitedAttachment_SetLimit = 0x1A006, //GGL::CEventAttachmentTypeInteger
	LimitedAttachment_GetMax = 0x1A007, //GGL::CEventAttachmentTypeGetInteger
	LimitedAttachment_GetFree = 0x1A008, //GGL::CEventAttachmentTypeGetInteger
	LimitedAttachment_Activate = 0x1A009, //GGL::CEventAttachmentType
	LimitedAttachment_DeActivate = 0x1A00A, //GGL::CEventAttachmentType

	LogicEvent_DiplomacyChanged = 0x1C002,
	LogicEvent_TributePaid = 0x1C003,
	LogicEvent_ResearchDone = 0x1C004,
	LogicEvent_TradeCompleted = 0x1C005,
	LogicEvent_WeatherChanged = 0x1C006,
	LogicEvent_HurtEntity = 0x1C007,

	// 20001 move to entity?
	Movement_TaskMoveToPosRealtiveToEntity = 0x20002, //EGL::CEventPositionAndEntity
	Movement_TaskMoveToPos = 0x20003, //EGL::CEventPosition
	MultiSubAnim_SetSubAnim = 0x20004, //EGL::CEventSubAnim
	Behavior_Tick = 0x20005, //BB::CEvent ticks every second, also LogicEvent_Second
	LogicEvent_OnTick = 0x20006, // 10 per sec
	Movement_IsMoving = 0x20009, //EGL::CEventGetValue<bool,1709081367>
	Die = 0x2000A, ///BB::CEvent
	// 0x2000D tasklist reset?

	Animation_GetAnim = 0x20013, //EGL::CEventGetValue<int,1211121895>
	Animation_UnSuspend = 0x20014, //EGL::CEventValue<int,-27574121> argument is ticks spent suspended
	Animation_Suspend = 0x20015, //EGL::CEventValue<int,-27574121> argument is current tick
	LogicEvent_EntityCreated = 0x20016,
	LogicEvent_EntityDestroyed = 0x20017,
	LogicEvent_PlayerDied = 0x20018,
	LogicEvent_InRange = 0x20019,
	Animation_SetAnim = 0x2001D, //EGL::CEventAnimation
	Animation_ResetTaskType = 0x2001E, //BB::CEvent
	// worker 20024 get some int, leader get something barracks related, soldier simmilar, serf get terrainH < waterHeight
	ReplaceableEntity_Disable = 0x20025, //BB::CEvent
	// 20026 set uv anim?
	// 20027 BB::CEvent also suspend?
};

enum class shok_NetEventIds : int {
	CommandEntityAttackEntity = 69650,
	CommandSerfConstructBuilding = 69655,
	CommandSerfRepairBuilding = 69656,
	CommandEntityGuardEntity = 69664,
	CommandHeroConvertSettler = 69695,
	CommandThiefStealFrom = 69699,
	CommandThiefCarryStolenStuffToHQ = 69700,
	CommandThiefSabotageBuilding = 69701,
	CommandThiefDefuseKeg = 69702,
	CommandHeroSnipeSettler = 69705,
	CommandHeroThrowShurikenAt = 69708,
	CommandHeroPlaceCannonAt = 69679,
	CommandHeroPlaceBombAt = 69668,
	CommandEntityAttackPos = 69663,
	CommandHeroSendHawkToPos = 69676,
	CommandScoutUseBinocularsAt = 69704,
	CommandScoutPlaceTorchAtPos = 69706,
	CommandEntityMove = 69634,
	CommandEntityPatrol = 69669,
	CommandBuildingStartUpgrade = 69640,
	CommandLeaderBuySoldier = 69644,
	CommandSettlerExpell = 69647,
	CommandBuildingCancelResearch = 69659,
	CommandMarketCancelTrade = 69661,
	CommandBuildingCancelUpgrade = 69662,
	CommandLeaderHoldPosition = 69665,
	CommandLeaderDefend = 69667,
	CommandBattleSerfTurnToSerf = 69677,
	CommandSerfTurnToBattleSerf = 69678,
	CommandHeroActivateCamouflage = 69682,
	CommandHeroActivateSummon = 69685,
	CommandBuildingToggleOvertime = 69683,
	CommandHeroAffectEntities = 69689,
	CommandHeroCircularAttack = 69690,
	CommandHeroInflictFear = 69694,
	CommandBarracksRecruitGroups = 69696,
	CommandBarracksRecruitLeaderOnly = 69697,
	CommandHeroMotivateWorkers = 69703,
	CommandScoutFindResources = 69707,
	CommandPlaceBuilding = 69635,
	CommandHQBuySerf = 69636,
	CommandBuildingSell = 69638,
	CommandPlayerActivateAlarm = 69680,
	CommandPlayerDeactivateAlarm = 69681,
	PlayerUpgradeSettlerCategory = 69642,
	CommandPlayerSetTaxes = 69646,
	CommandWeathermachineChangeWeather = 69686,
	CommandMonasteryBlessSettlerGroup = 69687,
	CommandPlayerPayTribute = 69670,
	CommandBarracksBuyLeader = 69643,
	CommandLeaderSetFormation = 69653,
	CommandBuildingSetCurrentMaxWorkers = 69672,
	CommandFoundryBuildCannon = 69684,
	CommandSerfExtractResource = 69657,
	CommandBuildingStartResearch = 69658,
	CommandMarketStartTrade = 69660,
	SetGameSpeed = 0x50001,
};

enum class shok_FeedbackEventIds : int {
	FEEDBACK_EVENT_RESOURCE_MINED = 0x13001,
	FEEDBACK_EVENT_RESOURCE_RUNNING_OUT = 0x13002,
	FEEDBACK_EVENT_MINE_RESOURCE_GONE = 0x13003,
	FEEDBACK_EVENT_ENTITIES_EXCHANGED = 0x13004,
	FEEDBACK_EVENT_BATTLING = 0x13005, // GGL::CFeedbackEventBattling attackerplayer, attackerid, targetplayer, attackerx, attackery
	FEEDBACK_EVENT_SETTLER_WALK = 0x13006,
	FEEDBACK_EVENT_TECHNOLOGY_RESEARCHED = 0x13007,
	FEEDBACK_EVENT_SETTLER_GRIEVANCE = 0x13008,
	FEEDBACK_EVENT_SETTLER_LEFT_SETTLEMENT = 0x13009,
	FEEDBACK_EVENT_SETTLER_JOINED_SETTLEMENT = 0x1300A,
	FEEDBACK_EVENT_QUEST_CHANGED = 0x1300B,
	FEEDBACK_EVENT_MARKET_TRANSACTION_COMPLETE = 0x1300C,
	FEEDBACK_EVENT_NO_MONEY = 0x1300D,
	FEEDBACK_EVENT_PAYDAY = 0x1300E,
	FEEDBACK_EVENT_NOT_ENOUGH_RESOURCES = 0x13010,
	FEEDBACK_EVENT_LEADER_COMMAND = 0x13012,
	FEEDBACK_EVENT_HERO_ABILITY = 0x13013,
	FEEDBACK_EVENT_SEND_RESOURCES = 0x13014,
	FEEDBACK_EVENT_GOT_RESOURCES = 0x13015,
	FEEDBACK_EVENT_WEATHER_CHANGED = 0x13016,
	FEEDBACK_EVENT_POPULATION_LIMIT_REACHED = 0x13017,

	FEEDBACK_EVENT_SIMPLE = 0x23003,
	FEEDBACK_EVENT_ENTITY_HURT = 0x23004, // EGL::CNetEventEntityIDAndInteger entity, damage

	FEEDBACK_EVENT_FARM_LIMIT_REACHED = 0x25001,
	FEEDBACK_EVENT_RESIDENCE_LIMIT_REACHED = 0x25002,
	FEEDBACK_EVENT_HQ_2_POSSIBLE = 0x25003,
	FEEDBACK_EVENT_HQ_3_POSSIBLE = 0x25004,
	FEEDBACK_EVENT_BUILDING_UPGRADE_FINISHED = 0x25005,
	FEEDBACK_EVENT_LEADER_CHANGE_FORMATION = 0x25006,
	FEEDBACK_EVENT_WEATHER_FORECAST = 0x25007,
};

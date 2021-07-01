#include "pch.h"
#include "s5events.h"

shok_BB_CEvent::shok_BB_CEvent(int eventid)
{
	vtable = shok_BB_CEvent::vtp;
	EventTypeId = eventid;
}
shok_BB_CEvent::shok_BB_CEvent(shok_EventIDs eventid) : shok_BB_CEvent(static_cast<int>(eventid))
{
}
shok_BB_CEvent::shok_BB_CEvent(shok_NetEventIds eventid) : shok_BB_CEvent(static_cast<int>(eventid))
{
}

shok_EGL_CEventValue_int::shok_EGL_CEventValue_int(shok_EventIDs e, int d) : shok_BB_CEvent(e)
{
	vtable = shok_EGL_CEventValue_int::vtp;
	Data = d;
}

shok_EGL_CEventGetValue_bool::shok_EGL_CEventGetValue_bool(shok_EventIDs e) : shok_BB_CEvent(e)
{
	vtable = shok_EGL_CEventGetValue_bool::vtp;
}
shok_EGL_CEventGetValue_int::shok_EGL_CEventGetValue_int(shok_EventIDs e) : shok_BB_CEvent(e)
{
	vtable = shok_EGL_CEventGetValue_int::vtp;
}

shok_EGL_CEvent1Entity::shok_EGL_CEvent1Entity(shok_EventIDs e, int ent) : shok_BB_CEvent(e)
{
	vtable = shok_EGL_CEvent1Entity::vtp;
	EntityID = ent;
}

shok_EGL_CEventPosition::shok_EGL_CEventPosition(shok_EventIDs e, const shok_position& p) : shok_BB_CEvent(e)
{
	vtable = shok_EGL_CEventPosition::vtp;
	Position = p;
}

shok_GGL_CEventAttachmentTypeGetInteger::shok_GGL_CEventAttachmentTypeGetInteger(shok_EventIDs e, shok_AttachmentType t) : shok_BB_CEvent(e)
{
	vtable = shok_GGL_CEventAttachmentTypeGetInteger::vtp;
	AttachmentType = t;
}

shok_GGL_CEventEntityIndex::shok_GGL_CEventEntityIndex(shok_EventIDs e, int eid, int ind) : shok_BB_CEvent(e)
{
	vtable = shok_GGL_CEventEntityIndex::vtp;
	EntityId = eid;
	Index = ind;
}

shok_EGL_CEventPlayerID::shok_EGL_CEventPlayerID(shok_EventIDs e, int p) : shok_BB_CEvent(e)
{
	vtable = shok_EGL_CEventPlayerID::vtp;
	PlayerID = p;
}

shok_EGL_CEventSubAnim::shok_EGL_CEventSubAnim(shok_EventIDs e, int an, int subind, bool back, bool loop) : shok_BB_CEvent(e)
{
	vtable = shok_EGL_CEventSubAnim::vtp;
	AnimId = an;
	SubAnimIndex = subind;
	PlayBackwards = back;
	IsLooped = loop;
}

shok_EGL_CNetEvent2Entities::shok_EGL_CNetEvent2Entities(shok_NetEventIds id, int actor, int tar) : shok_BB_CEvent(id)
{
	vtable = shok_EGL_CNetEvent2Entities::vtp;
	ActorId = actor;
	TargetId = tar;
}

shok_EGL_CNetEventEntityAndPos::shok_EGL_CNetEventEntityAndPos(shok_NetEventIds id, int ent, const shok_position& pos) : shok_BB_CEvent(id)
{
	vtable = shok_EGL_CNetEventEntityAndPos::vtp;
	EntityId = ent;
	Position = pos;
}

shok_EGL_CNetEventEntityAndPosArray::shok_EGL_CNetEventEntityAndPosArray(shok_NetEventIds id, int ent, float r) : shok_BB_CEvent(id)
{
	vtable = shok_EGL_CNetEventEntityAndPosArray::vtp;
	EntityId = ent;
	Rotation = r;
}

shok_GGL_CNetEventExtractResource::shok_GGL_CNetEventExtractResource(shok_NetEventIds id, int ent, shok_ResourceType resty, const shok_position& pos) : shok_BB_CEvent(id)
{
	vtable = shok_GGL_CNetEventExtractResource::vtp;
	EntityId = ent;
	ResourceType = resty;
	Position = pos;
}

shok_GGL_CNetEventTransaction::shok_GGL_CNetEventTransaction(shok_NetEventIds id, int ent, shok_ResourceType sell, shok_ResourceType buy, int buyam) : shok_BB_CEvent(id)
{
	vtable = shok_GGL_CNetEventTransaction::vtp;
	EntityId = ent;
	SellType = sell;
	BuyType = buy;
	BuyAmount = buyam;
}

shok_EGL_CNetEventEntityID::shok_EGL_CNetEventEntityID(shok_NetEventIds id, int ent) : shok_BB_CEvent(id)
{
	vtable = shok_EGL_CNetEventEntityID::vtp;
	EntityId = ent;
}

shok_GGL_CNetEventCannonCreator::shok_GGL_CNetEventCannonCreator(shok_NetEventIds id, int ent, int bty, int tty, const shok_position& pos) : shok_EGL_CNetEventEntityID(id, ent)
{
	vtable = shok_GGL_CNetEventCannonCreator::vtp;
	BottomType = bty;
	TopType = tty;
	Position = pos;
}

shok_GGL_CNetEventEntityIDAndUpgradeCategory::shok_GGL_CNetEventEntityIDAndUpgradeCategory(shok_NetEventIds id, int ent, int ucat) : shok_EGL_CNetEventEntityID(id, ent)
{
	vtable = shok_GGL_CNetEventEntityIDAndUpgradeCategory::vtp;
	UpgradeCategory = ucat;
}

shok_EGL_CNetEventEntityIDAndInteger::shok_EGL_CNetEventEntityIDAndInteger(shok_NetEventIds id, int ent, int d) : shok_EGL_CNetEventEntityID(id, ent)
{
	vtable = shok_EGL_CNetEventEntityIDAndInteger::vtp;
	Data = d;
}

shok_GGL_CNetEventTechnologyAndEntityID::shok_GGL_CNetEventTechnologyAndEntityID(shok_NetEventIds id, int ent, int tech) : shok_EGL_CNetEventEntityID(id, ent)
{
	vtable = shok_GGL_CNetEventTechnologyAndEntityID::vtp;
	Technology = tech;
}

shok_EGL_CNetEventPlayerID::shok_EGL_CNetEventPlayerID(shok_NetEventIds id, int pl) : shok_BB_CEvent(id)
{
	vtable = shok_EGL_CNetEventPlayerID::vtp;
	PlayerId = pl;
}

shok_GGL_CNetEventBuildingCreator::shok_GGL_CNetEventBuildingCreator(shok_NetEventIds id, int pl, int ucat, const shok_positionRot& p) : shok_EGL_CNetEventPlayerID(id, pl)
{
	vtable = shok_GGL_CNetEventBuildingCreator::vtp;
	UpgradeCategory = ucat;
	Position = p;
}

shok_EGL_CNetEventIntegerAndPlayerID::shok_EGL_CNetEventIntegerAndPlayerID(shok_NetEventIds id, int pl, int d) : shok_EGL_CNetEventPlayerID(id, pl)
{
	vtable = shok_EGL_CNetEventIntegerAndPlayerID::vtp;
	Data = d;
}

shok_EGL_CNetEventPlayerIDAndInteger::shok_EGL_CNetEventPlayerIDAndInteger(shok_NetEventIds id, int pl, int d) : shok_EGL_CNetEventPlayerID(id, pl)
{
	vtable = shok_EGL_CNetEventPlayerIDAndInteger::vtp;
	Data = d;
}

shok_EGL_CNetEventEntityIDAndPlayerID::shok_EGL_CNetEventEntityIDAndPlayerID(shok_NetEventIds id, int pl, int e) : shok_EGL_CNetEventPlayerID(id, pl)
{
	vtable = shok_EGL_CNetEventEntityIDAndPlayerID::vtp;
	EntityId = e;
}

shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType::shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType(shok_NetEventIds id, int pl, int e, int ety) : shok_EGL_CNetEventEntityIDAndPlayerID(id, pl, e)
{
	vtable = shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType::vtp;
	EntityType = ety;
}

shok_GGL_CNetEventEntityIDPlayerIDAndInteger::shok_GGL_CNetEventEntityIDPlayerIDAndInteger(shok_NetEventIds id, int pl, int e, int d) : shok_EGL_CNetEventEntityIDAndPlayerID(id, pl, e)
{
	vtable = shok_GGL_CNetEventEntityIDPlayerIDAndInteger::vtp;
	Data = d;
}

shok_Framework_CEventGameSpeed::shok_Framework_CEventGameSpeed(shok_NetEventIds id, double speed) : shok_BB_CEvent(id)
{
	vtable = shok_Framework_CEventGameSpeed::vtp;
	SpeedFactor = speed;
}
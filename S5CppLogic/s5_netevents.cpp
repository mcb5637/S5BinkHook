#include "pch.h"
#include "s5_netevents.h"


EGL::CNetEvent2Entities::CNetEvent2Entities(shok::NetEventIds id, int actor, int tar) : BB::CEvent(id)
{
	SetVT(EGL::CNetEvent2Entities::vtp);
	ActorId = actor;
	TargetId = tar;
}

EGL::CNetEventEntityAndPos::CNetEventEntityAndPos(shok::NetEventIds id, int ent, const shok::Position& pos) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityAndPos::vtp);
	EntityId = ent;
	Position = pos;
}

EGL::CNetEventEntityAndPosArray::CNetEventEntityAndPosArray(shok::NetEventIds id, int ent, float r) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityAndPosArray::vtp);
	EntityId = ent;
	Rotation = r;
}

EGL::CNetEventEntityID::CNetEventEntityID(shok::NetEventIds id, int ent) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityID::vtp);
	EntityId = ent;
}
EGL::CNetEventEntityID::CNetEventEntityID(shok::FeedbackEventIds id, int ent) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityID::vtp);
	EntityId = ent;
}

EGL::CNetEventEntityIDAndInteger::CNetEventEntityIDAndInteger(shok::NetEventIds id, int ent, int d) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(EGL::CNetEventEntityIDAndInteger::vtp);
	Data = d;
}
EGL::CNetEventEntityIDAndInteger::CNetEventEntityIDAndInteger(shok::FeedbackEventIds id, int ent, int d) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(EGL::CNetEventEntityIDAndInteger::vtp);
	Data = d;
}

EGL::CNetEventPlayerID::CNetEventPlayerID(shok::NetEventIds id, int pl) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventPlayerID::vtp);
	PlayerId = pl;
}
EGL::CNetEventPlayerID::CNetEventPlayerID(shok::FeedbackEventIds id, int pl) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventPlayerID::vtp);
	PlayerId = pl;
}

EGL::CNetEventIntegerAndPlayerID::CNetEventIntegerAndPlayerID(shok::NetEventIds id, int pl, int d) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventIntegerAndPlayerID::vtp);
	Data = d;
}

EGL::CNetEventPlayerIDAndInteger::CNetEventPlayerIDAndInteger(shok::NetEventIds id, int pl, int d) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventPlayerIDAndInteger::vtp);
	Data = d;
}

EGL::CNetEventEntityIDAndPlayerID::CNetEventEntityIDAndPlayerID(shok::NetEventIds id, int pl, int e) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventEntityIDAndPlayerID::vtp);
	EntityId = e;
}
EGL::CNetEventEntityIDAndPlayerID::CNetEventEntityIDAndPlayerID(shok::FeedbackEventIds id, int pl, int e) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventEntityIDAndPlayerID::vtp);
	EntityId = e;
}

EGL::CNetEventEntityIDAndPlayerIDAndEntityType::CNetEventEntityIDAndPlayerIDAndEntityType(shok::NetEventIds id, int pl, int e, int ety) : EGL::CNetEventEntityIDAndPlayerID(id, pl, e)
{
	SetVT(EGL::CNetEventEntityIDAndPlayerIDAndEntityType::vtp);
	EntityType = ety;
}



GGL::CNetEventExtractResource::CNetEventExtractResource(shok::NetEventIds id, int ent, shok::ResourceType resty, const shok::Position& pos) : BB::CEvent(id)
{
	SetVT(GGL::CNetEventExtractResource::vtp);
	EntityId = ent;
	ResourceType = resty;
	Position = pos;
}

GGL::CNetEventTransaction::CNetEventTransaction(shok::NetEventIds id, int ent, shok::ResourceType sell, shok::ResourceType buy, int buyam) : BB::CEvent(id)
{
	SetVT(GGL::CNetEventTransaction::vtp);
	EntityId = ent;
	SellType = sell;
	BuyType = buy;
	BuyAmount = buyam;
}

GGL::CNetEventCannonCreator::CNetEventCannonCreator(shok::NetEventIds id, int ent, int bty, int tty, const shok::Position& pos) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(GGL::CNetEventCannonCreator::vtp);
	BottomType = bty;
	TopType = tty;
	Position = pos;
}

GGL::CNetEventEntityIDAndUpgradeCategory::CNetEventEntityIDAndUpgradeCategory(shok::NetEventIds id, int ent, int ucat) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(GGL::CNetEventEntityIDAndUpgradeCategory::vtp);
	UpgradeCategory = ucat;
}

GGL::CNetEventTechnologyAndEntityID::CNetEventTechnologyAndEntityID(shok::NetEventIds id, int ent, int tech) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(GGL::CNetEventTechnologyAndEntityID::vtp);
	Technology = tech;
}

GGL::CNetEventBuildingCreator::CNetEventBuildingCreator(shok::NetEventIds id, int pl, int ucat, const shok::PositionRot& p) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(GGL::CNetEventBuildingCreator::vtp);
	UpgradeCategory = ucat;
	Position = p;
}

GGL::CNetEventEntityIDPlayerIDAndInteger::CNetEventEntityIDPlayerIDAndInteger(shok::NetEventIds id, int pl, int e, int d) : EGL::CNetEventEntityIDAndPlayerID(id, pl, e)
{
	SetVT(GGL::CNetEventEntityIDPlayerIDAndInteger::vtp);
	Data = d;
}

GGL::CFeedbackEventBattling::CFeedbackEventBattling(shok::FeedbackEventIds id, int eid, int p, const shok::Position& pos, int tp) : EGL::CNetEventEntityIDAndPlayerID(id, p, eid)
{
	SetVT(GGL::CFeedbackEventBattling::vtp);
	Pos = pos;
	TargetPlayer = tp;
}

GGL::CFeedbackEventResource::CFeedbackEventResource(shok::FeedbackEventIds id, int player, int resentity, shok::ResourceType rty, int extrAm)
	: EGL::CNetEventEntityIDAndPlayerID(id, player, resentity)
{
	SetVT(GGL::CFeedbackEventResource::vtp);
	ResourceType = rty;
	ExtractionAmount = extrAm;
}


Framework::CEventGameSpeed::CEventGameSpeed(shok::NetEventIds id, double speed) : BB::CEvent(id)
{
	SetVT(Framework::CEventGameSpeed::vtp);
	SpeedFactor = speed;
}

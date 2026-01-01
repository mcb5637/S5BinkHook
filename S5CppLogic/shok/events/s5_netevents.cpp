#include "s5_netevents.h"


EGL::CNetEvent2Entities::CNetEvent2Entities(shok::NetEventIds id, shok::EntityId actor, shok::EntityId tar) : BB::CEvent(id)
{
	SetVT(EGL::CNetEvent2Entities::vtp);
	EntityID1 = actor;
	EntityID2 = tar;
}

EGL::CNetEventEntityAndPos::CNetEventEntityAndPos(shok::NetEventIds id, shok::EntityId ent, const shok::Position& pos) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityAndPos::vtp);
	EntityID = ent;
	X = pos.X;
	Y = pos.Y;
}

EGL::CNetEventEntityAndPosArray::CNetEventEntityAndPosArray(shok::NetEventIds id, shok::EntityId ent, float r) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityAndPosArray::vtp);
	EntityID = ent;
	Orientation = r;
}

EGL::CNetEventEntityID::CNetEventEntityID(shok::NetEventIds id, shok::EntityId ent) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityID::vtp);
	EntityID = ent;
}
EGL::CNetEventEntityID::CNetEventEntityID(shok::FeedbackEventIds id, shok::EntityId ent) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventEntityID::vtp);
	EntityID = ent;
}

EGL::CNetEventEntityIDAndInteger::CNetEventEntityIDAndInteger(shok::NetEventIds id, shok::EntityId ent, int d) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(EGL::CNetEventEntityIDAndInteger::vtp);
	Value = d;
}
EGL::CNetEventEntityIDAndInteger::CNetEventEntityIDAndInteger(shok::FeedbackEventIds id, shok::EntityId ent, int d) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(EGL::CNetEventEntityIDAndInteger::vtp);
	Value = d;
}

EGL::CNetEventPlayerID::CNetEventPlayerID(shok::NetEventIds id, shok::PlayerId pl) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventPlayerID::vtp);
	PlayerID = pl;
}
EGL::CNetEventPlayerID::CNetEventPlayerID(shok::FeedbackEventIds id, shok::PlayerId pl) : BB::CEvent(id)
{
	SetVT(EGL::CNetEventPlayerID::vtp);
	PlayerID = pl;
}

EGL::CNetEventIntegerAndPlayerID::CNetEventIntegerAndPlayerID(shok::NetEventIds id, shok::PlayerId pl, int d) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventIntegerAndPlayerID::vtp);
	Value = d;
}

EGL::CNetEventPlayerIDAndInteger::CNetEventPlayerIDAndInteger(shok::NetEventIds id, shok::PlayerId pl, int d) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventPlayerIDAndInteger::vtp);
	Value = d;
}

EGL::CNetEventEntityIDAndPlayerID::CNetEventEntityIDAndPlayerID(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventEntityIDAndPlayerID::vtp);
	EntityID = e;
}
EGL::CNetEventEntityIDAndPlayerID::CNetEventEntityIDAndPlayerID(shok::FeedbackEventIds id, shok::PlayerId pl, shok::EntityId e) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(EGL::CNetEventEntityIDAndPlayerID::vtp);
	EntityID = e;
}

EGL::CNetEventEntityIDAndPlayerIDAndEntityType::CNetEventEntityIDAndPlayerIDAndEntityType(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e, shok::EntityTypeId ety) : EGL::CNetEventEntityIDAndPlayerID(id, pl, e)
{
	SetVT(EGL::CNetEventEntityIDAndPlayerIDAndEntityType::vtp);
	EntityType = ety;
}

EGL::CNetEvent2PlayerIDs::CNetEvent2PlayerIDs(shok::NetEventIds id, shok::PlayerId p1, shok::PlayerId p2) : BB::CEvent(id)
{
	SetVT(vtp);
	PlayerID1 = p1;
	PlayerID2 = p2;
}

EGL::CNetEvent2PlayerIDsAndInteger::CNetEvent2PlayerIDsAndInteger(shok::NetEventIds id, shok::PlayerId p1, shok::PlayerId p2, int v) : CNetEvent2PlayerIDs(id, p1, p2)
{
	SetVT(vtp);
	Value = v;
}



GGL::CNetEventExtractResource::CNetEventExtractResource(shok::NetEventIds id, shok::EntityId ent, shok::ResourceType resty, const shok::Position& pos) : BB::CEvent(id)
{
	SetVT(GGL::CNetEventExtractResource::vtp);
	SerfID = ent;
	ResourceType = resty;
	Position = pos;
}

GGL::CNetEventTransaction::CNetEventTransaction(shok::NetEventIds id, shok::EntityId ent, shok::ResourceType sell, shok::ResourceType buy, int buyam) : BB::CEvent(id)
{
	SetVT(GGL::CNetEventTransaction::vtp);
	MarketID = ent;
	SellResource = sell;
	BuyResource = buy;
	BuyAmount = buyam;
}

GGL::CNetEventCannonCreator::CNetEventCannonCreator(shok::NetEventIds id, shok::EntityId ent, shok::EntityTypeId bty, shok::EntityTypeId tty, const shok::PositionRot& pos) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(GGL::CNetEventCannonCreator::vtp);
	FoundationType = bty;
	CannonType = tty;
	Position = { pos.X, pos.Y };
	Orientation = pos.r;
}

GGL::CNetEventEntityIDAndUpgradeCategory::CNetEventEntityIDAndUpgradeCategory(shok::NetEventIds id, shok::EntityId ent, shok::UpgradeCategoryId ucat) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(GGL::CNetEventEntityIDAndUpgradeCategory::vtp);
	UpgradeCategory = ucat;
}

GGL::CNetEventTechnologyAndEntityID::CNetEventTechnologyAndEntityID(shok::NetEventIds id, shok::EntityId ent, shok::TechnologyId tech) : EGL::CNetEventEntityID(id, ent)
{
	SetVT(GGL::CNetEventTechnologyAndEntityID::vtp);
	TechnologyType = tech;
}

GGL::CNetEventBuildingCreator::CNetEventBuildingCreator(shok::NetEventIds id, shok::PlayerId pl, shok::UpgradeCategoryId ety, const shok::PositionRot& p) : EGL::CNetEventPlayerID(id, pl)
{
	SetVT(GGL::CNetEventBuildingCreator::vtp);
	EntityType = ety;
	Position = { p.X, p.Y };
	Orientation = p.r;
}

GGL::CNetEventEntityIDPlayerIDAndInteger::CNetEventEntityIDPlayerIDAndInteger(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e, int d) : EGL::CNetEventEntityIDAndPlayerID(id, pl, e)
{
	SetVT(GGL::CNetEventEntityIDPlayerIDAndInteger::vtp);
	Value = d;
}

GGL::CFeedbackEventBattling::CFeedbackEventBattling(shok::FeedbackEventIds id, shok::EntityId eid, shok::PlayerId p, const shok::Position& pos, shok::PlayerId tp) : EGL::CNetEventEntityIDAndPlayerID(id, p, eid)
{
	SetVT(GGL::CFeedbackEventBattling::vtp);
	Pos = pos;
	TargetPlayer = tp;
}

GGL::CFeedbackEventResource::CFeedbackEventResource(shok::FeedbackEventIds id, shok::PlayerId player, shok::EntityId resentity, shok::ResourceType rty, float extrAm)
	: EGL::CNetEventEntityIDAndPlayerID(id, player, resentity)
{
	SetVT(GGL::CFeedbackEventResource::vtp);
	ResourceType = rty;
	ExtractionAmount = extrAm;
}

GGL::CNetEventEventTechnologyPlayerIDAndEntityID::CNetEventEventTechnologyPlayerIDAndEntityID(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e, shok::TechnologyId t)
	: EGL::CNetEventEntityIDAndPlayerID(id, pl, e)
{
	SetVT(GGL::CNetEventEventTechnologyPlayerIDAndEntityID::vtp);
	TechId = t;
}

GGL::CNetEventPlayerResourceDonation::CNetEventPlayerResourceDonation(shok::NetEventIds id, shok::PlayerId p1, shok::PlayerId p2, shok::ResourceType rt, int am)
	: EGL::CNetEvent2PlayerIDs(id, p1, p2)
{
	SetVT(GGL::CNetEventPlayerResourceDonation::vtp);
	ResourceType = rt;
	ResourceAmount = am;
}


Framework::CEventGameSpeed::CEventGameSpeed(shok::NetEventIds id, double speed) : BB::CEvent(id)
{
	SetVT(Framework::CEventGameSpeed::vtp);
	SpeedFactor = speed;
}

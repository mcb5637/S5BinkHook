#include "pch.h"
#include "s5_events.h"
#include "s5_defines.h"

BB::CEvent::CEvent(int eventid)
{
	SetVT(BB::CEvent::vtp);
	EventTypeId = eventid;
}
BB::CEvent::CEvent(shok::EventIDs eventid) : BB::CEvent(static_cast<int>(eventid))
{
}
BB::CEvent::CEvent(shok::NetEventIds eventid) : BB::CEvent(static_cast<int>(eventid))
{
}
BB::CEvent::CEvent(shok::FeedbackEventIds eventid) : BB::CEvent(static_cast<int>(eventid))
{
}
BB::CEvent::CEvent(shok::InputEventIds eventid) : BB::CEvent(static_cast<int>(eventid))
{
}

bool BB::CEvent::IsEvent(shok::EventIDs id)
{
	return EventTypeId == static_cast<int>(id);
}
bool BB::CEvent::IsEvent(shok::NetEventIds id)
{
	return EventTypeId == static_cast<int>(id);
}
bool BB::CEvent::IsEvent(shok::FeedbackEventIds id)
{
	return EventTypeId == static_cast<int>(id);
}
bool BB::CEvent::IsEvent(shok::InputEventIds id)
{
	return EventTypeId == static_cast<int>(id);
}

void BB::CEvent::SetVT(int vt)
{
	*reinterpret_cast<int*>(this) = vt;
}
unsigned int __stdcall BB::CEvent::GetClassIdentifier() const
{
	return 0;
}
void* __stdcall BB::CEvent::CastToIdentifier(unsigned int id)
{
	return nullptr;
}

int EGL::CEvent1Entity::GetEntityID() const
{
	return EntityID;
}
void* __stdcall EGL::CEvent1Entity::CastToIdentifier(unsigned int id)
{
	if (id == IEventEntityID::Identifier)
		return static_cast<IEventEntityID*>(this);
	return nullptr;
}

EGL::CEvent1Entity::CEvent1Entity(shok::EventIDs e, int ent) : BB::CEvent(e)
{
	SetVT(EGL::CEvent1Entity::vtp);
	EntityID = ent;
	*reinterpret_cast<int*>(static_cast<EGL::IEventEntityID*>(this)) = EGL::CEvent1Entity::vtp_IEventEntityID;
}

EGL::CEventPosition::CEventPosition(shok::EventIDs e, const shok::Position& p) : BB::CEvent(e)
{
	SetVT(EGL::CEventPosition::vtp);
	Position = p;
}

EGL::CEventPositionAndTaskState::CEventPositionAndTaskState(shok::EventIDs e, const shok::Position& p, shok::TaskState s)
	: CEventPosition(e, p)
{
	SetVT(EGL::CEventPositionAndTaskState::vtp);
	State = s;
}

int EGL::CEventPlayerID::GetPlayerID() const {
	return PlayerID;
}
EGL::CEventPlayerID::CEventPlayerID(shok::EventIDs e, int p) : BB::CEvent(e)
{
	SetVT(EGL::CEventPlayerID::vtp);
	PlayerID = p;
	*reinterpret_cast<int*>(static_cast<EGL::IEventPlayerID*>(this)) = EGL::CEventPlayerID::vtp_IEventPlayerID;
}

EGL::CEventSubAnim::CEventSubAnim(shok::EventIDs e, int an, int subind, bool back, bool loop) : BB::CEvent(e)
{
	SetVT(EGL::CEventSubAnim::vtp);
	AnimId = an;
	SubAnimIndex = subind;
	PlayBackwards = back;
	IsLooped = loop;
}

EGL::CEvent2Entities::CEvent2Entities(shok::EventIDs e, int aid, int tid) : BB::CEvent(e)
{
	SetVT(EGL::CEvent2Entities::vtp);
	ActorId = aid;
	TargetId = tid;
	*reinterpret_cast<int*>(static_cast<EGL::IEvent2Entities*>(this)) = EGL::CEvent2Entities::vtp_IEvent2Entities;
}
int EGL::CEvent2Entities::GetActorID() const
{
	return ActorId;
}
int EGL::CEvent2Entities::GetTargetID() const
{
	return TargetId;
}
void* __stdcall EGL::CEvent2Entities::CastToIdentifier(unsigned int id)
{
	if (id == IEvent2Entities::Identifier)
		return static_cast<IEvent2Entities*>(this);
	return nullptr;
}

EGL::CEventThousandthsGetInteger::CEventThousandthsGetInteger(shok::EventIDs e, int thousands)
	: BB::CEvent(e)
{
	SetVT(EGL::CEventThousandthsGetInteger::vtp);
	Thousands = thousands;
}

EGL::CEventSoundPositionAndID::CEventSoundPositionAndID(shok::EventIDs e, int soundid, bool haspos, const shok::Position& p, float z, int vol, bool looped)
	: BB::CEvent(e)
{
	SetVT(vtp);
	SoundId = soundid;
	HasPosition = haspos;
	X = p.X;
	Y = p.Y;
	Z = z;
	Volume = vol;
	Looped = looped;
}

GGL::CEventAttachmentTypeGetInteger::CEventAttachmentTypeGetInteger(shok::EventIDs e, shok::AttachmentType t) : BB::CEvent(e)
{
	SetVT(GGL::CEventAttachmentTypeGetInteger::vtp);
	AttachmentType = t;
}
GGL::CEventAttachmentTypeInteger::CEventAttachmentTypeInteger(shok::EventIDs e, shok::AttachmentType t, int d) : BB::CEvent(e)
{
	SetVT(GGL::CEventAttachmentTypeInteger::vtp);
	AttachmentType = t;
	Data = d;
}

GGL::CEventEntityIndex::CEventEntityIndex(shok::EventIDs e, int eid, int ind) : BB::CEvent(e)
{
	SetVT(GGL::CEventEntityIndex::vtp);
	EntityId = eid;
	Index = ind;
}

GGL::CEventTransaction::CEventTransaction(shok::EventIDs e, shok::ResourceType sell, shok::ResourceType buy, float buyAm) : BB::CEvent(e)
{
	SetVT(GGL::CEventTransaction::vtp);
	SellType = sell;
	BuyType = buy;
	BuyAmount = buyAm;
}

GGL::CEventGoodsTraded::CEventGoodsTraded(shok::EventIDs e, shok::ResourceType sell, shok::ResourceType buy, float buyAm, int en, float sellam)
	: CEventTransaction(e, sell, buy, buyAm)
{
	SetVT(vtp);
	*reinterpret_cast<int*>(static_cast<EGL::IEventEntityID*>(this)) = vtp_IEventEntityID;
	Entity = en;
	SellAmount = sellam;
}
int GGL::CEventGoodsTraded::GetEntityID() const
{
	return Entity;
}

GGL::CEventPositionAnd2EntityTypes::CEventPositionAnd2EntityTypes(shok::EventIDs e, const shok::Position& p, int t1, int t2) : EGL::CEventPosition(e, p)
{
	SetVT(GGL::CEventPositionAnd2EntityTypes::vtp);
	Type1 = t1;
	Type2 = t2;
}

GGL::CEventEntityAttachment::CEventEntityAttachment(shok::EventIDs e, shok::AttachmentType ty, int eid, shok::EventIDs detach) : BB::CEvent(e)
{
	SetVT(GGL::CEventEntityAttachment::vtp);
	Type = ty;
	EntityId = eid;
	DetachEvent = detach;
}

BB::CInputEvent::CInputEvent(shok::InputEventIds id) : BB::CEvent(id)
{
	SetVT(BB::CInputEvent::vtp);
	EventHandeled = false;
}

BB::CMouseEvent::CMouseEvent(shok::InputEventIds id, shok::Keys keydata, int clicks, int x, int y, int delta) : BB::CInputEvent(id)
{
	SetVT(BB::CMouseEvent::vtp);
	KeyData = keydata;
	Clicks = clicks;
	X = x;
	Y = y;
	Delta = delta;
}
bool BB::CMouseEvent::IsKey(shok::Keys key)
{
	return (KeyData & shok::Keys::MaskCode) == key;
}
bool BB::CMouseEvent::IsModifier(shok::Keys mod)
{
	return (KeyData & shok::Keys::MaskModifiers) == mod;
}

BB::CKeyEvent::CKeyEvent(shok::InputEventIds id, shok::Keys keydata) : BB::CInputEvent(id)
{
	SetVT(BB::CKeyEvent::vtp);
	KeyData = keydata;
}

BB::CKeyPressEvent::CKeyPressEvent(shok::InputEventIds id, int keychar, shok::Keys keymodif) : BB::CInputEvent(id)
{
	SetVT(BB::CKeyPressEvent::vtp);
	KeyChar = keychar;
	KeyModifier = keymodif;
}

CppLogic::Events::AdvHurtEvent::AdvHurtEvent(shok::EventIDs e, int aid, int tid, int dmg, shok::AdvancedDealDamageSource sou, int attpl)
	: EGL::CEvent2Entities(e, aid, tid)
{
	Damage = dmg;
	Source = sou;
	AttackerPlayer = attpl;
}
unsigned int __stdcall CppLogic::Events::AdvHurtEvent::GetClassIdentifier() const
{
	return Identifier;
}

CppLogic::Events::ResourceEvent::ResourceEvent(shok::EventIDs e, int id, shok::ResourceType rt, int am)
	: EGL::CEvent1Entity(e, id)
{
	Type = rt;
	ResourceAmount = am;
}
unsigned int __stdcall CppLogic::Events::ResourceEvent::GetClassIdentifier() const
{
	return Identifier;
}

CppLogic::Events::ConversionEvent::ConversionEvent(shok::EventIDs e, int id, int told, int tnew)
	: BB::CEvent(e)
{
	ConverterId = id;
	TargetIDOld = told;
	TargetIDNew = tnew;
}
unsigned int __stdcall CppLogic::Events::ConversionEvent::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall CppLogic::Events::ConversionEvent::CastToIdentifier(unsigned int id)
{
	if (id == EGL::IEventEntityID::Identifier)
		return static_cast<EGL::IEventEntityID*>(this);
	if (id == EGL::IEvent2Entities::Identifier)
		return static_cast<EGL::IEvent2Entities*>(this);
	return nullptr;
}
int CppLogic::Events::ConversionEvent::GetEntityID() const
{
	return ConverterId;
}
int CppLogic::Events::ConversionEvent::GetActorID() const
{
	return TargetIDOld;
}
int CppLogic::Events::ConversionEvent::GetTargetID() const
{
	return TargetIDNew;
}

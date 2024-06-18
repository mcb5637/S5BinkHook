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
shok::ClassId __stdcall BB::CEvent::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall BB::CEvent::CastToIdentifier(shok::ClassId id)
{
	return nullptr;
}

EGL::CEventEntityGetBool::CEventEntityGetBool(shok::EventIDs e, shok::EntityId eid)
	: CEventGetValue_Bool(e)
{
	SetVT(vtp);
	Entity = eid;
}

shok::EntityId EGL::CEvent1Entity::GetEntityID() const
{
	return EntityID;
}
void* __stdcall EGL::CEvent1Entity::CastToIdentifier(shok::ClassId id)
{
	if (id == IEventEntityID::Identifier)
		return static_cast<IEventEntityID*>(this);
	return nullptr;
}

EGL::CEvent1Entity::CEvent1Entity(shok::EventIDs e, shok::EntityId ent) : BB::CEvent(e)
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

shok::PlayerId EGL::CEventPlayerID::GetPlayerID() const {
	return PlayerID;
}
EGL::CEventPlayerID::CEventPlayerID(shok::EventIDs e, shok::PlayerId p) : BB::CEvent(e)
{
	SetVT(EGL::CEventPlayerID::vtp);
	PlayerID = p;
	*reinterpret_cast<int*>(static_cast<EGL::IEventPlayerID*>(this)) = EGL::CEventPlayerID::vtp_IEventPlayerID;
}

EGL::CEventSubAnim::CEventSubAnim(shok::EventIDs e, shok::AnimationId an, int subind, bool back, bool loop) : BB::CEvent(e)
{
	SetVT(EGL::CEventSubAnim::vtp);
	AnimId = an;
	SubAnimIndex = subind;
	PlayBackwards = back;
	IsLooped = loop;
}

EGL::CEvent2Entities::CEvent2Entities(shok::EventIDs e, shok::EntityId aid, shok::EntityId tid) : BB::CEvent(e)
{
	SetVT(EGL::CEvent2Entities::vtp);
	ActorId = aid;
	TargetId = tid;
	*reinterpret_cast<int*>(static_cast<EGL::IEvent2Entities*>(this)) = EGL::CEvent2Entities::vtp_IEvent2Entities;
}
shok::EntityId EGL::CEvent2Entities::GetActorID() const
{
	return ActorId;
}
shok::EntityId EGL::CEvent2Entities::GetTargetID() const
{
	return TargetId;
}
void* __stdcall EGL::CEvent2Entities::CastToIdentifier(shok::ClassId id)
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

EGL::CEventSoundPositionAndID::CEventSoundPositionAndID(shok::EventIDs e, shok::SoundId soundid, bool haspos, const shok::Position& p, float z, int vol, bool looped)
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

EGL::CEventGetPosition::CEventGetPosition(shok::EventIDs e)
	: BB::CEvent(e)
{
	SetVT(vtp);
}

EGL::CEventIndexAndEffectType::CEventIndexAndEffectType(shok::EventIDs id, int index, shok::EffectTypeId ety)
	: BB::CEvent(id), Index(index), EffectType(ety)
{
	SetVT(vtp);
}

EGL::CEventAnimation::CEventAnimation(shok::EventIDs id, shok::AnimationId anim, bool backw, shok::AnimationCategoryId acat) : 
	BB::CEvent(id), AnimID(anim), PlayBackwards(backw), Category(acat)
{
	SetVT(vtp);
}

EGL::CEventPositionAndEntity::CEventPositionAndEntity(shok::EventIDs id, const shok::Position& p, shok::EntityId e)
	: BB::CEvent(id), Pos(p), Entity(e)
{
	SetVT(vtp);
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

GGL::CEventAttachmentTypeGetBool::CEventAttachmentTypeGetBool(shok::EventIDs e, shok::AttachmentType t) : BB::CEvent(e)
{
	SetVT(vtp);
	AttachmentType = t;
}

GGL::CEventEntityIndex::CEventEntityIndex(shok::EventIDs e, shok::EntityId eid, int ind) : BB::CEvent(e)
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

GGL::CEventGoodsTraded::CEventGoodsTraded(shok::EventIDs e, shok::ResourceType sell, shok::ResourceType buy, float buyAm, shok::EntityId en, float sellam)
	: CEventTransaction(e, sell, buy, buyAm)
{
	SetVT(vtp);
	*reinterpret_cast<int*>(static_cast<EGL::IEventEntityID*>(this)) = vtp_IEventEntityID;
	Entity = en;
	SellAmount = sellam;
}
shok::EntityId GGL::CEventGoodsTraded::GetEntityID() const
{
	return Entity;
}

GGL::CEventPositionAnd2EntityTypes::CEventPositionAnd2EntityTypes(shok::EventIDs e, const shok::Position& p, shok::EntityTypeId t1, shok::EntityTypeId t2) : EGL::CEventPosition(e, p)
{
	SetVT(GGL::CEventPositionAnd2EntityTypes::vtp);
	Type1 = t1;
	Type2 = t2;
}

GGL::CEventEntityAttachment::CEventEntityAttachment(shok::EventIDs e, shok::AttachmentType ty, shok::EntityId eid, shok::EventIDs detach) : BB::CEvent(e)
{
	SetVT(GGL::CEventEntityAttachment::vtp);
	Type = ty;
	EntityId = eid;
	DetachEvent = detach;
}

GGL::CEventChangeMotivation::CEventChangeMotivation(shok::EventIDs e, float d, shok::WorkerReason r)
	: EGL::CEventValue_Float(e, d)
{
	SetVT(GGL::CEventChangeMotivation::vtp);
	Reason = r;
}

GGL::CEventHeroAbilityInteger::CEventHeroAbilityInteger(shok::EventIDs id, int data, shok::AbilityId ab) : EGL::CEventValue_Int(id, data), Ability(ab)
{
	SetVT(vtp);
}

GGL::CEventHeroAbilityGetInteger::CEventHeroAbilityGetInteger(shok::EventIDs id, shok::AbilityId ab) : EGL::CEventGetValue_Int(id), Ability(ab)
{
	SetVT(vtp);
}

GGL::CEventPlayerIDInteger::CEventPlayerIDInteger(shok::EventIDs id, shok::PlayerId pl, int d)
	: EGL::CEventPlayerID(id, pl), Data(d)
{
	SetVT(vtp);
}

GGL::CEventSourceTargetPlayerID::CEventSourceTargetPlayerID(shok::EventIDs id, shok::PlayerId s, shok::PlayerId t)
	: BB::CEvent(id), Source(s), Target(t)
{
	SetVT(vtp);
	*reinterpret_cast<int*>(static_cast<EGL::IEventSourcePlayerID*>(this)) = 0x76F8B4;
	*reinterpret_cast<int*>(static_cast<EGL::IEventTargetPlayerID*>(this)) = 0x76F8AC;
}
shok::PlayerId GGL::CEventSourceTargetPlayerID::GetSourcePlayer() const
{
	return Source;
}
shok::PlayerId GGL::CEventSourceTargetPlayerID::GetTargetPlayer() const
{
	return Target;
}

GGL::CEventDiplomacyChanged::CEventDiplomacyChanged(shok::EventIDs id, shok::PlayerId s, shok::PlayerId t, shok::DiploState n)
	: CEventSourceTargetPlayerID(id, s, t), NewState(n)
{
	SetVT(vtp);
	*reinterpret_cast<int*>(static_cast<EGL::IEventSourcePlayerID*>(this)) = 0x76F8DC;
	*reinterpret_cast<int*>(static_cast<EGL::IEventTargetPlayerID*>(this)) = 0x76F8D4;
	*reinterpret_cast<int*>(static_cast<IEventDiplomacyState*>(this)) = 0x76F8CC;
}
shok::DiploState GGL::CEventDiplomacyChanged::GetState() const
{
	return NewState;
}

GGL::CEventWeatherStateChanged::CEventWeatherStateChanged(shok::EventIDs id, shok::WeatherState o, shok::WeatherState n)
	: BB::CEvent(id), Old(o), New(n)
{
	SetVT(vtp);
}

GGL::CEventBridgeProgress::CEventBridgeProgress(shok::EventIDs id, shok::PlayerId pl, float p)
	: EGL::CEventPlayerID(id, pl), Progress(p)
{
	SetVT(vtp);
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
bool BB::CKeyEvent::IsKey(shok::Keys key)
{
	return (KeyData & shok::Keys::MaskCode) == key;
}
bool BB::CKeyEvent::IsModifier(shok::Keys mod)
{
	return (KeyData & shok::Keys::MaskModifiers) == mod;
}

BB::CKeyPressEvent::CKeyPressEvent(shok::InputEventIds id, int keychar, shok::Keys keymodif) : BB::CInputEvent(id)
{
	SetVT(BB::CKeyPressEvent::vtp);
	KeyChar = keychar;
	KeyModifier = keymodif;
}

CppLogic::Events::AdvHurtEvent::AdvHurtEvent(shok::EventIDs e, shok::EntityId aid, shok::EntityId tid, int dmg, shok::AdvancedDealDamageSource sou, shok::PlayerId attpl)
	: EGL::CEvent2Entities(e, aid, tid)
{
	Damage = dmg;
	Source = sou;
	AttackerPlayer = attpl;
}
shok::ClassId __stdcall CppLogic::Events::AdvHurtEvent::GetClassIdentifier() const
{
	return Identifier;
}

CppLogic::Events::ResourceEvent::ResourceEvent(shok::EventIDs e, shok::EntityId id, shok::ResourceType rt, int am)
	: EGL::CEvent1Entity(e, id)
{
	Type = rt;
	ResourceAmount = am;
}
shok::ClassId __stdcall CppLogic::Events::ResourceEvent::GetClassIdentifier() const
{
	return Identifier;
}

CppLogic::Events::ConversionEvent::ConversionEvent(shok::EventIDs e, shok::EntityId id, shok::EntityId told, shok::EntityId tnew)
	: BB::CEvent(e)
{
	ConverterId = id;
	TargetIDOld = told;
	TargetIDNew = tnew;
}
shok::ClassId __stdcall CppLogic::Events::ConversionEvent::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall CppLogic::Events::ConversionEvent::CastToIdentifier(shok::ClassId id)
{
	if (id == EGL::IEventEntityID::Identifier)
		return static_cast<EGL::IEventEntityID*>(this);
	if (id == EGL::IEvent2Entities::Identifier)
		return static_cast<EGL::IEvent2Entities*>(this);
	return nullptr;
}
shok::EntityId CppLogic::Events::ConversionEvent::GetEntityID() const
{
	return ConverterId;
}
shok::EntityId CppLogic::Events::ConversionEvent::GetActorID() const
{
	return TargetIDOld;
}
shok::EntityId CppLogic::Events::ConversionEvent::GetTargetID() const
{
	return TargetIDNew;
}

CppLogic::Events::AdvHurtByEvent::AdvHurtByEvent(shok::EventIDs e, shok::EntityId aid, int dmg, shok::AdvancedDealDamageSource sou, shok::PlayerId attpl)
	: EGL::CEvent1Entity(e, aid)
{
	Damage = dmg;
	Source = sou;
	AttackerPlayer = attpl;
}
shok::ClassId __stdcall CppLogic::Events::AdvHurtByEvent::GetClassIdentifier() const
{
	return Identifier;
}

CppLogic::Events::CanBuySettlerEvent::CanBuySettlerEvent(shok::EventIDs e, shok::EntityId buyat, shok::EntityId target, shok::EntityTypeId tobuy, bool vc, bool cost, bool moti, bool alarm, bool hqpop)
	: EGL::CEvent1Entity(e, buyat), ToBuy(tobuy), Target(target), VCPop(vc), Cost(cost), Motivation(moti), Alarm(alarm), HQPop(hqpop)
{
}
shok::ClassId __stdcall CppLogic::Events::CanBuySettlerEvent::GetClassIdentifier() const
{
	return Identifier;
}

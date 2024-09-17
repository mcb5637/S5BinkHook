#include "pch.h"
#include "ModBehavior.h"
#include "s5_events.h"
#include "s5_classfactory.h"
#include "s5_effects.h"
#include "s5_tasklist.h"

void CppLogic::Mod::RegisterClasses()
{
    BB::CClassFactory* f = *BB::CClassFactory::GlobalObj;
    f->AddClassToFactory<FormationSpacedBehaviorProps>();
    f->AddClassToFactory<FormationSpacedBehavior>();
    f->AddClassToFactory<ResourceTrackerBehavior>();
	f->AddClassToFactory<HawkCircleBehavior>();
	f->AddClassToFactory<HawkOwnerAbility>();
    f->AddClassToFactory<LightningStrikeAbilityProps>();
    f->AddClassToFactory<LightningStrikeAbility>();
}

shok::ClassId __stdcall CppLogic::Mod::FormationSpacedBehaviorProps::GetClassIdentifier() const
{
    return Identifier;
}

void* CppLogic::Mod::FormationSpacedBehaviorProps::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::FormationSpacedBehaviorProps::operator delete(void* p) {
    shok::Free(p);
}

const BB::SerializationData CppLogic::Mod::FormationSpacedBehaviorProps::SerializationData[]{
    BB::SerializationData::AutoBaseClass<FormationSpacedBehaviorProps, GGL::CFormationBehaviorProperties>(),
    AutoMemberSerialization(FormationSpacedBehaviorProps, SpaceFactor),
    BB::SerializationData::GuardData(),
};

shok::ClassId __stdcall CppLogic::Mod::FormationSpacedBehavior::GetClassIdentifier() const
{
    return Identifier;
}

void* __stdcall CppLogic::Mod::FormationSpacedBehavior::CastToIdentifier(shok::ClassId id)
{
    if (id == IFormationBehaviorExtProvider::Identifier)
        return static_cast<IFormationBehaviorExtProvider*>(this);
    return nullptr;
}

shok::Position CppLogic::Mod::FormationSpacedBehavior::GetPosExt(EGL::CGLEEntity* leader)
{
    shok::Position n;
    GetFormationPosition(leader, &n);
    EGL::CEventGetPosition ev{ shok::EventIDs::Movement_GetNextFineWaypoint };
    leader->FireEvent(&ev);
    shok::Position diff = n - ev.Position;
    diff *= static_cast<FormationSpacedBehaviorProps*>(PropPointer)->SpaceFactor;
    return ev.Position + diff;
}

CppLogic::Mod::FormationSpacedBehavior::FormationSpacedBehavior()
{
    HookGetPosExtI();
}

const BB::SerializationData CppLogic::Mod::FormationSpacedBehavior::SerializationData[] {
    BB::SerializationData::AutoBaseClass<FormationSpacedBehavior, GGL::CFormationBehavior>(),
    BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::FormationSpacedBehavior::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::FormationSpacedBehavior::operator delete(void* p) {
    shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::ResourceTrackerBehavior::GetClassIdentifier() const
{
    return Identifier;
}

const BB::SerializationData CppLogic::Mod::ResourceTrackerBehavior::SerializationData[]{
    BB::SerializationData::AutoBaseClass<ResourceTrackerBehavior, EGL::CGLEBehavior>(),
    AutoMemberSerialization(ResourceTrackerBehavior, Produced),
    AutoMemberSerialization(ResourceTrackerBehavior, Used),
    BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::ResourceTrackerBehavior::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::ResourceTrackerBehavior::operator delete(void* p) {
    shok::Free(p);
}

void __thiscall CppLogic::Mod::ResourceTrackerBehavior::AddHandlers(shok::EntityId id)
{
    EntityId = id;
    auto* e = EGL::CGLEEntity::GetEntityByID(id);
    e->CreateEventHandler<shok::EventIDs::CppLogicEvent_OnResourceMined>(this, &ResourceTrackerBehavior::EventMinedOrRefined);
    e->CreateEventHandler<shok::EventIDs::CppLogicEvent_OnResourceRefined>(this, &ResourceTrackerBehavior::EventMinedOrRefined);
    e->CreateEventHandler<shok::EventIDs::CppLogicEvent_OnRefinerSupplyTaken>(this, &ResourceTrackerBehavior::EventSupplied);
}
void __thiscall CppLogic::Mod::ResourceTrackerBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
    PropPointer = p;
}
void __thiscall CppLogic::Mod::ResourceTrackerBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
    PropPointer = p;
}
void __thiscall CppLogic::Mod::ResourceTrackerBehavior::OnEntityUpgrade(EGL::CGLEEntity* old)
{
    auto* o = old->GetBehavior<ResourceTrackerBehavior>();
    if (o) {
        Produced = o->Produced;
        Used = o->Used;
    }
}

void CppLogic::Mod::ResourceTrackerBehavior::EventMinedOrRefined(GGL::CEventGoodsTraded* ev)
{
    Produced.AddToType(ev->BuyType, ev->BuyAmount);
}
void CppLogic::Mod::ResourceTrackerBehavior::EventSupplied(GGL::CEventGoodsTraded* ev)
{
    Used.AddToType(ev->SellType, ev->SellAmount);
}

shok::ClassId __stdcall CppLogic::Mod::HawkCircleBehavior::GetClassIdentifier() const
{
	return Identifier;
}

void CppLogic::Mod::HawkCircleBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void CppLogic::Mod::HawkCircleBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void* CppLogic::Mod::HawkCircleBehavior::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::HawkCircleBehavior::operator delete(void* p)
{
	shok::Free(p);
}

void CppLogic::Mod::HawkCircleBehavior::AddHandlers(shok::EntityId id)
{
	EntityId = id;
	auto* e = EGL::CGLEEntity::GetEntityByID(id);
	e->CreateEventHandler<shok::EventIDs::Hawk_OnHeroDetach, BB::CEvent>(this, &HawkCircleBehavior::EventOnHeroDetach);
	e->CreateEventHandler<shok::EventIDs::HeroHawk_SendHawk, EGL::CEventPosition>(this, &HawkCircleBehavior::EventSendHawk);
	e->CreateTaskHandler<shok::Task::TASK_MOVE_TO_EXPLORATION_POINT, EGL::CGLETaskArgs>(this, &HawkCircleBehavior::TaskExplore);
	e->CreateTaskHandler<shok::Task::TASK_CHECK_HERO_POSITION, EGL::CGLETaskArgs>(this, &HawkCircleBehavior::TaskCheckHeroPos);
	e->CreateStateHandler<shok::TaskState::HawkExplore>(this, &HawkCircleBehavior::StateExplore);
	e->CreateStateHandler<shok::TaskState::HawkCheckHero>(this, &HawkCircleBehavior::StateCheckHeroPos);
}

shok::Position CppLogic::Mod::HawkCircleBehavior::GetNextCirclePosition(const shok::Position& center)
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	float a = center.GetAngleBetween(e->Position);
	float range = static_cast<GGL::CHawkBehaviorProps*>(PropPointer)->MaxCirclingDistance;
	a -= 10;
	return center + shok::Position{ range,0 }.Rotate(CppLogic::DegreesToRadians(a));
}

void CppLogic::Mod::HawkCircleBehavior::EventOnHeroDetach(BB::CEvent* e)
{
	EGL::CGLEEntity* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	en->Destroy();
}

void CppLogic::Mod::HawkCircleBehavior::EventSendHawk(EGL::CEventPosition* e)
{
	ExploreTarget = e->Position;
	EGL::CGLEEntity::GetEntityByID(EntityId)->SetTaskList(static_cast<GGL::CHawkBehaviorProps*>(PropPointer)->ExploreTaskList);
}

int CppLogic::Mod::HawkCircleBehavior::TaskExplore(EGL::CGLETaskArgs* a)
{
	EGL::CGLEEntity* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	EGL::CGLEEntity* hero = EGL::CGLEEntity::GetEntityByID(en->GetFirstAttachedToMe(shok::AttachmentType::HERO_HAWK));
	if (!hero) {
		en->Destroy();
		return 2;
	}
	float r = hero->GetEntityType()->GetBehaviorProps<GGL::CHeroHawkBehaviorProps>()->HawkMaxRange;
	if (!en->Position.IsInRange(hero->Position, r)) {
		en->SetTaskList(static_cast<GGL::CGLAnimalProps*>(en->GetEntityType()->LogicProps)->DefaultTaskList);
		return 2;
	}
	shok::Position t = GetNextCirclePosition(ExploreTarget);
	EGL::CEventPositionAndTaskState ev{ shok::EventIDs::Movement_TaskMoveToPosAndSetState, t, shok::TaskState::HawkExplore };
	en->FireEvent(&ev);
	en->GetBehaviorDynamic<EGL::CMovementBehavior>()->IsPathingUsed = false;
	return 1;
}

shok::TaskStateExecutionResult CppLogic::Mod::HawkCircleBehavior::StateExplore(int t)
{
	EGL::CGLEEntity* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	EGL::CGLEEntity* hero = EGL::CGLEEntity::GetEntityByID(en->GetFirstAttachedToMe(shok::AttachmentType::HERO_HAWK));
	if (!hero) {
		en->Destroy();
		return shok::TaskStateExecutionResult::Finished;
	}
	float r = hero->GetEntityType()->GetBehaviorProps<GGL::CHeroHawkBehaviorProps>()->HawkMaxRange;
	if (hero->Health <= 0 || !en->Position.IsInRange(hero->Position, r)) {
		en->SetTaskList(static_cast<GGL::CGLAnimalProps*>(en->GetEntityType()->LogicProps)->DefaultTaskList);
		return shok::TaskStateExecutionResult::Finished;
	}

	shok::Position tp = GetNextCirclePosition(ExploreTarget);
	EGL::CEventPositionAndTaskState ev{ shok::EventIDs::Movement_TaskMoveToPosAndSetState, tp, shok::TaskState::HawkExplore };
	en->FireEvent(&ev);
	en->GetBehaviorDynamic<EGL::CMovementBehavior>()->IsPathingUsed = false;

	EGL::CEventThousandthsGetInteger ev2{ shok::EventIDs::Movement_StatePerformMovementStep, t };
	en->FireEvent(&ev2);

	return static_cast<shok::TaskStateExecutionResult>(ev2.Data);
}

int CppLogic::Mod::HawkCircleBehavior::TaskCheckHeroPos(EGL::CGLETaskArgs* a)
{
	EGL::CGLEEntity* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	EGL::CGLEEntity* hero = EGL::CGLEEntity::GetEntityByID(en->GetFirstAttachedToMe(shok::AttachmentType::HERO_HAWK));
	if (!hero) {
		en->Destroy();
		return 2;
	}
	shok::Position tp = GetNextCirclePosition(hero->Position);
	EGL::CEventPositionAndTaskState ev{ shok::EventIDs::Movement_TaskMoveToPosAndSetState, tp, shok::TaskState::HawkCheckHero };
	en->FireEvent(&ev);
	en->GetBehaviorDynamic<EGL::CMovementBehavior>()->IsPathingUsed = false;
	return 1;
}

shok::TaskStateExecutionResult CppLogic::Mod::HawkCircleBehavior::StateCheckHeroPos(int t)
{
	EGL::CGLEEntity* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	EGL::CGLEEntity* hero = EGL::CGLEEntity::GetEntityByID(en->GetFirstAttachedToMe(shok::AttachmentType::HERO_HAWK));
	if (!hero) {
		en->Destroy();
		return shok::TaskStateExecutionResult::Finished;
	}

	shok::Position tp = GetNextCirclePosition(hero->Position);
	EGL::CEventPositionAndTaskState ev{ shok::EventIDs::Movement_TaskMoveToPosAndSetState, tp, shok::TaskState::HawkCheckHero };
	en->FireEvent(&ev);
	en->GetBehaviorDynamic<EGL::CMovementBehavior>()->IsPathingUsed = false;

	EGL::CEventThousandthsGetInteger ev2{ shok::EventIDs::Movement_StatePerformMovementStep, t };
	en->FireEvent(&ev2);

	return static_cast<shok::TaskStateExecutionResult>(ev2.Data);
}

BB::SerializationData CppLogic::Mod::HawkCircleBehavior::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<CppLogic::Mod::HawkCircleBehavior, EGL::CGLEBehavior>(),
	AutoMemberSerialization(HawkCircleBehavior, ExploreTarget),
	BB::SerializationData::GuardData(),
};


void* CppLogic::Mod::HawkOwnerAbility::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::HawkOwnerAbility::operator delete(void* p)
{
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::HawkOwnerAbility::GetClassIdentifier() const
{
	return Identifier;
}

bool CppLogic::Mod::HawkOwnerAbility::IsAbility(shok::AbilityId ability)
{
	return ability == AbilityId;
}

void CppLogic::Mod::HawkOwnerAbility::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	GGL::CHeroAbility::OnEntityCreate(p);
	EventCreateHawk(nullptr); // i know the event is not used, so just pass 0
}

void CppLogic::Mod::HawkOwnerAbility::AddHandlers(shok::EntityId id)
{
	GGL::CHeroAbility::AddHandlers(id);
	auto* e = EGL::CGLEEntity::GetEntityByID(id);
	e->CreateEventHandler<shok::EventIDs::HeroAbility_StandUp, BB::CEvent>(this, &HawkOwnerAbility::EventCreateHawk);
	e->CreateEventHandler<shok::EventIDs::HeroHawk_SendHawk, EGL::CEventPosition>(this, &HawkOwnerAbility::EventSendHawk);
	e->CreateEventHandler<shok::EventIDs::Die, BB::CEvent>(this, &HawkOwnerAbility::EventDie);
}

void CppLogic::Mod::HawkOwnerAbility::EventCreateHawk(BB::CEvent* e)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	shok::EntityId hawkid = en->GetFirstAttachedEntity(shok::AttachmentType::HERO_HAWK);
	if (hawkid == shok::EntityId::Invalid) {
		EGL::CGLEEntityCreator cr{};
		cr.EntityType = static_cast<GGL::CHeroHawkBehaviorProps*>(AbilityProps)->HawkType;
		cr.Pos = en->Position;
		cr.PlayerId = en->PlayerId;
		hawkid = (*EGL::CGLEGameLogic::GlobalObj)->CreateEntity(&cr);
		en->AttachEntity(shok::AttachmentType::HERO_HAWK, hawkid, shok::EventIDs::Hawk_OnHeroDetach, shok::EventIDs::NoDetachEvent);
	}
	auto* hen = EGL::CGLEEntity::GetEntityByID(hawkid);
	hen->Exploration = hen->GetEntityType()->LogicProps->Exploration;
}

void CppLogic::Mod::HawkOwnerAbility::EventSendHawk(EGL::CEventPosition* ev)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	shok::EntityId hid = en->GetFirstAttachedEntity(shok::AttachmentType::HERO_HAWK);
	if (hid != shok::EntityId::Invalid) {
		auto* hen = EGL::CGLEEntity::GetEntityByID(hid);
		hen->FireEvent(ev);
	}
}

void CppLogic::Mod::HawkOwnerAbility::EventDie(BB::CEvent* e)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	shok::EntityId hid = en->GetFirstAttachedEntity(shok::AttachmentType::HERO_HAWK);
	if (hid != shok::EntityId::Invalid)
		EGL::CGLEEntity::GetEntityByID(hid)->Exploration = 0;
}

BB::SerializationData CppLogic::Mod::HawkOwnerAbility::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<GGL::CHeroAbility, GGL::CHeroAbility>(),
	BB::SerializationData::GuardData(),
};



const BB::SerializationData CppLogic::Mod::LightningStrikeAbilityProps::SerializationData[]{
    BB::SerializationData::AutoBaseClass<LightningStrikeAbilityProps, GGL::CHeroAbilityProps>(),
    AutoMemberSerialization(LightningStrikeAbilityProps, Damage),
    AutoMemberSerialization(LightningStrikeAbilityProps, DamageClass),
    AutoMemberSerialization(LightningStrikeAbilityProps, Range),
    AutoMemberSerialization(LightningStrikeAbilityProps, DamageRange),
    AutoMemberSerialization(LightningStrikeAbilityProps, Effect),
    AutoMemberSerialization(LightningStrikeAbilityProps, WeatherEnergyCost),
    BB::SerializationData::GuardData(),
};

shok::ClassId __stdcall CppLogic::Mod::LightningStrikeAbilityProps::GetClassIdentifier() const
{
    return Identifier;
}

void* CppLogic::Mod::LightningStrikeAbilityProps::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::LightningStrikeAbilityProps::operator delete(void* p) {
    shok::Free(p);
}

const BB::SerializationData CppLogic::Mod::LightningStrikeAbility::SerializationData[]{
    BB::SerializationData::AutoBaseClass<LightningStrikeAbility, GGL::CHeroAbility>(),
    BB::SerializationData::GuardData(),
};

GGL::CStaticHeroAbilityID CppLogic::Mod::LightningStrikeAbility::AbilityStatic = GGL::CStaticHeroAbilityID::Create<LightningStrikeAbility::AbilityId>();

shok::ClassId __stdcall CppLogic::Mod::LightningStrikeAbility::GetClassIdentifier() const
{
    return Identifier;
}

void* CppLogic::Mod::LightningStrikeAbility::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::LightningStrikeAbility::operator delete(void* p) {
    shok::Free(p);
}

void CppLogic::Mod::LightningStrikeAbility::AddHandlers(shok::EntityId id)
{
    GGL::CHeroAbility::AddHandlers(id);
    auto* e = EGL::CGLEEntity::GetEntityByID(id);
    e->CreateEventHandler<shok::EventIDs::CppL_LightningStrike_Activate>(this, &LightningStrikeAbility::EventLightningStrike);
}

bool CppLogic::Mod::LightningStrikeAbility::IsAbility(shok::AbilityId ability)
{
    return ability == AbilityId;
}

void CppLogic::Mod::LightningStrikeAbility::EventLightningStrike(EGL::CEventPosition* p)
{
    auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
    const auto* prop = static_cast<LightningStrikeAbilityProps*>(AbilityProps);

	if (p->Position.GetDistanceSquaredTo(e->Position) > prop->Range * prop->Range)
		return;
    auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId);
    if (pl->CurrentResources.WeatherEnergy < prop->WeatherEnergyCost)
        return;
    if (!CheckAndResetCooldown())
        return;

    pl->CurrentResources.SubFromType(shok::ResourceType::WeatherEnergy, prop->WeatherEnergyCost);

    EGL::CGLEEffectCreator c{};
    c.EffectType = prop->Effect;
    c.PlayerID = e->PlayerId;
    c.StartPos = p->Position;
    (*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&c);

    EGL::CGLEEntity::AdvancedDealAoEDamage(e, p->Position, prop->DamageRange, prop->Damage, e->PlayerId, prop->DamageClass, true, true, true, shok::AdvancedDealDamageSource::AbilityCircularAttack);
}

void CppLogic::Mod::LightningStrikeAbility::NetEventLightningStrike(EGL::CNetEventEntityAndPos* ev)
{
    auto* e = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
    EGL::CEventPosition ev2{ shok::EventIDs::CppL_LightningStrike_Activate, {ev->X, ev->Y} };
    e->FireEvent(&ev2);
}

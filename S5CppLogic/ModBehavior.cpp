#include "pch.h"
#include "ModBehavior.h"
#include "s5_events.h"
#include "s5_classfactory.h"
#include "s5_effects.h"
#include "s5_tasklist.h"
#include "entityiterator.h"

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
	f->AddClassToFactory<ResDoodadRefillBehaviorProps>();
	f->AddClassToFactory<ResDoodadRefillBehavior>();
	f->AddClassToFactory<ShieldCoverAbilityProps>();
	f->AddClassToFactory<ShieldCoverAbility>();
	f->AddClassToFactory<ResurrectAbilityProps>();
	f->AddClassToFactory<ResurrectAbility>();
	f->AddClassToFactory<BombardmentAbilityProps>();
	f->AddClassToFactory<BombardmentAbility>();
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

const BB::SerializationData CppLogic::Mod::FormationSpacedBehavior::SerializationData[]{
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
	return (center + shok::Position{ range,0 }.Rotate(CppLogic::DegreesToRadians(a))).ClampToWorld();
}

void CppLogic::Mod::HawkCircleBehavior::EventOnHeroDetach(BB::CEvent* e)
{
	EGL::CGLEEntity* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	en->Destroy();
}

void CppLogic::Mod::HawkCircleBehavior::EventSendHawk(EGL::CEventPosition* e)
{
	ExploreTarget = e->Position.ClampToWorld(static_cast<GGL::CHawkBehaviorProps*>(PropPointer)->MaxCirclingDistance);
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
	if (hero->Health <= 0 || !en->Position.IsInRange(hero->Position, r)
		|| ExploreTarget.IsInRange(hero->Position, static_cast<GGL::CHawkBehaviorProps*>(PropPointer)->MaxCirclingDistance)) {
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
	if (EGL::CGLEEntity::EntityIDIsDead(ev->EntityID))
		return;
	auto* e = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
	EGL::CEventPosition ev2{ shok::EventIDs::CppL_LightningStrike_Activate, {ev->X, ev->Y} };
	e->FireEvent(&ev2);
}

const BB::SerializationData CppLogic::Mod::ResDoodadRefillBehaviorProps::SerializationData[]{
	BB::SerializationData::AutoBaseClass<ResDoodadRefillBehaviorProps, GGL::CHeroAbilityProps>(),
	AutoMemberSerialization(ResDoodadRefillBehaviorProps, RefillAmount),
	AutoMemberSerialization(ResDoodadRefillBehaviorProps, Effect),
	AutoMemberSerialization(ResDoodadRefillBehaviorProps, AffectedTypes),
	AutoMemberSerialization(ResDoodadRefillBehaviorProps, TaskList),
	BB::SerializationData::GuardData(),
};

shok::ClassId __stdcall CppLogic::Mod::ResDoodadRefillBehaviorProps::GetClassIdentifier() const
{
	return Identifier;
}

void* CppLogic::Mod::ResDoodadRefillBehaviorProps::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::ResDoodadRefillBehaviorProps::operator delete(void* p) {
	shok::Free(p);
}

const BB::SerializationData CppLogic::Mod::ResDoodadRefillBehavior::SerializationData[]{
	BB::SerializationData::AutoBaseClass<ResDoodadRefillBehavior, GGL::CHeroAbility>(),
	BB::SerializationData::GuardData(),
};

GGL::CStaticHeroAbilityID CppLogic::Mod::ResDoodadRefillBehavior::AbilityStatic = GGL::CStaticHeroAbilityID::Create<ResDoodadRefillBehavior::AbilityId>();

shok::ClassId __stdcall CppLogic::Mod::ResDoodadRefillBehavior::GetClassIdentifier() const
{
	return Identifier;
}

void* CppLogic::Mod::ResDoodadRefillBehavior::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::ResDoodadRefillBehavior::operator delete(void* p) {
	shok::Free(p);
}

void CppLogic::Mod::ResDoodadRefillBehavior::AddHandlers(shok::EntityId id)
{
	GGL::CHeroAbility::AddHandlers(id);
	auto* e = EGL::CGLEEntity::GetEntityByID(id);
	e->CreateTaskHandler<shok::Task::TASK_GO_TO_RESOURCE>(this, &ResDoodadRefillBehavior::TaskGoToResource);
	e->CreateTaskHandler<shok::Task::TASK_EXTRACT_RESOURCE>(this, &ResDoodadRefillBehavior::TaskExtractRes);
	e->CreateEventHandler<shok::EventIDs::CppL_ResDoodadRefill_Activate>(this, &ResDoodadRefillBehavior::EventActivate);
}

bool CppLogic::Mod::ResDoodadRefillBehavior::IsAbility(shok::AbilityId ability)
{
	return ability == AbilityId;
}

int CppLogic::Mod::ResDoodadRefillBehavior::TaskGoToResource(EGL::CGLETaskArgs* a)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto eid = e->GetFirstAttachedEntity(shok::AttachmentType::SERF_RESOURCE);
	EGL::CEvent1Entity ev{ shok::EventIDs::Settler_MoveToBuildingApproachPos, eid };
	e->FireEvent(&ev);
	return 0;
}

int CppLogic::Mod::ResDoodadRefillBehavior::TaskExtractRes(EGL::CGLETaskArgs* a)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto t = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::SERF_RESOURCE));
	if (t == nullptr)
		return 0;
	auto* res = EGL::CGLEEntity::GetEntityByID(t->GetFirstAttachedEntity(shok::AttachmentType::MINE_RESOURCE));
	if (res == nullptr)
		return 0;
	auto* pr = static_cast<ResDoodadRefillBehaviorProps*>(AbilityProps);
	if (auto* rese = BB::IdentifierCast<GGL::CResourceDoodad>(res)) {
		rese->SetCurrentResourceAmount(std::min(rese->ResourceAmount + pr->RefillAmount, rese->ResourceAmountAdd));
	}
	if (pr->Effect != shok::EffectTypeId::Invalid) {
		EGL::CGLEEffectCreator c{};
		c.EffectType = pr->Effect;
		c.PlayerID = e->PlayerId;
		c.StartPos = t->Position;
		(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&c);
	}
	return 0;
}

void CppLogic::Mod::ResDoodadRefillBehavior::EventActivate(EGL::CEvent1Entity* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* t = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
	if (t == nullptr || EGL::CGLEEntity::EntityIDIsDead(ev->EntityID))
		return;
	if (t->PlayerId != e->PlayerId)
		return;
	auto* res = EGL::CGLEEntity::GetEntityByID(t->GetFirstAttachedEntity(shok::AttachmentType::MINE_RESOURCE));
	if (res == nullptr)
		return;
	auto* pr = static_cast<ResDoodadRefillBehaviorProps*>(AbilityProps);
	if (!res->IsEntityInCategory(pr->AffectedTypes))
		return;
	if (!CheckAndResetCooldown())
		return;
	while (true) {
		auto id = e->GetFirstAttachedEntity(shok::AttachmentType::SERF_RESOURCE);
		if (id == shok::EntityId::Invalid)
			break;
		e->DetachObservedEntity(shok::AttachmentType::SERF_RESOURCE, id, false);
	}
	e->AttachEntity(shok::AttachmentType::SERF_RESOURCE, ev->EntityID, shok::EventIDs::NoDetachEvent, shok::EventIDs::NoDetachEvent);
	e->SetTaskList(pr->TaskList);
	e->GetBehaviorDynamic<GGL::CBattleBehavior>()->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
}

void CppLogic::Mod::ResDoodadRefillBehavior::NetEventRefillResDoodad(EGL::CNetEvent2Entities* ev)
{
	if (EGL::CGLEEntity::EntityIDIsDead(ev->EntityID1))
		return;
	auto* e = EGL::CGLEEntity::GetEntityByID(ev->EntityID1);
	EGL::CEvent1Entity ev2{ shok::EventIDs::CppL_ResDoodadRefill_Activate, ev->EntityID2 };
	e->FireEvent(&ev2);
}


void* CppLogic::Mod::ShieldCoverAbilityProps::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::ShieldCoverAbilityProps::operator delete(void* p) {
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::ShieldCoverAbilityProps::GetClassIdentifier() const
{
	return Identifier;
}

BB::SerializationData CppLogic::Mod::ShieldCoverAbilityProps::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<ShieldCoverAbilityProps, GGL::CHeroAbilityProps>(),
	AutoMemberSerialization(ShieldCoverAbilityProps, Range),
	AutoMemberSerialization(ShieldCoverAbilityProps, Duration),
	AutoMemberSerialization(ShieldCoverAbilityProps, TaskList),
	BB::SerializationData::GuardData(),
};



void* CppLogic::Mod::ShieldCoverAbility::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::ShieldCoverAbility::operator delete(void* p) {
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::ShieldCoverAbility::GetClassIdentifier() const
{
	return Identifier;
}

BB::SerializationData CppLogic::Mod::ShieldCoverAbility::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<ShieldCoverAbility, GGL::CHeroAbility>(),
	AutoMemberSerialization(ShieldCoverAbility, DurationLeft),
	BB::SerializationData::GuardData(),
};

GGL::CStaticHeroAbilityID CppLogic::Mod::ShieldCoverAbility::AbilityStatic = GGL::CStaticHeroAbilityID::Create<ShieldCoverAbility::AbilityId>();

void CppLogic::Mod::ShieldCoverAbility::AddHandlers(shok::EntityId id)
{
	GGL::CHeroAbility::AddHandlers(id);
	auto* en = EGL::CGLEEntity::GetEntityByID(id);
	en->CreateTaskHandler<shok::Task::TASK_SHIELD_COVER, EGL::CGLETaskArgs>(this, &ShieldCoverAbility::TaskShieldCover);
	en->CreateStateHandler<shok::TaskState::ShieldCover>(this, &ShieldCoverAbility::StateShieldCover);
	en->CreateEventHandler<shok::EventIDs::Behavior_Tick, BB::CEvent>(this, &ShieldCoverAbility::EventTick);
	en->CreateEventHandler<shok::EventIDs::CppL_ShieldCoverActivate, BB::CEvent>(this, &ShieldCoverAbility::EventActivate);
}

bool CppLogic::Mod::ShieldCoverAbility::IsAbility(shok::AbilityId ability)
{
	return ability == AbilityId;
}

int CppLogic::Mod::ShieldCoverAbility::TaskShieldCover(EGL::CGLETaskArgs* a)
{
	EGL::CGLEEntity::HookCanCancelState();
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	en->SetTaskState(shok::TaskState::ShieldCover);
	ClearProjectiles();
	return 1;
}

shok::TaskStateExecutionResult CppLogic::Mod::ShieldCoverAbility::StateShieldCover(int)
{
	if (!DurationLeft)
		return shok::TaskStateExecutionResult::Finished;
	ClearProjectiles();
	return shok::TaskStateExecutionResult::NotFinished;
}

void CppLogic::Mod::ShieldCoverAbility::EventTick(BB::CEvent* ev)
{
	if (DurationLeft)
		--DurationLeft;
}

void CppLogic::Mod::ShieldCoverAbility::Activate()
{
	if (!CheckAndResetCooldown())
		return;
	auto* pr = static_cast<ShieldCoverAbilityProps*>(AbilityProps);
	DurationLeft = pr->Duration;
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	en->SetTaskList(pr->TaskList);
	en->GetBehaviorDynamic<GGL::CLeaderBehavior>()->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
}
void CppLogic::Mod::ShieldCoverAbility::EventActivate(BB::CEvent* ev)
{
	Activate();
}

void CppLogic::Mod::ShieldCoverAbility::ClearProjectiles()
{
	auto* pr = static_cast<ShieldCoverAbilityProps*>(AbilityProps);
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	CppLogic::Iterator::PredicateInCircle<EGL::CEffect> ic{ en->Position, pr->Range * pr->Range };
	CppLogic::Iterator::GlobalEffectIterator it{ &ic };
	for (auto* eff : it) {
		if (GGL::CPlayerStatus::ArePlayersHostile(eff->PlayerID, en->PlayerId)) {
			if (dynamic_cast<GGL::CArrowEffect*>(eff))
				delete eff;
		}
	}
}

void CppLogic::Mod::ShieldCoverAbility::NetEventShieldCover(EGL::CNetEventEntityID* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
	if (e->IsDead())
		return;
	auto* ab = e->GetBehavior<CppLogic::Mod::ShieldCoverAbility>();
	if (!ab)
		return;
	if (!ab->CanUseAbility())
		return;
	ab->Activate();
}




void* CppLogic::Mod::ResurrectAbilityProps::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::ResurrectAbilityProps::operator delete(void* p) {
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::ResurrectAbilityProps::GetClassIdentifier() const
{
	return Identifier;
}

BB::SerializationData CppLogic::Mod::ResurrectAbilityProps::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<ResurrectAbilityProps, GGL::CHeroAbilityProps>(),
	AutoMemberSerialization(ResurrectAbilityProps, Range),
	AutoMemberSerialization(ResurrectAbilityProps, ProgressPerTick),
	AutoMemberSerialization(ResurrectAbilityProps, TaskList),
	AutoMemberSerialization(ResurrectAbilityProps, RechargeAbilitiesBy),
	BB::SerializationData::GuardData(),
};



void* CppLogic::Mod::ResurrectAbility::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::ResurrectAbility::operator delete(void* p) {
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::ResurrectAbility::GetClassIdentifier() const
{
	return Identifier;
}

BB::SerializationData CppLogic::Mod::ResurrectAbility::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<ResurrectAbility, GGL::CHeroAbility>(),
	AutoMemberSerialization(ResurrectAbility, Target),
	AutoMemberSerialization(ResurrectAbility, SelfCasting),
	BB::SerializationData::GuardData(),
};

GGL::CStaticHeroAbilityID CppLogic::Mod::ResurrectAbility::AbilityStatic = GGL::CStaticHeroAbilityID::Create<ResurrectAbility::AbilityId>();

void CppLogic::Mod::ResurrectAbility::AddHandlers(shok::EntityId id)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(id);
	en->CreateEventHandler<shok::EventIDs::Die>(this, &ResurrectAbility::EventDie);
	GGL::CHeroAbility::AddHandlers(id);
	en->CreateEventHandler<shok::EventIDs::CppL_Resurrect_Activate>(this, &ResurrectAbility::EventActivate);
	en->CreateEventHandler<shok::EventIDs::Behavior_Tick>(this, &ResurrectAbility::EventTick);
	en->CreateTaskHandler<shok::Task::TASK_RESURRECT>(this, &ResurrectAbility::TaskResurrect);
	en->CreateTaskHandler<shok::Task::TASK_TURN_TO_RESURRECT>(this, &ResurrectAbility::TaskTurnTo);
	en->CreateStateHandler<shok::TaskState::Resurrect>(this, &ResurrectAbility::StateResurrect);
	en->CreateStateHandler<shok::TaskState::SelfResurrect>(this, &ResurrectAbility::StateSelfResurrect);
}

bool CppLogic::Mod::ResurrectAbility::IsAbility(shok::AbilityId ability)
{
	return ability == AbilityId;
}

void CppLogic::Mod::ResurrectAbility::EventActivate(EGL::CEvent1Entity* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* t = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
	if (t == nullptr || !EGL::CGLEEntity::EntityIDIsDead(ev->EntityID))
		return;
	if (t->PlayerId != e->PlayerId)
		return;
	if (t->GetBehavior<GGL::CHeroBehavior>() == nullptr)
		return;
	auto* pr = static_cast<ResurrectAbilityProps*>(AbilityProps);
	if (!e->Position.IsInRange(t->Position, pr->Range))
		return;
	if (!CheckAndResetCooldown())
		return;
	Target = t->EntityId;
	e->SetTaskList(pr->TaskList);
	e->GetBehaviorDynamic<GGL::CBattleBehavior>()->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
}

void CppLogic::Mod::ResurrectAbility::EventDie(BB::CEvent* ev)
{
	auto* pr = static_cast<ResurrectAbilityProps*>(AbilityProps);
	if (SecondsCharged >= pr->RechargeTimeSeconds)
		SelfCasting = true;
	SecondsCharged = 0;
}

void CppLogic::Mod::ResurrectAbility::EventTick(BB::CEvent* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	if (SelfCasting)
	{
		if (e->CurrentState == shok::TaskState::HeroComatose) {
			SelfCasting = false;
			e->SetTaskState(shok::TaskState::SelfResurrect);
		}
		else if (!e->IsDead()) {
			SelfCasting = false;
		}
	}
}

int CppLogic::Mod::ResurrectAbility::TaskResurrect(EGL::CGLETaskArgs* a)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* t = EGL::CGLEEntity::GetEntityByID(Target);
	if (t == nullptr || !t->IsDead())
		return 0;
	e->SetTaskState(shok::TaskState::Resurrect);
	return 0;
}

int CppLogic::Mod::ResurrectAbility::TaskTurnTo(EGL::CGLETaskArgs* a)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* t = EGL::CGLEEntity::GetEntityByID(Target);
	if (t == nullptr || !t->IsDead())
		return 0;
	static_cast<EGL::CMovingEntity*>(e)->SetTargetRotation(e->Position.GetAngleBetweenR(t->Position));
	EGL::CGLETaskArgs ta{};
	ta.TaskType = shok::Task::TASK_TURN_TO_TARGET_ORIENTATION;
	return static_cast<int>(e->ExecuteTask(&ta));
}

shok::TaskStateExecutionResult CppLogic::Mod::ResurrectAbility::StateResurrect(int time)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* t = EGL::CGLEEntity::GetEntityByID(Target);
	if (t == nullptr || !t->IsDead())
		return shok::TaskStateExecutionResult::Finished;
	auto* b = t->GetBehavior<GGL::CHeroBehavior>();
	if (b == nullptr)
		return shok::TaskStateExecutionResult::Finished;
	auto* pr = static_cast<ResurrectAbilityProps*>(AbilityProps);
	if (b->AdvanceProgress(static_cast<int>(pr->ProgressPerTick * time * *reinterpret_cast<const float*>(0x874970)))) {
		ChargeAbilitiesFor(t);
		return shok::TaskStateExecutionResult::Finished;
	}
	return shok::TaskStateExecutionResult::NotFinished;
}

void CppLogic::Mod::ResurrectAbility::NetEventResurrect(EGL::CNetEvent2Entities* ev)
{
	if (EGL::CGLEEntity::EntityIDIsDead(ev->EntityID1))
		return;
	auto* e = EGL::CGLEEntity::GetEntityByID(ev->EntityID1);
	EGL::CEvent1Entity ev2{ shok::EventIDs::CppL_Resurrect_Activate, ev->EntityID2 };
	e->FireEvent(&ev2);
}

shok::TaskStateExecutionResult CppLogic::Mod::ResurrectAbility::StateSelfResurrect(int time)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	if (!e->IsDead())
		return shok::TaskStateExecutionResult::Finished;
	auto* b = e->GetBehavior<GGL::CHeroBehavior>();
	auto* pr = static_cast<ResurrectAbilityProps*>(AbilityProps);
	if (b->AdvanceProgress(static_cast<int>(pr->ProgressPerTick * time * *reinterpret_cast<const float*>(0x874970)))) {
		ChargeAbilitiesFor(e);
		return shok::TaskStateExecutionResult::Finished;
	}
	return shok::TaskStateExecutionResult::NotFinished;
}

void CppLogic::Mod::ResurrectAbility::ChargeAbilitiesFor(EGL::CGLEEntity* e)
{
	auto* pr = static_cast<ResurrectAbilityProps*>(AbilityProps);
	for (auto* a : e->Behaviours) {
		if (auto* ab = dynamic_cast<GGL::CHeroAbility*>(a)) {
			if (ab->IsAbility(shok::AbilityId::AbilityResurrect))
				continue;
			ab->SecondsCharged = std::min(ab->AbilityProps->RechargeTimeSeconds, pr->RechargeAbilitiesBy);
		}
	}
}

void* CppLogic::Mod::BombardmentAbilityProps::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::BombardmentAbilityProps::operator delete(void* p) {
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::BombardmentAbilityProps::GetClassIdentifier() const
{
	return Identifier;
}

BB::SerializationData CppLogic::Mod::BombardmentAbilityProps::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<BombardmentAbilityProps, GGL::CHeroAbilityProps>(),
	AutoMemberSerialization(BombardmentAbilityProps, EffectType),
	AutoMemberSerialization(BombardmentAbilityProps, AttackRange),
	AutoMemberSerialization(BombardmentAbilityProps, DamageRange),
	AutoMemberSerialization(BombardmentAbilityProps, Damage),
	AutoMemberSerialization(BombardmentAbilityProps, DamageClass),
	AutoMemberSerialization(BombardmentAbilityProps, TaskList),
	AutoMemberSerialization(BombardmentAbilityProps, DistanceOverride),
	BB::SerializationData::GuardData(),
};


void* CppLogic::Mod::BombardmentAbility::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::BombardmentAbility::operator delete(void* p) {
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::BombardmentAbility::GetClassIdentifier() const
{
	return Identifier;
}

BB::SerializationData CppLogic::Mod::BombardmentAbility::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<BombardmentAbility, GGL::CHeroAbility>(),
	AutoMemberSerialization(BombardmentAbility, Target),
	BB::SerializationData::GuardData(),
};

GGL::CStaticHeroAbilityID CppLogic::Mod::BombardmentAbility::AbilityStatic = GGL::CStaticHeroAbilityID::Create<BombardmentAbility::AbilityId>();

void CppLogic::Mod::BombardmentAbility::AddHandlers(shok::EntityId id)
{
	GGL::CHeroAbility::AddHandlers(id);
	auto* en = EGL::CGLEEntity::GetEntityByID(id);
	en->CreateTaskHandler<shok::Task::TASK_BOMBARD_TARGET_AREA, EGL::CGLETaskArgs>(this, &BombardmentAbility::TaskBombard);
	en->CreateTaskHandler<shok::Task::TASK_TURN_TO_BOMBARD_TARGET, EGL::CGLETaskArgs>(this, &BombardmentAbility::TaskTurnToBombardTarget);
	en->CreateEventHandler<shok::EventIDs::CppL_Bombardment_Activate, EGL::CEventPosition>(this, &BombardmentAbility::EventActivate);
}

bool CppLogic::Mod::BombardmentAbility::IsAbility(shok::AbilityId ability)
{
	return ability == AbilityId;
}

void CppLogic::Mod::BombardmentAbility::Activate(const shok::Position& tar)
{
	if (!CheckAndResetCooldown())
		return;
	Target = tar;
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* pr = static_cast<BombardmentAbilityProps*>(AbilityProps);
	en->SetTaskList(pr->TaskList);
	en->GetBehaviorDynamic<GGL::CLeaderBehavior>()->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
}

void CppLogic::Mod::BombardmentAbility::NetEventBombard(EGL::CNetEventEntityAndPos* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
	if (e->IsDead())
		return;
	auto* ab = e->GetBehavior<CppLogic::Mod::BombardmentAbility>();
	if (!ab)
		return;
	shok::Position p{ ev->X, ev->Y };
	if (!ab->CanUseAbility())
		return;
	auto* pr = static_cast<CppLogic::Mod::BombardmentAbilityProps*>(ab->AbilityProps);
	if (p.GetDistanceSquaredTo(e->Position) > pr->AttackRange * pr->AttackRange)
		return;
	ab->Activate(p);
}

int CppLogic::Mod::BombardmentAbility::TaskBombard(EGL::CGLETaskArgs* a)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* pr = static_cast<BombardmentAbilityProps*>(AbilityProps);
	if (pr->DistanceOverride > 0) {
		shok::Position dir = Target - en->Position;
		Target = en->Position + dir.Normalize() * pr->DistanceOverride;
	}

	CProjectileEffectCreator cr{};
	cr.EffectType = pr->EffectType;
	cr.PlayerID = en->PlayerId;
	cr.StartPos = cr.CurrentPos = en->Position;
	cr.TargetPos = Target;
	cr.AttackerID = en->EntityId;
	cr.Damage = pr->Damage;
	cr.DamageRadius = pr->DamageRange;
	cr.DamageClass = pr->DamageClass;
	cr.SourcePlayer = en->PlayerId;
	cr.AdvancedDamageSourceOverride = shok::AdvancedDealDamageSource::AbilityBombardment;
	(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&cr);
	return -2;
}

int CppLogic::Mod::BombardmentAbility::TaskTurnToBombardTarget(EGL::CGLETaskArgs* a)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	float rot = en->Position.GetAngleBetween(Target);
	rot = CppLogic::DegreesToRadians(rot);
	static_cast<EGL::CMovingEntity*>(en)->SetTargetRotation(rot);
	EGL::CGLETaskArgs t{};
	t.TaskType = shok::Task::TASK_TURN_TO_TARGET_ORIENTATION;
	en->ExecuteTask(&t);
	return 0;
}

void CppLogic::Mod::BombardmentAbility::EventActivate(EGL::CEventPosition* p)
{
	Activate(p->Position);
}

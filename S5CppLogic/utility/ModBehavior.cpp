#include "pch.h"
#include "ModBehavior.h"
#include <shok/events/s5_events.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/effect/s5_effects.h>
#include <shok/entity/s5_tasklist.h>
#include <utility/entityiterator.h>
#include <utility/ModEffect.h>

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
	f->AddClassToFactory<LimitedAmmoBehavior>();
	f->AddClassToFactory<ReloadableCannonBuilderAbilityProps>();
	f->AddClassToFactory<ReloadableCannonBuilderAbility>();
	f->AddClassToFactory<LimitedAmmoUIDisplayBehavior>();
	f->AddClassToFactory<AdvancedMarketBehaviorProps>();
	f->AddClassToFactory<AdvancedMarketBehavior>();
	f->AddClassToFactory<AdvancedFoundationBehaviorProps>();
	f->AddClassToFactory<AdvancedFoundationBehavior>();
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

	if (!p->Position.IsInRange(e->Position, prop->Range))
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

	int dmg = static_cast<int>(e->ModifyDamage(prop->Damage) * e->GetTotalAffectedDamageModifier());
	EGL::CGLEEntity::AdvancedDealAoEDamage(e, p->Position, prop->DamageRange, dmg, e->PlayerId, prop->DamageClass, true, true, true, shok::AdvancedDealDamageSource::AbilityCircularAttack);
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
	AutoMemberSerialization(BombardmentAbilityProps, EntityToCreate),
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
	en->CreateEventHandler<shok::EventIDs::CppL_Bombardment_Activate>(this, &BombardmentAbility::EventActivate);
	en->CreateTaskHandler<shok::Task::TASK_BOMBARD_TARGET_AREA>(this, &BombardmentAbility::TaskBombard);
	if (dynamic_cast<EGL::CMovingEntity*>(en) != nullptr) {
		en->CreateTaskHandler<shok::Task::TASK_TURN_TO_BOMBARD_TARGET>(this, &BombardmentAbility::TaskTurnToBombardTargetSettler);
	}
	else { // autocannon
		en->CreateTaskHandler<shok::Task::TASK_TURN_TO_BOMBARD_TARGET>(this, &BombardmentAbility::TaskTurnToBombardTargetAutocannon);
		en->CreateTaskHandler<shok::Task::TASK_BACK_TO_DEFAULT_TASKLIST>(this, &BombardmentAbility::TaskBackToDefaultTL);
		en->CreateStateHandler<shok::TaskState::TurnToBombardTarget>(this, &BombardmentAbility::StateTurnToBombardTargetAutocannon);
	}
}

bool CppLogic::Mod::BombardmentAbility::IsAbility(shok::AbilityId ability)
{
	return ability == AbilityId;
}

void CppLogic::Mod::BombardmentAbility::Activate(const shok::Position& tar)
{
	if (!CheckAndResetCooldown())
		return;
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* pr = static_cast<BombardmentAbilityProps*>(AbilityProps);
	if (!en->Position.IsInRange(tar, pr->AttackRange))
		return;
	Target = tar;
	en->SetTaskList(pr->TaskList);
	if (auto* lb = en->GetBehaviorDynamic<GGL::CLeaderBehavior>())
		lb->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
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
	if (!p.IsInRange(e->Position, pr->AttackRange))
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

	auto eid = en->GetFirstAttachedToMe(shok::AttachmentType::SUMMONER_SUMMONED);
	if (eid == shok::EntityId::Invalid)
		eid = EntityId;

	CProjectileEffectCreator cr{};
	cr.EffectType = pr->EffectType;
	cr.PlayerID = en->PlayerId;
	cr.StartPos = cr.CurrentPos = en->Position;
	cr.TargetPos = Target;
	cr.AttackerID = eid;
	cr.Damage = static_cast<int>(en->ModifyDamage(pr->Damage) * en->GetTotalAffectedDamageModifier());
	cr.DamageRadius = pr->DamageRange;
	cr.DamageClass = pr->DamageClass;
	cr.SourcePlayer = en->PlayerId;
	cr.AdvancedDamageSourceOverride = shok::AdvancedDealDamageSource::AbilityBombardment;
	auto eff = (*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&cr);
	if (EGL::CEffect* e = (*EGL::CGLEEffectManager::GlobalObj)->GetById(eff)) {
		if (auto* epe = dynamic_cast<Effect::EntityPlacerEffect*>(e)) {
			epe->ToCreate = pr->EntityToCreate;
			epe->AttachCreated = shok::AttachmentType::BOMBER_BOMB;
		}
	}
	return -2;
}

int CppLogic::Mod::BombardmentAbility::TaskTurnToBombardTargetSettler(EGL::CGLETaskArgs* a)
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

int CppLogic::Mod::BombardmentAbility::TaskTurnToBombardTargetAutocannon(EGL::CGLETaskArgs* a)
{
	EGL::CGLEEntity::GetEntityByID(EntityId)->SetTaskState(shok::TaskState::TurnToBombardTarget);
	return 0;
}

int CppLogic::Mod::BombardmentAbility::TaskBackToDefaultTL(EGL::CGLETaskArgs* a)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	en->SetTaskState(shok::TaskState::Default);
	return 0;
}

void CppLogic::Mod::BombardmentAbility::EventActivate(EGL::CEventPosition* p)
{
	Activate(p->Position);
}

shok::TaskStateExecutionResult CppLogic::Mod::BombardmentAbility::StateTurnToBombardTargetAutocannon(int time)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* acb = en->GetBehavior<GGL::CAutoCannonBehavior>();
	float rotspeed = acb->ACProps->RotationSpeed;

	float trot = CppLogic::DegreesToRadians(en->Position.GetAngleBetween(Target));
	float rdiff = shok::PositionRot::AngleDifference(en->Position.r, trot);
	float newrot;
	if (std::abs(rdiff) < rotspeed)
		newrot = trot;
	else
		newrot = en->Position.r + (rdiff > 0.0f ? rotspeed : -rotspeed);

	while (newrot < CppLogic::DegreesToRadians(0.0f))
		newrot += CppLogic::DegreesToRadians(360.0f);
	while (newrot > CppLogic::DegreesToRadians(360.0f))
		newrot -= CppLogic::DegreesToRadians(360.0f);

	acb->LastTurnOrientation = en->Position.r;
	en->SetRotation(newrot);

	if (std::abs(trot - newrot) <= 1.0e-10f) {
		acb->LastTurnOrientation = newrot;
		return shok::TaskStateExecutionResult::Finished;
	}

	return shok::TaskStateExecutionResult::NotFinished;
}

shok::ClassId __stdcall CppLogic::Mod::LimitedAmmoBehavior::GetClassIdentifier() const
{
	return Identifier;
}

void CppLogic::Mod::LimitedAmmoBehavior::AddHandlers(shok::EntityId id)
{
	EntityId = id;
	auto* e = EGL::CGLEEntity::GetEntityByID(id);
	e->CreateTaskHandler<shok::Task::TASK_DECREMENT_AMMO>(this, &LimitedAmmoBehavior::TaskDecrementAmmo);
	e->CreateTaskHandler<shok::Task::TASK_CHECK_AMMO>(this, &LimitedAmmoBehavior::TaskCheckAmmo);
	e->CreateStateHandler<shok::TaskState::CheckAmmo>(this, &LimitedAmmoBehavior::StateCheckAmmo);
}

void CppLogic::Mod::LimitedAmmoBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void CppLogic::Mod::LimitedAmmoBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

BB::SerializationData CppLogic::Mod::LimitedAmmoBehavior::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<LimitedAmmoBehavior, EGL::CGLEBehavior>(),
	AutoMemberSerialization(LimitedAmmoBehavior, RemainingAmmo),
	AutoMemberSerialization(LimitedAmmoBehavior, MaxAmmo),
	BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::LimitedAmmoBehavior::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::LimitedAmmoBehavior::operator delete(void* p)
{
	shok::Free(p);
}

int CppLogic::Mod::LimitedAmmoBehavior::TaskDecrementAmmo(EGL::CGLETaskArgs* a)
{
	--RemainingAmmo;
	return 0;
}

void CppLogic::Mod::LimitedAmmoBehavior::ReloadToMax(int max)
{
	RemainingAmmo = std::max(RemainingAmmo, max);
	MaxAmmo = std::max(MaxAmmo, max);
}

int CppLogic::Mod::LimitedAmmoBehavior::TaskCheckAmmo(EGL::CGLETaskArgs* a)
{
	if (RemainingAmmo <= 0) {
		auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
		e->SetTaskState(shok::TaskState::CheckAmmo);
	}
	return 0;
}

shok::TaskStateExecutionResult CppLogic::Mod::LimitedAmmoBehavior::StateCheckAmmo(int time)
{
	if (RemainingAmmo > 0)
		return shok::TaskStateExecutionResult::Finished;
	return shok::TaskStateExecutionResult::NotFinished;
}


BB::SerializationData CppLogic::Mod::ReloadableCannonBuilderAbilityProps::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<ReloadableCannonBuilderAbilityProps, GGL::CCannonBuilderBehaviorProps>(),
	AutoMemberSerialization(ReloadableCannonBuilderAbilityProps, Reloads),
	AutoMemberSerialization(ReloadableCannonBuilderAbilityProps, ReloadRange),
	BB::SerializationData::GuardData(),
};

shok::ClassId __stdcall CppLogic::Mod::ReloadableCannonBuilderAbilityProps::GetClassIdentifier() const
{
	return Identifier;
}

void* CppLogic::Mod::ReloadableCannonBuilderAbilityProps::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::ReloadableCannonBuilderAbilityProps::operator delete(void* p)
{
	shok::Free(p);
}

BB::SerializationData CppLogic::Mod::ReloadableCannonBuilderAbility::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<ReloadableCannonBuilderAbility, GGL::CCannonBuilderBehavior>(),
	BB::SerializationData::GuardData(),
};

shok::ClassId __stdcall CppLogic::Mod::ReloadableCannonBuilderAbility::GetClassIdentifier() const
{
	return Identifier;
}

void* CppLogic::Mod::ReloadableCannonBuilderAbility::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::ReloadableCannonBuilderAbility::operator delete(void* p)
{
	shok::Free(p);
}

void CppLogic::Mod::ReloadableCannonBuilderAbility::AddHandlers(shok::EntityId id)
{
	GGL::CHeroAbility::AddHandlers(id);
	auto* e = EGL::CGLEEntity::GetEntityByID(id);
	auto* cb = static_cast<GGL::CCannonBuilderBehavior*>(this);
	e->CreateTaskHandler<shok::Task::TASK_GO_TO_CANNON_POSITION>(cb, &ReloadableCannonBuilderAbility::TaskGoToCannonPos);
	e->CreateTaskHandler<shok::Task::TASK_BUILD_CANNON>(this, &ReloadableCannonBuilderAbility::TaskBuildCannon);
	e->CreateEventHandler<shok::EventIDs::CannonBuilder_BuildCannonCommand>(cb, &ReloadableCannonBuilderAbility::EventActivate);
	e->CreateEventHandler<shok::EventIDs::HeroAbility_Cancel>(cb, &ReloadableCannonBuilderAbility::EventCancel);
	e->CreateEventHandler<shok::EventIDs::Die>(cb, &ReloadableCannonBuilderAbility::EventCancel);
	e->CreateEventHandler<shok::EventIDs::AutoCannon_OnFoundationDetach>(cb, &ReloadableCannonBuilderAbility::EventOnFoundationDetach);
	e->CreateEventHandler<shok::EventIDs::Behavior_Tick>(this, &ReloadableCannonBuilderAbility::EventTick);
	e->CreateEventHandler<shok::EventIDs::CppL_BombComboCannon_Activate>(this, &ReloadableCannonBuilderAbility::EventBombComboCannonActivate);
}

void CppLogic::Mod::ReloadableCannonBuilderAbility::NetEventBombComboCannon(EGL::CNetEventEntityAndPos* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
	if (e->IsDead())
		return;
	auto* abc = e->GetBehavior<CppLogic::Mod::ReloadableCannonBuilderAbility>();
	if (!abc)
		return;
	auto* ab = e->GetBehavior<GGL::CBombPlacerBehavior>();
	if (!ab)
		return;
	if (!ab->CanUseAbility())
		return;
	EGL::CEventPosition ev2{ shok::EventIDs::CppL_BombComboCannon_Activate, { ev->X, ev->Y } };
	e->FireEvent(&ev2);
}

int CppLogic::Mod::ReloadableCannonBuilderAbility::TaskBuildCannon(EGL::CGLETaskArgs* a)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	while (auto* t = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::SUMMONER_SUMMONED))) {
		t->Destroy();
	}
	auto fid = e->GetFirstAttachedEntity(shok::AttachmentType::BUILDER_FOUNDATION);
	int r = GGL::CCannonBuilderBehavior::TaskBuildCannon(a);
	if (auto* f = EGL::CGLEEntity::GetEntityByID(fid)) {
		auto tid = f->GetFirstAttachedEntity(shok::AttachmentType::FOUNDATION_TOP_ENTITY);
		if (auto* t = EGL::CGLEEntity::GetEntityByID(tid)) {
			auto* pr = static_cast<ReloadableCannonBuilderAbilityProps*>(AbilityProps);
			e->AttachEntity(shok::AttachmentType::SUMMONER_SUMMONED, tid, shok::EventIDs::NoDetachEvent, shok::EventIDs::NoDetachEvent);
			if (auto* lab = t->GetBehavior<LimitedAmmoBehavior>())
				lab->ReloadToMax(pr->Reloads);
		}
	}
	return r;
}

void CppLogic::Mod::ReloadableCannonBuilderAbility::EventTick(BB::CEvent* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	if (auto* t = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::SUMMONER_SUMMONED))) {
		auto* pr = static_cast<ReloadableCannonBuilderAbilityProps*>(AbilityProps);
		if (e->Position.IsInRange(t->Position, pr->ReloadRange)) {
			if (auto* lab = t->GetBehavior<LimitedAmmoBehavior>())
				lab->ReloadToMax(pr->Reloads);
		}
	}
}

void CppLogic::Mod::ReloadableCannonBuilderAbility::EventBombComboCannonActivate(EGL::CEventPosition* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	if (auto* t = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::SUMMONER_SUMMONED))) {
		auto* pr = static_cast<ReloadableCannonBuilderAbilityProps*>(AbilityProps);
		if (e->Position.IsInRange(t->Position, pr->ReloadRange)) {
			if (auto* ba = t->GetBehavior<BombardmentAbility>()) {
				if (t->Position.IsInRange(ev->Position, static_cast<BombardmentAbilityProps*>(ba->AbilityProps)->AttackRange)) {
					ba->Activate(ev->Position);
					GGL::CEventHeroAbilityInteger ev2{ shok::EventIDs::HeroAbility_SetChargeCurrent, 0, shok::AbilityId::AbilityPlaceBomb };
					e->FireEvent(&ev2);
					return;
				}
			}
		}
	}
	EGL::CEventPosition ev2{ shok::EventIDs::BombPlacer_CommandPlaceBomb, ev->Position };
	e->FireEvent(&ev2);
}

shok::ClassId __stdcall CppLogic::Mod::LimitedAmmoUIDisplayBehavior::GetClassIdentifier() const
{
	return Identifier;
}

void* __stdcall CppLogic::Mod::LimitedAmmoUIDisplayBehavior::CastToIdentifier(shok::ClassId id)
{
	if (id == CppLogic::Mod::OnScreenInfoDisplayBehavior::Identifier)
		return static_cast<CppLogic::Mod::OnScreenInfoDisplayBehavior*>(this);
	return nullptr;
}

void __stdcall CppLogic::Mod::LimitedAmmoUIDisplayBehavior::OnAdd(ED::CEntity* edispl, ED::CBehaviorProps* props, shok::ModelId* modelOverride)
{
	Entity = edispl;
}

void __stdcall CppLogic::Mod::LimitedAmmoUIDisplayBehavior::Initialize(ED::CEntity* edispl, ED::CBehaviorProps* props)
{
}

void __stdcall CppLogic::Mod::LimitedAmmoUIDisplayBehavior::UpdateRenderNoTick(int count, float uk)
{
}

void __stdcall CppLogic::Mod::LimitedAmmoUIDisplayBehavior::UpdateRenderOneTick(int count, float uk)
{
}

void __stdcall CppLogic::Mod::LimitedAmmoUIDisplayBehavior::UpdateRenderManyTick(int count, float uk)
{
}

bool CppLogic::Mod::LimitedAmmoUIDisplayBehavior::RenderUI(GGUI::OnScreenInfoRenderer* renderer, shok::Position* screenPos, GGL::IGLGUIInterface::UIData* data, bool* active)
{
	if (auto* f = dynamic_cast<EGL::CGLEEntity*>(Entity->Entity)) {
		if (auto* t = EGL::CGLEEntity::GetEntityByID(f->GetFirstAttachedEntity(shok::AttachmentType::FOUNDATION_TOP_ENTITY))) {
			if (auto* la = t->GetBehavior<LimitedAmmoBehavior>()) {
				if (la->MaxAmmo > 0)
					data->LimitedLifespanBar = std::max(static_cast<float>(la->RemainingAmmo) / la->MaxAmmo, 0.00001f);
			}
		}
	}
	return false;
}

void* CppLogic::Mod::LimitedAmmoUIDisplayBehavior::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::LimitedAmmoUIDisplayBehavior::operator delete(void* p)
{
	shok::Free(p);
}

CppLogic::Mod::LimitedAmmoUIDisplayBehavior::LimitedAmmoUIDisplayBehavior()
{
	GGUI::C3DOnScreenInformationCustomWidget::HookOnScreenInfoDisplayBehavior();
}

shok::ClassId __stdcall CppLogic::Mod::AdvancedMarketBehaviorProps::GetClassIdentifier() const
{
	return Identifier;
}

BB::SerializationData CppLogic::Mod::AdvancedMarketBehaviorProps::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<AdvancedMarketBehaviorProps, GGL::CServiceBuildingBehaviorProperties>(),
	AutoMemberSerialization(AdvancedMarketBehaviorProps, WorkStepsNeededForTrade),
	BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::AdvancedMarketBehaviorProps::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::AdvancedMarketBehaviorProps::operator delete(void* p)
{
	shok::Free(p);
}

float& CppLogic::Mod::AdvancedMarketBehavior::ResourceData::ByResT(shok::ResourceType t)
{
	switch (t) {
	case shok::ResourceType::Gold:
	case shok::ResourceType::GoldRaw:
		return Gold;
	case shok::ResourceType::Wood:
	case shok::ResourceType::WoodRaw:
		return Wood;
	case shok::ResourceType::Clay:
	case shok::ResourceType::ClayRaw:
		return Clay;
	case shok::ResourceType::Stone:
	case shok::ResourceType::StoneRaw:
		return Stone;
	case shok::ResourceType::Iron:
	case shok::ResourceType::IronRaw:
		return Iron;
	case shok::ResourceType::Sulfur:
	case shok::ResourceType::SulfurRaw:
		return Sulfur;
	default:
		throw std::invalid_argument{ "invalid resource type" };
	}
}

BB::SerializationData CppLogic::Mod::AdvancedMarketBehavior::ResourceData::SerializationData[] = {
	AutoMemberSerialization(ResourceData, Gold),
	AutoMemberSerialization(ResourceData, Wood),
	AutoMemberSerialization(ResourceData, Clay),
	AutoMemberSerialization(ResourceData, Stone),
	AutoMemberSerialization(ResourceData, Iron),
	AutoMemberSerialization(ResourceData, Sulfur),
	BB::SerializationData::GuardData(),
};

shok::ClassId __stdcall CppLogic::Mod::AdvancedMarketBehavior::GetClassIdentifier() const
{
	return Identifier;
}

void CppLogic::Mod::AdvancedMarketBehavior::AddHandlers(shok::EntityId id)
{
	EntityId = id;
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	e->CreateEventHandler<shok::EventIDs::Market_WorkStep>(this, &AdvancedMarketBehavior::EventWork);
	e->CreateEventHandler<shok::EventIDs::Market_StartTrade>(this, &AdvancedMarketBehavior::EventStartTrade);
	e->CreateEventHandler<shok::EventIDs::Market_CancelTrade>(this, &AdvancedMarketBehavior::EventCancel);
	e->CreateEventHandler<shok::EventIDs::Market_GetProgress>(this, &AdvancedMarketBehavior::EventGetProgress);
}

void CppLogic::Mod::AdvancedMarketBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void CppLogic::Mod::AdvancedMarketBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

CppLogic::Mod::AdvancedMarketBehavior::AdvancedMarketBehavior()
{
	GGUI::C3DOnScreenInformationCustomWidget::HookAdditionalFloaties();
}

void* CppLogic::Mod::AdvancedMarketBehavior::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::AdvancedMarketBehavior::operator delete(void* p)
{
	shok::Free(p);
}

BB::SerializationData CppLogic::Mod::AdvancedMarketBehavior::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<AdvancedMarketBehavior, EGL::CGLEBehavior>(),
	AutoMemberSerialization(AdvancedMarketBehavior, SellResources),
	AutoMemberSerialization(AdvancedMarketBehavior, MinResources),
	AutoMemberSerialization(AdvancedMarketBehavior, WorkSteps),
	BB::SerializationData::GuardData(),
};

float CppLogic::Mod::AdvancedMarketBehavior::PredictPriceFor(shok::ResourceType rt, float am)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* m = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->TradeManager;
	float tradestep = TradeStepSize();

	auto golddata = *m->GetResource(shok::ResourceType::Gold);
	auto resdata = *m->GetResource(rt);

	float r = 0.0f;

	if (am > 0.0f) {
		while (am > 0.0f) {
			float t = std::min(tradestep, am);
			float p = std::ceil(t * resdata.CurrentPrice / golddata.CurrentPrice);
			resdata.AdjustPrice(t, true);
			golddata.AdjustPrice(p, false);
			r += p;
			am -= t;
		}
	}
	else {
		while (am < 0.0f) {
			float t = std::min(tradestep, -am);
			float p = std::ceil(t * golddata.CurrentPrice / resdata.CurrentPrice);
			resdata.AdjustPrice(p, false);
			golddata.AdjustPrice(t, true);
			r -= p;
			am += t;
		}
	}

	return r;
}

void CppLogic::Mod::AdvancedMarketBehavior::FillCurrentTrade()
{
	float tradestep = TradeStepSize();
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	CurrentTrade.Player = e->PlayerId;

	float amount = 0;
	shok::ResourceType resty = shok::ResourceType::None;
	bool isMin = false;

	for (auto currentres : ResTradeOrder) {
		float currentam = SellResources.ByResT(currentres);
		if (std::abs(currentam) > std::abs(amount)) {
			FillCurrentTrade(currentres, currentam, tradestep);
			if (!CurrentTrade.IsTradeValid())
				continue;
			amount = currentam;
			resty = currentres;
		}
	}

	if (resty == shok::ResourceType::None) {
		auto* m = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(CurrentTrade.Player);

		{
			float bank = m->CurrentResources.GetResourceAmountFromType(shok::ResourceType::Gold, true);
			float currentam = MinResources.ByResT(shok::ResourceType::Gold);
			if (bank < currentam) {
				for (auto currentres : ResTradeOrder) {
					float currentam = MinResources.ByResT(currentres);
					float bank = m->CurrentResources.GetResourceAmountFromType(currentres, true);
					if (currentam + tradestep > bank)
						continue;
					currentam = currentam - bank;
					if (std::abs(currentam) > std::abs(amount)) {
						FillCurrentTrade(currentres, currentam, tradestep);
						if (!CurrentTrade.IsTradeValid())
							continue;
						amount = currentam;
						resty = currentres;
						isMin = true;
					}
				}
			}
		}

		if (resty == shok::ResourceType::None) {
			for (auto currentres : ResTradeOrder) {
				float currentam = MinResources.ByResT(currentres);
				float bank = m->CurrentResources.GetResourceAmountFromType(currentres, true);
				if (currentam < bank)
					continue;
				currentam = currentam - bank;
				if (std::abs(currentam) > std::abs(amount)) {
					FillCurrentTrade(currentres, currentam, tradestep);
					if (!CurrentTrade.IsTradeValid())
						continue;
					amount = currentam;
					resty = currentres;
					isMin = true;
				}
			}
		}
	}

	if (resty != shok::ResourceType::None) {
		FillCurrentTrade(resty, amount, tradestep);
		if (isMin)
			CurrentTrade.ProgressAmount = 1.0f;
	}
	else {
		CurrentTrade = GGL::CTradeManager::TradeOrder{};
	}
}

float CppLogic::Mod::AdvancedMarketBehavior::TradeStepSize()
{
	auto pl = EGL::CGLEEntity::GetEntityByID(EntityId)->PlayerId;
	auto* p = static_cast<GGL::CServiceBuildingBehaviorProperties*>(PropPointer);
	return p->GetProgressAmount(pl);
}

void CppLogic::Mod::AdvancedMarketBehavior::FillCurrentTrade(shok::ResourceType res, float amount, float tradestep)
{
	CurrentTrade.ProgressAmount = 0.0f;
	auto* m = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(CurrentTrade.Player)->TradeManager;
	if (amount > 0) {
		CurrentTrade.BuyResourceType = res;
		CurrentTrade.SellResourceType = shok::ResourceType::Gold;
		CurrentTrade.BuyAmount = std::min(tradestep, amount);
		CurrentTrade.SellAmount = std::ceil(CurrentTrade.BuyAmount * m->GetRelativePrice(res, shok::ResourceType::Gold));
	}
	else {
		CurrentTrade.BuyResourceType = shok::ResourceType::Gold;
		CurrentTrade.SellResourceType = res;
		CurrentTrade.SellAmount = std::min(tradestep, -amount);
		CurrentTrade.BuyAmount = std::ceil(CurrentTrade.SellAmount * m->GetRelativePrice(shok::ResourceType::Gold, res));
	}
}

std::string_view CppLogic::Mod::AdvancedMarketBehavior::GetResIcon(shok::ResourceType t)
{
	switch (t) {
	case shok::ResourceType::Gold:
	case shok::ResourceType::GoldRaw:
		return "@icon:graphics\\textures\\gui\\i_res_gold_large,0.0625,0,0.875,0.71875,2,2,255,255,255,a|";
	case shok::ResourceType::Wood:
	case shok::ResourceType::WoodRaw:
		return "@icon:graphics\\textures\\gui\\i_res_wood_large,0.0625,0,0.875,0.875,2,2,255,255,255,a|";
	case shok::ResourceType::Clay:
	case shok::ResourceType::ClayRaw:
		return "@icon:graphics\\textures\\gui\\i_res_mud_large,0.0625,0,0.875,0.71875,2,2,255,255,255,a|";
	case shok::ResourceType::Stone:
	case shok::ResourceType::StoneRaw:
		return "@icon:graphics\\textures\\gui\\i_res_stone_large,0.0625,0,0.875,0.8125,2,2,255,255,255,a|";
	case shok::ResourceType::Iron:
	case shok::ResourceType::IronRaw:
		return "@icon:graphics\\textures\\gui\\i_res_iron_large,0.0625,0,0.875,0.71875,2,2,255,255,255,a|";
	case shok::ResourceType::Sulfur:
	case shok::ResourceType::SulfurRaw:
		return "@icon:graphics\\textures\\gui\\i_res_sulfur_large,0.0625,0,0.875,0.71875,2,2,255,255,255,a|";
	default:
		throw std::invalid_argument{ "invalid resource type" };
	}
}

std::string_view CppLogic::Mod::AdvancedMarketBehavior::ResPrefix(float r)
{
	if (r < 0)
		return "@defaultcolor|";
	else
		return "@color:0,255,0,a|+";
}

void CppLogic::Mod::AdvancedMarketBehavior::PerformTradeStep()
{
	FillCurrentTrade();
	if (!CurrentTrade.IsTradeValid())
		return;
	CurrentTrade.Complete(true);

	if (CurrentTrade.ProgressAmount <= 0.0f) {
		if (CurrentTrade.BuyResourceType == shok::ResourceType::Gold) {
			SellResources.ByResT(CurrentTrade.SellResourceType) += CurrentTrade.SellAmount;
		}
		else {
			SellResources.ByResT(CurrentTrade.BuyResourceType) -= CurrentTrade.BuyAmount;
		}
	}
	
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	// a is whitespace in s5extended onscreennumberssmall, cause space breaks other stuff...
	auto txt = std::format("@skipWhitespace:off|{}{}{}aa{}{}{}",
		GetResIcon(CurrentTrade.SellResourceType),
		ResPrefix(-CurrentTrade.SellAmount),
		-CurrentTrade.SellAmount,
		GetResIcon(CurrentTrade.BuyResourceType),
		ResPrefix(CurrentTrade.BuyAmount),
		CurrentTrade.BuyAmount
	);
	GGUI::AdvancedFloatieManager::GlobalObj.AddFloatie(e->Position, std::move(txt));
}

void CppLogic::Mod::AdvancedMarketBehavior::EventWork(BB::CEvent* ev)
{
	auto* p = static_cast<AdvancedMarketBehaviorProps*>(PropPointer);
	++WorkSteps;
	if (WorkSteps >= p->WorkStepsNeededForTrade) {
		PerformTradeStep();
		WorkSteps = 0;
	}
}

void CppLogic::Mod::AdvancedMarketBehavior::EventStartTrade(GGL::CEventTransaction* ev)
{
	if (ev->SellType == shok::ResourceType::Gold)
		SellResources.ByResT(ev->BuyType) = ev->BuyAmount;
	else
		MinResources.ByResT(ev->BuyType) = std::max(0.0f, ev->BuyAmount);
}

void CppLogic::Mod::AdvancedMarketBehavior::EventCancel(BB::CEvent* ev)
{
	SellResources = ResourceData{};
}

void CppLogic::Mod::AdvancedMarketBehavior::EventGetProgress(EGL::CEventGetValue_Float* ev)
{
	ev->Data = 1.0f;
}

shok::ClassId __stdcall CppLogic::Mod::AdvancedFoundationBehaviorProps::GetClassIdentifier() const
{
	return Identifier;
}

CreateSerializationListFor(CppLogic::Mod::AdvancedFoundationBehaviorProps, Turret);
BB::SerializationData CppLogic::Mod::AdvancedFoundationBehaviorProps::TurretData::SerializationData[] = {
	AutoMemberSerialization(TurretData, Turret),
	AutoMemberSerialization(TurretData, Offset),
	AutoMemberSerialization(TurretData, BuyCost),
	BB::SerializationData::GuardData(),
};
BB::SerializationData CppLogic::Mod::AdvancedFoundationBehaviorProps::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<AdvancedFoundationBehaviorProps, EGL::CGLEBehaviorProps>(),
	AutoMemberSerialization(AdvancedFoundationBehaviorProps, Turret),
	BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::AdvancedFoundationBehaviorProps::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::AdvancedFoundationBehaviorProps::operator delete(void* p)
{
	shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::AdvancedFoundationBehavior::GetClassIdentifier() const
{
	return Identifier;
}

CreateSerializationListFor(CppLogic::Mod::AdvancedFoundationBehavior, Turret);
BB::SerializationData CppLogic::Mod::AdvancedFoundationBehavior::TurretData::SerializationData[] = {
	AutoMemberSerialization(TurretData, Turret),
	AutoMemberSerialization(TurretData, Active),
	BB::SerializationData::GuardData(),
};
BB::SerializationData CppLogic::Mod::AdvancedFoundationBehavior::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<AdvancedFoundationBehavior, EGL::CGLEBehavior>(),
	AutoMemberSerialization(AdvancedFoundationBehavior, Turret),
	BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::AdvancedFoundationBehavior::operator new(size_t s)
{
	return shok::New(s);
}

void CppLogic::Mod::AdvancedFoundationBehavior::operator delete(void* p)
{
	shok::Free(p);
}

void CppLogic::Mod::AdvancedFoundationBehavior::AddHandlers(shok::EntityId id)
{
	EntityId = id;
	auto* e = EGL::CGLEEntity::GetEntityByID(id);
	e->CreateEventHandler<shok::EventIDs::Building_OnConstructionComplete>(this, &AdvancedFoundationBehavior::EventComplete);
	e->CreateEventHandler<shok::EventIDs::Building_OnUpgradeCancel>(this, &AdvancedFoundationBehavior::EventComplete);
	e->CreateEventHandler<shok::EventIDs::Building_OnUpgradeStart>(this, &AdvancedFoundationBehavior::EventStartUpgrade);
	e->CreateEventHandler<shok::EventIDs::Foundation_OnAutoCannonDetach>(this, &AdvancedFoundationBehavior::EventTurretDetach);
	e->CreateEventHandler<shok::EventIDs::Leader_AttackEntity>(this, &AdvancedFoundationBehavior::EventAttack);
	e->CreateEventHandler<shok::EventIDs::CppL_AdvancedFoundation_BuyTurret>(this, &AdvancedFoundationBehavior::EventBuyTurret);
}

void CppLogic::Mod::AdvancedFoundationBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void CppLogic::Mod::AdvancedFoundationBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void CppLogic::Mod::AdvancedFoundationBehavior::OnEntityUpgrade(EGL::CGLEEntity* old)
{
	auto* o = old->GetBehavior<AdvancedFoundationBehavior>();
	if (o == nullptr)
		return;
	InitData();
	for (unsigned int i = 0; i < std::min(o->Turret.size(), Turret.size()); ++i)
		Turret[i].Active = o->Turret[i].Active;
	CreateMissingTurrets();
}

bool CppLogic::Mod::AdvancedFoundationBehavior::CanBuyTurret(int i) const
{
	if (i < 0 || static_cast<size_t>(i) >= Turret.size())
		return false;
	if (Turret[i].Active)
		return false;
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto& r = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->CurrentResources;
	return r.HasResources(&static_cast<AdvancedFoundationBehaviorProps*>(PropPointer)->Turret[i].BuyCost);
}

bool CppLogic::Mod::AdvancedFoundationBehavior::IsTurretActive(int i) const
{
	if (i < 0 || static_cast<size_t>(i) >= Turret.size())
		return false;
	return Turret[i].Active;
}

void CppLogic::Mod::AdvancedFoundationBehavior::InitData()
{
	auto* p = static_cast<AdvancedFoundationBehaviorProps*>(PropPointer);
	while (Turret.size() > p->Turret.size()) {
		ClearTurret(Turret.back());
		Turret.pop_back();
	}
	if (Turret.size() != p->Turret.size())
		Turret.resize(p->Turret.size());
}

void CppLogic::Mod::AdvancedFoundationBehavior::CreateMissingTurrets()
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* p = static_cast<AdvancedFoundationBehaviorProps*>(PropPointer);
	InitData();
	for (unsigned int i = 0; i < Turret.size(); ++i) {
		if (!Turret[i].Active && p->Turret[i].BuyCost.Gold < 0.0f)
			Turret[i].Active = true;
		if (Turret[i].Active && EGL::CGLEEntity::EntityIDIsDead(Turret[i].Turret)) {
			EGL::CGLEEntityCreator c{};
			c.PlayerId = e->PlayerId;
			c.EntityType = p->Turret[i].Turret;
			c.Pos = e->Position + p->Turret[i].Offset;
			Turret[i].Turret = (*EGL::CGLEGameLogic::GlobalObj)->CreateEntity(&c);
			auto* t = EGL::CGLEEntity::GetEntityByID(Turret[i].Turret);
			e->AttachEntity(shok::AttachmentType::FOUNDATION_TOP_ENTITY, Turret[i].Turret, shok::EventIDs::Foundation_OnAutoCannonDetach, shok::EventIDs::NoDetachEvent);
			t->AttachEntity(shok::AttachmentType::TOP_ENTITY_FOUNDATION, EntityId, shok::EventIDs::AutoCannon_OnFoundationDetach, shok::EventIDs::NoDetachEvent);
		}
	}
}

void CppLogic::Mod::AdvancedFoundationBehavior::ClearTurret(TurretData& t)
{
	if (!EGL::CGLEEntity::EntityIDIsDead(t.Turret)) {
		auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
		e->DetachObservedEntity(shok::AttachmentType::FOUNDATION_TOP_ENTITY, t.Turret, false);
		e->DetachObserverEntity(shok::AttachmentType::TOP_ENTITY_FOUNDATION, t.Turret, false);
		EGL::CGLEEntity::GetEntityByID(t.Turret)->Destroy();
	}
}

void CppLogic::Mod::AdvancedFoundationBehavior::EventComplete(BB::CEvent* ev)
{
	CreateMissingTurrets();
}

void CppLogic::Mod::AdvancedFoundationBehavior::EventStartUpgrade(BB::CEvent* ev)
{
	for (auto& t : Turret)
		ClearTurret(t);
}

void CppLogic::Mod::AdvancedFoundationBehavior::EventTurretDetach(EGL::CEvent1Entity* ev)
{
	for (auto& t : Turret) {
		if (t.Turret == ev->EntityID)
			t.Turret = {};
	}
	CreateMissingTurrets();
}

void CppLogic::Mod::AdvancedFoundationBehavior::EventAttack(EGL::CEvent1Entity* ev)
{
	for (auto& t : Turret) {
		auto* e = EGL::CGLEEntity::GetEntityByID(t.Turret);
		if (e == nullptr)
			continue;
		e->FireEvent(ev);
	}
}

void CppLogic::Mod::AdvancedFoundationBehavior::EventBuyTurret(EGL::CEventValue_Int* ev)
{
	if (!CanBuyTurret(ev->Data))
		return;

	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto& r = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->CurrentResources;
	r.SubResources(static_cast<AdvancedFoundationBehaviorProps*>(PropPointer)->Turret[ev->Data].BuyCost);

	Turret[ev->Data].Active = true;
	CreateMissingTurrets();
}

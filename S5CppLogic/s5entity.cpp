#include "pch.h"
#include "s5data.h"


static inline void(__thiscall* const shok_EGL_CGLEEntityCreator_ctor)(shok_EGL_CGLEEntityCreator* th) = (void(__thiscall*)(shok_EGL_CGLEEntityCreator*))0x4493A4;
shok_EGL_CGLEEntityCreator::shok_EGL_CGLEEntityCreator(bool _)
{
	shok_EGL_CGLEEntityCreator_ctor(this);
}

shok_EGL_CGLEEntityCreator::shok_EGL_CGLEEntityCreator()
{
}

shok_GGL_CGLConstructionSiteCreator::shok_GGL_CGLConstructionSiteCreator()
{

}
static inline void(__thiscall* const shok_GGL_CResourceDoodadCreator_ctor)(shok_GGL_CResourceDoodadCreator* th) = (void(__thiscall*)(shok_GGL_CResourceDoodadCreator*)) 0x4DD7E2;
shok_GGL_CResourceDoodadCreator::shok_GGL_CResourceDoodadCreator()
{
	shok_GGL_CResourceDoodadCreator_ctor(this);
}

static inline void(__thiscall* const shok_EGL_CGLEEntityCreator_dtor)(shok_EGL_CGLEEntityCreator* th) = (void(__thiscall*)(shok_EGL_CGLEEntityCreator*))0x449E23;
shok_EGL_CGLEEntityCreator::~shok_EGL_CGLEEntityCreator()
{
	shok_EGL_CGLEEntityCreator_dtor(this);
}

shok_EGL_CGLEBehavior* shok_EGL_CGLEEntity::SearchBehavior(int* vts, int num)
{
	for (shok_EGL_CGLEBehavior* b : Behaviours) {
		if (b != nullptr && contains(vts, b->vtable, num))
			return b;
	}
	return nullptr;
}

shok_EGL_CGLEBehavior* shok_EGL_CGLEEntity::SearchBehavior(int vt) {
	return SearchBehavior(&vt, 1);
}

shok_GGL_CSoldierBehavior* shok_EGL_CGLEEntity::GetSoldierBehavior()
{
	return (shok_GGL_CSoldierBehavior*)SearchBehavior(shok_GGL_CSoldierBehavior::vtp);
}

shok_GGL_CLeaderBehavior* shok_EGL_CGLEEntity::GetLeaderBehavior()
{
	int data[2] = { shok_GGL_CLeaderBehavior::vtp, shok_GGL_CSoldierBehavior::vtp };
	return (shok_GGL_CLeaderBehavior*)SearchBehavior(data, 2);
}

shok_GGL_CBehaviorDefaultMovement* shok_EGL_CGLEEntity::GetMovementBehavior()
{
	int data[4] = { shok_GGL_CBehaviorDefaultMovement::vtp, shok_GGL_CLeaderMovement::vtp, shok_GGL_CSoldierMovement::vtp, shok_GGL_CSettlerMovement::vtp };
	return (shok_GGL_CBehaviorDefaultMovement*)SearchBehavior(data, 4);
}

shok_GGL_CBarrackBehavior* shok_EGL_CGLEEntity::GetBarrackBehavior()
{
	return (shok_GGL_CBarrackBehavior*)SearchBehavior(shok_GGL_CBarrackBehavior::vtp);
}

shok_GGL_CCamouflageBehavior* shok_EGL_CGLEEntity::GetCamoAbilityBehavior()
{
	int data[2] = { shok_GGL_CCamouflageBehavior::vtp, shok_GGL_CThiefCamouflageBehavior::vtp };
	return (shok_GGL_CCamouflageBehavior*)SearchBehavior(data, 2);
}

shok_GGL_CThiefCamouflageBehavior* shok_EGL_CGLEEntity::GetCamoThiefBehavior()
{
	return (shok_GGL_CThiefCamouflageBehavior*)SearchBehavior(shok_GGL_CThiefCamouflageBehavior::vtp);
}

shok_GGL_CHeroBehavior* shok_EGL_CGLEEntity::GetHeroBehavior()
{
	return (shok_GGL_CHeroBehavior*)SearchBehavior(shok_GGL_CHeroBehavior::vtp);
}

shok_GGL_CLimitedLifespanBehavior* shok_EGL_CGLEEntity::GetLimitedLifespanBehavior()
{
	return (shok_GGL_CLimitedLifespanBehavior*)SearchBehavior(shok_GGL_CLimitedLifespanBehavior::vtp);
}

shok_GGL_CWorkerBehavior* shok_EGL_CGLEEntity::GetWorkerBehavior()
{
	return (shok_GGL_CWorkerBehavior*)SearchBehavior(shok_GGL_CWorkerBehavior::vtp);
}

shok_GGL_CMarketBehavior* shok_EGL_CGLEEntity::GetMarketBehavior()
{
	return (shok_GGL_CMarketBehavior*)SearchBehavior(shok_GGL_CMarketBehavior::vtp);
}

shok_GGL_CThiefBehavior* shok_EGL_CGLEEntity::GetThiefBehavior()
{
	return (shok_GGL_CThiefBehavior*)SearchBehavior(shok_GGL_CThiefBehavior::vtp);
}

shok_GGL_CBattleBehavior* shok_EGL_CGLEEntity::GetBattleBehavior()
{
	int v[5] = { shok_GGL_CBattleBehavior::vtp, shok_GGL_CLeaderBehavior::vtp, shok_GGL_CSoldierBehavior::vtp, shok_GGL_CSerfBattleBehavior::vtp, shok_GGL_CBattleSerfBehavior::vtp };
	return (shok_GGL_CBattleBehavior*)SearchBehavior(v, 5);
}

shok_GGL_CHeroHawkBehavior* shok_EGL_CGLEEntity::GetHeroHawkBehavior()
{
	return (shok_GGL_CHeroHawkBehavior*)SearchBehavior(shok_GGL_CHeroHawkBehavior::vtp);
}

shok_GGL_CInflictFearAbility* shok_EGL_CGLEEntity::GetInflictFearBehavior()
{
	return (shok_GGL_CInflictFearAbility*)SearchBehavior(shok_GGL_CInflictFearAbility::vtp);
}

shok_GGL_CBombPlacerBehavior* shok_EGL_CGLEEntity::GetBombPlacerBehavior()
{
	return (shok_GGL_CBombPlacerBehavior*)SearchBehavior(shok_GGL_CBombPlacerBehavior::vtp);
}

shok_GGL_CCannonBuilderBehavior* shok_EGL_CGLEEntity::GetCannonBuilderBehavior()
{
	return (shok_GGL_CCannonBuilderBehavior*)SearchBehavior(shok_GGL_CCannonBuilderBehavior::vtp);
}

shok_GGL_CRangedEffectAbility* shok_EGL_CGLEEntity::GetRangedEffectBehavior()
{
	return (shok_GGL_CRangedEffectAbility*)SearchBehavior(shok_GGL_CRangedEffectAbility::vtp);
}

shok_GGL_CCircularAttack* shok_EGL_CGLEEntity::GetCircularAttackBehavior()
{
	return (shok_GGL_CCircularAttack*)SearchBehavior(shok_GGL_CCircularAttack::vtp);
}

shok_GGL_CSummonBehavior* shok_EGL_CGLEEntity::GetSummonBehavior()
{
	return (shok_GGL_CSummonBehavior*)SearchBehavior(shok_GGL_CSummonBehavior::vtp);
}

shok_GGL_CConvertSettlerAbility* shok_EGL_CGLEEntity::GetConvertSettlerBehavior()
{
	return (shok_GGL_CConvertSettlerAbility*)SearchBehavior(shok_GGL_CConvertSettlerAbility::vtp);
}

shok_GGL_CSniperAbility* shok_EGL_CGLEEntity::GetSniperBehavior()
{
	return (shok_GGL_CSniperAbility*)SearchBehavior(shok_GGL_CSniperAbility::vtp);
}

shok_GGL_CShurikenAbility* shok_EGL_CGLEEntity::GetShurikenBehavior()
{
	return (shok_GGL_CShurikenAbility*)SearchBehavior(shok_GGL_CShurikenAbility::vtp);
}

shok_GGL_CKegPlacerBehavior* shok_EGL_CGLEEntity::GetKegPlacerBehavior()
{
	return (shok_GGL_CKegPlacerBehavior*)SearchBehavior(shok_GGL_CKegPlacerBehavior::vtp);
}

shok_GGL_CAbilityScoutBinocular* shok_EGL_CGLEEntity::GetBinocularBehavior()
{
	return (shok_GGL_CAbilityScoutBinocular*)SearchBehavior(shok_GGL_CAbilityScoutBinocular::vtp);
}

shok_GGL_CTorchPlacerBehavior* shok_EGL_CGLEEntity::GetTorchPlacerBehavior()
{
	return (shok_GGL_CTorchPlacerBehavior*)SearchBehavior(shok_GGL_CTorchPlacerBehavior::vtp);
}

shok_GGL_CPointToResourceBehavior* shok_EGL_CGLEEntity::GetPointToResBehavior()
{
	return (shok_GGL_CPointToResourceBehavior*)SearchBehavior(shok_GGL_CPointToResourceBehavior::vtp);
}

shok_GGL_CKegBehavior* shok_EGL_CGLEEntity::GetKegBehavior()
{
	return (shok_GGL_CKegBehavior*)SearchBehavior(shok_GGL_CKegBehavior::vtp);
}

shok_GGL_CFoundryBehavior* shok_EGL_CGLEEntity::GetFoundryBehavior()
{
	return (shok_GGL_CFoundryBehavior*)SearchBehavior(shok_GGL_CFoundryBehavior::vtp);
}

shok_GGL_CBattleSerfBehavior* shok_EGL_CGLEEntity::GetBattleSerfBehavior()
{
	return (shok_GGL_CBattleSerfBehavior*)SearchBehavior(shok_GGL_CBattleSerfBehavior::vtp);
}

shok_GGL_CSerfBehavior* shok_EGL_CGLEEntity::GetSerfBehavior()
{
	return (shok_GGL_CSerfBehavior*)SearchBehavior(shok_GGL_CSerfBehavior::vtp);
}

shok_GGL_CLimitedAttachmentBehavior* shok_EGL_CGLEEntity::GetLimitedAttachmentBehavior()
{
	return (shok_GGL_CLimitedAttachmentBehavior*)SearchBehavior(shok_GGL_CLimitedAttachmentBehavior::vtp);
}

shok_GGL_CBuildingMerchantBehavior* shok_EGL_CGLEEntity::GetBuildingMerchantBehavior()
{
	int vts[3] = { shok_GGL_CBuildingMercenaryBehavior::vtp, shok_GGL_CBuildingTechTraderBehavior::vtp, shok_GGL_CBuildingMerchantBehavior::vtp };
	return (shok_GGL_CBuildingMerchantBehavior*)SearchBehavior(vts, 3);
}

shok_GGL_CBuildingMercenaryBehavior* shok_EGL_CGLEEntity::GetMercenaryBehavior()
{
	return (shok_GGL_CBuildingMercenaryBehavior*)SearchBehavior(shok_GGL_CBuildingMercenaryBehavior::vtp);
}

static inline bool(__thiscall* const shok_IsEntityInCategory)(shok_EGL_CGLEEntity* e, int cat) = (bool(__thiscall*)(shok_EGL_CGLEEntity * e, int cat)) 0x57BBEB;
bool shok_EGL_CGLEEntity::IsEntityInCategory(int cat)
{
	return shok_IsEntityInCategory(this, cat);
}

bool shok_EGL_CGLEEntity::IsMovingEntity()
{
	int data[4] = { shok_EGL_CMovingEntity::vtp, shok_GGL_CEvadingEntity::vtp, shok_GGL_CSettler::vtp, shok_GGL_CAnimal::vtp };
	return contains(data, vtable, 4);
}

static inline int(_cdecl* const shok_EntityIsSettler)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4A2B62;
bool shok_EGL_CGLEEntity::IsSettler()
{
	return shok_EntityIsSettler(this);
}

static inline int(_cdecl* const shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x449432;
bool shok_EGL_CGLEEntity::IsBuilding()
{
	return shok_EntityIsBuilding(this);
}

static inline int(_cdecl* const shok_EntityIsResourceDoodad)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4B82C7;
bool shok_EGL_CGLEEntity::IsResourceDoodad()
{
	return shok_EntityIsResourceDoodad(this);
}

int shok_EGL_CGLEEntity::GetResourceProvided()
{
	return shok_EntityGetProvidedResourceByID(this->EntityId);
}

shok_GGlue_CGlueEntityProps* shok_EGL_CGLEEntity::GetEntityType()
{
	return (*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(EntityType);
}

int shok_EGL_CGLEEntity::EventGetIntById(int id)
{
	shok_event_data_EGL_CEventGetValue_int_1211121895 d = shok_event_data_EGL_CEventGetValue_int_1211121895();
	d.id = id;
	shok_vtable_EGL_CGLEEntity* vt = (shok_vtable_EGL_CGLEEntity*)vtable;
	vt->FireEvent(this, &d);
	return d.result;
}

int shok_EGL_CGLEEntity::EventGetDamage()
{
	return EventGetIntById(0x1503D);
}
int shok_EGL_CGLEEntity::EventGetArmor()
{
	return EventGetIntById(0x1503E);
}
int shok_EGL_CGLEEntity::EventGetMaxWorktime()
{
	return EventGetIntById(0x1301A);
}

int shok_EGL_CGLEEntity::EventLeaderGetCurrentCommand()
{
	if (GetLeaderBehavior() == nullptr)
		return -1;
	return EventGetIntById(0x1502D);
}

float shok_EGL_CGLEEntity::GetExploration()
{
	shok_vtable_EGL_CGLEEntity* vt = (shok_vtable_EGL_CGLEEntity*)vtable;
	return vt->GetExploration(this);
}

static inline int(__thiscall* const entitygetmaxhealth)(shok_EGL_CGLEEntity* th) = (int(__thiscall*)(shok_EGL_CGLEEntity*)) 0x57B798;
int shok_EGL_CGLEEntity::GetMaxHealth()
{
	return entitygetmaxhealth(this);
}

int shok_EGL_CGLEEntity::LimitedAttachmentGetMaximum(int attachType)
{
	shok_event_data_GGL_CEventAttachmentTypeGetInteger ev = shok_event_data_GGL_CEventAttachmentTypeGetInteger();
	ev.id = 0x1A007;
	ev.AttachmentType = attachType;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return ev.i;
}


int shok_EGL_CGLEEntity::GetFirstAttachedToMe(int attachmentId)
{
	shok_attachment* r = ObserverEntities.GetFirstMatch([attachmentId](shok_attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}

int shok_EGL_CGLEEntity::GetFirstAttachedEntity(int attachmentId)
{
	shok_attachment* r = ObservedEntities.GetFirstMatch([attachmentId](shok_attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}

int shok_EGL_CMovingEntity::LeaderGetNearbyBarracks()
{
	return EventGetIntById(0x15036);
}

bool shok_EGL_CMovingEntity::IsMoving()
{
	shok_event_data_EGL_CEventValue_bool_703333479 d = shok_event_data_EGL_CEventValue_bool_703333479();
	d.id = 0x20009;
	d.b = false;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &d);
	return d.b;
}

bool shok_GGL_CSettler::IsIdle()
{
	int com = EventLeaderGetCurrentCommand();
	return com == shok_LeaderCommand::Defend || com == shok_LeaderCommand::HoldPos || CurrentTaskListID == ((shok_GGL_CGLSettlerProps*)GetEntityType())->IdleTaskList;
}

int shok_GGL_CBuilding::GetConstructionSite()
{
	shok_attachment* a = ObserverEntities.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == shok_AttachmentType::CONSTRUCTION_SITE_BUILDING; });
	if (a)
		return a->EntityId;
	return 0;
}

static inline int(__thiscall* const shok_build_getnearestfreeslot)(shok_EGL_CGLEEntity* th, shok_position* p) = (int(__thiscall*) (shok_EGL_CGLEEntity*, shok_position*)) 0x4AB02D;
int shok_GGL_CBuilding::GetNearestFreeConstructionSlotFor(shok_position* p)
{
	int cid = GetConstructionSite();
	if (!cid)
		return -1;
	shok_EGL_CGLEEntity* consi = shok_eid2obj(cid);
	if (!consi)
		return -1;
	return shok_build_getnearestfreeslot(consi, p);
}

int shok_GGL_CBuilding::GetNearestFreeRepairSlotFor(shok_position* p)
{
	return shok_build_getnearestfreeslot(this, p);
}

bool shok_GGL_CBuilding::IsConstructionFinished()
{
	return BuildingHeight >= 1;
}

bool shok_GGL_CBuilding::IsIdle()
{
	if (!IsConstructionFinished())
		return false;
	if (IsUpgrading)
		return false;
	if (CurrentState == 0x13) // alarm mode
		return false;
	if (GetTechnologyInResearch())
		return false;
	{
		shok_GGL_CFoundryBehavior* f = GetFoundryBehavior();
		if (f && (f->CannonType != 0 || GetCannonProgress() != 100))
			return false;
	}
	if (GetMarketBehavior() && GetMarketProgress() < 1.0f)
		return false;
	if (GetBarrackBehavior()) {
		if (ObserverEntities.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == shok_AttachmentType::FIGHTER_BARRACKS; }))
			return false;
	}
	return true;
}

static inline int(__thiscall* building_gettechinres)(shok_GGL_CBuilding* th) = (int(__thiscall*)(shok_GGL_CBuilding*))0x4AAD09;
int shok_GGL_CBuilding::GetTechnologyInResearch()
{
	return building_gettechinres(this);
}

int shok_GGL_CBuilding::GetCannonProgress()
{
	return EventGetIntById(0x17014);
}

float shok_GGL_CBuilding::GetMarketProgress()
{
	shok_event_data_EGL_CEventGetValue_float_1468983543 ev = shok_event_data_EGL_CEventGetValue_float_1468983543();
	ev.id = 0x17009;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return ev.f;
}

static inline void(__thiscall* const entitysethealth)(shok_EGL_CGLEEntity* th, int h) = (void(__thiscall* const)(shok_EGL_CGLEEntity *, int)) 0x57A6D9;
void shok_EGL_CGLEEntity::SetHealth(int h)
{
	entitysethealth(this, h);
}

void shok_EGL_CGLEEntity::Destroy()
{
	((shok_vtable_EGL_CGLEEntity*)vtable)->Destroy(this, 0);
}

void shok_EGL_CGLEEntity::ClearAttackers()
{
	std::vector<shok_GGL_CSettler*> tomove = std::vector<shok_GGL_CSettler*>();
	ObserverEntities.ForAll([&tomove](shok_attachment* a) {
		if (a->AttachmentType == shok_AttachmentType::ATTACKER_COMMAND_TARGET || a->AttachmentType == shok_AttachmentType::LEADER_TARGET || a->AttachmentType == shok_AttachmentType::FOLLOWER_FOLLOWED) {
			shok_EGL_CGLEEntity* at = shok_eid2obj(a->EntityId);
			if (at->IsSettler()) {
				shok_GGL_CSettler* s = (shok_GGL_CSettler*)at;
				tomove.emplace_back(s);
			}
		}
		});
	for (shok_GGL_CSettler* s : tomove)
		s->Defend();
}



void shok_EGL_CMovingEntity::AttackMove(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x1502E; // attack move
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	//shok_event_data_EGL_CEventValue_bool_703333479 e2 = shok_event_data_EGL_CEventValue_bool_703333479();
	//e2.id = 0x1503B; // set target rotation state
	//e2.b = 0;
	//((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &e2);
	TargetRotationValid = 0;
}

void shok_EGL_CMovingEntity::AttackEntity(int targetId)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x15004;
	ev.entityId = targetId;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::Move(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x11002; // move
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	TargetRotationValid = 0;
}

void shok_EGL_CMovingEntity::HoldPosition()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x1502F;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::Defend()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x15032;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::LeaderAttachSoldier(int soldierId)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x15009;
	ev.entityId = soldierId;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x15017;
	shok_EGL_CGLEEntity* sol = shok_eid2obj(soldierId);
	((shok_vtable_EGL_CGLEEntity*)sol->vtable)->FireEvent(sol, &e2);
}

static inline void(__thiscall* const shok_entity_expellSettler)(shok_EGL_CGLEEntity* th, int i) = (void(__thiscall*)(shok_EGL_CGLEEntity*, int))0x4A39BB;
void shok_EGL_CMovingEntity::SettlerExpell()
{
	if (GetLeaderBehavior() && ObservedEntities.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER; })) {
		shok_event_data_EGL_CEventValue_bool_703333479 ev = shok_event_data_EGL_CEventValue_bool_703333479();
		ev.id = 0x15046;
		ev.b = true;
		((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	}
	else {
		//((shok_vtable_EGL_CGLEEntity*)vtable)->Destroy(this, 1);
		shok_entity_expellSettler(this, 1);
	}
}

void shok_EGL_CMovingEntity::HeroAbilitySendHawk(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x16002;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityInflictFear()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x16026;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceBomb(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x15033;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceCannon(shok_position& p, int FoundationType, int CannonType)
{
	shok_event_data_GGL_CEventPositionAnd2EntityTypes ev = shok_event_data_GGL_CEventPositionAnd2EntityTypes();
	ev.id = 0x1600A;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	ev.EntityType1 = FoundationType;
	ev.EntityType2 = CannonType;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityRangedEffect()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x1601C;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityCircularAttack()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x16022;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilitySummon()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x1601A;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityConvert(int target)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x16027;
	ev.entityId = target;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilitySnipe(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x1602D;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityShuriken(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x1602F;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityActivateCamoflage()
{
	shok_event_data_BB_CEvent e = shok_event_data_BB_CEvent();
	e.id = 0x16015;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &e);
}

void shok_EGL_CMovingEntity::ThiefSabotage(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10107;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefDefuse(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x1010E;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefStealFrom(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10102;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefSecureGoods(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10103;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutBinoculars(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x11102;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutFindResource()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x11104;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutPlaceTorch(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x11103;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

bool shok_EGL_CMovingEntity::SerfConstructBuilding(shok_GGL_CBuilding* build)
{
	int cid = build->GetNearestFreeConstructionSlotFor(&this->Position);
	if (cid < 0)
		return false;
	shok_event_data_GGL_CEventEntityIndex ev = shok_event_data_GGL_CEventEntityIndex();
	ev.id = 0x14003;
	ev.entity = build->GetConstructionSite();
	ev.index = cid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return true;
}

bool shok_EGL_CMovingEntity::SerfRepairBuilding(shok_GGL_CBuilding* build)
{
	int cid = build->GetNearestFreeRepairSlotFor(&this->Position);
	if (cid < 0)
		return false;
	shok_event_data_GGL_CEventEntityIndex ev = shok_event_data_GGL_CEventEntityIndex();
	ev.id = 0x14003; // repair and build are the same event, just different target entity
	ev.entity = build->EntityId;
	ev.index = cid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return true;
}

void shok_EGL_CMovingEntity::SerfExtractResource(int id)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10009;
	ev.entityId = id;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::SerfTurnToBattleSerf()
{
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x14005;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

void shok_EGL_CMovingEntity::BattleSerfTurnToSerf()
{
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x14006;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

void(__thiscall* const movingentity_setpos)(shok_EGL_CMovingEntity* th, shok_position* p) = (void(__thiscall*)(shok_EGL_CMovingEntity*, shok_position*))0x57BC78;
void shok_EGL_CMovingEntity::SetPosition(shok_position& p)
{
	movingentity_setpos(this, &p);
	Move(p);
}

static inline void(__thiscall* const building_startUpgrade)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*)) 0x4AF1B5;
void shok_GGL_CBuilding::StartUpgrade()
{
	building_startUpgrade(this);
}

static inline void(__thiscall* const buildingCancelUpgrade)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*))0x4AF228;
void shok_GGL_CBuilding::CancelUpgrade()
{
	buildingCancelUpgrade(this);
}

void shok_GGL_CBuilding::CommandBuildCannon(int entitytype)
{
	shok_event_data_EGL_CEventValue_int_27574121 e = shok_event_data_EGL_CEventValue_int_27574121();
	e.id = 0x17016;
	e.i = entitytype;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &e);
}

void shok_GGL_CBuilding::CommandRecruitSoldierForLeader(int id)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x15037;
	ev.entityId = id;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

static inline void(__thiscall* const shok_buildingActivateOvertime)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*)) 0x4AE566;
void shok_GGL_CBuilding::ActivateOvertime()
{
	shok_buildingActivateOvertime(this);
}
static inline void(__thiscall* const shok_buildingDeactivateOvertime)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*)) 0x4AA073;
void shok_GGL_CBuilding::DeactivateOvertime()
{
	shok_buildingDeactivateOvertime(this);
}

void shok_GGL_CBuilding::BarracksRecruitGroups()
{
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x1701E;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::BarracksRecruitLeaders()
{
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x1701F;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::HQBuySerf()
{
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x14004;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::SellBuilding()
{
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x2000A;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

static inline void(__thiscall* const building_startresearch)(shok_GGL_CBuilding* th, int tech) = (void(__thiscall*)(shok_GGL_CBuilding*, int))0x4AAC76;
void shok_GGL_CBuilding::StartResearch(int tech)
{
	building_startresearch(this, tech);
}

static inline void(__thiscall* const building_cancelresearch)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*))0x4AACC0;
void shok_GGL_CBuilding::CancelResearch()
{
	building_cancelresearch(this);
}

void shok_GGL_CBuilding::MarketStartTrade(int ResourceTypeSell, int ResourceTypeBuy, float BuyAmount)
{
	shok_event_data_GGL_CEventTransaction e2 = shok_event_data_GGL_CEventTransaction();
	e2.SellType = ResourceTypeSell;
	e2.BuyType = ResourceTypeBuy;
	e2.BuyAmount = BuyAmount;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::MarketCancelTrade()
{
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x1700B;
	((shok_vtable_EGL_CGLEEntity*)this->vtable)->FireEvent(this, &e2);
}

int __cdecl fixedChangePlayer(int id, int pl) {
	shok_EGL_CGLEEntity* e = shok_eid2obj(id);
	if (!e)
		return 0;
	if (e->PlayerId == pl)
		return id;
	shok_EGL_CGLEEntityCreator c = shok_EGL_CGLEEntityCreator(true);
	c.EntityType = e->EntityType;
	c.PlayerId = pl;
	c.Pos = e->Position;
	c.Scale = e->Scale;
	c.Health = e->Health;
	if (e->ScriptName) {
		size_t len = strlen(e->ScriptName) + 1;
		c.ScriptName = (char*)shok_malloc(sizeof(char) * len);
		strcpy_s(c.ScriptName, len, e->ScriptName);
	}
	else {
		c.ScriptName = nullptr;
	}
	int nid = (*shok_EGL_CGLEGameLogicObject)->CreateEntity(&c);
	if (e->IsSettler()) {
		shok_GGL_CLeaderBehavior* lb = e->GetLeaderBehavior();
		if (lb) {
			std::vector<int> sol = std::vector<int>();
			e->ObservedEntities.ForAll([&sol](shok_attachment* a) { if (a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER) sol.push_back(a->EntityId); });
			shok_GGL_CSettler* settler = (shok_GGL_CSettler*)shok_eid2obj(nid);
			for (int i : sol) {
				settler->LeaderAttachSoldier(fixedChangePlayer(i, pl));
			}
			shok_GGL_CLeaderBehavior* nlb = settler->GetLeaderBehavior();
			nlb->Experience = lb->Experience;
			nlb->TroopHealthCurrent = lb->TroopHealthCurrent;
			nlb->TroopHealthPerSoldier = lb->TroopHealthPerSoldier;
		}
	}
	e->Destroy();
	return nid;
}
void ActivateEntityChangePlayerFix()
{
	WriteJump(shok_entityChangePlayer, &fixedChangePlayer);
}

void (*Hero6ConvertHookCb)(int id, int pl, int nid, int converter) = nullptr;
int _cdecl hero6convertchangeplayer(int id, int pl) {
	shok_EGL_CGLEEntity* c = (shok_EGL_CGLEEntity*)1;
	_asm { mov c, esi }
	int r = shok_entityChangePlayer(id, pl);
	if (Hero6ConvertHookCb)
		Hero6ConvertHookCb(id, pl, r, c->EntityId);
	return r;
}
void HookHero6Convert()
{
	ActivateEntityChangePlayerFix();
	RedirectCall((void*)0x4FCD26, &hero6convertchangeplayer);
}

int ResetCamoIgnoreIfNotEntity = 0;
void __fastcall camo_behaviorReset(shok_GGL_CCamouflageBehavior* th, int _, int a) {
	if (ResetCamoIgnoreIfNotEntity == 0 || th->EntityId == ResetCamoIgnoreIfNotEntity)
		th->InvisibilityRemaining = 0;
}
void HookResetCamo() {
	WriteJump((void*)0x5011DF, &camo_behaviorReset);
}

static inline int(__thiscall* const activateCamoOrig)(shok_GGL_CCamouflageBehavior* th) = (int(__thiscall*)(shok_GGL_CCamouflageBehavior*)) 0x501561;
void (*CamoActivateCb)(shok_GGL_CCamouflageBehavior* th);
int __fastcall camoActivateHook(shok_GGL_CCamouflageBehavior* th) {
	int i = activateCamoOrig(th);
	if (CamoActivateCb)
		CamoActivateCb(th);
	return i;
}
void HookCamoActivate()
{
	RedirectCall((void*)0x4D51A4, &camoActivateHook);
	RedirectCall((void*)0x50163A, &camoActivateHook);
}


static inline void(__thiscall* const event2entitiesctor)(int* e, int id, int at, int de) = (void(__thiscall*)(int*, int, int, int))0x49847F;
int* HurtEntityDamagePointer = nullptr;
void __stdcall hurtentityhookc(int* damage, shok_EGL_CGLEEntity** target, shok_EGL_CGLEEntity** attacker) { // argument order is reversed, for asm reasons
	if (!*target)
		return;
	int aid = 0;
	if (*attacker)
		aid = (*attacker)->EntityId;
	int tid = (*target)->EntityId;
	int ev[6];
	event2entitiesctor(ev, 0x1C007, aid, tid);
	HurtEntityDamagePointer = damage;
	(*shok_EScr_CScriptTriggerSystemObj)->RunTrigger((shok_BB_CEvent*)ev);
	HurtEntityDamagePointer = nullptr;
}

void __declspec(naked) hurtentityhook() { // push arguments, call func,do the move i did override, last jump back
	__asm {
		mov eax, esp
		add eax, 0x4
		push eax
		add eax, 0x4
		push eax
		add eax, 0x4
		push eax
		call hurtentityhookc

		mov eax, 0x72d687
		push 0x49f35d
		ret
	}
}

void HookHurtEntity()
{
	if (HasSCELoader())
		DEBUGGER_BREAK
	WriteJump((void*)0x49f358, &hurtentityhook); // call my own func, that calls the trigger
	byte* p = (byte*)0x49F4A6; // remove call to trigger
	*p = 0x90; // nop
	p = (byte*)0x49F4A7;
	*p = 0x90;
	p = (byte*)0x49F4AC;
	*p = 0x90;
	p = (byte*)0x49F4AD;
	*p = 0x90;
}

std::multimap<int, int> BuildingMaxHpBoni = std::multimap<int, int>();
int __fastcall hookGetMaxHP(shok_EGL_CGLEEntity* e) {
	float hp;
	entityAddonData* d = e->GetAdditionalData(false);
	shok_GGlue_CGlueEntityProps* et = e->GetEntityType();
	if (d && d->HealthOverride > 0) {
		hp = (float) d->HealthOverride;
	}
	else {
		hp = (float) et->LogicProps->MaxHealth;
	}
	if (d && !d->HealthUseBoni)
		return (int)hp;
	if (e->IsSettler()) {
		for (int t : ((shok_GGL_CGLSettlerProps*)et->LogicProps)->ModifyHitpoints.TechList) {
			if ((*shok_GGL_CGLGameLogicObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != TechState::Researched)
				continue;
			shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(t);
			hp = tech->HitpointModifier.ModifyValue(hp);
		}
	}
	else if (e->IsBuilding()) {
		std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> it = BuildingMaxHpBoni.equal_range(e->EntityType);
		for (std::multimap<int, int>::iterator i = it.first; i != it.second; ++i) {
			int t = i->second;
			if ((*shok_GGL_CGLGameLogicObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != TechState::Researched)
				continue;
			shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(t);
			hp = tech->HitpointModifier.ModifyValue(hp);
		}
	}
	return (int) hp;
}
void __declspec(naked) hookgetmaxhpui() {
	__asm {
		mov ecx, [ebp+0xC]
		call hookGetMaxHP
		push eax
		fild [esp]
		pop eax
		push 0x4BDEE0
		ret
	}
}
void __fastcall createentityfixhp(shok_EGL_CGLEEntity* th, int hpIn) {
	if (hpIn != -1)
		return;
	if (th->IsSettler()) {
		th->Health = th->GetMaxHealth();
	}
	else if (th->IsBuilding()) {
		th->Health = th->GetMaxHealth();
		if (!((shok_GGL_CBuilding*)th)->IsConstructionFinished()) {
			th->Health = (int)(th->Health * (*shok_GGL_CLogicPropertiesObj)->ConstructionSiteHealthFactor);
		}
	}
}
void __declspec(naked) hookcreatentityfixhp() {
	__asm {
		mov ecx, esi
		pop edx // creator, param for eax+0x14
		mov eax, [edx+7*4] // health
		push eax // push health, then old param
		push edx
		mov eax, [ecx]
		call [eax+0x14] // call
		mov ecx, esi // get params
		pop edx
		call createentityfixhp // hp fix
		push 0x571B98
		ret
	}
}
void EnableMaxHealthTechBoni()
{
	if (HasSCELoader())
		DEBUGGER_BREAK
	WriteJump((void*)0x57B798, &hookGetMaxHP);
	WriteJump((void*)0x4BDED8, &hookgetmaxhpui);
	WriteJump((void*)0x571B93, &hookcreatentityfixhp);
}

std::map<int, entityAddonData> AddonDataMap = std::map<int, entityAddonData>();
entityAddonData LastRemovedEntityAddonData = entityAddonData();
void __fastcall destroyentity_gemoveadd(shok_EGL_CGLEEntity* e) {
	auto a = AddonDataMap.find(e->EntityId);
	if (a != AddonDataMap.end()) {
		LastRemovedEntityAddonData = a->second;
		AddonDataMap.erase(a);
	}
}
void __declspec(naked) destroyentityhook() {
	__asm {
		push ecx
		call destroyentity_gemoveadd
		pop ecx

		push esi
		mov esi, ecx
		cmp byte ptr [esi+0x6D], 0
		jnz jump
		push 0x57C957
		ret
	jump:
		push 0x57C9DE
		ret
	}

}
void HookDestroyEntity()
{
	WriteJump((void*)0x57C94A, &destroyentityhook);
}
entityAddonData* shok_EGL_CGLEEntity::GetAdditionalData(bool create)
{
	auto a = AddonDataMap.find(EntityId);
	if (a != AddonDataMap.end()) {
		return &a->second;
	}
	if (!create) {
		return nullptr;
	}
	else {
		AddonDataMap[EntityId] = entityAddonData();
		entityAddonData* r = &AddonDataMap[EntityId];
		r->EntityId = EntityId;
		return r;
	}
}
void shok_EGL_CGLEEntity::CloneAdditionalDataFrom(entityAddonData* other)
{
	if (other) {
		entityAddonData* t = GetAdditionalData(true);
		t->HealthOverride = other->HealthOverride;
		t->HealthUseBoni = other->HealthUseBoni;
		t->DamageOverride = other->DamageOverride;
		t->ArmorOverride = other->ArmorOverride;
		t->ExplorationOverride = other->ExplorationOverride;
	}
}

float __fastcall entitygetdamagemod(shok_GGL_CBattleBehavior* b) {
	shok_EGL_CGLEEntity* e = shok_eid2obj(b->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		return (float)d->DamageOverride;
	else
		return (float)e->GetEntityType()->GetBattleBehaviorProps()->DamageAmount;
}
void __declspec(naked) entitydamagemodeventbattleasm() {
	__asm {
		sub esp, 0x24
		push ebx
		push esi
		mov esi, ecx

		call entitygetdamagemod
		//mov eax, [esi+0x30]
		// flid [eax+0x38]

		push esi
		push 0x50C793
		ret
	}
}
void __declspec(naked) entitydamagemodbattlemelleonhitasm() {
	__asm {
		pushad

		mov ecx, esi
		call entitygetdamagemod

		popad

		push 0x50C23B
		ret
	}
}
shok_event_data_EGL_CEventGetValue_int_1211121895* __fastcall entitydamagemodeventautocannonasm(shok_GGL_CAutoCannonBehavior* th, int, shok_event_data_EGL_CEventGetValue_int_1211121895* ev) {
	shok_EGL_CGLEEntity* e = shok_eid2obj(th->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		ev->result = d->DamageOverride;
	else
		ev->result = e->GetEntityType()->GetAutoCannonProps()->DamageAmount;
	return ev;
}
void EnableEntityDamageMod() {
	WriteJump((void*)0x50C785, &entitydamagemodeventbattleasm);
	WriteJump((void*)0x50C235, &entitydamagemodbattlemelleonhitasm);
	WriteJump((void*)0x50F5ED, &entitydamagemodeventautocannonasm);
}

int __fastcall entityarmormod(shok_EGL_CGLEEntity* e) {
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->ArmorOverride >= 0)
		return d->ArmorOverride;
	shok_GGlue_CGlueEntityProps* p = e->GetEntityType();
	if (p->IsSettlerType())
		return ((shok_GGL_CGLSettlerProps*)p->LogicProps)->ArmorAmount;
	else if (p->IsBuildingType())
		return ((shok_GGL_CGLBuildingProps*)p->LogicProps)->ArmorAmount;
	return 0;
}
void __declspec(naked) entityarmormodsettlerasm() {
	__asm {
		mov esi, ecx
		push [esi+0x10]

		call entityarmormod

		push 0x4A6B25
		ret
	}
}
void __declspec(naked) entityarmormodbuildingrasm() {
	__asm {
		mov esi, ecx
		push[esi + 0x10]

		call entityarmormod
		push eax
		fild [esp]
		pop eax

		mov eax, 0x4AAB98
		call eax

		push 0x04AB170
		ret
	}
}
void EnableEntityArmorMod()
{
	WriteJump((void*)0x4A6B15, &entityarmormodsettlerasm);
	WriteJump((void*)0x4AB160, &entityarmormodbuildingrasm);
}

float __fastcall entityexplmod(shok_EGL_CGLEEntity* e) {
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->ExplorationOverride >= 0.0f)
		return d->ExplorationOverride;
	return e->GetEntityType()->LogicProps->Exploration;
}
void __declspec(naked) entityexplmodsettasm() {
	__asm {
		push esi
		push [esi+0x10]

		call entityexplmod

		push 0x4A4AD4
		ret
	}
}
void __declspec(naked) entityexplmodbuildasm() {
	__asm {
		push esi
		push [esi + 0x10]
		mov eax, 0x4AAB98
		call eax

		push eax
		mov ecx, esi
		call entityexplmod
		pop eax

		push 0x4AB1A4
		ret
	}
}
void EnableEntityExplorationMod()
{
	WriteJump((void*)0x4A4AC3, &entityexplmodsettasm);
	WriteJump((void*)0x4AB199, &entityexplmodbuildasm);
}

int shok_GGL_CSettler::LeaderGetRegenHealth()
{
	entityAddonData* d = GetAdditionalData(false);
	if (d && d->RegenHPOverride >= 0)
		return d->RegenHPOverride;
	else
		return GetEntityType()->GetLeaderBehaviorProps()->HealingPoints;
}

bool LeaderRegenRegenerateSoldiers = false;
void __fastcall leaderregen(shok_GGL_CLeaderBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_eid2obj(th->EntityId);
	th->SecondsSinceHPRefresh = 0;
	int hp = e->Health, mhp = e->GetMaxHealth();
	if (hp <= 0)
		return;
	int r = ((shok_GGL_CSettler*)e)->LeaderGetRegenHealth();
	if (hp < mhp) {
		hp += r;
		if (hp > mhp)
			hp = mhp;
		e->SetHealth(hp);
		return;
	}
	if (LeaderRegenRegenerateSoldiers) {
		int shp = th->GetTroopHealth(), smp = th->GetTroopHealthPerSoldier();
		int numsol = 0;
		e->ObservedEntities.ForAll([&numsol](shok_attachment* a) { if (a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER) numsol++; });
		smp *= numsol;
		shp += r;
		if (shp > smp)
			shp = smp;
		th->TroopHealthCurrent = shp;
		return;
	}
}
void HookLeaderRegen()
{
	WriteJump((void*)0x4EAE92, &leaderregen);
}

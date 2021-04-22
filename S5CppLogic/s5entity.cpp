#include "pch.h"
#include "s5data.h"


static inline void(__thiscall* shok_EGL_CGLEEntityCreator_ctor)(shok_EGL_CGLEEntityCreator* th) = (void(__thiscall*)(shok_EGL_CGLEEntityCreator*))0x4493A4;
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
static inline void(__thiscall* shok_GGL_CResourceDoodadCreator_ctor)(shok_GGL_CResourceDoodadCreator* th) = (void(__thiscall*)(shok_GGL_CResourceDoodadCreator*)) 0x4DD7E2;
shok_GGL_CResourceDoodadCreator::shok_GGL_CResourceDoodadCreator()
{
	shok_GGL_CResourceDoodadCreator_ctor(this);
}

static inline void(__thiscall* shok_EGL_CGLEEntityCreator_dtor)(shok_EGL_CGLEEntityCreator* th) = (void(__thiscall*)(shok_EGL_CGLEEntityCreator*))0x449E23;
shok_EGL_CGLEEntityCreator::~shok_EGL_CGLEEntityCreator()
{
	shok_EGL_CGLEEntityCreator_dtor(this);
}

shok_EGL_CGLEBehavior* shok_EGL_CGLEEntity::SearchBehavior(void** vts, int num)
{
	for (shok_EGL_CGLEBehavior* b : BehaviorList) {
		if (b != nullptr && contains(vts, b->vtable, num))
			return b;
	}
	return nullptr;
}

shok_EGL_CGLEBehavior* shok_EGL_CGLEEntity::SearchBehavior(void* vt) {
	return SearchBehavior(&vt, 1);
}

shok_GGL_CSoldierBehavior* shok_EGL_CGLEEntity::GetSoldierBehavior()
{
	return (shok_GGL_CSoldierBehavior*)SearchBehavior(shok_vtp_GGL_CSoldierBehavior);
}

shok_GGL_CLeaderBehavior* shok_EGL_CGLEEntity::GetLeaderBehavior()
{
	void* data[2] = { shok_vtp_GGL_CLeaderBehavior, shok_vtp_GGL_CBattleSerfBehavior };
	return (shok_GGL_CLeaderBehavior*)SearchBehavior(data, 2);
}

shok_GGL_CBehaviorDefaultMovement* shok_EGL_CGLEEntity::GetMovementBehavior()
{
	void* data[4] = { shok_vtp_GGL_CBehaviorDefaultMovement, shok_vtp_GGL_CLeaderMovement, shok_vtp_GGL_CSoldierMovement, shok_vtp_GGL_CSettlerMovement };
	return (shok_GGL_CBehaviorDefaultMovement*)SearchBehavior(data, 4);
}

shok_GGL_CBarrackBehavior* shok_EGL_CGLEEntity::GetBarrackBehavior()
{
	return (shok_GGL_CBarrackBehavior*)SearchBehavior(shok_vtp_GGL_CBarrackBehavior);
}

shok_GGL_CCamouflageBehavior* shok_EGL_CGLEEntity::GetCamoAbilityBehavior()
{
	void* data[2] = { shok_vtp_GGL_CCamouflageBehavior, shok_vtp_GGL_CThiefCamouflageBehavior };
	return (shok_GGL_CCamouflageBehavior*)SearchBehavior(data, 2);
}

shok_GGL_CThiefCamouflageBehavior* shok_EGL_CGLEEntity::GetCamoThiefBehavior()
{
	return (shok_GGL_CThiefCamouflageBehavior*)SearchBehavior(shok_vtp_GGL_CThiefCamouflageBehavior);
}

shok_GGL_CHeroBehavior* shok_EGL_CGLEEntity::GetHeroBehavior()
{
	return (shok_GGL_CHeroBehavior*)SearchBehavior(shok_vtp_GGL_CHeroBehavior);
}

shok_GGL_CLimitedLifespanBehavior* shok_EGL_CGLEEntity::GetLimitedLifespanBehavior()
{
	return (shok_GGL_CLimitedLifespanBehavior*)SearchBehavior(shok_vtp_GGL_CLimitedLifespanBehavior);
}

shok_GGL_CWorkerBehavior* shok_EGL_CGLEEntity::GetWorkerBehavior()
{
	return (shok_GGL_CWorkerBehavior*)SearchBehavior(shok_vtp_GGL_CWorkerBehavior);
}

shok_GGL_CMarketBehavior* shok_EGL_CGLEEntity::GetMarketBehavior()
{
	return (shok_GGL_CMarketBehavior*)SearchBehavior(shok_vtp_GGL_CMarketBehavior);
}

shok_GGL_CThiefBehavior* shok_EGL_CGLEEntity::GetThiefBehavior()
{
	return (shok_GGL_CThiefBehavior*)SearchBehavior(shok_vtp_GGL_CThiefBehavior);
}

shok_GGL_CBattleBehavior* shok_EGL_CGLEEntity::GetBattleBehavior()
{
	void* v[5] = { shok_vtp_GGL_CBattleBehavior, shok_vtp_GGL_CLeaderBehavior, shok_vtp_GGL_CSoldierBehavior, shok_vtp_GGL_CSerfBattleBehavior, shok_vtp_GGL_CBattleSerfBehavior };
	return (shok_GGL_CBattleBehavior*)SearchBehavior(v, 5);
}

shok_GGL_CHeroHawkBehavior* shok_EGL_CGLEEntity::GetHeroHawkBehavior()
{
	return (shok_GGL_CHeroHawkBehavior*)SearchBehavior(shok_vtp_GGL_CHeroHawkBehavior);
}

shok_GGL_CInflictFearAbility* shok_EGL_CGLEEntity::GetInflictFearBehavior()
{
	return (shok_GGL_CInflictFearAbility*)SearchBehavior(shok_vtp_GGL_CInflictFearAbility);
}

shok_GGL_CBombPlacerBehavior* shok_EGL_CGLEEntity::GetBombPlacerBehavior()
{
	return (shok_GGL_CBombPlacerBehavior*)SearchBehavior(shok_vtp_GGL_CBombPlacerBehavior);
}

shok_GGL_CCannonBuilderBehavior* shok_EGL_CGLEEntity::GetCannonBuilderBehavior()
{
	return (shok_GGL_CCannonBuilderBehavior*)SearchBehavior(shok_vtp_GGL_CCannonBuilderBehavior);
}

shok_GGL_CRangedEffectAbility* shok_EGL_CGLEEntity::GetRangedEffectBehavior()
{
	return (shok_GGL_CRangedEffectAbility*)SearchBehavior(shok_vtp_GGL_CRangedEffectAbility);
}

shok_GGL_CCircularAttack* shok_EGL_CGLEEntity::GetCircularAttackBehavior()
{
	return (shok_GGL_CCircularAttack*)SearchBehavior(shok_vtp_GGL_CCircularAttack);
}

shok_GGL_CSummonBehavior* shok_EGL_CGLEEntity::GetSummonBehavior()
{
	return (shok_GGL_CSummonBehavior*)SearchBehavior(shok_vtp_GGL_CSummonBehavior);
}

shok_GGL_CConvertSettlerAbility* shok_EGL_CGLEEntity::GetConvertSettlerBehavior()
{
	return (shok_GGL_CConvertSettlerAbility*)SearchBehavior(shok_vtp_GGL_CConvertSettlerAbility);
}

shok_GGL_CSniperAbility* shok_EGL_CGLEEntity::GetSniperBehavior()
{
	return (shok_GGL_CSniperAbility*)SearchBehavior(shok_vtp_GGL_CSniperAbility);
}

shok_GGL_CShurikenAbility* shok_EGL_CGLEEntity::GetShurikenBehavior()
{
	return (shok_GGL_CShurikenAbility*)SearchBehavior(shok_vtp_GGL_CShurikenAbility);
}

shok_GGL_CKegPlacerBehavior* shok_EGL_CGLEEntity::GetKegPlacerBehavior()
{
	return (shok_GGL_CKegPlacerBehavior*)SearchBehavior(shok_vtp_GGL_CKegPlacerBehavior);
}

shok_GGL_CAbilityScoutBinocular* shok_EGL_CGLEEntity::GetBinocularBehavior()
{
	return (shok_GGL_CAbilityScoutBinocular*)SearchBehavior(shok_vtp_GGL_CAbilityScoutBinocular);
}

shok_GGL_CTorchPlacerBehavior* shok_EGL_CGLEEntity::GetTorchPlacerBehavior()
{
	return (shok_GGL_CTorchPlacerBehavior*)SearchBehavior(shok_vtp_GGL_CTorchPlacerBehavior);
}

shok_GGL_CPointToResourceBehavior* shok_EGL_CGLEEntity::GetPointToResBehavior()
{
	return (shok_GGL_CPointToResourceBehavior*)SearchBehavior(shok_vtp_GGL_CPointToResourceBehavior);
}

shok_GGL_CKegBehavior* shok_EGL_CGLEEntity::GetKegBehavior()
{
	return (shok_GGL_CKegBehavior*)SearchBehavior(shok_vtp_GGL_CKegBehavior);
}

shok_GGL_CFoundryBehavior* shok_EGL_CGLEEntity::GetFoundryBehavior()
{
	return (shok_GGL_CFoundryBehavior*)SearchBehavior(shok_vtp_GGL_CFoundryBehavior);
}

shok_GGL_CBattleSerfBehavior* shok_EGL_CGLEEntity::GetBattleSerfBehavior()
{
	return (shok_GGL_CBattleSerfBehavior*)SearchBehavior(shok_vtp_GGL_CBattleSerfBehavior);
}

shok_GGL_CSerfBehavior* shok_EGL_CGLEEntity::GetSerfBehavior()
{
	return (shok_GGL_CSerfBehavior*)SearchBehavior(shok_vtp_GGL_CSerfBehavior);
}

static inline bool(__thiscall* shok_IsEntityInCategory)(shok_EGL_CGLEEntity* e, int cat) = (bool(__thiscall*)(shok_EGL_CGLEEntity * e, int cat)) 0x57BBEB;
bool shok_EGL_CGLEEntity::IsEntityInCategory(int cat)
{
	return shok_IsEntityInCategory(this, cat);
}

bool shok_EGL_CGLEEntity::IsMovingEntity()
{
	void* data[4] = { shok_vtp_EGL_CMovingEntity, shok_vtp_GGL_CEvadingEntity, shok_vtp_GGL_CSettler, shok_vtp_GGL_CAnimal };
	return contains(data, vtable, 4);
}

static inline int(_cdecl* shok_EntityIsSettler)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4A2B62;
bool shok_EGL_CGLEEntity::IsSettler()
{
	return shok_EntityIsSettler(this);
}

static inline int(_cdecl* shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x449432;
bool shok_EGL_CGLEEntity::IsBuilding()
{
	return shok_EntityIsBuilding(this);
}

static inline int(_cdecl* shok_EntityIsResourceDoodad)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4B82C7;
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

static inline int(__thiscall* entitygetmaxhealth)(shok_EGL_CGLEEntity* th) = (int(__thiscall*)(shok_EGL_CGLEEntity*)) 0x57B798;
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
	shok_attachment* r = EntitiesAttachedToMe.GetFirstMatch([attachmentId](shok_attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}

int shok_EGL_CGLEEntity::GetFirstAttachedEntity(int attachmentId)
{
	shok_attachment* r = AttachedToEntities.GetFirstMatch([attachmentId](shok_attachment* a) {return a->AttachmentType == attachmentId; });
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
	return com == shok_LeaderCommandDefend || com == shok_LeaderCommandHoldPos || TaskListId == ((shok_GGL_CGLSettlerProps*)GetEntityType())->IdleTaskList;
}

int shok_GGL_CBuilding::GetConstructionSite()
{
	shok_attachment* a = EntitiesAttachedToMe.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == AttachmentType_ATTACHMENT_CONSTRUCTION_SITE_BUILDING; });
	if (a)
		return a->EntityId;
	return 0;
}

static inline int(__thiscall* shok_build_getnearestfreeslot)(shok_EGL_CGLEEntity* th, shok_position* p) = (int(__thiscall*) (shok_EGL_CGLEEntity*, shok_position*)) 0x4AB02D;
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
		if (EntitiesAttachedToMe.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == AttachmentType_FIGHTER_BARRACKS; }))
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




void shok_EGL_CGLEEntity::Destroy()
{
	((shok_vtable_EGL_CGLEEntity*)vtable)->Destroy(this, 0);
}

void shok_EGL_CGLEEntity::ClearAttackers()
{
	std::vector<shok_GGL_CSettler*> tomove = std::vector<shok_GGL_CSettler*>();
	EntitiesAttachedToMe.ForAll([&tomove](shok_attachment* a) {
		if (a->AttachmentType == AttachmentType_ATTACKER_COMMAND_TARGET || a->AttachmentType == AttachmentType_LEADER_TARGET || a->AttachmentType == AttachmentType_FOLLOWER_FOLLOWED) {
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

static inline void(__thiscall* shok_entity_expellSettler)(shok_EGL_CGLEEntity* th, int i) = (void(__thiscall*)(shok_EGL_CGLEEntity*, int))0x4A39BB;
void shok_EGL_CMovingEntity::SettlerExpell()
{
	if (GetLeaderBehavior() && AttachedToEntities.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == AttachmentType_LEADER_SOLDIER; })) {
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

static inline void(__thiscall* building_startUpgrade)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*)) 0x4AF1B5;
void shok_GGL_CBuilding::StartUpgrade()
{
	building_startUpgrade(this);
}

static inline void(__thiscall* buildingCancelUpgrade)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*))0x4AF228;
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

static inline void(__thiscall* shok_buildingActivateOvertime)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*)) 0x4AE566;
void shok_GGL_CBuilding::ActivateOvertime()
{
	shok_buildingActivateOvertime(this);
}
static inline void(__thiscall* shok_buildingDeactivateOvertime)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*)) 0x4AA073;
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


void (*Hero6ConvertHookCb)(int id, int pl, bool post, int converter) = nullptr;
int _cdecl hero6convertchangeplayer(int id, int pl) {
	shok_EGL_CGLEEntity* c = (shok_EGL_CGLEEntity*)1;
	_asm { mov c, esi }
	if (Hero6ConvertHookCb)
		Hero6ConvertHookCb(id, pl, false, c->EntityId);
	int r = shok_entityChangePlayer(id, pl);
	if (Hero6ConvertHookCb)
		Hero6ConvertHookCb(id, pl, true, c->EntityId);
	return r;
}
void HookHero6Convert()
{
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

static inline int(__thiscall* activateCamoOrig)(shok_GGL_CCamouflageBehavior* th) = (int(__thiscall*)(shok_GGL_CCamouflageBehavior*)) 0x501561;
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


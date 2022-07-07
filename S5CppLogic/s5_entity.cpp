#include "pch.h"
#include "s5_entity.h"
#include "s5_mem.h"
#include "s5_behaviors.h"
#include "s5_entitytype.h"
#include "s5_defines.h"
#include "s5_maplogic.h"
#include "entityiterator.h"
#include "s5_netevents.h"
#include "s5_tasklist.h"
#include "s5_player.h"
#include "s5_effects.h"
#include "s5_scriptsystem.h"
#include "s5_widget.h"
#include "s5_config.h"
#include "s5_behaviorProps.h"
#include "Luapp/luapp50.h"
#include "hooks.h"

struct shok_vtable_EGL_CGLEEntity : BB::IObject::_vtableS {
	void(__thiscall* Destroy)(EGL::CGLEEntity* th, int i); // 3
	PADDINGI(1);
	void(__thiscall* CopyDataFromCreator)(EGL::CGLEEntity* th, EGL::CGLEEntityCreator* cr); // 5
	PADDINGI(1);
	void(__thiscall* InitializeEntity)(EGL::CGLEEntity* th); // dont use outside of createentity
	void(__thiscall* InitializeAfterSaveLoad)(EGL::CGLEEntity* th); // dont use outside of save load
	void(__thiscall* FireOnCreatedTriggers)(EGL::CGLEEntity* th); // 9, script trigger + scriptcommandline
	void(__thiscall* SetPosition)(EGL::CGLEEntity* th, shok::Position* p); // 10 works for settlers, check if it does for other stuff to, called every tick
	void(__thiscall* SetRotation)(EGL::CGLEEntity* th, float r);
	int(__thiscall* GetSector)(EGL::CGLEEntity* th); // 12
	bool(__thiscall* IsInSector)(EGL::CGLEEntity* th, int sector);
	PADDINGI(1); // maybe execute task?/ontick?, settler update auras
	void(__thiscall* ExecuteTask)(EGL::CGLEEntity* th, EGL::CGLETaskArgs* t); // 15 return values: 2->same task, next tick, 1->next task, next tick, 0->next task, immediately
	void(__thiscall* FireEvent)(EGL::CGLEEntity* th, BB::CEvent* d); // 16
	void(__thiscall* AddBehavior)(EGL::CGLEEntity* th, EGL::CGLEBehavior* bh); // 17 probably not usable outside of createentity
	PADDINGI(1); // add behavior slot
	void(__thiscall* AddTaskHandler)(EGL::CGLEEntity* th, shok::Task task, EGL::TaskHandler* taskhandler); // 19
	void(__thiscall* AddEventHandler)(EGL::CGLEEntity* th, shok::EventIDs eventid, EGL::EventHandler* eventhandler); // 20
	void(__thiscall* AddStateHandler)(EGL::CGLEEntity* th, shok::TaskState state, EGL::IGLEStateHandler* statehandler); // 21
	void(__thiscall* GetApproachPos)(EGL::CGLEEntity* th, shok::Position* outpos);
	float(__thiscall* GetApproachRot)(EGL::CGLEEntity* th);
	void(__thiscall* SetTaskState)(EGL::CGLEEntity* th, shok::TaskState state); // 24
	PADDINGI(3);
	float(__thiscall* GetExploration)(EGL::CGLEEntity* e); // 28
	PADDINGI(5); // get flags as float, then jmp to 33
	bool(__thiscall* CanCancelCurrentState)(EGL::CGLEEntity* e); // 34
};
//constexpr int i = offsetof(shok_vtable_EGL_CGLEEntity, CanCancelCurrentState) / 4;


struct shok_vtable_EGL_IEntityDisplay {
	void(__thiscall* dtor)(EGL::IEntityDisplay* th);
	void* (__thiscall* GetSlotByIndex)(EGL::IEntityDisplay* th, int i);
	void(__thiscall* GetModelData)(const EGL::IEntityDisplay* th, EGL::IEntityDisplay::modeldata* d);
	void(__thiscall* GetPlayerModelData)(const EGL::IEntityDisplay* th, EGL::IEntityDisplay::playermodeldata* d);
	void(__thiscall* GetPosData)(const EGL::IEntityDisplay* th, EGL::IEntityDisplay::posdata* d);
	PADDINGI(1); // ret zero, 1 arg
};

EGL::IEntityDisplay::modeldata EGL::IEntityDisplay::GetModelData() const
{
	modeldata r;
	GetModelData(&r);
	return r;
}
EGL::IEntityDisplay::playermodeldata EGL::IEntityDisplay::GetPlayerModelData() const
{
	playermodeldata r;
	GetPlayerModelData(&r);
	return r;
}
EGL::IEntityDisplay::posdata EGL::IEntityDisplay::GetPosData() const
{
	posdata r;
	GetPosData(&r);
	return r;
}

static inline void(__thiscall* const shok_EGL_CGLEEntityCreator_ctor)(EGL::CGLEEntityCreator* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEntityCreator*)>(0x4493A4);
EGL::CGLEEntityCreator::CGLEEntityCreator()
{
	shok_EGL_CGLEEntityCreator_ctor(this);
}
EGL::CGLEEntityCreator::CGLEEntityCreator(int _)
{
}

static inline void(__thiscall* const shok_GGL_CGLConstructionSiteCreator_ctor)(GGL::CGLConstructionSiteCreator* th) = reinterpret_cast<void(__thiscall*)(GGL::CGLConstructionSiteCreator*)>(0x4B8E8F);
GGL::CGLConstructionSiteCreator::CGLConstructionSiteCreator() : EGL::CGLEEntityCreator(1)
{
	shok_GGL_CGLConstructionSiteCreator_ctor(this);
}
static inline void(__thiscall* const shok_GGL_CResourceDoodadCreator_ctor)(GGL::CResourceDoodadCreator* th) = reinterpret_cast<void(__thiscall*)(GGL::CResourceDoodadCreator*)>(0x4DD7E2);
GGL::CResourceDoodadCreator::CResourceDoodadCreator() : EGL::CGLEEntityCreator(1)
{
	shok_GGL_CResourceDoodadCreator_ctor(this);
}

static inline void(__thiscall* const shok_EGL_CGLEEntityCreator_dtor)(EGL::CGLEEntityCreator* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEntityCreator*)>(0x449E23);
EGL::CGLEEntityCreator::~CGLEEntityCreator()
{
	shok_EGL_CGLEEntityCreator_dtor(this);
}

unsigned int __stdcall EGL::CGLEEntityCreator::GetClassIdentifier() const {
	return 0;
}
void* __stdcall EGL::CGLEEntityCreator::CastToIdentifier(unsigned int id) {
	return nullptr;
}

static inline bool(__thiscall* const shok_IsEntityInCategory)(const EGL::CGLEEntity* e, shok::EntityCategory cat) = reinterpret_cast<bool(__thiscall*)(const EGL::CGLEEntity * e, shok::EntityCategory)>(0x57BBEB);
bool EGL::CGLEEntity::IsEntityInCategory(shok::EntityCategory cat) const
{
	return shok_IsEntityInCategory(this, cat);
}

shok::ResourceType EGL::CGLEEntity::GetResourceProvided() const
{
	return EGL::CGLEEntity::EntityIDGetProvidedResource(this->EntityId);
}

GGlue::CGlueEntityProps* EGL::CGLEEntity::GetEntityType() const
{
	return (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(EntityType);
}

int EGL::CGLEEntity::EventGetIntById(shok::EventIDs id)
{
	EGL::CEventGetValue_Int d{ id };
	FireEvent(&d);
	return d.Data;
}

int EGL::CGLEEntity::EventGetDamage()
{
	return EventGetIntById(shok::EventIDs::GetDamage);
}
int EGL::CGLEEntity::EventGetArmor()
{
	return EventGetIntById(shok::EventIDs::GetArmor);
}
int EGL::CGLEEntity::EventGetMaxWorktime()
{
	return EventGetIntById(shok::EventIDs::Worker_GetMaxWorkTime);
}

shok::LeaderCommand EGL::CGLEEntity::EventLeaderGetCurrentCommand()
{
	if (GetBehavior<GGL::CLeaderBehavior>() == nullptr)
		return shok::LeaderCommand::Unknown;
	return static_cast<shok::LeaderCommand>(EventGetIntById(shok::EventIDs::Leader_GetCommand));
}

static inline int(__thiscall* const entitygetmaxhealth)(EGL::CGLEEntity* th) = reinterpret_cast<int(__thiscall*)(EGL::CGLEEntity*)>(0x57B798);
int EGL::CGLEEntity::GetMaxHealth()
{
	return entitygetmaxhealth(this);
}

int EGL::CGLEEntity::LimitedAttachmentGetMaximum(shok::AttachmentType attachType)
{
	GGL::CEventAttachmentTypeGetInteger ev{ shok::EventIDs::LimitedAttachment_GetMax, attachType };
	FireEvent(&ev);
	return ev.Data;
}
int EGL::CGLEEntity::ResourceTreeGetNearestSector() const
{
	shok::Position p = Position;
	shok::Position p2 = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetNearestFreePos(&p, 600);
	if (!(*EGL::CGLEGameLogic::GlobalObj)->Landscape->IsValidPos(&p2)) {
		return 0;
	}
	return (*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p2);
}


int EGL::CGLEEntity::GetFirstAttachedToMe(shok::AttachmentType attachmentId) const
{
	for (const shok::Attachment& r : ObserverEntities) {
		if (r.AttachmentType == attachmentId)
			return r.EntityId;
	}
	return 0;
}

int EGL::CGLEEntity::GetFirstAttachedEntity(shok::AttachmentType attachmentId) const
{
	for (const shok::Attachment& r : ObservedEntities) {
		if (r.AttachmentType == attachmentId)
			return r.EntityId;
	}
	return 0;
}
static inline void(__thiscall* entattach_attach)(EGL::CGLEAttachableBase* th, shok::AttachmentType at, int id, shok::EventIDs evth, shok::EventIDs evot) = reinterpret_cast<void(__thiscall*)(EGL::CGLEAttachableBase*, shok::AttachmentType, int, shok::EventIDs, shok::EventIDs)>(0x4A61B3);
void EGL::CGLEEntity::AttachEntity(shok::AttachmentType attachtype, int otherId, shok::EventIDs eventIdOnThisDetach, shok::EventIDs eventIdOnOtherDetach)
{
	entattach_attach(static_cast<EGL::CGLEAttachableBase*>(this), attachtype, otherId, eventIdOnThisDetach, eventIdOnOtherDetach);
}
static inline void(__thiscall* entattach_detach)(EGL::CGLEAttachableBase* th, shok::AttachmentType at, int id, byte ev) = reinterpret_cast<void(__thiscall*)(EGL::CGLEAttachableBase*, shok::AttachmentType, int, byte)>(0x4A2E5D);
void EGL::CGLEEntity::DetachObservedEntity(shok::AttachmentType attachtype, int otherId, bool fireEvent)
{
	entattach_detach(static_cast<EGL::CGLEAttachableBase*>(this), attachtype, otherId, fireEvent);
}

int EGL::CMovingEntity::LeaderGetNearbyBarracks()
{
	return EventGetIntById(shok::EventIDs::Leader_GetNearbyBarracks);
}

bool EGL::CMovingEntity::IsMoving()
{
	EGL::CEventGetValue_Bool d{ shok::EventIDs::Movement_IsMoving };
	FireEvent(&d);
	return d.Data;
}

bool EGL::CMovingEntity::IsFleeingFrom(const shok::Position& center, float range) const
{
	if (GetFirstAttachedToMe(shok::AttachmentType::INFLICTOR_TERRORIZED) != 0)
		return true;
	float posrsq = Position.GetDistanceSquaredTo(center);
	float tprsq = TargetPosition.GetDistanceSquaredTo(center);
	return std::sqrtf(posrsq) + range < std::sqrtf(tprsq);
}

float EGL::IProfileModifierSetObserver::GetModifiedValue(ModifierType t, float z)
{
	throw 0;
}

static inline float(__thiscall* const modentitydb_getmod)(GGL::ModifierEntityDatabase* th, int id, GGL::CEntityProfile::ModifierType ty, float initial) = reinterpret_cast<float(__thiscall*)(GGL::ModifierEntityDatabase*, int, GGL::CEntityProfile::ModifierType, float)>(0x584078);
float GGL::ModifierEntityDatabase::GetModifiedStat(int id, CEntityProfile::ModifierType ty, float initial)
{
	return modentitydb_getmod(this, id, ty, initial);
}

bool GGL::CSettler::IsIdle()
{
	shok::LeaderCommand com = EventLeaderGetCurrentCommand();
	return com == shok::LeaderCommand::Defend || com == shok::LeaderCommand::HoldPos || CurrentTaskListID == static_cast<GGL::CGLSettlerProps*>(GetEntityType()->LogicProps)->IdleTaskList;
}

static inline void(__thiscall* const shok_GGL_CResourceDoodad_setresam)(GGL::CResourceDoodad* th, int am) = reinterpret_cast<void(__thiscall* const)(GGL::CResourceDoodad*, int)>(0x4B864B);
void GGL::CResourceDoodad::SetResourceAmount(int am)
{
	shok_GGL_CResourceDoodad_setresam(this, am);
}

int GGL::CBuilding::GetConstructionSite() const
{
	return GetFirstAttachedToMe(shok::AttachmentType::CONSTRUCTION_SITE_BUILDING);
}

static inline int(__thiscall* const shok_build_getnearestfreeslot)(EGL::CGLEEntity* th, shok::Position* p) = reinterpret_cast<int(__thiscall*) (EGL::CGLEEntity*, shok::Position*)>(0x4AB02D);
int GGL::CBuilding::GetNearestFreeConstructionSlotFor(shok::Position* p)
{
	int cid = GetConstructionSite();
	if (!cid)
		return -1;
	EGL::CGLEEntity* consi = EGL::CGLEEntity::GetEntityByID(cid);
	if (!consi)
		return -1;
	return shok_build_getnearestfreeslot(consi, p);
}

int GGL::CBuilding::GetNearestFreeRepairSlotFor(shok::Position* p)
{
	return shok_build_getnearestfreeslot(this, p);
}

bool GGL::CBuilding::IsConstructionFinished() const
{
	return BuildingHeight >= 1;
}

bool GGL::CBuilding::IsIdle()
{
	return IsIdle(false);
}

bool GGL::CBuilding::IsIdle(bool forRecruitemnt)
{
	if (!IsConstructionFinished())
		return false;
	if (IsUpgrading)
		return false;
	if (CurrentState == shok::TaskState::BuildingAlarmDefend) // alarm mode
		return false;
	if (GetTechnologyInResearch())
		return false;
	{
		GGL::CFoundryBehavior* f = GetBehavior<GGL::CFoundryBehavior>();
		if (f && (f->CannonType != 0 || GetCannonProgress() != 100))
			return false;
	}
	if (GetBehavior<GGL::CMarketBehavior>() && GetMarketProgress() < 1.0f)
		return false;
	if (GetBehavior<GGL::CBarrackBehavior>()) {

		if (forRecruitemnt) {
			int c = 0;
			ObserverEntities.ForAll([&c](shok::Attachment* a) {
				if (a->AttachmentType == shok::AttachmentType::FIGHTER_BARRACKS && !EGL::CGLEEntity::GetEntityByID(a->EntityId)->GetBehavior<GGL::CSoldierBehavior>())
					c++;
				});
			if (c >= 3)
				return false;
		}
		else {
			if (ObserverEntities.GetFirstMatch([](shok::Attachment* a) { return a->AttachmentType == shok::AttachmentType::FIGHTER_BARRACKS; }))
				return false;
		}

	}
	return true;
}

static inline int(__thiscall* building_gettechinres)(GGL::CBuilding* th) = reinterpret_cast<int(__thiscall*)(GGL::CBuilding*)>(0x4AAD09);
int GGL::CBuilding::GetTechnologyInResearch()
{
	return building_gettechinres(this);
}

int GGL::CBuilding::GetCannonProgress()
{
	return EventGetIntById(shok::EventIDs::Foundry_GetProgress);
}

float GGL::CBuilding::GetMarketProgress()
{
	EGL::CEventGetValue_Float ev{ shok::EventIDs::Market_GetProgress };
	FireEvent(&ev);
	return ev.Data;
}

static inline float(__thiscall* building_getupremainingtime)(GGL::CBuilding* th) = reinterpret_cast<float(__thiscall*)(GGL::CBuilding*)>(0x4ABA5B);
float GGL::CBuilding::GetRemainingUpgradeTime()
{
	return building_getupremainingtime(this);
}

static inline void(__thiscall* const entitysethealth)(EGL::CGLEEntity* th, int h) = reinterpret_cast<void(__thiscall* const)(EGL::CGLEEntity*, int)>(0x57A6D9);
void EGL::CGLEEntity::SetHealth(int h)
{
	entitysethealth(this, h);
}
static inline void(__thiscall* const entityhurt)(EGL::CGLEEntity* th, int h) = reinterpret_cast<void(__thiscall* const)(EGL::CGLEEntity*, int)>(0x57AD47);
void EGL::CGLEEntity::Hurt(int dmg)
{
	entityhurt(this, dmg);
}

static inline void(__thiscall* const shok_entity_settasklistbyid)(EGL::CGLEEntity* th, int tl, int t) = reinterpret_cast<void(__thiscall* const)(EGL::CGLEEntity*, int, int)>(0x57B3B6);
void EGL::CGLEEntity::SetTaskList(int tl)
{
	shok_entity_settasklistbyid(this, tl, 1);
}
static inline void(__thiscall* const shok_entity_settasklist)(EGL::CGLEEntity* th, EGL::CGLETaskList* tl, int t) = reinterpret_cast<void(__thiscall* const)(EGL::CGLEEntity*, EGL::CGLETaskList*, int)>(0x57B208);
void EGL::CGLEEntity::SetTaskList(EGL::CGLETaskList* tl)
{
	shok_entity_settasklist(this, tl, 1);
}
static inline EGL::CGLETaskList* (__thiscall* const shok_entity_GetCurrentTaskList)(EGL::CGLEEntity* th) = reinterpret_cast<EGL::CGLETaskList * (__thiscall* const)(EGL::CGLEEntity*)>(0x57A892);
EGL::CGLETaskList* EGL::CGLEEntity::GetCurrentTaskList()
{
	return shok_entity_GetCurrentTaskList(this);
}
static inline EGL::TaskHandler* (__thiscall* const shok_entitytaskhandler_gettaskhandler)(shok::Set<EGL::CGLEEntity::TaskIdAndTaskHandler>* th, shok::Task id) = reinterpret_cast<EGL::TaskHandler * (__thiscall* const)(shok::Set<EGL::CGLEEntity::TaskIdAndTaskHandler>*, shok::Task)>(0x57BDD3);
EGL::TaskHandler* EGL::CGLEEntity::GetTaskHandler(shok::Task task)
{
	return shok_entitytaskhandler_gettaskhandler(&TaskHandlers, task);
}

void EGL::CGLEEntity::ExecuteTask(EGL::CGLETaskArgs& targ)
{
	ExecuteTask(&targ);
}

void EGL::CGLEEntity::Destroy()
{
	Destroy(0);
}

void EGL::CGLEEntity::ClearAttackers()
{
	std::vector<GGL::CSettler*> tomove = std::vector<GGL::CSettler*>();
	ObserverEntities.ForAll([&tomove](shok::Attachment* a) {
		if (a->AttachmentType == shok::AttachmentType::ATTACKER_COMMAND_TARGET || a->AttachmentType == shok::AttachmentType::LEADER_TARGET || a->AttachmentType == shok::AttachmentType::FOLLOWER_FOLLOWED) {
			EGL::CGLEEntity* at = EGL::CGLEEntity::GetEntityByID(a->EntityId);
			if (GGL::CSettler* s = dynamic_cast<GGL::CSettler*>(at)) {
				tomove.emplace_back(s);
			}
		}
		});
	for (GGL::CSettler* s : tomove)
		s->Defend();
}



void EGL::CMovingEntity::AttackMove(const shok::Position& p)
{
	EGL::CEventPosition ev{ shok::EventIDs::Leader_AttackMove, p };
	FireEvent(&ev);
	TargetRotationValid = 0;
}

void EGL::CMovingEntity::AttackEntity(int targetId)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Leader_AttackEntity, targetId };
	FireEvent(&ev);
}

void EGL::CMovingEntity::Move(const shok::Position& p)
{
	EGL::CEventPosition ev{ shok::EventIDs::MoveCommand_Move, p };
	FireEvent(&ev);
	TargetRotationValid = 0;
}

void EGL::CMovingEntity::HoldPosition()
{
	BB::CEvent ev{ shok::EventIDs::Leader_HoldPosition };
	FireEvent(&ev);
}

void EGL::CMovingEntity::Defend()
{
	BB::CEvent ev{ shok::EventIDs::Leader_Defend };
	FireEvent(&ev);
}

void EGL::CMovingEntity::LeaderAttachSoldier(int soldierId)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Leader_AttachSoldier, soldierId };
	FireEvent(&ev);
	BB::CEvent e2{ shok::EventIDs::Formation_AssumePosition };
	EGL::CGLEEntity* sol = EGL::CGLEEntity::GetEntityByID(soldierId);
	sol->FireEvent(&e2);
}

static inline void(__thiscall* const shok_entity_expellSettler)(EGL::CGLEEntity* th, int i) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEntity*, int)>(0x4A39BB);
void EGL::CMovingEntity::SettlerExpell()
{
	if (GetBehavior<GGL::CLeaderBehavior>() && ObservedEntities.GetFirstMatch([](shok::Attachment* a) { return a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER; })) {
		EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_ExpellSoldier, true };
		FireEvent(&ev);
	}
	else {
		shok_entity_expellSettler(this, 1);
	}
}

void EGL::CMovingEntity::HeroAbilitySendHawk(shok::Position& p)
{
	EGL::CEventPosition ev{ shok::EventIDs::HeroHawk_SendHawk, p };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityInflictFear()
{
	BB::CEvent ev{ shok::EventIDs::InflictFear_Activate };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityPlaceBomb(shok::Position& p)
{
	EGL::CEventPosition ev{ shok::EventIDs::BombPlacer_CommandPlaceBomb, p };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityPlaceCannon(shok::Position& p, int FoundationType, int CannonType)
{
	GGL::CEventPositionAnd2EntityTypes ev{ shok::EventIDs::CannonBuilder_BuildCannonCommand, p, FoundationType, CannonType };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityRangedEffect()
{
	BB::CEvent ev{ shok::EventIDs::RangedEffect_Activate };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityCircularAttack()
{
	BB::CEvent ev{ shok::EventIDs::CircularAttack_ActivateCommand };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilitySummon()
{
	BB::CEvent ev{ shok::EventIDs::Summon_ActivateCommand };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityConvert(int target)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::ConvertSettler_ActivateCommand, target };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityConvertBuilding(int target)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::ConvertBuilding_ActivateCommand, target };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilitySnipe(int tid)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Sniper_SnipeCommand, tid };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityShuriken(int tid)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Shuriken_ActivateCommand, tid };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityMotivateWorkers()
{
	BB::CEvent ev{ shok::EventIDs::MotivateVorkers_ActivateCommand };
	FireEvent(&ev);
}

void EGL::CMovingEntity::HeroAbilityActivateCamoflage()
{
	BB::CEvent e{ shok::EventIDs::Camouflage_Activate };
	FireEvent(&e);
}

void EGL::CMovingEntity::ThiefSabotage(int tid)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::KegPlacer_SabotageCommand, tid };
	FireEvent(&ev);
}

void EGL::CMovingEntity::ThiefDefuse(int tid)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::KegPlacer_DefuseCommand, tid };
	FireEvent(&ev);
}

void EGL::CMovingEntity::ThiefStealFrom(int tid)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Thief_StealFromCommand, tid };
	FireEvent(&ev);
}

void EGL::CMovingEntity::ThiefSecureGoods(int tid)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Thief_SecureGoodsCommand, tid };
	FireEvent(&ev);
}

void EGL::CMovingEntity::ScoutBinoculars(shok::Position& p)
{
	EGL::CEventPosition ev{ shok::EventIDs::Binocular_ExploreCommand, p };
	FireEvent(&ev);
}

void EGL::CMovingEntity::ScoutFindResource()
{
	BB::CEvent ev{ shok::EventIDs::PointToResources_Activate };
	FireEvent(&ev);
}

void EGL::CMovingEntity::ScoutPlaceTorch(shok::Position& p)
{
	EGL::CEventPosition ev{ shok::EventIDs::TorchPlacer_PlaceTorch, p };
	FireEvent(&ev);
}

bool EGL::CMovingEntity::SerfConstructBuilding(GGL::CBuilding* build)
{
	int cid = build->GetNearestFreeConstructionSlotFor(&this->Position);
	if (cid < 0)
		return false;
	GGL::CEventEntityIndex ev{ shok::EventIDs::Serf_Construct, build->GetConstructionSite(), cid };
	FireEvent(&ev);
	return true;
}

bool EGL::CMovingEntity::SerfRepairBuilding(GGL::CBuilding* build)
{
	int cid = build->GetNearestFreeRepairSlotFor(&this->Position);
	if (cid < 0)
		return false;
	GGL::CEventEntityIndex ev{ shok::EventIDs::Serf_Construct, build->EntityId, cid };
	FireEvent(&ev);
	return true;
}

void EGL::CMovingEntity::SerfExtractResource(int id)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Serf_ExtractResource, id };
	FireEvent(&ev);
}

void EGL::CMovingEntity::SerfTurnToBattleSerf()
{
	BB::CEvent e2{ shok::EventIDs::Serf_CommandTurnToBattleSerf };
	FireEvent(&e2);
}

void EGL::CMovingEntity::BattleSerfTurnToSerf()
{
	BB::CEvent e2{ shok::EventIDs::BattleSerf_CommandTurnToSerf };
	FireEvent(&e2);
}

void(__thiscall* const movingentity_settarrot)(EGL::CMovingEntity* th, float r) = reinterpret_cast<void(__thiscall*)(EGL::CMovingEntity*, float)>(0x57DBC8);
void EGL::CMovingEntity::SetTargetRotation(float f)
{
	movingentity_settarrot(this, f);
}

void(__thiscall* const movingentity_setpos)(EGL::CMovingEntity* th, shok::Position* p) = reinterpret_cast<void(__thiscall*)(EGL::CMovingEntity*, shok::Position*)>(0x57BC78);
void EGL::CMovingEntity::SetPosition(shok::Position& p)
{
	movingentity_setpos(this, &p);
	Move(p);
}

static inline void(__thiscall* const building_startUpgrade)(GGL::CBuilding* th) = reinterpret_cast<void(__thiscall*)(GGL::CBuilding*)>(0x4AF1B5);
void GGL::CBuilding::StartUpgrade()
{
	building_startUpgrade(this);
}

static inline void(__thiscall* const buildingCancelUpgrade)(GGL::CBuilding* th) = reinterpret_cast<void(__thiscall*)(GGL::CBuilding*)>(0x4AF228);
void GGL::CBuilding::CancelUpgrade()
{
	buildingCancelUpgrade(this);
}

void GGL::CBuilding::CommandBuildCannon(int entitytype)
{
	EGL::CEventValue_Int e{ shok::EventIDs::Foundry_BuildCannonCommand, entitytype };
	FireEvent(&e);
}

void GGL::CBuilding::CommandRecruitSoldierForLeader(int id)
{
	EGL::CEvent1Entity ev{ shok::EventIDs::Barracks_BuySoldierForLeader, id };
	FireEvent(&ev);
}

static inline void(__thiscall* const shok_buildingActivateOvertime)(GGL::CBuilding* th) = reinterpret_cast<void(__thiscall*)(GGL::CBuilding*)>(0x4AE566);
void GGL::CBuilding::ActivateOvertime()
{
	shok_buildingActivateOvertime(this);
}
static inline void(__thiscall* const shok_buildingDeactivateOvertime)(GGL::CBuilding* th) = reinterpret_cast<void(__thiscall*)(GGL::CBuilding*)>(0x4AA073);
void GGL::CBuilding::DeactivateOvertime()
{
	shok_buildingDeactivateOvertime(this);
}

void GGL::CBuilding::BarracksRecruitGroups()
{
	BB::CEvent e2{ shok::EventIDs::Barracks_ActivateAutoFill };
	FireEvent(&e2);
}

void GGL::CBuilding::BarracksRecruitLeaders()
{
	BB::CEvent e2{ shok::EventIDs::Barracks_DeActivateAutoFill };
	FireEvent(&e2);
}

void GGL::CBuilding::HQBuySerf()
{
	BB::CEvent e2{ shok::EventIDs::Keep_BuySerfCommand };
	FireEvent(&e2);
}

void GGL::CBuilding::SellBuilding()
{
	BB::CEvent e2{ shok::EventIDs::Die };
	FireEvent(&e2);
}

static inline void(__thiscall* const building_startresearch)(GGL::CBuilding* th, int tech) = reinterpret_cast<void(__thiscall*)(GGL::CBuilding*, int)>(0x4AAC76);
void GGL::CBuilding::StartResearch(int tech)
{
	building_startresearch(this, tech);
}

static inline void(__thiscall* const building_cancelresearch)(GGL::CBuilding* th) = reinterpret_cast<void(__thiscall*)(GGL::CBuilding*)>(0x4AACC0);
void GGL::CBuilding::CancelResearch()
{
	building_cancelresearch(this);
}

void GGL::CBuilding::MarketStartTrade(shok::ResourceType ResourceTypeSell, shok::ResourceType ResourceTypeBuy, float BuyAmount)
{
	GGL::CEventTransaction e2{ shok::EventIDs::Market_StartTrade, ResourceTypeSell, ResourceTypeBuy, BuyAmount };
	FireEvent(&e2);
}

void GGL::CBuilding::MarketCancelTrade()
{
	BB::CEvent e2{ shok::EventIDs::Market_CancelTrade };
	FireEvent(&e2);
}

static inline int(__thiscall* const raxbeh_createentityandattach)(GGL::CBarrackBehavior* th, int ety) = reinterpret_cast<int(__thiscall*)(GGL::CBarrackBehavior*, int)>(0x50EA18);
static inline int(__thiscall* const raxbeh_gettrainingtl)(GGL::CBarrackBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CBarrackBehavior*)>(0x50EBCE);
int GGL::CBuilding::BuyLeaderByType(int ety)
{
	GGL::CBarrackBehavior* rax = GetBehavior<GGL::CBarrackBehavior>();
	int id = raxbeh_createentityandattach(rax, ety);
	if (id) {
		EGL::CEventValue_Int ev = { shok::EventIDs::Leader_SetTrainingTL, raxbeh_gettrainingtl(rax) };
		EGL::CGLEEntity::GetEntityByID(id)->FireEvent(&ev);
	}
	return id;
}


std::vector<shok::AdditionalTechModifier> GGL::CBuilding::ConstructionSpeedModifiers{};
float __fastcall constructionsite_getprogresspertick_hook(GGL::CBuilding* th) { // param is constructionsite, just not done yet ;)
	GGL::CGLSettlerProps* serf = static_cast<GGL::CGLSettlerProps*>((*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(*GGlue::CGlueEntityProps::EntityTypeIDSerf)->LogicProps);
	GGL::CGLBuildingProps* bty = static_cast<GGL::CGLBuildingProps*>((*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(th->ConstructionSiteType)->LogicProps);
	float constructionfactor = serf->BuildFactor;
	GGL::CPlayerStatus* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(th->PlayerId);
	for (const shok::AdditionalTechModifier& tmod : GGL::CBuilding::ConstructionSpeedModifiers) {
		if (pl->GetTechStatus(tmod.TechID) != shok::TechState::Researched)
			continue;
		constructionfactor = tmod.ModifyValue(constructionfactor);
	}
	constructionfactor = constructionfactor * bty->ConstructionInfo.Time * 10;
	if (constructionfactor <= 0.0f)
		return 0.0f;
	else
		return 1.0f / constructionfactor;
}
bool EnableConstructionSpeedTechs_Hooked = false;
void GGL::CBuilding::EnableConstructionSpeedTechs()
{
	if (EnableConstructionSpeedTechs_Hooked)
		return;
	EnableConstructionSpeedTechs_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4B8EAD), 0x4B8EB2 - 0x4B8EAD };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4B8EAD), &constructionsite_getprogresspertick_hook, reinterpret_cast<void*>(0x4B8EB2));
}


int __cdecl fixedChangePlayer(int id, int pl) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
	if (!e)
		return 0;
	if (e->PlayerId == pl)
		return id;
	EGL::CGLEEntityCreator c{};
	c.EntityType = e->EntityType;
	c.PlayerId = pl;
	c.Pos = e->Position;
	c.Scale = e->Scale;
	c.Health = e->Health;
	if (e->ScriptName) {
		size_t len = strlen(e->ScriptName) + 1;
		c.ScriptName = (char*)shok::Malloc(sizeof(char) * len);
		strcpy_s(c.ScriptName, len, e->ScriptName);
	}
	else {
		c.ScriptName = nullptr;
	}
	int nid = (*EGL::CGLEGameLogic::GlobalObj)->CreateEntity(&c);
	if (dynamic_cast<GGL::CSettler*>(e)) {
		GGL::CLeaderBehavior* lb = e->GetBehavior<GGL::CLeaderBehavior>();
		if (lb) {
			std::vector<int> sol = std::vector<int>();
			e->ObservedEntities.ForAll([&sol](shok::Attachment* a) { if (a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER) sol.push_back(a->EntityId); });
			GGL::CSettler* settler = static_cast<GGL::CSettler*>(EGL::CGLEEntity::GetEntityByID(nid));
			for (int i : sol) {
				settler->LeaderAttachSoldier(fixedChangePlayer(i, pl));
			}
			GGL::CLeaderBehavior* nlb = settler->GetBehavior<GGL::CLeaderBehavior>();
			nlb->Experience = lb->Experience;
			nlb->TroopHealthCurrent = lb->TroopHealthCurrent;
			nlb->TroopHealthPerSoldier = lb->TroopHealthPerSoldier;
		}
	}
	if (GGL::CReplaceableEntityBehavior* rep = e->GetBehavior<GGL::CReplaceableEntityBehavior>()) {
		rep->IsReplacementActive = false;
	}
	e->Destroy();
	return nid;
}
bool ActivateEntityChangePlayerFix_Hooked = false;
void EGL::CGLEEntity::ActivateEntityChangePlayerFix()
{
	if (ActivateEntityChangePlayerFix_Hooked)
		return;
	ActivateEntityChangePlayerFix_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ EGL::CGLEEntity::EntityIDChangePlayer, 0x49A6AC - 0x49A6A7 };
	CppLogic::Hooks::WriteJump(EGL::CGLEEntity::EntityIDChangePlayer, &fixedChangePlayer, reinterpret_cast<void*>(0x49A6AC));
}

void (*EGL::CGLEEntity::Hero6ConvertHookCb)(int id, int pl, int nid, int converter) = nullptr;
int _cdecl hero6convertchangeplayer(int id, int pl) {
	EGL::CGLEEntity* c = (EGL::CGLEEntity*)1;
	_asm { mov c, esi }
	int r = EGL::CGLEEntity::EntityIDChangePlayer(id, pl);
	if (EGL::CGLEEntity::Hero6ConvertHookCb)
		EGL::CGLEEntity::Hero6ConvertHookCb(id, pl, r, c->EntityId);
	return r;
}
bool HookHero6Convert_Hooked = false;
void EGL::CGLEEntity::HookHero6Convert()
{
	if (HookHero6Convert_Hooked)
		return;
	HookHero6Convert_Hooked = true;
	EGL::CGLEEntity::ActivateEntityChangePlayerFix();
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4FCD26), 10 };
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4FCD26), &hero6convertchangeplayer);
}

int EGL::CGLEEntity::ResetCamoIgnoreIfNotEntity = 0;
void __fastcall camo_behaviorReset(GGL::CCamouflageBehavior* th, int _, int a) {
	if (EGL::CGLEEntity::ResetCamoIgnoreIfNotEntity == 0 || th->EntityId == EGL::CGLEEntity::ResetCamoIgnoreIfNotEntity)
		th->InvisibilityRemaining = 0;
}
bool HookResetCamo_Hooked = false;
void EGL::CGLEEntity::HookResetCamo()
{
	if (HookResetCamo_Hooked)
		return;
	HookResetCamo_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5011DF), 0x5011E6 - 0x5011DF };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5011DF), &camo_behaviorReset, reinterpret_cast<void*>(0x5011E6));
}

static inline int(__thiscall* const activateCamoOrig)(GGL::CCamouflageBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CCamouflageBehavior*)>(0x501561);
void (*EGL::CGLEEntity::CamoActivateCb)(GGL::CCamouflageBehavior* th);
int __fastcall camoActivateHook(GGL::CCamouflageBehavior* th) {
	int i = activateCamoOrig(th);
	if (EGL::CGLEEntity::CamoActivateCb)
		EGL::CGLEEntity::CamoActivateCb(th);
	return i;
}
bool HookCamoActivate_Hooked = false;
void EGL::CGLEEntity::HookCamoActivate()
{
	if (HookCamoActivate_Hooked)
		return;
	HookCamoActivate_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4D51A4), 10 };
	CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x50163A), 10 };
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4D51A4), &camoActivateHook);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x50163A), &camoActivateHook);
}


static inline void(__thiscall* const event2entitiesctor)(int* e, int id, int at, int de) = reinterpret_cast<void(__thiscall*)(int*, int, int, int)>(0x49847F);
int* EGL::CGLEEntity::HurtEntityDamagePointer = nullptr;
bool EGL::CGLEEntity::HurtEntityCallWithNoAttacker = false;
shok::AdvancedDealDamageSource EGL::CGLEEntity::HurtEntityDamageSource = shok::AdvancedDealDamageSource::Unknown;
int  EGL::CGLEEntity::HurtEntityAttackerPlayer = 0;
void (*EGL::CGLEEntity::HurtEntityOnKillCb)(EGL::CGLEEntity* att, EGL::CGLEEntity* kill, int attpl, shok::AdvancedDealDamageSource sourc) = nullptr;
bool HookHurtEntity_Hooked = false;
void __cdecl hurtentity_override(EGL::CGLEEntity* att, EGL::CGLEEntity* tar, int dmg) {
	tar->AdvancedHurtEntityBy(att, dmg, 0, true, true, true, shok::AdvancedDealDamageSource::Unknown);
}
void __cdecl hurtaoe_override(EGL::CGLEEntity* att, shok::Position* p, float r, int dmg, int pl, int dmgcl) {
	EGL::CGLEEntity::AdvancedDealAoEDamage(att, *p, r, dmg, pl, dmgcl, true, true, true, shok::AdvancedDealDamageSource::Unknown);
}
void __declspec(naked) arrowonhit_damage() {
	__asm {
		mov ebx, 0;
		mov bl, [esi + 50*4 + 3]; // AdvancedDamageSourceOverride
		cmp ebx, 0;
		jne pu;
		mov ebx, 2; // arrow

	pu:
		push ebx;
		push 1;
		push 1;
		push 1;

		push[esi + 22 * 4]; // effect player

		push ecx;
		push eax;

		mov ecx, [ebp - 0x10];
		call EGL::CGLEEntity::AdvancedHurtEntityBy;

		push 0x5113CF;
		ret;
	};
}
void __declspec(naked) meleeonhit_damage() {
	__asm {
		push 1;
		push 1;
		push 1;
		push 1;
		push 0;
		push[ebp - 0x10];

		push[edi + 8];
		call EGL::CGLEEntity::GetEntityByID;
		push eax;

		mov ecx, esi;
		call EGL::CGLEEntity::AdvancedHurtEntityBy;

		push 0x50CA6D;
		ret;
	};
}
void __declspec(naked) circularatt_damage() {
	__asm {
		push 11;
		push 1;
		push 1;
		push 1;
		push 0;
		push eax;
		push esi;
		mov ecx, [ebp - 0x10];
		call EGL::CGLEEntity::AdvancedHurtEntityBy;

		push 0x4FE72F;
		ret;
	};
}
void __declspec(naked) cannonballhit_damage() {
	__asm {
		mov eax, [esi + 52 * 4];
		shr eax, 24;
		cmp eax, 0;
		jne pu;

		mov eax, 3;
	pu:
		push eax;

		push 1;
		push 1;
		push 1;
		mov eax, [esi + 52 * 4];
		and eax, 0xFFFFFF;
		push eax;
		push[esi + 48 * 4];
		push edx;
		push edx;
		fstp[esp];

		lea eax, [esi + 16 * 4]; // pos
		push eax;
		push ebx;
		call EGL::CGLEEntity::AdvancedDealAoEDamage;

		mov eax, [esi + 0xC4];
		push 0x4FF51B;
		ret;
	};
}
void __declspec(naked) bombexplode_damage() {
	__asm {
		push 12;
		push 1;
		push 1;
		push 1;
		push 0;
		push edi;
		push ebx;

		push ebx;
		fstp[esp];

		lea eax, [eax + 22 * 4];
		push eax;
		push[ebp - 0x10];
		call EGL::CGLEEntity::AdvancedDealAoEDamage;

		push 0x506B3F;
		ret;
	};
}
void __fastcall kegdealdmgproxy(GGL::CKegBehavior* th) {
	th->AdvancedDealDamage();
}
void __declspec(naked) shurikenthrow() {
	__asm {
		lea edx, [ebp - 0x5C];

		mov byte ptr [edx + 17 * 4 + 3], 15;

		mov[ebp - 0x20], eax;
		mov eax, [ecx];
		push edx;


		push 0x4DC6E2;
		ret;
	};
}
void EGL::CGLEEntity::HookHurtEntity()
{
	if (HookHurtEntity_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		throw std::logic_error("hook hurt entity does not work with SCELoader");
	HookHurtEntity_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
		EGL::CGLEEntity::EntityHurtEntity,
		EGL::CGLEEntity::EntityDealAoEDamage,
		reinterpret_cast<void*>(0x5113C2),
		reinterpret_cast<void*>(0x4DBA20),
		reinterpret_cast<void*>(0x511634),
		reinterpret_cast<void*>(0x50CA59),
		reinterpret_cast<void*>(0x4FE722),
		reinterpret_cast<void*>(0x4FF4EB),
		reinterpret_cast<void*>(0x506B28),
		reinterpret_cast<void*>(0x4F1E77),
		reinterpret_cast<void*>(0x4DC6D9)
	} };
	CppLogic::Hooks::WriteJump(EGL::CGLEEntity::EntityHurtEntity, &hurtentity_override, reinterpret_cast<void*>(0x49F35D));
	CppLogic::Hooks::WriteJump(EGL::CGLEEntity::EntityDealAoEDamage, &hurtaoe_override, reinterpret_cast<void*>(0x49F82F));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5113C2), &arrowonhit_damage, reinterpret_cast<void*>(0x5113CF));
	// projectile creator bigger zero for AdvancedDamageSourceOverride
	*reinterpret_cast<byte*>(0x4DBA20) = 0x89; // mov [esi+0x44], al -> mov [esi+0x44], eax
	// arrow projcetile AdvancedDamageSourceOverride
	*reinterpret_cast<byte*>(0x511634) = 0x8B; // mov al, [edi+0x44] -> mov eax, [edi+0x44]
	*reinterpret_cast<byte*>(0x511637) = 0x89; // mov [esi+0xC8], al -> mov [esi+0xC8], eax
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50CA59), &meleeonhit_damage, reinterpret_cast<void*>(0x50CA6D));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FE722), &circularatt_damage, reinterpret_cast<void*>(0x4FE72F));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FF4EB), &cannonballhit_damage, reinterpret_cast<void*>(0x4FF51B));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x506B28), &bombexplode_damage, reinterpret_cast<void*>(0x506B3F));
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4F1E77), &kegdealdmgproxy);
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4DC6D9), &shurikenthrow, reinterpret_cast<void*>(0x4DC6E2));

	HookDamageMod(); // set projectile player field in creator
	GGL::CBombPlacerBehavior::FixBombAttachment();
	GGL::CCannonBallEffect::AddDamageSourceOverride = true;
}
void EGL::CGLEEntity::AdvancedHurtEntityBy(EGL::CGLEEntity* attacker, int damage, int attackerFallback, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo)
{
	if ((*GGL::CGLGameLogic::GlobalObj)->GlobalInvulnerability)
		return;
	if (Health <= 0)
		return;
	EGL::CEventGetValue_Bool getbool{ shok::EventIDs::IsSettlerOrBuilding };
	FireEvent(&getbool);
	if (!getbool.Data && !dynamic_cast<GGL::CBridgeEntity*>(this))
		return;
	if (GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING) || GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE))
		return;
	int attackerplayer = attacker ? attacker->PlayerId : attackerFallback;
	if (attacker || EGL::CGLEEntity::HurtEntityCallWithNoAttacker) {
		EGL::CEvent2Entities ev{ shok::EventIDs::LogicEvent_HurtEntity, attacker ? attacker->EntityId : 0, EntityId };
		EGL::CGLEEntity::HurtEntityDamagePointer = &damage;
		EGL::CGLEEntity::HurtEntityDamageSource = sourceInfo;
		EGL::CGLEEntity::HurtEntityAttackerPlayer = attackerplayer;
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
		EGL::CGLEEntity::HurtEntityDamagePointer = nullptr;
	}
	if (damage <= 0)
		return;
	getbool = EGL::CEventGetValue_Bool{ shok::EventIDs::IsWorker };
	FireEvent(&getbool);
	if (attacker) {
		if (!getbool.Data || !(*EGL::CGLEGameLogic::GlobalObj)->Landscape->IsPosBlockedInMode(&Position, EGL::CGLELandscape::BlockingMode::Blocked)) {
			if (!GGL::CPlayerStatus::ArePlayersFriendly(PlayerId, attacker->PlayerId)) {
				EGL::CEvent1Entity ev{ shok::EventIDs::OnAttackedBy, attacker->EntityId };
				FireEvent(&ev);
			}
		}
	}
	if (uiFeedback) {
		if (attackerplayer) {
			GGL::CFeedbackEventBattling ev{ shok::FeedbackEventIds::FEEDBACK_EVENT_BATTLING, EntityId, PlayerId, attacker ? attacker->Position : Position, attackerplayer };
			EGUIX::FeedbackEventHandler::GlobalObj()->FireEvent(&ev);
		}
		EGL::CNetEventEntityIDAndInteger ev{ shok::FeedbackEventIds::FEEDBACK_EVENT_ENTITY_HURT, EntityId, damage };
		EGUIX::FeedbackEventHandler::GlobalObj()->FireEvent(&ev);
	}
	if (attacker) {
		for (auto& a : ObserverEntities) {
			if (a.AttachmentType == shok::AttachmentType::GUARD_GUARDED) {
				EGL::CEvent1Entity ev{ shok::EventIDs::Leader_OnGuardedAttackedBy, attacker->EntityId };
				EGL::CGLEEntity::GetEntityByID(a.EntityId)->FireEvent(&ev);
			}
		}
	}

	getbool = EGL::CEventGetValue_Bool{ shok::EventIDs::IsBattleOrAutocannon };
	FireEvent(&getbool);
	EGL::CEventGetValue_Bool getbool2{ shok::EventIDs::IsSerfOrWorker };
	FireEvent(&getbool2);
	std::vector<int> idskilled{};
	int xptoadd = 0;
	EGL::CGLEEntity* firsttodie = this;
	int damageDone = 0;
	if (getbool.Data && !getbool2.Data) { // has potentially soldiers
		getbool = EGL::CEventGetValue_Bool{ shok::EventIDs::IsSoldier };
		FireEvent(&getbool);
		EGL::CGLEEntity* attackedleader = this;
		if (getbool.Data) {
			int id = this->GetFirstAttachedToMe(shok::AttachmentType::LEADER_SOLDIER);
			if (id)
				attackedleader = EGL::CGLEEntity::GetEntityByID(id);
		}
		GGL::CLeaderBehavior* lbeh = attackedleader->GetBehavior<GGL::CLeaderBehavior>();
		if (lbeh && firsttodie == attackedleader) {
			int id = attackedleader->GetFirstAttachedEntity(shok::AttachmentType::LEADER_SOLDIER);
			if (id)
				firsttodie = EGL::CGLEEntity::GetEntityByID(id);
		}
		if (lbeh) {
			int troophp = lbeh->GetTroopHealth();
			int hppersol = lbeh->GetTroopHealthPerSoldier();
			int currentsol = 0;
			for (auto& a : attackedleader->ObservedEntities) {
				if (a.AttachmentType == shok::AttachmentType::LEADER_SOLDIER)
					currentsol++;
			}
			while (troophp > 0 && damage > 0) {
				int thissolhp = troophp - ((currentsol - 1) * hppersol);
				if (damage >= thissolhp) {
					damage -= thissolhp;
					troophp -= thissolhp;
					damageDone += thissolhp;
					attackedleader->DetachObservedEntity(shok::AttachmentType::LEADER_SOLDIER, firsttodie->EntityId, false);
					currentsol--;
					idskilled.push_back(firsttodie->EntityId);
					xptoadd += firsttodie->GetEntityType()->LogicProps->ExperiencePoints;
					GGL::CEventEntityIndex kev{ shok::EventIDs::CppL_OnEntityKilled, attacker ? attacker->EntityId : 0, attackerplayer };
					firsttodie->FireEvent(&kev);
					if (EGL::CGLEEntity::HurtEntityOnKillCb)
						EGL::CGLEEntity::HurtEntityOnKillCb(attacker, firsttodie, attackerplayer, sourceInfo);
					firsttodie->Hurt(firsttodie->Health);

					int id = attackedleader->GetFirstAttachedEntity(shok::AttachmentType::LEADER_SOLDIER);
					if (id)
						firsttodie = EGL::CGLEEntity::GetEntityByID(id);
					else
						firsttodie = attackedleader;
				}
				else {
					damageDone += damage;
					troophp -= damage;
					damage = 0;
					break;
				}
			}
			lbeh->TroopHealthCurrent = troophp;
			firsttodie = attackedleader;
		}
	}
	if (damage > 0) {
		if (damage >= firsttodie->Health) {
			damageDone += firsttodie->Health;
			idskilled.push_back(firsttodie->EntityId);
			xptoadd += firsttodie->GetEntityType()->LogicProps->ExperiencePoints;
			GGL::CEventEntityIndex kev{ shok::EventIDs::CppL_OnEntityKilled, attacker ? attacker->EntityId : 0, attackerplayer };
			firsttodie->FireEvent(&kev);
			if (EGL::CGLEEntity::HurtEntityOnKillCb)
				EGL::CGLEEntity::HurtEntityOnKillCb(attacker, firsttodie, attackerplayer, sourceInfo);
			firsttodie->Hurt(firsttodie->Health);
		}
		else {
			damageDone += damage;
			firsttodie->Hurt(damage);
		}
	}
	if (attacker) {
		GGL::CEventEntityIndex dmgdone{ shok::EventIDs::CppL_OnDamageDealt, this->EntityId, damageDone };
		attacker->FireEvent(&dmgdone);
	}
	if (xp && attacker && xptoadd) {
		GGL::CLeaderBehavior* al = attacker->GetBehavior<GGL::CLeaderBehavior>();
		if (al) {
			al->Experience += xptoadd;
		}
	}
	if (idskilled.size() == 0)
		return;

	const char* callback;
	if (dynamic_cast<GGL::CBuilding*>(this)) {
		if (addStat) {
			if (attackerplayer)
				(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(attackerplayer)->Statistics.NumberOfBuildingsDestroyed += idskilled.size();
			if (GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(this->PlayerId))
				ps->Statistics.NumberOfBuildingsLost += idskilled.size();
		}
		callback = "GameCallback_BuildingDestroyed";
	}
	else {
		if (addStat) {
			if (attackerplayer)
				(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(attackerplayer)->Statistics.NumberOfUnitsKilled += idskilled.size();
			if (GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(this->PlayerId))
				ps->Statistics.NumberOfUnitsDied += idskilled.size();
		}
		callback = "GameCallback_SettlerKilled";
	}
	if (attackerplayer || EGL::CGLEEntity::HurtEntityCallWithNoAttacker) {

		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		int t = L.GetTop();
		L.Push(callback);
		L.GetTableRaw(L.GLOBALSINDEX);
		L.Push(attackerplayer);
		L.Push(this->PlayerId);
		L.Push(attacker ? attacker->EntityId : 0);
		L.CheckStack(idskilled.size());
		for (int i : idskilled) {
			L.Push(i);
		}
		L.PCall(3 + idskilled.size(), 0, 0);
		L.SetTop(t);
	}
}
void __stdcall EGL::CGLEEntity::AdvancedDealAoEDamage(EGL::CGLEEntity* attacker, const shok::Position& center, float range, int damage, int player, int damageclass, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo)
{
	if ((*GGL::CGLGameLogic::GlobalObj)->GlobalInvulnerability)
		return;
	if (range <= 0)
		return;
	int pl = attacker ? attacker->PlayerId : player;
	CppLogic::Iterator::EntityPredicateIsCombatRelevant irel{};
	CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> icircl{ center, range * range };
	CppLogic::Iterator::EntityPredicateIsAlive iali{};
	CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 3> p{ &irel, &icircl, &iali };

	auto lam = [range, damage, damageclass, attacker, pl, uiFeedback, xp, addStat, sourceInfo](EGL::CGLEEntity* curr, float cr) {
		cr = std::sqrtf(cr) / range;
		if (cr < 0 || cr > 1)
			cr = 0;
		else
			cr = 1 - cr * cr;

		if (cr != 0) {
			EGL::CEventGetValue_Int getac{ shok::EventIDs::GetArmorClass };
			curr->FireEvent(&getac);
			EGL::CEventGetValue_Int geta{ shok::EventIDs::GetArmor };
			curr->FireEvent(&geta);

			float dmg = damage * cr;
			if (damageclass > 0 && damageclass < static_cast<int>((*GGL::DamageClassesHolder::GlobalObj)->DamageClassList.size()))
				dmg *= (*GGL::DamageClassesHolder::GlobalObj)->DamageClassList[damageclass]->GetBonusVsArmorClass(getac.Data);
			dmg -= geta.Data;

			if (dmg < 1)
				dmg = 1;

			curr->AdvancedHurtEntityBy(attacker, static_cast<int>(dmg), pl, uiFeedback, xp, addStat, sourceInfo);
		}
	};

	if (pl) {
		CppLogic::Iterator::MultiPlayerEntityIterator it{ &p };
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, pl);
		for (auto& ei : it.ExtendedIterate()) {
			lam(ei.Object, ei.Range);
		}
	}
	else {
		CppLogic::Iterator::GlobalEntityIterator it{ &p };
		for (auto& ei : it.ExtendedIterate()) {
			lam(ei.Object, ei.Range);
		}
	}
}

std::multimap<int, int> EGL::CGLEEntity::BuildingMaxHpTechBoni = std::multimap<int, int>();
bool EGL::CGLEEntity::UseMaxHPTechBoni = false;
int __fastcall hookGetMaxHP(EGL::CGLEEntity* e) {
	float hp;
	EGL::CGLEEntity::EntityAddonData* d = e->GetAdditionalData(false);
	GGlue::CGlueEntityProps* et = e->GetEntityType();
	if (d && d->HealthOverride > 0) {
		hp = static_cast<float>(d->HealthOverride);
	}
	else {
		hp = static_cast<float>(et->LogicProps->MaxHealth);
	}
	if (d && !d->HealthUseBoni)
		return static_cast<int>(hp);
	if (EGL::CGLEEntity::UseMaxHPTechBoni)
	{
		if (dynamic_cast<GGL::CSettler*>(e)) {
			for (int t : static_cast<GGL::CGLSettlerProps*>(et->LogicProps)->ModifyHitpoints.TechList) {
				if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != shok::TechState::Researched)
					continue;
				shok::Technology* tech = (*GGL::CGLGameLogic::GlobalObj)->GetTech(t);
				hp = tech->HitpointModifier.ModifyValue(hp);
			}
		}
		else if (dynamic_cast<GGL::CSettler*>(e)) {
			std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> it = EGL::CGLEEntity::BuildingMaxHpTechBoni.equal_range(e->EntityType);
			for (std::multimap<int, int>::iterator i = it.first; i != it.second; ++i) {
				int t = i->second;
				if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != shok::TechState::Researched)
					continue;
				shok::Technology* tech = (*GGL::CGLGameLogic::GlobalObj)->GetTech(t);
				hp = tech->HitpointModifier.ModifyValue(hp);
			}
		}
	}
	return (int)hp;
}
void __declspec(naked) hookgetmaxhpui() {
	__asm {
		mov ecx, [ebp + 0xC];
		call hookGetMaxHP;
		push eax;
		fild[esp];
		pop eax;
		push 0x4BDEE0;
		ret;
	}
}
void __fastcall createentityfixhp(EGL::CGLEEntity* th, int hpIn) {
	if (hpIn != -1)
		return;
	if (dynamic_cast<GGL::CSettler*>(th)) {
		th->Health = th->GetMaxHealth();
	}
	else if (GGL::CBuilding* b = dynamic_cast<GGL::CBuilding*>(th)) {
		th->Health = th->GetMaxHealth();
		if (!b->IsConstructionFinished()) {
			th->Health = static_cast<int>(th->Health * (*GGL::CLogicProperties::GlobalObj)->ConstructionSiteHealthFactor);
		}
	}
}
void __declspec(naked) hookcreatentityfixhp() {
	__asm {
		mov ecx, esi;
		pop edx; // creator, param for eax+0x14
		mov eax, [edx + 7 * 4]; // health
		push eax; // push health, then old param
		push edx;
		mov eax, [ecx];
		call[eax + 0x14]; // call
		mov ecx, esi; // get params
		pop edx;
		call createentityfixhp; // hp fix
		push 0x571B98;
		ret;
	}
}
bool HookMaxHP_Hooked = false;
void EGL::CGLEEntity::HookMaxHP()
{
	if (HookMaxHP_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		throw std::logic_error{ "does not work with SCELoader" };
	HookMaxHP_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x16, {
		reinterpret_cast<void*>(0x57B798),
		reinterpret_cast<void*>(0x4BDED8),
		reinterpret_cast<void*>(0x571B93)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x57B798), &hookGetMaxHP, reinterpret_cast<void*>(0x57B7AD));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4BDED8), &hookgetmaxhpui, reinterpret_cast<void*>(0x4BDEE0));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x571B93), &hookcreatentityfixhp, reinterpret_cast<void*>(0x571B98));
}

int (*EGL::CGLEEntity::LuaTaskListCallback)(EGL::CGLEEntity* e, int val) = nullptr;
int EGL::CGLEEntity::ExecuteLuaTask(EGL::CTaskArgsInteger* arg)
{
	int i = 0;
	int val = static_cast<EGL::CTaskArgsInteger*>(arg)->Value;
	if (EGL::CGLEEntity::LuaTaskListCallback)
		i = EGL::CGLEEntity::LuaTaskListCallback(this, val);
	if (i == 3) {
		GetAdditionalData(true)->FakeTaskValue = val;
		SetTaskState(shok::TaskState::LuaFunc);
		i = 1;
	}
	return i;
}
int EGL::CGLEEntity::ExecuteLuaTaskState(int p)
{
	int i = -2;
	if (EGL::CGLEEntity::LuaTaskListCallback)
		if (EGL::CGLEEntity::LuaTaskListCallback(this, GetAdditionalData(true)->FakeTaskValue) == 3)
			i = -1;
	return i;
}
void(__thiscall* const entityaddluatlhandlers)(EGL::CGLEEntity* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEntity*)>(0x57BA34);
void __fastcall EGL::CGLEEntity::AddHandlerLuaTask(EGL::CGLEEntity* th) {
	entityaddluatlhandlers(th);
	th->AddTaskHandler(shok::Task::TASK_LUA_FUNC, CreateTaskHandler<static_cast<int>(shok::Task::TASK_LUA_FUNC)>(th, &ExecuteLuaTask));
	th->AddStateHandler(shok::TaskState::LuaFunc, CreateStateHandler(th, &ExecuteLuaTaskState));
}
bool HookLuaTaskList_Hooked = false;
void EGL::CGLEEntity::HookLuaTaskList()
{
	if (HookLuaTaskList_Hooked)
		return;
	HookLuaTaskList_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57D6CA), 10 };
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x57D6CA), &EGL::CGLEEntity::AddHandlerLuaTask);
}

class CustomTaskHandler : public EGL::TaskHandler {
public:
	GGL::CGLBehaviorAnimationEx* th = nullptr;

	virtual int Handle(EGL::CGLETaskArgs* args) override {
		int wait = static_cast<EGL::CGLETaskArgsThousandths*>(args)->Thousandths;
		EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
		th->TurnToWaitFor = th->StartTurn + th->Duration * wait / 1000;
		e->SetTaskState(shok::TaskState::WaitForAnimNonCancelable);
		return 1;
	}
};
static inline int(__thiscall* const behanim_statehandlerwait)(GGL::CGLBehaviorAnimationEx* th, int i) = reinterpret_cast<int(__thiscall*)(GGL::CGLBehaviorAnimationEx*, int)>(0x587E20);
class CustomStateHandler : public EGL::IGLEStateHandler {
public:
	GGL::CGLBehaviorAnimationEx* th = nullptr;

	virtual int Handle(int i) override {
		return behanim_statehandlerwait(th, i);
	}
};

void __stdcall entity_hooknoncancelanim(EGL::CGLEEntity* th, GGL::CGLBehaviorAnimationEx* beh) {
	CustomTaskHandler* thand = new (shok::Malloc(sizeof(CustomTaskHandler))) CustomTaskHandler();
	thand->th = beh;
	th->AddTaskHandler(shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE, thand);
	CustomStateHandler* shand = new (shok::Malloc(sizeof(CustomStateHandler))) CustomStateHandler();
	shand->th = beh;
	th->AddStateHandler(shok::TaskState::WaitForAnimNonCancelable, shand);
}
void __declspec(naked) entity_hooknoncancelanim_asm() {
	__asm {
		mov ecx, esi;
		call[eax + 0x48];

		push edi;
		push esi;
		call entity_hooknoncancelanim;

		mov ecx, [ebp + 0xC];
		push 0x588410;
		ret;
	};
}
bool HookNonCancelableAnim_Hooked = false;
void EGL::CGLEEntity::HookNonCancelableAnim()
{
	if (HookNonCancelableAnim_Hooked)
		return;
	HookNonCancelableAnim_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x588408), 0x588410 - 0x588408 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x588408), &entity_hooknoncancelanim_asm, reinterpret_cast<void*>(0x588410));
}

void(__thiscall* movementbeh_setmovetarget)(GGL::CBehaviorDefaultMovement* m, shok::Position* p) = reinterpret_cast<void(__thiscall*)(GGL::CBehaviorDefaultMovement*, shok::Position*)>(0x586894);
void __fastcall entity_buildonsetpos(EGL::CMovingEntity* e) {
	if (e->MovementState && EGL::CGLEEntity::BuildOnSetPosFixMovement) {
		GGL::CBehaviorDefaultMovement* mov = e->GetBehavior<GGL::CBehaviorDefaultMovement>();
		if (mov) {
			movementbeh_setmovetarget(mov, &e->TargetPosition);
		}
	}
	else {
		BB::CEvent ev{ 0x2000D };
		e->FireEvent(&ev);
		if (e->MovementState == 0) {
			ev.EventTypeId = 0x11017;
			e->FireEvent(&ev);
			e->MovementState = 0; // ?? original code
		}
	}
}
void __declspec(naked) entity_buildonsetpos_asm() {
	__asm {
		mov ecx, edi;
		call entity_buildonsetpos;

		push 0x4ADB76;
		ret;
	};
}
bool EGL::CGLEEntity::BuildOnSetPosFixMovement = false;
bool HookBuildOnSetPos_Hooked = false;
void EGL::CGLEEntity::HookBuildOnSetPos()
{
	if (HookBuildOnSetPos_Hooked)
		return;
	HookBuildOnSetPos_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4ADB16), 0x4ADB76 - 0x4ADB16 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4ADB16), &entity_buildonsetpos_asm, reinterpret_cast<void*>(0x4ADB76));
}

void __declspec(naked) HookSetTaskListNonCancelable_asm() {
	__asm {
		cmp dword ptr[ecx + 34 * 4], 501;
		je ca;


		mov eax, [esi + 0x8c];
		push 0x57B229;
		ret;

	ca:
		mov eax, [ebp + 8];
		mov eax, [eax + 20];
		mov[esi + 36 * 4], eax;
		mov[esi + 37 * 4], 0;

		push 0x57B3A6;
		ret;
	};
}
long long HookSetTaskListNonCancelable_backup = 0;
bool HookSetTaskListNonCancelable_Active = false;
byte HookSetTaskListNonCancelable_Backup[0x57B229 - 0x57B223 + 1]{};
void EGL::CGLEEntity::HookSetTaskListNonCancelable(bool active)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57B223), 0x57B229 - 0x57B223 };
	if (active) {
		if (!HookSetTaskListNonCancelable_Active) {
			CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x57B223), &HookSetTaskListNonCancelable_asm, reinterpret_cast<void*>(0x57B229), HookSetTaskListNonCancelable_Backup);
			HookSetTaskListNonCancelable_Active = true;
		}
	}
	else {
		if (HookSetTaskListNonCancelable_Active) {
			CppLogic::Hooks::RestoreJumpBackup(reinterpret_cast<void*>(0x57B223), HookSetTaskListNonCancelable_Backup);
			HookSetTaskListNonCancelable_Active = false;
		}
	}
}


EGL::CGLEEntity* EGL::CGLEEntity::ReplaceEntityWithResourceEntity(EGL::CGLEEntity* e)
{
	EGL::CGLEEntityProps* ty = e->GetEntityType()->LogicProps;
	if (typeid(*ty) != typeid(GGL::CEntityProperties))
		return nullptr;
	GGL::CEntityProperties* t = static_cast<GGL::CEntityProperties*>(ty);
	GGL::CResourceDoodadCreator c{};
	c.EntityType = t->ResourceEntity;
	c.PlayerId = e->PlayerId;
	c.Pos = e->Position;
	c.ResourceAmount = t->ResourceAmount;
	c.Scale = e->Scale;
	if (e->ScriptName) {
		size_t len = strlen(e->ScriptName) + 1;
		c.ScriptName = (char*)shok::Malloc(sizeof(char) * len);
		strcpy_s(c.ScriptName, len, e->ScriptName);
	}
	else {
		c.ScriptName = nullptr;
	}
	int id = (*EGL::CGLEGameLogic::GlobalObj)->CreateEntity(&c);
	EGL::CGLEEntity* r = EGL::CGLEEntity::GetEntityByID(id);
	EGL::CEventValue_Int ev{ shok::EventIDs::Tree_Init, e->EntityType };
	r->FireEvent(&ev);
	e->Destroy();
	return r;
}


std::map<int, EGL::CGLEEntity::EntityAddonData> EGL::CGLEEntity::AddonDataMap = std::map<int, EGL::CGLEEntity::EntityAddonData>();
EGL::CGLEEntity::EntityAddonData EGL::CGLEEntity::LastRemovedEntityAddonData = EGL::CGLEEntity::EntityAddonData();
void __fastcall destroyentity_gemoveadd(EGL::CGLEEntity* e) {
	BB::CEvent ev{ shok::EventIDs::CppL_OnEntityDestroy };
	e->FireEvent(&ev);
	auto a = EGL::CGLEEntity::AddonDataMap.find(e->EntityId);
	if (a != EGL::CGLEEntity::AddonDataMap.end()) {
		EGL::CGLEEntity::LastRemovedEntityAddonData = a->second;
		EGL::CGLEEntity::AddonDataMap.erase(a);
	}
}
void __declspec(naked) destroyentityhook() {
	__asm {
		push ecx;
		call destroyentity_gemoveadd;
		pop ecx;

		push esi;
		mov esi, ecx;
		cmp byte ptr[esi + 0x6D], 0;
		jnz jump;
		push 0x57C957;
		ret;
	jump:
		push 0x57C9DE;
		ret;
	}

}
bool HookDestroyEntity_Hooked = false;
void EGL::CGLEEntity::HookDestroyEntity()
{
	if (HookDestroyEntity_Hooked)
		return;
	HookDestroyEntity_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57C94A), 0x57C957 - 0x57C94A };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x57C94A), &destroyentityhook, reinterpret_cast<void*>(0x57C957));
}
EGL::CGLEEntity::EntityAddonData* EGL::CGLEEntity::GetAdditionalData(bool create)
{
	auto a = EGL::CGLEEntity::AddonDataMap.find(EntityId);
	if (a != EGL::CGLEEntity::AddonDataMap.end()) {
		return &a->second;
	}
	if (!create) {
		return nullptr;
	}
	else {
		EGL::CGLEEntity::AddonDataMap[EntityId] = EGL::CGLEEntity::EntityAddonData();
		EGL::CGLEEntity::EntityAddonData* r = &EGL::CGLEEntity::AddonDataMap[EntityId];
		r->EntityId = EntityId;
		return r;
	}
}
void EGL::CGLEEntity::CloneAdditionalDataFrom(EGL::CGLEEntity::EntityAddonData* other)
{
	if (other) {
		EGL::CGLEEntity::EntityAddonData* t = GetAdditionalData(true);
		t->HealthOverride = other->HealthOverride;
		t->HealthUseBoni = other->HealthUseBoni;
		t->DamageOverride = other->DamageOverride;
		t->ArmorOverride = other->ArmorOverride;
		t->ExplorationOverride = other->ExplorationOverride;
		t->RegenHPOverride = other->RegenHPOverride;
		t->RegenSecondsOverride = other->RegenSecondsOverride;
		t->MaxRangeOverride = other->MaxRangeOverride;
		t->NameOverride = other->NameOverride;
		t->FakeTaskValue = other->FakeTaskValue;
	}
}


float __fastcall entitygetdamagemod(GGL::CBattleBehavior* b) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(b->EntityId);
	EGL::CGLEEntity::EntityAddonData* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		return static_cast<float>(d->DamageOverride);
	else
		return static_cast<float>(e->GetEntityType()->GetBehaviorProps<GGL::CBattleBehaviorProps>()->DamageAmount);
}
void __declspec(naked) entitydamagemodeventbattleasm() {
	__asm {
		sub esp, 0x24;
		push ebx;
		push esi;
		mov esi, ecx;

		call entitygetdamagemod;

		push esi;
		push 0x50C793;
		ret;
	}
}
void __declspec(naked) entitydamagemodbattlecalcsingletargetdmgasm() {
	__asm {
		pushad;

		mov ecx, esi;
		call entitygetdamagemod;

		popad;

		push 0x50C23B;
		ret;
	}
}
EGL::CEventGetValue_Int* __fastcall entitydamagemodeventautocannonasm(GGL::CAutoCannonBehavior* th, int, EGL::CEventGetValue_Int* ev) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	EGL::CGLEEntity::EntityAddonData* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		ev->Data = d->DamageOverride;
	else
		ev->Data = e->GetEntityType()->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>()->DamageAmount;
	return ev;
}
void __declspec(naked) entitydamagemodbattleprojectile() {
	__asm {
		test al, al;
		jz arrow;

		mov ecx, [ebp - 0x10]; // attacker obj
		mov eax, [ecx + 6 * 4]; // player
		mov[ebp - 0x28], eax; // creator source player
		mov[ebp - 0x5C], eax; // creator player

		call EGL::CGLEEntity::EventGetDamage;
		push 0x50C3F7;
		ret;

	arrow:

		mov ecx, [ebp - 0x10]; // attacker obj
		mov eax, [ecx + 6 * 4]; // player
		mov[ebp - 0x5C], eax; // creator player

		
		push esi;
		mov ecx, edi;
		mov eax, 0x50C1CD;
		call eax;
		push 0x50C3F7;
		ret;
	}
}
void __declspec(naked) entitydamagemodautocannonprojectileasm() {
	__asm {
		push ecx;

		mov ecx, edi;
		mov eax, [ecx + 6 * 4]; // player
		mov[ebp - 0x38], eax; // source player
		call EGL::CGLEEntity::EventGetDamage;
		mov[ebp - 0x44], eax; // damage

		pop ecx;

		push 0x510776;
		ret;
	}
}
bool HookDamageMod_Hooked = false;
void EGL::CGLEEntity::HookDamageMod()
{
	if (HookDamageMod_Hooked)
		return;
	HookDamageMod_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x50C785),
		reinterpret_cast<void*>(0x50C235),
		reinterpret_cast<void*>(0x50F5ED),
		reinterpret_cast<void*>(0x50C3E7),
		reinterpret_cast<void*>(0x51076C),
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50C785), &entitydamagemodeventbattleasm, reinterpret_cast<void*>(0x50C793));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50C235), &entitydamagemodbattlecalcsingletargetdmgasm, reinterpret_cast<void*>(0x50C23B));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50F5ED), &entitydamagemodeventautocannonasm, reinterpret_cast<void*>(0x50F5FD));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50C3E7), &entitydamagemodbattleprojectile, reinterpret_cast<void*>(0x50C3F7));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x51076C), &entitydamagemodautocannonprojectileasm, reinterpret_cast<void*>(0x510776));
}

int __fastcall entityarmormod(EGL::CGLEEntity* e) {
	EGL::CGLEEntity::EntityAddonData* d = e->GetAdditionalData(false);
	if (d && d->ArmorOverride >= 0)
		return d->ArmorOverride;
	GGlue::CGlueEntityProps* p = e->GetEntityType();
	if (p->IsSettlerType())
		return static_cast<GGL::CGLSettlerProps*>(p->LogicProps)->ArmorAmount;
	else if (p->IsBuildingType())
		return static_cast<GGL::CGLBuildingProps*>(p->LogicProps)->ArmorAmount;
	return 0;
}
void __declspec(naked) entityarmormodsettlerasm() {
	__asm {
		mov esi, ecx;
		push[esi + 0x10];

		call entityarmormod;

		push 0x4A6B25;
		ret;
	}
}
void __declspec(naked) entityarmormodbuildingrasm() {
	__asm {
		mov esi, ecx;
		push[esi + 0x10];

		call entityarmormod;
		push eax;
		fild[esp];
		pop eax;

		mov eax, 0x4AAB98;
		call eax;

		push 0x04AB170;
		ret;
	}
}
bool HookArmorMod_Hooked = false;
void EGL::CGLEEntity::HookArmorMod()
{
	if (HookArmorMod_Hooked)
		return;
	HookArmorMod_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x4A6B15),
		reinterpret_cast<void*>(0x4AB160)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4A6B15), &entityarmormodsettlerasm, reinterpret_cast<void*>(0x4A6B25));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4AB160), &entityarmormodbuildingrasm, reinterpret_cast<void*>(0x04AB170));
}

float __fastcall entityexplmod(EGL::CGLEEntity* e) {
	EGL::CGLEEntity::EntityAddonData* d = e->GetAdditionalData(false);
	if (d && d->ExplorationOverride >= 0.0f)
		return d->ExplorationOverride;
	return e->GetEntityType()->LogicProps->Exploration;
}
void __declspec(naked) entityexplmodsettasm() {
	__asm {
		push esi;
		push[esi + 0x10];

		call entityexplmod;

		push 0x4A4AD4;
		ret;
	}
}
void __declspec(naked) entityexplmodbuildasm() {
	__asm {
		push esi;
		push[esi + 0x10];
		mov eax, 0x4AAB98;
		call eax;

		push eax;
		mov ecx, esi;
		call entityexplmod;
		pop eax;

		push 0x4AB1A4;
		ret;
	}
}
bool HookExplorationMod_Hooked = false;
void EGL::CGLEEntity::HookExplorationMod()
{
	if (HookExplorationMod_Hooked)
		return;
	HookExplorationMod_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x4A4AC3),
		reinterpret_cast<void*>(0x4AB199)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4A4AC3), &entityexplmodsettasm, reinterpret_cast<void*>(0x4A4AD4));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4AB199), &entityexplmodbuildasm, reinterpret_cast<void*>(0x4AB1A4));
}

int GGL::CSettler::LeaderGetRegenHealth()
{
	EGL::CGLEEntity::EntityAddonData* d = GetAdditionalData(false);
	if (d && d->RegenHPOverride >= 0)
		return d->RegenHPOverride;
	else
		return GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->HealingPoints;
}
int GGL::CSettler::LeaderGetRegenHealthSeconds()
{
	EGL::CGLEEntity::EntityAddonData* d = GetAdditionalData(false);
	if (d && d->RegenSecondsOverride >= 0)
		return d->RegenSecondsOverride;
	else
		return GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->HealingSeconds;
}
static inline void(__thiscall* const settler_killbyenviro)(GGL::CSettler* th) = reinterpret_cast<void(__thiscall*)(GGL::CSettler*)>(0x4A5049);
void GGL::CSettler::KillSettlerByEnvironment()
{
	settler_killbyenviro(this);
}

static inline void(__thiscall* const settler_upgrade)(GGL::CSettler* th) = reinterpret_cast<void(__thiscall*)(GGL::CSettler*)>(0x4A6C4A);
void GGL::CSettler::Upgrade()
{
	settler_upgrade(this);
}


void EGL::CGLEEntity::PerformHeal(int r, bool healSoldiers)
{
	int hp = Health, mhp = GetMaxHealth();
	if (hp <= 0)
		return;
	if (hp < mhp) {
		int healsel = min(r, mhp - hp);
		hp += healsel;
		r -= healsel;
		SetHealth(hp);
	}
	if (r <= 0)
		return;
	GGL::CLeaderBehavior* lb = GetBehavior<GGL::CLeaderBehavior>();
	if (lb && healSoldiers) {
		int shp = lb->GetTroopHealth(), smp = lb->GetTroopHealthPerSoldier();
		int numsol = 0;
		ObservedEntities.ForAll([&numsol](shok::Attachment* a) { if (a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER) numsol++; });
		smp *= numsol;
		shp += r;
		if (shp > smp)
			shp = smp;
		lb->TroopHealthCurrent = shp;
		return;
	}
}

bool EGL::CGLEEntity::LeaderRegenRegenerateSoldiers = false;
void __fastcall leaderregen(GGL::CLeaderBehavior* th) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	th->SecondsSinceHPRefresh = 0;
	int hp = e->Health, mhp = e->GetMaxHealth();
	if (hp <= 0)
		return;
	int r = static_cast<GGL::CSettler*>(e)->LeaderGetRegenHealth();
	e->PerformHeal(r, EGL::CGLEEntity::LeaderRegenRegenerateSoldiers);
}
void __fastcall leaderregenseconds(GGL::CLeaderBehavior* th) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	int max = static_cast<GGL::CSettler*>(e)->LeaderGetRegenHealthSeconds();
	th->SecondsSinceHPRefresh++;
	if (th->SecondsSinceHPRefresh >= max)
		leaderregen(th);
}
void __declspec(naked) leaderregensecondsasm() {
	__asm {
		mov ecx, esi;
		call leaderregenseconds;
		push 0x4EFC42;
		ret;
	}
}
bool HookLeaderRegen_Hooked = false;
void EGL::CGLEEntity::HookLeaderRegen()
{
	if (HookLeaderRegen_Hooked)
		return;
	HookLeaderRegen_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x4EAE92),
		reinterpret_cast<void*>(0x4EFC29)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4EAE92), &leaderregen, reinterpret_cast<void*>(0x4EAE9B));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4EFC29), &leaderregensecondsasm, reinterpret_cast<void*>(0x4EFC42));
}

float __fastcall leadermaxrange(GGL::CBattleBehavior* th) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	EGL::CGLEEntity::EntityAddonData* d = e->GetAdditionalData(false);
	if (d && d->MaxRangeOverride >= 0)
		return d->MaxRangeOverride;
	else
		return e->GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->MaxRange;
}
void __declspec(naked) leadermaxrangeasm() {
	__asm {
		mov esi, ecx;

		pushad;
		call leadermaxrange;
		popad;

		mov eax, [esi + 0x30];
		push 0x50AB50;
		ret;
	}
}
float __fastcall autocannonmaxrange(GGL::CAutoCannonBehavior* th) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	EGL::CGLEEntity::EntityAddonData* d = e->GetAdditionalData(false);
	if (d && d->MaxRangeOverride >= 0)
		return d->MaxRangeOverride;
	else
		return e->GetEntityType()->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>()->MaxAttackRange;
}
void __declspec(naked) autocannonmaxrangeasm() {
	__asm {
		mov esi, ecx;

		pushad;
		call autocannonmaxrange;
		popad;

		mov eax, [esi + 0x14];
		push 0x50F515;
		ret;
	}
}
bool HookMaxRange_Hooked = false;
void EGL::CGLEEntity::HookMaxRange()
{
	if (HookMaxRange_Hooked)
		return;
	HookMaxRange_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x50AB48),
		reinterpret_cast<void*>(0x50F50D)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50AB48), &leadermaxrangeasm, reinterpret_cast<void*>(0x50AB50));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50F50D), &autocannonmaxrangeasm, reinterpret_cast<void*>(0x50F515));
}

const char* __stdcall entitydisplayname(int* e, int type) {
	// lots of hacks to recover that entity pointer in a safe way
	// there are 2 branches that can end up calling this, with 2 different pointers in eax
	int d[5] = { EGL::CGLEEntity::vtp_IEntityDisplay, GGL::CSettler::vtp_IEntityDisplay, GGL::CBuilding::vtp_IEntityDisplay,
		GGL::CResourceDoodad::vtp_IEntityDisplay, GGL::CBridgeEntity::vtp_IEntityDisplay
	};
	EGL::CGLEEntity* ent = nullptr;
	if (CppLogic::ContainsValue(d, *e, 5)) {
		ent = reinterpret_cast<EGL::CGLEEntity*>(e - 1);
	}
	else {
		int d2[5] = { EGL::CGLEEntity::vtp, GGL::CSettler::vtp, GGL::CBuilding::vtp,
			GGL::CResourceDoodad::vtp, GGL::CBridgeEntity::vtp
		};
		if (CppLogic::ContainsValue(d2, *e, 5)) {
			ent = reinterpret_cast<EGL::CGLEEntity*>(e);
		}
	}
	if (ent) {
		EGL::CGLEEntity::EntityAddonData* d = ent->GetAdditionalData(false);
		if (d && d->NameOverride.size() > 0) {
			return d->NameOverride.c_str();
		}
	}

	return EGL::CGLEEntitiesProps::GetEntityTypeDisplayName(type);
}
void __declspec(naked) entitydisplaynameasm() {
	__asm {
		push eax;
		call entitydisplayname;

		push 0x53F91D;
		ret;
	}
}
bool HookDisplayName_Hooked = false;
void EGL::CGLEEntity::HookDisplayName()
{
	if (HookDisplayName_Hooked)
		return;
	HookDisplayName_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x53F911), 0x53F91D - 0x53F911 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x53F911), &entitydisplaynameasm, reinterpret_cast<void*>(0x53F91D));
}

void __fastcall rangedeffecthealhook(GGL::CRangedEffectAbility* th) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	GGL::CRangedEffectAbilityProps* pr = e->GetEntityType()->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
	float hpfact = pr->HealthRecoveryFactor;
	if (hpfact <= 0)
		return;
	EGL::CGLEEffectCreator ecr{};
	ecr.PlayerID = e->PlayerId;
	ecr.EffectType = pr->HealEffect;
	e->ObservedEntities.ForAll([hpfact, &ecr](shok::Attachment* a) {
		if (a->AttachmentType == shok::AttachmentType::HERO_AFFECTED) {
			EGL::CGLEEntity* toheal = EGL::CGLEEntity::GetEntityByID(a->EntityId);
			if (!toheal)
				return;
			float heal = toheal->GetMaxHealth() * hpfact;
			if (ecr.EffectType) {
				ecr.StartPos.X = toheal->Position.X;
				ecr.StartPos.Y = toheal->Position.Y;
				(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&ecr);
			}
			if (toheal->GetBehavior<GGL::CSoldierBehavior>()) {
				toheal = EGL::CGLEEntity::GetEntityByID(toheal->GetFirstAttachedToMe(shok::AttachmentType::LEADER_SOLDIER));
			}
			if (!toheal)
				return;
			toheal->PerformHeal(static_cast<int>(heal), true);
		}
		});
}
void EGL::CGLEEntity::HookRangedEffectActivateHeal(bool hookActive)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4E3C78), 10 };
	if (hookActive)
		CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4E3C78), &rangedeffecthealhook);
	else
		CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4E3C78), reinterpret_cast<void*>(0x4E39B4));
}

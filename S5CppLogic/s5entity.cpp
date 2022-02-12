#include "pch.h"
#include "s5data.h"
#include <math.h>
#include "entityiterator.h"


struct shok_vtable_EGL_CGLEEntity : shok_vtable_BB_IObject {
	void(__thiscall* Destroy)(shok_EGL_CGLEEntity* th, int i); // 3
	PADDINGI(1);
	void(__thiscall* CopyDataFromCreator)(shok_EGL_CGLEEntity* th, shok_EGL_CGLEEntityCreator* cr); // 5
	PADDINGI(1);
	void(__thiscall* InitializeEntity)(shok_EGL_CGLEEntity* th); // dont use outside of createentity
	void(__thiscall* InitializeAfterSaveLoad)(shok_EGL_CGLEEntity* th); // dont use outside of save load
	void(__thiscall* FireOnCreatedTriggers)(shok_EGL_CGLEEntity* th); // 9, script trigger + scriptcommandline
	void(__thiscall* SetPosition)(shok_EGL_CGLEEntity* th, shok::Position* p); // 10 works for settlers, check if it does for other stuff to, called every tick
	void(__thiscall* SetRotation)(shok_EGL_CGLEEntity* th, float r);
	int(__thiscall* GetSector)(shok_EGL_CGLEEntity* th); // 12
	bool(__thiscall* IsInSector)(shok_EGL_CGLEEntity* th, int sector);
	PADDINGI(1); // maybe execute task?/ontick?, settler update auras
	void(__thiscall* ExecuteTask)(shok_EGL_CGLEEntity* th, shok_EGL_CGLETaskArgs* t); // 15 return values: 2->same task, next tick, 1->next task, next tick, 0->next task, immediately
	void(__thiscall* FireEvent)(shok_EGL_CGLEEntity* th, shok_BB_CEvent* d); // 16
	void(__thiscall* AddBehavior)(shok_EGL_CGLEEntity* th, EGL::CGLEBehavior* bh); // 17 probably not usable outside of createentity
	PADDINGI(1); // add behavior slot
	void(__thiscall* AddTaskHandler)(shok_EGL_CGLEEntity* th, shok_Task task, shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* taskhandler); // 19
	void(__thiscall* AddEventHandler)(shok_EGL_CGLEEntity* th, shok_EventIDs eventid, shok_EGL_IGLEHandler_BB_CEvent_void* eventhandler); // 20
	void(__thiscall* AddStateHandler)(shok_EGL_CGLEEntity* th, shok_TaskState state, shok_EGL_IGLEStateHandler* statehandler); // 21
	void(__thiscall* GetApproachPos)(shok_EGL_CGLEEntity* th, shok::Position* outpos);
	float(__thiscall* GetApproachRot)(shok_EGL_CGLEEntity* th);
	void(__thiscall* SetTaskState)(shok_EGL_CGLEEntity* th, shok_TaskState state); // 24
	PADDINGI(3);
	float(__thiscall* GetExploration)(shok_EGL_CGLEEntity* e); // 28
	PADDINGI(5); // get flags as float, then jmp to 33
	bool(__thiscall* CanCancelCurrentState)(shok_EGL_CGLEEntity* e); // 34
};
//constexpr int i = offsetof(shok_vtable_EGL_CGLEEntity, CanCancelCurrentState) / 4;


struct shok_vtable_EGL_IEntityDisplay {
	void(__thiscall* dtor)(shok_EGL_IEntityDisplay* th);
	void* (__thiscall* GetSlotByIndex)(shok_EGL_IEntityDisplay* th, int i);
	void(__thiscall* GetModelData)(const shok_EGL_IEntityDisplay* th, shok_EGL_IEntityDisplay::modeldata* d);
	void(__thiscall* GetPlayerModelData)(const shok_EGL_IEntityDisplay* th, shok_EGL_IEntityDisplay::playermodeldata* d);
	void(__thiscall* GetPosData)(const shok_EGL_IEntityDisplay* th, shok_EGL_IEntityDisplay::posdata* d);
	PADDINGI(1); // ret zero, 1 arg
};

shok_EGL_IEntityDisplay::modeldata shok_EGL_IEntityDisplay::GetModelData() const
{
	modeldata r;
	(*reinterpret_cast<shok_vtable_EGL_IEntityDisplay* const*>(this))->GetModelData(this, &r);
	return r;
}
shok_EGL_IEntityDisplay::playermodeldata shok_EGL_IEntityDisplay::GetPlayerModelData() const
{
	playermodeldata r;
	(*reinterpret_cast<shok_vtable_EGL_IEntityDisplay* const*>(this))->GetPlayerModelData(this, &r);
	return r;
}
shok_EGL_IEntityDisplay::posdata shok_EGL_IEntityDisplay::GetPosData() const
{
	posdata r;
	(*reinterpret_cast<shok_vtable_EGL_IEntityDisplay* const*>(this))->GetPosData(this, &r);
	return r;
}

static inline void(__thiscall* const shok_EGL_CGLEEntityCreator_ctor)(shok_EGL_CGLEEntityCreator* th) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLEEntityCreator*)>(0x4493A4);
shok_EGL_CGLEEntityCreator::shok_EGL_CGLEEntityCreator()
{
	shok_EGL_CGLEEntityCreator_ctor(this);
}
shok_EGL_CGLEEntityCreator::shok_EGL_CGLEEntityCreator(int _)
{
	shok_EGL_CGLEEntityCreator_ctor(this);
}

static inline void(__thiscall* const shok_GGL_CGLConstructionSiteCreator_ctor)(shok_GGL_CGLConstructionSiteCreator* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CGLConstructionSiteCreator*)>(0x4B8E8F);
shok_GGL_CGLConstructionSiteCreator::shok_GGL_CGLConstructionSiteCreator() : shok_EGL_CGLEEntityCreator(1)
{
	shok_GGL_CGLConstructionSiteCreator_ctor(this);
}
static inline void(__thiscall* const shok_GGL_CResourceDoodadCreator_ctor)(shok_GGL_CResourceDoodadCreator* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CResourceDoodadCreator*)>(0x4DD7E2);
shok_GGL_CResourceDoodadCreator::shok_GGL_CResourceDoodadCreator() : shok_EGL_CGLEEntityCreator(1)
{
	shok_GGL_CResourceDoodadCreator_ctor(this);
}

static inline void(__thiscall* const shok_EGL_CGLEEntityCreator_dtor)(shok_EGL_CGLEEntityCreator* th) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLEEntityCreator*)>(0x449E23);
shok_EGL_CGLEEntityCreator::~shok_EGL_CGLEEntityCreator()
{
	shok_EGL_CGLEEntityCreator_dtor(this);
}

static inline bool(__thiscall* const shok_IsEntityInCategory)(shok_EGL_CGLEEntity* e, shok::EntityCategory cat) = reinterpret_cast<bool(__thiscall*)(shok_EGL_CGLEEntity * e, shok::EntityCategory)>(0x57BBEB);
bool shok_EGL_CGLEEntity::IsEntityInCategory(shok::EntityCategory cat)
{
	return shok_IsEntityInCategory(this, cat);
}

int shok_EGL_CGLEEntity::GetResourceProvided()
{
	return shok_EGL_CGLEEntity::EntityIDGetProvidedResource(this->EntityId);
}

GGlue::CGlueEntityProps* shok_EGL_CGLEEntity::GetEntityType()
{
	return (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(EntityType);
}

int shok_EGL_CGLEEntity::EventGetIntById(shok_EventIDs id)
{
	shok_EGL_CEventGetValue_int d{ id };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &d);
	return d.Data;
}

void shok_EGL_CGLEEntity::FireEvent(shok_BB_CEvent* ev)
{
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, ev);
}

int shok_EGL_CGLEEntity::EventGetDamage()
{
	return EventGetIntById(shok_EventIDs::GetDamage);
}
int shok_EGL_CGLEEntity::EventGetArmor()
{
	return EventGetIntById(shok_EventIDs::GetArmor);
}
int shok_EGL_CGLEEntity::EventGetMaxWorktime()
{
	return EventGetIntById(shok_EventIDs::Worker_GetMaxWorkTime);
}

shok::LeaderCommand shok_EGL_CGLEEntity::EventLeaderGetCurrentCommand()
{
	if (GetBehavior<GGL::CLeaderBehavior>() == nullptr)
		return shok::LeaderCommand::Unknown;
	return static_cast<shok::LeaderCommand>(EventGetIntById(shok_EventIDs::Leader_GetCommand));
}

float shok_EGL_CGLEEntity::GetExploration()
{
	return reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->GetExploration(this);
}

static inline int(__thiscall* const entitygetmaxhealth)(shok_EGL_CGLEEntity* th) = reinterpret_cast<int(__thiscall*)(shok_EGL_CGLEEntity*)>(0x57B798);
int shok_EGL_CGLEEntity::GetMaxHealth()
{
	return entitygetmaxhealth(this);
}

int shok_EGL_CGLEEntity::LimitedAttachmentGetMaximum(shok::AttachmentType attachType)
{
	shok_GGL_CEventAttachmentTypeGetInteger ev{ shok_EventIDs::LimitedAttachment_GetMax, attachType };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	return ev.Data;
}
int shok_EGL_CGLEEntity::GetSector()
{
	return reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->GetSector(this);
}
int shok_EGL_CGLEEntity::ResourceTreeGetNearestSector()
{
	shok::Position p = Position;
	shok::Position p2 = (*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape->GetNearestFreePos(&p, 600);
	if (!(*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape->IsValidPos(&p2)) {
		return 0;
	}
	return (*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p2);
}


int shok_EGL_CGLEEntity::GetFirstAttachedToMe(shok::AttachmentType attachmentId)
{
	shok::Attachment* r = ObserverEntities.GetFirstMatch([attachmentId](shok::Attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}

int shok_EGL_CGLEEntity::GetFirstAttachedEntity(shok::AttachmentType attachmentId)
{
	shok::Attachment* r = ObservedEntities.GetFirstMatch([attachmentId](shok::Attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}
static inline void(__thiscall* entattach_attach)(int* th, shok::AttachmentType at, int id, shok_EventIDs evth, shok_EventIDs evot) = reinterpret_cast<void(__thiscall*)(int*, shok::AttachmentType, int, shok_EventIDs, shok_EventIDs)>(0x4A61B3);
void shok_EGL_CGLEEntity::AttachEntity(shok::AttachmentType attachtype, int otherId, shok_EventIDs eventIdOnThisDetach, shok_EventIDs eventIdOnOtherDetach)
{
	entattach_attach(&attachmentvt, attachtype, otherId, eventIdOnThisDetach, eventIdOnOtherDetach);
}
static inline void(__thiscall* entattach_detach)(int* th, shok::AttachmentType at, int id, byte ev) = reinterpret_cast<void(__thiscall*)(int*, shok::AttachmentType, int, byte)>(0x4A2E5D);
void shok_EGL_CGLEEntity::DetachObservedEntity(shok::AttachmentType attachtype, int otherId, bool fireEvent)
{
	entattach_detach(&attachmentvt, attachtype, otherId, fireEvent);
}

int shok_EGL_CMovingEntity::LeaderGetNearbyBarracks()
{
	return EventGetIntById(shok_EventIDs::Leader_GetNearbyBarracks);
}

bool shok_EGL_CMovingEntity::IsMoving()
{
	shok_EGL_CEventGetValue_bool d{ shok_EventIDs::Movement_IsMoving };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &d);
	return d.Data;
}

bool shok_GGL_CSettler::IsIdle()
{
	shok::LeaderCommand com = EventLeaderGetCurrentCommand();
	return com == shok::LeaderCommand::Defend || com == shok::LeaderCommand::HoldPos || CurrentTaskListID == static_cast<GGL::CGLSettlerProps*>(GetEntityType()->LogicProps)->IdleTaskList;
}

static inline void(__thiscall* const shok_GGL_CResourceDoodad_setresam)(shok_GGL_CResourceDoodad* th, int am) = reinterpret_cast<void(__thiscall* const)(shok_GGL_CResourceDoodad*, int)>(0x4B864B);
void shok_GGL_CResourceDoodad::SetCurrentResourceAmount(int am)
{
	shok_GGL_CResourceDoodad_setresam(this, am);
}

int shok_GGL_CBuilding::GetConstructionSite()
{
	return GetFirstAttachedToMe(shok::AttachmentType::CONSTRUCTION_SITE_BUILDING);
}

static inline int(__thiscall* const shok_build_getnearestfreeslot)(shok_EGL_CGLEEntity* th, shok::Position* p) = reinterpret_cast<int(__thiscall*) (shok_EGL_CGLEEntity*, shok::Position*)>(0x4AB02D);
int shok_GGL_CBuilding::GetNearestFreeConstructionSlotFor(shok::Position* p)
{
	int cid = GetConstructionSite();
	if (!cid)
		return -1;
	shok_EGL_CGLEEntity* consi = shok_EGL_CGLEEntity::GetEntityByID(cid);
	if (!consi)
		return -1;
	return shok_build_getnearestfreeslot(consi, p);
}

int shok_GGL_CBuilding::GetNearestFreeRepairSlotFor(shok::Position* p)
{
	return shok_build_getnearestfreeslot(this, p);
}

bool shok_GGL_CBuilding::IsConstructionFinished()
{
	return BuildingHeight >= 1;
}

bool shok_GGL_CBuilding::IsIdle()
{
	return IsIdle(false);
}

bool shok_GGL_CBuilding::IsIdle(bool forRecruitemnt)
{
	if (!IsConstructionFinished())
		return false;
	if (IsUpgrading)
		return false;
	if (CurrentState == shok_TaskState::BuildingAlarmDefend) // alarm mode
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
				if (a->AttachmentType == shok::AttachmentType::FIGHTER_BARRACKS && !shok_EGL_CGLEEntity::GetEntityByID(a->EntityId)->GetBehavior<GGL::CSoldierBehavior>())
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

static inline int(__thiscall* building_gettechinres)(shok_GGL_CBuilding* th) = reinterpret_cast<int(__thiscall*)(shok_GGL_CBuilding*)>(0x4AAD09);
int shok_GGL_CBuilding::GetTechnologyInResearch()
{
	return building_gettechinres(this);
}

int shok_GGL_CBuilding::GetCannonProgress()
{
	return EventGetIntById(shok_EventIDs::Foundry_GetProgress);
}

float shok_GGL_CBuilding::GetMarketProgress()
{
	shok_EGL_CEventGetValue_float ev{ shok_EventIDs::Market_GetProgress };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	return ev.Data;
}

static inline void(__thiscall* const entitysethealth)(shok_EGL_CGLEEntity* th, int h) = reinterpret_cast<void(__thiscall* const)(shok_EGL_CGLEEntity*, int)>(0x57A6D9);
void shok_EGL_CGLEEntity::SetHealth(int h)
{
	entitysethealth(this, h);
}
static inline void(__thiscall* const entityhurt)(shok_EGL_CGLEEntity* th, int h) = reinterpret_cast<void(__thiscall* const)(shok_EGL_CGLEEntity*, int)>(0x57AD47);
void shok_EGL_CGLEEntity::Hurt(int dmg)
{
	entityhurt(this, dmg);
}

static inline void(__thiscall* const shok_entity_settasklistbyid)(shok_EGL_CGLEEntity* th, int tl, int t) = reinterpret_cast<void(__thiscall* const)(shok_EGL_CGLEEntity*, int, int)>(0x57B3B6);
void shok_EGL_CGLEEntity::SetTaskList(int tl)
{
	shok_entity_settasklistbyid(this, tl, 1);
}
static inline void(__thiscall* const shok_entity_settasklist)(shok_EGL_CGLEEntity* th, shok_EGL_CGLETaskList* tl, int t) = reinterpret_cast<void(__thiscall* const)(shok_EGL_CGLEEntity*, shok_EGL_CGLETaskList*, int)>(0x57B208);
void shok_EGL_CGLEEntity::SetTaskList(shok_EGL_CGLETaskList* tl)
{
	shok_entity_settasklist(this, tl, 1);
}
static inline shok_EGL_CGLETaskList* (__thiscall* const shok_entity_GetCurrentTaskList)(shok_EGL_CGLEEntity* th) = reinterpret_cast<shok_EGL_CGLETaskList * (__thiscall* const)(shok_EGL_CGLEEntity*)>(0x57A892);
shok_EGL_CGLETaskList* shok_EGL_CGLEEntity::GetCurrentTaskList()
{
	return shok_entity_GetCurrentTaskList(this);
}
static inline shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* (__thiscall* const shok_entitytaskhandler_gettaskhandler)(shok::Set<shok_entity_TaskIdAndTaskHandler>* th, shok_Task id) = reinterpret_cast<shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int * (__thiscall* const)(shok::Set<shok_entity_TaskIdAndTaskHandler>*, shok_Task)>(0x57BDD3);
shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* shok_EGL_CGLEEntity::GetTaskHandler(shok_Task task)
{
	return shok_entitytaskhandler_gettaskhandler(&TaskHandlers, task);
}

void shok_EGL_CGLEEntity::ExecuteTask(shok_EGL_CGLETaskArgs& targ)
{
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->ExecuteTask(this, &targ);
}

void shok_EGL_CGLEEntity::Destroy()
{
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->Destroy(this, 0);
}

void shok_EGL_CGLEEntity::ClearAttackers()
{
	std::vector<shok_GGL_CSettler*> tomove = std::vector<shok_GGL_CSettler*>();
	ObserverEntities.ForAll([&tomove](shok::Attachment* a) {
		if (a->AttachmentType == shok::AttachmentType::ATTACKER_COMMAND_TARGET || a->AttachmentType == shok::AttachmentType::LEADER_TARGET || a->AttachmentType == shok::AttachmentType::FOLLOWER_FOLLOWED) {
			shok_EGL_CGLEEntity* at = shok_EGL_CGLEEntity::GetEntityByID(a->EntityId);
			if (shok_GGL_CSettler* s = shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(at)) {
				tomove.emplace_back(s);
			}
		}
		});
	for (shok_GGL_CSettler* s : tomove)
		s->Defend();
}



void shok_EGL_CMovingEntity::AttackMove(const shok::Position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::Leader_AttackMove, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	TargetRotationValid = 0;
}

void shok_EGL_CMovingEntity::AttackEntity(int targetId)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Leader_AttackEntity, targetId };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::Move(const shok::Position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::MoveCommand_Move, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	TargetRotationValid = 0;
}

void shok_EGL_CMovingEntity::HoldPosition()
{
	shok_BB_CEvent ev{ shok_EventIDs::Leader_HoldPosition };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::Defend()
{
	shok_BB_CEvent ev{ shok_EventIDs::Leader_Defend };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::LeaderAttachSoldier(int soldierId)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Leader_AttachSoldier, soldierId };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	shok_BB_CEvent e2{ shok_EventIDs::Formation_AssumePosition };
	shok_EGL_CGLEEntity* sol = shok_EGL_CGLEEntity::GetEntityByID(soldierId);
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(sol, &e2);
}

static inline void(__thiscall* const shok_entity_expellSettler)(shok_EGL_CGLEEntity* th, int i) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLEEntity*, int)>(0x4A39BB);
void shok_EGL_CMovingEntity::SettlerExpell()
{
	if (GetBehavior<GGL::CLeaderBehavior>() && ObservedEntities.GetFirstMatch([](shok::Attachment* a) { return a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER; })) {
		shok_EGL_CEventValue_bool ev{ shok_EventIDs::Leader_ExpellSoldier, true };
		reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	}
	else {
		shok_entity_expellSettler(this, 1);
	}
}

void shok_EGL_CMovingEntity::HeroAbilitySendHawk(shok::Position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::HeroHawk_SendHawk, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityInflictFear()
{
	shok_BB_CEvent ev{ shok_EventIDs::InflictFear_Activate };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceBomb(shok::Position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::BombPlacer_CommandPlaceBomb, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceCannon(shok::Position& p, int FoundationType, int CannonType)
{
	shok_GGL_CEventPositionAnd2EntityTypes ev{ shok_EventIDs::CannonBuilder_BuildCannonCommand, p, FoundationType, CannonType };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityRangedEffect()
{
	shok_BB_CEvent ev{ shok_EventIDs::RangedEffect_Activate };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityCircularAttack()
{
	shok_BB_CEvent ev{ shok_EventIDs::CircularAttack_ActivateCommand };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilitySummon()
{
	shok_BB_CEvent ev{ shok_EventIDs::Summon_ActivateCommand };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityConvert(int target)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::ConvertSettler_ActivateCommand, target };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityConvertBuilding(int target)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::ConvertBuilding_ActivateCommand, target };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilitySnipe(int tid)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Sniper_SnipeCommand, tid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityShuriken(int tid)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Shuriken_ActivateCommand, tid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityMotivateWorkers()
{
	shok_BB_CEvent ev{ shok_EventIDs::MotivateVorkers_ActivateCommand };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityActivateCamoflage()
{
	shok_BB_CEvent e{ shok_EventIDs::Camouflage_Activate };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e);
}

void shok_EGL_CMovingEntity::ThiefSabotage(int tid)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::KegPlacer_SabotageCommand, tid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefDefuse(int tid)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::KegPlacer_DefuseCommand, tid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefStealFrom(int tid)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Thief_StealFromCommand, tid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefSecureGoods(int tid)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Thief_SecureGoodsCommand, tid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutBinoculars(shok::Position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::Binocular_ExploreCommand, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutFindResource()
{
	shok_BB_CEvent ev{ shok_EventIDs::PointToResources_Activate };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutPlaceTorch(shok::Position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::TorchPlacer_PlaceTorch, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

bool shok_EGL_CMovingEntity::SerfConstructBuilding(shok_GGL_CBuilding* build)
{
	int cid = build->GetNearestFreeConstructionSlotFor(&this->Position);
	if (cid < 0)
		return false;
	shok_GGL_CEventEntityIndex ev{ shok_EventIDs::Serf_Construct, build->GetConstructionSite(), cid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	return true;
}

bool shok_EGL_CMovingEntity::SerfRepairBuilding(shok_GGL_CBuilding* build)
{
	int cid = build->GetNearestFreeRepairSlotFor(&this->Position);
	if (cid < 0)
		return false;
	shok_GGL_CEventEntityIndex ev{ shok_EventIDs::Serf_Construct, build->EntityId, cid };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	return true;
}

void shok_EGL_CMovingEntity::SerfExtractResource(int id)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Serf_ExtractResource, id };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::SerfTurnToBattleSerf()
{
	shok_BB_CEvent e2{ shok_EventIDs::Serf_CommandTurnToBattleSerf };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

void shok_EGL_CMovingEntity::BattleSerfTurnToSerf()
{
	shok_BB_CEvent e2{ shok_EventIDs::BattleSerf_CommandTurnToSerf };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

void(__thiscall* const movingentity_settarrot)(shok_EGL_CMovingEntity* th, float r) = reinterpret_cast<void(__thiscall*)(shok_EGL_CMovingEntity*, float)>(0x57DBC8);
void shok_EGL_CMovingEntity::SetTargetRotation(float f)
{
	movingentity_settarrot(this, f);
}

void(__thiscall* const movingentity_setpos)(shok_EGL_CMovingEntity* th, shok::Position* p) = reinterpret_cast<void(__thiscall*)(shok_EGL_CMovingEntity*, shok::Position*)>(0x57BC78);
void shok_EGL_CMovingEntity::SetPosition(shok::Position& p)
{
	movingentity_setpos(this, &p);
	Move(p);
}

static inline void(__thiscall* const building_startUpgrade)(shok_GGL_CBuilding* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CBuilding*)>(0x4AF1B5);
void shok_GGL_CBuilding::StartUpgrade()
{
	building_startUpgrade(this);
}

static inline void(__thiscall* const buildingCancelUpgrade)(shok_GGL_CBuilding* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CBuilding*)>(0x4AF228);
void shok_GGL_CBuilding::CancelUpgrade()
{
	buildingCancelUpgrade(this);
}

void shok_GGL_CBuilding::CommandBuildCannon(int entitytype)
{
	shok_EGL_CEventValue_int e{ shok_EventIDs::Foundry_BuildCannonCommand, entitytype };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e);
}

void shok_GGL_CBuilding::CommandRecruitSoldierForLeader(int id)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Barracks_BuySoldierForLeader, id };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

static inline void(__thiscall* const shok_buildingActivateOvertime)(shok_GGL_CBuilding* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CBuilding*)>(0x4AE566);
void shok_GGL_CBuilding::ActivateOvertime()
{
	shok_buildingActivateOvertime(this);
}
static inline void(__thiscall* const shok_buildingDeactivateOvertime)(shok_GGL_CBuilding* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CBuilding*)>(0x4AA073);
void shok_GGL_CBuilding::DeactivateOvertime()
{
	shok_buildingDeactivateOvertime(this);
}

void shok_GGL_CBuilding::BarracksRecruitGroups()
{
	shok_BB_CEvent e2{ shok_EventIDs::Barracks_ActivateAutoFill };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::BarracksRecruitLeaders()
{
	shok_BB_CEvent e2{ shok_EventIDs::Barracks_DeActivateAutoFill };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::HQBuySerf()
{
	shok_BB_CEvent e2{ shok_EventIDs::Keep_BuySerfCommand };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::SellBuilding()
{
	shok_BB_CEvent e2{ shok_EventIDs::Die };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

static inline void(__thiscall* const building_startresearch)(shok_GGL_CBuilding* th, int tech) = reinterpret_cast<void(__thiscall*)(shok_GGL_CBuilding*, int)>(0x4AAC76);
void shok_GGL_CBuilding::StartResearch(int tech)
{
	building_startresearch(this, tech);
}

static inline void(__thiscall* const building_cancelresearch)(shok_GGL_CBuilding* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CBuilding*)>(0x4AACC0);
void shok_GGL_CBuilding::CancelResearch()
{
	building_cancelresearch(this);
}

void shok_GGL_CBuilding::MarketStartTrade(shok::ResourceType ResourceTypeSell, shok::ResourceType ResourceTypeBuy, float BuyAmount)
{
	shok_GGL_CEventTransaction e2{ shok_EventIDs::Market_StartTrade, ResourceTypeSell, ResourceTypeBuy, BuyAmount };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::MarketCancelTrade()
{
	shok_BB_CEvent e2{ shok_EventIDs::Market_CancelTrade };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

static inline int(__thiscall* const raxbeh_createentityandattach)(GGL::CBarrackBehavior* th, int ety) = reinterpret_cast<int(__thiscall*)(GGL::CBarrackBehavior*, int)>(0x50EA18);
static inline int(__thiscall* const raxbeh_gettrainingtl)(GGL::CBarrackBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CBarrackBehavior*)>(0x50EBCE);
int shok_GGL_CBuilding::BuyLeaderByType(int ety)
{
	GGL::CBarrackBehavior* rax = GetBehavior<GGL::CBarrackBehavior>();
	int id = raxbeh_createentityandattach(rax, ety);
	if (id) {
		shok_EGL_CEventValue_int ev = { shok_EventIDs::Leader_SetTrainingTL, raxbeh_gettrainingtl(rax) };
		shok_EGL_CGLEEntity::GetEntityByID(id)->FireEvent(&ev);
	}
	return id;
}

int __cdecl fixedChangePlayer(int id, int pl) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(id);
	if (!e)
		return 0;
	if (e->PlayerId == pl)
		return id;
	shok_EGL_CGLEEntityCreator c{};
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
	int nid = (*shok_EGL_CGLEGameLogic::GlobalObj)->CreateEntity(&c);
	if (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e)) {
		GGL::CLeaderBehavior* lb = e->GetBehavior<GGL::CLeaderBehavior>();
		if (lb) {
			std::vector<int> sol = std::vector<int>();
			e->ObservedEntities.ForAll([&sol](shok::Attachment* a) { if (a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER) sol.push_back(a->EntityId); });
			shok_GGL_CSettler* settler = static_cast<shok_GGL_CSettler*>(shok_EGL_CGLEEntity::GetEntityByID(nid));
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
void shok_EGL_CGLEEntity::ActivateEntityChangePlayerFix()
{
	if (ActivateEntityChangePlayerFix_Hooked)
		return;
	ActivateEntityChangePlayerFix_Hooked = true;
	shok::SaveVirtualProtect vp{ shok_EGL_CGLEEntity::EntityIDChangePlayer, 10 };
	WriteJump(shok_EGL_CGLEEntity::EntityIDChangePlayer, &fixedChangePlayer);
}

void (*shok_EGL_CGLEEntity::Hero6ConvertHookCb)(int id, int pl, int nid, int converter) = nullptr;
int _cdecl hero6convertchangeplayer(int id, int pl) {
	shok_EGL_CGLEEntity* c = (shok_EGL_CGLEEntity*)1;
	_asm { mov c, esi }
	int r = shok_EGL_CGLEEntity::EntityIDChangePlayer(id, pl);
	if (shok_EGL_CGLEEntity::Hero6ConvertHookCb)
		shok_EGL_CGLEEntity::Hero6ConvertHookCb(id, pl, r, c->EntityId);
	return r;
}
bool HookHero6Convert_Hooked = false;
void shok_EGL_CGLEEntity::HookHero6Convert()
{
	if (HookHero6Convert_Hooked)
		return;
	HookHero6Convert_Hooked = true;
	shok_EGL_CGLEEntity::ActivateEntityChangePlayerFix();
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4FCD26), 10 };
	RedirectCall(reinterpret_cast<void*>(0x4FCD26), &hero6convertchangeplayer);
}

int shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity = 0;
void __fastcall camo_behaviorReset(GGL::CCamouflageBehavior* th, int _, int a) {
	if (shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity == 0 || th->EntityId == shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity)
		th->InvisibilityRemaining = 0;
}
bool HookResetCamo_Hooked = false;
void shok_EGL_CGLEEntity::HookResetCamo()
{
	if (HookResetCamo_Hooked)
		return;
	HookResetCamo_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5011DF), 10 };
	WriteJump(reinterpret_cast<void*>(0x5011DF), &camo_behaviorReset);
}

static inline int(__thiscall* const activateCamoOrig)(GGL::CCamouflageBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CCamouflageBehavior*)>(0x501561);
void (*shok_EGL_CGLEEntity::CamoActivateCb)(GGL::CCamouflageBehavior* th);
int __fastcall camoActivateHook(GGL::CCamouflageBehavior* th) {
	int i = activateCamoOrig(th);
	if (shok_EGL_CGLEEntity::CamoActivateCb)
		shok_EGL_CGLEEntity::CamoActivateCb(th);
	return i;
}
bool HookCamoActivate_Hooked = false;
void shok_EGL_CGLEEntity::HookCamoActivate()
{
	if (HookCamoActivate_Hooked)
		return;
	HookCamoActivate_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4D51A4), 10 };
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x50163A), 10 };
	RedirectCall(reinterpret_cast<void*>(0x4D51A4), &camoActivateHook);
	RedirectCall(reinterpret_cast<void*>(0x50163A), &camoActivateHook);
}


static inline void(__thiscall* const event2entitiesctor)(int* e, int id, int at, int de) = reinterpret_cast<void(__thiscall*)(int*, int, int, int)>(0x49847F);
int* shok_EGL_CGLEEntity::HurtEntityDamagePointer = nullptr;
bool shok_EGL_CGLEEntity::HurtEntityCallWithNoAttacker = false;
AdvancedDealDamageSource shok_EGL_CGLEEntity::HurtEntityDamageSource = AdvancedDealDamageSource::Unknown;
int  shok_EGL_CGLEEntity::HurtEntityAttackerPlayer = 0;
void (*shok_EGL_CGLEEntity::HurtEntityOnKillCb)(shok_EGL_CGLEEntity* att, shok_EGL_CGLEEntity* kill, int attpl, AdvancedDealDamageSource sourc) = nullptr;
bool HookHurtEntity_Hooked = false;
void __cdecl hurtentity_override(shok_EGL_CGLEEntity* att, shok_EGL_CGLEEntity* tar, int dmg) {
	tar->AdvancedHurtEntityBy(att, dmg, 0, true, true, true, AdvancedDealDamageSource::Unknown);
}
void __cdecl hurtaoe_override(shok_EGL_CGLEEntity* att, shok::Position* p, float r, int dmg, int pl, int dmgcl) {
	shok_EGL_CGLEEntity::AdvancedDealAoEDamage(att, *p, r, dmg, pl, dmgcl, true, true, true, AdvancedDealDamageSource::Unknown);
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
		call shok_EGL_CGLEEntity::AdvancedHurtEntityBy;

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
		call shok_EGL_CGLEEntity::GetEntityByID;
		push eax;

		mov ecx, esi;
		call shok_EGL_CGLEEntity::AdvancedHurtEntityBy;

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
		call shok_EGL_CGLEEntity::AdvancedHurtEntityBy;

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
		call shok_EGL_CGLEEntity::AdvancedDealAoEDamage;

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
		call shok_EGL_CGLEEntity::AdvancedDealAoEDamage;

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
void shok_EGL_CGLEEntity::HookHurtEntity()
{
	if (HookHurtEntity_Hooked)
		return;
	if (HasSCELoader())
		DEBUGGER_BREAK;
	HookHurtEntity_Hooked = true;
	shok::SaveVirtualProtect vp{ shok_EGL_CGLEEntity::EntityHurtEntity, 10 };
	WriteJump(shok_EGL_CGLEEntity::EntityHurtEntity, &hurtentity_override);
	shok::SaveVirtualProtect vp2{ shok_EGL_CGLEEntity::EntityDealAoEDamage, 10 };
	WriteJump(shok_EGL_CGLEEntity::EntityDealAoEDamage, &hurtaoe_override);
	shok::SaveVirtualProtect vp3{ reinterpret_cast<void*>(0x5113C2), 10 };
	WriteJump(reinterpret_cast<void*>(0x5113C2), &arrowonhit_damage);
	shok::SaveVirtualProtect vp4{ reinterpret_cast<void*>(0x4DBA20), 10 }; // projectile creator bigger zero for AdvancedDamageSourceOverride
	*reinterpret_cast<byte*>(0x4DBA20) = 0x89;
	shok::SaveVirtualProtect vp5{ reinterpret_cast<void*>(0x511634), 20 }; // arrow projcetile AdvancedDamageSourceOverride
	*reinterpret_cast<byte*>(0x511634) = 0x8B;
	*reinterpret_cast<byte*>(0x511637) = 0x89;
	shok::SaveVirtualProtect vp6{ reinterpret_cast<void*>(0x50CA59), 10 };
	WriteJump(reinterpret_cast<void*>(0x50CA59), &meleeonhit_damage);
	shok::SaveVirtualProtect vp7{ reinterpret_cast<void*>(0x4FE722), 10 };
	WriteJump(reinterpret_cast<void*>(0x4FE722), &circularatt_damage);
	shok::SaveVirtualProtect vp8{ reinterpret_cast<void*>(0x4FF4EB), 10 };
	WriteJump(reinterpret_cast<void*>(0x4FF4EB), &cannonballhit_damage);
	shok::SaveVirtualProtect vp9{ reinterpret_cast<void*>(0x506B28), 10 };
	WriteJump(reinterpret_cast<void*>(0x506B28), &bombexplode_damage);
	shok::SaveVirtualProtect vp10{ reinterpret_cast<void*>(0x4F1E77), 10 };
	RedirectCall(reinterpret_cast<void*>(0x4F1E77), &kegdealdmgproxy);
	shok::SaveVirtualProtect vp11{ reinterpret_cast<void*>(0x4DC6D9), 10 };
	WriteJump(reinterpret_cast<void*>(0x4DC6D9), &shurikenthrow);

	HookDamageMod(); // set projectile player field in creator
	GGL::CBombPlacerBehavior::FixBombAttachment();
	shok_GGL_CCannonBallEffect::AddDamageSourceOverride = true;
}
void shok_EGL_CGLEEntity::AdvancedHurtEntityBy(shok_EGL_CGLEEntity* attacker, int damage, int attackerFallback, bool uiFeedback, bool xp, bool addStat, AdvancedDealDamageSource sourceInfo)
{
	if ((*shok_GGL_CGLGameLogic::GlobalObj)->GlobalInvulnerability)
		return;
	if (Health <= 0)
		return;
	shok_EGL_CEventGetValue_bool getbool{ shok_EventIDs::IsSettlerOrBuilding };
	FireEvent(&getbool);
	if (!getbool.Data && !shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CBridgeEntity>(this))
		return;
	if (GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING) || GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE))
		return;
	int attackerplayer = attacker ? attacker->PlayerId : attackerFallback;
	if (attacker || shok_EGL_CGLEEntity::HurtEntityCallWithNoAttacker) {
		shok_EGL_CEvent2Entities ev{ shok_EventIDs::LogicEvent_HurtEntity, attacker ? attacker->EntityId : 0, EntityId };
		shok_EGL_CGLEEntity::HurtEntityDamagePointer = &damage;
		shok_EGL_CGLEEntity::HurtEntityDamageSource = sourceInfo;
		shok_EGL_CGLEEntity::HurtEntityAttackerPlayer = attackerplayer;
		(*shok_EScr_CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
		shok_EGL_CGLEEntity::HurtEntityDamagePointer = nullptr;
	}
	if (damage <= 0)
		return;
	getbool = shok_EGL_CEventGetValue_bool{ shok_EventIDs::IsWorker };
	FireEvent(&getbool);
	if (attacker) {
		if (!getbool.Data || !(*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape->IsPosBlockedInMode(&Position, shok_EGL_CGLELandscape::BlockingMode::Blocked)) {
			if (!ArePlayersFriendly(PlayerId, attacker->PlayerId)) {
				shok_EGL_CEvent1Entity ev{ shok_EventIDs::OnAttackedBy, attacker->EntityId };
				FireEvent(&ev);
			}
		}
	}
	if (uiFeedback) {
		if (attackerplayer) {
			shok_GGL_CFeedbackEventBattling ev{ shok_FeedbackEventIds::FEEDBACK_EVENT_BATTLING, EntityId, PlayerId, attacker ? attacker->Position : Position, attackerplayer };
			shok_feedbackEventHandler::GlobalObj()->FireEvent(&ev);
		}
		shok_EGL_CNetEventEntityIDAndInteger ev{ shok_FeedbackEventIds::FEEDBACK_EVENT_ENTITY_HURT, EntityId, damage };
		shok_feedbackEventHandler::GlobalObj()->FireEvent(&ev);
	}
	if (attacker) {
		ObserverEntities.ForAll([attacker](shok::Attachment* a) {
			if (a->AttachmentType == shok::AttachmentType::GUARD_GUARDED) {
				shok_EGL_CEvent1Entity ev{ shok_EventIDs::Leader_OnGuardedAttackedBy, attacker->EntityId };
				shok_EGL_CGLEEntity::GetEntityByID(a->EntityId)->FireEvent(&ev);
			}
			});
	}

	getbool = shok_EGL_CEventGetValue_bool{ shok_EventIDs::IsBattleOrAutocannon };
	FireEvent(&getbool);
	shok_EGL_CEventGetValue_bool getbool2{ shok_EventIDs::IsSerfOrWorker };
	FireEvent(&getbool2);
	std::vector<int> idskilled{};
	int xptoadd = 0;
	shok_EGL_CGLEEntity* firsttodie = this;
	if (getbool.Data && !getbool2.Data) { // has potentially soldiers
		getbool = shok_EGL_CEventGetValue_bool{ shok_EventIDs::IsSoldier };
		FireEvent(&getbool);
		shok_EGL_CGLEEntity* attackedleader = this;
		if (getbool.Data) {
			int id = this->GetFirstAttachedToMe(shok::AttachmentType::LEADER_SOLDIER);
			if (id)
				attackedleader = shok_EGL_CGLEEntity::GetEntityByID(id);
		}
		GGL::CLeaderBehavior* lbeh = attackedleader->GetBehavior<GGL::CLeaderBehavior>();
		if (lbeh && firsttodie == attackedleader) {
			int id = attackedleader->GetFirstAttachedEntity(shok::AttachmentType::LEADER_SOLDIER);
			if (id)
				firsttodie = shok_EGL_CGLEEntity::GetEntityByID(id);
		}
		if (lbeh) {
			int troophp = lbeh->GetTroopHealth();
			int hppersol = lbeh->GetTroopHealthPerSoldier();
			int currentsol = 0;
			attackedleader->ObservedEntities.ForAll([&currentsol](shok::Attachment* a) {
				if (a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER)
					currentsol++;
				});
			while (troophp > 0 && damage > 0) {
				int thissolhp = troophp - ((currentsol - 1) * hppersol);
				if (damage >= thissolhp) {
					damage -= thissolhp;
					troophp -= thissolhp;
					attackedleader->DetachObservedEntity(shok::AttachmentType::LEADER_SOLDIER, firsttodie->EntityId, false);
					currentsol--;
					idskilled.push_back(firsttodie->EntityId);
					xptoadd += firsttodie->GetEntityType()->LogicProps->ExperiencePoints;
					shok_GGL_CEventEntityIndex kev{ shok_EventIDs::CppL_OnEntityKilled, attacker ? attacker->EntityId : 0, attackerplayer };
					firsttodie->FireEvent(&kev);
					if (shok_EGL_CGLEEntity::HurtEntityOnKillCb)
						shok_EGL_CGLEEntity::HurtEntityOnKillCb(attacker, firsttodie, attackerplayer, sourceInfo);
					firsttodie->Hurt(firsttodie->Health);

					int id = attackedleader->GetFirstAttachedEntity(shok::AttachmentType::LEADER_SOLDIER);
					if (id)
						firsttodie = shok_EGL_CGLEEntity::GetEntityByID(id);
					else
						firsttodie = attackedleader;
				}
				else {
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
			idskilled.push_back(firsttodie->EntityId);
			xptoadd += firsttodie->GetEntityType()->LogicProps->ExperiencePoints;
			shok_GGL_CEventEntityIndex kev{ shok_EventIDs::CppL_OnEntityKilled, attacker ? attacker->EntityId : 0, attackerplayer };
			firsttodie->FireEvent(&kev);
			if (shok_EGL_CGLEEntity::HurtEntityOnKillCb)
				shok_EGL_CGLEEntity::HurtEntityOnKillCb(attacker, firsttodie, attackerplayer, sourceInfo);
			firsttodie->Hurt(firsttodie->Health);
		}
		else {
			firsttodie->Hurt(damage);
		}
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
	if (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CBuilding>(this)) {
		if (addStat) {
			if (attackerplayer)
				(*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(attackerplayer)->Statistics.NumberOfBuildingsDestroyed += idskilled.size();
			if (shok_GGL_CPlayerStatus* ps = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(this->PlayerId))
				ps->Statistics.NumberOfBuildingsLost += idskilled.size();
		}
		callback = "GameCallback_BuildingDestroyed";
	}
	else {
		if (addStat) {
			if (attackerplayer)
				(*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(attackerplayer)->Statistics.NumberOfUnitsKilled += idskilled.size();
			if (shok_GGL_CPlayerStatus* ps = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(this->PlayerId))
				ps->Statistics.NumberOfUnitsDied += idskilled.size();
		}
		callback = "GameCallback_SettlerKilled";
	}
	if (attackerplayer || shok_EGL_CGLEEntity::HurtEntityCallWithNoAttacker) {

		lua_State* L = *shok_luastate_game;
		int t = lua_gettop(L);
		lua_pushstring(L, callback);
		lua_rawget(L, LUA_GLOBALSINDEX);
		lua_pushnumber(L, attackerplayer);
		lua_pushnumber(L, this->PlayerId);
		lua_pushnumber(L, attacker ? attacker->EntityId : 0);
		lua_checkstack(L, idskilled.size());
		for (int i : idskilled) {
			lua_pushnumber(L, i);
		}
		lua_pcall(L, 3 + idskilled.size(), 0, 0);
		lua_settop(L, t);
	}
}
void __stdcall shok_EGL_CGLEEntity::AdvancedDealAoEDamage(shok_EGL_CGLEEntity* attacker, const shok::Position& center, float range, int damage, int player, int damageclass, bool uiFeedback, bool xp, bool addStat, AdvancedDealDamageSource sourceInfo)
{
	if ((*shok_GGL_CGLGameLogic::GlobalObj)->GlobalInvulnerability)
		return;
	if (range <= 0)
		return;
	int pl = attacker ? attacker->PlayerId : player;
	EntityIteratorPredicateIsRelevant irel{};
	EntityIteratorPredicateInCircle icircl{ center, range };
	EntityIteratorPredicateIsAlive iali{};
	int buff[8];
	int bufflen = 8;
	if (pl)
		EntityIteratorPredicateAnyPlayer::FillHostilePlayers(pl, buff, bufflen);
	EntityIteratorPredicateAnyPlayer ipl{buff, bufflen};
	EntityIteratorPredicate* iandd[4] = { &irel, &icircl, &iali, &ipl };
	EntityIteratorPredicateAnd iand{ iandd, pl ? 4 : 3 };
	EntityIterator it{ &iand };
	float cr = 0;
	shok_EGL_CGLEEntity* curr = it.GetNext(&cr, nullptr);
	while (curr) {
		cr = std::sqrtf(cr) / range;
		if (cr < 0 || cr > 1)
			cr = 0;
		else
			cr = 1 - cr * cr;
		
		if (cr != 0) {
			shok_EGL_CEventGetValue_int getac{ shok_EventIDs::GetArmorClass };
			curr->FireEvent(&getac);
			shok_EGL_CEventGetValue_int geta{ shok_EventIDs::GetArmor };
			curr->FireEvent(&geta);

			float dmg = damage * cr;
			if (damageclass > 0 && damageclass < static_cast<int>((*shok_damageClassHolder::GlobalObj)->DamageClassList.size()))
				dmg *= (*shok_damageClassHolder::GlobalObj)->DamageClassList[damageclass]->BonusVsArmorClass[getac.Data - 1];
			dmg -= geta.Data;

			if (dmg < 1)
				dmg = 1;

			curr->AdvancedHurtEntityBy(attacker, static_cast<int>(dmg), pl, uiFeedback, xp, addStat, sourceInfo);
		}

		curr = it.GetNext(&cr, nullptr);
	}
}

std::multimap<int, int> shok_EGL_CGLEEntity::BuildingMaxHpTechBoni = std::multimap<int, int>();
bool shok_EGL_CGLEEntity::UseMaxHPTechBoni = false;
int __fastcall hookGetMaxHP(shok_EGL_CGLEEntity* e) {
	float hp;
	entityAddonData* d = e->GetAdditionalData(false);
	GGlue::CGlueEntityProps* et = e->GetEntityType();
	if (d && d->HealthOverride > 0) {
		hp = static_cast<float>(d->HealthOverride);
	}
	else {
		hp = static_cast<float>(et->LogicProps->MaxHealth);
	}
	if (d && !d->HealthUseBoni)
		return static_cast<int>(hp);
	if (shok_EGL_CGLEEntity::UseMaxHPTechBoni)
	{
		if (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e)) {
			for (int t : static_cast<GGL::CGLSettlerProps*>(et->LogicProps)->ModifyHitpoints.TechList) {
				if ((*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != shok::TechState::Researched)
					continue;
				shok_technology* tech = (*shok_GGL_CGLGameLogic::GlobalObj)->GetTech(t);
				hp = tech->HitpointModifier.ModifyValue(hp);
			}
		}
		else if (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e)) {
			std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> it = shok_EGL_CGLEEntity::BuildingMaxHpTechBoni.equal_range(e->EntityType);
			for (std::multimap<int, int>::iterator i = it.first; i != it.second; ++i) {
				int t = i->second;
				if ((*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != shok::TechState::Researched)
					continue;
				shok_technology* tech = (*shok_GGL_CGLGameLogic::GlobalObj)->GetTech(t);
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
void __fastcall createentityfixhp(shok_EGL_CGLEEntity* th, int hpIn) {
	if (hpIn != -1)
		return;
	if (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(th)) {
		th->Health = th->GetMaxHealth();
	}
	else if (shok_GGL_CBuilding* b = shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CBuilding>(th)) {
		th->Health = th->GetMaxHealth();
		if (!b->IsConstructionFinished()) {
			th->Health = static_cast<int>(th->Health * (*shok_GGL_CLogicProperties::GlobalObj)->ConstructionSiteHealthFactor);
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
void shok_EGL_CGLEEntity::HookMaxHP()
{
	if (HookMaxHP_Hooked)
		return;
	if (HasSCELoader())
		DEBUGGER_BREAK;
	HookMaxHP_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57B798), 10 };
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x4BDED8), 10 };
	shok::SaveVirtualProtect vp3{ reinterpret_cast<void*>(0x571B93), 10 };
	WriteJump(reinterpret_cast<void*>(0x57B798), &hookGetMaxHP);
	WriteJump(reinterpret_cast<void*>(0x4BDED8), &hookgetmaxhpui);
	WriteJump(reinterpret_cast<void*>(0x571B93), &hookcreatentityfixhp);
}

int (*shok_EGL_CGLEEntity::LuaTaskListCallback)(shok_EGL_CGLEEntity* e, int val) = nullptr;
int __fastcall entity_executeluatask(shok_EGL_CGLEEntity* th, int _, shok_EGL_CGLETaskArgs* args) {
	int i = 0;
	int val = static_cast<shok_EGL_CTaskArgsInteger*>(args)->Value;
	if (shok_EGL_CGLEEntity::LuaTaskListCallback)
		i = shok_EGL_CGLEEntity::LuaTaskListCallback(th, val);
	if (i == 3) {
		th->GetAdditionalData(true)->FakeTaskValue = val;
		shok_vtable_EGL_CGLEEntity* vt = reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(th->vtable);
		vt->SetTaskState(th, shok_TaskState::LuaFunc);
		i = 1;
	}
	return i;
}
int __fastcall entity_executeluataskstate(shok_EGL_CGLEEntity* th, int _, int onek) {
	int i = -2;
	if (shok_EGL_CGLEEntity::LuaTaskListCallback)
		if (shok_EGL_CGLEEntity::LuaTaskListCallback(th, th->GetAdditionalData(true)->FakeTaskValue) == 3)
			i = -1;
	return i;
}
void(__thiscall* const entityaddluatlhandlers)(shok_EGL_CGLEEntity* th) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLEEntity*)>(0x57BA34);
void __fastcall entity_addluatlhandlershook(shok_EGL_CGLEEntity* th) {
	entityaddluatlhandlers(th);
	shok_vtable_EGL_CGLEEntity* vt = reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(th->vtable);
	shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* thand = static_cast<shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int*>(shok::Malloc(sizeof(shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int)));
	thand->vtable = 0x783D7C; // this is EGL::THandler<12,EGL::CGLETaskArgs,EGL::CGLETaskArgs,EGL::CGLEEntity,int>, but the func is always the same anyway...
	thand->Object = th;
	thand->Func = reinterpret_cast<int(__thiscall*)(void* th, shok_EGL_CGLETaskArgs * args)>(&entity_executeluatask);
	thand->Zero = 0;
	vt->AddTaskHandler(th, shok_Task::TASK_LUA_FUNC, thand);
	shok_EGL_IGLEStateHandler* shand = static_cast<shok_EGL_IGLEStateHandler*>(shok::Malloc(sizeof(shok_EGL_IGLEStateHandler)));
	shand->vtable = 0x783D9C; // EGL::TStateHandler<EGL::CGLEEntity>
	shand->Object = th;
	shand->Func = reinterpret_cast<int(__thiscall*)(void*, int)>(&entity_executeluataskstate);
	shand->Zero = 0;
	vt->AddStateHandler(th, shok_TaskState::LuaFunc, shand);
}
bool HookLuaTaskList_Hooked = false;
void shok_EGL_CGLEEntity::HookLuaTaskList()
{
	if (HookLuaTaskList_Hooked)
		return;
	HookLuaTaskList_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57D6CA), 10 };
	RedirectCall(reinterpret_cast<void*>(0x57D6CA), &entity_addluatlhandlershook);
}

int __fastcall entity_hooknoncancelanim_task(GGL::CGLBehaviorAnimationEx* th, int _, shok_EGL_CGLETaskArgs* args) {
	int wait = static_cast<shok_EGL_CGLETaskArgsThousandths*>(args)->Thousandths;
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	shok_vtable_EGL_CGLEEntity* vt = reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(e->vtable);
	th->TurnToWaitFor = th->StartTurn + th->Duration * wait / 1000;
	vt->SetTaskState(e, shok_TaskState::WaitForAnimNonCancelable);
	return 1;
}
void __stdcall entity_hooknoncancelanim(shok_EGL_CGLEEntity* th, GGL::CGLBehaviorAnimationEx* beh) {
	shok_vtable_EGL_CGLEEntity* vt = reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(th->vtable);
	shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* thand = static_cast<shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int*>(shok::Malloc(sizeof(shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int)));
	thand->vtable = 0x784A4C; // EGL::THandler<5,EGL::CGLETaskArgs,EGL::CGLETaskArgsThousandths,EGL::CBehaviorAnimation,int>
	thand->Object = beh;
	thand->Func = reinterpret_cast<int(__thiscall*)(void* th, shok_EGL_CGLETaskArgs * args)>(entity_hooknoncancelanim_task);
	thand->Zero = 0;
	vt->AddTaskHandler(th, shok_Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE, thand);
	shok_EGL_IGLEStateHandler* shand = static_cast<shok_EGL_IGLEStateHandler*>(shok::Malloc(sizeof(shok_EGL_IGLEStateHandler)));
	shand->vtable = 0x784A7C; // EGL::TStateHandler<EGL::CBehaviorAnimation>
	shand->Object = beh;
	shand->Func = reinterpret_cast<int(__thiscall*)(void*, int)>(0x587E20);
	shand->Zero = 0;
	vt->AddStateHandler(th, shok_TaskState::WaitForAnimNonCancelable, shand);
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
void shok_EGL_CGLEEntity::HookNonCancelableAnim()
{
	if (HookNonCancelableAnim_Hooked)
		return;
	HookNonCancelableAnim_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x588408), 10 };
	WriteJump(reinterpret_cast<void*>(0x588408), &entity_hooknoncancelanim_asm);
}

void(__thiscall* movementbeh_setmovetarget)(GGL::CBehaviorDefaultMovement* m, shok::Position* p) = reinterpret_cast<void(__thiscall*)(GGL::CBehaviorDefaultMovement*, shok::Position*)>(0x586894);
void __fastcall entity_buildonsetpos(shok_EGL_CMovingEntity* e) {
	if (e->MovementState && shok_EGL_CGLEEntity::BuildOnSetPosFixMovement) {
		GGL::CBehaviorDefaultMovement* mov = e->GetBehavior<GGL::CBehaviorDefaultMovement>();
		if (mov) {
			movementbeh_setmovetarget(mov, &e->TargetPosition);
		}
	}
	else {
		shok_BB_CEvent ev{ 0x2000D };
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
bool shok_EGL_CGLEEntity::BuildOnSetPosFixMovement = false;
bool HookBuildOnSetPos_Hooked = false;
void shok_EGL_CGLEEntity::HookBuildOnSetPos()
{
	if (HookBuildOnSetPos_Hooked)
		return;
	HookBuildOnSetPos_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4ADB16), 10 };
	WriteJump(reinterpret_cast<void*>(0x4ADB16), &entity_buildonsetpos_asm);
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
void shok_EGL_CGLEEntity::HookSetTaskListNonCancelable(bool active)
{
	shok::SaveVirtualProtect vp{ (void*)0x57B223, 10 };
	if (active) {
		if (!HookSetTaskListNonCancelable_backup)
			HookSetTaskListNonCancelable_backup = WriteJump(reinterpret_cast<void*>(0x57B223), &HookSetTaskListNonCancelable_asm);
	}
	else {
		if (HookSetTaskListNonCancelable_backup)
			*reinterpret_cast<long long*>(0x57B223) = HookSetTaskListNonCancelable_backup;
		HookSetTaskListNonCancelable_backup = 0;
	}
}


shok_EGL_CGLEEntity* shok_EGL_CGLEEntity::ReplaceEntityWithResourceEntity(shok_EGL_CGLEEntity* e)
{
	EGL::CGLEEntityProps* ty = e->GetEntityType()->LogicProps;
	if (typeid(*ty) != typeid(GGL::CEntityProperties))
		return nullptr;
	GGL::CEntityProperties* t = static_cast<GGL::CEntityProperties*>(ty);
	shok_GGL_CResourceDoodadCreator c{};
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
	int id = (*shok_EGL_CGLEGameLogic::GlobalObj)->CreateEntity(&c);
	shok_EGL_CGLEEntity* r = shok_EGL_CGLEEntity::GetEntityByID(id);
	shok_EGL_CEventValue_int ev{ shok_EventIDs::ResourceTree_Init, e->EntityType };
	((shok_vtable_EGL_CGLEEntity*)r->vtable)->FireEvent(r, &ev);
	e->Destroy();
	return r;
}


std::map<int, entityAddonData> shok_EGL_CGLEEntity::AddonDataMap = std::map<int, entityAddonData>();
entityAddonData shok_EGL_CGLEEntity::LastRemovedEntityAddonData = entityAddonData();
void __fastcall destroyentity_gemoveadd(shok_EGL_CGLEEntity* e) {
	shok_BB_CEvent ev{ shok_EventIDs::CppL_OnEntityDestroy };
	e->FireEvent(&ev);
	auto a = shok_EGL_CGLEEntity::AddonDataMap.find(e->EntityId);
	if (a != shok_EGL_CGLEEntity::AddonDataMap.end()) {
		shok_EGL_CGLEEntity::LastRemovedEntityAddonData = a->second;
		shok_EGL_CGLEEntity::AddonDataMap.erase(a);
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
void shok_EGL_CGLEEntity::HookDestroyEntity()
{
	if (HookDestroyEntity_Hooked)
		return;
	HookDestroyEntity_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57C94A), 10 };
	WriteJump(reinterpret_cast<void*>(0x57C94A), &destroyentityhook);
}
entityAddonData* shok_EGL_CGLEEntity::GetAdditionalData(bool create)
{
	auto a = shok_EGL_CGLEEntity::AddonDataMap.find(EntityId);
	if (a != shok_EGL_CGLEEntity::AddonDataMap.end()) {
		return &a->second;
	}
	if (!create) {
		return nullptr;
	}
	else {
		shok_EGL_CGLEEntity::AddonDataMap[EntityId] = entityAddonData();
		entityAddonData* r = &shok_EGL_CGLEEntity::AddonDataMap[EntityId];
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
		t->RegenHPOverride = other->RegenHPOverride;
		t->RegenSecondsOverride = other->RegenSecondsOverride;
		t->MaxRangeOverride = other->MaxRangeOverride;
		t->NameOverride = other->NameOverride;
		t->FakeTaskValue = other->FakeTaskValue;
	}
}

void shok_EGL_CGLEEntity::AddTaskHandler(shok_Task task, void* obj, int(__fastcall* Handler)(void* obj, int _, shok_EGL_CGLETaskArgs* taskargs))
{
	shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* thand = static_cast<shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int*>(shok::Malloc(sizeof(shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int)));
	thand->vtable = 0x783D7C; // this is EGL::THandler<12,EGL::CGLETaskArgs,EGL::CGLETaskArgs,EGL::CGLEEntity,int>, but the func is always the same anyway...
	thand->Object = obj;
	thand->Func = reinterpret_cast<int(__thiscall*)(void* th, shok_EGL_CGLETaskArgs * args)>(Handler);
	thand->Zero = 0;
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->AddTaskHandler(this, task, thand);
}
void shok_EGL_CGLEEntity::AddTaskStateHandler(shok_TaskState state, void* obj, int(__fastcall* Handler)(void* obj, int _, int onek))
{
	shok_EGL_IGLEStateHandler* shand = static_cast<shok_EGL_IGLEStateHandler*>(shok::Malloc(sizeof(shok_EGL_IGLEStateHandler)));
	shand->vtable = 0x783D9C; // EGL::TStateHandler<EGL::CGLEEntity>
	shand->Object = obj;
	shand->Func = reinterpret_cast<int(__thiscall*)(void*, int)>(Handler);
	shand->Zero = 0;
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->AddStateHandler(this, state, shand);
}
void shok_EGL_CGLEEntity::AddEventHandler(shok_EventIDs ev, void* ob, void(__fastcall* Handler)(void* obj, int _, shok_BB_CEvent* ev))
{
	shok_EGL_IGLEHandler_BB_CEvent_void* ehand = static_cast<shok_EGL_IGLEHandler_BB_CEvent_void*>(shok::Malloc(sizeof(shok_EGL_IGLEHandler_BB_CEvent_void)));
	ehand->vtable = 0x774948; // EGL::THandler<81926, class BB::CEvent, class BB::CEvent, class GGL::CSerfBehavior, void>
	ehand->Object = ob;
	ehand->Func = reinterpret_cast<void(__thiscall*)(void*, shok_BB_CEvent*)>(Handler);
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->AddEventHandler(this, ev, ehand);
}

float __fastcall entitygetdamagemod(GGL::CBattleBehavior* b) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(b->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
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
shok_EGL_CEventGetValue_int* __fastcall entitydamagemodeventautocannonasm(GGL::CAutoCannonBehavior* th, int, shok_EGL_CEventGetValue_int* ev) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
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

		call shok_EGL_CGLEEntity::EventGetDamage;
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
		call shok_EGL_CGLEEntity::EventGetDamage;
		mov[ebp - 0x44], eax; // damage

		pop ecx;

		push 0x510776;
		ret;
	}
}
bool HookDamageMod_Hooked = false;
void shok_EGL_CGLEEntity::HookDamageMod()
{
	if (HookDamageMod_Hooked)
		return;
	HookDamageMod_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x50C235), 0x51076C - 0x50C235 + 10 };
	WriteJump(reinterpret_cast<void*>(0x50C785), &entitydamagemodeventbattleasm);
	WriteJump(reinterpret_cast<void*>(0x50C235), &entitydamagemodbattlecalcsingletargetdmgasm);
	WriteJump(reinterpret_cast<void*>(0x50F5ED), &entitydamagemodeventautocannonasm);
	WriteJump(reinterpret_cast<void*>(0x50C3E7), &entitydamagemodbattleprojectile);
	WriteJump(reinterpret_cast<void*>(0x51076C), &entitydamagemodautocannonprojectileasm);
}

int __fastcall entityarmormod(shok_EGL_CGLEEntity* e) {
	entityAddonData* d = e->GetAdditionalData(false);
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
void shok_EGL_CGLEEntity::HookArmorMod()
{
	if (HookArmorMod_Hooked)
		return;
	HookArmorMod_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4A6B15), 10 };
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x4AB160), 10 };
	WriteJump(reinterpret_cast<void*>(0x4A6B15), &entityarmormodsettlerasm);
	WriteJump(reinterpret_cast<void*>(0x4AB160), &entityarmormodbuildingrasm);
}

float __fastcall entityexplmod(shok_EGL_CGLEEntity* e) {
	entityAddonData* d = e->GetAdditionalData(false);
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
void shok_EGL_CGLEEntity::HookExplorationMod()
{
	if (HookExplorationMod_Hooked)
		return;
	HookExplorationMod_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4A4AC3), 10 };
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x4AB199), 10 };
	WriteJump(reinterpret_cast<void*>(0x4A4AC3), &entityexplmodsettasm);
	WriteJump(reinterpret_cast<void*>(0x4AB199), &entityexplmodbuildasm);
}

int shok_GGL_CSettler::LeaderGetRegenHealth()
{
	entityAddonData* d = GetAdditionalData(false);
	if (d && d->RegenHPOverride >= 0)
		return d->RegenHPOverride;
	else
		return GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->HealingPoints;
}
int shok_GGL_CSettler::LeaderGetRegenHealthSeconds()
{
	entityAddonData* d = GetAdditionalData(false);
	if (d && d->RegenSecondsOverride >= 0)
		return d->RegenSecondsOverride;
	else
		return GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->HealingSeconds;
}
static inline void(__thiscall* const settler_killbyenviro)(shok_GGL_CSettler* th) = reinterpret_cast<void(__thiscall*)(shok_GGL_CSettler*)>(0x4A5049);
void shok_GGL_CSettler::KillSettlerByEnvironment()
{
	settler_killbyenviro(this);
}

void shok_EGL_CGLEEntity::PerformHeal(int r, bool healSoldiers)
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

bool shok_EGL_CGLEEntity::LeaderRegenRegenerateSoldiers = false;
void __fastcall leaderregen(GGL::CLeaderBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	th->SecondsSinceHPRefresh = 0;
	int hp = e->Health, mhp = e->GetMaxHealth();
	if (hp <= 0)
		return;
	int r = static_cast<shok_GGL_CSettler*>(e)->LeaderGetRegenHealth();
	e->PerformHeal(r, shok_EGL_CGLEEntity::LeaderRegenRegenerateSoldiers);
}
void __fastcall leaderregenseconds(GGL::CLeaderBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	int max = static_cast<shok_GGL_CSettler*>(e)->LeaderGetRegenHealthSeconds();
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
void shok_EGL_CGLEEntity::HookLeaderRegen()
{
	if (HookLeaderRegen_Hooked)
		return;
	HookLeaderRegen_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4EAE92), 10 };
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x4EFC29), 10 };
	WriteJump(reinterpret_cast<void*>(0x4EAE92), &leaderregen);
	WriteJump(reinterpret_cast<void*>(0x4EFC29), &leaderregensecondsasm);
}

float __fastcall leadermaxrange(GGL::CBattleBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
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
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
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
void shok_EGL_CGLEEntity::HookMaxRange()
{
	if (HookMaxRange_Hooked)
		return;
	HookMaxRange_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x50AB48), 10 };
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x50F50D), 10 };
	WriteJump(reinterpret_cast<void*>(0x50AB48), &leadermaxrangeasm);
	WriteJump(reinterpret_cast<void*>(0x50F50D), &autocannonmaxrangeasm);
}

const char* __stdcall entitydisplayname(int* e, int type) {
	// lots of hacks to recover that entity pointer in a safe way
	// there are 2 branches that can end up calling this, with 2 different pointers in eax
	int d[5] = { shok_EGL_CGLEEntity::vtp_IEntityDisplay, shok_GGL_CSettler::vtp_IEntityDisplay, shok_GGL_CBuilding::vtp_IEntityDisplay,
		shok_GGL_CResourceDoodad::vtp_IEntityDisplay, shok_GGL_CBridgeEntity::vtp_IEntityDisplay
	};
	shok_EGL_CGLEEntity* ent = nullptr;
	if (contains(d, *e, 5)) {
		ent = reinterpret_cast<shok_EGL_CGLEEntity*>(e - 1);
	}
	else {
		int d2[5] = { shok_EGL_CGLEEntity::vtp, shok_GGL_CSettler::vtp, shok_GGL_CBuilding::vtp,
			shok_GGL_CResourceDoodad::vtp, shok_GGL_CBridgeEntity::vtp
		};
		if (contains(d2, *e, 5)) {
			ent = reinterpret_cast<shok_EGL_CGLEEntity*>(e);
		}
	}
	if (ent) {
		entityAddonData* d = ent->GetAdditionalData(false);
		if (d && d->NameOverride.size() > 0) {
			return d->NameOverride.c_str();
		}
	}

	return shok_EGL_CGLEEntitiesProps::GetEntityTypeDisplayName(type);
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
void shok_EGL_CGLEEntity::HookDisplayName()
{
	if (HookDisplayName_Hooked)
		return;
	HookDisplayName_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x53F911), 10 };
	WriteJump(reinterpret_cast<void*>(0x53F911), &entitydisplaynameasm);
}

void __fastcall rangedeffecthealhook(GGL::CRangedEffectAbility* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	GGL::CRangedEffectAbilityProps* pr = e->GetEntityType()->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
	float hpfact = pr->HealthRecoveryFactor;
	if (hpfact <= 0)
		return;
	shok_EGL_CGLEEffectCreator ecr{};
	ecr.PlayerID = e->PlayerId;
	ecr.EffectType = pr->HealEffect;
	e->ObservedEntities.ForAll([hpfact, &ecr](shok::Attachment* a) {
		if (a->AttachmentType == shok::AttachmentType::HERO_AFFECTED) {
			shok_EGL_CGLEEntity* toheal = shok_EGL_CGLEEntity::GetEntityByID(a->EntityId);
			if (!toheal)
				return;
			float heal = toheal->GetMaxHealth() * hpfact;
			if (ecr.EffectType) {
				ecr.StartPos.X = toheal->Position.X;
				ecr.StartPos.Y = toheal->Position.Y;
				(*shok_EGL_CGLEGameLogic::GlobalObj)->CreateEffect(&ecr);
			}
			if (toheal->GetBehavior<GGL::CSoldierBehavior>()) {
				toheal = shok_EGL_CGLEEntity::GetEntityByID(static_cast<shok_GGL_CSettler*>(toheal)->LeaderId);
			}
			if (!toheal)
				return;
			toheal->PerformHeal(static_cast<int>(heal), true);
		}
		});
}
void shok_EGL_CGLEEntity::HookRangedEffectActivateHeal(bool hookActive)
{
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4E3C78), 10 };
	if (hookActive)
		RedirectCall(reinterpret_cast<void*>(0x4E3C78), &rangedeffecthealhook);
	else
		RedirectCall(reinterpret_cast<void*>(0x4E3C78), reinterpret_cast<void*>(0x4E39B4));
}

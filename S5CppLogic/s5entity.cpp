#include "pch.h"
#include "s5data.h"
#include <math.h>


struct shok_vtable_EGL_CGLEEntity {
	PADDINGI(3);
	void(__thiscall* Destroy)(shok_EGL_CGLEEntity* th, int i); // 3
	PADDINGI(11);
	void(__thiscall* ExecuteTask)(shok_EGL_CGLEEntity* th, shok_EGL_CGLETaskArgs* t); // 15 return values: 2->same task, next tick, 1->next task, next tick, 0->next task, immediately
	void(__thiscall* FireEvent)(shok_EGL_CGLEEntity* th, shok_BB_CEvent* d); // 16
	PADDINGI(2);
	void(__thiscall* AddTaskHandler)(shok_EGL_CGLEEntity* th, shok_Task task, shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* taskhandler); // 19
	void(__thiscall* AddEventHandler)(shok_EGL_CGLEEntity* th, shok_EventIDs eventid, int eventhandler); // 20
	void(__thiscall* AddStateHandler)(shok_EGL_CGLEEntity* th, shok_TaskState state, shok_EGL_IGLEStateHandler* statehandler); // 21
	PADDINGI(2);
	void(__thiscall* SetTaskState)(shok_EGL_CGLEEntity* th, shok_TaskState state); // 24
	PADDINGI(3);
	float(__thiscall* GetExploration)(shok_EGL_CGLEEntity* e); // 28
	PADDINGI(5);
	bool(__thiscall* CanCancelCurrentState)(shok_EGL_CGLEEntity* e); // 34
};
//constexpr int i = offsetof(shok_vtable_EGL_CGLEEntity, CanCancelCurrentState) / 4;

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

static inline bool(__thiscall* const shok_IsEntityInCategory)(shok_EGL_CGLEEntity* e, shok_EntityCategory cat) = reinterpret_cast<bool(__thiscall*)(shok_EGL_CGLEEntity * e, shok_EntityCategory)>(0x57BBEB);
bool shok_EGL_CGLEEntity::IsEntityInCategory(shok_EntityCategory cat)
{
	return shok_IsEntityInCategory(this, cat);
}

int shok_EGL_CGLEEntity::GetResourceProvided()
{
	return shok_EGL_CGLEEntity::EntityIDGetProvidedResource(this->EntityId);
}

shok_GGlue_CGlueEntityProps* shok_EGL_CGLEEntity::GetEntityType()
{
	return (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(EntityType);
}

int shok_EGL_CGLEEntity::EventGetIntById(shok_EventIDs id)
{
	shok_EGL_CEventGetValue_int d {id};
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

shok_LeaderCommand shok_EGL_CGLEEntity::EventLeaderGetCurrentCommand()
{
	if (GetBehavior<shok_GGL_CLeaderBehavior>() == nullptr)
		return shok_LeaderCommand::Unknown;
	return static_cast<shok_LeaderCommand>(EventGetIntById(shok_EventIDs::Leader_GetCommand));
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

int shok_EGL_CGLEEntity::LimitedAttachmentGetMaximum(shok_AttachmentType attachType)
{
	shok_GGL_CEventAttachmentTypeGetInteger ev{ shok_EventIDs::LimitedAttachment_GetMax, attachType };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	return ev.Data;
}


int shok_EGL_CGLEEntity::GetFirstAttachedToMe(shok_AttachmentType attachmentId)
{
	shok_attachment* r = ObserverEntities.GetFirstMatch([attachmentId](shok_attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}

int shok_EGL_CGLEEntity::GetFirstAttachedEntity(shok_AttachmentType attachmentId)
{
	shok_attachment* r = ObservedEntities.GetFirstMatch([attachmentId](shok_attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}
static inline void(__thiscall* entattach_attach)(int* th, shok_AttachmentType at, int id, int evth, int evot) = reinterpret_cast<void(__thiscall*)(int*, shok_AttachmentType, int, int, int)>(0x4A61B3);
void shok_EGL_CGLEEntity::AttachEntity(shok_AttachmentType attachtype, int otherId, int eventIdOnThisDetach, int eventIdOnOtherDetach)
{
	entattach_attach(&attachmentvt, attachtype, otherId, eventIdOnThisDetach, eventIdOnOtherDetach);
}
static inline void(__thiscall* entattach_detach)(int* th, shok_AttachmentType at, int id, byte ev) = reinterpret_cast<void(__thiscall*)(int*, shok_AttachmentType, int, byte)>(0x4A2E5D);
void shok_EGL_CGLEEntity::DetachObservedEntity(shok_AttachmentType attachtype, int otherId, bool fireEvent)
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
	shok_LeaderCommand com = EventLeaderGetCurrentCommand();
	return com == shok_LeaderCommand::Defend || com == shok_LeaderCommand::HoldPos || CurrentTaskListID == static_cast<shok_GGL_CGLSettlerProps*>(GetEntityType()->LogicProps)->IdleTaskList;
}

int shok_GGL_CBuilding::GetConstructionSite()
{
	return GetFirstAttachedToMe(shok_AttachmentType::CONSTRUCTION_SITE_BUILDING);
}

static inline int(__thiscall* const shok_build_getnearestfreeslot)(shok_EGL_CGLEEntity* th, shok_position* p) = reinterpret_cast<int(__thiscall*) (shok_EGL_CGLEEntity*, shok_position*)>(0x4AB02D);
int shok_GGL_CBuilding::GetNearestFreeConstructionSlotFor(shok_position* p)
{
	int cid = GetConstructionSite();
	if (!cid)
		return -1;
	shok_EGL_CGLEEntity* consi = shok_EGL_CGLEEntity::GetEntityByID(cid);
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
	if (CurrentState == shok_TaskState::BuildingAlarmDefend) // alarm mode
		return false;
	if (GetTechnologyInResearch())
		return false;
	{
		shok_GGL_CFoundryBehavior* f = GetBehavior<shok_GGL_CFoundryBehavior>();
		if (f && (f->CannonType != 0 || GetCannonProgress() != 100))
			return false;
	}
	if (GetBehavior<shok_GGL_CMarketBehavior>() && GetMarketProgress() < 1.0f)
		return false;
	if (GetBehavior<shok_GGL_CBarrackBehavior>()) {
		if (ObserverEntities.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == shok_AttachmentType::FIGHTER_BARRACKS; }))
			return false;
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

static inline void(__thiscall* const entitysethealth)(shok_EGL_CGLEEntity* th, int h) = reinterpret_cast<void(__thiscall* const)(shok_EGL_CGLEEntity *, int)>(0x57A6D9);
void shok_EGL_CGLEEntity::SetHealth(int h)
{
	entitysethealth(this, h);
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
static inline shok_EGL_CGLETaskList*(__thiscall* const shok_entity_GetCurrentTaskList)(shok_EGL_CGLEEntity* th) = reinterpret_cast<shok_EGL_CGLETaskList*(__thiscall* const)(shok_EGL_CGLEEntity*)>(0x57A892);
shok_EGL_CGLETaskList* shok_EGL_CGLEEntity::GetCurrentTaskList()
{
	return shok_entity_GetCurrentTaskList(this);
}
static inline shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* (__thiscall* const shok_entitytaskhandler_gettaskhandler)(shok_set<shok_entity_TaskIdAndTaskHandler>* th, shok_Task id) = reinterpret_cast<shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int * (__thiscall* const)(shok_set<shok_entity_TaskIdAndTaskHandler>*, shok_Task)>(0x57BDD3);
shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* shok_EGL_CGLEEntity::GetTaskHandler(shok_Task task)
{
	return shok_entitytaskhandler_gettaskhandler(&TaskHandlers, task);
}

void shok_EGL_CGLEEntity::Destroy()
{
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->Destroy(this, 0);
}

void shok_EGL_CGLEEntity::ClearAttackers()
{
	std::vector<shok_GGL_CSettler*> tomove = std::vector<shok_GGL_CSettler*>();
	ObserverEntities.ForAll([&tomove](shok_attachment* a) {
		if (a->AttachmentType == shok_AttachmentType::ATTACKER_COMMAND_TARGET || a->AttachmentType == shok_AttachmentType::LEADER_TARGET || a->AttachmentType == shok_AttachmentType::FOLLOWER_FOLLOWED) {
			shok_EGL_CGLEEntity* at = shok_EGL_CGLEEntity::GetEntityByID(a->EntityId);
			if (shok_GGL_CSettler* s = shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(at)) {
				tomove.emplace_back(s);
			}
		}
		});
	for (shok_GGL_CSettler* s : tomove)
		s->Defend();
}



void shok_EGL_CMovingEntity::AttackMove(shok_position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::Leader_AttackMove, p};
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	TargetRotationValid = 0;
}

void shok_EGL_CMovingEntity::AttackEntity(int targetId)
{
	shok_EGL_CEvent1Entity ev{ shok_EventIDs::Leader_AttackEntity, targetId };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::Move(shok_position& p)
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
	if (GetBehavior<shok_GGL_CLeaderBehavior>() && ObservedEntities.GetFirstMatch([](shok_attachment* a) { return a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER; })) {
		shok_EGL_CEventValue_bool ev{ shok_EventIDs::Leader_ExpellSoldier, true };
		reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
	}
	else {
		shok_entity_expellSettler(this, 1);
	}
}

void shok_EGL_CMovingEntity::HeroAbilitySendHawk(shok_position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::HeroHawk_SendHawk, p};
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityInflictFear()
{
	shok_BB_CEvent ev{ shok_EventIDs::InflictFear_Activate };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceBomb(shok_position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::BombPlacer_CommandPlaceBomb, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceCannon(shok_position& p, int FoundationType, int CannonType)
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

void shok_EGL_CMovingEntity::ScoutBinoculars(shok_position& p)
{
	shok_EGL_CEventPosition ev{ shok_EventIDs::Binocular_ExploreCommand, p };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutFindResource()
{
	shok_BB_CEvent ev{ shok_EventIDs::PointToResources_Activate };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutPlaceTorch(shok_position& p)
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

void(__thiscall* const movingentity_setpos)(shok_EGL_CMovingEntity* th, shok_position* p) = reinterpret_cast<void(__thiscall*)(shok_EGL_CMovingEntity*, shok_position*)>(0x57BC78);
void shok_EGL_CMovingEntity::SetPosition(shok_position& p)
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

void shok_GGL_CBuilding::MarketStartTrade(shok_ResourceType ResourceTypeSell, shok_ResourceType ResourceTypeBuy, float BuyAmount)
{
	shok_GGL_CEventTransaction e2{ shok_EventIDs::Market_StartTrade, ResourceTypeSell, ResourceTypeBuy, BuyAmount };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
}

void shok_GGL_CBuilding::MarketCancelTrade()
{
	shok_BB_CEvent e2{ shok_EventIDs::Market_CancelTrade };
	reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(vtable)->FireEvent(this, &e2);
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
		c.ScriptName = (char*)shok_malloc(sizeof(char) * len);
		strcpy_s(c.ScriptName, len, e->ScriptName);
	}
	else {
		c.ScriptName = nullptr;
	}
	int nid = (*shok_EGL_CGLEGameLogic::GlobalObj)->CreateEntity(&c);
	if (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e)) {
		shok_GGL_CLeaderBehavior* lb = e->GetBehavior<shok_GGL_CLeaderBehavior>();
		if (lb) {
			std::vector<int> sol = std::vector<int>();
			e->ObservedEntities.ForAll([&sol](shok_attachment* a) { if (a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER) sol.push_back(a->EntityId); });
			shok_GGL_CSettler* settler = static_cast<shok_GGL_CSettler*>(shok_EGL_CGLEEntity::GetEntityByID(nid));
			for (int i : sol) {
				settler->LeaderAttachSoldier(fixedChangePlayer(i, pl));
			}
			shok_GGL_CLeaderBehavior* nlb = settler->GetBehavior<shok_GGL_CLeaderBehavior>();
			nlb->Experience = lb->Experience;
			nlb->TroopHealthCurrent = lb->TroopHealthCurrent;
			nlb->TroopHealthPerSoldier = lb->TroopHealthPerSoldier;
		}
	}
	if (shok_GGL_CReplaceableEntityBehavior* rep = e->GetBehavior<shok_GGL_CReplaceableEntityBehavior>()) {
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
	shok_saveVirtualProtect vp{ shok_EGL_CGLEEntity::EntityIDChangePlayer, 10 };
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4FCD26), 10 };
	RedirectCall(reinterpret_cast<void*>(0x4FCD26), &hero6convertchangeplayer);
}

int shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity = 0;
void __fastcall camo_behaviorReset(shok_GGL_CCamouflageBehavior* th, int _, int a) {
	if (shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity == 0 || th->EntityId == shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity)
		th->InvisibilityRemaining = 0;
}
bool HookResetCamo_Hooked = false;
void shok_EGL_CGLEEntity::HookResetCamo()
{
	if (HookResetCamo_Hooked)
		return;
	HookResetCamo_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x5011DF), 10 };
	WriteJump(reinterpret_cast<void*>(0x5011DF), &camo_behaviorReset);
}

static inline int(__thiscall* const activateCamoOrig)(shok_GGL_CCamouflageBehavior* th) = reinterpret_cast<int(__thiscall*)(shok_GGL_CCamouflageBehavior*)>(0x501561);
void (*shok_EGL_CGLEEntity::CamoActivateCb)(shok_GGL_CCamouflageBehavior* th);
int __fastcall camoActivateHook(shok_GGL_CCamouflageBehavior* th) {
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4D51A4), 10 };
	shok_saveVirtualProtect vp2{ reinterpret_cast<void*>(0x50163A), 10 };
	RedirectCall(reinterpret_cast<void*>(0x4D51A4), &camoActivateHook);
	RedirectCall(reinterpret_cast<void*>(0x50163A), &camoActivateHook);
}


static inline void(__thiscall* const event2entitiesctor)(int* e, int id, int at, int de) = reinterpret_cast<void(__thiscall*)(int*, int, int, int)>(0x49847F);
int* shok_EGL_CGLEEntity::HurtEntityDamagePointer = nullptr;
void __stdcall hurtentityhookc(int* damage, shok_EGL_CGLEEntity** target, shok_EGL_CGLEEntity** attacker) { // argument order is reversed, for asm reasons
	if (!*target)
		return;
	int aid = 0;
	if (*attacker)
		aid = (*attacker)->EntityId;
	int tid = (*target)->EntityId;
	int ev[6];
	event2entitiesctor(ev, 0x1C007, aid, tid);
	shok_EGL_CGLEEntity::HurtEntityDamagePointer = damage;
	(*shok_EScr_CScriptTriggerSystem::GlobalObj)->RunTrigger((shok_BB_CEvent*)ev);
	shok_EGL_CGLEEntity::HurtEntityDamagePointer = nullptr;
}

void __declspec(naked) hurtentityhook() { // push arguments, call func, do the move i did override, last jump back
	__asm {
		mov eax, esp;
		add eax, 0x4;
		push eax;
		add eax, 0x4;
		push eax;
		add eax, 0x4;
		push eax;
		call hurtentityhookc;

		mov eax, 0x72d687;
		push 0x49f35d;
		ret;
	}
}
bool HookHurtEntity_Hooked = false;
void shok_EGL_CGLEEntity::HookHurtEntity()
{
	if (HookHurtEntity_Hooked)
		return;
	if (HasSCELoader())
		DEBUGGER_BREAK;
	HookHurtEntity_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x49f358), 0x49F4AD - 0x49f358 + 10 };
	WriteJump(reinterpret_cast<void*>(0x49f358), &hurtentityhook); // call my own func, that calls the trigger
	byte* p = reinterpret_cast<byte*>(0x49F4A6); // remove call to trigger
	*p = 0x90; // nop
	p = reinterpret_cast<byte*>(0x49F4A7);
	*p = 0x90;
	p = reinterpret_cast<byte*>(0x49F4AC);
	*p = 0x90;
	p = reinterpret_cast<byte*>(0x49F4AD);
	*p = 0x90;
}

std::multimap<int, int> shok_EGL_CGLEEntity::BuildingMaxHpTechBoni = std::multimap<int, int>();
bool shok_EGL_CGLEEntity::UseMaxHPTechBoni = false;
int __fastcall hookGetMaxHP(shok_EGL_CGLEEntity* e) {
	float hp;
	entityAddonData* d = e->GetAdditionalData(false);
	shok_GGlue_CGlueEntityProps* et = e->GetEntityType();
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
			for (int t : static_cast<shok_GGL_CGLSettlerProps*>(et->LogicProps)->ModifyHitpoints.TechList) {
				if ((*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != shok_TechState::Researched)
					continue;
				shok_technology* tech = (*shok_GGL_CGLGameLogic::GlobalObj)->GetTech(t);
				hp = tech->HitpointModifier.ModifyValue(hp);
			}
		}
		else if (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e)) {
			std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> it = shok_EGL_CGLEEntity::BuildingMaxHpTechBoni.equal_range(e->EntityType);
			for (std::multimap<int, int>::iterator i = it.first; i != it.second; ++i) {
				int t = i->second;
				if ((*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->GetTechStatus(t) != shok_TechState::Researched)
					continue;
				shok_technology* tech = (*shok_GGL_CGLGameLogic::GlobalObj)->GetTech(t);
				hp = tech->HitpointModifier.ModifyValue(hp);
			}
		}
	}
	return (int) hp;
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x57B798), 10 };
	shok_saveVirtualProtect vp2{ reinterpret_cast<void*>(0x4BDED8), 10 };
	shok_saveVirtualProtect vp3{ reinterpret_cast<void*>(0x571B93), 10 };
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
	shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* thand = static_cast<shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int*>(shok_malloc(sizeof(shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int)));
	thand->vtable = 0x783D7C; // this is EGL::THandler<12,EGL::CGLETaskArgs,EGL::CGLETaskArgs,EGL::CGLEEntity,int>, but the func is always the same anyway...
	thand->Object = th;
	thand->Func = reinterpret_cast<int(__thiscall *)(void* th, shok_EGL_CGLETaskArgs * args)>(&entity_executeluatask);
	thand->Zero = 0;
	vt->AddTaskHandler(th, shok_Task::TASK_LUA_FUNC, thand);
	shok_EGL_IGLEStateHandler* shand = static_cast<shok_EGL_IGLEStateHandler*>(shok_malloc(sizeof(shok_EGL_IGLEStateHandler)));
	shand->vtable = 0x783D9C; // EGL::TStateHandler<EGL::CGLEEntity>
	shand->Object = th;
	shand->Func = reinterpret_cast<int(__thiscall *)(void*, int)>(&entity_executeluataskstate);
	shand->Zero = 0;
	vt->AddStateHandler(th, shok_TaskState::LuaFunc, shand);
}
bool HookLuaTaskList_Hooked = false;
void shok_EGL_CGLEEntity::HookLuaTaskList()
{
	if (HookLuaTaskList_Hooked)
		return;
	HookLuaTaskList_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x57D6CA), 10 };
	RedirectCall(reinterpret_cast<void*>(0x57D6CA), &entity_addluatlhandlershook);
}

int __fastcall entity_hooknoncancelanim_task(shok_GGL_CGLBehaviorAnimationEx* th, int _, shok_EGL_CGLETaskArgs* args) {
	int wait = static_cast<shok_EGL_CGLETaskArgsThousandths*>(args)->Thousandths;
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	shok_vtable_EGL_CGLEEntity* vt = reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(e->vtable);
	th->TurnToWaitFor = th->StartTurn + th->Duration * wait / 1000;
	vt->SetTaskState(e, shok_TaskState::WaitForAnimNonCancelable);
	return 1;
}
void __stdcall entity_hooknoncancelanim(shok_EGL_CGLEEntity* th, shok_GGL_CGLBehaviorAnimationEx* beh) {
	if (beh->vtable != shok_GGL_CGLBehaviorAnimationEx::vtp)
		DEBUGGER_BREAK;
	shok_vtable_EGL_CGLEEntity* vt = reinterpret_cast<shok_vtable_EGL_CGLEEntity*>(th->vtable);
	shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* thand = static_cast<shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int*>(shok_malloc(sizeof(shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int)));
	thand->vtable = 0x784A4C; // EGL::THandler<5,EGL::CGLETaskArgs,EGL::CGLETaskArgsThousandths,EGL::CBehaviorAnimation,int>
	thand->Object = beh;
	thand->Func = reinterpret_cast<int(__thiscall*)(void* th, shok_EGL_CGLETaskArgs * args)>(entity_hooknoncancelanim_task);
	thand->Zero = 0;
	vt->AddTaskHandler(th, shok_Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE, thand);
	shok_EGL_IGLEStateHandler* shand = static_cast<shok_EGL_IGLEStateHandler*>(shok_malloc(sizeof(shok_EGL_IGLEStateHandler)));
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x588408), 10 };
	WriteJump(reinterpret_cast<void*>(0x588408), &entity_hooknoncancelanim_asm);
}


shok_EGL_CGLEEntity* shok_EGL_CGLEEntity::ReplaceEntityWithResourceEntity(shok_EGL_CGLEEntity* e)
{
	shok_EGL_CGLEEntityProps* ty = e->GetEntityType()->LogicProps;
	if (ty->vtable != shok_GGL_CEntityProperties::vtp)
		return nullptr;
	shok_GGL_CEntityProperties* t = static_cast<shok_GGL_CEntityProperties*>(ty);
	shok_GGL_CResourceDoodadCreator c{};
	c.EntityType = t->ResourceEntity;
	c.PlayerId = e->PlayerId;
	c.Pos = e->Position;
	c.ResourceAmount = t->ResourceAmount;
	c.Scale = e->Scale;
	if (e->ScriptName) {
		size_t len = strlen(e->ScriptName) + 1;
		c.ScriptName = (char*)shok_malloc(sizeof(char) * len);
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x57C94A), 10 };
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

float __fastcall entitygetdamagemod(shok_GGL_CBattleBehavior* b) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(b->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		return static_cast<float>(d->DamageOverride);
	else
		return static_cast<float>(e->GetEntityType()->GetBehaviorProps<shok_GGL_CBattleBehaviorProps>()->DamageAmount);
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
shok_EGL_CEventGetValue_int* __fastcall entitydamagemodeventautocannonasm(shok_GGL_CAutoCannonBehavior* th, int, shok_EGL_CEventGetValue_int* ev) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		ev->Data = d->DamageOverride;
	else
		ev->Data = e->GetEntityType()->GetBehaviorProps<shok_GGL_CAutoCannonBehaviorProps>()->DamageAmount;
	return ev;
}
void __declspec(naked) entitydamagemodbattleprojectile() {
	__asm {
		test al, al;
		jz arrow;

		mov ecx, [ebp - 0x10]; // attacker obj
		mov eax, [ecx + 6 * 4]; // player
		mov[ebp - 0x28], eax; // source player

		call shok_EGL_CGLEEntity::EventGetDamage;
		push 0x50C3F7;
		ret;

	arrow:
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x50C235), 0x51076C - 0x50C235 + 10 };
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
	shok_GGlue_CGlueEntityProps* p = e->GetEntityType();
	if (p->IsSettlerType())
		return static_cast<shok_GGL_CGLSettlerProps*>(p->LogicProps)->ArmorAmount;
	else if (p->IsBuildingType())
		return static_cast<shok_GGL_CGLBuildingProps*>(p->LogicProps)->ArmorAmount;
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4A6B15), 10};
	shok_saveVirtualProtect vp2{ reinterpret_cast<void*>(0x4AB160), 10 };
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4A4AC3), 10 };
	shok_saveVirtualProtect vp2{ reinterpret_cast<void*>(0x4AB199), 10 };
	WriteJump(reinterpret_cast<void*>(0x4A4AC3), &entityexplmodsettasm);
	WriteJump(reinterpret_cast<void*>(0x4AB199), &entityexplmodbuildasm);
}

int shok_GGL_CSettler::LeaderGetRegenHealth()
{
	entityAddonData* d = GetAdditionalData(false);
	if (d && d->RegenHPOverride >= 0)
		return d->RegenHPOverride;
	else
		return GetEntityType()->GetBehaviorProps<shok_GGL_CLeaderBehaviorProps>()->HealingPoints;
}
int shok_GGL_CSettler::LeaderGetRegenHealthSeconds()
{
	entityAddonData* d = GetAdditionalData(false);
	if (d && d->RegenSecondsOverride >= 0)
		return d->RegenSecondsOverride;
	else
		return GetEntityType()->GetBehaviorProps<shok_GGL_CLeaderBehaviorProps>()->HealingSeconds;
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
	shok_GGL_CLeaderBehavior* lb = GetBehavior<shok_GGL_CLeaderBehavior>();
	if (lb && healSoldiers) {
		int shp = lb->GetTroopHealth(), smp = lb->GetTroopHealthPerSoldier();
		int numsol = 0;
		ObservedEntities.ForAll([&numsol](shok_attachment* a) { if (a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER) numsol++; });
		smp *= numsol;
		shp += r;
		if (shp > smp)
			shp = smp;
		lb->TroopHealthCurrent = shp;
		return;
	}
}

bool shok_EGL_CGLEEntity::LeaderRegenRegenerateSoldiers = false;
void __fastcall leaderregen(shok_GGL_CLeaderBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	th->SecondsSinceHPRefresh = 0;
	int hp = e->Health, mhp = e->GetMaxHealth();
	if (hp <= 0)
		return;
	int r = static_cast<shok_GGL_CSettler*>(e)->LeaderGetRegenHealth();
	e->PerformHeal(r, shok_EGL_CGLEEntity::LeaderRegenRegenerateSoldiers);
}
void __fastcall leaderregenseconds(shok_GGL_CLeaderBehavior* th) {
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4EAE92), 10 };
	shok_saveVirtualProtect vp2{ reinterpret_cast<void*>(0x4EFC29), 10 };
	WriteJump(reinterpret_cast<void*>(0x4EAE92), &leaderregen);
	WriteJump(reinterpret_cast<void*>(0x4EFC29), &leaderregensecondsasm);
}

float __fastcall leadermaxrange(shok_GGL_CBattleBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->MaxRangeOverride >= 0)
		return d->MaxRangeOverride;
	else
		return e->GetEntityType()->GetBehaviorProps<shok_GGL_CLeaderBehaviorProps>()->MaxRange;
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
float __fastcall autocannonmaxrange(shok_GGL_CAutoCannonBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	entityAddonData* d = e->GetAdditionalData(false);
	if (d && d->MaxRangeOverride >= 0)
		return d->MaxRangeOverride;
	else
		return e->GetEntityType()->GetBehaviorProps<shok_GGL_CAutoCannonBehaviorProps>()->MaxAttackRange;
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x50AB48), 10 };
	shok_saveVirtualProtect vp2{ reinterpret_cast<void*>(0x50F50D), 10 };
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x53F911), 10 };
	WriteJump(reinterpret_cast<void*>(0x53F911), &entitydisplaynameasm);
}

void __fastcall rangedeffecthealhook(shok_GGL_CRangedEffectAbility* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	shok_GGL_CRangedEffectAbilityProps* pr = e->GetEntityType()->GetBehaviorProps<shok_GGL_CRangedEffectAbilityProps>();
	float hpfact = pr->HealthRecoveryFactor;
	if (hpfact <= 0)
		return;
	shok_EGL_CGLEEffectCreator ecr{};
	ecr.PlayerID = e->PlayerId;
	ecr.EffectType = pr->HealEffect;
	e->ObservedEntities.ForAll([hpfact, &ecr](shok_attachment* a) {
		if (a->AttachmentType == shok_AttachmentType::HERO_AFFECTED) {
			shok_EGL_CGLEEntity* toheal = shok_EGL_CGLEEntity::GetEntityByID(a->EntityId);
			if (!toheal)
				return;
			float heal = toheal->GetMaxHealth() * hpfact;
			if (ecr.EffectType) {
				ecr.StartPos.X = toheal->Position.X;
				ecr.StartPos.Y = toheal->Position.Y;
				(*shok_EGL_CGLEGameLogic::GlobalObj)->CreateEffect(&ecr);
			}
			if (toheal->GetBehavior<shok_GGL_CSoldierBehavior>()) {
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
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4E3C78), 10 };
	if (hookActive)
		RedirectCall(reinterpret_cast<void*>(0x4E3C78), &rangedeffecthealhook);
	else
		RedirectCall(reinterpret_cast<void*>(0x4E3C78), reinterpret_cast<void*>(0x4E39B4));
}

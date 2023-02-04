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
#include "s5_classfactory.h"
#include "luaext.h"
#include "hooks.h"
#include "savegame_extra.h"
#include "EntityAddonData.h"

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
bool EGL::CGLEEntity::EventIsInvisible()
{
	EGL::CEventGetValue_Bool ev{ shok::EventIDs::Camouflage_IsInvisible };
	FireEvent(&ev);
	return ev.Data;
}
bool EGL::CGLEEntity::EventIsSettlerOrBuilding()
{
	EGL::CEventGetValue_Bool ev{ shok::EventIDs::IsSettlerOrBuilding };
	FireEvent(&ev);
	return ev.Data;
}
bool EGL::CGLEEntity::EventIsWorker()
{
	EGL::CEventGetValue_Bool ev{ shok::EventIDs::IsWorker };
	FireEvent(&ev);
	return ev.Data;
}
bool EGL::CGLEEntity::EventIsBattleOrAutocannon()
{
	EGL::CEventGetValue_Bool ev{ shok::EventIDs::IsBattleOrAutocannon };
	FireEvent(&ev);
	return ev.Data;
}
bool EGL::CGLEEntity::EventIsSerfOrWorker()
{
	EGL::CEventGetValue_Bool ev{ shok::EventIDs::IsSerfOrWorker };
	FireEvent(&ev);
	return ev.Data;
}
bool EGL::CGLEEntity::EventIsSoldier()
{
	EGL::CEventGetValue_Bool ev{ shok::EventIDs::IsSoldier };
	FireEvent(&ev);
	return ev.Data;
}
int EGL::CGLEEntity::EventGetWorktime()
{
	return EventGetIntById(shok::EventIDs::Worker_GetWorkTimeRemaining);
}
float EGL::CGLEEntity::EventGetMotivation()
{
	EGL::CEventGetValue_Float ev{ shok::EventIDs::Worker_GetMotivation };
	FireEvent(&ev);
	return ev.Data;
}


int EGL::CGLEEntity::GetFirstAttachedToMe(shok::AttachmentType attachmentId) const
{
	for (const auto& r : ObserverEntities) {
		if (r.first == attachmentId)
			return r.second.EntityId;
	}
	return 0;
}

int EGL::CGLEEntity::GetFirstAttachedEntity(shok::AttachmentType attachmentId) const
{
	for (const auto& r : ObservedEntities) {
		if (r.first == attachmentId)
			return r.second.EntityId;
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

static inline void(__thiscall* const entityprofile_ref_checkinit)(GGL::CEntityProfile::EntityRef* th) = reinterpret_cast<void(__thiscall*)(GGL::CEntityProfile::EntityRef*)>(0x4C7062);
void GGL::CEntityProfile::EntityRef::CheckInit()
{
	entityprofile_ref_checkinit(this);
}

static inline GGL::ExperienceClass::LevelData* (__thiscall* const entityref_getxpclasslvl)(GGL::CEntityProfile::EntityRef* th, int lvl) = reinterpret_cast<GGL::ExperienceClass::LevelData * (__thiscall*)(GGL::CEntityProfile::EntityRef*, int)>(0x4C66BA);
const GGL::ExperienceClass::LevelData* GGL::CEntityProfile::GetExperienceClassLevel()
{
	return entityref_getxpclasslvl(&this->EntityReference, static_cast<int>(ExperienceLevel.Value));
}


void GGL::CEntityProfile::HookExperience(bool active)
{
	byte opcode = active ? 0x7C : 0x7E; // ? JumpIfLess : JumpIfLessOrEqual
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x10, {
		reinterpret_cast<void*>(0x4C6A68),
		reinterpret_cast<void*>(0x4C6CE8),
		reinterpret_cast<void*>(0x4C6C4E),
		reinterpret_cast<void*>(0x4C6FB7),
		reinterpret_cast<void*>(0x4C6DD3),
		reinterpret_cast<void*>(0x4C6E5C),
		reinterpret_cast<void*>(0x4C6B15),
		reinterpret_cast<void*>(0x4C6BA1),
		reinterpret_cast<void*>(0x4C6F09),

		reinterpret_cast<void*>(0x4C6F28),
	} };
	*reinterpret_cast<byte*>(0x4C6A68) = opcode; // get explo
	*reinterpret_cast<byte*>(0x4C6CE8) = opcode; // get move speed
	*reinterpret_cast<byte*>(0x4C6C4E) = opcode; // get damage
	*reinterpret_cast<byte*>(0x4C6FB7) = opcode; // get max attack range
	*reinterpret_cast<byte*>(0x4C6DD3) = opcode; // get damage bonus
	*reinterpret_cast<byte*>(0x4C6E5C) = opcode; // get dodge
	*reinterpret_cast<byte*>(0x4C6B15) = opcode; // get autoattack range
	*reinterpret_cast<byte*>(0x4C6BA1) = opcode; // get healing points
	*reinterpret_cast<byte*>(0x4C6F09) = opcode; // get miss chance
	if (active) {
		CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x4C6F28), 2); // nop out the check to only use positive xp miss chance modifiers
	}
	else {
		*reinterpret_cast<byte*>(0x4C6F28) = 0x74;
		*reinterpret_cast<byte*>(0x4C6F29) = 0x08;
	}
}

void GGL::CEntityProfile::EntityRef::AssignAdvExperienceClass()
{
	ExperienceClass = shok::ExperienceClass::Invalid;
	for (const auto& d : GGL::ExperienceClassHolder::EntityCategoryToExperienceClass) {
		if (Self->IsEntityInCategory(d.EntityCategory)) {
			ExperienceClass = d.ExperienceClass;
			break;
		}
	}
}
void __declspec(naked) entityprofile_experienceclassassignmenthook_asm() {
	__asm {
		mov ecx, esi;
		call GGL::CEntityProfile::EntityRef::AssignAdvExperienceClass;

		push 0x4C72AB;
		ret;
	};
}
byte entityprofile_experienceclassassignmenthook_backup[10]{};
void GGL::CEntityProfile::HookExperienceClassAssignment(bool active) {
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4C71CA), 0x10 };
	if (active)
		CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4C71CA), &entityprofile_experienceclassassignmenthook_asm, reinterpret_cast<void*>(0x4C71D0), entityprofile_experienceclassassignmenthook_backup);
	else if (entityprofile_experienceclassassignmenthook_backup[0])
		CppLogic::Hooks::RestoreJumpBackup(reinterpret_cast<void*>(0x4C71CA), entityprofile_experienceclassassignmenthook_backup);
	CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x4C7039), 0x10 };
	CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x4C7039), reinterpret_cast<void*>(0x4C703E));
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
void GGL::CResourceDoodad::SetCurrentResourceAmount(int am)
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
	return ConstructionProgress >= 1;
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
			for (const auto& a : ObserverEntities)
				if (a.first == shok::AttachmentType::FIGHTER_BARRACKS && !EGL::CGLEEntity::GetEntityByID(a.second.EntityId)->GetBehavior<GGL::CSoldierBehavior>())
					c++;
			if (c >= 3)
				return false;
		}
		else {
			for (const auto& a : ObserverEntities)
				if (a.first == shok::AttachmentType::FIGHTER_BARRACKS)
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

int __thiscall GGL::CBuilding::GetBaseArmor()
{
	auto* d = GetAdditionalData(false);
	if (d && d->ArmorOverride >= 0)
		return d->ArmorOverride;
	GGlue::CGlueEntityProps* p = GetEntityType();
	if (const auto* l = dynamic_cast<GGL::CGLBuildingProps*>(p->LogicProps))
		return l->ArmorAmount;
	return 0;
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
static inline EGL::TaskHandler* (__thiscall* const shok_entitytaskhandler_gettaskhandler)(shok::Map<shok::Task, EGL::TaskHandler*>* th, shok::Task id) = reinterpret_cast<EGL::TaskHandler * (__thiscall* const)(shok::Map<shok::Task, EGL::TaskHandler*>*, shok::Task)>(0x57BDD3);
EGL::TaskHandler* EGL::CGLEEntity::GetTaskHandler(shok::Task task)
{
	return shok_entitytaskhandler_gettaskhandler(&TaskHandlers, task);
}

void EGL::CGLEEntity::ExecuteTask(EGL::CGLETaskArgs& targ)
{
	ExecuteTask(&targ);
}
static inline void(__thiscall* const entity_adddelayedevent)(EGL::CGLEEntity* th, BB::CEvent* ev, int u) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEntity*, BB::CEvent*, int)>(0x57D078);
void EGL::CGLEEntity::AddDelayedEvent(BB::CEvent* ev)
{
	entity_adddelayedevent(this, ev, 0);
}

void EGL::CGLEEntity::Destroy()
{
	DestroyVT(0);
}

static inline bool(__stdcall* const entity_checkdodge)(EGL::CGLEEntity* th) = reinterpret_cast<bool(__stdcall*)(EGL::CGLEEntity*)>(0x50ABB9);
bool EGL::CGLEEntity::CheckDodge()
{
	return !entity_checkdodge(this); // returns dodge failed
}

inline bool(__thiscall* const entity_isinblocking)(const EGL::CGLEEntity* th) = reinterpret_cast<bool(__thiscall*)(const EGL::CGLEEntity*)>(0x57ACF5);
bool EGL::CGLEEntity::IsInBlocking() const
{
	return entity_isinblocking(this);
}

bool EGL::CGLEEntity::IsDead() const
{
	return Health <= 0 || IsInBlocking();
}

inline shok::AccessCategory(__thiscall* const entity_getaccesscat)(const EGL::CGLEEntity* e) = reinterpret_cast<shok::AccessCategory(__thiscall*)(const EGL::CGLEEntity*)>(0x57BA1F);
shok::AccessCategory EGL::CGLEEntity::GetAccessCategory() const
{
	return entity_getaccesscat(this);
}

float __thiscall EGL::CGLEEntity::GetBaseExploration()
{
	auto* d = GetAdditionalData(false);
	if (d && d->ExplorationOverride >= 0.0f)
		return d->ExplorationOverride;
	return GetEntityType()->LogicProps->Exploration;
}

void EGL::CGLEEntity::ClearAttackers()
{
	std::vector<GGL::CSettler*> tomove = std::vector<GGL::CSettler*>();
	for (const auto& a : ObserverEntities) {
		if (a.first == shok::AttachmentType::ATTACKER_COMMAND_TARGET || a.first == shok::AttachmentType::LEADER_TARGET || a.first == shok::AttachmentType::FOLLOWER_FOLLOWED) {
			EGL::CGLEEntity* at = EGL::CGLEEntity::GetEntityByID(a.second.EntityId);
			if (GGL::CSettler* s = dynamic_cast<GGL::CSettler*>(at)) {
				tomove.emplace_back(s);
			}
		}
	}
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
	if (GetBehavior<GGL::CLeaderBehavior>() && ObservedEntities.GetFirstMatch([](std::pair<shok::AttachmentType, shok::Attachment>* a) { return a->first == shok::AttachmentType::LEADER_SOLDIER; })) {
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

static inline void(__thiscall* const build_getabsapproachpos)(GGL::CBuilding* th, shok::Position* p) = reinterpret_cast<void(__thiscall*)(GGL::CBuilding*, shok::Position*)>(0x4A9F0B);
shok::Position GGL::CBuilding::GetAbsoluteApproachPos()
{
	shok::Position p{};
	build_getabsapproachpos(this, &p);
	return p;
}

std::vector<shok::AdditionalTechModifier> GGL::CBuilding::ConstructionSpeedModifiers{};
float __fastcall constructionsite_getprogresspertick_hook(GGL::CConstructionSite* th) { // param is constructionsite, just not done yet ;)
	GGL::CGLSettlerProps* serf = static_cast<GGL::CGLSettlerProps*>((*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(*GGlue::CGlueEntityProps::EntityTypeIDSerf)->LogicProps);
	GGL::CGLBuildingProps* bty = static_cast<GGL::CGLBuildingProps*>((*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(th->BuildingType)->LogicProps);
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

void __thiscall GGL::CBridgeEntity::ApplyHeightOverride()
{
	GGL::CBridgeProperties* p = static_cast<GGL::CBridgeProperties*>(GetEntityType()->LogicProps);
	auto* lr = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes;
	int h = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes->GetTerrainHeight(Position) + p->Height;
	for (const shok::AARect& area : p->BridgeArea) {
		EGL::CGLELandscape::AdvancedAARectIterator it{ Position, area, Position.r, !EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled, true };
		for (const auto& c : it) {
			if (!lr->IsBridgeHeightCoordValid(c.x, c.y))
				continue;
			*lr->GetBridgeHeightP(c.x, c.y) = h;
		}
	}
}

EGL::CGLEEntity* EGL::CGLEEntity::AdvChangePlayer(int player)
{
	if (PlayerId == player)
		return this;

	EGL::CGLEEntityCreator c{};
	c.EntityType = EntityType;
	c.PlayerId = player;
	c.Pos = Position;
	c.Scale = Scale;
	c.Health = Health;
	if (ScriptName) {
		size_t len = strlen(ScriptName) + 1;
		c.ScriptName = (char*)shok::Malloc(sizeof(char) * len);
		strcpy_s(c.ScriptName, len, ScriptName);
	}
	else {
		c.ScriptName = nullptr;
	}
	int nid = (*EGL::CGLEGameLogic::GlobalObj)->CreateEntity(&c);
	EGL::CGLEEntity* ne = EGL::CGLEEntity::GetEntityByID(nid);

	if (GGL::CLeaderBehavior* lb = GetBehavior<GGL::CLeaderBehavior>()) {
		std::vector<int> sol = std::vector<int>();
		for (const auto& a : ObservedEntities) {
			if (a.first == shok::AttachmentType::LEADER_SOLDIER)
				sol.push_back(a.second.EntityId);
		}
		GGL::CSettler* settler = static_cast<GGL::CSettler*>(ne);
		for (int i : sol) {
			settler->LeaderAttachSoldier(EGL::CGLEEntity::GetEntityByID(i)->AdvChangePlayer(player)->EntityId);
		}
		GGL::CLeaderBehavior* nlb = settler->GetBehavior<GGL::CLeaderBehavior>();
		nlb->Experience = lb->Experience;
		nlb->TroopHealthCurrent = lb->TroopHealthCurrent;
		nlb->TroopHealthPerSoldier = lb->TroopHealthPerSoldier;
	}

	if (GGL::CReplaceableEntityBehavior* rep = GetBehavior<GGL::CReplaceableEntityBehavior>()) {
		rep->IsReplacementActive = false;
	}

	Destroy();
	return ne;
}
int __cdecl EGL::CGLEEntity::FixedChangePlayer(int id, int pl)
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
	if (e == nullptr) {
		return 0;
	}
	return e->AdvChangePlayer(pl)->EntityId;
}
bool ActivateEntityChangePlayerFix_Hooked = false;
void EGL::CGLEEntity::ActivateEntityChangePlayerFix()
{
	if (ActivateEntityChangePlayerFix_Hooked)
		return;
	ActivateEntityChangePlayerFix_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ EGL::CGLEEntity::EntityIDChangePlayer, 0x49A6AC - 0x49A6A7 };
	CppLogic::Hooks::WriteJump(EGL::CGLEEntity::EntityIDChangePlayer, &EGL::CGLEEntity::FixedChangePlayer, reinterpret_cast<void*>(0x49A6AC));
}


static inline void(__thiscall* const event2entitiesctor)(int* e, int id, int at, int de) = reinterpret_cast<void(__thiscall*)(int*, int, int, int)>(0x49847F);
bool EGL::CGLEEntity::HurtEntityCallWithNoAttacker = false;
bool HookHurtEntity_Hooked = false;
void __cdecl EGL::CGLEEntity::FixedHurtEntity(EGL::CGLEEntity* att, EGL::CGLEEntity* tar, int dmg)
{
	tar->AdvancedHurtEntityBy(att, dmg, 0, true, true, true, shok::AdvancedDealDamageSource::Unknown);
}
void __cdecl EGL::CGLEEntity::FixedHurtEntityAoE(EGL::CGLEEntity* att, shok::Position* p, float r, int dmg, int pl, int dmgcl)
{
	EGL::CGLEEntity::AdvancedDealAoEDamage(att, *p, r, dmg, pl, dmgcl, true, true, true, shok::AdvancedDealDamageSource::Unknown);
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
	} };
	CppLogic::Hooks::WriteJump(EGL::CGLEEntity::EntityHurtEntity, &EGL::CGLEEntity::FixedHurtEntity, reinterpret_cast<void*>(0x49F35D));
	CppLogic::Hooks::WriteJump(EGL::CGLEEntity::EntityDealAoEDamage, &EGL::CGLEEntity::FixedHurtEntityAoE, reinterpret_cast<void*>(0x49F82F));
	GGL::CArrowEffect::HookDealDamage();
	GGL::CBattleBehavior::HookDealDamage();
	GGL::CCircularAttack::HookDealDamage();
	GGL::CCannonBallEffect::HookDealDamage();
	GGL::CBombBehavior::HookDealDamage();
	GGL::CShurikenAbility::HookDealDamage();

	HookDamageMod(); // set projectile player field in creator
	GGL::CBombPlacerBehavior::HookFixBombAttachment();
	GGL::CCannonBallEffect::AddDamageSourceOverride = true;
}

float EGL::CGLEEntity::CalculateDamageAgainstMe(int damage, int damageclass, float aoeFactor)
{
	float dmg = static_cast<float>(damage) * aoeFactor;
	EGL::CEventGetValue_Int getac{ shok::EventIDs::GetArmorClass };
	FireEvent(&getac);
	EGL::CEventGetValue_Int geta{ shok::EventIDs::GetArmor };
	FireEvent(&geta);

	if (damageclass > 0 && damageclass < static_cast<int>((*GGL::DamageClassesHolder::GlobalObj)->DamageClassList.size()))
		dmg *= (*GGL::DamageClassesHolder::GlobalObj)->DamageClassList[damageclass]->GetBonusVsArmorClass(getac.Data);
	dmg -= geta.Data;
	return dmg;
}

bool EGL::CGLEEntity::AdvHurtEntity_CheckOverHeal = false;
void EGL::CGLEEntity::AdvancedHurtEntityBy(EGL::CGLEEntity* attacker, int damage, int attackerFallback, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo)
{
	if ((*GGL::CGLGameLogic::GlobalObj)->GlobalInvulnerability)
		return;
	if (Health <= 0)
		return;
	if (!EventIsSettlerOrBuilding() && !dynamic_cast<GGL::CBridgeEntity*>(this))
		return;
	if (GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING) || GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE))
		return;
	int attackerplayer = attacker ? attacker->PlayerId : attackerFallback;
	if (attacker || EGL::CGLEEntity::HurtEntityCallWithNoAttacker) {
		CppLogic::Events::AdvHurtEvent ev{ shok::EventIDs::LogicEvent_HurtEntity, attacker ? attacker->EntityId : 0, EntityId, damage, sourceInfo, attackerplayer };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
		damage = ev.Damage;
	}
	if (damage <= 0)
		return;
	if (attacker && !attacker->EventIsInvisible()) {
		if (!EventIsWorker() || !(*EGL::CGLEGameLogic::GlobalObj)->Landscape->IsPosBlockedInMode(&Position, EGL::CGLELandscape::BlockingMode::Blocked)) {
			if (!GGL::CPlayerStatus::ArePlayersFriendly(PlayerId, attacker->PlayerId)) {
				CppLogic::Events::AdvHurtByEvent ev{ shok::EventIDs::OnAttackedBy, attacker->EntityId, damage, sourceInfo, attackerplayer };
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
			if (a.first == shok::AttachmentType::GUARD_GUARDED) {
				EGL::CEvent1Entity ev{ shok::EventIDs::Leader_OnGuardedAttackedBy, attacker->EntityId };
				EGL::CGLEEntity::GetEntityByID(a.second.EntityId)->FireEvent(&ev);
			}
		}
	}

	std::vector<int> idskilled{};
	int xptoadd = 0;
	EGL::CGLEEntity* firsttodie = this;
	int damageDone = 0;
	if (EventIsBattleOrAutocannon() && !EventIsSerfOrWorker()) { // has potentially soldiers
		EGL::CGLEEntity* attackedleader = this;
		if (EventIsSoldier()) {
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
			if (AdvHurtEntity_CheckOverHeal && attackedleader->Health > attackedleader->GetMaxHealth()) {
				int h = std::min(attackedleader->Health - attackedleader->GetMaxHealth(), damage);
				attackedleader->Hurt(h);
				damage -= h;
			}
			for (auto& a : attackedleader->ObservedEntities) {
				if (a.first == shok::AttachmentType::LEADER_SOLDIER)
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
					CppLogic::Events::AdvHurtEvent ev{ shok::EventIDs::CppLogicEvent_OnEntityKilled, attacker ? attacker->EntityId : 0, firsttodie->EntityId, damage, sourceInfo, attackerplayer };
					(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);

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
			CppLogic::Events::AdvHurtEvent ev{ shok::EventIDs::CppLogicEvent_OnEntityKilled, attacker ? attacker->EntityId : 0, firsttodie->EntityId, damage, sourceInfo, attackerplayer };
			(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
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
		GGL::CEventEntityIndex xpev{ shok::EventIDs::CppL_AffectedExperienceGained, attacker->EntityId, xptoadd };
		for (const auto& at : attacker->ObserverEntities) {
			if (at.first == shok::AttachmentType::HERO_AFFECTED) {
				EGL::CGLEEntity::GetEntityByID(at.second.EntityId)->FireEvent(&xpev);
			}
		}
		xptoadd = xpev.Index;
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
		L.GetGlobal();
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
			float dmg = curr->CalculateDamageAgainstMe(damage, damageclass, cr);

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
int __thiscall EGL::CGLEEntity::GetMaxHPOverride()
{
	float hp;
	auto* d = GetAdditionalData(false);
	GGlue::CGlueEntityProps* et = GetEntityType();
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
		auto player = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(PlayerId);
		if (dynamic_cast<GGL::CSettler*>(this)) {
			for (int t : static_cast<GGL::CGLSettlerProps*>(et->LogicProps)->ModifyHitpoints.TechList) {
				if (player->GetTechStatus(t) != shok::TechState::Researched)
					continue;
				shok::Technology* tech = (*GGL::CGLGameLogic::GlobalObj)->GetTech(t);
				hp = tech->HitpointModifier.ModifyValue(hp);
			}
		}
		else if (dynamic_cast<GGL::CBuilding*>(this)) {
			std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> it = EGL::CGLEEntity::BuildingMaxHpTechBoni.equal_range(EntityType);
			for (std::multimap<int, int>::iterator i = it.first; i != it.second; ++i) {
				int t = i->second;
				if (player->GetTechStatus(t) != shok::TechState::Researched)
					continue;
				shok::Technology* tech = (*GGL::CGLGameLogic::GlobalObj)->GetTech(t);
				hp = tech->HitpointModifier.ModifyValue(hp);
			}
		}
	}
	return static_cast<int>(hp);
}
void __thiscall EGL::CGLEEntity::OnCreateFixHP(const EGL::CGLEEntityCreator* c)
{
	if (c->Health != -1)
		return;
	if (dynamic_cast<GGL::CSettler*>(this)) {
		Health = GetMaxHealth();
	}
	else if (GGL::CBuilding* b = dynamic_cast<GGL::CBuilding*>(this)) {
		Health = GetMaxHealth();
		if (!b->IsConstructionFinished()) {
			Health = static_cast<int>(Health * (*GGL::CLogicProperties::GlobalObj)->ConstructionSiteHealthFactor);
		}
	}
}
void __declspec(naked) hookcreatentityfixhp() {
	__asm {
		mov ecx, esi;
		push edi; // param for OnCreateFixHP
		call[eax + 0x14]; // InitializeFromCreator

		mov ecx, esi; // get params
		call EGL::CGLEEntity::OnCreateFixHP;


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
		reinterpret_cast<void*>(0x571B93)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x57B798), CppLogic::Hooks::MemberFuncPointerToVoid(&GetMaxHPOverride, 0), reinterpret_cast<void*>(0x57B7AD));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x571B93), &hookcreatentityfixhp, reinterpret_cast<void*>(0x571B98));
	GGL::CGLGUIInterface::HookFillDataHealth();
}

shok::TaskExecutionResult(*EGL::CGLEEntity::LuaTaskListCallback)(EGL::CGLEEntity* e, int val) = nullptr;
int EGL::CGLEEntity::ExecuteLuaTask(EGL::CTaskArgsInteger* arg)
{
	shok::TaskExecutionResult i = shok::TaskExecutionResult::NextTask;
	int val = static_cast<EGL::CTaskArgsInteger*>(arg)->Value;
	if (EGL::CGLEEntity::LuaTaskListCallback)
		i = EGL::CGLEEntity::LuaTaskListCallback(this, val);
	if (i == shok::TaskExecutionResult::LuaTaskState) {
		GetAdditionalData(true)->FakeTaskValue = val;
		SetTaskState(shok::TaskState::LuaFunc);
		i = shok::TaskExecutionResult::StateChanged;
	}
	return static_cast<int>(i);
}
shok::TaskStateExecutionResult EGL::CGLEEntity::ExecuteLuaTaskState(int p)
{
	shok::TaskStateExecutionResult i = shok::TaskStateExecutionResult::Finished;
	if (EGL::CGLEEntity::LuaTaskListCallback)
		if (EGL::CGLEEntity::LuaTaskListCallback(this, GetAdditionalData(true)->FakeTaskValue) == shok::TaskExecutionResult::LuaTaskState)
			i = shok::TaskStateExecutionResult::NotFinished;
	return i;
}
void(__thiscall* const entityaddluatlhandlers)(EGL::CGLEEntity* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEntity*)>(0x57BA34);
void __thiscall EGL::CGLEEntity::AddHandlerLuaTask() {
	entityaddluatlhandlers(this);
	CreateTaskHandler<shok::Task::TASK_LUA_FUNC>(this, &EGL::CGLEEntity::ExecuteLuaTask);
	CreateStateHandler<shok::TaskState::LuaFunc>(this, &EGL::CGLEEntity::ExecuteLuaTaskState);
}
bool HookLuaTaskList_Hooked = false;
void EGL::CGLEEntity::HookLuaTaskList()
{
	if (HookLuaTaskList_Hooked)
		return;
	HookLuaTaskList_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57D6CA), 10 };
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x57D6CA), CppLogic::Hooks::MemberFuncPointerToVoid(&AddHandlerLuaTask, 0));
}

void(__thiscall* movementbeh_setmovetarget)(GGL::CBehaviorDefaultMovement* m, shok::Position* p) = reinterpret_cast<void(__thiscall*)(GGL::CBehaviorDefaultMovement*, shok::Position*)>(0x586894);
void __thiscall EGL::CMovingEntity::BuildOnSetPosFixed()
{
	if (MovementState != shok::MovementState::Moving && EGL::CGLEEntity::BuildOnSetPosFixMovement) {
		GGL::CBehaviorDefaultMovement* mov = GetBehavior<GGL::CBehaviorDefaultMovement>();
		if (mov) {
			movementbeh_setmovetarget(mov, &TargetPosition);
		}
	}
	else {
		BB::CEvent ev{ 0x2000D };
		FireEvent(&ev);
		if (MovementState == shok::MovementState::Moving) {
			ev.EventTypeId = 0x11017;
			FireEvent(&ev);
			MovementState = shok::MovementState::Moving; // ?? original code
		}
	}
}
void __declspec(naked) entity_buildonsetpos_asm() {
	__asm {
		mov ecx, edi;
		call EGL::CMovingEntity::BuildOnSetPosFixed;

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

int __stdcall EGL::CGLEEntity::CanCancelStateAdditionalCheck()
{
	if (Health <= 0)
		return 1;
	for (auto a : EGL::CGLEEntity::AdditionalCancelableStates)
		if (a == CurrentState)
			return 1;
	return 0;
}
void __declspec(naked) cancancelstate_check_asm() {
	__asm {
		push ecx;
		push ecx;
		call EGL::CGLEEntity::CanCancelStateAdditionalCheck;
		pop ecx;
		test eax, eax;
		jz cont;
		ret;

	cont:
		mov eax, [ecx + 0x88];
		push 0x57A689;
		ret;
	};
}
std::vector<shok::TaskState> EGL::CGLEEntity::AdditionalCancelableStates{};
bool HookCanCancelState_Hooked = false;
void EGL::CGLEEntity::HookCanCancelState()
{
	if (HookCanCancelState_Hooked)
		return;
	HookCanCancelState_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57A683) , 0x10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x57A683), &cancancelstate_check_asm, reinterpret_cast<void*>(0x57A689));
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


CppLogic::EntityAddon::EntityAddonData EGL::CGLEEntity::LastRemovedEntityAddonData{};

CppLogic::EntityAddon::EntityAddonData* EGL::CGLEEntity::GetAdditionalData(bool create)
{
	auto* a = GetBehavior<CppLogic::EntityAddon::EntityAddonData>();
	if (a == nullptr && create) {
		a = (*BB::CClassFactory::GlobalObj)->CreateObject<CppLogic::EntityAddon::EntityAddonData>();
		AddBehavior(a);
	}
	return a;
}
const CppLogic::EntityAddon::EntityAddonData* EGL::CGLEEntity::GetAdditionalData() const
{
	return GetBehavior<CppLogic::EntityAddon::EntityAddonData>();
}
void EGL::CGLEEntity::CloneAdditionalDataFrom(const CppLogic::EntityAddon::EntityAddonData& other)
{
	auto* t = GetAdditionalData(true);
	*t = other;
}


EGL::CEventGetValue_Int* __fastcall entitydamagemodeventautocannonasm(GGL::CAutoCannonBehavior* th, int, EGL::CEventGetValue_Int* ev) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	auto* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		ev->Data = d->DamageOverride;
	else
		ev->Data = e->GetEntityType()->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>()->DamageAmount;
	return ev;
}
bool HookDamageMod_Hooked = false;
void EGL::CGLEEntity::HookDamageMod()
{
	if (HookDamageMod_Hooked)
		return;
	HookDamageMod_Hooked = true;
	GGL::CBattleBehavior::HookDamageOverride();
	GGL::CAutoCannonBehavior::HookDamageOverride();
}

void __declspec(naked) entityarmormodsettlerasm() {
	__asm {
		mov esi, ecx;
		push[esi + 0x10];

		call GGL::CSettler::GetBaseArmor;

		push 0x4A6B25;
		ret;
	}
}
void __declspec(naked) entityarmormodbuildingrasm() {
	__asm {
		mov esi, ecx;
		push[esi + 0x10];

		call GGL::CBuilding::GetBaseArmor;
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

void __declspec(naked) entityexplmodsettasm() {
	__asm {
		push esi;
		push[esi + 0x10];

		call EGL::CGLEEntity::GetBaseExploration;

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
		call EGL::CGLEEntity::GetBaseExploration;
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
	auto* d = GetAdditionalData(false);
	int i;
	if (d && d->RegenHPOverride >= 0)
		i = d->RegenHPOverride;
	else
		i = GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->HealingPoints;
	return static_cast<int>(ModifierProfile.GetModifiedValue(EGL::IProfileModifierSetObserver::ModifierType::HealingPoints, static_cast<float>(i)));
}
int GGL::CSettler::LeaderGetRegenHealthSeconds()
{
	auto* d = GetAdditionalData(false);
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
static inline int(__thiscall* const settler_getdodge)(GGL::CSettler* th) = reinterpret_cast<int(__thiscall*)(GGL::CSettler*)>(0x4A4C1E);
int GGL::CSettler::GetDodgeChance()
{
	return settler_getdodge(this);
}

int __thiscall GGL::CSettler::GetBaseArmor()
{
	auto* d = GetAdditionalData(false);
	if (d && d->ArmorOverride >= 0)
		return d->ArmorOverride;
	GGlue::CGlueEntityProps* p = GetEntityType();
	if (const auto* l = dynamic_cast<GGL::CGLSettlerProps*>(p->LogicProps))
		return l->ArmorAmount;
	return 0;
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
		int healsel = std::min(r, mhp - hp);
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
		for (const auto& a : ObservedEntities) {
			if (a.first == shok::AttachmentType::LEADER_SOLDIER)
				numsol++;
		}
		smp *= numsol;
		shp += r;
		if (shp > smp)
			shp = smp;
		lb->TroopHealthCurrent = shp;
		return;
	}
}

void EGL::CGLEEntity::HookMaxRange()
{
	GGL::CBattleBehavior::HookRangeOverride();
	GGL::CAutoCannonBehavior::HookRangeOverride();
}

void EGL::CGLEEntity::HookDisplayName()
{
	GGUI::COnScreenElementType::HookDisplayName();
}

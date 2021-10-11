#pragma once
#include "s5data.h"
#include <map>


struct entityAddonData {
	int EntityId = 0;
	int HealthOverride = -1;
	bool HealthUseBoni = true;
	int DamageOverride = -1;
	int ArmorOverride = -1;
	float ExplorationOverride = -1.0f;
	int RegenHPOverride = -1;
	int RegenSecondsOverride = -1;
	float MaxRangeOverride = -1.0f;
	std::string NameOverride = "";
	int FakeTaskValue = 0;
};

struct shok_EGL_CGLETaskList;
struct shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int;
struct shok_EGL_IGLEStateHandler;
struct shok_entity_TaskIdAndTaskHandler {
	shok_Task TaskID;
	shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* TaskHandler;
};
struct shok_entity_StateIdAndStateHandler {
	shok_TaskState StateID;
	shok_EGL_IGLEStateHandler* StateHandler;
};

enum class AdvancedDealDamageSource : int {
	Unknown = 0,
	Melee = 1,
	Arrow = 2,
	Cannonball = 3,

	AbilitySnipe = 10,
	AbilityCircularAttack = 11,
	AbilityBomb = 12,
	AbilitySabotageSingleTarget = 13,
	AbilitySabotageBlast = 14,
	AbilityShuriken = 15,

	Script = 25,
};

struct shok_IEntityDisplay { // warning: you have to dymamic cast this! , entity offsets dont match
	static inline constexpr int TypeDesc = 0x810C10;
};

struct shok_EGL_CGLEEntity : shok_BB_IObject, shok_IEntityDisplay {
	PADDINGI(1);
	int EntityId;
	PADDINGI(1); // some type of flags
	int EntityType;
	int ModelOverride;
	int PlayerId;
	int attachmentvt; // 7
	shok_set<shok_attachment> ObserverEntities; // 8
	shok_set<shok_attachment> ObserverEffects; // 11
	shok_set<shok_attachment> ObservedEntities; // 14
	shok_set<shok_attachment> ObservedEffects; // 17
	byte SendEvent; // 20
	PADDING(3);
	int SetNewTaskListDepth;
	shok_positionRot Position; // 22
	float Scale; // 25
	PADDINGI(1);
	byte StandardBehaviorActive, TaskAdvanceNextTurnNextTask, ValidPosition, IsOnWater;
	byte UserSelectableFlag, UserControlableFlag, IsVisible, OnlySetTaskListWhenAlive;
	int TaskListToSet; // 29
	vector_padding;
	std::vector<shok_EGL_CGLEBehavior*, shok_allocator<shok_EGL_CGLEBehavior*>> Behaviours; // 30, first field in 31
	shok_TaskState CurrentState; // 34
	int EntityState, CurrentTaskListID, CurrentTaskIndex; // la37
	shok_set<shok_entity_StateIdAndStateHandler> StateHandlers; // 38
	shok_set<shok_entity_TaskIdAndTaskHandler> TaskHandlers; // 41
	PADDINGI(6); // la49 49 p to array EGL::ISlot of behaviors
	int Health; // 50
	char* ScriptName; // "Name" in loader
	char* ScriptCommandLine;
	float Exploration;
	int TaskListStart; // 54
	PADDINGI(2);
	byte InvulnerabilityFlag, Suspended; // 57
	PADDING(2);
	int SuspensionTurn;
	int ScriptingValue[4]; // 59
	int StateChangeCounter; // 63
	int TaskListChangeCounter;
	int NumberOfAuras; // 65

	static inline constexpr int vtp = 0x783E74;
	static inline constexpr int TypeDesc = 0x8077CC;
	static inline constexpr int vtp_IEntityDisplay = 0x783E58;
	static inline constexpr unsigned int Identifier = 0x3F2913E0;

	template<typename T, typename std::enable_if<std::is_base_of<shok_EGL_CGLEBehavior, T>::value>::type* = nullptr>
	T* GetBehavior() {
		for (shok_EGL_CGLEBehavior* b : Behaviours) {
			if (b) {
				T* r = shok_DynamicCast<shok_EGL_CGLEBehavior, T>(b);
				if (r)
					return r;
			}
		}
		return nullptr;
	}

	bool IsEntityInCategory(shok_EntityCategory cat);
	int GetResourceProvided();
	shok_GGlue_CGlueEntityProps* GetEntityType();

	void FireEvent(shok_BB_CEvent* ev);
	int EventGetDamage();
	int EventGetArmor();
	int EventGetMaxWorktime();
	shok_LeaderCommand EventLeaderGetCurrentCommand();
	float GetExploration();
	int GetMaxHealth();
	int LimitedAttachmentGetMaximum(shok_AttachmentType attachType);
	int GetSector();
	int ResourceTreeGetNearestSector();

	void SetHealth(int h);
	void Hurt(int dmg);
	void SetTaskList(int tl);
	void SetTaskList(shok_EGL_CGLETaskList* tl);
	shok_EGL_CGLETaskList* GetCurrentTaskList();
	shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* GetTaskHandler(shok_Task task);
	void ExecuteTask(shok_EGL_CGLETaskArgs& targ);

	void PerformHeal(int hp, bool healSoldiers);

	void Destroy();

	int GetFirstAttachedToMe(shok_AttachmentType attachmentId);
	int GetFirstAttachedEntity(shok_AttachmentType attachmentId);
	/**
	fires event eventIdOnThisDetach on otherId, if this gets detached/destroyed, fires eventIdOnOtherDetach on this, if otherId gets detached/destroyed
	**/
	void AttachEntity(shok_AttachmentType attachtype, int otherId, shok_EventIDs eventIdOnThisDetach, shok_EventIDs eventIdOnOtherDetach);
	void DetachObservedEntity(shok_AttachmentType attachtype, int otherId, bool fireEvent);

	void ClearAttackers();

	entityAddonData* GetAdditionalData(bool create);
	void CloneAdditionalDataFrom(entityAddonData* other);

	void AddTaskHandler(shok_Task task, void* obj, int(__fastcall* Handler)(void* obj, int _, shok_EGL_CGLETaskArgs* taskargs)); // _ is undefined
	void AddTaskStateHandler(shok_TaskState state, void* obj, int(__fastcall* Handler)(void* obj, int _, int onek)); // _ is undefined
	void AddEventHandler(shok_EventIDs ev, void* ob, void(__fastcall* Handler)(void* obj, int _, shok_BB_CEvent* ev)); // _ is undefined

	void AdvancedHurtEntityBy(shok_EGL_CGLEEntity* attacker, int damage, int attackerFallback, bool uiFeedback, bool xp, bool addStat, AdvancedDealDamageSource sourceInfo);
	static void __stdcall AdvancedDealAoEDamage(shok_EGL_CGLEEntity* attacker, const shok_position& center, float range, int damage, int player, int damageclass, bool uiFeedback, bool xp, bool addStat, AdvancedDealDamageSource sourceInfo);

	static void HookDamageMod();
	static void HookArmorMod();
	static void HookExplorationMod();
	static bool LeaderRegenRegenerateSoldiers;
	static void HookLeaderRegen();
	static void HookMaxRange();
	static void HookDisplayName();
	static void HookRangedEffectActivateHeal(bool hookActive);
	static std::multimap<int, int> BuildingMaxHpTechBoni; // ety->techid
	static bool UseMaxHPTechBoni;
	static void HookMaxHP();

	static int (*LuaTaskListCallback)(shok_EGL_CGLEEntity* e, int val); // 0 next task, 1 state changed, 2 tasklist changed, 3 lua task repeat
	static void HookLuaTaskList();
	static void HookNonCancelableAnim();
	static bool BuildOnSetPosFixMovement;
	static void HookBuildOnSetPos();
	static void HookSetTaskListNonCancelable(bool active);

	static void (*Hero6ConvertHookCb)(int id, int pl, int nid, int converter);
	static void HookHero6Convert();
	static void ActivateEntityChangePlayerFix();
	static int ResetCamoIgnoreIfNotEntity;
	static void HookResetCamo();
	static void (*CamoActivateCb)(shok_GGL_CCamouflageBehavior* th);
	static void HookCamoActivate();
	static void HookHurtEntity();
	static int* HurtEntityDamagePointer;
	static bool HurtEntityCallWithNoAttacker;
	static AdvancedDealDamageSource HurtEntityDamageSource;
	static int HurtEntityAttackerPlayer;
	static void (*HurtEntityOnKillCb)(shok_EGL_CGLEEntity* att, shok_EGL_CGLEEntity* kill, int attpl, AdvancedDealDamageSource sourc);
	static void HookDestroyEntity();
	static std::map<int, entityAddonData> AddonDataMap;
	static entityAddonData LastRemovedEntityAddonData;

	static inline shok_EGL_CGLEEntity* (__stdcall* const GetEntityByID)(int id) = reinterpret_cast<shok_EGL_CGLEEntity * (__stdcall*)(int)>(0x5825B4);
	static inline void(__cdecl* const EntityHurtEntity)(shok_EGL_CGLEEntity* attackerObj, shok_EGL_CGLEEntity* targetObj, int damage) = reinterpret_cast<void(__cdecl*)(shok_EGL_CGLEEntity*, shok_EGL_CGLEEntity*, int)>(0x49F358);
	static inline void(__cdecl* const EntityDealAoEDamage)(shok_EGL_CGLEEntity* attacker, shok_position* center, float range, int damage, int player, int damageclass) = reinterpret_cast<void(__cdecl*)(shok_EGL_CGLEEntity*, shok_position*, float, int, int, int)>(0x49F82A);
	static inline int(__cdecl* const GetEntityIDByScriptName)(const char* str) = reinterpret_cast<int(__cdecl*)(const char* str)>(0x576624);
	static inline int(__cdecl* const EntityIDGetProvidedResource)(int id) = reinterpret_cast<int(__cdecl*)(int id)>(0x4B8489);
	static inline bool(__cdecl* const EntityIDIsDead)(int id) = reinterpret_cast<bool(__cdecl*)(int)>(0x44B096);
	static inline int(__cdecl* const EntityIDChangePlayer)(int entityid, int player) = reinterpret_cast<int(__cdecl*)(int, int)>(0x49A6A7);

	static shok_EGL_CGLEEntity* ReplaceEntityWithResourceEntity(shok_EGL_CGLEEntity* e);

protected:
	int EventGetIntById(shok_EventIDs id);

	// defined tasks: TASK_RESET_TASK_LIST_TIMER, TASK_SET_MODEL, TASK_PLAY_SOUND, TASK_STOP_SOUND
	// defined states: SetNextTaskList
};
//constexpr int i = sizeof(shok_EGL_CGLEEntity) / 4;
struct shok_GGL_CBuilding;
struct shok_EGL_CMovingEntity : shok_EGL_CGLEEntity {
	shok_position TargetPosition; // la67
	byte TargetRotationValid;
	PADDING(3);
	float TargetRotation;
	int MovementState; // 70

	static inline constexpr int vtp = 0x783F84;
	static inline constexpr int TypeDesc = 0x8077EC;
	static inline constexpr unsigned int Identifier = 0x77D4E427;

	void AttackMove(const shok_position& p);
	void AttackEntity(int targetId);
	void HoldPosition();
	void Defend();
	void Move(const shok_position& p);
	void LeaderAttachSoldier(int soldierId);
	void SettlerExpell();
	void HeroAbilitySendHawk(shok_position& p);
	void HeroAbilityInflictFear();
	void HeroAbilityPlaceBomb(shok_position& p);
	void HeroAbilityPlaceCannon(shok_position& p, int FoundationType, int CannonType);
	void HeroAbilityRangedEffect();
	void HeroAbilityCircularAttack();
	void HeroAbilitySummon();
	void HeroAbilityConvert(int target);
	void HeroAbilityConvertBuilding(int target);
	void HeroAbilitySnipe(int tid);
	void HeroAbilityShuriken(int tid);
	void HeroAbilityMotivateWorkers();
	void HeroAbilityActivateCamoflage();
	void ThiefSabotage(int tid);
	void ThiefDefuse(int tid);
	void ThiefStealFrom(int tid);
	void ThiefSecureGoods(int tid);
	void ScoutBinoculars(shok_position& p);
	void ScoutFindResource();
	void ScoutPlaceTorch(shok_position& p);
	bool SerfConstructBuilding(shok_GGL_CBuilding* build);
	bool SerfRepairBuilding(shok_GGL_CBuilding* build);
	void SerfExtractResource(int id);
	void SerfTurnToBattleSerf();
	void BattleSerfTurnToSerf();
	void SetTargetRotation(float f);

	void SetPosition(shok_position& p);

	int LeaderGetNearbyBarracks();
	bool IsMoving();
};

struct shok_GGL_CEvadingEntity : shok_EGL_CMovingEntity {
	PADDINGI(4);

	static inline constexpr int vtp = 0x770A7C;
	static inline constexpr int TypeDesc = 0x810C34;
	static inline constexpr unsigned int Identifier = 0x42D23A53;
};

struct shok_GGL_CSettler : shok_GGL_CEvadingEntity {
	int TimeToWait, HeadIndex, HeadParams; // la77
	PADDINGI(7);
	float ExperiencePoints; // 85
	int ExperienceLevel;
	PADDINGI(2);
	float MovingSpeed; //89
	PADDINGI(1);
	float Damage;
	PADDINGI(1);
	float Dodge;
	PADDINGI(1);
	float Motivation;
	PADDINGI(1);
	float Armor;
	PADDINGI(1);
	float CurrentAmountSoldiers;
	PADDINGI(1);
	float MaxAmountSoldiers;
	PADDINGI(1);
	float DamageBonus;
	PADDINGI(1);
	float ExplorationCache;
	PADDINGI(1);
	float MaxAttackRange;
	PADDINGI(1);
	float AutoAttackRange;
	PADDINGI(1);
	float HealingPoints;
	PADDINGI(1);
	float MissChance;
	PADDINGI(1);
	float Healthbar; // 115
	PADDINGI(7);
	float SomeIntRegardingTaskLists; // 123
	PADDINGI(3);
	int LeaderId;
	PADDINGI(2);
	int OverheadWidget;
	int ExperienceClass;
	PADDINGI(2);
	int BlessBuff, NPCMarker, LeaveBuildingTurn; //la136

	static inline constexpr int vtp = 0x76E3CC;
	static inline constexpr int TypeDesc = 0x807858;
	static inline constexpr int vtp_IEntityDisplay = 0x76E3B0;
	static inline constexpr unsigned int Identifier = 0x0EDC11EA8;

	bool IsIdle();

	int LeaderGetRegenHealth();
	int LeaderGetRegenHealthSeconds();
	void KillSettlerByEnvironment();

	// defined tasks: TASK_VANISH, TASK_APPEAR, TASK_ENTER_BUILDING, TASK_LEAVE_BUILDING, TASK_LIST_DONE, TASK_SET_TASK_LIST, TASK_WAIT, TASK_LEFT_BUILDING, TASK_WAIT_UNTIL
	//		TASK_RESOLVE_COLLISION, TASK_GO_TO_FREE_POSITION, TASK_SET_POS (set relative to workplace)
	// defined events: GetArmorClass, GetArmor, Settler_XXX, Worker_Bless (set BlessBuff to 10), IsConvertible
	// defined states: BattleWaitUntilAutoCannon, Default (set tl to default)
};

struct shok_GGL_CAnimal : shok_EGL_CMovingEntity {
	shok_position TerritoryCenter; // 71
	float TerritoryRadius; // int?
	shok_position DangerPosition;

	static inline constexpr int vtp = 0x778F7C;
	static inline constexpr int TypeDesc = 0x812038;
	static inline constexpr unsigned int Identifier = 0x5D6B737;
};

struct shok_GGL_CResourceDoodad : shok_EGL_CGLEEntity {
	shok_ResourceType ResourceType;
	int ResourceAmount, ResourceAmountAdd; //66

	// set current res amount 4B864B thiscall (th, int)
	void SetCurrentResourceAmount(int am);

	static inline constexpr int vtp = 0x76FEA4;
	static inline constexpr int TypeDesc = 0x8118AC;
	static inline constexpr int vtp_IEntityDisplay = 0x76FE88;
	static inline constexpr unsigned int Identifier = 0xB0778F47;
};

struct shok_GGL_CBuilding : shok_EGL_CGLEEntity {
	shok_position ApproachPosition, LeavePosition; // fi 66
	byte IsActive, IsRegistered, IsUpgrading, IsOvertimeActive;
	byte HQAlarmActive;
	PADDING(3);
	int MaxNumWorkers, CurrentTechnology, LatestAttackTurn, MostRecentDepartureTurn; // 72
	float BuildingHeight, Helathbar, UpgradeProgress; //la78
	PADDINGI(2);// list Slots with NumberOfRepairingSerfs?
	int NumberOfRepairingSerfs;
	int OvertimeCooldown;
	int ConstructionSiteType; // 83

	static inline constexpr int vtp = 0x76EB94;
	static inline constexpr int TypeDesc = 0x807898;
	static inline constexpr int vtp_IEntityDisplay = 0x76EB78;
	static inline constexpr unsigned int Identifier = 0x15EBDB60;

	int GetConstructionSite();
	int GetNearestFreeConstructionSlotFor(shok_position* p);
	int GetNearestFreeRepairSlotFor(shok_position* p);
	bool IsConstructionFinished();
	bool IsIdle();
	bool IsIdle(bool forRecruitemnt);
	int GetTechnologyInResearch();
	int GetCannonProgress();
	float GetMarketProgress();

	void StartUpgrade();
	void CancelUpgrade();
	void CommandBuildCannon(int entitytype);
	void CommandRecruitSoldierForLeader(int id);
	void ActivateOvertime();
	void DeactivateOvertime();
	void BarracksRecruitGroups();
	void BarracksRecruitLeaders();
	void HQBuySerf();
	void SellBuilding();
	void StartResearch(int tech);
	void CancelResearch();
	void MarketStartTrade(shok_ResourceType ResourceTypeSell, shok_ResourceType ResourceTypeBuy, float BuyAmount);
	void MarketCancelTrade();
	int BuyLeaderByType(int ety);

	// defined events: IsConvertible, GetArmorClass, GetArmor, OnAttackedBy, WorkerAlarmMode_Enable, WorkerAlarmMode_Disable
};

struct shok_GGL_CBridgeEntity : shok_GGL_CBuilding {
	static inline constexpr int vtp = 0x77805C;
	static inline constexpr int TypeDesc = 0x812054;
	static inline constexpr int vtp_IEntityDisplay = 0x778040;
	static inline constexpr unsigned int Identifier = 0x3736FF8E;
};

struct shok_EGL_CAmbientSoundEntity : shok_EGL_CGLEEntity {
	PADDINGI(1);
	int AmbientSoundType;

	static inline constexpr int vtp = 0x78568C;
	static inline constexpr int TypeDesc = 0x839FF0;
	static inline constexpr unsigned int Identifier = 0xE12A3723;
};

struct shok_EGL_CGLEEntityCreator : shok_BB_IObject {
	int EntityType = 0;
	shok_positionRot Pos = { 0,0,0 };
	int Flags = 0; // 5
	int PlayerId = 0;
	int Health = 0; // 7
	char* ScriptName = nullptr; // use shok_malloc, gets freed by destructor
	char* ScriptCommandLine = nullptr; // use shok_malloc, gets freed by destructor
	int AmbientSoundType = 0;
	float Scale = 0; // 11

	static inline constexpr int vtp = 0x766B50;
	static inline constexpr int TypeDesc = 0x807874;

	shok_EGL_CGLEEntityCreator();
	~shok_EGL_CGLEEntityCreator();
protected:
	shok_EGL_CGLEEntityCreator(int _);
};
struct shok_GGL_CGLConstructionSiteCreator : shok_EGL_CGLEEntityCreator {
	int BuildingType = 0; // set EntityType to construction site type (from building props) and BuildingType to the building youactually want to construct

	static inline constexpr int vtp = 0x770114;
	static inline constexpr int TypeDesc = 0x811A80;

	shok_GGL_CGLConstructionSiteCreator();
};
struct shok_GGL_CResourceDoodadCreator : shok_EGL_CGLEEntityCreator {
	int ResourceAmount = 0;

	static inline constexpr int vtp = 0x774898;
	static inline constexpr int TypeDesc = 0x811974;

	shok_GGL_CResourceDoodadCreator();
};

#pragma once
#include <map>

#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_tech.h"
#include "s5_behaviors.h"
#include "s5_config.h"

namespace shok {
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
	enum class TaskExecutionResult : int {
		NextTask = 0,
		StateChanged = 1,
		TaskListChanged = 2,
		LuaTaskState = 3,
	};
	enum class TaskStateExecutionResult : int {
		NotFinished = -1,
		Finished = -2,
	};
}


namespace EGL {
	class IEntityDisplay {
	public:

		struct modeldata {
			int EntityID = 0;
			int EntityType = 0;
			int ModelOverride = 0;
			shok::Position Pos{};
		};
		struct playermodeldata {
			int EntityType = 0;
			int EntityID = 0;
			int Player = 0;
			int ModelOverride = 0;
		};
		struct posdata {
			shok::PositionRot Pos{};
			float Scale = 0;
			int NumAuras = 0;
		};

		virtual ~IEntityDisplay() = default;
		virtual ISlot* GetSlotByIndex(int idx) = 0;
	protected:
		virtual void GetModelData(modeldata* out) const = 0;
		virtual void GetPlayerModelData(playermodeldata* o) const = 0;
		virtual void GetPosData(posdata* o) const = 0;
	public:


		static inline constexpr int TypeDesc = 0x810C10;
		static inline constexpr int vtp = 0x783CFC;

		modeldata GetModelData() const;
		playermodeldata GetPlayerModelData() const;
		posdata GetPosData() const;

		int EntityId; // 2 in entity
		PADDINGI(1); // some type of flags
		int EntityType;
		int ModelOverride;
		int PlayerId;
	};


	class CGLEEntity : public BB::IObject, public IEntityDisplay, public EGL::TGLEAttachable<EGL::CGLEEntity, EGL::CEntityAttachmentProxy> {
	public:
		int SetNewTaskListDepth;
		shok::PositionRot Position; // 22
		float Scale; // 25
		PADDINGI(1);
		bool StandardBehaviorActive, TaskAdvanceNextTurnNextTask, ValidPosition, IsOnWater;
		bool UserSelectableFlag, UserControlableFlag, IsVisible, OnlySetTaskListWhenAlive;
		int TaskListToSet; // 29
		shok::Vector<EGL::CGLEBehavior*> Behaviours; // 30, first field in 31
		shok::TaskState CurrentState; // 34
		int EntityState, CurrentTaskListID, CurrentTaskIndex; // la37
		shok::Map<shok::TaskState, EGL::IGLEStateHandler*> StateHandlers; // 38
		shok::Map<shok::Task, EGL::TaskHandler*> TaskHandlers; // 41
		PADDINGI(1);
		shok::Map<shok::EventIDs, EGL::EventHandler*> EventHandlers; //45
		PADDINGI(1);
		EGL::ISlot* Slots; // same size as behavioProps vector of type, initialized on create
		int Health; // 50
		char* ScriptName; // "Name" in loader
		char* ScriptCommandLine;
		float Exploration;
		int TaskListStart; // 54
		PADDINGI(2);
		bool InvulnerabilityFlag, Suspended; // 57
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

	private:
		virtual void OnDestroy(int i) = 0;
		virtual void DestroyVT(int i) = 0;
		virtual void InitializeFromCreator(const EGL::CGLEEntityCreator* cr) = 0; // 5
		virtual void CopyToCreator(EGL::CGLEEntityCreator* cr) const = 0;
		virtual void OnCreated() = 0;
		virtual void OnLoaded() = 0;
		virtual void FireEntityCreatedTrigger() = 0;
	public:
		virtual void SetPosition(shok::Position* p) = 0; // 10 works for settlers, check if it does for other stuff to, called every tick
		virtual void SetRotation(float r) = 0;
		virtual int GetSector() = 0;
		virtual bool IsInSector(int sec) = 0;
	private:
		virtual void Tick() = 0;
	public:
		virtual shok::TaskExecutionResult ExecuteTask(EGL::CGLETaskArgs* t) = 0;
		virtual void FireEvent(BB::CEvent* ev) = 0;
		virtual void AddBehavior(EGL::CGLEBehavior* b) = 0; // 17 probably not usable outside of createentity
		virtual void AddSlot(EGL::ISlot* slot, int i) = 0;
		virtual void AddTaskHandler(shok::Task t, EGL::TaskHandler* handl) = 0; //19
		virtual void AddEventHandler(shok::EventIDs ev, EGL::EventHandler* handl) = 0; //20
		virtual void AddStateHandler(shok::TaskState st, EGL::IGLEStateHandler* handl) = 0;
		virtual void GetApproachPos(shok::Position* p) = 0;
		virtual float GetApproachRot() = 0;
		virtual void SetTaskState(shok::TaskState st) = 0; // 24
	private:
		virtual bool UnknownEntityFunc2() = 0;
		virtual void UnknownEntityFunc3() = 0;
		virtual void UnknownEntityFunc4() = 0;
	public:
		virtual float GetExploration() = 0; // 28
	private:
		virtual float GetUserSelectable() = 0;
		virtual float GetUserControl() = 0; // 30
		virtual void OnUpdateBlocking() = 0;
		virtual void UnknownEntityFunc5() = 0;
		virtual void ApplyBlocking() = 0;
	public:
		virtual bool CanCancelCurrentState() = 0;
	private:
		virtual void UnknownEntityFunc6() = 0;
		virtual void UnknownEntityFunc7() = 0;
		virtual void OnEntityUpgrade(CGLEEntity* old) = 0;
		virtual void UnknownEntityFunc8(int) = 0; // 38

	public:
		template<typename T>
		requires std::derived_from<T, EGL::CGLEBehavior>
		T* GetBehavior() {
			for (EGL::CGLEBehavior* b : Behaviours) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
		template<typename T>
		requires std::derived_from<T, EGL::CGLEBehavior>
		const T* GetBehavior() const {
			for (const EGL::CGLEBehavior* b : Behaviours) {
				if (b) {
					const T* r = dynamic_cast<const T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}

		bool IsEntityInCategory(shok::EntityCategory cat) const;
		shok::ResourceType GetResourceProvided() const;
		GGlue::CGlueEntityProps* GetEntityType() const;

		int EventGetDamage();
		int EventGetArmor();
		int EventGetMaxWorktime();
		shok::LeaderCommand EventLeaderGetCurrentCommand();
		int GetMaxHealth();
		int LimitedAttachmentGetMaximum(shok::AttachmentType attachType);
		int ResourceTreeGetNearestSector() const;
		bool EventIsInvisible(); // GGL::CCamouflageBehavior
		bool EventIsSettlerOrBuilding();
		bool EventIsWorker();
		bool EventIsBattleOrAutocannon();
		bool EventIsSerfOrWorker();
		bool EventIsSoldier();

		void SetHealth(int h);
		void Hurt(int dmg);
		void SetTaskList(int tl);
		void SetTaskList(EGL::CGLETaskList* tl);
		EGL::CGLETaskList* GetCurrentTaskList();
		EGL::TaskHandler* GetTaskHandler(shok::Task task);
		void ExecuteTask(EGL::CGLETaskArgs& targ);

		void PerformHeal(int hp, bool healSoldiers);
		bool CheckDodge(); // rolls random, returns if dodge successful
		bool IsInBlocking() const;
		bool IsDead() const;
		shok::AccessCategory GetAccessCategory() const;
		float __thiscall GetBaseExploration();

		void Destroy();

		int GetFirstAttachedToMe(shok::AttachmentType attachmentId) const;
		int GetFirstAttachedEntity(shok::AttachmentType attachmentId) const;
		/**
		fires event eventIdOnThisDetach on otherId, if this gets detached/destroyed, fires eventIdOnOtherDetach on this, if otherId gets detached/destroyed
		**/
		void AttachEntity(shok::AttachmentType attachtype, int otherId, shok::EventIDs eventIdOnThisDetach, shok::EventIDs eventIdOnOtherDetach);
		void DetachObservedEntity(shok::AttachmentType attachtype, int otherId, bool fireEvent);

		void ClearAttackers();

		CppLogic::EntityAddon::EntityAddonData* GetAdditionalData(bool create = false);
		const CppLogic::EntityAddon::EntityAddonData* GetAdditionalData() const;
		void CloneAdditionalDataFrom(const CppLogic::EntityAddon::EntityAddonData& other);

		static bool AdvHurtEntity_CheckOverHeal;
		float CalculateDamageAgainstMe(int damage, int damageclass, float aoeFactor = 1.0f);
		void AdvancedHurtEntityBy(EGL::CGLEEntity* attacker, int damage, int attackerFallback, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo);
		static void __stdcall AdvancedDealAoEDamage(EGL::CGLEEntity* attacker, const shok::Position& center, float range, int damage, int player, int damageclass, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo);
		CGLEEntity* AdvChangePlayer(int player); // destroys entity, returns new one

		template<shok::EventIDs id, class EventType, class ObjectType>
		void CreateEventHandler(ObjectType* ob, void (ObjectType::* handler)(EventType*)) {
			using hty = EGL::THandler<static_cast<int>(id), BB::CEvent, EventType, ObjectType, void>;
			AddEventHandler(id, new hty(ob, handler));
		}

		// task handlers return value does actually get ignored. instread it gets reconstructed via the changed counters.
		template<shok::Task id, class TaskArgs, class ObjectType>
		void CreateTaskHandler(ObjectType* ob, int (ObjectType::* handler)(TaskArgs*)) {
			using hty = EGL::THandler<static_cast<int>(id), EGL::CGLETaskArgs, TaskArgs, ObjectType, int>;
			AddTaskHandler(id, new hty(ob, handler));
		}

		template<shok::TaskState id, class ObjectType>
		void CreateStateHandler(ObjectType* ob, shok::TaskStateExecutionResult(ObjectType::* handler)(int)) {
			using hty = EGL::TStateHandler<ObjectType>;
			AddStateHandler(id, new hty(ob, handler));
		}


		static void HookDamageMod();
		static void HookArmorMod();
		static void HookExplorationMod();
		static void HookMaxRange();
		static void HookDisplayName();
		static std::multimap<int, int> BuildingMaxHpTechBoni; // ety->techid
		static bool UseMaxHPTechBoni;
		static void HookMaxHP();

	private:
		int ExecuteLuaTask(EGL::CTaskArgsInteger* arg);
		shok::TaskStateExecutionResult ExecuteLuaTaskState(int i);
		void __thiscall AddHandlerLuaTask();
		int __stdcall CanCancelStateAdditionalCheck();
		int __thiscall GetMaxHPOverride();
		void __thiscall OnCreateFixHP(const EGL::CGLEEntityCreator* c);

	public:
		static shok::TaskExecutionResult(*LuaTaskListCallback)(EGL::CGLEEntity* e, int val); // 0 next task, 1 state changed, 2 tasklist changed, 3 lua task repeat
		static void HookLuaTaskList();
		static bool BuildOnSetPosFixMovement;
		static void HookBuildOnSetPos();
		static void HookSetTaskListNonCancelable(bool active);
		static std::vector<shok::TaskState> AdditionalCancelableStates;
		static void HookCanCancelState();

		static void ActivateEntityChangePlayerFix();
	private:
		static int __cdecl FixedChangePlayer(int id, int pl);
		static void __cdecl FixedHurtEntity(EGL::CGLEEntity* att, EGL::CGLEEntity* tar, int dmg);
		static void __cdecl FixedHurtEntityAoE(EGL::CGLEEntity* att, shok::Position* p, float r, int dmg, int pl, int dmgcl);
	public:
		static void HookHurtEntity();
		static bool HurtEntityCallWithNoAttacker;
		static CppLogic::EntityAddon::EntityAddonData LastRemovedEntityAddonData;

		static inline EGL::CGLEEntity* (__stdcall* const GetEntityByID)(int id) = reinterpret_cast<EGL::CGLEEntity * (__stdcall*)(int)>(0x5825B4);
		static inline void(__cdecl* const EntityHurtEntity)(EGL::CGLEEntity* attackerObj, EGL::CGLEEntity* targetObj, int damage) = reinterpret_cast<void(__cdecl*)(EGL::CGLEEntity*, EGL::CGLEEntity*, int)>(0x49F358);
		static inline void(__cdecl* const EntityDealAoEDamage)(EGL::CGLEEntity* attacker, shok::Position* center, float range, int damage, int player, int damageclass) = reinterpret_cast<void(__cdecl*)(EGL::CGLEEntity*, shok::Position*, float, int, int, int)>(0x49F82A);
		static inline int(__cdecl* const GetEntityIDByScriptName)(const char* str) = reinterpret_cast<int(__cdecl*)(const char* str)>(0x576624);
		static inline shok::ResourceType(__cdecl* const EntityIDGetProvidedResource)(int id) = reinterpret_cast<shok::ResourceType(__cdecl*)(int id)>(0x4B8489);
		static inline bool(__cdecl* const EntityIDIsDead)(int id) = reinterpret_cast<bool(__cdecl*)(int)>(0x44B096);
		static inline int(__cdecl* const EntityIDChangePlayer)(int entityid, int player) = reinterpret_cast<int(__cdecl*)(int, int)>(0x49A6A7);

		static EGL::CGLEEntity* ReplaceEntityWithResourceEntity(EGL::CGLEEntity* e);

	protected:
		int EventGetIntById(shok::EventIDs id);

		// defined tasks: TASK_RESET_TASK_LIST_TIMER, TASK_SET_MODEL, TASK_PLAY_SOUND, TASK_STOP_SOUND
		// defined states: SetNextTaskList
	};
#ifdef __INTELLISENSE__
#pragma diag_suppress 2784
#endif
	static_assert(offsetof(CGLEEntity, EntityId) == 2 * 4);
	static_assert(offsetof(CGLEEntity, ObserverEntities) == 8 * 4);
#ifdef __INTELLISENSE__
#pragma diag_default 2784
#endif
	static_assert(offsetof(CGLEEntity, Position) == 22 * 4);
	static_assert(offsetof(CGLEEntity, NumberOfAuras) == 65 * 4);
	static_assert(sizeof(CGLEEntity) == 66 * 4);

	class CMovingEntity : public EGL::CGLEEntity {
		friend class EGL::CGLEEntity;
		virtual void KillThenSimpleFeedbackEvent4() = 0; // 39
	public:
		shok::Position TargetPosition; // la67
		byte TargetRotationValid;
		PADDING(3);
		float TargetRotation;
		int MovementState; // 70 0 when staning, 2 when moving

		static inline constexpr int vtp = 0x783F84;
		static inline constexpr int TypeDesc = 0x8077EC;
		static inline constexpr unsigned int Identifier = 0x77D4E427;

		void AttackMove(const shok::Position& p);
		void AttackEntity(int targetId);
		void HoldPosition();
		void Defend();
		void Move(const shok::Position& p);
		void LeaderAttachSoldier(int soldierId);
		void SettlerExpell();
		void HeroAbilitySendHawk(shok::Position& p);
		void HeroAbilityInflictFear();
		void HeroAbilityPlaceBomb(shok::Position& p);
		void HeroAbilityPlaceCannon(shok::Position& p, int FoundationType, int CannonType);
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
		void ScoutBinoculars(shok::Position& p);
		void ScoutFindResource();
		void ScoutPlaceTorch(shok::Position& p);
		bool SerfConstructBuilding(GGL::CBuilding* build);
		bool SerfRepairBuilding(GGL::CBuilding* build);
		void SerfExtractResource(int id);
		void SerfTurnToBattleSerf();
		void BattleSerfTurnToSerf();
		void SetTargetRotation(float f);

		void SetPosition(shok::Position& p);

		int LeaderGetNearbyBarracks();
		bool IsMoving();
		bool IsFleeingFrom(const shok::Position& center, float range) const;

	private:
		void __thiscall BuildOnSetPosFixed();
	};
	static_assert(sizeof(CMovingEntity) == 71 * 4);
	static_assert(offsetof(CMovingEntity, MovementState) == 70 * 4);

	class IEntitySound {
	public:
		virtual ~IEntitySound() = default;
	private:
		virtual void UnknownFuncIEntitySound1(int, int) = 0;

	};

	class CAmbientSoundEntity : public EGL::CGLEEntity, public IEntitySound {
	public:
		int AmbientSoundType;

		static inline constexpr int vtp = 0x78568C;
		static inline constexpr int TypeDesc = 0x839FF0;
		static inline constexpr unsigned int Identifier = 0xE12A3723;
	};
	static_assert(offsetof(CAmbientSoundEntity, AmbientSoundType) == 67 * 4);

	class IProfileModifierSetObserver {
		// no vtable
	public:
		enum class ModifierType : int {
			MovingSpeed = 0, // gets called, modified by tech, experience and weather
			HealthBar = 100, // no modification, straight from entity
			MaxHealth = 101, // no modification, straigt from GetMaxHealth func
			Exploration = 102, // gets called, modified by tech and experience (returns constant for dead hero)
			Damage = 103, // gets called, also shuriken, circularattack, modified by tech and experience
			DamageBonus = 104, // gets called, modified by tech and experience
			MaxAttackRange = 105, // gets called, battle+autocannon?, modified by tech and experience
			MinAttackRange = 106, // modified by tech
			Experience = 107, // no modifier, straight to event Leader_GetXP
			ExperienceLevels = 108, // no modifier, straigt calculaton from Experience
			// 109 unknown just passes through initial, called max soldiers?
			Armor = 110, // gets called, modified by technology
			Motivation = 111, // no modifier, straight to event Worker_GetMotivation (if not dead)
			CurrentSoldierAmount = 112, // no modifier, straight to event Leader_GetCurrentNumSoldier
			MaxSoldierAmount = 113, // no modifier, straight to event Leader_GetMaxNumSoldier
			Dodge = 114, // gets called, modified by tech and experience
			AutoAttackRange = 115, // gets called, modified by experience
			HealingPoints = 116, // gets called, modified by experience
			MissChance = 117, // gets called, modified by experience

			// note: buindings do not have a IProfileModifierSetObserver, but they have a tech modifier call on the virtual GetExploration and the GetArmor event
		};

		virtual float GetModifiedValue(ModifierType t, float z);
	};
}

namespace GGL {
	class CEntityProfile : public EGL::IProfileModifierSetObserver {
	public:
		struct ModifyableValue {
			bool NeedsRefresh;
			PADDING(3);
			float Value;
		};

		PADDINGI(4); // vector?
		int EntityId;
		ModifyableValue ExperiencePoints; // 6, 84 in settler
		ModifyableValue ExperienceLevel;
		ModifyableValue MovingSpeed; // 10
		ModifyableValue Damage; // 12
		ModifyableValue Dodge; // 14
		ModifyableValue Motivation; //16
		ModifyableValue Armor; // 18
		ModifyableValue CurrentAmountSoldiers; // 20
		ModifyableValue MaxAmountSoldiers; //22
		ModifyableValue DamageBonus; // 24
		ModifyableValue Exploration; // 26
		ModifyableValue MaxAttackRange; // 28
		ModifyableValue AutoAttackRange; // 30
		ModifyableValue HealingPoints; // 32
		ModifyableValue MissChance; // 34, 112 in settler
		PADDINGI(1); // 0
		float HealthBar; //37
		float HealthMax; // 38
		PADDINGI(1); // 39
		PADDINGI(7); // 40 string?
		struct EntityRef {
			friend class CEntityProfile;
			EGL::CGLEEntity* Self; // 125 in settler
			EGL::CGLEEntity* Leader; // self if it is a leader
			int LeaderId;
			GGL::CSettler* SelfSett;
			bool NeedsInitialization;
			PADDING(3);
			int OverheadWidget; // 5, 130 in settler, soldier=4, leader=3, worker=2, serf=1
			shok::ExperienceClass ExperienceClass; // 6 gets set by checking entitycategories
			int EntityId; // 7
			GGL::CGLSettlerProps* Props;

			void CheckInit();
		private:
			void AssignAdvExperienceClass();
		} EntityReference;

		const GGL::ExperienceClass::LevelData* GetExperienceClassLevel();


		static inline constexpr int vtp = 0x7727A4;

		static void HookExperience(bool active);
		static void HookExperienceClassAssignment(bool active);
	};
	static_assert(sizeof(CEntityProfile) == 56 * 4);
	struct ModifierEntityDatabase {
		PADDINGI(3); // map?

		static inline ModifierEntityDatabase* const GlobalObj = reinterpret_cast<ModifierEntityDatabase*>(0x895E5C);

		float GetModifiedStat(int id, CEntityProfile::ModifierType ty, float initial);
	};

	class CEvadingEntity : public EGL::CMovingEntity {
	public:
		PADDINGI(4);

		static inline constexpr int vtp = 0x770A7C;
		static inline constexpr int TypeDesc = 0x810C34;
		static inline constexpr unsigned int Identifier = 0x42D23A53;
	};
	static_assert(sizeof(CEvadingEntity) == 75 * 4);

	class CSettler : public GGL::CEvadingEntity {
	public:
		int TimeToWait, HeadIndex, HeadParams; // la77

		CEntityProfile ModifierProfile;

		int BlessBuff, NPCMarker, LeaveBuildingTurn; //la136

		static inline constexpr int vtp = 0x76E3CC;
		static inline constexpr int TypeDesc = 0x807858;
		static inline constexpr int vtp_IEntityDisplay = 0x76E3B0;
		static inline constexpr unsigned int Identifier = 0x0EDC11EA8;

		bool IsIdle();

		int LeaderGetRegenHealth();
		int LeaderGetRegenHealthSeconds();
		void KillSettlerByEnvironment();
		int GetDodgeChance(); // used in melee combat, percent
		int __thiscall GetBaseArmor();

		void Upgrade();

		// defined tasks: TASK_VANISH, TASK_APPEAR, TASK_ENTER_BUILDING, TASK_LEAVE_BUILDING, TASK_LIST_DONE, TASK_SET_TASK_LIST, TASK_WAIT, TASK_LEFT_BUILDING, TASK_WAIT_UNTIL
		//		TASK_RESOLVE_COLLISION, TASK_GO_TO_FREE_POSITION, TASK_SET_POS (set relative to workplace)
		// defined events: GetArmorClass, GetArmor, Settler_XXX, Worker_Bless (set BlessBuff to 10), IsConvertible
		// defined states: BattleWaitUntilAutoCannon, Default (set tl to default)
	};
	static_assert(sizeof(CSettler) == 137 * 4);
	static_assert(offsetof(CSettler, ModifierProfile.EntityReference.OverheadWidget) == 130 * 4);
	static_assert(offsetof(CSettler, ModifierProfile.EntityReference.Self) == 125 * 4);
	//constexpr int i = offsetof(CSettler, ModifierProfile.OverheadWidget) / 4;

	class CAnimal : public EGL::CMovingEntity {
	public:
		shok::Position TerritoryCenter; // 71
		float TerritoryRadius; // int?
		shok::Position DangerPosition;

		static inline constexpr int vtp = 0x778F7C;
		static inline constexpr int TypeDesc = 0x812038;
		static inline constexpr unsigned int Identifier = 0x5D6B737;
	};

	class CResourceDoodad : public EGL::CGLEEntity {
	public:
		shok::ResourceType ResourceType; //66
		int ResourceAmount, ResourceAmountAdd;

		// sets current and refreshes model depending on max
		void SetCurrentResourceAmount(int am);

		static inline constexpr int vtp = 0x76FEA4;
		static inline constexpr int TypeDesc = 0x8118AC;
		static inline constexpr int vtp_IEntityDisplay = 0x76FE88;
		static inline constexpr unsigned int Identifier = 0xB0778F47;
	};

	class CBuilding : public EGL::CGLEEntity {
	public:
		shok::Position ApproachPosition, LeavePosition; // fi 66
		bool IsActive, IsRegistered, IsUpgrading, IsOvertimeActive;
		bool HQAlarmActive;
		PADDING(3);
		int MaxNumWorkers, CurrentTechnology, LatestAttackTurn, MostRecentDepartureTurn; // 72
		float BuildingHeight, Healthbar, UpgradeProgress; //la78
		PADDINGI(2);// list Slots with NumberOfRepairingSerfs?
		int NumberOfRepairingSerfs;
		int OvertimeCooldown;
		int ConstructionSiteType; // 83

		// a lot more v funcs

		static inline constexpr int vtp = 0x76EB94;
		static inline constexpr int TypeDesc = 0x807898;
		static inline constexpr int vtp_IEntityDisplay = 0x76EB78;
		static inline constexpr unsigned int Identifier = 0x15EBDB60;

		int GetConstructionSite() const;
		int GetNearestFreeConstructionSlotFor(shok::Position* p);
		int GetNearestFreeRepairSlotFor(shok::Position* p);
		bool IsConstructionFinished() const;
		bool IsIdle();
		bool IsIdle(bool forRecruitemnt);
		int GetTechnologyInResearch();
		int GetCannonProgress();
		float GetMarketProgress();
		// returns max time if not currently upgrading
		float GetRemainingUpgradeTime();
		int __thiscall GetBaseArmor();

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
		void MarketStartTrade(shok::ResourceType ResourceTypeSell, shok::ResourceType ResourceTypeBuy, float BuyAmount);
		void MarketCancelTrade();
		int BuyLeaderByType(int ety);
		shok::Position GetAbsoluteApproachPos();

		// defined events: IsConvertible, GetArmorClass, GetArmor, OnAttackedBy, WorkerAlarmMode_Enable, WorkerAlarmMode_Disable


		static std::vector<shok::AdditionalTechModifier> ConstructionSpeedModifiers;
		static void EnableConstructionSpeedTechs();
	};

	class CBridgeEntity : public GGL::CBuilding {
		friend class EGL::CGLETerrainLowRes;
	public:
		static inline constexpr int vtp = 0x77805C;
		static inline constexpr int TypeDesc = 0x812054;
		static inline constexpr int vtp_IEntityDisplay = 0x778040;
		static inline constexpr unsigned int Identifier = 0x3736FF8E;

	private:
		void __thiscall ApplyHeightOverride();
	};

}

namespace EGL {
	class CGLEEntityCreator : public BB::IObject {
	public:
		int EntityType = 0;
		shok::PositionRot Pos = { 0,0,0 };
		int Flags = 0; // 5
		int PlayerId = 0;
		int Health = 0; // 7
		char* ScriptName = nullptr; // use shok::Malloc, gets freed by destructor
		char* ScriptCommandLine = nullptr; // use shok::Malloc, gets freed by destructor
		int AmbientSoundType = 0;
		float Scale = 0; // 11

		static inline constexpr int vtp = 0x766B50;
		static inline constexpr int TypeDesc = 0x807874;

		// i have to implement them so the compiler stops complaining.
		// i call the original constructor and replace the vtable with that, so they really dont matter.
		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(unsigned int id) override;
		virtual ~CGLEEntityCreator() override;

		CGLEEntityCreator();
	protected:
		CGLEEntityCreator(int _);
	};
}

namespace GGL {
	class CGLConstructionSiteCreator : public EGL::CGLEEntityCreator {
	public:
		int BuildingType = 0; // set EntityType to construction site type (from building props) and BuildingType to the building youactually want to construct

		static inline constexpr int vtp = 0x770114;
		static inline constexpr int TypeDesc = 0x811A80;

		CGLConstructionSiteCreator();
	};
	class CResourceDoodadCreator : public EGL::CGLEEntityCreator {
	public:
		int ResourceAmount = 0;

		static inline constexpr int vtp = 0x774898;
		static inline constexpr int TypeDesc = 0x811974;

		CResourceDoodadCreator();
	};
}

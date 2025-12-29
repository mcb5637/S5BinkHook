#pragma once
#include <map>

#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_baseprops.h>
#include <shok/tech/s5_tech.h>
#include <shok/entity/s5_behaviors.h>
#include <shok/globals/s5_config.h>
#include <utility/EnumIdManagerMagic.h>

namespace CppLogic {
	template<>
	inline auto GetIdManager<shok::AdvancedDealDamageSource>() {
		return CppLogic::MagicEnum::EnumIdManager<shok::AdvancedDealDamageSource>{};
	}
}


namespace EGL {
	class IEntityDisplay {
	public:

		struct modeldata {
			shok::EntityId EntityID = shok::EntityId::Invalid;
			shok::EntityTypeId EntityType = shok::EntityTypeId::Invalid;
			shok::ModelId ModelOverride = shok::ModelId::Invalid;
			shok::Position Pos{};
		};
		struct playermodeldata {
			shok::EntityTypeId EntityType = shok::EntityTypeId::Invalid;
			shok::EntityId EntityID = shok::EntityId::Invalid;
			shok::PlayerId Player = shok::PlayerId::Invalid;
			shok::ModelId ModelOverride = shok::ModelId::Invalid;
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

		[[nodiscard]] modeldata GetModelData() const;
		[[nodiscard]] playermodeldata GetPlayerModelData() const;
		[[nodiscard]] posdata GetPosData() const;

		shok::EntityId EntityId; // 2 in entity
		PADDINGI(1); // some type of flags
		shok::EntityTypeId EntityType;
		shok::ModelId ModelOverride;
		shok::PlayerId PlayerId;
	};

	class DelayedEventExecutor { // NOLINT(*-pro-type-member-init)
	public:
		shok::Deque<BB::CEvent*> Events;
		CGLEEntity* Entity;
	};

	class CGLEEntity : public BB::IObject, public IEntityDisplay, public EGL::TGLEAttachable<EGL::CGLEEntity, EGL::CEntityAttachmentProxy> {
	public:
		int SetNewTaskListDepth;
		shok::PositionRot Position; // 22
		float Scale; // 25
		PADDINGI(1);
		bool StandardBehaviorActive, TaskAdvanceNextTurnNextTask, ValidPosition, IsOnWater;
		bool UserSelectableFlag, UserControlableFlag, IsVisible, OnlySetTaskListWhenAlive;
		shok::TaskListId TaskListToSet; // 29
		shok::Vector<EGL::CGLEBehavior*> Behaviours; // 30, first field in 31
		shok::TaskState CurrentState; // 34
		shok::EntityStaus EntityState;
		shok::TaskListId CurrentTaskListID;
		int CurrentTaskIndex; // 37
		shok::Map<shok::TaskState, EGL::IGLEStateHandler*> StateHandlers; // 38
		struct TaskHandlerList {
			shok::Map<shok::Task, EGL::TaskHandler*> Handlers;
			bool AllowAdd;
		} TaskHandlers; // 41
		EventHandlerList EventHandlers; //45
		EGL::ISlot* Slots; // same size as behavioProps vector of type, initialized on create
		int Health; // 50
		char* ScriptName; // "Name" in loader
		char* ScriptCommandLine;
		float Exploration;
		int TaskListStart; // 54
		PADDINGI(1);
		DelayedEventExecutor* DelayedEvents; // 56
		bool InvulnerabilityFlag, Suspended; // 57
		PADDING(2);
		int SuspensionTurn;
		shok::Array<int, 3> ScriptingValue; // 59
		int StateChangeCounter; // 63
		int TaskListChangeCounter;
		int NumberOfAuras; // 65

		static inline constexpr int vtp = 0x783E74;
		static inline constexpr int TypeDesc = 0x8077CC;
		static inline constexpr int vtp_IEntityDisplay = 0x783E58;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3F2913E0);

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
		virtual shok::SectorId GetSector() = 0;
		virtual bool IsInSector(shok::SectorId sec) = 0;
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
		virtual void SetTaskState(shok::TaskState st) = 0;
		virtual bool CountsForTechConditions() = 0; // 25 maybe also used for other stuff?
	private:
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
		virtual void OnHealthDecrease(int oldHealth) = 0; // 38

	public:
		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, EGL::CGLEBehavior>
		T* GetBehavior() {
			for (EGL::CGLEBehavior* b : Behaviours) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
		template<typename T>
		requires std::derived_from<T, EGL::CGLEBehavior>
		T* GetBehaviorDynamic() {
			for (EGL::CGLEBehavior* b : Behaviours) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, EGL::CGLEBehavior>
		const T* GetBehavior() const {
			for (const EGL::CGLEBehavior* b : Behaviours) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<const T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
		template<typename T>
		requires std::derived_from<T, EGL::CGLEBehavior>
		const T* GetBehaviorDynamic() const {
			for (const EGL::CGLEBehavior* b : Behaviours) {
				if (b) {
					const T* r = dynamic_cast<const T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}

		[[nodiscard]] bool IsEntityInCategory(shok::EntityCategory cat) const;
		[[nodiscard]] shok::ResourceType GetResourceProvided() const;
		[[nodiscard]] GGlue::CGlueEntityProps* GetEntityType() const;

		int EventGetDamage();
		int EventGetArmor();
		int EventGetMaxWorktime();
		shok::LeaderCommand EventLeaderGetCurrentCommand();
		int GetMaxHealth();
		int LimitedAttachmentGetMaximum(shok::AttachmentType attachType);
		[[nodiscard]] shok::SectorId ResourceTreeGetNearestSector() const;
		bool EventIsInvisible(); // GGL::CCamouflageBehavior
		bool EventIsSettlerOrBuilding();
		bool EventIsWorker();
		bool EventIsBattleOrAutocannon();
		bool EventIsSerfOrWorker();
		bool EventIsSoldier();
		bool EventIsConvertible();
		int EventGetWorktime();
		float EventGetMotivation();

		void SetHealth(int h);
		// nullptr to remove
		// overrides name->id mapping, if already exists
		// but does not remove id->name mapping
		void SetScriptName(const char* n);
		void Hurt(int dmg);
		void SetTaskList(shok::TaskListId tl);
		void SetTaskList(EGL::CGLETaskList* tl);
		EGL::CGLETaskList* GetCurrentTaskList();
		EGL::TaskHandler* GetTaskHandler(shok::Task task);
		void ExecuteTask(EGL::CGLETaskArgs& targ);
		// adds event to queue, gets executed as soon as state is cancelable, then event get deleted, so make sure you allocate on shok heap
		void AddDelayedEvent(BB::CEvent* ev);

		void PerformHeal(int hp, bool healSoldiers);
		bool CheckDodge(); // rolls random, returns if dodge successful
		[[nodiscard]] bool IsInBlocking() const;
		[[nodiscard]] bool IsDead() const;
		[[nodiscard]] shok::AccessCategory GetAccessCategory() const;
		float __thiscall GetBaseExploration();
		static float __thiscall GetBaseExplorationStatic(CGLEEntity* th);

		void Destroy();

		[[nodiscard]] shok::EntityId GetFirstAttachedToMe(shok::AttachmentType attachmentId) const; // other -> this
		[[nodiscard]] shok::EntityId GetFirstAttachedEntity(shok::AttachmentType attachmentId) const; // this -> other
		/**
		fires event eventIdOnThisDetach on otherId, if this gets detached/destroyed, fires eventIdOnOtherDetach on this, if otherId gets detached/destroyed
		**/
		void AttachEntity(shok::AttachmentType attachtype, shok::EntityId otherId, shok::EventIDs eventIdOnThisDetach, shok::EventIDs eventIdOnOtherDetach); // this -> other
		static void __thiscall AttachEntityStatic(EGL::CGLEEntity* th, shok::AttachmentType attachtype, shok::EntityId otherId, shok::EventIDs eventIdOnThisDetach, shok::EventIDs eventIdOnOtherDetach); // this -> other
		void DetachObservedEntity(shok::AttachmentType attachtype, shok::EntityId otherId, bool fireEvent); // this -> other
		void DetachObserverEntity(shok::AttachmentType attachtype, shok::EntityId otherId, bool fireEvent); // other -> this, in shok only triggered by other dying

		void ClearAttackers();

		CppLogic::EntityAddon::EntityAddonData* GetAdditionalData(bool create = false);
		[[nodiscard]] const CppLogic::EntityAddon::EntityAddonData* GetAdditionalData() const;
		void CloneAdditionalDataFrom(const CppLogic::EntityAddon::EntityAddonData& other);

		static bool AdvHurtEntity_CheckOverHeal;
		float CalculateDamageAgainstMe(int damage, shok::DamageClassId damageclass, float aoeFactor = 1.0f);
		[[nodiscard]] float ModifyDamage(int baseDamage) const;
		[[nodiscard]] float ModifyDamage(float baseDamage) const;
		float GetTotalAffectedDamageModifier();
		void AdvancedHurtEntityBy(EGL::CGLEEntity* attacker, int damage, shok::PlayerId attackerFallback, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo);
		static void __thiscall AdvancedHurtEntityByStatic(EGL::CGLEEntity* th, EGL::CGLEEntity* attacker, int damage, shok::PlayerId attackerFallback, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo);
		static void __stdcall AdvancedDealAoEDamage(EGL::CGLEEntity* attacker, const shok::Position& center, float range, int damage, shok::PlayerId player, shok::DamageClassId damageclass, bool uiFeedback, bool xp, bool addStat, shok::AdvancedDealDamageSource sourceInfo);
		CGLEEntity* AdvChangePlayer(shok::PlayerId player); // destroys entity, returns new one

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
		static std::multimap<shok::EntityTypeId, shok::TechnologyId> BuildingMaxHpTechBoni; // ety->techid
		static bool UseMaxHPTechBoni;
		static void HookMaxHP();

	private:
		int ExecuteLuaTask(EGL::CTaskArgsInteger* arg);
		shok::TaskStateExecutionResult ExecuteLuaTaskState(int i);
		void __thiscall AddHandlerLuaTask();
		static int __stdcall CanCancelStateAdditionalCheck(CGLEEntity* th);
		static void NAKED_DECL CanCancelStateAdditionalCheckASM();
		int __thiscall GetMaxHPOverride();
		static int __thiscall GetMaxHPOverrideStatic(CGLEEntity* th);
		friend class GGL::CGLGUIInterface;
		static void __thiscall OnCreateFixHP(CGLEEntity* th, const EGL::CGLEEntityCreator* c);
		static void NAKED_DECL OnCreateFixHPASM();

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
		static shok::EntityId __cdecl FixedChangePlayer(shok::EntityId id, shok::PlayerId pl);
		static void __cdecl FixedHurtEntity(EGL::CGLEEntity* att, EGL::CGLEEntity* tar, int dmg);
		static void __cdecl FixedHurtEntityAoE(EGL::CGLEEntity* att, shok::Position* p, float r, int dmg, shok::PlayerId pl, shok::DamageClassId dmgcl);
	public:
		static void HookHurtEntity();
		static bool HurtEntityCallWithNoAttacker;
		static CppLogic::EntityAddon::EntityAddonData LastRemovedEntityAddonData;

		static inline EGL::CGLEEntity* (__stdcall* const GetEntityByID)(shok::EntityId id) = reinterpret_cast<EGL::CGLEEntity * (__stdcall*)(shok::EntityId)>(0x5825B4);
		static inline void(__cdecl* const EntityHurtEntity)(EGL::CGLEEntity* attackerObj, EGL::CGLEEntity* targetObj, int damage) = reinterpret_cast<void(__cdecl*)(EGL::CGLEEntity*, EGL::CGLEEntity*, int)>(0x49F358);
		static inline void(__cdecl* const EntityDealAoEDamage)(EGL::CGLEEntity* attacker, shok::Position* center, float range, int damage, shok::PlayerId player, shok::DamageClassId damageclass) = reinterpret_cast<void(__cdecl*)(EGL::CGLEEntity*, shok::Position*, float, int, shok::PlayerId, shok::DamageClassId)>(0x49F82A);
		static inline shok::EntityId(__cdecl* const GetEntityIDByScriptName)(const char* str) = reinterpret_cast<shok::EntityId(__cdecl*)(const char* str)>(0x576624);
		static inline shok::ResourceType(__cdecl* const EntityIDGetProvidedResource)(shok::EntityId id) = reinterpret_cast<shok::ResourceType(__cdecl*)(shok::EntityId)>(0x4B8489);
		static inline bool(__cdecl* const EntityIDIsDead)(shok::EntityId id) = reinterpret_cast<bool(__cdecl*)(shok::EntityId)>(0x44B096);
		static inline shok::EntityId(__cdecl* const EntityIDChangePlayer)(shok::EntityId entityid, shok::PlayerId player) = reinterpret_cast<shok::EntityId(__cdecl*)(shok::EntityId, shok::PlayerId)>(0x49A6A7);

		static EGL::CGLEEntity* ReplaceEntityWithResourceEntity(EGL::CGLEEntity* e);

	protected:
		int EventGetIntById(shok::EventIDs id);

		// defined tasks: TASK_RESET_TASK_LIST_TIMER, TASK_SET_MODEL, TASK_PLAY_SOUND, TASK_STOP_SOUND
		// defined states: SetNextTaskList

		// add to idmanager 57B1E6()
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
	//constexpr int i = offsetof(CGLEEntity, DelayedEvents) / 4;

	class CMovingEntity : public EGL::CGLEEntity { // NOLINT(*-pro-type-member-init)
		friend class EGL::CGLEEntity;
		virtual void Drown() = 0; // 39
	public:
		shok::Position TargetPosition; // la67
		bool TargetRotationValid;
		PADDING(3);
		float TargetRotation;
		shok::MovementState MovementState; // 70

		static inline constexpr int vtp = 0x783F84;
		static inline constexpr int TypeDesc = 0x8077EC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x77D4E427);

		void AttackMove(const shok::Position& p);
		void AttackEntity(shok::EntityId targetId);
		void HoldPosition();
		void Defend();
		void Move(const shok::Position& p);
		void LeaderAttachSoldier(shok::EntityId soldierId);
		void SettlerExpell();
		void HeroAbilitySendHawk(shok::Position& p);
		void HeroAbilityInflictFear();
		void HeroAbilityPlaceBomb(shok::Position& p);
		void HeroAbilityPlaceCannon(shok::Position& p, shok::EntityTypeId FoundationType, shok::EntityTypeId CannonType);
		void HeroAbilityRangedEffect();
		void HeroAbilityCircularAttack();
		void HeroAbilitySummon();
		void HeroAbilityConvert(shok::EntityId target);
		void HeroAbilityConvertBuilding(shok::EntityId target);
		void HeroAbilitySnipe(shok::EntityId tid);
		void HeroAbilityShuriken(shok::EntityId tid);
		void HeroAbilityMotivateWorkers();
		void HeroAbilityActivateCamoflage();
		void ThiefSabotage(shok::EntityId tid);
		void ThiefDefuse(shok::EntityId tid);
		void ThiefStealFrom(shok::EntityId tid);
		void ThiefSecureGoods(shok::EntityId tid);
		void ScoutBinoculars(shok::Position& p);
		void ScoutFindResource();
		void ScoutPlaceTorch(shok::Position& p);
		bool SerfConstructBuilding(GGL::CBuilding* build);
		bool SerfRepairBuilding(GGL::CBuilding* build);
		void SerfExtractResource(shok::EntityId id);
		void SerfTurnToBattleSerf();
		void BattleSerfTurnToSerf();
		void SetTargetRotation(float f);

		void SetPosition(shok::Position& p);

		int LeaderGetNearbyBarracks();
		bool IsMoving();
		[[nodiscard]] bool IsFleeingFrom(const shok::Position& center, float range) const;

	private:
		static void __thiscall BuildOnSetPosFixed(CMovingEntity* th);
		static void NAKED_DECL BuildOnSetPosFixedASM();
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
		shok::AmbientSoundId AmbientSoundType;

		static inline constexpr int vtp = 0x78568C;
		static inline constexpr int TypeDesc = 0x839FF0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE12A3723);
	};
	static_assert(offsetof(CAmbientSoundEntity, AmbientSoundType) == 67 * 4);

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
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
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CEntityProfile : public EGL::IProfileModifierSetObserver {
	public:
		struct ModifyableValue {
			bool NeedsRefresh;
			PADDING(3);
			float Value;
		};

		shok::Vector<ModifyableValue*> OnTickReset;
		shok::EntityId EntityId;
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
		shok::String CurrentTaskList; // 40 where does this get updated?
		struct EntityRef {
			friend class CEntityProfile;
			EGL::CGLEEntity* Self; // 125 in settler
			EGL::CGLEEntity* Leader; // self if it is a leader
			shok::EntityId LeaderId;
			GGL::CSettler* SelfSett;
			bool NeedsInitialization;
			PADDING(3);
			int OverheadWidget; // 5, 130 in settler, soldier=4, leader=3, worker=2, serf=1
			shok::ExperienceClass ExperienceClass; // 6 gets set by checking entitycategories
			shok::EntityId EntityId; // 7
			GGL::CGLSettlerProps* Props;

			void CheckInit();
		private:
			static void __thiscall AssignAdvExperienceClass(EntityRef* th);
		} EntityReference;

		const GGL::ExperienceClass::LevelData* GetExperienceClassLevel();


		static inline constexpr int vtp = 0x7727A4;

		static void HookExperience(bool active);
		static void HookExperienceClassAssignment(bool active);
	private:
		static void NAKED_DECL AssignAdvExperienceClassASM();
	};
	static_assert(sizeof(CEntityProfile) == 56 * 4);
	struct ModifierEntityDatabase {
		shok::Map<shok::EntityId, CEntityProfile*> Profiles;

		static inline ModifierEntityDatabase* const GlobalObj = reinterpret_cast<ModifierEntityDatabase*>(0x895E5C);

		float GetModifiedStat(shok::EntityId id, CEntityProfile::ModifierType ty, float initial);
		float GetModifiedStatNoCache(shok::EntityId id, CEntityProfile::ModifierType ty, float initial);
	};

	class CEvadingEntity : public EGL::CMovingEntity {
	public:
		struct EvaderWaitData {
			int Turns;
			shok::EntityId EvaderID;
		};

		shok::Vector<EvaderWaitData> EvaderWaitObject;

		static inline constexpr int vtp = 0x770A7C;
		static inline constexpr int TypeDesc = 0x810C34;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x42D23A53);
	};
	static_assert(sizeof(CEvadingEntity) == 75 * 4);

	class CSettler : public GGL::CEvadingEntity { // NOLINT(*-pro-type-member-init)
	public:
		int TimeToWait, HeadIndex, HeadParams; // la77

		CEntityProfile ModifierProfile;

		int BlessBuff, NPCMarker, LeaveBuildingTurn; //la136

		static inline constexpr int vtp = 0x76E3CC;
		static inline constexpr int TypeDesc = 0x807858;
		static inline constexpr int vtp_IEntityDisplay = 0x76E3B0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x0EDC11EA8);

		bool IsIdle();

		int LeaderGetRegenHealth();
		int LeaderGetRegenHealthSeconds();
		void KillSettlerByEnvironment();
		int GetDodgeChance(); // used in melee combat, percent
		int __thiscall GetBaseArmor();
		static int __thiscall GetBaseArmorStatic(CSettler* th);

		void Upgrade();
		void Vanish();
		void Appear();

		// defined tasks: TASK_VANISH, TASK_APPEAR, TASK_ENTER_BUILDING, TASK_LEAVE_BUILDING, TASK_LIST_DONE, TASK_SET_TASK_LIST, TASK_WAIT, TASK_LEFT_BUILDING, TASK_WAIT_UNTIL
		//		TASK_RESOLVE_COLLISION, TASK_GO_TO_FREE_POSITION, TASK_SET_POS (set relative to workplace)
		// defined events: GetArmorClass, GetArmor, Settler_XXX, Worker_Bless (set BlessBuff to 10), IsConvertible
		// defined states: Wait, Default (set tl to default)
	};
	static_assert(sizeof(CSettler) == 137 * 4);
	static_assert(offsetof(CSettler, ModifierProfile.EntityReference.OverheadWidget) == 130 * 4);
	static_assert(offsetof(CSettler, ModifierProfile.EntityReference.Self) == 125 * 4);
	//constexpr int i = offsetof(CSettler, ModifierProfile.OverheadWidget) / 4;

	class CAnimal : public EGL::CMovingEntity { // NOLINT(*-pro-type-member-init)
	public:
		shok::Position TerritoryCenter; // 71
		float TerritoryRadius; // int?
		shok::Position DangerPosition;

		static inline constexpr int vtp = 0x778F7C;
		static inline constexpr int TypeDesc = 0x812038;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5D6B737);

		// defined tasks: TASK_WANDER, TASK_SET_TASK_LIST, TASK_BACK_TO_DEFAULT_TASKLIST, TASK_FLEE
		// defined events: Animal_XXX, Die, Leader_SetTerritory
		// tick checks flee from each second (on a tick depending on entityid)
	};

	class CResourceDoodad : public EGL::CGLEEntity { // NOLINT(*-pro-type-member-init)
	public:
		shok::ResourceType ResourceType; //66
		int ResourceAmount, ResourceAmountAdd; // current, max

		// sets current and refreshes model depending on max
		void SetCurrentResourceAmount(int am);

		static inline constexpr int vtp = 0x76FEA4;
		static inline constexpr int TypeDesc = 0x8118AC;
		static inline constexpr int vtp_IEntityDisplay = 0x76FE88;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB0778F47);

		static shok::EntityCategory RefillableCategory;
		static void HookAutoDestroyIfEmpty();
	private:
		static void __thiscall OnEmptyDestroy(CResourceDoodad* th);
		static void NAKED_DECL OnEmptyDestroyASM();
	};

	class CBuilding : public EGL::CGLEEntity { // NOLINT(*-pro-type-member-init)
	public:
		shok::Position ApproachPosition, LeavePosition; // fi 66
		bool IsActive, IsRegistered, IsUpgrading, IsOvertimeActive; // 70
		bool WorkerAlarmModeActive;
		PADDING(3);
		int MaxNumWorkers; // 72
		shok::TechnologyId CurrentTechnology;
		int LatestAttackTurn, MostRecentDepartureTurn;
		float ConstructionProgress, RepairProgress, UpgradeProgress; // 76 la78
		PADDINGI(2);// list Slots with NumberOfRepairingSerfs? 79
		int NumberOfRepairingSerfs;
		int OvertimeRechargeTime; // 82

		virtual shok::EntityId GetLastAttachedWorker() = 0;// 39
	private:
		virtual int UnknownBuildingFunc1() = 0; // 40
	public:
		virtual shok::Position* GetBuilderSlotOffset(shok::Position* ret, int num) const = 0;
		virtual shok::Position* GetBuilderSlotAbs(shok::Position* ret, int num) const = 0;
		[[nodiscard]] virtual float GetBuilderSlotRotationOff(int num) const = 0;
		virtual void SetConstructionProgress(float p) = 0;
		[[nodiscard]] virtual float GetConstructionProgress() const = 0; //45
		virtual shok::Position* GetDoorPosOff(shok::Position* ret) const = 0;
		virtual shok::Position* GetDoorPosAbs(shok::Position* ret) const = 0;
		virtual shok::Position* GetApproachPosAbsFallbackNearbyPos(shok::Position* ret, float fallbackRange) = 0;
	private:
		virtual int UnknownBuildingFunc2() = 0; // calculates some pos (Bridge returns its pos)
	public:
		[[nodiscard]] virtual bool IsNotPlayer0() const = 0; //50
		virtual void RegisterToPlayer() = 0; // adds itself to player, then sets IsRegistered=true
		[[nodiscard]] virtual int GetNumberOfBuilderSlots() const = 0; // 52

		static inline constexpr int vtp = 0x76EB94;
		static inline constexpr int TypeDesc = 0x807898;
		static inline constexpr int vtp_IEntityDisplay = 0x76EB78;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x15EBDB60);

		[[nodiscard]] shok::EntityId GetConstructionSite() const;
		int GetNearestFreeConstructionSlotFor(shok::Position* p) const;
		int GetNearestFreeRepairSlotFor(shok::Position* p);
		[[nodiscard]] bool IsConstructionFinished() const;
		bool IsIdle(bool forRecruitemnt = false, bool ignoreAlarm = false);
		shok::TechnologyId GetTechnologyInResearch();
		int GetCannonProgress();
		float GetMarketProgress();
		// returns max time if not currently upgrading
		float GetRemainingUpgradeTime();
		int __thiscall GetBaseArmor();
		static int __thiscall GetBaseArmorStatic(CBuilding* th);

		void StartUpgrade();
		void CancelUpgrade();
		void CommandBuildCannon(shok::EntityTypeId entitytype);
		void CommandRecruitSoldierForLeader(shok::EntityId id);
		void ActivateOvertime();
		void DeactivateOvertime();
		void BarracksRecruitGroups();
		void BarracksRecruitLeaders();
		void HQBuySerf();
		void SellBuilding();
		void StartResearch(shok::TechnologyId tech);
		void CancelResearch();
		void MarketStartTrade(shok::ResourceType ResourceTypeSell, shok::ResourceType ResourceTypeBuy, float BuyAmount);
		void MarketCancelTrade();
		shok::Position GetAbsoluteApproachPos();
		bool IsHealthBurning(int health);
		bool IsHealthBurning();
		void CatchFire();
		shok::TaskListId GetWorkTaskList(); // gets first unused tasklist, or last tasklist
		[[nodiscard]] shok::EntityTypeId GetWorkerType() const;

		// defined events: IsConvertible, GetArmorClass, GetArmor, OnAttackedBy, WorkerAlarmMode_Enable, WorkerAlarmMode_Disable


		static std::vector<shok::AdditionalTechModifier> ConstructionSpeedModifiers;
		static void EnableConstructionSpeedTechs();
	};
	static_assert(offsetof(CBuilding, IsActive) == 70 * 4);
	static_assert(offsetof(CBuilding, UpgradeProgress) == 78 * 4);
	static_assert(offsetof(CBuilding, ConstructionProgress) == 76 * 4);

	class CConstructionSite : public CBuilding { // NOLINT(*-pro-type-member-init)
	public:
		shok::EntityTypeId BuildingType;

		static inline constexpr int vtp = 0x77003C;
		static inline constexpr int TypeDesc = 0x8078DC;
		static inline constexpr int vtp_IEntityDisplay = 0x77001C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2B5B9C80);
	};
	static_assert(offsetof(CConstructionSite, BuildingType) == 83 * 4);
	static_assert(sizeof(CConstructionSite) == 84 * 4);

	class CBridgeEntity : public GGL::CBuilding {
		friend class EGL::CGLETerrainLowRes;
	public:
		static inline constexpr int vtp = 0x77805C;
		static inline constexpr int TypeDesc = 0x812054;
		static inline constexpr int vtp_IEntityDisplay = 0x778040;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3736FF8E);

	private:
		void __thiscall ApplyHeightOverride() const;
	};

}

namespace EGL {
	class CGLEEntityCreator : public BB::IObject {
	public:
		shok::EntityTypeId EntityType = shok::EntityTypeId::Invalid;
		shok::PositionRot Pos = { 0,0,0 };
		int Flags = 0; // 5
		shok::PlayerId PlayerId = shok::PlayerId::Invalid;
		int Health = 0; // 7
		char* ScriptName = nullptr; // use shok::Malloc, gets freed by destructor
		char* ScriptCommandLine = nullptr; // use shok::Malloc, gets freed by destructor
		shok::AmbientSoundId AmbientSoundType = shok::AmbientSoundId::Invalid;
		float Scale = 0; // 11

		static inline constexpr int vtp = 0x766B50;
		static inline constexpr int TypeDesc = 0x807874;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x753286D);

		// i have to implement them so the compiler stops complaining.
		// i call the original constructor and replace the vtable with that, so they really dont matter.
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;
		virtual ~CGLEEntityCreator() override;

		CGLEEntityCreator();
	protected:
		CGLEEntityCreator(int _);
	};
}

namespace GGL {
	class CGLConstructionSiteCreator : public EGL::CGLEEntityCreator {
	public:
		shok::EntityTypeId BuildingType = shok::EntityTypeId::Invalid; // set EntityType to construction site type (from building props) and BuildingType to the building you actually want to construct

		static inline constexpr int vtp = 0x770114;
		static inline constexpr int TypeDesc = 0x811A80;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF088740);

		CGLConstructionSiteCreator();
	};
	class CResourceDoodadCreator : public EGL::CGLEEntityCreator {
	public:
		int ResourceAmount = 0;

		static inline constexpr int vtp = 0x774898;
		static inline constexpr int TypeDesc = 0x811974;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x85589767);

		CResourceDoodadCreator();
	};
}

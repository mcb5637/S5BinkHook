#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_behaviors.h"
#include "s5_behaviorProps.h"

#include "mod.h"
#include "s5_staticlist.h"
#include "s5_netevents.h"

namespace CppLogic::Mod {
	void RegisterClasses();

	class FormationSpacedBehaviorProps : public GGL::CFormationBehaviorProperties {
	public:
		float SpaceFactor = 0;

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1009);
		static const BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	class FormationSpacedBehavior : public GGL::CFormationBehavior, public IFormationBehaviorExtProvider {
	public:

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		virtual shok::Position GetPosExt(EGL::CGLEEntity* leader) override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1010);
		static const BB::SerializationData SerializationData[];

		FormationSpacedBehavior();

		void* operator new(size_t s);
		void operator delete(void* p);
	};

	class ResourceTrackerBehavior : public EGL::CGLEBehavior {
	public:
		shok::CostInfo Produced, Used;

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void __thiscall AddHandlers(shok::EntityId id) override;
		virtual void __thiscall OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void __thiscall OnEntityLoad(EGL::CGLEBehaviorProps* p) override;
		virtual void __thiscall OnEntityUpgrade(EGL::CGLEEntity* old) override;


		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x100B);
		static const BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);

	private:
		void EventMinedOrRefined(GGL::CEventGoodsTraded* ev);
		void EventSupplied(GGL::CEventGoodsTraded* ev);
	};


	class HawkCircleBehavior : public EGL::CGLEBehavior {
	public:
		shok::Position ExploreTarget;

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1015);
		static BB::SerializationData SerializationData[];


		void* operator new(size_t s);
		void operator delete(void* p);

		shok::Position GetNextCirclePosition(const shok::Position& center);

	private:
		void EventOnHeroDetach(BB::CEvent* e);
		void EventSendHawk(EGL::CEventPosition* e);

		int TaskExplore(EGL::CGLETaskArgs* a);
		shok::TaskStateExecutionResult StateExplore(int t);
		int TaskCheckHeroPos(EGL::CGLETaskArgs* a);
		shok::TaskStateExecutionResult StateCheckHeroPos(int t);
	};
	class HawkOwnerAbility : public GGL::CHeroAbility {
	public:

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1016);
		static BB::SerializationData SerializationData[];
		static inline constexpr shok::AbilityId AbilityId = shok::AbilityId::AbilitySendHawk;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

	private:
		void EventCreateHawk(BB::CEvent* e);
		void EventSendHawk(EGL::CEventPosition* ev);
		void EventDie(BB::CEvent* e);
	};


	class LightningStrikeAbilityProps : public GGL::CHeroAbilityProps {
	public:
		int Damage = 0;
		shok::DamageClassId DamageClass = {};
		float Range = 0;
		float DamageRange = 0;
		shok::EffectTypeId Effect = {};
		float WeatherEnergyCost = 0;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1013);
		static const BB::SerializationData SerializationData[];


		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	class LightningStrikeAbility : public GGL::CHeroAbility {
	public:
		
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1014);
		static const BB::SerializationData SerializationData[];
		static inline constexpr shok::AbilityId AbilityId = shok::AbilityId::AbilityLightningStrike;
		static GGL::CStaticHeroAbilityID AbilityStatic;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual ~LightningStrikeAbility() override = default;
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

		static void NetEventLightningStrike(EGL::CNetEventEntityAndPos* ev);
	private:
		void EventLightningStrike(EGL::CEventPosition* p);
	};

	class ResDoodadRefillBehaviorProps : public GGL::CHeroAbilityProps {
	public:
		int RefillAmount = 0;
		shok::EffectTypeId Effect = {};
		shok::EntityCategory AffectedTypes = {};
		shok::TaskListId TaskList = {};

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1017);
		static const BB::SerializationData SerializationData[];


		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	class ResDoodadRefillBehavior : public GGL::CHeroAbility {
	public:

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1018);
		static const BB::SerializationData SerializationData[];
		static inline constexpr shok::AbilityId AbilityId = shok::AbilityId::AbiltyResourceDoodadRefill;
		static GGL::CStaticHeroAbilityID AbilityStatic;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual ~ResDoodadRefillBehavior() override = default;
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

		static void NetEventRefillResDoodad(EGL::CNetEvent2Entities* ev);
	private:
		int TaskGoToResource(EGL::CGLETaskArgs* a);
		int TaskExtractRes(EGL::CGLETaskArgs* a);
		void EventActivate(EGL::CEvent1Entity* ev);
	};


	class ShieldCoverAbilityProps : public GGL::CHeroAbilityProps {
	public:
		float Range = 0;
		int Duration = 0;
		shok::TaskListId TaskList = shok::TaskListId::Invalid;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1019);
		static BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	};
	class ShieldCoverAbility : public GGL::CHeroAbility {
	public:
		int DurationLeft = 0;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x101A);
		static BB::SerializationData SerializationData[];
		static inline constexpr shok::AbilityId AbilityId = shok::AbilityId::AbilityShieldCover;
		static GGL::CStaticHeroAbilityID AbilityStatic;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

		void Activate();
		void ClearProjectiles();
		static void NetEventShieldCover(EGL::CNetEventEntityID* ev);
	private:
		int TaskShieldCover(EGL::CGLETaskArgs* a);
		shok::TaskStateExecutionResult StateShieldCover(int);
		void EventTick(BB::CEvent* ev);
		void EventActivate(BB::CEvent* ev);
	};

	class ResurrectAbilityProps : public GGL::CHeroAbilityProps {
	public:
		float Range = 0;
		int ProgressPerTick = 0;
		shok::TaskListId TaskList = shok::TaskListId::Invalid;
		int RechargeAbilitiesBy = 0;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x101B);
		static BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	};
	class ResurrectAbility : public GGL::CHeroAbility {
	public:
		shok::EntityId Target = {};
		bool SelfCasting = false;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x101C);
		static BB::SerializationData SerializationData[];
		static inline constexpr shok::AbilityId AbilityId = shok::AbilityId::AbilityResurrect;
		static GGL::CStaticHeroAbilityID AbilityStatic;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

		static void NetEventResurrect(EGL::CNetEvent2Entities* ev);
	private:
		void EventActivate(EGL::CEvent1Entity* ev);
		void EventDie(BB::CEvent* ev);
		void EventTick(BB::CEvent* ev);
		int TaskResurrect(EGL::CGLETaskArgs* a);
		int TaskTurnTo(EGL::CGLETaskArgs* a);
		shok::TaskStateExecutionResult StateResurrect(int time);
		shok::TaskStateExecutionResult StateSelfResurrect(int time);
		void ChargeAbilitiesFor(EGL::CGLEEntity* e);
	};


	class BombardmentAbilityProps : public GGL::CHeroAbilityProps {
	public:
		shok::EffectTypeId EffectType = shok::EffectTypeId::Invalid;
		float AttackRange = 0;
		float DamageRange = 0;
		int Damage = 0;
		shok::DamageClassId DamageClass = shok::DamageClassId::Invalid;
		shok::TaskListId TaskList = shok::TaskListId::Invalid;
		float DistanceOverride = -1;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x101E);
		static BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	};
	class BombardmentAbility : public GGL::CHeroAbility {
	public:
		shok::Position Target;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x101F);
		static BB::SerializationData SerializationData[];
		static inline constexpr shok::AbilityId AbilityId = shok::AbilityId::AbilityBombardment;
		static GGL::CStaticHeroAbilityID AbilityStatic;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

		void Activate(const shok::Position& tar);
		static void NetEventBombard(EGL::CNetEventEntityAndPos* ev);
	private:
		int TaskBombard(EGL::CGLETaskArgs* a);
		int TaskTurnToBombardTarget(EGL::CGLETaskArgs* a);
		void EventActivate(EGL::CEventPosition* p);
	};

	class LimitedAmmoBehavior : public EGL::CGLEBehavior {
	public:
		int RemainingAmmo = 0;

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1021);
		static BB::SerializationData SerializationData[];


		void* operator new(size_t s);
		void operator delete(void* p);

	private:
		int TaskDecrementAmmo(EGL::CGLETaskArgs* a);
		int TaskCheckAmmo(EGL::CGLETaskArgs* a);
		shok::TaskStateExecutionResult StateCheckAmmo(int time);
	};
}

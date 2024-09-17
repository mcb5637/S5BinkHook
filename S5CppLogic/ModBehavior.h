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

}

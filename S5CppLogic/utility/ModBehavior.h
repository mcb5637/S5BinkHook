#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_behaviors.h>
#include <shok/entitytype/s5_behaviorProps.h>

#include <utility/mod.h>
#include <shok/globals/s5_staticlist.h>
#include <shok/events/s5_netevents.h>

namespace CppLogic::Mod {
	void RegisterClasses();

	class FormationSpacedBehaviorProps : public GGL::CFormationBehaviorProperties {
	public:
		float SpaceFactor = 0;

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1009);
		static const BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	class FormationSpacedBehavior : public GGL::CFormationBehavior, public IFormationBehaviorExtProvider {
	public:

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1015);
		static BB::SerializationData SerializationData[];


		void* operator new(size_t s);
		void operator delete(void* p);

		[[nodiscard]] shok::Position GetNextCirclePosition(const shok::Position& center) const;

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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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


		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;

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
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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


		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;

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
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

		void Activate();
		void ClearProjectiles() const;
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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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
		shok::EntityTypeId EntityToCreate = {};

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x101E);
		static BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual bool IsAbility(shok::AbilityId ability) override;

		void Activate(const shok::Position& tar);
		static void NetEventBombard(EGL::CNetEventEntityAndPos* ev);
	private:
		int TaskBombard(EGL::CGLETaskArgs* a);
		int TaskTurnToBombardTargetSettler(EGL::CGLETaskArgs* a);
		int TaskTurnToBombardTargetAutocannon(EGL::CGLETaskArgs* a);
		int TaskBackToDefaultTL(EGL::CGLETaskArgs* a);
		void EventActivate(EGL::CEventPosition* p);
		shok::TaskStateExecutionResult StateTurnToBombardTargetAutocannon(int time);
	};

	class LimitedAmmoBehavior : public EGL::CGLEBehavior {
	public:
		int RemainingAmmo = 0, MaxAmmo = 0;

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1021);
		static BB::SerializationData SerializationData[];


		void* operator new(size_t s);
		void operator delete(void* p);

		void ReloadToMax(int max);
	private:
		int TaskDecrementAmmo(EGL::CGLETaskArgs* a);
		int TaskCheckAmmo(EGL::CGLETaskArgs* a);
		shok::TaskStateExecutionResult StateCheckAmmo(int time);
	};

	class ReloadableCannonBuilderAbilityProps : public GGL::CCannonBuilderBehaviorProps {
	public:
		int Reloads = 0;
		float ReloadRange = 0;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1022);
		static BB::SerializationData SerializationData[];

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		void* operator new(size_t s);
		void operator delete(void* p);
	};

	class ReloadableCannonBuilderAbility : public GGL::CCannonBuilderBehavior {
	public:

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1023);
		static BB::SerializationData SerializationData[];

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;

		void* operator new(size_t s);
		void operator delete(void* p);

		static void NetEventBombComboCannon(EGL::CNetEventEntityAndPos* ev);
	protected:
		int TaskBuildCannonA(EGL::CGLETaskArgs* a);
		void EventTick(BB::CEvent* ev);
		void EventBombComboCannonActivate(EGL::CEventPosition* ev);
	};

	class LimitedAmmoUIDisplayBehavior : public CppLogic::Mod::OnScreenInfoDisplayBehavior {
	public:
		ED::CEntity* Entity = nullptr;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1024);
		static constexpr inline const BB::SerializationData* const SerializationData = nullptr;

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;
		virtual void __stdcall OnAdd(ED::CEntity* edispl, ED::CBehaviorProps* props, shok::ModelId* modelOverride) override;
		virtual void __stdcall Initialize(ED::CEntity* edispl, ED::CBehaviorProps* props) override;
		virtual void __stdcall UpdateRenderNoTick(int count, float uk) override;
		virtual void __stdcall UpdateRenderOneTick(int count, float uk) override;
		virtual void __stdcall UpdateRenderManyTick(int count, float uk) override;
		virtual bool RenderUI(GGUI::OnScreenInfoRenderer* renderer, shok::Position* screenPos, GGL::IGLGUIInterface::UIData* data, bool* active) override;

		LimitedAmmoUIDisplayBehavior();

		void* operator new(size_t s);
		void operator delete(void* p);
	};

	class AdvancedMarketBehaviorProps : public GGL::CServiceBuildingBehaviorProperties {
	public:
		int WorkStepsNeededForTrade = 0;

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1026);
		static BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);
	};

	class AdvancedMarketBehavior : public EGL::CGLEBehavior {
	public:
		struct ResourceData {
			float Gold = 0;
			float Wood = 0;
			float Clay = 0;
			float Stone = 0;
			float Iron = 0;
			float Sulfur = 0;

			float& ByResT(shok::ResourceType t);

			static BB::SerializationData SerializationData[];
		} SellResources, MinResources;
		GGL::CTradeManager::TradeOrder CurrentTrade; // not serialized
		int WorkSteps = 0;

		static constexpr std::array ResTradeOrder{
			shok::ResourceType::Wood,
			shok::ResourceType::Clay,
			shok::ResourceType::Stone,
			shok::ResourceType::Iron,
			shok::ResourceType::Sulfur,
		};

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1025);
		static BB::SerializationData SerializationData[];

		AdvancedMarketBehavior();

		void* operator new(size_t s);
		void operator delete(void* p);

		float PredictPriceFor(shok::ResourceType rt, float am);
	private:
		void FillCurrentTrade();
		float TradeStepSize();
		void FillCurrentTrade(shok::ResourceType res, float amount, float tradestep);
		std::string_view GetResIcon(shok::ResourceType t);
		static std::string_view ResPrefix(float r);
		void PerformTradeStep();

		void EventWork(BB::CEvent* ev);
		void EventStartTrade(GGL::CEventTransaction* ev);
		void EventCancel(BB::CEvent* ev);
		void EventGetProgress(EGL::CEventGetValue_Float* ev);
	};

	class AdvancedFoundationBehaviorProps : public EGL::CGLEBehaviorProps {
	public:
		struct TurretData {
			shok::EntityTypeId Turret = {};
			shok::Position Offset;
			shok::CostInfo BuyCost;

			static BB::SerializationData SerializationData[];
		};
		std::vector<TurretData> Turret;

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1027);
		static BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);
	};

	class AdvancedFoundationBehavior : public EGL::CGLEBehavior {
	public:
		struct TurretData {
			shok::EntityId Turret = {};
			bool Active = false;

			static BB::SerializationData SerializationData[];
		};
		std::vector<TurretData> Turret;

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void AddHandlers(shok::EntityId id) override;
		virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override;
		virtual void OnEntityUpgrade(EGL::CGLEEntity* old) override;

		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1028);
		static BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);

		[[nodiscard]] bool CanBuyTurret(int i) const;
		[[nodiscard]] bool IsTurretActive(int i) const;
	private:
		void InitData();
		void CreateMissingTurrets();
		void ClearTurret(TurretData& t);
		void EventComplete(BB::CEvent* ev);
		void EventStartUpgrade(BB::CEvent* ev);
		void EventTurretDetach(EGL::CEvent1Entity* ev);
		void EventAttack(EGL::CEvent1Entity* ev);
		void EventBuyTurret(EGL::CEventValue_Int* ev);
	};
}

#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/globals/s5_idmanager.h>

namespace EGL {
	class CGLEAnimProps : public BB::IObject {
	public:
		int Duration = 0; // ticks
		bool IsWalkOrRun = false;

		static inline constexpr int vtp = 0x788D7C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x12FC4F78);

		CGLEAnimProps();
		CGLEAnimProps(const CGLEAnimProps& o);
		CGLEAnimProps(CGLEAnimProps&& o) noexcept;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	private:
		void SetVT(int vt);
	};
}

namespace GGL {
	class CDamageClassProps : public BB::IObject {
	public:
		//float BonusVsArmorClass[7]; // remember to access with ArmorClass - 1
		float ArmorClassNoneFactor = 0, ArmorClassJerkinFactor = 0, ArmorClassLeatherFactor = 0;
		float ArmorClassIronFactor = 0, ArmorClassFortificationFactor = 0, ArmorClassHeroFactor = 0, ArmorClassFurFactor = 0;
		shok::UpgradeCategoryId SoldierCategory = {}, LeaderCategory = {}; // unused
		
		float& GetBonusVsArmorClass(shok::ArmorClassId ac);

		static inline constexpr int vtp = 0x788978;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x520829DD);
		static inline const BB::SerializationData* SerializationData = reinterpret_cast<const BB::SerializationData*>(0xA0D068);
	};
	static_assert(sizeof(CDamageClassProps) == 10 * 4);
	static_assert(offsetof(CDamageClassProps, LeaderCategory) == 9 * 4);
	class DamageClassesHolder {
	public:
		BB::CIDManagerEx* DamageClassManager;
		shok::Vector<GGL::CDamageClassProps*> DamageClassList; // there is a damageclass 0, probably not working at all

		GGL::CDamageClassProps* Get(shok::DamageClassId id);

		static inline GGL::DamageClassesHolder** const GlobalObj = reinterpret_cast<GGL::DamageClassesHolder**>(0x85A3DC);
	};

	class CLogicProperties : public BB::IObject { // noted here are default values, loading logic.xml might override stuff
	public:
		struct SBuildingUpgradeCategory {
			shok::UpgradeCategoryId Category;
			shok::EntityTypeId FirstBuilding;
			int NumScholars;

			static inline constexpr int vtp = 0x76EF10;
			static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9498E0E7);
			virtual shok::ClassId GetClassIdentifier() const;

			SBuildingUpgradeCategory(shok::UpgradeCategoryId car, shok::EntityTypeId first);
			SBuildingUpgradeCategory(SBuildingUpgradeCategory&& o) noexcept;
		};
		struct SSettlerUpgradeCategory {
			shok::UpgradeCategoryId Category;
			shok::EntityTypeId FirstSettler;

			static inline constexpr int vtp = 0x76EF18;
			static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8DC75427);
			virtual shok::ClassId GetClassIdentifier() const;

			SSettlerUpgradeCategory(shok::UpgradeCategoryId cat, shok::EntityTypeId first);
			SSettlerUpgradeCategory(SSettlerUpgradeCategory&& o) noexcept;
		};
		struct STaxationLevel {
			int RegularTax;
			float MotivationChange;

			static inline constexpr int vtp = 0x76EF20;
			virtual shok::ClassId GetClassIdentifier() const = 0;
		};
		struct STradeResource {
			shok::ResourceType ResourceType;
			float BasePrice, MinPrice, MaxPrice, Inflation, Deflation, WorkAmount;

			static inline constexpr int vtp = 0x76EF28;
			virtual shok::ClassId GetClassIdentifier() const = 0;
		};
		struct SBlessCategory {
			shok::BlessCategoryId Name;
			float RequiredFaith;
			shok::Vector<shok::EntityTypeId> EntityTypes;

			static inline constexpr int vtp = 0x76EFC4;
			virtual unsigned int GetClassIdentifier() const = 0;
		};

		int CompensationOnBuildingSale = 100;
		shok::Vector<SBuildingUpgradeCategory> BuildingUpgrades;
		shok::Vector<SSettlerUpgradeCategory> SettlerUpgrades;
		shok::Vector<STaxationLevel> TaxationLevels;
		shok::Vector<STradeResource> TradeResources;
		shok::Vector<SBlessCategory> BlessCategories; // 18
		float BuildingUnderConstructionExplorationFactor = 1.0f, BuildingPlacementSnapDistance = 100.0f, BuildingClosedHealthFactor; // 22
		float WeatherExplorationBuildingSnowFactor = 1.0f, WeatherExplorationSettlerSnowFactor = 1.0f; // 25
		float WeatherMoveSpeedSnowFactor = 1.0f, WeatherExplorationBuildingRainFactor = 1.0f, WeatherExplorationSettlerRainFactor = 1.0f;
		float WeatherMoveSpeedRainFactor = 1.0f; // 30
		int WeatherMissChanceChangeRain = 0, WeatherMissChanceChangeSnow = 0;
		float AttackMoveRange = 1.0f; // 33
		int TaxAmount = 0;
		float TaxPenalty = 0;
		int InitialTaxLevel = 0; // 36
		shok::CostInfo BlessingCost; // per settler cost
		float BlessingBonus = 0; // 55
		PADDINGI(1);
		int ForceToWorkPenalty = 0;
		PADDINGI(1);
		int ResourceDoodadWarnAmount = 0; // 59
		int FeedbackHandlerUpdateFrequency = 15; // 60
		int FeedbackHandlerTimeStaysInSystem = 60;
		float MilitaryLeaderAutoAttackRange = -1.0f;
		float MilitaryBuildingAutoAttackRangeFactor = 0;
		float MilitaryCivilianAutoAttackRangeFactor = 0;
		float MilitaryDefendRange = 0;
		int MilitaryRecentlyAttackedTime = 0;
		int MilitaryMaxAttackersPerEntity = 0;
		float CivilianDefenseRadius = 1.0f;
		float MotivationThresholdHappy = 0; // 69
		float MotivationThresholdSad = 0, MotivationThresholdAngry = 0, MotivationThresholdLeave = 0, AverageMotivationVillageCenterLockThreshold = 0;
		int WorkTimeBase = 0, WorkTimeThresholdWork = 0, WorkTimeThresholdFarm = 0, WorkTimeThresholdResidence = 0, WorkTimeThresholdCampFire = 0;
		float MotivationGameStartMaxMotivation = 0;
		float MotivationAbsoluteMaxMotivation = 0; // 80
		int MotivationMillisecondsWithoutJob = 0;
		int HeroResurrectionTime = 1, HeroResurrectionSlippage = 0;
		float HeroResurrectionRadius = 0, HeroResurrectionHealthFactor = 0, HeroResurrectionActionPointFactor = 0;
		int HeroComatoseExploration = 0;
		int BattleSerfMaxSeconds = 0;
		shok::EffectTypeId ExpelEffectID = shok::EffectTypeId::Invalid; // 89
		int DefenderMSPerShot = 0; // 90
		float DefenderMaxRange = 0;
		shok::EffectTypeId DefenderProjectileEffectType = shok::EffectTypeId::Invalid;
		int DefenderProjectileDamage = 0;
		shok::DamageClassId DefenderProjectileDamageClass = shok::DamageClassId::Invalid;
		int DefenderMissChance = 0; // 95
		float WorkerFlightDistance = 0;
		int MaxExperiencePoints = 0;
		int BuildingRecentlyAttackedDuration = 0;
		float EnergyRequiredForWeatherChange = 0; // 99
		int DefaultDurationOfWeatherChange = 0;
		float EnergyDeductedFromOtherPlayers = 0;
		float ConstructionSiteHealthFactor = 1.0f;
		float MaxFaith = 0; // 103
		float GuardMaxDistanceBuilding = 0;
		float GuardMoveDistanceBuilding = 0;
		float GuardMaxDistanceOther = 0;
		float GuardMoveDistanceOther = 0;
		float NPCInteractionDistance = 300.0f; // 108
		shok::AnimationId WeatherTowerAnim = shok::AnimationId::Invalid;
		int LeaderNudgeCount = 3;
		float LeaderApproachRange = 200.0f;
		int AlarmRechargeTime = 180000;
		int OvertimeRechargeTimeInMs = 60000; // 113

		static inline constexpr int vtp = 0x76EFCC;

		static inline GGL::CLogicProperties** const GlobalObj = reinterpret_cast<GGL::CLogicProperties**>(0x85A3E0);

		STradeResource* GetResource(shok::ResourceType rt);

		static inline float(__stdcall* const ExperiencePointsToLevels)(float xp) = reinterpret_cast<float(__stdcall*)(float)>(0x4C674D); // -1->level 0, 0->lvl1...
	};
	//constexpr int i = offsetof(CLogicProperties, MilitaryBuildingAutoAttackRangeFactor) / 4;

	class CPlayerAttractionProps : public BB::IObject {
	public:
		int AttractionFrequency, PaydayFrequency, EntityTypeBanTime, ReAttachWorkerFrequency, PlayerMoneyDispo;
		float MaximumDistanceWorkerToFarm, MaximumDistanceWorkerToResidence;

		static inline constexpr int vtp = 0x770834;

		static inline GGL::CPlayerAttractionProps** const GlobalObj = reinterpret_cast<GGL::CPlayerAttractionProps**>(0x866A80);
	};

	class ExperienceClass {
	public:
		shok::String Table;

		struct LevelData { // size 9
			float DamageAmount;
			float DamageBonus;
			float HealingPoints;
			float DodgeChance;
			float AutoAttackRange;
			float MaxRange;
			float Speed;
			float Exploration;
			float MissChance; // without GGL::CEntityProfile::HookExperience only positive modifiers apply
			// all of them get used by profile modification
		};

		shok::Vector<LevelData> Level;

		LevelData* GetLevel(int lvl);

		void* operator new(size_t s);
		void operator delete(void* p);

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x87CE88);
	};
	static_assert(sizeof(ExperienceClass::LevelData) == 9 * 4);
	static_assert(sizeof(ExperienceClass) == 11 * 4);
	class ExperienceClassHolder {
	public:
		shok::Vector<ExperienceClass*> Classes;
		bool Loaded;

		ExperienceClass* GetClass(shok::ExperienceClass cl);

		static ExperienceClassHolder* GlobalObj(); // loads on first use

		void LoadExperienceClass(shok::ExperienceClass c);
		shok::ExperienceClass AddExperienceClass(const char* name, shok::EntityCategory cat);
		void PopExpeienceClass(shok::ExperienceClass c);

		struct EntityCategoryToExperienceClassData {
			shok::EntityCategory EntityCategory;
			shok::ExperienceClass ExperienceClass;
		};
		static std::vector<EntityCategoryToExperienceClassData> EntityCategoryToExperienceClass;
	};
}

namespace ED {
	class CDisplayProps {
	public:
		virtual ~CDisplayProps() = default;

		bool ShadowBlur;
		PADDING(3);
		unsigned int ShadowRasterSize;
		float ShadowStrength;
		shok::Color InvalidPositionColorModulate;
		shok::Color InvalidPositionColorEmissive;
		shok::Vector<shok::Color> PlayerColor;
		shok::Vector<shok::Color> MiniMapColor;
		float SelectionRadiusScaleForModelsWithDecal;
		int FogOfWarNeverSeenLuminance;
		int FogOfWarSeenLuminance;
		float AuraRadius;
		float AuraHeight;
		shok::String AuraTexture;
		shok::ModelId CommandAcknowledgementModel;
		int CommandAcknowledgementDuration;
		PADDINGI(1);

		static inline constexpr int vtp = 0x7AE630;

		static inline const BB::SerializationData* const SerializationData = reinterpret_cast<const BB::SerializationData*>(0xA1A320);
	};
	static_assert(sizeof(CDisplayProps) == 29 * 4);
}

namespace GGUI {
	class CGuiProperties : public BB::IObject {
	public:
		float ShortMessageDisplayTime, ShortMessageHistoryTime;

		static constexpr int vtp = 0x77BCA8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x700839B6);

		static inline CGuiProperties* (__cdecl* const GlobalObj)() = reinterpret_cast<CGuiProperties * (__cdecl*)()>(0x52EBC6);
	};
}

namespace CppLogic {
	class ExperienceClassManager {
		GGL::ExperienceClassHolder* Manager;

	public:
		inline ExperienceClassManager(GGL::ExperienceClassHolder* mng) : Manager(mng) {
		}

		// does not add
		inline shok::ExperienceClass GetIdByName(const char* name) const {
			for (int i = 0; i < static_cast<int>(Manager->Classes.size()); ++i) {
				if (Manager->Classes[i]->Table == name)
					return static_cast<shok::ExperienceClass>(i);
			}
			return shok::ExperienceClass::Invalid;
		}
		inline const char* GetNameByID(shok::ExperienceClass id) const {
			return Manager->Classes.at(static_cast<int>(id))->Table.c_str();
		}
		inline size_t size() const {
			return Manager->Classes.size();
		}

		struct Iter {
		protected:
			friend class ExperienceClassManager;
			int I;

			inline Iter(int i) : I(i) {}

		public:
			auto operator<=>(const Iter&) const noexcept = default;
			inline shok::ExperienceClass operator*() const noexcept {
				return static_cast<shok::ExperienceClass>(I);
			}
			inline Iter& operator++() {
				++I;
				return *this;
			}
			inline Iter operator++(int) {
				Iter r = *this;
				++(*this);
				return r;
			}
		};
		inline Iter begin() const {
			return Iter{ 0 };
		}
		inline Iter end() const {
			return Iter{ static_cast<int>(Manager->Classes.size()) };
		}
	};
}
template<>
inline auto CppLogic::GetIdManager<shok::ExperienceClass>() {
	auto mng = GGL::ExperienceClassHolder::GlobalObj();
	return ExperienceClassManager{mng};
}

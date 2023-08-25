#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

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
		float BonusVsArmorClass[7]; // remember to access with ArmorClass - 1
	public:
		float& GetBonusVsArmorClass(shok::ArmorClassId ac);

		static inline constexpr int vtp = 0x788978;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x520829DD);
	};
	class DamageClassesHolder {
	public:
		BB::CIDManagerEx* DamageClassManager;
		shok::Vector<GGL::CDamageClassProps*> DamageClassList; // there is a damageclass 0, probably not working at all

		GGL::CDamageClassProps* TryGet(shok::DamageClassId id);

		static inline GGL::DamageClassesHolder** const GlobalObj = reinterpret_cast<GGL::DamageClassesHolder**>(0x85A3DC);
	};

	class CLogicProperties : public BB::IObject {
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

		int CompensationOnBuildingSale;
		shok::Vector<SBuildingUpgradeCategory> BuildingUpgrades;
		shok::Vector<SSettlerUpgradeCategory> SettlerUpgrades;
		shok::Vector<STaxationLevel> TaxationLevels;
		shok::Vector<STradeResource> TradeResources;
		shok::Vector<SBlessCategory> BlessCategories; // 18
		float BuildingUnderConstructionExplorationFactor, BuildingPlacementSnapDistance, BuildingClosedHealthFactor; // 22
		float WeatherExplorationBuildingSnowFactor, WeatherExplorationSettlerSnowFactor; // 25
		float WeatherMoveSpeedSnowFactor, WeatherExplorationBuildingRainFactor, WeatherExplorationSettlerRainFactor;
		float WeatherMoveSpeedRainFactor; // 30
		int WeatherMissChanceChangeRain, WeatherMissChanceChangeSnow;
		float AttackMoveRange; // 33
		int TaxAmount;
		float TaxPenalty;
		int InitialTaxLevel; // 36
		shok::CostInfo BlessingCost; // per settler cost
		float BlessingBonus; // 55
		PADDINGI(1);
		int ForceToWorkPenalty;
		PADDINGI(1);
		int ResourceDoodadWarnAmount; // 59
		int FeedbackHandlerUpdateFrequency; // 60
		int FeedbackHandlerTimeStaysInSystem;
		float MilitaryLeaderAutoAttackRange;
		float MilitaryBuildingAutoAttackRangeFactor;
		float MilitaryCivilianAutoAttackRangeFactor;
		float MilitaryDefendRange;
		int MilitaryRecentlyAttackedTime;
		int MilitaryMaxAttackersPerEntity;
		float CivilianDefenseRadius;
		float MotivationThresholdHappy; // 69
		float MotivationThresholdSad, MotivationThresholdAngry, MotivationThresholdLeave, AverageMotivationVillageCenterLockThreshold;
		int WorkTimeBase, WorkTimeThresholdWork, WorkTimeThresholdFarm, WorkTimeThresholdResidence, WorkTimeThresholdCampFire;
		float MotivationGameStartMaxMotivation;
		float MotivationAbsoluteMaxMotivation; // 80
		int MotivationMillisecondsWithoutJob;
		int HeroResurrectionTime, HeroResurrectionSlippage;
		float HeroResurrectionRadius, HeroResurrectionHealthFactor, HeroResurrectionActionPointFactor;
		int HeroComatoseExploration;
		int BattleSerfMaxSeconds;
		shok::EffectTypeId ExpelEffectID; // 89
		int DefenderMSPerShot; // 90
		float DefenderMaxRange;
		shok::EffectTypeId DefenderProjectileEffectType;
		int DefenderProjectileDamage;
		shok::DamageClassId DefenderProjectileDamageClass;
		int DefenderMissChance; // 95
		float WorkerFlightDistance;
		int MaxExperiencePoints;
		int BuildingRecentlyAttackedDuration;
		float EnergyRequiredForWeatherChange; // 99
		int DefaultDurationOfWeatherChange;
		float EnergyDeductedFromOtherPlayers;
		float ConstructionSiteHealthFactor;
		float MaxFaith; // 103
		float GuardMaxDistanceBuilding;
		float GuardMoveDistanceBuilding;
		float GuardMaxDistanceOther;
		float GuardMoveDistanceOther;
		float NPCInteractionDistance;
		shok::AnimationId WeatherTowerAnim;
		int LeaderNudgeCount;
		float LeaderApproachRange;
		int AlarmRechargeTime;
		int OvertimeRechargeTimeInMs; // 113

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

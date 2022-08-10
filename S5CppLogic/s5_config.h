#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace EGL {
	class CGLEAnimProps : public BB::IObject {
	public:
		int Duration = 0; // ticks
		bool IsWalkOrRun = false;

		static inline constexpr int vtp = 0x788D7C;
		static inline constexpr unsigned int Identifier = 0x12FC4F78;

		CGLEAnimProps();
		CGLEAnimProps(const CGLEAnimProps& o);
		CGLEAnimProps(CGLEAnimProps&& o) noexcept;

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual unsigned int __stdcall GetClassIdentifier() const override;
	private:
		void SetVT(int vt);
	};
}

namespace GGL {
	class CDamageClassProps : public BB::IObject {
		float BonusVsArmorClass[7]; // remember to access with ArmorClass - 1
	public:
		float& GetBonusVsArmorClass(int ac);

		static inline constexpr int vtp = 0x788978;
	};
	class DamageClassesHolder {
	public:
		BB::CIDManagerEx* DamageClassManager;
		shok::Vector<GGL::CDamageClassProps*> DamageClassList; // there is a damageclass 0, probably not working at all

		static inline GGL::DamageClassesHolder** const GlobalObj = reinterpret_cast<GGL::DamageClassesHolder**>(0x85A3DC);
	};

	class CLogicProperties : public BB::IObject {
	public:
		struct SBuildingUpgradeCategory {
			int Category, FirstBuilding;
			int NumScholars;

			static inline constexpr int vtp = 0x76EF10;
			static inline constexpr unsigned int Identifier = 0x9498E0E7;
			virtual unsigned int GetClassIdentifier() const;

			SBuildingUpgradeCategory(int car, int first);
			SBuildingUpgradeCategory(SBuildingUpgradeCategory&& o) noexcept;
		};
		struct SSettlerUpgradeCategory {
			int Category, FirstSettler;

			static inline constexpr int vtp = 0x76EF18;
			static inline constexpr unsigned int Identifier = 0x8DC75427;
			virtual unsigned int GetClassIdentifier() const;

			SSettlerUpgradeCategory(int cat, int first);
			SSettlerUpgradeCategory(SSettlerUpgradeCategory&& o) noexcept;
		};
		struct STaxationLevel {
			int RegularTax;
			float MotivationChange;

			static inline constexpr int vtp = 0x76EF20;
			virtual unsigned int GetClassIdentifier() const = 0;
		};
		struct STradeResource {
			int ResourceType;
			float BasePrice, MinPrice, MaxPrice, Inflation, Deflation, WorkAmount;

			static inline constexpr int vtp = 0x76EF28;
			virtual unsigned int GetClassIdentifier() const = 0;
		};
		struct SBlessCategory {
			int Name;
			float RequiredFaith;
			shok::Vector<int> EntityTypes;

			static inline constexpr int vtp = 0x76EFC4;
			virtual unsigned int GetClassIdentifier() const = 0;
		};

		int CompensationOnBuildingSale;
		shok::Vector<SBuildingUpgradeCategory> BuildingUpgrades;
		shok::Vector<SSettlerUpgradeCategory> SettlerUpgrades;
		shok::Vector<STaxationLevel> TaxationLevels;
		shok::Vector<STradeResource> TradeResources;
		shok::Vector<SBlessCategory> BlessCategories; // 18
		float BuildingUnderConstructionExplorationFactor, BuildingPlacementSnapDistance, BuildingClosedHealthFactor, WeatherExplorationBuildingSnowFactor, WeatherExplorationSettlerSnowFactor;
		float WeatherMoveSpeedSnowFactor, WeatherExplorationBuildingRainFactor, WeatherExplorationSettlerRainFactor, WeatherMoveSpeedRainFactor;
		int WeatherMissChanceChangeRain, WeatherMissChanceChangeSnow;
		float AttackMoveRange; // 33
		int TaxAmount;
		float TaxPenalty;
		int InitialTaxLevel; // 36
		shok::CostInfo BlessingCost;
		float BlessingBonus; // 55
		PADDINGI(1);
		int ForceToWorkPenalty;
		PADDINGI(1);
		int ResourceDoodadWarnAmount; // 59
		int FeedbackHandlerUpdateFrequency;
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
		int ExpelEffectID, DefenderMSPerShot; // fi 89
		float DefenderMaxRange;
		int DefenderProjectileEffectType;
		int DefenderProjectileDamage;
		int DefenderProjectileDamageClass;
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
		int WeatherTowerAnim;
		int LeaderNudgeCount;
		float LeaderApproachRange;
		int AlarmRechargeTime;
		int OvertimeRechargeTimeInMs; // 113

		static inline constexpr int vtp = 0x76EFCC;

		static inline GGL::CLogicProperties** const GlobalObj = reinterpret_cast<GGL::CLogicProperties**>(0x85A3E0);

		static inline float(__stdcall* const ExperiencePointsToLevels)(float xp) = reinterpret_cast<float(__stdcall*)(float)>(0x4C674D); // -1->level 0, 0->lvl1...
	};

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
			float MissChance; // works for positive
			// all of them get used by profile modification, check there, if it actually gets used somewhere
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
		int CommandAcknowledgementModel;
		int CommandAcknowledgementDuration;
		PADDINGI(1);

		static inline constexpr int vtp = 0x7AE630;

		static inline const BB::SerializationData* const SerializationData = reinterpret_cast<const BB::SerializationData*>(0xA1A320);
	};
	static_assert(sizeof(CDisplayProps) == 29 * 4);
}

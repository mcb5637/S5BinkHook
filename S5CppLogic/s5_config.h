#pragma once
#include "s5data.h"

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
			PADDINGI(1);

			static inline constexpr int vtp = 0x76EF10;
			virtual unsigned int GetClassIdentifier() const = 0;
		};
		struct SSettlerUpgradeCategory {
			int Category, FirstSettler;

			static inline constexpr int vtp = 0x76EF18;
			virtual unsigned int GetClassIdentifier() const = 0;
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
	};

	class CPlayerAttractionProps : public BB::IObject {
	public:
		int AttractionFrequency, PaydayFrequency, EntityTypeBanTime, ReAttachWorkerFrequency, PlayerMoneyDispo;
		float MaximumDistanceWorkerToFarm, MaximumDistanceWorkerToResidence;

		static inline constexpr int vtp = 0x770834;

		static inline GGL::CPlayerAttractionProps** const GlobalObj = reinterpret_cast<GGL::CPlayerAttractionProps**>(0x866A80);
	};
}

namespace ED {
	class CDisplayProps {
	public:
		virtual ~CDisplayProps() = default;

		byte ShadowBlur;
		PADDING(3);
		int ShadowRasterSize;
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

		static inline constexpr int vtp = 0x7AE630;
	};
}

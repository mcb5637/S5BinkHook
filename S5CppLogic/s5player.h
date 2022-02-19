#pragma once
#include "s5data.h"

namespace GGL {
	class CSerfManager : public BB::IObject {
	public:
		// seems to be just playerid and 2 unknown values
		static inline constexpr int vtp = 0x76EA2C;
	};

	class CPlayerAttractionHandler : public BB::IObject {
	public:
		struct EntityInSystemData {
			int EntityID, GameTurn;
		};
		struct TypeThatLeftData {
			int EntityType, GameTurn;
		};
		struct TypeCountMapData {
			int EntityType, Count;
		};

		int PlayerID;
		byte PaydayActive;
		PADDING(3);
		int PaydayFirstOccuraceGameTurn;
		byte AIPlayerFlag, PayLeaderFlag;
		PADDING(2);
		PADDINGI(2); // maybe char pointer? GUI_WorkersWithoutSleepPlace m_GUI_WorkersWithoutEatPlace
		shok::Vector<EntityInSystemData> EntityInSystem;
		shok::Vector<int> HeadquarterArray;
		shok::Vector<int> VillageCenterArray;
		shok::Vector<int> WorkBuildingsArray;
		shok::Vector<int> ResidenceBuildingArray;
		shok::Vector<int> FarmBuildingArray;
		shok::Vector<int> BarrackBuildingArray;
		shok::Vector<int> FreeWorkerArray;
		shok::Vector<int> EmployedWorkerArray;
		shok::Vector<int> SoldierArray;
		shok::Vector<int> LeaderArray;
		shok::Vector<int> HeroArray;
		shok::Vector<int> SerfArray;
		shok::Vector<TypeThatLeftData> EntityTypeThatLeft;
		shok::Vector<TypeCountMapData> EntityTypeCountMap;

		static inline constexpr int vtp = 0x770868;

		int GetAttractionLimit();
		int GetAttractionUsage();
		int GetWorkerPaydayIncome();
		int GetNumberOfAttractedWorkers();
		float GetLeaderPaydayCost();
		int GetNumberOfLeaders();

		// checkpayday 4C25FB thiscall
		static void HookCheckPayday();
		static void (*OnCheckPayDayCallback)(GGL::CPlayerAttractionHandler* th);
	};

	class CUpgradeManager : public BB::IObject {
	public:
		struct Job {
			int Category;
			shok::Set<int[3]> UpgradeJobData; // UpgradeProgress, Category, UpgradeManager
		};
		struct UCatEntry {
			int UCat;
			int NumUpgrades;
			int FirstEntityType;
		};


		shok::Set<Job> JobData; // empty
		shok::Set<UCatEntry> UpgradeCategories;
		int PlayerID;

		static inline constexpr int vtp = 0x7728CC;

		int GetUpgradeCategoryOfEntityType(int etype);
	};
	class CBuildingUpgradeManager : public GGL::CUpgradeManager {
	public:
		shok::Set<int[3]> ScholarInfoElement; // Category, ScholarInfo { ?, CurrentAmount } ?

		static inline constexpr int vtp = 0x772948;
	};
	class CSettlerUpgradeManager : public GGL::CUpgradeManager {
	public:

		static inline constexpr int vtp = 0x772904;

		int GetSettlerTypeByUCat(int ucat);
	};

	class CTradeManager : public BB::IObject {
	public:
		struct ResData {
			int ResourceType;
			float CurrentPrice, CurrentInflation, CurrentDeflation;
			bool CanBeSold, CanBeBought;
		};


		int PlayerID;
		shok::Vector<ResData> TradeData;

		static inline constexpr int vtp = 0x772860;
	};

	struct PlayerTechManager { // size 12
		struct Tech {
			int TechStatus;
			int ResearchProgress;
			int StartTick;
			int ResearcherId;
		};


		int PlayerID;
		shok::Vector<Tech> TechnologyState;
		PADDINGI(4); // vector<?> TechnologyInProcess
		PADDINGI(3);
	};
	static_assert(sizeof(PlayerTechManager) == 12 * 4);

	struct PlayerTributesManager {
		struct Tribute {
			int UniqueTributeID;
			shok::CostInfo Costs;
			int OwnerEntityID;
			int OfferingPlayerID;
			shok::String OfferStringTableKey;
		};

		int PlayerID;
		shok::Vector<Tribute> Tributes;
	};
	static_assert(sizeof(PlayerTributesManager) == 5 * 4);

	struct PlayerQuestManager {
		struct Quest {
			int UniqueQuestID;
			int QuestType;
			shok::String QuestNameStringTableKey, QuestDescriptionStringTableKey;
			int SubQuestDoneFlagArray[6]; // type?
		};


		int PlayerID;
		shok::List<Quest> Quests;
	};

	struct GameStatisticsTimeline {
		shok::Vector<int> Amounts;
		int PlayerID;
		int LastGatherTurn;
	};
	class CResourceStatistics {
	public:
		struct ResData {
			int ResourceType;
			int AmountMined;
			int AmountRefined;
			GGL::GameStatisticsTimeline TimeLine;
		};

		shok::Vector<ResData> Data;
		PADDINGI(1);

		virtual unsigned int __stdcall GetClassIdentifier() const;

		static inline constexpr int vtp = 0x76E0D8;
	};
	class CGameStatistics {
	public:
		struct TechResearchData {
			int TechnologyType, Time;
		};
		struct BuildingUpgradedData {
			int BuildingType, Time;
		};

		int PlayerID;
		int NumberOfUnitsKilled, NumberOfUnitsDied, NumberOfBuildingsDestroyed, NumberOfBuildingsLost;
		GGL::CResourceStatistics ResourceStatistics; //6
		GGL::GameStatisticsTimeline UnitsKilledTimeLine, UnitsDiedTimeLine, BuildingsDestroyedTimeLine, BuildingsLostTimeLine, // 12, 18
			SerfTimeLine, WorkerTimeLine, LeaderTimeLine, HeroTimeLine, SoldierTimeLine, FarmTimeLine, ResidenceTimeLine,
			WorkplaceTimeLine, MilitaryBuildingTimeLine, VillageCenterTimeLine, BuildingTimeLine, MotivationTimeLine; // la 102, 96, 90
		shok::Vector<TechResearchData> ResearchedTechnologies;
		shok::Vector<BuildingUpgradedData> UpgradedBuildings;

		virtual unsigned int __stdcall GetClassIdentifier() const;

		static inline constexpr int vtp = 0x76E0E0;
	};
	static_assert(sizeof(GGL::CGameStatistics) == 116 * 4);

	class CPlayerStatus : public BB::IObject {
	public:
		int PlayerGameState;
		byte PlayerHasLeftGameFlag;
		PADDING(3);
		int PlayerGameStateChangeGameTurn;
		byte PlayerIsHumanFlag;
		PADDING(3);
		int PlayerColorR, PlayerColorG, PlayerColorB;
		int PlayerID;
		shok::String PlayerNameStringTableKey, PlayerNameStringRaw;
		shok::CostInfo CurrentResources; // 23
		float TaxAmountFactor;
		int TaxLevel; // 42
		float CurrentMaxMotivation;
		int DurationOfWeatherChange; // seems to be always 0
		int NumberOfBuyableHeros;
		byte WorkerAlarmMode;
		PADDING(3);
		int AlarmRechargeTime;
		int DiplomacyData[11]; // 48
		GGL::PlayerTechManager TechnologyStates; // 59
		GGL::PlayerTributesManager Tributes; // 71
		GGL::PlayerQuestManager Quests; // 76
		GGL::CGameStatistics Statistics; // 80
		GGL::CSerfManager* SerfManager;
		GGL::CPlayerAttractionHandler* PlayerAttractionHandler; // 197
		GGL::CBuildingUpgradeManager* BuildingUpgradeManager;
		GGL::CSettlerUpgradeManager* SettlerUpgradeManager;
		GGL::CTradeManager* TradeManager;

		static inline constexpr int vtp = 0x76FA88;

		shok::DiploState GetDiploStateTo(int p);
		shok::TechState GetTechStatus(int tech);


		static bool ArePlayersHostile(int p1, int p2);
		static bool ArePlayersFriendly(int p1, int p2);

		static inline bool(_cdecl* const CanPlaceBuilding)(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId) = reinterpret_cast<bool(_cdecl*)(int, int, shok::Position*, float, int)>(0x4B442C);
		static inline bool(_cdecl* const CanPlaceBuildingAtPos)(int entitytype, int player, shok::Position* pos, float rotation) = reinterpret_cast<bool(_cdecl*)(int, int, shok::Position*, float)>(0x4B45C8);

		static bool (*CanPlaceBuildingCallback)(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId);
		static void HookCanPlaceBuilding();

	};

}

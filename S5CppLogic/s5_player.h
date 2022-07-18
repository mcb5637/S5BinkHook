#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entitytype.h"

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
		bool PaydayActive;
		PADDING(3);
		int PaydayFirstOccuraceGameTurn;
		bool AIPlayerFlag, PayLeaderFlag;
		PADDING(2);
		PADDINGI(2); // maybe char pointer? GUI_WorkersWithoutSleepPlace m_GUI_WorkersWithoutEatPlace
		shok::Vector<EntityInSystemData> EntityInSystem;
		shok::Vector<int> HeadquarterArray;
		shok::Vector<int> VillageCenterArray;
		shok::Vector<int> WorkBuildingsArray;
		shok::Vector<int> ResidenceBuildingArray; // 23
		shok::Vector<int> FarmBuildingArray; // 27
		shok::Vector<int> BarrackBuildingArray;
		shok::Vector<int> FreeWorkerArray; // 35
		shok::Vector<int> EmployedWorkerArray; // 39
		shok::Vector<int> SoldierArray; // 43
		shok::Vector<int> LeaderArray; // 47
		shok::Vector<int> HeroArray;
		shok::Vector<int> SerfArray;
		shok::Vector<TypeThatLeftData> EntityTypeThatLeft;
		shok::Vector<TypeCountMapData> EntityTypeCountMap;

		static inline constexpr int vtp = 0x770868;

		int GetAttractionLimit();
		int GetAttractionUsage();
		// taxes from workers
		int GetWorkerPaydayIncome();
		int GetNumberOfAttractedWorkers();
		float GetLeaderPaydayCost();
		int GetNumberOfLeaders();
		// checks VC capactity and motivation. always true if AIPlayerFlag set.
		bool IsAttractionsSlotAvailable();
		int GetNumberOfSettlers();
		int GetNumberOfMilitaryUnits();
		// in 1/100 ticks
		int GetTimeToNextPayday();
		float GetAverageMotivation();

		int GetSleepPlaceLimit();
		int GetFreeSleepPlaces();
		int GetUsedSleepPlaces();
		int GetNumberOfWorkersWithoutSleepPlace();

		int GetFarmPlaceLimit();
		int GetFreeFarmPlace();
		int GetUsedFarmPlaces();
		int GetNumberOfWorkersWithoutFarmPlace();

		void CheckWorkerAttachment(bool forceReAttach);

		// checkpayday 4C25FB thiscall
		static void HookCheckPayday();
		static void (*OnCheckPayDayCallback)(GGL::CPlayerAttractionHandler* th);
	};
	//constexpr int i = offsetof(CPlayerAttractionHandler, EmployedWorkerArray) / 4;

	class CUpgradeManager : public BB::IObject {
	public:
		virtual EGL::CGLEEntityProps::UpgradeInfo* GetUpgradeInfo(int ety) = 0;
		virtual void UpgradeEntity(int id) = 0;
		// 2 more empty funcs

		struct JobDataEntry {
			float UpgradeProgress;
			int Category; //ucat
			BB::IObject* UpgradeManager;
		};
		struct UCatEntry {
			int CurrentLevel;
			int Level0TypeID;
		};

		shok::Map<int, JobDataEntry> JobData; // Category -> UpgradeJobData empty
		shok::Map<int, UCatEntry> UpgradeCategories; // Category -> Info gets filled by player ctor
		int PlayerID;

		static inline constexpr int vtp = 0x7728CC;

		int GetUpgradeCategoryOfEntityType(int etype);
		int GetTypeByUCat(int ucat);
	};
	class CBuildingUpgradeManager : public GGL::CUpgradeManager {
	public:
		struct ScholarInfo {
			int MaxAmount;
			int CurrentAmount;
		};

		shok::Map<int, ScholarInfo> ScholarInfoElement; // Category -> ScholarInfo gets filled by player ctor

		static inline constexpr unsigned int Identifier = 0x6987C1B3;
		static inline constexpr int vtp = 0x772948;

		void AddCategory(int ucat, int firstEntity, int maxscholar);
	};
	static_assert(sizeof(CBuildingUpgradeManager) == 11 * 4);
	class CSettlerUpgradeManager : public GGL::CUpgradeManager {
	public:

		static inline constexpr unsigned int Identifier = 0x126CFE03;
		static inline constexpr int vtp = 0x772904;

		void AddCategory(int ucat, int firstEntity);
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

		struct TradeOrder {
			int Player;
			shok::ResourceType SellResourceType, BuyResourceType;
			float BuyAmount, SellAmount, ProgressAmount; // prog max is buyam * workamount + sellam * workamount

			// calculates price
			void SetData(int player, shok::ResourceType sellTy, shok::ResourceType buyTy, float buyAm);
			// checks resource count and CanBeSold+CanBeBought flags
			// does not get used by normal market behavior
			bool IsTradeValid();
			float GetProgressMax();
			bool IsFinished();
			bool RemoveSellRes();
			void Complete(bool removeSell);
			// calls complete(false) if progress reaches max
			void AdvanceProgress(float p);
		};

		void ApplyPriceChangesOfTrade(const TradeOrder* o);
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
			int OwnerEntityID; // 19
			int OfferingPlayerID;
			shok::String OfferStringTableKey;
		};

		int PlayerID;
		shok::Vector<Tribute> Tributes;

		// creates tribute or overrides it, if it has an existing id
		void SetTributeData(int tid, const shok::CostInfo& c, int ownerEntityId, int offeringPlayerId, const char* text);
		// returns if it was there
		bool RemoveTribute(int tid);
	};
	static_assert(sizeof(PlayerTributesManager) == 5 * 4);
	//constexpr int i = offsetof(PlayerTributesManager::Tribute, OwnerEntityID) / 4;

	struct PlayerQuestManager {
		enum class QuestStatus : int {
			MainQuest_Open = 1,
			MainQuest_Closed = 2,
			SubQuest_Open = 11,
			SubQuest_Closed = 12,
		};

		struct Quest {
			int UniqueQuestID;
			QuestStatus QuestType;
			shok::String QuestNameStringTableKey, QuestDescriptionStringTableKey;
			shok::Position Pos; // not serialized
			bool SubQuestDoneFlagArray[20]; // not sure about the size and the full layout
		};


		int PlayerID;
		shok::List<Quest> Quests;

		// pos can be nullptr, creates quest or overrides it
		void SetQuestData(int questId, QuestStatus questType, const char* name, const char* text, const shok::Position* pos, bool info);
		void SetQuestType(int questId, QuestStatus questType, bool info);
		// pos can be nullptr
		void SetQuestPosition(int questId, const shok::Position* pos, bool info);
		// returns if it was there
		bool RemoveQuest(int questId, bool info);
	};
	static_assert(sizeof(PlayerQuestManager::Quest) == 23 * 4);
	//constexpr int i = offsetof(PlayerQuestManager::Quest, SubQuestDoneFlagArray) / 4;

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
		enum class PlayerStatus : int {
			Playing = 1,
			Won = 2,
			Lost = 3,
			Left = 4,
		};

		int PlayerGameState;
		bool PlayerHasLeftGameFlag;
		PADDING(3);
		int PlayerGameStateChangeGameTurn;
		bool PlayerIsHumanFlag;
		PADDING(3);
		int PlayerColorR, PlayerColorG, PlayerColorB;
		int PlayerID;
		shok::String PlayerNameStringTableKey, PlayerNameStringRaw;
		shok::CostInfo CurrentResources; // 23
		float TaxAmountFactor; // used only for levytax
		int TaxLevel; // 42
		float CurrentMaxMotivation;
		int DurationOfWeatherChange; // seems to be always 0
		int NumberOfBuyableHeros;
		bool WorkerAlarmMode;
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
		GGL::CTradeManager* TradeManager; // 200

		static inline constexpr int vtp = 0x76FA88;

		shok::DiploState GetDiploStateTo(int p);
		shok::TechState GetTechStatus(int tech);

		int GetTaxPerWorker();
		int GetLevyTaxPerWorker();
		int GetLevyTaxAmount();

		// returns if successful
		bool SetState(PlayerStatus s);


		static bool ArePlayersHostile(int p1, int p2);
		static bool ArePlayersFriendly(int p1, int p2);

		static inline bool(_cdecl* const CanPlaceBuilding)(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId) = reinterpret_cast<bool(_cdecl*)(int, int, shok::Position*, float, int)>(0x4B442C);
		static inline bool(_cdecl* const CanPlaceBuildingAtPos)(int entitytype, int player, shok::Position* pos, float rotation) = reinterpret_cast<bool(_cdecl*)(int, int, shok::Position*, float)>(0x4B45C8);

		static bool (*CanPlaceBuildingCallback)(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId);
		static void HookCanPlaceBuilding();

	};
	//constexpr int i = offsetof(CPlayerStatus, CurrentResources) / 4;

}

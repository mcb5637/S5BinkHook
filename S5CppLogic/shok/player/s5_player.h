#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entitytype/s5_entitytype.h>

namespace GGL {
	class CSerfManager : public BB::IObject {
	public:
		// seems to be just playerid and 2 unknown values
		static inline constexpr int vtp = 0x76EA2C;
	};

	class CPlayerAttractionHandler : public BB::IObject {
	public:
		struct EntityInSystemData {
			shok::EntityId EntityID;
			int GameTurn;
		};
		struct TypeThatLeftData {
			shok::EntityTypeId EntityType;
			int GameTurn;
		};

		shok::PlayerId PlayerID;
		bool PaydayActive;
		PADDING(3);
		int PaydayFirstOccuraceGameTurn;
		bool AIPlayerFlag, PayLeaderFlag;
		PADDING(2);
		int GUI_WorkersWithoutSleepPlace, m_GUI_WorkersWithoutEatPlace; // just the number of workers, dont ask me why there is this m_
		shok::Vector<EntityInSystemData> EntityInSystem; // 7
		shok::Vector<shok::EntityId> HeadquarterArray; // 11
		shok::Vector<shok::EntityId> VillageCenterArray; // 15
		shok::Vector<shok::EntityId> WorkBuildingsArray; // 19
		shok::Vector<shok::EntityId> ResidenceBuildingArray; // 23
		shok::Vector<shok::EntityId> FarmBuildingArray; // 27
		shok::Vector<shok::EntityId> BarrackBuildingArray; // 31
		shok::Vector<shok::EntityId> FreeWorkerArray; // 35
		shok::Vector<shok::EntityId> EmployedWorkerArray; // 39
		shok::Vector<shok::EntityId> SoldierArray; // 43
		shok::Vector<shok::EntityId> LeaderArray; // 47
		shok::Vector<shok::EntityId> HeroArray; // 51
		shok::Vector<shok::EntityId> SerfArray; // 55
		shok::Map<shok::EntityTypeId, int> EntityTypeCountMap; // 59 EntityType->Count
		shok::List<TypeThatLeftData> EntityTypeThatLeft; // 62

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
		bool IsMotivationLocked();
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
		bool AttachWorker(shok::EntityId worker, shok::EntityId building);
		shok::EntityId PerformSpawnWorker(GGL::CBuilding* workplace, GGL::CBuilding* spawner);

		[[nodiscard]] int GetAttractionUsageOfVector(const shok::Vector<shok::EntityId>& v) const;

		// checkpayday 4C25FB thiscall
		static void HookCheckPayday();
		static void (*OnCheckPayDayCallback)(GGL::CPlayerAttractionHandler* th);

		static void HookWorkerSpawn();
		static bool AttractionCannonInProgress;
		static void HookAttractionCannonInProgress();

		// on entity created 0x4C38FE __thiscall(id)

		// attraction of vector 0x4C2335 __thiscall(shok::Vector<shok::EntityId>*)

		// update() 0x4C4A09
		// checkWorkerSpawn() 0x4C47FB
		// getWorkplacesToSpawnWorker(shok::List<Data>*) 0x4C46BB
		// get nearest reachable(shok::Vector<shok::EntityId>*, shok::Position*) 0x4C205C

	private:
		static void __thiscall CheckPaydayHook(CPlayerAttractionHandler* th);
		static void NAKED_DECL CheckPaydayHookASM();
		void __thiscall CheckWorkerSpawnHook();
		static int __thiscall CannonsInProgressAttraction(const CPlayerAttractionHandler* th);
		static void NAKED_DECL CannonsInProgressAttractionASM();
	};
	//constexpr int i = offsetof(CPlayerAttractionHandler, EntityTypeCountMap) / 4;

	class CUpgradeManager : public BB::IObject {
	public:
		virtual EGL::CGLEEntityProps::UpgradeInfo* GetUpgradeInfo(shok::EntityTypeId ety) = 0;
		virtual void UpgradeEntity(shok::EntityId id) = 0;
		// 2 more empty funcs

		struct JobDataEntry {
			float UpgradeProgress;
			shok::UpgradeCategoryId Category; //ucat
			BB::IObject* UpgradeManager;
		};
		struct UCatEntry {
			int CurrentLevel;
			shok::EntityTypeId Level0TypeID;
		};

		shok::Map<shok::UpgradeCategoryId, JobDataEntry> JobData; // Category -> UpgradeJobData empty
		shok::Map<shok::UpgradeCategoryId, UCatEntry> UpgradeCategories; // Category -> Info gets filled by player ctor
		shok::PlayerId PlayerID;

		static inline constexpr int vtp = 0x7728CC;

		shok::UpgradeCategoryId GetUpgradeCategoryOfEntityType(shok::EntityTypeId etype);
		shok::EntityTypeId GetTypeByUCat(shok::UpgradeCategoryId ucat);
	};
	class CBuildingUpgradeManager : public GGL::CUpgradeManager {
	public:
		struct ScholarInfo {
			int MaxAmount;
			int CurrentAmount;
		};

		shok::Map<shok::UpgradeCategoryId, ScholarInfo> ScholarInfoElement; // Category -> ScholarInfo gets filled by player ctor

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x6987C1B3);
		static inline constexpr int vtp = 0x772948;

		void AddCategory(shok::UpgradeCategoryId ucat, shok::EntityTypeId firstEntity, int maxscholar);
	};
	static_assert(sizeof(CBuildingUpgradeManager) == 11 * 4);
	class CSettlerUpgradeManager : public GGL::CUpgradeManager {
	public:

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x126CFE03);
		static inline constexpr int vtp = 0x772904;

		void AddCategory(shok::UpgradeCategoryId ucat, shok::EntityTypeId firstEntity);
	};

	class CTradeManager : public BB::IObject {
	public:
		struct ResData {
			shok::ResourceType ResourceType;
			float CurrentPrice, CurrentInflation, CurrentDeflation;
			bool CanBeSold, CanBeBought;

			// adjusts one price (except gold, hardcoded) respects min/max in GGL::CLogicProperties::GlobalObj
			void AdjustPrice(float amount, bool buyed);
		};


		shok::PlayerId PlayerID;
		shok::Vector<ResData> TradeData;

		static inline constexpr int vtp = 0x772860;

		struct TradeOrder {
			shok::PlayerId Player = {};
			shok::ResourceType SellResourceType = {}, BuyResourceType = {};
			float BuyAmount = 0.0f, SellAmount = 0.0f, ProgressAmount = 0.0f; // prog max is buyam * workamount + sellam * workamount

			// calculates price
			void SetData(shok::PlayerId player, shok::ResourceType sellTy, shok::ResourceType buyTy, float buyAm);
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
		ResData* GetResource(shok::ResourceType rt);
		// 0x4C8942 float __thiscall GetPrice(rty)
		float GetRelativePrice(shok::ResourceType buy, shok::ResourceType sell);
	};

	struct PlayerTechManager;
	// checks for TechState::Researched (4)
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CTechConditionPredicate {
	public:
		virtual bool CheckTech(PlayerTechManager* mng, shok::Technology techId) const = 0;

		CTechConditionPredicate();

		static constexpr int vtp = 0x766B3C;
	};
	// checks if all own requirements fulfill CTechConditionPredicate
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CPreConditionPredicate : public CTechConditionPredicate {
	public:
		CPreConditionPredicate();

		static constexpr int vtp = 0x76E0F4;
	};

	struct PlayerTechManager { // size 12
		struct Tech {
			shok::TechState TechStatus; // dos not contain Waiting or Future
			int ResearchProgress;
			int StartTick;
			shok::EntityId ResearcherId;
		};

		shok::PlayerId PlayerID; // 0
		shok::Vector<Tech> TechnologyState; // 1 indexed by tech id
		shok::Vector<shok::TechnologyId> TechnologyInProgress; // 5 only AutomaticResearch==true
		shok::List<BB::TSlot1<const GGL::CNetEventEventTechnologyPlayerIDAndEntityID&>> OnResearched; // 9

		// just adds progress and sets tech state if done, returns tech done, amount * 1000
		bool AddTechProgressRaw(shok::TechnologyId techId, float amount);
		void TechResearched(shok::TechnologyId techId, shok::EntityId researcherId);
		void AddTechProgressWorker(shok::TechnologyId techId, float amount);
		bool CheckTechnologyRequirementsFor(shok::TechnologyId tech, const CTechConditionPredicate& p);
		bool CheckAllRequirementsFor(shok::TechnologyId tech, const CTechConditionPredicate& p);
		shok::TechState GetTechState(shok::TechnologyId tech); // may also return substates of allowed
		void ForceResearch(shok::TechnologyId tech);
		// on update 0x4A1B42 __thiscall()
		// start research 0x4A29DD __thiscall(techId, researcherId) only to be called by building::startResearch

		void ForceResearchNoFeedback(shok::TechnologyId tech);
	};
	static_assert(sizeof(PlayerTechManager) == 12 * 4);

	struct PlayerTributesManager {
		struct Tribute {
			int UniqueTributeID;
			shok::CostInfo Costs;
			shok::EntityId OwnerEntityID; // 19
			shok::PlayerId OfferingPlayerID;
			shok::String OfferStringTableKey;
		};

		shok::PlayerId PlayerID;
		shok::Vector<Tribute> Tributes;

		// creates tribute or overrides it, if it has an existing id
		void SetTributeData(int tid, const shok::CostInfo& c, shok::EntityId ownerEntityId, shok::PlayerId offeringPlayerId, const char* text);
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
			shok::Array<bool, 20> SubQuestDoneFlagArray; // not sure about the size and the full layout
		};


		shok::PlayerId PlayerID;
		shok::List<Quest> Quests;

		// pos can be nullptr, creates quest or overrides it
		void SetQuestData(int questId, QuestStatus questType, const char* name, const char* text, const shok::Position* pos, bool info);
		void SetQuestType(int questId, QuestStatus questType, bool info);
		// pos can be nullptr
		void SetQuestPosition(int questId, const shok::Position* pos, bool info);
		// returns if it was there
		bool RemoveQuest(int questId, bool info);
	};
	static_assert(sizeof(PlayerQuestManager::Quest) == 24 * 4);
	//constexpr int i = offsetof(PlayerQuestManager::Quest, SubQuestDoneFlagArray) / 4;

	struct GameStatisticsTimeline {
		shok::Vector<int> Amounts;
		shok::PlayerId PlayerID;
		int LastGatherTurn;
	};
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CResourceStatistics {
	public:
		struct ResData {
			shok::ResourceType ResourceType{};
			int AmountMined = 0;
			int AmountRefined = 0;
			GGL::GameStatisticsTimeline TimeLine;
		};

		shok::Vector<ResData> Data;
		PADDINGI(1);

		[[nodiscard]] virtual unsigned int __stdcall GetClassIdentifier() const;

		static inline constexpr int vtp = 0x76E0D8;
	};
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CGameStatistics {
	public:
		struct TechResearchData {
			shok::TechnologyId TechnologyType;
			int Time;
		};
		struct BuildingUpgradedData {
			shok::EntityTypeId BuildingType;
			int Time;
		};

		shok::PlayerId PlayerID;
		int NumberOfUnitsKilled, NumberOfUnitsDied, NumberOfBuildingsDestroyed, NumberOfBuildingsLost;
		GGL::CResourceStatistics ResourceStatistics; //6
		GGL::GameStatisticsTimeline UnitsKilledTimeLine, UnitsDiedTimeLine, BuildingsDestroyedTimeLine, BuildingsLostTimeLine, // 12, 18, 24, 30
			SerfTimeLine, WorkerTimeLine, LeaderTimeLine, HeroTimeLine, SoldierTimeLine, FarmTimeLine, ResidenceTimeLine, // 36, 42, 48, 54, 60, 66, 72
			WorkplaceTimeLine, MilitaryBuildingTimeLine, VillageCenterTimeLine, BuildingTimeLine, MotivationTimeLine; // 78, 84, 90, 96, 102
		shok::Vector<TechResearchData> ResearchedTechnologies;
		shok::Vector<BuildingUpgradedData> UpgradedBuildings;

		[[nodiscard]] virtual unsigned int __stdcall GetClassIdentifier() const;

		static inline constexpr int vtp = 0x76E0E0;

		void AddTechResearched(shok::TechnologyId tech);
		void OnResRefined(shok::ResourceType rt, float am);
		void OnResMined(shok::ResourceType rt, float am); // use normal res type, not raw
	};
	static_assert(sizeof(GGL::CGameStatistics) == 116 * 4);
	//constexpr int i = offsetof(CGameStatistics, MotivationTimeLine) / 4;

	class PlayerDiplomacyManager {
		shok::PlayerId PlayerID{};
		shok::Array<shok::DiploState, 9> DiplomacyState;
	};

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
		shok::PlayerId PlayerID;
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
		PlayerDiplomacyManager DiplomacyData; // 48
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

		shok::DiploState GetDiploStateTo(shok::PlayerId p);
		shok::TechState GetTechStatus(shok::TechnologyId tech);

		int GetTaxPerWorker();
		int GetLevyTaxPerWorker();
		int GetLevyTaxAmount();
		float GetMaxMotivation(); // checks MotivationAbsoluteMaxMotivation
		void ChangeMotivationForAllWorkers(float delta, shok::WorkerReason reason);
		void ChangeMaxMotivation(float delta); // forwards to ChangeMotivationForAllWorkers with max(delta, 0) and WorkerReason::None

		// returns if successful
		bool SetState(PlayerStatus s);

		bool HasResourcesFeedback(const shok::CostInfo& c, bool feedback = true);

		// ctor 4B6F6B

		static bool ArePlayersHostile(shok::PlayerId p1, shok::PlayerId p2);
		static bool ArePlayersFriendly(shok::PlayerId p1, shok::PlayerId p2);

		static inline bool(_cdecl* const CanPlaceBuilding)(shok::EntityTypeId entitytype, shok::PlayerId player, shok::Position* pos, float rotation, shok::EntityId buildOnId) = reinterpret_cast<bool(_cdecl*)(shok::EntityTypeId, shok::PlayerId, shok::Position*, float, shok::EntityId)>(0x4B442C);
		static inline bool(_cdecl* const CanPlaceBuildingAtPos)(shok::EntityTypeId entitytype, shok::PlayerId player, shok::Position* pos, float rotation) = reinterpret_cast<bool(_cdecl*)(shok::EntityTypeId, shok::PlayerId, shok::Position*, float)>(0x4B45C8);

		static bool (*CanPlaceBuildingCallback)(shok::EntityTypeId entitytype, shok::PlayerId player, shok::Position* pos, float rotation, shok::EntityId buildOnId);
		static void HookCanPlaceBuilding();

	private:
		static int __stdcall CanPlaceBuildingHook(shok::EntityTypeId entitytype, shok::PlayerId player, shok::Position* pos, float rotation, shok::EntityId buildOnId);
		static void NAKED_DECL CanPlaceBuildingHookASM();
	};
	static_assert(sizeof(CPlayerStatus) == 804);
	//constexpr int i = offsetof(CPlayerStatus, CurrentResources) / 4;

}

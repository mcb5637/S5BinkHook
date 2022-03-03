#pragma once
#include "s5_forwardDecls.h"
#include "s5_events.h"

namespace EGL {
	class CNetEvent2Entities : public BB::CEvent {
	public:
		int ActorId, TargetId;

		CNetEvent2Entities(shok::NetEventIds id, int actor, int tar);
		CNetEvent2Entities(CNetEvent2Entities&&) = default;
		CNetEvent2Entities(const CNetEvent2Entities&) = default;
		CNetEvent2Entities& operator=(CNetEvent2Entities&&) = default;
		CNetEvent2Entities& operator=(const CNetEvent2Entities&) = default;

		static inline constexpr int vtp = 0x76DD60;
		static inline constexpr int TypeDesc = 0x80FF1C;
	};

	class CNetEventEntityAndPos : public BB::CEvent {
	public:
		int EntityId;
		shok::Position Position;

		CNetEventEntityAndPos(shok::NetEventIds id, int ent, const shok::Position& pos);
		CNetEventEntityAndPos(CNetEventEntityAndPos&&) = default;
		CNetEventEntityAndPos(const CNetEventEntityAndPos&) = default;
		CNetEventEntityAndPos& operator=(CNetEventEntityAndPos&&) = default;
		CNetEventEntityAndPos& operator=(const CNetEventEntityAndPos&) = default;

		static inline constexpr int vtp = 0x76DD50;
		static inline constexpr int TypeDesc = 0x80FEF4;
	};

	class CNetEventEntityAndPosArray : public BB::CEvent {
	public:
		int EntityId;
		shok::Vector<shok::Position> Positions;
		float Rotation;

		EGL::CNetEventEntityAndPosArray(shok::NetEventIds id, int ent, float r);
		EGL::CNetEventEntityAndPosArray(const EGL::CNetEventEntityAndPosArray&) = default;
		EGL::CNetEventEntityAndPosArray& operator=(const EGL::CNetEventEntityAndPosArray&) = default;

		static inline constexpr int vtp = 0x770704;
		static inline constexpr int TypeDesc = 0x8125EC;
	};

	class CNetEventEntityID : public BB::CEvent {
	public:
		int EntityId;

		CNetEventEntityID(shok::NetEventIds id, int ent);
		CNetEventEntityID(shok::FeedbackEventIds id, int ent);
		CNetEventEntityID(CNetEventEntityID&&) = default;
		CNetEventEntityID(const CNetEventEntityID&) = default;
		CNetEventEntityID& operator=(CNetEventEntityID&&) = default;
		CNetEventEntityID& operator=(const CNetEventEntityID&) = default;

		static inline constexpr int vtp = 0x766C28;
		static inline constexpr int TypeDesc = 0x807924;
	};

	class CNetEventEntityIDAndInteger : public EGL::CNetEventEntityID {
	public:
		int Data;

		CNetEventEntityIDAndInteger(shok::NetEventIds id, int ent, int d);
		CNetEventEntityIDAndInteger(shok::FeedbackEventIds id, int ent, int d);
		CNetEventEntityIDAndInteger(CNetEventEntityIDAndInteger&&) = default;
		CNetEventEntityIDAndInteger(const CNetEventEntityIDAndInteger&) = default;
		CNetEventEntityIDAndInteger& operator=(CNetEventEntityIDAndInteger&&) = default;
		CNetEventEntityIDAndInteger& operator=(const CNetEventEntityIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x807978;
		static inline constexpr int vtp = 0x766C48;
	};

	class CNetEventPlayerID : public BB::CEvent {
	public:
		int PlayerId;

		CNetEventPlayerID(shok::NetEventIds id, int pl);
		CNetEventPlayerID(shok::FeedbackEventIds id, int pl);
		CNetEventPlayerID(CNetEventPlayerID&&) = default;
		CNetEventPlayerID(const CNetEventPlayerID&) = default;
		CNetEventPlayerID& operator=(CNetEventPlayerID&&) = default;
		CNetEventPlayerID& operator=(const CNetEventPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x807900;
		static inline constexpr int vtp = 0x766C18;
	};

	class CNetEventIntegerAndPlayerID : public EGL::CNetEventPlayerID {
	public:
		int Data;

		CNetEventIntegerAndPlayerID(shok::NetEventIds id, int pl, int d);
		CNetEventIntegerAndPlayerID(CNetEventIntegerAndPlayerID&&) = default;
		CNetEventIntegerAndPlayerID(const CNetEventIntegerAndPlayerID&) = default;
		CNetEventIntegerAndPlayerID& operator=(CNetEventIntegerAndPlayerID&&) = default;
		CNetEventIntegerAndPlayerID& operator=(const CNetEventIntegerAndPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x812404;
		static inline constexpr int vtp = 0x7705BC;
	};

	class CNetEventPlayerIDAndInteger : public EGL::CNetEventPlayerID {
	public:
		int Data;

		CNetEventPlayerIDAndInteger(shok::NetEventIds id, int pl, int d);
		CNetEventPlayerIDAndInteger(CNetEventPlayerIDAndInteger&&) = default;
		CNetEventPlayerIDAndInteger(const CNetEventPlayerIDAndInteger&) = default;
		CNetEventPlayerIDAndInteger& operator=(CNetEventPlayerIDAndInteger&&) = default;
		CNetEventPlayerIDAndInteger& operator=(const CNetEventPlayerIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x812434;
		static inline constexpr int vtp = 0x7705CC;
	};

	class CNetEventEntityIDAndPlayerID : public EGL::CNetEventPlayerID {
	public:
		int EntityId;

		CNetEventEntityIDAndPlayerID(shok::NetEventIds id, int pl, int e);
		CNetEventEntityIDAndPlayerID(shok::FeedbackEventIds id, int pl, int e);
		CNetEventEntityIDAndPlayerID(CNetEventEntityIDAndPlayerID&&) = default;
		CNetEventEntityIDAndPlayerID(const CNetEventEntityIDAndPlayerID&) = default;
		CNetEventEntityIDAndPlayerID& operator=(CNetEventEntityIDAndPlayerID&&) = default;
		CNetEventEntityIDAndPlayerID& operator=(const CNetEventEntityIDAndPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x807948;
		static inline constexpr int vtp = 0x766C38;
	};

	class CNetEventEntityIDAndPlayerIDAndEntityType : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		int EntityType;

		CNetEventEntityIDAndPlayerIDAndEntityType(shok::NetEventIds id, int pl, int e, int ety);
		CNetEventEntityIDAndPlayerIDAndEntityType(CNetEventEntityIDAndPlayerIDAndEntityType&&) = default;
		CNetEventEntityIDAndPlayerIDAndEntityType(const CNetEventEntityIDAndPlayerIDAndEntityType&) = default;
		CNetEventEntityIDAndPlayerIDAndEntityType& operator=(CNetEventEntityIDAndPlayerIDAndEntityType&&) = default;
		CNetEventEntityIDAndPlayerIDAndEntityType& operator=(const CNetEventEntityIDAndPlayerIDAndEntityType&) = default;

		static inline constexpr int TypeDesc = 0x812340;
		static inline constexpr int vtp = 0x77057C;
	};


}

namespace GGL {
	class CNetEventExtractResource : public BB::CEvent {
	public:
		int EntityId;
		shok::ResourceType ResourceType;
		shok::Position Position;

		CNetEventExtractResource(shok::NetEventIds id, int ent, shok::ResourceType resty, const shok::Position& pos);
		CNetEventExtractResource(CNetEventExtractResource&&) = default;
		CNetEventExtractResource(const CNetEventExtractResource&) = default;
		CNetEventExtractResource& operator=(CNetEventExtractResource&&) = default;
		CNetEventExtractResource& operator=(const CNetEventExtractResource&) = default;

		static inline constexpr int vtp = 0x77061C;
		static inline constexpr int TypeDesc = 0x81252C;
	};

	class CNetEventTransaction : public BB::CEvent {
	public:
		int EntityId;
		shok::ResourceType SellType, BuyType;
		int BuyAmount;

		CNetEventTransaction(shok::NetEventIds id, int ent, shok::ResourceType sell, shok::ResourceType buy, int buyam);
		CNetEventTransaction(CNetEventTransaction&&) = default;
		CNetEventTransaction(const CNetEventTransaction&) = default;
		CNetEventTransaction& operator=(CNetEventTransaction&&) = default;
		CNetEventTransaction& operator=(const CNetEventTransaction&) = default;

		static inline constexpr int vtp = 0x77062C;
		static inline constexpr int TypeDesc = 0x812558;
	};

	class CNetEventCannonCreator : public EGL::CNetEventEntityID {
	public:
		int BottomType, TopType;
		shok::Position Position; // maybe also rot?

		CNetEventCannonCreator(shok::NetEventIds id, int ent, int bty, int tty, const shok::Position& pos);
		CNetEventCannonCreator(CNetEventCannonCreator&&) = default;
		CNetEventCannonCreator(const CNetEventCannonCreator&) = default;
		CNetEventCannonCreator& operator=(CNetEventCannonCreator&&) = default;
		CNetEventCannonCreator& operator=(const CNetEventCannonCreator&) = default;

		static inline constexpr int TypeDesc = 0x812494;
		static inline constexpr int vtp = 0x7705EC;
	};

	class CNetEventEntityIDAndUpgradeCategory : public EGL::CNetEventEntityID {
	public:
		int UpgradeCategory;

		CNetEventEntityIDAndUpgradeCategory(shok::NetEventIds id, int ent, int ucat);
		CNetEventEntityIDAndUpgradeCategory(CNetEventEntityIDAndUpgradeCategory&&) = default;
		CNetEventEntityIDAndUpgradeCategory(const CNetEventEntityIDAndUpgradeCategory&) = default;
		CNetEventEntityIDAndUpgradeCategory& operator=(CNetEventEntityIDAndUpgradeCategory&&) = default;
		CNetEventEntityIDAndUpgradeCategory& operator=(const CNetEventEntityIDAndUpgradeCategory&) = default;

		static inline constexpr int TypeDesc = 0x8124F4;
		static inline constexpr int vtp = 0x77060C;
	};

	class CNetEventTechnologyAndEntityID : public EGL::CNetEventEntityID {
	public:
		int Technology;

		CNetEventTechnologyAndEntityID(shok::NetEventIds id, int ent, int tech);
		CNetEventTechnologyAndEntityID(CNetEventTechnologyAndEntityID&&) = default;
		CNetEventTechnologyAndEntityID(const CNetEventTechnologyAndEntityID&) = default;
		CNetEventTechnologyAndEntityID& operator=(CNetEventTechnologyAndEntityID&&) = default;
		CNetEventTechnologyAndEntityID& operator=(const CNetEventTechnologyAndEntityID&) = default;

		static inline constexpr int TypeDesc = 0x8124C0;
		static inline constexpr int vtp = 0x7705FC;
	};

	class CNetEventBuildingCreator : public EGL::CNetEventPlayerID { // add at least one serf, or it crashes
	public:
		int UpgradeCategory;
		shok::PositionRot Position;
		shok::Vector<int> Serfs;

		CNetEventBuildingCreator(shok::NetEventIds id, int pl, int ucat, const shok::PositionRot& p);
		CNetEventBuildingCreator(CNetEventBuildingCreator&&) = default;
		CNetEventBuildingCreator(const CNetEventBuildingCreator&) = default;
		CNetEventBuildingCreator& operator=(CNetEventBuildingCreator&&) = default;
		CNetEventBuildingCreator& operator=(const CNetEventBuildingCreator&) = default;

		static inline constexpr int TypeDesc = 0x81261C;
		static inline constexpr int vtp = 0x770714;
	};

	class CNetEventEntityIDPlayerIDAndInteger : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		int Data;

		CNetEventEntityIDPlayerIDAndInteger(shok::NetEventIds id, int pl, int e, int d);
		CNetEventEntityIDPlayerIDAndInteger(CNetEventEntityIDPlayerIDAndInteger&&) = default;
		CNetEventEntityIDPlayerIDAndInteger(const CNetEventEntityIDPlayerIDAndInteger&) = default;
		CNetEventEntityIDPlayerIDAndInteger& operator=(CNetEventEntityIDPlayerIDAndInteger&&) = default;
		CNetEventEntityIDPlayerIDAndInteger& operator=(const CNetEventEntityIDPlayerIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x8125B4;
		static inline constexpr int vtp = 0x77064C;
	};

	class CFeedbackEventBattling : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		shok::Position Pos;
		int TargetPlayer;

		CFeedbackEventBattling(shok::FeedbackEventIds id, int eid, int p, const shok::Position& pos, int tp);
		CFeedbackEventBattling(CFeedbackEventBattling&&) = default;
		CFeedbackEventBattling(const CFeedbackEventBattling&) = default;
		CFeedbackEventBattling& operator=(CFeedbackEventBattling&&) = default;
		CFeedbackEventBattling& operator=(const CFeedbackEventBattling&) = default;

		static inline constexpr int TypeDesc = 0x80FFE4;
		static inline constexpr int vtp = 0x76DDA0;
	};

}

namespace Framework {
	class CEventGameSpeed : public BB::CEvent {
	public:
		double SpeedFactor;

		CEventGameSpeed(shok::NetEventIds id, double speed);
		CEventGameSpeed(CEventGameSpeed&&) = default;
		CEventGameSpeed(const CEventGameSpeed&) = default;
		CEventGameSpeed& operator=(CEventGameSpeed&&) = default;
		CEventGameSpeed& operator=(const CEventGameSpeed&) = default;

		static inline constexpr int TypeDesc = 0x800078;
		static inline constexpr int vtp = 0x762474;
	};
}

namespace shok {
	enum class NetEventIds : int {
		CommandEntityAttackEntity = 69650,
		CommandSerfConstructBuilding = 69655,
		CommandSerfRepairBuilding = 69656,
		CommandEntityGuardEntity = 69664,
		CommandHeroConvertSettler = 69695,
		CommandThiefStealFrom = 69699,
		CommandThiefCarryStolenStuffToHQ = 69700,
		CommandThiefSabotageBuilding = 69701,
		CommandThiefDefuseKeg = 69702,
		CommandHeroSnipeSettler = 69705,
		CommandHeroThrowShurikenAt = 69708,
		CommandHeroPlaceCannonAt = 69679,
		CommandHeroPlaceBombAt = 69668,
		CommandEntityAttackPos = 69663,
		CommandHeroSendHawkToPos = 69676,
		CommandScoutUseBinocularsAt = 69704,
		CommandScoutPlaceTorchAtPos = 69706,
		CommandEntityMove = 69634,
		CommandEntityPatrol = 69669,
		CommandBuildingStartUpgrade = 69640,
		CommandLeaderBuySoldier = 69644,
		CommandSettlerExpell = 69647,
		CommandBuildingCancelResearch = 69659,
		CommandMarketCancelTrade = 69661,
		CommandBuildingCancelUpgrade = 69662,
		CommandLeaderHoldPosition = 69665,
		CommandLeaderDefend = 69667,
		CommandBattleSerfTurnToSerf = 69677,
		CommandSerfTurnToBattleSerf = 69678,
		CommandHeroActivateCamouflage = 69682,
		CommandHeroActivateSummon = 69685,
		CommandBuildingToggleOvertime = 69683,
		CommandHeroAffectEntities = 69689,
		CommandHeroCircularAttack = 69690,
		CommandHeroInflictFear = 69694,
		CommandBarracksRecruitGroups = 69696,
		CommandBarracksRecruitLeaderOnly = 69697,
		CommandHeroMotivateWorkers = 69703,
		CommandScoutFindResources = 69707,
		CommandPlaceBuilding = 69635,
		CommandHQBuySerf = 69636,
		CommandBuildingSell = 69638,
		CommandPlayerActivateAlarm = 69680,
		CommandPlayerDeactivateAlarm = 69681,
		PlayerUpgradeSettlerCategory = 69642,
		CommandPlayerSetTaxes = 69646,
		CommandWeathermachineChangeWeather = 69686,
		CommandMonasteryBlessSettlerGroup = 69687,
		CommandPlayerPayTribute = 69670,
		CommandBarracksBuyLeader = 69643,
		CommandLeaderSetFormation = 69653,
		CommandBuildingSetCurrentMaxWorkers = 69672,
		CommandFoundryBuildCannon = 69684,
		CommandSerfExtractResource = 69657,
		CommandBuildingStartResearch = 69658,
		CommandMarketStartTrade = 69660,
		SetGameSpeed = 0x50001,
	};

	enum class FeedbackEventIds : int {
		FEEDBACK_EVENT_RESOURCE_MINED = 0x13001,
		FEEDBACK_EVENT_RESOURCE_RUNNING_OUT = 0x13002,
		FEEDBACK_EVENT_MINE_RESOURCE_GONE = 0x13003,
		FEEDBACK_EVENT_ENTITIES_EXCHANGED = 0x13004,
		FEEDBACK_EVENT_BATTLING = 0x13005, // GGL::CFeedbackEventBattling attackerplayer, attackerid, targetplayer, attackerx, attackery
		FEEDBACK_EVENT_SETTLER_WALK = 0x13006,
		FEEDBACK_EVENT_TECHNOLOGY_RESEARCHED = 0x13007,
		FEEDBACK_EVENT_SETTLER_GRIEVANCE = 0x13008,
		FEEDBACK_EVENT_SETTLER_LEFT_SETTLEMENT = 0x13009,
		FEEDBACK_EVENT_SETTLER_JOINED_SETTLEMENT = 0x1300A,
		FEEDBACK_EVENT_QUEST_CHANGED = 0x1300B,
		FEEDBACK_EVENT_MARKET_TRANSACTION_COMPLETE = 0x1300C,
		FEEDBACK_EVENT_NO_MONEY = 0x1300D,
		FEEDBACK_EVENT_PAYDAY = 0x1300E,
		FEEDBACK_EVENT_NOT_ENOUGH_RESOURCES = 0x13010,
		FEEDBACK_EVENT_LEADER_COMMAND = 0x13012,
		FEEDBACK_EVENT_HERO_ABILITY = 0x13013,
		FEEDBACK_EVENT_SEND_RESOURCES = 0x13014,
		FEEDBACK_EVENT_GOT_RESOURCES = 0x13015,
		FEEDBACK_EVENT_WEATHER_CHANGED = 0x13016,
		FEEDBACK_EVENT_POPULATION_LIMIT_REACHED = 0x13017,

		FEEDBACK_EVENT_SIMPLE = 0x23003,
		FEEDBACK_EVENT_ENTITY_HURT = 0x23004, // EGL::CNetEventEntityIDAndInteger entity, damage

		FEEDBACK_EVENT_FARM_LIMIT_REACHED = 0x25001,
		FEEDBACK_EVENT_RESIDENCE_LIMIT_REACHED = 0x25002,
		FEEDBACK_EVENT_HQ_2_POSSIBLE = 0x25003,
		FEEDBACK_EVENT_HQ_3_POSSIBLE = 0x25004,
		FEEDBACK_EVENT_BUILDING_UPGRADE_FINISHED = 0x25005,
		FEEDBACK_EVENT_LEADER_CHANGE_FORMATION = 0x25006,
		FEEDBACK_EVENT_WEATHER_FORECAST = 0x25007,
	};
}
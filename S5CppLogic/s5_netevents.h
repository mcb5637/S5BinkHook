#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_events.h"

namespace EGL {
	class CNetEvent2Entities : public BB::CEvent {
	public:
		int EntityID1, EntityID2;

		CNetEvent2Entities(shok::NetEventIds id, int actor, int tar);
		CNetEvent2Entities(CNetEvent2Entities&&) = default;
		CNetEvent2Entities(const CNetEvent2Entities&) = default;
		CNetEvent2Entities& operator=(CNetEvent2Entities&&) = default;
		CNetEvent2Entities& operator=(const CNetEvent2Entities&) = default;

		static inline constexpr int vtp = 0x76DD60;
		static inline constexpr int TypeDesc = 0x80FF1C;
		static inline constexpr unsigned int Identifier = 0x84A88A7D;
	};

	class CNetEventEntityAndPos : public BB::CEvent {
	public:
		int EntityID;
		float X, Y;

		CNetEventEntityAndPos(shok::NetEventIds id, int ent, const shok::Position& pos);
		CNetEventEntityAndPos(CNetEventEntityAndPos&&) = default;
		CNetEventEntityAndPos(const CNetEventEntityAndPos&) = default;
		CNetEventEntityAndPos& operator=(CNetEventEntityAndPos&&) = default;
		CNetEventEntityAndPos& operator=(const CNetEventEntityAndPos&) = default;

		static inline constexpr int vtp = 0x76DD50;
		static inline constexpr int TypeDesc = 0x80FEF4;
		static inline constexpr unsigned int Identifier = 0x573B9A0;
	};

	class CNetEventEntityAndPosArray : public BB::CEvent {
	public:
		int EntityID;
		shok::Vector<shok::Position> Position;
		float Orientation;

		EGL::CNetEventEntityAndPosArray(shok::NetEventIds id, int ent, float r);
		EGL::CNetEventEntityAndPosArray(const EGL::CNetEventEntityAndPosArray&) = default;
		EGL::CNetEventEntityAndPosArray& operator=(const EGL::CNetEventEntityAndPosArray&) = default;

		static inline constexpr int vtp = 0x770704;
		static inline constexpr int TypeDesc = 0x8125EC;
		static inline constexpr unsigned int Identifier = 0x4CD042B7;
	};

	class CNetEventEntityID : public BB::CEvent {
	public:
		int EntityID;

		CNetEventEntityID(shok::NetEventIds id, int ent);
		CNetEventEntityID(shok::FeedbackEventIds id, int ent);
		CNetEventEntityID(CNetEventEntityID&&) = default;
		CNetEventEntityID(const CNetEventEntityID&) = default;
		CNetEventEntityID& operator=(CNetEventEntityID&&) = default;
		CNetEventEntityID& operator=(const CNetEventEntityID&) = default;

		static inline constexpr int vtp = 0x766C28;
		static inline constexpr int TypeDesc = 0x807924;
		static inline constexpr unsigned int Identifier = 0x5D413628;
	};

	class CNetEventEntityIDAndInteger : public EGL::CNetEventEntityID {
	public:
		int Value;

		CNetEventEntityIDAndInteger(shok::NetEventIds id, int ent, int d);
		CNetEventEntityIDAndInteger(shok::FeedbackEventIds id, int ent, int d);
		CNetEventEntityIDAndInteger(CNetEventEntityIDAndInteger&&) = default;
		CNetEventEntityIDAndInteger(const CNetEventEntityIDAndInteger&) = default;
		CNetEventEntityIDAndInteger& operator=(CNetEventEntityIDAndInteger&&) = default;
		CNetEventEntityIDAndInteger& operator=(const CNetEventEntityIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x807978;
		static inline constexpr int vtp = 0x766C48;
		static inline constexpr unsigned int Identifier = 0x8D682418;
	};

	class CNetEventPlayerID : public BB::CEvent {
	public:
		int PlayerID;

		CNetEventPlayerID(shok::NetEventIds id, int pl);
		CNetEventPlayerID(shok::FeedbackEventIds id, int pl);
		CNetEventPlayerID(CNetEventPlayerID&&) = default;
		CNetEventPlayerID(const CNetEventPlayerID&) = default;
		CNetEventPlayerID& operator=(CNetEventPlayerID&&) = default;
		CNetEventPlayerID& operator=(const CNetEventPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x807900;
		static inline constexpr int vtp = 0x766C18;
		static inline constexpr unsigned int Identifier = 0x59FA0FB7;
	};

	class CNetEventIntegerAndPlayerID : public EGL::CNetEventPlayerID {
	public:
		int Value;

		CNetEventIntegerAndPlayerID(shok::NetEventIds id, int pl, int d);
		CNetEventIntegerAndPlayerID(CNetEventIntegerAndPlayerID&&) = default;
		CNetEventIntegerAndPlayerID(const CNetEventIntegerAndPlayerID&) = default;
		CNetEventIntegerAndPlayerID& operator=(CNetEventIntegerAndPlayerID&&) = default;
		CNetEventIntegerAndPlayerID& operator=(const CNetEventIntegerAndPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x812404;
		static inline constexpr int vtp = 0x7705BC;
		static inline constexpr unsigned int Identifier = 0xB365483;
	};

	class CNetEventPlayerIDAndInteger : public EGL::CNetEventPlayerID {
	public:
		int Value;

		CNetEventPlayerIDAndInteger(shok::NetEventIds id, int pl, int d);
		CNetEventPlayerIDAndInteger(CNetEventPlayerIDAndInteger&&) = default;
		CNetEventPlayerIDAndInteger(const CNetEventPlayerIDAndInteger&) = default;
		CNetEventPlayerIDAndInteger& operator=(CNetEventPlayerIDAndInteger&&) = default;
		CNetEventPlayerIDAndInteger& operator=(const CNetEventPlayerIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x812434;
		static inline constexpr int vtp = 0x7705CC;
		static inline constexpr unsigned int Identifier = 0xAA030F86;
	};

	class CNetEventEntityIDAndPlayerID : public EGL::CNetEventPlayerID {
	public:
		int EntityID;

		CNetEventEntityIDAndPlayerID(shok::NetEventIds id, int pl, int e);
		CNetEventEntityIDAndPlayerID(shok::FeedbackEventIds id, int pl, int e);
		CNetEventEntityIDAndPlayerID(CNetEventEntityIDAndPlayerID&&) = default;
		CNetEventEntityIDAndPlayerID(const CNetEventEntityIDAndPlayerID&) = default;
		CNetEventEntityIDAndPlayerID& operator=(CNetEventEntityIDAndPlayerID&&) = default;
		CNetEventEntityIDAndPlayerID& operator=(const CNetEventEntityIDAndPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x807948;
		static inline constexpr int vtp = 0x766C38;
		static inline constexpr unsigned int Identifier = 0xDD5C5BD7;
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
		static inline constexpr unsigned int Identifier = 0xF1B577F8;
	};

	class CNetEvent2PlayerIDs : public BB::CEvent {
	public:
		int PlayerID1;
		int PlayerID2;

		CNetEvent2PlayerIDs(shok::NetEventIds id, int p1, int p2);
		CNetEvent2PlayerIDs(CNetEvent2PlayerIDs&&) = default;
		CNetEvent2PlayerIDs(const CNetEvent2PlayerIDs&) = default;
		CNetEvent2PlayerIDs& operator=(CNetEvent2PlayerIDs&&) = default;
		CNetEvent2PlayerIDs& operator=(const CNetEvent2PlayerIDs&) = default;

		static inline constexpr int TypeDesc = 0x8123DC;
		static inline constexpr int vtp = 0x7705AC;
		static inline constexpr unsigned int Identifier = 0xD39DDE58;
	};

	class CNetEvent2PlayerIDsAndInteger : public CNetEvent2PlayerIDs {
	public:
		int Value;

		CNetEvent2PlayerIDsAndInteger(shok::NetEventIds id, int p1, int p2, int v);
		CNetEvent2PlayerIDsAndInteger(CNetEvent2PlayerIDsAndInteger&&) = default;
		CNetEvent2PlayerIDsAndInteger(const CNetEvent2PlayerIDsAndInteger&) = default;
		CNetEvent2PlayerIDsAndInteger& operator=(CNetEvent2PlayerIDsAndInteger&&) = default;
		CNetEvent2PlayerIDsAndInteger& operator=(const CNetEvent2PlayerIDsAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x812464;
		static inline constexpr int vtp = 0x7705DC;
		static inline constexpr unsigned int Identifier = 0x82DAE6F6;
	};
}

namespace GGL {
	class CNetEventExtractResource : public BB::CEvent {
	public:
		int SerfID;
		shok::ResourceType ResourceType;
		shok::Position Position;

		CNetEventExtractResource(shok::NetEventIds id, int ent, shok::ResourceType resty, const shok::Position& pos);
		CNetEventExtractResource(CNetEventExtractResource&&) = default;
		CNetEventExtractResource(const CNetEventExtractResource&) = default;
		CNetEventExtractResource& operator=(CNetEventExtractResource&&) = default;
		CNetEventExtractResource& operator=(const CNetEventExtractResource&) = default;

		static inline constexpr int vtp = 0x77061C;
		static inline constexpr int TypeDesc = 0x81252C;
		static inline constexpr unsigned int Identifier = 0x315F6EB7;
	};

	class CNetEventTransaction : public BB::CEvent {
	public:
		int MarketID;
		shok::ResourceType SellResource, BuyResource;
		int BuyAmount;

		CNetEventTransaction(shok::NetEventIds id, int ent, shok::ResourceType sell, shok::ResourceType buy, int buyam);
		CNetEventTransaction(CNetEventTransaction&&) = default;
		CNetEventTransaction(const CNetEventTransaction&) = default;
		CNetEventTransaction& operator=(CNetEventTransaction&&) = default;
		CNetEventTransaction& operator=(const CNetEventTransaction&) = default;

		static inline constexpr int vtp = 0x77062C;
		static inline constexpr int TypeDesc = 0x812558;
		static inline constexpr unsigned int Identifier = 0xF18688D7;
	};

	class CNetEventCannonCreator : public EGL::CNetEventEntityID {
	public:
		int FoundationType, CannonType;
		shok::Position Position;
		float Orientation;

		CNetEventCannonCreator(shok::NetEventIds id, int ent, int bty, int tty, const shok::PositionRot& pos);
		CNetEventCannonCreator(CNetEventCannonCreator&&) = default;
		CNetEventCannonCreator(const CNetEventCannonCreator&) = default;
		CNetEventCannonCreator& operator=(CNetEventCannonCreator&&) = default;
		CNetEventCannonCreator& operator=(const CNetEventCannonCreator&) = default;

		static inline constexpr int TypeDesc = 0x812494;
		static inline constexpr int vtp = 0x7705EC;
		static inline constexpr unsigned int Identifier = 0xA0E8444D;
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
		static inline constexpr unsigned int Identifier = 0xE0855817;
	};

	class CNetEventTechnologyAndEntityID : public EGL::CNetEventEntityID {
	public:
		int TechnologyType;

		CNetEventTechnologyAndEntityID(shok::NetEventIds id, int ent, int tech);
		CNetEventTechnologyAndEntityID(CNetEventTechnologyAndEntityID&&) = default;
		CNetEventTechnologyAndEntityID(const CNetEventTechnologyAndEntityID&) = default;
		CNetEventTechnologyAndEntityID& operator=(CNetEventTechnologyAndEntityID&&) = default;
		CNetEventTechnologyAndEntityID& operator=(const CNetEventTechnologyAndEntityID&) = default;

		static inline constexpr int TypeDesc = 0x8124C0;
		static inline constexpr int vtp = 0x7705FC;
		static inline constexpr unsigned int Identifier = 0x70DF356;
	};

	class CNetEventBuildingCreator : public EGL::CNetEventPlayerID { // add at least one serf, or it crashes
	public:
		int EntityType; // this is a upgradecategory
		shok::Position Position;
		float Orientation;
		shok::Vector<int> Serf;

		CNetEventBuildingCreator(shok::NetEventIds id, int pl, int ety, const shok::PositionRot& p);
		CNetEventBuildingCreator(CNetEventBuildingCreator&&) = default;
		CNetEventBuildingCreator(const CNetEventBuildingCreator&) = default;
		CNetEventBuildingCreator& operator=(CNetEventBuildingCreator&&) = default;
		CNetEventBuildingCreator& operator=(const CNetEventBuildingCreator&) = default;

		static inline constexpr int TypeDesc = 0x81261C;
		static inline constexpr int vtp = 0x770714;
		static inline constexpr unsigned int Identifier = 0xF446A1A3;
	};

	class CNetEventEntityIDPlayerIDAndInteger : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		int Value;

		CNetEventEntityIDPlayerIDAndInteger(shok::NetEventIds id, int pl, int e, int d);
		CNetEventEntityIDPlayerIDAndInteger(CNetEventEntityIDPlayerIDAndInteger&&) = default;
		CNetEventEntityIDPlayerIDAndInteger(const CNetEventEntityIDPlayerIDAndInteger&) = default;
		CNetEventEntityIDPlayerIDAndInteger& operator=(CNetEventEntityIDPlayerIDAndInteger&&) = default;
		CNetEventEntityIDPlayerIDAndInteger& operator=(const CNetEventEntityIDPlayerIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x8125B4;
		static inline constexpr int vtp = 0x77064C;
		static inline constexpr unsigned int Identifier = 0x7766FA33;
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
		static inline constexpr unsigned int Identifier = 0xFA523F5E;
	};

	class CFeedbackEventResource : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		shok::ResourceType ResourceType;
		float ExtractionAmount;

		CFeedbackEventResource(shok::FeedbackEventIds id, int player, int resentity, shok::ResourceType rty, float extrAm);
		CFeedbackEventResource(CFeedbackEventResource&&) = default;
		CFeedbackEventResource(const CFeedbackEventResource&) = default;
		CFeedbackEventResource& operator=(CFeedbackEventResource&&) = default;
		CFeedbackEventResource& operator=(const CFeedbackEventResource&) = default;

		static inline constexpr int TypeDesc = 0x81A3C4;
		static inline constexpr int vtp = 0x774970;
		static inline constexpr unsigned int Identifier = 0x4AFA2726;
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
		static inline constexpr unsigned int Identifier = 0x3C68E6F3;
	};
}

namespace shok {
	enum class NetEventIds : int {
		CommandEntityMove = 0x11002,
		CommandPlaceBuilding = 0x11003,
		CommandHQBuySerf = 0x11004,
		CommandBuildingSell = 0x11006,
		CommandBuildingStartUpgrade = 0x11008,
		PlayerUpgradeBuildingCategory = 0x11009,
		PlayerUpgradeSettlerCategory = 0x1100A,
		CommandBarracksBuyLeader = 0x1100B,
		CommandLeaderBuySoldier = 0x1100C,
		CommandPlayerSetTaxes = 0x1100E,
		CommandSettlerExpell = 0x1100F,
		CommandEntityAttackEntity = 0x11012,
		LeaderAttachOneSoldier = 0x11013,
		LeaderExpellOneSoldier = 0x11014,
		CommandLeaderSetFormation = 0x11015,
		CommandSerfConstructBuilding = 0x11017,
		CommandSerfRepairBuilding = 0x11018,
		CommandSerfExtractResource = 0x11019,
		CommandBuildingStartResearch = 0x1101A,
		CommandBuildingCancelResearch = 0x1101B,
		CommandMarketStartTrade = 0x1101C,
		CommandMarketCancelTrade = 0x1101D,
		CommandBuildingCancelUpgrade = 0x1101E,
		CommandEntityAttackPos = 0x1101F,
		CommandEntityGuardEntity = 0x11020,
		CommandLeaderHoldPosition = 0x11021,
		CommandLeaderDefend = 0x11023,
		CommandHeroPlaceBombAt = 0x11024,
		CommandEntityPatrol = 0x11025,
		CommandPlayerPayTribute = 0x11026,
		SetDiplomacy = 0x11027, // not sure if this can be used normally
		CommandBuildingSetCurrentMaxWorkers = 0x11028,
		CommandHeroSendHawkToPos = 0x1102C,
		CommandBattleSerfTurnToSerf = 0x1102D,
		CommandSerfTurnToBattleSerf = 0x1102E,
		CommandHeroPlaceCannonAt = 0x1102F,
		CommandPlayerActivateAlarm = 0x11030,
		CommandPlayerDeactivateAlarm = 0x11031,
		CommandHeroActivateCamouflage = 0x11032,
		CommandBuildingToggleOvertime = 0x11033,
		CommandFoundryBuildCannon = 0x11034,
		CommandHeroActivateSummon = 0x11035,
		CommandWeathermachineChangeWeather = 0x11036,
		CommandMonasteryBlessSettlerGroup = 0x11037,
		CommandHeroAffectEntities = 0x11039,
		CommandHeroCircularAttack = 0x1103A,
		CommandHeroInflictFear = 0x1103E,
		CommandHeroConvertSettler = 0x1103F,
		CommandBarracksRecruitGroups = 0x11040,
		CommandBarracksRecruitLeaderOnly = 0x11041,
		CommandThiefStealFrom = 0x11043,
		CommandThiefCarryStolenStuffToHQ = 0x11044,
		CommandThiefSabotageBuilding = 0x11045,
		CommandThiefDefuseKeg = 0x11046,
		CommandHeroMotivateWorkers = 0x11047,
		CommandScoutUseBinocularsAt = 0x11048,
		CommandHeroSnipeSettler = 0x11049,
		CommandScoutPlaceTorchAtPos = 0x1104A,
		CommandScoutFindResources = 0x1104B,
		CommandHeroThrowShurikenAt = 0x1104C,
		SetGameSpeed = 0x50001,
	};

	enum class FeedbackEventIds : int {
		FEEDBACK_EVENT_RESOURCE_MINED = 0x13001, // GGL::CFeedbackEventResource
		FEEDBACK_EVENT_RESOURCE_RUNNING_OUT = 0x13002, // GGL::CFeedbackEventResourceRunningOut
		FEEDBACK_EVENT_MINE_RESOURCE_GONE = 0x13003, // GGL::CFeedbackEventResourceRunningOut
		FEEDBACK_EVENT_ENTITIES_EXCHANGED = 0x13004, // EGL::CNetEvent2Entities
		FEEDBACK_EVENT_BATTLING = 0x13005, // GGL::CFeedbackEventBattling attackerplayer, attackerid, targetplayer, attackerx, attackery
		FEEDBACK_EVENT_SETTLER_WALK = 0x13006, // EGL::CNetEventEntityAndPo
		FEEDBACK_EVENT_TECHNOLOGY_RESEARCHED = 0x13007, // GGL::CNetEventEventTechnologyPlayerIDAndEntityID
		FEEDBACK_EVENT_SETTLER_GRIEVANCE = 0x13008, // EGL::CNetEventEntityIDAnd3Integer
		FEEDBACK_EVENT_SETTLER_LEFT_SETTLEMENT = 0x13009, // EGL::CNetEventEntityIDAndInteger
		FEEDBACK_EVENT_SETTLER_JOINED_SETTLEMENT = 0x1300A, // EGL::CNetEventEntityIDAndInteger
		FEEDBACK_EVENT_QUEST_CHANGED = 0x1300B, // GGL::CFeedbackEventPlayerIDMapPositionInteger
		FEEDBACK_EVENT_MARKET_TRANSACTION_COMPLETE = 0x1300C, // EGL::CNetEventEntityID
		FEEDBACK_EVENT_NO_MONEY = 0x1300D, // EGL::CNetEventPlayerID
		FEEDBACK_EVENT_PAYDAY = 0x1300E, // EGL::CNetEventPlayerID
		FEEDBACK_EVENT_NOT_ENOUGH_RESOURCES = 0x13010, // EGL::CNetEventPlayerIDGoodTypeAndInteger
		//? 13011 EGL::CNetEventEntityID convert/die?
		FEEDBACK_EVENT_LEADER_COMMAND = 0x13012, // EGL::CNetEventEntityIDAndInteger
		FEEDBACK_EVENT_HERO_ABILITY = 0x13013, // EGL::CNetEventEntityIDAndInteger
		FEEDBACK_EVENT_SEND_RESOURCES = 0x13014, // EGL::CNetEventPlayerIDGoodTypeAndInteger
		FEEDBACK_EVENT_GOT_RESOURCES = 0x13015, // EGL::CNetEventPlayerIDGoodTypeAndInteger
		FEEDBACK_EVENT_WEATHER_CHANGED = 0x13016, // EGL::CEventValue<int,-27574121>
		FEEDBACK_EVENT_POPULATION_LIMIT_REACHED = 0x13017, // EGL::CEventPlayerID

		// 23001 BB::CEvent map loaded?
		// 23002 BB::CEvent map saved?
		FEEDBACK_EVENT_SIMPLE = 0x23003, // EGL::CFeedbackEventSimple
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
#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/events/s5_events.h>
#include <utility/EnumIdManagerMagic.h>

namespace EGL {
	class CNetEvent2Entities : public BB::CEvent {
	public:
		shok::EntityId EntityID1, EntityID2;

		CNetEvent2Entities(shok::NetEventIds id, shok::EntityId actor, shok::EntityId tar);
		CNetEvent2Entities(CNetEvent2Entities&&) = default;
		CNetEvent2Entities(const CNetEvent2Entities&) = default;
		CNetEvent2Entities& operator=(CNetEvent2Entities&&) = default;
		CNetEvent2Entities& operator=(const CNetEvent2Entities&) = default;

		static inline constexpr int vtp = 0x76DD60;
		static inline constexpr int TypeDesc = 0x80FF1C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x84A88A7D);
	};

	class CNetEventEntityAndPos : public BB::CEvent {
	public:
		shok::EntityId EntityID;
		float X, Y;

		CNetEventEntityAndPos(shok::NetEventIds id, shok::EntityId ent, const shok::Position& pos);
		CNetEventEntityAndPos(CNetEventEntityAndPos&&) = default;
		CNetEventEntityAndPos(const CNetEventEntityAndPos&) = default;
		CNetEventEntityAndPos& operator=(CNetEventEntityAndPos&&) = default;
		CNetEventEntityAndPos& operator=(const CNetEventEntityAndPos&) = default;

		static inline constexpr int vtp = 0x76DD50;
		static inline constexpr int TypeDesc = 0x80FEF4;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x573B9A0);
	};

	class CNetEventEntityAndPosArray : public BB::CEvent {
	public:
		shok::EntityId EntityID;
		shok::Vector<shok::Position> Position;
		float Orientation;

		CNetEventEntityAndPosArray(shok::NetEventIds id, shok::EntityId ent, float r);
		CNetEventEntityAndPosArray(const EGL::CNetEventEntityAndPosArray&) = default;
		CNetEventEntityAndPosArray& operator=(const EGL::CNetEventEntityAndPosArray&) = default;

		static inline constexpr int vtp = 0x770704;
		static inline constexpr int TypeDesc = 0x8125EC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4CD042B7);
	};

	class CNetEventEntityID : public BB::CEvent {
	public:
		shok::EntityId EntityID;

		CNetEventEntityID(shok::NetEventIds id, shok::EntityId ent);
		CNetEventEntityID(shok::FeedbackEventIds id, shok::EntityId ent);
		CNetEventEntityID(CNetEventEntityID&&) = default;
		CNetEventEntityID(const CNetEventEntityID&) = default;
		CNetEventEntityID& operator=(CNetEventEntityID&&) = default;
		CNetEventEntityID& operator=(const CNetEventEntityID&) = default;

		static inline constexpr int vtp = 0x766C28;
		static inline constexpr int TypeDesc = 0x807924;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5D413628);
	};

	class CNetEventEntityIDAndInteger : public EGL::CNetEventEntityID {
	public:
		int Value;

		CNetEventEntityIDAndInteger(shok::NetEventIds id, shok::EntityId ent, int d);
		CNetEventEntityIDAndInteger(shok::FeedbackEventIds id, shok::EntityId ent, int d);
		CNetEventEntityIDAndInteger(CNetEventEntityIDAndInteger&&) = default;
		CNetEventEntityIDAndInteger(const CNetEventEntityIDAndInteger&) = default;
		CNetEventEntityIDAndInteger& operator=(CNetEventEntityIDAndInteger&&) = default;
		CNetEventEntityIDAndInteger& operator=(const CNetEventEntityIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x807978;
		static inline constexpr int vtp = 0x766C48;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8D682418);
	};

	class CNetEventPlayerID : public BB::CEvent {
	public:
		shok::PlayerId PlayerID;

		CNetEventPlayerID(shok::NetEventIds id, shok::PlayerId pl);
		CNetEventPlayerID(shok::FeedbackEventIds id, shok::PlayerId pl);
		CNetEventPlayerID(CNetEventPlayerID&&) = default;
		CNetEventPlayerID(const CNetEventPlayerID&) = default;
		CNetEventPlayerID& operator=(CNetEventPlayerID&&) = default;
		CNetEventPlayerID& operator=(const CNetEventPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x807900;
		static inline constexpr int vtp = 0x766C18;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x59FA0FB7);
	};

	class CNetEventIntegerAndPlayerID : public EGL::CNetEventPlayerID {
	public:
		int Value;

		CNetEventIntegerAndPlayerID(shok::NetEventIds id, shok::PlayerId pl, int d);
		CNetEventIntegerAndPlayerID(CNetEventIntegerAndPlayerID&&) = default;
		CNetEventIntegerAndPlayerID(const CNetEventIntegerAndPlayerID&) = default;
		CNetEventIntegerAndPlayerID& operator=(CNetEventIntegerAndPlayerID&&) = default;
		CNetEventIntegerAndPlayerID& operator=(const CNetEventIntegerAndPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x812404;
		static inline constexpr int vtp = 0x7705BC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB365483);
	};

	class CNetEventPlayerIDAndInteger : public EGL::CNetEventPlayerID {
	public:
		int Value;

		CNetEventPlayerIDAndInteger(shok::NetEventIds id, shok::PlayerId pl, int d);
		CNetEventPlayerIDAndInteger(CNetEventPlayerIDAndInteger&&) = default;
		CNetEventPlayerIDAndInteger(const CNetEventPlayerIDAndInteger&) = default;
		CNetEventPlayerIDAndInteger& operator=(CNetEventPlayerIDAndInteger&&) = default;
		CNetEventPlayerIDAndInteger& operator=(const CNetEventPlayerIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x812434;
		static inline constexpr int vtp = 0x7705CC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xAA030F86);
	};

	class CNetEventEntityIDAndPlayerID : public EGL::CNetEventPlayerID {
	public:
		shok::EntityId EntityID;

		CNetEventEntityIDAndPlayerID(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e);
		CNetEventEntityIDAndPlayerID(shok::FeedbackEventIds id, shok::PlayerId pl, shok::EntityId e);
		CNetEventEntityIDAndPlayerID(CNetEventEntityIDAndPlayerID&&) = default;
		CNetEventEntityIDAndPlayerID(const CNetEventEntityIDAndPlayerID&) = default;
		CNetEventEntityIDAndPlayerID& operator=(CNetEventEntityIDAndPlayerID&&) = default;
		CNetEventEntityIDAndPlayerID& operator=(const CNetEventEntityIDAndPlayerID&) = default;

		static inline constexpr int TypeDesc = 0x807948;
		static inline constexpr int vtp = 0x766C38;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xDD5C5BD7);
	};

	class CNetEventEntityIDAndPlayerIDAndEntityType : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		shok::EntityTypeId EntityType;

		CNetEventEntityIDAndPlayerIDAndEntityType(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e, shok::EntityTypeId ety);
		CNetEventEntityIDAndPlayerIDAndEntityType(CNetEventEntityIDAndPlayerIDAndEntityType&&) = default;
		CNetEventEntityIDAndPlayerIDAndEntityType(const CNetEventEntityIDAndPlayerIDAndEntityType&) = default;
		CNetEventEntityIDAndPlayerIDAndEntityType& operator=(CNetEventEntityIDAndPlayerIDAndEntityType&&) = default;
		CNetEventEntityIDAndPlayerIDAndEntityType& operator=(const CNetEventEntityIDAndPlayerIDAndEntityType&) = default;

		static inline constexpr int TypeDesc = 0x812340;
		static inline constexpr int vtp = 0x77057C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF1B577F8);
	};

	class CNetEvent2PlayerIDs : public BB::CEvent {
	public:
		shok::PlayerId PlayerID1;
		shok::PlayerId PlayerID2;

		CNetEvent2PlayerIDs(shok::NetEventIds id, shok::PlayerId p1, shok::PlayerId p2);
		CNetEvent2PlayerIDs(CNetEvent2PlayerIDs&&) = default;
		CNetEvent2PlayerIDs(const CNetEvent2PlayerIDs&) = default;
		CNetEvent2PlayerIDs& operator=(CNetEvent2PlayerIDs&&) = default;
		CNetEvent2PlayerIDs& operator=(const CNetEvent2PlayerIDs&) = default;

		static inline constexpr int TypeDesc = 0x8123DC;
		static inline constexpr int vtp = 0x7705AC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD39DDE58);
	};

	class CNetEvent2PlayerIDsAndInteger : public CNetEvent2PlayerIDs {
	public:
		int Value;

		CNetEvent2PlayerIDsAndInteger(shok::NetEventIds id, shok::PlayerId p1, shok::PlayerId p2, int v);
		CNetEvent2PlayerIDsAndInteger(CNetEvent2PlayerIDsAndInteger&&) = default;
		CNetEvent2PlayerIDsAndInteger(const CNetEvent2PlayerIDsAndInteger&) = default;
		CNetEvent2PlayerIDsAndInteger& operator=(CNetEvent2PlayerIDsAndInteger&&) = default;
		CNetEvent2PlayerIDsAndInteger& operator=(const CNetEvent2PlayerIDsAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x812464;
		static inline constexpr int vtp = 0x7705DC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x82DAE6F6);
	};
}

namespace GGL {
	class CNetEventExtractResource : public BB::CEvent {
	public:
		shok::EntityId SerfID;
		shok::ResourceType ResourceType;
		shok::Position Position;

		CNetEventExtractResource(shok::NetEventIds id, shok::EntityId ent, shok::ResourceType resty, const shok::Position& pos);
		CNetEventExtractResource(CNetEventExtractResource&&) = default;
		CNetEventExtractResource(const CNetEventExtractResource&) = default;
		CNetEventExtractResource& operator=(CNetEventExtractResource&&) = default;
		CNetEventExtractResource& operator=(const CNetEventExtractResource&) = default;

		static inline constexpr int vtp = 0x77061C;
		static inline constexpr int TypeDesc = 0x81252C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x315F6EB7);
	};

	class CNetEventTransaction : public BB::CEvent {
	public:
		shok::EntityId MarketID;
		shok::ResourceType SellResource, BuyResource;
		int BuyAmount;

		CNetEventTransaction(shok::NetEventIds id, shok::EntityId ent, shok::ResourceType sell, shok::ResourceType buy, int buyam);
		CNetEventTransaction(CNetEventTransaction&&) = default;
		CNetEventTransaction(const CNetEventTransaction&) = default;
		CNetEventTransaction& operator=(CNetEventTransaction&&) = default;
		CNetEventTransaction& operator=(const CNetEventTransaction&) = default;

		static inline constexpr int vtp = 0x77062C;
		static inline constexpr int TypeDesc = 0x812558;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF18688D7);
	};

	class CNetEventCannonCreator : public EGL::CNetEventEntityID {
	public:
		shok::EntityTypeId FoundationType, CannonType;
		shok::Position Position;
		float Orientation;

		CNetEventCannonCreator(shok::NetEventIds id, shok::EntityId ent, shok::EntityTypeId bty, shok::EntityTypeId tty, const shok::PositionRot& pos);
		CNetEventCannonCreator(CNetEventCannonCreator&&) = default;
		CNetEventCannonCreator(const CNetEventCannonCreator&) = default;
		CNetEventCannonCreator& operator=(CNetEventCannonCreator&&) = default;
		CNetEventCannonCreator& operator=(const CNetEventCannonCreator&) = default;

		static inline constexpr int TypeDesc = 0x812494;
		static inline constexpr int vtp = 0x7705EC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA0E8444D);
	};

	class CNetEventEntityIDAndUpgradeCategory : public EGL::CNetEventEntityID {
	public:
		shok::UpgradeCategoryId UpgradeCategory;

		CNetEventEntityIDAndUpgradeCategory(shok::NetEventIds id, shok::EntityId ent, shok::UpgradeCategoryId ucat);
		CNetEventEntityIDAndUpgradeCategory(CNetEventEntityIDAndUpgradeCategory&&) = default;
		CNetEventEntityIDAndUpgradeCategory(const CNetEventEntityIDAndUpgradeCategory&) = default;
		CNetEventEntityIDAndUpgradeCategory& operator=(CNetEventEntityIDAndUpgradeCategory&&) = default;
		CNetEventEntityIDAndUpgradeCategory& operator=(const CNetEventEntityIDAndUpgradeCategory&) = default;

		static inline constexpr int TypeDesc = 0x8124F4;
		static inline constexpr int vtp = 0x77060C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE0855817);
	};

	class CNetEventTechnologyAndEntityID : public EGL::CNetEventEntityID {
	public:
		shok::TechnologyId TechnologyType;

		CNetEventTechnologyAndEntityID(shok::NetEventIds id, shok::EntityId ent, shok::TechnologyId tech);
		CNetEventTechnologyAndEntityID(CNetEventTechnologyAndEntityID&&) = default;
		CNetEventTechnologyAndEntityID(const CNetEventTechnologyAndEntityID&) = default;
		CNetEventTechnologyAndEntityID& operator=(CNetEventTechnologyAndEntityID&&) = default;
		CNetEventTechnologyAndEntityID& operator=(const CNetEventTechnologyAndEntityID&) = default;

		static inline constexpr int TypeDesc = 0x8124C0;
		static inline constexpr int vtp = 0x7705FC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x70DF356);
	};

	class CNetEventBuildingCreator : public EGL::CNetEventPlayerID { // add at least one serf, or it crashes
	public:
		shok::UpgradeCategoryId EntityType; // this is a upgradecategory
		shok::Position Position;
		float Orientation;
		shok::Vector<shok::EntityId> Serf;

		CNetEventBuildingCreator(shok::NetEventIds id, shok::PlayerId pl, shok::UpgradeCategoryId ety, const shok::PositionRot& p);
		CNetEventBuildingCreator(CNetEventBuildingCreator&&) = default;
		CNetEventBuildingCreator(const CNetEventBuildingCreator&) = default;
		CNetEventBuildingCreator& operator=(CNetEventBuildingCreator&&) = default;
		CNetEventBuildingCreator& operator=(const CNetEventBuildingCreator&) = default;

		static inline constexpr int TypeDesc = 0x81261C;
		static inline constexpr int vtp = 0x770714;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF446A1A3);
	};

	class CNetEventEntityIDPlayerIDAndInteger : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		int Value;

		CNetEventEntityIDPlayerIDAndInteger(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e, int d);
		CNetEventEntityIDPlayerIDAndInteger(CNetEventEntityIDPlayerIDAndInteger&&) = default;
		CNetEventEntityIDPlayerIDAndInteger(const CNetEventEntityIDPlayerIDAndInteger&) = default;
		CNetEventEntityIDPlayerIDAndInteger& operator=(CNetEventEntityIDPlayerIDAndInteger&&) = default;
		CNetEventEntityIDPlayerIDAndInteger& operator=(const CNetEventEntityIDPlayerIDAndInteger&) = default;

		static inline constexpr int TypeDesc = 0x8125B4;
		static inline constexpr int vtp = 0x77064C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7766FA33);
	};

	class CFeedbackEventBattling : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		shok::Position Pos;
		shok::PlayerId TargetPlayer;

		CFeedbackEventBattling(shok::FeedbackEventIds id, shok::EntityId eid, shok::PlayerId p, const shok::Position& pos, shok::PlayerId tp);
		CFeedbackEventBattling(CFeedbackEventBattling&&) = default;
		CFeedbackEventBattling(const CFeedbackEventBattling&) = default;
		CFeedbackEventBattling& operator=(CFeedbackEventBattling&&) = default;
		CFeedbackEventBattling& operator=(const CFeedbackEventBattling&) = default;

		static inline constexpr int TypeDesc = 0x80FFE4;
		static inline constexpr int vtp = 0x76DDA0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFA523F5E);
	};

	class CFeedbackEventResource : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		shok::ResourceType ResourceType;
		float ExtractionAmount;

		CFeedbackEventResource(shok::FeedbackEventIds id, shok::PlayerId player, shok::EntityId resentity, shok::ResourceType rty, float extrAm);
		CFeedbackEventResource(CFeedbackEventResource&&) = default;
		CFeedbackEventResource(const CFeedbackEventResource&) = default;
		CFeedbackEventResource& operator=(CFeedbackEventResource&&) = default;
		CFeedbackEventResource& operator=(const CFeedbackEventResource&) = default;

		static inline constexpr int TypeDesc = 0x81A3C4;
		static inline constexpr int vtp = 0x774970;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4AFA2726);
	};

	class CNetEventEventTechnologyPlayerIDAndEntityID : public EGL::CNetEventEntityIDAndPlayerID {
	public:
		shok::TechnologyId TechId;

		CNetEventEventTechnologyPlayerIDAndEntityID(shok::NetEventIds id, shok::PlayerId pl, shok::EntityId e, shok::TechnologyId t);

		static inline constexpr int TypeDesc = 0x80FF7C;
		static inline constexpr int vtp = 0x76DD80;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3F0173);
	};

	class CNetEventPlayerResourceDonation : public EGL::CNetEvent2PlayerIDs {
	public:
		shok::ResourceType ResourceType;
		int ResourceAmount;

		CNetEventPlayerResourceDonation(shok::NetEventIds id, shok::PlayerId p1, shok::PlayerId p2, shok::ResourceType rt, int am);

		static inline constexpr int TypeDesc = 0x812580;
		static inline constexpr int vtp = 0x77063C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x531EF206);
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
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3C68E6F3);
	};
}

namespace shok {
	enum class NetEventIds : int {
		Invalid = 0,
		Entity_Move = 0x11002, //EGL::CNetEventEntityAndPosArray
		Player_BuyBuilding = 0x11003, //GGL::CNetEventBuildingCreator
		HQ_BuySerf = 0x11004, //EGL::CNetEventEntityIDAndPlayerID
		Player_SellSerf = 0x11005, //EGL::CNetEventPlayerID deprecated? unused, but probably works
		Building_Sell = 0x11006, //EGL::CNetEventEntityIDAndPlayerID
		Building_UpgradeImmediately = 0x11007, //EGL::CNetEventEntityIDAndPlayerID deprecated? immediately?
		Building_StartUpgrade = 0x11008, //EGL::CNetEventEntityID
		Player_UpgradeBuildingCategory = 0x11009, //EGL::CNetEventIntegerAndPlayerID deprecated?
		Player_UpgradeSettlerCategory = 0x1100A, //EGL::CNetEventIntegerAndPlayerID
		Barracks_BuyLeader = 0x1100B, //GGL::CNetEventEntityIDAndUpgradeCategory
		Leader_BuySoldier = 0x1100C, //EGL::CNetEventEntityID
		Player_LevyTax = 0x1100D, //EGL::CNetEventPlayerID
		Player_SetTaxes = 0x1100E, //EGL::CNetEventIntegerAndPlayerID
		Settler_Expell = 0x1100F, //EGL::CNetEventEntityID
		Worker_BlessSimgle = 0x11010, //EGL::CNetEventEntityID deprecated?
		Worker_OvertimeSingle = 0x11011, //EGL::CNetEventEntityID
		Entity_AttackEntity = 0x11012, //EGL::CNetEvent2Entities
		Leader_AttachOneSoldier = 0x11013, //EGL::CNetEventEntityID
		Leader_ExpellOneSoldier = 0x11014, //EGL::CNetEventEntityID deprecated, handler empty
		Leader_SetFormation = 0x11015, //EGL::CNetEventEntityIDAndInteger
		Serf_ConstructBuilding = 0x11017, //EGL::CNetEvent2Entities
		Serf_RepairBuilding = 0x11018, //EGL::CNetEvent2Entities
		Serf_ExtractResource = 0x11019, //GGL::CNetEventExtractResource
		Building_StartResearch = 0x1101A, //GGL::CNetEventTechnologyAndEntityID
		Building_CancelResearch = 0x1101B, //EGL::CNetEventEntityID
		Market_StartTrade = 0x1101C, //GGL::CNetEventTransaction
		Market_CancelTrade = 0x1101D, //EGL::CNetEventEntityID
		Building_CancelUpgrade = 0x1101E, //EGL::CNetEventEntityID
		Entity_AttackPos = 0x1101F, //EGL::CNetEventEntityAndPos
		Entity_GuardEntity = 0x11020, //EGL::CNetEvent2Entities
		Leader_HoldPosition = 0x11021, //EGL::CNetEventEntityID
		Leader_Aggressive = 0x11022, //EGL::CNetEventEntityID deprecated, leader seems to do the same as defend
		Leader_Defend = 0x11023, //EGL::CNetEventEntityID
		BombPlacer_PlaceBombAt = 0x11024, //EGL::CNetEventEntityAndPos
		Entity_Patrol = 0x11025, //EGL::CNetEventEntityAndPosArray
		Player_PayTribute = 0x11026, //EGL::CNetEventPlayerIDAndInteger
		Player_SetDiplomacy = 0x11027, //EGL::CNetEvent2PlayerIDsAndInteger deprecated?
		Building_SetCurrentMaxWorkers = 0x11028, //EGL::CNetEventEntityIDAndInteger
		Player_AllSerfsToBattleSerfs = 0x1102A, //EGL::CNetEventPlayerID
		Player_AllBattleSerfsToSerfs = 0x1102B, //EGL::CNetEventPlayerID
		HawkOwner_SendHawkToPos = 0x1102C, //EGL::CNetEventEntityAndPos
		BattleSerf_TurnToSerf = 0x1102D, //EGL::CNetEventEntityID
		Serf_TurnToBattleSerf = 0x1102E, //EGL::CNetEventEntityID
		CannonPlacer_HeroPlaceCannonAt = 0x1102F, //GGL::CNetEventCannonCreator
		Player_ActivateAlarm = 0x11030, //EGL::CNetEventPlayerID
		Player_DeactivateAlarm = 0x11031, //EGL::CNetEventPlayerID
		Camouflage_Activate = 0x11032, //EGL::CNetEventEntityID
		Building_ToggleOvertime = 0x11033, //EGL::CNetEventEntityID
		Foundry_BuildCannon = 0x11034, //EGL::CNetEventEntityIDAndIntege
		Summoner_Summon = 0x11035, //EGL::CNetEventEntityID
		Weathermachine_Activate = 0x11036, //EGL::CNetEventIntegerAndPlayerID
		Player_BlessGroup = 0x11037, //EGL::CNetEventIntegerAndPlayerID
		Hero_NPCInteraction = 0x11038, //EGL::CNetEvent2Entities
		AffectEntities_Activate = 0x11039, //EGL::CNetEventEntityID
		CircularAttack_Activate = 0x1103A, //EGL::CNetEventEntityID
		Player_SendMPResources = 0x1103B, //GGL::CNetEventPlayerResourceDonation
		Player_BuyHero = 0x1103C, //EGL::CNetEventEntityIDAndPlayerIDAndEntityType
		ConvertBuilding_Convert = 0x1103D, //EGL::CNetEvent2Entities
		InflictFear_Activate = 0x1103E, //EGL::CNetEventEntityID
		ConvertSettler_Convert = 0x1103F, //EGL::CNetEvent2Entities
		Barracks_RecruitGroups = 0x11040, //EGL::CNetEventEntityID
		Barracks_RecruitLeaderOnly = 0x11041, //EGL::CNetEventEntityID
		Hero_BuyMerchantOffer = 0x11042, //GGL::CNetEventEntityIDPlayerIDAndInteger
		Thief_StealFrom = 0x11043, //EGL::CNetEvent2Entities
		Thief_CarryStolenStuffToHQ = 0x11044, //EGL::CNetEvent2Entities
		Sabotage_Sabotage = 0x11045, //EGL::CNetEvent2Entities
		Sabotage_Defuse = 0x11046, //EGL::CNetEvent2Entities
		MotivateWorkers_Activate = 0x11047, //EGL::CNetEventEntityID
		Binoculars_Observe = 0x11048, //EGL::CNetEventEntityAndPos
		Sniper_Snipe = 0x11049, //EGL::CNetEvent2Entities
		TorchPlacer_Place = 0x1104A, //EGL::CNetEventEntityAndPos
		ResourceFinder_Find = 0x1104B, //EGL::CNetEventEntityID
		Shuriken_Activate = 0x1104C, //EGL::CNetEvent2Entities
		CalculateHash = 0x1D002, //EGL::CEventGetValue<int,1211121895>
		SetGameSpeed = 0x50001, //Framework::CEventGameSpeed
		// 0x50002 replay checksum (from error message)
		Escape = 0x50003, //BB::CEvent

		CppL_LightningStrike_Activate = 0x11100, //EGL::CNetEventEntityAndPos
		CppL_ResDoodadRefill_Activate, //EGL::CNetEvent2Entities
		CppL_ShieldCover_Activate, //EGL::CNetEventEntityID
		CppL_Resurrect_Activate, //EGL::CNetEvent2Entities
		CppL_BombardmentActivate, //EGL::CNetEventEntityAndPos
		CppL_BombComboCannonActivate, //EGL::CNetEventEntityAndPos
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
		FEEDBACK_EVENT_BUILDING_UPGRADE_FINISHED = 0x25005, // GGL::CNetEventEntityIDAndUpgradeCategory
		FEEDBACK_EVENT_LEADER_CHANGE_FORMATION = 0x25006,
		FEEDBACK_EVENT_WEATHER_FORECAST = 0x25007,
	};
}

template <>
struct magic_enum::customize::enum_range<shok::NetEventIds> {
	static constexpr int min = static_cast<int>(shok::NetEventIds::Entity_Move);
	static constexpr int max = static_cast<int>(shok::NetEventIds::Shuriken_Activate);
};
namespace CppLogic {
	template<>
	inline auto GetIdManager<shok::NetEventIds>() {
		return CppLogic::MagicEnum::EnumIdManager<shok::NetEventIds, shok::NetEventIds::Invalid, shok::NetEventIds::SetGameSpeed, shok::NetEventIds::Escape>{};
	}
}

#pragma once
#include "enumflags.h"

namespace shok {
	enum class DiploState : int {
		Friendly = 1,
		Neutral = 2,
		Hostile = 3,
	};

	enum class LeaderCommand : int {
		Unknown = -1,
		Attack = 0,
		Aggressive = 2, // deprecated, seems to do the same as defend
		Defend = 3,
		Patrol = 4,
		AttackMove = 5,
		Guard = 6,
		HoldPos = 7,
		Move = 8,
		HeroAbility = 10,
	};

	enum class AttachmentType : int {
		SETTLER_SOURCE_PILE = 2,
		SETTLER_DESTINATION_PILE = 3,
		SETTLER_COLLECT_GOOD_RESOURCE = 4,
		GATHERER_RESOURCE_DOODAD = 5,
		SETTLER_EMPOLYING_BUILDING = 6,
		SETTLER_TARGET_FIRE = 7,
		CROP_FIELD = 8,
		SETTLER_ENTERED_BUILDING = 9,
		SETTLER_FIELD = 10,
		SETTLER_RESOURCE_DOODAD = 11,
		SETTLER_HARBOR = 12,
		SETTLER_ENTITY_TO_DESTROY = 13,
		SETTLER_TARGET_BUILDING = 14,
		SETTLER_WORKPLACE = 15,
		HUNTER_PREY = 16,
		MILITIA_ENEMY = 17,
		APPROACHING_SERF_CONSTRUCTION_SITE = 18,
		SERF_CONSTRUCTION_SITE = 19,
		CONSTRUCTION_SITE_BUILDING = 20,
		WORKER_FARM = 21,
		WORKER_RESIDENCE = 22,
		WORKER_WORKPLACE = 23,
		MINE_RESOURCE = 24,
		MINE_LORRY = 25,
		MINE_LORRY_STORE = 26,
		CONSUMER_LORRY = 27,
		MARKET_LORRY_STORE = 28,
		SERF_MARKET = 29,
		MARKET_MARKET = 30,
		LEADER_SOLDIER = 31,
		ATTACKER_TARGET = 32, // currently in combat engaged (not walking towarts to attack)
		ATTACKED_DEAD = 33,
		CAMP_SETTLER = 34,
		ATTACKER_COMMAND_TARGET = 35, // targeting to engage in combat
		BUILDING_BASE = 36,
		FOLLOWER_FOLLOWED = 37,
		WORKER_VILLAGE_CENTER = 38,
		PLACEHOLDER_BUILDING_CONSTRUCTION_SITE = 39,
		SERF_RESOURCE = 40,
		GUARD_GUARDED = 41,
		FIGHTER_BARRACKS = 42,
		BOMBER_BOMB = 43,
		SERF_BATTLE_SERF = 44,
		HERO_HAWK = 45,
		TOP_ENTITY_FOUNDATION = 46,
		BUILDER_FOUNDATION = 47,
		DEFENDER_BUILDING = 48,
		APPROACHING_DEFENDER_BUILDING = 49,
		//[[ Missing ]]
		LEADER_TARGET = 51, // command to attack (via direct command or autoassigned), might be garbage, check current command
		WORKER_SUPPLIER = 52,
		SETTLER_BUILDING_TO_LEAVE = 53,
		SUMMONER_SUMMONED = 54,
		FOUNDATION_TOP_ENTITY = 55,
		FOUNDATION_BUILDER = 56,
		BUILDING_UPGRADE_SITE = 57,
		HERO_AFFECTED = 58,
		HERO_NPC = 59,
		CONVERTER_BUILDING = 60,
		INFLICTOR_TERRORIZED = 61,
		CONVERTER_SETTLER = 62,
		MERCHANT_TRADER = 63,
		BUILDER_BRIDGE = 64,
		APPROACHING_BUILDER_BRIDGE = 65,
		SCOUT_EXPLORATION = 66,
		THIEF_TARGET_BUILDING = 67,
		THIEF_SECURE_BUILDING = 68,
		KEG_TARGET_BUILDING = 69,
		ARMING_THIEF_KEG = 70,
		DISARMING_THIEF_KEG = 71,
		THIEF_KEG_TARGET = 72,
		FIRE_BURNING_ENTITY = 73,
	};

	enum class EntityCategory : int {
		CavalryHeavy = 1,
		CavalryLight = 2,
		Spear = 3,
		Bow = 4,
		Sword = 5,
		VillageCenter = 6,
		Headquarters = 7,
		Scout = 8,
		Thief = 9,
		Rifle = 10,
		Cannon = 11,
		Worker = 12,
		Serf = 13,
		Hero8 = 14,
		Hero7 = 15,
		Hero6 = 16,
		Hero5 = 17,
		Hero4 = 18,
		Hero3 = 19,
		Hero2 = 20,
		Hero1 = 21,
		Hero = 22,
		Military = 23,
		ResourcePit = 24,
		Coiner = 25,
		Hero11 = 26,
		Hero10 = 27,
		Soldier = 28,
		Leader = 29,
		SplashDamage = 30,
		EvilLeader = 31,
		LongRange = 32,
		Melee = 33,
		ThiefRefuge = 34,
		NeutralBridge = 35,
		SerfConversionBuilding = 36,
		Bridge = 37,
		Wall = 38,
		SulfurSupplier = 39,
		IronSupplier = 40,
		ClaySupplier = 41,
		WoodSupplier = 42,
		StoneSupplier = 43,
		GoldSupplier = 44,
		DefendableBuilding = 45,
		Barracks = 46,
		MilitaryBuilding = 47,
		Workplace = 48,
		Farm = 49,
		Residence = 50,
		TargetFilter_CustomRanged = 102,
		TargetFilter_NonCombat = 103,
		TargetFilter_TargetType = 100,
		TargetFilter_TargetTypeLeader = 101,
	};

	enum class TechState : int {
		Forbidden = 0,
		Waiting = 1, // fake, all tech requirements have their tech requirements fullfilled or are researched
		Allowed = 2, // can be researched at this time
		InProgress = 3,
		Researched = 4,
		Future = 5, // fake, Allowed, but not Waiting
	};

	enum class ResourceType : int {
		None = 0,
		Gold = 1,
		GoldRaw = 2,
		Silver = 3,
		SilverRaw = 4,
		Stone = 5,
		StoneRaw = 6,
		Iron = 7,
		IronRaw = 8,
		Sulfur = 9,
		SulfurRaw = 10,
		Clay = 11,
		ClayRaw = 12,
		Wood = 13,
		WoodRaw = 14,
		WeatherEnergy = 15,
		Knowledge = 16,
		Faith = 17,
	};
	constexpr int ResourceType_MaxValue = 17;

	enum class Goods : int {
		XG_Gold = 2,
		XG_Iron = 1,
		XG_Stone = 3,
		XG_Sulfur = 4,
		XVG_Gold = 6,
		XVG_Iron = 7,
		XVG_Stone = 5,
		XVG_Sulfur = 8,
	};

	enum class Keys : int {
		None = 0,
		A = 65,
		Add = 107,
		B = 66,
		Back = 8,
		C = 67,
		Control = 17,
		D = 68,
		D0 = 48,
		D1 = 49,
		D2 = 50,
		D3 = 51,
		D4 = 52,
		D5 = 53,
		D6 = 54,
		D7 = 55,
		D8 = 56,
		D9 = 57,
		Decimal = 110,
		Delete = 46,
		Divide = 111,
		Down = 40,
		E = 69,
		End = 35,
		Enter = 13,
		Escape = 27,
		F = 70,
		F1 = 112,
		F10 = 121,
		F11 = 122,
		F12 = 123,
		F2 = 113,
		F3 = 114,
		F4 = 115,
		F5 = 116,
		F6 = 117,
		F7 = 118,
		F8 = 119,
		F9 = 120,
		G = 71,
		H = 72,
		Home = 36,
		I = 73,
		Insert = 45,
		J = 74,
		K = 75,
		L = 76,
		Left = 37,
		M = 77,
		MaskCode = 65535,
		MaskModifiers = -65536,
		Menu = 18,
		ModifierAlt = 262144,
		ModifierControl = 131072,
		ModifierShift = 65536,
		MouseLButton = 1,
		MouseMButton = 4,
		MouseRButton = 2,
		Multiply = 106,
		N = 78,
		Next = 34,
		NumLock = 144,
		NumPad0 = 96,
		NumPad1 = 97,
		NumPad2 = 98,
		NumPad3 = 99,
		NumPad4 = 100,
		NumPad5 = 101,
		NumPad6 = 102,
		NumPad7 = 103,
		NumPad8 = 104,
		NumPad9 = 105,
		O = 79,
		OemBackslash = 226,
		OemCloseBrackets = 221,
		OemComma = 188,
		OemMinus = 189,
		OemOpenBrackets = 219,
		OemPeriod = 190,
		OemPipe = 220,
		OemPlus = 187,
		OemQuestion = 191,
		OemQuotes = 222,
		OemSemicolon = 186,
		OemTilde = 192,
		P = 80,
		Pause = 19,
		Print = 42,
		Prior = 33,
		Q = 81,
		R = 82,
		Right = 39,
		S = 83,
		Scroll = 145,
		Separator = 108,
		Shift = 16,
		Snapshot = 44,
		Space = 32,
		Subtract = 109,
		T = 84,
		Tab = 9,
		U = 85,
		Up = 38,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
	};
	constexpr shok::Keys operator&(shok::Keys a, shok::Keys b) {
		using under = std::underlying_type<shok::Keys>::type;
		return static_cast<shok::Keys>(static_cast<under>(a) & static_cast<under>(b));
	}
	constexpr shok::Keys operator|(shok::Keys a, shok::Keys b) {
		using under = std::underlying_type<shok::Keys>::type;
		return static_cast<shok::Keys>(static_cast<under>(a) | static_cast<under>(b));
	}
	constexpr shok::Keys operator^(shok::Keys a, shok::Keys b) {
		using under = std::underlying_type<shok::Keys>::type;
		return static_cast<shok::Keys>(static_cast<under>(a) ^ static_cast<under>(b));
	}
	constexpr shok::Keys operator~(shok::Keys a) {
		using under = std::underlying_type<shok::Keys>::type;
		return static_cast<shok::Keys>(~static_cast<under>(a));
	}

	enum class ExperienceClass : int {
		Invalid = -1,
		Sword = 0,
		Bow = 1,
		Spear = 2,
		LightCavalry = 3,
		HeavyCavalry = 4,
		Cannon = 5,
		Rifle = 6, // also scout and thief
	};
	enum class AccessCategory : int {
		AccessCategoryNone = 0,
		AccessCategorySettler = 1,
		AccessCategoryAnimal = 2,
		AccessCategoryBuilding = 3,
		AccessCategoryResourceDoodad = 4,
		AccessCategoryStatic = 5,
		AccessCategoryOrnamental = 6,
	};
	enum class AccessCategoryFlags : int {
		None = 0,
		AccessCategoryNone = 1,
		AccessCategorySettler = 1 << 1,
		AccessCategoryAnimal = 1 << 2,
		AccessCategoryBuilding = 1 << 3,
		AccessCategoryResourceDoodad = 1 << 4,
		AccessCategoryStatic = 1 << 5,
		AccessCategoryOrnamental = 1 << 6,
	};
	template<>
	class ::enum_is_flags<AccessCategoryFlags> : public std::true_type {};
	enum class WeatherState : int {
		Invalid = 0,
		Summer = 1,
		Rain = 2,
		Winter = 3,
	};
	enum class WorkerReason : int {
		None = 0,
		Taxes = 1,
		Overtime = 2, // 'Too Much Work' in xml
		NoWork = 3,
		NoFood = 4,
		NoRest = 5,
		NoPay = 6,
	};
	enum class WorkerGrievance : int {
		Happy = 1,
		Sad = 2,
		Angry = 3,
		Leaving = 4,
	};

	enum class MapType : int {
		Campagn = -1,
		Singleplayer = 0,
		Development = 1,
		Multiplayer = 2,
		User = 3, // s5x and folder, sp and mp
	};

	enum class PlayerId : int {
		Invalid = -1,
		P0 = 0,
		P1 = 1,
	};
	enum class EntityId : int {
		Invalid = 0,
	};
	enum class ModelId : int {
		Invalid = 0,
	};
	enum class SectorId : int {
		Invalid = 0, // in most cases => blocked
	};
	enum class TechnologyId : int {
		Invalid = 0,
	};
	enum class TaskListId : int {
		Invalid = 0,
	};
	enum class AnimationId : int {
		Invalid = 0,
	};
	enum class WidgetId : int {
		Invalid = 0,
	};
	enum class WidgetGroupId : int {
		Invalid = 0,
	};
	enum class FontId : int {
		Invalid = 0,
	};
	enum class UpgradeCategoryId : int {
		Invalid = 0,
	};
	enum class ClassId : unsigned int {
		Invalid = 0xEEFFFFFF, // guranteed to not appear in shok
	};
	enum class GUITextureId : int {
		Invalid = 0,
	};
	enum class EntityTypeId : int {
		Invalid = 0,
	};
}

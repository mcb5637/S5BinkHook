#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>

namespace GAI {
	class CObject {
		virtual void setbool();
		virtual void clearbool();
		virtual void empty();
	public:
		virtual void Tick();
		virtual ~CObject() = default;

		PADDINGI(1); // some type?
		PADDINGI(1); // bool?
		PADDINGI(1); // object number?

		static inline constexpr int vtp = 0x766ADC;

		//ctor 44841E
	};
	class CPlayer;

	class CAgent {
		virtual void ClearType();
		virtual void dosomecalc();
		virtual bool TypeNot0();
		virtual void Clear();
		virtual void uk();
		virtual void uk1();
	public:
		virtual ~CAgent() = default;


		static inline constexpr int vtp = 0x7672D8;

		// ctor 451524
	};

	class CAgentSearch : public CAgent {
	public:

		PADDINGI(2);
		PADDINGI(2); // pos +{4,0}?
		shok::Position TargetPos;
		PADDINGI(2); // {0,10}?
		shok::EntityTypeId Type;
		PADDINGI(1); //1 bool
		int NumberOfUpgrades;
		PADDINGI(1);
		float ExpansionRadius;
		PADDINGI(1); //1 bool
		PADDINGI(1); // p to something?


		static inline constexpr int vtp = 0x767338;

		// ctor 451A17
	};
	static_assert(sizeof(CAgentSearch) == 16 * 4);
	static_assert(offsetof(CAgentSearch, TargetPos) == 5 * 4);
	static_assert(offsetof(CAgentSearch, Type) == 9 * 4);
	static_assert(offsetof(CAgentSearch, NumberOfUpgrades) == 11 * 4);

	class CAgentResearch : public CAgent {
	public:
		PADDINGI(2);
		PADDINGI(1);
		float Probability;
		shok::PlayerId Player; //5
		int Type; // 2 tech, 1 upgrade
		union {
			shok::TechnologyId Tech;
			shok::EntityTypeId Upgrade; // type?
		} ToResearch; // id of tech or upgrade
		int Loc; // ???


		static inline constexpr int vtp = 0x767318;

		// ctor 451534
	};
	static_assert(sizeof(CAgentResearch) == 9 * 4);
	static_assert(offsetof(CAgentResearch, Probability) == 4 * 4);

	class CAgentUpgrade : public CAgent {
	public:
		PADDINGI(1);
		int TargetUpgradeLevel;
		shok::EntityTypeId Type;
		shok::PlayerId Player;
		shok::Position Pos;

		static inline constexpr int vtp = 0x7672F8;

		// ctor 451261
	};
	static_assert(sizeof(CAgentUpgrade) == 7 * 4);

	enum class AIFocus : int {
		XD_Iron1 = 1,
		XD_IronPit1 = 4,
		XD_Sulfur1 = 5,
		XD_SulfurPit1 = 6,
		XD_Stone1 = 7,
		XD_StonePit1 = 8,
		XD_Clay1 = 10,
		XD_ClayPit1 = 11,
		PU_Serf = 12,
		PB_Barracks = 13,
		PB_Stable = 14,
		PB_Archery = 15,
		PB_Foundry = 16,
		PB_Farm = 17,
		PB_Residence = 18,
		PB_Monastery = 19,
		PB_Tower = 20,
		PB_Sawmill = 21,
		PB_Brickworks = 22,
		PB_University = 23,

		PB_Bank = 25,
		PB_Blacksmith = 26,
		PB_StoneMason = 27,
		PB_Alchemist = 28,
		PB_VillageCenter1 = 29, // only 1

		PB_IronMine = 31, // only 1 & 2
		PB_SulfurMine = 32,
		PB_StoneMine = 33,
		PB_ClayMine = 34,

		ConstructionSite = 35, // ?
	};
	static inline AIFocus(__cdecl* const EntityTypeToResFocus)(shok::EntityTypeId t) = reinterpret_cast<AIFocus(__cdecl*)(shok::EntityTypeId)>(0x4495F6);

	class CVillage : public CObject {
	public:
		CPlayer* Player; // 4
		struct {

			float LastUpdate, UpdateDelta;
			int SerfLimit;
			bool Constructing, Repairing, Extracting;
			CVillage* Village;
			PADDINGI(2);
			AIFocus ResFocus;

			// ety to res focus 4495F6
		} SerfManager; // 5
		struct {
			CVillage* Village;
			PADDINGI(2);
			shok::Vector<CAgentSearch> ConstructionQueue; // 3
			shok::Vector<CAgentUpgrade> UpgradeQueue;
			shok::Vector<CAgentResearch> ReseachQueue; // 11
			PADDINGI(3);
			shok::EntityId ConstructionOrRepairTarget; // csite if construction, building if repair
			float ExpansionRadius; //19

			// check building placement 44CD3C
		} V2; // 13

		static inline constexpr int vtp = 0x767484;

		// ctor 44FBA3
	};
	static_assert(offsetof(CVillage, SerfManager) == 5 * 4);
	static_assert(offsetof(CVillage, Player) == 4 * 4);
	static_assert(offsetof(CVillage, V2) == 13 * 4);
	static_assert(offsetof(CVillage, V2.ExpansionRadius) == (13+19) * 4);
	static_assert(offsetof(CVillage, V2.ConstructionQueue) == (13 + 3) * 4);
	static_assert(offsetof(CVillage, V2.ReseachQueue) == (13 + 11) * 4);

	class CArmy : public CObject {
	public:
		virtual void OnTick();

		bool AlwaysAggressive; //4
		PADDINGI(3);
		struct {
			CArmy* Army;
			PADDINGI(3);
			struct {
				shok::Position CurrentPos; // /100
				PADDINGI(3);
			} U2;
			int ArmyFormationSize; // 1,0->big, small deprecated???
			struct {
				PADDINGI(2); // 10
				float FormationScatterTolerance; // [5-300] spacing between leaders?
			} U1;
			PADDINGI(4);
			PADDINGI(5);
			PADDINGI(14);
			shok::Vector<void*> SomeVector; // 36
			PADDINGI(6);
			PADDINGI(6);
			PADDINGI(6);
			PADDINGI(6);
			PADDINGI(1);
			int ArmySize;
		} U1; // 8
		struct {
			void* SomeArmySubObj; // CArmy+12*4
			float RodeLength; // /100
			PADDINGI(5);
			PADDINGI(1); //bool?
		} U2;
		PADDINGI(2);

		static inline constexpr int vtp = 0x767378;

		// ctor 44D20A
	};
	static_assert(sizeof(CArmy) == 84 * 4);
	static_assert(offsetof(CArmy, U1) == 8 * 4);
	static_assert(offsetof(CArmy, U1.SomeVector) == (8+36) * 4);
	static_assert(offsetof(CArmy, U2) == 74 * 4);
	static_assert(offsetof(CArmy, U1.U1.FormationScatterTolerance) == 20 * 4);
	static_assert(offsetof(CArmy, U1.U2.CurrentPos) == 12 * 4);

	class CArmyFighting : public CArmy {
	public:

		static inline constexpr int vtp = 0x7674B8;

		// ctor 44FDAE
	};
	static_assert(sizeof(CArmyFighting) == 84 * 4);

	class CPlayer : public CObject {
	public:
		bool Active = false; // 2 bools
		shok::PlayerId Player; //5
		PADDINGI(1);
		CVillage Village; // 7
		CArmyFighting Armies[10];

		// 1947 vector of CEntityDynamic* serfs
		// 1951 vector of CEntityDynamic* not serfs

		static inline constexpr int vtp = 0x766A38;

		// ctor 447787
		// tick 444C9C
		// set active 4472B2 also sets isai in GGL attractionhandler
	};
	static_assert(offsetof(CPlayer, Player) == 5 * 4);
	static_assert(offsetof(CPlayer, Village) == 7 * 4);
	static_assert(offsetof(CPlayer, Armies) == 40 * 4);
	static_assert(offsetof(CPlayer, Armies[0].U1.ArmySize) == 113 * 4);
	static_assert(offsetof(CPlayer, Armies[0].U2.RodeLength) == 115 * 4);

	class CEntity : public CObject {
	public:
		virtual void Init(shok::EntityId id, CPlayer* player) = 0; // 5

		enum class Category : int {
			Invalid = -1, // not entity or settler
			Bow = 2, // or cannon
			Sword = 3, // or anything else
			Cavalry = 4,
			Spear = 5,
			Building = 7,
			HQ = 8,
		};

		shok::EntityId EntityId, EntityAgain;
		shok::PlayerId PlayerId;
		CPlayer* Player; // 7
		Category Cat;

		static inline constexpr int vtp = 0x7674E4;

		// ctor 450378
	};

	class CEntityDynamic : public CEntity {
	public:
		PADDINGI(6);
		PADDINGI(10);
		PADDINGI(2);
		struct {
			PADDINGI(3);
			int MaxSoldiers = 0;
			PADDINGI(5);
		} U2; //27
		struct {
			CEntityDynamic* E = nullptr;
			bool IsSerf = false;
			PADDINGI(3);
		} U1;
		PADDINGI(3);
		shok::ArmyId Army{}; // 44

		static inline constexpr int vtp = 0x766A70;

		// ctor 44786B
	};
	static_assert(sizeof(CEntityDynamic) == 0xb4);
	static_assert(offsetof(CEntityDynamic, U1.IsSerf) == 148);

	class CEntityStatic : public CEntity {
	public:

		static inline constexpr int vtp = 0x766AA8;

		// ctor 447ECB
	};

	struct AIHolder {
		bool Init = true;
		struct {
			shok::Vector<CPlayer> Players;
			shok::Vector<CEntityDynamic*> Dynamic;
			shok::Vector<CEntityStatic*> Static;
		}* Data = nullptr;

		// ctor 444C58
		// tick 444C9C

		static inline AIHolder* (__stdcall* GlobalObj)() = reinterpret_cast<AIHolder* (__stdcall*)()>(0x4023D3);
		CPlayer* GetPlayer(shok::PlayerId i);
		// searches static and dynamic
		CEntity* GetEntity(shok::EntityId id);
		// getentity dynamic 447258
	};
}

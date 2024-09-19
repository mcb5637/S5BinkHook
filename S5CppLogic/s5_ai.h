#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

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

	class CVillage : public CObject {
	public:
		CPlayer* Player;
		struct {
			enum class ResourceFocus : int; // todo investigate

			float LastUpdate, UpdateDelta;
			int SerfLimit;
			bool Constructing, Repairing, Extracting;
			CVillage* Village;
			PADDINGI(2);
			ResourceFocus ResFocus;

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
	static_assert(offsetof(CVillage, V2) == 13 * 4);
	static_assert(offsetof(CVillage, V2.ExpansionRadius) == (13+19) * 4);
	static_assert(offsetof(CVillage, V2.ConstructionQueue) == (13 + 3) * 4);
	static_assert(offsetof(CVillage, V2.ReseachQueue) == (13 + 11) * 4);

	class CArmy : public CObject {

		static inline constexpr int vtp = 0x767378;

		// ctor 44D20A
	};

	class CArmyFighting : public CArmy {

		static inline constexpr int vtp = 0x7674B8;

		// ctor 44FDAE
	};

	class CPlayer : public CObject {
	public:
		bool Active = false; // 2 bools
		shok::PlayerId Player; //5
		PADDINGI(1);
		CVillage Village; // 7
		int firstarmy;

		// 1947 vector of serfs?

		static inline constexpr int vtp = 0x766A38;

		// ctor 447787
		// tick 444C9C
		// set active 4472B2 also sets isai in GGL attractionhandler
	};
	static_assert(offsetof(CPlayer, Player) == 5 * 4);
	static_assert(offsetof(CPlayer, Village) == 7 * 4);
	static_assert(offsetof(CPlayer, firstarmy) == 40 * 4);


	struct AIHolder {
		bool Init = true;
		shok::Vector<CPlayer>* Players;

		// ctor 444C58
		// tick 444C9C

		static inline AIHolder* (__stdcall* GlobalObj)() = reinterpret_cast<AIHolder* (__stdcall*)()>(0x4023D3);
		CPlayer* GetPlayer(shok::PlayerId i);
	};
}

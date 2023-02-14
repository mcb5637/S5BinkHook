#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace shok {
	enum class TechModifierType : int {
		SpeedModifier = 0, // technically this is a default in a switch case...
		ExplorationModifier = 1,
		HitpointModifier = 2,
		DamageModifier = 3,
		ArmorModifier = 4,
		DodgeModifier = 5,
		RangeModifier = 6,
		MinRangeModifier = 7,
		DamageBonusModifier = 8,
		GroupLimitModifier = 9,
	};

	class Technology {
	public:
		struct Modifier {
			float Value = 0;
			shok::String Operator;

			float ModifyValue(float i) const;
			Modifier();
		};
		struct TechReq {
			int TecType = 0;
			int TecCategoryType = 0;
		};
		struct ETypeReq {
			int EntityType = 0;
			int Amount = 0;
		};
		struct ECatReq {
			int EntityCategory = 0;
			int Amount = 0;
		};
		struct UCatReq {
			int UpgradeCategory = 0;
			int Amount = 0;
		};



		int TecCategoryType = 0;
		float TimeToResearch = 0;
		bool AutomaticResearch = true;
		PADDING(3);
		shok::CostInfo ResourceCosts;
		int RequiredTecConditions = 0;
		shok::Vector<TechReq> TecConditions;
		int RequiredEntityConditions = 0;
		shok::Vector<ETypeReq> EntityConditions;
		int RequiredEntityCategoryConditions = 0;
		shok::Vector<ECatReq> EntityCategoryConditions;
		int RequiredUpgradeCategoryConditions = 0;
		shok::Vector<UCatReq> UpgradeCategoryConditions; // 38
		shok::String Effect_Script; // 41 Effect -> Script, not sure if this is used for something
		Modifier ExplorationModifier; // 48
		Modifier SpeedModifier; // 56
		Modifier HitpointModifier; // 64 unused (except CppLogic)
		Modifier DamageModifier; // 72 also affects ciruclarattack/shuriken
		Modifier DamageBonusModifier; // 80
		Modifier RangeModifier; // 88
		Modifier MinRangeModifier; // 96
		Modifier ArmorModifier; // 104
		Modifier DodgeModifier; // 112
		Modifier GroupLimitModifier; // 120 unused
		bool UseForStatisticsFlag = false; // 128

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x8645C8);

		void* operator new(size_t s);
		void operator delete(void* p);

		Modifier& GetModifierOfType(TechModifierType t);
	};
	static_assert(sizeof(Technology) == 0x204);

	class TechManager {
	public:
		shok::Vector<shok::Technology*> Techs;

		void FreeTech(int id);
		void LoadTech(int id);
		void PopTech(int id);
	};

	class TechCategoryManager {
	public:
		BB::CIDManagerEx* TechCategoryManager;
		shok::Vector<int*> TechnologyCategoryToFirstTechnologyType;
		// gets loaded from Data\\Config\\TechnologyCategories.xml into temp vector, then creates this vector out of it
	};

	struct AdditionalTechModifier {
		int TechID;
		float Value;
		char Operator;

		float ModifyValue(float i) const;
	};
}

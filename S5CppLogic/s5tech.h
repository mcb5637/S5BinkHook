#pragma once
#include "s5data.h"

namespace shok {
	class Technology {
	public:
		struct Modifier {
			float Value;
			shok::String Operator;

			float ModifyValue(float i) const;
		};
		struct TechReq {
			int TecType;
			int TecCategoryType;
		};
		struct ETypeReq {
			int EntityType;
			int Amount;
		};
		struct ECatReq {
			int EntityCategory;
			int Amount;
		};
		struct UCatReq {
			int UpgradeCategory;
			int Amount;
		};



		int TecCategoryType;
		float TimeToResearch;
		byte AutomaticResearch;
		PADDING(3);
		shok::CostInfo ResourceCosts;
		int RequiredTecConditions;
		shok::Vector<TechReq> TecConditions;
		int RequiredEntityConditions;
		shok::Vector<ETypeReq> EntityConditions;
		int RequiredEntityCategoryConditions;
		shok::Vector<ECatReq> EntityCategoryConditions;
		int RequiredUpgradeCategoryConditions;
		shok::Vector<UCatReq> UpgradeCategoryConditions; // 38
		PADDINGI(7); // 41 Effect -> Script, size 7
		Modifier ExplorationModifier; // 48
		Modifier SpeedModifier; // 56
		Modifier HitpointModifier; // 64
		Modifier DamageModifier; // 72
		Modifier DamageBonusModifier; // 80
		Modifier RangeModifier; // 88
		Modifier MinRangeModifier; // 96
		Modifier ArmorModifier; // 104
		Modifier DodgeModifier; // 112
		Modifier GroupLimitModifier; // 120
		byte UseForStatisticsFlag; // 128
	};

	struct AdditionalTechModifier {
		int TechID;
		float Value;
		char Operator;

		float ModifyValue(float i) const;
	};
}

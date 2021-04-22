#pragma once
#include "s5data.h"

struct shok_technologyRequirementEType {
	int EntityType;
	int Amount;
};
struct shok_technologyRequirementTech {
	int TecType;
	int TecCategoryType;
};
struct shok_technologyRequirementUCat {
	int UpgradeCategory;
	int Amount;
};
struct shok_technologyModifier {
	float Value;
	PADDINGI(1)
		char Operator;
	PADDING(3)
};
struct shok_technology {
	PADDINGI(1)
		float TimeToResearch;
	PADDINGI(1)
		shok_costInfo ResourceCosts;
	int RequiredTecConditions;
	vector_padding
		std::vector<shok_technologyRequirementTech, shok_allocator<shok_technologyRequirementTech>> TecConditions;
	int RequiredEntityConditions;
	vector_padding
		std::vector<shok_technologyRequirementEType, shok_allocator<shok_technologyRequirementEType>> EntityConditions;
	PADDINGI(5)
		int RequiredUpgradeCategoryConditions;
	vector_padding
		std::vector<shok_technologyRequirementUCat, shok_allocator<shok_technologyRequirementUCat>> UpgradeCategoryConditions;
	PADDINGI(7)
		shok_technologyModifier ExplorationModifier;
	PADDINGI(5)
		shok_technologyModifier SpeedModifier;
	PADDINGI(13)
		shok_technologyModifier DamageModifier;
	PADDINGI(13)
		shok_technologyModifier RangeModifier;
	PADDINGI(13)
		shok_technologyModifier ArmorModifier;
};

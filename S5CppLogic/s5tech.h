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
struct shok_technologyRequirementECat {
	int EntityCategory;
	int Amount;
};
struct shok_technologyModifier {
	float Value;
	PADDINGI(1);
	char Operator;
	PADDING(3);
	PADDINGI(5); // most likely a shok::String

	float ModifyValue(float i);
};
struct shok_technology {
	int TecCategoryType;
	float TimeToResearch;
	byte AutomaticResearch;
	PADDING(3);
	shok::CostInfo ResourceCosts;
	int RequiredTecConditions;
	vector_padding;
	std::vector<shok_technologyRequirementTech, shok::Allocator<shok_technologyRequirementTech>> TecConditions;
	int RequiredEntityConditions;
	vector_padding;
	std::vector<shok_technologyRequirementEType, shok::Allocator<shok_technologyRequirementEType>> EntityConditions;
	int RequiredEntityCategoryConditions;
	vector_padding;
	std::vector<shok_technologyRequirementUCat, shok::Allocator<shok_technologyRequirementUCat>> EntityCategoryConditions;
	int RequiredUpgradeCategoryConditions;
	vector_padding;
	std::vector<shok_technologyRequirementUCat, shok::Allocator<shok_technologyRequirementUCat>> UpgradeCategoryConditions; // 38
	PADDINGI(7); // 41 Effect -> Script, size 7
	shok_technologyModifier ExplorationModifier; // 48
	shok_technologyModifier SpeedModifier; // 56
	shok_technologyModifier HitpointModifier; // 64
	shok_technologyModifier DamageModifier; // 72
	shok_technologyModifier DamageBonusModifier; // 80
	shok_technologyModifier RangeModifier; // 88
	shok_technologyModifier MinRangeModifier; // 96
	shok_technologyModifier ArmorModifier; // 104
	shok_technologyModifier DodgeModifier; // 112
	shok_technologyModifier GroupLimitModifier; // 120
	byte UseForStatisticsFlag; // 128
};

struct additionalTechModifier {
	int TechID;
	float Value;
	char Operator;

	float ModifyValue(float i);
};

extern std::vector<additionalTechModifier> ConstructionSpeedModifiers;
void EnableConstructionSpeedTechs();

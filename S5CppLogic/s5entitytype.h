#pragma once
#include "s5data.h"


struct shok_modifyEntityProps {
	int MysteriousInt;
	vector_padding;
	std::vector<int, shok_allocator<int>> TechList;
};


struct shok_EGL_CGLEEntityProps : shok_object {
	PADDINGI(1);
	int Class;
	vector_padding;
	std::vector<shok_EntityCategory, shok_allocator<shok_EntityCategory>> Categories;
	shok_positionRot ApproachPos;
private:
	int Race;
public:
	byte CanFloat, CanDrown, MapFileDontSave, DividesTwoSectors, ForceNoPlayer, AdjustWalkAnimSpeed, Visible, DoNotExecute;
	int MaxHealth; // 13
private:
	int Models[5];
public:
	float Exploration;
	int ExperiencePoints, AccessCategory, NumBlockedPoints;
	float SnapTolerance;
	byte DeleteWhenBuiltOn, NeedsPlayer;
	PADDING(2);
	PADDINGI(9);
	vector_padding;
	std::vector<shok_AARect, shok_allocator<shok_AARect>> BlockingArea; // la37

	static inline constexpr int vtp = 0x76E47C;
};

struct shok_GGL_CEntityProperties : shok_EGL_CGLEEntityProps {
	int ResourceEntity, ResourceAmount, SummerEffect, WinterEffect;

	static inline constexpr int vtp = 0x776FEC;
};

struct shok_upgradeInfo {
	float Time;
	shok_costInfo Cost;
	int Type, Category;
};
struct shok_GGL_CGLSettlerProps : shok_EGL_CGLEEntityProps {
private:
	int HeadSet, Hat;
public:
	shok_costInfo Cost;
	float BuildFactor, RepairFactor;
	int ArmorClass;
	int ArmorAmount; // 61
	int DodgeChance;
	int IdleTaskList;
	shok_upgradeInfo Upgrade;
	byte Fearless, Convertible;
	PADDING(2);
	shok_modifyEntityProps ModifyExploration, ModifyHitpoints, ModifySpeed, ModifyDamage, ModifyArmor, ModifyDodge, ModifyMaxRange, ModifyMinRange, ModifyDamageBonus, ModifyGroupLimit;
	int AttractionSlots;

	static inline constexpr int vtp = 0x76E498;
};

struct shok_GGL_CGLAnimalProps : shok_EGL_CGLEEntityProps {
	int DefaultTaskList;
	float TerritoryRadius, WanderRangeMin, WanderRangeMax, ShyRange, MaxBuildingPollution;
	int FleeTaskList;

	static inline constexpr int vtp = 0x779074;
};

struct shok_GGL_CBuildBlockProperties : shok_EGL_CGLEEntityProps {
	shok_AARect BuildBlockArea; // la 41

	static inline constexpr int vtp = 0x76EB38;
};

struct shok_GGL_CResourceDoodadProperties : shok_GGL_CBuildBlockProperties {
	float Radius;
	shok_position Center, LineStart, LineEnd;
	int ExtractTaskList, Model1, Model2;

	static inline constexpr int vtp = 0x76FF68;
};

struct shok_constructionInfo {
	PADDINGI(1);
	vector_padding;
	std::vector<shok_positionRot, shok_allocator<shok_positionRot>> BuilderSlot;
	int Time;
	shok_costInfo Cost;
	int ConstructionSite;
};
struct shok_GGL_CGLBuildingProps : shok_GGL_CBuildBlockProperties {
	int MaxWorkers, InitialMaxWorkers, NumberOfAttractableSettlers, Worker;
	shok_position DoorPos, LeavePos;
	shok_constructionInfo ConstructionInfo;
	vector_padding;
	std::vector<int, shok_allocator<int>> BuildOn;
	byte HideBase, CanBeSold, IsWall;
	PADDING(1);
	shok_upgradeInfo Upgrade;
	int UpgradeSite, ArmorClass, ArmorAmount;
	vector_padding;
	std::vector<int, shok_allocator<int>> WorkTaskList;
private:
	int MilitaryInfo[4];
public:
	float CollapseTime;
	byte Convertible;
	PADDING(3);
	shok_modifyEntityProps ModifyExploration, ModifyArmor;
	float KegEffectFactor; // 124

	static inline constexpr int vtp = 0x76EC78;
};

struct shok_GGL_CBridgeProperties : shok_GGL_CGLBuildingProps {
	shok_AARect BridgeArea;
	int Height, ConstructionModel0, ConstructionModel1, ConstructionModel2;

	static inline constexpr int vtp = 0x778148;
};

struct shok_ED_CDisplayEntityProps : shok_object {
	int DisplayClass;
	int Model[4];
	byte DrawPlayerColor, CastShadow, RenderInFoW, HighQualityOnly, MapEditor_Rotateable, MapEditor_Placeable;
	PADDING(2);
	vector_padding;
	std::vector<int, shok_allocator<int>> AnimList;

	static inline constexpr int vtp = 0x788840;
};

struct shok_GGlue_CGlueEntityProps : shok_object {
	PADDINGI(1);
	shok_EGL_CGLEEntityProps* LogicProps;
	shok_ED_CDisplayEntityProps* DisplayProps;
	vector_padding;
	std::vector<shok_EGL_CGLEBehaviorProps*, shok_allocator<shok_EGL_CGLEBehaviorProps*>> BehaviorProps;

	static inline constexpr int vtp = 0x788824;


private:
	shok_EGL_CGLEBehaviorProps* SearchBehaviorProp(int* vts, int num);
	shok_EGL_CGLEBehaviorProps* SearchBehaviorProp(int vt);

public:
	shok_GGL_CLeaderBehaviorProps* GetLeaderBehaviorProps();
	shok_GGL_CSoldierBehaviorProps* GetSoldierBehaviorProps();
	shok_GGL_CBattleBehaviorProps* GetBattleBehaviorProps();
	shok_GGL_CLimitedLifespanBehaviorProps* GetLimitedLifespanBehaviorProps();
	shok_GGL_CAffectMotivationBehaviorProps* GetAffectMotivationBehaviorProps();
	shok_GGL_CGLAnimationBehaviorExProps* GetAnimationExProps();
	shok_GGL_CAutoCannonBehaviorProps* GetAutoCannonProps();
	shok_GGL_CThiefBehaviorProperties* GetThiefBehaviorProps();
	shok_GGL_CCamouflageBehaviorProps* GetCamouflageBehaviorProps();
	shok_GGL_CCircularAttackProps* GetCircularAttackBehaviorProps();
	shok_GGL_CShurikenAbilityProps* GetShurikenBehaviorProps();
	shok_GGL_CSniperAbilityProps* GetSniperBehaviorProps();
	shok_GGL_CRangedEffectAbilityProps* GetRangedEffectBehaviorProps();
	shok_GGL_CWorkerBehaviorProps* GetWorkerBehaviorProps();
	shok_GGL_CHeroHawkBehaviorProps* GetHeroHawkBehaviorProps();
	shok_GGL_CInflictFearAbilityProps* GetInflictFearBehaviorProps();
	shok_GGL_CHeroAbilityProps* GetBombPlacerBehaviorProps();
	shok_GGL_CCannonBuilderBehaviorProps* GetCannonBuilderBehaviorProps();
	shok_GGL_CSummonBehaviorProps* GetSummonBehaviorProps();
	shok_GGL_CConvertSettlerAbilityProps* GetConvertSettlersBehaviorProps();
	shok_GGL_CKegPlacerBehaviorProperties* GetKegPlacerBehaviorProps();
	shok_GGL_CAbilityScoutBinocularProps* GetBinocularBehaviorProps();
	shok_GGL_CTorchPlacerBehaviorProperties* GetTorchPlacerBehaviorProps();
	shok_GGL_CPointToResourceBehaviorProperties* GetPointToResBehaviorProps();
	shok_GGL_CSerfBehaviorProps* GetSerfBehaviorProps();
	shok_GGL_CResourceRefinerBehaviorProperties* GetResourceRefinerBehaviorProps();
	shok_GGL_CBarrackBehaviorProperties* GetBarrackBehaviorProps();


	bool IsSettlerType();
	bool IsBuildingType();
	bool IsCEntityProperties();
	bool IsOfCategory(shok_EntityCategory cat);
};

int* const shok_entityTypeIDSerf = reinterpret_cast<int*>(0x863830);


#pragma once
#include "s5data.h"


struct shok_modifyEntityProps {
	int MysteriousInt;
	vector_padding
		std::vector<int, shok_allocator<int>> TechList;
};


#define shok_vtp_EGL_CGLEEntityProps (void*)0x76E47C
struct shok_EGL_CGLEEntityProps : shok_object {
private:
	int u3;
public:
	int Class;
	vector_padding
		std::vector<int, shok_allocator<int>> Categories;
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
private:
	byte u[2];
	int u2[9];
public:
	vector_padding
		std::vector<shok_AARect, shok_allocator<shok_AARect>> BlockingArea; // la37
};

#define shok_vtp_GGL_CEntityProperties (void*)0x776FEC
struct shok_GGL_CEntityProperties : shok_EGL_CGLEEntityProps {
	int ResourceEntity, ResourceAmount, SummerEffect, WinterEffect;
};

struct shok_upgradeInfo {
	float Time;
	shok_costInfo Cost;
	int Type, Category;
};
#define shok_vtp_GGL_CGLSettlerProps (void*)0x76E498
struct shok_GGL_CGLSettlerProps : shok_EGL_CGLEEntityProps {
private:
	int HeadSet, Hat;
public:
	shok_costInfo Cost;
	float BuildFactor, RepairFactor;
	int ArmorClass;
	int ArmorAmount;
	int DodgeChance;
	int IdleTaskList;
	shok_upgradeInfo Upgrade;
	byte Fearless, Convertible;
private:
	byte u[2];
public:
	shok_modifyEntityProps ModifyExploration, ModifyHitpoints, ModifySpeed, ModifyDamage, ModifyArmor, ModifyDodge, ModifyMaxRange, ModifyMinRange, ModifyDamageBonus, ModifyGroupLimit;
	int AttractionSlots;
};

#define shok_vtp_GGL_CGLAnimalProps (void*)0x779074
struct shok_GGL_CGLAnimalProps : shok_EGL_CGLEEntityProps {
	int DefaultTaskList;
	float TerritoryRadius, WanderRangeMin, WanderRangeMax, ShyRange, MaxBuildingPollution;
	int FleeTaskList;
};

#define shok_vtp_GGL_CBuildBlockProperties (void*)0x76EB38
struct shok_GGL_CBuildBlockProperties : shok_EGL_CGLEEntityProps {
	shok_AARect BuildBlockArea; // la 41
};

#define shok_vtp_GGL_CResourceDoodadProperties (void*)0x76FF68
struct shok_GGL_CResourceDoodadProperties : shok_GGL_CBuildBlockProperties {
	float Radius;
	shok_position Center, LineStart, LineEnd;
	int ExtractTaskList, Model1, Model2;
};

struct shok_constructionInfo {
private:
	int u;
public:
	vector_padding
		std::vector<shok_positionRot, shok_allocator<shok_positionRot>> BuilderSlot;
	int Time;
	shok_costInfo Cost;
	int ConstructionSite;
};
#define shok_vtp_GGL_CGLBuildingProps (void*)0x76EC78
struct shok_GGL_CGLBuildingProps : shok_GGL_CBuildBlockProperties {
	int MaxWorkers, InitialMaxWorkers, NumberOfAttractableSettlers, Worker;
	shok_position DoorPos, LeavePos;
	shok_constructionInfo ConstructionInfo;
	vector_padding
		std::vector<int, shok_allocator<int>> BuildOn;
	byte HideBase, CanBeSold, IsWall;
private:
	byte u;
public:
	shok_upgradeInfo Upgrade;
	int UpgradeSite, ArmorClass, ArmorAmount;
	vector_padding
		std::vector<int, shok_allocator<int>> WorkTaskList;
private:
	int MilitaryInfo[4];
public:
	float CollapseTime;
	byte Convertible;
private:
	byte u2[3];
public:
	shok_modifyEntityProps ModifyExploration, ModifyArmor;
	float KegEffectFactor; // 124
};

#define shok_vtp_GGL_CBridgeProperties (void*)0x778148
struct shok_GGL_CBridgeProperties : shok_GGL_CGLBuildingProps {
	shok_AARect BridgeArea;
	int Height, ConstructionModel0, ConstructionModel1, ConstructionModel2;
};

#define shok_vtp_ED_CDisplayEntityProps (void*)0x788840
struct shok_ED_CDisplayEntityProps : shok_object {
	int DisplayClass;
	int Model[4];
	byte DrawPlayerColor, CastShadow, RenderInFoW, HighQualityOnly, MapEditor_Rotateable, MapEditor_Placeable;
private:
	byte u[2];
public:
	vector_padding
		std::vector<int, shok_allocator<int>> AnimList;
};

#define shok_vtp_GGlue_CGlueEntityProps (void*)0x788824
struct shok_GGlue_CGlueEntityProps : shok_object {
public:
	int u;
public:
	shok_EGL_CGLEEntityProps* LogicProps;
	shok_ED_CDisplayEntityProps* DisplayProps;
	vector_padding
		std::vector<shok_EGL_CGLEBehaviorProps*, shok_allocator<shok_EGL_CGLEBehaviorProps*>> BehaviorProps;


private:
	shok_EGL_CGLEBehaviorProps* SearchBehaviorProp(void** vts, int num);
	shok_EGL_CGLEBehaviorProps* SearchBehaviorProp(void* vt);

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


	bool IsSettlerType();
	bool IsBuildingType();
	bool IsCEntityProperties();
	bool IsOfCategory(int cat);
};


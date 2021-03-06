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
	static inline constexpr int TypeDesc = 0x810B0C;
};

struct shok_GGL_CEntityProperties : shok_EGL_CGLEEntityProps {
	int ResourceEntity, ResourceAmount, SummerEffect, WinterEffect;

	static inline constexpr int vtp = 0x776FEC;
	static inline constexpr int TypeDesc = 0x81192C;
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
	byte Fearless, Convertible; //85
	PADDING(2);
	shok_modifyEntityProps ModifyExploration, ModifyHitpoints, ModifySpeed, ModifyDamage, ModifyArmor, ModifyDodge, ModifyMaxRange, ModifyMinRange, ModifyDamageBonus, ModifyGroupLimit;
	int AttractionSlots;

	static inline constexpr int vtp = 0x76E498;
	static inline constexpr int TypeDesc = 0x810B30;
};
//constexpr int i = offsetof(shok_GGL_CGLSettlerProps, Fearless) / 4;

struct shok_GGL_CGLAnimalProps : shok_EGL_CGLEEntityProps {
	int DefaultTaskList;
	float TerritoryRadius, WanderRangeMin, WanderRangeMax, ShyRange, MaxBuildingPollution;
	int FleeTaskList;

	static inline constexpr int vtp = 0x779074;
	static inline constexpr int TypeDesc = 0x828DD0;
};

struct shok_GGL_CBuildBlockProperties : shok_EGL_CGLEEntityProps {
	shok_AARect BuildBlockArea; // la 41

	static inline constexpr int vtp = 0x76EB38;
	static inline constexpr int TypeDesc = 0x811284;
};

struct shok_GGL_CResourceDoodadProperties : shok_GGL_CBuildBlockProperties {
	float Radius;
	shok_position Center, LineStart, LineEnd;
	int ExtractTaskList, Model1, Model2;

	static inline constexpr int vtp = 0x76FF68;
	static inline constexpr int TypeDesc = 0x811900;
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
	byte Convertible; //113
	PADDING(3);
	shok_modifyEntityProps ModifyExploration, ModifyArmor;
	float KegEffectFactor; // 124

	static inline constexpr int vtp = 0x76EC78;
	static inline constexpr int TypeDesc = 0x811210;
};
//constexpr int i = offsetof(shok_GGL_CGLBuildingProps, Convertible) / 4;

struct shok_GGL_CBridgeProperties : shok_GGL_CGLBuildingProps {
	shok_AARect BridgeArea;
	int Height, ConstructionModel0, ConstructionModel1, ConstructionModel2;

	static inline constexpr int vtp = 0x778148;
	static inline constexpr int TypeDesc = 0x824A84;
};

struct shok_ED_CDisplayEntityProps : shok_object {
	int DisplayClass;
	int Model[4];
	byte DrawPlayerColor, CastShadow, RenderInFoW, HighQualityOnly, MapEditor_Rotateable, MapEditor_Placeable;
	PADDING(2);
	vector_padding;
	std::vector<int, shok_allocator<int>> AnimList;

	static inline constexpr int vtp = 0x788840;
	static inline constexpr int TypeDesc = 0x83C918;
};

struct shok_GGlue_CGlueEntityProps : shok_object {
	PADDINGI(1);
	shok_EGL_CGLEEntityProps* LogicProps;
	shok_ED_CDisplayEntityProps* DisplayProps;
	vector_padding;
	std::vector<shok_EGL_CGLEBehaviorProps*, shok_allocator<shok_EGL_CGLEBehaviorProps*>> BehaviorProps;

	static inline constexpr int vtp = 0x788824;
	static inline constexpr int TypeDesc = 0x83C8CC;


	template<typename T, typename std::enable_if<std::is_base_of<shok_EGL_CGLEBehaviorProps, T>::value>::type* = nullptr>
	T* GetBehaviorProps() {
		for (shok_EGL_CGLEBehaviorProps* b : BehaviorProps) {
			if (b) {
				T* r = shok_DynamicCast<shok_EGL_CGLEBehaviorProps, T>(b);
				if (r)
					return r;
			}
		}
		return nullptr;
	}

	bool IsSettlerType();
	bool IsBuildingType();
	bool IsCEntityProperties();
	bool IsOfCategory(shok_EntityCategory cat);
};

int* const shok_entityTypeIDSerf = reinterpret_cast<int*>(0x863830);


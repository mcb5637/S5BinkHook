#pragma once
#include "s5data.h"


struct shok_modifyEntityProps {
	int MysteriousInt;
	vector_padding;
	std::vector<int, shok::Allocator<int>> TechList;
};


struct shok_EGL_CGLEEntityProps : shok_BB_IObject {
	PADDINGI(1);
	int Class;
	shok::Vector<shok_EntityCategory> Categories;
	shok::PositionRot ApproachPos;
private:
	int Race;
public:
	byte CanFloat, CanDrown, MapFileDontSave, DividesTwoSectors; // 11
	byte ForceNoPlayer, AdjustWalkAnimSpeed, Visible, DoNotExecute; // 12
	int MaxHealth; // 13
private:
	int Models[5];
public:
	float Exploration;
	int ExperiencePoints, AccessCategory, NumBlockedPoints; //20
	float SnapTolerance; //23 seems to be a max change in every coordinate on entity placement
	byte DeleteWhenBuiltOn, NeedsPlayer;
	PADDING(2);
	vector_padding;
	std::vector<EGL::CGLEBehaviorProps*, shok::Allocator<EGL::CGLEBehaviorProps*>> BehaviorProps; // 25
	int NumberOfBehaviors; //29
	PADDINGI(4);
	vector_padding;
	std::vector<shok::AARect, shok::Allocator<shok::AARect>> BlockingArea; // la37

	static inline constexpr int vtp = 0x76E47C;
	static inline constexpr int TypeDesc = 0x810B0C;

	template<typename T, typename std::enable_if<std::is_base_of<EGL::CGLEBehaviorProps, T>::value>::type* = nullptr>
	T* GetBehaviorProps() {
		for (EGL::CGLEBehaviorProps* b : BehaviorProps) {
			if (b) {
				T* r = shok_DynamicCast<EGL::CGLEBehaviorProps, T>(b);
				if (r)
					return r;
			}
		}
		return nullptr;
	}
	template<typename T, typename std::enable_if<std::is_base_of<EGL::CGLEBehaviorProps, T>::value>::type* = nullptr>
	const T* GetBehaviorProps() const {
		for (const EGL::CGLEBehaviorProps* b : BehaviorProps) {
			if (b) {
				const T* r = shok_DynamicCast<const EGL::CGLEBehaviorProps, const T>(b);
				if (r)
					return r;
			}
		}
		return nullptr;
	}
};
constexpr int i = offsetof(shok_EGL_CGLEEntityProps, ApproachPos) / 4;

struct shok_GGL_CEntityProperties : shok_EGL_CGLEEntityProps {
	int ResourceEntity, ResourceAmount, SummerEffect, WinterEffect;

	static inline constexpr int vtp = 0x776FEC;
	static inline constexpr int TypeDesc = 0x81192C;
};

struct shok_upgradeInfo {
	float Time;
	shok::CostInfo Cost;
	int Type, Category;
};
struct shok_GGL_CGLSettlerProps : shok_EGL_CGLEEntityProps {
private:
	int HeadSet, Hat;
public:
	shok::CostInfo Cost;
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
	shok::AARect BuildBlockArea; // 38 la 41

	static inline constexpr int vtp = 0x76EB38;
	static inline constexpr int TypeDesc = 0x811284;
};
//constexpr int i = offsetof(shok_GGL_CBuildBlockProperties, BuildBlockArea) / 4;

struct shok_GGL_CResourceDoodadProperties : shok_GGL_CBuildBlockProperties {
	float Radius;
	shok::Position Center, LineStart, LineEnd;
	int ExtractTaskList, Model1, Model2;

	static inline constexpr int vtp = 0x76FF68;
	static inline constexpr int TypeDesc = 0x811900;
};

struct shok_constructionInfo {
	PADDINGI(1);
	vector_padding;
	std::vector<shok::PositionRot, shok::Allocator<shok::PositionRot>> BuilderSlot;
	int Time;
	shok::CostInfo Cost;
	int ConstructionSite;
};
struct shok_GGL_CGLBuildingProps : shok_GGL_CBuildBlockProperties {
	int MaxWorkers, InitialMaxWorkers, NumberOfAttractableSettlers, Worker; // 42
	shok::Position DoorPos, LeavePos;
	shok_constructionInfo ConstructionInfo;
	vector_padding;
	std::vector<int, shok::Allocator<int>> BuildOn; // 75
	byte HideBase, CanBeSold, IsWall; // 79
	PADDING(1);
	shok_upgradeInfo Upgrade;
	int UpgradeSite, ArmorClass, ArmorAmount;
	vector_padding;
	std::vector<int, shok::Allocator<int>> WorkTaskList; // 104
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
//constexpr int i = offsetof(shok_GGL_CGLBuildingProps, BuildOn) / 4;

struct shok_GGL_CBridgeProperties : shok_GGL_CGLBuildingProps {
	vector_padding;
	std::vector<shok::AARect, shok::Allocator<shok::AARect>> BridgeArea;
	int Height, ConstructionModel0, ConstructionModel1, ConstructionModel2;

	static inline constexpr int vtp = 0x778148;
	static inline constexpr int TypeDesc = 0x824A84;
};

struct shok_ED_CBehaviorProps;
struct shok_ED_CDisplayEntityProps : shok_BB_IObject {
	int DisplayClass;
	int Model[4];
	bool DrawPlayerColor, CastShadow, RenderInFoW, HighQualityOnly, MapEditor_Rotateable, MapEditor_Placeable;
	PADDING(2);
	vector_padding;
	std::vector<int, shok::Allocator<int>> AnimList;
	vector_padding;
	std::vector<shok_ED_CBehaviorProps*, shok::Allocator<shok_ED_CBehaviorProps*>> DisplayBehaviorProps;

	template<typename T, typename std::enable_if<std::is_base_of<shok_ED_CBehaviorProps, T>::value>::type* = nullptr>
	T* GetDisplayBehaviorProps() {
		for (shok_ED_CBehaviorProps* b : DisplayBehaviorProps) {
			if (b) {
				T* r = shok_DynamicCast<shok_ED_CBehaviorProps, T>(b);
				if (r)
					return r;
			}
		}
		return nullptr;
	}
	template<typename T, typename std::enable_if<std::is_base_of<shok_ED_CBehaviorProps, T>::value>::type* = nullptr>
	const T* GetDisplayBehaviorProps() const {
		for (const shok_ED_CBehaviorProps* b : DisplayBehaviorProps) {
			if (b) {
				const T* r = shok_DynamicCast<const shok_ED_CBehaviorProps, const T>(b);
				if (r)
					return r;
			}
		}
		return nullptr;
	}

	static inline constexpr int vtp = 0x788840;
	static inline constexpr int TypeDesc = 0x83C918;
};
static_assert(sizeof(shok_ED_CDisplayEntityProps) == 16 * 4);

struct shok_GGlue_CGlueEntityProps_behavior {
	EGL::CGLEBehaviorProps* Logic;
	shok_ED_CBehaviorProps* Display;
};
struct shok_GGlue_CGlueEntityProps : shok_BB_IObject {
	PADDINGI(1);
	shok_EGL_CGLEEntityProps* LogicProps;
	shok_ED_CDisplayEntityProps* DisplayProps;
	vector_padding;
	std::vector<shok_GGlue_CGlueEntityProps_behavior, shok::Allocator<shok_GGlue_CGlueEntityProps_behavior>> BehaviorProps;

	static inline constexpr int vtp = 0x788824;
	static inline constexpr int TypeDesc = 0x83C8CC;


	template<typename T>
	T* GetBehaviorProps() {
		return LogicProps->GetBehaviorProps<T>();
	}
	template<typename T>
	T* GetDisplayBehaviorProps() {
		return DisplayProps->GetDisplayBehaviorProps<T>();
	}

	bool IsSettlerType() const;
	bool IsBuildingType() const;
	bool IsCEntityProperties() const;
	bool IsOfCategory(shok_EntityCategory cat) const;
};

int* const shok_entityTypeIDSerf = reinterpret_cast<int*>(0x863830);


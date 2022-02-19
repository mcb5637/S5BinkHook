#pragma once
#include "s5data.h"

namespace EGL {
	class CGLEEntityProps : public BB::IObject {
	public:
		class ModifyEntityProps {
		public:
			int MysteriousInt;
			shok::Vector<int> TechList;
		};
		class UpgradeInfo {
		public:
			float Time;
			shok::CostInfo Cost;
			int Type, Category;
		};


		PADDINGI(1);
		int Class;
		shok::Vector<shok::EntityCategory> Categories;
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
		shok::Vector<EGL::CGLEBehaviorProps*> BehaviorProps; // 25
		int NumberOfBehaviors; //29
		PADDINGI(4);
		shok::Vector<shok::AARect> BlockingArea; // la37

		static inline constexpr int vtp = 0x76E47C;
		static inline constexpr int TypeDesc = 0x810B0C;

		template<class T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		T* GetBehaviorProps() {
			for (EGL::CGLEBehaviorProps* b : BehaviorProps) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
		template<class T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		const T* GetBehaviorProps() const {
			for (const EGL::CGLEBehaviorProps* b : BehaviorProps) {
				if (b) {
					const T* r = dynamic_cast<const T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
	};
	static_assert(offsetof(EGL::CGLEEntityProps, ApproachPos) == 4 * 7);


}

namespace GGL {
	class CEntityProperties : public EGL::CGLEEntityProps {
	public:
		int ResourceEntity, ResourceAmount, SummerEffect, WinterEffect;

		static inline constexpr int vtp = 0x776FEC;
		static inline constexpr int TypeDesc = 0x81192C;
	};

	class CGLSettlerProps : public EGL::CGLEEntityProps {
	private:
		int HeadSet, Hat;
	public:
		shok::CostInfo Cost;
		float BuildFactor, RepairFactor;
		int ArmorClass;
		int ArmorAmount; // 61
		int DodgeChance;
		int IdleTaskList;
		UpgradeInfo Upgrade;
		byte Fearless, Convertible; //85
		PADDING(2);
		ModifyEntityProps ModifyExploration, ModifyHitpoints, ModifySpeed, ModifyDamage, ModifyArmor, ModifyDodge, ModifyMaxRange, ModifyMinRange, ModifyDamageBonus, ModifyGroupLimit;
		int AttractionSlots;

		static inline constexpr int vtp = 0x76E498;
		static inline constexpr int TypeDesc = 0x810B30;
	};

	class CGLAnimalProps : public EGL::CGLEEntityProps {
	public:
		int DefaultTaskList;
		float TerritoryRadius, WanderRangeMin, WanderRangeMax, ShyRange, MaxBuildingPollution;
		int FleeTaskList;

		static inline constexpr int vtp = 0x779074;
		static inline constexpr int TypeDesc = 0x828DD0;
	};

	class CBuildBlockProperties : public EGL::CGLEEntityProps {
	public:
		shok::AARect BuildBlockArea; // 38 la 41

		static inline constexpr int vtp = 0x76EB38;
		static inline constexpr int TypeDesc = 0x811284;
	};

	class CResourceDoodadProperties : public GGL::CBuildBlockProperties {
	public:
		float Radius;
		shok::Position Center, LineStart, LineEnd;
		int ExtractTaskList, Model1, Model2;

		static inline constexpr int vtp = 0x76FF68;
		static inline constexpr int TypeDesc = 0x811900;
	};

	class CGLBuildingProps : public GGL::CBuildBlockProperties {
	public:
		class ConstructionInfo {
		public:
			PADDINGI(1);
			vector_padding;
			std::vector<shok::PositionRot, shok::Allocator<shok::PositionRot>> BuilderSlot;
			int Time;
			shok::CostInfo Cost;
			int ConstructionSite;
		};
		int MaxWorkers, InitialMaxWorkers, NumberOfAttractableSettlers, Worker; // 42
		shok::Position DoorPos, LeavePos;
		ConstructionInfo ConstructionInfo;
		shok::Vector<int> BuildOn; // 75
		byte HideBase, CanBeSold, IsWall; // 79
		PADDING(1);
		UpgradeInfo Upgrade;
		int UpgradeSite, ArmorClass, ArmorAmount;
		shok::Vector<int> WorkTaskList; // 104
	private:
		int MilitaryInfo[4];
	public:
		float CollapseTime;
		byte Convertible; //113
		PADDING(3);
		ModifyEntityProps ModifyExploration, ModifyArmor;
		float KegEffectFactor; // 124

		static inline constexpr int vtp = 0x76EC78;
		static inline constexpr int TypeDesc = 0x811210;
	};

	class CBridgeProperties : public GGL::CGLBuildingProps {
	public:
		shok::Vector<shok::AARect> BridgeArea;
		int Height, ConstructionModel0, ConstructionModel1, ConstructionModel2;

		static inline constexpr int vtp = 0x778148;
		static inline constexpr int TypeDesc = 0x824A84;
	};

}

namespace ED {
	class CDisplayEntityProps : public BB::IObject {
	public:
		int DisplayClass;
		int Model[4];
		bool DrawPlayerColor, CastShadow, RenderInFoW, HighQualityOnly, MapEditor_Rotateable, MapEditor_Placeable;
		shok::Vector<int> AnimList;
		shok::Vector<ED::CBehaviorProps*> DisplayBehaviorProps;

		template<class T>
		requires std::derived_from<T, ED::CBehaviorProps>
		T* GetDisplayBehaviorProps() {
			for (ED::CBehaviorProps* b : DisplayBehaviorProps) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
		template<class T>
		requires std::derived_from<T, ED::CBehaviorProps>
		const T* GetDisplayBehaviorProps() const {
			for (const ED::CBehaviorProps* b : DisplayBehaviorProps) {
				if (b) {
					const T* r = dynamic_cast<const T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}

		static inline constexpr int vtp = 0x788840;
		static inline constexpr int TypeDesc = 0x83C918;
	};
	static_assert(sizeof(ED::CDisplayEntityProps) == 16 * 4);
}

namespace GGlue {
	class CGlueEntityProps : public BB::IObject {
	public:
		struct BehaviorData {
			EGL::CGLEBehaviorProps* Logic;
			ED::CBehaviorProps* Display;
		};
		PADDINGI(1);
		EGL::CGLEEntityProps* LogicProps;
		ED::CDisplayEntityProps* DisplayProps;
		shok::Vector<BehaviorData> BehaviorProps;

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
		bool IsOfCategory(shok::EntityCategory cat) const;


		static inline int* const EntityTypeIDSerf = reinterpret_cast<int*>(0x863830);
	};
}

namespace EGL {
	class CGLEEntitiesProps : public ECore::ICheckData {
	public:
		PADDINGI(1); // float 240
		BB::CIDManagerEx* EntityTypeManager;
		BB::CIDManagerEx* EntityCategoryManager;
		BB::CIDManagerEx* UpgradeCategoryManager;
		BB::CIDManagerEx* BlessCategoryManager;
		shok::Vector<EGL::CGLEEntityProps*> EntityTypesLogicProps; // 6
		BB::CIDManagerEx* EntityTypeManagerAgain;
		shok::Vector<ED::CDisplayEntityProps*> EntityTypesDisplayProps; // 11
		shok::Vector<GGlue::CGlueEntityProps> EntityTypes; // 15

		static inline constexpr int vtp = 0x788834;

		GGlue::CGlueEntityProps* GetEntityType(int i);

		static inline EGL::CGLEEntitiesProps** const GlobalObj = reinterpret_cast<EGL::CGLEEntitiesProps**>(0x895DB0);

		static const char* GetEntityTypeDisplayName(int i);
	};
}

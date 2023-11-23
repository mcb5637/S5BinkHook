#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_baseprops.h"

namespace EGL {
	class CGLEEntityProps : public BB::IObject {
	public:
		class ModifyEntityProps {
		public:
			shok::TechModifierType ModifierType;
			shok::Vector<shok::TechnologyId> TechList;

			float ModifyValue(int player, float initial);
		};
		class UpgradeInfo {
		public:
			float Time;
			shok::CostInfo Cost;
			shok::EntityTypeId Type;
			shok::UpgradeCategoryId Category;
		};


		PADDINGI(1);
		shok::ClassId Class;
		shok::Vector<shok::EntityCategory> Categories;
		shok::PositionRot ApproachPos;
	private:
		int Race; // unused
	public:
		bool CanFloat, CanDrown, MapFileDontSave, NeedsPlayer; // 11
		bool ForceNoPlayer, AdjustWalkAnimSpeed, Visible, DoNotExecute; // 12
		int MaxHealth; // 13
	private:
		shok::ModelId Models[5];
	public:
		float Exploration;
		int ExperiencePoints; //20
		shok::AccessCategory AccessCategory;
		int NumBlockedPoints;
		float SnapTolerance; //23 seems to be a max change in every coordinate on entity placement
		bool DeleteWhenBuiltOn, DividesTwoSectors;
		PADDING(2);
		shok::Vector<EGL::CGLEBehaviorProps*> BehaviorProps; // 25
		int NumberOfBehaviors; //29
		shok::AARect Blocked; // 30 as Blocked1 and Blocked2 in xml
		shok::Vector<shok::AARect> BlockingArea; // 34 la37

		static inline constexpr int vtp = 0x76E47C;
		static inline constexpr int TypeDesc = 0x810B0C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x23962D3D);

		void InitializeBlocking();

		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		T* GetBehaviorProps() {
			for (EGL::CGLEBehaviorProps* b : BehaviorProps) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
		template<class T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		T* GetBehaviorPropsDynamic() {
			for (EGL::CGLEBehaviorProps* b : BehaviorProps) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		const T* GetBehaviorProps() const {
			for (const EGL::CGLEBehaviorProps* b : BehaviorProps) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<const T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
		template<class T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		const T* GetBehaviorPropsDynamic() const {
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
	static_assert(offsetof(EGL::CGLEEntityProps, BlockingArea) == 4 * 34);
	static_assert(offsetof(EGL::CGLEEntityProps, ExperiencePoints) == 4 * 20);
}

namespace GGL {
	class CEntityProperties : public EGL::CGLEEntityProps {
	public:
		shok::EntityTypeId ResourceEntity;
		int ResourceAmount;
		shok::EffectTypeId SummerEffect, WinterEffect;

		static inline constexpr int vtp = 0x776FEC;
		static inline constexpr int TypeDesc = 0x81192C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC58BF747);
	};

	class CGLSettlerProps : public EGL::CGLEEntityProps {
	private:
		int HeadSet, Hat;
	public:
		shok::CostInfo Cost;
		float BuildFactor, RepairFactor;
		shok::ArmorClassId ArmorClass;
		int ArmorAmount; // 61
		int DodgeChance;
		shok::TaskListId IdleTaskList;
		UpgradeInfo Upgrade;
		bool Fearless, Convertible; //85
		PADDING(2);
		ModifyEntityProps ModifyExploration; // 86
		ModifyEntityProps ModifyHitpoints; // 91
		ModifyEntityProps ModifySpeed; // 96
		ModifyEntityProps ModifyDamage; // 101
		ModifyEntityProps ModifyArmor; // 106
		ModifyEntityProps ModifyDodge; // 111
		ModifyEntityProps ModifyMaxRange; // 116
		ModifyEntityProps ModifyMinRange; // 121
		ModifyEntityProps ModifyDamageBonus; // 126
		ModifyEntityProps ModifyGroupLimit; // 131
		int AttractionSlots; // 136

		static inline constexpr int vtp = 0x76E498;
		static inline constexpr int TypeDesc = 0x810B30;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA2A53F58);
	};
	//constexpr int i = offsetof(CGLSettlerProps, ModifyGroupLimit) / 4;

	class CGLAnimalProps : public EGL::CGLEEntityProps {
	public:
		shok::TaskListId DefaultTaskList;
		float TerritoryRadius, WanderRangeMin, WanderRangeMax, ShyRange, MaxBuildingPollution;
		shok::TaskListId FleeTaskList;

		static inline constexpr int vtp = 0x779074;
		static inline constexpr int TypeDesc = 0x828DD0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x21487A57);
	};

	class CBuildBlockProperties : public EGL::CGLEEntityProps {
	public:
		shok::AARect BuildBlockArea; // 38 la 41

		static inline constexpr int vtp = 0x76EB38;
		static inline constexpr int TypeDesc = 0x811284;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xDE473B17);
	};

	class CResourceDoodadProperties : public GGL::CBuildBlockProperties {
	public:
		float Radius;
		shok::Position Center, LineStart, LineEnd;
		shok::TaskListId ExtractTaskList;
		shok::ModelId Model1, Model2;

		static inline constexpr int vtp = 0x76FF68;
		static inline constexpr int TypeDesc = 0x811900;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9B03A097);
	};

	class CGLBuildingProps : public GGL::CBuildBlockProperties {
	public:
		class ConstructionInfo {
		public:
			PADDINGI(1);
			shok::Vector<shok::PositionRot> BuilderSlot;
			int Time;
			shok::CostInfo Cost;
			shok::EntityTypeId ConstructionSite;
		};
		struct WorkTL {
			shok::TaskListId Start, Work;
		};
		int MaxWorkers, InitialMaxWorkers, NumberOfAttractableSettlers; // 42
		shok::EntityTypeId Worker;
		shok::Position DoorPos, LeavePos;
		ConstructionInfo ConstructionInfo;
		shok::Vector<shok::EntityTypeId> BuildOn; // 75
		bool HideBase, CanBeSold, IsWall; // 79
		PADDING(1);
		UpgradeInfo Upgrade;
		shok::EntityTypeId UpgradeSite;
		shok::ArmorClassId ArmorClass;
		int ArmorAmount;
		shok::Vector<WorkTL> WorkTaskList; // 104
	private:
		int MilitaryInfo[4];
	public:
		float CollapseTime;
		bool Convertible; //113
		PADDING(3);
		ModifyEntityProps ModifyExploration, ModifyArmor;
		float KegEffectFactor; // 124

		static inline constexpr int vtp = 0x76EC78;
		static inline constexpr int TypeDesc = 0x811210;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3A4D8B20);
	};
	static_assert(offsetof(CGLBuildingProps, MaxWorkers) == 42 * 4);

	class CBridgeProperties : public GGL::CGLBuildingProps {
	public:
		shok::Vector<shok::AARect> BridgeArea;
		int Height;
		shok::ModelId ConstructionModel0, ConstructionModel1, ConstructionModel2;

		static inline constexpr int vtp = 0x778148;
		static inline constexpr int TypeDesc = 0x824A84;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7706B02E);
	};

}

namespace ED {
	class CDisplayEntityProps : public BB::IObject {
	public:
		shok::ClassId DisplayClass;
		shok::ModelId Model[4];
		bool DrawPlayerColor, CastShadow, RenderInFoW, HighQualityOnly, MapEditor_Rotateable, MapEditor_Placeable;
		shok::Vector<shok::AnimationId> AnimList; // 8
		shok::Vector<ED::CBehaviorProps*> DisplayBehaviorProps;

		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, ED::CBehaviorProps>
		T* GetDisplayBehaviorProps() {
			for (ED::CBehaviorProps* b : DisplayBehaviorProps) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
		template<class T>
		requires std::derived_from<T, ED::CBehaviorProps>
		T* GetDisplayBehaviorPropsDynamic() {
			for (ED::CBehaviorProps* b : DisplayBehaviorProps) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, ED::CBehaviorProps>
		const T* GetDisplayBehaviorProps() const {
			for (const ED::CBehaviorProps* b : DisplayBehaviorProps) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<const T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
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
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x20E25C5D);
	};
	static_assert(sizeof(ED::CDisplayEntityProps) == 16 * 4);
	//constexpr int i = offsetof(CDisplayEntityProps, AnimList)/4;
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
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD397707);

		CGlueEntityProps();
		CGlueEntityProps(const CGlueEntityProps& o);
		CGlueEntityProps(CGlueEntityProps&& o) noexcept;

		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		T* GetBehaviorProps() {
			return LogicProps->GetBehaviorProps<T>();
		}
		// dynamic_cast, may return subclasses
		template<class T>
		requires std::derived_from<T, EGL::CGLEBehaviorProps>
		T* GetBehaviorPropsDynamic() {
			return LogicProps->GetBehaviorPropsDynamic<T>();
		}
		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, ED::CBehaviorProps>
		T* GetDisplayBehaviorProps() {
			return DisplayProps->GetDisplayBehaviorProps<T>();
		}
		// dynamic_cast, may return subclasses
		template<typename T>
		requires std::derived_from<T, ED::CBehaviorProps>
		T* GetDisplayBehaviorPropsDynamic() {
			return DisplayProps->GetDisplayBehaviorPropsDynamic<T>();
		}

		bool IsSettlerType() const;
		bool IsBuildingType() const;
		bool IsCEntityProperties() const;
		bool IsOfCategory(shok::EntityCategory cat) const;

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;


		static inline shok::EntityTypeId* const EntityTypeIDSerf = reinterpret_cast<shok::EntityTypeId*>(0x863830);
	};
}

namespace EGL {
	class CGLEEntitiesProps : public ECore::ICheckData {
	public:
		float DefaultWalkSpeed; // ?? loaded from Entities.xml
		BB::CIDManagerEx* EntityTypeManager;
		BB::CIDManagerEx* EntityCategoryManager;
		BB::CIDManagerEx* UpgradeCategoryManager;
		BB::CIDManagerEx* BlessCategoryManager;
		shok::Vector<EGL::CGLEEntityProps*> EntityTypesLogicProps; // 6

		static inline constexpr int vtp = 0x788834;

		static GGlue::CGlueEntityProps* GetEntityType(shok::EntityTypeId i);

		static inline EGL::CGLEEntitiesProps** const GlobalObj = reinterpret_cast<EGL::CGLEEntitiesProps**>(0x895DB0);

		static const char* GetEntityTypeDisplayName(shok::EntityTypeId i);

		// init blocking for everything but the last: 5846BE
	};

	class EntityCategoryCache {
	public:
		shok::Map<shok::EntityCategory, shok::Vector<shok::EntityTypeId>> CategoryToEntityTypes;

		static inline EntityCategoryCache* const GlobalObj = reinterpret_cast<EntityCategoryCache*>(0x8975A4);
		static inline void(__cdecl* const RefreshCache)(CGLEEntitiesProps* props) = reinterpret_cast<void(__cdecl*)(CGLEEntitiesProps*)>(0x5770CA);
		static inline shok::Vector<int>* (__cdecl* const GetEntitiyTypeWithCategory)(shok::EntityCategory cat) = reinterpret_cast<shok::Vector<int>*(__cdecl*)(shok::EntityCategory)>(0x5765A8);
	};
}

namespace ED {
	class EntityTypeDisplayProps {
	public:
		BB::CIDManagerEx* EntityTypeManagerAgain;
		shok::Vector<ED::CDisplayEntityProps*> EntityTypesDisplayProps; // 11
	};
}
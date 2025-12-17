#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_baseprops.h>
#include <shok/tech/s5_tech.h>

namespace EGL {
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CGLEModelSet {
	public:
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const;

		shok::Vector<shok::ModelId> Models;

		static inline constexpr int vtp = 0x76e380;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x21d80520);
	};
	class CGLEEntityProps : public BB::IObject {
	public:
		class ModifyEntityProps {
		public:
			shok::TechModifierType ModifierType;
			shok::Vector<shok::TechnologyId> TechList{};

			float ModifyValue(int player, float initial);

			explicit inline ModifyEntityProps(shok::TechModifierType t = shok::TechModifierType::SpeedModifier) : ModifierType(t) {}
		};
		class UpgradeInfo {
		public:
			float Time = 0.0f;
			shok::CostInfo Cost;
			shok::EntityTypeId Type{};
			shok::UpgradeCategoryId Category{};
		};


		PADDINGI(1);
		shok::ClassId Class{};
		shok::Vector<shok::EntityCategory> Categories;
		shok::PositionRot ApproachPos{0.0f,0.0f,0.0f};
	private:
		int Race = 0; // unused
	public:
		bool CanFloat = false, CanDrown = true, MapFileDontSave = false, NeedsPlayer = false; // 11
		bool ForceNoPlayer = false, AdjustWalkAnimSpeed = false, Visible = true, DoNotExecute = false; // 12
		int MaxHealth = 100; // 13
		CGLEModelSet Models;
		float Exploration = 0.0f;
		int ExperiencePoints = 0; //20
		shok::AccessCategory AccessCategory{};
		int NumBlockedPoints = 0;
		float SnapTolerance = 0.0f; //23 seems to be a max change in every coordinate on entity placement
		bool DeleteWhenBuiltOn = true, DividesTwoSectors = false;
		PADDING(2);
		shok::Vector<EGL::CGLEBehaviorProps*> BehaviorProps; // 25
		int NumberOfBehaviors = 0; //29
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
		shok::EntityTypeId ResourceEntity{};
		int ResourceAmount = 0;
		shok::EffectTypeId SummerEffect{}, WinterEffect{};

		static inline constexpr int vtp = 0x776FEC;
		static inline constexpr int TypeDesc = 0x81192C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC58BF747);
	};

	class CGLSettlerProps : public EGL::CGLEEntityProps {
	private:
		int HeadSet = 0, Hat = 0;
	public:
		shok::CostInfo Cost;
		float BuildFactor = 1.0f, RepairFactor = 1.0f;
		shok::ArmorClassId ArmorClass = shok::ArmorClassId::ArmorClassNone;
		int ArmorAmount = 0; // 61
		int DodgeChance = 0;
		shok::TaskListId IdleTaskList{};
		UpgradeInfo Upgrade;
		bool Fearless = false, Convertible = true; //85
		PADDING(2);
		ModifyEntityProps ModifyExploration{shok::TechModifierType::ExplorationModifier}; // 86
		ModifyEntityProps ModifyHitpoints{shok::TechModifierType::HitpointModifier}; // 91
		ModifyEntityProps ModifySpeed{shok::TechModifierType::SpeedModifier}; // 96
		ModifyEntityProps ModifyDamage{shok::TechModifierType::DamageModifier}; // 101
		ModifyEntityProps ModifyArmor{shok::TechModifierType::ArmorModifier}; // 106
		ModifyEntityProps ModifyDodge{shok::TechModifierType::DodgeModifier}; // 111
		ModifyEntityProps ModifyMaxRange{shok::TechModifierType::RangeModifier}; // 116
		ModifyEntityProps ModifyMinRange{shok::TechModifierType::MinRangeModifier}; // 121
		ModifyEntityProps ModifyDamageBonus{shok::TechModifierType::DamageBonusModifier}; // 126
		ModifyEntityProps ModifyGroupLimit{shok::TechModifierType::GroupLimitModifier}; // 131
		int AttractionSlots = 1; // 136

		static inline constexpr int vtp = 0x76E498;
		static inline constexpr int TypeDesc = 0x810B30;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA2A53F58);
	};
	//constexpr int i = offsetof(CGLSettlerProps, ModifyGroupLimit) / 4;

	class CGLAnimalProps : public EGL::CGLEEntityProps {
	public:
		shok::TaskListId DefaultTaskList{}; //38
		float TerritoryRadius = 0.0f, WanderRangeMin = 2.0f, WanderRangeMax = 6.0f, ShyRange = 500.0f, MaxBuildingPollution = 0.5f;
		shok::TaskListId FleeTaskList{};

		static inline constexpr int vtp = 0x779074;
		static inline constexpr int TypeDesc = 0x828DD0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x21487A57);
	};
	//constexpr int i = offsetof(CGLAnimalProps, DefaultTaskList) / 4;

	class CBuildBlockProperties : public EGL::CGLEEntityProps {
	public:
		shok::AARect BuildBlockArea; // 38 la 41

		static inline constexpr int vtp = 0x76EB38;
		static inline constexpr int TypeDesc = 0x811284;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xDE473B17);
	};

	class CResourceDoodadProperties : public GGL::CBuildBlockProperties {
	public:
		float Radius = 200.0f;
		shok::Position Center{0.0f, 0.0f}, LineStart{0.0f, 0.0f}, LineEnd{0.0f, 0.0f};
		shok::TaskListId ExtractTaskList{};
		shok::ModelId Model1{}, Model2{};

		static inline constexpr int vtp = 0x76FF68;
		static inline constexpr int TypeDesc = 0x811900;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9B03A097);
	};

	class CGLBuildingMilitaryProps {
	public:
		virtual ~CGLBuildingMilitaryProps() = default;

		int LeaderLimit = 0;
		int SoldierLimit = 0;
		shok::DamageClassId DamageClass{};

		static inline constexpr int vtp = 0x76ea50;
	};
	class CGLBuildingProps : public GGL::CBuildBlockProperties {
	public:
		struct WorkTL {
			shok::TaskListId Start{}, Work{};
		};
		int MaxWorkers = 0, InitialMaxWorkers = 0, NumberOfAttractableSettlers = 0; // 42
		shok::EntityTypeId Worker{};
		shok::Position DoorPos{0.0f, 0.0f}, LeavePos{0.0f, 0.0f}; // 46
		CGLBuildingConstructionProps ConstructionInfo;
		shok::Vector<shok::EntityTypeId> BuildOn; // 75
		bool HideBase = false, CanBeSold = true, IsWall = false; // 79
		PADDING(1);
		UpgradeInfo Upgrade;
		shok::EntityTypeId UpgradeSite{};
		shok::ArmorClassId ArmorClass = shok::ArmorClassId::ArmorClassFortification;
		int ArmorAmount = 0;
		shok::Vector<WorkTL> WorkTaskList; // 104
		CGLBuildingMilitaryProps MilitaryInfo;
		float CollapseTime = 1.0f;
		bool Convertible = true; //113
		PADDING(3);
		ModifyEntityProps ModifyExploration{shok::TechModifierType::ExplorationModifier}, ModifyArmor{shok::TechModifierType::ArmorModifier};
		float KegEffectFactor = 1.0f; // 124

		static inline constexpr int vtp = 0x76EC78;
		static inline constexpr int TypeDesc = 0x811210;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3A4D8B20);
	};
	static_assert(offsetof(CGLBuildingProps, MaxWorkers) == 42 * 4);
	static_assert(offsetof(CGLBuildingProps, Upgrade.Category) == 100 * 4);

	class CBridgeProperties : public GGL::CGLBuildingProps {
	public:
		shok::Vector<shok::AARect> BridgeArea;
		int Height = 0;
		shok::ModelId ConstructionModel0{}, ConstructionModel1{}, ConstructionModel2{};

		static inline constexpr int vtp = 0x778148;
		static inline constexpr int TypeDesc = 0x824A84;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7706B02E);
	};

}

namespace ED {
	class CDisplayEntityProps : public BB::IObject {
	public:
		shok::ClassId DisplayClass{};
		shok::ModelId Model[4]{};
		bool DrawPlayerColor = false, CastShadow = true, RenderInFoW = false, HighQualityOnly = false;
		bool MapEditor_Rotateable = true, MapEditor_Placeable = true;
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

		[[nodiscard]] bool IsSettlerType() const;
		[[nodiscard]] bool IsBuildingType() const;
		[[nodiscard]] bool IsCEntityProperties() const;
		[[nodiscard]] bool IsOfCategory(shok::EntityCategory cat) const;

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;


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
		BB::CIDManagerEx* EntityTypeManagerAgain = nullptr;
		shok::Vector<ED::CDisplayEntityProps*> EntityTypesDisplayProps; // 11
	};
}
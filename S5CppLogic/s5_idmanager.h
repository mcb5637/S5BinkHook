#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include <format>
#include "Luapp/constexprTypename.h"

namespace BB {
	class IIDManager {
	public:
		virtual ~IIDManager() = default;

		static inline constexpr int vtp = 0x77F8B0;


		struct IDData {
			char* Name;
			unsigned int Hash;
		};
		PADDINGI(1);
		shok::Vector<IDData> TypeNames;

		// does not add
		int GetIdByName(const char* name);
		const char* GetNameByID(int id);
		int GetIDByNameOrCreate(const char* name); // throws if id invalid
		int GetIDByNameOrCreate(const char* name, int newid); // sets id id >0, throws otherwise or if id does not match or already used
		void RemoveID(int id); // remove highest id first, cause that way the vector gets shrunk. ids get reused, use this only for cleanup
		void DumpManagerToLuaGlobal(lua_State* L, const char* global);
		size_t size();
		void clear();
	};

	class IIDManagerEx : public IIDManager {
		// one more unknown v func, destroy?
	public:
		static inline constexpr int vtp = 0x77F8B8;
	};


	class CIDManagerEx : public IIDManagerEx {
	public:
		static inline constexpr int vtp = 0x77F8C4;

		static inline BB::CIDManagerEx** const AnimManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C838);
		static inline BB::CIDManagerEx** const EntityTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0x895DC0); // A0C834 same
		static inline BB::CIDManagerEx** const UpgradeCategoryManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C84C);
		static inline BB::CIDManagerEx** const EntityCategoryManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3C4); // 867838 same
		static inline BB::CIDManagerEx** const EntityCategoryManager2 = reinterpret_cast<BB::CIDManagerEx**>(0x867838); // same object
		static inline BB::CIDManagerEx** const DamageClassManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3B8); // A0D034 same
		static inline BB::CIDManagerEx** const TechnologiesManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3BC); // 8640EC same
		static inline BB::CIDManagerEx** const TechnologyCategoryManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3C0); // 864854 same
		static inline BB::CIDManagerEx** const AbilityManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3CC);
		static inline BB::CIDManagerEx** const GoodsManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3B4); // i dont think they are actually used somewhere..., A0D348 same
		static inline BB::CIDManagerEx** const ArmorClassManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C850); // you probably cannot add anything here, cause fixed array, 85A3C8 same
		static inline BB::CIDManagerEx** const AttachmentTypesManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3D0); // 85A3D4 too
		static inline BB::CIDManagerEx** const FeedBackEventManager = reinterpret_cast<BB::CIDManagerEx**>(0x880BA0); // ids are not correct here
		static inline BB::CIDManagerEx** const SoundsManager = reinterpret_cast<BB::CIDManagerEx**>(0x859F14); // 8979C8 same
		static inline BB::CIDManagerEx** const AmbientSoundsManager = reinterpret_cast<BB::CIDManagerEx**>(0x8979CC);
		static inline BB::CIDManagerEx** const ResourceTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0x885EE5C);
		static inline BB::CIDManagerEx** const FeedbackStateManager = reinterpret_cast<BB::CIDManagerEx**>(0x880C10);
		static inline BB::CIDManagerEx** const TaskManager = reinterpret_cast<BB::CIDManagerEx**>(0x898208);
		static inline BB::CIDManagerEx** const PrincipalTaskManager = reinterpret_cast<BB::CIDManagerEx**>(0x89820C);
		static inline BB::CIDManagerEx** const AnimCategoryManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C82C);
		static inline BB::CIDManagerEx** const AnimSetManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C830);
		static inline BB::CIDManagerEx** const EffectTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0CC7C);
		static inline BB::CIDManagerEx** const TaskListManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C848);
		static inline BB::CIDManagerEx** const BlessCategoryManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C858);
		static inline BB::CIDManagerEx** const WeatherEffectTextureManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0CC80); // lightning and snow
		static inline BB::CIDManagerEx** const TerrainTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0D5CC);
		static inline BB::CIDManagerEx** const TerrainTextureManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0D5D0);
		static inline BB::CIDManagerEx** const WaterTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0D9C8);
		static inline BB::CIDManagerEx** const ShorewaveTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA19F34);
		static inline BB::CIDManagerEx** const ModelManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C83C);
	};
	static_assert(sizeof(BB::CIDManagerEx) == 6 * 4);

	class CIDManager : public IIDManager {
	public:
		static inline constexpr int vtp = 0x76C028;

		static inline BB::CIDManager** const WidgetIDManager = reinterpret_cast<BB::CIDManager**>(0x8945C8);
	};
}

namespace CppLogic {
	template<class En>
	requires std::is_enum_v<En>&& std::is_same_v<int, std::underlying_type_t<En>>
	class EnumIdManager {
		BB::IIDManager* Manager;

	public:
		EnumIdManager(BB::IIDManager* mng) : Manager(mng) {

		}

		// does not add
		En GetIdByName(const char* name) {
			return static_cast<En>(Manager->GetIdByName(name));
		}
		const char* GetNameByID(En id) {
			return Manager->GetNameByID(static_cast<int>(id));
		}
		// throws if id invalid
		En GetIDByNameOrCreate(const char* name) {
			return static_cast<En>(Manager->GetIDByNameOrCreate(name));
		}
		// sets id id >0, throws otherwise or if id does not match or already used
		En GetIDByNameOrCreate(const char* name, En newid) {
			return static_cast<En>(Manager->GetIDByNameOrCreate(name, static_cast<int>(newid)));
		}
		// remove highest id first, cause that way the vector gets shrunk. ids get reused, use this only for cleanup
		void RemoveID(En id) {
			Manager->RemoveID(static_cast<int>(id));
		}
		void DumpManagerToLuaGlobal(lua_State* L, const char* global) {
			Manager->DumpManagerToLuaGlobal(L, global);
		}
		size_t size() {
			return Manager->size();
		}
		void clear() {
			Manager->clear();
		}
	};

	template<class En>
	struct IdManagerMapping {
	};
	template<>
	struct IdManagerMapping<shok::AnimationId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::AnimManager;
	};
	template<>
	struct IdManagerMapping<shok::EntityTypeId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::EntityTypeManager;
	};
	template<>
	struct IdManagerMapping<shok::UpgradeCategoryId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::UpgradeCategoryManager;
	};
	template<>
	struct IdManagerMapping<shok::EntityCategory> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::EntityCategoryManager;
	};
	template<>
	struct IdManagerMapping<shok::DamageClassId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::DamageClassManager;
	};
	template<>
	struct IdManagerMapping<shok::TechnologyId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::TechnologiesManager;
	};
	template<>
	struct IdManagerMapping<shok::TechnologyCategoryId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::TechnologyCategoryManager;
	};
	template<>
	struct IdManagerMapping<shok::AbilityId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::AbilityManager;
	};
	template<>
	struct IdManagerMapping<shok::Goods> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::GoodsManager;
	};
	template<>
	struct IdManagerMapping<shok::ArmorClassId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::ArmorClassManager;
	};
	template<>
	struct IdManagerMapping<shok::AttachmentType> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::AttachmentTypesManager;
	};
	template<>
	struct IdManagerMapping<shok::FeedbackEventIds> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::FeedBackEventManager;
	};
	template<>
	struct IdManagerMapping<shok::SoundId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::SoundsManager;
	};
	template<>
	struct IdManagerMapping<shok::AmbientSoundId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::AmbientSoundsManager;
	};
	template<>
	struct IdManagerMapping<shok::ResourceType> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::ResourceTypeManager;
	};
	template<>
	struct IdManagerMapping<shok::FeedbackStateId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::FeedbackStateManager;
	};
	template<>
	struct IdManagerMapping<shok::Task> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::TaskManager;
	};
	template<>
	struct IdManagerMapping<shok::PrincipalTaskId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::PrincipalTaskManager;
	};
	template<>
	struct IdManagerMapping<shok::AnimationCategoryId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::AnimCategoryManager;
	};
	template<>
	struct IdManagerMapping<shok::AnimSetId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::AnimSetManager;
	};
	template<>
	struct IdManagerMapping<shok::EffectTypeId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::EffectTypeManager;
	};
	template<>
	struct IdManagerMapping<shok::TaskListId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::TaskListManager;
	};
	template<>
	struct IdManagerMapping<shok::BlessCategoryId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::BlessCategoryManager;
	};
	template<>
	struct IdManagerMapping<shok::WeatherEffectTextureId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::WeatherEffectTextureManager;
	};
	template<>
	struct IdManagerMapping<shok::TerrainTypeId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::TerrainTypeManager;
	};
	template<>
	struct IdManagerMapping<shok::TerrainTextureId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::TerrainTextureManager;
	};
	template<>
	struct IdManagerMapping<shok::WaterTypeId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::WaterTypeManager;
	};
	template<>
	struct IdManagerMapping<shok::ShorewaveTypeId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::ShorewaveTypeManager;
	};
	template<>
	struct IdManagerMapping<shok::ModelId> {
		static inline BB::CIDManagerEx** const Manager = BB::CIDManagerEx::ModelManager;
	};
	template<>
	struct IdManagerMapping<shok::WidgetId> {
		static inline BB::CIDManager** const Manager = BB::CIDManager::WidgetIDManager;
	};

	template<class En>
	requires std::is_enum_v<En>
	EnumIdManager<En> GetIdManager() {
		static_assert(std::is_same_v<int, std::underlying_type_t<En>> && requires { IdManagerMapping<En>::Manager; }, "needs explicit specialization");
		auto mng = *IdManagerMapping<En>::Manager;
		if (mng == nullptr)
			throw std::runtime_error{std::format("{} manager not yet initialized", typename_details::type_name<En>())};
		return EnumIdManager<En>{mng};
	}
}

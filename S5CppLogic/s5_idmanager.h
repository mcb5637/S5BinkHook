#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

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
		static inline BB::CIDManagerEx** const GoodsManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3B4); // i dont think they are actually used somewhere...
		static inline BB::CIDManagerEx** const ArmorClassManager = reinterpret_cast<BB::CIDManagerEx**>(0x85A3C8); // you probably cannot add anything here, cause fixed array, A0C850 same
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
		static inline BB::CIDManagerEx** const EffectTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0CC7C); // A0C83C ?
		static inline BB::CIDManagerEx** const TaskListManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C848);
		static inline BB::CIDManagerEx** const BlessCategoryManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0C858);
		static inline BB::CIDManagerEx** const WeatherEffectTextureManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0CC80); // lightning and snow
		static inline BB::CIDManagerEx** const TerrainTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0D5CC);
		static inline BB::CIDManagerEx** const TerrainTextureManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0D5D0);
		static inline BB::CIDManagerEx** const WaterTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA0D9C8);
		static inline BB::CIDManagerEx** const ShorewaveTypeManager = reinterpret_cast<BB::CIDManagerEx**>(0xA19F34);
	};
	static_assert(sizeof(BB::CIDManagerEx) == 6 * 4);

	class CIDManager : public IIDManager {
	public:
		static inline constexpr int vtp = 0x76C028;
	};

}

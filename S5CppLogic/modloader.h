#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_framework.h"
#include "s5_mapdisplay.h"
#include "s5_classfactory.h"
#include "luaext.h"


namespace CppLogic::ModLoader {
	class ModLoader {
		static void Init(lua::State L, const char* mappath, std::string_view func);
		static void PreMapStart(lua_State* ingame, const char* name, const char* path, bool externalmap);
		static void PreSaveLoad(lua_State* ingame, Framework::GameModeStartMapData* data, bool externalmap);
		static void Log(lua::State L, const char* log);
		static void AddLib(lua::State L);
		static void RemoveLib(lua::State L);

		static constexpr std::string_view ModLoaderLib = "ModLoader";

		static constexpr std::string_view ModpackFolder = "ModPacks";

		class DataTypeLoader {
		public:
			virtual ~DataTypeLoader() = default;
			virtual void Reset() = 0;
			virtual void SanityCheck() = 0;
			virtual void RegisterFuncs(luaext::EState L) = 0;
		};
		template<class En>
		class DataTypeLoaderCommon : public DataTypeLoader {
		public:
			virtual void OnIdAllocated(En id) = 0;
			virtual void OnIdLoaded(En id) = 0;

			static auto IdManager() {
				return CppLogic::GetIdManager<En>();
			}
			static const char* TableName() {
				static_assert(sizeof(DataTypeLoaderCommon<En>) != sizeof(DataTypeLoaderCommon<En>));
				return nullptr;
			}
			static const char* FuncName() {
				static_assert(sizeof(DataTypeLoaderCommon<En>) != sizeof(DataTypeLoaderCommon<En>));
				return nullptr;
			}
			static void Load(En id, luaext::EState L) { // L is nullptr on reload
				static_assert(sizeof(DataTypeLoaderCommon<En>) != sizeof(DataTypeLoaderCommon<En>));
			}

			En GetId(luaext::EState L, int idx) {
				auto mng = IdManager();
				auto t = L.Type(idx);
				if (t == lua::LType::Number) {
					En id = static_cast<En>(L.CheckInt(idx));
					if (mng.GetNameByID(id) == nullptr) {
						throw lua::LuaException{std::format("invalid {} at {}: {} does not exist", typename_details::type_name<En>(), idx, static_cast<int>(id))};
					}
					return id;
				}
				else if (t == lua::LType::String) {
					const char* s = L.ToString(idx);
					En id = mng.GetIdByName(s);
					if (id == static_cast<En>(0)) {
						id = mng.GetIDByNameOrCreate(s);
						OnIdAllocated(id);
						if (TableName()) {
							L.Push(TableName());
							L.GetGlobal();
							L.PushValue(1);
							L.Push(id);
							L.SetTableRaw(-3);
							L.Pop(1);
						}
					}
					return id;
				}
				else {
					throw lua::LuaException{std::format("invalid {} at {}: invalid type", typename_details::type_name<En>(), idx)};
				}
			}
			int Preload(luaext::EState L) {
				En id = GetId(L, 1);
				L.Push(id);
				return 1;
			}
			int Load(luaext::EState L) {
				En id = GetId(L, 1);
				Load(id, L);
				L.Push(id);
				OnIdLoaded(id);
				return 1;
			}
		};
		template<class En>
		class DataTypeLoaderTracking : public DataTypeLoaderCommon<En> {
			std::vector<En> ToRemove, ToReload;

		public:
			virtual void Reset() override {
				while (!ToRemove.empty()) {
					En id = ToRemove.back();
					ToRemove.pop_back();
					UnLoad(id);
				}
				for (En id : ToReload) {
					DataTypeLoaderCommon<En>::Load(id, luaext::EState{nullptr});
				}
				ToReload.clear();
			}
			virtual void SanityCheck() override {
				static_assert(sizeof(DataTypeLoaderCommon<En>) != sizeof(DataTypeLoaderCommon<En>));
			}
			virtual void OnIdAllocated(En id) override {
				if (std::find(ToRemove.begin(), ToRemove.end(), id) == ToRemove.end())
					ToRemove.push_back(id);
			}
			virtual void OnIdLoaded(En id) override {
				if (std::find(ToRemove.begin(), ToRemove.end(), id) != ToRemove.end())
					return;
				if (std::find(ToReload.begin(), ToReload.end(), id) != ToReload.end())
					return;
				ToReload.push_back(id);
			}
			static void UnLoad(En id) {
				static_assert(sizeof(DataTypeLoaderCommon<En>) != sizeof(DataTypeLoaderCommon<En>));
			}

			static int Prel(lua::State L) {
				return Obj.Preload(luaext::EState{L});
			}
			static int Lo(lua::State L) {
				return Obj.Load(luaext::EState{L});
			}

			static constexpr bool RegisterFuncPreLoad = true;
			static constexpr bool RegisterFuncLoad = true;
			virtual void RegisterFuncs(luaext::EState L) override {
				if constexpr (RegisterFuncPreLoad) {
					L.Push("PreLoad");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Prel>();
					L.SetTableRaw(-3);
				}
				if constexpr (RegisterFuncLoad) {
					L.Push("Add");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Lo>();
					L.SetTableRaw(-3);
					L.Push("Reload");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Lo>();
					L.SetTableRaw(-3);
				}
			}

			static DataTypeLoaderTracking Obj;
		};
		template<class En>
		class DataTypeLoaderHalf : public DataTypeLoaderCommon<En> {
			std::vector<En> ToRemove;
			bool NeedsReload = false;

		public:
			virtual void Reset() override {
				while (!ToRemove.empty()) {
					En id = ToRemove.back();
					ToRemove.pop_back();
					UnLoad(id);
				}
				if (NeedsReload)
					Reload();
				NeedsReload = false;
			}
			virtual void SanityCheck() override {
				static_assert(sizeof(DataTypeLoaderHalf<En>) != sizeof(DataTypeLoaderHalf<En>));
			}
			virtual void OnIdAllocated(En id) override {
				if (std::find(ToRemove.begin(), ToRemove.end(), id) == ToRemove.end())
					ToRemove.push_back(id);
			}
			virtual void OnIdLoaded(En id) override {
				if (std::find(ToRemove.begin(), ToRemove.end(), id) != ToRemove.end())
					return;
				NeedsReload = true;
			}
			static void UnLoad(En id) {
				static_assert(sizeof(DataTypeLoaderHalf<En>) != sizeof(DataTypeLoaderHalf<En>));
			}
			void Reload() {
				static_assert(sizeof(DataTypeLoaderHalf<En>) != sizeof(DataTypeLoaderHalf<En>));
			}

			static int Prel(lua::State L) {
				return Obj.Preload(luaext::EState{L});
			}
			static int Lo(lua::State L) {
				return Obj.Load(luaext::EState{L});
			}

			static constexpr bool RegisterFuncPreLoad = true;
			static constexpr bool RegisterFuncLoad = true;
			virtual void RegisterFuncs(luaext::EState L) override {
				if constexpr (RegisterFuncPreLoad) {
					L.Push("PreLoad");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Prel>();
					L.SetTableRaw(-3);
				}
				if constexpr (RegisterFuncLoad) {
					L.Push("Add");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Lo>();
					L.SetTableRaw(-3);
					L.Push("Reload");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Lo>();
					L.SetTableRaw(-3);
				}
			}

			static DataTypeLoaderHalf Obj;
		};
		template<class En>
		class DataTypeLoaderReload : public DataTypeLoaderCommon<En> {
			bool NeedsReload = false;
		public:

			virtual void Reset() override {
				static_assert(sizeof(DataTypeLoaderReload<En>) != sizeof(DataTypeLoaderReload<En>));
			}
			virtual void SanityCheck() override {
				static_assert(sizeof(DataTypeLoaderReload<En>) != sizeof(DataTypeLoaderReload<En>));
			}
			virtual void OnIdAllocated(En id) override {
				NeedsReload = true;
			}
			virtual void OnIdLoaded(En id) override {
				NeedsReload = true;
			}

			static int Prel(lua::State L) {
				return Obj.Preload(luaext::EState{L});
			}
			static int Lo(lua::State L) {
				return Obj.Load(luaext::EState{L});
			}

			static constexpr bool RegisterFuncPreLoad = true;
			static constexpr bool RegisterFuncLoad = true;
			virtual void RegisterFuncs(luaext::EState L) override {
				if constexpr (RegisterFuncPreLoad) {
					L.Push("PreLoad");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Prel>();
					L.SetTableRaw(-3);
				}
				if constexpr (RegisterFuncLoad) {
					L.Push("Add");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Lo>();
					L.SetTableRaw(-3);
					L.Push("Reload");
					L.Push(DataTypeLoaderCommon<En>::FuncName());
					L.Concat(2);
					L.Push<Lo>();
					L.SetTableRaw(-3);
				}
			}

			static DataTypeLoaderReload Obj;
		};
		class UpgradeCategoriesLoader : public DataTypeLoaderCommon<shok::UpgradeCategoryId> {
			std::vector<shok::UpgradeCategoryId> ToRemoveSettler, ToRemoveBuilding;
		public:
			virtual void Reset();
			virtual void SanityCheck();
			virtual void RegisterFuncs(luaext::EState L);
			virtual void OnIdAllocated(shok::UpgradeCategoryId id);
			virtual void OnIdLoaded(shok::UpgradeCategoryId id);

			static int Prel(lua::State L);
			static int AddSettlerUpgradeCategory(lua::State L);
			static int AddBuildingUpgradeCategory(lua::State L);

			static UpgradeCategoriesLoader Obj;
		};
		class ExperienceClassesLoader : public DataTypeLoader {
			std::vector<shok::ExperienceClass> ToRemove, ToReload;
		public:
			virtual void Reset() override;
			virtual void SanityCheck() override;
			virtual void RegisterFuncs(luaext::EState L) override;

			static int Add(lua::State L);

			static ExperienceClassesLoader Obj;
		};
		class SoundGroupsLoader : public DataTypeLoader {
			std::vector<shok::SoundId> ToRemove;
		public:
			virtual void Reset() override;
			virtual void SanityCheck() override;
			virtual void RegisterFuncs(luaext::EState L) override;

			static int Add(lua::State L);

			static SoundGroupsLoader Obj;
		};
		class DirectXEffectLoader : public DataTypeLoader {
			std::vector<int> ToRemove;
		public:
			virtual void Reset() override;
			virtual void SanityCheck() override;
			virtual void RegisterFuncs(luaext::EState L) override;

			static int Add(lua::State L);

			static DirectXEffectLoader Obj;
		};

		static std::array<DataTypeLoader*, 19> Loaders;
		static std::array<DataTypeLoader*, 1> LoadersIngame;

		static bool Initialized;

		static void InitExtraECats();

		static int SetEntityTypeToReload(lua::State L);
		static int SetTaskListToReload(lua::State L);
		static int SetDamageclassesToReload(lua::State L);
		static int RefreshEntityCategoryCache(lua::State L);
		static int SanityCheck(lua::State L);
		static int GetModpackInfo(lua::State L);
		static int LoadModpackBBA(lua::State L);
		static int InvalidModPackPanic(lua::State L);
		static int GetModpacks(lua::State L);
		static int ReserializeEntityType(lua::State L);

		static constexpr std::array LuaFuncs{
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<SetTaskListToReload>("SetTaskListToReload"),
				lua::FuncReference::GetRef<SetDamageclassesToReload>("SetDamageclassesToReload"),
				lua::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
				lua::FuncReference::GetRef<SanityCheck>("SanityCheck"),
				lua::FuncReference::GetRef<GetModpackInfo>("GetModpackInfo"),
				lua::FuncReference::GetRef<LoadModpackBBA>("LoadModpackBBA"),
				lua::FuncReference::GetRef<InvalidModPackPanic>("InvalidModPackPanic"),
				lua::FuncReference::GetRef<ReserializeEntityType>("ReserializeEntityType"),
		};

		static constexpr std::array NoLoaderFuncs{
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<SetTaskListToReload>("SetTaskListToReload"),
				lua::FuncReference::GetRef<SetDamageclassesToReload>("SetDamageclassesToReload"),
				lua::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
				lua::FuncReference::GetRef<GetModpackInfo>("GetModpackInfo"),
				lua::FuncReference::GetRef<LoadModpackBBA>("LoadModpackBBA"),
				lua::FuncReference::GetRef<ReserializeEntityType>("ReserializeEntityType"),
		};

		static constexpr std::array Mainmenu{
				lua::FuncReference::GetRef<GetModpackInfo>("GetModpackInfo"),
				lua::FuncReference::GetRef<LoadModpackBBA>("LoadModpackBBA"),
				lua::FuncReference::GetRef<GetModpacks>("GetModpacks"),
		};

	public:
		static void Initialize();
		static void Cleanup(Framework::CMain::NextMode n);
		static void InitMainmenu(lua::State L);

		static bool IsInitialized();
		static void AddTaskListToRemove(shok::TaskListId id);

		static std::string GetModPackPath(std::string_view n);
	};

	struct ModpackDesc {
		std::string Name, BBAPath, LoaderPath, ScriptPath;
		std::vector<std::string> Required, Incompatible, Override;
		bool DataMod = false, ScriptMod = false, MainmenuMod = false, KeepArchive = false;
		bool UserRequestable = false;

		static const BB::SerializationData SerializationData[];
		static const BB::SerializationData SerializationDataEx[];
	};

	class ArchivePopHelper {
		std::string Archive;

		static int Remove(lua::State L);
		static int IsLoaded(lua::State L);
		static int ToString(lua::State L);
	public:
		inline ArchivePopHelper(std::string_view a) : Archive(a) {}
		inline ArchivePopHelper(std::string&& a) : Archive(std::move(a)) {}

		static constexpr const std::array LuaMethods = {
			lua::FuncReference::GetRef<Remove>("Remove"),
			lua::FuncReference::GetRef<IsLoaded>("IsLoaded"),
			lua::FuncReference::GetRef<ToString>("ToString"),
		};
	};
}

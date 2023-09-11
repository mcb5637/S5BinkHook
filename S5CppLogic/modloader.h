#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_framework.h"
#include "luaext.h"


namespace CppLogic::ModLoader {
	class ModLoader {
		static void Init(lua::State L, const char* mappath, const char* func);
		static void PreMapStart(lua_State* ingame, const char* name, const char* path, bool externalmap);
		static void PreSaveLoad(lua_State* ingame, Framework::GameModeStartMapData* data, bool externalmap);
		static void Log(lua::State L, const char* log);
		static void AddLib(lua::State L);
		static void RemoveLib(lua::State L);

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
					En id = static_cast<En>(L.ToInteger(idx));
					if (mng.GetNameByID(id) == nullptr) {
						throw lua::LuaException{std::format("invalid {} at {}: {} does not exist", typename_details::type_name<En>(), idx, static_cast<int>(id))};
					}
					return id;
				}
				else if (t == lua::LType::String) {
					const char* s = L.ToString(idx);
					En id = mng.GetIdByName(s);
					if (id == static_cast<En>(0) && TableName()) {
						id = mng.GetIDByNameOrCreate(s);
						OnIdAllocated(id);
						L.Push(TableName());
						L.GetGlobal();
						L.PushValue(1);
						L.Push(id);
						L.SetTableRaw(-3);
						L.Pop(1);
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

			virtual void RegisterFuncs(luaext::EState L) override {
				L.Push("PreLoad");
				L.Push(DataTypeLoaderCommon<En>::FuncName());
				L.Concat(2);
				L.Push<Prel>();
				L.SetTableRaw(-3);
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

			virtual void RegisterFuncs(luaext::EState L) override {
				L.Push("PreLoad");
				L.Push(DataTypeLoaderCommon<En>::FuncName());
				L.Concat(2);
				L.Push<Prel>();
				L.SetTableRaw(-3);
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

			virtual void RegisterFuncs(luaext::EState L) override {
				L.Push("PreLoad");
				L.Push(DataTypeLoaderCommon<En>::FuncName());
				L.Concat(2);
				L.Push<Prel>();
				L.SetTableRaw(-3);
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

		static std::array<DataTypeLoader*, 11> Loaders;
		static std::array<DataTypeLoader*, 1> LoadersIngame;

		static bool Initialized;
		static std::vector<int> SelectionTexturesToRemove;
		static std::vector<int> SelectionTexturesToReload;
		static std::vector<shok::TerrainTextureId> TerrainTexturesToRemove;
		static std::vector<shok::TerrainTextureId> TerrainTexturesToReload;
		static bool ReloadTerrainTypes;
		static std::vector<shok::ExperienceClass> ExperienceClassesToRemove;
		static std::vector<shok::ExperienceClass> ExperienceClassesToReload;
		static std::vector<shok::SoundId> SoundGroupsToRemove;
		static std::vector<shok::AnimSetId> AnimSetsToRemove;
		static std::vector<shok::AnimSetId> AnimSetsToReload;
		static std::vector<int> DirectXEffectsToFree;

		static int SetEntityTypeToReload(lua::State L);
		static int AddSelectionTexture(lua::State L);
		static int ReloadSelectionTexture(lua::State L);
		static int AddTerrainTexture(lua::State L);
		static int ReloadTerrainTexture(lua::State L);
		static int AddTerrainType(lua::State L);
		static int ReloadTerrainType(lua::State L);
		static int AddExperienceClass(lua::State L);
		static int ReloadExperienceClass(lua::State L);
		static int AddSounds(lua::State L);
		static int AddAnimSet(lua::State L);
		static int ReloadAnimSet(lua::State L);
		static int LoadDirectXEffect(lua::State L);
		static int RefreshEntityCategoryCache(lua::State L);
		static int SanityCheck(lua::State L);

		static constexpr std::array<lua::FuncReference, 15> LuaFuncs{ {
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<AddSelectionTexture>("AddSelectionTexture"),
				lua::FuncReference::GetRef<ReloadSelectionTexture>("ReloadSelectionTexture"),
				lua::FuncReference::GetRef<AddTerrainTexture>("AddTerrainTexture"),
				lua::FuncReference::GetRef<ReloadTerrainTexture>("ReloadTerrainTexture"),
				lua::FuncReference::GetRef<AddTerrainType>("AddTerrainType"),
				lua::FuncReference::GetRef<ReloadTerrainType>("ReloadTerrainType"),
				lua::FuncReference::GetRef<AddExperienceClass>("AddExperienceClass"),
				lua::FuncReference::GetRef<ReloadExperienceClass>("ReloadExperienceClass"),
				lua::FuncReference::GetRef<AddSounds>("AddSounds"),
				lua::FuncReference::GetRef<AddAnimSet>("AddAnimSet"),
				lua::FuncReference::GetRef<ReloadAnimSet>("ReloadAnimSet"),
				lua::FuncReference::GetRef<LoadDirectXEffect>("LoadDirectXEffect"),
				lua::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
				lua::FuncReference::GetRef<SanityCheck>("SanityCheck"),
		} };

		static constexpr std::array<lua::FuncReference, 2> NoLoaderFuncs{ {
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
		} };

	public:
		static void Initialize();
		static void Cleanup(Framework::CMain::NextMode n);

		static bool IsInitialized();
		static void AddTaskListToRemove(shok::TaskListId id);
	};
}

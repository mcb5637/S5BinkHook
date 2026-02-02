#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/s5_defines.h>
#include <shok/globals/s5_framework.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/globals/s5_classfactory.h>
#include <luaext.h>
#include <utility/ModFilesystem.h>


namespace CppLogic::ModLoader {
	class ModLoader {
		static void Init(luaext::State L, const char* mappath, std::string_view func, const std::function<void(luaext::State)>& pushMapInfo);
		static void PreMapStart(lua_State* ingame, const char* name, const char* path, bool externalmap);
		static void PostMapscriptLoaded();
		static void PreSaveLoad(lua_State* ingame, Framework::GameModeStartMapData* data, bool externalmap);
		static void Log(luaext::State L, const char* log);
		static void AddLib(luaext::State L);
		static void RemoveLib(luaext::State L);
		static void PreSave(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name);
		static void PostSave(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name);

		static constexpr std::string_view ModLoaderLib = "ModLoader";

		static constexpr std::string_view ModpackFolder = "ModPacks";

		static shok::String ModPackList;
		static size_t GUIDLength;
		static luaext::Reference SavegameValidOverride;

		class DataTypeLoader {
		public:
			virtual ~DataTypeLoader() = default;
			virtual void Reset() = 0;
			virtual void SanityCheck() = 0;
			virtual void RegisterFuncs(luaext::State L) = 0;
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
				return "";
			}
			static const char* FuncName() {
				static_assert(sizeof(DataTypeLoaderCommon<En>) != sizeof(DataTypeLoaderCommon<En>));
				return "";
			}
			static void Load(En id, luaext::State L) { // L is nullptr on reload
				static_assert(sizeof(DataTypeLoaderCommon<En>) != sizeof(DataTypeLoaderCommon<En>));
			}

			En GetId(luaext::State L, int idx) {
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
						// ReSharper disable once CppDFAConstantConditions
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
			int Preload(luaext::State L) {
				En id = GetId(L, 1);
				L.Push(id);
				return 1;
			}
			int Load(luaext::State L) {
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
					DataTypeLoaderCommon<En>::Load(id, luaext::State{nullptr});
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

			static int Prel(luaext::State L) {
				return Obj.Preload(luaext::State{L});
			}
			static int Lo(luaext::State L) {
				return Obj.Load(luaext::State{L});
			}

			static constexpr bool RegisterFuncPreLoad = true;
			static constexpr bool RegisterFuncLoad = true;
			virtual void RegisterFuncs(luaext::State L) override {
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

			static int Prel(luaext::State L) {
				return Obj.Preload(luaext::State{L});
			}
			static int Lo(luaext::State L) {
				return Obj.Load(luaext::State{L});
			}

			static constexpr bool RegisterFuncPreLoad = true;
			static constexpr bool RegisterFuncLoad = true;
			virtual void RegisterFuncs(luaext::State L) override {
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

			static int Prel(luaext::State L) {
				return Obj.Preload(luaext::State{L});
			}
			static int Lo(luaext::State L) {
				return Obj.Load(luaext::State{L});
			}

			static constexpr bool RegisterFuncPreLoad = true;
			static constexpr bool RegisterFuncLoad = true;
			virtual void RegisterFuncs(luaext::State L) override {
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
			virtual void RegisterFuncs(luaext::State L);
			virtual void OnIdAllocated(shok::UpgradeCategoryId id);
			virtual void OnIdLoaded(shok::UpgradeCategoryId id);

			static int Prel(luaext::State L);
			static int AddSettlerUpgradeCategory(luaext::State L);
			static int AddBuildingUpgradeCategory(luaext::State L);

			static UpgradeCategoriesLoader Obj;
		};
		class ExperienceClassesLoader : public DataTypeLoader {
			std::vector<shok::ExperienceClass> ToRemove, ToReload;
		public:
			virtual void Reset() override;
			virtual void SanityCheck() override;
			virtual void RegisterFuncs(luaext::State L) override;

			static int Add(luaext::State L);

			static ExperienceClassesLoader Obj;
		};
		class SoundGroupsLoader : public DataTypeLoader {
			std::vector<shok::SoundId> ToRemove;
		public:
			virtual void Reset() override;
			virtual void SanityCheck() override;
			virtual void RegisterFuncs(luaext::State L) override;

			static int Add(luaext::State L);

			static SoundGroupsLoader Obj;
		};
		class DirectXEffectLoader : public DataTypeLoader {
			std::vector<int> ToRemove;
		public:
			virtual void Reset() override;
			virtual void SanityCheck() override;
			virtual void RegisterFuncs(luaext::State L) override;

			static int Add(luaext::State L);

			static DirectXEffectLoader Obj;
		};
		class FeedbackEventSoundDataLoader : public DataTypeLoader {
			// does not need to get reset, automatically reloaded anyway
		public:
			virtual void Reset() override;
			virtual void SanityCheck() override;
			virtual void RegisterFuncs(luaext::State L) override;

			static int Add(luaext::State L);

			static FeedbackEventSoundDataLoader Obj;
		};

		static std::array<DataTypeLoader*, 21> Loaders;
		static std::array<DataTypeLoader*, 1> LoadersIngame;

		static bool Initialized;

		static void InitExtraECats();

		static int SetEntityTypeToReload(luaext::State L);
		static int SetTaskListToReload(luaext::State L);
		static int SetDamageclassesToReload(luaext::State L);
		static int RefreshEntityCategoryCache(luaext::State L);
		static int SanityCheck(luaext::State L);
		static int GetModpackInfo(luaext::State L);
		static int LoadModpackBBA(luaext::State L);
		static int CreateModpackRedirectLayer(luaext::State L);
		static int InvalidModPackPanic(luaext::State L);
		static int GetModpacks(luaext::State L);
		static int ReserializeEntityType(luaext::State L);
		static int ReserializeTaskList(luaext::State L);
		static int ReserializeModel(luaext::State L);
		static int SetModPackList(luaext::State L);
		static int GetModPackList(luaext::State L);
		static int OverrideSavegameValid(luaext::State L);
		static int LoadStringTableTextOverrides(luaext::State L);
		static int MapGetModPacks(luaext::State L);
		static int GetEntityTypeMem(luaext::State L);
		static int GetTaskListMem(luaext::State l);
		static int GetTechnologyMem(luaext::State l);
		static int GetEffectTypeMem(luaext::State l);
		static int GetFeedbackEventMem(luaext::State l);
		static int SetMaxPlayers(luaext::State L);
		static int GetMaxPlayers(luaext::State L);

		static constexpr std::array LuaFuncs{
				luaext::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				luaext::FuncReference::GetRef<SetTaskListToReload>("SetTaskListToReload"),
				luaext::FuncReference::GetRef<SetDamageclassesToReload>("SetDamageclassesToReload"),
				luaext::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
				luaext::FuncReference::GetRef<SanityCheck>("SanityCheck"),
				luaext::FuncReference::GetRef<GetModpackInfo>("GetModpackInfo"),
				luaext::FuncReference::GetRef<LoadModpackBBA>("LoadModpackBBA"),
				luaext::FuncReference::GetRef<CreateModpackRedirectLayer>("CreateModpackRedirectLayer"),
				luaext::FuncReference::GetRef<InvalidModPackPanic>("InvalidModPackPanic"),
				luaext::FuncReference::GetRef<ReserializeEntityType>("ReserializeEntityType"),
				luaext::FuncReference::GetRef<ReserializeTaskList>("ReserializeTaskList"),
				luaext::FuncReference::GetRef<SetModPackList>("SetModPackList"),
				luaext::FuncReference::GetRef<GetModPackList>("GetModPackList"),
				luaext::FuncReference::GetRef<LoadStringTableTextOverrides>("LoadStringTableTextOverrides"),
				luaext::FuncReference::GetRef<MapGetModPacks>("MapGetModPacks"),
				luaext::FuncReference::GetRef<GetEntityTypeMem>("GetEntityTypeMem"),
				luaext::FuncReference::GetRef<GetTaskListMem>("GetTaskListMem"),
				luaext::FuncReference::GetRef<GetTechnologyMem>("GetTechnologyMem"),
				luaext::FuncReference::GetRef<GetEffectTypeMem>("GetEffectTypeMem"),
				luaext::FuncReference::GetRef<GetFeedbackEventMem>("GetFeedbackEventMem"),
				luaext::FuncReference::GetRef<SetMaxPlayers>("SetMaxPlayers"),
				luaext::FuncReference::GetRef<GetMaxPlayers>("GetMaxPlayers"),
		};

		static constexpr std::array NoLoaderFuncs{
				luaext::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				luaext::FuncReference::GetRef<SetTaskListToReload>("SetTaskListToReload"),
				luaext::FuncReference::GetRef<SetDamageclassesToReload>("SetDamageclassesToReload"),
				luaext::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
				luaext::FuncReference::GetRef<GetModpackInfo>("GetModpackInfo"),
				luaext::FuncReference::GetRef<LoadModpackBBA>("LoadModpackBBA"),
				luaext::FuncReference::GetRef<ReserializeEntityType>("ReserializeEntityType"),
				luaext::FuncReference::GetRef<ReserializeTaskList>("ReserializeTaskList"),
				luaext::FuncReference::GetRef<ReserializeModel>("ReserializeModel"),
				luaext::FuncReference::GetRef<LoadStringTableTextOverrides>("LoadStringTableTextOverrides"),
				luaext::FuncReference::GetRef<MapGetModPacks>("MapGetModPacks"),
				luaext::FuncReference::GetRef<GetMaxPlayers>("GetMaxPlayers"),
		};

		static constexpr std::array Mainmenu{
				luaext::FuncReference::GetRef<GetModpackInfo>("GetModpackInfo"),
				luaext::FuncReference::GetRef<LoadModpackBBA>("LoadModpackBBA"),
				luaext::FuncReference::GetRef<GetModpacks>("GetModpacks"),
				luaext::FuncReference::GetRef<OverrideSavegameValid>("OverrideSavegameValid"),
				luaext::FuncReference::GetRef<LoadStringTableTextOverrides>("LoadStringTableTextOverrides"),
				luaext::FuncReference::GetRef<MapGetModPacks>("MapGetModPacks"),
		};

	public:
		static void Initialize();
		static void Cleanup(Framework::CMain::NextMode n);
		static void InitMainmenu(luaext::State L);

		static bool IsInitialized();
		static void AddTaskListToRemove(shok::TaskListId id);

		static std::string GetModPackPath(std::string_view n);
	};

	struct ModpackDesc {
		std::string Name, BBAPath, LoaderPath, ScriptPath, MainmenuPath, Version, Description;
		std::vector<std::string> Required, Incompatible, Override;
		bool DataMod = false, ScriptMod = false, MainmenuMod = false, KeepArchive = false;
		bool UserRequestable = false, ScriptLib = false;

		static const BB::SerializationData SerializationData[];
		static const BB::SerializationData SerializationDataEx[];
	};

	struct ExtendedMapInfo : Framework::MapInfo {
		struct SModPacks {
			std::vector<std::string> Required, Incompatible, UserRequestable;

			static const BB::SerializationData SerializationData[];
		} ModPacks;

		static const BB::SerializationData SerializationData[];
	};

	class ArchivePopHelper {
		std::string Archive;
	protected:
		BB::IFileSystem* FS; // not owning, may be already deleted!

		static int Remove(luaext::State L);
		static int IsLoaded(luaext::State L);
		static int ToString(luaext::State L);
	public:
		virtual ~ArchivePopHelper() = default;

		inline ArchivePopHelper(std::string_view a, BB::IFileSystem* fs) : Archive(a), FS(fs) {}
		inline ArchivePopHelper(std::string&& a, BB::IFileSystem* fs) : Archive(std::move(a)), FS(fs) {}

		static constexpr const std::array LuaMethods = {
			luaext::FuncReference::GetRef<Remove>("Remove"),
			luaext::FuncReference::GetRef<IsLoaded>("IsLoaded"),
			luaext::FuncReference::GetRef<ToString>("ToString"),
		};
	};

	class ArchiveRedirectHelper : public ArchivePopHelper {

		CppLogic::Mod::FileSystem::RedirectFileSystem* CheckStillValid();

		static int Get(luaext::State L);
		static int Set(luaext::State L);
	public:
		inline ArchiveRedirectHelper(std::string_view a, CppLogic::Mod::FileSystem::RedirectFileSystem* fs) : ArchivePopHelper(a, fs) {}
		inline ArchiveRedirectHelper(std::string&& a, CppLogic::Mod::FileSystem::RedirectFileSystem* fs) : ArchivePopHelper(std::move(a), fs) {}


		using InheritsFrom = std::tuple<ArchivePopHelper>;

		static constexpr const std::array LuaMethods = {
			luaext::FuncReference::GetRef<Get>("Get"),
			luaext::FuncReference::GetRef<Set>("Set"),
		};
	};
}

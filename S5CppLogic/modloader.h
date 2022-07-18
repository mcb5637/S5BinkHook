#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_framework.h"
#include "Luapp/luapp50.h"


namespace CppLogic::ModLoader {
	class ModLoader {
		static void Init(lua::State L, const char* mappath, const char* func);
		static void PreMapStart(lua_State* ingame, const char* name, const char* path, bool externalmap);
		static void PreSaveLoad(lua_State* ingame, Framework::GameModeStartMapData* data, bool externalmap);
		static void Log(lua::State L, const char* log);
		static void AddLib(lua::State L);
		static void RemoveLib(lua::State L);

		static bool Initialized;
		static std::vector<int> EntityTypesToRemove;
		static std::vector<int> EntityTypesToReload;
		static bool ReloadEffectTypes;
		static std::vector<int> TaskListsToRemove;
		static std::vector<int> TechsToRemove;
		static std::vector<int> ModelsToRemove;
		static bool ReloadModels;
		static std::vector<int> TexturesToRemove;
		static std::vector<int> TexturesToReload;
		static std::vector<int> AnimsToRemove;
		static std::vector<int> AnimsToReload;
		static std::vector<int> SettlerUCatsToRemove;

		static int AddEntityType(lua::State L);
		static int ReloadEntityType(lua::State L);
		static int SetEntityTypeToReload(lua::State L);
		static int ReloadEffectType(lua::State L);
		static int AddEffectType(lua::State L);
		static int ReloadTaskList(lua::State L);
		static int AddTaskList(lua::State L);
		static int ReloadTechnology(lua::State L);
		static int AddTechnology(lua::State L);
		static int AddModel(lua::State L);
		static int ReloadModel(lua::State L);
		static int AddGUITexture(lua::State L);
		static int ReloadGUITexture(lua::State L);
		static int AddAnimation(lua::State L);
		static int ReloadAnimation(lua::State L);
		static int AddSettlerUpgradeCategory(lua::State L);

		static constexpr std::array<lua::FuncReference, 16> LuaFuncs{ {
				lua::FuncReference::GetRef<AddEntityType>("AddEntityType"),
				lua::FuncReference::GetRef<ReloadEntityType>("ReloadEntityType"),
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<ReloadEffectType>("ReloadEffectType"),
				lua::FuncReference::GetRef<AddEffectType>("AddEffectType"),
				lua::FuncReference::GetRef<ReloadTaskList>("ReloadTaskList"),
				lua::FuncReference::GetRef<AddTaskList>("AddTaskList"),
				lua::FuncReference::GetRef<ReloadTechnology>("ReloadTechnology"),
				lua::FuncReference::GetRef<AddTechnology>("AddTechnology"),
				lua::FuncReference::GetRef<AddModel>("AddModel"),
				lua::FuncReference::GetRef<ReloadModel>("ReloadModel"),
				lua::FuncReference::GetRef<AddGUITexture>("AddGUITexture"),
				lua::FuncReference::GetRef<ReloadGUITexture>("ReloadGUITexture"),
				lua::FuncReference::GetRef<AddAnimation>("AddAnimation"),
				lua::FuncReference::GetRef<ReloadAnimation>("ReloadAnimation"),
				lua::FuncReference::GetRef<AddSettlerUpgradeCategory>("AddSettlerUpgradeCategory"),
		} };

		static constexpr std::array<lua::FuncReference, 3> NoLoaderFuncs{ {
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<AddGUITexture>("AddGUITexture"),
				lua::FuncReference::GetRef<ReloadGUITexture>("ReloadGUITexture"),
		} };

	public:
		static void Initialize();
		static void Cleanup(Framework::CMain::NextMode n);

		static bool IsInitialized();
		static void AddTaskListToRemove(int id);
	};
}

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

		static int AddEntityType(lua::State L);
		static int ReloadEntityType(lua::State L);
		static int SetEntityTypeToReload(lua::State L);
		static int ReloadEffectType(lua::State L);
		static int AddEffectType(lua::State L);
		static int ReloadTaskList(lua::State L);
		static int AddTaskList(lua::State L);
		static int ReloadTechnology(lua::State L);
		static int AddTechnology(lua::State L);

		static constexpr std::array<lua::FuncReference, 9> LuaFuncs{ {
				lua::FuncReference::GetRef<AddEntityType>("AddEntityType"),
				lua::FuncReference::GetRef<ReloadEntityType>("ReloadEntityType"),
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<ReloadEffectType>("ReloadEffectType"),
				lua::FuncReference::GetRef<AddEffectType>("AddEffectType"),
				lua::FuncReference::GetRef<ReloadTaskList>("ReloadTaskList"),
				lua::FuncReference::GetRef<AddTaskList>("AddTaskList"),
				lua::FuncReference::GetRef<ReloadTechnology>("ReloadTechnology"),
				lua::FuncReference::GetRef<AddTechnology>("AddTechnology"),
		} };

	public:
		static void Initialize();
		static void Cleanup(Framework::CMain::NextMode n);

		static bool IsInitialized();
		static void AddTaskListToRemove(int id);
	};
}

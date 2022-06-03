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

		static std::vector<int> EntityTypesToRemove;
		static std::vector<int> EntityTypesToReload;
		static bool ReloadEffectTypes;

		static int AddEntityType(lua::State L);
		static int ReloadEntityType(lua::State L);
		static int SetEntityTypeToReload(lua::State L);
		static int ReloadEffectType(lua::State L);
		static int AddEffectType(lua::State L);

		static constexpr std::array<lua::FuncReference, 5> LuaFuncs{ {
				lua::FuncReference::GetRef<AddEntityType>("AddEntityType"),
				lua::FuncReference::GetRef<ReloadEntityType>("ReloadEntityType"),
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<ReloadEffectType>("ReloadEffectType"),
				lua::FuncReference::GetRef<AddEffectType>("AddEffectType"),
		} };

	public:
		static void Initialize();
		static void Cleanup(Framework::CMain::NextMode n);
	};
}

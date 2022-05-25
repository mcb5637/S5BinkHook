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

	public:
		static void Initialize();
		static void Cleanup(lua::State L);
	};
}

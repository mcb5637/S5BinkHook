#include "pch.h"
#include "l_mem.h"
#include "pch.h"

#include "luaimport.h"
#include "luaext.h"
#include "s5data.h"

int l_mem_setFPU(lua_State* L) {
	shok_SetHighPrecFPU();
	return 0;
}

int l_mem_reloadCutscene(lua_State* L) {
	const char* data = luaL_optlstring(L, 1, "Maps\\ExternalMap", nullptr);
	(**shok_ECS_CManagerObject)->ReloadCutscene(data);
	return 0;
}

int l_mem_getAnimIdFromName(lua_State* L) {
	const char* data = luaL_checkstring(L, 1);
	int id = (*shok_BB_CIDManagerExObj)->GetAnimIdByName(data);
	lua_pushnumber(L, id);
	return 1;
}

void l_mem_init(lua_State* L)
{
	luaext_registerFunc(L, "SetFPU", &l_mem_setFPU);
	luaext_registerFunc(L, "ReloadCutscene", &l_mem_reloadCutscene);
	luaext_registerFunc(L, "GetAnimIdFromName", &l_mem_getAnimIdFromName);
}
//CppLogic.Memory.ReloadCutscene
//CppLogic.Memory.GetAnimIdFromName("CU_Barbarian_LeaderClub1_Walk")
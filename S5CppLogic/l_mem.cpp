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
	void* vt = (***shok_ECS_CManagerObject).vtable;
	if ((int)vt != 0x7860B4)
		return luaL_error(L, "vt");
	((shok_vtable_ECS_CManager*)vt)->ReloadCutscene(**shok_ECS_CManagerObject, data);
	return 0;
}

void l_mem_init(lua_State* L)
{
	luaext_registerFunc(L, "SetFPU", &l_mem_setFPU);
	luaext_registerFunc(L, "ReloadCutscene", &l_mem_reloadCutscene);
}
//CppLogic.Memory.ReloadCutscene
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

void l_mem_init(lua_State* L)
{
	luaext_registerFunc(L, "SetFPU", &l_mem_setFPU);
}

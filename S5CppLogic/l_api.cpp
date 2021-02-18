#include "pch.h"
#include "l_api.h"
#include "luaext.h"

int l_api_eval(lua_State* L) {
	size_t strlen = 0;
	const char* s = luaL_checklstring(L, 1, &strlen);
	int r = luaL_loadbuffer(L, s, strlen, "Eval");
	lua_pushboolean(L, r==0);
	return 2;
}

int l_api_log(lua_State* L) {
	size_t strlen = 0;
	const char* s = luaL_checklstring(L, 1, &strlen);
	// use errorfb to get stack trace?
	shok_logString("%s\n", s);
	return 0;
}

void l_api_init(lua_State* L)
{
	luaext_registerFunc(L, "Eval", &l_api_eval);
	luaext_registerFunc(L, "Log", &l_api_log);
}

// CppLogic.API.Log("string")
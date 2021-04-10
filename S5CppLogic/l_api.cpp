#include "pch.h"
#include "l_api.h"
#include "luaext.h"

void l_api_checkEvalEnabled(lua_State* L) {
	int t = lua_gettop(L);
	lua_getglobal(L, "XNetwork");
	if (!lua_isnil(L, -1)) {
		lua_pushstring(L, "EXTENDED_GameInformation_IsLuaDebuggerDisabled");
		lua_rawget(L, -2);
		if (!lua_isnil(L, -1)) {
			lua_pcall(L, 0, 1, 0);
			if (lua_toboolean(L, -1))
				luaL_error(L, "LuaDebugger disabled for MP");
		}
	}
	lua_settop(L, t);
}

int l_api_eval(lua_State* L) {
	l_api_checkEvalEnabled(L);
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

int l_api_getfile(lua_State* L) {
	size_t strlen = 0;
	const char* s = luaL_checklstring(L, 1, &strlen);
	const char* data = ReadFileToString(s, &strlen);
	if (!data)
		return 0;
	lua_pushlstring(L, data, strlen);
	delete[] data;
	return 1;
}

int l_api_hasfile(lua_State* L) {
	size_t strlen = 0;
	const char* s = luaL_checklstring(L, 1, &strlen);
	lua_pushboolean(L, DoesFileExist(s));
	return 1;
}

int l_api_loadString(lua_State* L) {
	l_api_checkEvalEnabled(L);
	size_t strlen = 0;
	const char* s = luaL_checklstring(L, 1, &strlen);
	size_t strlen2 = 0;
	const char* na = luaL_checklstring(L, 2, &strlen2);
	return shok_loadBuffer(L, s, strlen, na)-2;
}

void l_api_init(lua_State* L)
{
	luaext_registerFunc(L, "Eval", &l_api_eval);
	luaext_registerFunc(L, "Log", &l_api_log);
	luaext_registerFunc(L, "ReadFileAsString", &l_api_getfile);
	luaext_registerFunc(L, "DoesFileExist", &l_api_hasfile);
	luaext_registerFunc(L, "LoadString", &l_api_loadString);
}

// CppLogic.API.Log("string")
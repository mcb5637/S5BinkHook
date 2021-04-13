#include "pch.h"
#include "l_api.h"
#include "luaext.h"
#include <libloaderapi.h>

void l_api_checkEvalEnabled(lua_State* L) {
	if (GetModuleHandle(L"SCELoader.dll"))
		luaL_error(L, "Loading lua code disabled for Kimichura");
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
	std::string str = std::string(s);
	if (str.rfind("data\\maps\\externalmap\\", 0) != 0)
		luaL_error(L, "not a map file");
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

int l_api_doString(lua_State* L) {
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
	luaext_registerFunc(L, "DoString", &l_api_doString);
}

// CppLogic.API.Log("string")
#include "pch.h"
#include "l_api.h"
#include "luaext.h"
#include <libloaderapi.h>

void l_api_checkEvalEnabled(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "Loading lua code disabled for Kimichura");
}

bool l_apiIsExternalmap(const char* s) {
	std::string str = std::string(s);
	return str.rfind("data\\maps\\externalmap\\", 0) != std::string::npos;
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
	if (!l_apiIsExternalmap(s))
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

int l_api_mapGetDataPath(lua_State* L) {
	int ty = luaL_checkint(L, 2);
	const char* cn = lua_tostring(L, 3); // optional
	const char* n = luaL_checkstring(L, 1);
	shok_framework_campagnInfo* ci = (*shok_Framework_CMainObj)->GetCampagnInfo(ty, cn);
	luaext_assertPointer(L, ci, "invalid map type/campagn");
	shok_framework_mapinfo* i = ci->GetMapInfoByName(n);
	luaext_assertPointer(L, i, "invalid map");
	lua_pushstring(L, i->MapFilePath.c_str());
	return 1;
}

int l_api_saveGetMap(lua_State* L) {
	const char* save = luaL_checkstring(L, 1);
	shok_framework_saveData* sdata = shok_getframework_saveDataObj();
	luaext_assert(L, sdata->LoadSaveData(save), "save doesnt exist");
	lua_pushstring(L, sdata->CurrentSave->MapData.MapName.c_str());
	lua_pushnumber(L, sdata->CurrentSave->MapData.MapType);
	lua_pushstring(L, sdata->CurrentSave->MapData.MapCampagnName.c_str());
	lua_pushstring(L, sdata->CurrentSave->MapData.MapGUID.c_str());
	return 4;
}

void l_api_init(lua_State* L)
{
	luaext_registerFunc(L, "Eval", &l_api_eval);
	luaext_registerFunc(L, "Log", &l_api_log);
	luaext_registerFunc(L, "ReadFileAsString", &l_api_getfile);
	luaext_registerFunc(L, "DoesFileExist", &l_api_hasfile);
	luaext_registerFunc(L, "DoString", &l_api_doString);
	luaext_registerFunc(L, "MapGetDataPath", &l_api_mapGetDataPath);
	luaext_registerFunc(L, "SaveGetMapInfo", &l_api_saveGetMap);
}

// CppLogic.API.Log("string")
// CppLogic.API.ReadFileAsString("data\\maps\\externalmap\\info.xml")
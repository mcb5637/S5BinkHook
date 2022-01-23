#include "pch.h"
#include "l_api.h"
#include "luaext.h"
#include <libloaderapi.h>
#include <sstream>

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

int l_api_stacktrace(lua_State* L) {
	int lvl = 0;
	lua_Debug ar;
	std::ostringstream trace{};
	while (lua_getstack(L, lvl, &ar)) {
		if (lua_getinfo(L, "nSl", &ar)) {
			trace << ar.what << " ";
			trace << ar.namewhat << " ";
			trace << (ar.name ? ar.name : "null") << " (defined in:";
			trace << ar.short_src << ":";
			trace << ar.currentline << ")\r\n";
		}
		lvl++;
	}
	lua_pushstring(L, trace.str().c_str());
	return 1;
}

int l_api_getfuncdebug(lua_State* L) {
	luaext_assert(L, lua_isfunction(L, 1), "no func");
	lua_Debug ar;
	lua_pushvalue(L, 1);
	lua_getinfo(L, ">Snlu", &ar);
	lua_newtable(L);
	lua_pushstring(L, "name");
	lua_pushstring(L, ar.name);
	lua_rawset(L, -3);
	lua_pushstring(L, "namewhat");
	lua_pushstring(L, ar.namewhat);
	lua_rawset(L, -3);
	lua_pushstring(L, "nups");
	lua_pushnumber(L, ar.nups);
	lua_rawset(L, -3);
	lua_pushstring(L, "short_src");
	lua_pushstring(L, ar.short_src);
	lua_rawset(L, -3);
	lua_pushstring(L, "linedefined");
	lua_pushnumber(L, ar.linedefined);
	lua_rawset(L, -3);
	lua_pushstring(L, "what");
	lua_pushstring(L, ar.what);
	lua_rawset(L, -3);
	return 1;
}

int l_api_getfile(lua_State* L) {
	size_t strlen = 0;
	const char* s = luaL_checklstring(L, 1, &strlen);
#ifndef _DEBUG
	if (!l_apiIsExternalmap(s))
		luaL_error(L, "not a map file");
#endif
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
	shok_framework_campagnInfo* ci = (*shok_Framework_CMain::GlobalObj)->GetCampagnInfo(ty, cn);
	luaext_assertPointer(L, ci, "invalid map type/campagn");
	shok_framework_mapinfo* i = ci->GetMapInfoByName(n);
	luaext_assertPointer(L, i, "invalid map");
	lua_pushstring(L, i->MapFilePath.c_str());
	return 1;
}

int l_api_saveGetMap(lua_State* L) {
	const char* save = luaL_checkstring(L, 1);
	shok_framework_saveData* sdata = shok_framework_saveData::GlobalObj();
	luaext_assert(L, sdata->LoadSaveData(save), "save doesnt exist");
	lua_pushstring(L, sdata->CurrentSave->MapData.MapName.c_str());
	lua_pushnumber(L, sdata->CurrentSave->MapData.MapType);
	lua_pushstring(L, sdata->CurrentSave->MapData.MapCampagnName.c_str());
	lua_pushstring(L, sdata->CurrentSave->MapData.MapGUID.c_str());
	return 4;
}

int l_api_getGDB(lua_State* L) {
	lua_newtable(L);
	std::function<void(shok_GDBEntry*)> a = [L,&a](shok_GDBEntry* e) {
		lua_pushstring(L, e->Key.c_str());
		if (e->Data->vtable == shok_GDB_CString::vtp)
			lua_pushstring(L, static_cast<shok_GDB_CString*>(e->Data)->Data.c_str());
		else if (e->Data->vtable == shok_GDB_CValue::vtp)
			lua_pushnumber(L, static_cast<shok_GDB_CValue*>(e->Data)->Data);
		else if (e->Data->vtable == shok_GDB_CList::vtp) {
			luaL_checkstack(L, 4, "stackoverflow");
			lua_newtable(L);
			static_cast<shok_GDB_CList*>(e->Data)->Entries.ForAll(a);
		}
		else
			lua_pushboolean(L, true);
		lua_rawset(L, -3);
	};
	(*shok_Framework_CMain::GlobalObj)->GDB.Entries.ForAll(a);
	return 1;
}

void l_api_getruntimesore() {
	lua_State* mm = mainmenu_state;
	lua_pushlightuserdata(mm, &l_api_getruntimesore);
	lua_rawget(mm, LUA_REGISTRYINDEX);
	if (lua_isnil(mm, -1)) {
		lua_newtable(mm);
		lua_pushlightuserdata(mm, &l_api_getruntimesore);
		lua_pushvalue(mm, -2);
		lua_rawset(mm, LUA_REGISTRYINDEX);
	}
}
int l_api_runtimestore_set(lua_State* L) {
	lua_State* mm = mainmenu_state;
	int t = lua_gettop(mm);

	const char* s = luaL_checkstring(L, 1);
	const char* ds = nullptr;
	lua_Number di = 0;
	int ty = lua_type(L, 2);
	luaext_assert(L, ty == LUA_TNUMBER || ty == LUA_TSTRING || ty == LUA_TNIL, "not saveable");
	if (ty == LUA_TNUMBER)
		di = lua_tonumber(L, 2);
	else if (ty == LUA_TSTRING)
		ds = lua_tostring(L, 2);
	l_api_getruntimesore();
	lua_pushstring(mm, s);
	if (ty == LUA_TNUMBER)
		lua_pushnumber(mm, di);
	else if (ty == LUA_TSTRING)
		lua_pushstring(mm, ds);
	else
		lua_pushnil(mm);
	lua_rawset(mm, -3);

	lua_settop(mm, t);
	return 0;
}
int l_api_runtimestore_get(lua_State* L) {
	lua_State* mm = mainmenu_state;
	int t = lua_gettop(mm);

	const char* s = luaL_checkstring(L, 1);
	l_api_getruntimesore();
	lua_pushstring(mm, s);
	lua_rawget(mm, -2);
	int ty = lua_type(mm, -1);
	const char* ds = nullptr;
	lua_Number di = 0;
	if (ty == LUA_TNUMBER)
		di = lua_tonumber(mm, -1);
	else if (ty == LUA_TSTRING)
		ds = lua_tostring(mm, -1);

	lua_settop(mm, t);

	if (ty == LUA_TNUMBER)
		lua_pushnumber(L, di);
	else if (ty == LUA_TSTRING)
		lua_pushstring(L, ds);
	else
		lua_pushnil(L);

	return 1;
}

int l_api_dumpExtraTables(lua_State* L) {
	(*shok_BB_CIDManagerEx::AnimManager)->DumpManagerToLuaGlobal(L, "Animations");
	(*shok_BB_CIDManagerEx::TerrainTypeManager)->DumpManagerToLuaGlobal(L, "TerrainTypes");
	(*shok_BB_CIDManagerEx::WaterTypeManager)->DumpManagerToLuaGlobal(L, "WaterTypes");
	(*shok_BB_CIDManagerEx::ArmorClassManager)->DumpManagerToLuaGlobal(L, "ArmorClasses");
	return 0;
}

void l_api_init(lua_State* L)
{
	luaext_registerFunc(L, "Eval", &l_api_eval);
	luaext_registerFunc(L, "Log", &l_api_log);
	luaext_registerFunc(L, "StackTrace", &l_api_stacktrace);
	luaext_registerFunc(L, "ReadFileAsString", &l_api_getfile);
	luaext_registerFunc(L, "DoesFileExist", &l_api_hasfile);
	luaext_registerFunc(L, "DoString", &l_api_doString);
	luaext_registerFunc(L, "MapGetDataPath", &l_api_mapGetDataPath);
	luaext_registerFunc(L, "SaveGetMapInfo", &l_api_saveGetMap);
	luaext_registerFunc(L, "GetGDB", &l_api_getGDB);
	luaext_registerFunc(L, "RuntimeStoreSet", &l_api_runtimestore_set);
	luaext_registerFunc(L, "RuntimeStoreGet", &l_api_runtimestore_get);
	luaext_registerFunc(L, "CreateExtraDataTables", &l_api_dumpExtraTables);
	luaext_registerFunc(L, "GetFuncDebug", &l_api_getfuncdebug);
}

// CppLogic.API.Log("string")
// CppLogic.API.ReadFileAsString("data\\maps\\externalmap\\info.xml")
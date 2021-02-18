#include "pch.h"
#include "luaext.h"
#include "luaimport.h"
#include "s5data.h"

void luaext_registerFunc(lua_State* L, const char* name, lua_CFunction func)
{
	lua_pushstring(L, name);
	lua_pushcfunction(L, func);
	lua_settable(L, -3);
}

shok_EGL_CGLEEntity* luaext_checkEntity(lua_State* L, int ind)
{
	shok_EGL_CGLEEntity* d = luaext_optEntity(L, ind);
	if (d == nullptr)
		luaL_error(L, "no entity at argument %d", ind);
	return d;
}

shok_EGL_CGLEEntity* luaext_optEntity(lua_State* L, int ind)
{
	int id = 0;
	if (lua_type(L, ind) == LUA_TSTRING) {

	}
	else if (lua_type(L, ind) == LUA_TNUMBER) {
		id = (int)lua_tonumber(L, ind);
	}
	if (id == 0)
		return nullptr;
	return shok_eid2obj(id);
}

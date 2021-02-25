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

void* luaext_checkudata(lua_State* L, int ind)
{
	if (!lua_isuserdata(L, ind))
		luaL_error(L, "no userdata at %d", ind);
	return lua_touserdata(L, ind);
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
		id = shok_getEntityIdByScriptName(lua_tostring(L, ind));
	}
	else if (lua_type(L, ind) == LUA_TNUMBER) {
		id = (int)lua_tonumber(L, ind);
	}
	if (id == 0)
		return nullptr;
	return shok_eid2obj(id);
}

int luaext_optEntityId(lua_State* L, int ind) {
	shok_EGL_CGLEEntity* e = luaext_optEntity(L, ind);
	return e == nullptr ? 0 : e->EntityId;
}

int luaext_checkEntityId(lua_State* L, int ind) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, ind);
	return e == nullptr ? 0 : e->EntityId;
}

shok_GGL_CSettler* luaext_optSettler(lua_State* L, int ind) {
	shok_EGL_CGLEEntity* d = luaext_optEntity(L, ind);
	if (d != nullptr && shok_EntityIsSettler(d))
		return (shok_GGL_CSettler*)d;
	return nullptr;
}

shok_GGL_CSettler* luaext_checkSettler(lua_State* L, int ind) {
	shok_GGL_CSettler* d = luaext_optSettler(L, ind);
	if (d == nullptr)
		luaL_error(L, "no settler at argument %d", ind);
	return d;
}

shok_GGL_CBuilding* luaext_optBuilding(lua_State* L, int ind) {
	shok_EGL_CGLEEntity* d = luaext_optEntity(L, ind);
	if (d != nullptr && shok_EntityIsBuilding(d))
		return (shok_GGL_CBuilding*)d;
	return nullptr;
}

shok_GGL_CBuilding* luaext_checkBulding(lua_State* L, int ind) {
	shok_GGL_CBuilding* d = luaext_optBuilding(L, ind);
	if (d == nullptr)
		luaL_error(L, "no building at argument %d", ind);
	return d;
}

shok_GGL_CResourceDoodad* luaext_optResourceDoodad(lua_State* L, int ind) {
	shok_EGL_CGLEEntity* d = luaext_optEntity(L, ind);
	if (d != nullptr && shok_EntityIsResourceDoodad(d))
		return (shok_GGL_CResourceDoodad*)d;
	return nullptr;
}

shok_GGL_CResourceDoodad* luaext_checkResourceDoodad(lua_State* L, int ind) {
	shok_GGL_CResourceDoodad* d = luaext_optResourceDoodad(L, ind);
	if (d == nullptr)
		luaL_error(L, "no resource entity at argument %d", ind);
	return d;
}

int luaext_assertPointer(lua_State* L, void* p, const char* msg) {
	if (p == nullptr)
		luaL_error(L, msg);
	return 0;
}

int luaext_assert(lua_State* L, bool b, const char* msg) {
	if (!b)
		luaL_error(L, msg);
	return 0;
}

shok_GGlue_CGlueEntityProps* luaext_optEntityType(lua_State* L, int i) {
	if (!lua_isnumber(L, i))
		return nullptr;
	int t = luaL_checkint(L, i);
	return (*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(t);
}

shok_GGlue_CGlueEntityProps* luaext_checkEntityType(lua_State* L, int i) {
	shok_GGlue_CGlueEntityProps* t = luaext_optEntityType(L, i);
	if (t == nullptr)
		luaL_error(L, "no entitytype at %d", i);
	return t;
}

void luaext_pushPos(lua_State* L, shok_position& p) {
	lua_newtable(L);
	lua_pushstring(L, "X");
	lua_pushnumber(L, p.X);
	lua_rawset(L, -3);
	lua_pushstring(L, "Y");
	lua_pushnumber(L, p.Y);
	lua_rawset(L, -3);
}

void luaext_pushPosRot(lua_State* L, shok_positionRot& p) {
	luaext_pushPos(L, p);
	lua_pushstring(L, "r");
	lua_pushnumber(L, p.r);
	lua_rawset(L, -3);
}

void luaext_checkPos(lua_State* L, shok_position& p, int i) {
	lua_pushstring(L, "X");
	lua_gettable(L, i);
	float x = luaL_checkfloat(L, -1);
	lua_pushstring(L, "Y");
	lua_gettable(L, i);
	float y = luaL_checkfloat(L, -1);
	p.X = x;
	p.Y = y;
	lua_pop(L, 2);
}

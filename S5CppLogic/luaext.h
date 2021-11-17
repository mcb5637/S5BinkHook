#pragma once

#include "luaimport.h"
#include "s5data.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define luaL_checkfloat (float)luaL_checknumber
#define luaL_optfloat (float)luaL_optnumber

void luaext_registerFunc(lua_State* L, const char* name, lua_CFunction func);
void* luaext_checkudata(lua_State* L, int ind);

shok_EGL_CGLEEntity* luaext_checkEntity(lua_State* L, int ind);
shok_EGL_CGLEEntity* luaext_optEntity(lua_State* L, int ind);
int luaext_optEntityId(lua_State* L, int ind);
int luaext_checkEntityId(lua_State* L, int ind);
shok_GGL_CSettler* luaext_optSettler(lua_State* L, int ind);
shok_GGL_CSettler* luaext_checkSettler(lua_State* L, int ind);
shok_GGL_CBuilding* luaext_optBuilding(lua_State* L, int ind);
shok_GGL_CBuilding* luaext_checkBulding(lua_State* L, int ind);
shok_GGL_CResourceDoodad* luaext_optResourceDoodad(lua_State* L, int ind);
shok_GGL_CResourceDoodad* luaext_checkResourceDoodad(lua_State* L, int ind);
int luaext_assertPointer(lua_State* L, void* p, const char* msg);
int luaext_assert(lua_State* L, bool b, const char* msg);
shok_GGlue_CGlueEntityProps* luaext_optEntityType(lua_State* L, int i);
shok_GGlue_CGlueEntityProps* luaext_checkEntityType(lua_State* L, int i);
void luaext_pushPos(lua_State* L, const shok_position& p);
void luaext_pushPosRot(lua_State* L, const shok_positionRot& p, bool rad = false);
void luaext_checkPos(lua_State* L, shok_position& p, int i);
void luaext_checkPosRot(lua_State* L, shok_positionRot& p, int i, bool rad = false);
void luaext_writeCostInfo(lua_State* L, const shok_costInfo& c);
void luaext_readCostInfo(lua_State* L, int index, shok_costInfo& c, bool ignoreZeroes);
shok_ResourceType luaext_getresourcetype(lua_State* L, int i);
void luaext_assertEntityAlive(lua_State* L, int id, const char* msg);
int str_ends_with(const char* str, const char* suffix);
void luaext_tolower(lua_State* L);
bool luaext_optbool(lua_State* L, int i, bool b);

template<class T>
T* luaext_GetUserData(lua_State* L, int i) {
	T* t = static_cast<T*>(luaL_checkudata(L, i, typeid(T).name()));
	if (t == nullptr)
		luaL_error(L, "no %s at argument %d", typeid(T).name(), i);
	return t;
}
template<class T>
void luaext_getUserDataMetatable(lua_State* L) {
	if (luaL_newmetatable(L, typeid(T).name())) {
		lua_pushstring(L, "__index");
		lua_newtable(L);

		const luaL_reg* f = T::LuaMethods;
		while (f->func && f->name) {
			luaext_registerFunc(L, f->name, f->func);
			f++;
		}

		lua_rawset(L, -3);
		luaext_registerFunc(L, "__gc", [](lua_State* L) {
			luaext_GetUserData<T>(L, 1)->~T();
			return 0;
			});
	}
}
template<class T>
void luaext_prepareUserDataType(lua_State* L) {
	luaext_getUserDataMetatable<T>(L);
	lua_pop(L, 1);
}
template<class T, class ... Args>
T* luaext_newUserData(lua_State* L, Args&& ... args) {
	T* t = new (lua_newuserdata(L, sizeof(T))) T(std::forward<Args>(args)...);
	luaext_getUserDataMetatable<T>(L);
	lua_setmetatable(L, -2);
	return t;
}

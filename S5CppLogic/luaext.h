#pragma once

#include "luaimport.h"
#include "s5data.h"

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
shok_GGL_CResourceDoodad* luaext_optResourceDoodad(lua_State* L, int ind);
shok_GGL_CResourceDoodad* luaext_checkResourceDoodad(lua_State* L, int ind);
int luaext_assertPointer(lua_State* L, void* p, const char* msg);

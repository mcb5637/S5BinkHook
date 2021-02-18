#pragma once

#include "luaimport.h"
#include "s5data.h"

#define luaL_checkfloat (float)luaL_checknumber
#define luaL_optfloat (float)luaL_optnumber

void luaext_registerFunc(lua_State* L, const char* name, lua_CFunction func);

shok_EGL_CGLEEntity* luaext_checkEntity(lua_State* L, int ind);
shok_EGL_CGLEEntity* luaext_optEntity(lua_State* L, int ind);

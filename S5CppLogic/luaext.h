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
void luaext_pushPos(lua_State* L, shok_position& p);
void luaext_pushPosRot(lua_State* L, shok_positionRot& p);
void luaext_checkPos(lua_State* L, shok_position& p, int i);
void luaext_checkPosRot(lua_State* L, shok_positionRot& p, int i);
void luaext_writeCostInfo(lua_State* L, shok_costInfo& c);
void luaext_readCostInfo(lua_State* L, int index, shok_costInfo& c, bool ignoreZeroes);
void luaext_assertEntityAlive(lua_State* L, int id, const char* msg);
int str_ends_with(const char* str, const char* suffix);

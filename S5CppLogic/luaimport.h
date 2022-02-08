#pragma once

#define LUA_API __declspec(dllimport)

extern "C" {
#include "lua50/lua.h"
#include "lua50/lauxlib.h"
#include "lua50/lualib.h"
}

#define lua_abs_index(L, i) ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : lua_gettop(L) + (i) + 1)

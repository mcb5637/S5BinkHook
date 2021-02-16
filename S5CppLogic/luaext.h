#pragma once

#include "luaimport.h"

void luaext_registerFunc(lua_State* L, const char* name, lua_CFunction func);

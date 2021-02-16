#pragma once

#define LUA_API __declspec(dllimport)

extern "C" {
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
}

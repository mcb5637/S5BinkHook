// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "luaimport.h"
#include "s5data.h"
#include "luaext.h"
#include <string>
#include "l_mem.h"
#include "l_api.h"
#include "l_effect.h"
#include "l_combat.h"
#include "l_entity.h"
#include "l_entitytype.h"
#include "l_logic.h"
#include "l_tech.h"
#include "l_ua.h"

int __cdecl test(lua_State* L) {
    shok_technologyModifier s = shok_technologyModifier();
    int st = (int)&s;
    int test = (int)&s.Operator;
    lua_pushnumber(L, (test - st) / 4);
    return 1;
}

extern "C" void __cdecl install(lua_State * L) {
    lua_pushcfunction(L, &test);
    lua_setglobal(L, "test");

    lua_pushstring(L, "CppLogic");
    lua_newtable(L);

    lua_pushstring(L, "Memory");
    lua_newtable(L);
    l_mem_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "API");
    lua_newtable(L);
    l_api_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "Effect");
    lua_newtable(L);
    l_effect_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "Combat");
    lua_newtable(L);
    l_combat_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "Entity");
    lua_newtable(L);
    l_entity_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "EntityType");
    lua_newtable(L);
    l_entitytype_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "Logic");
    lua_newtable(L);
    l_logic_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "Technology");
    lua_newtable(L);
    l_tech_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "UA");
    lua_newtable(L);
    l_ua_init(L);
    lua_rawset(L, -3);

    lua_rawset(L, LUA_GLOBALSINDEX);
}

// CUtilMemory.GetMemory(tonumber("897558", 16))

extern "C" lua_State * __lua_open() {
    lua_State* o = lua_open();
    install(o);
    return o;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    int *data = (int*) SHOK_Import_LUA_OPEN;
    DWORD vp = 0;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        VirtualProtect((LPVOID)SHOK_SEGMENTSTART, SHOK_SEGMENTLENGTH, PAGE_EXECUTE_READWRITE, &vp);
        *data = (int)&__lua_open;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

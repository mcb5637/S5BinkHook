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

int __cdecl test(lua_State* L) {
    int id = luaL_checkint(L, 1);
    shok_EGL_CGLEEntity* eo = shok_eid2obj(id);
    if (eo == NULL)
        luaL_error(L, "invalid entity id %d", id);
    else
        lua_pushnumber(L, eo->Position.X);
    return 1;
}

extern "C" void __cdecl install(lua_State * L) {
    lua_pushboolean(L, TRUE);
    lua_setglobal(L, "hooked");
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

    lua_rawset(L, LUA_GLOBALSINDEX);
}

// CUtilMemory.GetMemory(tonumber("A0C838", 16))

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

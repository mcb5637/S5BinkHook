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
#include "l_ui.h"
#include <map>
#include <set>
#include <list>

void dumpClassSerialization(lua_State* L, shok_BB_CClassFactory_serializationData* d) {
    if (!d) {
        lua_pushstring(L, "unknown serialization data");
        return;
    }
    lua_newtable(L);
    while (d->Type) {
        lua_pushnumber(L, d->Position);
        lua_newtable(L);

        if (d->SerializationName) {
            lua_pushstring(L, "name");
            lua_pushstring(L, d->SerializationName);
            lua_rawset(L, -3);
        }
        lua_pushstring(L, "size");
        lua_pushnumber(L, d->Size);
        lua_rawset(L, -3);

        if (d->Type == 2) {
            lua_pushstring(L, "DataType");
            if (d->DataConverter == shok_BB_CClassFactory_serializationData_FieldSerilaizer::TypeInt)
                lua_pushstring(L, "Int");
            else if (d->DataConverter == shok_BB_CClassFactory_serializationData_FieldSerilaizer::TypeFloat)
                lua_pushstring(L, "Float");
            else if (d->DataConverter == shok_BB_CClassFactory_serializationData_FieldSerilaizer::TypeBool)
                lua_pushstring(L, "Bool");
            else
                lua_pushnumber(L, reinterpret_cast<int>(d->DataConverter));
            lua_rawset(L, -3);
        }
        else if (d->Type == 3) {
            lua_pushstring(L, "SubElement");
            dumpClassSerialization(L, d->SubElementData);
            lua_rawset(L, -3);
        }

        if (d->ListOptions) {
            lua_pushstring(L, "ListOpions");
            lua_pushnumber(L, d->ListOptions);
            lua_rawset(L, -3);
        }

        lua_rawset(L, -3);
        d++;
    }
}
void dumpClassSerialization(lua_State* L, unsigned int id) {
    shok_BB_CClassFactory_serializationData* d = (*shok_BB_CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
    dumpClassSerialization(L, d);
}

int __cdecl test(lua_State* L) {
    lua_pushnumber(L, (int) luaext_checkEntity(L,1)->GetBehavior<shok_GGL_CMarketBehavior>());
    return 1;
}

int cleanup(lua_State* L) {
    l_entity_cleanup(L);
    l_logic_cleanup(L);
    l_combat_cleanup(L);
    l_effect_cleanup(L);
    l_tech_cleanup(L);
    l_ui_cleanup(L);
    return 0;
}

void initGame() {
    HookTextPrinting();
    l_logic_onload();
    shok_ESnd_CSoEMusic::HookStartMusicFilesystem();
}

constexpr double Version = 1.2003;

int resetCppLogic(lua_State* L) {
    lua_pushstring(L, "CppLogic");
    lua_pushlightuserdata(L, &resetCppLogic);
    lua_rawget(L, LUA_REGISTRYINDEX);
    lua_rawset(L, LUA_GLOBALSINDEX);
    return 0;
}

 void install(lua_State * L) {
     if (!mainmenu_state) {
         mainmenu_state = L;
         initGame();
     }

#ifdef _DEBUG
    lua_pushcfunction(L, &test);
    lua_setglobal(L, "test");
#endif
    
    lua_pushstring(L, "CppLogic");
    lua_newtable(L);

    lua_pushlightuserdata(L, &resetCppLogic);
    lua_pushvalue(L, -2);
    lua_rawset(L, LUA_REGISTRYINDEX);

    luaext_registerFunc(L, "OnLeaveMap", &cleanup);
    lua_pushstring(L, "Version");
    lua_pushnumber(L, Version);
    lua_rawset(L, -3);

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

    lua_pushstring(L, "UI");
    lua_newtable(L);
    l_ui_init(L);
    lua_rawset(L, -3);

    lua_pushstring(L, "UA");
    lua_newtable(L);
    l_ua_init(L);
    lua_rawset(L, -3);

    lua_rawset(L, LUA_GLOBALSINDEX);

    lua_register(L, "CppLogic_ResetGlobal", &resetCppLogic);

    luaopen_debug(L);

    shok_logString("loaded CppLogic %f %s into %X with SCELoader status %i\n", Version,
#ifdef _DEBUG
        "debug",
#else
        "release",
#endif
        reinterpret_cast<int>(L), static_cast<int>(HasSCELoader()));
}

// CUtilMemory.GetMemory(tonumber("897558", 16))

extern "C" lua_State * __lua_open() {
    lua_State* o = lua_open();
    install(o);
    return o;
}
void __lua_close(lua_State* L) { // TODO do something better than this when i come around writing a modloader...
    cleanup(L);
    lua_close(L);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    DWORD vp = 0;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        {
            int *data = reinterpret_cast<int*>(SHOK_Import_LUA_OPEN);
            shok_saveVirtualProtect vp{ data, 4 };
            *data = reinterpret_cast<int>(&__lua_open);
            data = reinterpret_cast<int*>(SHOK_Import_LUA_CLOSE);
            shok_saveVirtualProtect vp2{ data, 4 };
            *data = reinterpret_cast<int>(&__lua_close);
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

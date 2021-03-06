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


const char* SCELoaderFuncOverrides = R"(
function CppLogic.Logic.EnableAllHurtEntityTrigger()
    CEntity.EnableAllHurtTrigger()
end
function CppLogic.Logic.HurtEntityGetDamage()
    return CEntity.TriggerGetDamage()
end
function CppLogic.Logic.HurtEntitySetDamage(d)
    CEntity.TriggerSetDamage(d)
end
function CppLogic.Logic.SetPaydayCallback(func)
    GameCallback_PaydayPayed = func
end
function CppLogic.Entity.SetMaxHP(id, hp)
    CEntity.SetMaxHP(GetID(id), hp)
end
function CppLogic.Entity.SetDamage(id, dmg)
    CEntity.SetDamage(GetID(id), dmg)
end
function CppLogic.Entity.SetArmor(id, armor)
    CEntity.SetArmor(GetID(id), armor)
end
function CppLogic.Entity.SetArmor(id, armor)
    CEntity.SetArmor(GetID(id), armor)
end
function CppLogic.Entity.SetExploration(id, ex)
    CEntity.SetExploration(GetID(id), ex)
end
function CppLogic.Entity.Leader.SetRegeneration(id, hp, seconds)
    if hp then
        CEntity.SetHealingPoints(GetID(id), hp)
    end
    if seconds then
        CEntity.SetHealingSeconds(GetID(id), seconds)
    end
end
function CppLogic.Entity.SetAutoAttackMaxRange(id, ran)
    CEntity.SetAttackRange(GetID(id), ran)
end
function CppLogic.Logic.SetStringTableText(key, text)
    CUtil.SetStringTableText(key, text)
end
function CppLogic.Entity.SetDisplayName(id, n)
    CUtil.SetEntityDisplayName(GetID(id), n)
end
function CppLogic.Logic.SetPlaceBuildingCB(func)
    GameCallback_PlaceBuildingAdditionalCheck = function(ety, x, y, r, isbuildon)
        if not func then
            return true
        end
        return func(ety, -1, {X=x,Y=y}, r, isbuildon)
    end
end
function CppLogic.Logic.GetPlaceBuildingRotation()
    return CPlaceBuilding.GetRotation()
end
function CppLogic.Logic.SetPlaceBuildingRotation(r)
    return CPlaceBuilding.SetRotation(r)
end
)";

struct x {
    const char* name;
    int i, id;
};

int __cdecl test(lua_State* L) {
    /*shok_GS3DTools_CMapData s = shok_GS3DTools_CMapData();
    int st = (int)&s;
    int test = (int)&s.MapType;
    lua_pushnumber(L, (test - st) / 4);*/
    /*if (lua_gettop(L) == 0) {
        lua_pushnumber(L, (int)L);
        return 1;
    }
    if (lua_isnumber(L, 1)) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, luaL_checkint(L, 1));

    }*/
    lua_pushnumber(L, 1);
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
    shok_taskData::OnGameInit();
}

constexpr double Version = 1.0;

extern "C" void __cdecl install(lua_State * L) {
#ifdef _DEBUG
    lua_pushcfunction(L, &test);
    lua_setglobal(L, "test");
#endif
    
    lua_pushstring(L, "CppLogic");
    lua_newtable(L);
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

    luaopen_debug(L);

    if (!mainmenu_state) {
        mainmenu_state = L;
        initGame();
    }
    
    if (HasSCELoader()) {
        lua_dobuffer(L, SCELoaderFuncOverrides, strlen(SCELoaderFuncOverrides), "CppLogic");
    }

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
void __lua_close(lua_State* L) {
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

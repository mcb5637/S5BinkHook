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
#include <iostream>
#include <fstream>
#include <string_view>

struct CppLogicOptions {
    bool DoNotLoad = false;
    bool DoNotUseCenterFix = false;

    void LoadFromFile(const char* name) {
        std::ifstream f{ name, std::ios::in };
        if (f.is_open()) {
            std::string line;
            while (std::getline(f >> std::ws, line, '=')) {
                if (line == "DoNotLoad") {
                    f >> DoNotLoad;
                }
                else if (line == "DoNotUseCenterFix") {
                    f >> DoNotUseCenterFix;
                }
                f.ignore();
            }
            f.close();
        }
    }
};
CppLogicOptions Options{};

void dumpClassSerialization(lua_State* L, const BB::SerializationData* d) {
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
            if (d->DataConverter == BB::FieldSerilaizer::TypeInt)
                lua_pushstring(L, "Int");
            else if (d->DataConverter == BB::FieldSerilaizer::TypeFloat)
                lua_pushstring(L, "Float");
            else if (d->DataConverter == BB::FieldSerilaizer::TypeBool)
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
            lua_pushnumber(L, reinterpret_cast<int>(d->ListOptions));
            lua_rawset(L, -3);
        }

        lua_rawset(L, -3);
        d++;
    }
}
void dumpClassSerialization(lua_State* L, unsigned int id) {
    const BB::SerializationData* d = (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
    dumpClassSerialization(L, d);
}

#define typeandoffset(T,M) sizeof(decltype(T::M)), offsetof(T, M)

struct testdata {
    int x, y;
};

int test(lua::State L) {
    BB::SerializationData s = BB::SerializationData::ValueData("y", typeandoffset(testdata, x), BB::FieldSerilaizer::GetSerilalizer<int>());
    L.Push(static_cast<int>(s.Size));
    L.Push(static_cast<int>(s.Position));
    return 2;
}

int cleanup(lua_State* L) {
    lua::State L2 = lua::State{ L };
    CppLogic::Effect::Cleanup(L2);
    CppLogic::Combat::Cleanup(L2);
    CppLogic::Entity::Cleanup(L2);

    l_logic_cleanup(L);
    l_tech_cleanup(L);
    l_ui_cleanup(L);
    return 0;
}

void InitGame() {
    if (!Options.DoNotUseCenterFix)
        shok::HookTextPrinting();
    l_logic_onload();
    ESnd::CSoEMusic::HookStartMusicFilesystem();
}

constexpr double Version = 1.3002;

int ResetCppLogic(lua::State L) {
    L.Push("CppLogic");
    L.PushLightUserdata(&ResetCppLogic);
    L.GetTableRaw(L.REGISTRYINDEX);
    L.SetTableRaw(L.GLOBALSINDEX);
    return 0;
}

 void install(lua_State * L) {
     lua::State L2 = lua::State{ L };
     if (!shok::LuaStateMainmenu) {
         shok::LuaStateMainmenu = L;
         InitGame();
     }

#ifdef _DEBUG
    /*lua_pushcfunction(L, &test);
    lua_setglobal(L, "test");*/
    L2.RegisterFunc<test>("test");
#endif
    
    L2.Push("CppLogic");
    L2.NewTable();

    L2.PushLightUserdata(&ResetCppLogic);
    L2.PushValue(-2);
    L2.SetTableRaw(L2.REGISTRYINDEX);

    L2.RegisterFunc<cleanup>("OnLeaveMap", -3);
    L2.Push("Version");
    L2.Push(Version);
    L2.SetTableRaw(-3);

    L2.Push("Memory");
    L2.NewTable();
    CppLogic::Mem::Init(L2);
    L2.SetTableRaw(-3);

    L2.Push("API");
    L2.NewTable();
    CppLogic::API::Init(L2);
    L2.SetTableRaw(-3);

    L2.Push("Effect");
    L2.NewTable();
    CppLogic::Effect::Init(L2);
    L2.SetTableRaw(-3);

    L2.Push("Combat");
    L2.NewTable();
    CppLogic::Combat::Init(L2);
    L2.SetTableRaw(-3);

    L2.Push("Entity");
    L2.NewTable();
    CppLogic::Entity::Init(L2);
    L2.SetTableRaw(-3);

    L2.Push("EntityType");
    L2.NewTable();
    l_entitytype_init(L);
    L2.SetTableRaw(-3);

    L2.Push("Logic");
    L2.NewTable();
    l_logic_init(L);
    L2.SetTableRaw(-3);

    L2.Push("Technology");
    L2.NewTable();
    l_tech_init(L);
    L2.SetTableRaw(-3);

    L2.Push("UI");
    L2.NewTable();
    l_ui_init(L);
    L2.SetTableRaw(-3);

    L2.Push("UA");
    L2.NewTable();
    l_ua_init(L);
    L2.SetTableRaw(-3);

    L2.SetTableRaw(L2.GLOBALSINDEX);

    L2.RegisterFunc<ResetCppLogic>("CppLogic_ResetGlobal");

    luaopen_debug(L);

    shok::LogString("loaded CppLogic %f %s into %X with SCELoader status %i\n", Version,
#ifdef _DEBUG
        "debug",
#else
        "release",
#endif
        reinterpret_cast<int>(L), static_cast<int>(CppLogic::HasSCELoader()));
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
    EGL::CGLETerrainLowRes::ClearBridgeArea();
}

struct DebuggerOrig {
    bool loaded = false;
    HMODULE debugger = nullptr;
    void(__stdcall* AddLuaState)(lua_State* L) = nullptr;
    void(__stdcall* RemoveLuaState)(lua_State* L) = nullptr;
    void(__stdcall* NewFile)(lua_State* L, const char* filename, const char* filedata, size_t len) = nullptr;
    void(__stdcall* Show)() = nullptr;
    void(__stdcall* Hide)() = nullptr;
    void(__stdcall* Break)(lua_State* L) = nullptr;
    void(__stdcall* ShowExecuteLine)() = nullptr;


    void Load() {
        if (loaded)
            return;
        loaded = true;
        debugger = LoadLibrary("LuaDebuggerOrig.dll");
        if (debugger) {
            AddLuaState = reinterpret_cast<void(__stdcall*)(lua_State*)>(GetProcAddress(debugger, "_AddLuaState@4"));
            RemoveLuaState = reinterpret_cast<void(__stdcall*)(lua_State*)>(GetProcAddress(debugger, "_RemoveLuaState@4"));
            NewFile = reinterpret_cast<void(__stdcall*)(lua_State*, const char*, const char*, size_t)>(GetProcAddress(debugger, "_NewFile@16"));
            Show = reinterpret_cast<void(__stdcall*)()>(GetProcAddress(debugger, "_Show@0"));
            Hide = reinterpret_cast<void(__stdcall*)()>(GetProcAddress(debugger, "_Hide@0"));
            Break = reinterpret_cast<void(__stdcall*)(lua_State*)>(GetProcAddress(debugger, "_Break@4"));
            ShowExecuteLine = reinterpret_cast<void(__stdcall*)()>(GetProcAddress(debugger, "_ShowExecuteLine@0"));
        }
    }
};

DebuggerOrig dbg{};

extern "C" {
    void __declspec(dllexport) __stdcall AddLuaState(lua_State* L) {
        install(L);
        dbg.Load();
        if (dbg.AddLuaState)
            dbg.AddLuaState(L);
    }

    void __declspec(dllexport) __stdcall RemoveLuaState(lua_State* L) {
        cleanup(L);
        EGL::CGLETerrainLowRes::ClearBridgeArea();
        if (dbg.RemoveLuaState)
            dbg.RemoveLuaState(L);
    }

    void __declspec(dllexport) __stdcall NewFile(lua_State* L, const char* filename, const char* filedata, size_t len) {
        if (dbg.NewFile)
            dbg.NewFile(L, filename, filedata, len);
    }

    void __declspec(dllexport) __stdcall Show() {
        if (dbg.Show)
            dbg.Show();
    }

    void __declspec(dllexport) __stdcall Hide() {
        if (dbg.Hide)
            dbg.Hide();
    }

    void __declspec(dllexport) __stdcall Break(lua_State* L) {
        if (dbg.Break)
            dbg.Break(L);
    }

    void __declspec(dllexport) __stdcall ShowExecuteLine() {
        if (dbg.ShowExecuteLine)
            dbg.ShowExecuteLine();
    }
}

bool __declspec(dllexport) __stdcall HasRealDebugger() {
    if (dbg.debugger)
        return true;
    return false;
}

double __declspec(dllexport) __stdcall GetCppLogicVersion() {
    return Version;
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
        /*{
            Options.LoadFromFile("./CppLogicOptions.txt");
            if (Options.DoNotLoad)
                return false;
        }
    
        {
            int *data = reinterpret_cast<int*>(SHOK_Import_LUA_OPEN);
            shok::SaveVirtualProtect vp{ data, 4 };
            *data = reinterpret_cast<int>(&__lua_open);
            data = reinterpret_cast<int*>(SHOK_Import_LUA_CLOSE);
            shok::SaveVirtualProtect vp2{ data, 4 };
            *data = reinterpret_cast<int>(&__lua_close);
        }*/
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return true;
}

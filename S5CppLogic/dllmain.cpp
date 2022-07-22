// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include <map>
#include <set>
#include <list>
#include <iostream>
#include <fstream>
#include <string_view>
#include "Luapp/luapp50.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_classfactory.h"
#include "s5_entity.h"
#include "s5_scriptsystem.h"
#include "s5_sound.h"
#include "s5_ui.h"
#include "s5_maplogic.h"
#include "s5_filesystem.h"
#include "s5_exception.h"
#include "s5_widget.h"
#include "s5_framework.h"
#include "s5_tasklist.h"
#include "s5_mapdisplay.h"
#include "s5_idmanager.h"
#include "s5_effects.h"
#include "s5_staticlist.h"
#include "modloader.h"
#include "entityiterator.h"
#include "hooks.h"
#include "luaext.h"
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
#include "luaserializer.h"

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

int Test(lua::State Ls) {
	luaext::EState L{ Ls };
	//CppLogic::Serializer::LuaSerializer::Serialize(Ls, L.CheckEntity(1));
	//CppLogic::Serializer::LuaSerializer::DumpClassSerializationData(Ls, reinterpret_cast<const BB::SerializationData*>(0xA0DE88));
	//CppLogic::Serializer::LuaSerializer::DumpClassSerializationData(Ls, 0x6987C1B3);
	int id = L.CheckInt(1);
	L.Push((int)(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(id)->SelectionTexture);
	return 1;
}

int Cleanup(lua::State L) {
	CppLogic::Effect::Cleanup(L);
	CppLogic::Combat::Cleanup(L);
	CppLogic::Entity::Cleanup(L);
	CppLogic::Logic::Cleanup(L);
	CppLogic::Tech::Cleanup(L);
	CppLogic::UI::Cleanup(L);
	return 0;
}

void OnFrameworkChangeMode(Framework::CMain::NextMode n) {
	if (!CppLogic::HasSCELoader())
		CppLogic::ModLoader::ModLoader::Cleanup(n);
	if ((*Framework::CMain::GlobalObj)->CurrentMode != Framework::CMain::Mode::MainMenu) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		Cleanup(L);
	}
}

void InitGame() {
	Framework::CMain::HookModeChange();
	Framework::CMain::OnModeChange = &OnFrameworkChangeMode;
	if (!CppLogic::HasSCELoader())
		CppLogic::ModLoader::ModLoader::Initialize();
	if (!Options.DoNotUseCenterFix)
		shok::HookTextPrinting();
	CppLogic::Logic::OnLoad();
	ESnd::CSoEMusic::HookStartMusicFilesystem();
	EScr::CScriptTriggerSystem::HookRemoveFuncOverrides();
}

constexpr double Version = 1.4000;

int ResetCppLogic(lua::State L) {
	L.Push("CppLogic");
	L.PushLightUserdata(&ResetCppLogic);
	L.GetTableRaw(L.REGISTRYINDEX);
	L.SetTableRaw(L.GLOBALSINDEX);
	return 0;
}

int Nop(lua::State L) {
	return 0;
}

void Install(lua::State L) {
	if (!shok::LuaStateMainmenu) {
		shok::LuaStateMainmenu = L.GetState();
		InitGame();
	}

#ifdef _DEBUG
	L.RegisterFunc<Test>("test");
#endif

	L.Push("CppLogic");
	L.NewTable();

	L.PushLightUserdata(&ResetCppLogic);
	L.PushValue(-2);
	L.SetTableRaw(L.REGISTRYINDEX);

	L.RegisterFunc<Nop>("OnLeaveMap", -3);
	L.Push("Version");
	L.Push(Version);
	L.SetTableRaw(-3);

	L.Push("Memory");
	L.NewTable();
	CppLogic::Mem::Init(L);
	L.SetTableRaw(-3);

	L.Push("API");
	L.NewTable();
	CppLogic::API::Init(L);
	L.SetTableRaw(-3);

	L.Push("Effect");
	L.NewTable();
	CppLogic::Effect::Init(L);
	L.SetTableRaw(-3);

	L.Push("Combat");
	L.NewTable();
	CppLogic::Combat::Init(L);
	L.SetTableRaw(-3);

	L.Push("Entity");
	L.NewTable();
	CppLogic::Entity::Init(L);
	L.SetTableRaw(-3);

	L.Push("EntityType");
	L.NewTable();
	CppLogic::EntityType::Init(L);
	L.SetTableRaw(-3);

	L.Push("Logic");
	L.NewTable();
	CppLogic::Logic::Init(L);
	L.SetTableRaw(-3);

	L.Push("Technology");
	L.NewTable();
	CppLogic::Tech::Init(L);
	L.SetTableRaw(-3);

	L.Push("UI");
	L.NewTable();
	CppLogic::UI::Init(L);
	L.SetTableRaw(-3);

	L.Push("UA");
	L.NewTable();
	CppLogic::UA::Init(L);
	L.SetTableRaw(-3);

	L.SetTableRaw(L.GLOBALSINDEX);

	L.RegisterFunc<ResetCppLogic>("CppLogic_ResetGlobal");

	//luaopen_debug(L);

	shok::LogString("loaded CppLogic %f %s into %X with SCELoader status %i\n", Version,
#ifdef _DEBUG
		"debug",
#else
		"release",
#endif
		reinterpret_cast<int>(L.GetState()), static_cast<int>(CppLogic::HasSCELoader()));
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
		Install(lua::State{ L });
		dbg.Load();
		if (dbg.AddLuaState)
			dbg.AddLuaState(L);
	}

	void __declspec(dllexport) __stdcall RemoveLuaState(lua_State* L) {
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




BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	DWORD vp = 0;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}

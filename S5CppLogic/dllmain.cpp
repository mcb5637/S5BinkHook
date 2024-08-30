// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include <map>
#include <set>
#include <deque>
#include <list>
#include <iostream>
#include <fstream>
#include <string_view>
#include "luaext.h"
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
#include "s5_entitydisplay.h"
#include "s5_events.h"
#include "s5_behaviorProps.h"
#include "s5_RWE_2d.h"
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
#include "savegame_extra.h"
#include "EntityAddonData.h"
#include "ModBehavior.h"
#include "ModUI.h"
#include "ModConfig.h"
#include "EnumIdManagerMagic.h"

struct CppLogicOptions {
	bool DoNotLoad = false;
	bool DisableAdvStringPrinting = false;
	bool DisableModLoader = false;
	bool DisableAdvLuaSerializer = false;
	bool DisableMapListFix = false;

	bool Loaded = false;

	void LoadFromFile(const char* name) {
		if (Loaded)
			return;
		Loaded = true;
		std::ifstream f{ name, std::ios::in };
		if (f.is_open()) {
			std::string line;
			while (std::getline(f >> std::ws, line, '=')) {
				if (line == "DoNotLoad") {
					f >> DoNotLoad;
				}
				else if (line == "DisableAdvStringPrinting") {
					f >> DisableAdvStringPrinting;
				}
				else if (line == "DisableModLoader") {
					f >> DisableModLoader;
				}
				else if (line == "DisableAdvLuaSerializer") {
					f >> DisableAdvLuaSerializer;
				}
				else if (line == "DisableMapListFix") {
					f >> DisableMapListFix;
				}
				f.ignore();
			}
			f.close();
		}
	}
};
CppLogicOptions Options{};

class BreakOnCmdBehavior : public EGL::CGLEBehavior {
	static constexpr bool VsDbg = true;
	static constexpr bool Int3Dbg = false;
	// note no serf, heroability, building cmds

	void EventMove(EGL::CEventPosition* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventAttack(EGL::CEvent1Entity* ev) {
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventAMove(EGL::CEventPosition* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventHoldPos(BB::CEvent* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventDefend(BB::CEvent* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventGuard(EGL::CEvent1Entity* ev) {
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventPatrol(EGL::CEventPosition* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}

	void EventStealGoods(EGL::CEvent1Entity* ev) {
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventSecureGoods(EGL::CEvent1Entity* ev) {
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventSabotage(EGL::CEvent1Entity* ev) {
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventDisarm(EGL::CEvent1Entity* ev) {
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventBinoc(EGL::CEventPosition* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventTorch(EGL::CEventPosition* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}
	void EventPointRes(BB::CEvent* ev) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		if constexpr (VsDbg)
			__debugbreak();
		else if constexpr (Int3Dbg)
			DEBUGGER_BREAK;
		auto s = L.GenerateStackTrace();
		auto _ = s.c_str();
	}


protected:
	virtual void AddHandlers(shok::EntityId id) override {
		EntityId = id;
		auto* e = EGL::CGLEEntity::GetEntityByID(id);
		e->CreateEventHandler<shok::EventIDs::MoveCommand_Move>(this, &BreakOnCmdBehavior::EventMove);
		e->CreateEventHandler<shok::EventIDs::Leader_AttackEntity>(this, &BreakOnCmdBehavior::EventAttack);
		e->CreateEventHandler<shok::EventIDs::Leader_AttackMove>(this, &BreakOnCmdBehavior::EventAMove);
		e->CreateEventHandler<shok::EventIDs::Leader_Defend>(this, &BreakOnCmdBehavior::EventDefend);
		e->CreateEventHandler<shok::EventIDs::Leader_HoldPosition>(this, &BreakOnCmdBehavior::EventHoldPos);
		e->CreateEventHandler<shok::EventIDs::Leader_Guard>(this, &BreakOnCmdBehavior::EventGuard);
		e->CreateEventHandler<shok::EventIDs::LeaderPatrol>(this, &BreakOnCmdBehavior::EventPatrol);
		e->CreateEventHandler<shok::EventIDs::LeaderPatrolAddPoint>(this, &BreakOnCmdBehavior::EventPatrol);

		e->CreateEventHandler<shok::EventIDs::Thief_StealFromCommand>(this, &BreakOnCmdBehavior::EventStealGoods);
		e->CreateEventHandler<shok::EventIDs::Thief_SecureGoodsCommand>(this, &BreakOnCmdBehavior::EventSecureGoods);
		e->CreateEventHandler<shok::EventIDs::KegPlacer_SabotageCommand>(this, &BreakOnCmdBehavior::EventSabotage);
		e->CreateEventHandler<shok::EventIDs::KegPlacer_DefuseCommand>(this, &BreakOnCmdBehavior::EventDisarm);
		e->CreateEventHandler<shok::EventIDs::Binocular_ExploreCommand>(this, &BreakOnCmdBehavior::EventBinoc);
		e->CreateEventHandler<shok::EventIDs::PointToResources_Activate>(this, &BreakOnCmdBehavior::EventPointRes);
	}
	virtual void OnEntityCreate(EGL::CGLEBehaviorProps* p) override {

	}
	virtual void OnEntityLoad(EGL::CGLEBehaviorProps* p) override {

	}
public:

	void* operator new(size_t s)
	{
		return shok::Malloc(s);
	}
	void operator delete(void* p)
	{
		shok::Free(p);
	}
	static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1006);
	virtual shok::ClassId __stdcall GetClassIdentifier() const {
		return Identifier;
	}
};

std::string BBExceptionConverter(std::exception_ptr ex, const char* funcsig)
{
	try {
		std::rethrow_exception(ex);
	}
	catch (const BB::CException& e) {
		char buff[200]{};
		e.CopyMessage(buff, sizeof(buff));
		return std::format("{}: {} in {}", typeid(e).name(), buff, funcsig);
	}
	catch (const exception& e) {
		return std::format("{}: {} in {}", typeid(e).name(), e.what(), funcsig);
	}
}

int Test(lua::State Ls) {
	luaext::EState L{ Ls };
	//CppLogic::Serializer::ObjectToLuaSerializer::Serialize(Ls, L.CheckEntity(1));
	//CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(Ls, reinterpret_cast<const BB::SerializationData*>(0xA0D228));
	//CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(Ls, reinterpret_cast<const BB::SerializationData*(__stdcall*)()>(0x54768A)());
	//CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(Ls, static_cast<shok::ClassId>(0x8BCFE1F7));
	//CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(Ls, GGL::CGLSettlerProps::Identifier);
	/*auto e = L.CheckEntity(1);
	auto cf = *BB::CClassFactory::GlobalObj;
	if (cf->GetClassDemangledName(BreakOnCmdBehavior::Identifier) == nullptr) {
		cf->AddClassToFactory<BreakOnCmdBehavior>();
	}
	e->AddBehavior(cf->CreateObject<BreakOnCmdBehavior>());*/

	return 0;
}

int GetOptions(lua::State L) {
	L.Push(Options.DisableAdvStringPrinting);
	L.Push(Options.DisableAdvLuaSerializer);
	L.Push(Options.DisableModLoader);
	L.Push(CppLogic::HasSCELoader());
	return 4;
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
	if (!CppLogic::HasSCELoader() && !Options.DisableModLoader)
		CppLogic::ModLoader::ModLoader::Cleanup(n);
	if ((*Framework::CMain::GlobalObj)->CurrentMode != Framework::CMain::Mode::MainMenu) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
		Cleanup(L);
	}
}

void OnSaveLoaded() {
	auto* s = Framework::SavegameSystem::GlobalObj()->CurrentSave;
	CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.DeserializeFrom(s->SavePath.c_str(), s->AdditionalInfo.c_str());
	lua::State L{ *EScr::CScriptTriggerSystem::GameState };
	int t = L.GetTop();
	CppLogic::Effect::OnSaveLoaded(L);
	CppLogic::Combat::OnSaveLoaded(L);
	CppLogic::Entity::OnSaveLoaded(L);
	CppLogic::Logic::OnSaveLoaded(L);
	CppLogic::UI::OnSaveLoaded(L);
	L.SetTop(t);
}

void OnSaveDone(const char* path, const char* savename) {
	CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.SerializeTo(path, savename);
}

void InitGame() {
	lua::v50::ExceptionConverter = &BBExceptionConverter;
	Framework::CMain::HookModeChange();
	Framework::CMain::OnModeChange = &OnFrameworkChangeMode;
	Framework::CMain::OnSaveLoaded = &OnSaveLoaded;
	Framework::SavegameSystem::HookSaveGame();
	Framework::SavegameSystem::OnGameSavedTo = &OnSaveDone;
	BB::CBBArchiveFile::HookFixDoubleFree();
	if (!CppLogic::HasSCELoader() && !Options.DisableModLoader)
		CppLogic::ModLoader::ModLoader::Initialize();
	if (!Options.DisableAdvStringPrinting)
		shok::HookTextPrinting();
	CppLogic::Logic::OnLoad();
	ESnd::CSoEMusic::HookStartMusicFilesystem();
	EScr::CScriptTriggerSystem::HookRemoveFuncOverrides();
	EGL::CFlyingEffect::HookOnLoadFix();
	if (!Options.DisableAdvLuaSerializer)
		EScr::LuaStateSerializer::HookSerializationOverride();
	EScr::CScriptTriggerSystem::HookFireEvent();
	CppLogic::EntityAddon::EntityAddonData::Init();
	CppLogic::Mod::RegisterClasses();
	CppLogic::Mod::UI::RegisterClasses();
	CppLogic::Mod::Config::RegisterClasses();
	EGUIX::CTextButtonWidget::HookFixTextRender();
	if (!Options.DisableMapListFix)
		Framework::CampagnInfo::HookLoad();
}

double Version = 0.0;
std::string VersionString{ "failed loading version info" };

int ResetCppLogic(lua::State L) {
	L.Push("CppLogic");
	L.PushLightUserdata(&ResetCppLogic);
	L.GetTableRaw(L.REGISTRYINDEX);
	L.SetGlobal();
	return 0;
}

int Nop(lua::State L) {
	return 0;
}

void Install(lua::State L) {
	if (!shok::LuaStateMainmenu) {
		shok::LuaStateMainmenu = L.GetState();
		InitGame();
		BB::CFileSystemMgr::LoadorderTop = (*BB::CFileSystemMgr::GlobalObj)->LoadOrder[0];
	}

	CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.Clear();

	static constexpr const char* CppLogic = "CppLogic";

	EScr::LuaStateSerializer::AddGlobalToNotSerialize(CppLogic);

#ifdef _DEBUG
	L.RegisterFunc<Test>("test");
#endif

	L.Push(CppLogic);
	L.NewTable();

	L.RegisterFunc<GetOptions>("GetOptions", -3);

	L.PushLightUserdata(&ResetCppLogic);
	L.PushValue(-2);
	L.SetTableRaw(L.REGISTRYINDEX);

	L.RegisterFunc<Nop>("OnLeaveMap", -3);
	L.Push("Version");
	L.Push(Version);
	L.SetTableRaw(-3);

	L.Push("VersionString");
	L.Push(VersionString);
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

	if (L.GetState() == shok::LuaStateMainmenu) {
		CppLogic::ModLoader::ModLoader::InitMainmenu(L);
	}

	L.SetGlobal();

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
		Options.LoadFromFile(".\\bin\\CppLogicOptions.txt");
		if (!Options.DoNotLoad)
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

#pragma comment(lib, "version")
void LoadVersionInfo(HMODULE m) {
	char path[MAX_PATH + 1]{};
	if (GetModuleFileName(m, path, MAX_PATH) == 0)
		return;
	DWORD handle;
	size_t s = GetFileVersionInfoSize(path, &handle);
	if (s == 0)
		return;
	std::vector<char> data{};
	data.resize(s);
	if (!GetFileVersionInfo(path, 0, data.size(), data.data()))
		return;
	void* vinfo = nullptr;
	size_t vs = 0;
	if (!VerQueryValue(data.data(), "\\", &vinfo, &vs))
		return;
	if (vs == 0)
		return;
	auto* vi = static_cast<VS_FIXEDFILEINFO*>(vinfo);
	if (vi->dwSignature != 0xFEEF04BD)
		return;
	int major = HIWORD(vi->dwFileVersionMS);
	int minor = LOWORD(vi->dwFileVersionMS);
	int patch = HIWORD(vi->dwFileVersionLS);
	int rev = LOWORD(vi->dwFileVersionLS);
	VersionString = std::format("{}.{}.{}.{}", major, minor, patch, rev);
	auto v = std::format("{}.{:02}", major, minor);
	std::from_chars(v.data(), v.data() + v.size(), Version);
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
		LoadVersionInfo(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}

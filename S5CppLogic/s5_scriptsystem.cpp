#include "pch.h"
#include "s5_scriptsystem.h"
#include "hooks.h"

lua_State* shok::LuaStateMainmenu = nullptr;

void EScr::CScriptTriggerSystem::RunTrigger(BB::CEvent* ev)
{
	PostEvent(ev);
}

void __stdcall overrideluafunc_empty(lua_State* L, const char* name, lua::CFunction f) {

}
bool HookRemoveFuncOverrides_Hooked = false;
void EScr::CScriptTriggerSystem::HookRemoveFuncOverrides()
{
	if (HookRemoveFuncOverrides_Hooked)
		return;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x59BFD6), 0x59C012 - 0x59BFD6 + 10 };
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x59BFD6), &overrideluafunc_empty); // error
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x59C012), &overrideluafunc_empty); // pcall
}

lua::Reference __stdcall EScr::CLuaFuncRefCommand::GetRefToFunc()
{
	throw 0;
}

static inline void(__stdcall* const funccmd_set)(EScr::CLuaFuncRefCommand* th, const char* c) = reinterpret_cast<void(__stdcall* const)(EScr::CLuaFuncRefCommand*, const char*)>(0x5A18EB);
void EScr::CLuaFuncRefCommand::SetCommandString(const char* c)
{
	funccmd_set(this, c);
}

static inline void(__stdcall* const funccmd_clear)(EScr::CLuaFuncRefCommand* th) = reinterpret_cast<void(__stdcall*)(EScr::CLuaFuncRefCommand*)>(0x5A15E5);
void EScr::CLuaFuncRefCommand::Clear()
{
	funccmd_clear(this);
}

static inline void(__stdcall* const funccmd_createref)(EScr::CLuaFuncRefCommand* th) = reinterpret_cast<void(__stdcall*)(EScr::CLuaFuncRefCommand*)>(0x5A1665);
void EScr::CLuaFuncRefCommand::CreateRef()
{
	funccmd_createref(this);
}

static inline bool(__stdcall* const funccmd_checkref)(EScr::CLuaFuncRefCommand* th) = reinterpret_cast<bool(__stdcall*)(EScr::CLuaFuncRefCommand*)>(0x51B247);
bool EScr::CLuaFuncRefCommand::CheckRef()
{
	return funccmd_checkref(this);
}

static inline bool(__stdcall* const funccmd_call)(EScr::CLuaFuncRefCommand* th, int a, int r) = reinterpret_cast<bool(__stdcall*)(EScr::CLuaFuncRefCommand*, int, int)>(0x5A16BD);
bool EScr::CLuaFuncRefCommand::Call(int nargs, int nres)
{
	return funccmd_call(this, nargs, nres);
}

void EScr::CLuaFuncRefCommand::ReplaceFunc(lua::State L, int idx)
{
	CheckRef();
	Clear();
	this->L = L;
	L.CheckType(idx, lua::LType::Function);
	L.PushValue(idx);
	Ref = L.Ref(L.REGISTRYINDEX);
	NeedsCompile = false;
}

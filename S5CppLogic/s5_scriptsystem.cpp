#include "pch.h"
#include "s5_scriptsystem.h"
#include "hooks.h"

lua_State* shok::LuaStateMainmenu = nullptr;

void EScr::CScriptTriggerSystem::RunTrigger(BB::CEvent* ev)
{
	PostEvent(ev);
}

static inline void(__thiscall* const scripttriggersys_create)(EScr::CScriptTriggerSystem* th, int* id, EScr::CScriptTrigger* t) = reinterpret_cast<void(__thiscall*)(EScr::CScriptTriggerSystem*, int*, EScr::CScriptTrigger*)>(0x5A0FDA);
int EScr::CScriptTriggerSystem::CreateTrigger(CScriptTrigger* trigger)
{
	int id = 0;
	scripttriggersys_create(this, &id, trigger);
	return id;
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

static inline void(__stdcall* const funcref_clear)(EScr::CLuaFuncRef* th) = reinterpret_cast<void(__stdcall*)(EScr::CLuaFuncRef*)>(0x5A15E5);
void EScr::CLuaFuncRef::Clear()
{
	funcref_clear(this);
}

static inline void(__stdcall* const funcref_createref)(EScr::CLuaFuncRef* th) = reinterpret_cast<void(__stdcall*)(EScr::CLuaFuncRef*)>(0x5A1665);
void EScr::CLuaFuncRef::CreateRef()
{
	funcref_createref(this);
}

static inline bool(__stdcall* const funcref_checkref)(EScr::CLuaFuncRef* th) = reinterpret_cast<bool(__stdcall*)(EScr::CLuaFuncRef*)>(0x51B247);
bool EScr::CLuaFuncRef::CheckRef()
{
	return funcref_checkref(this);
}

static inline bool(__stdcall* const funcref_call)(EScr::CLuaFuncRef* th, int a, int r) = reinterpret_cast<bool(__stdcall*)(EScr::CLuaFuncRef*, int, int)>(0x5A16BD);
bool EScr::CLuaFuncRef::Call(int nargs, int nres)
{
	return funcref_call(this, nargs, nres);
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

lua::Reference __stdcall EScr::CLuaFuncRefGlobal::GetRefToFunc()
{
	throw 0;
}

static inline void(__stdcall* const funcglo_set)(EScr::CLuaFuncRefGlobal* th, const char* c) = reinterpret_cast<void(__stdcall* const)(EScr::CLuaFuncRefGlobal*, const char*)>(0x5A18EB);
void EScr::CLuaFuncRefGlobal::SetCommandString(const char* c)
{
	funcglo_set(this, c);
}

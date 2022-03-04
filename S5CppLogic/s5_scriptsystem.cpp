#include "pch.h"
#include "s5data.h"

void EScr::CScriptTriggerSystem::RunTrigger(BB::CEvent* ev)
{
	PostEvent(ev);
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

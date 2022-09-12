#include "pch.h"
#include "s5_scriptsystem.h"
#include <format>
#include "s5_exception.h"
#include "hooks.h"
#include "luaserializer.h"

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

static inline void(__thiscall* const scripttriggersys_pushtoactive)(EScr::CScriptTriggerSystem* th, shok::EventIDs id, EScr::CScriptTrigger* t) = reinterpret_cast<void(__thiscall*)(EScr::CScriptTriggerSystem*, shok::EventIDs, EScr::CScriptTrigger*)>(0x5A0C7D);
void EScr::CScriptTriggerSystem::EnableTrigger(unsigned int id)
{
	auto i = Trigger.find(id);
	if (i == Trigger.end())
		throw std::logic_error{ "invalid trigger" };
	CScriptTrigger* t = i->second;
	if (t->ActionFunc.CheckRef()) {
		scripttriggersys_pushtoactive(this, t->EventType, t);
	}
	t->Switch = true;
}

static inline void(__thiscall* const scripttriggersys_removefromactive)(EScr::CScriptTriggerSystem* th, shok::EventIDs id, EScr::CScriptTrigger* t) = reinterpret_cast<void(__thiscall*)(EScr::CScriptTriggerSystem*, shok::EventIDs, EScr::CScriptTrigger*)>(0x59F3F4);
void EScr::CScriptTriggerSystem::DisableTrigger(unsigned int id)
{
	auto i = Trigger.find(id);
	if (i == Trigger.end())
		throw std::logic_error{ "invalid trigger" };
	CScriptTrigger* t = i->second;
	scripttriggersys_removefromactive(this, t->EventType, t);
	t->Switch = false;
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

static inline int(__thiscall* const scripttrigger_callcond)(EScr::CScriptTrigger* th) = reinterpret_cast<int(__thiscall*)(EScr::CScriptTrigger*)>(0x59EEE4);
bool EScr::CScriptTrigger::CallCondition()
{
	return scripttrigger_callcond(this);
}

static inline int(__thiscall* const scripttrigger_callact)(EScr::CScriptTrigger* th) = reinterpret_cast<int(__thiscall*)(EScr::CScriptTrigger*)>(0x59EF21);
bool EScr::CScriptTrigger::CallAction()
{
	return scripttrigger_callact(this);
}

inline void(__thiscall* const stateseri_serialize)(EScr::LuaStateSerializer* th, lua_State* L) = reinterpret_cast<void(__thiscall*)(EScr::LuaStateSerializer*, lua_State*)>(0x5A1A78);
void EScr::LuaStateSerializer::SerializeState(lua_State* L)
{
	stateseri_serialize(this, L);
}
inline void(__thiscall* const stateseri_deserialize)(EScr::LuaStateSerializer* th, lua_State* L) = reinterpret_cast<void(__thiscall*)(EScr::LuaStateSerializer*, lua_State*)>(0x5A1D98);
void EScr::LuaStateSerializer::DeserializeState(lua_State* L)
{
	stateseri_deserialize(this, L);
}
inline void(__thiscall* const stateseri_ctor)(EScr::LuaStateSerializer* th) = reinterpret_cast<void(__thiscall*)(EScr::LuaStateSerializer * th)>(0x572E2A);
EScr::LuaStateSerializer::LuaStateSerializer() {
	stateseri_ctor(this);
}

void __stdcall luastateseri_hookserialize(BB::CFileStreamEx* f, lua_State* L, const char* filename) {
	try {
		int wr = 4;
		f->Write(&wr, sizeof(int));
		wr = 0;
		f->Write(&wr, sizeof(int));
		CppLogic::Serializer::AdvLuaStateSerializer seri{ *f, L };
		seri.SerializeState();
	}
	catch (const std::format_error& fe) {
		shok::LogString("AdvLuaStateSerializer: serialize std::format_error: %s\n", fe.what());
		throw;
	}
	catch (const BB::CException& be) {
		char buff[201]{};
		be.CopyMessage(buff, 200);
		shok::LogString("AdvLuaStateSerializer: serialize BB::CException: %s\n", buff);
		throw;
	}
}
void __declspec(naked) luastateseri_hookserializeasm() {
	__asm {
		lea eax, [ebp - 0x250];
		push eax;
		mov eax, 0x895DE8;
		mov eax, [eax];
		push eax;
		lea eax, [ebp - 0x18];
		push eax;
		call luastateseri_hookserialize;

		push 0x57510B;
		ret;
	};
}
void __stdcall lusstateseri_hookdeseri(BB::CFileStreamEx* f, lua_State* L, const char* filename) {
	int resi = 0;
	f->Read(&resi, sizeof(int));
	int renull = 0;
	f->Read(&renull, sizeof(int));
	if (resi == 4 && renull == 0) {
		try {
			CppLogic::Serializer::AdvLuaStateSerializer seri{ *f, L };
			seri.DeserializeState();
		}
		catch (const std::format_error& fe) {
			shok::LogString("AdvLuaStateSerializer: deserialize std::format_error: %s\n", fe.what());
			throw;
		}
		catch (const BB::CException& be) {
			char buff[201]{};
			be.CopyMessage(buff, 200);
			shok::LogString("AdvLuaStateSerializer: deserialize BB::CException: %s\n", buff);
			throw;
		}
	}
	else {
		f->Close();
		f->OpenFile(filename, BB::IStream::Flags::DefaultRead);
		EScr::LuaStateSerializer se{};
		se.Stream.CopyFromStream(*f);
		se.DeserializeState(L);
	}
}
void __declspec(naked) lusstateseri_hookdeseriasm() {
	__asm {
		// overriden func call (EScr::LuaStateSerializer ctor, only call it to not have to patch out its inlined dtor)
		mov eax, 0x572E2A;
		call eax;

		lea eax, [ebp - 0x250];
		push eax;
		mov eax, 0x895DE8;
		mov eax, [eax];
		push eax;
		lea eax, [ebp - 0x1C];
		push eax;
		call lusstateseri_hookdeseri;

		push 0x575334;
		ret;
	};
}

void EScr::LuaStateSerializer::HookSerializationOverride()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x5750ED),
		reinterpret_cast<void*>(0x575311),
		} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5750ED), &luastateseri_hookserializeasm, reinterpret_cast<void*>(0x5750F3));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x575311), &lusstateseri_hookdeseriasm, reinterpret_cast<void*>(0x57531A));
}

#include "pch.h"
#include "s5_scriptsystem.h"
#include <format>
#include <shok/s5_exception.h>
#include <shok/events/s5_events.h>
#include <shok/globals/s5_filesystem.h>
#include <utility/hooks.h>
#include <utility/luaserializer.h>

lua_State* shok::LuaStateMainmenu = nullptr;

void EScr::CScriptTriggerSystem::RunTrigger(BB::CEvent* ev)
{
	PostEvent(ev);
}

static inline void(__thiscall* const scripttriggersys_create)(EScr::CScriptTriggerSystem* th, shok::TriggerId* id, EScr::CScriptTrigger* t) = reinterpret_cast<void(__thiscall*)(EScr::CScriptTriggerSystem*, shok::TriggerId*, EScr::CScriptTrigger*)>(0x5A0FDA);
shok::TriggerId EScr::CScriptTriggerSystem::CreateTrigger(CScriptTrigger* trigger)
{
	shok::TriggerId id = {};
	scripttriggersys_create(this, &id, trigger);
	return id;
}

static inline void(__thiscall* const scripttriggersys_pushtoactive)(EScr::CScriptTriggerSystem* th, shok::EventIDs id, EScr::CScriptTrigger* t) = reinterpret_cast<void(__thiscall*)(EScr::CScriptTriggerSystem*, shok::EventIDs, EScr::CScriptTrigger*)>(0x5A0C7D);
void EScr::CScriptTriggerSystem::EnableTrigger(shok::TriggerId id)
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
void EScr::CScriptTriggerSystem::DisableTrigger(shok::TriggerId id)
{
	auto i = Trigger.find(id);
	if (i == Trigger.end())
		throw std::logic_error{ "invalid trigger" };
	CScriptTrigger* t = i->second;
	scripttriggersys_removefromactive(this, t->EventType, t);
	t->Switch = false;
}

void __stdcall ScriptTriggerSys_FireEventHooked(BB::IPostEvent* th, BB::CEvent* ev) {
	auto* t = static_cast<EScr::CScriptTriggerSystem*>(th);
	BB::CEvent* oldget = *EScr::CScriptTriggerSystem::CurrentRunningEventGet;
	BB::CEvent* oldset = *EScr::CScriptTriggerSystem::CurrentRunningEventSet;

	if (!t->TriggerSystemDisabled) {
		auto vec = t->ActiveTrigger.find(static_cast<shok::EventIDs>(ev->EventTypeId));
		if (vec != t->ActiveTrigger.end()) {
			for (auto* tr : vec->second) {
				if (!tr->Switch)
					continue;
				if (tr->MarkedForUnrequest)
					continue;
				*EScr::CScriptTriggerSystem::CurrentRunningEventGet = ev;
				*EScr::CScriptTriggerSystem::CurrentRunningEventSet = ev;
				if (!tr->ConditionFunc.CheckRef() || tr->CallCondition()) {
					*EScr::CScriptTriggerSystem::CurrentRunningEventGet = ev;
					*EScr::CScriptTriggerSystem::CurrentRunningEventSet = ev;
					tr->MarkedForUnrequest = tr->CallAction();
				}
			}

			auto v = vec->second.SaveVector();
			auto it = v.Vector.begin();
			while (it != v.Vector.end()) {
				auto* tr = *it;
				if (tr->MarkedForUnrequest) {
					// i could use BB funcs here, but why bother when this works
					t->Trigger.erase(t->Trigger.find(tr->UniqueID.UniqueID)); // 0x59FCFD
					it = v.Vector.erase(it); // 0x44803A
					delete tr;
				}
				else
					++it;
			}
		}
	}

	*EScr::CScriptTriggerSystem::CurrentRunningEventGet = oldget;
	*EScr::CScriptTriggerSystem::CurrentRunningEventSet = oldset;
}
void EScr::CScriptTriggerSystem::HookFireEvent()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x786674), 4 };
	*reinterpret_cast<void(__stdcall**)(BB::IPostEvent*, BB::CEvent*)>(0x786674) = &ScriptTriggerSys_FireEventHooked;;
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

void EScr::CScriptTriggerSystem::HookLoadFileToLuaState()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x59C04D), 20 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x59C04D), &LoadFileToLuaStateOverride, reinterpret_cast<void*>(0x59C052));
}
bool EScr::CScriptTriggerSystem::FileLoadAddArchive = false;
void __stdcall EScr::CScriptTriggerSystem::LoadFileToLuaStateOverride(lua_State* L, const char* name)
{
	auto [arch, s] = (*BB::CFileSystemMgr::GlobalObj)->OpenFileStreamWithSource(name, BB::IStream::Flags::DefaultRead);
	std::string fmt{};

	if (!arch.empty() && FileLoadAddArchive) {
		fmt = std::format("{}@{}", name, arch);
	}

	std::string data{};
	data.resize(s->GetSize());
	s->Read(data.data(), data.size());
	while (data.size() > 0 && data.back() == '\0')
		data.pop_back();
	reinterpret_cast<void(__stdcall* const)(lua_State*, const char*, size_t, const char*)>(0x59BE57)(L, data.data(), data.size(), fmt.empty() ? name : fmt.c_str());
}

int __stdcall EScr::CLuaFuncRefCommand::GetRefToFunc()
{
	return reinterpret_cast<int(__stdcall* const)(CLuaFuncRefCommand*)>(0x5A1884)(this);
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
inline void(__stdcall* const funcref_setstate)(EScr::CLuaFuncRef* th, lua::State L) = reinterpret_cast<void(__stdcall*)(EScr::CLuaFuncRef*, lua::State)>(0x5A1617);
void EScr::CLuaFuncRef::SetState(lua::State L)
{
	funcref_setstate(this, L);
}

void EScr::CLuaFuncRefCommand::ReplaceFunc(lua::State L, int idx)
{
	StateAddon::HookSetAllRecompile();
	CheckRef();
	Clear();
	SetState(L);
	L.CheckType(idx, lua::LType::Function);
	L.PushValue(idx);
	Ref = L.Ref(L.REGISTRYINDEX).Value();
	NeedsCompile = false;
	LuaCommand = std::string_view("");
}

int __stdcall EScr::CLuaFuncRefGlobal::GetRefToFunc()
{
	return (*reinterpret_cast<int(__stdcall** const)(CLuaFuncRefGlobal*)>(0x786BD8))(this);
}

static inline void(__stdcall* const funcglo_set)(EScr::CLuaFuncRefGlobal* th, const char* c) = reinterpret_cast<void(__stdcall* const)(EScr::CLuaFuncRefGlobal*, const char*)>(0x5A18EB);
void EScr::CLuaFuncRefGlobal::SetCommandString(const char* c)
{
	funcglo_set(this, c);
}

void EScr::CLuaFuncRefGlobal::HookFuncAccess(bool active)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x786BD8), 4 };
	*reinterpret_cast<void**>(0x786BD8) = active ? CppLogic::Hooks::MemberFuncPointerToVoid(&CLuaFuncRefGlobal::GetRefOverride, 0) : reinterpret_cast<void*>(0x5A180C);
}

int __stdcall EScr::CLuaFuncRefGlobal::GetRefOverride()
{
	int t = L.GetTop();
	lua::Reference r = lua::State::NoRef;
	if (FuncName.size() == 0)
		return r.Value();
	L.Push(FuncName);
	L.GetGlobal();
	if (L.IsFunction(-1)) {
		r = L.Ref(lua::State::REGISTRYINDEX);
	}
	else {
		try {
			L.DoStringT(std::format("return {};", static_cast<std::string_view>(FuncName)), "EScr::CLuaFuncRefGlobal::GetRefOverride");
			if (L.IsFunction(-1)) {
				r = L.Ref(lua::State::REGISTRYINDEX);
			}
		}
		catch (const lua::LuaException&) {}
	}
	L.SetTop(t);
	return r.Value();
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

void __stdcall EScr::LuaStateSerializer::SerializeOverride(BB::CFileStreamEx* f, lua_State* L, const char* filename)
{
	try {
		int wr = 4;
		f->Write(&wr, sizeof(int));
		wr = 0;
		f->Write(&wr, sizeof(int));
		CppLogic::Serializer::AdvLuaStateSerializer seri{ *f, L };
		seri.SerializeState();
	}
	catch (const std::format_error& fe) {
		std::string errmnsg{ "AdvLuaStateSerializer: serialize std::format_error: " };
		errmnsg.append(fe.what());
		MessageBox(0, errmnsg.c_str(), "", 0);
		shok::LogString("%s\n", errmnsg.c_str());
		throw;
	}
	catch (const BB::CException& be) {
		char buff[201]{};
		be.CopyMessage(buff, 200);
		std::string errmnsg{ "AdvLuaStateSerializer: serialize BB::CException: " };
		errmnsg.append(buff);
		MessageBox(0, errmnsg.c_str(), "", 0);
		shok::LogString("%s\n", errmnsg.c_str());
		throw;
	}
}

void EScr::LuaStateSerializer::SerializeOverrideASM() {
	__asm {
		lea eax, [ebp - 0x250];
		push eax;
		mov eax, 0x895DE8;
		mov eax, [eax];
		push eax;
		lea eax, [ebp - 0x18];
		push eax;
		call EScr::LuaStateSerializer::SerializeOverride;

		push 0x57510B;
		ret;
	};
}
void __stdcall EScr::LuaStateSerializer::DeserializeOverride(BB::CFileStreamEx* f, lua_State* L, const char* filename)
{
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
			std::string errmnsg{ "AdvLuaStateSerializer: deserialize std::format_error: " };
			errmnsg.append(fe.what());
			MessageBox(0, errmnsg.c_str(), "", 0);
			shok::LogString("%s\n", errmnsg.c_str());
			throw;
		}
		catch (const BB::CException& be) {
			char buff[201]{};
			be.CopyMessage(buff, 200);
			std::string errmnsg{ "AdvLuaStateSerializer: deserialize BB::CException: " };
			errmnsg.append(buff);
			MessageBox(0, errmnsg.c_str(), "", 0);
			shok::LogString("%s\n", errmnsg.c_str());
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

void EScr::LuaStateSerializer::DeserializeOverrideASM() {
	__asm {
		// overridden func call (EScr::LuaStateSerializer ctor, only call it to not have to patch out its inlined dtor)
		mov eax, 0x572E2A;
		call eax;

		lea eax, [ebp - 0x250];
		push eax;
		mov eax, 0x895DE8;
		mov eax, [eax];
		push eax;
		lea eax, [ebp - 0x1C];
		push eax;
		call EScr::LuaStateSerializer::DeserializeOverride;

		push 0x575334;
		ret;
	};
}
void __cdecl EScr::LuaStateSerializer::AddGlobalNoSaveOverride(const char* n)
{
	shok::String s{ n };
	auto* v = EScr::LuaStateSerializer::DoNotSerializeGlobals();
	for (const auto& i : *v) {
		if (s == i)
			return;
	}
	auto sv = v->SaveVector();
	sv.Vector.push_back(s);
}

void EScr::LuaStateSerializer::HookSerializationOverride()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x5750ED),
		reinterpret_cast<void*>(0x575311),
		AddGlobalToNotSerialize,
		} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5750ED), &SerializeOverrideASM, reinterpret_cast<void*>(0x5750F3));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x575311), &DeserializeOverrideASM, reinterpret_cast<void*>(0x57531A));
	CppLogic::Hooks::WriteJump(AddGlobalToNotSerialize, &AddGlobalNoSaveOverride, reinterpret_cast<void*>(0x5A1E16));
}

void __fastcall EScr::StateAddon::SetAllLuaFuncsRecompileOverride(lua_State* L)
{
	auto* state = Get(L);
	if (state) {
		for (auto* ref : state->RegisteredRefs) {
			auto* cmd = dynamic_cast<CLuaFuncRefCommand*>(ref);
			if (cmd != nullptr && cmd->LuaCommand.size() == 0)
				continue;
			if (ref == nullptr)
				continue;
			ref->NeedsCompile = true;
		}
	}
}

bool StateAddon_HookSetAllRecompileHooked = false;
void EScr::StateAddon::HookSetAllRecompile()
{
	if (StateAddon_HookSetAllRecompileHooked)
		return;
	StateAddon_HookSetAllRecompileHooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x59B1C7),
		} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x59B1C7), &EScr::StateAddon::SetAllLuaFuncsRecompileOverride, reinterpret_cast<void*>(0x59B1CD));
}

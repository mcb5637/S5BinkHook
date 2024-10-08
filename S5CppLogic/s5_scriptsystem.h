#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_filesystem.h"
#include "luaext.h"

namespace shok {
	extern lua_State* LuaStateMainmenu;
}

namespace EScr {
	// ingame state if exists, mainmenu otherwise
	inline lua_State** (__cdecl* const GetCurrentLuaState)() = reinterpret_cast<lua_State** (__cdecl*)()>(0x55ADE0);

	class IScriptTriggerSystem : public BB::IObject, public BB::IPostEvent {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual void RegisterEventFuncGet(const char* funcName, int(__cdecl* getIntFromEvent)(BB::CEvent* ev)) = 0;
		virtual void RegisterEventFuncSet(const char* funcName, void(__cdecl* setIntFromEvent)(BB::CEvent* ev, int i)) = 0;
		virtual void __stdcall CallGlobal(const char* globalName, BB::CEvent* ev) = 0; // actually sets CurrentRunningEventSet
	private:
		virtual void __stdcall RefreshFuncs() = 0;
		virtual void* GetDebugIterator(int) = 0; // returns new EScr::CTriggerDebugDataIterator
	};

	class CScriptTriggerSystem : public IScriptTriggerSystem {
	public:
		shok::Map<shok::TriggerId, CScriptTrigger*> Trigger; // 2 UniqueID -> Data
		shok::Map<shok::EventIDs, shok::Vector<CScriptTrigger*>> ActiveTrigger; // 5
		shok::TriggerId UniqueTriggerID; // next trigger id 8
		bool TriggerSystemDisabled;
		lua_State* State;


		static inline constexpr int vtp = 0x78667C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3F9B1F03);

		void RunTrigger(BB::CEvent* ev);
		shok::TriggerId CreateTrigger(CScriptTrigger* trigger);
		void EnableTrigger(shok::TriggerId id);
		void DisableTrigger(shok::TriggerId id);

		static inline EScr::CScriptTriggerSystem** const GlobalObj = reinterpret_cast<EScr::CScriptTriggerSystem**>(0x895DEC); // also 0xA06548
		static inline lua_State** const GameState = reinterpret_cast<lua_State**>(0x853A9C);
		static inline BB::CEvent** const CurrentRunningEventSet = reinterpret_cast<BB::CEvent**>(0xA06544); // always nullptr
		static inline BB::CEvent** const CurrentRunningEventGet = reinterpret_cast<BB::CEvent**>(0xA06540);

		// loads and executes a file from the internal filesystem. also passes it to the luadebugger.
		static inline void(__stdcall* const LoadFileToLuaState)(lua_State* L, const char* name) = reinterpret_cast<void(__stdcall*) (lua_State*, const char*)> (0x59C04D);
		// funcName may be nullptr, does nothing on success
		static inline void(__stdcall* const HandleLuaError)(lua_State* L, lua::ErrorCode e, const char* funcName) = reinterpret_cast<void(__stdcall*)(lua_State*, lua::ErrorCode, const char*)>(0x5A28A7);

		static void HookFireEvent();
		static void HookRemoveFuncOverrides();
		static void HookLoadFileToLuaState();
		static bool FileLoadAddArchive;
	private:
		static void __stdcall LoadFileToLuaStateOverride(lua_State* L, const char* name);
	};
	//constexpr int i = offsetof(CScriptTriggerSystem, ActiveTrigger) / 4;

	class CLuaFuncRef {
	public:
		virtual int __stdcall GetRefToFunc() = 0;

		lua::State L;
		bool NeedsCompile;
		PADDING(3);
		int Ref; // 3

		void Clear();
		void CreateRef(); // unrefs anything previously refd
		bool CheckRef(); // creates if not already done, then return if the ref is valid
		bool Call(int nargs, int nres); // uses shok error handling, so you get a popup on errors
		void SetState(lua::State L); // removes from prev StateAddon and adds to new one (both with null check)
	};

	class CLuaFuncRefCommand : public CLuaFuncRef {
	public:
		shok::String LuaCommand; // 4 - 10
		PADDINGI(1); // 11

		virtual int __stdcall GetRefToFunc() override;
		void SetCommandString(const char* c);

		void ReplaceFunc(lua::State L, int idx);

		static inline constexpr int vtp = 0x786BE0;
		static inline constexpr int TypeDesc = 0x83B83C;
	};
	static_assert(sizeof(EScr::CLuaFuncRefCommand) == 12 * 4);

	class CLuaFuncRefGlobal : public CLuaFuncRef {
	public:
		shok::String FuncName;

		virtual int __stdcall GetRefToFunc() override;
		void SetCommandString(const char* c);

		static inline constexpr int vtp = 0x786BD8;

		static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA068B0);

		static void HookFuncAccess(bool active);
	private:
		int __stdcall GetRefOverride();
	};
	static_assert(sizeof(CLuaFuncRefGlobal) == 11 * 4);

	class CScriptTrigger : public BB::IObject {
	public:
		struct Parameter {
			double Value;
			int Type; // 1 double, 2 string, 0 nothing
			shok::String String;
		};

		shok::EventIDs EventType; // 1
		bool Switch; // active, 2
		CLuaFuncRefGlobal ConditionFunc; // 3
		CLuaFuncRefGlobal ActionFunc; // 14
		bool MarkedForUnrequest; // 25
		struct {
			shok::TriggerId UniqueID;
		} UniqueID; // 26
		shok::Vector<Parameter> ConditionParameter;
		shok::Vector<Parameter> ActionParameter;

		static inline constexpr int vtp = 0x786664;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x772113A3);

		bool CallCondition();
		bool CallAction();
	};
	static_assert(offsetof(CScriptTrigger, ActionFunc) == 14 * 4);
	static_assert(sizeof(CScriptTrigger) == 35 * 4);

	class ILuaDebugger {
		// no vtable
	public:
		virtual void __stdcall NewFile(lua_State* L, const char* filename, const char* filedata, size_t dataLength) = 0;
		virtual void __stdcall AddLuaState(lua_State* L) = 0;
		virtual void __stdcall RemoveLuaState(lua_State* L) = 0;
	};

	class LuaStateSerializer {
	public:
		BB::CMemoryStream Stream;
		int TableDepth = 0;
		shok::Vector<int> References; // refs to tables

		// ctor 572E2A
		// serialize table 5A19EF (this, L, index)
		// serialize table kv pair 5A1E50 (this, L) primitive type, value, primitive key string, return success, if false, have to remove type from stream wtf?
		// serialize primitive 5A19AB (this, data*, len) writes len, data
		// deserialize primitive 5A1AA6 (this, len*) reads len, then data, fixed size buffer 0xA06C98! end 0xA0AB18?
		// deseraialize primitive, throw away size 5A1B6D (this)
		// deserialize table 5A1BB0 (this, L, index)
		void SerializeState(lua_State* L);
		void DeserializeState(lua_State* L);
		LuaStateSerializer();

		static inline shok::Vector<shok::String>* (__cdecl* const DoNotSerializeGlobals)() = reinterpret_cast<shok::Vector<shok::String>*(__cdecl*)()>(0x5A1DC5);
		static inline void(__cdecl* const AddGlobalToNotSerialize)(const char* n) = reinterpret_cast<void(__cdecl*)(const char*)>(0x5A1E0C);

		static void HookSerializationOverride();
	private:
		static void __stdcall SerializeOverride(BB::CFileStreamEx* f, lua_State* L, const char* filename);
		static void __stdcall DeserializeOverride(BB::CFileStreamEx* f, lua_State* L, const char* filename);
		static void __cdecl AddGlobalNoSaveOverride(const char* n);
	};

	struct StateAddon {
		shok::Map<shok::String, const char*> Docs;
		ILuaDebugger* LuaDebugger;
		bool DebugScript;
		shok::List<CLuaFuncRef*> RefsToClose;

		// ctor 0x5A282C, dtor 0x59D39F

		static inline StateAddon* (__stdcall* const Get)(lua_State* L) = reinterpret_cast<StateAddon* (__stdcall*)(lua_State*)>(0x59B1A3);
		static inline void(__stdcall* const Set)(lua_State* L, StateAddon* a) = reinterpret_cast<void(__stdcall*)(lua_State*, StateAddon*)>(0x5A21FC);
	};
	static_assert(sizeof(StateAddon) == 8 * 4);
}

class CLuaDebuggerPort : public EScr::ILuaDebugger {
public:
	static inline constexpr int vtp = 0x76233C;
};

namespace EGL {
	class IGLEScripting {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual void __stdcall LoadMapscript(const char* buff) = 0;
		virtual const char* __stdcall GetMapscript() = 0;
		virtual void __stdcall CallSetDefaultValues() = 0;
		virtual void __stdcall CallNewGame() = 0;
		virtual void __stdcall OnTick() = 0;
		virtual void __stdcall UpdateTimer() = 0;
		virtual ~IGLEScripting() = default;

		static inline constexpr int vtp = 0x784534;
	};

	class CGLEScripting : public IGLEScripting {
	public:
		lua_State* Ingame;
		lua_State* Ingame2;
		char* Mapscript;
		int Timer;

		static inline constexpr int vtp = 0x784558;
	};
}

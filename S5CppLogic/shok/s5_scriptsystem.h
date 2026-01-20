#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/globals/s5_filesystem.h>
#include <luaext.h>

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
		shok::TriggerId UniqueTriggerID = static_cast<shok::TriggerId>(1); // next trigger id 8
		bool TriggerSystemDisabled = false;
		lua_State* State = nullptr;


		static inline constexpr int vtp = 0x78667C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3F9B1F03);

		void RunTrigger(BB::CEvent* ev);
		shok::TriggerId CreateTrigger(CScriptTrigger* trigger);
		void EnableTrigger(shok::TriggerId id);
		void DisableTrigger(shok::TriggerId id);

		static inline EScr::CScriptTriggerSystem** const GlobalObj = reinterpret_cast<EScr::CScriptTriggerSystem**>(0x895DEC); // also 0xA06548, 0x895de8
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

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CLuaFuncRef {
	public:
		virtual int __stdcall GetRefToFunc() = 0;

		lua::State L = nullptr;
		bool NeedsCompile = true;
		PADDING(3);
		int Ref = lua::State::NoRef.Value(); // 3

		void Clear();
		void CreateRef(); // unrefs anything previously refd
		bool CheckRef(); // creates if not already done, then return if the ref is valid
		bool Call(int nargs, int nres); // uses shok error handling, so you get a popup on errors
		void SetState(lua::State l); // removes from prev StateAddon and adds to new one (both with null check)

		static inline constexpr int vtp = 0x786bd0;
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
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

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
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
			shok::String String;
			int Type; // 1 double, 2 string, 0 nothing

			// read 59f7f0 stdcall(L, index) -> valid
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

		// read params 5a0dc1(L, index)

		static inline constexpr int vtp = 0x786664;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x772113A3);

		bool CallCondition();
		bool CallAction();
	};
	static_assert(offsetof(CScriptTrigger, ActionFunc) == 14 * 4);
	static_assert(sizeof(CScriptTrigger) == 35 * 4);

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
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
		// deserialize primitive, throw away size 5A1B6D (this)
		// deserialize table 5A1BB0 (this, L, index)
		void SerializeState(lua_State* L);
		void DeserializeState(lua_State* L);
		LuaStateSerializer();

		static inline shok::Vector<shok::String>* (__cdecl* const DoNotSerializeGlobals)() = reinterpret_cast<shok::Vector<shok::String>*(__cdecl*)()>(0x5A1DC5);
		static inline void(__cdecl* const AddGlobalToNotSerialize)(const char* n) = reinterpret_cast<void(__cdecl*)(const char*)>(0x5A1E0C);

		static void HookSerializationOverride();
	private:
		static void __stdcall SerializeOverride(BB::CFileStreamEx* f, lua_State* L, const char* filename);
		static void NAKED_DECL SerializeOverrideASM();
		static void __stdcall DeserializeOverride(BB::CFileStreamEx* f, lua_State* L, const char* filename);
		static void NAKED_DECL DeserializeOverrideASM();
		static void __cdecl AddGlobalNoSaveOverride(const char* n);
	};

	struct StateAddon {
		shok::Map<shok::String, const char*> Docs;
		ILuaDebugger* LuaDebugger = nullptr;
		bool DebugScript = false;
		shok::List<CLuaFuncRef*> RegisteredRefs;

		// ctor 0x5A282C, dtor 0x59D39F

		static inline StateAddon* (__stdcall* const Get)(lua_State* L) = reinterpret_cast<StateAddon* (__stdcall*)(lua_State*)>(0x59B1A3);
		static inline void(__stdcall* const Set)(lua_State* L, StateAddon* a) = reinterpret_cast<void(__stdcall*)(lua_State*, StateAddon*)>(0x5A21FC);
		static inline void(__fastcall* const SetAllLuaFuncsRecompile)(lua_State* L) = reinterpret_cast<void(__fastcall*)(lua_State*)>(0x59B1C7);

		static void HookSetAllRecompile();
		static void __fastcall SetAllLuaFuncsRecompileOverride(lua_State* L);
	};
	static_assert(sizeof(StateAddon) == 8 * 4);

	struct StaticFuncListEntry {
		StaticFuncListEntry* Next = nullptr;
		const char* Table = nullptr;
		const char* Name = nullptr;
		lua::CFunction* Func = nullptr;
		const char* Description = nullptr;
	};
	struct StaticFuncList {
		StaticFuncListEntry* First = nullptr;

		// 59d417 add stdcall(StaticFuncListEntry*)
		// 59b22b register func static stdcall(state, table, name, func, desc)
		// 59d42c register list stdcall(state, table) (table nullptr, use from entry)

		static inline auto* FrameworkList = reinterpret_cast<StaticFuncList*(*)()>(0x408cbd);
		static inline auto* ScriptList = reinterpret_cast<StaticFuncList*(*)()>(0x408cda);
		static inline auto* LuaDebuggerList = reinterpret_cast<StaticFuncList*(*)()>(0x408cf7);
		static inline auto* MouseList = reinterpret_cast<StaticFuncList*(*)()>(0x408d14);
		static inline auto* GameList = reinterpret_cast<StaticFuncList*(*)()>(0x408daa);
		static inline auto* GDBList = reinterpret_cast<StaticFuncList*(*)()>(0x408d31);
		static inline auto* DisplayOptionsList = reinterpret_cast<StaticFuncList*(*)()>(0x0408d4e);
		static inline auto* SoundOptionsList = reinterpret_cast<StaticFuncList*(*)()>(0x408d6b);
		static inline auto* AIList = reinterpret_cast<StaticFuncList*(*)()>(0x446071);
		static inline auto* XNetworkList = reinterpret_cast<StaticFuncList*(*)()>(0x452860);
		static inline auto* DisplayList = reinterpret_cast<StaticFuncList*(*)()>(0x467018);
		static inline auto* Logic1List = reinterpret_cast<StaticFuncList*(*)()>(0x4c0d12);
		static inline auto* Logic2List = reinterpret_cast<StaticFuncList*(*)()>(0x4c0d2f);
		static inline auto* Logic3List = reinterpret_cast<StaticFuncList*(*)()>(0x5712f3);
		static inline auto* Logic4List = reinterpret_cast<StaticFuncList*(*)()>(0x571310);
		static inline auto* CameraList = reinterpret_cast<StaticFuncList*(*)()>(0x51b22a);
		static inline auto* GUIList = reinterpret_cast<StaticFuncList*(*)()>(0x52345a);
		static inline auto* GUIList2 = reinterpret_cast<StaticFuncList*(*)()>(0x523477);
		static inline auto* XGUIEngList = reinterpret_cast<StaticFuncList*(*)()>(0x55ae45);
		static inline auto* SoundList = reinterpret_cast<StaticFuncList*(*)()>(0x496c60);
		static inline auto* MusicList = reinterpret_cast<StaticFuncList*(*)()>(0x4971a5);
		static inline auto* CutsceneList = reinterpret_cast<StaticFuncList*(*)()>(0x596a90);
		static inline auto* InputList = reinterpret_cast<StaticFuncList*(*)()>(0x59dc0a);
		static inline auto* XNetworkUbiComList = reinterpret_cast<StaticFuncList*(*)()>(0x5a56f0);
	};
}

// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
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

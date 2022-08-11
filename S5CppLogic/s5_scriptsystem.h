#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "Luapp/luapp50.h"

namespace shok {
	extern lua_State* LuaStateMainmenu;
}

namespace EScr {
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
		shok::Map<unsigned int, CScriptTrigger*> Trigger; // 2 UniqueID -> Data
		shok::Map<shok::EventIDs, shok::Vector<CScriptTrigger*>> ActiveTrigger; // 5
		unsigned int UniqueTriggerID; // next trigger id 8
		bool TriggerSystemDisabled;
		lua_State* State;


		static inline constexpr int vtp = 0x78667C;
		static inline constexpr unsigned int Identifier = 0x3F9B1F03;

		void RunTrigger(BB::CEvent* ev);
		int CreateTrigger(CScriptTrigger* trigger);
		void EnableTrigger(unsigned int id);
		void DisableTrigger(unsigned int id);

		static inline EScr::CScriptTriggerSystem** const GlobalObj = reinterpret_cast<EScr::CScriptTriggerSystem**>(0x895DEC); // also 0xA06548
		static inline lua_State** const GameState = reinterpret_cast<lua_State**>(0x853A9C);
		static inline BB::CEvent** const CurrentRunningEventSet = reinterpret_cast<BB::CEvent**>(0xA06544); // always nullptr
		static inline BB::CEvent** const CurrentRunningEventGet = reinterpret_cast<BB::CEvent**>(0xA06540);

		// loads and executes a file from the internal filesystem. also passes it to the luadebugger.
		static inline void(__stdcall* const LoadFileToLuaState)(lua_State* L, const char* name) = reinterpret_cast<void(__stdcall*) (lua_State*, const char*)> (0x59C04D);

		static void HookRemoveFuncOverrides();
	};
	//constexpr int i = offsetof(CScriptTriggerSystem, ActiveTrigger) / 4;

	class CLuaFuncRef {
	public:
		virtual lua::Reference __stdcall GetRefToFunc() = 0;

		lua::State L; // check which of the funcs below are also used by CLuaFuncRefGlobal
		bool NeedsCompile;
		PADDING(3);
		lua::Reference Ref; // 3

		void Clear();
		void CreateRef(); // unrefs anything previously refd
		bool CheckRef(); // creates if not already done, then return if the ref is valid
		bool Call(int nargs, int nres); // uses shok error handling, so you get a popup on errors
	};

	class CLuaFuncRefCommand : public CLuaFuncRef {
	public:
		shok::String LuaCommand; // 4 - 10
		PADDINGI(1); // 11

		virtual lua::Reference __stdcall GetRefToFunc() override;
		void SetCommandString(const char* c);

		void ReplaceFunc(lua::State L, int idx);

		static inline constexpr int vtp = 0x786BE0;
		static inline constexpr int TypeDesc = 0x83B83C;
	};
	static_assert(sizeof(EScr::CLuaFuncRefCommand) == 12 * 4);

	class CLuaFuncRefGlobal : public CLuaFuncRef {
	public:
		shok::String FuncName;

		virtual lua::Reference __stdcall GetRefToFunc() override;
		void SetCommandString(const char* c);

		static inline constexpr int vtp = 0x786BD8;

		static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA068B0);
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
			unsigned int UniqueID;
		} UniqueID; // 26
		shok::Vector<Parameter> ConditionParameter;
		shok::Vector<Parameter> ActionParameter;

		static inline constexpr int vtp = 0x786664;
		static inline constexpr unsigned int Identifier = 0x772113A3;

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
}

class CLuaDebuggerPort : public EScr::ILuaDebugger {
public:
	static inline constexpr int vtp = 0x76233C;
};

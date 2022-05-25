#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "Luapp/luapp50.h"

namespace shok {
	extern lua_State* LuaStateMainmenu;
}

namespace EScr {
	class IScriptTriggerSystem : public BB::IObject, public BB::IPostEvent {

	};

	class CScriptTriggerSystem : public IScriptTriggerSystem {

	public:
		static inline constexpr int vtp = 0x78667C;

		void RunTrigger(BB::CEvent* ev);

		static inline EScr::CScriptTriggerSystem** const GlobalObj = reinterpret_cast<EScr::CScriptTriggerSystem**>(0x895DEC);
		static inline lua_State** const GameState = reinterpret_cast<lua_State**>(0x853A9C);

		// loads and executes a file from the internal filesystem. also passes it to the luadebugger.
		static inline void(__stdcall* const LoadFileToLuaState)(lua_State* L, const char* name) = reinterpret_cast<void(__stdcall*) (lua_State*, const char*)> (0x59C04D);

		static void HookRemoveFuncOverrides();
	};

	class CLuaFuncRef {
	public:
		virtual lua::Reference __stdcall GetRefToFunc() = 0;
	};

	class CLuaFuncRefCommand : public CLuaFuncRef {
	public:
		lua::State L; // check which of the funcs below are also used by CLuaFuncRefGlobal
		bool NeedsCompile;
		PADDING(3);
		lua::Reference Ref; // 3
		shok::String LuaCommand; // 4 - 10
		PADDINGI(1); // 11

		virtual lua::Reference __stdcall GetRefToFunc() override;
		void SetCommandString(const char* c);
		void Clear();
		void CreateRef(); // unrefs anything previously refd
		bool CheckRef(); // creates if not already done, then return if the ref is valid
		bool Call(int nargs, int nres); // uses shok error handling, so you get a popup on errors

		void ReplaceFunc(lua::State L, int idx);

		static inline constexpr int vtp = 0x786BE0;
		static inline constexpr int TypeDesc = 0x83B83C;
	};
	static_assert(sizeof(EScr::CLuaFuncRefCommand) == 12 * 4);

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

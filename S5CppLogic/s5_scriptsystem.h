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
}

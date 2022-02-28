#pragma once
#include "s5data.h"

namespace EScr {
	class IScriptTriggerSystem : public BB::IObject, public BB::IPostEvent {

	};

	class CScriptTriggerSystem : public IScriptTriggerSystem {

	public:
		static inline constexpr int vtp = 0x78667C;

		void RunTrigger(BB::CEvent* ev);

		static inline EScr::CScriptTriggerSystem** const GlobalObj = reinterpret_cast<EScr::CScriptTriggerSystem**>(0x895DEC);
		static inline lua_State** const GameState = reinterpret_cast<lua_State**>(0x853A9C);
	};
}

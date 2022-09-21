#pragma once

#include "luaext.h"

namespace CppLogic::Logic {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnLoad();
	void OnSaveLoaded(lua::State L);
	constexpr const char* HurtEntityCallWithNoAttackerRegKey = "CppLogic::Logic::HurtEntityCallWithNoAttacker";
}

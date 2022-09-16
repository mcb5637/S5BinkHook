#pragma once

#include "luaext.h"

namespace CppLogic::Logic {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnLoad();
}

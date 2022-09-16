#pragma once

#include "luaext.h"

namespace CppLogic::Tech {
	void Init(lua::State L);
	void Cleanup(lua::State L);
}

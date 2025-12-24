#pragma once

#include <luaext.h>

namespace CppLogic::Tech {
	void Init(luaext::State L);
	void Cleanup(luaext::State L);
}

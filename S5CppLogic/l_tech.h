#pragma once

#include "Luapp/luapp50.h"

namespace CppLogic::Tech {
	void Init(lua::State L);
	void Cleanup(lua::State L);
}

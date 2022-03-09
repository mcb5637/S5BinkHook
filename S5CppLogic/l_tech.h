#pragma once

#include "luaimport.h"
#include "s5data.h"

namespace CppLogic::Tech {
	void Init(lua::State L);
	void Cleanup(lua::State L);
}

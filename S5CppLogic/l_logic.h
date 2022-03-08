#pragma once

#include "luaimport.h"
#include "s5data.h"

namespace CppLogic::Logic {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnLoad();
}

#pragma once
#include "Luapp/luapp50.h"

namespace CppLogic::Effect {
	void Init(lua::State L);
	void Cleanup(lua::State L);
}

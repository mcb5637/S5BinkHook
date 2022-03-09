#pragma once

#include "Luapp/luapp50.h"

namespace CppLogic::Logic {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnLoad();
}

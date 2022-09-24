#pragma once

#include "luaext.h"

namespace CppLogic::Entity {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnSaveLoaded(lua::State L);
}

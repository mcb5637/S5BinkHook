#pragma once

#include <luaext.h>

namespace CppLogic::Entity {
	void Init(luaext::State L);
	void Cleanup(luaext::State L);
	void OnSaveLoaded(luaext::State L);
}

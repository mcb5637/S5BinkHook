#pragma once
#include <luaext.h>

namespace CppLogic::Combat {
	void Init(luaext::State L);
	void Cleanup(luaext::State L);
	void OnSaveLoaded(luaext::State L);
}

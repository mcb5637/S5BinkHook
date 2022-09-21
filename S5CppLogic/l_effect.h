#pragma once
#include "luaext.h"

namespace CppLogic::Effect {
	void Init(lua::State L);
	void Cleanup(lua::State L);

	constexpr const char* EffectOnHitKey = "CppLogic::Effect_OnHit";
}

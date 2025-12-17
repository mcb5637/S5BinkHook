#pragma once
#include <luaext.h>

namespace CppLogic::Effect {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnSaveLoaded(lua::State L);

	constexpr std::string_view EffectOnHitKey = "CppLogic::Effect_OnHit";
}

#pragma once
#include <luaext.h>

namespace CppLogic::Effect {
	void Init(luaext::State L);
	void Cleanup(luaext::State L);
	void OnSaveLoaded(luaext::State L);

	constexpr std::string_view EffectOnHitKey = "CppLogic::Effect_OnHit";
}

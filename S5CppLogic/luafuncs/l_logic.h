#pragma once

#include <luaext.h>

namespace CppLogic::Logic {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnLoad();
	void OnSaveLoaded(lua::State L);
	constexpr std::string_view CanPlaceBuildingCallbackRegKey = "CppLogic::Logic::CanPlaceBuildingCallback";
	constexpr std::string_view SnipeDamageOverrideRegKey = "CppLogic::Logic::SnipeDamageOverride";
	constexpr std::string_view SetLuaTaskListFuncRegKey = "CppLogic::Logic::SetLuaTaskListFunc";
	constexpr std::string_view NetEventSetHookRegKey = "CppLogic::Logic::NetEventSetHook";
}

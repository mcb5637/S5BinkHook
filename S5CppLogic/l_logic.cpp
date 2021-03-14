#include "pch.h"
#include "l_logic.h"
#include "luaext.h"
#include <assert.h>

int l_logicGetDamageModifier(lua_State* L) {
	int dmgclass = luaL_checkint(L, 1);
	int size = (int)(*shok_DamageClassHolderObj)->DamageClassList.size();
	luaext_assert(L, dmgclass > 0 && dmgclass < size, "invalid damagecass");
	int amclass = luaL_checkint(L, 2);
	amclass--;
	luaext_assert(L, amclass >= 0 && amclass < 7, "invalid armorclass");
	assert(amclass >= 0 && amclass < 7);
	lua_pushnumber(L, (*shok_DamageClassHolderObj)->DamageClassList[dmgclass]->BonusVsArmorClass[amclass]);
	return 1;
}
int l_logicSetDamageModifier(lua_State* L) {
	int dmgclass = luaL_checkint(L, 1);
	int size = (int)(*shok_DamageClassHolderObj)->DamageClassList.size();
	luaext_assert(L, dmgclass > 0 && dmgclass < size, "invalid damagecass");
	int amclass = luaL_checkint(L, 2);
	amclass--;
	luaext_assert(L, amclass >= 0 && amclass < 7, "invalid armorclass");
	assert(amclass >= 0 && amclass < 7);
	(*shok_DamageClassHolderObj)->DamageClassList[dmgclass]->BonusVsArmorClass[amclass] = luaL_checkfloat(L, 2);
	return 0;
}

int l_logicReloadCutscene(lua_State* L) {
	const char* data = luaL_optlstring(L, 1, "Maps\\ExternalMap", nullptr);
	(**shok_ECS_CManagerObject)->ReloadCutscene(data);
	return 0;
}

int l_logicGetAnimIdFromName(lua_State* L) {
	const char* data = luaL_checkstring(L, 1);
	int id = (*shok_BB_CIDManagerExObj)->GetAnimIdByName(data);
	lua_pushnumber(L, id);
	return 1;
}

int l_playerGetPaydayStatus(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	if (p->PlayerAttractionHandler->PaydayStarted)
		lua_pushnumber(L, p->PlayerAttractionHandler->PaydayStartTick);
	else
		lua_pushnumber(L, -1);
	return 1;
}
int l_playerSetPaydayStatus(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int st = luaL_checkint(L, 2);
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	if (st < 0) {
		p->PlayerAttractionHandler->PaydayStarted = 0;
	}
	else {
		p->PlayerAttractionHandler->PaydayStarted = 1;
		p->PlayerAttractionHandler->PaydayStartTick = st;
	}
	return 0;
}


int l_netEventCbRef = LUA_NOREF;
bool l_netEventPostCallback(shok_BB_CEvent* ev) {
	int id = ev->EventTypeId;
	lua_State* L = *shok_luastate_game;
	int top = lua_gettop(L);
	luaL_checkstack(L, 2, "");
	lua_rawgeti(L, LUA_REGISTRYINDEX, l_netEventCbRef);
	bool r = false;
	if (lua_isfunction(L, -1)) {
		lua_pushnumber(L, id);
		lua_pcall(L, 1, 1, 0);
		r = lua_toboolean(L, -1);
	}
	lua_settop(L, top);
	return r;
}

int l_netEventSetHook(lua_State* L) {
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	luaL_unref(L, LUA_REGISTRYINDEX, l_netEventCbRef);
	l_netEventCbRef = luaL_ref(L, LUA_REGISTRYINDEX);
	PostEventCallback = &l_netEventPostCallback;
	shok_GetGuiManager()->HackPostEvent();
	return 0;
}
int l_netEventUnSetHook(lua_State* L) {
	luaL_unref(L, LUA_REGISTRYINDEX, l_netEventCbRef);
	PostEventCallback = nullptr;
	return 0;
}


void l_logic_init(lua_State* L)
{
	luaext_registerFunc(L, "GetDamageFactor", l_logicGetDamageModifier);
	luaext_registerFunc(L, "SetDamageFactor", l_logicSetDamageModifier);
	luaext_registerFunc(L, "ReloadCutscene", &l_logicReloadCutscene);
	luaext_registerFunc(L, "GetAnimIdFromName", &l_logicGetAnimIdFromName);
	luaext_registerFunc(L, "PlayerGetPaydayStartetTick", &l_playerGetPaydayStatus);
	luaext_registerFunc(L, "PlayerSetPaydayStartetTick", &l_playerSetPaydayStatus);
	luaext_registerFunc(L, "NetEventSetCallback", &l_netEventSetHook);
	luaext_registerFunc(L, "NetEventUnSetCallback", &l_netEventUnSetHook);
}

// CppLogic.Logic.NetEventSetCallback(function(id) LuaDebugger.Log(id) return true end)

#include "pch.h"
#include "l_effect.h"
#include "pch.h"
#include "s5data.h"
#include "luaext.h"

void l_effectFlyingEffectOnHitCallback(shok_EGL_CFlyingEffect* eff) {
	int id = eff->EffectID;
	lua_State* L = *shok_luastate_game;
	int top = lua_gettop(L);
	luaL_checkstack(L, 5, "");
	lua_pushlightuserdata(L, &l_effect_init);
	lua_rawget(L, LUA_REGISTRYINDEX);
	lua_rawgeti(L, -1, id);
	if (lua_isfunction(L, -1)) {
		lua_pushnumber(L, id);
		lua_pcall(L, 1, 0, 0);
	}
	lua_settop(L, top + 1);
	lua_pushnil(L);
	lua_rawseti(L, -2, id);
	lua_settop(L, top);
}

int l_effect_createProjectile(lua_State* L) { // (effecttype, startx, starty, tarx, tary, dmg, radius, tarid, attid, playerid, dmgclass, callback, source)
	shok_CProjectileEffectCreator data = shok_CProjectileEffectCreator();
	data.EffectType = luaL_checkint(L, 1);
	data.CurrentPos.X = data.StartPos.X = luaL_checkfloat(L, 2);
	data.CurrentPos.Y = data.StartPos.Y = luaL_checkfloat(L, 3);
	data.TargetPos.X = luaL_checkfloat(L, 4);
	data.TargetPos.Y = luaL_checkfloat(L, 5);
	data.Damage = luaL_optint(L, 6, 0);
	data.DamageRadius = luaL_optfloat(L, 7, -1);
	data.TargetID = luaext_optEntityId(L, 8);
	data.AttackerID = luaext_optEntityId(L, 9);
	int player = luaL_optint(L, 10, 0);
	data.PlayerID = player;
	data.SourcePlayer = player;
	int dmgclass = luaL_optint(L, 11, 0);
	data.DamageClass = dmgclass;
	data.AdvancedDamageSourceOverride = luaL_optint(L, 13, static_cast<int>(AdvancedDealDamageSource::Script));
	shok_EGL_CGLEGameLogic* gl = *shok_EGL_CGLEGameLogic::GlobalObj;
	int id = gl->CreateEffect(&data);
	shok_EGL_CEffect* ef = (*shok_EGL_CGLEEffectManager::GlobalObj)->GetEffectById(id);
	if (!shok_GGL_CCannonBallEffect::FixDamageClass)
		if (shok_GGL_CCannonBallEffect* cbeff = shok_DynamicCast<shok_EGL_CEffect, shok_GGL_CCannonBallEffect>(ef))
			cbeff->DamageClass = dmgclass;
	if (lua_isfunction(L, 12)) {
		shok_EGL_CFlyingEffect::HookOnHit();
		shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback = &l_effectFlyingEffectOnHitCallback;
		lua_pushlightuserdata(L, &l_effect_init);
		lua_rawget(L, LUA_REGISTRYINDEX);
		lua_pushvalue(L, 12);
		lua_rawseti(L, -2, id);
	}
	lua_pushnumber(L, id);
	return 1;
}

int l_effect_isValid(lua_State* L) {
	int id = luaL_checkint(L, 1);
	bool r = (*shok_EGL_CGLEEffectManager::GlobalObj)->IsEffectValid(id);
	lua_pushboolean(L, r);
	return 1;
}

int l_effect_getCbs(lua_State* L) {
	lua_pushlightuserdata(L, &l_effect_init);
	lua_rawget(L, LUA_REGISTRYINDEX);
	return 1;
}

void l_effect_init(lua_State* L)
{
	luaext_registerFunc(L, "CreateProjectile", &l_effect_createProjectile);
	luaext_registerFunc(L, "IsValidEffect", &l_effect_isValid);
	luaext_registerFunc(L, "GetProjectileCallbacks", &l_effect_getCbs);

	lua_pushlightuserdata(L, &l_effect_init);
	lua_newtable(L);
	lua_rawset(L, LUA_REGISTRYINDEX);
}

void l_effect_cleanup(lua_State* L) {
	shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback = nullptr;
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, x-10000, y, x, y, 500, 1000, 0, 0, 1, 0, LuaDebugger.Log)
// local id = e(); local p = GetPosition(id); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, p.X-1000, p.Y, p.X, p.Y, 100, 1000, 0, 0, 1, 0)
// CppLogic.Effect.IsValidEffect(effid)

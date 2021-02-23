#include "pch.h"
#include "l_effect.h"
#include "pch.h"
#include "s5data.h"
#include "luaext.h"

int l_effect_createProjectile(lua_State* L) { // (effecttype, startx, starty, tarx, tary, dmg, radius, tarid, attid, playerid)
	shok_effectCreatorData data = shok_effectCreatorData();
	data.CreatorType = shok_effectCreatorData_CreatorType_Projectile;
	data.EffectType = luaL_checkint(L, 1);
	data.CurrentPos.X = data.StartPos.X = luaL_checkfloat(L, 2);
	data.CurrentPos.Y = data.StartPos.Y = luaL_checkfloat(L, 3);
	data.TargetPos.X = luaL_checkfloat(L, 4);
	data.TargetPos.Y = luaL_checkfloat(L, 5);
	data.Damage = luaL_optint(L, 6, 0);
	data.DamageRadius = luaL_optfloat(L, 7, -1);
	data.TargetID = luaext_optEntityId(L, 8);
	data.AttackerID = luaext_optEntityId(L, 9);
	data.PlayerID = luaL_optint(L, 10, 0);
	shok_EGL_CGLEGameLogic* gl = *shok_EGL_CGLEGameLogicObject;
	shok_vtable_EGL_CGLEGameLogic* vt = (shok_vtable_EGL_CGLEGameLogic*)gl->vtable;
	int id = vt->CreateEffect(gl, &data);
	lua_pushnumber(L, id);
	return 1;
}

int l_effect_isValid(lua_State* L) {
	int id = luaL_checkint(L, 1);
	bool r = (*shok_EGL_CGLEEffectManagerObject)->IsEffectValid(id);
	lua_pushboolean(L, r);
	return 1;
}

void l_effect_init(lua_State* L)
{
	luaext_registerFunc(L, "CreateProjectile", &l_effect_createProjectile);
	luaext_registerFunc(L, "IsValidEffect", &l_effect_isValid);
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); effid = CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, x, y, x+1000, y, 500, 1000, 0, 0, 2)
// CppLogic.Effect.IsValidEffect(effid)

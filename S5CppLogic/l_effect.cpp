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
	int player = luaL_optint(L, 10, 0);
	data.PlayerID = player;
	shok_EGL_CGLEGameLogic* gl = *shok_EGL_CGLEGameLogicObject;
	int id = gl->CreateEffect(&data);
	lua_pushnumber(L, id);
	shok_EGL_CEffect* ef = (*shok_EGL_CGLEEffectManagerObject)->GetEffectById(id);
	if (ef->IsCannonBallEffect()) {
		shok_GGL_CCannonBallEffect* cbeff = (shok_GGL_CCannonBallEffect*)ef;
		cbeff->SourcePlayer = player;
		cbeff->DamageClass = luaL_optint(L, 11, 0);
	}
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

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, x, y, x+1000, y, 500, 1000, 0, 0, 1, 0)
// CppLogic.Effect.IsValidEffect(effid)

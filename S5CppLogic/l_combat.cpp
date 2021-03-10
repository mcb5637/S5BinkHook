#include "pch.h"
#include "l_combat.h"
#include "s5data.h"
#include "luaext.h"
#include "l_entity.h"

int l_combat_dealDamage(lua_State* L) {
	shok_EGL_CGLEEntity* targ = luaext_checkEntity(L, 1);
	int dmg = luaL_checkint(L, 2);
	shok_EGL_CGLEEntity* att = luaext_optEntity(L, 3);
	shok_entityHurtEntity(att, targ, dmg);
	return 0;
}

int l_combat_dealAOEDamage(lua_State* L) {
	shok_EGL_CGLEEntity* source = luaext_optEntity(L, 1);
	shok_position pos = shok_position();
	pos.X = luaL_checkfloat(L, 2);
	pos.Y = luaL_checkfloat(L, 3);
	float range = luaL_checkfloat(L, 4);
	int dmg = luaL_checkint(L, 5);
	int player = luaL_optint(L, 6, 0);
	int dmgclass = luaL_optint(L, 7, 0);
	shok_entityDealAOEDamage(source, &pos, range, dmg, player, dmgclass);
	return 0;
}

void l_combat_init(lua_State* L)
{
	luaext_registerFunc(L, "DealDamage", &l_combat_dealDamage);
	luaext_registerFunc(L, "DealAoEDamage", &l_combat_dealAOEDamage);
}

// CppLogic.Combat.DealDamage(GUI.GetEntityAtPosition(GUI.GetMousePosition()), 100)
// local id = e(); local p = GetPosition(id); CppLogic.Combat.DealAoEDamage(0, p.X, p.Y, 1000, 100, 0, 0)

// suspected deal aoe damage; 0049F82A (attacker obj, targetid, range, *pos?, player, damage)
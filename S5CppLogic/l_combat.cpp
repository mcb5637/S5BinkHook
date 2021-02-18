#include "pch.h"
#include "l_combat.h"
#include "s5data.h"
#include "luaext.h"

int l_combat_dealDamage(lua_State* L) {
	shok_EGL_CGLEEntity* targ = luaext_checkEntity(L, 1);
	int dmg = luaL_checkint(L, 2);
	shok_EGL_CGLEEntity* att = luaext_optEntity(L, 3);
	shok_entityHurtEntity(att, targ, dmg);
	return 0;
}

void l_combat_init(lua_State* L)
{
	luaext_registerFunc(L, "DealDamage", &l_combat_dealDamage);
}

// CppLogic.Combat.DealDamage(GUI.GetEntityAtPosition(GUI.GetMousePosition()), 100)

// suspected deal aoe damage; 0049F82A (attacker obj, targetid, range, *pos?, player, damage)
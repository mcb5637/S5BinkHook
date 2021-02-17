#include "pch.h"
#include "l_combat.h"
#include "s5data.h"
#include "luaext.h"

int l_combat_dealDamage(lua_State* L) {
	int target = luaL_checkint(L, 1);
	int dmg = luaL_checkint(L, 2);
	int attacker = luaL_optint(L, 3, 0);
	shok_EGL_CGLEEntity* targ = shok_eid2obj(target);
	if (targ == nullptr)
		return luaL_error(L, "invalid target");
	shok_entityHurtEntity(shok_eid2obj(attacker), targ, dmg);
	return 0;
}

void l_combat_init(lua_State* L)
{
	luaext_registerFunc(L, "DealDamage", &l_combat_dealDamage);
}

// CppLogic.Combat.DealDamage(GUI.GetEntityAtPosition(GUI.GetMousePosition()), 100)

// suspected deal aoe damage; 0049F82A (player, targetid, *pos, var14, ebx-0)
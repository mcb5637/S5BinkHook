#include "pch.h"
#include "l_entitytype.h"
#include "pch.h"
#include "luaext.h"

int l_leaderTyGetSoldierType(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLeaderBehaviorProps* p = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, p, "no leader type at 1");
	lua_pushnumber(L, p->SoldierType);
	return 1;
}

int l_leaderTySetSoldierType(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLeaderBehaviorProps* p = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, p, "no leader type at 1");
	luaext_assertPointer(L, luaext_checkEntityType(L, 2)->GetSoldierBehaviorProp(), "no soldier type at 2");
	p->SoldierType = luaL_checkint(L, 2);
	return 0;
}

void l_entitytype_init(lua_State* L)
{
	lua_pushstring(L, "Settler");
	lua_newtable(L);
	luaext_registerFunc(L, "LeaderTypeGetSoldierType", &l_leaderTyGetSoldierType);
	luaext_registerFunc(L, "LeaderTypeSetSoldierType", &l_leaderTySetSoldierType);
	lua_rawset(L, -3);
}

// CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(Entities.PU_LeaderSword1)

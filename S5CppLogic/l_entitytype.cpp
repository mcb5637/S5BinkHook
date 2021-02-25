#include "pch.h"
#include "l_entitytype.h"
#include "pch.h"
#include "luaext.h"

int l_leaderTyGetSoldierType(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	shok_GGL_CLeaderBehaviorProps* p = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, p, "no leader type at 1");
	lua_pushnumber(L, p->SoldierType);
	return 1;
}

int l_leaderTySetSoldierType(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	shok_GGL_CLeaderBehaviorProps* p = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, p, "no leader type at 1");
	shok_GGlue_CGlueEntityProps* t2 = luaext_checkEntityType(L, 2);
	luaext_assert(L, t2->IsSettlerType(), "no settler type at 2");
	luaext_assertPointer(L, t2->GetSoldierBehaviorProp(), "no soldier type at 2");
	p->SoldierType = luaL_checkint(L, 2);
	return 0;
}

int l_entityTyGetMaxRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* p = t->GetBattleBehaviorProp();
	luaext_assertPointer(L, p, "no combat type at 1");
	lua_pushnumber(L, p->MaxRange);
	return 1;
}

int l_entityTySetMaxRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* p = t->GetBattleBehaviorProp();
	luaext_assertPointer(L, p, "no combat type at 1");
	p->MaxRange = luaL_checkfloat(L, 2);
	return 0;
}

void l_entitytype_init(lua_State* L)
{
	luaext_registerFunc(L, "GetMaxRange", &l_entityTyGetMaxRange);
	luaext_registerFunc(L, "SetMaxRange", &l_entityTySetMaxRange);

	lua_pushstring(L, "Settler");
	lua_newtable(L);
	luaext_registerFunc(L, "LeaderTypeGetSoldierType", &l_leaderTyGetSoldierType);
	luaext_registerFunc(L, "LeaderTypeSetSoldierType", &l_leaderTySetSoldierType);
	lua_rawset(L, -3);
}

// CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(Entities.PU_LeaderSword1)

#include "pch.h"
#include "l_tech.h"
#include "luaext.h"


int l_techGetResearchInfo(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	lua_pushnumber(L, tech->TimeToResearch);
	luaext_writeCostInfo(L, tech->ResourceCosts);
	return 2;
}
int l_techSetResearchInfo(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	if (lua_isnumber(L, 2))
		tech->TimeToResearch = luaL_checkfloat(L, 2);
	bool ig = false;
	if (lua_isboolean(L, 4))
		ig = lua_toboolean(L, 4);
	if (lua_istable(L, 3))
		luaext_readCostInfo(L, 3, tech->ResourceCosts, ig);
	return 0;
}

int l_techGetRequirements(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	lua_pushnumber(L, tech->RequiredEntityConditions);
	lua_newtable(L);
	for (shok_technologyRequirementEType ec : tech->EntityConditions) {
		lua_pushnumber(L, ec.EntityType);
		lua_pushnumber(L, ec.Amount);
		lua_settable(L, -3);
	}
	lua_pushnumber(L, tech->RequiredTecConditions);
	lua_newtable(L);
	int i = 1;
	for (shok_technologyRequirementTech ec : tech->TecConditions) {
		lua_pushnumber(L, ec.TecType);
		lua_rawseti(L, -2, i);
		i++;
	}
	lua_pushnumber(L, tech->RequiredUpgradeCategoryConditions);
	lua_newtable(L);
	for (shok_technologyRequirementUCat ec : tech->UpgradeCategoryConditions) {
		lua_pushnumber(L, ec.UpgradeCategory);
		lua_pushnumber(L, ec.Amount);
		lua_settable(L, -3);
	}
	return 6;
}

void inline l_techPushMod(lua_State* L, shok_technologyModifier& mod) {
	lua_pushnumber(L, mod.Operator);
	lua_pushnumber(L, mod.Value);
}

int l_techGetExpl(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	l_techPushMod(L, tech->ExplorationModifier);
	return 2;
}
int l_techGetArmor(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	l_techPushMod(L, tech->ArmorModifier);
	return 2;
}
int l_techGetDamage(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	l_techPushMod(L, tech->DamageModifier);
	return 2;
}
int l_techGetRange(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	l_techPushMod(L, tech->RangeModifier);
	return 2;
}
int l_techGetSpeed(lua_State* L) {
	int tid = luaL_checkint(L, 1);
	shok_technology* tech = (*shok_GGL_CGLGameLogicObj)->GetTech(tid);
	luaext_assertPointer(L, tech, "no tech at 1");
	l_techPushMod(L, tech->SpeedModifier);
	return 2;
}

void l_tech_init(lua_State* L)
{
	luaext_registerFunc(L, "GetResearchInfo", l_techGetResearchInfo);
	luaext_registerFunc(L, "SetResearchInfo", l_techSetResearchInfo);
	luaext_registerFunc(L, "GetRequirements", l_techGetRequirements);
	luaext_registerFunc(L, "GetExplorationModifier", l_techGetExpl);
	luaext_registerFunc(L, "GetArmorModifier", l_techGetArmor);
	luaext_registerFunc(L, "GetDamageModifier", l_techGetDamage);
	luaext_registerFunc(L, "GetRangeModifier", l_techGetRange);
	luaext_registerFunc(L, "GetSpeedModifier", l_techGetSpeed);
}

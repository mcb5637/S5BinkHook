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

int l_entityTyGetLimitedLifespanDur(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLimitedLifespanBehaviorProps* l = t->GetLimitedLifespanBehaviorProp();
	luaext_assertPointer(L, l, "no limited lifespan entitytype at 1");
	lua_pushnumber(L, l->LifespanSeconds);
	return 1;
}
int l_entityTySetLimitedLifespanDur(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLimitedLifespanBehaviorProps* l = t->GetLimitedLifespanBehaviorProp();
	luaext_assertPointer(L, l, "no limited lifespan entitytype at 1");
	l->LifespanSeconds = luaL_checkint(L, 2);
	return 0;
}

int l_leaderTyGetUpkeep(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	shok_GGL_CLeaderBehaviorProps* p = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, p, "no leader type at 1");
	lua_pushnumber(L, p->UpkeepCosts);
	return 1;
}
int l_leaderTySetUpkeep(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	shok_GGL_CLeaderBehaviorProps* p = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, p, "no leader type at 1");
	p->UpkeepCosts = luaL_checkfloat(L, 2);
	return 0;
}

int l_entityTyGetMaxHealth(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	lua_pushnumber(L, t->LogicProps->MaxHealth);
	return 1;
}
int l_entityTySetMaxHealth(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	t->LogicProps->MaxHealth = luaL_checkint(L, 2);
	return 0;
}

int l_buildingTyGetSabotageFactor(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsBuildingType(), "no building type at 1");
	lua_pushnumber(L, ((shok_GGL_CGLBuildingProps*)t->LogicProps)->KegEffectFactor);
	return 1;
}
int l_buildingTySetSabotageFactor(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsBuildingType(), "no building type at 1");
	((shok_GGL_CGLBuildingProps*) t->LogicProps)->KegEffectFactor = luaL_checkfloat(L, 2);
	return 0;
}

int l_entityTyGetAffectMotivation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CAffectMotivationBehaviorProps* a = t->GetAffectMotivationBehaviorProp();
	luaext_assertPointer(L, a, "no affect motivation entitytype at 1");
	lua_pushnumber(L, a->MotivationEffect);
	return 1;
}
int l_entityTySetAffectMotivation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CAffectMotivationBehaviorProps* a = t->GetAffectMotivationBehaviorProp();
	luaext_assertPointer(L, a, "no affect motivation entitytype at 1");
	a->MotivationEffect = luaL_checkfloat(L, 2);
	return 0;
}

int l_entityTyGetSuspendedAnimation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLAnimationBehaviorExProps* a = t->GetAnimationExProp();
	luaext_assertPointer(L, a, "no animated entitytype at 1");
	lua_pushnumber(L, a->SuspensionAnimation);
	return 1;
}
int l_entityTySetSuspendedAnimation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLAnimationBehaviorExProps* a = t->GetAnimationExProp();
	luaext_assertPointer(L, a, "no animated entitytype at 1");
	a->SuspensionAnimation = luaL_checkint(L, 2);
	return 0;
}

int l_leaderTyGetRegen(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLeaderBehaviorProps* l = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, l, "no leader entitytype at 1");
	lua_pushnumber(L, l->HealingPoints);
	lua_pushnumber(L, l->HealingSeconds);
	return 2;
}
int l_leaderTySetRegen(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLeaderBehaviorProps* l = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, l, "no leader entitytype at 1");
	if (lua_isnumber(L, 2))
		l->HealingPoints = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		l->HealingSeconds = luaL_checkint(L, 3);
	return 0;
}

int l_entityTyGetAutoAttackDamage(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* b = t->GetBattleBehaviorProp();
	if (b != nullptr) {
		lua_pushnumber(L, b->DamageAmount);
		lua_pushnumber(L, b->DamageClass);
		return 2;
	}
	shok_GGL_CAutoCannonBehaviorProps* a = t->GetAutoCannonProp();
	if (a != nullptr) {
		lua_pushnumber(L, a->DamageAmount);
		lua_pushnumber(L, a->DamageClass);
		return 2;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}
int l_entityTySetAutoAttackDamage(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* b = t->GetBattleBehaviorProp();
	if (b != nullptr) {
		b->DamageAmount = luaL_checkint(L, 2);
		b->DamageClass = luaL_checkint(L, 3);
		return 0;
	}
	shok_GGL_CAutoCannonBehaviorProps* a = t->GetAutoCannonProp();
	if (a != nullptr) {
		a->DamageAmount = luaL_checkint(L, 2);
		a->DamageClass = luaL_checkint(L, 3);
		return 0;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}

int l_entityTypeAddEntityCat(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	int cat = luaL_checkint(L, 2);
	luaext_assert(L, !t->IsOfCategory(cat), "entitytype is already of this cat");
	shok_saveVector<int>(&t->LogicProps->Categories, [cat](auto& v) {
		v.push_back(cat);
		});
	return 0;
}
int l_entityTypeRemoveEntityCat(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	int cat = luaL_checkint(L, 2);
	shok_saveVector<int>(&t->LogicProps->Categories, [cat](auto& v) {
			auto e = std::remove_if(v.begin(), v.end(), [cat](int c) { return c == cat; });
			v.erase(e, v.end());
		});
	return 0;
}
int l_entityTypeDump(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	int s = t->LogicProps->Categories.size();
	luaL_checkstack(L, s + 5, "");
	int* d = (int*)&t->LogicProps->Categories;
	for (int i = 0; i < 4; i++)
		lua_pushnumber(L, d[i]);
	lua_pushnumber(L, s);
	for (int i : t->LogicProps->Categories) {
		lua_pushnumber(L, i);
	}
	return s + 5;
}

int l_entityTyGetAutoAttackMissChance(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* b = t->GetBattleBehaviorProp();
	luaext_assertPointer(L, b, "no battle entity at 1");
	lua_pushnumber(L, b->MissChance);
	return 1;
}
int l_entityTySetAutoAttackMissChance(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* b = t->GetBattleBehaviorProp();
	luaext_assertPointer(L, b, "no battle entity at 1");
	b->MissChance = luaL_checkint(L, 2);
	return 0;
}

int l_entityTyGetRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* b = t->GetBattleBehaviorProp();
	if (b != nullptr) {
		lua_pushnumber(L, b->MaxRange);
		lua_pushnumber(L, b->MinRange);
		return 2;
	}
	shok_GGL_CAutoCannonBehaviorProps* a = t->GetAutoCannonProp();
	if (a != nullptr) {
		lua_pushnumber(L, a->MaxAttackRange);
		return 1;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}
int l_entityTySetRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CBattleBehaviorProps* b = t->GetBattleBehaviorProp();
	if (b != nullptr) {
		if (lua_isnumber(L, 2))
			b->MaxRange = luaL_checkfloat(L, 2);
		if (lua_isnumber(L, 3))
			b->MinRange = luaL_checkfloat(L, 3);
		return 0;
	}
	shok_GGL_CAutoCannonBehaviorProps* a = t->GetAutoCannonProp();
	if (a != nullptr) {
		if (lua_isnumber(L, 2))
			a->MaxAttackRange = luaL_checkfloat(L, 2);
		return 0;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}

int l_leaderTyGetAutoAggressiveRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLeaderBehaviorProps* b = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, b, "no battle entity at 1");
	lua_pushnumber(L, b->AutoAttackRange);
	return 1;
}
int l_leaderTySetAutoAggressiveRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CLeaderBehaviorProps* b = t->GetLeaderBehaviorProp();
	luaext_assertPointer(L, b, "no battle entity at 1");
	b->AutoAttackRange = luaL_checkfloat(L, 2);
	return 0;
}

int l_buildingTyGetVCAttractionPlaces(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsBuildingType(), "no building type at 1");
	lua_pushnumber(L, ((shok_GGL_CGLBuildingProps*)t->LogicProps)->NumberOfAttractableSettlers);
	return 1;
}
int l_buildingTySetVCAttractionPlaces(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsBuildingType(), "no building type at 1");
	((shok_GGL_CGLBuildingProps*)t->LogicProps)->NumberOfAttractableSettlers = luaL_checkint(L, 2);
	return 0;
}

int l_settlerTyGetThiefStealingInfo(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CThiefBehaviorProperties* p = t->GetThiefBehaviorProp();
	luaext_assertPointer(L, p, "no thief entity at 1");
	lua_pushnumber(L, p->SecondsNeededToSteal);
	lua_pushnumber(L, p->MaximumAmountToSteal);
	lua_pushnumber(L, p->MinimumAmountToSteal);
	return 3;
}
int l_settlerTySetThiefStealingInfo(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CThiefBehaviorProperties* p = t->GetThiefBehaviorProp();
	luaext_assertPointer(L, p, "no thief entity at 1");
	if (lua_isnumber(L, 2))
		p->SecondsNeededToSteal = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		p->MaximumAmountToSteal = luaL_checkint(L, 3);
	if (lua_isnumber(L, 4))
		p->MinimumAmountToSteal = luaL_checkint(L, 4);
	return 0;
}

int l_settlerTyGetCamouflageAbilityData(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CCamouflageBehaviorProps* p = t->GetCamouflageBehaviorProp();
	luaext_assertPointer(L, p, "no camoflague entity at 1");
	lua_pushnumber(L, p->DurationSeconds);
	lua_pushnumber(L, p->DiscoveryRange);
	lua_pushnumber(L, p->RechargeTimeSeconds);
	return 3;
}
int l_settlerTySetCamouflageAbilityData(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CCamouflageBehaviorProps* p = t->GetCamouflageBehaviorProp();
	luaext_assertPointer(L, p, "no camoflague entity at 1");
	if (lua_isnumber(L, 2))
		p->DurationSeconds = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		p->DiscoveryRange = luaL_checkfloat(L, 3);
	if (lua_isnumber(L, 4))
		p->RechargeTimeSeconds = luaL_checkint(L, 4);
	return 0;
}

int l_entityTyGetArmor(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	if (t->IsSettlerType()) {
		shok_GGL_CGLSettlerProps* s = (shok_GGL_CGLSettlerProps*)t->LogicProps;
		lua_pushnumber(L, s->ArmorAmount);
		lua_pushnumber(L, s->ArmorClass);
		return 2;
	}
	if (t->IsBuildingType()) {
		shok_GGL_CGLBuildingProps* s = (shok_GGL_CGLBuildingProps*)t->LogicProps;
		lua_pushnumber(L, s->ArmorAmount);
		lua_pushnumber(L, s->ArmorClass);
		return 2;
	}
	return luaL_error(L, "no settler or building entity type at 1");
}
int l_entityTySetArmor(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	if (t->IsSettlerType()) {
		shok_GGL_CGLSettlerProps* s = (shok_GGL_CGLSettlerProps*)t->LogicProps;
		if (lua_isnumber(L, 2))
			s->ArmorAmount = luaL_checkint(L, 2);
		if (lua_isnumber(L, 3))
			s->ArmorClass = luaL_checkint(L, 3);
		return 0;
	}
	if (t->IsBuildingType()) {
		shok_GGL_CGLBuildingProps* s = (shok_GGL_CGLBuildingProps*)t->LogicProps;
		if (lua_isnumber(L, 2))
			s->ArmorAmount = luaL_checkint(L, 2);
		if (lua_isnumber(L, 3))
			s->ArmorClass = luaL_checkint(L, 3);
		return 0;
	}
	return luaL_error(L, "no settler or building entity type at 1");
}

int l_entityTyGetModels(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	for (int i = 0; i < 4; i++)
		lua_pushnumber(L, t->DisplayProps->Model[i]);
	return 4;
}
int l_entityTySetModels(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	for (int i = 0; i < 4; i++)
		if (lua_isnumber(L, 1 + i))
			t->DisplayProps->Model[i] = luaL_checkint(L, 1 + i);
	return 0;
}

void l_entitytype_init(lua_State* L)
{
	luaext_registerFunc(L, "GetLimitedLifespanDuration", &l_entityTyGetLimitedLifespanDur);
	luaext_registerFunc(L, "SetLimitedLifespanDuration", &l_entityTySetLimitedLifespanDur);
	luaext_registerFunc(L, "GetMaxHealth", &l_entityTyGetMaxHealth);
	luaext_registerFunc(L, "SetMaxHealth", &l_entityTySetMaxHealth);
	luaext_registerFunc(L, "GetAffectMotivation", &l_entityTyGetAffectMotivation);
	luaext_registerFunc(L, "SetAffectMotivation", &l_entityTySetAffectMotivation);
	luaext_registerFunc(L, "GetSuspendedAnimation", &l_entityTyGetSuspendedAnimation);
	luaext_registerFunc(L, "SetSuspendedAnimation", &l_entityTySetSuspendedAnimation);
	luaext_registerFunc(L, "GetAutoAttackDamage", &l_entityTyGetAutoAttackDamage);
	luaext_registerFunc(L, "SetAutoAttackDamage", &l_entityTySetAutoAttackDamage);
	luaext_registerFunc(L, "AddEntityCategory", &l_entityTypeAddEntityCat);
	luaext_registerFunc(L, "RemoveEntityCategory", &l_entityTypeRemoveEntityCat);
	luaext_registerFunc(L, "DumpCategories", &l_entityTypeDump);
	luaext_registerFunc(L, "GetAutoAttackMissChance", &l_entityTyGetAutoAttackMissChance);
	luaext_registerFunc(L, "SetAutoAttackMissChance", &l_entityTySetAutoAttackMissChance);
	luaext_registerFunc(L, "GetAutoAttackRange", &l_entityTyGetRange);
	luaext_registerFunc(L, "SetAutoAttackRange", &l_entityTySetRange);
	luaext_registerFunc(L, "GetArmor", &l_entityTyGetArmor);
	luaext_registerFunc(L, "SetArmor", &l_entityTySetArmor);
	luaext_registerFunc(L, "GetModels", &l_entityTyGetModels);
	luaext_registerFunc(L, "SetModels", &l_entityTySetModels);

	lua_pushstring(L, "Settler");
	lua_newtable(L);
	luaext_registerFunc(L, "LeaderTypeGetSoldierType", &l_leaderTyGetSoldierType);
	luaext_registerFunc(L, "LeaderTypeSetSoldierType", &l_leaderTySetSoldierType);
	luaext_registerFunc(L, "LeaderTypeGetUpkeep", &l_leaderTyGetUpkeep);
	luaext_registerFunc(L, "LeaderTypeSetUpkeep", &l_leaderTySetUpkeep);
	luaext_registerFunc(L, "LeaderTypeGetRegeneration", &l_leaderTyGetRegen);
	luaext_registerFunc(L, "LeaderTypeSetRegeneration", &l_leaderTySetRegen);
	luaext_registerFunc(L, "LeaderTypeGetAutoAggressiveRange", &l_leaderTyGetAutoAggressiveRange);
	luaext_registerFunc(L, "LeaderTypeSetAutoAggressiveRange", &l_leaderTySetAutoAggressiveRange);
	luaext_registerFunc(L, "ThiefTypeGetStealingInfo", &l_settlerTyGetThiefStealingInfo);
	luaext_registerFunc(L, "ThiefTypeSetStealingInfo", &l_settlerTySetThiefStealingInfo);
	luaext_registerFunc(L, "GetAbilityDataCamouflage", &l_settlerTyGetCamouflageAbilityData);
	luaext_registerFunc(L, "SetAbilityDataCamouflage", &l_settlerTySetCamouflageAbilityData);
	lua_rawset(L, -3);

	lua_pushstring(L, "Building");
	lua_newtable(L);
	luaext_registerFunc(L, "GetSabotageFactor", &l_buildingTyGetSabotageFactor);
	luaext_registerFunc(L, "SetSabotageFactor", &l_buildingTySetSabotageFactor);
	luaext_registerFunc(L, "GetVCAttractionSlotsProvided", &l_buildingTyGetVCAttractionPlaces);
	luaext_registerFunc(L, "SetVCAttractionSlotsProvided", &l_buildingTySetVCAttractionPlaces);
	lua_rawset(L, -3);
}

// CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(Entities.PU_LeaderSword1)
// Logic.IsEntityInCategory(GUI.GetSelectedEntity(), 100)
// CppLogic.EntityType.AddEntityCategory(Entities.PU_LeaderSword1, 100)
// CppLogic.EntityType.GetAutoAttackMissChance(Entities.PU_LeaderBow1)
// CppLogic.EntityType.Settler.GetAbilityDataCamouflage(Entities.PU_Hero5)
// CppLogic.EntityType.GetArmor(Entities.PU_LeaderSword1)

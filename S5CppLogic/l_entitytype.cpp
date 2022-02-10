#include "pch.h"
#include "l_entitytype.h"
#include "pch.h"
#include "luaext.h"

int l_leaderTyGetSoldierType(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, p, "no leader type at 1");
	lua_pushnumber(L, p->SoldierType);
	return 1;
}
int l_leaderTySetSoldierType(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, p, "no leader type at 1");
	shok_GGlue_CGlueEntityProps* t2 = luaext_checkEntityType(L, 2);
	luaext_assert(L, t2->IsSettlerType(), "no settler type at 2");
	luaext_assertPointer(L, t2->GetBehaviorProps<GGL::CSoldierBehaviorProps>(), "no soldier type at 2");
	p->SoldierType = luaL_checkint(L, 2);
	return 0;
}

int l_entityTyGetLimitedLifespanDur(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CLimitedLifespanBehaviorProps* l = t->GetBehaviorProps<GGL::CLimitedLifespanBehaviorProps>();
	luaext_assertPointer(L, l, "no limited lifespan entitytype at 1");
	lua_pushnumber(L, l->LifespanSeconds);
	return 1;
}
int l_entityTySetLimitedLifespanDur(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CLimitedLifespanBehaviorProps* l = t->GetBehaviorProps<GGL::CLimitedLifespanBehaviorProps>();
	luaext_assertPointer(L, l, "no limited lifespan entitytype at 1");
	l->LifespanSeconds = luaL_checkint(L, 2);
	return 0;
}

int l_leaderTyGetUpkeep(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, p, "no leader type at 1");
	lua_pushnumber(L, p->UpkeepCosts);
	return 1;
}
int l_leaderTySetUpkeep(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsSettlerType(), "no settler type at 1");
	GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
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
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	lua_pushnumber(L, bp->KegEffectFactor);
	return 1;
}
int l_buildingTySetSabotageFactor(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	bp->KegEffectFactor = luaL_checkfloat(L, 2);
	return 0;
}

int l_entityTyGetAffectMotivation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CAffectMotivationBehaviorProps* a = t->GetBehaviorProps<GGL::CAffectMotivationBehaviorProps>();
	luaext_assertPointer(L, a, "no affect motivation entitytype at 1");
	lua_pushnumber(L, a->MotivationEffect);
	return 1;
}
int l_entityTySetAffectMotivation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CAffectMotivationBehaviorProps* a = t->GetBehaviorProps<GGL::CAffectMotivationBehaviorProps>();
	luaext_assertPointer(L, a, "no affect motivation entitytype at 1");
	a->MotivationEffect = luaL_checkfloat(L, 2);
	return 0;
}

int l_entityTyGetSuspendedAnimation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CGLAnimationBehaviorExProps* a = t->GetBehaviorProps<GGL::CGLAnimationBehaviorExProps>();
	luaext_assertPointer(L, a, "no animated entitytype at 1");
	lua_pushnumber(L, a->SuspensionAnimation);
	return 1;
}
int l_entityTySetSuspendedAnimation(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CGLAnimationBehaviorExProps* a = t->GetBehaviorProps<GGL::CGLAnimationBehaviorExProps>();
	luaext_assertPointer(L, a, "no animated entitytype at 1");
	a->SuspensionAnimation = luaL_checkint(L, 2);
	return 0;
}

int l_leaderTyGetRegen(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CLeaderBehaviorProps* l = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, l, "no leader entitytype at 1");
	lua_pushnumber(L, l->HealingPoints);
	lua_pushnumber(L, l->HealingSeconds);
	return 2;
}
int l_leaderTySetRegen(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CLeaderBehaviorProps* l = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, l, "no leader entitytype at 1");
	if (lua_isnumber(L, 2))
		l->HealingPoints = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		l->HealingSeconds = luaL_checkint(L, 3);
	return 0;
}

int l_entityTyGetAutoAttackDamage(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	if (b != nullptr) {
		lua_pushnumber(L, b->DamageAmount);
		lua_pushnumber(L, b->DamageClass);
		return 2;
	}
	GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
	if (a != nullptr) {
		lua_pushnumber(L, a->DamageAmount);
		lua_pushnumber(L, a->DamageClass);
		return 2;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}
int l_entityTySetAutoAttackDamage(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	if (b != nullptr) {
		b->DamageAmount = luaL_checkint(L, 2);
		b->DamageClass = luaL_checkint(L, 3);
		return 0;
	}
	GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
	if (a != nullptr) {
		a->DamageAmount = luaL_checkint(L, 2);
		a->DamageClass = luaL_checkint(L, 3);
		return 0;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}

int l_entityTypeAddEntityCat(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_EntityCategory cat = static_cast<shok_EntityCategory>(luaL_checkint(L, 2));
	luaext_assert(L, !t->IsOfCategory(cat), "entitytype is already of this cat");
	auto v = t->LogicProps->Categories.SaveVector();
	v.Vector.push_back(cat);
	return 0;
}
int l_entityTypeRemoveEntityCat(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_EntityCategory cat = static_cast<shok_EntityCategory>(luaL_checkint(L, 2));
	auto v = t->LogicProps->Categories.SaveVector();
	auto e = std::remove_if(v.Vector.begin(), v.Vector.end(), [cat](shok_EntityCategory c) { return c == cat; });
	v.Vector.erase(e, v.Vector.end());
	return 0;
}

int l_entityTyGetAutoAttackMissChance(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	luaext_assertPointer(L, b, "no battle entity at 1");
	lua_pushnumber(L, b->MissChance);
	return 1;
}
int l_entityTySetAutoAttackMissChance(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	luaext_assertPointer(L, b, "no battle entity at 1");
	b->MissChance = luaL_checkint(L, 2);
	return 0;
}

int l_entityTyGetRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	if (b != nullptr) {
		lua_pushnumber(L, b->MaxRange);
		lua_pushnumber(L, b->MinRange);
		return 2;
	}
	GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
	if (a != nullptr) {
		lua_pushnumber(L, a->MaxAttackRange);
		return 1;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}
int l_entityTySetRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	if (b != nullptr) {
		if (lua_isnumber(L, 2))
			b->MaxRange = luaL_checkfloat(L, 2);
		if (lua_isnumber(L, 3))
			b->MinRange = luaL_checkfloat(L, 3);
		return 0;
	}
	GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
	if (a != nullptr) {
		if (lua_isnumber(L, 2))
			a->MaxAttackRange = luaL_checkfloat(L, 2);
		return 0;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}

int l_entityTyGetBattleWaitUntil(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	if (b != nullptr) {
		lua_pushnumber(L, b->BattleWaitUntil);
		return 1;
	}
	GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
	if (a != nullptr) {
		lua_pushnumber(L, a->ReloadTime);
		return 1;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}
int l_entityTySetBattleWaitUntil(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
	if (b != nullptr) {
		b->BattleWaitUntil = luaL_checkint(L, 2);
		return 0;
	}
	GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
	if (a != nullptr) {
		a->ReloadTime = luaL_checkint(L, 2);
		return 0;
	}
	return luaL_error(L, "no battle or autocannon entity type at 1");
}

int l_leaderTyGetAutoAggressiveRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CLeaderBehaviorProps* b = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, b, "no battle entity at 1");
	lua_pushnumber(L, b->AutoAttackRange);
	return 1;
}
int l_leaderTySetAutoAggressiveRange(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CLeaderBehaviorProps* b = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, b, "no battle entity at 1");
	b->AutoAttackRange = luaL_checkfloat(L, 2);
	return 0;
}

int l_buildingTyGetVCAttractionPlaces(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	lua_pushnumber(L, bp->NumberOfAttractableSettlers);
	return 1;
}
int l_buildingTySetVCAttractionPlaces(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	bp->NumberOfAttractableSettlers = luaL_checkint(L, 2);
	return 0;
}

int l_settlerTyGetThiefStealingInfo(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CThiefBehaviorProperties* p = t->GetBehaviorProps<GGL::CThiefBehaviorProperties>();
	luaext_assertPointer(L, p, "no thief entity at 1");
	lua_pushnumber(L, p->SecondsNeededToSteal);
	lua_pushnumber(L, p->MaximumAmountToSteal);
	lua_pushnumber(L, p->MinimumAmountToSteal);
	return 3;
}
int l_settlerTySetThiefStealingInfo(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CThiefBehaviorProperties* p = t->GetBehaviorProps<GGL::CThiefBehaviorProperties>();
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
	GGL::CCamouflageBehaviorProps* p = t->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
	luaext_assertPointer(L, p, "no camoflague entity at 1");
	lua_pushnumber(L, p->DurationSeconds);
	lua_pushnumber(L, p->DiscoveryRange);
	lua_pushnumber(L, p->RechargeTimeSeconds);
	return 3;
}
int l_settlerTySetCamouflageAbilityData(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CCamouflageBehaviorProps* p = t->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
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
	if (shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps)) {
		if (lua_isnumber(L, 2))
			s->ArmorAmount = luaL_checkint(L, 2);
		if (lua_isnumber(L, 3))
			s->ArmorClass = luaL_checkint(L, 3);
		return 0;
	}
	if (shok_GGL_CGLBuildingProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps)) {
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

int l_settlerTyGetAbilityDataCircularAttack(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CCircularAttackProps* p = t->GetBehaviorProps<GGL::CCircularAttackProps>();
	luaext_assertPointer(L, p, "no circularattack entity at 1");
	lua_pushnumber(L, p->Damage);
	lua_pushnumber(L, p->DamageClass);
	lua_pushnumber(L, p->Range);
	lua_pushnumber(L, p->RechargeTimeSeconds);
	return 4;
}
int l_settlerTySetAbilityDataCircularAttack(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CCircularAttackProps* p = t->GetBehaviorProps<GGL::CCircularAttackProps>();
	luaext_assertPointer(L, p, "no circularattack entity at 1");
	if (lua_isnumber(L, 2))
		p->Damage = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		p->DamageClass = luaL_checkint(L, 3);
	if (lua_isnumber(L, 4))
		p->Range = luaL_checkfloat(L, 4);
	if (lua_isnumber(L, 5))
		p->RechargeTimeSeconds = luaL_checkint(L, 5);
	return 0;
}

int l_settlerTyGetAbilityDataShuriken(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CShurikenAbilityProps* p = t->GetBehaviorProps<GGL::CShurikenAbilityProps>();
	luaext_assertPointer(L, p, "no shuriken entity at 1");
	lua_pushnumber(L, p->DamageAmount);
	lua_pushnumber(L, p->DamageClass);
	lua_pushnumber(L, p->NumberShuriken);
	lua_pushnumber(L, p->Range);
	lua_pushnumber(L, p->MaxArcDegree);
	lua_pushnumber(L, p->RechargeTimeSeconds);
	lua_pushnumber(L, p->ProjectileType);
	return 7;
}
int l_settlerTySetAbilityDataShuriken(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CShurikenAbilityProps* p = t->GetBehaviorProps<GGL::CShurikenAbilityProps>();
	luaext_assertPointer(L, p, "no shuriken entity at 1");
	if (lua_isnumber(L, 2))
		p->DamageAmount = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		p->DamageClass = luaL_checkint(L, 3);
	if (lua_isnumber(L, 4))
		p->NumberShuriken = luaL_checkint(L, 4);
	if (lua_isnumber(L, 5))
		p->Range = luaL_checkfloat(L, 5);
	if (lua_isnumber(L, 6))
		p->MaxArcDegree = luaL_checkfloat(L, 6);
	if (lua_isnumber(L, 7))
		p->RechargeTimeSeconds = luaL_checkint(L, 7);
	return 0;
}

int l_settlerTyGetAbilityDataSniper(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CSniperAbilityProps* p = t->GetBehaviorProps<GGL::CSniperAbilityProps>();
	luaext_assertPointer(L, p, "no sniper entity at 1");
	lua_pushnumber(L, p->DamageFactor);
	lua_pushnumber(L, p->Range);
	lua_pushnumber(L, p->RechargeTimeSeconds);
	return 3;
}
int l_settlerTySetAbilityDataSniper(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CSniperAbilityProps* p = t->GetBehaviorProps<GGL::CSniperAbilityProps>();
	luaext_assertPointer(L, p, "no sniper entity at 1");
	if (lua_isnumber(L, 2))
		p->DamageFactor = luaL_checkfloat(L, 2);
	if (lua_isnumber(L, 3))
		p->Range = luaL_checkfloat(L, 3);
	if (lua_isnumber(L, 4))
		p->RechargeTimeSeconds = luaL_checkint(L, 4);
	return 0;
}

int l_settlerTyGetAbilityDataRangedEffect(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CRangedEffectAbilityProps* p = t->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
	luaext_assertPointer(L, p, "no ranged effect entity at 1");
	lua_pushnumber(L, p->DamageFactor);
	lua_pushnumber(L, p->ArmorFactor);
	lua_pushnumber(L, p->HealthRecoveryFactor);
	lua_pushnumber(L, p->Range);
	lua_pushnumber(L, p->RechargeTimeSeconds);
	return 5;
}
int l_settlerTySetAbilityDataRangedEffect(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CRangedEffectAbilityProps* p = t->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
	luaext_assertPointer(L, p, "no ranged effect entity at 1");
	if (lua_isnumber(L, 2))
		p->DamageFactor = luaL_checkfloat(L, 2);
	if (lua_isnumber(L, 3))
		p->ArmorFactor = luaL_checkfloat(L, 3);
	if (lua_isnumber(L, 4))
		p->HealthRecoveryFactor = luaL_checkfloat(L, 4);
	if (lua_isnumber(L, 5))
		p->Range = luaL_checkfloat(L, 5);
	if (lua_isnumber(L, 6))
		p->RechargeTimeSeconds = luaL_checkint(L, 6);
	return 0;
}

int l_entityTyHasResourceTreeData(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CEntityProperties* l = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CEntityProperties>(t->LogicProps);
	lua_pushboolean(L, l != nullptr);
	return 1;
}
int l_entityTyGetResourceTreeData(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CEntityProperties* l = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CEntityProperties>(t->LogicProps);
	luaext_assertPointer(L, l, "no resource tree entity at 1");
	lua_pushnumber(L, l->ResourceEntity);
	lua_pushnumber(L, l->ResourceAmount);
	return 2;
}
int l_entityTySetResourceTreeData(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CEntityProperties* l = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CEntityProperties>(t->LogicProps);
	luaext_assertPointer(L, l, "no resource tree entity at 1");
	if (lua_isnumber(L, 2))
		l->ResourceEntity = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		l->ResourceAmount = luaL_checkint(L, 3);
	return 0;
}

int l_settlerTyGetFearless(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps);
	luaext_assertPointer(L, s, "no settler entity at 1");
	lua_pushboolean(L, s->Fearless);
	return 1;
}
int l_settlerTySetFearless(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps);
	luaext_assertPointer(L, s, "no settler entity at 1");
	luaL_checkany(L, 2);
	s->Fearless = lua_toboolean(L, 2);
	return 0;
}

int l_settlerTyGetCost(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps);
	luaext_assertPointer(L, s, "no settler entity at 1");
	luaext_writeCostInfo(L, s->Cost);
	return 1;
}
int l_settlerTySetCost(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps);
	luaext_assertPointer(L, s, "no settler entity at 1");
	bool ig = false;
	if (lua_isboolean(L, 3))
		ig = lua_toboolean(L, 3);
	luaext_readCostInfo(L, 2, s->Cost, ig);
	return 0;
}

int l_buildingGetCostConstr(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	luaext_writeCostInfo(L, bp->ConstructionInfo.Cost);
	return 1;
}
int l_buildingSetCostConstr(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	bool ig = false;
	if (lua_isboolean(L, 3))
		ig = lua_toboolean(L, 3);
	luaext_readCostInfo(L, 2, bp->ConstructionInfo.Cost, ig);
	return 0;
}

int l_buildingGetCostUpgr(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	luaext_writeCostInfo(L, bp->Upgrade.Cost);
	return 1;
}
int l_buildingSetCostUpgr(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLBuildingProps* bp = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps);
	luaext_assertPointer(L, bp, "no building type at 1");
	bool ig = false;
	if (lua_isboolean(L, 3))
		ig = lua_toboolean(L, 3);
	luaext_readCostInfo(L, 2, bp->Upgrade.Cost, ig);
	return 0;
}

int l_entityTypeGetBlocking(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	lua_newtable(L);
	int i = 1;
	for (const shok::AARect& r : t->LogicProps->BlockingArea) {
		lua_newtable(L);
		luaext_pushPos(L, r.low);
		lua_rawseti(L, -2, 1);
		luaext_pushPos(L, r.high);
		lua_rawseti(L, -2, 2);
		lua_rawseti(L, -2, i);
		i++;
	}
	lua_pushnumber(L, t->LogicProps->NumBlockedPoints);
	if (shok_GGL_CBuildBlockProperties* bb = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CBuildBlockProperties>(t->LogicProps)) {
		lua_newtable(L);
		luaext_pushPos(L, bb->BuildBlockArea.low);
		lua_rawseti(L, -2, 1);
		luaext_pushPos(L, bb->BuildBlockArea.high);
		lua_rawseti(L, -2, 2);
		return 3;
	}
	return 2;
}

int l_entityTyGetArmorModifierTechs(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	if (shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps)) {
		lua_newtable(L);
		int c = 1;
		for (int i : s->ModifyArmor.TechList) {
			lua_pushnumber(L, i);
			lua_rawseti(L, -2, c);
			c++;
		}
		return 1;
	}
	if (shok_GGL_CGLBuildingProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps)) {
		lua_newtable(L);
		int c = 1;
		for (int i : s->ModifyArmor.TechList) {
			lua_pushnumber(L, i);
			lua_rawseti(L, -2, c);
			c++;
		}
		return 1;
	}
	return luaL_error(L, "no settler or building entity type at 1");
}

int l_entityTyGetExploModifierTechs(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	if (shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps)) {
		lua_newtable(L);
		int c = 1;
		for (int i : s->ModifyExploration.TechList) {
			lua_pushnumber(L, i);
			lua_rawseti(L, -2, c);
			c++;
		}
		return 1;
	}
	if (shok_GGL_CGLBuildingProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps)) {
		lua_newtable(L);
		int c = 1;
		for (int i : s->ModifyExploration.TechList) {
			lua_pushnumber(L, i);
			lua_rawseti(L, -2, c);
			c++;
		}
		return 1;
	}
	return luaL_error(L, "no settler or building entity type at 1");
}

int l_entityTyGetDamageModifierTechs(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps);
	luaext_assertPointer(L, s, "no settler entity at 1");
	lua_newtable(L);
	int c = 1;
	for (int i : s->ModifyDamage.TechList) {
		lua_pushnumber(L, i);
		lua_rawseti(L, -2, c);
		c++;
	}
	return 1;
}

int l_entityTyGetRangeModifierTechs(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps);
	luaext_assertPointer(L, s, "no settler entity at 1");
	lua_newtable(L);
	int c = 1;
	for (int i : s->ModifyMaxRange.TechList) {
		lua_pushnumber(L, i);
		lua_rawseti(L, -2, c);
		c++;
	}
	return 1;
}

int l_entityTyGetSpeedModifierTechs(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	shok_GGL_CGLSettlerProps* s = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(t->LogicProps);
	luaext_assertPointer(L, s, "no settler entity at 1");
	lua_newtable(L);
	int c = 1;
	for (int i : s->ModifySpeed.TechList) {
		lua_pushnumber(L, i);
		lua_rawseti(L, -2, c);
		c++;
	}
	return 1;
}

int l_buildingTyAddHPTechMod(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "not supportet with SCELoader");
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsBuildingType(), "no building type at 1");
	int tech = luaL_checkint(L, 2);
	shok_technology* techo = (*shok_GGL_CGLGameLogic::GlobalObj)->GetTech(tech);
	luaext_assertPointer(L, techo, "no tech at 2");
	shok_EGL_CGLEEntity::BuildingMaxHpTechBoni.emplace(luaL_checkint(L, 1), tech);
	return 0;
}

int l_buildingTy_GetBuildOnTypes(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaext_assert(L, t->IsBuildingType(), "no building type at 1");
	lua_newtable(L);
	int i = 1;
	for (int ty : shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(t->LogicProps)->BuildOn) {
		lua_pushnumber(L, ty);
		lua_rawseti(L, -2, i);
		i++;
	}
	return 1;
}

int l_leaderTy_GetMaxNumOfSoldiers(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	GGL::CLimitedAttachmentBehaviorProperties* pr = t->GetBehaviorProps<GGL::CLimitedAttachmentBehaviorProperties>();
	luaext_assertPointer(L, pr, "no limited attachment");
	int limit = -1;
	std::string_view v = "ATTACHMENT_LEADER_SOLDIER";
	for (auto& l : pr->Attachments) {
		if (v.compare(l.Type.c_str()) == 0) {
			limit = l.Limit;
			break;
		}
	}
	luaext_assert(L, limit >= 0, "no attachment limit ATTACHMENT_LEADER_SOLDIER");
	lua_pushnumber(L, limit);
	return 1;
}

int l_settlerTy_GetUpgradeCategory(lua_State* L) {
	luaext_checkEntityType(L, 1);
	int ucat = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(1)->SettlerUpgradeManager->GetUpgradeCategoryOfEntityType(luaL_checkint(L, 1));
	lua_pushnumber(L, ucat);
	return 1;
}

int l_entityTy_GetDeleteWhenBuildOn(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	lua_pushboolean(L, t->LogicProps->DeleteWhenBuiltOn);
	return 1;
}
int l_entityTy_SetDeleteWhenBuildOn(lua_State* L) {
	shok_GGlue_CGlueEntityProps* t = luaext_checkEntityType(L, 1);
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	t->LogicProps->DeleteWhenBuiltOn = lua_toboolean(L, 2);
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
	luaext_registerFunc(L, "GetAutoAttackMissChance", &l_entityTyGetAutoAttackMissChance);
	luaext_registerFunc(L, "SetAutoAttackMissChance", &l_entityTySetAutoAttackMissChance);
	luaext_registerFunc(L, "GetAutoAttackRange", &l_entityTyGetRange);
	luaext_registerFunc(L, "SetAutoAttackRange", &l_entityTySetRange);
	luaext_registerFunc(L, "GetArmor", &l_entityTyGetArmor);
	luaext_registerFunc(L, "SetArmor", &l_entityTySetArmor);
	luaext_registerFunc(L, "GetModels", &l_entityTyGetModels);
	luaext_registerFunc(L, "SetModels", &l_entityTySetModels);
	luaext_registerFunc(L, "ResourceTreeTypeHasData", &l_entityTyHasResourceTreeData);
	luaext_registerFunc(L, "ResourceTreeTypeGetData", &l_entityTyGetResourceTreeData);
	luaext_registerFunc(L, "ResourceTreeTypeSetData", &l_entityTySetResourceTreeData);
	luaext_registerFunc(L, "GetBlocking", &l_entityTypeGetBlocking);
	luaext_registerFunc(L, "GetArmorModifierTechs", &l_entityTyGetArmorModifierTechs);
	luaext_registerFunc(L, "GetExplorationModifierTechs", &l_entityTyGetExploModifierTechs);
	luaext_registerFunc(L, "GetDeleteWhenBuildOn", &l_entityTy_GetDeleteWhenBuildOn);
	luaext_registerFunc(L, "SetDeleteWhenBuildOn", &l_entityTy_SetDeleteWhenBuildOn);
	luaext_registerFunc(L, "GetBattleWaitUntil", &l_entityTyGetBattleWaitUntil);
	luaext_registerFunc(L, "SetBattleWaitUntil", &l_entityTySetBattleWaitUntil);

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
	luaext_registerFunc(L, "GetAbilityDataCircularAttack", &l_settlerTyGetAbilityDataCircularAttack);
	luaext_registerFunc(L, "SetAbilityDataCircularAttack", &l_settlerTySetAbilityDataCircularAttack);
	luaext_registerFunc(L, "GetAbilityDataShuriken", &l_settlerTyGetAbilityDataShuriken);
	luaext_registerFunc(L, "SetAbilityDataShuriken", &l_settlerTySetAbilityDataShuriken);
	luaext_registerFunc(L, "GetAbilityDataSniper", &l_settlerTyGetAbilityDataSniper);
	luaext_registerFunc(L, "SetAbilityDataSniper", &l_settlerTySetAbilityDataSniper);
	luaext_registerFunc(L, "GetAbilityDataRangedEffect", &l_settlerTyGetAbilityDataRangedEffect);
	luaext_registerFunc(L, "SetAbilityDataRangedEffect", &l_settlerTySetAbilityDataRangedEffect);
	luaext_registerFunc(L, "GetFearless", &l_settlerTyGetFearless);
	luaext_registerFunc(L, "SetFearless", &l_settlerTySetFearless);
	luaext_registerFunc(L, "GetCost", &l_settlerTyGetCost);
	luaext_registerFunc(L, "SetCost", &l_settlerTySetCost);
	luaext_registerFunc(L, "GetDamageModifierTechs", &l_entityTyGetDamageModifierTechs);
	luaext_registerFunc(L, "GetMaxRangeModifierTechs", &l_entityTyGetRangeModifierTechs);
	luaext_registerFunc(L, "GetSpeedModifierTechs", &l_entityTyGetSpeedModifierTechs);
	luaext_registerFunc(L, "LeaderTypeGetMaxNumberOfSoldiers", &l_leaderTy_GetMaxNumOfSoldiers);
	luaext_registerFunc(L, "GetUpgradeCategory", &l_settlerTy_GetUpgradeCategory);
	lua_rawset(L, -3);

	lua_pushstring(L, "Building");
	lua_newtable(L);
	luaext_registerFunc(L, "GetSabotageFactor", &l_buildingTyGetSabotageFactor);
	luaext_registerFunc(L, "SetSabotageFactor", &l_buildingTySetSabotageFactor);
	luaext_registerFunc(L, "GetVCAttractionSlotsProvided", &l_buildingTyGetVCAttractionPlaces);
	luaext_registerFunc(L, "SetVCAttractionSlotsProvided", &l_buildingTySetVCAttractionPlaces);
	luaext_registerFunc(L, "GetConstructionCost", &l_buildingGetCostConstr);
	luaext_registerFunc(L, "SetConstructionCost", &l_buildingSetCostConstr);
	luaext_registerFunc(L, "GetUpgradeCost", &l_buildingGetCostUpgr);
	luaext_registerFunc(L, "SetUpgradeCost", &l_buildingSetCostUpgr);
	luaext_registerFunc(L, "AddHPTechMod", &l_buildingTyAddHPTechMod);
	luaext_registerFunc(L, "GetBuildOnTypes", &l_buildingTy_GetBuildOnTypes);
	lua_rawset(L, -3);
}

// CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(Entities.PU_LeaderSword1)
// Logic.IsEntityInCategory(GUI.GetSelectedEntity(), 100)
// CppLogic.EntityType.AddEntityCategory(Entities.PU_LeaderSword1, 100)
// CppLogic.EntityType.GetAutoAttackMissChance(Entities.PU_LeaderBow1)
// CppLogic.EntityType.Settler.GetAbilityDataCamouflage(Entities.PU_Hero5)
// CppLogic.EntityType.GetArmor(Entities.PU_LeaderSword1
// CppLogic.EntityType.Building.AddHPTechMod(Entities.PB_Farm1, Technologies.T_Fletching)

#include "pch.h"
#include "l_entity.h"
#include "s5data.h"
#include "luaext.h"

EntityIteratorPredicate* l_entity_checkpredicate(lua_State* L, int i) {
	return reinterpret_cast<EntityIteratorPredicate*>(luaext_checkudata(L, i));
};

int l_entity_getNum(lua_State* L) {
	lua_pushnumber(L, (*shok_EGL_CGLEEntityManager::GlobalObj)->EntityCount);
	return 1;
}

int l_entity_get(lua_State* L) {
	int i = luaL_checkint(L, 1);
	shok_EGL_CGLEEntity* p = (*shok_EGL_CGLEEntityManager::GlobalObj)->GetEntityByNum(i);
	if (p == nullptr)
		lua_pushnumber(L, 0);
	else
		lua_pushnumber(L, p->EntityId);
	return 1;
}

int l_entityPredicateOfType(lua_State* L) {
	int ty = luaL_checkint(L, 1);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateOfType));
	new(ud) EntityIteratorPredicateOfType(ty);
	return 1;
}

int l_entityPredicateOfPlayer(lua_State* L) {
	int ty = luaL_checkint(L, 1);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateOfPlayer));
	new(ud) EntityIteratorPredicateOfPlayer(ty);
	return 1;
}

int l_entityPredicateInCircle(lua_State* L) {
	float x = luaL_checkfloat(L, 1);
	float y = luaL_checkfloat(L, 2);
	float r = luaL_checkfloat(L, 3);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateInCircle));
	new(ud) EntityIteratorPredicateInCircle(x, y, r);
	return 1;
}

int l_entityPredicateAnyPlayer(lua_State* L) {
	int num = lua_gettop(L);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateAnyPlayer) + sizeof(int) * num);
	int* pred = reinterpret_cast<int*>(&reinterpret_cast<EntityIteratorPredicateAnyPlayer*>(ud)[1]); // get start address of array
	for (int i = 0; i < num; i++) {
		pred[i] = luaL_checkint(L, i+1);
	}
	new(ud) EntityIteratorPredicateAnyPlayer(pred, num);
	return 1;
}

int l_entityPredicateAnyEntityType(lua_State* L) {
	int num = lua_gettop(L);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateAnyEntityType) + sizeof(int) * num);
	int* pred = reinterpret_cast<int*>(&reinterpret_cast<EntityIteratorPredicateAnyEntityType*>(ud)[1]); // get start address of array
	for (int i = 0; i < num; i++) {
		pred[i] = luaL_checkint(L, i+1);
	}
	new(ud) EntityIteratorPredicateAnyEntityType(pred, num);
	return 1;
}

int l_entityPredicateAnd(lua_State* L) {
	int num = lua_gettop(L);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateAnd) + sizeof(EntityIteratorPredicate*) * num);
	lua_insert(L, 1);
	EntityIteratorPredicate** pred = reinterpret_cast<EntityIteratorPredicate**>(&reinterpret_cast<EntityIteratorPredicateAnd*>(ud)[1]); // get start address of predicate array
	new(ud) EntityIteratorPredicateAnd(pred, num);
	lua_newtable(L);
	lua_insert(L, 2);
	for (int i = num-1; i >= 0; i--) { // keep predicates as metatable, so they dont get gced
		pred[i] = l_entity_checkpredicate(L, -1);
		lua_rawseti(L, 2, i);
	}
	lua_setmetatable(L, 1);
	return 1;
}

int l_entityPredicateOr(lua_State* L) {
	int num = lua_gettop(L);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateOr) + sizeof(EntityIteratorPredicate*) * num);
	lua_insert(L, 1);
	EntityIteratorPredicate** pred = reinterpret_cast<EntityIteratorPredicate**>(&reinterpret_cast<EntityIteratorPredicateOr*>(ud)[1]); // get start address of predicate array
	new(ud) EntityIteratorPredicateOr(pred, num);
	lua_newtable(L);
	lua_insert(L, 2);
	for (int i = num - 1; i >= 0; i--) { // keep predicates as metatable, so they dont get gced
		pred[i] = l_entity_checkpredicate(L, -1);
		lua_rawseti(L, 2, i);
	}
	lua_setmetatable(L, 1);
	return 1;
}

int l_entityPredicateNot(lua_State* L) {
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 1);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateNot));
	new(ud) EntityIteratorPredicateNot(pred);
	lua_newtable(L);
	lua_pushvalue(L, 1);// keep predicate as metatable, so they dont get gced
	lua_rawseti(L, -2, 1);
	lua_setmetatable(L, -2);
	return 1;
}

int l_entityPredicatePriority(lua_State* L) {
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 1);
	int p = luaL_checkint(L, 2);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicatePriority));
	new(ud) EntityIteratorPredicatePriority(p, pred);
	lua_newtable(L);
	lua_pushvalue(L, 1);// keep predicate as metatable, so they dont get gced
	lua_rawseti(L, -2, 1);
	lua_setmetatable(L, -2);
	return 1;
}

int l_entityPredicateIsSettler(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateIsSettler));
	new(ud) EntityIteratorPredicateIsSettler();
	return 1;
}

int l_entityPredicateIsBuilding(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateIsBuilding));
	new(ud) EntityIteratorPredicateIsBuilding();
	return 1;
}

int l_entityPredicateIsRelevant(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateIsRelevant));
	new(ud) EntityIteratorPredicateIsRelevant();
	return 1;
}
int l_entityPredicateIsNotSoldier(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateIsNotSoldier));
	new(ud) EntityIteratorPredicateIsNotSoldier();
	return 1;
}

int l_entityPredicateOfEntityCategory(lua_State* L) {
	int ty = luaL_checkint(L, 1);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateOfEntityCategory));
	new(ud) EntityIteratorPredicateOfEntityCategory(static_cast<shok_EntityCategory>(ty));
	return 1;
}

int l_entityPredicateProvidesResource(lua_State* L) {
	int ty = luaL_checkint(L, 1);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateProvidesResource));
	new(ud) EntityIteratorPredicateProvidesResource(ty);
	return 1;
}

int l_entityPredicateInRect(lua_State* L) {
	float x1 = luaL_checkfloat(L, 1);
	float y1 = luaL_checkfloat(L, 2);
	float x2 = luaL_checkfloat(L, 3);
	float y2 = luaL_checkfloat(L, 4);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateInRect));
	new(ud) EntityIteratorPredicateInRect(x1, y1, x2, y2);
	return 1;
}

int l_entityPredicateIsVisible(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateIsVisible));
	new(ud) EntityIteratorPredicateIsVisible();
	return 1;
}

int l_entityPredicateOfUpgradeCategory(lua_State* L) {
	int ty = luaL_checkint(L, 1);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateOfUpgradeCategory));
	new(ud) EntityIteratorPredicateOfUpgradeCategory(ty);
	return 1;
}

int l_entityPredicateIsAlive(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateIsAlive));
	new(ud) EntityIteratorPredicateIsAlive();
	return 1;
}

int l_entityIteratorToTable(lua_State* L) {
	if (lua_gettop(L) > 1) { // auto create an and predicate
		l_entityPredicateAnd(L);
	}
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 1);
	int index = 1;
	lua_newtable(L);
	EntityIterator it{ pred };
	shok_EGL_CGLEEntity* e = nullptr;
	while (true) {
		e = it.GetNext(nullptr, nullptr);
		if (e == nullptr)
			break;
		lua_pushnumber(L, e->EntityId);
		lua_rawseti(L, 2, index);
		index++;
	}
	return 1;
}

int l_entityIteratorCount(lua_State* L) {
	if (lua_gettop(L) > 1) { // auto create an and predicate
		l_entityPredicateAnd(L);
	}
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 1);
	int count = 1;
	EntityIterator it{ pred };
	shok_EGL_CGLEEntity* e = nullptr;
	while (true) {
		e = it.GetNext(nullptr, nullptr);
		if (e == nullptr)
			break;
		count++;
	}
	lua_pushnumber(L, count);
	return 1;
}

int l_entityIteratorGetNearest(lua_State* L) {
	if (lua_gettop(L) > 1) { // auto create an and predicate
		l_entityPredicateAnd(L);
	}
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 1);
	EntityIterator it{ pred };
	float maxR = -1;
	shok_EGL_CGLEEntity* e = it.GetNearest(&maxR);
	lua_pushnumber(L, e == nullptr ? 0 : e->EntityId);
	lua_pushnumber(L, maxR);
	return 2;
}

int l_entityIteratorNext(lua_State* L) { // (state, last value) -> next value
	EntityIterator* it = reinterpret_cast<EntityIterator*>(lua_touserdata(L, 1)); // no error checking here, cause that would cost speed
	float r = -1;
	int p = -1;
	shok_EGL_CGLEEntity* e = it->GetNext(&r, &p);
	if (e == nullptr) {
		lua_pushnil(L);
		lua_pushnil(L);
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, e->EntityId);
		lua_pushnumber(L, r);
		lua_pushnumber(L, p);
	}
	return 3;
}

int l_entityIterator(lua_State* L) {
	if (lua_gettop(L) > 1) { // auto create an and predicate
		l_entityPredicateAnd(L);
	}
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 1);
	lua_pushcfunction(L, &l_entityIteratorNext); // func
	void* ud = lua_newuserdata(L, sizeof(EntityIterator)); // state (using state instead of c closure, cause i have one less lua api call in next)
	new(ud) EntityIterator(pred);
	lua_newtable(L);
	lua_pushvalue(L, 1);
	lua_rawseti(L, -2, 1);
	lua_setmetatable(L, 3); // set predicate as metatable of state, to keep it from getting gced
	lua_pushnil(L); // initial value
	return 3;
}

int l_entityCheckPredicate(lua_State* L) {
	shok_EGL_CGLEEntity* s = luaext_checkEntity(L, 1);
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 2);
	lua_pushboolean(L, pred->MatchesEntity(s, nullptr, nullptr));
	return 1;
}

int l_settlerGetLeaderOfSoldier(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	lua_pushnumber(L, s->LeaderId);
	return 1;
}

int l_entityGetNumberOfBehaviors(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->Behaviours.size());
	return 1;
}

int l_leaderGetLeaderXP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetBehavior<shok_GGL_CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	lua_pushnumber(L, b->Experience);
	return 1;
}
int l_leaderSetLeaderXP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetBehavior<shok_GGL_CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	b->Experience = luaL_checkint(L, 2);
	return 0;
}

int l_leaderGetLeaderHP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetBehavior<shok_GGL_CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	lua_pushnumber(L, b->GetTroopHealth());
	lua_pushnumber(L, b->GetTroopHealthPerSoldier());
	return 2;
}
int l_leaderSetLeaderHP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetBehavior<shok_GGL_CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	b->TroopHealthCurrent = luaL_checkint(L, 2);
	return 0;
}

int l_settlerGetBaseMovementSpeed(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CBehaviorDefaultMovement* b = s->GetBehavior<shok_GGL_CBehaviorDefaultMovement>();
	luaext_assertPointer(L, b, "no moving entity at 1");
	lua_pushnumber(L, b->MovementSpeed);
	lua_pushnumber(L, rad2deg(b->TurningSpeed));
	return 2;
}
int l_settlerSetBaseMovementSpeed(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CBehaviorDefaultMovement* b = s->GetBehavior<shok_GGL_CBehaviorDefaultMovement>();
	luaext_assertPointer(L, b, "no moving entity at 1");
	if (lua_isnumber(L, 2))
		b->MovementSpeed = luaL_checkfloat(L, 2);
	if (lua_isnumber(L, 3))
		b->TurningSpeed = static_cast<float>(deg2rad(lua_tonumber(L, 3)));
	return 0;
}

int l_buildingGetAutoFillActive(lua_State* L) {
	shok_GGL_CBuilding* e = luaext_checkBulding(L, 1);
	shok_GGL_CBarrackBehavior* b = e->GetBehavior<shok_GGL_CBarrackBehavior>();
	luaext_assertPointer(L, b, "no barracks at 1");
	lua_pushboolean(L, b->AutoFillActive);
	return 1;
}

int l_entityGetScale(lua_State* L) {
	shok_EGL_CGLEEntity* s = luaext_checkEntity(L, 1);
	lua_pushnumber(L, s->Scale);
	return 1;
}
int l_entitySetScale(lua_State* L) {
	shok_EGL_CGLEEntity* s = luaext_checkEntity(L, 1);
	s->Scale = luaL_checkfloat(L, 2);
	return 0;
}

int l_buildingGetHeight(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	lua_pushnumber(L, b->BuildingHeight);
	return 1;
}
int l_buildingSetHeight(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	b->BuildingHeight = luaL_checkfloat(L, 2);
	return 0;
}

int l_settlerGetOvereadWidget(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	lua_pushnumber(L, s->OverheadWidget);
	return 1;
}
int l_settlerSetOvereadWidget(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	int ov = luaL_checkint(L, 2);
	luaext_assert(L, ov >= 0 && ov <= 4, "invalid overhead code");
	s->OverheadWidget = ov;
	return 1;
}

int l_movingEntityGetTargetPos(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_EGL_CMovingEntity* m = shok_DynamicCast<shok_EGL_CGLEEntity, shok_EGL_CMovingEntity>(e);
	luaext_assertPointer(L, m, "no moving entity at 1");
	luaext_pushPos(L, m->TargetPosition);
	if (m->TargetRotationValid) {
		lua_pushstring(L, "r");
		lua_pushnumber(L, rad2deg(m->TargetRotation));
		lua_rawset(L, -3);
	}
	return 1;
}
int l_movingEntitySetTargetPos(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_EGL_CMovingEntity* m = shok_DynamicCast<shok_EGL_CGLEEntity, shok_EGL_CMovingEntity>(e);
	luaext_assertPointer(L, m, "no moving entity at 1");
	luaext_checkPos(L, m->TargetPosition, 2);
	lua_pushstring(L, "r");
	lua_gettable(L, 2);
	if (lua_isnumber(L, -1)) {
		m->TargetRotation = static_cast<float>(deg2rad(lua_tonumber(L, -1)));
		m->TargetRotationValid = 1;
	}
	else {
		m->TargetRotationValid = 0;
	}
	return 0;
}

int l_settlerHeroCamouflageGetDurationLeft(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CCamouflageBehavior* c = s->GetBehavior<shok_GGL_CCamouflageBehavior>();
	luaext_assertPointer(L, c, "no camo hero at 1");
	lua_pushnumber(L, c->InvisibilityRemaining);
	return 1;
}
int l_settlerHeroCamouflageSetDurationLeft(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CCamouflageBehavior* c = s->GetBehavior<shok_GGL_CCamouflageBehavior>();
	luaext_assertPointer(L, c, "no camo hero at 1");
	luaext_assert(L, !shok_DynamicCast<shok_GGL_CCamouflageBehavior, shok_GGL_CThiefCamouflageBehavior>(c), "thief at 1, use ThiefSetCamouflageTimeTo instead");
	c->InvisibilityRemaining = luaL_checkint(L, 2);
	return 1;
}

int l_settlerThiefCamouflageGetTimeTo(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CThiefCamouflageBehavior* c = s->GetBehavior<shok_GGL_CThiefCamouflageBehavior>();
	luaext_assertPointer(L, c, "no thief at 1");
	lua_pushnumber(L, c->TimeToInvisibility);
	return 1;
}
int l_settlerThiefCamouflageSetTimeTo(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CThiefCamouflageBehavior* c = s->GetBehavior<shok_GGL_CThiefCamouflageBehavior>();
	luaext_assertPointer(L, c, "no thief at 1");
	int i = luaL_checkint(L, 2);
	c->TimeToInvisibility = i;
	c->InvisibilityRemaining = i <= 0 ? 15 : 0;
	return 0;
}

int l_settlerHeroGetResurrectionTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CHeroBehavior* h = s->GetBehavior<shok_GGL_CHeroBehavior>();
	luaext_assertPointer(L, h, "no hero at 1");
	lua_pushnumber(L, h->ResurrectionTimePassed);
	return 1;
}
int l_settlerHeroSetResurrectionTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CHeroBehavior* h = s->GetBehavior<shok_GGL_CHeroBehavior>();
	luaext_assertPointer(L, h, "no hero at 1");
	h->ResurrectionTimePassed = luaL_checkint(L, 2);
	return 0;
}

int l_entityGetLimitedLifespanRemaining(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CLimitedLifespanBehavior* l = e->GetBehavior<shok_GGL_CLimitedLifespanBehavior>();
	luaext_assertPointer(L, l, "no limited lifespan at 1");
	lua_pushnumber(L, l->RemainingLifespanSeconds);
	return 1;
}
int l_entitySetLimitedLifespanRemaining(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CLimitedLifespanBehavior* l = e->GetBehavior<shok_GGL_CLimitedLifespanBehavior>();
	luaext_assertPointer(L, l, "no limited lifespan at 1");
	l->RemainingLifespanSeconds = luaL_checkint(L, 2);
	return 0;
}

void(__thiscall* FireEvent)(shok_EGL_CGLEEntity* th, shok_BB_CEvent* d);
void __fastcall fireeventhook(shok_EGL_CGLEEntity* th, int _, shok_BB_CEvent* d) {
	lua_State* L = *shok_luastate_game;
	int t = lua_gettop(L);
	lua_getglobal(L, "event");
	lua_pushnumber(L, ((int*)d)[1]);
	lua_pushnumber(L, (int)d);
	lua_pushnumber(L, ((int*)d)[0]);
	lua_pcall(L, 3, 0, 0);
	lua_settop(L, t);
	FireEvent(th, d);
}

int l_entity_test(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	//shok_GGL_CBuilding* e = luaext_checkBulding(L, 1);
	/*shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	if (!FireEvent) {
		shok_vtable_EGL_CGLEEntity* vt = (shok_vtable_EGL_CGLEEntity*)e->vtable;
		FireEvent = vt->FireEvent;
		vt->FireEvent = (void(__thiscall *)(shok_EGL_CGLEEntity * th, shok_event_data * d)) &fireeventhook;
	}*/
	//DEBUGGER_BREAK
	//lua_pushnumber(L, (int) &(e->GetEntityType()->LogicProps->MaxHealth));
	lua_pushnumber(L, (int)&e->CurrentState);
	return 1;
}

int l_entityGetTaskListIndex(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->CurrentTaskIndex);
	return 1;
}
int l_entitySetTaskListIndex(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	e->CurrentTaskIndex = luaL_checkint(L, 2);
	return 0;
}

int l_movingEntityGetSpeedFactor(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CBehaviorDefaultMovement* m = e->GetBehavior<shok_GGL_CBehaviorDefaultMovement>();
	luaext_assertPointer(L, m, "no moving entity at 1");
	lua_pushnumber(L, m->SpeedFactor);
	return 1;
}

int l_settlerGetWorkerCurrentWorkTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CWorkerBehavior* w = s->GetBehavior<shok_GGL_CWorkerBehavior>();
	luaext_assertPointer(L, w, "no worker at 1");
	lua_pushnumber(L, w->WorkTimeRemaining);
	return 1;
}
int l_settlerSetWorkerCurrentWorkTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CWorkerBehavior* w = s->GetBehavior<shok_GGL_CWorkerBehavior>();
	luaext_assertPointer(L, w, "no worker at 1");
	w->WorkTimeRemaining = luaL_checkint(L, 2);
	return 0;
}

int l_buildingGetCurrentTradeInfo(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CMarketBehavior* m = b->GetBehavior<shok_GGL_CMarketBehavior>();
	luaext_assertPointer(L, m, "no market at 1");
	lua_pushnumber(L, m->BuyResourceType);
	lua_pushnumber(L, m->SellResourceType);
	lua_pushnumber(L, m->BuyAmount);
	lua_pushnumber(L, m->SellAmount);
	lua_pushnumber(L, m->ProgressAmount);
	return 5;
}
int l_buildingSetCurrentTradeInfo(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CMarketBehavior* m = b->GetBehavior<shok_GGL_CMarketBehavior>();
	luaext_assertPointer(L, m, "no market at 1");
	if (lua_isnumber(L, 2))
		m->BuyResourceType = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		m->SellResourceType = luaL_checkint(L, 3);
	if (lua_isnumber(L, 4))
		m->BuyAmount = luaL_checkfloat(L, 4);
	if (lua_isnumber(L, 5))
		m->SellAmount = luaL_checkfloat(L, 5);
	if (lua_isnumber(L, 6))
		m->ProgressAmount = luaL_checkfloat(L, 6);
	return 0;
}

int l_entityIsSoldier(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushboolean(L, e->GetBehavior<shok_GGL_CSoldierBehavior>() != nullptr);
	return 1;
}

int l_heroResurrect(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CHeroBehavior* h = e->GetBehavior<shok_GGL_CHeroBehavior>();
	luaext_assertPointer(L, h, "no hero at 1");
	h->ResurrectionTimePassed = 20000;
	h->EnemyNear = 0;
	h->FriendNear = 1;
	return 0;
}

int l_settlerThiefSetStolenResourceInfo(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CThiefBehavior* t = e->GetBehavior<shok_GGL_CThiefBehavior>();
	luaext_assertPointer(L, t, "no thief at 1");
	int ty = luaL_checkint(L, 2);
	int am;
	if (ty == 0)
		am = 0;
	else
		am = luaL_checkint(L, 3);
	t->ResourceType = ty;
	t->Amount = am;
	if (ty == 0)
		t->StolenFromPlayer = 0;
	else if (lua_isnumber(L, 4))
		t->StolenFromPlayer = luaL_checkint(L, 4); // todo: check for func that refreshes model
	return 0;
}

int l_entityGetAutoAttackMaxRange(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CBattleBehavior* b = e->GetBehavior<shok_GGL_CBattleBehavior>();
	if (b) {
		lua_pushnumber(L, b->GetMaxRange());
		return 1;
	}
	shok_GGL_CAutoCannonBehavior* a = e->GetBehavior<shok_GGL_CAutoCannonBehavior>();
	if (a) {
		lua_pushnumber(L, a->GetMaxRange());
		return 1;
	}
	return luaL_error(L, "no battle entity or autocannon at 1");
}

int l_entityGetModel(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	int m = e->ModelOverride;
	if (m == 0) {
		m = e->GetEntityType()->DisplayProps->Model[0];
	}
	lua_pushnumber(L, m);
	return 1;
}

int l_entityGetExploration(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->GetExploration());
	return 1;
}

int l_entityGetSpeed(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CBehaviorDefaultMovement* m = e->GetBehavior<shok_GGL_CBehaviorDefaultMovement>();
	luaext_assertPointer(L, m, "no moving entity at 1");
	lua_pushnumber(L, m->GetMovementSpeed() * 10.0);
	return 1;
}

int l_settlerIsVisible(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CCamouflageBehavior* c = e->GetBehavior<shok_GGL_CCamouflageBehavior>();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}

int l_settlerSendHawk(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CHeroHawkBehavior* b = e->GetBehavior<shok_GGL_CHeroHawkBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CHeroHawkBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CHeroHawkBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->HeroAbilitySendHawk(p);
	return 0;
}

int l_settlerInflictFear(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CInflictFearAbility* b = e->GetBehavior<shok_GGL_CInflictFearAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CInflictFearAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CInflictFearAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityInflictFear();
	return 0;
}

int l_settlerPlaceBomb(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CBombPlacerBehavior* b = e->GetBehavior<shok_GGL_CBombPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CHeroAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CHeroAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->HeroAbilityPlaceBomb(p);
	return 0;
}

int l_settlerPlaceCannon(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CCannonBuilderBehavior* b = e->GetBehavior<shok_GGL_CCannonBuilderBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CCannonBuilderBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CCannonBuilderBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	p.FloorToBuildingPlacement();
	int bottom = luaL_checkint(L, 3);
	shok_GGlue_CGlueEntityProps* ety = (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(bottom);
	luaext_assertPointer(L, ety, "no bottom entitytype");
	luaext_assert(L, ety->IsBuildingType(), "bottom not a building");
	int top = luaL_checkint(L, 4);
	luaext_assertPointer(L, (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(top), "no top entitytype");
	luaext_assert(L, shok_canPlaceBuilding(bottom, e->PlayerId, &p, 0, 0), "cannot place foundation at that position");
	e->HeroAbilityPlaceCannon(p, bottom, top);
	return 0;
}

int l_settlerRangedEffect(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CRangedEffectAbility* b = e->GetBehavior<shok_GGL_CRangedEffectAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CRangedEffectAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CRangedEffectAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityRangedEffect();
	return 0;
}

int l_settlerCircularAttack(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CCircularAttack* b = e->GetBehavior<shok_GGL_CCircularAttack>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CCircularAttackProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CCircularAttackProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityCircularAttack();
	return 0;
}

int l_settlerSummon(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CSummonBehavior* b = e->GetBehavior<shok_GGL_CSummonBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CSummonBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CSummonBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilitySummon();
	int summoned = 0;
	e->ObservedEntities.ForAll([L, &summoned](shok_attachment* a) {
		if (a->AttachmentType == shok_AttachmentType::SUMMONER_SUMMONED)
			lua_pushnumber(L, a->EntityId);
			summoned++;
		});
	return summoned;
}

int l_settlerActivateCamo(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CCamouflageBehavior* b = e->GetBehavior<shok_GGL_CCamouflageBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, !shok_DynamicCast<shok_GGL_CCamouflageBehavior, shok_GGL_CThiefCamouflageBehavior>(b), "thief camo cannot be manually activated");
	shok_GGL_CCamouflageBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CCamouflageBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityActivateCamoflage();
	return 0;
}

int l_settlerConvert(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CConvertSettlerAbility* b = e->GetBehavior<shok_GGL_CConvertSettlerAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CConvertSettlerAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CConvertSettlerAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_GGL_CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	e->HeroAbilityConvert(t->EntityId);
	return 0;
}

int l_settlerSnipe(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CSniperAbility* b = e->GetBehavior<shok_GGL_CSniperAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CSniperAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CSniperAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_GGL_CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	luaext_assert(L, e->Position.IsInRange(t->Position, bp->Range), "target not in range");
	e->HeroAbilitySnipe(t->EntityId);
	return 0;
}

int l_settlerShuriken(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CShurikenAbility* b = e->GetBehavior<shok_GGL_CShurikenAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CShurikenAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CShurikenAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_GGL_CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	luaext_assert(L, e->Position.IsInRange(t->Position, bp->Range), "target not in range");
	e->HeroAbilityShuriken(t->EntityId);
	return 0;
}

int l_settlerMotivateWorkers(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CMotivateWorkersAbility* b = e->GetBehavior<shok_GGL_CMotivateWorkersAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CMotivateWorkersAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CMotivateWorkersAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityMotivateWorkers();
	return 0;
}

int l_settlerSabotage(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CKegPlacerBehavior* b = e->GetBehavior<shok_GGL_CKegPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CKegPlacerBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CKegPlacerBehaviorProperties>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	luaext_assert(L, e->GetBehavior<shok_GGL_CThiefBehavior>()->ResourceType == 0, "is carrying resources");
	shok_GGL_CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, t->IsEntityInCategory(shok_EntityCategory::Bridge) || ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile or bridge");
	e->ThiefSabotage(t->EntityId);
	return 0;
}

int l_settlerDefuse(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CKegPlacerBehavior* b = e->GetBehavior<shok_GGL_CKegPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, e->GetBehavior<shok_GGL_CThiefBehavior>()->ResourceType == 0, "is carrying resources");
	shok_EGL_CGLEEntity* t = luaext_checkEntity(L, 2);
	luaext_assertPointer(L, t->GetBehavior<shok_GGL_CKegBehavior>(), "no keg at 2");
	e->ThiefDefuse(t->EntityId);
	return 0;
}

int l_settlerBinoculars(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CAbilityScoutBinocular* b = e->GetBehavior<shok_GGL_CAbilityScoutBinocular>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CAbilityScoutBinocularProps* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CAbilityScoutBinocularProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->ScoutBinoculars(p);
	return 0;
}

int l_settlerPlaceTorch(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CTorchPlacerBehavior* b = e->GetBehavior<shok_GGL_CTorchPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CTorchPlacerBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CTorchPlacerBehaviorProperties>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->ScoutPlaceTorch(p);
	return 0;
}

int l_settlerPointToRes(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CPointToResourceBehavior* b = e->GetBehavior<shok_GGL_CPointToResourceBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CPointToResourceBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<shok_GGL_CPointToResourceBehaviorProperties>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->ScoutFindResource();
	return 0;
}

int l_settlerStealFrom(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CThiefBehavior* b = e->GetBehavior<shok_GGL_CThiefBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	e->ThiefStealFrom(t->EntityId);
	return 0;
}

int l_settlerSecureGoods(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CThiefBehavior* b = e->GetBehavior<shok_GGL_CThiefBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, b->ResourceType != 0, "no resources carried");
	shok_GGL_CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, e->PlayerId == t->PlayerId, "entities are not of same player");
	luaext_assert(L, t->IsEntityInCategory(shok_EntityCategory::Headquarters), "target not hq");
	e->ThiefSecureGoods(t->EntityId);
	return 0;
}

int l_settlerEnableConversionHook(lua_State* L) {
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	lua_pushlightuserdata(L, &l_settlerEnableConversionHook);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	shok_EGL_CGLEEntity::HookHero6Convert();
	shok_EGL_CGLEEntity::Hero6ConvertHookCb = [](int id, int pl, int nid, int converter) {
		lua_State* L = *shok_luastate_game;
		int t = lua_gettop(L);
		lua_pushlightuserdata(L, &l_settlerEnableConversionHook);
		lua_rawget(L, LUA_REGISTRYINDEX);
		lua_pushnumber(L, id);
		lua_pushnumber(L, pl);
		lua_pushnumber(L, nid);
		lua_pushnumber(L, converter);
		lua_pcall(L, 4, 0, 0);
		lua_settop(L, t);
	};
	return 0;
}
int l_settlerDisableConversionHook(lua_State* L) {
	shok_EGL_CGLEEntity::Hero6ConvertHookCb = nullptr;
	return 0;
}

int l_entityIsFeared(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	int id = e->GetFirstAttachedToMe(shok_AttachmentType::INFLICTOR_TERRORIZED);
	if (id == 0)
		lua_pushboolean(L, false);
	else
		lua_pushnumber(L, id);
	return 1;
}

int l_settlerMove(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	shok_position p;
	luaext_checkPos(L, p, 2);
	e->Move(p);
	if (lua_isnumber(L, 3)) {
		e->TargetRotation = static_cast<float>(deg2rad(lua_tonumber(L, 3)));
		e->TargetRotationValid = 1;
	}
	return 0;
}

int l_entityClearAttackers(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	e->ClearAttackers();
	return 0;
}

int l_buildingFoundryBuildCannon(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<shok_GGL_CFoundryBehavior>(), "no foundry at 1");
	luaext_assert(L, b->IsIdle(), "is not idle");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	luaext_assert(L, p->PlayerAttractionHandler->GetAttractionUsage() < p->PlayerAttractionHandler->GetAttractionLimit(), "pop capped");
	b->CommandBuildCannon(luaL_checkint(L, 2));
	return 0;
}

int l_settlerIsIdle(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	lua_pushboolean(L, e->IsIdle());
	return 1;
}

int l_buildingGetConstructionSite(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	lua_pushnumber(L, b->GetConstructionSite());
	return 1;
}

int l_buildingGetNextFreeConstructionSlotFor(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_position p;
	luaext_checkPos(L, p, 2);
	lua_pushnumber(L, b->GetNearestFreeConstructionSlotFor(&p));
	return 1;
}

int l_settlerSerfConstruct(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 2);
	luaext_assertPointer(L, s->GetBehavior<shok_GGL_CSerfBehavior>(), "no serf");
	luaext_assert(L, !b->IsConstructionFinished(), "no construction site");
	bool suc = s->SerfConstructBuilding(b);
	lua_pushboolean(L, suc);
	return 1;
}

int l_buildingGetNextFreeRepairSlotFor(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_position p;
	luaext_checkPos(L, p, 2);
	lua_pushnumber(L, b->GetNearestFreeRepairSlotFor(&p));
	return 1;
}

int l_settlerSerfRepair(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 2);
	luaext_assertPointer(L, s->GetBehavior<shok_GGL_CSerfBehavior>(), "no serf");
	luaext_assert(L, b->IsConstructionFinished(), "construction site");
	luaext_assert(L, b->Health < b->GetMaxHealth(), "at full health");
	bool suc = s->SerfRepairBuilding(b);
	lua_pushboolean(L, suc);
	return 1;
}

int l_entityReplaceWithResourceEntity(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	e = shok_EGL_CGLEEntity::ReplaceEntityWithResourceEntity(e);
	if (e)
		lua_pushnumber(L, e->EntityId);
	else
		lua_pushnil(L);
	return 1;
}

int l_settlerSerfExtract(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_EGL_CGLEEntity* b = luaext_checkEntity(L, 2);
	luaext_assertPointer(L, s->GetBehavior<shok_GGL_CSerfBehavior>(), "no serf");
	if (!shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CResourceDoodad>(b))
		b = shok_EGL_CGLEEntity::ReplaceEntityWithResourceEntity(b);
	if (b == nullptr || !shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CResourceDoodad>(b)) {
		return luaL_error(L, "no resource entity");
	}
	s->SerfExtractResource(b->EntityId);
	lua_pushnumber(L, b->EntityId);
	return 1;
}

int l_buildingStartUpgrade(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsIdle(), "is not idle");
	b->StartUpgrade();
	return 0;
}

int l_buildingCancelUpgrade(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsUpgrading, "not upgrading");
	b->CancelUpgrade();
	return 0;
}

int l_buildingIsIdle(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	lua_pushboolean(L, b->IsIdle());
	return 1;
}

int l_buildingBarracksGetLeadersTrainingAt(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<shok_GGL_CBarrackBehavior>(), "no barracks");
	int i = 0;
	b->ObserverEntities.ForAll([L, &i](shok_attachment* a) {
		if (a->AttachmentType == shok_AttachmentType::FIGHTER_BARRACKS && !shok_EGL_CGLEEntity::GetEntityByID(a->EntityId)->GetBehavior<shok_GGL_CSoldierBehavior>()) {
			lua_pushnumber(L, a->EntityId);
			i++;
		}
		});
	return i;
}

int l_buildingFoundryGetCannonTypeInConstruction(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CFoundryBehavior* f = b->GetBehavior<shok_GGL_CFoundryBehavior>();
	luaext_assertPointer(L, f, "no foundry");
	lua_pushnumber(L, f->CannonType);
	return 1;
}

int l_settlerExpell(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	s->SettlerExpell();
	return 0;
}

int l_buildingBuySoldierForLeader(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CSettler* s = luaext_checkSettler(L, 2);
	luaext_assertPointer(L, b->GetBehavior<shok_GGL_CBarrackBehavior>(), "no barracks");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading, "barracks is upgrading or under construction");
	shok_GGL_CLeaderBehavior* l = s->GetBehavior<shok_GGL_CLeaderBehavior>();
	shok_GGL_CLeaderBehaviorProps* lp = s->GetEntityType()->GetBehaviorProps<shok_GGL_CLeaderBehaviorProps>();
	luaext_assertPointer(L, l, "no leader");
	luaext_assert(L, b->PlayerId == s->PlayerId, "different players");
	shok_GGlue_CGlueEntityProps* solty = (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(lp->SoldierType);
	luaext_assertPointer(L, solty, "no soldier type set");
	if (!lua_toboolean(L, 3)) {
		int ucat = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(1)->BuildingUpgradeManager->GetUpgradeCategoryOfBuildingType(b->EntityType);
		luaext_assert(L, lp->BarrackUpgradeCategory == ucat, "leader type doesnt match barracks type");
	}
	int max = s->LimitedAttachmentGetMaximum(shok_AttachmentType::LEADER_SOLDIER);
	int curr = 0;
	s->ObservedEntities.ForAll([&curr](shok_attachment* a) { if (a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER) curr++; });
	luaext_assert(L, curr < max, "no free soldier slot left");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(s->PlayerId);
	luaext_assert(L, p->PlayerAttractionHandler->GetAttractionUsage() < p->PlayerAttractionHandler->GetAttractionLimit(), "pop capped");
	shok_GGL_CGLSettlerProps* sprop = shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(solty->LogicProps);
	luaext_assertPointer(L, sprop, "error: soldier no settler type");
	luaext_assert(L, p->CurrentResources.HasResources(&sprop->Cost), "missing res");
	b->CommandRecruitSoldierForLeader(s->EntityId);
	return 0;
}

int l_leaderAttachSoldier(lua_State* L) {
	shok_GGL_CSettler* l = luaext_checkSettler(L, 1);
	shok_GGL_CSettler* s = luaext_checkSettler(L, 2);
	shok_GGL_CLeaderBehavior* lb = l->GetBehavior<shok_GGL_CLeaderBehavior>();
	luaext_assertPointer(L, lb, "no leader");
	luaext_assertPointer(L, s->GetBehavior<shok_GGL_CSoldierBehavior>(), "no soldier");
	luaext_assert(L, s->EntityType == l->GetEntityType()->GetBehaviorProps<shok_GGL_CLeaderBehaviorProps>()->SoldierType, "leader and soldier type doesnt match");
	l->LeaderAttachSoldier(s->EntityId);
	return 0;
}

int l_settlerSerfToBattleSerf(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, s->GetBehavior<shok_GGL_CSerfBehavior>(), "no serf");
	s->SerfTurnToBattleSerf();
	return 0;
}

int l_settlerBattleSerfToSerf(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, s->GetBehavior<shok_GGL_CBattleSerfBehavior>(), "no battleserf");
	s->BattleSerfTurnToSerf();
	return 0;
}

int l_buildingActivateOvertime(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, !b->IsOvertimeActive, "overtime already active");
	luaext_assert(L, b->OvertimeCooldown <= 0, "cooldown active");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading && b->CurrentState != 0x13, "building not idle");
	b->ActivateOvertime();
	return 0;
}

int l_buildingDeactivateOvertime(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsOvertimeActive, "overtime not active");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading && b->CurrentState != 0x13, "building not idle");
	b->DeactivateOvertime();
	return 0;
}

int l_buildingBarracksRecruitLeaders(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CBarrackBehavior* r = b->GetBehavior<shok_GGL_CBarrackBehavior>();
	luaext_assertPointer(L, r, "no barracks");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading, "building not idle");
	luaext_assert(L, r->AutoFillActive, "already recruiting leaders");
	b->BarracksRecruitLeaders();
	return 0;
}
int l_buildingBarracksRecruitGroups(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CBarrackBehavior* r = b->GetBehavior<shok_GGL_CBarrackBehavior>();
	luaext_assertPointer(L, r, "no barracks");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading, "building not idle");
	luaext_assert(L, !r->AutoFillActive, "already recruiting groups");
	b->BarracksRecruitGroups();
	return 0;
}

int l_buildingHQBuySerf(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsEntityInCategory(shok_EntityCategory::Headquarters), "no hq");
	luaext_assert(L, b->IsIdle(), "building not idle");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	luaext_assert(L, p->PlayerAttractionHandler->GetAttractionUsage() < p->PlayerAttractionHandler->GetAttractionLimit(), "pop capped");
	shok_GGlue_CGlueEntityProps* solty = (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(*shok_entityTypeIDSerf);
	luaext_assert(L, p->CurrentResources.HasResources(&shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(solty->LogicProps)->Cost), "missing res");
	b->HQBuySerf();
	return 0;
}

int l_buildingSell(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, ((shok_GGL_CGLBuildingProps*)b->GetEntityType()->LogicProps)->CanBeSold, "cannot be sold");
	b->SellBuilding();
	return 0;
}

int l_buildingStartResearch(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsIdle(), "building not idle");
	int tech = luaL_checkint(L, 2);
	shok_technology* techo = (*shok_GGL_CGLGameLogic::GlobalObj)->GetTech(tech);
	luaext_assertPointer(L, techo, "no tech at 2");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	shok_TechState techstate = p->GetTechStatus(tech);
	luaext_assert(L, techstate == shok_TechState::Allowed, "wrong techstate");
	luaext_assert(L, p->CurrentResources.HasResources(&techo->ResourceCosts), "not enough res");
	b->StartResearch(tech);
	return 0;
}

int l_buildingCancelResearch(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->GetTechnologyInResearch(), "no tech in research");
	b->CancelResearch();
	return 0;
}

bool marketIsRes(shok_ResourceType rty) {
	return rty == shok_ResourceType::Clay || rty == shok_ResourceType::Gold || rty == shok_ResourceType::Iron || rty == shok_ResourceType::Stone || rty == shok_ResourceType::Sulfur || rty == shok_ResourceType::Wood;
}

int l_buildingMarketStartTrade(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<shok_GGL_CMarketBehavior>(), "no market at 1");
	luaext_assert(L, b->IsIdle(), "building not idle");
	shok_ResourceType sellty = static_cast<shok_ResourceType>(luaL_checkint(L, 2));
	shok_ResourceType buyty = static_cast<shok_ResourceType>(luaL_checkint(L, 3));
	float am = luaL_checkfloat(L, 4);
	luaext_assert(L, marketIsRes(sellty), "sell type is invalid");
	luaext_assert(L, marketIsRes(buyty), "buy type is invalid");
	luaext_assert(L, am > 0, "amount <= 0");
	b->MarketStartTrade(sellty, buyty, am);
	return 0;
}

int l_buildingMarketCancelTrade(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<shok_GGL_CMarketBehavior>(), "no market at 1");
	luaext_assert(L, b->GetMarketProgress() < 1.0f, "no transaction in progress");
	b->MarketCancelTrade();
	return 0;
}

int l_settlerSetPos(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_position p;
	luaext_checkPos(L, p, 2);
	luaext_assert(L, (*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p) != 0, "position is blocked");
	s->SetPosition(p);
	return 0;
}

int l_leaderSetSoldierLimit(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLimitedAttachmentBehavior* l = s->GetBehavior<shok_GGL_CLimitedAttachmentBehavior>();
	luaext_assertPointer(L, l, "no limited attachment");
	int limit = luaL_checkint(L, 2);
	luaext_assert(L, limit >= 0, "limit < 0");
	auto a = l->AttachmentLimits.GetFirstMatch([](auto a) { return a->AttachmentType == shok_AttachmentType::LEADER_SOLDIER; });
	luaext_assertPointer(L, a, "no matching limited attachment");
	a->Limit = limit;
	return 0;
}

int l_buildingRemoveLastMerchantOffer(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CBuildingMerchantBehavior* m = b->GetBehavior<shok_GGL_CBuildingMerchantBehavior>();
	luaext_assertPointer(L, m, "no merchant");
	luaext_assert(L, m->Offer.size() > 0, "is empty");
	shok_saveVector<shok_GGL_CBuildingMerchantBehavior_COffer*>(&m->Offer, [](std::vector<shok_GGL_CBuildingMerchantBehavior_COffer*, shok_allocator<shok_GGL_CBuildingMerchantBehavior_COffer*>> &a) {
		a.pop_back();
		});
	return 0;
}
int l_buildingMerchantOfferSetData(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CBuildingMerchantBehavior* m = b->GetBehavior<shok_GGL_CBuildingMerchantBehavior>();
	int i = luaL_checkint(L, 2);
	luaext_assertPointer(L, m, "no merchant");
	luaext_assert(L, i >= 0 && i < (int)m->Offer.size(), "invalid index");
	if (lua_isnumber(L, 3)) {
		m->Offer[i]->OffersRemaining = luaL_checkint(L, 3);
	}
	if (lua_istable(L, 4)) {
		bool ig = false;
		if (lua_isboolean(L, 5))
			ig = lua_toboolean(L, 5);
		luaext_readCostInfo(L, 4, m->Offer[i]->ResourceCosts, ig);
	}
	return 0;
}

int l_entitySetMaxHP(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_EGL_CGLEEntity* b = luaext_checkEntity(L, 1);
	shok_EGL_CGLEEntity::HookDestroyEntity();
	shok_EGL_CGLEEntity::HookMaxHP();
	entityAddonData* d = b->GetAdditionalData(true);
	if (lua_isnumber(L, 2)) {
		int mhp = luaL_checkint(L, 2);
		d->HealthOverride = mhp;
	}
	if (lua_isboolean(L, 3)) {
		d->HealthUseBoni = lua_toboolean(L, 3);
	}
	return 0;
};
int l_entityCloneAddData(lua_State* L) {
	shok_EGL_CGLEEntity* fro = luaext_optEntity(L, 1);
	shok_EGL_CGLEEntity* to = luaext_checkEntity(L, 2);
	if (fro)
		to->CloneAdditionalDataFrom(fro->GetAdditionalData(false));
	else if (shok_EGL_CGLEEntity::LastRemovedEntityAddonData.EntityId == luaL_checkint(L, 1))
		to->CloneAdditionalDataFrom(&shok_EGL_CGLEEntity::LastRemovedEntityAddonData);
	return 0;
}
int l_entity_SetDamage(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_EGL_CGLEEntity* b = luaext_checkEntity(L, 1);
	shok_EGL_CGLEEntity::HookDamageMod();
	shok_EGL_CGLEEntity::HookDestroyEntity();
	entityAddonData* d = b->GetAdditionalData(true);
	d->DamageOverride = luaL_checkint(L, 2);
	return 0;
}
int l_entity_SetArmor(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_EGL_CGLEEntity* b = luaext_checkEntity(L, 1);
	shok_EGL_CGLEEntity::HookArmorMod();
	shok_EGL_CGLEEntity::HookDestroyEntity();
	entityAddonData* d = b->GetAdditionalData(true);
	d->ArmorOverride = luaL_checkint(L, 2);
	return 0;
}
int l_entity_SetExploration(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_EGL_CGLEEntity* b = luaext_checkEntity(L, 1);
	shok_EGL_CGLEEntity::HookExplorationMod();
	shok_EGL_CGLEEntity::HookDestroyEntity();
	entityAddonData* d = b->GetAdditionalData(true);
	d->ExplorationOverride = luaL_checkfloat(L, 2);
	return 0;
}
int l_leader_SetRegen(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_GGL_CSettler* b = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, b->GetBehavior<shok_GGL_CLeaderBehavior>(), "no leader");
	shok_EGL_CGLEEntity::HookLeaderRegen();
	shok_EGL_CGLEEntity::HookDestroyEntity();
	entityAddonData* d = b->GetAdditionalData(true);
	if (lua_isnumber(L, 2))
		d->RegenHPOverride = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		d->RegenSecondsOverride = luaL_checkint(L, 3);
	return 0;
}
int l_entity_SetAutoAttackMaxRange(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_EGL_CGLEEntity* b = luaext_checkEntity(L, 1);
	shok_EGL_CGLEEntity::HookMaxRange();
	shok_EGL_CGLEEntity::HookDestroyEntity();
	entityAddonData* d = b->GetAdditionalData(true);
	d->MaxRangeOverride = luaL_checkfloat(L, 2);
	return 0;
}
int l_entity_SetDisplayName(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_EGL_CGLEEntity* b = luaext_checkEntity(L, 1);
	shok_EGL_CGLEEntity::HookDisplayName();
	shok_EGL_CGLEEntity::HookDestroyEntity();
	entityAddonData* d = b->GetAdditionalData(true);
	d->NameOverride = luaL_checkstring(L, 2);
	return 0;
}

int l_leader_GetRegen(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_GGL_CSettler* b = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, b->GetBehavior<shok_GGL_CLeaderBehavior>(), "no leader");
	lua_pushnumber(L, b->LeaderGetRegenHealth());
	lua_pushnumber(L, b->LeaderGetRegenHealthSeconds());
	return 2;
}

int l_settler_EnableRangedEffectSoldierHeal(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	shok_EGL_CGLEEntity::HookRangedEffectActivateHeal(lua_toboolean(L, 1));
	return 0;
}

int l_entity_PerformHeal(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	int h = luaL_checkint(L, 2);
	luaext_assert(L, h > 0, "heal is <=0");
	e->PerformHeal(h, lua_toboolean(L, 3));
	return 0;
}

int l_building_GetBuildOn(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkBulding(L, 1);
	lua_pushnumber(L, e->GetFirstAttachedEntity(shok_AttachmentType::BUILDING_BASE));
	return 1;
}
int l_building_GetBuildOnReverse(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->GetFirstAttachedToMe(shok_AttachmentType::BUILDING_BASE));
	return 1;
}

int l_building_GetConstructionSite(lua_State* L) {
	shok_GGL_CBuilding* e = luaext_checkBulding(L, 1);
	lua_pushnumber(L, e->GetConstructionSite());
	return 1;
}
int l_building_ConSiteGetBuilding(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->GetFirstAttachedEntity(shok_AttachmentType::CONSTRUCTION_SITE_BUILDING));
	return 1;
}

void l_entity_cleanup(lua_State* L) {
	l_settlerDisableConversionHook(L);
	shok_EGL_CGLEEntity::BuildingMaxHpTechBoni.clear();
	shok_EGL_CGLEEntity::AddonDataMap.clear();
	shok_EGL_CGLEEntity::LastRemovedEntityAddonData.EntityId = 0;
	shok_EGL_CGLEEntity::HookRangedEffectActivateHeal(false);
}

void l_entity_init(lua_State* L)
{
	luaext_registerFunc(L, "GetNumberOfAllocatedEntities", &l_entity_getNum);
	luaext_registerFunc(L, "Get", &l_entity_get);
	luaext_registerFunc(L, "EntityIteratorTableize", &l_entityIteratorToTable);
	luaext_registerFunc(L, "EntityIterator", &l_entityIterator);
	luaext_registerFunc(L, "EntityIteratorGetNearest", &l_entityIteratorGetNearest);
	luaext_registerFunc(L, "EntityIteratorCount", &l_entityIteratorCount);
	luaext_registerFunc(L, "GetNumberOfBehaviors", &l_entityGetNumberOfBehaviors);
	luaext_registerFunc(L, "test", &l_entity_test);
	luaext_registerFunc(L, "CheckPredicate", &l_entityCheckPredicate);
	luaext_registerFunc(L, "GetScale", &l_entityGetScale);
	luaext_registerFunc(L, "SetScale", &l_entitySetScale);
	luaext_registerFunc(L, "MovingEntityGetTargetPos", &l_movingEntityGetTargetPos);
	luaext_registerFunc(L, "MovingEntitySetTargetPos", &l_movingEntitySetTargetPos);
	luaext_registerFunc(L, "GetLimitedLifespanRemaining", &l_entityGetLimitedLifespanRemaining);
	luaext_registerFunc(L, "SetLimitedLifespanRemaining", &l_entitySetLimitedLifespanRemaining);
	luaext_registerFunc(L, "GetTaskListIndex", &l_entityGetTaskListIndex);
	luaext_registerFunc(L, "SetTaskListIndex", &l_entitySetTaskListIndex);
	luaext_registerFunc(L, "MovingEntityGetSpeedFactor", &l_movingEntityGetSpeedFactor);
	luaext_registerFunc(L, "IsSoldier", &l_entityIsSoldier);
	luaext_registerFunc(L, "GetAutoAttackMaxRange", &l_entityGetAutoAttackMaxRange);
	luaext_registerFunc(L, "GetModel", &l_entityGetModel);
	luaext_registerFunc(L, "GetExploration", &l_entityGetExploration);
	luaext_registerFunc(L, "GetSpeed", &l_entityGetSpeed);
	luaext_registerFunc(L, "IsFeared", &l_entityIsFeared);
	luaext_registerFunc(L, "ClearAttackers", &l_entityClearAttackers);
	luaext_registerFunc(L, "ReplaceWithResourceEntity", &l_entityReplaceWithResourceEntity);
	luaext_registerFunc(L, "SetMaxHP", &l_entitySetMaxHP);
	luaext_registerFunc(L, "CloneOverrideData", &l_entityCloneAddData);
	luaext_registerFunc(L, "SetDamage", &l_entity_SetDamage);
	luaext_registerFunc(L, "SetArmor", &l_entity_SetArmor);
	luaext_registerFunc(L, "SetExploration", &l_entity_SetExploration);
	luaext_registerFunc(L, "SetAutoAttackMaxRange", &l_entity_SetAutoAttackMaxRange);
	luaext_registerFunc(L, "SetDisplayName", &l_entity_SetDisplayName);
	luaext_registerFunc(L, "PerformHeal", &l_entity_PerformHeal);

	lua_pushstring(L, "Predicates");
	lua_newtable(L);
	luaext_registerFunc(L, "And", &l_entityPredicateAnd);
	luaext_registerFunc(L, "Or", &l_entityPredicateOr);
	luaext_registerFunc(L, "Not", &l_entityPredicateNot);
	luaext_registerFunc(L, "SetPriority", &l_entityPredicatePriority);
	luaext_registerFunc(L, "OfType", &l_entityPredicateOfType);
	luaext_registerFunc(L, "OfPlayer", &l_entityPredicateOfPlayer);
	luaext_registerFunc(L, "InCircle", &l_entityPredicateInCircle);
	luaext_registerFunc(L, "IsBuilding", &l_entityPredicateIsBuilding);
	luaext_registerFunc(L, "IsSettler", &l_entityPredicateIsSettler);
	luaext_registerFunc(L, "IsCombatRelevant", &l_entityPredicateIsRelevant);
	luaext_registerFunc(L, "OfAnyPlayer", &l_entityPredicateAnyPlayer);
	luaext_registerFunc(L, "OfAnyEntityType", &l_entityPredicateAnyEntityType);
	luaext_registerFunc(L, "IsNotSoldier", &l_entityPredicateIsNotSoldier);
	luaext_registerFunc(L, "OfEntityCategory", &l_entityPredicateOfEntityCategory);
	luaext_registerFunc(L, "ProvidesResource", &l_entityPredicateProvidesResource);
	luaext_registerFunc(L, "InRect", &l_entityPredicateInRect);
	luaext_registerFunc(L, "IsVisible", &l_entityPredicateIsVisible);
	luaext_registerFunc(L, "OfUpgradeCategory", &l_entityPredicateOfUpgradeCategory);
	luaext_registerFunc(L, "IsAlive", &l_entityPredicateIsAlive);
	lua_rawset(L, -3);

	lua_pushstring(L, "Settler");
	lua_newtable(L);
	luaext_registerFunc(L, "GetLeaderOfSoldier", &l_settlerGetLeaderOfSoldier);
	luaext_registerFunc(L, "GetBaseMovementSpeed", &l_settlerGetBaseMovementSpeed);
	luaext_registerFunc(L, "SetBaseMovementSpeed", &l_settlerSetBaseMovementSpeed);
	luaext_registerFunc(L, "GetOverheadWidget", &l_settlerGetOvereadWidget);
	luaext_registerFunc(L, "SetOverheadWidget", &l_settlerSetOvereadWidget);
	luaext_registerFunc(L, "HeroGetCamouflageDurationLeft", &l_settlerHeroCamouflageGetDurationLeft);
	luaext_registerFunc(L, "HeroSetCamouflageDurationLeft", &l_settlerHeroCamouflageSetDurationLeft);
	luaext_registerFunc(L, "ThiefGetCamouflageTimeTo", &l_settlerThiefCamouflageGetTimeTo);
	luaext_registerFunc(L, "ThiefSetCamouflageTimeTo", &l_settlerThiefCamouflageSetTimeTo);
	luaext_registerFunc(L, "HeroGetResurrectionTime", &l_settlerHeroGetResurrectionTime);
	luaext_registerFunc(L, "HeroSetResurrectionTime", &l_settlerHeroSetResurrectionTime);
	luaext_registerFunc(L, "WorkerGetCurrentWorkTime", &l_settlerGetWorkerCurrentWorkTime);
	luaext_registerFunc(L, "WorkerSetCurrentWorkTime", &l_settlerSetWorkerCurrentWorkTime);
	luaext_registerFunc(L, "HeroResurrect", &l_heroResurrect);
	luaext_registerFunc(L, "ThiefSetStolenResourceInfo", &l_settlerThiefSetStolenResourceInfo);
	luaext_registerFunc(L, "IsVisible", &l_settlerIsVisible);
	luaext_registerFunc(L, "IsIdle", &l_settlerIsIdle);
	luaext_registerFunc(L, "CommandSendHawk", &l_settlerSendHawk);
	luaext_registerFunc(L, "CommandInflictFear", &l_settlerInflictFear);
	luaext_registerFunc(L, "CommandPlaceBomb", &l_settlerPlaceBomb);
	luaext_registerFunc(L, "CommandPlaceCannon", &l_settlerPlaceCannon);
	luaext_registerFunc(L, "CommandRangedEffect", &l_settlerRangedEffect);
	luaext_registerFunc(L, "CommandCircularAttack", &l_settlerCircularAttack);
	luaext_registerFunc(L, "CommandSummon", &l_settlerSummon);
	luaext_registerFunc(L, "CommandActivateCamoflage", &l_settlerActivateCamo);
	luaext_registerFunc(L, "CommandConvert", &l_settlerConvert);
	luaext_registerFunc(L, "CommandSnipe", &l_settlerSnipe);
	luaext_registerFunc(L, "CommandShuriken", &l_settlerShuriken);
	luaext_registerFunc(L, "CommandMotivateWorkers", &l_settlerMotivateWorkers);
	luaext_registerFunc(L, "CommandSabotage", &l_settlerSabotage);
	luaext_registerFunc(L, "CommandDefuse", &l_settlerDefuse);
	luaext_registerFunc(L, "CommandBinocular", &l_settlerBinoculars);
	luaext_registerFunc(L, "CommandPlaceTorch", &l_settlerPlaceTorch);
	luaext_registerFunc(L, "CommandPointToRes", &l_settlerPointToRes);
	luaext_registerFunc(L, "CommandStealFrom", &l_settlerStealFrom);
	luaext_registerFunc(L, "CommandSecureGoods", &l_settlerSecureGoods);
	luaext_registerFunc(L, "EnableConversionHook", &l_settlerEnableConversionHook);
	luaext_registerFunc(L, "DisableConversionHook", &l_settlerDisableConversionHook);
	luaext_registerFunc(L, "CommandMove", &l_settlerMove);
	luaext_registerFunc(L, "CommandSerfConstructBuilding", &l_settlerSerfConstruct);
	luaext_registerFunc(L, "CommandSerfRepairBuilding", &l_settlerSerfRepair);
	luaext_registerFunc(L, "CommandSerfExtract", &l_settlerSerfExtract);
	luaext_registerFunc(L, "CommandExpell", &l_settlerExpell);
	luaext_registerFunc(L, "CommandTurnSerfToBattleSerf", &l_settlerSerfToBattleSerf);
	luaext_registerFunc(L, "CommandTurnBattleSerfToSerf", &l_settlerBattleSerfToSerf);
	luaext_registerFunc(L, "SetPosition", &l_settlerSetPos);
	luaext_registerFunc(L, "SetPosition", &l_settlerSetPos);
	luaext_registerFunc(L, "EnableRangedEffectSoldierHeal", &l_settler_EnableRangedEffectSoldierHeal);
	lua_rawset(L, -3);

	lua_pushstring(L, "Leader");
	lua_newtable(L);
	luaext_registerFunc(L, "GetExperience", &l_leaderGetLeaderXP);
	luaext_registerFunc(L, "SetExperience", &l_leaderSetLeaderXP);
	luaext_registerFunc(L, "GetTroopHealth", &l_leaderGetLeaderHP);
	luaext_registerFunc(L, "SetTroopHealth", &l_leaderSetLeaderHP);
	luaext_registerFunc(L, "AttachSoldier", &l_leaderAttachSoldier);
	luaext_registerFunc(L, "SetSoldierLimit", &l_leaderSetSoldierLimit);
	luaext_registerFunc(L, "SetRegeneration", &l_leader_SetRegen);
	luaext_registerFunc(L, "GetRegeneration", &l_leader_GetRegen);
	lua_rawset(L, -3);

	lua_pushstring(L, "Building");
	lua_newtable(L);
	luaext_registerFunc(L, "GetBarracksAutoFillActive", &l_buildingGetAutoFillActive);
	luaext_registerFunc(L, "GetHeight", &l_buildingGetHeight);
	luaext_registerFunc(L, "SetHeight", &l_buildingSetHeight);
	luaext_registerFunc(L, "MarketGetCurrentTradeData", &l_buildingGetCurrentTradeInfo);
	luaext_registerFunc(L, "MarketSetCurrentTradeData", &l_buildingSetCurrentTradeInfo);
	luaext_registerFunc(L, "CommandFoundryBuildCannon", &l_buildingFoundryBuildCannon);
	luaext_registerFunc(L, "GetConstructionSite", &l_buildingGetConstructionSite);
	luaext_registerFunc(L, "GetNearestFreeConstructionSlotFor", &l_buildingGetNextFreeConstructionSlotFor);
	luaext_registerFunc(L, "GetNearestFreeRepairSlotFor", &l_buildingGetNextFreeRepairSlotFor);
	luaext_registerFunc(L, "GetNearestFreeRepairSlotFor", &l_buildingGetNextFreeRepairSlotFor);
	luaext_registerFunc(L, "StartUpgrade", &l_buildingStartUpgrade);
	luaext_registerFunc(L, "CancelUpgrade", &l_buildingCancelUpgrade);
	luaext_registerFunc(L, "IsIdle", &l_buildingIsIdle);
	luaext_registerFunc(L, "BarracksGetLeadersTrainingAt", &l_buildingBarracksGetLeadersTrainingAt);
	luaext_registerFunc(L, "FoundryGetCannonTypeInConstruction", &l_buildingFoundryGetCannonTypeInConstruction);
	luaext_registerFunc(L, "BarracksBuySoldierForLeader", &l_buildingBuySoldierForLeader);
	luaext_registerFunc(L, "ActivateOvertime", &l_buildingActivateOvertime);
	luaext_registerFunc(L, "DeactivateOvertime", &l_buildingDeactivateOvertime);
	luaext_registerFunc(L, "BarracksRecruitGroups", &l_buildingBarracksRecruitGroups);
	luaext_registerFunc(L, "BarracksRecruitLeaders", &l_buildingBarracksRecruitLeaders);
	luaext_registerFunc(L, "HQBuySerf", &l_buildingHQBuySerf);
	luaext_registerFunc(L, "SellBuilding", &l_buildingSell);
	luaext_registerFunc(L, "StartResearch", &l_buildingStartResearch);
	luaext_registerFunc(L, "CancelResearch", &l_buildingCancelResearch);
	luaext_registerFunc(L, "MarketStartTrade", &l_buildingMarketStartTrade);
	luaext_registerFunc(L, "MercenaryRemoveLastOffer", &l_buildingRemoveLastMerchantOffer);
	luaext_registerFunc(L, "MercenarySetOfferData", &l_buildingMerchantOfferSetData);
	luaext_registerFunc(L, "GetBuildOnEntity", &l_building_GetBuildOn);
	luaext_registerFunc(L, "BuildOnEntityGetBuilding", &l_building_GetBuildOnReverse);
	luaext_registerFunc(L, "GetConstructionSite", &l_building_GetConstructionSite);
	luaext_registerFunc(L, "ConstructionSiteGetBuilding", &l_building_ConSiteGetBuilding);
	lua_rawset(L, -3);
}

// CppLogic.Entity.CheckPredicate(GUI.GetEntityAtPosition(GUI.GetMousePosition()), CppLogic.Entity.Predicates.ProvidesResource(ResourceType.WoodRaw))
// CppLogic.Entity.test(GUI.GetEntityAtPosition(GUI.GetMousePosition()), EntityCategories.Sword)
// CppLogic.Entity.GetNumberOfAllocatedEntities()
// CppLogic.Entity.GetNumberOfBehaviors(GUI.GetEntityAtPosition(GUI.GetMousePosition()))
// CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.IsSettler(), CppLogic.Entity.Predicates.IsNotSoldier(), CppLogic.Entity.Predicates.OfPlayer(1))
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.InCircle(x,y, 1000))
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Entity.EntityIteratorGetNearest(CppLogic.Entity.Predicates.InCircle(x,y, 1000))
// local x,y = GUI.Debug_GetMapPositionUnderMouse();  for id,r in CppLogic.Entity.EntityIterator(CppLogic.Entity.Predicates.OfAnyEntityType(Entities.PU_Serf, Entities.PB_Headquarters1), CppLogic.Entity.Predicates.InCircle(x,y, 1000)) do LuaDebugger.Log(id.."   "..r) end
// CppLogic.Entity.Settler.GetBaseMovementSpeed(GUI.GetSelectedEntity())
// CppLogic.Entity.MovingEntityGetTargetPos(GUI.GetSelectedEntity())
// local x,y = GUI.Debug_GetMapPositionUnderMouse() CppLogic.Entity.MovingEntitySetTargetPos(GUI.GetSelectedEntity(), {X=x,Y=y})

EntityIterator::EntityIterator(EntityIteratorPredicate* Predicate)
{
	this->Predicate = Predicate;
	Reset();
}

void EntityIterator::Reset()
{
	current = 0;
}

shok_EGL_CGLEEntity* EntityIterator::GetNext(float* rangeOut, int* prio)
{
	shok_EGL_CGLEEntityManager* eman = *shok_EGL_CGLEEntityManager::GlobalObj;
	while (true) {
		if (current > eman->EntityCount)
			return nullptr;
		shok_EGL_CGLEEntity* e = eman->GetEntityByNum(current);
		current++;
		if (e != nullptr && Predicate->MatchesEntity(e, rangeOut, prio))
			return e;
	}
	return nullptr;
}
shok_EGL_CGLEEntity* EntityIterator::GetNearest(float* rangeOut)
{
	shok_EGL_CGLEEntity* curr, * ret = nullptr;
	float currR = -1, maxR = -1;
	int currPrio = -1, maxPrio = -1;
	Reset();
	curr = GetNext(&currR, &currPrio);
	while (curr != nullptr) {
		if (currPrio > maxPrio || maxR == -1 || (currR < maxR && currPrio==maxPrio)) {
			ret = curr;
			maxR = currR;
			maxPrio = currPrio;
		}
		currPrio = -1;
		currR = -1;
		curr = GetNext(&currR, &currPrio);
	}
	if (rangeOut != nullptr)
		*rangeOut = maxR;
	return ret;
}
shok_EGL_CGLEEntity* EntityIterator::GetFurthest(float* rangeOut)
{
	shok_EGL_CGLEEntity* curr, * ret = nullptr;
	float currR = -1, maxR = -1;
	int currPrio = -1, maxPrio = -1;
	Reset();
	curr = GetNext(&currR, &currPrio);
	while (curr != nullptr) {
		if (currPrio > maxPrio || maxR == -1 || (currR > maxR && currPrio == maxPrio)) {
			ret = curr;
			maxR = currR;
			maxPrio = currPrio;
		}
		currPrio = -1;
		currR = -1;
		curr = GetNext(&currR, &currPrio);
	}
	if (rangeOut != nullptr)
		*rangeOut = maxR;
	return ret;
}

EntityIteratorPredicateOfType::EntityIteratorPredicateOfType(int etype)
{
	type = etype;
}

bool EntityIteratorPredicateOfType::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->EntityType == type;
}

bool EntityIteratorPredicateOfPlayer::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->PlayerId == player;
}

EntityIteratorPredicateOfPlayer::EntityIteratorPredicateOfPlayer(int player)
{
	this->player = player;
}

bool EntityIteratorPredicateAnd::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	for (int i = 0; i < numPreds; i++) {
		if (!predicates[i]->MatchesEntity(e, rangeOut, prio))
			return false;
	}
	return true;
}

EntityIteratorPredicateAnd::EntityIteratorPredicateAnd(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateOr::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	for (int i = 0; i < numPreds; i++) {
		if (predicates[i]->MatchesEntity(e, rangeOut, prio))
			return true;
	}
	return false;
}

EntityIteratorPredicateOr::EntityIteratorPredicateOr(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateInCircle::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	float ran = p.GetDistanceSquaredTo(e->Position);
	if (rangeOut != nullptr)
		*rangeOut = ran;
	return ran <= r;
}

EntityIteratorPredicateInCircle::EntityIteratorPredicateInCircle(float x, float y, float r)
{
	this->p = { x,y };
	this->r = r * r;
}
EntityIteratorPredicateInCircle::EntityIteratorPredicateInCircle(shok_position& p, float r)
{
	this->p = p;
	this->r = r * r;
}

bool EntityIteratorPredicateIsSettler::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e);
}

bool EntityIteratorPredicateIsBuilding::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CBuilding>(e);
}

bool EntityIteratorPredicateIsRelevant::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->PlayerId != 0 && (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e) || shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e));
}

bool EntityIteratorPredicateAnyPlayer::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	int pl = e->PlayerId;
	for (int i = 0; i < numPlayers; i++) {
		if (pl == players[i])
			return true;
	}
	return false;
}
void EntityIteratorPredicateAnyPlayer::FillHostilePlayers(int source, int* players, int& maxP)
{
	shok_GGL_CPlayerStatus* pl = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(source);
	int curr = 0;
	for (int p = 1; p <= maxP; p++) {
		if (pl->GetDiploStateTo(p) == shok_DiploState::Hostile) {
			players[curr] = p;
			curr++;
		}
	}
	maxP = curr;
}

EntityIteratorPredicateAnyPlayer::EntityIteratorPredicateAnyPlayer(int* pl, int numPlayers)
{
	players = pl;
	this->numPlayers = numPlayers;
}

bool EntityIteratorPredicateAnyEntityType::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	int pl = e->EntityType;
	for (int i = 0; i < numTypes; i++) {
		if (pl == types[i])
			return true;
	}
	return false;
}

EntityIteratorPredicateAnyEntityType::EntityIteratorPredicateAnyEntityType(int* ty, int numTy)
{
	types = ty;
	numTypes = numTy;
}

bool EntityIteratorPredicateIsNotSoldier::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->GetBehavior<shok_GGL_CSoldierBehavior>() == nullptr;
}

bool EntityIteratorPredicateOfEntityCategory::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->IsEntityInCategory(category);
}

EntityIteratorPredicateOfEntityCategory::EntityIteratorPredicateOfEntityCategory(shok_EntityCategory cat)
{
	category = cat;
}

bool EntityIteratorPredicateProvidesResource::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->GetResourceProvided() == resource;
}

EntityIteratorPredicateProvidesResource::EntityIteratorPredicateProvidesResource(int res)
{
	resource = res;
}

bool EntityIteratorPredicateInRect::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return low.X <= e->Position.X && e->Position.X <= high.X && low.Y <= e->Position.Y && e->Position.Y <= high.Y;
}

EntityIteratorPredicateInRect::EntityIteratorPredicateInRect(float x1, float y1, float x2, float y2)
{
	low.X = min(x1, x2);
	low.Y = min(y1, y2);
	high.X = max(x1, x2);
	high.Y = max(y1, y2);
}

bool EntityIteratorPredicateNot::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return !predicate->MatchesEntity(e, rangeOut, prio);
}

EntityIteratorPredicateNot::EntityIteratorPredicateNot(EntityIteratorPredicate* pred)
{
	predicate = pred;
}

bool EntityIteratorPredicateIsVisible::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	shok_GGL_CCamouflageBehavior* c = e->GetBehavior<shok_GGL_CCamouflageBehavior>();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}

bool EntityIteratorPredicateOfUpgradeCategory::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	shok_GGlue_CGlueEntityProps* t = e->GetEntityType();
	if (t->IsBuildingType()) {
		shok_GGL_CGLBuildingProps* p = (shok_GGL_CGLBuildingProps*)t;
		return p->Upgrade.Category == category;
	}
	if (t->IsSettlerType()) {
		shok_GGL_CGLSettlerProps* p = (shok_GGL_CGLSettlerProps*)t;
		return p->Upgrade.Category == category;
	}
	return false;
}

EntityIteratorPredicateOfUpgradeCategory::EntityIteratorPredicateOfUpgradeCategory(int cat)
{
	category = cat;
}

bool EntityIteratorPredicateIsAlive::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return !shok_EGL_CGLEEntity::EntityIDIsDead(e->EntityId); // performance improvement by not doing obj -> id ->obj ?
}

bool EntityIteratorPredicateIsNotFleeingFrom::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return IsNotFleeingFrom(e, Center, Range);
}

bool EntityIteratorPredicateIsNotFleeingFrom::IsNotFleeingFrom(shok_EGL_CGLEEntity* e, shok_position Center, float Range)
{
	if (!shok_DynamicCast<shok_EGL_CGLEEntity, shok_EGL_CMovingEntity>(e))
		return true;
	if (e->GetFirstAttachedToMe(shok_AttachmentType::INFLICTOR_TERRORIZED) != 0)
		return false;
	shok_EGL_CMovingEntity* me = (shok_EGL_CMovingEntity*)e;
	float dx1 = Center.X - e->Position.X;
	float dy1 = Center.Y - e->Position.Y;
	float dx2 = Center.X - me->TargetPosition.X;
	float dy2 = Center.Y - me->TargetPosition.Y;
	return std::sqrtf(dx1 * dx1 + dy1 * dy1) + Range >= std::sqrtf(dx2 * dx2 + dy2 * dy2);
}

EntityIteratorPredicateIsNotFleeingFrom::EntityIteratorPredicateIsNotFleeingFrom(shok_position& p, float r)
{
	Center = p;
	Range = r;
}

bool EntityIteratorPredicateFunc::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return func(e);
}
EntityIteratorPredicateFunc::EntityIteratorPredicateFunc(std::function<bool(shok_EGL_CGLEEntity* e)> f)
{
	func = f;
}

bool EntityIteratorPredicatePriority::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	if (prio && Pred->MatchesEntity(e, rangeOut, nullptr))
		*prio = Prio;
	return true;
}
EntityIteratorPredicatePriority::EntityIteratorPredicatePriority(int prio, EntityIteratorPredicate* pred)
{
	Prio = prio;
	Pred = pred;
}

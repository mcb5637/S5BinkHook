#include "pch.h"
#include "l_entity.h"
#include "s5data.h"
#include "luaext.h"

int l_entity_getNum(lua_State* L) {
	lua_pushnumber(L, (*shok_EGL_CGLEEntityManagerObj)->EntityCount);
	return 1;
}

int l_entity_get(lua_State* L) {
	int i = luaL_checkint(L, 1);
	shok_EGL_CGLEEntity* p = (*shok_EGL_CGLEEntityManagerObj)->GetEntityByNum(i);
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
	int* pred = (int*)&((EntityIteratorPredicateAnyPlayer*)ud)[1]; // get start address of array
	for (int i = 0; i < num; i++) {
		pred[i] = luaL_checkint(L, i+1);
	}
	new(ud) EntityIteratorPredicateAnyPlayer(pred, num);
	return 1;
}

int l_entityPredicateAnyEntityType(lua_State* L) {
	int num = lua_gettop(L);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateAnyEntityType) + sizeof(int) * num);
	int* pred = (int*)&((EntityIteratorPredicateAnyEntityType*)ud)[1]; // get start address of array
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
	EntityIteratorPredicate** pred = (EntityIteratorPredicate**)&((EntityIteratorPredicateAnd*)ud)[1]; // get start address of predicate array
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
	EntityIteratorPredicate** pred = (EntityIteratorPredicate**)&((EntityIteratorPredicateOr*)ud)[1]; // get start address of predicate array
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
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, -1);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateNot));
	new(ud) EntityIteratorPredicateNot(pred);
	lua_newtable(L);
	lua_pushvalue(L, 1);// keep predicate as metatable, so they dont get gced
	lua_rawseti(L, 2, 1);
	lua_setmetatable(L, 2);
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
	new(ud) EntityIteratorPredicateOfEntityCategory(ty);
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
	EntityIterator it = EntityIterator(pred);
	shok_EGL_CGLEEntity* e = nullptr;
	while (true) {
		e = it.GetNext(nullptr);
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
	EntityIterator it = EntityIterator(pred);
	shok_EGL_CGLEEntity* e = nullptr;
	while (true) {
		e = it.GetNext(nullptr);
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
	EntityIterator it = EntityIterator(pred);
	shok_EGL_CGLEEntity* e = nullptr;
	float currentR = -1;
	float maxR = -1;
	int maxID = -1;
	while (true) {
		e = it.GetNext(&currentR);
		if (e == nullptr)
			break;
		if (maxR <= -1 || currentR < maxR) {
			maxID = e->EntityId;
			maxR = currentR;
		}
	}
	lua_pushnumber(L, maxID);
	lua_pushnumber(L, maxR);
	return 2;
}

int l_entityIteratorNext(lua_State* L) { // (state, last value) -> next value
	EntityIterator* it = (EntityIterator*)lua_touserdata(L, 1); // no error checking here, cause that would cost speed
	float r = -1;
	shok_EGL_CGLEEntity* e = it->GetNext(&r);
	if (e == nullptr) {
		lua_pushnil(L);
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, e->EntityId);
		lua_pushnumber(L, r);
	}
	return 2;
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
	lua_pushboolean(L, pred->MatchesEntity(s, nullptr));
	return 1;
}

int l_settlerGetLeaderOfSoldier(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	lua_pushnumber(L, s->LeaderId);
	return 1;
}

int l_entityGetNumberOfBehaviors(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->BehaviorList.size());
	return 1;
}

int l_leaderGetLeaderXP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetLeaderBehavior();
	luaext_assertPointer(L, b, "no leader at 1");
	lua_pushnumber(L, b->Experience);
	return 1;
}
int l_leaderSetLeaderXP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetLeaderBehavior();
	luaext_assertPointer(L, b, "no leader at 1");
	b->Experience = luaL_checkint(L, 2);
	return 0;
}

int l_leaderGetLeaderHP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetLeaderBehavior();
	luaext_assertPointer(L, b, "no leader at 1");
	lua_pushnumber(L, b->TroopHealthCurrent);
	return 1;
}
int l_leaderSetLeaderHP(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CLeaderBehavior* b = s->GetLeaderBehavior();
	luaext_assertPointer(L, b, "no leader at 1");
	b->TroopHealthCurrent = luaL_checkint(L, 2);
	return 0;
}

int l_settlerGetBaseMovementSpeed(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CBehaviorDefaultMovement* b = s->GetMovementBehavior();
	luaext_assertPointer(L, b, "no moving entity at 1");
	lua_pushnumber(L, b->MovementSpeed);
	lua_pushnumber(L, rad2deg((double)b->TurningSpeed));
	return 2;
}
int l_settlerSetBaseMovementSpeed(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CBehaviorDefaultMovement* b = s->GetMovementBehavior();
	luaext_assertPointer(L, b, "no moving entity at 1");
	if (lua_isnumber(L, 2))
		b->MovementSpeed = luaL_checkfloat(L, 2);
	if (lua_isnumber(L, 3))
		b->TurningSpeed = (float)deg2rad(lua_tonumber(L, 3));
	return 0;
}

int l_buildingGetAutoFillActive(lua_State* L) {
	shok_GGL_CBuilding* e = luaext_checkBulding(L, 1);
	shok_GGL_CBarrackBehavior* b = e->GetBarrackBehavior();
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
	luaext_assert(L, e->IsMovingEntity(), "no moving entity at 1");
	shok_EGL_CMovingEntity* m = (shok_EGL_CMovingEntity*)e;
	luaext_pushPos(L, m->TargetPosition);
	if (m->TargetRotationValid) {
		lua_pushstring(L, "r");
		lua_pushnumber(L, rad2deg((double)m->TargetRotation));
		lua_rawset(L, -3);
	}
	return 1;
}
int l_movingEntitySetTargetPos(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	luaext_assert(L, e->IsMovingEntity(), "no moving entity at 1");
	shok_EGL_CMovingEntity* m = (shok_EGL_CMovingEntity*)e;
	luaext_checkPos(L, m->TargetPosition, 2);
	lua_pushstring(L, "r");
	lua_gettable(L, 2);
	if (lua_isnumber(L, -1)) {
		m->TargetRotation = (float)deg2rad(lua_tonumber(L, -1));
		m->TargetRotationValid = 1;
	}
	else {
		m->TargetRotationValid = 0;
	}
	return 0;
}

int l_settlerHeroCamouflageGetDurationLeft(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CCamouflageBehavior* c = s->GetCamoAbilityBehavior();
	luaext_assertPointer(L, c, "no camo hero at 1");
	lua_pushnumber(L, c->InvisibilityRemaining);
	return 1;
}
int l_settlerHeroCamouflageSetDurationLeft(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CCamouflageBehavior* c = s->GetCamoAbilityBehavior();
	luaext_assertPointer(L, c, "no camo hero at 1");
	luaext_assert(L, c->vtable != shok_vtp_GGL_CThiefCamouflageBehavior, "thief at 1, use ThiefSetCamouflageTimeTo instead");
	c->InvisibilityRemaining = luaL_checkint(L, 2);
	return 1;
}

int l_settlerThiefCamouflageGetTimeTo(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CThiefCamouflageBehavior* c = s->GetCamoThiefBehavior();
	luaext_assertPointer(L, c, "no thief at 1");
	lua_pushnumber(L, c->TimeToInvisibility);
	return 1;
}
int l_settlerThiefCamouflageSetTimeTo(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CThiefCamouflageBehavior* c = s->GetCamoThiefBehavior();
	luaext_assertPointer(L, c, "no thief at 1");
	int i = luaL_checkint(L, 2);
	c->TimeToInvisibility = i;
	c->InvisibilityRemaining = i <= 0 ? 15 : 0;
	return 0;
}

int l_settlerHeroGetResurrectionTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CHeroBehavior* h = s->GetHeroBehavior();
	luaext_assertPointer(L, h, "no hero at 1");
	lua_pushnumber(L, h->ResurrectionTimePassed);
	return 1;
}
int l_settlerHeroSetResurrectionTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CHeroBehavior* h = s->GetHeroBehavior();
	luaext_assertPointer(L, h, "no hero at 1");
	h->ResurrectionTimePassed = luaL_checkint(L, 2);
	return 0;
}

int l_entityGetLimitedLifespanRemaining(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CLimitedLifespanBehavior* l = e->GetLimitedLifespanBehavior();
	luaext_assertPointer(L, l, "no limited lifespan at 1");
	lua_pushnumber(L, l->RemainingLifespanSeconds);
	return 1;
}
int l_entitySetLimitedLifespanRemaining(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CLimitedLifespanBehavior* l = e->GetLimitedLifespanBehavior();
	luaext_assertPointer(L, l, "no limited lifespan at 1");
	l->RemainingLifespanSeconds = luaL_checkint(L, 2);
	return 0;
}

void(__thiscall* FireEvent)(shok_EGL_CGLEEntity* th, shok_event_data* d);
void __fastcall fireeventhook(shok_EGL_CGLEEntity* th, int _, shok_event_data* d) {
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
	/*shok_vtable_EGL_CGLEEntity* vt = (shok_vtable_EGL_CGLEEntity*)e->vtable;
	FireEvent = vt->FireEvent;
	vt->FireEvent = (void(__thiscall *)(shok_EGL_CGLEEntity * th, shok_event_data * d)) &fireeventhook;*/
	//lua_pushnumber(L, (int)&e->GetConvertSettlerBehavior()->TimeToConvert);
	//std::multimap<int, int> m = std::multimap < int, int >();
	//DEBUGGER_BREAK
	return 0;
}

int l_entityGetTaskListIndex(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->TaskIndex);
	return 1;
}
int l_entitySetTaskListIndex(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	e->TaskIndex = luaL_checkint(L, 2);
	return 0;
}

int l_movingEntityGetSpeedFactor(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CBehaviorDefaultMovement* m = e->GetMovementBehavior();
	luaext_assertPointer(L, m, "no moving entity at 1");
	lua_pushnumber(L, m->SpeedFactor);
	return 1;
}

int l_settlerGetWorkerCurrentWorkTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CWorkerBehavior* w = s->GetWorkerBehavior();
	luaext_assertPointer(L, w, "no worker at 1");
	lua_pushnumber(L, w->WorkTimeRemaining);
	return 1;
}
int l_settlerSetWorkerCurrentWorkTime(lua_State* L) {
	shok_GGL_CSettler* s = luaext_checkSettler(L, 1);
	shok_GGL_CWorkerBehavior* w = s->GetWorkerBehavior();
	luaext_assertPointer(L, w, "no worker at 1");
	w->WorkTimeRemaining = luaL_checkint(L, 2);
	return 0;
}

int l_buildingGetCurrentTradeInfo(lua_State* L) {
	shok_GGL_CBuilding* b = luaext_checkBulding(L, 1);
	shok_GGL_CMarketBehavior* m = b->GetMarketBehavior();
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
	shok_GGL_CMarketBehavior* m = b->GetMarketBehavior();
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
	lua_pushboolean(L, e->GetSoldierBehavior() != nullptr);
	return 1;
}

int l_heroResurrect(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CHeroBehavior* h = e->GetHeroBehavior();
	luaext_assertPointer(L, h, "no hero at 1");
	h->ResurrectionTimePassed = 20000;
	h->EnemyNear = 0;
	h->FriendNear = 1;
	return 0;
}

int l_settlerThiefSetStolenResourceInfo(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CThiefBehavior* t = e->GetThiefBehavior();
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
	shok_GGL_CBattleBehavior* b = e->GetBattleBehavior();
	luaext_assertPointer(L, b, "no battle entity at 1");
	lua_pushnumber(L, b->GetMaxRange());
	return 1;
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
	shok_GGL_CBehaviorDefaultMovement* m = e->GetMovementBehavior();
	luaext_assertPointer(L, m, "no moving entity at 1");
	lua_pushnumber(L, m->GetMovementSpeed() * 10.0);
	return 1;
}

int l_settlerIsVisible(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_GGL_CCamouflageBehavior* c = e->GetCamoAbilityBehavior();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}

int l_settlerSendHawk(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CHeroHawkBehavior* b = e->GetHeroHawkBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CHeroHawkBehaviorProps* bp = e->GetEntityType()->GetHeroHawkBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->HeroAbilitySendHawk(p);
	return 0;
}

int l_settlerInflictFear(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CInflictFearAbility* b = e->GetInflictFearBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CInflictFearAbilityProps* bp = e->GetEntityType()->GetInflictFearBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityInflictFear();
	return 0;
}

int l_settlerPlaceBomb(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CBombPlacerBehavior* b = e->GetBombPlacerBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CHeroAbilityProps* bp = e->GetEntityType()->GetBombPlacerBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->HeroAbilityPlaceBomb(p);
	return 0;
}

int l_settlerPlaceCannon(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CCannonBuilderBehavior* b = e->GetCannonBuilderBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CCannonBuilderBehaviorProps* bp = e->GetEntityType()->GetCannonBuilderBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	p.FloorToBuildingPlacement();
	int bottom = luaL_checkint(L, 3);
	shok_GGlue_CGlueEntityProps* ety = (*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(bottom);
	luaext_assertPointer(L, ety, "no bottom entitytype");
	luaext_assert(L, ety->IsBuildingType(), "bottom not a building");
	int top = luaL_checkint(L, 4);
	luaext_assertPointer(L, (*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(top), "no top entitytype");
	luaext_assert(L, shok_canPlaceBuilding(bottom, e->PlayerId, &p, 0, 0), "cannot place foundation at that position");
	e->HeroAbilityPlaceCannon(p, bottom, top);
	return 0;
}

int l_settlerRangedEffect(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CRangedEffectAbility* b = e->GetRangedEffectBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CRangedEffectAbilityProps* bp = e->GetEntityType()->GetRangedEffectBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityRangedEffect();
	return 0;
}

int l_settlerCircularAttack(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CCircularAttack* b = e->GetCircularAttackBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CCircularAttackProps* bp = e->GetEntityType()->GetCircularAttackBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityCircularAttack();
	return 0;
}

int l_settlerSummon(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CSummonBehavior* b = e->GetSummonBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CSummonBehaviorProps* bp = e->GetEntityType()->GetSummonBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilitySummon();
	return 0;
}

int l_settlerConvert(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CConvertSettlerAbility* b = e->GetConvertSettlerBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CConvertSettlerAbilityProps* bp = e->GetEntityType()->GetConvertSettlersBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_GGL_CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	e->HeroAbilityConvert(t->EntityId);
	return 0;
}

int l_settlerSnipe(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CSniperAbility* b = e->GetSniperBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CSniperAbilityProps* bp = e->GetEntityType()->GetSniperBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
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
	shok_GGL_CShurikenAbility* b = e->GetShurikenBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CShurikenAbilityProps* bp = e->GetEntityType()->GetShurikenBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_GGL_CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	luaext_assert(L, e->Position.IsInRange(t->Position, bp->Range), "target not in range");
	e->HeroAbilityShuriken(t->EntityId);
	return 0;
}

int l_settlerSabotage(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CKegPlacerBehavior* b = e->GetKegPlacerBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CKegPlacerBehaviorProperties* bp = e->GetEntityType()->GetKegPlacerBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	luaext_assert(L, e->GetThiefBehavior()->ResourceType == 0, "is carrying resources");
	shok_GGL_CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, t->IsEntityInCategory(EntityCategoryBridge) || ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile or bridge");
	e->ThiefSabotage(t->EntityId);
	return 0;
}

int l_settlerDefuse(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CKegPlacerBehavior* b = e->GetKegPlacerBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, e->GetThiefBehavior()->ResourceType == 0, "is carrying resources");
	shok_EGL_CGLEEntity* t = luaext_checkEntity(L, 2);
	luaext_assertPointer(L, t->GetKegBehavior(), "no keg at 2");
	e->ThiefDefuse(t->EntityId);
	return 0;
}

int l_settlerBinoculars(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CAbilityScoutBinocular* b = e->GetBinocularBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CAbilityScoutBinocularProps* bp = e->GetEntityType()->GetBinocularBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->ScoutBinoculars(p);
	return 0;
}

int l_settlerPlaceTorch(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CTorchPlacerBehavior* b = e->GetTorchPlacerBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CTorchPlacerBehaviorProperties* bp = e->GetEntityType()->GetTorchPlacerBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok_position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->ScoutPlaceTorch(p);
	return 0;
}

int l_settlerPointToRes(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CPointToResourceBehavior* b = e->GetPointToResBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	shok_GGL_CPointToResourceBehaviorProperties* bp = e->GetEntityType()->GetPointToResBehaviorProps();
	luaext_assert(L, b->AbilitySecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->ScoutFindResource();
	return 0;
}

int l_settlerStealFrom(lua_State* L) {
	shok_GGL_CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	shok_GGL_CThiefBehavior* b = e->GetThiefBehavior();
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
	shok_GGL_CThiefBehavior* b = e->GetThiefBehavior();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, b->ResourceType != 0, "no resources carried");
	shok_GGL_CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, e->PlayerId == t->PlayerId, "entities are not of same player");
	luaext_assert(L, t->IsEntityInCategory(EntityCategoryHeadquarters), "target not hq");
	e->ThiefSecureGoods(t->EntityId);
	return 0;
}

int ConversionHookRef = LUA_NOREF;
int l_settlerEnableConversionHook(lua_State* L) {
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	lua_pushvalue(L, 1);
	luaL_unref(L, LUA_REGISTRYINDEX, ConversionHookRef);
	ConversionHookRef = luaL_ref(L, LUA_REGISTRYINDEX);
	HookHero6Convert();
	Hero6ConvertHookCb = [](int id, int pl, bool post, int converter) {
		lua_State* L = *shok_luastate_game;
		int t = lua_gettop(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, ConversionHookRef);
		lua_pushnumber(L, id);
		lua_pushnumber(L, pl);
		lua_pushboolean(L, post);
		lua_pushnumber(L, converter);
		lua_pcall(L, 4, 0, 0);
		lua_settop(L, t);
	};
	return 0;
}
int l_settlerDisableConversionHook(lua_State* L) {
	luaL_unref(L, LUA_REGISTRYINDEX, ConversionHookRef);
	ConversionHookRef = LUA_NOREF;
	Hero6ConvertHookCb = nullptr;
	return 0;
}

int l_entityIsFeared(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	int id = e->GetFirstAttachedToMe(AttachmentType_INFLICTOR_TERRORIZED);
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
		e->TargetRotation = (float)deg2rad((double)lua_tonumber(L, 3));
		e->TargetRotationValid = 1;
	}
	return 0;
}

void l_entity_cleanup(lua_State* L) {
	l_settlerDisableConversionHook(L);
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

	lua_pushstring(L, "Predicates");
	lua_newtable(L);
	luaext_registerFunc(L, "And", &l_entityPredicateAnd);
	luaext_registerFunc(L, "Or", &l_entityPredicateOr);
	luaext_registerFunc(L, "Not", &l_entityPredicateNot);
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
	luaext_registerFunc(L, "CommandSendHawk", &l_settlerSendHawk);
	luaext_registerFunc(L, "CommandInflictFear", &l_settlerInflictFear);
	luaext_registerFunc(L, "CommandPlaceBomb", &l_settlerPlaceBomb);
	luaext_registerFunc(L, "CommandPlaceCannon", &l_settlerPlaceCannon);
	luaext_registerFunc(L, "CommandRangedEffect", &l_settlerRangedEffect);
	luaext_registerFunc(L, "CommandCircularAttack", &l_settlerCircularAttack);
	luaext_registerFunc(L, "CommandSummon", &l_settlerSummon);
	luaext_registerFunc(L, "CommandConvert", &l_settlerConvert);
	luaext_registerFunc(L, "CommandSnipe", &l_settlerSnipe);
	luaext_registerFunc(L, "CommandShuriken", &l_settlerShuriken);
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
	lua_rawset(L, -3);

	lua_pushstring(L, "Leader");
	lua_newtable(L);
	luaext_registerFunc(L, "GetExperience", &l_leaderGetLeaderXP);
	luaext_registerFunc(L, "SetExperience", &l_leaderSetLeaderXP);
	luaext_registerFunc(L, "GetTroopHealth", &l_leaderGetLeaderHP);
	luaext_registerFunc(L, "SetTroopHealth", &l_leaderSetLeaderHP);
	lua_rawset(L, -3);

	lua_pushstring(L, "Building");
	lua_newtable(L);
	luaext_registerFunc(L, "GetBarracksAutoFillActive", &l_buildingGetAutoFillActive);
	luaext_registerFunc(L, "GetHeight", &l_buildingGetHeight);
	luaext_registerFunc(L, "SetHeight", &l_buildingSetHeight);
	luaext_registerFunc(L, "MarketGetCurrentTradeData", &l_buildingGetCurrentTradeInfo);
	luaext_registerFunc(L, "MarketSetCurrentTradeData", &l_buildingSetCurrentTradeInfo);
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

shok_EGL_CGLEEntity* EntityIterator::GetNext(float* rangeOut)
{
	shok_EGL_CGLEEntityManager* eman = *shok_EGL_CGLEEntityManagerObj;
	while (true) {
		if (current > eman->EntityCount)
			return nullptr;
		shok_EGL_CGLEEntity* e = eman->GetEntityByNum(current);
		current++;
		if (e != nullptr && Predicate->MatchesEntity(e, rangeOut))
			return e;
	}
	return nullptr;
}
shok_EGL_CGLEEntity* EntityIterator::GetNearest(float* rangeOut)
{
	shok_EGL_CGLEEntity* curr, * ret = nullptr;
	float currR = -1, maxR = -1;
	Reset();
	curr = GetNext(&currR);
	while (curr != nullptr) {
		if (maxR == -1 || currR < maxR) {
			ret = curr;
			maxR = currR;
		}
		curr = GetNext(&currR);
	}
	if (rangeOut != nullptr)
		*rangeOut = maxR;
	return ret;
}
shok_EGL_CGLEEntity* EntityIterator::GetFurthest(float* rangeOut)
{
	shok_EGL_CGLEEntity* curr, * ret = nullptr;
	float currR = -1, maxR = -1;
	Reset();
	curr = GetNext(&currR);
	while (curr != nullptr) {
		if (maxR == -1 || currR > maxR) {
			ret = curr;
			maxR = currR;
		}
		curr = GetNext(&currR);
	}
	if (rangeOut != nullptr)
		*rangeOut = maxR;
	return ret;
}

EntityIteratorPredicateOfType::EntityIteratorPredicateOfType(int etype)
{
	type = etype;
}

bool EntityIteratorPredicateOfType::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return e->EntityType == type;
}

bool EntityIteratorPredicateOfPlayer::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return e->PlayerId == player;
}

EntityIteratorPredicateOfPlayer::EntityIteratorPredicateOfPlayer(int player)
{
	this->player = player;
}

bool EntityIteratorPredicateAnd::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	for (int i = 0; i < numPreds; i++) {
		if (!predicates[i]->MatchesEntity(e, rangeOut))
			return false;
	}
	return true;
}

EntityIteratorPredicateAnd::EntityIteratorPredicateAnd(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateOr::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	for (int i = 0; i < numPreds; i++) {
		if (predicates[i]->MatchesEntity(e, rangeOut))
			return true;
	}
	return false;
}

EntityIteratorPredicateOr::EntityIteratorPredicateOr(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateInCircle::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
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

bool EntityIteratorPredicateIsSettler::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return shok_EntityIsSettler(e);
}

bool EntityIteratorPredicateIsBuilding::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return shok_EntityIsBuilding(e);
}

bool EntityIteratorPredicateIsRelevant::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return e->PlayerId != 0 && (shok_EntityIsSettler(e) || shok_EntityIsBuilding(e));
}

bool EntityIteratorPredicateAnyPlayer::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
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
	shok_GGL_CPlayerStatus* pl = (*shok_GGL_CGLGameLogicObj)->GetPlayer(source);
	int curr = 0;
	for (int p = 1; p <= maxP; p++) {
		if (pl->GetDiploStateTo(p) == shok_DIPLOSTATE_HOSTILE) {
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

bool EntityIteratorPredicateAnyEntityType::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
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

bool EntityIteratorPredicateIsNotSoldier::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return e->GetSoldierBehavior() == nullptr;
}

bool EntityIteratorPredicateOfEntityCategory::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return e->IsEntityInCategory(category);
}

EntityIteratorPredicateOfEntityCategory::EntityIteratorPredicateOfEntityCategory(int cat)
{
	category = cat;
}

bool EntityIteratorPredicateProvidesResource::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return shok_EntityGetProvidedResourceByID(e->EntityId) == resource;
}

EntityIteratorPredicateProvidesResource::EntityIteratorPredicateProvidesResource(int res)
{
	resource = res;
}

bool EntityIteratorPredicateInRect::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
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

bool EntityIteratorPredicateNot::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return !predicate->MatchesEntity(e, rangeOut);
}

EntityIteratorPredicateNot::EntityIteratorPredicateNot(EntityIteratorPredicate* pred)
{
	predicate = pred;
}

bool EntityIteratorPredicateIsVisible::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	shok_GGL_CCamouflageBehavior* c = e->GetCamoAbilityBehavior();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}

bool EntityIteratorPredicateOfUpgradeCategory::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
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

bool EntityIteratorPredicateIsAlive::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return !shok_entityIsDead(e->EntityId); // performance improvement by not doing obj -> id ->obj ?
}

bool EntityIteratorPredicateIsNotFleeingFrom::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return IsNotFleeingFrom(e, Center, Range);
}

bool EntityIteratorPredicateIsNotFleeingFrom::IsNotFleeingFrom(shok_EGL_CGLEEntity* e, shok_position Center, float Range)
{
	if (!e->IsMovingEntity())
		return true;
	if (e->GetFirstAttachedToMe(AttachmentType_INFLICTOR_TERRORIZED) != 0)
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

bool EntityIteratorPredicateFunc::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut)
{
	return func(e);
}
EntityIteratorPredicateFunc::EntityIteratorPredicateFunc(std::function<bool(shok_EGL_CGLEEntity* e)> f)
{
	func = f;
}

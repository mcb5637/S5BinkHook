#include "pch.h"
#include "l_entity.h"
#include "s5data.h"
#include "luaext.h"
#include "entityiterator.h"

EntityIteratorPredicate* l_entity_checkpredicate(lua_State* L, int i) {
	return reinterpret_cast<EntityIteratorPredicate*>(luaext_checkudata(L, i));
};

int l_entity_getNum(lua_State* L) {
	lua_pushnumber(L, (*shok_EGL_CGLEEntityManager::GlobalObj)->EntityCount);
	return 1;
}

int l_entity_get(lua_State* L) {
	int i = luaL_checkint(L, 1);
	EGL::CGLEEntity* p = (*shok_EGL_CGLEEntityManager::GlobalObj)->GetEntityByNum(i);
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
		pred[i] = luaL_checkint(L, i + 1);
	}
	new(ud) EntityIteratorPredicateAnyPlayer(pred, num);
	return 1;
}

int l_entityPredicateAnyEntityType(lua_State* L) {
	int num = lua_gettop(L);
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateAnyEntityType) + sizeof(int) * num);
	int* pred = reinterpret_cast<int*>(&reinterpret_cast<EntityIteratorPredicateAnyEntityType*>(ud)[1]); // get start address of array
	for (int i = 0; i < num; i++) {
		pred[i] = luaL_checkint(L, i + 1);
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
	for (int i = num - 1; i >= 0; i--) { // keep predicates as metatable, so they dont get gced
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
	new(ud) EntityIteratorPredicateOfEntityCategory(static_cast<shok::EntityCategory>(ty));
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

int l_entityPredicateIsNotInBuilding(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateNotInBuilding));
	new(ud) EntityIteratorPredicateNotInBuilding();
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
	EGL::CGLEEntity* e = nullptr;
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
	int count = 0;
	EntityIterator it{ pred };
	EGL::CGLEEntity* e = nullptr;
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
	EGL::CGLEEntity* e = it.GetNearest(&maxR);
	lua_pushnumber(L, e == nullptr ? 0 : e->EntityId);
	lua_pushnumber(L, maxR);
	return 2;
}

int l_entityIteratorNext(lua_State* L) { // (state, last value) -> next value
	EntityIterator* it = reinterpret_cast<EntityIterator*>(lua_touserdata(L, 1)); // no error checking here, cause that would cost speed
	float r = -1;
	int p = -1;
	EGL::CGLEEntity* e = it->GetNext(&r, &p);
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
	EGL::CGLEEntity* s = luaext_checkEntity(L, 1);
	EntityIteratorPredicate* pred = l_entity_checkpredicate(L, 2);
	lua_pushboolean(L, pred->MatchesEntity(s, nullptr, nullptr));
	return 1;
}

int l_settlerGetLeaderOfSoldier(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	lua_pushnumber(L, s->LeaderId);
	return 1;
}

int l_entityGetNumberOfBehaviors(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->Behaviours.size());
	return 1;
}

int l_leaderGetLeaderXP(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	lua_pushnumber(L, b->Experience);
	return 1;
}
int l_leaderSetLeaderXP(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	b->Experience = luaL_checkint(L, 2);
	return 0;
}

int l_leaderGetLeaderHP(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	lua_pushnumber(L, b->GetTroopHealth());
	lua_pushnumber(L, b->GetTroopHealthPerSoldier());
	return 2;
}
int l_leaderSetLeaderHP(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
	luaext_assertPointer(L, b, "no leader at 1");
	b->TroopHealthCurrent = luaL_checkint(L, 2);
	return 0;
}

int l_settlerGetBaseMovementSpeed(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CBehaviorDefaultMovement* b = s->GetBehavior<GGL::CBehaviorDefaultMovement>();
	luaext_assertPointer(L, b, "no moving entity at 1");
	lua_pushnumber(L, b->MovementSpeed);
	lua_pushnumber(L, rad2deg(b->TurningSpeed));
	return 2;
}
int l_settlerSetBaseMovementSpeed(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CBehaviorDefaultMovement* b = s->GetBehavior<GGL::CBehaviorDefaultMovement>();
	luaext_assertPointer(L, b, "no moving entity at 1");
	if (lua_isnumber(L, 2))
		b->MovementSpeed = luaL_checkfloat(L, 2);
	if (lua_isnumber(L, 3))
		b->TurningSpeed = static_cast<float>(deg2rad(lua_tonumber(L, 3)));
	return 0;
}

int l_buildingGetAutoFillActive(lua_State* L) {
	GGL::CBuilding* e = luaext_checkBulding(L, 1);
	GGL::CBarrackBehavior* b = e->GetBehavior<GGL::CBarrackBehavior>();
	luaext_assertPointer(L, b, "no barracks at 1");
	lua_pushboolean(L, b->AutoFillActive);
	return 1;
}

int l_entityGetScale(lua_State* L) {
	EGL::CGLEEntity* s = luaext_checkEntity(L, 1);
	lua_pushnumber(L, s->Scale);
	return 1;
}
int l_entitySetScale(lua_State* L) {
	EGL::CGLEEntity* s = luaext_checkEntity(L, 1);
	s->Scale = luaL_checkfloat(L, 2);
	return 0;
}

int l_buildingGetHeight(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	lua_pushnumber(L, b->BuildingHeight);
	return 1;
}
int l_buildingSetHeight(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	b->BuildingHeight = luaL_checkfloat(L, 2);
	return 0;
}

int l_settlerGetOvereadWidget(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	lua_pushnumber(L, s->OverheadWidget);
	return 1;
}
int l_settlerSetOvereadWidget(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	int ov = luaL_checkint(L, 2);
	luaext_assert(L, ov >= 0 && ov <= 4, "invalid overhead code");
	s->OverheadWidget = ov;
	return 1;
}

int l_movingEntityGetTargetPos(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	EGL::CMovingEntity* m = shok_DynamicCast<EGL::CGLEEntity, EGL::CMovingEntity>(e);
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
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	EGL::CMovingEntity* m = shok_DynamicCast<EGL::CGLEEntity, EGL::CMovingEntity>(e);
	luaext_assertPointer(L, m, "no moving entity at 1");
	luaext_checkPos(L, m->TargetPosition, 2);
	lua_pushstring(L, "r");
	lua_gettable(L, 2);
	if (lua_isnumber(L, -1)) {
		m->SetTargetRotation(static_cast<float>(deg2rad(lua_tonumber(L, -1))));
		EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, true };
		m->FireEvent(&ev);
	}
	else {
		m->TargetRotationValid = 0;
		EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, false };
		m->FireEvent(&ev);
	}
	return 0;
}

int l_settlerHeroCamouflageGetDurationLeft(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CCamouflageBehavior* c = s->GetBehavior<GGL::CCamouflageBehavior>();
	luaext_assertPointer(L, c, "no camo hero at 1");
	lua_pushnumber(L, c->InvisibilityRemaining);
	return 1;
}
int l_settlerHeroCamouflageSetDurationLeft(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CCamouflageBehavior* c = s->GetBehavior<GGL::CCamouflageBehavior>();
	luaext_assertPointer(L, c, "no camo hero at 1");
	luaext_assert(L, !shok_DynamicCast<GGL::CCamouflageBehavior, GGL::CThiefCamouflageBehavior>(c), "thief at 1, use ThiefSetCamouflageTimeTo instead");
	c->InvisibilityRemaining = luaL_checkint(L, 2);
	return 1;
}

int l_settlerThiefCamouflageGetTimeTo(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CThiefCamouflageBehavior* c = s->GetBehavior<GGL::CThiefCamouflageBehavior>();
	luaext_assertPointer(L, c, "no thief at 1");
	lua_pushnumber(L, c->TimeToInvisibility);
	return 1;
}
int l_settlerThiefCamouflageSetTimeTo(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CThiefCamouflageBehavior* c = s->GetBehavior<GGL::CThiefCamouflageBehavior>();
	luaext_assertPointer(L, c, "no thief at 1");
	int i = luaL_checkint(L, 2);
	c->TimeToInvisibility = i;
	c->InvisibilityRemaining = i <= 0 ? 15 : 0;
	return 0;
}

int l_settlerHeroGetResurrectionTime(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CHeroBehavior* h = s->GetBehavior<GGL::CHeroBehavior>();
	luaext_assertPointer(L, h, "no hero at 1");
	lua_pushnumber(L, h->ResurrectionTimePassed);
	return 1;
}
int l_settlerHeroSetResurrectionTime(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CHeroBehavior* h = s->GetBehavior<GGL::CHeroBehavior>();
	luaext_assertPointer(L, h, "no hero at 1");
	h->ResurrectionTimePassed = luaL_checkint(L, 2);
	return 0;
}

int l_entityGetLimitedLifespanRemaining(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CLimitedLifespanBehavior* l = e->GetBehavior<GGL::CLimitedLifespanBehavior>();
	luaext_assertPointer(L, l, "no limited lifespan at 1");
	lua_pushnumber(L, l->RemainingLifespanSeconds);
	return 1;
}
int l_entitySetLimitedLifespanRemaining(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CLimitedLifespanBehavior* l = e->GetBehavior<GGL::CLimitedLifespanBehavior>();
	luaext_assertPointer(L, l, "no limited lifespan at 1");
	l->RemainingLifespanSeconds = luaL_checkint(L, 2);
	return 0;
}

void(__thiscall* FireEvent)(EGL::CGLEEntity* th, BB::CEvent* d);
void __fastcall fireeventhook(EGL::CGLEEntity* th, int _, BB::CEvent* d) {
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
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	//GGL::CBuilding* e = luaext_checkBulding(L, 1);
	/*EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	if (!FireEvent) {
		shok_vtable_EGL_CGLEEntity* vt = (shok_vtable_EGL_CGLEEntity*)e->vtable;
		FireEvent = vt->FireEvent;
		vt->FireEvent = (void(__thiscall *)(EGL::CGLEEntity * th, shok_event_data * d)) &fireeventhook;
	}*/
	//DEBUGGER_BREAK
	//lua_pushnumber(L, (int) &(e->GetEntityType()->LogicProps->MaxHealth));
	lua_pushnumber(L, (int)&e->CurrentState);
	return 1;
}

int l_entityGetTaskListIndex(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->CurrentTaskIndex);
	return 1;
}
int l_entitySetTaskListIndex(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	e->CurrentTaskIndex = luaL_checkint(L, 2);
	return 0;
}

int l_movingEntityGetSpeedFactor(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CBehaviorDefaultMovement* m = e->GetBehavior<GGL::CBehaviorDefaultMovement>();
	luaext_assertPointer(L, m, "no moving entity at 1");
	lua_pushnumber(L, m->SpeedFactor);
	return 1;
}

int l_settlerGetWorkerCurrentWorkTime(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CWorkerBehavior* w = s->GetBehavior<GGL::CWorkerBehavior>();
	luaext_assertPointer(L, w, "no worker at 1");
	lua_pushnumber(L, w->WorkTimeRemaining);
	return 1;
}
int l_settlerSetWorkerCurrentWorkTime(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CWorkerBehavior* w = s->GetBehavior<GGL::CWorkerBehavior>();
	luaext_assertPointer(L, w, "no worker at 1");
	w->WorkTimeRemaining = luaL_checkint(L, 2);
	return 0;
}

int l_buildingGetCurrentTradeInfo(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CMarketBehavior* m = b->GetBehavior<GGL::CMarketBehavior>();
	luaext_assertPointer(L, m, "no market at 1");
	lua_pushnumber(L, m->BuyResourceType);
	lua_pushnumber(L, m->SellResourceType);
	lua_pushnumber(L, m->BuyAmount);
	lua_pushnumber(L, m->SellAmount);
	lua_pushnumber(L, m->ProgressAmount);
	return 5;
}
int l_buildingSetCurrentTradeInfo(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CMarketBehavior* m = b->GetBehavior<GGL::CMarketBehavior>();
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
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushboolean(L, e->GetBehavior<GGL::CSoldierBehavior>() != nullptr);
	return 1;
}

int l_heroResurrect(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CHeroBehavior* h = e->GetBehavior<GGL::CHeroBehavior>();
	luaext_assertPointer(L, h, "no hero at 1");
	h->ResurrectionTimePassed = 20000;
	h->EnemyNear = 0;
	h->FriendNear = 1;
	return 0;
}

int l_settlerThiefSetStolenResourceInfo(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CThiefBehavior* t = e->GetBehavior<GGL::CThiefBehavior>();
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
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CBattleBehavior* b = e->GetBehavior<GGL::CBattleBehavior>();
	if (b) {
		lua_pushnumber(L, b->GetMaxRange());
		return 1;
	}
	GGL::CAutoCannonBehavior* a = e->GetBehavior<GGL::CAutoCannonBehavior>();
	if (a) {
		lua_pushnumber(L, a->GetMaxRange());
		return 1;
	}
	return luaL_error(L, "no battle entity or autocannon at 1");
}

int l_entityGetModel(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	int m = e->ModelOverride;
	if (m == 0) {
		m = e->GetEntityType()->DisplayProps->Model[0];
	}
	lua_pushnumber(L, m);
	return 1;
}

int l_entityGetExploration(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->GetExploration());
	return 1;
}

int l_entityGetSpeed(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CBehaviorDefaultMovement* m = e->GetBehavior<GGL::CBehaviorDefaultMovement>();
	luaext_assertPointer(L, m, "no moving entity at 1");
	lua_pushnumber(L, m->GetMovementSpeed() * 10.0);
	return 1;
}

int l_settlerIsVisible(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	GGL::CCamouflageBehavior* c = e->GetBehavior<GGL::CCamouflageBehavior>();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}

int l_settlerSendHawk(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CHeroHawkBehavior* b = e->GetBehavior<GGL::CHeroHawkBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CHeroHawkBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CHeroHawkBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok::Position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->HeroAbilitySendHawk(p);
	return 0;
}

int l_settlerInflictFear(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CInflictFearAbility* b = e->GetBehavior<GGL::CInflictFearAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CInflictFearAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CInflictFearAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityInflictFear();
	return 0;
}

int l_settlerPlaceBomb(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CBombPlacerBehavior* b = e->GetBehavior<GGL::CBombPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CHeroAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CHeroAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok::Position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->HeroAbilityPlaceBomb(p);
	return 0;
}

int l_settlerPlaceCannon(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CCannonBuilderBehavior* b = e->GetBehavior<GGL::CCannonBuilderBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CCannonBuilderBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CCannonBuilderBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok::Position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	p.FloorToBuildingPlacement();
	int bottom = luaL_checkint(L, 3);
	GGlue::CGlueEntityProps* ety = (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(bottom);
	luaext_assertPointer(L, ety, "no bottom entitytype");
	luaext_assert(L, ety->IsBuildingType(), "bottom not a building");
	int top = luaL_checkint(L, 4);
	luaext_assertPointer(L, (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(top), "no top entitytype");
	luaext_assert(L, shok_canPlaceBuilding(bottom, e->PlayerId, &p, 0, 0), "cannot place foundation at that position");
	e->HeroAbilityPlaceCannon(p, bottom, top);
	return 0;
}

int l_settlerRangedEffect(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CRangedEffectAbility* b = e->GetBehavior<GGL::CRangedEffectAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CRangedEffectAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityRangedEffect();
	return 0;
}

int l_settlerCircularAttack(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CCircularAttack* b = e->GetBehavior<GGL::CCircularAttack>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CCircularAttackProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CCircularAttackProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityCircularAttack();
	return 0;
}

int l_settlerSummon(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CSummonBehavior* b = e->GetBehavior<GGL::CSummonBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CSummonBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSummonBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilitySummon();
	int summoned = 0;
	e->ObservedEntities.ForAll([L, &summoned](shok::Attachment* a) {
		if (a->AttachmentType == shok::AttachmentType::SUMMONER_SUMMONED)
			lua_pushnumber(L, a->EntityId);
		summoned++;
		});
	return summoned;
}

int l_settlerActivateCamo(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CCamouflageBehavior* b = e->GetBehavior<GGL::CCamouflageBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, !shok_DynamicCast<GGL::CCamouflageBehavior, GGL::CThiefCamouflageBehavior>(b), "thief camo cannot be manually activated");
	GGL::CCamouflageBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityActivateCamoflage();
	return 0;
}

int l_settlerConvert(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CConvertSettlerAbility* b = e->GetBehavior<GGL::CConvertSettlerAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CConvertSettlerAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CConvertSettlerAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	GGL::CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	e->HeroAbilityConvert(t->EntityId);
	return 0;
}

int l_settlerSnipe(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CSniperAbility* b = e->GetBehavior<GGL::CSniperAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CSniperAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	GGL::CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	luaext_assert(L, e->Position.IsInRange(t->Position, bp->Range), "target not in range");
	e->HeroAbilitySnipe(t->EntityId);
	return 0;
}

int l_settlerShuriken(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CShurikenAbility* b = e->GetBehavior<GGL::CShurikenAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CShurikenAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CShurikenAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	GGL::CSettler* t = luaext_checkSettler(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	luaext_assert(L, e->Position.IsInRange(t->Position, bp->Range), "target not in range");
	e->HeroAbilityShuriken(t->EntityId);
	return 0;
}

int l_settlerMotivateWorkers(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CMotivateWorkersAbility* b = e->GetBehavior<GGL::CMotivateWorkersAbility>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CMotivateWorkersAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CMotivateWorkersAbilityProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->HeroAbilityMotivateWorkers();
	return 0;
}

int l_settlerSabotage(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CKegPlacerBehavior* b = e->GetBehavior<GGL::CKegPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CKegPlacerBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<GGL::CKegPlacerBehaviorProperties>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	luaext_assert(L, e->GetBehavior<GGL::CThiefBehavior>()->ResourceType == 0, "is carrying resources");
	GGL::CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, t->IsEntityInCategory(shok::EntityCategory::Bridge) || ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile or bridge");
	e->ThiefSabotage(t->EntityId);
	return 0;
}

int l_settlerDefuse(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CKegPlacerBehavior* b = e->GetBehavior<GGL::CKegPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, e->GetBehavior<GGL::CThiefBehavior>()->ResourceType == 0, "is carrying resources");
	EGL::CGLEEntity* t = luaext_checkEntity(L, 2);
	luaext_assertPointer(L, t->GetBehavior<GGL::CKegBehavior>(), "no keg at 2");
	e->ThiefDefuse(t->EntityId);
	return 0;
}

int l_settlerBinoculars(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CAbilityScoutBinocular* b = e->GetBehavior<GGL::CAbilityScoutBinocular>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CAbilityScoutBinocularProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CAbilityScoutBinocularProps>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok::Position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->ScoutBinoculars(p);
	return 0;
}

int l_settlerPlaceTorch(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CTorchPlacerBehavior* b = e->GetBehavior<GGL::CTorchPlacerBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CTorchPlacerBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<GGL::CTorchPlacerBehaviorProperties>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	shok::Position p = { 0,0 };
	luaext_checkPos(L, p, 2);
	e->ScoutPlaceTorch(p);
	return 0;
}

int l_settlerPointToRes(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CPointToResourceBehavior* b = e->GetBehavior<GGL::CPointToResourceBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CPointToResourceBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<GGL::CPointToResourceBehaviorProperties>();
	luaext_assert(L, b->SecondsCharged >= bp->RechargeTimeSeconds, "ability not ready at 1");
	e->ScoutFindResource();
	return 0;
}

int l_settlerStealFrom(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CThiefBehavior* b = e->GetBehavior<GGL::CThiefBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	GGL::CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, ArePlayersHostile(e->PlayerId, t->PlayerId), "entities are not hostile");
	e->ThiefStealFrom(t->EntityId);
	return 0;
}

int l_settlerSecureGoods(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	luaext_assertEntityAlive(L, e->EntityId, "entity dead at 1");
	GGL::CThiefBehavior* b = e->GetBehavior<GGL::CThiefBehavior>();
	luaext_assertPointer(L, b, "no matching ability at 1");
	luaext_assert(L, b->ResourceType != 0, "no resources carried");
	GGL::CBuilding* t = luaext_checkBulding(L, 2);
	luaext_assertEntityAlive(L, t->EntityId, "entity dead at 2");
	luaext_assert(L, e->PlayerId == t->PlayerId, "entities are not of same player");
	luaext_assert(L, t->IsEntityInCategory(shok::EntityCategory::Headquarters), "target not hq");
	e->ThiefSecureGoods(t->EntityId);
	return 0;
}

int l_settlerEnableConversionHook(lua_State* L) {
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	lua_pushlightuserdata(L, &l_settlerEnableConversionHook);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	EGL::CGLEEntity::HookHero6Convert();
	EGL::CGLEEntity::Hero6ConvertHookCb = [](int id, int pl, int nid, int converter) {
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
	EGL::CGLEEntity::Hero6ConvertHookCb = nullptr;
	return 0;
}

int l_entityIsFeared(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	int id = e->GetFirstAttachedToMe(shok::AttachmentType::INFLICTOR_TERRORIZED);
	if (id == 0)
		lua_pushboolean(L, false);
	else
		lua_pushnumber(L, id);
	return 1;
}

int l_settlerMove(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	shok::Position p;
	luaext_checkPos(L, p, 2);
	shok_EGL_CGLELandscape* ls = (*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape;
	if (ls->GetSector(&p) == 0) {
		shok::Position pou;
		if (ls->GetNearestPositionInSector(&p, 1000, e->GetSector(), &pou))
			p = pou;
	}
	e->Move(p);
	if (lua_isnumber(L, 3)) {
		e->SetTargetRotation(static_cast<float>(deg2rad( lua_tonumber(L, 3))));
		EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, true };
		e->FireEvent(&ev);
	}
	else {
		e->TargetRotationValid = false;
		EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, false };
		e->FireEvent(&ev);
	}
	return 0;
}

int l_entityClearAttackers(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	e->ClearAttackers();
	return 0;
}

int l_buildingFoundryBuildCannon(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<GGL::CFoundryBehavior>(), "no foundry at 1");
	luaext_assert(L, b->IsIdle(), "is not idle");
	GGlue::CGlueEntityProps* t = luaext_checkEntityType(L, 2);
	int ety = luaL_checkint(L, 2);
	bool found = false;
	for (GGL::CFoundryBehaviorProperties::CannonInfoData& i : b->GetEntityType()->GetBehaviorProps<GGL::CFoundryBehaviorProperties>()->CannonInfo) {
		if (i.Cannon == ety)
			found = true;
	}
	luaext_assert(L, found, "foundry cannot build entitytype");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	luaext_assert(L, p->CurrentResources.HasResources(&shok_DynamicCast<EGL::CGLEEntityProps, GGL::CGLSettlerProps>(t->LogicProps)->Cost), "missing res");
	luaext_assert(L, p->PlayerAttractionHandler->GetAttractionUsage() < p->PlayerAttractionHandler->GetAttractionLimit(), "pop capped");
	b->CommandBuildCannon(ety);
	return 0;
}

int l_settlerIsIdle(lua_State* L) {
	GGL::CSettler* e = luaext_checkSettler(L, 1);
	lua_pushboolean(L, e->IsIdle());
	return 1;
}

int l_buildingGetConstructionSite(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	lua_pushnumber(L, b->GetConstructionSite());
	return 1;
}

int l_buildingGetNextFreeConstructionSlotFor(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	shok::Position p;
	luaext_checkPos(L, p, 2);
	lua_pushnumber(L, b->GetNearestFreeConstructionSlotFor(&p));
	return 1;
}

int l_settlerSerfConstruct(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CBuilding* b = luaext_checkBulding(L, 2);
	luaext_assertPointer(L, s->GetBehavior<GGL::CSerfBehavior>(), "no serf");
	luaext_assert(L, !b->IsConstructionFinished(), "no construction site");
	bool suc = s->SerfConstructBuilding(b);
	lua_pushboolean(L, suc);
	return 1;
}

int l_buildingGetNextFreeRepairSlotFor(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	shok::Position p;
	luaext_checkPos(L, p, 2);
	lua_pushnumber(L, b->GetNearestFreeRepairSlotFor(&p));
	return 1;
}

int l_settlerSerfRepair(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CBuilding* b = luaext_checkBulding(L, 2);
	luaext_assertPointer(L, s->GetBehavior<GGL::CSerfBehavior>(), "no serf");
	luaext_assert(L, b->IsConstructionFinished(), "construction site");
	luaext_assert(L, b->Health < b->GetMaxHealth(), "at full health");
	bool suc = s->SerfRepairBuilding(b);
	lua_pushboolean(L, suc);
	return 1;
}

int l_entityReplaceWithResourceEntity(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	e = EGL::CGLEEntity::ReplaceEntityWithResourceEntity(e);
	if (e)
		lua_pushnumber(L, e->EntityId);
	else
		lua_pushnil(L);
	return 1;
}

int l_settlerSerfExtract(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	EGL::CGLEEntity* b = luaext_checkEntity(L, 2);
	luaext_assertPointer(L, s->GetBehavior<GGL::CSerfBehavior>(), "no serf");
	if (!shok_DynamicCast<EGL::CGLEEntity, GGL::CResourceDoodad>(b))
		b = EGL::CGLEEntity::ReplaceEntityWithResourceEntity(b);
	if (b == nullptr || !shok_DynamicCast<EGL::CGLEEntity, GGL::CResourceDoodad>(b)) {
		return luaL_error(L, "no resource entity");
	}
	s->SerfExtractResource(b->EntityId);
	lua_pushnumber(L, b->EntityId);
	return 1;
}

int l_buildingStartUpgrade(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsIdle(), "is not idle");
	b->StartUpgrade();
	return 0;
}

int l_buildingCancelUpgrade(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsUpgrading, "not upgrading");
	b->CancelUpgrade();
	return 0;
}

int l_buildingIsIdle(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	lua_pushboolean(L, b->IsIdle());
	return 1;
}

int l_buildingBarracksGetLeadersTrainingAt(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<GGL::CBarrackBehavior>(), "no barracks");
	int i = 0;
	b->ObserverEntities.ForAll([L, &i](shok::Attachment* a) {
		if (a->AttachmentType == shok::AttachmentType::FIGHTER_BARRACKS && !EGL::CGLEEntity::GetEntityByID(a->EntityId)->GetBehavior<GGL::CSoldierBehavior>()) {
			lua_pushnumber(L, a->EntityId);
			i++;
		}
		});
	return i;
}

int l_buildingFoundryGetCannonTypeInConstruction(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CFoundryBehavior* f = b->GetBehavior<GGL::CFoundryBehavior>();
	luaext_assertPointer(L, f, "no foundry");
	lua_pushnumber(L, f->CannonType);
	return 1;
}

int l_settlerExpell(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	s->SettlerExpell();
	return 0;
}

int l_buildingBuySoldierForLeader(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CSettler* s = luaext_checkSettler(L, 2);
	luaext_assertPointer(L, b->GetBehavior<GGL::CBarrackBehavior>(), "no barracks");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading, "barracks is upgrading or under construction");
	GGL::CLeaderBehavior* l = s->GetBehavior<GGL::CLeaderBehavior>();
	GGL::CLeaderBehaviorProps* lp = s->GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, l, "no leader");
	luaext_assert(L, b->PlayerId == s->PlayerId, "different players");
	GGlue::CGlueEntityProps* solty = (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(lp->SoldierType);
	luaext_assertPointer(L, solty, "no soldier type set");
	if (!lua_toboolean(L, 3)) {
		int ucat = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(1)->BuildingUpgradeManager->GetUpgradeCategoryOfEntityType(b->EntityType);
		luaext_assert(L, lp->BarrackUpgradeCategory == ucat, "leader type doesnt match barracks type");
	}
	int max = s->LimitedAttachmentGetMaximum(shok::AttachmentType::LEADER_SOLDIER);
	int curr = 0;
	s->ObservedEntities.ForAll([&curr](shok::Attachment* a) { if (a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER) curr++; });
	luaext_assert(L, curr < max, "no free soldier slot left");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(s->PlayerId);
	luaext_assert(L, p->PlayerAttractionHandler->GetAttractionUsage() < p->PlayerAttractionHandler->GetAttractionLimit(), "pop capped");
	GGL::CGLSettlerProps* sprop = shok_DynamicCast<EGL::CGLEEntityProps, GGL::CGLSettlerProps>(solty->LogicProps);
	luaext_assertPointer(L, sprop, "error: soldier no settler type");
	luaext_assert(L, p->CurrentResources.HasResources(&sprop->Cost), "missing res");
	b->CommandRecruitSoldierForLeader(s->EntityId);
	return 0;
}

int l_leaderAttachSoldier(lua_State* L) {
	GGL::CSettler* l = luaext_checkSettler(L, 1);
	GGL::CSettler* s = luaext_checkSettler(L, 2);
	GGL::CLeaderBehavior* lb = l->GetBehavior<GGL::CLeaderBehavior>();
	luaext_assertPointer(L, lb, "no leader");
	luaext_assertPointer(L, s->GetBehavior<GGL::CSoldierBehavior>(), "no soldier");
	luaext_assert(L, s->EntityType == l->GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->SoldierType, "leader and soldier type doesnt match");
	l->LeaderAttachSoldier(s->EntityId);
	return 0;
}

int l_settlerSerfToBattleSerf(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, s->GetBehavior<GGL::CSerfBehavior>(), "no serf");
	s->SerfTurnToBattleSerf();
	return 0;
}

int l_settlerBattleSerfToSerf(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, s->GetBehavior<GGL::CBattleSerfBehavior>(), "no battleserf");
	s->BattleSerfTurnToSerf();
	return 0;
}

int l_buildingActivateOvertime(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, !b->IsOvertimeActive, "overtime already active");
	luaext_assert(L, b->OvertimeCooldown <= 0, "cooldown active");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading && b->CurrentState != shok_TaskState::BuildingAlarmDefend, "building not idle");
	b->ActivateOvertime();
	return 0;
}

int l_buildingDeactivateOvertime(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsOvertimeActive, "overtime not active");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading && b->CurrentState != shok_TaskState::BuildingAlarmDefend, "building not idle");
	b->DeactivateOvertime();
	return 0;
}

int l_buildingBarracksRecruitLeaders(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CBarrackBehavior* r = b->GetBehavior<GGL::CBarrackBehavior>();
	luaext_assertPointer(L, r, "no barracks");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading, "building not idle");
	luaext_assert(L, r->AutoFillActive, "already recruiting leaders");
	b->BarracksRecruitLeaders();
	return 0;
}
int l_buildingBarracksRecruitGroups(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CBarrackBehavior* r = b->GetBehavior<GGL::CBarrackBehavior>();
	luaext_assertPointer(L, r, "no barracks");
	luaext_assert(L, b->IsConstructionFinished() && !b->IsUpgrading, "building not idle");
	luaext_assert(L, !r->AutoFillActive, "already recruiting groups");
	b->BarracksRecruitGroups();
	return 0;
}

int l_buildingHQBuySerf(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsEntityInCategory(shok::EntityCategory::Headquarters), "no hq");
	luaext_assert(L, b->IsIdle(), "building not idle");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	luaext_assert(L, p->PlayerAttractionHandler->GetAttractionUsage() < p->PlayerAttractionHandler->GetAttractionLimit(), "pop capped");
	GGlue::CGlueEntityProps* solty = (*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(*GGlue::CGlueEntityProps::EntityTypeIDSerf);
	luaext_assert(L, p->CurrentResources.HasResources(&shok_DynamicCast<EGL::CGLEEntityProps, GGL::CGLSettlerProps>(solty->LogicProps)->Cost), "missing res");
	b->HQBuySerf();
	return 0;
}

int l_buildingSell(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, ((GGL::CGLBuildingProps*)b->GetEntityType()->LogicProps)->CanBeSold, "cannot be sold");
	b->SellBuilding();
	return 0;
}

int l_buildingStartResearch(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->IsIdle(), "building not idle");
	int tech = luaL_checkint(L, 2);
	shok_technology* techo = (*shok_GGL_CGLGameLogic::GlobalObj)->GetTech(tech);
	luaext_assertPointer(L, techo, "no tech at 2");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	shok::TechState techstate = p->GetTechStatus(tech);
	luaext_assert(L, techstate == shok::TechState::Allowed, "wrong techstate");
	luaext_assert(L, p->CurrentResources.HasResources(&techo->ResourceCosts), "not enough res");
	b->StartResearch(tech);
	return 0;
}

int l_buildingCancelResearch(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assert(L, b->GetTechnologyInResearch(), "no tech in research");
	b->CancelResearch();
	return 0;
}

bool marketIsRes(shok::ResourceType rty) {
	return rty == shok::ResourceType::Clay || rty == shok::ResourceType::Gold || rty == shok::ResourceType::Iron || rty == shok::ResourceType::Stone || rty == shok::ResourceType::Sulfur || rty == shok::ResourceType::Wood;
}

int l_buildingMarketStartTrade(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<GGL::CMarketBehavior>(), "no market at 1");
	luaext_assert(L, b->IsIdle(), "building not idle");
	shok::ResourceType sellty = static_cast<shok::ResourceType>(luaL_checkint(L, 2));
	shok::ResourceType buyty = static_cast<shok::ResourceType>(luaL_checkint(L, 3));
	float am = luaL_checkfloat(L, 4);
	luaext_assert(L, marketIsRes(sellty), "sell type is invalid");
	luaext_assert(L, marketIsRes(buyty), "buy type is invalid");
	luaext_assert(L, am > 0, "amount <= 0");
	b->MarketStartTrade(sellty, buyty, am);
	return 0;
}

int l_buildingMarketCancelTrade(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, b->GetBehavior<GGL::CMarketBehavior>(), "no market at 1");
	luaext_assert(L, b->GetMarketProgress() < 1.0f, "no transaction in progress");
	b->MarketCancelTrade();
	return 0;
}

int l_settlerSetPos(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	shok::Position p;
	luaext_checkPos(L, p, 2);
	luaext_assert(L, (*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p) != 0, "position is blocked");
	s->SetPosition(p);
	return 0;
}

int l_leaderSetSoldierLimit(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CLimitedAttachmentBehavior* l = s->GetBehavior<GGL::CLimitedAttachmentBehavior>();
	luaext_assertPointer(L, l, "no limited attachment");
	int limit = luaL_checkint(L, 2);
	luaext_assert(L, limit >= 0, "limit < 0");
	auto a = l->AttachmentMap.GetFirstMatch([](auto a) { return a->AttachmentType == shok::AttachmentType::LEADER_SOLDIER; });
	luaext_assertPointer(L, a, "no matching limited attachment");
	a->Limit = limit;
	return 0;
}

int l_buildingRemoveLastMerchantOffer(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CBuildingMerchantBehavior* m = b->GetBehavior<GGL::CBuildingMerchantBehavior>();
	luaext_assertPointer(L, m, "no merchant");
	luaext_assert(L, m->Offer.size() > 0, "is empty");
	delete m->Offer[m->Offer.size() - 1];
	auto v = m->Offer.SaveVector();
	v.Vector.pop_back();
	return 0;
}
int l_buildingMerchantOfferSetData(lua_State* L) {
	GGL::CBuilding* b = luaext_checkBulding(L, 1);
	GGL::CBuildingMerchantBehavior* m = b->GetBehavior<GGL::CBuildingMerchantBehavior>();
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
	EGL::CGLEEntity* b = luaext_checkEntity(L, 1);
	EGL::CGLEEntity::HookDestroyEntity();
	EGL::CGLEEntity::HookMaxHP();
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
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
	EGL::CGLEEntity* fro = luaext_optEntity(L, 1);
	EGL::CGLEEntity* to = luaext_checkEntity(L, 2);
	if (fro)
		to->CloneAdditionalDataFrom(fro->GetAdditionalData(false));
	else if (EGL::CGLEEntity::LastRemovedEntityAddonData.EntityId == luaL_checkint(L, 1))
		to->CloneAdditionalDataFrom(&EGL::CGLEEntity::LastRemovedEntityAddonData);
	return 0;
}
int l_entity_SetDamage(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	EGL::CGLEEntity* b = luaext_checkEntity(L, 1);
	EGL::CGLEEntity::HookDamageMod();
	EGL::CGLEEntity::HookDestroyEntity();
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
	d->DamageOverride = luaL_checkint(L, 2);
	return 0;
}
int l_entity_SetArmor(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	EGL::CGLEEntity* b = luaext_checkEntity(L, 1);
	EGL::CGLEEntity::HookArmorMod();
	EGL::CGLEEntity::HookDestroyEntity();
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
	d->ArmorOverride = luaL_checkint(L, 2);
	return 0;
}
int l_entity_SetExploration(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	EGL::CGLEEntity* b = luaext_checkEntity(L, 1);
	EGL::CGLEEntity::HookExplorationMod();
	EGL::CGLEEntity::HookDestroyEntity();
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
	d->ExplorationOverride = luaL_checkfloat(L, 2);
	return 0;
}
int l_leader_SetRegen(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	GGL::CSettler* b = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, b->GetBehavior<GGL::CLeaderBehavior>(), "no leader");
	EGL::CGLEEntity::HookLeaderRegen();
	EGL::CGLEEntity::HookDestroyEntity();
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
	if (lua_isnumber(L, 2))
		d->RegenHPOverride = luaL_checkint(L, 2);
	if (lua_isnumber(L, 3))
		d->RegenSecondsOverride = luaL_checkint(L, 3);
	return 0;
}
int l_entity_SetAutoAttackMaxRange(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	EGL::CGLEEntity* b = luaext_checkEntity(L, 1);
	EGL::CGLEEntity::HookMaxRange();
	EGL::CGLEEntity::HookDestroyEntity();
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
	d->MaxRangeOverride = luaL_checkfloat(L, 2);
	return 0;
}
int l_entity_SetDisplayName(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	EGL::CGLEEntity* b = luaext_checkEntity(L, 1);
	EGL::CGLEEntity::HookDisplayName();
	EGL::CGLEEntity::HookDestroyEntity();
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
	d->NameOverride = luaL_checkstring(L, 2);
	return 0;
}
int l_entity_GetDisplayName(lua_State* L) {
	EGL::CGLEEntity* b = luaext_checkEntity(L, 1);
	EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(false);
	if (d && !d->NameOverride.empty())
		lua_pushstring(L, d->NameOverride.c_str());
	else
		lua_pushstring(L, shok_EGL_CGLEEntitiesProps::GetEntityTypeDisplayName(b->EntityType));
	return 1;
}

int l_leader_GetRegen(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	GGL::CSettler* b = luaext_checkSettler(L, 1);
	luaext_assertPointer(L, b->GetBehavior<GGL::CLeaderBehavior>(), "no leader");
	lua_pushnumber(L, b->LeaderGetRegenHealth());
	lua_pushnumber(L, b->LeaderGetRegenHealthSeconds());
	return 2;
}

int l_settler_EnableRangedEffectSoldierHeal(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	EGL::CGLEEntity::HookRangedEffectActivateHeal(lua_toboolean(L, 1));
	return 0;
}

int l_entity_PerformHeal(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	int h = luaL_checkint(L, 2);
	luaext_assert(L, h > 0, "heal is <=0");
	e->PerformHeal(h, lua_toboolean(L, 3));
	return 0;
}

int l_building_GetBuildOn(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkBulding(L, 1);
	lua_pushnumber(L, e->GetFirstAttachedEntity(shok::AttachmentType::BUILDING_BASE));
	return 1;
}
int l_building_GetBuildOnReverse(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->GetFirstAttachedToMe(shok::AttachmentType::BUILDING_BASE));
	return 1;
}

int l_building_GetConstructionSite(lua_State* L) {
	GGL::CBuilding* e = luaext_checkBulding(L, 1);
	lua_pushnumber(L, e->GetConstructionSite());
	return 1;
}
int l_building_ConSiteGetBuilding(lua_State* L) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	lua_pushnumber(L, e->GetFirstAttachedEntity(shok::AttachmentType::CONSTRUCTION_SITE_BUILDING));
	return 1;
}

int l_building_BarracksBuyLeaderByType(lua_State* L) {
	GGL::CBuilding* e = luaext_checkBulding(L, 1);
	luaext_assertPointer(L, e->GetBehavior<GGL::CBarrackBehavior>(), "no barracks");
	GGlue::CGlueEntityProps* ety = luaext_checkEntityType(L, 2);
	GGL::CLeaderBehaviorProps* lp = ety->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
	luaext_assertPointer(L, lp, "no leader type");
	luaext_assert(L, e->IsIdle(true), "building not idle");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId);
	luaext_assert(L, p->PlayerAttractionHandler->GetAttractionUsage() < p->PlayerAttractionHandler->GetAttractionLimit(), "pop capped");
	luaext_assert(L, p->CurrentResources.HasResources(&shok_DynamicCast<EGL::CGLEEntityProps, GGL::CGLSettlerProps>(ety->LogicProps)->Cost), "missing res");
	if (!lua_toboolean(L, 3)) {
		int ucat = p->BuildingUpgradeManager->GetUpgradeCategoryOfEntityType(e->EntityType);
		luaext_assert(L, lp->BarrackUpgradeCategory == ucat, "leader type doesnt match barracks type");
	}
	int id = e->BuyLeaderByType(luaL_checkint(L, 2));
	lua_pushnumber(L, id);
	return 1;
}

int l_settler_GetShurikenTarget(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CShurikenAbility* beh = s->GetBehavior<GGL::CShurikenAbility>();
	luaext_assertPointer(L, beh, "no shuriken ability");
	lua_pushnumber(L, beh->TargetId);
	return 1;
}
int l_settler_GetSnipeTarget(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	GGL::CSniperAbility* beh = s->GetBehavior<GGL::CSniperAbility>();
	luaext_assertPointer(L, beh, "no snipe ability");
	lua_pushnumber(L, beh->TargetId);
	return 1;
}

int l_settler_GetEnteredBuilding(lua_State* L) {
	GGL::CSettler* s = luaext_checkSettler(L, 1);
	lua_pushnumber(L, s->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING));
	lua_pushnumber(L, s->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE));
	return 2;
}

const char* AnimTaskList = "TL_SCRIPT_ANIMATION";
void l_settler_createAnimTaskList(lua_State* L) {
	shok_EGL_CGLETaskListMgr* tmng = *shok_EGL_CGLETaskListMgr::GlobalObj;
	int tid = tmng->TaskListManager->GetIdByName(AnimTaskList);
	if (!tid) {
		shok_EGL_CGLETaskList* tl = (*shok_BB_CClassFactory::GlobalObj)->CreateObject<shok_EGL_CGLETaskList>();

		shok_saveVector<shok_EGL_CGLETaskArgs*>(&tl->Task, [](std::vector<shok_EGL_CGLETaskArgs*, shok::Allocator<shok_EGL_CGLETaskArgs*>>& v) {
			shok_BB_CClassFactory* fact = *shok_BB_CClassFactory::GlobalObj;
			{
				shok_EGL_CGLETaskArgsThousandths* t = fact->CreateObject<shok_EGL_CGLETaskArgsThousandths>();
				t->TaskType = shok_Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE;
				t->Thousandths = 1000;
				v.push_back(t);
			}
			{
				shok_EGL_CGLETaskArgs* t = fact->CreateObject<shok_EGL_CGLETaskArgs>();
				t->TaskType = shok_Task::TASK_SET_BATTLE_IDLE_ANIM;
				v.push_back(t);
			}
			{
				shok_EGL_CGLETaskArgs* t = fact->CreateObject<shok_EGL_CGLETaskArgs>();
				t->TaskType = shok_Task::TASK_BATTLE_WAIT_UNTIL;
				v.push_back(t);
			}
			{
				shok_EGL_CGLETaskArgs* t = fact->CreateObject<shok_EGL_CGLETaskArgs>();
				t->TaskType = shok_Task::TASK_SET_DEFAULT_REACTION_TYPE;
				v.push_back(t);
			}
			{
				shok_EGL_CGLETaskArgs* t = fact->CreateObject<shok_EGL_CGLETaskArgs>();
				t->TaskType = shok_Task::TASK_LIST_DONE;
				v.push_back(t);
			}
			});
		tid = tmng->RegisterTaskList(tl, AnimTaskList);
		
		int top = lua_gettop(L);
		lua_getglobal(L, "TaskLists");
		lua_pushstring(L, AnimTaskList);
		lua_pushnumber(L, tid);
		lua_rawset(L, -3);
		lua_settop(L, top);
	}
}
void l_settler_cleanupAnimTask(lua_State* L) {
	shok_EGL_CGLETaskListMgr* tmng = *shok_EGL_CGLETaskListMgr::GlobalObj;
	if (!tmng)
		return;
	int tid = tmng->TaskListManager->GetIdByName(AnimTaskList);
	if (tid) {
		tmng->RemoveTaskList(tid);
	}
}
int l_settler_playScriptAnim(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "does not work with SCELoader");
	l_settler_createAnimTaskList(L);
	EGL::CGLEEntity* e = luaext_checkEntity(L, 1);
	shok_EGL_CGLETaskArgsAnimation setanim{};
	setanim.TaskType = shok_Task::TASK_SET_ANIM;
	setanim.AnimID = (*shok_BB_CIDManagerEx::AnimManager)->GetIdByName(luaL_checkstring(L, 2));
	luaext_assert(L, setanim.AnimID, "not an animation");
	setanim.PlayBackwards = luaext_optbool(L, 3, false);
	shok_EGL_CGLETaskArgs reset{};
	reset.TaskType = shok_Task::TASK_RESET_TASK_LIST_TIMER;
	e->ExecuteTask(reset);
	e->ExecuteTask(setanim);
	GGL::CGLBehaviorAnimationEx* animbeh = e->GetBehavior<GGL::CGLBehaviorAnimationEx>();
	animbeh->SpeedModifier = luaL_optfloat(L, 4, 1);
	animbeh->Duration = static_cast<int>(animbeh->Duration / animbeh->SpeedModifier);
	GGL::CBattleBehavior* batt = e->GetBehavior<GGL::CBattleBehavior>();
	if (batt)
		batt->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
	e->SetTaskList((*shok_EGL_CGLETaskListMgr::GlobalObj)->TaskListManager->GetIdByName(AnimTaskList));
	return 0;
}


void l_entity_cleanup(lua_State* L) {
	l_settlerDisableConversionHook(L);
	EGL::CGLEEntity::BuildingMaxHpTechBoni.clear();
	EGL::CGLEEntity::AddonDataMap.clear();
	EGL::CGLEEntity::LastRemovedEntityAddonData.EntityId = 0;
	EGL::CGLEEntity::HookRangedEffectActivateHeal(false);
	l_settler_cleanupAnimTask(L);
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
	luaext_registerFunc(L, "GetDisplayName", &l_entity_GetDisplayName);
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
	luaext_registerFunc(L, "IsNotInBuilding", &l_entityPredicateIsNotInBuilding);
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
	luaext_registerFunc(L, "EnableRangedEffectSoldierHeal", &l_settler_EnableRangedEffectSoldierHeal);
	luaext_registerFunc(L, "ShurikenGetTarget", &l_settler_GetShurikenTarget);
	luaext_registerFunc(L, "SniperGetTarget", &l_settler_GetSnipeTarget);
	luaext_registerFunc(L, "GetEnteredBuilding", &l_settler_GetEnteredBuilding);
	luaext_registerFunc(L, "PlayScriptAnimation", &l_settler_playScriptAnim);
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
	luaext_registerFunc(L, "BarracksBuyLeaderByType", &l_building_BarracksBuyLeaderByType);
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

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

int l_entityPredicateIsSettlerOrBuilding(lua_State* L) {
	void* ud = lua_newuserdata(L, sizeof(EntityIteratorPredicateIsSettlerOrBuilding));
	new(ud) EntityIteratorPredicateIsSettlerOrBuilding();
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
		e = it.GetNext();
		if (e == nullptr)
			break;
		lua_pushnumber(L, e->EntityId);
		lua_rawseti(L, 2, index);
		index++;
	}
	return 1;
}

int l_entityIteratorNext(lua_State* L) { // (state, last value) -> next value
	EntityIterator* it = (EntityIterator*)lua_touserdata(L, 1); // no error checking here, cause that would cost speed
	shok_EGL_CGLEEntity* e = it->GetNext();
	if (e == nullptr)
		lua_pushnil(L);
	else
		lua_pushnumber(L, e->EntityId);
	return 1;
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
	lua_pushboolean(L, pred->MatchesEntity(s));
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

int l_entity_test(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	int(_cdecl * func)(int id) = (int(_cdecl*)(int id))  0x4B8489;
	lua_pushnumber(L, func(e->EntityId));
	return 1;
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

void l_entity_init(lua_State* L)
{
	luaext_registerFunc(L, "GetNumberOfAllocatedEntities", &l_entity_getNum);
	luaext_registerFunc(L, "Get", &l_entity_get);
	luaext_registerFunc(L, "EntityIteratorTableize", &l_entityIteratorToTable);
	luaext_registerFunc(L, "EntityIterator", &l_entityIterator);
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
	luaext_registerFunc(L, "IsSettlerOrBuilding", &l_entityPredicateIsSettlerOrBuilding);
	luaext_registerFunc(L, "OfAnyPlayer", &l_entityPredicateAnyPlayer);
	luaext_registerFunc(L, "OfAnyEntityType", &l_entityPredicateAnyEntityType);
	luaext_registerFunc(L, "IsNotSoldier", &l_entityPredicateIsNotSoldier);
	luaext_registerFunc(L, "OfEntityCategory", &l_entityPredicateOfEntityCategory);
	luaext_registerFunc(L, "ProvidesResource", &l_entityPredicateProvidesResource);
	luaext_registerFunc(L, "InRect", &l_entityPredicateInRect);
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
	lua_rawset(L, -3);
}

// CppLogic.Entity.CheckPredicate(GUI.GetEntityAtPosition(GUI.GetMousePosition()), CppLogic.Entity.Predicates.ProvidesResource(ResourceType.WoodRaw))
// CppLogic.Entity.test(GUI.GetEntityAtPosition(GUI.GetMousePosition()), EntityCategories.Sword)
// CppLogic.Entity.GetNumberOfAllocatedEntities()
// CppLogic.Entity.GetNumberOfBehaviors(GUI.GetEntityAtPosition(GUI.GetMousePosition()))
// CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.IsSettler(), CppLogic.Entity.Predicates.IsNotSoldier(), CppLogic.Entity.Predicates.OfPlayer(1))
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.InCircle(x,y, 1000))
// for id in CppLogic.Entity.EntityIterator(CppLogic.Entity.Predicates.OfAnyEntityType(Entities.PU_Serf, Entities.PB_Headquarters1), CppLogic.Entity.Predicates.OfAnyPlayer(2, 3)) do LuaDebugger.Log(id) end
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

shok_EGL_CGLEEntity* EntityIterator::GetNext()
{
	shok_EGL_CGLEEntityManager* eman = *shok_EGL_CGLEEntityManagerObj;
	while (true) {
		if (current > eman->EntityCount)
			return nullptr;
		shok_EGL_CGLEEntity* e = eman->GetEntityByNum(current);
		current++;
		if (e != nullptr && Predicate->MatchesEntity(e))
			return e;
	}
	return nullptr;
}

EntityIteratorPredicateOfType::EntityIteratorPredicateOfType(int etype)
{
	type = etype;
}

bool EntityIteratorPredicateOfType::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return e->EntityType == type;
}

bool EntityIteratorPredicateOfPlayer::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return e->PlayerId == player;
}

EntityIteratorPredicateOfPlayer::EntityIteratorPredicateOfPlayer(int player)
{
	this->player = player;
}

bool EntityIteratorPredicateAnd::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	for (int i = 0; i < numPreds; i++) {
		if (!predicates[i]->MatchesEntity(e))
			return false;
	}
	return true;
}

EntityIteratorPredicateAnd::EntityIteratorPredicateAnd(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateOr::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	for (int i = 0; i < numPreds; i++) {
		if (predicates[i]->MatchesEntity(e))
			return true;
	}
	return false;
}

EntityIteratorPredicateOr::EntityIteratorPredicateOr(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateInCircle::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	float dx = e->Position.X - x;
	float dy = e->Position.Y - y;
	return (dx*dx + dy*dy) <= r;
}

EntityIteratorPredicateInCircle::EntityIteratorPredicateInCircle(float x, float y, float r)
{
	this->x = x;
	this->y = y;
	this->r = r * r;
}

bool EntityIteratorPredicateIsSettler::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return shok_EntityIsSettler(e);
}

bool EntityIteratorPredicateIsBuilding::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return shok_EntityIsBuilding(e);
}
bool EntityIteratorPredicateIsSettlerOrBuilding::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return shok_EntityIsSettler(e) || shok_EntityIsBuilding(e);
}

bool EntityIteratorPredicateAnyPlayer::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	int pl = e->PlayerId;
	for (int i = 0; i < numPlayers; i++) {
		if (pl == players[i])
			return true;
	}
	return false;
}

EntityIteratorPredicateAnyPlayer::EntityIteratorPredicateAnyPlayer(int* pl, int numPlayers)
{
	players = pl;
	this->numPlayers = numPlayers;
}

bool EntityIteratorPredicateAnyEntityType::MatchesEntity(shok_EGL_CGLEEntity* e)
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

bool EntityIteratorPredicateIsNotSoldier::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return e->GetSoldierBehavior() == nullptr;
}

bool EntityIteratorPredicateOfEntityCategory::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return e->IsEntityInCategory(category);
}

EntityIteratorPredicateOfEntityCategory::EntityIteratorPredicateOfEntityCategory(int cat)
{
	category = cat;
}

bool EntityIteratorPredicateProvidesResource::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return shok_EntityGetProvidedResourceByID(e->EntityId) == resource;
}

EntityIteratorPredicateProvidesResource::EntityIteratorPredicateProvidesResource(int res)
{
	resource = res;
}

bool EntityIteratorPredicateInRect::MatchesEntity(shok_EGL_CGLEEntity* e)
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

bool EntityIteratorPredicateNot::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return !predicate->MatchesEntity(e);
}

EntityIteratorPredicateNot::EntityIteratorPredicateNot(EntityIteratorPredicate* pred)
{
	predicate = pred;
}

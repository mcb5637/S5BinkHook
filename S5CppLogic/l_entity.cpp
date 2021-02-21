#include "pch.h"
#include "l_entity.h"
#include "pch.h"
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

int l_entity_test(lua_State* L) {
	shok_EGL_CGLEEntity* e = luaext_checkEntity(L, 1);
	int(_cdecl * func)(int id) = (int(_cdecl*)(int id))  0x4B8489;
	lua_pushnumber(L, func(e->EntityId));
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

	lua_pushstring(L, "Predicates");
	lua_newtable(L);
	luaext_registerFunc(L, "And", &l_entityPredicateAnd);
	luaext_registerFunc(L, "Or", &l_entityPredicateOr);
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
	lua_rawset(L, -3);

	lua_pushstring(L, "Settler");
	lua_newtable(L);
	luaext_registerFunc(L, "GetLeaderOfSoldier", &l_settlerGetLeaderOfSoldier);
	lua_rawset(L, -3);
}

// CppLogic.Entity.CheckPredicate(GUI.GetEntityAtPosition(GUI.GetMousePosition()), CppLogic.Entity.Predicates.ProvidesResource(ResourceType.WoodRaw))
// CppLogic.Entity.test(GUI.GetEntityAtPosition(GUI.GetMousePosition()), EntityCategories.Sword)
// CppLogic.Entity.GetNumberOfAllocatedEntities()
// CppLogic.Entity.GetNumberOfBehaviors(GUI.GetEntityAtPosition(GUI.GetMousePosition()))
// CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.IsSettler(), CppLogic.Entity.Predicates.IsNotSoldier(), CppLogic.Entity.Predicates.OfPlayer(1))
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.InCircle(x,y, 1000))
// for id in CppLogic.Entity.EntityIterator(CppLogic.Entity.Predicates.OfAnyEntityType(Entities.PU_Serf, Entities.PB_Headquarters1), CppLogic.Entity.Predicates.OfAnyPlayer(2, 3)) do LuaDebugger.Log(id) end
// CppLogic.Entity.Settler.GetLeaderOfSoldier(GUI.GetEntityAtPosition(GUI.GetMousePosition()))

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
	return shok_IsEntityInCategory(e, category);
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

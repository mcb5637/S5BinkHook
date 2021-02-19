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

int l_entityIteratorToTable(lua_State* L) {
	EntityIteratorPredicateOfType* pred = l_entity_checkpredicate(L, 1);
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

void l_entity_init(lua_State* L)
{
	luaext_registerFunc(L, "GetNumberOfAllocatedEntities", &l_entity_getNum);
	luaext_registerFunc(L, "Get", &l_entity_get);
	luaext_registerFunc(L, "EntityIteratorTableize", &l_entityIteratorToTable);

	lua_pushstring(L, "Predicates");
	lua_newtable(L);
	luaext_registerFunc(L, "And", &l_entityPredicateAnd);
	luaext_registerFunc(L, "Or", &l_entityPredicateOr);
	luaext_registerFunc(L, "OfType", &l_entityPredicateOfType);
	luaext_registerFunc(L, "OfPlayer", &l_entityPredicateOfPlayer);
	luaext_registerFunc(L, "InCircle", &l_entityPredicateInCircle);
	lua_rawset(L, -3);
}

// CppLogic.Entity.GetNumberOfAllocatedEntities()
// CppLogic.Entity.Get(1)
// CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.And(CppLogic.Entity.Predicates.OfType(Entities.PB_Headquarters1), CppLogic.Entity.Predicates.OfPlayer(1)))
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.InCircle(x,y, 1000))

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
		if (current >= eman->EntityCount)
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
	t = etype;
}

bool EntityIteratorPredicateOfType::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return e->EntityType == t;
}

bool EntityIteratorPredicateOfPlayer::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	return e->PlayerId == t;
}

EntityIteratorPredicateOfPlayer::EntityIteratorPredicateOfPlayer(int player)
{
	t = player;
}

bool EntityIteratorPredicateAnd::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	for (int i = 0; i < numPreds; i++) {
		if (!t[i]->MatchesEntity(e))
			return false;
	}
	return true;
}

EntityIteratorPredicateAnd::EntityIteratorPredicateAnd(EntityIteratorPredicate** t, int numPreds)
{
	this->t = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateOr::MatchesEntity(shok_EGL_CGLEEntity* e)
{
	for (int i = 0; i < numPreds; i++) {
		if (t[i]->MatchesEntity(e))
			return true;
	}
	return false;
}

EntityIteratorPredicateOr::EntityIteratorPredicateOr(EntityIteratorPredicate** t, int numPreds)
{
	this->t = t;
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

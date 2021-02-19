#pragma once

#include "luaimport.h"
#include "s5data.h"

#define l_entity_checkpredicate(L, i) (EntityIteratorPredicate *) luaext_checkudata(L, i)

void l_entity_init(lua_State* L);

class EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e) = 0;
};

class EntityIterator {
private:
	int current = 0;
public:
	EntityIteratorPredicate* Predicate;
	EntityIterator(EntityIteratorPredicate* Predicate);
	void Reset();
	shok_EGL_CGLEEntity* GetNext();
};

struct EntityIteratorPredicateOfType : EntityIteratorPredicate {
private:
	int t;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateOfType(int etype);
};

struct EntityIteratorPredicateOfPlayer : EntityIteratorPredicate {
private:
	int t;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateOfPlayer(int player);
};

struct EntityIteratorPredicateAnd : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** t;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateAnd(EntityIteratorPredicate** t, int numPreds);
};

struct EntityIteratorPredicateOr : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** t;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateOr(EntityIteratorPredicate** t, int numPreds);
};

struct EntityIteratorPredicateInCircle : EntityIteratorPredicate {
private:
	float x, y, r;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateInCircle(float x, float y, float r);
};

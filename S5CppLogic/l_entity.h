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
	int type;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateOfType(int etype);
};

struct EntityIteratorPredicateOfPlayer : EntityIteratorPredicate {
private:
	int player;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateOfPlayer(int player);
};

struct EntityIteratorPredicateAnd : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateAnd(EntityIteratorPredicate** preds, int numPreds);
};

struct EntityIteratorPredicateOr : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateOr(EntityIteratorPredicate** preds, int numPreds);
};

struct EntityIteratorPredicateInCircle : EntityIteratorPredicate {
private:
	float x, y, r;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateInCircle(float x, float y, float r);
};

struct EntityIteratorPredicateIsSettler : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
};

struct EntityIteratorPredicateIsBuilding : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
};

struct EntityIteratorPredicateIsSettlerOrBuilding : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
};

struct EntityIteratorPredicateAnyPlayer : EntityIteratorPredicate {
private:
	int* players;
	int numPlayers;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateAnyPlayer(int* pl, int numPlayers);
};

struct EntityIteratorPredicateAnyEntityType : EntityIteratorPredicate {
private:
	int* types;
	int numTypes;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateAnyEntityType(int* ty, int numTy);
};

struct EntityIteratorPredicateIsNotSoldier : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
};

struct EntityIteratorPredicateOfEntityCategory : EntityIteratorPredicate {
private:
	int category;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateOfEntityCategory(int cat);
};

struct EntityIteratorPredicateProvidesResource : EntityIteratorPredicate {
private:
	int resource;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e);
	EntityIteratorPredicateProvidesResource(int res);
};

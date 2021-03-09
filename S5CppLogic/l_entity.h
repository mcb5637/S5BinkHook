#pragma once

#include "luaimport.h"
#include "s5data.h"

#define l_entity_checkpredicate(L, i) (EntityIteratorPredicate *) luaext_checkudata(L, i)

void l_entity_init(lua_State* L);

class EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut) = 0;
};

class EntityIterator {
private:
	int current = 0;
public:
	EntityIteratorPredicate* Predicate;
	EntityIterator(EntityIteratorPredicate* Predicate);
	void Reset();
	shok_EGL_CGLEEntity* GetNext(float* rangeOut);
};

struct EntityIteratorPredicateOfType : EntityIteratorPredicate {
private:
	int type;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateOfType(int etype);
};

struct EntityIteratorPredicateOfPlayer : EntityIteratorPredicate {
private:
	int player;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateOfPlayer(int player);
};

struct EntityIteratorPredicateAnd : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateAnd(EntityIteratorPredicate** preds, int numPreds);
};

struct EntityIteratorPredicateOr : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateOr(EntityIteratorPredicate** preds, int numPreds);
};

struct EntityIteratorPredicateInCircle : EntityIteratorPredicate {
private:
	float x, y, r;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateInCircle(float x, float y, float r);
};

struct EntityIteratorPredicateIsSettler : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
};

struct EntityIteratorPredicateIsBuilding : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
};

struct EntityIteratorPredicateIsRelevant : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
};

struct EntityIteratorPredicateAnyPlayer : EntityIteratorPredicate {
private:
	int* players;
	int numPlayers;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateAnyPlayer(int* pl, int numPlayers);
};

struct EntityIteratorPredicateAnyEntityType : EntityIteratorPredicate {
private:
	int* types;
	int numTypes;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateAnyEntityType(int* ty, int numTy);
};

struct EntityIteratorPredicateIsNotSoldier : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
};

struct EntityIteratorPredicateOfEntityCategory : EntityIteratorPredicate {
private:
	int category;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateOfEntityCategory(int cat);
};

struct EntityIteratorPredicateProvidesResource : EntityIteratorPredicate {
private:
	int resource;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateProvidesResource(int res);
};

struct EntityIteratorPredicateInRect : EntityIteratorPredicate {
private:
	shok_position low;
	shok_position high;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateInRect(float x1, float y1, float x2, float y2);
};

struct EntityIteratorPredicateNot : EntityIteratorPredicate {
private:
	EntityIteratorPredicate* predicate;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateNot(EntityIteratorPredicate* pred);
};

struct EntityIteratorPredicateIsVisible : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
};

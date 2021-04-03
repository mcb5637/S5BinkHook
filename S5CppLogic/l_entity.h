#pragma once

#include "luaimport.h"
#include "s5data.h"

#define l_entity_checkpredicate(L, i) (EntityIteratorPredicate *) luaext_checkudata(L, i)

void l_entity_init(lua_State* L);
void l_entity_cleanup(lua_State* L);

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
	shok_EGL_CGLEEntity* GetNearest(float* rangeOut);
	shok_EGL_CGLEEntity* GetFurthest(float* rangeOut);
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

// not using sqrt
struct EntityIteratorPredicateInCircle : EntityIteratorPredicate {
private:
	shok_position p;
	float r;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateInCircle(float x, float y, float r);
	EntityIteratorPredicateInCircle(shok_position &p, float r);
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
	static void FillHostilePlayers(int source, int* players, int& maxP);
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

struct EntityIteratorPredicateOfUpgradeCategory : EntityIteratorPredicate {
private:
	int category;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateOfUpgradeCategory(int cat);
};

struct EntityIteratorPredicateIsAlive : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
};

// not the fastest, cause i have to use sqrt twice, use it last
struct EntityIteratorPredicateIsNotFleeingFrom : EntityIteratorPredicate {
private:
	shok_position Center;
	float Range;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateIsNotFleeingFrom(shok_position& p, float r);
	static bool IsNotFleeingFrom(shok_EGL_CGLEEntity* e, shok_position Center, float Range);
};

struct EntityIteratorPredicateFunc : EntityIteratorPredicate {
private:
	std::function<bool(shok_EGL_CGLEEntity* e)> func;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut);
	EntityIteratorPredicateFunc(std::function<bool(shok_EGL_CGLEEntity* e)> f);
};

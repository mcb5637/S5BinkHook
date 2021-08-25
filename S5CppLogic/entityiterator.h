#pragma once

#include "s5data.h"

class EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) = 0;
};

class EntityIterator {
private:
	int current = 0;
public:
	EntityIteratorPredicate* Predicate;
	EntityIterator(EntityIteratorPredicate* Predicate);
	void Reset();
	shok_EGL_CGLEEntity* GetNext(float* rangeOut, int* prio);
	shok_EGL_CGLEEntity* GetNearest(float* rangeOut);
	shok_EGL_CGLEEntity* GetFurthest(float* rangeOut);
};

struct EntityIteratorPredicateOfType : EntityIteratorPredicate {
private:
	int type;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateOfType(int etype);
};

struct EntityIteratorPredicateOfPlayer : EntityIteratorPredicate {
private:
	int player;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateOfPlayer(int player);
};

struct EntityIteratorPredicateAnd : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateAnd(EntityIteratorPredicate** preds, int numPreds);
};

struct EntityIteratorPredicateOr : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateOr(EntityIteratorPredicate** preds, int numPreds);
};

// not using sqrt
struct EntityIteratorPredicateInCircle : EntityIteratorPredicate {
private:
	shok_position p;
	float r;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateInCircle(float x, float y, float r);
	EntityIteratorPredicateInCircle(const shok_position& p, float r);
};

struct EntityIteratorPredicateIsSettler : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
};

struct EntityIteratorPredicateIsBuilding : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
};

struct EntityIteratorPredicateIsRelevant : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
};

struct EntityIteratorPredicateAnyPlayer : EntityIteratorPredicate {
private:
	int* players;
	int numPlayers;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateAnyPlayer(int* pl, int numPlayers);
	static void FillHostilePlayers(int source, int* players, int& maxP);
};

struct EntityIteratorPredicateAnyEntityType : EntityIteratorPredicate {
private:
	int* types;
	int numTypes;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateAnyEntityType(int* ty, int numTy);
};

struct EntityIteratorPredicateIsNotSoldier : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
};

struct EntityIteratorPredicateOfEntityCategory : EntityIteratorPredicate {
private:
	shok_EntityCategory category;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateOfEntityCategory(shok_EntityCategory cat);
};

struct EntityIteratorPredicateProvidesResource : EntityIteratorPredicate {
private:
	int resource;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateProvidesResource(int res);
};

struct EntityIteratorPredicateInRect : EntityIteratorPredicate {
private:
	shok_position low;
	shok_position high;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateInRect(float x1, float y1, float x2, float y2);
};

struct EntityIteratorPredicateNot : EntityIteratorPredicate {
private:
	EntityIteratorPredicate* predicate;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateNot(EntityIteratorPredicate* pred);
};

struct EntityIteratorPredicateIsVisible : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
};

struct EntityIteratorPredicateOfUpgradeCategory : EntityIteratorPredicate {
private:
	int category;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateOfUpgradeCategory(int cat);
};

struct EntityIteratorPredicateIsAlive : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
};

// not the fastest, cause i have to use sqrt twice, use it last
struct EntityIteratorPredicateIsNotFleeingFrom : EntityIteratorPredicate {
private:
	shok_position Center;
	float Range;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateIsNotFleeingFrom(shok_position& p, float r);
	static bool IsNotFleeingFrom(shok_EGL_CGLEEntity* e, shok_position Center, float Range);
};

struct EntityIteratorPredicateFunc : EntityIteratorPredicate {
private:
	std::function<bool(shok_EGL_CGLEEntity* e)> func;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicateFunc(std::function<bool(shok_EGL_CGLEEntity* e)> f);
};

struct EntityIteratorPredicatePriority : EntityIteratorPredicate {
private:
	int Prio;
	EntityIteratorPredicate* Pred;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio);
	EntityIteratorPredicatePriority(int prio, EntityIteratorPredicate* pred);
};

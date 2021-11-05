#pragma once

#include "s5data.h"

class EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const = 0;
};

class EntityIterator {
private:
	int current = 0;
public:
	const EntityIteratorPredicate* Predicate;
	EntityIterator(const EntityIteratorPredicate* Predicate);
	void Reset();
	shok_EGL_CGLEEntity* GetNext(float* rangeOut, int* prio);
	shok_EGL_CGLEEntity* GetNearest(float* rangeOut);
	shok_EGL_CGLEEntity* GetFurthest(float* rangeOut);

	shok_EGL_CGLEEntity* GetNext(int& icurr, float* rangeOut, int* prio) const;

	struct Iter {
	private:
		int curr;
		shok_EGL_CGLEEntity* e;
		const EntityIterator* I;

	public:
		shok_EGL_CGLEEntity* operator*() const;
		bool operator==(const Iter& o) const;
		bool operator!=(const Iter& o) const;
		Iter& operator++();
		Iter operator++(int);
		Iter(const EntityIterator& i, int c, shok_EGL_CGLEEntity* e);
	};
	// maybe implement an iterator that gives access to range+prio too...
	Iter begin() const;
	Iter end() const;
};

struct EntityIteratorPredicateOfType : EntityIteratorPredicate {
private:
	int type;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateOfType(int etype);
};

struct EntityIteratorPredicateOfPlayer : EntityIteratorPredicate {
private:
	int player;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateOfPlayer(int player);
};

struct EntityIteratorPredicateAnd : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateAnd(EntityIteratorPredicate** preds, int numPreds);
};

struct EntityIteratorPredicateOr : EntityIteratorPredicate {
private:
	EntityIteratorPredicate** predicates;
	int numPreds;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateOr(EntityIteratorPredicate** preds, int numPreds);
};

// not using sqrt
struct EntityIteratorPredicateInCircle : EntityIteratorPredicate {
private:
	shok_position p;
	float r;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateInCircle(float x, float y, float r);
	EntityIteratorPredicateInCircle(const shok_position& p, float r);
};

struct EntityIteratorPredicateIsSettler : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
};

struct EntityIteratorPredicateIsBuilding : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
};

struct EntityIteratorPredicateIsRelevant : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
};

struct EntityIteratorPredicateAnyPlayer : EntityIteratorPredicate {
private:
	int* players;
	int numPlayers;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateAnyPlayer(int* pl, int numPlayers);
	static void FillHostilePlayers(int source, int* players, int& maxP);
};

struct EntityIteratorPredicateAnyEntityType : EntityIteratorPredicate {
private:
	const int* types;
	size_t numTypes;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateAnyEntityType(const int* ty, size_t numTy);
};

struct EntityIteratorPredicateIsNotSoldier : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
};

struct EntityIteratorPredicateOfEntityCategory : EntityIteratorPredicate {
private:
	shok_EntityCategory category;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateOfEntityCategory(shok_EntityCategory cat);
};

struct EntityIteratorPredicateProvidesResource : EntityIteratorPredicate {
private:
	int resource;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateProvidesResource(int res);
};

struct EntityIteratorPredicateInRect : EntityIteratorPredicate {
private:
	shok_position low;
	shok_position high;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateInRect(float x1, float y1, float x2, float y2);
	EntityIteratorPredicateInRect(const shok_AARect& area);
};

struct EntityIteratorPredicateNot : EntityIteratorPredicate {
private:
	EntityIteratorPredicate* predicate;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateNot(EntityIteratorPredicate* pred);
};

struct EntityIteratorPredicateIsVisible : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
};

struct EntityIteratorPredicateOfUpgradeCategory : EntityIteratorPredicate {
private:
	int category;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateOfUpgradeCategory(int cat);
};

struct EntityIteratorPredicateIsAlive : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
};

// not the fastest, cause i have to use sqrt twice, use it last
struct EntityIteratorPredicateIsNotFleeingFrom : EntityIteratorPredicate {
private:
	shok_position Center;
	float Range;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateIsNotFleeingFrom(shok_position& p, float r);
	static bool IsNotFleeingFrom(shok_EGL_CGLEEntity* e, shok_position Center, float Range);
};

struct EntityIteratorPredicateFunc : EntityIteratorPredicate {
private:
	std::function<bool(shok_EGL_CGLEEntity* e)> func;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicateFunc(std::function<bool(shok_EGL_CGLEEntity* e)> f);
};

struct EntityIteratorPredicatePriority : EntityIteratorPredicate {
private:
	int Prio;
	EntityIteratorPredicate* Pred;
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
	EntityIteratorPredicatePriority(int prio, EntityIteratorPredicate* pred);
};

struct EntityIteratorPredicateNotInBuilding : EntityIteratorPredicate {
public:
	virtual bool MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio) const;
};

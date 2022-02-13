#include "pch.h"
#include "entityiterator.h"


EntityIterator::EntityIterator(const EntityIteratorPredicate* Predicate)
{
	this->Predicate = Predicate;
	Reset();
}

void EntityIterator::Reset()
{
	current = 0;
}

EGL::CGLEEntity* EntityIterator::GetNext(float* rangeOut, int* prio)
{
	return GetNext(current, rangeOut, prio);
}
EGL::CGLEEntity* EntityIterator::GetNext(int& icurr, float* rangeOut, int* prio) const
{
	shok_EGL_CGLEEntityManager* eman = *shok_EGL_CGLEEntityManager::GlobalObj;
	while (true) {
		if (icurr > eman->EntityCount)
			return nullptr;
		EGL::CGLEEntity* e = eman->GetEntityByNum(icurr);
		icurr++;
		if (e != nullptr && Predicate->MatchesEntity(e, rangeOut, prio))
			return e;
	}
	return nullptr;
}

EGL::CGLEEntity* EntityIterator::Iter::operator*() const {
	return e;
}
bool EntityIterator::Iter::operator==(const Iter& o) const {
	return curr == o.curr;
}
bool EntityIterator::Iter::operator!=(const Iter& o) const {
	return !(*this == o);
}
EntityIterator::Iter& EntityIterator::Iter::operator++() {
	EGL::CGLEEntity* e = I->GetNext(curr, nullptr, nullptr);
	this->e = e;
	if (!e)
		curr = 0;
	return *this;
}
EntityIterator::Iter EntityIterator::Iter::operator++(int) {
	Iter r = *this;
	++(*this);
	return r;
}
EntityIterator::Iter::Iter(const EntityIterator& i, int c, EGL::CGLEEntity* e)
{
	I = &i;
	curr = c;
	this->e = e;
}
EntityIterator::Iter EntityIterator::begin() const
{
	Iter e = end();
	e++;
	return e;
}
EntityIterator::Iter EntityIterator::end() const
{
	return { *this, 0, nullptr }; // 0 is guranteed to not be an entity
}

EGL::CGLEEntity* EntityIterator::GetNearest(float* rangeOut)
{
	EGL::CGLEEntity* curr, * ret = nullptr;
	float currR = -1, maxR = -1;
	int currPrio = -1, maxPrio = -1;
	Reset();
	curr = GetNext(&currR, &currPrio);
	while (curr != nullptr) {
		if (currPrio > maxPrio || maxR == -1 || (currR < maxR && currPrio == maxPrio)) {
			ret = curr;
			maxR = currR;
			maxPrio = currPrio;
		}
		currPrio = -1;
		currR = -1;
		curr = GetNext(&currR, &currPrio);
	}
	if (rangeOut != nullptr)
		*rangeOut = maxR;
	return ret;
}
EGL::CGLEEntity* EntityIterator::GetFurthest(float* rangeOut)
{
	EGL::CGLEEntity* curr, * ret = nullptr;
	float currR = -1, maxR = -1;
	int currPrio = -1, maxPrio = -1;
	Reset();
	curr = GetNext(&currR, &currPrio);
	while (curr != nullptr) {
		if (currPrio > maxPrio || maxR == -1 || (currR > maxR && currPrio == maxPrio)) {
			ret = curr;
			maxR = currR;
			maxPrio = currPrio;
		}
		currPrio = -1;
		currR = -1;
		curr = GetNext(&currR, &currPrio);
	}
	if (rangeOut != nullptr)
		*rangeOut = maxR;
	return ret;
}

EntityIteratorPredicateOfType::EntityIteratorPredicateOfType(int etype)
{
	type = etype;
}

bool EntityIteratorPredicateOfType::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->EntityType == type;
}

bool EntityIteratorPredicateOfPlayer::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->PlayerId == player;
}

EntityIteratorPredicateOfPlayer::EntityIteratorPredicateOfPlayer(int player)
{
	this->player = player;
}

bool EntityIteratorPredicateAnd::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	for (int i = 0; i < numPreds; i++) {
		if (!predicates[i]->MatchesEntity(e, rangeOut, prio))
			return false;
	}
	return true;
}

EntityIteratorPredicateAnd::EntityIteratorPredicateAnd(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateOr::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	for (int i = 0; i < numPreds; i++) {
		if (predicates[i]->MatchesEntity(e, rangeOut, prio))
			return true;
	}
	return false;
}

EntityIteratorPredicateOr::EntityIteratorPredicateOr(EntityIteratorPredicate** t, int numPreds)
{
	this->predicates = t;
	this->numPreds = numPreds;
}

bool EntityIteratorPredicateInCircle::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	float ran = p.GetDistanceSquaredTo(e->Position);
	if (rangeOut != nullptr)
		*rangeOut = ran;
	return ran <= r;
}

EntityIteratorPredicateInCircle::EntityIteratorPredicateInCircle(float x, float y, float r)
{
	this->p = { x,y };
	this->r = r * r;
}
EntityIteratorPredicateInCircle::EntityIteratorPredicateInCircle(const shok::Position& p, float r)
{
	this->p = p;
	this->r = r * r;
}

bool EntityIteratorPredicateIsSettler::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return shok_DynamicCast<EGL::CGLEEntity, GGL::CSettler>(e);
}

bool EntityIteratorPredicateIsBuilding::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return shok_DynamicCast<EGL::CGLEEntity, GGL::CBuilding>(e);
}

bool EntityIteratorPredicateIsRelevant::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->PlayerId != 0 && (shok_DynamicCast<EGL::CGLEEntity, GGL::CSettler>(e) || shok_DynamicCast<EGL::CGLEEntity, GGL::CBuilding>(e));
}

bool EntityIteratorPredicateAnyPlayer::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	int pl = e->PlayerId;
	for (int i = 0; i < numPlayers; i++) {
		if (pl == players[i])
			return true;
	}
	return false;
}
void EntityIteratorPredicateAnyPlayer::FillHostilePlayers(int source, int* players, int& maxP)
{
	shok_GGL_CPlayerStatus* pl = (*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(source);
	int curr = 0;
	for (int p = 1; p <= maxP; p++) {
		if (pl->GetDiploStateTo(p) == shok::DiploState::Hostile) {
			players[curr] = p;
			curr++;
		}
	}
	maxP = curr;
}

EntityIteratorPredicateAnyPlayer::EntityIteratorPredicateAnyPlayer(int* pl, int numPlayers)
{
	players = pl;
	this->numPlayers = numPlayers;
}

bool EntityIteratorPredicateAnyEntityType::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	int pl = e->EntityType;
	for (size_t i = 0; i < numTypes; i++) {
		if (pl == types[i])
			return true;
	}
	return false;
}

EntityIteratorPredicateAnyEntityType::EntityIteratorPredicateAnyEntityType(const int* ty, size_t numTy)
{
	types = ty;
	numTypes = numTy;
}

bool EntityIteratorPredicateIsNotSoldier::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->GetBehavior<GGL::CSoldierBehavior>() == nullptr;
}

bool EntityIteratorPredicateOfEntityCategory::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->IsEntityInCategory(category);
}

EntityIteratorPredicateOfEntityCategory::EntityIteratorPredicateOfEntityCategory(shok::EntityCategory cat)
{
	category = cat;
}

bool EntityIteratorPredicateProvidesResource::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->GetResourceProvided() == resource;
}

EntityIteratorPredicateProvidesResource::EntityIteratorPredicateProvidesResource(int res)
{
	resource = res;
}

bool EntityIteratorPredicateInRect::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
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

EntityIteratorPredicateInRect::EntityIteratorPredicateInRect(const shok::AARect& area) : EntityIteratorPredicateInRect(area.low.X, area.low.Y, area.high.X, area.high.Y)
{
}

bool EntityIteratorPredicateNot::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return !predicate->MatchesEntity(e, rangeOut, prio);
}

EntityIteratorPredicateNot::EntityIteratorPredicateNot(EntityIteratorPredicate* pred)
{
	predicate = pred;
}

bool EntityIteratorPredicateIsVisible::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	GGL::CCamouflageBehavior* c = e->GetBehavior<GGL::CCamouflageBehavior>();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}

bool EntityIteratorPredicateOfUpgradeCategory::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	GGlue::CGlueEntityProps* t = e->GetEntityType();
	if (t->IsBuildingType()) {
		GGL::CGLBuildingProps* p = (GGL::CGLBuildingProps*)t;
		return p->Upgrade.Category == category;
	}
	if (t->IsSettlerType()) {
		GGL::CGLSettlerProps* p = (GGL::CGLSettlerProps*)t;
		return p->Upgrade.Category == category;
	}
	return false;
}

EntityIteratorPredicateOfUpgradeCategory::EntityIteratorPredicateOfUpgradeCategory(int cat)
{
	category = cat;
}

bool EntityIteratorPredicateIsAlive::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return !EGL::CGLEEntity::EntityIDIsDead(e->EntityId); // performance improvement by not doing obj -> id ->obj ?
}

bool EntityIteratorPredicateIsNotFleeingFrom::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return IsNotFleeingFrom(e, Center, Range);
}

bool EntityIteratorPredicateIsNotFleeingFrom::IsNotFleeingFrom(EGL::CGLEEntity* e, shok::Position Center, float Range)
{
	if (!shok_DynamicCast<EGL::CGLEEntity, EGL::CMovingEntity>(e))
		return true;
	if (e->GetFirstAttachedToMe(shok::AttachmentType::INFLICTOR_TERRORIZED) != 0)
		return false;
	EGL::CMovingEntity* me = (EGL::CMovingEntity*)e;
	float dx1 = Center.X - e->Position.X;
	float dy1 = Center.Y - e->Position.Y;
	float dx2 = Center.X - me->TargetPosition.X;
	float dy2 = Center.Y - me->TargetPosition.Y;
	return std::sqrtf(dx1 * dx1 + dy1 * dy1) + Range >= std::sqrtf(dx2 * dx2 + dy2 * dy2);
}

EntityIteratorPredicateIsNotFleeingFrom::EntityIteratorPredicateIsNotFleeingFrom(shok::Position& p, float r)
{
	Center = p;
	Range = r;
}

bool EntityIteratorPredicateFunc::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return func(e);
}
EntityIteratorPredicateFunc::EntityIteratorPredicateFunc(std::function<bool(EGL::CGLEEntity* e)> f)
{
	func = f;
}

bool EntityIteratorPredicatePriority::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	if (prio && Pred->MatchesEntity(e, rangeOut, nullptr))
		*prio = Prio;
	return true;
}
EntityIteratorPredicatePriority::EntityIteratorPredicatePriority(int prio, EntityIteratorPredicate* pred)
{
	Prio = prio;
	Pred = pred;
}

bool EntityIteratorPredicateNotInBuilding::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return !e->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING) && !e->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE);
}

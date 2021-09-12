#include "pch.h"
#include "entityiterator.h"


EntityIterator::EntityIterator(EntityIteratorPredicate* Predicate)
{
	this->Predicate = Predicate;
	Reset();
}

void EntityIterator::Reset()
{
	current = 0;
}

shok_EGL_CGLEEntity* EntityIterator::GetNext(float* rangeOut, int* prio)
{
	shok_EGL_CGLEEntityManager* eman = *shok_EGL_CGLEEntityManager::GlobalObj;
	while (true) {
		if (current > eman->EntityCount)
			return nullptr;
		shok_EGL_CGLEEntity* e = eman->GetEntityByNum(current);
		current++;
		if (e != nullptr && Predicate->MatchesEntity(e, rangeOut, prio))
			return e;
	}
	return nullptr;
}
shok_EGL_CGLEEntity* EntityIterator::GetNearest(float* rangeOut)
{
	shok_EGL_CGLEEntity* curr, * ret = nullptr;
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
shok_EGL_CGLEEntity* EntityIterator::GetFurthest(float* rangeOut)
{
	shok_EGL_CGLEEntity* curr, * ret = nullptr;
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

bool EntityIteratorPredicateOfType::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->EntityType == type;
}

bool EntityIteratorPredicateOfPlayer::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->PlayerId == player;
}

EntityIteratorPredicateOfPlayer::EntityIteratorPredicateOfPlayer(int player)
{
	this->player = player;
}

bool EntityIteratorPredicateAnd::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
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

bool EntityIteratorPredicateOr::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
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

bool EntityIteratorPredicateInCircle::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
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
EntityIteratorPredicateInCircle::EntityIteratorPredicateInCircle(const shok_position& p, float r)
{
	this->p = p;
	this->r = r * r;
}

bool EntityIteratorPredicateIsSettler::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e);
}

bool EntityIteratorPredicateIsBuilding::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CBuilding>(e);
}

bool EntityIteratorPredicateIsRelevant::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->PlayerId != 0 && (shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CSettler>(e) || shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CBuilding>(e));
}

bool EntityIteratorPredicateAnyPlayer::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
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
		if (pl->GetDiploStateTo(p) == shok_DiploState::Hostile) {
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

bool EntityIteratorPredicateAnyEntityType::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
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

bool EntityIteratorPredicateIsNotSoldier::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->GetBehavior<shok_GGL_CSoldierBehavior>() == nullptr;
}

bool EntityIteratorPredicateOfEntityCategory::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->IsEntityInCategory(category);
}

EntityIteratorPredicateOfEntityCategory::EntityIteratorPredicateOfEntityCategory(shok_EntityCategory cat)
{
	category = cat;
}

bool EntityIteratorPredicateProvidesResource::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return e->GetResourceProvided() == resource;
}

EntityIteratorPredicateProvidesResource::EntityIteratorPredicateProvidesResource(int res)
{
	resource = res;
}

bool EntityIteratorPredicateInRect::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
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

bool EntityIteratorPredicateNot::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return !predicate->MatchesEntity(e, rangeOut, prio);
}

EntityIteratorPredicateNot::EntityIteratorPredicateNot(EntityIteratorPredicate* pred)
{
	predicate = pred;
}

bool EntityIteratorPredicateIsVisible::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	shok_GGL_CCamouflageBehavior* c = e->GetBehavior<shok_GGL_CCamouflageBehavior>();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}

bool EntityIteratorPredicateOfUpgradeCategory::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	shok_GGlue_CGlueEntityProps* t = e->GetEntityType();
	if (t->IsBuildingType()) {
		shok_GGL_CGLBuildingProps* p = (shok_GGL_CGLBuildingProps*)t;
		return p->Upgrade.Category == category;
	}
	if (t->IsSettlerType()) {
		shok_GGL_CGLSettlerProps* p = (shok_GGL_CGLSettlerProps*)t;
		return p->Upgrade.Category == category;
	}
	return false;
}

EntityIteratorPredicateOfUpgradeCategory::EntityIteratorPredicateOfUpgradeCategory(int cat)
{
	category = cat;
}

bool EntityIteratorPredicateIsAlive::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return !shok_EGL_CGLEEntity::EntityIDIsDead(e->EntityId); // performance improvement by not doing obj -> id ->obj ?
}

bool EntityIteratorPredicateIsNotFleeingFrom::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return IsNotFleeingFrom(e, Center, Range);
}

bool EntityIteratorPredicateIsNotFleeingFrom::IsNotFleeingFrom(shok_EGL_CGLEEntity* e, shok_position Center, float Range)
{
	if (!shok_DynamicCast<shok_EGL_CGLEEntity, shok_EGL_CMovingEntity>(e))
		return true;
	if (e->GetFirstAttachedToMe(shok_AttachmentType::INFLICTOR_TERRORIZED) != 0)
		return false;
	shok_EGL_CMovingEntity* me = (shok_EGL_CMovingEntity*)e;
	float dx1 = Center.X - e->Position.X;
	float dy1 = Center.Y - e->Position.Y;
	float dx2 = Center.X - me->TargetPosition.X;
	float dy2 = Center.Y - me->TargetPosition.Y;
	return std::sqrtf(dx1 * dx1 + dy1 * dy1) + Range >= std::sqrtf(dx2 * dx2 + dy2 * dy2);
}

EntityIteratorPredicateIsNotFleeingFrom::EntityIteratorPredicateIsNotFleeingFrom(shok_position& p, float r)
{
	Center = p;
	Range = r;
}

bool EntityIteratorPredicateFunc::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return func(e);
}
EntityIteratorPredicateFunc::EntityIteratorPredicateFunc(std::function<bool(shok_EGL_CGLEEntity* e)> f)
{
	func = f;
}

bool EntityIteratorPredicatePriority::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
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

bool EntityIteratorPredicateNotInBuilding::MatchesEntity(shok_EGL_CGLEEntity* e, float* rangeOut, int* prio)
{
	return !e->GetFirstAttachedEntity(shok_AttachmentType::SETTLER_ENTERED_BUILDING) && !e->GetFirstAttachedEntity(shok_AttachmentType::SETTLER_BUILDING_TO_LEAVE);
}

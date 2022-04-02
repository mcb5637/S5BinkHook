#include "pch.h"
#include "entityiterator.h"
#include "s5_entityandeffectmanager.h"
#include "s5_maplogic.h"
#include "s5_player.h"
#include "s5_entitytype.h"

CppLogic::Iterator::GlobalEntityIterator::GlobalEntityIterator(const Predicate<EGL::CGLEEntity>* const p)
	: GlobalManagedIterator<EGL::CGLEEntity>(*EGL::CGLEEntityManager::GlobalObj, p) {
}

CppLogic::Iterator::PlayerEntityIterator::PlayerEntityIterator(int player, const Predicate<EGL::CGLEEntity>* const p)
	: ManagedIterator<EGL::CGLEEntity>(p), ah(*(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(player)->PlayerAttractionHandler)
{
}
EGL::CGLEEntity* CppLogic::Iterator::PlayerEntityIterator::GetNextBase(int& c) const
{
	c++;
	if (c >= static_cast<int>(ah.EntityInSystem.size()))
		return nullptr;
	return (*EGL::CGLEEntityManager::GlobalObj)->GetById(ah.EntityInSystem[c].EntityID);
}
EGL::CGLEEntity* CppLogic::Iterator::PlayerEntityIterator::GetCurrentBase(int c) const
{
	if (c >= static_cast<int>(ah.EntityInSystem.size()))
		return nullptr;
	return (*EGL::CGLEEntityManager::GlobalObj)->GetById(ah.EntityInSystem[c].EntityID);
}

CppLogic::Iterator::MultiPlayerEntityIterator::MultiPlayerEntityIterator(const Predicate<EGL::CGLEEntity>* const p)
	: ManagedIterator<EGL::CGLEEntity>(p)
{
	Players = { -1,-1,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 };
}
CppLogic::Iterator::MultiPlayerEntityIterator::MultiPlayerEntityIterator(const Predicate<EGL::CGLEEntity>* const p, std::initializer_list<int> pls)
	: MultiPlayerEntityIterator(p)
{
	if (pls.size() > 9)
		throw std::out_of_range("too many players");
	std::copy(pls.begin(), pls.end(), Players.begin());
}
EGL::CGLEEntity* CppLogic::Iterator::MultiPlayerEntityIterator::GetNextBase(int& c) const
{
	int base;
	int pl;
	if (c == -1) {
		if (Players[0] <= 0)
			return nullptr;
		base = 0;
		pl = 0;
	}
	else {
		base = (c & 0xFFFFFF) + 1;
		pl = (c & 0xF000000) >> (6 * 4);
	}
	const GGL::CPlayerAttractionHandler* ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(Players[pl])->PlayerAttractionHandler;
	if (base >= static_cast<int>(ah->EntityInSystem.size())) {
		while (true) {
			++pl;
			if (pl >= 9)
				return nullptr;
			if (Players[pl] >= 0) {
				ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(Players[pl])->PlayerAttractionHandler;
				if (ah->EntityInSystem.size() > 0)
					break;
			}
		}
		base = 0;
	}
	c = (pl << (6 * 4)) | base;
	return (*EGL::CGLEEntityManager::GlobalObj)->GetById(ah->EntityInSystem[base].EntityID);
}
EGL::CGLEEntity* CppLogic::Iterator::MultiPlayerEntityIterator::GetCurrentBase(int c) const
{
	if (c == -1)
		return nullptr;
	int base = (c & 0xFFFFFF);
	int pl = (c & 0xF000000) >> (6 * 4);
	const GGL::CPlayerAttractionHandler* ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(Players[pl])->PlayerAttractionHandler;
	if (base >= static_cast<int>(ah->EntityInSystem.size()))
		return nullptr;
	return(*EGL::CGLEEntityManager::GlobalObj)->GetById(ah->EntityInSystem[base].EntityID);
}



CppLogic::Iterator::EntityPredicateOfType::EntityPredicateOfType(int ety)
{
	this->ety = ety;
}
bool CppLogic::Iterator::EntityPredicateOfType::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->EntityType == ety;
}

CppLogic::Iterator::EntityPredicateOfPlayer::EntityPredicateOfPlayer(int pl)
{
	player = pl;
}
bool CppLogic::Iterator::EntityPredicateOfPlayer::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->PlayerId == player;
}

CppLogic::Iterator::EntityPredicateOfEntityCategory::EntityPredicateOfEntityCategory(shok::EntityCategory category)
{
	ecat = category;
}
bool CppLogic::Iterator::EntityPredicateOfEntityCategory::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->IsEntityInCategory(ecat);
}

CppLogic::Iterator::EntityPredicateProvidesResource::EntityPredicateProvidesResource(shok::ResourceType resType)
{
	res = resType;
}
bool CppLogic::Iterator::EntityPredicateProvidesResource::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->GetResourceProvided() == res;
}

CppLogic::Iterator::EntityPredicateOfUpgradeCategory::EntityPredicateOfUpgradeCategory(int category)
{
	ucat = category;
}
bool CppLogic::Iterator::EntityPredicateOfUpgradeCategory::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	const GGlue::CGlueEntityProps* t = e->GetEntityType();
	if (const GGL::CGLSettlerProps* s = dynamic_cast<const GGL::CGLSettlerProps*>(t->LogicProps))
		return s->Upgrade.Category == ucat;
	if (const GGL::CGLBuildingProps* s = dynamic_cast<const GGL::CGLBuildingProps*>(t->LogicProps))
		return s->Upgrade.Category == ucat;
	return false;
}

CppLogic::Iterator::EntityPredicateIsNotFleeingFrom::EntityPredicateIsNotFleeingFrom(const shok::Position& center, float r)
	: pos(center), range(r)
{
}
bool CppLogic::Iterator::EntityPredicateIsNotFleeingFrom::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	if (const EGL::CMovingEntity* m = dynamic_cast<const EGL::CMovingEntity*>(e))
		return !m->IsFleeingFrom(pos, range);
	return true;
}

bool CppLogic::Iterator::EntityPredicateIsSettler::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return dynamic_cast<const GGL::CSettler*>(e) != nullptr;
}
bool CppLogic::Iterator::EntityPredicateIsBuilding::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return dynamic_cast<const GGL::CBuilding*>(e) != nullptr;
}
bool CppLogic::Iterator::EntityPredicateIsCombatRelevant::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->PlayerId != 0 && (dynamic_cast<const GGL::CSettler*>(e) != nullptr || dynamic_cast<const GGL::CBuilding*>(e) != nullptr);
}
bool CppLogic::Iterator::EntityPredicateIsNotSoldier::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->GetBehavior<GGL::CSoldierBehavior>() == nullptr;
}
bool CppLogic::Iterator::EntityPredicateIsVisible::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	if (const GGL::CCamouflageBehavior* c = e->GetBehavior<GGL::CCamouflageBehavior>()) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}
bool CppLogic::Iterator::EntityPredicateIsAlive::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return !EGL::CGLEEntity::EntityIDIsDead(e->EntityId); // performance improvement by not doing obj -> id ->obj ?
}
bool CppLogic::Iterator::EntityPredicateIsNotInBuilding::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return !e->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING) && !e->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE);
}

CppLogic::Iterator::EntityPredicateOfAnyPlayer::EntityPredicateOfAnyPlayer()
{
	players = { -1,-1,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 };
}
CppLogic::Iterator::EntityPredicateOfAnyPlayer::EntityPredicateOfAnyPlayer(std::initializer_list<int> pl) : EntityPredicateOfAnyPlayer()
{
	if (pl.size() > players.size())
		throw std::out_of_range("too many players");
	std::copy(pl.begin(), pl.end(), players.begin());
}
bool CppLogic::Iterator::EntityPredicateOfAnyPlayer::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	for (int pl : players)
		if (e->PlayerId == pl)
			return true;
	return false;
}
int CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(std::array<int, 9>& players, int pl)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pl);
	int curr = 0;
	for (int p = 1; p <= 8; p++) {
		if (ps->GetDiploStateTo(p) == shok::DiploState::Hostile) {
			players[curr] = p;
			curr++;
		}
	}
	return curr;
}

CppLogic::Iterator::EntityPredicateOfAnyType::EntityPredicateOfAnyType(std::initializer_list<int> ety)
{
	entityTypes.resize(ety.size());
	std::copy(ety.begin(), ety.end(), entityTypes.begin());
}
bool CppLogic::Iterator::EntityPredicateOfAnyType::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	for (int ety : entityTypes)
		if (e->EntityType == ety)
			return true;
	return false;
}


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
	EGL::CGLEEntityManager* eman = *EGL::CGLEEntityManager::GlobalObj;
	while (true) {
		if (icurr > eman->HighestUsedSlot)
			return nullptr;
		EGL::CGLEEntity* e = eman->GetInSlot(icurr);
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
	return dynamic_cast<GGL::CSettler*>(e);
}

bool EntityIteratorPredicateIsBuilding::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return dynamic_cast<GGL::CBuilding*>(e);
}

bool EntityIteratorPredicateIsRelevant::MatchesEntity(EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->PlayerId != 0 && (dynamic_cast<GGL::CSettler*>(e) || dynamic_cast<GGL::CBuilding*>(e));
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
	GGL::CPlayerStatus* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(source);
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
	return static_cast<int>(e->GetResourceProvided()) == resource;
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
	if (!dynamic_cast<EGL::CMovingEntity*>(e))
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

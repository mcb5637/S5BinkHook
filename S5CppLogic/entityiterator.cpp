#include "pch.h"
#include "entityiterator.h"
#include "s5_entityandeffectmanager.h"
#include "s5_maplogic.h"
#include "s5_player.h"
#include "s5_entitytype.h"

CppLogic::Iterator::GlobalEntityIterator::GlobalEntityIterator(const Predicate<EGL::CGLEEntity>* const p)
	: GlobalManagedIterator<EGL::CGLEEntity>(*EGL::CGLEEntityManager::GlobalObj, p) {
}

CppLogic::Iterator::GlobalEffectIterator::GlobalEffectIterator(const Predicate<EGL::CEffect>* const p)
	: GlobalManagedIterator<EGL::CEffect>(*EGL::CGLEEffectManager::GlobalObj, p)
{
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
			++curr;
		}
	}
	return curr;
}
int  CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillFriendlyPlayers(std::array<int, 9>& players, int pl, bool addSelf)
{
	int curr = 0;
	if (addSelf) {
		players[curr] = pl;
		++curr;
	}
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pl);
	for (int p = 1; p <= 8; p++) {
		if (ps->GetDiploStateTo(p) == shok::DiploState::Friendly) {
			players[curr] = p;
			++curr;
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

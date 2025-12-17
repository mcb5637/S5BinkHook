#include "pch.h"
#include "entityiterator.h"
#include <shok/globals/s5_entityandeffectmanager.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/player/s5_player.h>
#include <shok/entitytype/s5_entitytype.h>

CppLogic::Iterator::GlobalEntityIterator::GlobalEntityIterator(const Predicate<EGL::CGLEEntity>* const p)
	: GlobalManagedIterator<EGL::CGLEEntity>(*EGL::CGLEEntityManager::GlobalObj, p) {
}

CppLogic::Iterator::GlobalEffectIterator::GlobalEffectIterator(const Predicate<EGL::CEffect>* const p)
	: GlobalManagedIterator<EGL::CEffect>(*EGL::CGLEEffectManager::GlobalObj, p)
{
}

CppLogic::Iterator::PlayerEntityIterator::PlayerEntityIterator(shok::PlayerId player, const Predicate<EGL::CGLEEntity>* const p)
	: ManagedIterator<EGL::CGLEEntity>(p), ah(*(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(player)->PlayerAttractionHandler)
{
}
EGL::CGLEEntity* CppLogic::Iterator::PlayerEntityIterator::GetNextBase(EntityIteratorStatus& c) const
{
	c.EntityIndex++;
	if (c.EntityIndex >= static_cast<int>(ah.EntityInSystem.size()))
		return nullptr;
	return (*EGL::CGLEEntityManager::GlobalObj)->GetById(static_cast<int>(ah.EntityInSystem[c.EntityIndex].EntityID));
}
EGL::CGLEEntity* CppLogic::Iterator::PlayerEntityIterator::GetCurrentBase(const EntityIteratorStatus& c) const
{
	if (c.EntityIndex >= static_cast<int>(ah.EntityInSystem.size()))
		return nullptr;
	return (*EGL::CGLEEntityManager::GlobalObj)->GetById(static_cast<int>(ah.EntityInSystem[c.EntityIndex].EntityID));
}

CppLogic::Iterator::MultiPlayerEntityIterator::MultiPlayerEntityIterator(const Predicate<EGL::CGLEEntity>* const p)
	: ManagedIterator<EGL::CGLEEntity>(p)
{
	static constexpr auto ip = shok::PlayerId::Invalid;
	Players = { ip,ip,ip ,ip ,ip ,ip ,ip ,ip ,ip };
}
CppLogic::Iterator::MultiPlayerEntityIterator::MultiPlayerEntityIterator(const Predicate<EGL::CGLEEntity>* const p, std::initializer_list<shok::PlayerId> pls)
	: MultiPlayerEntityIterator(p)
{
	if (pls.size() > 9)
		throw std::out_of_range("too many players");
	std::copy(pls.begin(), pls.end(), Players.begin());
}
EGL::CGLEEntity* CppLogic::Iterator::MultiPlayerEntityIterator::GetNextBase(EntityIteratorStatus& c) const
{
	int base;
	int pl;
	if (c.EntityIndex == -1) {
		if (Players[0] <= shok::PlayerId::P0)
			return nullptr;
		base = 0;
		pl = 0;
	}
	else {
		base = c.EntityIndex + 1;
		pl = c.X;
	}
	const GGL::CPlayerAttractionHandler* ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(Players[pl])->PlayerAttractionHandler;
	if (base >= static_cast<int>(ah->EntityInSystem.size())) {
		while (true) {
			++pl;
			if (pl >= 9)
				return nullptr;
			if (Players[pl] >= shok::PlayerId::P0) {
				ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(Players[pl])->PlayerAttractionHandler;
				if (ah->EntityInSystem.size() > 0)
					break;
			}
		}
		base = 0;
	}
	c.EntityIndex = base;
	c.X = pl;
	return (*EGL::CGLEEntityManager::GlobalObj)->GetById(static_cast<int>(ah->EntityInSystem[base].EntityID));
}
EGL::CGLEEntity* CppLogic::Iterator::MultiPlayerEntityIterator::GetCurrentBase(const EntityIteratorStatus& c) const
{
	if (c.EntityIndex == -1)
		return nullptr;
	int base = c.EntityIndex;
	int pl = c.X;
	const GGL::CPlayerAttractionHandler* ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(Players[pl])->PlayerAttractionHandler;
	if (base >= static_cast<int>(ah->EntityInSystem.size()))
		return nullptr;
	return(*EGL::CGLEEntityManager::GlobalObj)->GetById(static_cast<int>(ah->EntityInSystem[base].EntityID));
}

CppLogic::Iterator::MultiRegionEntityIterator::MultiRegionEntityIterator(float x1, float y1, float x2, float y2, shok::AccessCategoryFlags accessCategories,
	const Predicate<EGL::CGLEEntity>* const p)
	: ManagedIterator<EGL::CGLEEntity>(p), AccessCat(accessCategories),
	BaseX((*EGL::CGLEGameLogic::GlobalObj)->RegionDataEntityObj.GetSingleEntryComponent(std::min(x1, x2))),
	BaseY((*EGL::CGLEGameLogic::GlobalObj)->RegionDataEntityObj.GetSingleEntryComponent(std::min(y1, y2))),
	EndX((*EGL::CGLEGameLogic::GlobalObj)->RegionDataEntityObj.GetSingleEntryComponent(std::max(x1, x2))),
	EndY((*EGL::CGLEGameLogic::GlobalObj)->RegionDataEntityObj.GetSingleEntryComponent(std::max(y1, y2)))
{
}
CppLogic::Iterator::MultiRegionEntityIterator::MultiRegionEntityIterator(const shok::AARect& area, shok::AccessCategoryFlags accessCategories,
	const Predicate<EGL::CGLEEntity>* const p)
	: MultiRegionEntityIterator(area.high.X, area.high.Y, area.low.X, area.low.Y, accessCategories, p)
{
}
CppLogic::Iterator::MultiRegionEntityIterator::MultiRegionEntityIterator(const shok::Position& center, float range, shok::AccessCategoryFlags accessCategories,
	const Predicate<EGL::CGLEEntity>* const p)
	: MultiRegionEntityIterator(center.X + range, center.Y + range, center.X - range, center.Y - range, accessCategories, p)
{
}

EGL::CGLEEntity* CppLogic::Iterator::MultiRegionEntityIterator::GetNextBase(EntityIteratorStatus& c) const
{
	if (c.EntityIndex == -1) {
		c.X = BaseX - 1;
		c.Y = BaseY;
		NextRegion(c);
	}
	EGL::RegionDataEntity& reg = (*EGL::CGLEGameLogic::GlobalObj)->RegionDataEntityObj;
	while (true) {
		++c.EntityIndex;
		auto* ent = reg.GetEntry(c.X, c.Y);
		if (ent != nullptr) {
			if (c.EntityIndex < static_cast<int>(ent->GetByAccessCategory(c.AccessCat).size()))
				return ent->GetByAccessCategory(c.AccessCat)[c.EntityIndex];
			if (NextAccessCategory(c))
				continue;
		}
		if (NextRegion(c))
			continue;
		return nullptr;
	}
	return nullptr;
}
EGL::CGLEEntity* CppLogic::Iterator::MultiRegionEntityIterator::GetCurrentBase(const EntityIteratorStatus& c) const
{
	if (c.EntityIndex == -1)
		return nullptr;
	EGL::RegionDataEntity& reg = (*EGL::CGLEGameLogic::GlobalObj)->RegionDataEntityObj;
	auto* ent = reg.GetEntry(c.X, c.Y);
	return ent->GetByAccessCategory(c.AccessCat)[c.EntityIndex];
}
bool CppLogic::Iterator::MultiRegionEntityIterator::NextAccessCategory(EntityIteratorStatus& c) const {
	int ac = static_cast<int>(c.AccessCat);
	c.EntityIndex = -1;
	while (true) {
		++ac;
		if (ac > static_cast<int>(shok::AccessCategory::AccessCategoryOrnamental)) {
			c.AccessCat = shok::AccessCategory::AccessCategoryNone;
			return false;
		}
		if ((1 << ac) & static_cast<int>(this->AccessCat)) {
			c.AccessCat = static_cast<shok::AccessCategory>(ac);
			return true;
		}
	}
}
bool CppLogic::Iterator::MultiRegionEntityIterator::NextRegion(EntityIteratorStatus& c) const {
	int s = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes->MaxSizeX >> 5;
	while (true) {
		++c.X;
		if (c.X > EndX) {
			c.X = BaseX - 1;
			++c.Y;
			if (c.Y > EndY)
				return false;
			continue;
		}
		if (c.X < 0 || c.X > s)
			continue;
		if (c.Y < 0 || c.Y > s)
			continue;
		c.AccessCat = shok::AccessCategory::AccessCategoryNone;
		NextAccessCategory(c);
		return true;
	}
}



CppLogic::Iterator::EntityPredicateOfType::EntityPredicateOfType(shok::EntityTypeId ety)
{
	this->ety = ety;
}
bool CppLogic::Iterator::EntityPredicateOfType::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->EntityType == ety;
}

CppLogic::Iterator::EntityPredicateOfPlayer::EntityPredicateOfPlayer(shok::PlayerId pl)
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

CppLogic::Iterator::EntityPredicateOfUpgradeCategory::EntityPredicateOfUpgradeCategory(shok::UpgradeCategoryId category)
{
	ucat = category;
}
bool CppLogic::Iterator::EntityPredicateOfUpgradeCategory::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	const GGlue::CGlueEntityProps* t = e->GetEntityType();
	if (const auto* s = BB::IdentifierCast<const GGL::CGLSettlerProps>(t->LogicProps))
		return s->Upgrade.Category == ucat;
	if (const auto* s = BB::IdentifierCast<const GGL::CGLBuildingProps, EGL::CGLEEntityProps, GGL::CBridgeProperties>(t->LogicProps))
		return s->Upgrade.Category == ucat;
	return false;
}

CppLogic::Iterator::EntityPredicateIsNotFleeingFrom::EntityPredicateIsNotFleeingFrom(const shok::Position& center, float r)
	: pos(center), range(r)
{
}
bool CppLogic::Iterator::EntityPredicateIsNotFleeingFrom::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	if (const auto* m = dynamic_cast<const EGL::CMovingEntity*>(e))
		return !m->IsFleeingFrom(pos, range);
	return true;
}

bool CppLogic::Iterator::EntityPredicateIsSettler::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	const auto id = e->GetClassIdentifier();
	return id == GGL::CSettler::Identifier;
}
bool CppLogic::Iterator::EntityPredicateIsBuilding::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	const auto id = e->GetClassIdentifier();
	return id == GGL::CBuilding::Identifier || id == GGL::CBridgeEntity::Identifier || id == GGL::CConstructionSite::Identifier;
}
bool CppLogic::Iterator::EntityPredicateIsBuildingAndNotConstructionSite::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	const auto id = e->GetClassIdentifier();
	return id == GGL::CBuilding::Identifier || id == GGL::CBridgeEntity::Identifier;
}
bool CppLogic::Iterator::EntityPredicateIsCombatRelevant::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	const auto id = e->GetClassIdentifier();
	return e->PlayerId != shok::PlayerId::P0
		&& (id == GGL::CBuilding::Identifier || id == GGL::CBridgeEntity::Identifier || id == GGL::CConstructionSite::Identifier || id == GGL::CSettler::Identifier);
}
bool CppLogic::Iterator::EntityPredicateIsNotSoldier::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->GetBehavior<GGL::CSoldierBehavior>() == nullptr;
}
bool CppLogic::Iterator::EntityPredicateIsVisible::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	if (const auto* c = e->GetBehaviorDynamic<GGL::CCamouflageBehavior>()) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
}
bool CppLogic::Iterator::EntityPredicateIsAlive::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return !e->IsDead();
}
bool CppLogic::Iterator::EntityPredicateIsNotInBuilding::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	return e->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING) == shok::EntityId::Invalid && e->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE) == shok::EntityId::Invalid;
}

CppLogic::Iterator::EntityPredicateOfAnyPlayer::EntityPredicateOfAnyPlayer()
{
	static constexpr auto ip = shok::PlayerId::Invalid;
	players = { ip,ip,ip ,ip ,ip ,ip ,ip ,ip ,ip };
}
CppLogic::Iterator::EntityPredicateOfAnyPlayer::EntityPredicateOfAnyPlayer(std::initializer_list<shok::PlayerId> pl) : EntityPredicateOfAnyPlayer()
{
	if (pl.size() > players.size())
		throw std::out_of_range("too many players");
	std::copy(pl.begin(), pl.end(), players.begin());
}
bool CppLogic::Iterator::EntityPredicateOfAnyPlayer::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	for (auto pl : players)
		if (e->PlayerId == pl)
			return true;
	return false;
}
int CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(std::array<shok::PlayerId, 9>& players, shok::PlayerId pl)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pl);
	int curr = 0;
	for (int p = 1; p <= 8; p++) {
		if (ps->GetDiploStateTo(static_cast<shok::PlayerId>(p)) == shok::DiploState::Hostile) {
			players[curr] = static_cast<shok::PlayerId>(p);
			++curr;
		}
	}
	return curr;
}
int  CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillFriendlyPlayers(std::array<shok::PlayerId, 9>& players, shok::PlayerId pl, bool addSelf)
{
	int curr = 0;
	if (addSelf) {
		players[curr] = pl;
		++curr;
	}
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pl);
	for (int p = 1; p <= 8; p++) {
		if (ps->GetDiploStateTo(static_cast<shok::PlayerId>(p)) == shok::DiploState::Friendly) {
			players[curr] = static_cast<shok::PlayerId>(p);
			++curr;
		}
	}
	return curr;
}

CppLogic::Iterator::EntityPredicateOfAnyType::EntityPredicateOfAnyType(std::initializer_list<shok::EntityTypeId> ety)
{
	entityTypes.resize(ety.size());
	std::copy(ety.begin(), ety.end(), entityTypes.begin());
}
bool CppLogic::Iterator::EntityPredicateOfAnyType::Matches(const EGL::CGLEEntity* e, float* rangeOut, int* prio) const
{
	for (shok::EntityTypeId ety : entityTypes)
		if (e->EntityType == ety)
			return true;
	return false;
}

CppLogic::Iterator::EffectPredicateOfType::EffectPredicateOfType(shok::EffectTypeId ty)
{
	Type = ty;
}
bool CppLogic::Iterator::EffectPredicateOfType::Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const
{
	return e->EffectType == Type;
}

CppLogic::Iterator::EffectPredicateOfPlayer::EffectPredicateOfPlayer(shok::PlayerId pl)
{
	Player = pl;
}
bool CppLogic::Iterator::EffectPredicateOfPlayer::Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const
{
	return e->PlayerID == Player;
}

bool CppLogic::Iterator::EffectPredicateIsArrow::Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const
{
	return e->GetClassIdentifier() == GGL::CArrowEffect::Identifier;
}

bool CppLogic::Iterator::EffectPredicateIsCannonBall::Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const
{
	return e->GetClassIdentifier() == GGL::CCannonBallEffect::Identifier;
}

bool CppLogic::Iterator::EffectPredicateIsArrowOrCannonBall::Matches(const EGL::CEffect* e, float* rangeOut, int* prio) const
{
	auto id = e->GetClassIdentifier();
	return id == GGL::CArrowEffect::Identifier || id == GGL::CCannonBallEffect::Identifier;
}

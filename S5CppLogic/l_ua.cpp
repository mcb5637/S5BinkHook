#include "pch.h"
#include "l_ua.h"
#include "luaext.h"
#include "l_entity.h"
#include <random>
#include "entityiterator.h"


std::vector<UACannonBuilderAbilityData> UnlimitedArmy::CannonBuilderAbilityData = std::vector<UACannonBuilderAbilityData>();

UnlimitedArmy::~UnlimitedArmy()
{
	luaL_unref(L, LUA_REGISTRYINDEX, Formation);
	luaL_unref(L, LUA_REGISTRYINDEX, CommandQueue);
	luaL_unref(L, LUA_REGISTRYINDEX, Spawner);
	luaL_unref(L, LUA_REGISTRYINDEX, Normalize);
}
UnlimitedArmy::UnlimitedArmy(int p)
{
	Player = p;
}
void UnlimitedArmy::CalculatePos()
{
	int tick = (*shok_EGL_CGLEGameLogic::GlobalObj)->GetTick();
	if (tick == PosLastUpdatedTick)
		return;
	float x = 0, y = 0;
	int num = 0;
	for (int id : Leaders) {
		bool con = false;
		for (UAReset& r : PrepDefenseReset)
			if (r.EntityId == id) {
				con = true;
				break;
			}
		if (con)
			continue;
		shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(id);
		if (e != nullptr) {
			x += e->Position.X;
			y += e->Position.Y;
			num++;
		}
	}
	if (num == 0) {
		LastPos.X = -1;
		LastPos.Y = -1;
	}
	else {
		LastPos.X = x / num;
		LastPos.Y = y / num;
	}
	PosLastUpdatedTick = tick;
}
void UnlimitedArmy::CleanDead()
{
	auto remo = [this](int id) {
		if (shok_EGL_CGLEEntity::EntityIDIsDead(id)) {
			shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(id);
			if (e != nullptr && e->IsEntityInCategory(shok::EntityCategory::Hero)) {
				this->DeadHeroes.push_back(id);
			}
			NeedFormat();
			return true;
		}
		return false;
	};
	auto e = std::remove_if(Leaders.begin(), Leaders.end(), remo);
	Leaders.erase(e, Leaders.end());
	e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), remo);
	LeaderInTransit.erase(e, LeaderInTransit.end());
	auto e2 = std::remove_if(Cannons.begin(), Cannons.end(), [](UACannonData& d) { return shok_EGL_CGLEEntity::EntityIDIsDead(d.EntityId); });
	Cannons.erase(e2, Cannons.end());
	e = std::remove_if(DeadHeroes.begin(), DeadHeroes.end(), [this](int id) {
		if (!shok_EGL_CGLEEntity::EntityIDIsDead(id)) {
			this->AddLeader(shok_EGL_CGLEEntity::GetEntityByID(id));
			return true;
		}
		return shok_EGL_CGLEEntity::GetEntityByID(id) == nullptr;
		});
	DeadHeroes.erase(e, DeadHeroes.end());
	int tick = (*shok_EGL_CGLEGameLogic::GlobalObj)->GetTick();
	TargetCache.erase(std::remove_if(TargetCache.begin(), TargetCache.end(), [tick](UATargetCache& c) { return c.Tick < tick; }), TargetCache.end());
}
void UnlimitedArmy::Tick()
{
	CleanDead();
	CalculatePos();
	CheckTransit();
	CallSpawner();
	if (Leaders.size() == 0) {
		CallCommandQueue();
		return;
	}
	PosLastUpdatedTick = -1;
	CalculatePos();
	if (!IsTargetValid(CurrentBattleTarget)) {
		shok_EGL_CGLEEntity* e = GetNearestTargetInArea(Player, LastPos, Area, IgnoreFleeing);
		if (e == nullptr)
			CurrentBattleTarget = 0;
		else {
			CurrentBattleTarget = e->EntityId;
			for (UACannonData d : Cannons) {
				d.LastUpdated = -1;
			}
		}
	}
	bool preventComands = false;
	if (!preventComands && Status != UAStatus::MovingNoBattle && CurrentBattleTarget != 0) {
		CheckStatus(UAStatus::Battle);
		preventComands = true;
	}
	if (!preventComands && !LastPos.IsInRange(Target, 1500)) {
		if (Status != UAStatus::Moving && Status != UAStatus::MovingNoBattle)
			CheckStatus(UAStatus::Moving);
		preventComands = true;
	}
	if (!preventComands) {
		CheckStatus(UAStatus::Idle);
		if (AutoRotateFormation >= 0) {
			shok_EGL_CGLEEntity* e = GetNearestTargetInArea(Player, LastPos, AutoRotateFormation, IgnoreFleeing);
			if (e != nullptr) {
				float a = LastPos.GetAngleBetween(e->Position);
				if (std::fabsf(a - LastRotation) > 10) {
					LastRotation = a;
					ReMove = true;
				}
			}
		}
		preventComands = true;
	}
	CallCommandQueue();
	if (Status == UAStatus::Battle)
		BattleCommand();
	else if (Status == UAStatus::Moving || Status == UAStatus::MovingNoBattle)
		MoveCommand();
	else if (Status == UAStatus::Idle)
		FormationCommand();
}
void UnlimitedArmy::NeedFormat()
{
	if (Status == UAStatus::Idle)
		Status = UAStatus::IdleUnformated;
}
void UnlimitedArmy::AddLeader(shok_EGL_CGLEEntity* e)
{
	CalculatePos();
	if (Leaders.size() == 0 || LastPos.IsInRange(e->Position, Area)) {
		Leaders.push_back(e->EntityId);
		PosLastUpdatedTick = -1;
		if (Target.X == -1) {
			Target = e->Position;
		}
		NeedFormat();
		if (e->IsEntityInCategory(shok::EntityCategory::Cannon))
			Cannons.push_back({ e->EntityId, -1 });
	}
	else
		LeaderInTransit.push_back(e->EntityId);
}
void UnlimitedArmy::OnIdChanged(int old, int ne)
{
	for (int i = 0; i < static_cast<int>(Leaders.size()); i++) {
		if (Leaders[i] == old)
			Leaders[i] = ne;
	}
	for (int i = 0; i < static_cast<int>(LeaderInTransit.size()); i++) {
		if (LeaderInTransit[i] == old)
			LeaderInTransit[i] = ne;
	}
	for (int i = 0; i < static_cast<int>(DeadHeroes.size()); i++) {
		if (DeadHeroes[i] == old)
			DeadHeroes[i] = ne;
	}
	for (int i = 0; i < static_cast<int>(Cannons.size()); i++) {
		if (Cannons[i].EntityId == old)
			Cannons[i].EntityId = ne;
	}
}
void UnlimitedArmy::CheckTransit()
{
	if (Leaders.size() == 0 && LeaderInTransit.size() > 0) {
		Leaders.push_back(LeaderInTransit[0]);
		if (shok_EGL_CGLEEntity::GetEntityByID(LeaderInTransit[0])->IsEntityInCategory(shok::EntityCategory::Cannon))
			Cannons.push_back({ LeaderInTransit[0], -1 });
		LeaderInTransit.erase(LeaderInTransit.begin());
		PosLastUpdatedTick = -1;
		CalculatePos();
		NeedFormat();
	}
	auto e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), [this](int id) {
		shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(id);
		if (this->LastPos.IsInRange(e->Position, this->Area)) {
			this->Leaders.push_back(id);
			if (e->IsEntityInCategory(shok::EntityCategory::Cannon))
				this->Cannons.push_back({ e->EntityId, -1 });
			this->NeedFormat();
			return true;
		}
		return false;
	});
	LeaderInTransit.erase(e, LeaderInTransit.end());
	for (int id : LeaderInTransit) {
		shok_EGL_CMovingEntity* e = ((shok_EGL_CMovingEntity*)shok_EGL_CGLEEntity::GetEntityByID(id));
		if (!LeaderIsMoving(e)) {
			e->Move(LastPos);
		}
	}
}
int UnlimitedArmy::GetSize(bool transit, bool hero)
{
	int r = Leaders.size();
	if (transit)
		r += LeaderInTransit.size();
	if (hero)
		r += DeadHeroes.size();
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestTargetInArea(int player, shok::Position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory catLeader = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::TargetFilter_TargetTypeLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::TargetFilter_TargetType);
	EntityIteratorPredicatePriority prio1 = EntityIteratorPredicatePriority(2, &catLeader);
	EntityIteratorPredicatePriority prio2 = EntityIteratorPredicatePriority(1, &buil);
	EntityIteratorPredicateNotInBuilding notinbuild = EntityIteratorPredicateNotInBuilding();
	EntityIteratorPredicate* preds[11] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &notinbuild, &cir, &prio1, &prio2, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 11 : 10);
	EntityIterator iter = EntityIterator(&a);
	shok_EGL_CGLEEntity* r = iter.GetNearest(nullptr);
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetFurthestConversionTargetInArea(int player, shok::Position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::TargetFilter_TargetTypeLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicateNotInBuilding notinbuild = EntityIteratorPredicateNotInBuilding();
	EntityIteratorPredicateFunc fun = EntityIteratorPredicateFunc([this](shok_EGL_CGLEEntity* e) {
		return this->CheckTargetCache(e->EntityId, 1);
		});
	EntityIteratorPredicate* preds[10] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &notinbuild, &cir, &fun, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 10 : 9);
	EntityIterator iter = EntityIterator(&a);
	shok_EGL_CGLEEntity* r = iter.GetFurthest(nullptr);
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestSettlerInArea(int player, shok::Position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::TargetFilter_TargetTypeLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicateNotInBuilding notinbuild = EntityIteratorPredicateNotInBuilding();
	EntityIteratorPredicate* preds[9] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &notinbuild, &cir, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 9 : 8);
	EntityIterator iter = EntityIterator(&a);
	return iter.GetNearest(nullptr);
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestBuildingInArea(int player, shok::Position& p, float ran) {
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateFunc fun = EntityIteratorPredicateFunc([this](shok_EGL_CGLEEntity* e) {
		return this->CheckTargetCache(e->EntityId, 2);
		});
	EntityIteratorPredicate* preds[7] = {
		&relev, &pl, &al, &buil, &vis, &cir, &fun
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, 7);
	EntityIterator iter = EntityIterator(&a);
	return iter.GetNearest(nullptr);
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestBridgeInArea(shok::Position& p, float ran) {
	EntityIteratorPredicateOfPlayer pl = EntityIteratorPredicateOfPlayer(0);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::Bridge);
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateFunc fun = EntityIteratorPredicateFunc([this](shok_EGL_CGLEEntity* e) {
		return this->CheckTargetCache(e->EntityId, 1);
		});
	EntityIteratorPredicate* preds[5] = {
		&pl, &buil, &cat, &cir, &fun
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, 5);
	EntityIterator iter = EntityIterator(&a);
	return iter.GetNearest(nullptr);
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestSnipeTargetInArea(int player, shok::Position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);

	EntityIteratorPredicateOfEntityCategory he = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::Hero);
	EntityIteratorPredicateOfEntityCategory can = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::Cannon);
	EntityIteratorPredicate* cats[2] = {
		&he, &can
	};
	EntityIteratorPredicateOr cat = EntityIteratorPredicateOr(cats, 2);

	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateNotInBuilding notinbuild = EntityIteratorPredicateNotInBuilding();
	EntityIteratorPredicateFunc fun = EntityIteratorPredicateFunc([this](shok_EGL_CGLEEntity* e) {
		return this->CheckTargetCache(e->EntityId, 1);
		});
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicate* preds[9] = {
		&relev, &pl, &cat, &al, &vis, &notinbuild, &cir, &fun, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 9 : 8);
	EntityIterator iter = EntityIterator(&a);
	return iter.GetNearest(nullptr);
}
bool UnlimitedArmy::IsTargetValid(int id)
{
	if (shok_EGL_CGLEEntity::EntityIDIsDead(id))
		return false;
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(id);
	if (!LastPos.IsInRange(e->Position, Area))
		return false;
	GGL::CCamouflageBehavior* c = e->GetBehavior<GGL::CCamouflageBehavior>();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	if (!IgnoreFleeing)
		return true;
	return EntityIteratorPredicateIsNotFleeingFrom::IsNotFleeingFrom(e, LastPos, 500);
}
void UnlimitedArmy::CheckStatus(UAStatus status)
{
	if (Status != status) {
		Status = status;
		ReMove = true;
	}
}
bool UnlimitedArmy::LeaderIsMoving(shok_EGL_CGLEEntity* e)
{
	shok::LeaderCommand comm = e->EventLeaderGetCurrentCommand();
	return (comm == shok::LeaderCommand::Move || comm == shok::LeaderCommand::AttackMove || comm == shok::LeaderCommand::Patrol) && !LeaderIsIdle(e);
}
bool UnlimitedArmy::LeaderIsIdle(shok_EGL_CGLEEntity* e)
{
	return static_cast<shok_GGL_CSettler*>(e)->IsIdle();
}
bool UnlimitedArmy::LeaderIsInBattle(shok_EGL_CGLEEntity* e)
{
	shok::LeaderCommand comm = e->EventLeaderGetCurrentCommand();
	return (comm == shok::LeaderCommand::Attack || comm == shok::LeaderCommand::AttackMove || comm == shok::LeaderCommand::HeroAbility) && !LeaderIsIdle(e);
}
bool UnlimitedArmy::IsRanged(shok_EGL_CGLEEntity* e)
{
	return e->IsEntityInCategory(shok::EntityCategory::LongRange) || e->IsEntityInCategory(shok::EntityCategory::Cannon) || e->IsEntityInCategory(shok::EntityCategory::TargetFilter_CustomRanged);
}
bool UnlimitedArmy::IsNonCombat(shok_EGL_CGLEEntity* e)
{
	return e->IsEntityInCategory(shok::EntityCategory::TargetFilter_NonCombat);
}
void UnlimitedArmy::BattleCommand()
{
	shok::Position p = shok_EGL_CGLEEntity::GetEntityByID(CurrentBattleTarget)->Position;
	shok_EGL_CGLELandscape* ls = (*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape;
	if (ls->GetSector(&p) == 0) {
		shok::Position pou;
		if (ls->GetNearestPositionInSector(&p, 1000, shok_EGL_CGLEEntity::GetEntityByID(Leaders[0])->GetSector(), &pou))
			p = pou;
	}
	if (ReMove) {
		PrepDefenseReset.clear();
		NormalizeSpeed(false, false);
	}
	for (int id : Leaders) {
		shok_EGL_CMovingEntity* e = static_cast<shok_EGL_CMovingEntity*>(shok_EGL_CGLEEntity::GetEntityByID(id));
		if (e->IsEntityInCategory(shok::EntityCategory::Hero) || IsNonCombat(e)) {
			if (ExecuteHeroAbility(e))
				continue;
		}
		if ((ReMove || !LeaderIsInBattle(e) ) && !IsNonCombat(e) && !e->IsEntityInCategory(shok::EntityCategory::Cannon)) {
			if (IsRanged(e)) {
				e->AttackEntity(CurrentBattleTarget);
			}
			else {
				e->AttackMove(p);
			}
		}
	}
	int tick = (*shok_EGL_CGLEGameLogic::GlobalObj)->GetTick();
	for (UACannonData cd : Cannons) {
		shok_EGL_CMovingEntity* e = static_cast<shok_EGL_CMovingEntity*>(shok_EGL_CGLEEntity::GetEntityByID(cd.EntityId));
		if (ReMove || !LeaderIsInBattle(e) || cd.LastUpdated == -1) {
			if (cd.LastUpdated < 0 || cd.LastUpdated < tick) {
				e->AttackEntity(CurrentBattleTarget);
				cd.LastUpdated = tick + 1;
			}
			else {
				e->AttackMove(p);
				cd.LastUpdated = -1;
			}
		}
	}
	ReMove = false;
}
void UnlimitedArmy::MoveCommand()
{
	if (ReMove) {
		PrepDefenseReset.clear();
		NormalizeSpeed(true, false);
	}
	shok::Position p = Target;
	shok_EGL_CGLELandscape* ls = (*shok_EGL_CGLEGameLogic::GlobalObj)->Landscape;
	if (ls->GetSector(&p) == 0) {
		shok::Position pou;
		if (ls->GetNearestPositionInSector(&p, 1000, shok_EGL_CGLEEntity::GetEntityByID(Leaders[0])->GetSector(), &pou))
			p = pou;
	}
	if (Status == UAStatus::MovingNoBattle) {
		for (int id : Leaders) {
			shok_EGL_CMovingEntity* e = static_cast<shok_EGL_CMovingEntity*>(shok_EGL_CGLEEntity::GetEntityByID(id));
			if (ReMove || !LeaderIsMoving(e)) {
				e->Move(p);
			}
		}
	}
	else {
		for (int id : Leaders) {
			shok_EGL_CMovingEntity* e = static_cast<shok_EGL_CMovingEntity*>(shok_EGL_CGLEEntity::GetEntityByID(id));
			if (ReMove || LeaderIsIdle(e)) {
				e->AttackMove(p);
			}
		}
	}
	ReMove = false;
}
void UnlimitedArmy::FormationCommand()
{
	if (ReMove) {
		NormalizeSpeed(false, false);
		PrepDefenseReset.clear();
		CallFormation();
	}
	else if (PrepDefense && IsIdle()) {
		for (int id : Leaders) {
			shok_EGL_CMovingEntity* e = static_cast<shok_EGL_CMovingEntity*>(shok_EGL_CGLEEntity::GetEntityByID(id));
			if ((e->IsEntityInCategory(shok::EntityCategory::Hero) || IsNonCombat(e)) && e->EventLeaderGetCurrentCommand() != shok::LeaderCommand::HeroAbility) {
				if (ExecutePrepDefense(e)) {
					UAReset r;
					r.EntityId = id;
					r.Pos = e->Position;
					PrepDefenseReset.push_back(r);
				}
			}
		}
		PrepDefenseReset.erase(std::remove_if(PrepDefenseReset.begin(), PrepDefenseReset.end(), [this](UAReset& r) {
			if (shok_EGL_CGLEEntity::EntityIDIsDead(r.EntityId))
				return true;
			shok_EGL_CMovingEntity* e = static_cast<shok_EGL_CMovingEntity*>(shok_EGL_CGLEEntity::GetEntityByID(r.EntityId));
			if (e->EventLeaderGetCurrentCommand() != shok::LeaderCommand::HeroAbility) {
				shok_EGL_CMovingEntity* m = static_cast<shok_EGL_CMovingEntity*>(e);
				m->Move(r.Pos);
				m->SetTargetRotation(r.Pos.r);
				shok_EGL_CEventValue_bool ev{ shok_EventIDs::Leader_SetIsUsingTargetOrientation, true };
				m->FireEvent(&ev);
				return true;
			}
			return false;
			}), PrepDefenseReset.end());
	}
	ReMove = false;
}
void UnlimitedArmy::RemoveLeader(shok_EGL_CGLEEntity* e)
{
	int id = e->EntityId;
	auto f = [id](int i) { return i == id; };
	auto en = std::remove_if(Leaders.begin(), Leaders.end(), f);
	Leaders.erase(en, Leaders.end());
	en = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), f);
	LeaderInTransit.erase(en, LeaderInTransit.end());
	NeedFormat();
}
bool UnlimitedArmy::IsIdle()
{
	if (GetSize(true, false) == 0)
		return true;
	if (Status != UAStatus::Idle)
		return false;
	if (LeaderInTransit.size() > 0)
		return false;
	CalculatePos();
	if (!LastPos.IsInRange(Target, 1000))
		return false;
	for (int id : Leaders)
		if (!LeaderIsIdle(shok_EGL_CGLEEntity::GetEntityByID(id)))
			return false;
	return true;
}
int UnlimitedArmy::CountTargetsInArea(int player, shok::Position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok::EntityCategory::TargetFilter_TargetType);
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicate* preds[7] = {
		&relev, &pl, &cat, &al, &vis, &cir, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 7 : 6);
	EntityIterator iter = EntityIterator(&a);
	int count = 0;
	shok_EGL_CGLEEntity* e = nullptr;
	while (true) {
		e = iter.GetNext(nullptr, nullptr);
		if (e == nullptr)
			break;
		count++;
	}
	return count;
}
void UnlimitedArmy::CallFormation()
{
	int t = lua_gettop(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, Formation);
	lua_pushnumber(L, LastRotation);
	lua_call(L, 1, 0); // errors get propagated to lua tick
	lua_settop(L, t);
}
void UnlimitedArmy::CallCommandQueue()
{
	int t = lua_gettop(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, CommandQueue);
	lua_call(L, 0, 0); // errors get propagated to lua tick
	lua_settop(L, t);
}
void UnlimitedArmy::CallSpawner()
{
	int t = lua_gettop(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, Spawner);
	lua_call(L, 0, 0); // errors get propagated to lua tick
	lua_settop(L, t);
}
void UnlimitedArmy::NormalizeSpeed(bool normalize, bool force)
{
	int t = lua_gettop(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, Normalize);
	lua_pushboolean(L, normalize);
	lua_pushboolean(L, force);
	lua_call(L, 2, 0); // errors get propagated to lua tick
	lua_settop(L, t);
}
std::random_device                  rand_dev;
std::mt19937                        generator(rand_dev());
std::uniform_int_distribution<int>  distr(-10, 10);
bool UnlimitedArmy::ExecuteHeroAbility(shok_EGL_CGLEEntity* e)
{
	// first instant abilities
	{
		GGL::CRangedEffectAbility* a = e->GetBehavior<GGL::CRangedEffectAbility>();
		if (a != nullptr) {
			GGL::CRangedEffectAbilityProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				float r = 1000;
				if (p->AffectsLongRangeOnly)
					r = 3000;
				if (p->IsAggressive()) {
					if (!static_cast<shok_GGL_CSettler*>(e)->IsMoving() && CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 10) {
						static_cast<shok_GGL_CSettler*>(e)->HeroAbilityRangedEffect();
					}
				}
				else if (p->IsDefensive()) {
					if (CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 10) {
						static_cast<shok_GGL_CSettler*>(e)->HeroAbilityRangedEffect();
					}
				}
				else if (p->IsHeal()) {
					if (e->Health <= e->GetMaxHealth()/2 && CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 2) {
						static_cast<shok_GGL_CSettler*>(e)->HeroAbilityRangedEffect();
					}
				}
			}
		}
	}
	{
		GGL::CSummonBehavior* a = e->GetBehavior<GGL::CSummonBehavior>();
		if (a != nullptr) {
			GGL::CSummonBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CSummonBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, Area, IgnoreFleeing) >= 10) {
					static_cast<shok_GGL_CSettler*>(e)->HeroAbilitySummon();
					e->ObservedEntities.ForAll([this](shok::Attachment* a) {
						if (a->AttachmentType == shok::AttachmentType::SUMMONER_SUMMONED)
							AddLeader(shok_EGL_CGLEEntity::GetEntityByID(a->EntityId));
						});
				}
			}
		}
	}

	// then noninstant
	{
		GGL::CCircularAttack* a = e->GetBehavior<GGL::CCircularAttack>();
		if (a != nullptr) {
			GGL::CCircularAttackProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CCircularAttackProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range, IgnoreFleeing) >= 10) {
					static_cast<shok_GGL_CSettler*>(e)->HeroAbilityCircularAttack();
					return true;
				}
			}
		}
	}
	{
		GGL::CBombPlacerBehavior* a = e->GetBehavior<GGL::CBombPlacerBehavior>();
		if (a != nullptr) {
			GGL::CHeroAbilityProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CHeroAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, 500, IgnoreFleeing) >= 10) {
					static_cast<shok_GGL_CSettler*>(e)->HeroAbilityPlaceBomb(e->Position);
					return true;
				}
			}
		}
	}
	{
		GGL::CCannonBuilderBehavior* a = e->GetBehavior<GGL::CCannonBuilderBehavior>();
		if (a != nullptr) {
			GGL::CCannonBuilderBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CCannonBuilderBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				int ety = e->EntityType;
				auto d = std::find_if(CannonBuilderAbilityData.begin(), CannonBuilderAbilityData.end(), [ety](UACannonBuilderAbilityData& d) {return d.HeroType == ety; });
				if (d != CannonBuilderAbilityData.end() && CountTargetsInArea(Player, e->Position, 2000, IgnoreFleeing) >= 10) {
					shok::Position p = e->Position;
					p.X += distr(generator)*100;
					p.Y += distr(generator)*100;
					p.FloorToBuildingPlacement();
					if (shok_canPlaceBuilding(d->BottomType, Player, &p, 0, 0)) {
						static_cast<shok_GGL_CSettler*>(e)->HeroAbilityPlaceCannon(p, d->BottomType, d->TopType);
						return true;
					}
				}
			}
		}
	}
	{
		GGL::CShurikenAbility* a = e->GetBehavior<GGL::CShurikenAbility>();
		if (a != nullptr) {
			GGL::CShurikenAbilityProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CShurikenAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range, IgnoreFleeing) >= 10) {
					shok_EGL_CGLEEntity* tar = GetNearestSettlerInArea(Player, e->Position, p->Range, IgnoreFleeing);
					if (tar != nullptr) {
						static_cast<shok_GGL_CSettler*>(e)->HeroAbilityShuriken(tar->EntityId);
						return true;
					}
				}
			}
		}
	}
	{
		GGL::CConvertSettlerAbility* a = e->GetBehavior<GGL::CConvertSettlerAbility>();
		if (a != nullptr) {
			GGL::CConvertSettlerAbilityProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CConvertSettlerAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->ConversionMaxRange, IgnoreFleeing) >= 10) {
					shok_EGL_CGLEEntity* tar = GetFurthestConversionTargetInArea(Player, e->Position, p->ConversionStartRange, IgnoreFleeing);
					if (tar != nullptr) {
						static_cast<shok_GGL_CSettler*>(e)->HeroAbilityConvert(tar->EntityId);
						UpdateTargetCache(tar->EntityId, 600);
						return true;
					}
				}
			}
		}
	}
	{
		GGL::CSniperAbility* a = e->GetBehavior<GGL::CSniperAbility>();
		if (a != nullptr) {
			GGL::CSniperAbilityProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				shok_EGL_CGLEEntity* tar = GetNearestSnipeTargetInArea(Player, e->Position, p->Range, IgnoreFleeing);
				if (tar != nullptr) {
					static_cast<shok_GGL_CSettler*>(e)->HeroAbilitySnipe(tar->EntityId);
					UpdateTargetCache(tar->EntityId, 600);
					return true;
				}
			}
		}
	}
	{
		GGL::CInflictFearAbility* a = e->GetBehavior<GGL::CInflictFearAbility>();
		if (a != nullptr) {
			GGL::CInflictFearAbilityProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CInflictFearAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range/2, IgnoreFleeing) >= 10) {
					static_cast<shok_GGL_CSettler*>(e)->HeroAbilityInflictFear();
					return true;
				}
			}
		}
	}
	{
		GGL::CKegPlacerBehavior* a = e->GetBehavior<GGL::CKegPlacerBehavior>();
		if (a != nullptr) {
			GGL::CKegPlacerBehaviorProperties* p = e->GetEntityType()->GetBehaviorProps<GGL::CKegPlacerBehaviorProperties>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				shok_EGL_CGLEEntity* tar = GetNearestBuildingInArea(Player, e->Position, Area);
				if (tar == nullptr && SabotageBridges)
					tar = GetNearestBridgeInArea(e->Position, 6000);
				if (tar != nullptr) {
					static_cast<shok_GGL_CSettler*>(e)->ThiefSabotage(tar->EntityId);
					UpdateTargetCache(tar->EntityId, 500);
					return true;
				}
			}
		}
	}
	if (shok_EGL_CGLEEntity::CamoActivateCb) { // if camo fix is active, use camo to get rid of attackers
		GGL::CCamouflageBehavior* a = e->GetBehavior<GGL::CCamouflageBehavior>();
		if (a != nullptr && !shok_DynamicCast<GGL::CCamouflageBehavior, GGL::CThiefCamouflageBehavior>(a)) {
			GGL::CCamouflageBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (e->Health <= e->GetMaxHealth() / 2 && CountTargetsInArea(Player, e->Position, Area, IgnoreFleeing) >= 5) {
					static_cast<shok_GGL_CSettler*>(e)->HeroAbilityActivateCamoflage();
					return true;
				}
			}
		}
	}

	return false;
}
bool UnlimitedArmy::ExecutePrepDefense(shok_EGL_CGLEEntity* e)
{
	{
		GGL::CCannonBuilderBehavior* a = e->GetBehavior<GGL::CCannonBuilderBehavior>();
		if (a != nullptr) {
			GGL::CCannonBuilderBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CCannonBuilderBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				int ety = e->EntityType;
				auto d = std::find_if(CannonBuilderAbilityData.begin(), CannonBuilderAbilityData.end(), [ety](UACannonBuilderAbilityData& d) {return d.HeroType == ety; });
				if (d != CannonBuilderAbilityData.end()) {
					shok::Position p = e->Position;
					p.X += distr(generator) * 100;
					p.Y += distr(generator) * 100;
					p.FloorToBuildingPlacement();
					if (shok_canPlaceBuilding(d->BottomType, Player, &p, 0, 0)) {
						static_cast<shok_GGL_CSettler*>(e)->HeroAbilityPlaceCannon(p, d->BottomType, d->TopType);
						return true;
					}
				}
			}
		}
	}
	{
		GGL::CKegPlacerBehavior* a = e->GetBehavior<GGL::CKegPlacerBehavior>();
		if (a != nullptr) {
			GGL::CKegPlacerBehaviorProperties* p = e->GetEntityType()->GetBehaviorProps<GGL::CKegPlacerBehaviorProperties>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				shok_EGL_CGLEEntity* tar = GetNearestBuildingInArea(Player, e->Position, Area);
				if (tar == nullptr && SabotageBridges)
					tar = GetNearestBridgeInArea(e->Position, Area);
				if (tar != nullptr) {
					static_cast<shok_GGL_CSettler*>(e)->ThiefSabotage(tar->EntityId);
					UpdateTargetCache(tar->EntityId, 500);
					return true;
				}
			}
		}
	}

	return false;
}

bool UnlimitedArmy::CheckTargetCache(int id, int count) {
	for (UATargetCache& t : TargetCache) {
		if (t.EntityId == id) {
			if (t.Num < count) {
				return true;
			}
			return false;
		}
	}
	return true;
}
void UnlimitedArmy::UpdateTargetCache(int id, int time) {
	int tick = (*shok_EGL_CGLEGameLogic::GlobalObj)->GetTick();
	for (UATargetCache& t : TargetCache) {
		if (t.EntityId == id) {
			t.Num++;
			t.Tick = tick + time;
			return;
		}
	}
	TargetCache.push_back({ id, tick + time, 1 });
}

int UnlimitedArmy::AddLeader(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->AddLeader(luaext_checkSettler(L, 2));
	return 0;
}

int UnlimitedArmy::GetPos(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->CalculatePos();
	luaext_pushPos(L, a->LastPos);
	return 1;
}
int UnlimitedArmy::Tick(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->Tick();
	return 0;
}

int UnlimitedArmy::Iterate(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	lua_pushcfunction(L, [](lua_State* L) {
		UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
		int i = luaL_checkint(L, 2);
		i++;
		if (i >= static_cast<int>(a->Leaders.size())) {
			lua_pushnil(L);
			lua_pushnil(L);
		}
		else {
			lua_pushnumber(L, i);
			lua_pushnumber(L, a->Leaders[i]);
		}
		return 2;
		} );
	lua_pushvalue(L, 1);
	lua_pushnumber(L, -1);
	return 3;
}
int UnlimitedArmy::IterateTransit(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	lua_pushcfunction(L, [](lua_State* L) {
		UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
		int i = luaL_checkint(L, 2);
		i++;
		if (i >= (int)a->LeaderInTransit.size()) {
			lua_pushnil(L);
			lua_pushnil(L);
		}
		else {
			lua_pushnumber(L, i);
			lua_pushnumber(L, a->LeaderInTransit[i]);
		}
		return 2;
		});
	lua_pushvalue(L, 1);
	lua_pushnumber(L, -1);
	return 3;
}

int UnlimitedArmy::OnIdChanged(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	int old = luaL_checkint(L, 2);
	int ne = luaL_checkint(L, 3);
	a->OnIdChanged(old, ne);
	return 0;
}

int UnlimitedArmy::GetSize(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	int i = a->GetSize(lua_toboolean(L, 2), lua_toboolean(L, 3));
	lua_pushnumber(L, i);
	return 1;
}

int UnlimitedArmy::RemoveLeader(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->RemoveLeader(luaext_checkEntity(L, 2));
	return 0;
}

int UnlimitedArmy::IsIdle(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	lua_pushboolean(L, a->IsIdle());
	return 1;
}
int UnlimitedArmy::GetStatus(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	lua_pushnumber(L, static_cast<int>(a->Status));
	return 1;
}

int UnlimitedArmy::SetArea(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->Area = luaL_checkfloat(L, 2);
	return 0;
}
int UnlimitedArmy::SetTarget(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	luaext_checkPos(L, a->Target, 2);
	return 0;
}

int UnlimitedArmy::DumpTable(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	lua_newtable(L);
	lua_pushstring(L, "Leaders");
	lua_newtable(L);
	int i = 1;
	for (int l : a->Leaders) {
		lua_pushnumber(L, l);
		lua_rawseti(L, -2, i);
		i++;
	}
	lua_rawset(L, -3);
	lua_pushstring(L, "LeadersTransit");
	lua_newtable(L);
	i = 1;
	for (int l : a->LeaderInTransit) {
		lua_pushnumber(L, l);
		lua_rawseti(L, -2, i);
		i++;
	}
	lua_rawset(L, -3);
	lua_pushstring(L, "DeadHeroes");
	lua_newtable(L);
	i = 1;
	for (int l : a->DeadHeroes) {
		lua_pushnumber(L, l);
		lua_rawseti(L, -2, i);
		i++;
	}
	lua_rawset(L, -3);
	lua_pushstring(L, "Player");
	lua_pushnumber(L, a->Player);
	lua_rawset(L, -3);
	lua_pushstring(L, "Status");
	lua_pushnumber(L, static_cast<int>(a->Status));
	lua_rawset(L, -3);
	lua_pushstring(L, "Area");
	lua_pushnumber(L, a->Area);
	lua_rawset(L, -3);
	lua_pushstring(L, "CurrentBattleTarget");
	lua_pushnumber(L, a->CurrentBattleTarget);
	lua_rawset(L, -3);
	lua_pushstring(L, "Target");
	luaext_pushPos(L, a->Target);
	lua_rawset(L, -3);
	lua_pushstring(L, "ReMove");
	lua_pushboolean(L, a->ReMove);
	lua_rawset(L, -3);
	lua_pushstring(L, "IgnoreFleeing");
	lua_pushboolean(L, a->IgnoreFleeing);
	lua_rawset(L, -3);
	lua_pushstring(L, "AutoRotateFormation");
	lua_pushnumber(L, a->AutoRotateFormation);
	lua_rawset(L, -3);
	return 1;
}
int UnlimitedArmy::ReadTable(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	lua_pushstring(L, "Leaders");
	lua_rawget(L, 2);
	int i = 1;
	a->Leaders.clear();
	while (true) {
		lua_rawgeti(L, -1, i);
		if (!lua_isnumber(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		int id = luaL_checkint(L, -1);
		a->Leaders.emplace_back(id);
		if (shok_EGL_CGLEEntity::GetEntityByID(id)->IsEntityInCategory(shok::EntityCategory::Cannon))
			a->Cannons.push_back({ id, -1 });
		lua_pop(L, 1);
		i++;
	}
	lua_pop(L, 1);
	lua_pushstring(L, "LeadersTransit");
	lua_rawget(L, 2);
	i = 1;
	a->LeaderInTransit.clear();
	while (true) {
		lua_rawgeti(L, -1, i);
		if (!lua_isnumber(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		a->LeaderInTransit.emplace_back(luaL_checkint(L, -1));
		lua_pop(L, 1);
		i++;
	}
	lua_pop(L, 1);
	lua_pushstring(L, "DeadHeroes");
	lua_rawget(L, 2);
	i = 1;
	a->DeadHeroes.clear();
	while (true) {
		lua_rawgeti(L, -1, i);
		if (!lua_isnumber(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		a->DeadHeroes.emplace_back(luaL_checkint(L, -1));
		lua_pop(L, 1);
		i++;
	}
	lua_pop(L, 1);
	lua_pushstring(L, "Player");
	lua_rawget(L, 2);
	a->Player = luaL_checkint(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "Status");
	lua_rawget(L, 2);
	a->Status = static_cast<UAStatus>(luaL_checkint(L, -1));
	lua_pop(L, 1);
	lua_pushstring(L, "Area");
	lua_rawget(L, 2);
	a->Area = luaL_checkfloat(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "CurrentBattleTarget");
	lua_rawget(L, 2);
	a->CurrentBattleTarget = luaL_checkint(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "Target");
	lua_rawget(L, 2);
	luaext_checkPos(L, a->Target, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "ReMove");
	lua_rawget(L, 2);
	a->ReMove = lua_toboolean(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "IgnoreFleeing");
	lua_rawget(L, 2);
	a->IgnoreFleeing = lua_toboolean(L, -1);
	lua_pop(L, 1);
	lua_pushstring(L, "AutoRotateFormation");
	lua_rawget(L, 2);
	a->AutoRotateFormation = luaL_checkfloat(L, -1);
	lua_pop(L, 1);
	return 0;
}

int UnlimitedArmy::SetStatus(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->Status = static_cast<UAStatus>(luaL_checkint(L, 2));
	return 0;
}
int UnlimitedArmy::SetReMove(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->ReMove = lua_toboolean(L, 2);
	return 0;
}
int UnlimitedArmy::SetCurrentBattleTarget(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->CurrentBattleTarget = luaext_checkEntityId(L, 2);
	return 0;
}

int UnlimitedArmy::GetRangedMelee(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	lua_newtable(L);
	lua_newtable(L);
	lua_newtable(L);
	int ran = 1, mel = 1, nc = 1;
	for (int id : a->Leaders) {
		lua_pushnumber(L, id);
		shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(id);
		if (a->IsRanged(e)) {
			lua_rawseti(L, -4, ran);
			ran++;
		}
		else if (a->IsNonCombat(e)) {
			lua_rawseti(L, -2, nc);
			nc++;
		}
		else {
			lua_rawseti(L, -3, mel);
			mel++;
		}
	}
	return 3;
}

int UnlimitedArmy::SetIgnoreFleeing(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->IgnoreFleeing = lua_toboolean(L, 2);
	return 0;
}
int UnlimitedArmy::SetAutoRotateFormation(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->AutoRotateFormation = luaL_checkfloat(L, 2);
	return 0;
}

int UnlimitedArmy::GetFirstDeadHero(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	if (a->DeadHeroes.size() > 0)
		lua_pushnumber(L, a->DeadHeroes[0]);
	else
		lua_pushnil(L);
	return 1;
}

int UnlimitedArmy::SetPrepDefense(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->PrepDefense = lua_toboolean(L, 2);
	return 0;
}
int UnlimitedArmy::SetSabotageBridges(lua_State* L)
{
	UnlimitedArmy* a = luaext_GetUserData<UnlimitedArmy>(L, 1);
	a->SabotageBridges = lua_toboolean(L, 2);
	return 0;
}

int l_uaNew(lua_State* L) {
	int pl = luaL_checkint(L, 1);
	UnlimitedArmy* a = luaext_newUserData<UnlimitedArmy>(L, pl);
	a->L = L;
	lua_pushvalue(L, 2);
	a->Formation = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, 3);
	a->CommandQueue = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, 4);
	a->Spawner = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, 5);
	a->Normalize = luaL_ref(L, LUA_REGISTRYINDEX);
	return 1;
}

int l_uaNearest(lua_State* L) {
	int pl = luaL_checkint(L, 1);
	shok::Position p;
	luaext_checkPos(L, p, 2);
	float r = luaL_checkfloat(L, 3);
	shok_EGL_CGLEEntity* e = UnlimitedArmy::GetNearestTargetInArea(pl, p, r, lua_toboolean(L, 4));
	if (e == nullptr)
		lua_pushnumber(L, 0);
	else
		lua_pushnumber(L, e->EntityId);
	return 1;
}

int l_uaCount(lua_State* L) {
	int pl = luaL_checkint(L, 1);
	shok::Position p;
	luaext_checkPos(L, p, 2);
	float r = luaL_checkfloat(L, 3);
	lua_pushnumber(L, UnlimitedArmy::CountTargetsInArea(pl, p, r, lua_toboolean(L, 4)));
	return 1;
}

int l_uaAddCannonBuilderData(lua_State* L) {
	UACannonBuilderAbilityData d = { luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3) };
	UnlimitedArmy::CannonBuilderAbilityData.emplace_back(d);
	return 0;
}

void l_ua_init(lua_State* L)
{
	luaext_registerFunc(L, "New", &l_uaNew);
	luaext_registerFunc(L, "GetNearestEnemyInArea", &l_uaNearest);
	luaext_registerFunc(L, "CountTargetEntitiesInArea", &l_uaCount);
	luaext_registerFunc(L, "AddCannonBuilderData", &l_uaAddCannonBuilderData);

	luaext_prepareUserDataType<UnlimitedArmy>(L);
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.UA.GetNearestEnemyInArea(1, x,y, 1000)

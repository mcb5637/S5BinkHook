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
			if (e != nullptr && e->IsEntityInCategory(shok_EntityCategory::Hero)) {
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
	if (Leaders.size() == 0)
		return;
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
	if (!preventComands && !LastPos.IsInRange(Target, 1000)) {
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
		if (e->IsEntityInCategory(shok_EntityCategory::Cannon))
			Cannons.push_back({ e->EntityId, -1 });
	}
	else
		LeaderInTransit.push_back(e->EntityId);
}
void UnlimitedArmy::OnIdChanged(int old, int ne)
{
	for (int i = 0; i < (int)Leaders.size(); i++) {
		if (Leaders[i] == old)
			Leaders[i] = ne;
	}
	for (int i = 0; i < (int)LeaderInTransit.size(); i++) {
		if (LeaderInTransit[i] == old)
			LeaderInTransit[i] = ne;
	}
	for (int i = 0; i < (int)DeadHeroes.size(); i++) {
		if (DeadHeroes[i] == old)
			DeadHeroes[i] = ne;
	}
	for (int i = 0; i < (int)Cannons.size(); i++) {
		if (Cannons[i].EntityId == old)
			Cannons[i].EntityId = ne;
	}
}
void UnlimitedArmy::CheckTransit()
{
	if (Leaders.size() == 0 && LeaderInTransit.size() > 0) {
		Leaders.push_back(LeaderInTransit[1]);
		if (shok_EGL_CGLEEntity::GetEntityByID(LeaderInTransit[1])->IsEntityInCategory(shok_EntityCategory::Cannon))
			Cannons.push_back({ LeaderInTransit[1], -1 });
		LeaderInTransit.erase(LeaderInTransit.begin());
		PosLastUpdatedTick = -1;
		CalculatePos();
		NeedFormat();
	}
	auto e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), [this](int id) {
		shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(id);
		if (this->LastPos.IsInRange(e->Position, this->Area)) {
			this->Leaders.push_back(id);
			if (e->IsEntityInCategory(shok_EntityCategory::Cannon))
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
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestTargetInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory catLeader = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::TargetFilter_TargetTypeLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::TargetFilter_TargetType);
	EntityIteratorPredicatePriority prio1 = EntityIteratorPredicatePriority(2, &catLeader);
	EntityIteratorPredicatePriority prio2 = EntityIteratorPredicatePriority(1, &buil);
	EntityIteratorPredicate* preds[10] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &cir, &nflee, &prio1, &prio2
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 10 : 9);
	EntityIterator iter = EntityIterator(&a);
	shok_EGL_CGLEEntity* r = iter.GetNearest(nullptr);
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetFurthestConversionTargetInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::TargetFilter_TargetTypeLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicateFunc fun = EntityIteratorPredicateFunc([this](shok_EGL_CGLEEntity* e) {
		return this->CheckTargetCache(e->EntityId, 1);
		});
	EntityIteratorPredicate* preds[9] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &cir, &fun, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 9 : 8);
	EntityIterator iter = EntityIterator(&a);
	shok_EGL_CGLEEntity* r = iter.GetFurthest(nullptr);
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestSettlerInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::TargetFilter_TargetTypeLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicate* preds[8] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &cir, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 8 : 7);
	EntityIterator iter = EntityIterator(&a);
	return iter.GetNearest(nullptr);
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestBuildingInArea(int player, shok_position& p, float ran) {
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
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestBridgeInArea(shok_position& p, float ran) {
	EntityIteratorPredicateOfPlayer pl = EntityIteratorPredicateOfPlayer(0);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::Bridge);
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
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestSnipeTargetInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);

	EntityIteratorPredicateOfEntityCategory he = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::Hero);
	EntityIteratorPredicateOfEntityCategory can = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::Cannon);
	EntityIteratorPredicate* cats[2] = {
		&he, &can
	};
	EntityIteratorPredicateOr cat = EntityIteratorPredicateOr(cats, 2);

	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateFunc fun = EntityIteratorPredicateFunc([this](shok_EGL_CGLEEntity* e) {
		return this->CheckTargetCache(e->EntityId, 1);
		});
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicate* preds[8] = {
		&relev, &pl, &cat, &al, &vis, &cir, &fun, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 8 : 7);
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
	shok_GGL_CCamouflageBehavior* c = e->GetBehavior<shok_GGL_CCamouflageBehavior>();
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
	shok_LeaderCommand comm = e->EventLeaderGetCurrentCommand();
	return (comm == shok_LeaderCommand::Move || comm == shok_LeaderCommand::AttackMove || comm == shok_LeaderCommand::Patrol) && !LeaderIsIdle(e);
}
bool UnlimitedArmy::LeaderIsIdle(shok_EGL_CGLEEntity* e)
{
	return ((shok_GGL_CSettler*)e)->IsIdle();
}
bool UnlimitedArmy::LeaderIsInBattle(shok_EGL_CGLEEntity* e)
{
	shok_LeaderCommand comm = e->EventLeaderGetCurrentCommand();
	return (comm == shok_LeaderCommand::Attack || comm == shok_LeaderCommand::AttackMove || comm == shok_LeaderCommand::HeroAbility) && !LeaderIsIdle(e);
}
bool UnlimitedArmy::IsRanged(shok_EGL_CGLEEntity* e)
{
	return e->IsEntityInCategory(shok_EntityCategory::LongRange) || e->IsEntityInCategory(shok_EntityCategory::Cannon) || e->IsEntityInCategory(shok_EntityCategory::TargetFilter_CustomRanged);
}
bool UnlimitedArmy::IsNonCombat(shok_EGL_CGLEEntity* e)
{
	return e->IsEntityInCategory(shok_EntityCategory::TargetFilter_NonCombat);
}
void UnlimitedArmy::BattleCommand()
{
	shok_position p = shok_EGL_CGLEEntity::GetEntityByID(CurrentBattleTarget)->Position;
	if (ReMove) {
		PrepDefenseReset.clear();
		NormalizeSpeed(false, false);
	}
	for (int id : Leaders) {
		shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*) shok_EGL_CGLEEntity::GetEntityByID(id);
		if (e->IsEntityInCategory(shok_EntityCategory::Hero) || IsNonCombat(e)) {
			if (ExecuteHeroAbility(e))
				continue;
		}
		if ((ReMove || !LeaderIsInBattle(e) ) && !IsNonCombat(e) && !e->IsEntityInCategory(shok_EntityCategory::Cannon)) {
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
		shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_EGL_CGLEEntity::GetEntityByID(cd.EntityId);
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
	if (Status == UAStatus::MovingNoBattle) {
		for (int id : Leaders) {
			shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_EGL_CGLEEntity::GetEntityByID(id);
			if (ReMove || !LeaderIsMoving(e)) {
				e->Move(Target);
			}
		}
	}
	else {
		for (int id : Leaders) {
			shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_EGL_CGLEEntity::GetEntityByID(id);
			if (ReMove || LeaderIsIdle(e)) {
				e->AttackMove(Target);
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
			shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_EGL_CGLEEntity::GetEntityByID(id);
			if ((e->IsEntityInCategory(shok_EntityCategory::Hero) || IsNonCombat(e)) && e->EventLeaderGetCurrentCommand() != shok_LeaderCommand::HeroAbility) {
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
			shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_EGL_CGLEEntity::GetEntityByID(r.EntityId);
			if (e->EventLeaderGetCurrentCommand() != shok_LeaderCommand::HeroAbility) {
				shok_EGL_CMovingEntity* m = (shok_EGL_CMovingEntity*)e;
				m->Move(r.Pos);
				m->TargetRotation = r.Pos.r;
				m->TargetRotationValid = 1;
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
int UnlimitedArmy::CountTargetsInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(shok_EntityCategory::TargetFilter_TargetType);
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
		shok_GGL_CRangedEffectAbility* a = e->GetBehavior<shok_GGL_CRangedEffectAbility>();
		if (a != nullptr) {
			shok_GGL_CRangedEffectAbilityProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CRangedEffectAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				float r = 1000;
				if (p->AffectsLongRangeOnly)
					r = 3000;
				if (p->IsAggressive()) {
					if (!((shok_GGL_CSettler*)e)->IsMoving() && CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 10) {
						((shok_GGL_CSettler*)e)->HeroAbilityRangedEffect();
					}
				}
				else if (p->IsDefensive()) {
					if (CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 10) {
						((shok_GGL_CSettler*)e)->HeroAbilityRangedEffect();
					}
				}
				else if (p->IsHeal()) {
					if (e->Health <= e->GetMaxHealth()/2 && CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 2) {
						((shok_GGL_CSettler*)e)->HeroAbilityRangedEffect();
					}
				}
			}
		}
	}
	{
		shok_GGL_CSummonBehavior* a = e->GetBehavior<shok_GGL_CSummonBehavior>();
		if (a != nullptr) {
			shok_GGL_CSummonBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CSummonBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, Area, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilitySummon();
					e->ObservedEntities.ForAll([this](shok_attachment* a) {
						if (a->AttachmentType == shok_AttachmentType::SUMMONER_SUMMONED)
							AddLeader(shok_EGL_CGLEEntity::GetEntityByID(a->EntityId));
						});
				}
			}
		}
	}

	// then noninstant
	{
		shok_GGL_CCircularAttack* a = e->GetBehavior<shok_GGL_CCircularAttack>();
		if (a != nullptr) {
			shok_GGL_CCircularAttackProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CCircularAttackProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilityCircularAttack();
					return true;
				}
			}
		}
	}
	{
		shok_GGL_CBombPlacerBehavior* a = e->GetBehavior<shok_GGL_CBombPlacerBehavior>();
		if (a != nullptr) {
			shok_GGL_CHeroAbilityProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CHeroAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, 500, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilityPlaceBomb(e->Position);
					return true;
				}
			}
		}
	}
	{
		shok_GGL_CCannonBuilderBehavior* a = e->GetBehavior<shok_GGL_CCannonBuilderBehavior>();
		if (a != nullptr) {
			shok_GGL_CCannonBuilderBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CCannonBuilderBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				int ety = e->EntityType;
				auto d = std::find_if(CannonBuilderAbilityData.begin(), CannonBuilderAbilityData.end(), [ety](UACannonBuilderAbilityData& d) {return d.HeroType == ety; });
				if (d != CannonBuilderAbilityData.end() && CountTargetsInArea(Player, e->Position, 2000, IgnoreFleeing) >= 10) {
					shok_position p = e->Position;
					p.X += distr(generator)*100;
					p.Y += distr(generator)*100;
					p.FloorToBuildingPlacement();
					if (shok_canPlaceBuilding(d->BottomType, Player, &p, 0, 0)) {
						((shok_GGL_CSettler*)e)->HeroAbilityPlaceCannon(p, d->BottomType, d->TopType);
						return true;
					}
				}
			}
		}
	}
	{
		shok_GGL_CShurikenAbility* a = e->GetBehavior<shok_GGL_CShurikenAbility>();
		if (a != nullptr) {
			shok_GGL_CShurikenAbilityProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CShurikenAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range, IgnoreFleeing) >= 10) {
					shok_EGL_CGLEEntity* tar = GetNearestSettlerInArea(Player, e->Position, p->Range, IgnoreFleeing);
					if (tar != nullptr) {
						((shok_GGL_CSettler*)e)->HeroAbilityShuriken(tar->EntityId);
						return true;
					}
				}
			}
		}
	}
	{
		shok_GGL_CConvertSettlerAbility* a = e->GetBehavior<shok_GGL_CConvertSettlerAbility>();
		if (a != nullptr) {
			shok_GGL_CConvertSettlerAbilityProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CConvertSettlerAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->ConversionMaxRange, IgnoreFleeing) >= 10) {
					shok_EGL_CGLEEntity* tar = GetFurthestConversionTargetInArea(Player, e->Position, p->ConversionStartRange, IgnoreFleeing);
					if (tar != nullptr) {
						((shok_GGL_CSettler*)e)->HeroAbilityConvert(tar->EntityId);
						UpdateTargetCache(tar->EntityId, 600);
						return true;
					}
				}
			}
		}
	}
	{
		shok_GGL_CSniperAbility* a = e->GetBehavior<shok_GGL_CSniperAbility>();
		if (a != nullptr) {
			shok_GGL_CSniperAbilityProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CSniperAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				shok_EGL_CGLEEntity* tar = GetNearestSnipeTargetInArea(Player, e->Position, p->Range, IgnoreFleeing);
				if (tar != nullptr) {
					((shok_GGL_CSettler*)e)->HeroAbilitySnipe(tar->EntityId);
					UpdateTargetCache(tar->EntityId, 600);
					return true;
				}
			}
		}
	}
	{
		shok_GGL_CInflictFearAbility* a = e->GetBehavior<shok_GGL_CInflictFearAbility>();
		if (a != nullptr) {
			shok_GGL_CInflictFearAbilityProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CInflictFearAbilityProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range/2, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilityInflictFear();
					return true;
				}
			}
		}
	}
	{
		shok_GGL_CKegPlacerBehavior* a = e->GetBehavior<shok_GGL_CKegPlacerBehavior>();
		if (a != nullptr) {
			shok_GGL_CKegPlacerBehaviorProperties* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CKegPlacerBehaviorProperties>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				shok_EGL_CGLEEntity* tar = GetNearestBuildingInArea(Player, e->Position, Area);
				if (tar == nullptr && SabotageBridges)
					tar = GetNearestBridgeInArea(e->Position, 6000);
				if (tar != nullptr) {
					((shok_GGL_CSettler*)e)->ThiefSabotage(tar->EntityId);
					UpdateTargetCache(tar->EntityId, 500);
					return true;
				}
			}
		}
	}
	if (shok_EGL_CGLEEntity::CamoActivateCb) { // if camo fix is active, use camo to get rid of attackers
		shok_GGL_CCamouflageBehavior* a = e->GetBehavior<shok_GGL_CCamouflageBehavior>();
		if (a != nullptr && !shok_DynamicCast<shok_GGL_CCamouflageBehavior, shok_GGL_CThiefCamouflageBehavior>(a)) {
			shok_GGL_CCamouflageBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CCamouflageBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				if (e->Health <= e->GetMaxHealth() / 2 && CountTargetsInArea(Player, e->Position, Area, IgnoreFleeing) >= 5) {
					((shok_GGL_CSettler*)e)->HeroAbilityActivateCamoflage();
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
		shok_GGL_CCannonBuilderBehavior* a = e->GetBehavior<shok_GGL_CCannonBuilderBehavior>();
		if (a != nullptr) {
			shok_GGL_CCannonBuilderBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CCannonBuilderBehaviorProps>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				int ety = e->EntityType;
				auto d = std::find_if(CannonBuilderAbilityData.begin(), CannonBuilderAbilityData.end(), [ety](UACannonBuilderAbilityData& d) {return d.HeroType == ety; });
				if (d != CannonBuilderAbilityData.end()) {
					shok_position p = e->Position;
					p.X += distr(generator) * 100;
					p.Y += distr(generator) * 100;
					p.FloorToBuildingPlacement();
					if (shok_canPlaceBuilding(d->BottomType, Player, &p, 0, 0)) {
						((shok_GGL_CSettler*)e)->HeroAbilityPlaceCannon(p, d->BottomType, d->TopType);
						return true;
					}
				}
			}
		}
	}
	{
		shok_GGL_CKegPlacerBehavior* a = e->GetBehavior<shok_GGL_CKegPlacerBehavior>();
		if (a != nullptr) {
			shok_GGL_CKegPlacerBehaviorProperties* p = e->GetEntityType()->GetBehaviorProps<shok_GGL_CKegPlacerBehaviorProperties>();
			if (a->SecondsCharged >= p->RechargeTimeSeconds) {
				shok_EGL_CGLEEntity* tar = GetNearestBuildingInArea(Player, e->Position, Area);
				if (tar == nullptr && SabotageBridges)
					tar = GetNearestBridgeInArea(e->Position, Area);
				if (tar != nullptr) {
					((shok_GGL_CSettler*)e)->ThiefSabotage(tar->EntityId);
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

const char* udname = "UnlimitedArmy";

int l_uam_gc(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->~UnlimitedArmy();
	return 0;
}

int l_uam_AddEntity(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->AddLeader(luaext_checkSettler(L, 2));
	return 0;
}

int l_ual_IterateNext(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	int i = luaL_checkint(L, 2);
	i++;
	if (i >= (int)a->Leaders.size()) {
		lua_pushnil(L);
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, i);
		lua_pushnumber(L, a->Leaders[i]);
	}
	return 2;
}

int l_uam_Iterate(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	lua_pushcfunction(L, &l_ual_IterateNext);
	lua_pushvalue(L, 1);
	lua_pushnumber(L, -1);
	return 3;
}

int l_uam_GetPos(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->CalculatePos();
	luaext_pushPos(L, a->LastPos);
	return 1;
}

int l_uam_Tick(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->Tick();
	return 0;
}

int l_uam_IdChanged(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	int old = luaL_checkint(L, 2);
	int ne = luaL_checkint(L, 3);
	a->OnIdChanged(old, ne);
	return 0;
}

int l_uam_GetSize(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	int i = a->GetSize(lua_toboolean(L, 2), lua_toboolean(L, 3));
	lua_pushnumber(L, i);
	return 1;
}

int l_uam_RemoveLeader(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->RemoveLeader(luaext_checkEntity(L, 2));
	return 0;
}

int l_uam_IsIdle(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	lua_pushboolean(L, a->IsIdle());
	return 1;
}

int l_uam_GetStatus(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	lua_pushnumber(L, (int) a->Status);
	return 1;
}

int l_uam_SetArea(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->Area = luaL_checkfloat(L, 2);
	return 0;
}

int l_uam_SetTarget(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	luaext_checkPos(L, a->Target, 2);
	return 0;
}

int l_uam_DumpTable(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
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
	lua_pushnumber(L, (int) a->Status);
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

int l_uam_ReadTable(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
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
		if (shok_EGL_CGLEEntity::GetEntityByID(id)->IsEntityInCategory(shok_EntityCategory::Cannon))
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
	a->Status = (UAStatus) luaL_checkint(L, -1);
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

int l_ual_IterateTransitNext(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
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
}
int l_uam_IterateTransit(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	lua_pushcfunction(L, &l_ual_IterateTransitNext);
	lua_pushvalue(L, 1);
	lua_pushnumber(L, -1);
	return 3;
}


int l_uam_SetStatus(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->Status = (UAStatus) luaL_checkint(L, 2);
	return 0;
}

int l_uam_SetReMove(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->ReMove = lua_toboolean(L, 2);
	return 0;
}

int l_uam_SetCurrentBattleTarget(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->CurrentBattleTarget = luaext_checkEntityId(L, 2);
	return 0;
}

int l_uam_GetRangedMelee(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
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

int l_uam_SetIgnoreFleeing(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->IgnoreFleeing = lua_toboolean(L, 2);
	return 0;
}

int l_uam_SetAutoRotateFormation(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->AutoRotateFormation = luaL_checkfloat(L, 2);
	return 0;
}

int l_uam_GetFirstDeadHero(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	if (a->DeadHeroes.size() > 0)
		lua_pushnumber(L, a->DeadHeroes[0]);
	else
		lua_pushnil(L);
	return 1;
}

int l_uam_SetPrepDefense(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->PrepDefense = lua_toboolean(L, 2);
	return 0;
}

int l_uam_SetSabotageBridges(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->SabotageBridges = lua_toboolean(L, 2);
	return 0;
}

int l_uaNew(lua_State* L) {
	int pl = luaL_checkint(L, 1);
	UnlimitedArmy* a = (UnlimitedArmy*)lua_newuserdata(L, sizeof(UnlimitedArmy));
	new (a) UnlimitedArmy(pl);
	luaL_getmetatable(L, udname);
	lua_setmetatable(L, -2);
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
	shok_position p;
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
	shok_position p;
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

	luaL_newmetatable(L, udname);
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, &l_uam_gc);
	lua_settable(L, -3);

	lua_pushstring(L, "__index");
	lua_newtable(L);
	luaext_registerFunc(L, "AddLeader", &l_uam_AddEntity);
	luaext_registerFunc(L, "GetPos", &l_uam_GetPos);
	luaext_registerFunc(L, "Tick", &l_uam_Tick);
	luaext_registerFunc(L, "Iterate", &l_uam_Iterate);
	luaext_registerFunc(L, "IterateTransit", &l_uam_IterateTransit);
	luaext_registerFunc(L, "OnIdChanged", &l_uam_IdChanged);
	luaext_registerFunc(L, "GetSize", &l_uam_GetSize);
	luaext_registerFunc(L, "RemoveLeader", &l_uam_RemoveLeader);
	luaext_registerFunc(L, "IsIdle", &l_uam_IsIdle);
	luaext_registerFunc(L, "GetStatus", &l_uam_GetStatus);
	luaext_registerFunc(L, "SetArea", &l_uam_SetArea);
	luaext_registerFunc(L, "SetTarget", &l_uam_SetTarget);
	luaext_registerFunc(L, "DumpTable", &l_uam_DumpTable);
	luaext_registerFunc(L, "ReadTable", &l_uam_ReadTable);
	luaext_registerFunc(L, "SetStatus", &l_uam_SetStatus);
	luaext_registerFunc(L, "SetReMove", &l_uam_SetReMove);
	luaext_registerFunc(L, "SetCurrentBattleTarget", &l_uam_SetCurrentBattleTarget);
	luaext_registerFunc(L, "GetRangedMelee", &l_uam_GetRangedMelee);
	luaext_registerFunc(L, "SetIgnoreFleeing", &l_uam_SetIgnoreFleeing);
	luaext_registerFunc(L, "SetAutoRotateFormation", &l_uam_SetAutoRotateFormation);
	luaext_registerFunc(L, "GetFirstDeadHero", &l_uam_GetFirstDeadHero);
	luaext_registerFunc(L, "SetPrepDefense", &l_uam_SetPrepDefense);
	luaext_registerFunc(L, "SetSabotageBridges", &l_uam_SetSabotageBridges);
	lua_settable(L, -3);
	lua_pop(L, 1);
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.UA.GetNearestEnemyInArea(1, x,y, 1000)

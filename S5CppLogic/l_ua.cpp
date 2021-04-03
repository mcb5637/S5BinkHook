#include "pch.h"
#include "l_ua.h"
#include "luaext.h"
#include "l_entity.h"
#include <random>


std::vector<int> UnlimitedArmy::IdleTaskLists = std::vector<int>();
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
	int tick = (*shok_EGL_CGLEGameLogicObject)->GetTick();
	if (tick == PosLastUpdatedTick)
		return;
	float x = 0, y = 0;
	int num = 0;
	for (int id : Leaders) {
		shok_EGL_CGLEEntity* e = shok_eid2obj(id);
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
		if (shok_entityIsDead(id)) {
			shok_EGL_CGLEEntity* e = shok_eid2obj(id);
			if (e != nullptr && e->IsEntityInCategory(EntityCategoryHero)) {
				this->DeadHeroes.push_back(id);
			}
			return true;
		}
		return false;
	};
	auto e = std::remove_if(Leaders.begin(), Leaders.end(), remo);
	Leaders.erase(e, Leaders.end());
	e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), remo);
	LeaderInTransit.erase(e, LeaderInTransit.end());
	auto e2 = std::remove_if(Cannons.begin(), Cannons.end(), [](UACannonData& d) { return shok_entityIsDead(d.EntityId); });
	Cannons.erase(e2, Cannons.end());
	e = std::remove_if(DeadHeroes.begin(), DeadHeroes.end(), [this](int id) {
		if (!shok_entityIsDead(id)) {
			this->AddLeader(shok_eid2obj(id));
			return true;
		}
		return shok_eid2obj(id) == nullptr;
		});
	DeadHeroes.erase(e, DeadHeroes.end());
	int tick = (*shok_EGL_CGLEGameLogicObject)->GetTick();
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
	if (!preventComands && Status != UAStatus_MovingNoBattle && CurrentBattleTarget != 0) {
		CheckStatus(UAStatus_Battle);
		preventComands = true;
	}
	if (!preventComands && !LastPos.IsInRange(Target, 1000)) {
		if (Status != UAStatus_Moving && Status != UAStatus_MovingNoBattle)
			CheckStatus(UAStatus_Moving);
		preventComands = true;
	}
	if (!preventComands) {
		CheckStatus(UAStatus_Idle);
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
	if (Status == UAStatus_Battle)
		BattleCommand();
	else if (Status == UAStatus_Moving || Status == UAStatus_MovingNoBattle)
		MoveCommand();
	else if (Status == UAStatus_Idle)
		FormationCommand();
}
void UnlimitedArmy::NeedFormat()
{
	if (Status == UAStatus_Idle)
		Status = UAStatus_IdleUnformated;
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
		if (e->IsEntityInCategory(EntityCategoryCannon))
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
		if (shok_eid2obj(LeaderInTransit[1])->IsEntityInCategory(EntityCategoryCannon))
			Cannons.push_back({ LeaderInTransit[1], -1 });
		LeaderInTransit.erase(LeaderInTransit.begin());
		PosLastUpdatedTick = -1;
		CalculatePos();
		NeedFormat();
	}
	auto e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), [this](int id) {
		shok_EGL_CGLEEntity* e = shok_eid2obj(id);
		if (this->LastPos.IsInRange(e->Position, this->Area)) {
			this->Leaders.push_back(id);
			if (e->IsEntityInCategory(EntityCategoryCannon))
				this->Cannons.push_back({ e->EntityId, -1 });
			this->NeedFormat();
			return true;
		}
		return false;
	});
	LeaderInTransit.erase(e, LeaderInTransit.end());
	for (int id : LeaderInTransit) {
		shok_EGL_CMovingEntity* e = ((shok_EGL_CMovingEntity*)shok_eid2obj(id));
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
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(UATargetCatLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicateIsNotFleeingFrom nflee = EntityIteratorPredicateIsNotFleeingFrom(p, 500);
	EntityIteratorPredicate* preds[8] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &cir, &nflee
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, notFleeing ? 8 : 7);
	EntityIterator iter = EntityIterator(&a);
	shok_EGL_CGLEEntity* r = iter.GetNearest(nullptr);
	if (r == nullptr) {
		preds[2] = &buil;
		r = iter.GetNearest(nullptr);
	}
	if (r == nullptr) {
		cat = EntityIteratorPredicateOfEntityCategory(UATargetCat);
		preds[2] = &cat;
		r = iter.GetNearest(nullptr);
	}
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetFurthestConversionTargetInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(UATargetCatLeader);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
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
	if (r == nullptr) {
		preds[2] = &buil;
		r = iter.GetFurthest(nullptr);
	}
	if (r == nullptr) {
		cat = EntityIteratorPredicateOfEntityCategory(UATargetCat);
		preds[2] = &cat;
		r = iter.GetFurthest(nullptr);
	}
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestSettlerInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(UATargetCatLeader);
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
shok_EGL_CGLEEntity* UnlimitedArmy::GetNearestSnipeTargetInArea(int player, shok_position& p, float ran, bool notFleeing)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);

	EntityIteratorPredicateOfEntityCategory he = EntityIteratorPredicateOfEntityCategory(EntityCategoryHero);
	EntityIteratorPredicateOfEntityCategory can = EntityIteratorPredicateOfEntityCategory(EntityCategoryCannon);
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
	if (shok_entityIsDead(id))
		return false;
	shok_EGL_CGLEEntity* e = shok_eid2obj(id);
	if (!LastPos.IsInRange(e->Position, Area))
		return false;
	shok_GGL_CCamouflageBehavior* c = e->GetCamoAbilityBehavior();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	if (!IgnoreFleeing)
		return true;
	return EntityIteratorPredicateIsNotFleeingFrom::IsNotFleeingFrom(e, LastPos, 500);
}
void UnlimitedArmy::CheckStatus(int status)
{
	if (Status != status) {
		Status = status;
		ReMove = true;
	}
}
bool UnlimitedArmy::LeaderIsMoving(shok_EGL_CGLEEntity* e)
{
	int comm = e->EventLeaderGetCurrentCommand();
	return (comm == shok_LeaderCommandMove || comm == shok_LeaderCommandAttackMove || comm == shok_LeaderCommandPatrol) && !LeaderIsIdleTL(e);
}
bool UnlimitedArmy::LeaderIsIdle(shok_EGL_CGLEEntity* e)
{
	int comm = e->EventLeaderGetCurrentCommand();
	return comm == shok_LeaderCommandDefend || comm == shok_LeaderCommandHoldPos || LeaderIsIdleTL(e);
}
bool UnlimitedArmy::LeaderIsInBattle(shok_EGL_CGLEEntity* e)
{
	int comm = e->EventLeaderGetCurrentCommand();
	return (comm == shok_LeaderCommandAttack || comm == shok_LeaderCommandAttackMove || comm == shok_LeaderCommandHeroAbility) && !LeaderIsIdleTL(e);
}
bool UnlimitedArmy::LeaderIsIdleTL(shok_EGL_CGLEEntity* e)
{
	int tl = e->TaskListId;
	for (int i : UnlimitedArmy::IdleTaskLists)
		if (i == tl)
			return true;
	return false;
}
bool UnlimitedArmy::IsRanged(shok_EGL_CGLEEntity* e)
{
	return e->IsEntityInCategory(EntityCategoryLongRange) || e->IsEntityInCategory(EntityCategoryCannon) || e->IsEntityInCategory(EntityCategoryCustomRanged);
}
bool UnlimitedArmy::IsNonCombat(shok_EGL_CGLEEntity* e)
{
	return e->IsEntityInCategory(EntityCategoryNonCombat);
}
void UnlimitedArmy::BattleCommand()
{
	shok_position p = shok_eid2obj(CurrentBattleTarget)->Position;
	if (ReMove)
		NormalizeSpeed(false, false);
	for (int id : Leaders) { // hero abilities
		shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*) shok_eid2obj(id);
		if (e->IsEntityInCategory(EntityCategoryHero) || IsNonCombat(e)) {
			if (ExecuteHeroAbility(e, 0))
				continue;
		}
		if ((ReMove || !LeaderIsInBattle(e) ) && !IsNonCombat(e) && !e->IsEntityInCategory(EntityCategoryCannon)) {
			if (IsRanged(e)) {
				e->AttackEntity(CurrentBattleTarget);
			}
			else {
				e->AttackMove(p);
			}
		}
	}
	int tick = (*shok_EGL_CGLEGameLogicObject)->GetTick();
	for (UACannonData cd : Cannons) {
		shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_eid2obj(cd.EntityId);
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
	if (ReMove)
		NormalizeSpeed(true, false);
	if (Status == UAStatus_MovingNoBattle) {
		for (int id : Leaders) {
			shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_eid2obj(id);
			if (ReMove || !LeaderIsMoving(e)) {
				e->Move(Target);
			}
		}
	}
	else {
		for (int id : Leaders) {
			shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*)shok_eid2obj(id);
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
		CallFormation();
	}
	// prep defense
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
	if (Status != UAStatus_Idle)
		return false;
	if (LeaderInTransit.size() > 0)
		return false;
	CalculatePos();
	if (!LastPos.IsInRange(Target, 1000))
		return false;
	for (int id : Leaders)
		if (!LeaderIsIdle(shok_eid2obj(id)))
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
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(UATargetCat);
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
		e = iter.GetNext(nullptr);
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
bool UnlimitedArmy::ExecuteHeroAbility(shok_EGL_CGLEEntity* e, bool nume)
{
	// first instant abilities
	{
		shok_GGL_CRangedEffectAbility* a = e->GetRangedEffectBehavior();
		if (a != nullptr) {
			shok_GGL_CRangedEffectAbilityProps* p = e->GetEntityType()->GetRangedEffectBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
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
					if (e->CurrentHealth <= e->GetEntityType()->LogicProps->MaxHealth/2 && CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 2) {
						((shok_GGL_CSettler*)e)->HeroAbilityRangedEffect();
					}
				}
			}
		}
	}
	{
		shok_GGL_CSummonBehavior* a = e->GetSummonBehavior();
		if (a != nullptr) {
			shok_GGL_CSummonBehaviorProps* p = e->GetEntityType()->GetSummonBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, Area, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilitySummon();
					e->AttachedToEntities.ForAll([this](Attachment* a) {
						if (a->AttachmentType == AttachmentType_SUMMONER_SUMMONED)
							AddLeader(shok_eid2obj(a->EntityId));
						});
				}
			}
		}
	}

	// then noninstant
	{
		shok_GGL_CCircularAttack* a = e->GetCircularAttackBehavior();
		if (a != nullptr) {
			shok_GGL_CCircularAttackProps* p = e->GetEntityType()->GetCircularAttackBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilityCircularAttack();
					return true;
				}
			}
		}
	}
	{
		shok_GGL_CBombPlacerBehavior* a = e->GetBombPlacerBehavior();
		if (a != nullptr) {
			shok_GGL_CHeroAbilityProps* p = e->GetEntityType()->GetBombPlacerBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, 500, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilityPlaceBomb(e->Position);
					return true;
				}
			}
		}
	}
	{
		shok_GGL_CCannonBuilderBehavior* a = e->GetCannonBuilderBehavior();
		if (a != nullptr) {
			shok_GGL_CCannonBuilderBehaviorProps* p = e->GetEntityType()->GetCannonBuilderBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
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
		shok_GGL_CShurikenAbility* a = e->GetShurikenBehavior();
		if (a != nullptr) {
			shok_GGL_CShurikenAbilityProps* p = e->GetEntityType()->GetShurikenBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
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
		shok_GGL_CConvertSettlerAbility* a = e->GetConvertSettlerBehavior();
		if (a != nullptr) {
			shok_GGL_CConvertSettlerAbilityProps* p = e->GetEntityType()->GetConvertSettlersBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
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
		shok_GGL_CSniperAbility* a = e->GetSniperBehavior();
		if (a != nullptr) {
			shok_GGL_CSniperAbilityProps* p = e->GetEntityType()->GetSniperBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
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
		shok_GGL_CInflictFearAbility* a = e->GetInflictFearBehavior();
		if (a != nullptr) {
			shok_GGL_CInflictFearAbilityProps* p = e->GetEntityType()->GetInflictFearBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
				if (CountTargetsInArea(Player, e->Position, p->Range/2, IgnoreFleeing) >= 10) {
					((shok_GGL_CSettler*)e)->HeroAbilityInflictFear();
					return true;
				}
			}
		}
	}
	{
		shok_GGL_CKegPlacerBehavior* a = e->GetKegPlacerBehavior();
		if (a != nullptr) {
			shok_GGL_CKegPlacerBehaviorProperties* p = e->GetEntityType()->GetKegPlacerBehaviorProps();
			if (a->AbilitySecondsCharged >= p->RechargeTimeSeconds) {
				shok_EGL_CGLEEntity* tar = GetNearestBuildingInArea(Player, e->Position, 3000);
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
	int tick = (*shok_EGL_CGLEGameLogicObject)->GetTick();
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
	lua_pushnumber(L, a->Status);
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
	lua_pushnumber(L, a->Status);
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
		if (shok_eid2obj(id)->IsEntityInCategory(EntityCategoryCannon))
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
	a->Status = luaL_checkint(L, -1);
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
	a->Status = luaL_checkint(L, 2);
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
		shok_EGL_CGLEEntity* e = shok_eid2obj(id);
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

int l_uaAddIdleTL(lua_State* L) {
	int tl = luaL_checkint(L, 1);
	UnlimitedArmy::IdleTaskLists.emplace_back(tl);
	return 0;
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
	luaext_registerFunc(L, "AddIdleTaskList", &l_uaAddIdleTL);
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
	lua_settable(L, -3);
	lua_pop(L, 1);
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.UA.GetNearestEnemyInArea(1, x,y, 1000)

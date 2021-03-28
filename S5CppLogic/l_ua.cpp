#include "pch.h"
#include "l_ua.h"
#include "luaext.h"
#include "l_entity.h"


std::vector<int> UnlimitedArmy::IdleTaskLists = std::vector<int>();
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
	int tick = (*shok_EGL_CGLEGameLogicObject)->GetTimeMS();
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
	auto e = std::remove_if(Leaders.begin(), Leaders.end(), shok_entityIsDead);
	Leaders.erase(e, Leaders.end());
	e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), shok_entityIsDead);
	LeaderInTransit.erase(e, LeaderInTransit.end());
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
		shok_EGL_CGLEEntity* e = GetNextTarget(Player, LastPos, Area);
		if (e == nullptr)
			CurrentBattleTarget = 0;
		else
			CurrentBattleTarget = e->EntityId;
	}
	bool preventComands = false;
	if (!preventComands && Status != UAStatus_MovingNoBattle && CurrentBattleTarget != 0) {
		CheckStatus(UAStatus_Battle);
		preventComands = true;
	}
	if (!preventComands && !IsInRange(LastPos, Target, 1000)) {
		if (Status != UAStatus_Moving && Status != UAStatus_MovingNoBattle)
			CheckStatus(UAStatus_Moving);
		preventComands = true;
	}
	if (!preventComands) {
		CheckStatus(UAStatus_Idle);
		// autorotate
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
	if (Leaders.size() == 0 || IsInRange(LastPos, e->Position, Area)) {
		Leaders.push_back(e->EntityId);
		PosLastUpdatedTick = -1;
		if (Target.X == -1) {
			Target = e->Position;
		}
		NeedFormat();
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
}
void UnlimitedArmy::CheckTransit()
{
	if (Leaders.size() == 0 && LeaderInTransit.size() > 0) {
		Leaders.push_back(LeaderInTransit[1]);
		LeaderInTransit.erase(LeaderInTransit.begin());
		CalculatePos();
	}
	auto e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), [this](int id) {
		shok_EGL_CGLEEntity* e = shok_eid2obj(id);
		if (IsInRange(this->LastPos, e->Position, this->Area)) {
			this->Leaders.push_back(id);
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
	return r;
}
shok_EGL_CGLEEntity* UnlimitedArmy::GetNextTarget(int player, shok_position& p, float ran)
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
	EntityIteratorPredicate* preds[7] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &cir
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, 7);
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
bool UnlimitedArmy::IsTargetValid(int id)
{
	if (shok_entityIsDead(id))
		return false;
	shok_EGL_CGLEEntity* e = shok_eid2obj(id);
	if (!IsInRange(LastPos, e->Position, Area))
		return false;
	// fleeing check
	shok_GGL_CCamouflageBehavior* c = e->GetCamoAbilityBehavior();
	if (c != nullptr) {
		return c->InvisibilityRemaining <= 0;
	}
	return true;
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
	for (int id : Leaders) {
		shok_EGL_CMovingEntity* e = (shok_EGL_CMovingEntity*) shok_eid2obj(id);
		if ((ReMove || !LeaderIsInBattle(e) ) && !IsNonCombat(e)) { // cannon command
			if (IsRanged(e)) {
				e->AttackEntity(CurrentBattleTarget);
			}
			else {
				e->AttackMove(p);
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
				e->Move(Target);
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
	if (!IsInRange(LastPos, Target, 1000))
		return false;
	for (int id : Leaders)
		if (!LeaderIsIdle(shok_eid2obj(id)))
			return false;
	return true;
}
int UnlimitedArmy::GetTargetsInArea(int player, shok_position& p, float ran)
{
	EntityIteratorPredicateIsRelevant relev = EntityIteratorPredicateIsRelevant();
	int buff[8];
	int bufflen = 8;
	EntityIteratorPredicateAnyPlayer::FillHostilePlayers(player, buff, bufflen);
	EntityIteratorPredicateAnyPlayer pl = EntityIteratorPredicateAnyPlayer(buff, bufflen);
	EntityIteratorPredicateOfEntityCategory cat = EntityIteratorPredicateOfEntityCategory(UATargetCat);
	EntityIteratorPredicateIsNotSoldier nsol = EntityIteratorPredicateIsNotSoldier();
	EntityIteratorPredicateIsVisible vis = EntityIteratorPredicateIsVisible();
	EntityIteratorPredicateInCircle cir = EntityIteratorPredicateInCircle(p.X, p.Y, ran);
	EntityIteratorPredicateIsBuilding buil = EntityIteratorPredicateIsBuilding();
	EntityIteratorPredicateIsAlive al = EntityIteratorPredicateIsAlive();
	EntityIteratorPredicate* preds[7] = {
		&relev, &pl, &cat, &al, &nsol, &vis, &cir
	};
	EntityIteratorPredicateAnd a = EntityIteratorPredicateAnd(preds, 7);
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
	lua_call(L, 0, 0); // errors get propagated to lua tick
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
		a->Leaders.emplace_back(luaL_checkint(L, -1));
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
	shok_position p = { luaL_checkfloat(L, 2), luaL_checkfloat(L, 3) };
	float r = luaL_checkfloat(L, 4);
	shok_EGL_CGLEEntity* e = UnlimitedArmy::GetNextTarget(pl, p, r);
	if (e == nullptr)
		lua_pushnumber(L, 0);
	else
		lua_pushnumber(L, e->EntityId);
	return 1;
}

int l_uaCount(lua_State* L) {
	int pl = luaL_checkint(L, 1);
	shok_position p = { luaL_checkfloat(L, 2), luaL_checkfloat(L, 3) };
	float r = luaL_checkfloat(L, 4);
	lua_pushnumber(L, UnlimitedArmy::GetTargetsInArea(pl, p, r));
	return 1;
}

int l_uaAddIdleTL(lua_State* L) {
	int tl = luaL_checkint(L, 1);
	UnlimitedArmy::IdleTaskLists.emplace_back(tl);
	return 0;
}

void l_ua_init(lua_State* L)
{
	luaext_registerFunc(L, "New", &l_uaNew);
	luaext_registerFunc(L, "GetNearestEnemyInArea", &l_uaNearest);
	luaext_registerFunc(L, "AddIdleTaskList", &l_uaAddIdleTL);
	luaext_registerFunc(L, "CountTargetEntitiesInArea", &l_uaCount);

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
	lua_settable(L, -3);
	lua_pop(L, 1);
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.UA.GetNearestEnemyInArea(1, x,y, 1000)

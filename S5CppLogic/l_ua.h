#pragma once

#include "luaimport.h"
#include "s5data.h"

void l_ua_init(lua_State* L);

enum class UAStatus : int {
	Idle = 1,
	Moving = 2,
	Battle = 3,
	Destroyed = 4,
	IdleUnformated = 5,
	MovingNoBattle = 6,
};

struct UACannonData {
	int EntityId, LastUpdated;
};
struct UACannonBuilderAbilityData {
	int HeroType, BottomType, TopType;
};
struct UATargetCache {
	int EntityId, Tick, Num;
};
struct UAReset {
	int EntityId;
	shok::PositionRot Pos;
};

class UnlimitedArmy {
public:
	int Player;
	std::vector<int> Leaders;
	std::vector<int> LeaderInTransit;
	std::vector<UACannonData> Cannons;
	std::vector<int> DeadHeroes;
	std::vector<UATargetCache> TargetCache;
	std::vector<UAReset> PrepDefenseReset;
	shok::Position LastPos = { -1,-1 };
	int PosLastUpdatedTick = -1;
	UAStatus Status = UAStatus::Idle;
	float Area = 0;
	int CurrentBattleTarget = 0;
	shok::Position Target = { -1,-1 };
	bool ReMove = false, IgnoreFleeing = false, PrepDefense = false, SabotageBridges = false;
	lua_State* L = nullptr;
	int Formation = LUA_NOREF, CommandQueue = LUA_NOREF, Spawner = LUA_NOREF, Normalize = LUA_NOREF;
	float AutoRotateFormation = -1;
	float LastRotation = 0;

	static std::vector<UACannonBuilderAbilityData> CannonBuilderAbilityData;

	UnlimitedArmy(int p);
	~UnlimitedArmy();
	void CalculatePos();
	void Tick();
	void AddLeader(EGL::CGLEEntity* e);
	void OnIdChanged(int ol, int ne);
	int GetSize(bool transit, bool hero);
	void RemoveLeader(EGL::CGLEEntity* e);
	bool IsIdle();
	bool IsRanged(EGL::CGLEEntity* e);
	bool IsNonCombat(EGL::CGLEEntity* e);

	static EGL::CGLEEntity* GetNearestTargetInArea(int player, shok::Position& p, float ran, bool notFleeing);
	static EGL::CGLEEntity* GetNearestSettlerInArea(int player, shok::Position& p, float ran, bool notFleeing);
	EGL::CGLEEntity* GetNearestBuildingInArea(int player, shok::Position& p, float ran);
	EGL::CGLEEntity* GetNearestBridgeInArea(shok::Position& p, float r);
	EGL::CGLEEntity* GetNearestSnipeTargetInArea(int player, shok::Position& p, float ran, bool notFleeing);
	EGL::CGLEEntity* GetFurthestConversionTargetInArea(int player, shok::Position& p, float ran, bool notFleeing);
	static int CountTargetsInArea(int player, shok::Position& p, float ran, bool notFleeing);



private:
	void CleanDead();
	inline void NeedFormat();
	void CheckTransit();
	bool IsTargetValid(int id);
	void CheckStatus(UAStatus status);
	bool LeaderIsMoving(EGL::CGLEEntity* e);
	bool LeaderIsIdle(EGL::CGLEEntity* e);
	bool LeaderIsInBattle(EGL::CGLEEntity* e);
	void BattleCommand();
	void MoveCommand();
	void FormationCommand();
	void CallFormation();
	void CallCommandQueue();
	void CallSpawner();
	void NormalizeSpeed(bool normalize, bool force);
	bool ExecuteHeroAbility(EGL::CGLEEntity* e);
	bool ExecutePrepDefense(EGL::CGLEEntity* e);
	bool CheckTargetCache(int id, int count);
	void UpdateTargetCache(int id, int time);

	static int AddLeader(lua_State* L);
	static int GetPos(lua_State* L);
	static int Tick(lua_State* L);
	static int Iterate(lua_State* L);
	static int IterateTransit(lua_State* L);
	static int OnIdChanged(lua_State* L);
	static int GetSize(lua_State* L);
	static int RemoveLeader(lua_State* L);
	static int IsIdle(lua_State* L);
	static int GetStatus(lua_State* L);
	static int SetArea(lua_State* L);
	static int SetTarget(lua_State* L);
	static int DumpTable(lua_State* L);
	static int ReadTable(lua_State* L);
	static int SetStatus(lua_State* L);
	static int SetReMove(lua_State* L);
	static int SetCurrentBattleTarget(lua_State* L);
	static int GetRangedMelee(lua_State* L);
	static int SetIgnoreFleeing(lua_State* L);
	static int SetAutoRotateFormation(lua_State* L);
	static int GetFirstDeadHero(lua_State* L);
	static int SetPrepDefense(lua_State* L);
	static int SetSabotageBridges(lua_State* L);

public:
	static constexpr const luaL_reg LuaMethods[] = {
		{"AddLeader", &AddLeader},
		{"GetPos", &GetPos},
		{"Tick", &Tick},
		{"Iterate", &Iterate},
		{"IterateTransit", &IterateTransit},
		{"OnIdChanged", &OnIdChanged},
		{"GetSize", &GetSize},
		{"RemoveLeader", &RemoveLeader},
		{"IsIdle", &IsIdle},
		{"GetStatus", &GetStatus},
		{"SetArea", &SetArea},
		{"SetTarget", &SetTarget},
		{"DumpTable", &DumpTable},
		{"ReadTable", &ReadTable},
		{"SetStatus", &SetStatus},
		{"SetReMove", &SetReMove},
		{"SetCurrentBattleTarget", &SetCurrentBattleTarget},
		{"GetRangedMelee", &GetRangedMelee},
		{"SetIgnoreFleeing", &SetIgnoreFleeing},
		{"SetAutoRotateFormation", &SetAutoRotateFormation},
		{"GetFirstDeadHero", &GetFirstDeadHero},
		{"SetPrepDefense", &SetPrepDefense},
		{"SetSabotageBridges", &SetSabotageBridges},
		{nullptr, nullptr},
	};
};

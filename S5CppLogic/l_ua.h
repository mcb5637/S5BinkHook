#pragma once

#include "luaimport.h"
#include "s5data.h"

void l_ua_init(lua_State* L);

#define UAStatus_Idle 1
#define UAStatus_Moving 2
#define UAStatus_Battle 3
#define UAStatus_Destroyed 4
#define UAStatus_IdleUnformated 5
#define UAStatus_MovingNoBattle 6

#define UATargetCat 100
#define UATargetCatLeader 101
#define EntityCategoryCustomRanged 102
#define EntityCategoryNonCombat 103

struct UACannonData {
	int EntityId, LastUpdated;
};

class UnlimitedArmy {
public:
	int Player;
	std::vector<int> Leaders;
	std::vector<int> LeaderInTransit;
	std::vector<UACannonData> Cannons;
	std::vector<int> DeadHeroes;
	shok_position LastPos = { -1,-1 };
	int PosLastUpdatedTick = -1;
	int Status = UAStatus_Idle;
	float Area = 0;
	int CurrentBattleTarget = 0;
	shok_position Target = { -1,-1 };
	bool ReMove = false, IgnoreFleeing = false;
	lua_State* L = nullptr;
	int Formation = LUA_NOREF, CommandQueue = LUA_NOREF, Spawner = LUA_NOREF, Normalize = LUA_NOREF;
	float AutoRotateFormation = -1;
	float LastRotation = 0;

	static std::vector<int> IdleTaskLists;

	UnlimitedArmy(int p);
	~UnlimitedArmy();
	void CalculatePos();
	void Tick();
	void AddLeader(shok_EGL_CGLEEntity* e);
	void OnIdChanged(int ol, int ne);
	int GetSize(bool transit, bool hero);
	void RemoveLeader(shok_EGL_CGLEEntity* e);
	bool IsIdle();
	bool IsRanged(shok_EGL_CGLEEntity* e);
	bool IsNonCombat(shok_EGL_CGLEEntity* e);

	static shok_EGL_CGLEEntity* GetNextTarget(int player, shok_position& p, float ran, bool notFleeing);
	static int GetTargetsInArea(int player, shok_position& p, float ran, bool notFleeing);

private:
	void CleanDead();
	inline void NeedFormat();
	void CheckTransit();
	bool IsTargetValid(int id);
	void CheckStatus(int status);
	bool LeaderIsMoving(shok_EGL_CGLEEntity* e);
	bool LeaderIsIdle(shok_EGL_CGLEEntity* e);
	bool LeaderIsInBattle(shok_EGL_CGLEEntity* e);
	bool LeaderIsIdleTL(shok_EGL_CGLEEntity* e);
	void BattleCommand();
	void MoveCommand();
	void FormationCommand();
	void CallFormation();
	void CallCommandQueue();
	void CallSpawner();
	void NormalizeSpeed(bool normalize, bool force);
};

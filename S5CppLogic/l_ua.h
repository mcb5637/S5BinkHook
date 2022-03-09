#pragma once

#include "luaimport.h"
#include "s5data.h"

namespace CppLogic::UA {
	void Init(lua::State L);

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
		int EntityId = 0;
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
		lua::State L = nullptr;
		lua::Reference Formation = lua::State::NoRef, CommandQueue = lua::State::NoRef, Spawner = lua::State::NoRef, Normalize = lua::State::NoRef;
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

		static int AddLeader(lua::State L);
		static int GetPos(lua::State L);
		static int Tick(lua::State L);
		static int Iterate(lua::State L);
		static int IterateTransit(lua::State L);
		static int OnIdChanged(lua::State L);
		static int GetSize(lua::State L);
		static int RemoveLeader(lua::State L);
		static int IsIdle(lua::State L);
		static int GetStatus(lua::State L);
		static int SetArea(lua::State L);
		static int SetTarget(lua::State L);
		static int DumpTable(lua::State L);
		static int ReadTable(lua::State L);
		static int SetStatus(lua::State L);
		static int SetReMove(lua::State L);
		static int SetCurrentBattleTarget(lua::State L);
		static int GetRangedMelee(lua::State L);
		static int SetIgnoreFleeing(lua::State L);
		static int SetAutoRotateFormation(lua::State L);
		static int GetFirstDeadHero(lua::State L);
		static int SetPrepDefense(lua::State L);
		static int SetSabotageBridges(lua::State L);

	public:
		static constexpr const std::array<lua::FuncReference, 23> LuaMethods = { {
				lua::FuncReference::GetRef<AddLeader>("AddLeader"),
				lua::FuncReference::GetRef<GetPos>("GetPos"),
				lua::FuncReference::GetRef<Tick>("Tick"),
				lua::FuncReference::GetRef<Iterate>("Iterate"),
				lua::FuncReference::GetRef<IterateTransit>("IterateTransit"),
				lua::FuncReference::GetRef<OnIdChanged>("OnIdChanged"),
				lua::FuncReference::GetRef<GetSize>("GetSize"),
				lua::FuncReference::GetRef<RemoveLeader>("RemoveLeader"),
				lua::FuncReference::GetRef<IsIdle>("IsIdle"),
				lua::FuncReference::GetRef<GetStatus>("GetStatus"),
				lua::FuncReference::GetRef<SetArea>("SetArea"),
				lua::FuncReference::GetRef<SetTarget>("SetTarget"),
				lua::FuncReference::GetRef<DumpTable>("DumpTable"),
				lua::FuncReference::GetRef<ReadTable>("ReadTable"),
				lua::FuncReference::GetRef<SetStatus>("SetStatus"),
				lua::FuncReference::GetRef<SetReMove>("SetReMove"),
				lua::FuncReference::GetRef<SetCurrentBattleTarget>("SetCurrentBattleTarget"),
				lua::FuncReference::GetRef<GetRangedMelee>("GetRangedMelee"),
				lua::FuncReference::GetRef<SetIgnoreFleeing>("SetIgnoreFleeing"),
				lua::FuncReference::GetRef<SetAutoRotateFormation>("SetAutoRotateFormation"),
				lua::FuncReference::GetRef<GetFirstDeadHero>("GetFirstDeadHero"),
				lua::FuncReference::GetRef<SetPrepDefense>("SetPrepDefense"),
				lua::FuncReference::GetRef<SetSabotageBridges>("SetSabotageBridges"),
		} };
	};
}

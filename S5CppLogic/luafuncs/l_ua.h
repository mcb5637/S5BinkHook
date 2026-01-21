#pragma once

#include <random>
#include "luaext.h"
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/s5_defines.h>
#include <utility/luaserializer.h>

namespace CppLogic::UA {
	void Init(luaext::State L);

	enum class UAStatus : int {
		Idle = 1,
		Moving = 2,
		Battle = 3,
		Destroyed = 4,
		IdleUnformated = 5,
		MovingNoBattle = 6,
	};

	struct UACannonData {
		shok::EntityId EntityId;
		int LastUpdated;

		static const BB::SerializationData SerializationData[];
	};
	struct UACannonBuilderAbilityData {
		shok::EntityTypeId HeroType, BottomType, TopType;

		static const BB::SerializationData SerializationData[];
	};
	struct UATargetCache {
		shok::EntityId EntityId;
		int Tick, Num;

		static const BB::SerializationData SerializationData[];
	};
	struct UAReset {
		shok::EntityId EntityId = shok::EntityId::Invalid;
		shok::PositionRot Pos;

		static const BB::SerializationData SerializationData[];
	};

	class UnlimitedArmy {
	public:
		shok::PlayerId Player;
		std::vector<shok::EntityId> Leaders;
		std::vector<shok::EntityId> LeaderInTransit;
		std::vector<UACannonData> Cannons;
		std::vector<shok::EntityId> DeadHeroes;
		std::vector<UATargetCache> TargetCache;
		std::vector<UAReset> PrepDefenseReset;
		shok::Position LastPos = { -1,-1 };
		int PosLastUpdatedTick = -1;
		UAStatus Status = UAStatus::Idle;
		float Area = 0;
		shok::EntityId CurrentBattleTarget = shok::EntityId::Invalid;
		shok::Position Target = { -1,-1 };
		bool ReMove = false, IgnoreFleeing = false, PrepDefense = false, SabotageBridges = false;
		bool DoNotNormalizeSpeed = false;
		luaext::State L{nullptr};
		luaext::Reference Formation = luaext::State::NoRef, CommandQueue = luaext::State::NoRef, Spawner = luaext::State::NoRef;
		float AutoRotateFormation = -1;
		float LastRotation = 0;

		std::minstd_rand RNG;

		static std::vector<UACannonBuilderAbilityData> CannonBuilderAbilityData;

		// not serialized references, lua state
		static const BB::SerializationData SerializationData[];

		explicit UnlimitedArmy(shok::PlayerId p);
		~UnlimitedArmy();
		void CalculatePos();
		void Tick();
		void AddLeader(EGL::CGLEEntity* e);
		void OnIdChanged(shok::EntityId ol, shok::EntityId ne);
		[[nodiscard]] int GetSize(bool transit, bool hero) const;
		void RemoveLeader(EGL::CGLEEntity* e);
		bool IsIdle();
		static bool IsRanged(EGL::CGLEEntity* e);
		static bool IsNonCombat(EGL::CGLEEntity* e);

		static EGL::CGLEEntity* GetNearestTargetInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing);
		static EGL::CGLEEntity* GetNearestSettlerInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing);
		EGL::CGLEEntity* GetNearestBuildingInArea(shok::PlayerId player, shok::Position& p, float ran);
		EGL::CGLEEntity* GetNearestBridgeInArea(shok::Position& p, float r);
		EGL::CGLEEntity* GetNearestSnipeTargetInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing);
		EGL::CGLEEntity* GetFurthestConversionTargetInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing);
		static int CountTargetsInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing);

		static void RegisterUDType(luaext::State L);


	private:
		void CleanDead();
		inline void NeedFormat();
		void CheckTransit();
		[[nodiscard]] bool IsTargetValid(shok::EntityId id) const;
		void CheckStatus(UAStatus status);
		static bool LeaderIsMoving(EGL::CGLEEntity* e);
		static bool LeaderIsIdle(EGL::CGLEEntity* e);

		static bool LeaderIsInBattle(EGL::CGLEEntity* e);
		void BattleCommand();
		void MoveCommand();
		void FormationCommand();
		void CallFormation();
		void CallCommandQueue();
		void CallSpawner();
		void NormalizeSpeed(bool normalize, bool force);
		bool ExecuteHeroAbility(EGL::CGLEEntity* e);
		bool ExecutePrepDefense(EGL::CGLEEntity* e);
		bool CheckTargetCache(shok::EntityId id, int count);
		void UpdateTargetCache(shok::EntityId id, int time);

		static int AddLeader(luaext::State L);
		static int GetPos(luaext::State L);
		static int Tick(luaext::State L);
		static int Iterate(luaext::State L);
		static int IterateTransit(luaext::State L);
		static int OnIdChanged(luaext::State L);
		static int GetSize(luaext::State L);
		static int RemoveLeader(luaext::State L);
		static int IsIdle(luaext::State L);
		static int GetStatus(luaext::State L);
		static int SetArea(luaext::State L);
		static int SetTarget(luaext::State L);
		static int DumpTable(luaext::State L);
		static int ReadTable(luaext::State L);
		static int SetStatus(luaext::State L);
		static int SetReMove(luaext::State L);
		static int SetCurrentBattleTarget(luaext::State L);
		static int GetRangedMelee(luaext::State L);
		static int SetIgnoreFleeing(luaext::State L);
		static int SetAutoRotateFormation(luaext::State L);
		static int GetFirstDeadHero(luaext::State L);
		static int SetPrepDefense(luaext::State L);
		static int SetSabotageBridges(luaext::State L);
		static int SetDoNotNormalizeSpeed(luaext::State L);

	public:
		static constexpr const std::array<luaext::FuncReference, 22> LuaMethods = { {
				luaext::FuncReference::GetRef<AddLeader>("AddLeader"),
				luaext::FuncReference::GetRef<GetPos>("GetPos"),
				luaext::FuncReference::GetRef<Tick>("Tick"),
				luaext::FuncReference::GetRef<Iterate>("Iterate"),
				luaext::FuncReference::GetRef<IterateTransit>("IterateTransit"),
				luaext::FuncReference::GetRef<OnIdChanged>("OnIdChanged"),
				luaext::FuncReference::GetRef<GetSize>("GetSize"),
				luaext::FuncReference::GetRef<RemoveLeader>("RemoveLeader"),
				luaext::FuncReference::GetRef<IsIdle>("IsIdle"),
				luaext::FuncReference::GetRef<GetStatus>("GetStatus"),
				luaext::FuncReference::GetRef<SetArea>("SetArea"),
				luaext::FuncReference::GetRef<SetTarget>("SetTarget"),
				//luaext::FuncReference::GetRef<DumpTable>("DumpTable"),
				//luaext::FuncReference::GetRef<ReadTable>("ReadTable"),
				luaext::FuncReference::GetRef<SetStatus>("SetStatus"),
				luaext::FuncReference::GetRef<SetReMove>("SetReMove"),
				luaext::FuncReference::GetRef<SetCurrentBattleTarget>("SetCurrentBattleTarget"),
				luaext::FuncReference::GetRef<GetRangedMelee>("GetRangedMelee"),
				luaext::FuncReference::GetRef<SetIgnoreFleeing>("SetIgnoreFleeing"),
				luaext::FuncReference::GetRef<SetAutoRotateFormation>("SetAutoRotateFormation"),
				luaext::FuncReference::GetRef<GetFirstDeadHero>("GetFirstDeadHero"),
				luaext::FuncReference::GetRef<SetPrepDefense>("SetPrepDefense"),
				luaext::FuncReference::GetRef<SetSabotageBridges>("SetSabotageBridges"),
				luaext::FuncReference::GetRef<SetDoNotNormalizeSpeed>("SetDoNotNormalizeSpeed"),
		} };

		static constexpr const std::array<luaext::FuncReference, 1> LuaMetaMethods{ {
				luaext::FuncReference::GetRef<DumpTable>(CppLogic::Serializer::AdvLuaStateSerializer::UserdataSerializerMetaEvent),
		} };
	};
}

template<>
// ReSharper disable once CppDFAConstantFunctionResult
inline BB::FieldSerializer* BB::FieldSerializer::GetSerializer<CppLogic::UA::UAStatus>() {
	return BB::FieldSerializer::TypeInt;
}

#include "pch.h"
#include "l_ua.h"
#include <random>
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entity.h"
#include "s5_maplogic.h"
#include "s5_defines.h"
#include "s5_events.h"
#include "s5_behaviorProps.h"
#include "s5_behaviors.h"
#include "s5_entitytype.h"
#include "s5_player.h"
#include "luaext.h"
#include "entityiterator.h"

namespace CppLogic::UA {
	std::vector<UACannonBuilderAbilityData> UnlimitedArmy::CannonBuilderAbilityData = std::vector<UACannonBuilderAbilityData>();

	UnlimitedArmy::~UnlimitedArmy()
	{
		L.UnRef(Formation, L.REGISTRYINDEX);
		L.UnRef(CommandQueue, L.REGISTRYINDEX);
		L.UnRef(Spawner, L.REGISTRYINDEX);
		L.UnRef(Normalize, L.REGISTRYINDEX);
	}
	UnlimitedArmy::UnlimitedArmy(int p)
	{
		Player = p;
	}
	void UnlimitedArmy::CalculatePos()
	{
		int tick = (*EGL::CGLEGameLogic::GlobalObj)->GetTick();
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
			EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
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
			if (EGL::CGLEEntity::EntityIDIsDead(id)) {
				EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
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
		auto e2 = std::remove_if(Cannons.begin(), Cannons.end(), [](UACannonData& d) { return EGL::CGLEEntity::EntityIDIsDead(d.EntityId); });
		Cannons.erase(e2, Cannons.end());
		e = std::remove_if(DeadHeroes.begin(), DeadHeroes.end(), [this](int id) {
			if (!EGL::CGLEEntity::EntityIDIsDead(id)) {
				this->AddLeader(EGL::CGLEEntity::GetEntityByID(id));
				return true;
			}
			return EGL::CGLEEntity::GetEntityByID(id) == nullptr;
			});
		DeadHeroes.erase(e, DeadHeroes.end());
		int tick = (*EGL::CGLEGameLogic::GlobalObj)->GetTick();
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
			EGL::CGLEEntity* e = GetNearestTargetInArea(Player, LastPos, Area, IgnoreFleeing);
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
				EGL::CGLEEntity* e = GetNearestTargetInArea(Player, LastPos, AutoRotateFormation, IgnoreFleeing);
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
	void UnlimitedArmy::AddLeader(EGL::CGLEEntity* e)
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
			if (EGL::CGLEEntity::GetEntityByID(LeaderInTransit[0])->IsEntityInCategory(shok::EntityCategory::Cannon))
				Cannons.push_back({ LeaderInTransit[0], -1 });
			LeaderInTransit.erase(LeaderInTransit.begin());
			PosLastUpdatedTick = -1;
			CalculatePos();
			NeedFormat();
		}
		auto e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), [this](int id) {
			EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
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
			EGL::CMovingEntity* e = ((EGL::CMovingEntity*)EGL::CGLEEntity::GetEntityByID(id));
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
	EGL::CGLEEntity* UnlimitedArmy::GetNearestTargetInArea(int player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catLeader{ shok::EntityCategory::TargetFilter_TargetTypeLeader };
		CppLogic::Iterator::EntityPredicateIsNotSoldier nsol{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsBuilding buil{};
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory cat{ shok::EntityCategory::TargetFilter_TargetType };
		CppLogic::Iterator::PredicatePriority<EGL::CGLEEntity> prio1{ &catLeader, 2 };
		CppLogic::Iterator::PredicatePriority<EGL::CGLEEntity> prio2{ &buil,1 };
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 10> a{
				&relev, &cat, &ali, &nsol, &vis, &notinbuild, &cir, &prio1, &prio2, &nflee,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetNearest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 9> a{
				&relev, &cat, &ali, &nsol, &vis, &notinbuild, &cir, &prio1, &prio2,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetNearest(nullptr);
		}
	}
	EGL::CGLEEntity* UnlimitedArmy::GetFurthestConversionTargetInArea(int player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catLeader{ shok::EntityCategory::TargetFilter_TargetTypeLeader };
		CppLogic::Iterator::EntityPredicateIsNotSoldier nsol{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity> fun{ [this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 1);
		} };
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 8> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &cir, &nflee,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetFurthest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 7> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &cir,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetFurthest(nullptr);
		}
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestSettlerInArea(int player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catLeader{ shok::EntityCategory::TargetFilter_TargetTypeLeader };
		CppLogic::Iterator::EntityPredicateIsNotSoldier nsol{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 8> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &cir, &nflee,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetNearest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 7> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &cir,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetNearest(nullptr);
		}
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestBuildingInArea(int player, shok::Position& p, float ran)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateIsBuilding buil{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity> fun{ [this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 2);
		} };
		CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 6> a{
			&relev, &buil, &ali, &vis, &cir, &fun,
		};
		CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
		return it.GetNearest(nullptr);
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestBridgeInArea(shok::Position& p, float ran) {
		CppLogic::Iterator::EntityPredicateOfPlayer pl{ 0 };
		CppLogic::Iterator::EntityPredicateIsBuilding buil{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory cat{ shok::EntityCategory::Bridge };
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran*ran };
		CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity> fun{ [this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 1);
		} };
		CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 5> a{
			&pl, &buil, &cat, &cir, &fun,
		};
		CppLogic::Iterator::GlobalEntityIterator it{ &a };
		return it.GetNearest(nullptr);
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestSnipeTargetInArea(int player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catHe{ shok::EntityCategory::Hero };
		CppLogic::Iterator::EntityPredicateOfEntityCategory catCan{ shok::EntityCategory::Cannon };
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran*ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity> fun{ [this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 1);
		} };
		CppLogic::Iterator::PredicateStaticOr<EGL::CGLEEntity, 2> cat{ &catHe, &catCan };
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 8> a{
				&relev, &cat, &ali, &vis, &notinbuild, &cir, &fun, &nflee,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetNearest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 7> a{
				&relev, &cat, &ali, &vis, &notinbuild, &cir, &fun,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.GetNearest(nullptr);
		}
	}
	bool UnlimitedArmy::IsTargetValid(int id)
	{
		if (EGL::CGLEEntity::EntityIDIsDead(id))
			return false;
		EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
		if (!LastPos.IsInRange(e->Position, Area))
			return false;
		GGL::CCamouflageBehavior* c = e->GetBehavior<GGL::CCamouflageBehavior>();
		if (c != nullptr) {
			return c->InvisibilityRemaining <= 0;
		}
		if (!IgnoreFleeing)
			return true;
		if (EGL::CMovingEntity* m = dynamic_cast<EGL::CMovingEntity*>(e)) {
			return !m->IsFleeingFrom(LastPos, 500);
		}
		return true;
	}
	void UnlimitedArmy::CheckStatus(UAStatus status)
	{
		if (Status != status) {
			Status = status;
			ReMove = true;
		}
	}
	bool UnlimitedArmy::LeaderIsMoving(EGL::CGLEEntity* e)
	{
		shok::LeaderCommand comm = e->EventLeaderGetCurrentCommand();
		return (comm == shok::LeaderCommand::Move || comm == shok::LeaderCommand::AttackMove || comm == shok::LeaderCommand::Patrol) && !LeaderIsIdle(e);
	}
	bool UnlimitedArmy::LeaderIsIdle(EGL::CGLEEntity* e)
	{
		return static_cast<GGL::CSettler*>(e)->IsIdle();
	}
	bool UnlimitedArmy::LeaderIsInBattle(EGL::CGLEEntity* e)
	{
		shok::LeaderCommand comm = e->EventLeaderGetCurrentCommand();
		return (comm == shok::LeaderCommand::Attack || comm == shok::LeaderCommand::AttackMove || comm == shok::LeaderCommand::HeroAbility) && !LeaderIsIdle(e);
	}
	bool UnlimitedArmy::IsRanged(EGL::CGLEEntity* e)
	{
		return e->IsEntityInCategory(shok::EntityCategory::LongRange) || e->IsEntityInCategory(shok::EntityCategory::Cannon) || e->IsEntityInCategory(shok::EntityCategory::TargetFilter_CustomRanged);
	}
	bool UnlimitedArmy::IsNonCombat(EGL::CGLEEntity* e)
	{
		return e->IsEntityInCategory(shok::EntityCategory::TargetFilter_NonCombat);
	}
	void UnlimitedArmy::BattleCommand()
	{
		shok::Position p = EGL::CGLEEntity::GetEntityByID(CurrentBattleTarget)->Position;
		EGL::CGLELandscape* ls = (*EGL::CGLEGameLogic::GlobalObj)->Landscape;
		if (ls->GetSector(&p) == 0) {
			shok::Position pou;
			if (ls->GetNearestPositionInSector(&p, 1000, EGL::CGLEEntity::GetEntityByID(Leaders[0])->GetSector(), &pou))
				p = pou;
		}
		if (ReMove) {
			PrepDefenseReset.clear();
			NormalizeSpeed(false, false);
		}
		for (int id : Leaders) {
			EGL::CMovingEntity* e = static_cast<EGL::CMovingEntity*>(EGL::CGLEEntity::GetEntityByID(id));
			if (e->IsEntityInCategory(shok::EntityCategory::Hero) || IsNonCombat(e)) {
				if (ExecuteHeroAbility(e))
					continue;
			}
			if ((ReMove || !LeaderIsInBattle(e)) && !IsNonCombat(e) && !e->IsEntityInCategory(shok::EntityCategory::Cannon)) {
				if (IsRanged(e)) {
					e->AttackEntity(CurrentBattleTarget);
				}
				else {
					e->AttackMove(p);
				}
			}
		}
		int tick = (*EGL::CGLEGameLogic::GlobalObj)->GetTick();
		for (UACannonData cd : Cannons) {
			EGL::CMovingEntity* e = static_cast<EGL::CMovingEntity*>(EGL::CGLEEntity::GetEntityByID(cd.EntityId));
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
		EGL::CGLELandscape* ls = (*EGL::CGLEGameLogic::GlobalObj)->Landscape;
		if (ls->GetSector(&p) == 0) {
			shok::Position pou;
			if (ls->GetNearestPositionInSector(&p, 1000, EGL::CGLEEntity::GetEntityByID(Leaders[0])->GetSector(), &pou))
				p = pou;
		}
		if (Status == UAStatus::MovingNoBattle) {
			for (int id : Leaders) {
				EGL::CMovingEntity* e = static_cast<EGL::CMovingEntity*>(EGL::CGLEEntity::GetEntityByID(id));
				if (ReMove || !LeaderIsMoving(e)) {
					e->Move(p);
				}
			}
		}
		else {
			for (int id : Leaders) {
				EGL::CMovingEntity* e = static_cast<EGL::CMovingEntity*>(EGL::CGLEEntity::GetEntityByID(id));
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
				EGL::CMovingEntity* e = static_cast<EGL::CMovingEntity*>(EGL::CGLEEntity::GetEntityByID(id));
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
				if (EGL::CGLEEntity::EntityIDIsDead(r.EntityId))
					return true;
				EGL::CMovingEntity* e = static_cast<EGL::CMovingEntity*>(EGL::CGLEEntity::GetEntityByID(r.EntityId));
				if (e->EventLeaderGetCurrentCommand() != shok::LeaderCommand::HeroAbility) {
					EGL::CMovingEntity* m = static_cast<EGL::CMovingEntity*>(e);
					m->Move(r.Pos);
					m->SetTargetRotation(r.Pos.r);
					EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, true };
					m->FireEvent(&ev);
					return true;
				}
				return false;
				}), PrepDefenseReset.end());
		}
		ReMove = false;
	}
	void UnlimitedArmy::RemoveLeader(EGL::CGLEEntity* e)
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
			if (!LeaderIsIdle(EGL::CGLEEntity::GetEntityByID(id)))
				return false;
		return true;
	}
	int UnlimitedArmy::CountTargetsInArea(int player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory cat{ shok::EntityCategory::TargetFilter_TargetType };
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran*ran };
		CppLogic::Iterator::EntityPredicateIsBuilding buil{};
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 6> a{
				&relev, &cat, &ali, &vis, &cir, &nflee,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.Count();
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 5> a{
				&relev, &cat, &ali, &vis, &cir,
			};
			CppLogic::Iterator::MultiPlayerEntityIterator it{ &a };
			CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(it.Players, player);
			return it.Count();
		}
	}
	void UnlimitedArmy::CallFormation()
	{
		int t = L.GetTop();
		L.Push(Formation, L.REGISTRYINDEX);
		L.Push(LastRotation);
		L.TCall(1, 0);
		L.SetTop(t);
	}
	void UnlimitedArmy::CallCommandQueue()
	{
		int t = L.GetTop();
		L.Push(CommandQueue, L.REGISTRYINDEX);
		L.TCall(0, 0);
		L.SetTop(t);
	}
	void UnlimitedArmy::CallSpawner()
	{
		int t = L.GetTop();
		L.Push(Spawner, L.REGISTRYINDEX);
		L.TCall(0, 0);
		L.SetTop(t);
	}
	void UnlimitedArmy::NormalizeSpeed(bool normalize, bool force)
	{
		int t = L.GetTop();
		L.Push(Normalize, L.REGISTRYINDEX);
		L.Push(normalize);
		L.Push(force);
		L.TCall(2, 0);
		L.SetTop(t);
	}
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr(-10, 10);
	bool UnlimitedArmy::ExecuteHeroAbility(EGL::CGLEEntity* e)
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
						if (!static_cast<GGL::CSettler*>(e)->IsMoving() && CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 10) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityRangedEffect();
						}
					}
					else if (p->IsDefensive()) {
						if (CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 10) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityRangedEffect();
						}
					}
					else if (p->IsHeal()) {
						if (e->Health <= e->GetMaxHealth() / 2 && CountTargetsInArea(Player, e->Position, r, IgnoreFleeing) >= 2) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityRangedEffect();
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
						static_cast<GGL::CSettler*>(e)->HeroAbilitySummon();
						e->ObservedEntities.ForAll([this](shok::Attachment* a) {
							if (a->AttachmentType == shok::AttachmentType::SUMMONER_SUMMONED)
								AddLeader(EGL::CGLEEntity::GetEntityByID(a->EntityId));
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
						static_cast<GGL::CSettler*>(e)->HeroAbilityCircularAttack();
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
						static_cast<GGL::CSettler*>(e)->HeroAbilityPlaceBomb(e->Position);
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
						p.X += distr(generator) * 100;
						p.Y += distr(generator) * 100;
						p.FloorToBuildingPlacement();
						if (GGL::CPlayerStatus::CanPlaceBuilding(d->BottomType, Player, &p, 0, 0)) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityPlaceCannon(p, d->BottomType, d->TopType);
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
						EGL::CGLEEntity* tar = GetNearestSettlerInArea(Player, e->Position, p->Range, IgnoreFleeing);
						if (tar != nullptr) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityShuriken(tar->EntityId);
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
						EGL::CGLEEntity* tar = GetFurthestConversionTargetInArea(Player, e->Position, p->ConversionStartRange, IgnoreFleeing);
						if (tar != nullptr) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityConvert(tar->EntityId);
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
					EGL::CGLEEntity* tar = GetNearestSnipeTargetInArea(Player, e->Position, p->Range, IgnoreFleeing);
					if (tar != nullptr) {
						static_cast<GGL::CSettler*>(e)->HeroAbilitySnipe(tar->EntityId);
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
					if (CountTargetsInArea(Player, e->Position, p->Range / 2, IgnoreFleeing) >= 10) {
						static_cast<GGL::CSettler*>(e)->HeroAbilityInflictFear();
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
					EGL::CGLEEntity* tar = GetNearestBuildingInArea(Player, e->Position, Area);
					if (tar == nullptr && SabotageBridges)
						tar = GetNearestBridgeInArea(e->Position, 6000);
					if (tar != nullptr) {
						static_cast<GGL::CSettler*>(e)->ThiefSabotage(tar->EntityId);
						UpdateTargetCache(tar->EntityId, 500);
						return true;
					}
				}
			}
		}
		if (EGL::CGLEEntity::CamoActivateCb) { // if camo fix is active, use camo to get rid of attackers
			GGL::CCamouflageBehavior* a = e->GetBehavior<GGL::CCamouflageBehavior>();
			if (a != nullptr && !dynamic_cast<GGL::CThiefCamouflageBehavior*>(a)) {
				GGL::CCamouflageBehaviorProps* p = e->GetEntityType()->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
				if (a->SecondsCharged >= p->RechargeTimeSeconds) {
					if (e->Health <= e->GetMaxHealth() / 2 && CountTargetsInArea(Player, e->Position, Area, IgnoreFleeing) >= 5) {
						static_cast<GGL::CSettler*>(e)->HeroAbilityActivateCamoflage();
						return true;
					}
				}
			}
		}

		return false;
	}
	bool UnlimitedArmy::ExecutePrepDefense(EGL::CGLEEntity* e)
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
						if (GGL::CPlayerStatus::CanPlaceBuilding(d->BottomType, Player, &p, 0, 0)) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityPlaceCannon(p, d->BottomType, d->TopType);
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
					EGL::CGLEEntity* tar = GetNearestBuildingInArea(Player, e->Position, Area);
					if (tar == nullptr && SabotageBridges)
						tar = GetNearestBridgeInArea(e->Position, Area);
					if (tar != nullptr) {
						static_cast<GGL::CSettler*>(e)->ThiefSabotage(tar->EntityId);
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
		int tick = (*EGL::CGLEGameLogic::GlobalObj)->GetTick();
		for (UATargetCache& t : TargetCache) {
			if (t.EntityId == id) {
				t.Num++;
				t.Tick = tick + time;
				return;
			}
		}
		TargetCache.push_back({ id, tick + time, 1 });
	}

	int UnlimitedArmy::AddLeader(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->AddLeader(luaext::EState{ L }.CheckSettler(2));
		return 0;
	}

	int UnlimitedArmy::GetPos(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->CalculatePos();
		luaext::EState{ L }.PushPos(a->LastPos);
		return 1;
	}
	int UnlimitedArmy::Tick(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->Tick();
		return 0;
	}

	int UA_Iterate_Next(lua::State L) {
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		int i = L.CheckInt(2);
		i++;
		if (i >= static_cast<int>(a->Leaders.size())) {
			L.Push();
			L.Push();
		}
		else {
			L.Push(i);
			L.Push(a->Leaders[i]);
		}
		return 2;
	}
	int UnlimitedArmy::Iterate(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		L.Push<UA_Iterate_Next>();
		L.PushValue(1);
		L.Push(-1);
		return 3;
	}

	int UA_IterateTransit_Next(lua::State L) {
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		int i = L.CheckInt(2);
		i++;
		if (i >= (int)a->LeaderInTransit.size()) {
			L.Push();
			L.Push();
		}
		else {
			L.Push(i);
			L.Push(a->LeaderInTransit[i]);
		}
		return 2;
	}
	int UnlimitedArmy::IterateTransit(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		L.Push<UA_IterateTransit_Next>();
		L.PushValue(1);
		L.Push(-1);
		return 3;
	}

	int UnlimitedArmy::OnIdChanged(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		int old = L.CheckInt(2);
		int ne = L.CheckInt(3);
		a->OnIdChanged(old, ne);
		return 0;
	}

	int UnlimitedArmy::GetSize(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		int i = a->GetSize(L.ToBoolean(2), L.ToBoolean(3));
		L.Push(i);
		return 1;
	}

	int UnlimitedArmy::RemoveLeader(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->RemoveLeader(luaext::EState{ L }.CheckEntity(2));
		return 0;
	}

	int UnlimitedArmy::IsIdle(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		L.Push(a->IsIdle());
		return 1;
	}
	int UnlimitedArmy::GetStatus(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		L.Push(static_cast<int>(a->Status));
		return 1;
	}

	int UnlimitedArmy::SetArea(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->Area = L.CheckFloat(2);
		return 0;
	}
	int UnlimitedArmy::SetTarget(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->Target = luaext::EState{ L }.CheckPos(2);
		return 0;
	}

	int UnlimitedArmy::DumpTable(lua::State Ls)
	{
		luaext::EState L{ Ls };
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		L.NewTable();
		L.Push("Leaders");
		L.NewTable();
		int i = 1;
		for (int l : a->Leaders) {
			L.Push(l);
			L.SetTableRawI(-2, i);
			i++;
		}
		L.SetTableRaw(-3);
		L.Push("LeadersTransit");
		L.NewTable();
		i = 1;
		for (int l : a->LeaderInTransit) {
			L.Push(l);
			L.SetTableRawI(-2, i);
			i++;
		}
		L.SetTableRaw(-3);
		L.Push("DeadHeroes");
		L.NewTable();
		i = 1;
		for (int l : a->DeadHeroes) {
			L.Push(l);
			L.SetTableRawI(-2, i);
			i++;
		}
		L.SetTableRaw(-3);
		L.Push("Player");
		L.Push(a->Player);
		L.SetTableRaw(-3);
		L.Push("Status");
		L.Push(static_cast<int>(a->Status));
		L.SetTableRaw(-3);
		L.Push("Area");
		L.Push(a->Area);
		L.SetTableRaw(-3);
		L.Push("CurrentBattleTarget");
		L.Push(a->CurrentBattleTarget);
		L.SetTableRaw(-3);
		L.Push("Target");
		L.PushPos(a->Target);
		L.SetTableRaw(-3);
		L.Push("ReMove");
		L.Push(a->ReMove);
		L.SetTableRaw(-3);
		L.Push("IgnoreFleeing");
		L.Push(a->IgnoreFleeing);
		L.SetTableRaw(-3);
		L.Push("AutoRotateFormation");
		L.Push(a->AutoRotateFormation);
		L.SetTableRaw(-3);
		return 1;
	}
	int UnlimitedArmy::ReadTable(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		L.Push("Leaders");
		L.GetTableRaw(2);
		int i = 1;
		a->Leaders.clear();
		while (true) {
			L.GetTableRawI(-1, i);
			if (!L.IsNumber(-1)) {
				L.Pop(1);
				break;
			}
			int id = L.CheckInt(-1);
			a->Leaders.emplace_back(id);
			if (EGL::CGLEEntity::GetEntityByID(id)->IsEntityInCategory(shok::EntityCategory::Cannon))
				a->Cannons.push_back({ id, -1 });
			L.Pop(1);
			i++;
		}
		L.Pop(1);
		L.Push("LeadersTransit");
		L.GetTableRaw(2);
		i = 1;
		a->LeaderInTransit.clear();
		while (true) {
			L.GetTableRawI(-1, i);
			if (!L.IsNumber(-1)) {
				L.Pop(1);
				break;
			}
			a->LeaderInTransit.emplace_back(L.CheckInt(-1));
			L.Pop(1);
			i++;
		}
		L.Pop(1);
		L.Push("DeadHeroes");
		L.GetTableRaw(2);
		i = 1;
		a->DeadHeroes.clear();
		while (true) {
			L.GetTableRawI(-1, i);
			if (!L.IsNumber(-1)) {
				L.Pop(1);
				break;
			}
			a->DeadHeroes.emplace_back(L.CheckInt(-1));
			L.Pop(1);
			i++;
		}
		L.Pop(1);
		L.Push("Player");
		L.GetTableRaw(2);
		a->Player = L.CheckInt(-1);
		L.Pop(1);
		L.Push("Status");
		L.GetTableRaw(2);
		a->Status = static_cast<UAStatus>(L.CheckInt(-1));
		L.Pop(1);
		L.Push("Area");
		L.GetTableRaw(2);
		a->Area = L.CheckFloat(-1);
		L.Pop(1);
		L.Push("CurrentBattleTarget");
		L.GetTableRaw(2);
		a->CurrentBattleTarget = L.CheckInt(-1);
		L.Pop(1);
		L.Push("Target");
		L.GetTableRaw(2);
		a->Target = luaext::EState{ L }.CheckPos(-1);
		L.Pop(1);
		L.Push("ReMove");
		L.GetTableRaw(2);
		a->ReMove = L.ToBoolean(-1);
		L.Pop(1);
		L.Push("IgnoreFleeing");
		L.GetTableRaw(2);
		a->IgnoreFleeing = L.ToBoolean(-1);
		L.Pop(1);
		L.Push("AutoRotateFormation");
		L.GetTableRaw(2);
		a->AutoRotateFormation = L.CheckFloat(-1);
		L.Pop(1);
		return 0;
	}

	int UnlimitedArmy::SetStatus(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->Status = static_cast<UAStatus>(L.CheckInt(2));
		return 0;
	}
	int UnlimitedArmy::SetReMove(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->ReMove = L.ToBoolean(2);
		return 0;
	}
	int UnlimitedArmy::SetCurrentBattleTarget(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->CurrentBattleTarget = luaext::EState{ L }.CheckEntity(2)->EntityId;
		return 0;
	}

	int UnlimitedArmy::GetRangedMelee(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		L.NewTable();
		L.NewTable();
		L.NewTable();
		int ran = 1, mel = 1, nc = 1;
		for (int id : a->Leaders) {
			L.Push(id);
			EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
			if (a->IsRanged(e)) {
				L.SetTableRawI(-4, ran);
				ran++;
			}
			else if (a->IsNonCombat(e)) {
				L.SetTableRawI(-2, nc);
				nc++;
			}
			else {
				L.SetTableRawI(-3, mel);
				mel++;
			}
		}
		return 3;
	}

	int UnlimitedArmy::SetIgnoreFleeing(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->IgnoreFleeing = L.ToBoolean(2);
		return 0;
	}
	int UnlimitedArmy::SetAutoRotateFormation(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->AutoRotateFormation = L.CheckFloat(2);
		return 0;
	}

	int UnlimitedArmy::GetFirstDeadHero(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		if (a->DeadHeroes.size() > 0)
			L.Push(a->DeadHeroes[0]);
		else
			L.Push();
		return 1;
	}

	int UnlimitedArmy::SetPrepDefense(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->PrepDefense = L.ToBoolean(2);
		return 0;
	}
	int UnlimitedArmy::SetSabotageBridges(lua::State L)
	{
		UnlimitedArmy* a = L.GetUserData<UnlimitedArmy>(1);
		a->SabotageBridges = L.ToBoolean(2);
		return 0;
	}

	int New(lua::State L) {
		int pl = L.CheckInt(1);
		L.CheckType(2, lua::LType::Function);
		L.CheckType(3, lua::LType::Function);
		L.CheckType(4, lua::LType::Function);
		L.CheckType(5, lua::LType::Function);
		UnlimitedArmy* a = L.NewUserData<UnlimitedArmy>(pl);
		a->L = L;
		L.PushValue(2);
		a->Formation = L.Ref(L.REGISTRYINDEX);
		L.PushValue(3);
		a->CommandQueue = L.Ref(L.REGISTRYINDEX);
		L.PushValue(4);
		a->Spawner = L.Ref(L.REGISTRYINDEX);
		L.PushValue(5);
		a->Normalize = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int GetNearestEnemyInArea(lua::State Ls) {
		luaext::EState L{ Ls };
		int pl = L.CheckInt(1);
		shok::Position p = L.CheckPos(2);
		float r = L.CheckFloat(3);
		EGL::CGLEEntity* e = UnlimitedArmy::GetNearestTargetInArea(pl, p, r, L.ToBoolean(4));
		if (e == nullptr)
			L.Push(0);
		else
			L.Push(e->EntityId);
		return 1;
	}

	int CountTargetEntitiesInArea(lua::State Ls) {
		luaext::EState L{ Ls };
		int pl = L.CheckInt(1);
		shok::Position p = L.CheckPos(2);
		float r = L.CheckFloat(3);
		L.Push(UnlimitedArmy::CountTargetsInArea(pl, p, r, L.ToBoolean(4)));
		return 1;
	}

	int AddCannonBuilderData(lua::State L) {
		UACannonBuilderAbilityData d = { L.CheckInt(1), L.CheckInt(2), L.CheckInt(3) };
		UnlimitedArmy::CannonBuilderAbilityData.emplace_back(d);
		return 0;
	}

	constexpr std::array<lua::FuncReference, 4> UA{ {
			lua::FuncReference::GetRef<New>("New"),
			lua::FuncReference::GetRef<GetNearestEnemyInArea>("GetNearestEnemyInArea"),
			lua::FuncReference::GetRef<CountTargetEntitiesInArea>("CountTargetEntitiesInArea"),
			lua::FuncReference::GetRef<AddCannonBuilderData>("AddCannonBuilderData"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(UA, -3);

		L.PrepareUserDataType<UnlimitedArmy>();
	}
}
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.UA.GetNearestEnemyInArea(1, x,y, 1000)

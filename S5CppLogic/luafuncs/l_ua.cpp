#include "pch.h"
#include "l_ua.h"
#include <random>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_entity.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/s5_defines.h>
#include <shok/events/s5_events.h>
#include <shok/entitytype/s5_behaviorProps.h>
#include <shok/entity/s5_behaviors.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/player/s5_player.h>
#include <shok/ui/s5_ui.h>
#include <utility/entityiterator.h>

CreateSerializationListFor(CppLogic::UA::UnlimitedArmy, Leaders);
CreateSerializationListFor(CppLogic::UA::UnlimitedArmy, Cannons);
CreateSerializationListFor(CppLogic::UA::UnlimitedArmy, TargetCache);
CreateSerializationListFor(CppLogic::UA::UnlimitedArmy, PrepDefenseReset);

namespace CppLogic::UA {
	std::vector<UACannonBuilderAbilityData> UnlimitedArmy::CannonBuilderAbilityData = std::vector<UACannonBuilderAbilityData>();

	const BB::SerializationData UACannonData::SerializationData[]{
		AutoMemberSerialization(UACannonData, EntityId),
		AutoMemberSerialization(UACannonData, LastUpdated),
		BB::SerializationData::GuardData(),
	};
	const BB::SerializationData UACannonBuilderAbilityData::SerializationData[]{
		AutoMemberSerialization(UACannonBuilderAbilityData, HeroType),
		AutoMemberSerialization(UACannonBuilderAbilityData, BottomType),
		AutoMemberSerialization(UACannonBuilderAbilityData, TopType),
		BB::SerializationData::GuardData(),
	};
	const BB::SerializationData UATargetCache::SerializationData[]{
		AutoMemberSerialization(UATargetCache, EntityId),
		AutoMemberSerialization(UATargetCache, Tick),
		AutoMemberSerialization(UATargetCache, Num),
		BB::SerializationData::GuardData(),
	};
	const BB::SerializationData UAReset::SerializationData[]{
		AutoMemberSerialization(UAReset, EntityId),
		AutoMemberSerialization(UAReset, Pos),
		BB::SerializationData::GuardData(),
	};
	const BB::SerializationData UnlimitedArmy::SerializationData[]{
		AutoMemberSerialization(UnlimitedArmy, Player),
		AutoMemberSerialization(UnlimitedArmy, Leaders),
		AutoMemberSerialization(UnlimitedArmy, LeaderInTransit),
		AutoMemberSerialization(UnlimitedArmy, Cannons),
		AutoMemberSerialization(UnlimitedArmy, DeadHeroes),
		AutoMemberSerialization(UnlimitedArmy, TargetCache),
		AutoMemberSerialization(UnlimitedArmy, PrepDefenseReset),
		AutoMemberSerialization(UnlimitedArmy, LastPos),
		AutoMemberSerialization(UnlimitedArmy, PosLastUpdatedTick),
		AutoMemberSerialization(UnlimitedArmy, Status),
		AutoMemberSerialization(UnlimitedArmy, Area),
		AutoMemberSerialization(UnlimitedArmy, CurrentBattleTarget),
		AutoMemberSerialization(UnlimitedArmy, Target),
		AutoMemberSerialization(UnlimitedArmy, ReMove),
		AutoMemberSerialization(UnlimitedArmy, IgnoreFleeing),
		AutoMemberSerialization(UnlimitedArmy, PrepDefense),
		AutoMemberSerialization(UnlimitedArmy, SabotageBridges),
		AutoMemberSerialization(UnlimitedArmy, DoNotNormalizeSpeed),
		AutoMemberSerialization(UnlimitedArmy, AutoRotateFormation),
		AutoMemberSerialization(UnlimitedArmy, LastRotation),
		AutoMemberSerialization(UnlimitedArmy, RNG),
		BB::SerializationData::GuardData(),
	};

	UnlimitedArmy::~UnlimitedArmy()
	{
		L.UnRef(Formation, L.REGISTRYINDEX);
		L.UnRef(CommandQueue, L.REGISTRYINDEX);
		L.UnRef(Spawner, L.REGISTRYINDEX);
	}
	UnlimitedArmy::UnlimitedArmy(shok::PlayerId p)
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
		for (auto id : Leaders) {
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
		auto remo = [this](shok::EntityId id) {
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
		e = std::remove_if(DeadHeroes.begin(), DeadHeroes.end(), [this](shok::EntityId id) {
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
			CleanDead();
			return;
		}
		PosLastUpdatedTick = -1;
		CalculatePos();
		if (!IsTargetValid(CurrentBattleTarget)) {
			EGL::CGLEEntity* e = GetNearestTargetInArea(Player, LastPos, Area, IgnoreFleeing);
			if (e == nullptr)
				CurrentBattleTarget = shok::EntityId::Invalid;
			else {
				CurrentBattleTarget = e->EntityId;
				for (UACannonData d : Cannons) {
					d.LastUpdated = -1;
				}
			}
		}
		bool preventComands = false;
		if (!preventComands && Status != UAStatus::MovingNoBattle && CurrentBattleTarget != shok::EntityId::Invalid) {
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
		CleanDead();
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
			if (Status == UAStatus::Moving || Status == UAStatus::MovingNoBattle)
				ReMove = true;
			if (e->IsEntityInCategory(shok::EntityCategory::Cannon))
				Cannons.push_back({ e->EntityId, -1 });
		}
		else
			LeaderInTransit.push_back(e->EntityId);
	}
	void UnlimitedArmy::OnIdChanged(shok::EntityId old, shok::EntityId ne)
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
		auto e = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), [this](shok::EntityId id) {
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
		for (shok::EntityId id : LeaderInTransit) {
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
	EGL::CGLEEntity* UnlimitedArmy::GetNearestTargetInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catLeader{ shok::EntityCategory::TargetFilter_TargetTypeLeader };
		CppLogic::Iterator::EntityPredicateIsNotSoldier nsol{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsBuildingAndNotConstructionSite buil{};
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory cat{ shok::EntityCategory::TargetFilter_TargetType };
		CppLogic::Iterator::EntityPredicateOfAnyPlayer pl{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(pl.players, player);
		CppLogic::Iterator::PredicatePriority<EGL::CGLEEntity> prio1{ &catLeader, 2 };
		CppLogic::Iterator::PredicatePriority<EGL::CGLEEntity> prio2{ &buil,1 };
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 11> a{
				&relev, &cat, &ali, &nsol, &vis, &notinbuild, &cir, &pl, &prio1, &prio2, &nflee,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategoryBuilding | shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetNearest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 10> a{
				&relev, &cat, &ali, &nsol, &vis, &notinbuild, &cir, &pl, &prio1, &prio2,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategoryBuilding | shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetNearest(nullptr);
		}
	}
	EGL::CGLEEntity* UnlimitedArmy::GetFurthestConversionTargetInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catLeader{ shok::EntityCategory::TargetFilter_TargetTypeLeader };
		CppLogic::Iterator::EntityPredicateIsNotSoldier nsol{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer pl{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(pl.players, player);
		auto fun = CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity>([this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 1);
		});
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 9> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &pl, &cir, &nflee,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetFurthest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 8> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &pl, &cir,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetFurthest(nullptr);
		}
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestSettlerInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catLeader{ shok::EntityCategory::TargetFilter_TargetTypeLeader };
		CppLogic::Iterator::EntityPredicateIsNotSoldier nsol{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer pl{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(pl.players, player);
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 9> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &pl, &cir, &nflee,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetNearest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 8> a{
				&relev, &catLeader, &ali, &nsol, &vis, &notinbuild, &pl, &cir,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetNearest(nullptr);
		}
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestBuildingInArea(shok::PlayerId player, shok::Position& p, float ran)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateIsBuildingAndNotConstructionSite buil{};
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran * ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer pl{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(pl.players, player);
		auto fun = CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity>([this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 2);
		});
		CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 7> a{
			&relev, &buil, &ali, &vis, &pl, &cir, &fun,
		};
		CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategoryBuilding, &a };
		return it.GetNearest(nullptr);
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestBridgeInArea(shok::Position& p, float ran) {
		CppLogic::Iterator::EntityPredicateOfPlayer pl{ shok::PlayerId::P0 };
		CppLogic::Iterator::EntityPredicateIsBuildingAndNotConstructionSite buil{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory cat{ shok::EntityCategory::Bridge };
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran*ran };
		auto fun = CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity>([this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 1);
		});
		CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 5> a{
			&pl, &buil, &cat, &cir, &fun,
		};
		CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategoryBuilding, &a };
		return it.GetNearest(nullptr);
	}
	EGL::CGLEEntity* UnlimitedArmy::GetNearestSnipeTargetInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory catHe{ shok::EntityCategory::Hero };
		CppLogic::Iterator::EntityPredicateOfEntityCategory catCan{ shok::EntityCategory::Cannon };
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran*ran };
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateIsNotInBuilding notinbuild{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer pl{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(pl.players, player);
		auto fun = CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity>([this](const EGL::CGLEEntity* e, float*, int*) {
			return this->CheckTargetCache(e->EntityId, 1);
		});
		CppLogic::Iterator::PredicateStaticOr<EGL::CGLEEntity, 2> cat{ &catHe, &catCan };
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 9> a{
				&relev, &cat, &ali, &vis, &notinbuild, &pl, &cir, &fun, &nflee,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetNearest(nullptr);
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 8> a{
				&relev, &cat, &ali, &vis, &notinbuild, &pl, &cir, &fun,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.GetNearest(nullptr);
		}
	}
	bool UnlimitedArmy::IsTargetValid(shok::EntityId id)
	{
		if (EGL::CGLEEntity::EntityIDIsDead(id))
			return false;
		EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
		if (!LastPos.IsInRange(e->Position, Area))
			return false;
		GGL::CCamouflageBehavior* c = e->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
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
		if (ls->GetSector(&p) == shok::SectorId::Invalid) {
			shok::Position pou;
			if (ls->GetNearestPositionInSector(&p, 1000, EGL::CGLEEntity::GetEntityByID(Leaders[0])->GetSector(), &pou))
				p = pou;
		}
		if (ReMove) {
			PrepDefenseReset.clear();
			NormalizeSpeed(false, false);
		}
		for (shok::EntityId id : Leaders) {
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
		if (ls->GetSector(&p) == shok::SectorId::Invalid) {
			shok::Position pou;
			if (ls->GetNearestPositionInSector(&p, 1000, EGL::CGLEEntity::GetEntityByID(Leaders[0])->GetSector(), &pou))
				p = pou;
		}
		if (Status == UAStatus::MovingNoBattle) {
			for (shok::EntityId id : Leaders) {
				EGL::CMovingEntity* e = static_cast<EGL::CMovingEntity*>(EGL::CGLEEntity::GetEntityByID(id));
				if (ReMove || !LeaderIsMoving(e)) {
					e->Move(p);
				}
			}
		}
		else {
			for (shok::EntityId id : Leaders) {
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
			for (shok::EntityId id : Leaders) {
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
		shok::EntityId id = e->EntityId;
		auto f = [id](shok::EntityId i) { return i == id; };
		auto en = std::remove_if(Leaders.begin(), Leaders.end(), f);
		Leaders.erase(en, Leaders.end());
		en = std::remove_if(LeaderInTransit.begin(), LeaderInTransit.end(), f);
		LeaderInTransit.erase(en, LeaderInTransit.end());
		NeedFormat();
		if (Status == UAStatus::Moving || Status == UAStatus::MovingNoBattle)
			ReMove = true;
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
		for (shok::EntityId id : Leaders)
			if (!LeaderIsIdle(EGL::CGLEEntity::GetEntityByID(id)))
				return false;
		return true;
	}
	int UnlimitedArmy::CountTargetsInArea(shok::PlayerId player, shok::Position& p, float ran, bool notFleeing)
	{
		CppLogic::Iterator::EntityPredicateIsCombatRelevant relev{};
		CppLogic::Iterator::EntityPredicateOfEntityCategory cat{ shok::EntityCategory::TargetFilter_TargetType };
		CppLogic::Iterator::EntityPredicateIsVisible vis{};
		CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity> cir{ p, ran*ran };
		CppLogic::Iterator::EntityPredicateIsBuildingAndNotConstructionSite buil{};
		CppLogic::Iterator::EntityPredicateIsAlive ali{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer pl{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(pl.players, player);
		if (notFleeing) {
			CppLogic::Iterator::EntityPredicateIsNotFleeingFrom nflee{ p, 500 };
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 7> a{
				&relev, &cat, &ali, &vis, &pl, &cir, &nflee,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategoryBuilding | shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.Count();
		}
		else {
			CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 6> a{
				&relev, &cat, &ali, &vis, &pl, &cir,
			};
			CppLogic::Iterator::MultiRegionEntityIterator it{ p, ran, shok::AccessCategoryFlags::AccessCategoryBuilding | shok::AccessCategoryFlags::AccessCategorySettler, &a };
			return it.Count();
		}
	}
	void UnlimitedArmy::CallFormation()
	{
		int t = L.GetTop();
		L.Push(Formation, L.REGISTRYINDEX);
		L.PushValue(2);
		L.Push(LastRotation);
		L.TCall(2, 0);
		L.SetTop(t);
	}
	void UnlimitedArmy::CallCommandQueue()
	{
		int t = L.GetTop();
		L.Push(CommandQueue, L.REGISTRYINDEX);
		L.PushValue(2);
		L.TCall(1, 0);
		L.SetTop(t);
	}
	void UnlimitedArmy::CallSpawner()
	{
		int t = L.GetTop();
		L.Push(Spawner, L.REGISTRYINDEX);
		L.PushValue(2);
		L.TCall(1, 0);
		L.SetTop(t);
	}
	void UnlimitedArmy::NormalizeSpeed(bool normalize, bool force)
	{
		if (DoNotNormalizeSpeed && !force)
			return;
		if (normalize && !DoNotNormalizeSpeed) {
			float lowest = -1.0f;
			for (shok::EntityId id : Leaders) {
				EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
				float s = e->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>()->GetMovementSpeed();
				if (lowest < 0 || s < lowest)
					lowest = s;
			}
			EGL::CEventValue_Float ev{ shok::EventIDs::Movement_SetSpeedFactor, 1.0f };
			for (shok::EntityId id : Leaders) {
				EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
				float s = e->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>()->GetMovementSpeed();
				ev.Data = lowest / s;
				e->FireEvent(&ev);
				for (const auto& s : e->ObservedEntities.ForKeys(shok::AttachmentType::LEADER_SOLDIER)) {
					EGL::CGLEEntity* es = EGL::CGLEEntity::GetEntityByID(s.second.EntityId);
					if (es && !es->IsDead())
						es->FireEvent(&ev);
				}
			}
		}
		else {
			EGL::CEventValue_Float ev{ shok::EventIDs::Movement_SetSpeedFactor, 1.0f };
			for (shok::EntityId id : Leaders) {
				EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
				e->FireEvent(&ev);
				for (const auto& s : e->ObservedEntities.ForKeys(shok::AttachmentType::LEADER_SOLDIER)) {
					EGL::CGLEEntity* es = EGL::CGLEEntity::GetEntityByID(s.second.EntityId);
					if (es && !es->IsDead())
						es->FireEvent(&ev);
				}
			}
		}
	}
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
						for (const auto& a : e->ObservedEntities) {
							if (a.first == shok::AttachmentType::SUMMONER_SUMMONED)
								AddLeader(EGL::CGLEEntity::GetEntityByID(a.second.EntityId));
						}
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
					shok::EntityTypeId ety = e->EntityType;
					auto d = std::find_if(CannonBuilderAbilityData.begin(), CannonBuilderAbilityData.end(), [ety](UACannonBuilderAbilityData& d) {return d.HeroType == ety; });
					if (d != CannonBuilderAbilityData.end() && CountTargetsInArea(Player, e->Position, 2000, IgnoreFleeing) >= 10) {
						shok::Position p = e->Position;
						p.X += distr(RNG) * 100;
						p.Y += distr(RNG) * 100;
						shok::Position out;
						p.FloorToBuildingPlacement();
						if (GGUI::CManager::GlobalObj()->GUIInterface->GetNearestFreePosForBuildingPlacement(d->BottomType, p, out)) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityPlaceCannon(out, d->BottomType, d->TopType);
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
		if (GGL::CCamouflageBehavior::CamoActivateCb) { // if camo fix is active, use camo to get rid of attackers
			GGL::CCamouflageBehavior* a = e->GetBehavior<GGL::CCamouflageBehavior>();
			if (a != nullptr) {
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
					shok::EntityTypeId ety = e->EntityType;
					auto d = std::find_if(CannonBuilderAbilityData.begin(), CannonBuilderAbilityData.end(), [ety](UACannonBuilderAbilityData& d) {return d.HeroType == ety; });
					if (d != CannonBuilderAbilityData.end()) {
						shok::Position p = e->Position;
						p.X += distr(RNG) * 100;
						p.Y += distr(RNG) * 100;
						shok::Position out;
						p.FloorToBuildingPlacement();
						if (GGUI::CManager::GlobalObj()->GUIInterface->GetNearestFreePosForBuildingPlacement(d->BottomType, p, out)) {
							static_cast<GGL::CSettler*>(e)->HeroAbilityPlaceCannon(out, d->BottomType, d->TopType);
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

	bool UnlimitedArmy::CheckTargetCache(shok::EntityId id, int count) {
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
	void UnlimitedArmy::UpdateTargetCache(shok::EntityId id, int time) {
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
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->AddLeader(luaext::EState{ L }.CheckSettler(2));
		return 0;
	}

	int UnlimitedArmy::GetPos(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->CalculatePos();
		luaext::EState{ L }.PushPos(a->LastPos);
		return 1;
	}
	int UnlimitedArmy::Tick(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->Tick();
		return 0;
	}

	int UA_Iterate_Next(lua::State ls) {
		luaext::EState L{ ls };
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
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
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		L.Push<UA_Iterate_Next>();
		L.PushValue(1);
		L.Push(-1);
		return 3;
	}

	int UA_IterateTransit_Next(lua::State ls) {
		luaext::EState L{ ls };
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		int i = L.CheckInt(2);
		i++;
		if (i >= static_cast<int>(a->LeaderInTransit.size())) {
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
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		L.Push<UA_IterateTransit_Next>();
		L.PushValue(1);
		L.Push(-1);
		return 3;
	}

	int UnlimitedArmy::OnIdChanged(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		int old = L.CheckInt(2);
		int ne = L.CheckInt(3);
		a->OnIdChanged(static_cast<shok::EntityId>(old), static_cast<shok::EntityId>(ne));
		return 0;
	}

	int UnlimitedArmy::GetSize(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		int i = a->GetSize(L.ToBoolean(2), L.ToBoolean(3));
		L.Push(i);
		return 1;
	}

	int UnlimitedArmy::RemoveLeader(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->RemoveLeader(luaext::EState{ L }.CheckEntity(2));
		return 0;
	}

	int UnlimitedArmy::IsIdle(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		L.Push(a->IsIdle());
		return 1;
	}
	int UnlimitedArmy::GetStatus(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		L.Push(static_cast<int>(a->Status));
		return 1;
	}

	int UnlimitedArmy::SetArea(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->Area = L.CheckFloat(2);
		return 0;
	}
	int UnlimitedArmy::SetTarget(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->Target = luaext::EState{ L }.CheckPos(2);
		return 0;
	}

	int UnlimitedArmy::DumpTable(lua::State Ls)
	{
		luaext::EState L{ Ls };
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);

		L.Push(typename_details::type_name<UnlimitedArmy>());

		CppLogic::Serializer::ObjectToLuaSerializer::Serialize(Ls, a, UnlimitedArmy::SerializationData);

		L.Push("Formation");
		L.Push(a->Formation);
		L.SetTableRaw(-3);
		L.Push("CommandQueue");
		L.Push(a->CommandQueue);
		L.SetTableRaw(-3);
		L.Push("Spawner");
		L.Push(a->Spawner);
		L.SetTableRaw(-3);

		return 2;
	}
	int UnlimitedArmy::ReadTable(lua::State L)
	{
		UnlimitedArmy* a = L.NewUserClass<UnlimitedArmy>(shok::PlayerId::P0); // player gets deserialized later

		L.PushValue(1);
		CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(L, a, UnlimitedArmy::SerializationData);
		L.Pop(1);

		a->L = L;
		L.Push("Formation");
		L.GetTableRaw(1);
		a->Formation = L.Ref();
		L.Push("CommandQueue");
		L.GetTableRaw(1);
		a->CommandQueue = L.Ref();
		L.Push("Spawner");
		L.GetTableRaw(1);
		a->Spawner = L.Ref();

		return 1;
	}

	int UnlimitedArmy::SetStatus(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->Status = static_cast<UAStatus>(L.CheckInt(2));
		return 0;
	}
	int UnlimitedArmy::SetReMove(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->ReMove = L.ToBoolean(2);
		return 0;
	}
	int UnlimitedArmy::SetCurrentBattleTarget(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->CurrentBattleTarget = luaext::EState{ L }.CheckEntity(2)->EntityId;
		return 0;
	}

	int UnlimitedArmy::GetRangedMelee(lua::State ls) {
		luaext::EState L{ ls };
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		L.NewTable();
		L.NewTable();
		L.NewTable();
		int ran = 1, mel = 1, nc = 1;
		for (auto id : a->Leaders) {
			L.Push(id);
			EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
			if (a->IsRanged(e)) {
				L.SetTableRaw(-4, ran);
				ran++;
			}
			else if (a->IsNonCombat(e)) {
				L.SetTableRaw(-2, nc);
				nc++;
			}
			else {
				L.SetTableRaw(-3, mel);
				mel++;
			}
		}
		return 3;
	}

	int UnlimitedArmy::SetIgnoreFleeing(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->IgnoreFleeing = L.OptBool(2, false);
		return 0;
	}
	int UnlimitedArmy::SetAutoRotateFormation(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->AutoRotateFormation = L.CheckFloat(2);
		return 0;
	}

	int UnlimitedArmy::GetFirstDeadHero(lua::State ls) {
		luaext::EState L{ ls };
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		if (a->DeadHeroes.size() > 0)
			L.Push(a->DeadHeroes[0]);
		else
			L.Push();
		return 1;
	}

	int UnlimitedArmy::SetPrepDefense(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->PrepDefense = L.OptBool(2, false);
		return 0;
	}
	int UnlimitedArmy::SetSabotageBridges(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->SabotageBridges = L.OptBool(2, false);
		return 0;
	}
	int CppLogic::UA::UnlimitedArmy::SetDoNotNormalizeSpeed(lua::State L)
	{
		UnlimitedArmy* a = L.CheckUserClass<UnlimitedArmy>(1);
		a->DoNotNormalizeSpeed = L.OptBool(2, false);
		a->NormalizeSpeed(a->Status == UAStatus::Moving || a->Status == UAStatus::MovingNoBattle, true);
		return 0;
	}

	int New(lua::State ls) {
		luaext::EState L{ ls };
		auto pl = L.CheckPlayerId(1);
		L.CheckType(2, lua::LType::Function);
		L.CheckType(3, lua::LType::Function);
		L.CheckType(4, lua::LType::Function);
		UnlimitedArmy* a = L.NewUserClass<UnlimitedArmy>(pl);
		a->L = L;
		L.PushValue(2);
		a->Formation = L.Ref(L.REGISTRYINDEX);
		L.PushValue(3);
		a->CommandQueue = L.Ref(L.REGISTRYINDEX);
		L.PushValue(4);
		a->Spawner = L.Ref(L.REGISTRYINDEX);
		a->RNG.seed(L.CheckInt(5));
		return 1;
	}

	int GetNearestEnemyInArea(lua::State Ls) {
		luaext::EState L{ Ls };
		auto pl = L.CheckPlayerId(1);
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
		auto pl = L.CheckPlayerId(1);
		shok::Position p = L.CheckPos(2);
		float r = L.CheckFloat(3);
		L.Push(UnlimitedArmy::CountTargetsInArea(pl, p, r, L.ToBoolean(4)));
		return 1;
	}

	int AddCannonBuilderData(lua::State ls) {
		luaext::EState L{ ls };
		UACannonBuilderAbilityData d = { L.CheckEnum<shok::EntityTypeId>(1), L.CheckEnum<shok::EntityTypeId>(2), L.CheckEnum<shok::EntityTypeId>(3) };
		UnlimitedArmy::CannonBuilderAbilityData.emplace_back(d);
		return 0;
	}

	void CppLogic::UA::UnlimitedArmy::RegisterUDType(lua::State L)
	{
		L.PrepareUserClassType<UnlimitedArmy>();
		CppLogic::Serializer::AdvLuaStateSerializer::UserdataDeserializer[std::string{ typename_details::type_name<UnlimitedArmy>() }] = &lua::State::CppToCFunction<ReadTable>;
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

		UnlimitedArmy::RegisterUDType(L);
		UnlimitedArmy::CannonBuilderAbilityData.clear();
	}
}
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.UA.GetNearestEnemyInArea(1, x,y, 1000)

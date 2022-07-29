#include "pch.h"
#include "l_entity.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entity.h"
#include "s5_events.h"
#include "s5_defines.h"
#include "s5_entitytype.h"
#include "s5_behaviors.h"
#include "s5_behaviorProps.h"
#include "s5_player.h"
#include "s5_scriptsystem.h"
#include "s5_maplogic.h"
#include "s5_tasklist.h"
#include "s5_idmanager.h"
#include "s5_classfactory.h"
#include "hooks.h"
#include "luaext.h"
#include "entityiterator.h"
#include "modloader.h"

namespace CppLogic::Entity {
	int PredicateOfType(lua::State L) {
		int ty = L.CheckInt(1);
		L.NewUserData<CppLogic::Iterator::EntityPredicateOfType>(ty);
		return 1;
	}

	int PredicateOfPlayer(lua::State L) {
		int pl = L.CheckInt(1);
		L.NewUserData<CppLogic::Iterator::EntityPredicateOfPlayer>(pl);
		return 1;
	}

	int PredicateInCircle(lua::State L) {
		float x = L.CheckFloat(1);
		float y = L.CheckFloat(2);
		float r = L.CheckFloat(3);
		L.NewUserData<CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity>>(shok::Position{ x,y }, r * r);
		return 1;
	}

	int PredicateOfAnyPlayer(lua::State L) {
		int num = L.GetTop();
		if (num > 9)
			throw lua::LuaException("too many players to check");
		auto* p = L.NewUserData<CppLogic::Iterator::EntityPredicateOfAnyPlayer>();
		for (int i = 0; i < num; ++i) {
			p->players[i] = L.CheckInt(i + 1);
		}
		return 1;
	}

	int PredicateOfAnyEntityType(lua::State L) {
		int num = L.GetTop();
		auto* p = L.NewUserData<CppLogic::Iterator::EntityPredicateOfAnyType>();
		p->entityTypes.reserve(num);
		for (int i = 1; i <= num; ++i)
			p->entityTypes.push_back(L.CheckInt(i));
		return 1;
	}

	int PredicateAnd(lua::State L) {
		int num = L.GetTop();
		auto* p = L.NewUserData<CppLogic::Iterator::PredicateDynamicAnd<EGL::CGLEEntity>>();
		p->preds.reserve(num);
		L.Insert(1);
		L.NewTable();
		L.Insert(2);
		for (int i = num - 1; i >= 0; i--) { // keep predicates, so they dont get gced
			p->preds.push_back(L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(-1));
			L.SetTableRaw(2, i);
		}
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateOr(lua::State L) {
		int num = L.GetTop();
		auto* p = L.NewUserData<CppLogic::Iterator::PredicateDynamicOr<EGL::CGLEEntity>>();
		p->preds.reserve(num);
		L.Insert(1);
		L.NewTable();
		L.Insert(2);
		for (int i = num - 1; i >= 0; i--) { // keep predicates, so they dont get gced
			p->preds.push_back(L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(-1));
			L.SetTableRaw(2, i);
		}
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateNot(lua::State L) {
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		auto* p = L.NewUserData<CppLogic::Iterator::PredicateNot<EGL::CGLEEntity>>(pred);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateSetPriority(lua::State L) {
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		int pri = L.CheckInt(2);
		auto* p = L.NewUserData<CppLogic::Iterator::PredicatePriority<EGL::CGLEEntity>>(pred, pri);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateIsSettler(lua::State L) {
		L.NewUserData<CppLogic::Iterator::EntityPredicateIsSettler>();
		return 1;
	}

	int PredicateIsBuilding(lua::State L) {
		L.NewUserData<CppLogic::Iterator::EntityPredicateIsBuilding>();
		return 1;
	}

	int PredicateIsCombatRelevant(lua::State L) {
		L.NewUserData<CppLogic::Iterator::EntityPredicateIsCombatRelevant>();
		return 1;
	}
	int PredicateIsNotSoldier(lua::State L) {
		L.NewUserData<CppLogic::Iterator::EntityPredicateIsNotSoldier>();
		return 1;
	}

	int PredicateOfEntityCategory(lua::State L) {
		int c = L.CheckInt(1);
		L.NewUserData<CppLogic::Iterator::EntityPredicateOfEntityCategory>(static_cast<shok::EntityCategory>(c));
		return 1;
	}

	int PredicateProvidesResource(lua::State L) {
		int ty = L.CheckInt(1);
		L.NewUserData<CppLogic::Iterator::EntityPredicateProvidesResource>(static_cast<shok::ResourceType>(ty));
		return 1;
	}

	int PredicateInRect(lua::State L) {
		float x1 = L.CheckFloat(1);
		float y1 = L.CheckFloat(2);
		float x2 = L.CheckFloat(3);
		float y2 = L.CheckFloat(4);
		L.NewUserData<CppLogic::Iterator::PredicateInRect<EGL::CGLEEntity>>(x1, y1, x2, y2);
		return 1;
	}

	int PredicateIsVisible(lua::State L) {
		L.NewUserData<CppLogic::Iterator::EntityPredicateIsVisible>();
		return 1;
	}

	int PredicateOfUpgradeCategory(lua::State L) {
		int ty = L.CheckInt(1);
		L.NewUserData<CppLogic::Iterator::EntityPredicateOfUpgradeCategory>(ty);
		return 1;
	}

	int PredicateIsAlive(lua::State L) {
		L.NewUserData<CppLogic::Iterator::EntityPredicateIsAlive>();
		return 1;
	}

	int PredicateIsNotInBuilding(lua::State L) {
		L.NewUserData<CppLogic::Iterator::EntityPredicateIsNotInBuilding>();
		return 1;
	}

	int EntityIteratorTableize(lua::State L) {
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAnd(L);
		}
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		int index = 1;
		L.NewTable();
		CppLogic::Iterator::GlobalEntityIterator it{ pred };
		for (EGL::CGLEEntity* e : it) {
			L.Push(e->EntityId);
			L.SetTableRaw(2, index);
			index++;
		}
		return 1;
	}

	int EntityIteratorCount(lua::State L) {
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAnd(L);
		}
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		int count = 0;
		CppLogic::Iterator::GlobalEntityIterator it{ pred };
		for (EGL::CGLEEntity* e : it) {
			++count;
		}
		L.Push(count);
		return 1;
	}

	int EntityIteratorGetNearest(lua::State L) {
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAnd(L);
		}
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		CppLogic::Iterator::GlobalEntityIterator it{ pred };
		float maxR = -1;
		EGL::CGLEEntity* e = it.GetNearest(&maxR);
		L.Push(e == nullptr ? 0 : e->EntityId);
		L.Push(maxR);
		return 2;
	}

	int EntityIteratorNext(lua::State L) { // (state nil, last value) -> next value
		// no error checking here, cause that would cost speed
		// just expect no other c/c++ code will change the funcs upvalue
		// make sure ManagedIterator is the first class inherited from the actual iterator and we do not use luapp inheritance here
		// (both would break this simplified pointer)
		auto* it = static_cast<CppLogic::Iterator::ManagedIterator<EGL::CGLEEntity>*>(L.ToUserdata(L.Upvalueindex(1)));
		float r = -1;
		int p = -1;
		EGL::CGLEEntity* e = it->GetNext(&r, &p);
		if (e == nullptr) {
			L.Push();
			L.Push();
			L.Push();
		}
		else {
			L.Push(e->EntityId);
			L.Push(r);
			L.Push(p);
		}
		return 3;
	}
	int LEntityIterator(lua::State L) {
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAnd(L);
		}
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);

		L.NewUserData<CppLogic::Iterator::GlobalEntityIterator>(pred); // upvalue of func
		L.Push<EntityIteratorNext>(1); // func
		L.Push(); // state
		L.Push(); // initial value
		return 3;
	}

	int PlayerEntityIterator(lua::State L) {
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		if (L.GetTop() == 2) {
			int pl = L.CheckInt(2);
			L.NewUserData<CppLogic::Iterator::PlayerEntityIterator>(pl, pred); // upvalue of func
		}
		else {
			int num = L.GetTop() - 1;
			if (num > 8)
				throw lua::LuaException("too many players");
			auto* it = L.NewUserData<CppLogic::Iterator::MultiPlayerEntityIterator>(pred); // upvalue of func
			for (int i = 0; i < num; ++i) {
				it->Players[i] = L.CheckInt(i + 2);
			}
		}
		L.Push<EntityIteratorNext>(1); // func
		L.Push(); // state
		L.Push(); // initial value
		return 3;
	}

	int CheckPredicate(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* s = L.CheckEntity(1);
		auto* pred = L.GetUserData<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		float r = -1;
		int pr = -1;
		L.Push(pred->Matches(s, &r, &pr));
		L.Push(r);
		L.Push(pr);
		return 3;
	}

	int SettlerGetLeaderOfSoldier(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		L.Push(s->GetFirstAttachedToMe(shok::AttachmentType::LEADER_SOLDIER));
		return 1;
	}

	int LeaderGetExperience(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		L.Push(b->Experience);
		return 1;
	}
	int LeaderSetExperience(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		b->Experience = L.CheckInt(2);
		return 0;
	}

	int LeaderGetTroopHealth(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		L.Push(b->GetTroopHealth());
		L.Push(b->GetTroopHealthPerSoldier());
		return 2;
	}
	int LeaderSetTroopHealth(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLeaderBehavior* b = s->GetBehavior<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		b->TroopHealthCurrent = L.CheckInt(2);
		return 0;
	}

	int SettlerGetBaseMovementSpeed(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CBehaviorDefaultMovement* b = s->GetBehavior<GGL::CBehaviorDefaultMovement>();
		if (!b)
			throw lua::LuaException("no moving entity at 1");
		L.Push(b->MovementSpeed);
		L.Push(CppLogic::RadiansToDegrees(b->TurningSpeed));
		return 2;
	}
	int SettlerSetBaseMovementSpeed(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CBehaviorDefaultMovement* b = s->GetBehavior<GGL::CBehaviorDefaultMovement>();
		if (!b)
			throw lua::LuaException("no moving entity at 1");
		if (L.IsNumber(2))
			b->MovementSpeed = L.CheckFloat(2);
		if (L.IsNumber(3))
			b->TurningSpeed = static_cast<float>(CppLogic::DegreesToRadians(L.ToNumber(3)));
		return 0;
	}

	int BuildingGetBarracksAutoFillActive(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* e = L.CheckBuilding(1);
		GGL::CBarrackBehavior* b = e->GetBehavior<GGL::CBarrackBehavior>();
		if (!b)
			throw lua::LuaException("no barracks at 1");
		L.Push(b->AutoFillActive);
		return 1;
	}

	int GetScale(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* s = L.CheckEntity(1);
		L.Push(s->Scale);
		return 1;
	}
	int SetScale(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* s = L.CheckEntity(1);
		s->Scale = L.CheckFloat(2);
		return 0;
	}

	int BuildingGetHeight(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		L.Push(b->BuildingHeight);
		return 1;
	}
	int BuildingSetHeight(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		b->BuildingHeight = L.CheckFloat(2);
		return 0;
	}

	int SettlerGetOverheadWidget(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		s->ModifierProfile.EntityReference.CheckInit();
		L.Push(s->ModifierProfile.EntityReference.OverheadWidget);
		return 1;
	}
	int SettlerSetOverheadWidget(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		int ov = L.CheckInt(2);
		if (!(ov >= 0 && ov <= 4))
			throw lua::LuaException("invalid overhead code");
		s->ModifierProfile.EntityReference.CheckInit();
		s->ModifierProfile.EntityReference.OverheadWidget = ov;
		return 1;
	}

	int MovingEntityGetTargetPos(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		EGL::CMovingEntity* m = dynamic_cast<EGL::CMovingEntity*>(e);
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		L.PushPos(m->TargetPosition);
		if (m->TargetRotationValid) {
			L.Push("r");
			L.Push(CppLogic::RadiansToDegrees(m->TargetRotation));
			L.SetTableRaw(-3);
		}
		return 1;
	}
	int MovingEntitySetTargetPos(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		EGL::CMovingEntity* m = dynamic_cast<EGL::CMovingEntity*>(e);
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		m->TargetPosition = L.CheckPos(2);
		L.Push("r");
		L.GetTableRaw(2);
		if (L.IsNumber(-1)) {
			m->SetTargetRotation(static_cast<float>(CppLogic::DegreesToRadians(L.ToNumber(-1))));
			EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, true };
			m->FireEvent(&ev);
		}
		else {
			m->TargetRotationValid = 0;
			EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, false };
			m->FireEvent(&ev);
		}
		return 0;
	}

	int HeroGetCamouflageDurationLeft(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CCamouflageBehavior* c = s->GetBehavior<GGL::CCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no camo hero at 1");
		L.Push(c->InvisibilityRemaining);
		return 1;
	}
	int HeroSetCamouflageDurationLeft(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CCamouflageBehavior* c = s->GetBehavior<GGL::CCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no camo hero at 1");
		if (dynamic_cast<GGL::CThiefCamouflageBehavior*>(c))
			throw lua::LuaException("thief at 1, use ThiefSetCamouflageTimeTo instead");
		c->InvisibilityRemaining = L.CheckInt(2);
		return 1;
	}

	int ThiefGetCamouflageTimeTo(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CThiefCamouflageBehavior* c = s->GetBehavior<GGL::CThiefCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no thief at 1");
		L.Push(c->TimeToInvisibility);
		return 1;
	}
	int ThiefSetCamouflageTimeTo(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CThiefCamouflageBehavior* c = s->GetBehavior<GGL::CThiefCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no thief at 1");
		int i = L.CheckInt(2);
		c->TimeToInvisibility = i;
		c->InvisibilityRemaining = i <= 0 ? 15 : 0;
		return 0;
	}

	int HeroGetResurrectionTime(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CHeroBehavior* h = s->GetBehavior<GGL::CHeroBehavior>();
		if (!h)
			throw lua::LuaException("no hero at 1");
		L.Push(h->ResurrectionTimePassed);
		return 1;
	}
	int HeroSetResurrectionTime(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CHeroBehavior* h = s->GetBehavior<GGL::CHeroBehavior>();
		if (!h)
			throw lua::LuaException("no hero at 1");
		h->ResurrectionTimePassed = L.CheckInt(2);
		return 0;
	}

	int GetLimitedLifespanRemaining(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CLimitedLifespanBehavior* b = e->GetBehavior<GGL::CLimitedLifespanBehavior>();
		if (!b)
			throw lua::LuaException("no limited lifespan at 1");
		L.Push(b->RemainingLifespanSeconds);
		return 1;
	}
	int SetLimitedLifespanRemaining(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CLimitedLifespanBehavior* b = e->GetBehavior<GGL::CLimitedLifespanBehavior>();
		if (!b)
			throw lua::LuaException("no limited lifespan at 1");
		b->RemainingLifespanSeconds = L.CheckInt(2);
		return 0;
	}

	int GetTaskListIndex(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->CurrentTaskIndex);
		return 1;
	}
	int SetTaskListIndex(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		e->CurrentTaskIndex = L.CheckInt(2);
		return 0;
	}

	int MovingEntityGetSpeedFactor(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CBehaviorDefaultMovement* m = e->GetBehavior<GGL::CBehaviorDefaultMovement>();
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		L.Push(m->SpeedFactor);
		return 1;
	}

	int WorkerGetCurrentWorkTime(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CWorkerBehavior* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		L.Push(w->WorkTimeRemaining);
		return 1;
	}
	int WorkerSetCurrentWorkTime(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CWorkerBehavior* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		w->WorkTimeRemaining = L.CheckInt(2);
		return 0;
	}

	int BuildingMarketGetCurrentTradeData(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CMarketBehavior* m = b->GetBehavior<GGL::CMarketBehavior>();
		if (!m)
			throw lua::LuaException("no market at 1");
		L.Push(static_cast<int>(m->CurrentTrade.BuyResourceType));
		L.Push(static_cast<int>(m->CurrentTrade.SellResourceType));
		L.Push(m->CurrentTrade.BuyAmount);
		L.Push(m->CurrentTrade.SellAmount);
		L.Push(m->CurrentTrade.ProgressAmount);
		return 5;
	}
	int BuildingMarketSetCurrentTradeData(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CMarketBehavior* m = b->GetBehavior<GGL::CMarketBehavior>();
		if (!m)
			throw lua::LuaException("no market at 1");
		if (L.IsNumber(2))
			m->CurrentTrade.BuyResourceType = static_cast<shok::ResourceType>(L.CheckInt(2));
		if (L.IsNumber(3))
			m->CurrentTrade.SellResourceType = static_cast<shok::ResourceType>(L.CheckInt(3));
		if (L.IsNumber(4))
			m->CurrentTrade.BuyAmount = L.CheckFloat(4);
		if (L.IsNumber(5))
			m->CurrentTrade.SellAmount = L.CheckFloat(5);
		if (L.IsNumber(6))
			m->CurrentTrade.ProgressAmount = L.CheckFloat(6);
		return 0;
	}

	int IsSoldier(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetBehavior<GGL::CSoldierBehavior>() != nullptr);
		return 1;
	}

	int HeroResurrect(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CHeroBehavior* h = e->GetBehavior<GGL::CHeroBehavior>();
		if (!h)
			throw lua::LuaException("no hero at 1");
		h->ResurrectionTimePassed = 20000;
		h->EnemyNear = 0;
		h->FriendNear = 1;
		return 0;
	}

	int ThiefSetStolenResourceInfo(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CThiefBehavior* t = e->GetBehavior<GGL::CThiefBehavior>();
		if (!t)
			throw lua::LuaException("no thief at 1");
		int ty = L.CheckInt(2);
		int am;
		if (ty == 0)
			am = 0;
		else
			am = L.CheckInt(3);
		t->ResourceType = ty;
		t->Amount = am;
		if (ty == 0)
			t->StolenFromPlayer = 0;
		else if (L.IsNumber(4))
			t->StolenFromPlayer = L.CheckInt(4); // todo: check for func that refreshes model
		return 0;
	}

	int GetAutoAttackMaxRange(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CBattleBehavior* b = e->GetBehavior<GGL::CBattleBehavior>();
		if (b) {
			L.Push(b->GetMaxRange());
			return 1;
		}
		GGL::CAutoCannonBehavior* a = e->GetBehavior<GGL::CAutoCannonBehavior>();
		if (a) {
			L.Push(a->GetMaxRange());
			return 1;
		}
		throw lua::LuaException("no battle entity or autocannon at 1");
	}

	int GetModel(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		int m = e->ModelOverride;
		if (m == 0) {
			m = e->GetEntityType()->DisplayProps->Model[0];
		}
		L.Push(m);
		return 1;
	}

	int GetExploration(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetExploration());
		return 1;
	}

	int GetSpeed(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CBehaviorDefaultMovement* m = e->GetBehavior<GGL::CBehaviorDefaultMovement>();
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		L.Push(m->GetMovementSpeed() * 10.0);
		return 1;
	}

	int SettlerIsVisible(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CCamouflageBehavior* c = e->GetBehavior<GGL::CCamouflageBehavior>();
		if (c != nullptr) {
			L.Push(c->InvisibilityRemaining <= 0);
			return 1;
		}
		L.Push(false);
		return 1;
	}

	int SettlerCommandSendHawk(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CHeroHawkBehavior* b = e->GetBehavior<GGL::CHeroHawkBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CHeroHawkBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CHeroHawkBehaviorProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		shok::Position p = L.CheckPos(2);
		e->HeroAbilitySendHawk(p);
		return 0;
	}

	int SettlerGetHawkOfHero(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		GGL::CHeroHawkBehavior* b = e->GetBehavior<GGL::CHeroHawkBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::HERO_HAWK));
		return 1;
	}

	int SettlerCommandInflictFear(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CInflictFearAbility* b = e->GetBehavior<GGL::CInflictFearAbility>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CInflictFearAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CInflictFearAbilityProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		e->HeroAbilityInflictFear();
		return 0;
	}

	int SettlerCommandPlaceBomb(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CBombPlacerBehavior* b = e->GetBehavior<GGL::CBombPlacerBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CHeroAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CHeroAbilityProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		shok::Position p = L.CheckPos(2);
		e->HeroAbilityPlaceBomb(p);
		return 0;
	}

	int SettlerCommandPlaceCannon(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CCannonBuilderBehavior* b = e->GetBehavior<GGL::CCannonBuilderBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CCannonBuilderBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CCannonBuilderBehaviorProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		shok::Position p = L.CheckPos(2);
		p.FloorToBuildingPlacement();
		int bottom = L.CheckInt(3);
		GGlue::CGlueEntityProps* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(bottom);
		if (!ety)
			throw lua::LuaException("no bottom entitytype");
		if (!ety->IsBuildingType())
			throw lua::LuaException("bottom not a building");
		int top = L.CheckInt(4);
		if (!(*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(top))
			throw lua::LuaException("no top entitytype");
		if (!GGL::CPlayerStatus::CanPlaceBuilding(bottom, e->PlayerId, &p, 0, 0))
			throw lua::LuaException("cannot place foundation at that position");
		e->HeroAbilityPlaceCannon(p, bottom, top);
		return 0;
	}

	int SettlerCommandRangedEffect(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CRangedEffectAbility* b = e->GetBehavior<GGL::CRangedEffectAbility>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CRangedEffectAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		e->HeroAbilityRangedEffect();
		return 0;
	}

	int SettlerCommandCircularAttack(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CCircularAttack* b = e->GetBehavior<GGL::CCircularAttack>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CCircularAttackProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CCircularAttackProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		e->HeroAbilityCircularAttack();
		return 0;
	}

	int SettlerCommandSummon(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CSummonBehavior* b = e->GetBehavior<GGL::CSummonBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CSummonBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSummonBehaviorProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		e->HeroAbilitySummon();
		int summoned = 0;
		for (const auto& a : e->ObserverEntities) {
			if (a.first == shok::AttachmentType::SUMMONER_SUMMONED) {
				L.Push(a.second.EntityId);
				summoned++;
			}
		}
		return summoned;
	}

	int SettlerCommandActivateCamoflage(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CCamouflageBehavior* b = e->GetBehavior<GGL::CCamouflageBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		if (dynamic_cast<GGL::CThiefCamouflageBehavior*>(b))
			throw lua::LuaException("thief camo cannot be manually activated");
		GGL::CCamouflageBehaviorProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		e->HeroAbilityActivateCamoflage();
		return 0;
	}

	int SettlerCommandConvert(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CConvertSettlerAbility* b = e->GetBehavior<GGL::CConvertSettlerAbility>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CConvertSettlerAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CConvertSettlerAbilityProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		GGL::CSettler* t = L.CheckSettler(2);
		L.CheckEntityAlive(t->EntityId, "entity dead at 2");
		if (!GGL::CPlayerStatus::ArePlayersHostile(e->PlayerId, t->PlayerId))
			throw lua::LuaException("entities are not hostile");
		e->HeroAbilityConvert(t->EntityId);
		return 0;
	}

	int SettlerCommandSnipe(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CSniperAbility* b = e->GetBehavior<GGL::CSniperAbility>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CSniperAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		GGL::CSettler* t = L.CheckSettler(2);
		L.CheckEntityAlive(t->EntityId, "entity dead at 2");
		if (!GGL::CPlayerStatus::ArePlayersHostile(e->PlayerId, t->PlayerId))
			throw lua::LuaException("entities are not hostile");
		if (!e->Position.IsInRange(t->Position, bp->Range))
			throw lua::LuaException("target not in range");
		e->HeroAbilitySnipe(t->EntityId);
		return 0;
	}

	int SettlerCommandShuriken(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CShurikenAbility* b = e->GetBehavior<GGL::CShurikenAbility>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CShurikenAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CShurikenAbilityProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		GGL::CSettler* t = L.CheckSettler(2);
		L.CheckEntityAlive(t->EntityId, "entity dead at 2");
		if (!GGL::CPlayerStatus::ArePlayersHostile(e->PlayerId, t->PlayerId))
			throw lua::LuaException("entities are not hostile");
		if (!e->Position.IsInRange(t->Position, bp->Range))
			throw lua::LuaException("target not in range");
		e->HeroAbilityShuriken(t->EntityId);
		return 0;
	}

	int SettlerCommandMotivateWorkers(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CMotivateWorkersAbility* b = e->GetBehavior<GGL::CMotivateWorkersAbility>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CMotivateWorkersAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CMotivateWorkersAbilityProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		e->HeroAbilityMotivateWorkers();
		return 0;
	}

	int SettlerCommandSabotage(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CKegPlacerBehavior* b = e->GetBehavior<GGL::CKegPlacerBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CKegPlacerBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<GGL::CKegPlacerBehaviorProperties>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		if (!(e->GetBehavior<GGL::CThiefBehavior>()->ResourceType == 0))
			throw lua::LuaException("is carrying resources");
		GGL::CBuilding* t = L.CheckBuilding(2);
		L.CheckEntityAlive(t->EntityId, "entity dead at 2");
		if (!(t->IsEntityInCategory(shok::EntityCategory::Bridge) || GGL::CPlayerStatus::ArePlayersHostile(e->PlayerId, t->PlayerId)))
			throw lua::LuaException("entities are not hostile or bridge");
		e->ThiefSabotage(t->EntityId);
		return 0;
	}

	int SettlerCommandDefuse(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CKegPlacerBehavior* b = e->GetBehavior<GGL::CKegPlacerBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		if (!(e->GetBehavior<GGL::CThiefBehavior>()->ResourceType == 0))
			throw lua::LuaException("is carrying resources");
		EGL::CGLEEntity* t = L.CheckEntity(2);
		if (!t->GetBehavior<GGL::CKegBehavior>())
			throw lua::LuaException("no keg at 2");
		e->ThiefDefuse(t->EntityId);
		return 0;
	}

	int SettlerCommandBinocular(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CAbilityScoutBinocular* b = e->GetBehavior<GGL::CAbilityScoutBinocular>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CAbilityScoutBinocularProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CAbilityScoutBinocularProps>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		shok::Position p = L.CheckPos(2);
		e->ScoutBinoculars(p);
		return 0;
	}

	int SettlerCommandPlaceTorch(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CTorchPlacerBehavior* b = e->GetBehavior<GGL::CTorchPlacerBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CTorchPlacerBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<GGL::CTorchPlacerBehaviorProperties>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		shok::Position p = L.CheckPos(2);
		e->ScoutPlaceTorch(p);
		return 0;
	}

	int SettlerCommandPointToRes(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CPointToResourceBehavior* b = e->GetBehavior<GGL::CPointToResourceBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CPointToResourceBehaviorProperties* bp = e->GetEntityType()->GetBehaviorProps<GGL::CPointToResourceBehaviorProperties>();
		if (!(b->SecondsCharged >= bp->RechargeTimeSeconds))
			throw lua::LuaException("ability not ready at 1");
		e->ScoutFindResource();
		return 0;
	}

	int SettlerCommandStealFrom(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CThiefBehavior* b = e->GetBehavior<GGL::CThiefBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		GGL::CBuilding* t = L.CheckBuilding(2);
		L.CheckEntityAlive(t->EntityId, "entity dead at 2");
		if (!GGL::CPlayerStatus::ArePlayersHostile(e->PlayerId, t->PlayerId))
			throw lua::LuaException("entities are not hostile");
		e->ThiefStealFrom(t->EntityId);
		return 0;
	}

	int SettlerCommandSecureGoods(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		GGL::CThiefBehavior* b = e->GetBehavior<GGL::CThiefBehavior>();
		if (!b)
			throw lua::LuaException("no matching ability at 1");
		if (!(b->ResourceType != 0))
			throw lua::LuaException("no resources carried");
		GGL::CBuilding* t = L.CheckBuilding(2);
		L.CheckEntityAlive(t->EntityId, "entity dead at 2");
		if (e->PlayerId != t->PlayerId)
			throw lua::LuaException("entities are not of same player");
		if (!t->IsEntityInCategory(shok::EntityCategory::Headquarters))
			throw lua::LuaException("target not hq");
		e->ThiefSecureGoods(t->EntityId);
		return 0;
	}

	int EnableConversionHook(lua::State L) {
		if (!L.IsFunction(1))
			throw lua::LuaException("no func");
		L.PushLightUserdata(&EnableConversionHook);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);
		EGL::CGLEEntity::HookHero6Convert();
		EGL::CGLEEntity::Hero6ConvertHookCb = [](int id, int pl, int nid, int converter) {
			lua::State L {*EScr::CScriptTriggerSystem::GameState};
			int t = L.GetTop();
			L.PushLightUserdata(&EnableConversionHook);
			L.GetTableRaw(L.REGISTRYINDEX);
			L.Push(id);
			L.Push(pl);
			L.Push(nid);
			L.Push(converter);
			L.PCall(4, 0, 0);
			L.SetTop(t);
		};
		return 0;
	}
	int DisableConversionHook(lua::State L) {
		EGL::CGLEEntity::Hero6ConvertHookCb = nullptr;
		return 0;
	}

	int IsFeared(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		int id = e->GetFirstAttachedToMe(shok::AttachmentType::INFLICTOR_TERRORIZED);
		if (id == 0)
			L.Push(false);
		else
			L.Push(id);
		return 1;
	}

	int SettlerCommandMove(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		shok::Position p = L.CheckPos(2);
		EGL::CGLELandscape* ls = (*EGL::CGLEGameLogic::GlobalObj)->Landscape;
		if (ls->GetSector(&p) == 0) {
			shok::Position pou;
			if (ls->GetNearestPositionInSector(&p, 1000, e->GetSector(), &pou))
				p = pou;
		}
		e->Move(p);
		if (L.IsNumber(3)) {
			e->SetTargetRotation(static_cast<float>(CppLogic::DegreesToRadians(L.ToNumber(3))));
			EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, true };
			e->FireEvent(&ev);
		}
		else {
			e->TargetRotationValid = false;
			EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, false };
			e->FireEvent(&ev);
		}
		return 0;
	}

	int ClearAttackers(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		e->ClearAttackers();
		return 0;
	}

	int BuildingCommandFoundryBuildCannon(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->GetBehavior<GGL::CFoundryBehavior>())
			throw lua::LuaException("no foundry at 1");
		if (!b->IsIdle())
			throw lua::LuaException("is not idle");
		GGlue::CGlueEntityProps* t = L.CheckEntityType(2);
		int ety = L.CheckInt(2);
		bool found = false;
		for (GGL::CFoundryBehaviorProperties::CannonInfoData& i : b->GetEntityType()->GetBehaviorProps<GGL::CFoundryBehaviorProperties>()->CannonInfo) {
			if (i.Cannon == ety)
				found = true;
		}
		if (!found)
			throw lua::LuaException("foundry cannot build entitytype");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
		if (!p->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(t->LogicProps)->Cost))
			throw lua::LuaException("missing res");
		if (p->PlayerAttractionHandler->GetAttractionUsage() >= p->PlayerAttractionHandler->GetAttractionLimit())
			throw lua::LuaException("pop capped");
		b->CommandBuildCannon(ety);
		return 0;
	}

	int SettlerIsIdle(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.Push(e->IsIdle());
		return 1;
	}

	int BuildingGetNearestFreeConstructionSlotFor(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		shok::Position p = L.CheckPos(2);
		L.Push(b->GetNearestFreeConstructionSlotFor(&p));
		return 1;
	}

	int SettlerCommandSerfConstructBuilding(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CBuilding* b = L.CheckBuilding(2);
		if (!s->GetBehavior<GGL::CSerfBehavior>())
			throw lua::LuaException("no serf");
		if (b->IsConstructionFinished())
			throw lua::LuaException("no construction site");
		bool suc = s->SerfConstructBuilding(b);
		L.Push(suc);
		return 1;
	}

	int BuildingGetNearestFreeRepairSlotFor(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		shok::Position p = L.CheckPos(2);
		L.Push(b->GetNearestFreeRepairSlotFor(&p));
		return 1;
	}

	int SettlerCommandSerfRepairBuilding(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CBuilding* b = L.CheckBuilding(2);
		if (!s->GetBehavior<GGL::CSerfBehavior>())
			throw lua::LuaException("no serf");
		if (!b->IsConstructionFinished())
			throw lua::LuaException("construction site");
		if (b->Health >= b->GetMaxHealth())
			throw lua::LuaException("at full health");
		bool suc = s->SerfRepairBuilding(b);
		L.Push(suc);
		return 1;
	}

	int ReplaceWithResourceEntity(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		e = EGL::CGLEEntity::ReplaceEntityWithResourceEntity(e);
		if (e)
			L.Push(e->EntityId);
		else
			L.Push();
		return 1;
	}

	int SettlerCommandSerfExtract(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		EGL::CGLEEntity* b = L.CheckEntity(2);
		if (!s->GetBehavior<GGL::CSerfBehavior>())
			throw lua::LuaException("no serf");
		if (!dynamic_cast<GGL::CResourceDoodad*>(b))
			b = EGL::CGLEEntity::ReplaceEntityWithResourceEntity(b);
		if (!dynamic_cast<GGL::CResourceDoodad*>(b))
			throw lua::LuaException("no resource entity");
		s->SerfExtractResource(b->EntityId);
		L.Push(b->EntityId);
		return 1;
	}

	int BuildingStartUpgrade(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsIdle())
			throw lua::LuaException("is not idle");
		b->StartUpgrade();
		return 0;
	}

	int BuildingCancelUpgrade(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsUpgrading)
			throw lua::LuaException("not upgrading");
		b->CancelUpgrade();
		return 0;
	}

	int BuildingIsIdle(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		L.Push(b->IsIdle());
		return 1;
	}

	int BarracksGetLeadersTrainingAt(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->GetBehavior<GGL::CBarrackBehavior>())
			throw lua::LuaException("no barracks");
		int i = 0;
		for (const auto& a : b->ObserverEntities) {
			if (a.first == shok::AttachmentType::FIGHTER_BARRACKS && !EGL::CGLEEntity::GetEntityByID(a.second.EntityId)->GetBehavior<GGL::CSoldierBehavior>()) {
				L.Push(a.second.EntityId);
				i++;
			}
		}
		return i;
	}

	int FoundryGetCannonTypeInConstruction(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CFoundryBehavior* f = b->GetBehavior<GGL::CFoundryBehavior>();
		if (!f)
			throw lua::LuaException("no foundry");
		L.Push(f->CannonType);
		return 1;
	}

	int SettlerCommandExpell(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		s->SettlerExpell();
		return 0;
	}

	int BarracksBuySoldierForLeader(lua::State ls) {
		luaext::EState L{ ls };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CSettler* s = L.CheckSettler(2);
		if (!b->GetBehavior<GGL::CBarrackBehavior>())
			throw lua::LuaException("no barracks");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading))
			throw lua::LuaException("barracks is upgrading or under construction");
		GGL::CLeaderBehavior* l = s->GetBehavior<GGL::CLeaderBehavior>();
		GGL::CLeaderBehaviorProps* lp = s->GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!l)
			throw lua::LuaException("no leader");
		if (b->PlayerId != s->PlayerId)
			throw lua::LuaException("different players");
		GGlue::CGlueEntityProps* solty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(lp->SoldierType);
		if (!solty)
			throw lua::LuaException("no soldier type set");
		if (!L.ToBoolean(3)) {
			int ucat = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(1)->BuildingUpgradeManager->GetUpgradeCategoryOfEntityType(b->EntityType);
			if (lp->BarrackUpgradeCategory != ucat)
				throw lua::LuaException("leader type doesnt match barracks type");
		}
		int max = s->LimitedAttachmentGetMaximum(shok::AttachmentType::LEADER_SOLDIER);
		int curr = 0;
		for (const auto& a : s->ObservedEntities) {
			if (a.first == shok::AttachmentType::LEADER_SOLDIER)
				curr++;
		}
		if (curr >= max)
			throw lua::LuaException("no free soldier slot left");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(s->PlayerId);
		if (p->PlayerAttractionHandler->GetAttractionUsage() >= p->PlayerAttractionHandler->GetAttractionLimit())
			throw lua::LuaException("pop capped");
		GGL::CGLSettlerProps* sprop = dynamic_cast<GGL::CGLSettlerProps*>(solty->LogicProps);
		if (!sprop)
			throw lua::LuaException("error: soldier no settler type");
		if (!p->CurrentResources.HasResources(&sprop->Cost))
			throw lua::LuaException("missing res");
		b->CommandRecruitSoldierForLeader(s->EntityId);
		return 0;
	}

	int LeaderAttachSoldier(lua::State ls) {
		luaext::EState L{ ls };
		GGL::CSettler* l = L.CheckSettler(1);
		GGL::CSettler* s = L.CheckSettler(2);
		GGL::CLeaderBehavior* lb = l->GetBehavior<GGL::CLeaderBehavior>();
		if (!lb)
			throw lua::LuaException("no leader");
		if (!s->GetBehavior<GGL::CSoldierBehavior>())
			throw lua::LuaException("no soldier");
		if (s->EntityType != l->GetEntityType()->GetBehaviorProps<GGL::CLeaderBehaviorProps>()->SoldierType)
			throw lua::LuaException("leader and soldier type doesnt match");
		l->LeaderAttachSoldier(s->EntityId);
		return 0;
	}

	int SettlerCommandTurnSerfToBattleSerf(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		if (!s->GetBehavior<GGL::CSerfBehavior>())
			throw lua::LuaException("no serf");
		s->SerfTurnToBattleSerf();
		return 0;
	}

	int SettlerCommandTurnBattleSerfToSerf(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		if (!s->GetBehavior<GGL::CBattleSerfBehavior>())
			throw lua::LuaException("no battleserf");
		s->BattleSerfTurnToSerf();
		return 0;
	}

	int BuildingActivateOvertime(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (b->IsOvertimeActive)
			throw lua::LuaException("overtime already active");
		if (b->OvertimeCooldown > 0)
			throw lua::LuaException("cooldown active");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading && b->CurrentState != shok::TaskState::BuildingAlarmDefend))
			throw lua::LuaException("building not idle");
		b->ActivateOvertime();
		return 0;
	}

	int BuildingDeactivateOvertime(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsOvertimeActive)
			throw lua::LuaException("overtime not active");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading && b->CurrentState != shok::TaskState::BuildingAlarmDefend))
			throw lua::LuaException("building not idle");
		b->DeactivateOvertime();
		return 0;
	}

	int BarracksRecruitLeaders(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CBarrackBehavior* r = b->GetBehavior<GGL::CBarrackBehavior>();
		if (!r)
			throw lua::LuaException("no barracks");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading))
			throw lua::LuaException("building not idle");
		if (!r->AutoFillActive)
			throw lua::LuaException("already recruiting leaders");
		b->BarracksRecruitLeaders();
		return 0;
	}
	int BarracksRecruitGroups(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CBarrackBehavior* r = b->GetBehavior<GGL::CBarrackBehavior>();
		if (!r)
			throw lua::LuaException("no barracks");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading))
			throw lua::LuaException("building not idle");
		if (r->AutoFillActive)
			throw lua::LuaException("already recruiting groups");
		b->BarracksRecruitGroups();
		return 0;
	}

	int HQBuySerf(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsEntityInCategory(shok::EntityCategory::Headquarters))
			throw lua::LuaException("no hq");
		if (!b->IsIdle())
			throw lua::LuaException("building not idle");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
		if (p->PlayerAttractionHandler->GetAttractionUsage() >= p->PlayerAttractionHandler->GetAttractionLimit())
			throw lua::LuaException("pop capped");
		GGlue::CGlueEntityProps* solty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(*GGlue::CGlueEntityProps::EntityTypeIDSerf);
		if (!p->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(solty->LogicProps)->Cost))
			throw lua::LuaException("missing res");
		b->HQBuySerf();
		return 0;
	}

	int SellBuilding(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!((GGL::CGLBuildingProps*)b->GetEntityType()->LogicProps)->CanBeSold)
			throw lua::LuaException("cannot be sold");
		b->SellBuilding();
		return 0;
	}

	int BuildingStartResearch(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsIdle())
			throw lua::LuaException("building not idle");
		int tech = L.CheckInt(2);
		shok::Technology* techo = (*GGL::CGLGameLogic::GlobalObj)->GetTech(tech);
		if (!techo)
			throw lua::LuaException("no tech at 2");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
		shok::TechState techstate = p->GetTechStatus(tech);
		if (techstate != shok::TechState::Allowed)
			throw lua::LuaException("wrong techstate");
		if (!p->CurrentResources.HasResources(&techo->ResourceCosts))
			throw lua::LuaException("not enough res");
		b->StartResearch(tech);
		return 0;
	}

	int BuildingCancelResearch(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (b->GetTechnologyInResearch())
			throw lua::LuaException("no tech in research");
		b->CancelResearch();
		return 0;
	}

	bool MarketIsRes(shok::ResourceType rty) {
		return rty == shok::ResourceType::Clay || rty == shok::ResourceType::Gold || rty == shok::ResourceType::Iron || rty == shok::ResourceType::Stone || rty == shok::ResourceType::Sulfur || rty == shok::ResourceType::Wood;
	}

	int MarketStartTrade(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->GetBehavior<GGL::CMarketBehavior>())
			throw lua::LuaException("no market at 1");
		if (!b->IsIdle())
			throw lua::LuaException("building not idle");
		shok::ResourceType sellty = static_cast<shok::ResourceType>(L.CheckInt(2));
		shok::ResourceType buyty = static_cast<shok::ResourceType>(L.CheckInt(3));
		float am = L.CheckFloat(4);
		if (!MarketIsRes(sellty))
			throw lua::LuaException("sell type is invalid");
		if (!MarketIsRes(buyty))
			throw lua::LuaException("buy type is invalid");
		if (am <= 0)
			throw lua::LuaException("amount <= 0");
		b->MarketStartTrade(sellty, buyty, am);
		return 0;
	}

	int MarketCancelTrade(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->GetBehavior<GGL::CMarketBehavior>())
			throw lua::LuaException("no market at 1");
		if (b->GetMarketProgress() >= 1.0f)
			throw lua::LuaException("no transaction in progress");
		b->MarketCancelTrade();
		return 0;
	}

	int SettlerSetPosition(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		shok::Position p = L.CheckPos(2);
		if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p) == 0)
			throw lua::LuaException("position is blocked");
		s->SetPosition(p);
		return 0;
	}

	int LeaderSetSoldierLimit(lua::State ls) {
		luaext::EState L{ ls };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLimitedAttachmentBehavior* l = s->GetBehavior<GGL::CLimitedAttachmentBehavior>();
		if (!l)
			throw lua::LuaException("no limited attachment");
		int limit = L.CheckInt(2);
		if (limit < 0)
			throw lua::LuaException("limit < 0");
		try {
			auto& a = l->AttachmentMap.at(shok::AttachmentType::LEADER_SOLDIER);
			a.Limit = limit;
		}
		catch (const std::out_of_range&) {
			throw lua::LuaException{ "invalid attachment type" };
		}
		return 0;
	}

	int MercenaryRemoveLastOffer(lua::State ls) {
		luaext::EState L{ ls };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CBuildingMerchantBehavior* m = b->GetBehavior<GGL::CBuildingMerchantBehavior>();
		if (!m)
			throw lua::LuaException("no merchant");
		if (m->Offer.size() == 0)
			throw lua::LuaException("is empty");
		delete m->Offer[m->Offer.size() - 1];
		auto v = m->Offer.SaveVector();
		v.Vector.pop_back();
		return 0;
	}
	int MercenarySetOfferData(lua::State ls) {
		luaext::EState L{ ls };
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CBuildingMerchantBehavior* m = b->GetBehavior<GGL::CBuildingMerchantBehavior>();
		int i = L.CheckInt(2);
		if (!m)
			throw lua::LuaException("no merchant");
		if (i < 0 || i >= static_cast<int>(m->Offer.size()))
			throw lua::LuaException("invalid index");
		if (L.IsNumber(3)) {
			m->Offer[i]->OffersRemaining = L.CheckInt(3);
		}
		if (L.IsTable(4)) {
			bool ig = L.OptBool(5, false);
			L.ReadCostInfo(4, m->Offer[i]->ResourceCosts, ig);
		}
		return 0;
	}

	int SetMaxHP(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookDestroyEntity();
		EGL::CGLEEntity::HookMaxHP();
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
		if (L.IsNumber(2)) {
			d->HealthOverride = L.CheckInt(2);
		}
		if (L.IsBoolean(3)) {
			d->HealthUseBoni = L.ToBoolean(3);
		}
		return 0;
	};
	int CloneOverrideData(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* fro = L.OptEntity(1);
		EGL::CGLEEntity* to = L.CheckEntity(2);
		if (fro)
			to->CloneAdditionalDataFrom(fro->GetAdditionalData(false));
		else if (EGL::CGLEEntity::LastRemovedEntityAddonData.EntityId == L.CheckInt(1))
			to->CloneAdditionalDataFrom(&EGL::CGLEEntity::LastRemovedEntityAddonData);
		return 0;
	}
	int SetDamage(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookDamageMod();
		EGL::CGLEEntity::HookDestroyEntity();
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
		d->DamageOverride = L.CheckInt(2);
		return 0;
	}
	int SetArmor(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookArmorMod();
		EGL::CGLEEntity::HookDestroyEntity();
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
		d->ArmorOverride = L.CheckInt(2);
		return 0;
	}
	int SetExploration(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookExplorationMod();
		EGL::CGLEEntity::HookDestroyEntity();
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
		d->ExplorationOverride = L.CheckFloat(2);
		return 0;
	}
	int LeaderSetRegeneration(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		GGL::CSettler* b = L.CheckSettler(1);
		if (!b->GetBehavior<GGL::CLeaderBehavior>())
			throw lua::LuaException("no leader");
		EGL::CGLEEntity::HookLeaderRegen();
		EGL::CGLEEntity::HookDestroyEntity();
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
		if (L.IsNumber(2))
			d->RegenHPOverride = L.CheckInt(2);
		if (L.IsNumber(3))
			d->RegenSecondsOverride = L.CheckInt(3);
		return 0;
	}
	int SetAutoAttackMaxRange(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookMaxRange();
		EGL::CGLEEntity::HookDestroyEntity();
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
		d->MaxRangeOverride = L.CheckFloat(2);
		return 0;
	}
	int SetDisplayName(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookDisplayName();
		EGL::CGLEEntity::HookDestroyEntity();
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(true);
		d->NameOverride = L.CheckString(2);
		return 0;
	}
	int GetDisplayName(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::EntityAddonData* d = b->GetAdditionalData(false);
		if (d && !d->NameOverride.empty())
			L.Push(d->NameOverride);
		else
			L.Push(EGL::CGLEEntitiesProps::GetEntityTypeDisplayName(b->EntityType));
		return 1;
	}

	int LeaderGetRegeneration(lua::State l) {
		luaext::EState L{ l };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		GGL::CSettler* b = L.CheckSettler(1);
		if (!b->GetBehavior<GGL::CLeaderBehavior>())
			throw lua::LuaException("no leader");
		L.Push(b->LeaderGetRegenHealth());
		L.Push(b->LeaderGetRegenHealthSeconds());
		return 2;
	}

	int EnableRangedEffectSoldierHeal(lua::State l) {
		luaext::EState L{ l };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity::HookRangedEffectActivateHeal(L.ToBoolean(1));
		return 0;
	}

	int PerformHeal(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		int h = L.CheckInt(2);
		if (h <= 0)
			throw lua::LuaException("heal is <=0");
		e->PerformHeal(h, L.ToBoolean(3));
		return 0;
	}

	int BuildingGetBuildOnEntity(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckBuilding(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::BUILDING_BASE));
		return 1;
	}
	int BuildOnEntityGetBuilding(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedToMe(shok::AttachmentType::BUILDING_BASE));
		return 1;
	}

	int BuildingGetConstructionSite(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* e = L.CheckBuilding(1);
		L.Push(e->GetConstructionSite());
		return 1;
	}
	int ConstructionSiteGetBuilding(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::CONSTRUCTION_SITE_BUILDING));
		return 1;
	}

	int BarracksBuyLeaderByType(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* e = L.CheckBuilding(1);
		if (!e->GetBehavior<GGL::CBarrackBehavior>())
			throw lua::LuaException("no barracks");
		GGlue::CGlueEntityProps* ety = L.CheckEntityType(2);
		GGL::CLeaderBehaviorProps* lp = ety->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!lp)
			throw lua::LuaException("no leader type");
		if (!e->IsIdle(true))
			throw lua::LuaException("building not idle");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId);
		if (p->PlayerAttractionHandler->GetAttractionUsage() >= p->PlayerAttractionHandler->GetAttractionLimit())
			throw lua::LuaException("pop capped");
		if (!p->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(ety->LogicProps)->Cost))
			throw lua::LuaException("missing res");
		if (!L.ToBoolean(3)) {
			int ucat = p->BuildingUpgradeManager->GetUpgradeCategoryOfEntityType(e->EntityType);
			if (lp->BarrackUpgradeCategory != ucat)
				throw lua::LuaException("leader type doesnt match barracks type");
		}
		int id = e->BuyLeaderByType(L.CheckInt(2));
		L.Push(id);
		return 1;
	}

	int SettlerShurikenGetTarget(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CShurikenAbility* beh = s->GetBehavior<GGL::CShurikenAbility>();
		if (!beh)
			throw lua::LuaException("no shuriken ability");
		L.Push(beh->TargetId);
		return 1;
	}
	int SettlerSniperGetTarget(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CSniperAbility* beh = s->GetBehavior<GGL::CSniperAbility>();
		if (!beh)
			throw lua::LuaException("no snipe ability");
		L.Push(beh->TargetId);
		return 1;
	}

	int SettlerGetEnteredBuilding(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		L.Push(s->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING));
		L.Push(s->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE));
		return 2;
	}

	const char* AnimTaskList = "TL_SCRIPT_ANIMATION";
	void SettlerCreateAnimTaskList(lua::State L) {
		EGL::CGLETaskListMgr* tmng = *EGL::CGLETaskListMgr::GlobalObj;
		int tid = tmng->TaskListManager->GetIdByName(AnimTaskList);
		if (!tid) {
			BB::CClassFactory* fact = *BB::CClassFactory::GlobalObj;
			EGL::CGLETaskList* tl = fact->CreateObject<EGL::CGLETaskList>();

			{
				auto v = tl->Task.SaveVector();
				{
					EGL::CGLETaskArgsThousandths* t = fact->CreateObject<EGL::CGLETaskArgsThousandths>();
					t->TaskType = shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE;
					t->Thousandths = 1000;
					v.Vector.push_back(t);
				}
				{
					EGL::CGLETaskArgs* t = fact->CreateObject<EGL::CGLETaskArgs>();
					t->TaskType = shok::Task::TASK_SET_BATTLE_IDLE_ANIM;
					v.Vector.push_back(t);
				}
				{
					EGL::CGLETaskArgs* t = fact->CreateObject<EGL::CGLETaskArgs>();
					t->TaskType = shok::Task::TASK_BATTLE_WAIT_UNTIL;
					v.Vector.push_back(t);
				}
				{
					EGL::CGLETaskArgs* t = fact->CreateObject<EGL::CGLETaskArgs>();
					t->TaskType = shok::Task::TASK_SET_DEFAULT_REACTION_TYPE;
					v.Vector.push_back(t);
				}
				{
					EGL::CGLETaskArgs* t = fact->CreateObject<EGL::CGLETaskArgs>();
					t->TaskType = shok::Task::TASK_LIST_DONE;
					v.Vector.push_back(t);
				}
			}

			tid = tmng->RegisterTaskList(tl, AnimTaskList);
		
			int top = L.GetTop();
			L.Push("TaskLists");
			L.GetTableRaw(L.GLOBALSINDEX);
			L.Push(AnimTaskList);
			L.Push(tid);
			L.SetTableRaw(-3);
			L.SetTop(top);

			if (CppLogic::ModLoader::ModLoader::IsInitialized())
				CppLogic::ModLoader::ModLoader::AddTaskListToRemove(tid);
		}
	}
	void SettlerCleanupAnimTask(lua::State L) {
		if (CppLogic::ModLoader::ModLoader::IsInitialized()) // modloader will take care of cleanup
			return;
		EGL::CGLETaskListMgr* tmng = *EGL::CGLETaskListMgr::GlobalObj;
		if (!tmng)
			return;
		int tid = tmng->TaskListManager->GetIdByName(AnimTaskList);
		if (tid) {
			tmng->RemoveTaskList(tid);
		}
	}
	int SettlerPlayScriptAnimation(lua::State l) {
		luaext::EState L{ l };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");
		SettlerCreateAnimTaskList(l);
		EGL::CGLEEntity* e = L.CheckEntity(1);
		EGL::CGLETaskArgsAnimation setanim{};
		setanim.TaskType = shok::Task::TASK_SET_ANIM;
		setanim.AnimID = (*BB::CIDManagerEx::AnimManager)->GetIdByName(L.CheckString(2));
		if (!setanim.AnimID)
			throw lua::LuaException("not an animation");
		setanim.PlayBackwards = L.OptBool(3, false);
		EGL::CGLETaskArgs reset{};
		reset.TaskType = shok::Task::TASK_RESET_TASK_LIST_TIMER;
		e->ExecuteTask(reset);
		e->ExecuteTask(setanim);
		GGL::CGLBehaviorAnimationEx* animbeh = e->GetBehavior<GGL::CGLBehaviorAnimationEx>();
		animbeh->SpeedModifier = L.OptFloat(4, 1);
		animbeh->Duration = static_cast<int>(animbeh->Duration / animbeh->SpeedModifier);
		GGL::CBattleBehavior* batt = e->GetBehavior<GGL::CBattleBehavior>();
		if (batt)
			batt->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
		e->SetTaskList((*EGL::CGLETaskListMgr::GlobalObj)->TaskListManager->GetIdByName(AnimTaskList));
		return 0;
	}

	int SettlerGetExperienceClass(lua::State l) {
		luaext::EState L{ l };
		auto* s = L.CheckSettler(1);
		s->ModifierProfile.EntityReference.CheckInit();
		L.Push(static_cast<int>(s->ModifierProfile.EntityReference.ExperienceClass));
		return 1;
	}

	int GetBattleTarget(lua::State ls) {
		luaext::EState L{ ls };
		auto* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::ATTACKER_TARGET));
		return 1;
	}
	int GetAttackCommandTarget(lua::State ls) {
		luaext::EState L{ ls };
		auto* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::LEADER_TARGET));
		return 1;
	}

	int BuildingRefreshRelativePositions(lua::State ls) {
		luaext::EState L{ ls };
		auto* b = L.CheckBuilding(1);
		auto* t = static_cast<GGL::CGLBuildingProps*>(b->GetEntityType()->LogicProps);
		b->ApproachPosition = b->Position + t->ApproachPos;
		b->LeavePosition = b->Position + t->LeavePos;
		return 0;
	}


	void Cleanup(lua::State L) {
		DisableConversionHook(L);
		EGL::CGLEEntity::BuildingMaxHpTechBoni.clear();
		EGL::CGLEEntity::AddonDataMap.clear();
		EGL::CGLEEntity::LastRemovedEntityAddonData.EntityId = 0;
		EGL::CGLEEntity::HookRangedEffectActivateHeal(false);
		SettlerCleanupAnimTask(L);
	}

	constexpr std::array<lua::FuncReference, 34> Entity{ {
			lua::FuncReference::GetRef<GetScale>("GetScale"),
			lua::FuncReference::GetRef<SetScale>("SetScale"),
			lua::FuncReference::GetRef<MovingEntityGetTargetPos>("MovingEntityGetTargetPos"),
			lua::FuncReference::GetRef<MovingEntitySetTargetPos>("MovingEntitySetTargetPos"),
			lua::FuncReference::GetRef<GetLimitedLifespanRemaining>("GetLimitedLifespanRemaining"),
			lua::FuncReference::GetRef<SetLimitedLifespanRemaining>("SetLimitedLifespanRemaining"),
			lua::FuncReference::GetRef<GetTaskListIndex>("GetTaskListIndex"),
			lua::FuncReference::GetRef<SetTaskListIndex>("SetTaskListIndex"),
			lua::FuncReference::GetRef<MovingEntityGetSpeedFactor>("MovingEntityGetSpeedFactor"),
			lua::FuncReference::GetRef<IsSoldier>("IsSoldier"),
			lua::FuncReference::GetRef<GetAutoAttackMaxRange>("GetAutoAttackMaxRange"),
			lua::FuncReference::GetRef<GetModel>("GetModel"),
			lua::FuncReference::GetRef<GetExploration>("GetExploration"),
			lua::FuncReference::GetRef<GetSpeed>("GetSpeed"),
			lua::FuncReference::GetRef<IsFeared>("IsFeared"),
			lua::FuncReference::GetRef<ClearAttackers>("ClearAttackers"),
			lua::FuncReference::GetRef<ReplaceWithResourceEntity>("ReplaceWithResourceEntity"),
			lua::FuncReference::GetRef<SetMaxHP>("SetMaxHP"),
			lua::FuncReference::GetRef<CloneOverrideData>("CloneOverrideData"),
			lua::FuncReference::GetRef<SetDamage>("SetDamage"),
			lua::FuncReference::GetRef<SetArmor>("SetArmor"),
			lua::FuncReference::GetRef<SetExploration>("SetExploration"),
			lua::FuncReference::GetRef<SetAutoAttackMaxRange>("SetAutoAttackMaxRange"),
			lua::FuncReference::GetRef<SetDisplayName>("SetDisplayName"),
			lua::FuncReference::GetRef<GetDisplayName>("GetDisplayName"),
			lua::FuncReference::GetRef<PerformHeal>("PerformHeal"),
			lua::FuncReference::GetRef<EntityIteratorTableize>("EntityIteratorTableize"),
			lua::FuncReference::GetRef<LEntityIterator>("EntityIterator"),
			lua::FuncReference::GetRef<PlayerEntityIterator>("PlayerEntityIterator"),
			lua::FuncReference::GetRef<EntityIteratorGetNearest>("EntityIteratorGetNearest"),
			lua::FuncReference::GetRef<EntityIteratorCount>("EntityIteratorCount"),
			lua::FuncReference::GetRef<CheckPredicate>("CheckPredicate"),
			lua::FuncReference::GetRef<GetBattleTarget>("GetBattleTarget"),
			lua::FuncReference::GetRef<GetAttackCommandTarget>("GetAttackCommandTarget"),
	} };

	constexpr std::array<lua::FuncReference, 20> Predicates{ {
			lua::FuncReference::GetRef<PredicateAnd>("And"),
			lua::FuncReference::GetRef<PredicateOr>("Or"),
			lua::FuncReference::GetRef<PredicateNot>("Not"),
			lua::FuncReference::GetRef<PredicateSetPriority>("SetPriority"),
			lua::FuncReference::GetRef<PredicateOfType>("OfType"),
			lua::FuncReference::GetRef<PredicateOfPlayer>("OfPlayer"),
			lua::FuncReference::GetRef<PredicateInCircle>("InCircle"),
			lua::FuncReference::GetRef<PredicateIsBuilding>("IsBuilding"),
			lua::FuncReference::GetRef<PredicateIsSettler>("IsSettler"),
			lua::FuncReference::GetRef<PredicateIsCombatRelevant>("IsCombatRelevant"),
			lua::FuncReference::GetRef<PredicateOfAnyPlayer>("OfAnyPlayer"),
			lua::FuncReference::GetRef<PredicateOfAnyEntityType>("OfAnyEntityType"),
			lua::FuncReference::GetRef<PredicateIsNotSoldier>("IsNotSoldier"),
			lua::FuncReference::GetRef<PredicateOfEntityCategory>("OfEntityCategory"),
			lua::FuncReference::GetRef<PredicateProvidesResource>("ProvidesResource"),
			lua::FuncReference::GetRef<PredicateInRect>("InRect"),
			lua::FuncReference::GetRef<PredicateIsVisible>("IsVisible"),
			lua::FuncReference::GetRef<PredicateOfUpgradeCategory>("OfUpgradeCategory"),
			lua::FuncReference::GetRef<PredicateIsAlive>("IsAlive"),
			lua::FuncReference::GetRef<PredicateIsNotInBuilding>("IsNotInBuilding"),
	} };

	constexpr std::array<lua::FuncReference, 53> Settlers{ {
			lua::FuncReference::GetRef<SettlerGetLeaderOfSoldier>("GetLeaderOfSoldier"),
			lua::FuncReference::GetRef<SettlerGetBaseMovementSpeed>("GetBaseMovementSpeed"),
			lua::FuncReference::GetRef<SettlerSetBaseMovementSpeed>("SetBaseMovementSpeed"),
			lua::FuncReference::GetRef<SettlerGetOverheadWidget>("GetOverheadWidget"),
			lua::FuncReference::GetRef<SettlerSetOverheadWidget>("SetOverheadWidget"),
			lua::FuncReference::GetRef<HeroGetCamouflageDurationLeft>("HeroGetCamouflageDurationLeft"),
			lua::FuncReference::GetRef<HeroSetCamouflageDurationLeft>("HeroSetCamouflageDurationLeft"),
			lua::FuncReference::GetRef<ThiefGetCamouflageTimeTo>("ThiefGetCamouflageTimeTo"),
			lua::FuncReference::GetRef<ThiefSetCamouflageTimeTo>("ThiefSetCamouflageTimeTo"),
			lua::FuncReference::GetRef<HeroGetResurrectionTime>("HeroGetResurrectionTime"),
			lua::FuncReference::GetRef<HeroSetResurrectionTime>("HeroSetResurrectionTime"),
			lua::FuncReference::GetRef<HeroResurrect>("HeroResurrect"),
			lua::FuncReference::GetRef<WorkerGetCurrentWorkTime>("WorkerGetCurrentWorkTime"),
			lua::FuncReference::GetRef<WorkerSetCurrentWorkTime>("WorkerSetCurrentWorkTime"),
			lua::FuncReference::GetRef<ThiefSetStolenResourceInfo>("ThiefSetStolenResourceInfo"),
			lua::FuncReference::GetRef<SettlerIsVisible>("IsVisible"),
			lua::FuncReference::GetRef<SettlerIsIdle>("IsIdle"),
			lua::FuncReference::GetRef<SettlerCommandSendHawk>("CommandSendHawk"),
			lua::FuncReference::GetRef<SettlerGetHawkOfHero>("GetHawkOfHero"),
			lua::FuncReference::GetRef<SettlerCommandInflictFear>("CommandInflictFear"),
			lua::FuncReference::GetRef<SettlerCommandPlaceBomb>("CommandPlaceBomb"),
			lua::FuncReference::GetRef<SettlerCommandPlaceCannon>("CommandPlaceCannon"),
			lua::FuncReference::GetRef<SettlerCommandRangedEffect>("CommandRangedEffect"),
			lua::FuncReference::GetRef<SettlerCommandCircularAttack>("CommandCircularAttack"),
			lua::FuncReference::GetRef<SettlerCommandSummon>("CommandSummon"),
			lua::FuncReference::GetRef<SettlerCommandActivateCamoflage>("CommandActivateCamoflage"),
			lua::FuncReference::GetRef<SettlerCommandConvert>("CommandConvert"),
			lua::FuncReference::GetRef<SettlerCommandSnipe>("CommandSnipe"),
			lua::FuncReference::GetRef<SettlerCommandShuriken>("CommandShuriken"),
			lua::FuncReference::GetRef<SettlerCommandMotivateWorkers>("CommandMotivateWorkers"),
			lua::FuncReference::GetRef<SettlerCommandSabotage>("CommandSabotage"),
			lua::FuncReference::GetRef<SettlerCommandDefuse>("CommandDefuse"),
			lua::FuncReference::GetRef<SettlerCommandBinocular>("CommandBinocular"),
			lua::FuncReference::GetRef<SettlerCommandPlaceTorch>("CommandPlaceTorch"),
			lua::FuncReference::GetRef<SettlerCommandPointToRes>("CommandPointToRes"),
			lua::FuncReference::GetRef<SettlerCommandStealFrom>("CommandStealFrom"),
			lua::FuncReference::GetRef<SettlerCommandSecureGoods>("CommandSecureGoods"),
			lua::FuncReference::GetRef<EnableConversionHook>("EnableConversionHook"),
			lua::FuncReference::GetRef<DisableConversionHook>("DisableConversionHook"),
			lua::FuncReference::GetRef<SettlerCommandMove>("CommandMove"),
			lua::FuncReference::GetRef<SettlerCommandSerfConstructBuilding>("CommandSerfConstructBuilding"),
			lua::FuncReference::GetRef<SettlerCommandSerfRepairBuilding>("CommandSerfRepairBuilding"),
			lua::FuncReference::GetRef<SettlerCommandSerfExtract>("CommandSerfExtract"),
			lua::FuncReference::GetRef<SettlerCommandExpell>("CommandExpell"),
			lua::FuncReference::GetRef<SettlerCommandTurnSerfToBattleSerf>("CommandTurnSerfToBattleSerf"),
			lua::FuncReference::GetRef<SettlerCommandTurnBattleSerfToSerf>("CommandTurnBattleSerfToSerf"),
			lua::FuncReference::GetRef<SettlerSetPosition>("SetPosition"),
			lua::FuncReference::GetRef<EnableRangedEffectSoldierHeal>("EnableRangedEffectSoldierHeal"),
			lua::FuncReference::GetRef<SettlerShurikenGetTarget>("ShurikenGetTarget"),
			lua::FuncReference::GetRef<SettlerSniperGetTarget>("SniperGetTarget"),
			lua::FuncReference::GetRef<SettlerGetEnteredBuilding>("GetEnteredBuilding"),
			lua::FuncReference::GetRef<SettlerPlayScriptAnimation>("PlayScriptAnimation"),
			lua::FuncReference::GetRef<SettlerGetExperienceClass>("GetExperienceClass"),
	} };

	constexpr std::array<lua::FuncReference, 8> Leader{ {
			lua::FuncReference::GetRef<LeaderGetExperience>("GetExperience"),
			lua::FuncReference::GetRef<LeaderSetExperience>("SetExperience"),
			lua::FuncReference::GetRef<LeaderGetTroopHealth>("GetTroopHealth"),
			lua::FuncReference::GetRef<LeaderSetTroopHealth>("SetTroopHealth"),
			lua::FuncReference::GetRef<LeaderAttachSoldier>("AttachSoldier"),
			lua::FuncReference::GetRef<LeaderSetSoldierLimit>("SetSoldierLimit"),
			lua::FuncReference::GetRef<LeaderSetRegeneration>("SetRegeneration"),
			lua::FuncReference::GetRef<LeaderGetRegeneration>("GetRegeneration"),
	} };

	constexpr std::array<lua::FuncReference, 32> Building{ {
			lua::FuncReference::GetRef<BuildingGetBarracksAutoFillActive>("GetBarracksAutoFillActive"),
			lua::FuncReference::GetRef<BuildingGetHeight>("GetHeight"),
			lua::FuncReference::GetRef<BuildingSetHeight>("SetHeight"),
			lua::FuncReference::GetRef<BuildingMarketGetCurrentTradeData>("MarketGetCurrentTradeData"),
			lua::FuncReference::GetRef<BuildingMarketSetCurrentTradeData>("MarketSetCurrentTradeData"),
			lua::FuncReference::GetRef<BuildingCommandFoundryBuildCannon>("CommandFoundryBuildCannon"),
			lua::FuncReference::GetRef<BuildingGetNearestFreeConstructionSlotFor>("GetNearestFreeConstructionSlotFor"),
			lua::FuncReference::GetRef<BuildingGetNearestFreeRepairSlotFor>("GetNearestFreeRepairSlotFor"),
			lua::FuncReference::GetRef<BuildingStartUpgrade>("StartUpgrade"),
			lua::FuncReference::GetRef<BuildingCancelUpgrade>("CancelUpgrade"),
			lua::FuncReference::GetRef<BuildingIsIdle>("IsIdle"),
			lua::FuncReference::GetRef<BarracksGetLeadersTrainingAt>("BarracksGetLeadersTrainingAt"),
			lua::FuncReference::GetRef<FoundryGetCannonTypeInConstruction>("FoundryGetCannonTypeInConstruction"),
			lua::FuncReference::GetRef<BarracksBuySoldierForLeader>("BarracksBuySoldierForLeader"),
			lua::FuncReference::GetRef<BuildingActivateOvertime>("ActivateOvertime"),
			lua::FuncReference::GetRef<BuildingDeactivateOvertime>("DeactivateOvertime"),
			lua::FuncReference::GetRef<BarracksRecruitGroups>("BarracksRecruitGroups"),
			lua::FuncReference::GetRef<BarracksRecruitLeaders>("BarracksRecruitLeaders"),
			lua::FuncReference::GetRef<HQBuySerf>("HQBuySerf"),
			lua::FuncReference::GetRef<SellBuilding>("SellBuilding"),
			lua::FuncReference::GetRef<BuildingStartResearch>("StartResearch"),
			lua::FuncReference::GetRef<BuildingCancelResearch>("CancelResearch"),
			lua::FuncReference::GetRef<MarketStartTrade>("MarketStartTrade"),
			lua::FuncReference::GetRef<MarketCancelTrade>("MarketCancelTrade"),
			lua::FuncReference::GetRef<MercenaryRemoveLastOffer>("MercenaryRemoveLastOffer"),
			lua::FuncReference::GetRef<MercenarySetOfferData>("MercenarySetOfferData"),
			lua::FuncReference::GetRef<BuildingGetBuildOnEntity>("GetBuildOnEntity"),
			lua::FuncReference::GetRef<BuildOnEntityGetBuilding>("BuildOnEntityGetBuilding"),
			lua::FuncReference::GetRef<BuildingGetConstructionSite>("GetConstructionSite"),
			lua::FuncReference::GetRef<ConstructionSiteGetBuilding>("ConstructionSiteGetBuilding"),
			lua::FuncReference::GetRef<BarracksBuyLeaderByType>("BarracksBuyLeaderByType"),
			lua::FuncReference::GetRef<BuildingRefreshRelativePositions>("RefreshRelativePositions"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(Entity, -3);

		L.Push("Predicates");
		L.NewTable();
		L.RegisterFuncs(Predicates, -3);
		L.SetTableRaw(-3);

		L.Push("Settler");
		L.NewTable();
		L.RegisterFuncs(Settlers, -3);
		L.SetTableRaw(-3);

		L.Push("Leader");
		L.NewTable();
		L.RegisterFuncs(Leader, -3);
		L.SetTableRaw(-3);

		L.Push("Building");
		L.NewTable();
		L.RegisterFuncs(Building, -3);
		L.SetTableRaw(-3);
	}
}

// CppLogic.Entity.CheckPredicate(GUI.GetEntityAtPosition(GUI.GetMousePosition()), CppLogic.Entity.Predicates.ProvidesResource(ResourceType.WoodRaw))
// CppLogic.Entity.test(GUI.GetEntityAtPosition(GUI.GetMousePosition()), EntityCategories.Sword)
// CppLogic.Entity.GetNumberOfAllocatedEntities()
// CppLogic.Entity.GetNumberOfBehaviors(GUI.GetEntityAtPosition(GUI.GetMousePosition()))
// CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.IsSettler(), CppLogic.Entity.Predicates.IsNotSoldier(), CppLogic.Entity.Predicates.OfPlayer(1))
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Entity.EntityIteratorTableize(CppLogic.Entity.Predicates.InCircle(x,y, 1000))
// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Entity.EntityIteratorGetNearest(CppLogic.Entity.Predicates.InCircle(x,y, 1000))
// local x,y = GUI.Debug_GetMapPositionUnderMouse();  for id,r in CppLogic.Entity.EntityIterator(CppLogic.Entity.Predicates.OfAnyEntityType(Entities.PU_Serf, Entities.PB_Headquarters1), CppLogic.Entity.Predicates.InCircle(x,y, 1000)) do LuaDebugger.Log(id.."   "..r) end
// CppLogic.Entity.Settler.GetBaseMovementSpeed(GUI.GetSelectedEntity())
// CppLogic.Entity.MovingEntityGetTargetPos(GUI.GetSelectedEntity())
// local x,y = GUI.Debug_GetMapPositionUnderMouse() CppLogic.Entity.MovingEntitySetTargetPos(GUI.GetSelectedEntity(), {X=x,Y=y})
// for id in CppLogic.Entity.PlayerEntityIterator(CppLogic.Entity.Predicates.IsSettler(), 1) do LuaDebugger.Log(id) end

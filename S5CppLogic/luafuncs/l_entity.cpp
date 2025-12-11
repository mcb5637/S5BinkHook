#include "pch.h"
#include "l_entity.h"
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_entity.h>
#include <shok/events/s5_events.h>
#include <shok/s5_defines.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/entity/s5_behaviors.h>
#include <shok/entitytype/s5_behaviorProps.h>
#include <shok/player/s5_player.h>
#include <shok/s5_scriptsystem.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/entity/s5_tasklist.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_classfactory.h>
#include <utility/hooks.h>
#include <luaext.h>
#include <utility/entityiterator.h>
#include <utility/modloader.h>
#include <utility/savegame_extra.h>
#include <utility/EntityAddonData.h>
#include <utility/ModBehavior.h>
#include <utility/LuaEventInterface.h>
#include <utility/luaserializer.h>

namespace CppLogic::Entity {
	int PredicateOfType(lua::State ls) {
		luaext::EState L{ ls };
		auto ty = L.CheckEnum<shok::EntityTypeId>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfType>(ty);
		return 1;
	}

	int PredicateOfPlayer(lua::State ls) {
		luaext::EState L{ ls };
		auto pl = L.CheckPlayerId(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfPlayer>(pl);
		return 1;
	}

	int PredicateInCircle(lua::State L) {
		float x = L.CheckFloat(1);
		float y = L.CheckFloat(2);
		float r = L.CheckFloat(3);
		L.NewUserClass<CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity>>(shok::Position{ x,y }, r * r);
		return 1;
	}

	int PredicateOfAnyPlayer(lua::State l) {
		luaext::EState L{ l };
		int num = L.GetTop();
		if (num > 9)
			throw lua::LuaException("too many players to check");
		auto* p = L.NewUserClass<CppLogic::Iterator::EntityPredicateOfAnyPlayer>();
		for (int i = 0; i < num; ++i) {
			p->players[i] = L.CheckPlayerId(i);
		}
		return 1;
	}

	int PredicateOfAnyEntityType(lua::State l) {
		luaext::EState L{ l };
		int num = L.GetTop();
		auto* p = L.NewUserClass<CppLogic::Iterator::EntityPredicateOfAnyType>();
		p->entityTypes.reserve(num);
		for (int i = 1; i <= num; ++i)
			p->entityTypes.push_back(L.CheckEnum<shok::EntityTypeId>(i));
		return 1;
	}

	int PredicateAnd(lua::State L) {
		const int num = L.GetTop();
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicateDynamicAnd<EGL::CGLEEntity>>();
		p->preds.reserve(num);
		L.NewTable();
		for (int i = 1; i <= num; ++i) { // keep predicates, so they dont get gced
			p->preds.push_back(L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(i));
			L.PushValue(i);
			L.SetTableRaw(-2, i);
		}
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}
	void PredicateAndAutoCreate(lua::State L) { // clear stack after creating and predicate
		PredicateAnd(L);
		L.Insert(1);
		L.SetTop(1);
	}

	int PredicateOr(lua::State L) {
		const int num = L.GetTop();
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicateDynamicOr<EGL::CGLEEntity>>();
		p->preds.reserve(num);
		L.NewTable();
		for (int i = 1; i <= num; ++i) { // keep predicates, so they dont get gced
			p->preds.push_back(L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(i));
			L.PushValue(i);
			L.SetTableRaw(-2, i);
		}
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateNot(lua::State L) {
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicateNot<EGL::CGLEEntity>>(pred);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateSetPriority(lua::State L) {
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		int pri = L.CheckInt(2);
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicatePriority<EGL::CGLEEntity>>(pred, pri);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateIsSettler(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsSettler>();
		return 1;
	}

	int PredicateIsBuilding(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsBuildingAndNotConstructionSite>();
		return 1;
	}

	int PredicateIsBuildingOrConstructionSite(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsBuilding>();
		return 1;
	}

	int PredicateIsCombatRelevant(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsCombatRelevant>();
		return 1;
	}
	int PredicateIsNotSoldier(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsNotSoldier>();
		return 1;
	}

	int PredicateOfEntityCategory(lua::State ls) {
		luaext::EState L{ ls };
		auto c = L.CheckEnum<shok::EntityCategory>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfEntityCategory>(c);
		return 1;
	}

	int PredicateProvidesResource(lua::State ls) {
		luaext::EState L{ ls };
		auto ty = L.CheckEnum<shok::ResourceType>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateProvidesResource>(ty);
		return 1;
	}

	int PredicateInRect(lua::State L) {
		float x1 = L.CheckFloat(1);
		float y1 = L.CheckFloat(2);
		float x2 = L.CheckFloat(3);
		float y2 = L.CheckFloat(4);
		L.NewUserClass<CppLogic::Iterator::PredicateInRect<EGL::CGLEEntity>>(x1, y1, x2, y2);
		return 1;
	}

	int PredicateIsVisible(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsVisible>();
		return 1;
	}

	int PredicateOfUpgradeCategory(lua::State ls) {
		luaext::EState L{ ls };
		auto ty = L.CheckEnum<shok::UpgradeCategoryId>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfUpgradeCategory>(ty);
		return 1;
	}

	int PredicateIsAlive(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsAlive>();
		return 1;
	}

	int PredicateIsNotInBuilding(lua::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsNotInBuilding>();
		return 1;
	}

	int EntityIteratorTableize(lua::State l) {
		luaext::EState L{ l };
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAndAutoCreate(L);
		}
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
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
			PredicateAndAutoCreate(L);
		}
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
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
			PredicateAndAutoCreate(L);
		}
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		CppLogic::Iterator::GlobalEntityIterator it{ pred };
		float maxR = -1;
		EGL::CGLEEntity* e = it.GetNearest(&maxR);
		L.Push(e == nullptr ? 0 : static_cast<int>(e->EntityId));
		L.Push(maxR);
		return 2;
	}

	int EntityIteratorNext(lua::State l) { // (state nil, last value) -> next value
		luaext::EState L{ l };
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
			PredicateAndAutoCreate(L);
		}
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);

		L.NewUserClass<CppLogic::Iterator::GlobalEntityIterator>(pred); // upvalue of func
		L.Push<EntityIteratorNext>(1); // func
		L.Push(); // state
		L.Push(); // initial value
		return 3;
	}

	int PlayerEntityIterator(lua::State l) {
		luaext::EState L{ l };
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		if (L.GetTop() == 2) {
			auto pl = L.CheckPlayerId(2);
			L.NewUserClass<CppLogic::Iterator::PlayerEntityIterator>(pl, pred); // upvalue of func
		}
		else {
			int num = L.GetTop() - 1;
			if (num > 8)
				throw lua::LuaException("too many players");
			auto* it = L.NewUserClass<CppLogic::Iterator::MultiPlayerEntityIterator>(pred); // upvalue of func
			for (int i = 0; i < num; ++i) {
				it->Players[i] = L.CheckPlayerId(i + 2, false);
			}
		}
		L.Push<EntityIteratorNext>(1); // func
		L.Push(); // state
		L.Push(); // initial value
		return 3;
	}

	int RegionEntityIteratorRect(lua::State Ls) {
		luaext::EState L{ Ls };
		shok::AARect area{ L.CheckPos(1), L.CheckPos(2) };
		shok::AccessCategoryFlags acf = static_cast<shok::AccessCategoryFlags>(L.CheckInt(3));
		L.Remove(3);
		L.Remove(2);
		L.NewUserClass<CppLogic::Iterator::PredicateInRect<EGL::CGLEEntity>>(area);
		if (L.GetTop() > 1) { // auto create an and predicate
			L.Replace(1);
			PredicateAndAutoCreate(L);
		}

		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);

		L.NewUserClass<CppLogic::Iterator::MultiRegionEntityIterator>(area, acf, pred); // upvalue of func
		L.Push<EntityIteratorNext>(1); // func
		L.Push(); // state
		L.Push(); // initial value
		return 3;
	}
	int RegionEntityIteratorCircle(lua::State Ls) {
		luaext::EState L{ Ls };
		shok::Position p = L.CheckPos(1);
		float r = L.CheckFloat(2);
		shok::AccessCategoryFlags acf = static_cast<shok::AccessCategoryFlags>(L.CheckInt(3));
		L.Remove(3);
		L.Remove(2);
		L.NewUserClass<CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity>>(p, r * r);
		if (L.GetTop() > 1) { // auto create an and predicate
			L.Replace(1);
			PredicateAndAutoCreate(L);
		}

		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);

		L.NewUserClass<CppLogic::Iterator::MultiRegionEntityIterator>(p, r, acf, pred); // upvalue of func
		L.Push<EntityIteratorNext>(1); // func
		L.Push(); // state
		L.Push(); // initial value
		return 3;
	}

	int CheckPredicate(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* s = L.CheckEntity(1);
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
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
		GGL::CLeaderBehavior* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		L.Push(b->Experience);
		return 1;
	}
	int LeaderSetExperience(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLeaderBehavior* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		b->Experience = L.CheckInt(2);
		return 0;
	}

	int LeaderGetTroopHealth(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLeaderBehavior* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		L.Push(b->GetTroopHealth());
		L.Push(b->GetTroopHealthPerSoldier());
		return 2;
	}
	int LeaderSetTroopHealth(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CLeaderBehavior* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		b->TroopHealthCurrent = L.CheckInt(2);
		return 0;
	}

	int SettlerGetBaseMovementSpeed(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CBehaviorDefaultMovement* b = s->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
		if (!b)
			throw lua::LuaException("no moving entity at 1");
		L.Push(b->MovementSpeed);
		L.Push(CppLogic::RadiansToDegrees(b->TurningSpeed));
		return 2;
	}
	int SettlerSetBaseMovementSpeed(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CBehaviorDefaultMovement* b = s->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
		if (!b)
			throw lua::LuaException("no moving entity at 1");
		if (L.IsNumber(2))
			b->MovementSpeed = L.CheckFloat(2);
		if (L.IsNumber(3))
			b->TurningSpeed = static_cast<float>(CppLogic::DegreesToRadians(*L.ToNumber(3)));
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
		L.Push(b->ConstructionProgress);
		return 1;
	}
	int BuildingSetHeight(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* b = L.CheckBuilding(1);
		b->ConstructionProgress = L.CheckFloat(2);
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
			m->SetTargetRotation(static_cast<float>(CppLogic::DegreesToRadians(*L.ToNumber(-1))));
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

	int MovingEntityIsFleeingFrom(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		EGL::CMovingEntity* m = dynamic_cast<EGL::CMovingEntity*>(e);
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		auto p = L.CheckPos(2);
		L.Push(m->IsFleeingFrom(p, L.OptFloat(3, 500.0f)));
		return 1;
	}

	int HeroGetCamouflageDurationLeft(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CCamouflageBehavior* c = s->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no camo hero at 1");
		L.Push(c->InvisibilityRemaining);
		return 1;
	}
	int HeroSetCamouflageDurationLeft(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CCamouflageBehavior* c = s->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
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

	int Debug_GetTaskInfo(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(CppLogic::GetIdManager<shok::TaskListId>().GetNameByID(e->CurrentTaskListID));
		auto* tl = e->GetCurrentTaskList();
		if (tl == nullptr)
			return 1;
		// CurrentTaskIndex gets increased after task exec, no matter if we go into state or not, so correct that
		L.Push(CppLogic::GetIdManager<shok::Task>().GetNameByID(tl->GetTask(e->CurrentTaskIndex - 1)->TaskType));
		const char* n = CppLogic::GetIdManager<shok::TaskState>().GetNameByID(e->CurrentState);
		if (n != nullptr)
			L.Push(n);
		else
			L.Push(static_cast<int>(e->CurrentState));
		return 3;
	}

	int GetTrackedResources(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::ResourceTrackerBehavior>();
		if (!b)
			return 0;
		L.PushCostInfo(b->Produced);
		L.PushCostInfo(b->Used);
		return 2;
	}

	int GetAllScriptNameMappings(lua::State l) {
		luaext::EState L{ l };
		auto& m = (*EGL::CGLEEntityManager::GlobalObj)->ScriptName;
		L.NewTable();
		for (auto& [str, id] : m) {
			L.Push(str);
			L.Push(id);
			L.SetTableRaw(-3);
		}
		return 1;
	}

	int GetLimitedAmmo(lua::State ls) {
		luaext::EState L{ ls };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::LimitedAmmoBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no limited ammo" };
		L.Push(b->RemainingAmmo);
		return 1;
	};
	int SetLimitedAmmo(lua::State ls) {
		luaext::EState L{ ls };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::LimitedAmmoBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no limited ammo" };
		b->RemainingAmmo = L.CheckInt(2);
		return 0;
	};

	int DumpEntity(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, e);
		return 1;
	}

	int MovingEntityGetSpeedFactor(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CBehaviorDefaultMovement* m = e->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
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
		L.Push(s->EventGetMaxWorktime());
		return 2;
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

	int WorkerGetMotivation(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CWorkerBehavior* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		L.Push(s->EventGetMotivation());
		return 1;
	}
	int WorkerChangeMotivation(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* s = L.CheckSettler(1);
		GGL::CWorkerBehavior* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		int r = L.CheckInt(3);
		if (r < static_cast<int>(shok::WorkerReason::None) || r > static_cast<int>(shok::WorkerReason::NoPay))
			throw lua::LuaException{ "invalid reason" };
		GGL::CEventChangeMotivation ev{ shok::EventIDs::Worker_ChangeMoti, L.CheckFloat(2), static_cast<shok::WorkerReason>(r) };
		s->FireEvent(&ev);
		return 0;
	}

	int WorkerGetResourceCarried(lua::State ls) {
		luaext::EState L{ ls };
		auto* e = L.CheckSettler(1);
		auto* b = e->GetBehavior<GGL::CWorkerBehavior>();
		if (!b)
			throw lua::LuaException{ "not a worker" };
		L.Push(b->CarriedResourceAmount);
		L.Push(b->CouldConsumeResource);
		return 2;
	}

	int SettlerGetSummoned(lua::State ls) {
		luaext::EState L{ ls };
		auto* e = L.CheckSettler(1);
		L.NewTable();
		int i = 1;
		for (const auto& [at, attach] : e->ObservedEntities.ForKeys(shok::AttachmentType::SUMMONER_SUMMONED)) {
			L.Push(attach.EntityId);
			L.SetTableRaw(-2, i);
			++i;
		}
		return 1;
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
		shok::ResourceType ty = L.CheckEnum<shok::ResourceType>(2, true);
		int am;
		if (ty == shok::ResourceType::None)
			am = 0;
		else
			am = L.CheckInt(3);
		t->ResourceType = ty;
		t->Amount = am;
		if (ty == shok::ResourceType::None)
			t->StolenFromPlayer = shok::PlayerId::P0;
		else if (L.IsNumber(4))
			t->StolenFromPlayer = L.CheckPlayerId(4); // todo: check for func that refreshes model
		return 0;
	}

	int GetAutoAttackMaxRange(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CBattleBehavior* b = e->GetBehaviorDynamic<GGL::CBattleBehavior>();
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
		auto m = e->ModelOverride;
		if (m == shok::ModelId::Invalid) {
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
		GGL::CBehaviorDefaultMovement* m = e->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		L.Push(m->GetMovementSpeed() * 10.0);
		return 1;
	}

	int SettlerIsVisible(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		GGL::CCamouflageBehavior* c = e->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
		if (c != nullptr) {
			L.Push(c->InvisibilityRemaining <= 0);
			return 1;
		}
		L.Push(true);
		return 1;
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

	void CheckPlaceCannonEvent(EGL::CGLEEntity* e, GGL::CEventPositionAnd2EntityTypes& ev) {
		ev.Position.FloorToBuildingPlacement();
		auto bottom = ev.Type1;
		GGlue::CGlueEntityProps* ety = CppLogic::GetEntityType(bottom);
		if (!ety)
			throw lua::LuaException("no bottom entitytype");
		if (!ety->IsBuildingType())
			throw lua::LuaException("bottom not a building");
		auto top = ev.Type2;
		if (!CppLogic::GetEntityType(top))
			throw lua::LuaException("no top entitytype");
		if (!GGL::CPlayerStatus::CanPlaceBuilding(bottom, e->PlayerId, &ev.Position, 0, shok::EntityId::Invalid))
			throw lua::LuaException("cannot place foundation at that position");
	}

	int SettlerCommandSummon(lua::State l) {
		luaext::EState L{ l };
		GGL::CSettler* e = L.CheckSettler(1);
		L.CheckEntityAlive(e->EntityId, "entity dead at 1");
		BB::CEvent ev{ shok::EventIDs::Summon_ActivateCommand };
		LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilitySummon>(e, ev);
		e->FireEvent(&ev);
		int summoned = 0;
		for (const auto& a : e->ObserverEntities) {
			if (a.first == shok::AttachmentType::SUMMONER_SUMMONED) {
				L.Push(a.second.EntityId);
				summoned++;
			}
		}
		return summoned;
	}

	void CheckCamoEvent(EGL::CGLEEntity* e, BB::CEvent& ev) {
		GGL::CCamouflageBehavior* b = e->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
		if (dynamic_cast<GGL::CThiefCamouflageBehavior*>(b))
			throw lua::LuaException("thief camo cannot be manually activated");
	}

	void CheckSnipeEvent(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		GGL::CSniperAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!e->Position.IsInRange(oth->Position, bp->Range))
			throw lua::LuaException("target not in range");
	}

	void CheckShurikenEvent(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		GGL::CShurikenAbilityProps* bp = e->GetEntityType()->GetBehaviorProps<GGL::CShurikenAbilityProps>();
		if (!e->Position.IsInRange(oth->Position, bp->Range))
			throw lua::LuaException("target not in range");
	}

	void CheckSabotageEvent(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (!(oth->IsEntityInCategory(shok::EntityCategory::Bridge) || GGL::CPlayerStatus::ArePlayersHostile(e->PlayerId, oth->PlayerId)))
			throw lua::LuaException("target is not hostile or bridge");
	}
	void CheckThiefNotCarryingEvent(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		if (!(e->GetBehavior<GGL::CThiefBehavior>()->ResourceType == shok::ResourceType::None))
			throw lua::LuaException("is carrying resources");
	}
	void CheckDefuseEvent(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* t = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (!t->GetBehavior<GGL::CKegBehavior>())
			throw lua::LuaException("target is no keg");
	}
	void CheckIsThief(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		if (e->GetBehavior<GGL::CThiefBehavior>() == nullptr)
			throw lua::LuaException("not a thief");
	}
	void CheckLightingStrike(EGL::CGLEEntity* e, EGL::CEventPosition& ev) {
		auto* b = e->GetBehavior<CppLogic::Mod::LightningStrikeAbility>();
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::LightningStrikeAbilityProps>();
		if (!e->Position.IsInRange(ev.Position, bp->Range))
			throw lua::LuaException("not in range");
		if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->CurrentResources.WeatherEnergy < bp->WeatherEnergyCost)
			throw lua::LuaException("not enough weather energy");
	}
	void CheckResourceRefill(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* b = e->GetBehavior<CppLogic::Mod::ResDoodadRefillBehavior>();
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::ResDoodadRefillBehaviorProps>();
		auto* t = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		auto* res = EGL::CGLEEntity::GetEntityByID(t->GetFirstAttachedEntity(shok::AttachmentType::MINE_RESOURCE));
		if (res == nullptr)
			throw lua::LuaException("no mine");
		if (!res->IsEntityInCategory(bp->AffectedTypes))
			throw lua::LuaException("not affectable");
	}
	void CheckResurrect(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* t = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (t->GetBehavior<GGL::CHeroBehavior>() == nullptr)
			throw lua::LuaException("not a hero");
		if (!t->IsDead())
			throw lua::LuaException("is alive");
	}
	void CheckBombardment(EGL::CGLEEntity* e, EGL::CEventPosition& ev) {
		auto* b = e->GetBehavior<CppLogic::Mod::BombardmentAbility>();
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::BombardmentAbilityProps>();
		if (!e->Position.IsInRange(ev.Position, bp->AttackRange))
			throw lua::LuaException("not in range");
	}

	int EnableConversionHook(lua::State L) {
		GGL::CConvertSettlerAbility::HookConvertEvent();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ConversionTrigger = true;
		return 0;
	}
	int DisableConversionHook(lua::State L) {
		return 0;
	}

	int IsFeared(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto id = e->GetFirstAttachedToMe(shok::AttachmentType::INFLICTOR_TERRORIZED);
		if (id == shok::EntityId::Invalid)
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
		if (ls->GetSector(&p) == shok::SectorId::Invalid) {
			shok::Position pou;
			if (ls->GetNearestPositionInSector(&p, 1000, e->GetSector(), &pou))
				p = pou;
		}
		e->Move(p);
		if (L.IsNumber(3)) {
			e->SetTargetRotation(static_cast<float>(CppLogic::DegreesToRadians(*L.ToNumber(3))));
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
		auto ety = L.CheckEnum<shok::EntityTypeId>(2);
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
		GGL::CLeaderBehaviorProps* lp = s->GetEntityType()->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!l)
			throw lua::LuaException("no leader");
		if (b->PlayerId != s->PlayerId)
			throw lua::LuaException("different players");
		GGlue::CGlueEntityProps* solty = CppLogic::GetEntityType(lp->SoldierType);
		if (!solty)
			throw lua::LuaException("no soldier type set");
		if (!L.ToBoolean(3)) {
			auto ucat = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(shok::PlayerId::P1)->BuildingUpgradeManager->GetUpgradeCategoryOfEntityType(b->EntityType);
			if (lp->BarrackUpgradeCategory != ucat)
				throw lua::LuaException("leader type doesnt match barracks type");
		}
		if (L.OptBool(4, true)) {
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
		}
		b->CommandRecruitSoldierForLeader(s->EntityId);
		return 0;
	}

	int LeaderAttachSoldier(lua::State ls) {
		luaext::EState L{ ls };
		GGL::CSettler* l = L.CheckSettler(1);
		GGL::CSettler* s = L.CheckSettler(2);
		GGL::CLeaderBehavior* lb = l->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!lb)
			throw lua::LuaException("no leader");
		if (!s->GetBehavior<GGL::CSoldierBehavior>())
			throw lua::LuaException("no soldier");
		if (s->EntityType != l->GetEntityType()->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>()->SoldierType)
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
		if (b->OvertimeRechargeTime > 0)
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
		GGlue::CGlueEntityProps* solty = CppLogic::GetEntityType(*GGlue::CGlueEntityProps::EntityTypeIDSerf);
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
		auto tech = L.CheckEnum<shok::TechnologyId>(2);
		shok::Technology* techo = CppLogic::GetTechnology(tech);
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
		if (b->GetTechnologyInResearch() != shok::TechnologyId::Invalid)
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
		if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p) == shok::SectorId::Invalid)
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
		GGL::CBuildingMerchantBehavior* m = b->GetBehaviorDynamic<GGL::CBuildingMerchantBehavior>();
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
		GGL::CBuildingMerchantBehavior* m = b->GetBehaviorDynamic<GGL::CBuildingMerchantBehavior>();
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
		EGL::CGLEEntity::HookMaxHP();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookMaxHP = true;
		auto* d = b->GetAdditionalData(true);
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
		if (fro) {
			auto* add = fro->GetAdditionalData();
			if (add)
				to->CloneAdditionalDataFrom(*add);
		}
		else if (EGL::CGLEEntity::LastRemovedEntityAddonData.EntityId == static_cast<shok::EntityId>(L.CheckInt(1)))
			to->CloneAdditionalDataFrom(EGL::CGLEEntity::LastRemovedEntityAddonData);
		return 0;
	}
	int SetDamage(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookDamageMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookDamage = true;
		auto* d = b->GetAdditionalData(true);
		d->DamageOverride = L.CheckInt(2);
		return 0;
	}
	int SetArmor(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookArmorMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookArmor = true;
		auto* d = b->GetAdditionalData(true);
		d->ArmorOverride = L.CheckInt(2);
		return 0;
	}
	int SetExploration(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookExplorationMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookExploration = true;
		auto* d = b->GetAdditionalData(true);
		d->ExplorationOverride = L.CheckFloat(2);
		return 0;
	}
	int LeaderSetRegeneration(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		GGL::CSettler* b = L.CheckSettler(1);
		if (!b->GetBehaviorDynamic<GGL::CLeaderBehavior>())
			throw lua::LuaException("no leader");
		GGL::CLeaderBehavior::HookLeaderRegen();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookRegen = true;
		auto* d = b->GetAdditionalData(true);
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
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookMaxRange = true;
		auto* d = b->GetAdditionalData(true);
		d->MaxRangeOverride = L.CheckFloat(2);
		return 0;
	}
	int SetDisplayName(lua::State l) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookDisplayName();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookDisplayName = true;
		auto* d = b->GetAdditionalData(true);
		d->NameOverride = L.CheckString(2);
		return 0;
	}
	int GetDisplayName(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* b = L.CheckEntity(1);
		auto* d = b->GetAdditionalData(false);
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
		if (!b->GetBehaviorDynamic<GGL::CLeaderBehavior>())
			throw lua::LuaException("no leader");
		L.Push(b->LeaderGetRegenHealth());
		L.Push(b->LeaderGetRegenHealthSeconds());
		return 2;
	}

	int EnableRangedEffectSoldierHeal(lua::State l) {
		luaext::EState L{ l };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		bool a = L.ToBoolean(1);
		GGL::CRangedEffectAbility::HookHealAffected(a);
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.RangedEffectSoldierHeal = a;
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

	int IsConstructionSite(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetClassIdentifier() == GGL::CConstructionSite::Identifier);
		return 1;
	}

	int BarracksBuyLeaderByType(lua::State l) {
		luaext::EState L{ l };
		GGL::CBuilding* e = L.CheckBuilding(1);
		auto rax = e->GetBehavior<GGL::CBarrackBehavior>();
		if (!rax)
			throw lua::LuaException("no barracks");
		GGlue::CGlueEntityProps* ety = L.CheckEntityType(2);
		GGL::CLeaderBehaviorProps* lp = ety->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!lp)
			throw lua::LuaException("no leader type");
		bool check = L.OptBool(4, true);
		if (!e->IsIdle(true, !check))
			throw lua::LuaException("building not idle");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId);
		if (check) {
			if (p->PlayerAttractionHandler->GetAttractionUsage() >= p->PlayerAttractionHandler->GetAttractionLimit())
				throw lua::LuaException("pop capped");
			if (p->PlayerAttractionHandler->IsMotivationLocked())
				throw lua::LuaException("motivation blocked");
		}
		if (!p->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(ety->LogicProps)->Cost))
			throw lua::LuaException("missing res");
		if (!L.OptBool(3, false)) {
			auto ucat = p->BuildingUpgradeManager->GetUpgradeCategoryOfEntityType(e->EntityType);
			if (lp->BarrackUpgradeCategory != ucat)
				throw lua::LuaException("leader type doesnt match barracks type");
		}
		auto id = rax->BuyLeaderByType(L.CheckEnum<shok::EntityTypeId>(2));
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
	void SettlerCreateAnimTaskList(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLETaskListMgr* tmng = *EGL::CGLETaskListMgr::GlobalObj;
		auto tid = CppLogic::GetIdManager<shok::TaskListId>().GetIdByName(AnimTaskList);
		if (tid == shok::TaskListId::Invalid) {
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
			L.GetGlobal();
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
		auto tid = CppLogic::GetIdManager<shok::TaskListId>().GetIdByName(AnimTaskList);
		if (tid != shok::TaskListId::Invalid) {
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
		setanim.AnimID = L.CheckEnum<shok::AnimationId>(2);
		if (setanim.AnimID == shok::AnimationId::Invalid)
			throw lua::LuaException("not an animation");
		setanim.PlayBackwards = L.OptBool(3, false);
		EGL::CGLETaskArgs reset{};
		reset.TaskType = shok::Task::TASK_RESET_TASK_LIST_TIMER;
		e->ExecuteTask(reset);
		e->ExecuteTask(setanim);
		GGL::CGLBehaviorAnimationEx* animbeh = e->GetBehaviorDynamic<GGL::CGLBehaviorAnimationEx>();
		animbeh->SpeedModifier = L.OptFloat(4, 1);
		animbeh->Duration = static_cast<int>(animbeh->Duration / animbeh->SpeedModifier);
		GGL::CBattleBehavior* batt = e->GetBehaviorDynamic<GGL::CBattleBehavior>();
		if (batt)
			batt->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
		e->SetTaskList(CppLogic::GetIdManager<shok::TaskListId>().GetIdByName(AnimTaskList));
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

	int BuildingGetRelativePositions(lua::State ls) {
		luaext::EState L{ ls };
		auto* b = L.CheckBuilding(1);
		auto* p = static_cast<GGL::CGLBuildingProps*>(b->GetEntityType()->LogicProps);
		shok::Position approach;
		b->GetApproachPos(&approach);
		L.PushPos(b->Position + approach);
		L.PushPos(b->Position + p->LeavePos);
		L.PushPos(b->Position + p->DoorPos);
		return 3;
	}

	int BuildingSpawnWorkerFor(lua::State ls) {
		luaext::EState L{ ls };
		auto* workplace = L.CheckBuilding(1);
		auto* spawner = L.CheckBuilding(2);
		auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(workplace->PlayerId);
		auto w = pl->PlayerAttractionHandler->PerformSpawnWorker(workplace, spawner);
		L.Push(w);
		return 1;
	}

	int MarketPredictPrice(lua::State ls) {
		luaext::EState L{ ls };
		auto* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::AdvancedMarketBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no adv market" };
		L.Push(b->PredictPriceFor(L.CheckEnum<shok::ResourceType>(2), L.CheckFloat(3)));
		return 1;
	}

	int MarketGetSellResources(lua::State ls) {
		luaext::EState L{ ls };
		auto* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::AdvancedMarketBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no adv market" };
		auto rt = L.CheckEnum<shok::ResourceType>(2);
		L.Push(b->SellResources.ByResT(rt));
		L.Push(b->MinResources.ByResT(rt));
		return 2;
	}


	void Cleanup(lua::State L) {
		DisableConversionHook(L);
		EGL::CGLEEntity::BuildingMaxHpTechBoni.clear();
		EGL::CGLEEntity::LastRemovedEntityAddonData.EntityId = shok::EntityId::Invalid;
		GGL::CRangedEffectAbility::HookHealAffected(false);
		SettlerCleanupAnimTask(L);
	}

	constexpr std::array Entity{
			lua::FuncReference::GetRef<GetScale>("GetScale"),
			lua::FuncReference::GetRef<SetScale>("SetScale"),
			lua::FuncReference::GetRef<MovingEntityGetTargetPos>("MovingEntityGetTargetPos"),
			lua::FuncReference::GetRef<MovingEntitySetTargetPos>("MovingEntitySetTargetPos"),
			lua::FuncReference::GetRef<MovingEntityIsFleeingFrom>("MovingEntityIsFleeingFrom"),
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
			lua::FuncReference::GetRef<RegionEntityIteratorRect>("RegionEntityIteratorRect"),
			lua::FuncReference::GetRef<RegionEntityIteratorCircle>("RegionEntityIteratorCircle"),
			lua::FuncReference::GetRef<EntityIteratorGetNearest>("EntityIteratorGetNearest"),
			lua::FuncReference::GetRef<EntityIteratorCount>("EntityIteratorCount"),
			lua::FuncReference::GetRef<CheckPredicate>("CheckPredicate"),
			lua::FuncReference::GetRef<GetBattleTarget>("GetBattleTarget"),
			lua::FuncReference::GetRef<GetAttackCommandTarget>("GetAttackCommandTarget"),
			lua::FuncReference::GetRef<Debug_GetTaskInfo>("Debug_GetTaskInfo"),
			lua::FuncReference::GetRef<GetTrackedResources>("GetTrackedResources"),
			lua::FuncReference::GetRef<GetAllScriptNameMappings>("GetAllScriptNameMappings"),
			lua::FuncReference::GetRef<GetLimitedAmmo>("GetLimitedAmmo"),
			lua::FuncReference::GetRef<SetLimitedAmmo>("SetLimitedAmmo"),
			lua::FuncReference::GetRef<DumpEntity>("DumpEntity"),
	};

	constexpr std::array<lua::FuncReference, 21> Predicates{ {
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
			lua::FuncReference::GetRef<PredicateIsBuildingOrConstructionSite>("PredicateIsBuildingOrConstructionSite"),
	} };

	constexpr std::array Settlers {
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
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::HeroHawk_SendHawk,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilitySendHawk>>>("CommandSendHawk"),
			lua::FuncReference::GetRef<SettlerGetHawkOfHero>("GetHawkOfHero"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::InflictFear_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityInflictFear>>>("CommandInflictFear"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::BombPlacer_CommandPlaceBomb,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>>>("CommandPlaceBomb"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<GGL::CEventPositionAnd2EntityTypes, shok::EventIDs::CannonBuilder_BuildCannonCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBuildCannon>, CheckPlaceCannonEvent>>("CommandPlaceCannon"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::RangedEffect_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityRangedEffect>>>("CommandRangedEffect"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::CircularAttack_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityCircularAttack>>>("CommandCircularAttack"),
			lua::FuncReference::GetRef<SettlerCommandSummon>("CommandSummon"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::Camouflage_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityCamouflage>, CheckCamoEvent>>("CommandActivateCamoflage"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::ConvertSettler_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityConvertSettlers>, LuaEventInterface::CheckSettler,
				LuaEventInterface::CheckConvertible, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("CommandConvert"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Sniper_SnipeCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilitySniper>, LuaEventInterface::CheckSettler,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>, CheckSnipeEvent>>("CommandSnipe"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Shuriken_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityShuriken>, LuaEventInterface::CheckSettler,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>, CheckShurikenEvent>>("CommandShuriken"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::MotivateVorkers_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityMotivateWorkers>>>("CommandMotivateWorkers"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::KegPlacer_SabotageCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceKeg>, LuaEventInterface::CheckBuilding,
				CheckThiefNotCarryingEvent, CheckSabotageEvent>>("CommandSabotage"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::KegPlacer_DefuseCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceKeg>,
				CheckThiefNotCarryingEvent, CheckDefuseEvent>>("CommandDefuse"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::Binocular_ExploreCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutBinoculars>>>("CommandBinocular"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::TorchPlacer_PlaceTorch,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutTorches>>>("CommandPlaceTorch"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::PointToResources_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutFindResources>>>("CommandPointToRes"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Thief_StealFromCommand,
				CheckIsThief, LuaEventInterface::CheckBuilding,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("CommandStealFrom"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Thief_StealFromCommand,
				CheckIsThief, LuaEventInterface::CheckBuilding, LuaEventInterface::CheckTargetCategory<shok::EntityCategory::Headquarters>,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("CommandSecureGoods"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::CppL_LightningStrike_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityLightningStrike>, CheckLightingStrike>>("CommandLightningStrike"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::CppL_ResDoodadRefill_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbiltyResourceDoodadRefill>, LuaEventInterface::CheckBuilding, CheckResourceRefill,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("CommandRefillResourceDoodad"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::CppL_ShieldCoverActivate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityShieldCover>>>("CommandShieldCover"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::CppL_Resurrect_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityResurrect>, CheckResurrect,
				LuaEventInterface::CheckEntitySamePlayer>>("CommandResurrect"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::CppL_Bombardment_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBombardment>, CheckBombardment>>("CommandBombardment"),
			lua::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::CppL_BombComboCannon_Activate,
				LuaEventInterface::CheckEntityBehavior<CppLogic::Mod::ReloadableCannonBuilderAbility>,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>>>("CommandBombCannonCombo"),
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
			lua::FuncReference::GetRef<WorkerGetMotivation>("WorkerGetMotivation"),
			lua::FuncReference::GetRef<WorkerChangeMotivation>("WorkerChangeMotivation"),
			lua::FuncReference::GetRef<WorkerGetResourceCarried>("WorkerGetResourceCarried"),
			lua::FuncReference::GetRef<SettlerGetSummoned>("SettlerGetSummoned"),
	};

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

	constexpr std::array Building{
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
			lua::FuncReference::GetRef<IsConstructionSite>("IsConstructionSite"),
			lua::FuncReference::GetRef<ConstructionSiteGetBuilding>("ConstructionSiteGetBuilding"),
			lua::FuncReference::GetRef<BarracksBuyLeaderByType>("BarracksBuyLeaderByType"),
			lua::FuncReference::GetRef<BuildingGetRelativePositions>("GetRelativePositions"),
			lua::FuncReference::GetRef<BuildingSpawnWorkerFor>("SpawnWorkerFor"),
			lua::FuncReference::GetRef<MarketPredictPrice>("MarketPredictPrice"),
			lua::FuncReference::GetRef<MarketGetSellResources>("MarketGetSellResources"),
	};

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

		if (L.GetState() != shok::LuaStateMainmenu) {
			L.GetSubTable("Events");
			L.Push("CPPLOGIC_EVENT_ON_CONVERT_ENTITY");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnConvert));
			L.SetTableRaw(-3);

			L.Pop(1);
		}
	}

	void OnSaveLoaded(lua::State L)
	{
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ConversionTrigger)
			EnableConversionHook(L);

		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookMaxHP)
			EGL::CGLEEntity::HookMaxHP();
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookDamage)
			EGL::CGLEEntity::HookDamageMod();
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookArmor)
			EGL::CGLEEntity::HookArmorMod();
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookExploration)
			EGL::CGLEEntity::HookExplorationMod();
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookRegen)
			GGL::CLeaderBehavior::HookLeaderRegen();
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookMaxRange)
			EGL::CGLEEntity::HookMaxRange();
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookDisplayName)
			EGL::CGLEEntity::HookDisplayName();

		GGL::CRangedEffectAbility::HookHealAffected(CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.RangedEffectSoldierHeal);
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

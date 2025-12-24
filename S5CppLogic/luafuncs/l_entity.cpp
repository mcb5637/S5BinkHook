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
	int PredicateOfType(luaext::State L) {
		auto ty = L.CheckEnum<shok::EntityTypeId>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfType>(ty);
		return 1;
	}

	int PredicateOfPlayer(luaext::State L) {
		auto pl = L.CheckPlayerId(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfPlayer>(pl);
		return 1;
	}

	int PredicateInCircle(luaext::State L) {
		float x = L.CheckFloat(1);
		float y = L.CheckFloat(2);
		float r = L.CheckFloat(3);
		L.NewUserClass<CppLogic::Iterator::PredicateInCircle<EGL::CGLEEntity>>(shok::Position{ x,y }, r * r);
		return 1;
	}

	int PredicateOfAnyPlayer(luaext::State L) {
		int num = L.GetTop();
		if (num > 9)
			throw lua::LuaException("too many players to check");
		auto* p = L.NewUserClass<CppLogic::Iterator::EntityPredicateOfAnyPlayer>();
		for (int i = 0; i < num; ++i) {
			p->players[i] = L.CheckPlayerId(i);
		}
		return 1;
	}

	int PredicateOfAnyEntityType(luaext::State L) {
		int num = L.GetTop();
		auto* p = L.NewUserClass<CppLogic::Iterator::EntityPredicateOfAnyType>();
		p->entityTypes.reserve(num);
		for (int i = 1; i <= num; ++i)
			p->entityTypes.push_back(L.CheckEnum<shok::EntityTypeId>(i));
		return 1;
	}

	int PredicateAnd(luaext::State L) {
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
	void PredicateAndAutoCreate(luaext::State L) { // clear stack after creating and predicate
		PredicateAnd(L);
		L.Insert(1);
		L.SetTop(1);
	}

	int PredicateOr(luaext::State L) {
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

	int PredicateNot(luaext::State L) {
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicateNot<EGL::CGLEEntity>>(pred);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateSetPriority(luaext::State L) {
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		int pri = L.CheckInt(2);
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicatePriority<EGL::CGLEEntity>>(pred, pri);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateIsSettler(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsSettler>();
		return 1;
	}

	int PredicateIsBuilding(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsBuildingAndNotConstructionSite>();
		return 1;
	}

	int PredicateIsBuildingOrConstructionSite(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsBuilding>();
		return 1;
	}

	int PredicateIsCombatRelevant(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsCombatRelevant>();
		return 1;
	}
	int PredicateIsNotSoldier(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsNotSoldier>();
		return 1;
	}

	int PredicateOfEntityCategory(luaext::State L) {
		auto c = L.CheckEnum<shok::EntityCategory>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfEntityCategory>(c);
		return 1;
	}

	int PredicateOfAnyEntityCategory(luaext::State L) {
		auto* p = L.NewUserClass<CppLogic::Iterator::EntityPredicateOfAnyEntityCategory>();
		for (int i = 1; i < L.GetTop(); ++i) {
			p->Categories.push_back(L.CheckEnum<shok::EntityCategory>(i));
		}
		return 1;
	}

	int PredicateOfAnyClass(luaext::State L) {
		auto* p = L.NewUserClass<CppLogic::Iterator::EntityPredicateOfAnyClass>();
		for (int i = 1; i < L.GetTop(); ++i) {
			p->Classes.push_back(L.CheckEnum<shok::ClassId>(i));
		}
		return 1;
	}

	int PredicateProvidesResource(luaext::State L) {
		auto ty = L.CheckEnum<shok::ResourceType>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateProvidesResource>(ty);
		return 1;
	}

	int PredicateInRect(luaext::State L) {
		float x1 = L.CheckFloat(1);
		float y1 = L.CheckFloat(2);
		float x2 = L.CheckFloat(3);
		float y2 = L.CheckFloat(4);
		L.NewUserClass<CppLogic::Iterator::PredicateInRect<EGL::CGLEEntity>>(x1, y1, x2, y2);
		return 1;
	}

	int PredicateIsVisible(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsVisible>();
		return 1;
	}

	int PredicateOfUpgradeCategory(luaext::State L) {
		auto ty = L.CheckEnum<shok::UpgradeCategoryId>(1);
		L.NewUserClass<CppLogic::Iterator::EntityPredicateOfUpgradeCategory>(ty);
		return 1;
	}

	int PredicateIsAlive(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsAlive>();
		return 1;
	}

	int PredicateIsNotInBuilding(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EntityPredicateIsNotInBuilding>();
		return 1;
	}

	int EntityIteratorTableize(luaext::State L) {
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

	int EntityIteratorCount(luaext::State L) {
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAndAutoCreate(L);
		}
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		CppLogic::Iterator::GlobalEntityIterator it{ pred };
		L.Push(it.Count());
		return 1;
	}

	int EntityIteratorGetNearest(luaext::State L) {
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

	int EntityIteratorNext(luaext::State L) { // (state nil, last value) -> next value
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
	int LEntityIterator(luaext::State L) {
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

	int PlayerEntityIterator(luaext::State L) {
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

	int RegionEntityIteratorRect(luaext::State L) {
		shok::AARect area{ L.CheckPos(1), L.CheckPos(2) };
		auto acf = static_cast<shok::AccessCategoryFlags>(L.CheckInt(3));
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
	int RegionEntityIteratorCircle(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		float r = L.CheckFloat(2);
		auto acf = static_cast<shok::AccessCategoryFlags>(L.CheckInt(3));
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

	int CheckPredicate(luaext::State L) {
		EGL::CGLEEntity* s = L.CheckEntity(1);
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CGLEEntity>>(1);
		float r = -1;
		int pr = -1;
		L.Push(pred->Matches(s, &r, &pr));
		L.Push(r);
		L.Push(pr);
		return 3;
	}

	int SettlerGetLeaderOfSoldier(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		L.Push(s->GetFirstAttachedToMe(shok::AttachmentType::LEADER_SOLDIER));
		return 1;
	}

	int LeaderGetExperience(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		L.Push(b->Experience);
		return 1;
	}
	int LeaderSetExperience(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		b->Experience = L.CheckInt(2);
		return 0;
	}

	int LeaderGetTroopHealth(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		L.Push(b->GetTroopHealth());
		L.Push(b->GetTroopHealthPerSoldier());
		return 2;
	}
	int LeaderSetTroopHealth(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* b = s->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!b)
			throw lua::LuaException("no leader at 1");
		b->TroopHealthCurrent = L.CheckInt(2);
		return 0;
	}

	int SettlerGetBaseMovementSpeed(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* b = s->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
		if (!b)
			throw lua::LuaException("no moving entity at 1");
		L.Push(b->MovementSpeed);
		L.Push(CppLogic::RadiansToDegrees(b->TurningSpeed));
		return 2;
	}
	int SettlerSetBaseMovementSpeed(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* b = s->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
		if (!b)
			throw lua::LuaException("no moving entity at 1");
		if (L.IsNumber(2))
			b->MovementSpeed = L.CheckFloat(2);
		if (L.IsNumber(3))
			b->TurningSpeed = static_cast<float>(CppLogic::DegreesToRadians(*L.ToNumber(3)));
		return 0;
	}

	int BuildingGetBarracksAutoFillActive(luaext::State L) {
		GGL::CBuilding* e = L.CheckBuilding(1);
		auto* b = e->GetBehavior<GGL::CBarrackBehavior>();
		if (!b)
			throw lua::LuaException("no barracks at 1");
		L.Push(b->AutoFillActive);
		return 1;
	}

	int GetScale(luaext::State L) {
		EGL::CGLEEntity* s = L.CheckEntity(1);
		L.Push(s->Scale);
		return 1;
	}
	int SetScale(luaext::State L) {
		EGL::CGLEEntity* s = L.CheckEntity(1);
		s->Scale = L.CheckFloat(2);
		return 0;
	}

	int BuildingGetHeight(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		L.Push(b->ConstructionProgress);
		return 1;
	}
	int BuildingSetHeight(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		b->ConstructionProgress = L.CheckFloat(2);
		return 0;
	}

	int SettlerGetOverheadWidget(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		s->ModifierProfile.EntityReference.CheckInit();
		L.Push(s->ModifierProfile.EntityReference.OverheadWidget);
		return 1;
	}
	int SettlerSetOverheadWidget(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		int ov = L.CheckInt(2);
		if (!(ov >= 0 && ov <= 4))
			throw lua::LuaException("invalid overhead code");
		s->ModifierProfile.EntityReference.CheckInit();
		s->ModifierProfile.EntityReference.OverheadWidget = ov;
		return 1;
	}

	int MovingEntityGetTargetPos(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* m = dynamic_cast<EGL::CMovingEntity*>(e);
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		L.Push(m->TargetPosition);
		if (m->TargetRotationValid) {
			L.Push("r");
			L.Push(CppLogic::RadiansToDegrees(m->TargetRotation));
			L.SetTableRaw(-3);
		}
		return 1;
	}
	int MovingEntitySetTargetPos(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* m = dynamic_cast<EGL::CMovingEntity*>(e);
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
			m->TargetRotationValid = false;
			EGL::CEventValue_Bool ev{ shok::EventIDs::Leader_SetIsUsingTargetOrientation, false };
			m->FireEvent(&ev);
		}
		return 0;
	}

	int MovingEntityIsFleeingFrom(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* m = dynamic_cast<EGL::CMovingEntity*>(e);
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		auto p = L.CheckPos(2);
		L.Push(m->IsFleeingFrom(p, L.OptFloat(3, 500.0f)));
		return 1;
	}

	int HeroGetCamouflageDurationLeft(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* c = s->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no camo hero at 1");
		L.Push(c->InvisibilityRemaining);
		return 1;
	}
	int HeroSetCamouflageDurationLeft(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* c = s->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no camo hero at 1");
		if (dynamic_cast<GGL::CThiefCamouflageBehavior*>(c))
			throw lua::LuaException("thief at 1, use ThiefSetCamouflageTimeTo instead");
		c->InvisibilityRemaining = L.CheckInt(2);
		return 1;
	}

	int ThiefGetCamouflageTimeTo(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* c = s->GetBehavior<GGL::CThiefCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no thief at 1");
		L.Push(c->TimeToInvisibility);
		return 1;
	}
	int ThiefSetCamouflageTimeTo(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* c = s->GetBehavior<GGL::CThiefCamouflageBehavior>();
		if (!c)
			throw lua::LuaException("no thief at 1");
		int i = L.CheckInt(2);
		c->TimeToInvisibility = i;
		c->InvisibilityRemaining = i <= 0 ? 15 : 0;
		return 0;
	}

	int HeroGetResurrectionTime(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* h = s->GetBehavior<GGL::CHeroBehavior>();
		if (!h)
			throw lua::LuaException("no hero at 1");
		L.Push(h->ResurrectionTimePassed);
		return 1;
	}
	int HeroSetResurrectionTime(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* h = s->GetBehavior<GGL::CHeroBehavior>();
		if (!h)
			throw lua::LuaException("no hero at 1");
		h->ResurrectionTimePassed = L.CheckInt(2);
		return 0;
	}

	int GetLimitedLifespanRemaining(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<GGL::CLimitedLifespanBehavior>();
		if (!b)
			throw lua::LuaException("no limited lifespan at 1");
		L.Push(b->RemainingLifespanSeconds);
		return 1;
	}
	int SetLimitedLifespanRemaining(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<GGL::CLimitedLifespanBehavior>();
		if (!b)
			throw lua::LuaException("no limited lifespan at 1");
		b->RemainingLifespanSeconds = L.CheckInt(2);
		return 0;
	}

	int GetTaskListIndex(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->CurrentTaskIndex);
		return 1;
	}
	int SetTaskListIndex(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		e->CurrentTaskIndex = L.CheckInt(2);
		return 0;
	}

	int Debug_GetTaskInfo(luaext::State L) {
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

	int GetTrackedResources(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::ResourceTrackerBehavior>();
		if (!b)
			return 0;
		L.Push(b->Produced);
		L.Push(b->Used);
		return 2;
	}

	int GetAllScriptNameMappings(luaext::State L) {
		auto& m = (*EGL::CGLEEntityManager::GlobalObj)->ScriptName;
		L.NewTable();
		for (auto& [str, id] : m) {
			L.Push(str);
			L.Push(id);
			L.SetTableRaw(-3);
		}
		return 1;
	}

	int GetLimitedAmmo(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::LimitedAmmoBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no limited ammo" };
		L.Push(b->RemainingAmmo);
		return 1;
	};
	int SetLimitedAmmo(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::LimitedAmmoBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no limited ammo" };
		b->RemainingAmmo = L.CheckInt(2);
		return 0;
	};

	int DumpEntity(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, e);
		return 1;
	}

	int MovingEntityGetSpeedFactor(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* m = e->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		L.Push(m->SpeedFactor);
		return 1;
	}

	int WorkerGetCurrentWorkTime(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		L.Push(w->WorkTimeRemaining);
		L.Push(s->EventGetMaxWorktime());
		return 2;
	}
	int WorkerSetCurrentWorkTime(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		w->WorkTimeRemaining = L.CheckInt(2);
		return 0;
	}

	int WorkerGetMotivation(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		L.Push(s->EventGetMotivation());
		return 1;
	}
	int WorkerChangeMotivation(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* w = s->GetBehavior<GGL::CWorkerBehavior>();
		if (!w)
			throw lua::LuaException("no worker at 1");
		int r = L.CheckInt(3);
		if (r < static_cast<int>(shok::WorkerReason::None) || r > static_cast<int>(shok::WorkerReason::NoPay))
			throw lua::LuaException{ "invalid reason" };
		GGL::CEventChangeMotivation ev{ shok::EventIDs::Worker_ChangeMoti, L.CheckFloat(2), static_cast<shok::WorkerReason>(r) };
		s->FireEvent(&ev);
		return 0;
	}

	int WorkerGetResourceCarried(luaext::State L) {
		auto* e = L.CheckSettler(1);
		auto* b = e->GetBehavior<GGL::CWorkerBehavior>();
		if (!b)
			throw lua::LuaException{ "not a worker" };
		L.Push(b->CarriedResourceAmount);
		L.Push(b->CouldConsumeResource);
		return 2;
	}

	int SettlerGetSummoned(luaext::State L) {
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

	int BuildingMarketGetCurrentTradeData(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		auto* m = b->GetBehavior<GGL::CMarketBehavior>();
		if (!m)
			throw lua::LuaException("no market at 1");
		L.Push(static_cast<int>(m->CurrentTrade.BuyResourceType));
		L.Push(static_cast<int>(m->CurrentTrade.SellResourceType));
		L.Push(m->CurrentTrade.BuyAmount);
		L.Push(m->CurrentTrade.SellAmount);
		L.Push(m->CurrentTrade.ProgressAmount);
		return 5;
	}
	int BuildingMarketSetCurrentTradeData(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		auto* m = b->GetBehavior<GGL::CMarketBehavior>();
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

	int IsSoldier(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetBehavior<GGL::CSoldierBehavior>() != nullptr);
		return 1;
	}

	int HeroResurrect(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* h = e->GetBehavior<GGL::CHeroBehavior>();
		if (!h)
			throw lua::LuaException("no hero at 1");
		h->ResurrectionTimePassed = 20000;
		h->EnemyNear = false;
		h->FriendNear = true;
		return 0;
	}

	int ThiefSetStolenResourceInfo(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* t = e->GetBehavior<GGL::CThiefBehavior>();
		if (!t)
			throw lua::LuaException("no thief at 1");
		auto ty = L.CheckEnum<shok::ResourceType>(2, true);
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

	int GetAutoAttackMaxRange(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* b = e->GetBehaviorDynamic<GGL::CBattleBehavior>();
		if (b) {
			L.Push(b->GetMaxRange());
			return 1;
		}
		auto* a = e->GetBehavior<GGL::CAutoCannonBehavior>();
		if (a) {
			L.Push(a->GetMaxRange());
			return 1;
		}
		throw lua::LuaException("no battle entity or autocannon at 1");
	}

	int GetModel(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto m = e->ModelOverride;
		if (m == shok::ModelId::Invalid) {
			m = e->GetEntityType()->DisplayProps->Model[0];
		}
		L.Push(m);
		return 1;
	}

	int GetExploration(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetExploration());
		return 1;
	}

	int GetSpeed(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* m = e->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>();
		if (!m)
			throw lua::LuaException("no moving entity at 1");
		L.Push(m->GetMovementSpeed() * 10.0);
		return 1;
	}

	int SettlerIsVisible(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto* c = e->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
		if (c != nullptr) {
			L.Push(c->InvisibilityRemaining <= 0);
			return 1;
		}
		L.Push(true);
		return 1;
	}

	int SettlerGetHawkOfHero(luaext::State L) {
		GGL::CSettler* e = L.CheckSettler(1);
		auto* b = e->GetBehavior<GGL::CHeroHawkBehavior>();
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

	int SettlerCommandSummon(luaext::State L) {
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
		auto* b = e->GetBehaviorDynamic<GGL::CCamouflageBehavior>();
		if (dynamic_cast<GGL::CThiefCamouflageBehavior*>(b))
			throw lua::LuaException("thief camo cannot be manually activated");
	}

	void CheckSnipeEvent(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		auto* bp = e->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!e->Position.IsInRange(oth->Position, bp->Range))
			throw lua::LuaException("target not in range");
	}

	void CheckShurikenEvent(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		auto* bp = e->GetEntityType()->GetBehaviorProps<GGL::CShurikenAbilityProps>();
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
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::LightningStrikeAbilityProps>();
		if (!e->Position.IsInRange(ev.Position, bp->Range))
			throw lua::LuaException("not in range");
		if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId)->CurrentResources.WeatherEnergy < bp->WeatherEnergyCost)
			throw lua::LuaException("not enough weather energy");
	}
	void CheckResourceRefill(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
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
		auto* bp = e->GetEntityType()->GetBehaviorProps<CppLogic::Mod::BombardmentAbilityProps>();
		if (!e->Position.IsInRange(ev.Position, bp->AttackRange))
			throw lua::LuaException("not in range");
	}

	int EnableConversionHook(luaext::State L) {
		GGL::CConvertSettlerAbility::HookConvertEvent();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ConversionTrigger = true;
		return 0;
	}
	int DisableConversionHook(luaext::State L) {
		return 0;
	}

	int IsFeared(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		auto id = e->GetFirstAttachedToMe(shok::AttachmentType::INFLICTOR_TERRORIZED);
		if (id == shok::EntityId::Invalid)
			L.Push(false);
		else
			L.Push(id);
		return 1;
	}

	int SettlerCommandMove(luaext::State L) {
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

	int ClearAttackers(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		e->ClearAttackers();
		return 0;
	}

	int BuildingCommandFoundryBuildCannon(luaext::State L) {
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
		if (!p->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(t->LogicProps)->Cost)) // NOLINT(*-pro-type-static-cast-downcast)
			throw lua::LuaException("missing res");
		if (p->PlayerAttractionHandler->GetAttractionUsage() >= p->PlayerAttractionHandler->GetAttractionLimit())
			throw lua::LuaException("pop capped");
		b->CommandBuildCannon(ety);
		return 0;
	}

	int SettlerIsIdle(luaext::State L) {
		GGL::CSettler* e = L.CheckSettler(1);
		L.Push(e->IsIdle());
		return 1;
	}

	int BuildingGetNearestFreeConstructionSlotFor(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		shok::Position p = L.CheckPos(2);
		L.Push(b->GetNearestFreeConstructionSlotFor(&p));
		return 1;
	}

	int SettlerCommandSerfConstructBuilding(luaext::State L) {
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

	int BuildingGetNearestFreeRepairSlotFor(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		shok::Position p = L.CheckPos(2);
		L.Push(b->GetNearestFreeRepairSlotFor(&p));
		return 1;
	}

	int SettlerCommandSerfRepairBuilding(luaext::State L) {
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

	int ReplaceWithResourceEntity(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		e = EGL::CGLEEntity::ReplaceEntityWithResourceEntity(e);
		if (e)
			L.Push(e->EntityId);
		else
			L.Push();
		return 1;
	}

	int SettlerCommandSerfExtract(luaext::State L) {
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

	int BuildingStartUpgrade(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsIdle())
			throw lua::LuaException("is not idle");
		b->StartUpgrade();
		return 0;
	}

	int BuildingCancelUpgrade(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsUpgrading)
			throw lua::LuaException("not upgrading");
		b->CancelUpgrade();
		return 0;
	}

	int BuildingIsIdle(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		L.Push(b->IsIdle());
		return 1;
	}

	int BarracksGetLeadersTrainingAt(luaext::State L) {
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

	int FoundryGetCannonTypeInConstruction(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		auto* f = b->GetBehavior<GGL::CFoundryBehavior>();
		if (!f)
			throw lua::LuaException("no foundry");
		L.Push(f->CannonType);
		return 1;
	}

	int SettlerCommandExpell(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		s->SettlerExpell();
		return 0;
	}

	int BarracksBuySoldierForLeader(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		GGL::CSettler* s = L.CheckSettler(2);
		if (!b->GetBehavior<GGL::CBarrackBehavior>())
			throw lua::LuaException("no barracks");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading))
			throw lua::LuaException("barracks is upgrading or under construction");
		auto* l = s->GetBehavior<GGL::CLeaderBehavior>();
		auto* lp = s->GetEntityType()->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
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
			auto* sprop = dynamic_cast<GGL::CGLSettlerProps*>(solty->LogicProps);
			if (!sprop)
				throw lua::LuaException("error: soldier no settler type");
			if (!p->CurrentResources.HasResources(&sprop->Cost))
				throw lua::LuaException("missing res");
		}
		b->CommandRecruitSoldierForLeader(s->EntityId);
		return 0;
	}

	int LeaderAttachSoldier(luaext::State L) {
		GGL::CSettler* l = L.CheckSettler(1);
		GGL::CSettler* s = L.CheckSettler(2);
		auto* lb = l->GetBehaviorDynamic<GGL::CLeaderBehavior>();
		if (!lb)
			throw lua::LuaException("no leader");
		if (!s->GetBehavior<GGL::CSoldierBehavior>())
			throw lua::LuaException("no soldier");
		if (s->EntityType != l->GetEntityType()->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>()->SoldierType)
			throw lua::LuaException("leader and soldier type doesnt match");
		l->LeaderAttachSoldier(s->EntityId);
		return 0;
	}

	int SettlerCommandTurnSerfToBattleSerf(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		if (!s->GetBehavior<GGL::CSerfBehavior>())
			throw lua::LuaException("no serf");
		s->SerfTurnToBattleSerf();
		return 0;
	}

	int SettlerCommandTurnBattleSerfToSerf(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		if (!s->GetBehavior<GGL::CBattleSerfBehavior>())
			throw lua::LuaException("no battleserf");
		s->BattleSerfTurnToSerf();
		return 0;
	}

	int BuildingActivateOvertime(luaext::State L) {
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

	int BuildingDeactivateOvertime(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsOvertimeActive)
			throw lua::LuaException("overtime not active");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading && b->CurrentState != shok::TaskState::BuildingAlarmDefend))
			throw lua::LuaException("building not idle");
		b->DeactivateOvertime();
		return 0;
	}

	int BarracksRecruitLeaders(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		auto* r = b->GetBehavior<GGL::CBarrackBehavior>();
		if (!r)
			throw lua::LuaException("no barracks");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading))
			throw lua::LuaException("building not idle");
		if (!r->AutoFillActive)
			throw lua::LuaException("already recruiting leaders");
		b->BarracksRecruitLeaders();
		return 0;
	}
	int BarracksRecruitGroups(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		auto* r = b->GetBehavior<GGL::CBarrackBehavior>();
		if (!r)
			throw lua::LuaException("no barracks");
		if (!(b->IsConstructionFinished() && !b->IsUpgrading))
			throw lua::LuaException("building not idle");
		if (r->AutoFillActive)
			throw lua::LuaException("already recruiting groups");
		b->BarracksRecruitGroups();
		return 0;
	}

	int HQBuySerf(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->IsEntityInCategory(shok::EntityCategory::Headquarters))
			throw lua::LuaException("no hq");
		if (!b->IsIdle())
			throw lua::LuaException("building not idle");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
		if (p->PlayerAttractionHandler->GetAttractionUsage() >= p->PlayerAttractionHandler->GetAttractionLimit())
			throw lua::LuaException("pop capped");
		GGlue::CGlueEntityProps* solty = CppLogic::GetEntityType(*GGlue::CGlueEntityProps::EntityTypeIDSerf);
		if (!p->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(solty->LogicProps)->Cost)) // NOLINT(*-pro-type-static-cast-downcast)
			throw lua::LuaException("missing res");
		b->HQBuySerf();
		return 0;
	}

	int SellBuilding(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!static_cast<GGL::CGLBuildingProps *>(b->GetEntityType()->LogicProps)->CanBeSold) // NOLINT(*-pro-type-static-cast-downcast)
			throw lua::LuaException("cannot be sold");
		b->SellBuilding();
		return 0;
	}

	int BuildingStartResearch(luaext::State L) {
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

	int BuildingCancelResearch(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (b->GetTechnologyInResearch() != shok::TechnologyId::Invalid)
			throw lua::LuaException("no tech in research");
		b->CancelResearch();
		return 0;
	}

	bool MarketIsRes(shok::ResourceType rty) {
		return rty == shok::ResourceType::Clay || rty == shok::ResourceType::Gold || rty == shok::ResourceType::Iron || rty == shok::ResourceType::Stone || rty == shok::ResourceType::Sulfur || rty == shok::ResourceType::Wood;
	}

	int MarketStartTrade(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->GetBehavior<GGL::CMarketBehavior>())
			throw lua::LuaException("no market at 1");
		if (!b->IsIdle())
			throw lua::LuaException("building not idle");
		auto sellty = static_cast<shok::ResourceType>(L.CheckInt(2));
		auto buyty = static_cast<shok::ResourceType>(L.CheckInt(3));
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

	int MarketCancelTrade(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		if (!b->GetBehavior<GGL::CMarketBehavior>())
			throw lua::LuaException("no market at 1");
		if (b->GetMarketProgress() >= 1.0f)
			throw lua::LuaException("no transaction in progress");
		b->MarketCancelTrade();
		return 0;
	}

	int SettlerSetPosition(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		shok::Position p = L.CheckPos(2);
		if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p) == shok::SectorId::Invalid)
			throw lua::LuaException("position is blocked");
		s->SetPosition(p);
		return 0;
	}

	int LeaderSetSoldierLimit(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* l = s->GetBehavior<GGL::CLimitedAttachmentBehavior>();
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

	int MercenaryRemoveLastOffer(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		auto* m = b->GetBehaviorDynamic<GGL::CBuildingMerchantBehavior>();
		if (!m)
			throw lua::LuaException("no merchant");
		if (m->Offer.size() == 0)
			throw lua::LuaException("is empty");
		delete m->Offer[m->Offer.size() - 1];
		auto v = m->Offer.SaveVector();
		v.Vector.pop_back();
		return 0;
	}
	int MercenarySetOfferData(luaext::State L) {
		GGL::CBuilding* b = L.CheckBuilding(1);
		auto* m = b->GetBehaviorDynamic<GGL::CBuildingMerchantBehavior>();
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

	int SetMaxHP(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
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
	int CloneOverrideData(luaext::State L) {
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
	int SetDamage(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookDamageMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookDamage = true;
		auto* d = b->GetAdditionalData(true);
		d->DamageOverride = L.CheckInt(2);
		return 0;
	}
	int SetArmor(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookArmorMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookArmor = true;
		auto* d = b->GetAdditionalData(true);
		d->ArmorOverride = L.CheckInt(2);
		return 0;
	}
	int SetExploration(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookExplorationMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookExploration = true;
		auto* d = b->GetAdditionalData(true);
		d->ExplorationOverride = L.CheckFloat(2);
		return 0;
	}
	int LeaderSetRegeneration(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
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
	int SetAutoAttackMaxRange(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookMaxRange();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookMaxRange = true;
		auto* d = b->GetAdditionalData(true);
		d->MaxRangeOverride = L.CheckFloat(2);
		return 0;
	}
	int SetDisplayName(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity* b = L.CheckEntity(1);
		EGL::CGLEEntity::HookDisplayName();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HookDisplayName = true;
		auto* d = b->GetAdditionalData(true);
		d->NameOverride = L.CheckString(2);
		return 0;
	}
	int GetDisplayName(luaext::State L) {
		EGL::CGLEEntity* b = L.CheckEntity(1);
		auto* d = b->GetAdditionalData(false);
		if (d && !d->NameOverride.empty())
			L.Push(d->NameOverride);
		else
			L.Push(EGL::CGLEEntitiesProps::GetEntityTypeDisplayName(b->EntityType));
		return 1;
	}

	int LeaderGetRegeneration(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		GGL::CSettler* b = L.CheckSettler(1);
		if (!b->GetBehaviorDynamic<GGL::CLeaderBehavior>())
			throw lua::LuaException("no leader");
		L.Push(b->LeaderGetRegenHealth());
		L.Push(b->LeaderGetRegenHealthSeconds());
		return 2;
	}

	int EnableRangedEffectSoldierHeal(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		bool a = L.ToBoolean(1);
		GGL::CRangedEffectAbility::HookHealAffected(a);
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.RangedEffectSoldierHeal = a;
		return 0;
	}

	int PerformHeal(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		int h = L.CheckInt(2);
		if (h <= 0)
			throw lua::LuaException("heal is <=0");
		e->PerformHeal(h, L.ToBoolean(3));
		return 0;
	}

	int BuildingGetBuildOnEntity(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckBuilding(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::BUILDING_BASE));
		return 1;
	}
	int BuildOnEntityGetBuilding(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedToMe(shok::AttachmentType::BUILDING_BASE));
		return 1;
	}

	int BuildingGetConstructionSite(luaext::State L) {
		GGL::CBuilding* e = L.CheckBuilding(1);
		L.Push(e->GetConstructionSite());
		return 1;
	}
	int ConstructionSiteGetBuilding(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::CONSTRUCTION_SITE_BUILDING));
		return 1;
	}

	int IsConstructionSite(luaext::State L) {
		EGL::CGLEEntity* e = L.CheckEntity(1);
		L.Push(e->GetClassIdentifier() == GGL::CConstructionSite::Identifier);
		return 1;
	}

	int BarracksBuyLeaderByType(luaext::State L) {
		GGL::CBuilding* e = L.CheckBuilding(1);
		auto rax = e->GetBehavior<GGL::CBarrackBehavior>();
		if (!rax)
			throw lua::LuaException("no barracks");
		GGlue::CGlueEntityProps* ety = L.CheckEntityType(2);
		auto* lp = ety->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
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
		if (!p->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(ety->LogicProps)->Cost)) // NOLINT(*-pro-type-static-cast-downcast)
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

	int SettlerShurikenGetTarget(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* beh = s->GetBehavior<GGL::CShurikenAbility>();
		if (!beh)
			throw lua::LuaException("no shuriken ability");
		L.Push(beh->TargetId);
		return 1;
	}
	int SettlerSniperGetTarget(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		auto* beh = s->GetBehavior<GGL::CSniperAbility>();
		if (!beh)
			throw lua::LuaException("no snipe ability");
		L.Push(beh->TargetId);
		return 1;
	}

	int SettlerGetEnteredBuilding(luaext::State L) {
		GGL::CSettler* s = L.CheckSettler(1);
		L.Push(s->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING));
		L.Push(s->GetFirstAttachedEntity(shok::AttachmentType::SETTLER_BUILDING_TO_LEAVE));
		return 2;
	}

	const char* AnimTaskList = "TL_SCRIPT_ANIMATION";
	void SettlerCreateAnimTaskList(luaext::State L) {
		EGL::CGLETaskListMgr* tmng = *EGL::CGLETaskListMgr::GlobalObj;
		auto tid = CppLogic::GetIdManager<shok::TaskListId>().GetIdByName(AnimTaskList);
		if (tid == shok::TaskListId::Invalid) {
			BB::CClassFactory* fact = *BB::CClassFactory::GlobalObj;
			auto* tl = fact->CreateObject<EGL::CGLETaskList>();

			{
				auto v = tl->Task.SaveVector();
				{
					auto* t = fact->CreateObject<EGL::CGLETaskArgsThousandths>();
					t->TaskType = shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE;
					t->Thousandths = 1000;
					v.Vector.push_back(t);
				}
				{
					auto* t = fact->CreateObject<EGL::CGLETaskArgs>();
					t->TaskType = shok::Task::TASK_SET_BATTLE_IDLE_ANIM;
					v.Vector.push_back(t);
				}
				{
					auto* t = fact->CreateObject<EGL::CGLETaskArgs>();
					t->TaskType = shok::Task::TASK_BATTLE_WAIT_UNTIL;
					v.Vector.push_back(t);
				}
				{
					auto* t = fact->CreateObject<EGL::CGLETaskArgs>();
					t->TaskType = shok::Task::TASK_SET_DEFAULT_REACTION_TYPE;
					v.Vector.push_back(t);
				}
				{
					auto* t = fact->CreateObject<EGL::CGLETaskArgs>();
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
	void SettlerCleanupAnimTask(luaext::State L) {
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
	int SettlerPlayScriptAnimation(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");
		SettlerCreateAnimTaskList(L);
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
		auto* animbeh = e->GetBehaviorDynamic<GGL::CGLBehaviorAnimationEx>();
		animbeh->SpeedModifier = L.OptFloat(4, 1);
		animbeh->Duration = static_cast<int>(static_cast<float>(animbeh->Duration) / animbeh->SpeedModifier);
		auto* batt = e->GetBehaviorDynamic<GGL::CBattleBehavior>();
		if (batt)
			batt->SetCurrentCommand(shok::LeaderCommand::HeroAbility);
		e->SetTaskList(CppLogic::GetIdManager<shok::TaskListId>().GetIdByName(AnimTaskList));
		return 0;
	}

	int SettlerGetExperienceClass(luaext::State L) {
		auto* s = L.CheckSettler(1);
		s->ModifierProfile.EntityReference.CheckInit();
		L.Push(static_cast<int>(s->ModifierProfile.EntityReference.ExperienceClass));
		return 1;
	}

	int GetBattleTarget(luaext::State L) {
		auto* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::ATTACKER_TARGET));
		return 1;
	}
	int GetAttackCommandTarget(luaext::State L) {
		auto* e = L.CheckEntity(1);
		L.Push(e->GetFirstAttachedEntity(shok::AttachmentType::LEADER_TARGET));
		return 1;
	}

	int BuildingGetRelativePositions(luaext::State L) {
		auto* b = L.CheckBuilding(1);
		auto* p = static_cast<GGL::CGLBuildingProps*>(b->GetEntityType()->LogicProps); // NOLINT(*-pro-type-static-cast-downcast)
		shok::Position approach;
		b->GetApproachPos(&approach);
		L.Push(b->Position + approach);
		L.Push(b->Position + p->LeavePos);
		L.Push(b->Position + p->DoorPos);
		return 3;
	}

	int BuildingSpawnWorkerFor(luaext::State L) {
		auto* workplace = L.CheckBuilding(1);
		auto* spawner = L.CheckBuilding(2);
		auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(workplace->PlayerId);
		auto w = pl->PlayerAttractionHandler->PerformSpawnWorker(workplace, spawner);
		L.Push(w);
		return 1;
	}

	int MarketPredictPrice(luaext::State L) {
		auto* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::AdvancedMarketBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no adv market" };
		L.Push(b->PredictPriceFor(L.CheckEnum<shok::ResourceType>(2), L.CheckFloat(3)));
		return 1;
	}

	int MarketGetSellResources(luaext::State L) {
		auto* e = L.CheckEntity(1);
		auto* b = e->GetBehavior<CppLogic::Mod::AdvancedMarketBehavior>();
		if (b == nullptr)
			throw lua::LuaException{ "no adv market" };
		auto rt = L.CheckEnum<shok::ResourceType>(2);
		L.Push(b->SellResources.ByResT(rt));
		L.Push(b->MinResources.ByResT(rt));
		return 2;
	}


	void Cleanup(luaext::State L) {
		DisableConversionHook(L);
		EGL::CGLEEntity::BuildingMaxHpTechBoni.clear();
		EGL::CGLEEntity::LastRemovedEntityAddonData.EntityId = shok::EntityId::Invalid;
		GGL::CRangedEffectAbility::HookHealAffected(false);
		SettlerCleanupAnimTask(L);
	}

	constexpr std::array Entity{
			luaext::FuncReference::GetRef<GetScale>("GetScale"),
			luaext::FuncReference::GetRef<SetScale>("SetScale"),
			luaext::FuncReference::GetRef<MovingEntityGetTargetPos>("MovingEntityGetTargetPos"),
			luaext::FuncReference::GetRef<MovingEntitySetTargetPos>("MovingEntitySetTargetPos"),
			luaext::FuncReference::GetRef<MovingEntityIsFleeingFrom>("MovingEntityIsFleeingFrom"),
			luaext::FuncReference::GetRef<GetLimitedLifespanRemaining>("GetLimitedLifespanRemaining"),
			luaext::FuncReference::GetRef<SetLimitedLifespanRemaining>("SetLimitedLifespanRemaining"),
			luaext::FuncReference::GetRef<GetTaskListIndex>("GetTaskListIndex"),
			luaext::FuncReference::GetRef<SetTaskListIndex>("SetTaskListIndex"),
			luaext::FuncReference::GetRef<MovingEntityGetSpeedFactor>("MovingEntityGetSpeedFactor"),
			luaext::FuncReference::GetRef<IsSoldier>("IsSoldier"),
			luaext::FuncReference::GetRef<GetAutoAttackMaxRange>("GetAutoAttackMaxRange"),
			luaext::FuncReference::GetRef<GetModel>("GetModel"),
			luaext::FuncReference::GetRef<GetExploration>("GetExploration"),
			luaext::FuncReference::GetRef<GetSpeed>("GetSpeed"),
			luaext::FuncReference::GetRef<IsFeared>("IsFeared"),
			luaext::FuncReference::GetRef<ClearAttackers>("ClearAttackers"),
			luaext::FuncReference::GetRef<ReplaceWithResourceEntity>("ReplaceWithResourceEntity"),
			luaext::FuncReference::GetRef<SetMaxHP>("SetMaxHP"),
			luaext::FuncReference::GetRef<CloneOverrideData>("CloneOverrideData"),
			luaext::FuncReference::GetRef<SetDamage>("SetDamage"),
			luaext::FuncReference::GetRef<SetArmor>("SetArmor"),
			luaext::FuncReference::GetRef<SetExploration>("SetExploration"),
			luaext::FuncReference::GetRef<SetAutoAttackMaxRange>("SetAutoAttackMaxRange"),
			luaext::FuncReference::GetRef<SetDisplayName>("SetDisplayName"),
			luaext::FuncReference::GetRef<GetDisplayName>("GetDisplayName"),
			luaext::FuncReference::GetRef<PerformHeal>("PerformHeal"),
			luaext::FuncReference::GetRef<EntityIteratorTableize>("EntityIteratorTableize"),
			luaext::FuncReference::GetRef<LEntityIterator>("EntityIterator"),
			luaext::FuncReference::GetRef<PlayerEntityIterator>("PlayerEntityIterator"),
			luaext::FuncReference::GetRef<RegionEntityIteratorRect>("RegionEntityIteratorRect"),
			luaext::FuncReference::GetRef<RegionEntityIteratorCircle>("RegionEntityIteratorCircle"),
			luaext::FuncReference::GetRef<EntityIteratorGetNearest>("EntityIteratorGetNearest"),
			luaext::FuncReference::GetRef<EntityIteratorCount>("EntityIteratorCount"),
			luaext::FuncReference::GetRef<CheckPredicate>("CheckPredicate"),
			luaext::FuncReference::GetRef<GetBattleTarget>("GetBattleTarget"),
			luaext::FuncReference::GetRef<GetAttackCommandTarget>("GetAttackCommandTarget"),
			luaext::FuncReference::GetRef<Debug_GetTaskInfo>("Debug_GetTaskInfo"),
			luaext::FuncReference::GetRef<GetTrackedResources>("GetTrackedResources"),
			luaext::FuncReference::GetRef<GetAllScriptNameMappings>("GetAllScriptNameMappings"),
			luaext::FuncReference::GetRef<GetLimitedAmmo>("GetLimitedAmmo"),
			luaext::FuncReference::GetRef<SetLimitedAmmo>("SetLimitedAmmo"),
			luaext::FuncReference::GetRef<DumpEntity>("DumpEntity"),
	};

	constexpr std::array Predicates{
			luaext::FuncReference::GetRef<PredicateAnd>("And"),
			luaext::FuncReference::GetRef<PredicateOr>("Or"),
			luaext::FuncReference::GetRef<PredicateNot>("Not"),
			luaext::FuncReference::GetRef<PredicateSetPriority>("SetPriority"),
			luaext::FuncReference::GetRef<PredicateOfType>("OfType"),
			luaext::FuncReference::GetRef<PredicateOfPlayer>("OfPlayer"),
			luaext::FuncReference::GetRef<PredicateInCircle>("InCircle"),
			luaext::FuncReference::GetRef<PredicateIsBuilding>("IsBuilding"),
			luaext::FuncReference::GetRef<PredicateIsSettler>("IsSettler"),
			luaext::FuncReference::GetRef<PredicateIsCombatRelevant>("IsCombatRelevant"),
			luaext::FuncReference::GetRef<PredicateOfAnyPlayer>("OfAnyPlayer"),
			luaext::FuncReference::GetRef<PredicateOfAnyEntityType>("OfAnyEntityType"),
			luaext::FuncReference::GetRef<PredicateIsNotSoldier>("IsNotSoldier"),
			luaext::FuncReference::GetRef<PredicateOfEntityCategory>("OfEntityCategory"),
			luaext::FuncReference::GetRef<PredicateOfAnyEntityCategory>("PredicateOfAnyEntityCategory"),
			luaext::FuncReference::GetRef<PredicateOfAnyClass>("PredicateOfAnyClass"),
			luaext::FuncReference::GetRef<PredicateProvidesResource>("ProvidesResource"),
			luaext::FuncReference::GetRef<PredicateInRect>("InRect"),
			luaext::FuncReference::GetRef<PredicateIsVisible>("IsVisible"),
			luaext::FuncReference::GetRef<PredicateOfUpgradeCategory>("OfUpgradeCategory"),
			luaext::FuncReference::GetRef<PredicateIsAlive>("IsAlive"),
			luaext::FuncReference::GetRef<PredicateIsNotInBuilding>("IsNotInBuilding"),
			luaext::FuncReference::GetRef<PredicateIsBuildingOrConstructionSite>("PredicateIsBuildingOrConstructionSite"),
	};

	constexpr std::array Settlers {
			luaext::FuncReference::GetRef<SettlerGetLeaderOfSoldier>("GetLeaderOfSoldier"),
			luaext::FuncReference::GetRef<SettlerGetBaseMovementSpeed>("GetBaseMovementSpeed"),
			luaext::FuncReference::GetRef<SettlerSetBaseMovementSpeed>("SetBaseMovementSpeed"),
			luaext::FuncReference::GetRef<SettlerGetOverheadWidget>("GetOverheadWidget"),
			luaext::FuncReference::GetRef<SettlerSetOverheadWidget>("SetOverheadWidget"),
			luaext::FuncReference::GetRef<HeroGetCamouflageDurationLeft>("HeroGetCamouflageDurationLeft"),
			luaext::FuncReference::GetRef<HeroSetCamouflageDurationLeft>("HeroSetCamouflageDurationLeft"),
			luaext::FuncReference::GetRef<ThiefGetCamouflageTimeTo>("ThiefGetCamouflageTimeTo"),
			luaext::FuncReference::GetRef<ThiefSetCamouflageTimeTo>("ThiefSetCamouflageTimeTo"),
			luaext::FuncReference::GetRef<HeroGetResurrectionTime>("HeroGetResurrectionTime"),
			luaext::FuncReference::GetRef<HeroSetResurrectionTime>("HeroSetResurrectionTime"),
			luaext::FuncReference::GetRef<HeroResurrect>("HeroResurrect"),
			luaext::FuncReference::GetRef<WorkerGetCurrentWorkTime>("WorkerGetCurrentWorkTime"),
			luaext::FuncReference::GetRef<WorkerSetCurrentWorkTime>("WorkerSetCurrentWorkTime"),
			luaext::FuncReference::GetRef<ThiefSetStolenResourceInfo>("ThiefSetStolenResourceInfo"),
			luaext::FuncReference::GetRef<SettlerIsVisible>("IsVisible"),
			luaext::FuncReference::GetRef<SettlerIsIdle>("IsIdle"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::HeroHawk_SendHawk,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilitySendHawk>>>("CommandSendHawk"),
			luaext::FuncReference::GetRef<SettlerGetHawkOfHero>("GetHawkOfHero"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::InflictFear_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityInflictFear>>>("CommandInflictFear"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::BombPlacer_CommandPlaceBomb,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>>>("CommandPlaceBomb"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<GGL::CEventPositionAnd2EntityTypes, shok::EventIDs::CannonBuilder_BuildCannonCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBuildCannon>, CheckPlaceCannonEvent>>("CommandPlaceCannon"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::RangedEffect_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityRangedEffect>>>("CommandRangedEffect"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::CircularAttack_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityCircularAttack>>>("CommandCircularAttack"),
			luaext::FuncReference::GetRef<SettlerCommandSummon>("CommandSummon"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::Camouflage_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityCamouflage>, CheckCamoEvent>>("CommandActivateCamoflage"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::ConvertSettler_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityConvertSettlers>, LuaEventInterface::CheckSettler,
				LuaEventInterface::CheckConvertible, LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("CommandConvert"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Sniper_SnipeCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilitySniper>, LuaEventInterface::CheckSettler,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>, CheckSnipeEvent>>("CommandSnipe"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Shuriken_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityShuriken>, LuaEventInterface::CheckSettler,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>, CheckShurikenEvent>>("CommandShuriken"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::MotivateVorkers_ActivateCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityMotivateWorkers>>>("CommandMotivateWorkers"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::KegPlacer_SabotageCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceKeg>, LuaEventInterface::CheckBuilding,
				CheckThiefNotCarryingEvent, CheckSabotageEvent>>("CommandSabotage"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::KegPlacer_DefuseCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceKeg>,
				CheckThiefNotCarryingEvent, CheckDefuseEvent>>("CommandDefuse"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::Binocular_ExploreCommand,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutBinoculars>>>("CommandBinocular"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::TorchPlacer_PlaceTorch,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutTorches>>>("CommandPlaceTorch"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::PointToResources_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityScoutFindResources>>>("CommandPointToRes"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Thief_StealFromCommand,
				CheckIsThief, LuaEventInterface::CheckBuilding,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Hostile>>>("CommandStealFrom"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::Thief_StealFromCommand,
				CheckIsThief, LuaEventInterface::CheckBuilding, LuaEventInterface::CheckTargetCategory<shok::EntityCategory::Headquarters>,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("CommandSecureGoods"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::CppL_LightningStrike_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityLightningStrike>, CheckLightingStrike>>("CommandLightningStrike"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::CppL_ResDoodadRefill_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbiltyResourceDoodadRefill>, LuaEventInterface::CheckBuilding, CheckResourceRefill,
				LuaEventInterface::CheckEntityDiploState<shok::DiploState::Friendly>>>("CommandRefillResourceDoodad"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<BB::CEvent, shok::EventIDs::CppL_ShieldCoverActivate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityShieldCover>>>("CommandShieldCover"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEvent1Entity, shok::EventIDs::CppL_Resurrect_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityResurrect>, CheckResurrect,
				LuaEventInterface::CheckEntitySamePlayer>>("CommandResurrect"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::CppL_Bombardment_Activate,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityBombardment>, CheckBombardment>>("CommandBombardment"),
			luaext::FuncReference::GetRef<LuaEventInterface::EntityCommandEvent<EGL::CEventPosition, shok::EventIDs::CppL_BombComboCannon_Activate,
				LuaEventInterface::CheckEntityBehavior<CppLogic::Mod::ReloadableCannonBuilderAbility>,
				LuaEventInterface::CheckEntityAbility<shok::AbilityId::AbilityPlaceBomb>>>("CommandBombCannonCombo"),
			luaext::FuncReference::GetRef<EnableConversionHook>("EnableConversionHook"),
			luaext::FuncReference::GetRef<DisableConversionHook>("DisableConversionHook"),
			luaext::FuncReference::GetRef<SettlerCommandMove>("CommandMove"),
			luaext::FuncReference::GetRef<SettlerCommandSerfConstructBuilding>("CommandSerfConstructBuilding"),
			luaext::FuncReference::GetRef<SettlerCommandSerfRepairBuilding>("CommandSerfRepairBuilding"),
			luaext::FuncReference::GetRef<SettlerCommandSerfExtract>("CommandSerfExtract"),
			luaext::FuncReference::GetRef<SettlerCommandExpell>("CommandExpell"),
			luaext::FuncReference::GetRef<SettlerCommandTurnSerfToBattleSerf>("CommandTurnSerfToBattleSerf"),
			luaext::FuncReference::GetRef<SettlerCommandTurnBattleSerfToSerf>("CommandTurnBattleSerfToSerf"),
			luaext::FuncReference::GetRef<SettlerSetPosition>("SetPosition"),
			luaext::FuncReference::GetRef<EnableRangedEffectSoldierHeal>("EnableRangedEffectSoldierHeal"),
			luaext::FuncReference::GetRef<SettlerShurikenGetTarget>("ShurikenGetTarget"),
			luaext::FuncReference::GetRef<SettlerSniperGetTarget>("SniperGetTarget"),
			luaext::FuncReference::GetRef<SettlerGetEnteredBuilding>("GetEnteredBuilding"),
			luaext::FuncReference::GetRef<SettlerPlayScriptAnimation>("PlayScriptAnimation"),
			luaext::FuncReference::GetRef<SettlerGetExperienceClass>("GetExperienceClass"),
			luaext::FuncReference::GetRef<WorkerGetMotivation>("WorkerGetMotivation"),
			luaext::FuncReference::GetRef<WorkerChangeMotivation>("WorkerChangeMotivation"),
			luaext::FuncReference::GetRef<WorkerGetResourceCarried>("WorkerGetResourceCarried"),
			luaext::FuncReference::GetRef<SettlerGetSummoned>("SettlerGetSummoned"),
	};

	constexpr std::array Leader{
			luaext::FuncReference::GetRef<LeaderGetExperience>("GetExperience"),
			luaext::FuncReference::GetRef<LeaderSetExperience>("SetExperience"),
			luaext::FuncReference::GetRef<LeaderGetTroopHealth>("GetTroopHealth"),
			luaext::FuncReference::GetRef<LeaderSetTroopHealth>("SetTroopHealth"),
			luaext::FuncReference::GetRef<LeaderAttachSoldier>("AttachSoldier"),
			luaext::FuncReference::GetRef<LeaderSetSoldierLimit>("SetSoldierLimit"),
			luaext::FuncReference::GetRef<LeaderSetRegeneration>("SetRegeneration"),
			luaext::FuncReference::GetRef<LeaderGetRegeneration>("GetRegeneration"),
	};

	constexpr std::array Building{
			luaext::FuncReference::GetRef<BuildingGetBarracksAutoFillActive>("GetBarracksAutoFillActive"),
			luaext::FuncReference::GetRef<BuildingGetHeight>("GetHeight"),
			luaext::FuncReference::GetRef<BuildingSetHeight>("SetHeight"),
			luaext::FuncReference::GetRef<BuildingMarketGetCurrentTradeData>("MarketGetCurrentTradeData"),
			luaext::FuncReference::GetRef<BuildingMarketSetCurrentTradeData>("MarketSetCurrentTradeData"),
			luaext::FuncReference::GetRef<BuildingCommandFoundryBuildCannon>("CommandFoundryBuildCannon"),
			luaext::FuncReference::GetRef<BuildingGetNearestFreeConstructionSlotFor>("GetNearestFreeConstructionSlotFor"),
			luaext::FuncReference::GetRef<BuildingGetNearestFreeRepairSlotFor>("GetNearestFreeRepairSlotFor"),
			luaext::FuncReference::GetRef<BuildingStartUpgrade>("StartUpgrade"),
			luaext::FuncReference::GetRef<BuildingCancelUpgrade>("CancelUpgrade"),
			luaext::FuncReference::GetRef<BuildingIsIdle>("IsIdle"),
			luaext::FuncReference::GetRef<BarracksGetLeadersTrainingAt>("BarracksGetLeadersTrainingAt"),
			luaext::FuncReference::GetRef<FoundryGetCannonTypeInConstruction>("FoundryGetCannonTypeInConstruction"),
			luaext::FuncReference::GetRef<BarracksBuySoldierForLeader>("BarracksBuySoldierForLeader"),
			luaext::FuncReference::GetRef<BuildingActivateOvertime>("ActivateOvertime"),
			luaext::FuncReference::GetRef<BuildingDeactivateOvertime>("DeactivateOvertime"),
			luaext::FuncReference::GetRef<BarracksRecruitGroups>("BarracksRecruitGroups"),
			luaext::FuncReference::GetRef<BarracksRecruitLeaders>("BarracksRecruitLeaders"),
			luaext::FuncReference::GetRef<HQBuySerf>("HQBuySerf"),
			luaext::FuncReference::GetRef<SellBuilding>("SellBuilding"),
			luaext::FuncReference::GetRef<BuildingStartResearch>("StartResearch"),
			luaext::FuncReference::GetRef<BuildingCancelResearch>("CancelResearch"),
			luaext::FuncReference::GetRef<MarketStartTrade>("MarketStartTrade"),
			luaext::FuncReference::GetRef<MarketCancelTrade>("MarketCancelTrade"),
			luaext::FuncReference::GetRef<MercenaryRemoveLastOffer>("MercenaryRemoveLastOffer"),
			luaext::FuncReference::GetRef<MercenarySetOfferData>("MercenarySetOfferData"),
			luaext::FuncReference::GetRef<BuildingGetBuildOnEntity>("GetBuildOnEntity"),
			luaext::FuncReference::GetRef<BuildOnEntityGetBuilding>("BuildOnEntityGetBuilding"),
			luaext::FuncReference::GetRef<BuildingGetConstructionSite>("GetConstructionSite"),
			luaext::FuncReference::GetRef<IsConstructionSite>("IsConstructionSite"),
			luaext::FuncReference::GetRef<ConstructionSiteGetBuilding>("ConstructionSiteGetBuilding"),
			luaext::FuncReference::GetRef<BarracksBuyLeaderByType>("BarracksBuyLeaderByType"),
			luaext::FuncReference::GetRef<BuildingGetRelativePositions>("GetRelativePositions"),
			luaext::FuncReference::GetRef<BuildingSpawnWorkerFor>("SpawnWorkerFor"),
			luaext::FuncReference::GetRef<MarketPredictPrice>("MarketPredictPrice"),
			luaext::FuncReference::GetRef<MarketGetSellResources>("MarketGetSellResources"),
	};

	void Init(luaext::State L)
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

	void OnSaveLoaded(luaext::State L)
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

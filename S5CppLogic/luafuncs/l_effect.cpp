#include "pch.h"
#include "l_effect.h"
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/effect/s5_effects.h>
#include <shok/s5_scriptsystem.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/globals/s5_entityandeffectmanager.h>
#include <shok/events/s5_events.h>
#include <shok/effect/s5_effecttype.h>
#include <luaext.h>
#include <utility/luaserializer.h>
#include <utility/savegame_extra.h>
#include <utility/entityiterator.h>

namespace CppLogic::Effect {
	void PushToHitCallbackReg(luaext::State L) {
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.GetSubTable(EffectOnHitKey, -1);
		L.Remove(-2);
	}

	static void FlyingEffectOnHitCallback(EGL::CFlyingEffect* eff) {
		int id = static_cast<int>(eff->EffectID);
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int top = L.GetTop();
		if (!L.CheckStack(5))
			return;
		PushToHitCallbackReg(L);
		L.GetTableRaw(-1, id);
		if (L.IsFunction(-1)) {
			L.Push(id);
			L.PCall(1, 0, 0);
		}
		L.SetTop(top + 1);
		L.Push();
		L.SetTableRaw(-2, id);
		L.SetTop(top);
	}

	int CreateProjectile(luaext::State L) { // (effecttype, startx, starty, tarx, tary, dmg, radius, tarid, attid, playerid, dmgclass, callback, source)
		CProjectileEffectCreator data = CProjectileEffectCreator();
		data.EffectType = L.CheckEnum<shok::EffectTypeId>(1);
		data.CurrentPos.X = data.StartPos.X = static_cast<float>(L.CheckNumber(2));
		data.CurrentPos.Y = data.StartPos.Y = static_cast<float>(L.CheckNumber(3));
		data.TargetPos.X = static_cast<float>(L.CheckNumber(4));
		data.TargetPos.Y = static_cast<float>(L.CheckNumber(5));
		data.Damage = L.OptInt(6, 0);
		data.DamageRadius = static_cast<float>(L.OptNumber(7, -1));
		data.TargetID = L.OptEntityId(8);
		data.AttackerID = L.OptEntityId(9);
		auto player = L.OptPlayerId(10, shok::PlayerId::P0);
		data.PlayerID = player;
		data.SourcePlayer = player;
		auto dmgclass = L.OptEnum<shok::DamageClassId>(11);
		data.DamageClass = dmgclass;
		data.AdvancedDamageSourceOverride = L.OptEnum<shok::AdvancedDealDamageSource>(13, shok::AdvancedDealDamageSource::Script);
		EGL::CGLEGameLogic* gl = *EGL::CGLEGameLogic::GlobalObj;
		auto id = gl->CreateEffect(&data);
		EGL::CEffect* ef = (*EGL::CGLEEffectManager::GlobalObj)->GetById(id);
		if (!GGL::CCannonBallEffect::FixDamageClass)
			if (auto* cbeff = dynamic_cast<GGL::CCannonBallEffect*>(ef))
				cbeff->DamageClass = dmgclass;
		if (L.IsFunction(12)) {
			EGL::CFlyingEffect::HookOnHit();
			EGL::CFlyingEffect::FlyingEffectOnHitCallback = &FlyingEffectOnHitCallback;
			PushToHitCallbackReg(L);
			L.PushValue(12);
			L.SetTableRaw(-2, static_cast<int>(id));
		}
		L.Push(id);
		return 1;
	}

	int IsValidEffect(luaext::State L) {
		int id = L.CheckInt(1);
		bool r = (*EGL::CGLEEffectManager::GlobalObj)->IsIdValid(static_cast<shok::EffectId>(id));
		L.Push(r);
		return 1;
	}

	int GetType(luaext::State L) {
		auto* eff = L.CheckEffect(1);
		L.Push(eff->EffectType);
		return 1;
	}
	int GetPosition(luaext::State L) {
		auto* eff = L.CheckEffect(1);
		L.Push(eff->Position);
		return 1;
	}
	int GetPlayer(luaext::State L) {
		auto* eff = L.CheckEffect(1);
		L.Push(eff->PlayerID);
		return 1;
	}

	int IsArrow(luaext::State L) {
		auto* eff = L.CheckEffect(1);
		L.Push(eff->GetClassIdentifier() == GGL::CArrowEffect::Identifier);
		return 1;
	}
	int IsCannonBall(luaext::State L) {
		auto* eff = L.CheckEffect(1);
		L.Push(eff->GetClassIdentifier() == GGL::CCannonBallEffect::Identifier);
		return 1;
	}

	int GetFlyingEffectStartEnd(luaext::State L) {
		auto* eff = dynamic_cast<EGL::CFlyingEffect*>(L.CheckEffect(1));
		if (eff == nullptr)
			throw lua::LuaException{ "no fyling effect at 1" };
		L.Push(eff->FlyingEffectSlot.StartPosition);
		L.Push(eff->FlyingEffectSlot.TargetPosition);
		return 2;
	}
	
	int GetArrowEffectData(luaext::State L) {
		auto* eff = dynamic_cast<GGL::CArrowEffect*>(L.CheckEffect(1));
		if (eff == nullptr)
			throw lua::LuaException{ "no arrow effect at 1" };
		L.Push(eff->AttackerID);
		L.Push(eff->TargetID);
		L.Push(eff->DamageAmount);
		L.Push(eff->Misses);
		return 4;
	}

	int GetCannonBallEffectData(luaext::State L) {
		auto* eff = dynamic_cast<GGL::CCannonBallEffect*>(L.CheckEffect(1));
		if (eff == nullptr)
			throw lua::LuaException{ "no cannonball effect at 1" };
		L.Push(eff->AttackerID);
		L.Push(eff->DamageAmount);
		L.Push(eff->AoERange);
		L.Push(static_cast<shok::DamageClassId>(eff->DamageClass));
		L.Push(eff->SourcePlayer);
		L.Push(eff->FlyingEffectSlot.TargetPosition);
		return 6;
	}

	int GetProjectileCallbacks(luaext::State L) {
		PushToHitCallbackReg(L);
		return 1;
	}

	void EffectOnCreate(shok::EffectId id) {
		EGL::CEvent1Entity ev{ shok::EventIDs::CppLogicEvent_OnEffectCreated, static_cast<shok::EntityId>(static_cast<int>(id)) };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
	}
	void EffectOnHit(EGL::CFlyingEffect* eff) {
		EGL::CEvent1Entity ev{ shok::EventIDs::CppLogicEvent_OnFlyingEffectHit, static_cast<shok::EntityId>(static_cast<int>(eff->EffectID)) };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
	}
	void __stdcall EffectOnDestroy(EGL::CEffect* eff) {
		EGL::CEvent1Entity ev{ shok::EventIDs::CppLogicEvent_OnEffectDestroyed, static_cast<shok::EntityId>(static_cast<int>(eff->EffectID)) };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
	}
	void SetupEffectTriggers(bool enable) {
		if (enable) {
			EGL::CGLEGameLogic::HookCreateEffect();
			EGL::CGLEGameLogic::CreateEffectHookCallback = &EffectOnCreate;
			EGL::CFlyingEffect::HookOnHit();
			EGL::CFlyingEffect::FlyingEffectOnHitCallback2 = &EffectOnHit;
			EGL::CEffect::HookOnDestroy();
			EGL::CEffect::OnDestroyCb = &EffectOnDestroy;
			CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.EffectTriggers = true;
		}
		else {
			EGL::CGLEGameLogic::CreateEffectHookCallback = nullptr;
			EGL::CFlyingEffect::FlyingEffectOnHitCallback2 = nullptr;
			EGL::CEffect::OnDestroyCb = nullptr;
			CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.EffectTriggers = false;
		}
	}
	int EnableEffectTriggers(luaext::State L) {
		SetupEffectTriggers(L.OptBool(1, true));
		return 0;
	}

	int EnableLightningFix(luaext::State L) {
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.LightningEffectFix = L.CheckBool(1);
		GD::CDisplayEffectLightning::HookColorOverride(CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.LightningEffectFix);
		return 0;
	}

	int DumpEffectType(luaext::State L) {
		auto et = L.CheckEnum<shok::EffectTypeId>(1);
		auto* t = GetEffectType(et);
		Serializer::ObjectToLuaSerializer::Serialize(L, t, EGL::EffectType::SerializationData);
		return 1;
	}

	int DumpEffect(luaext::State L) {
		auto e = L.CheckEffect(1);
		Serializer::ObjectToLuaSerializer::Serialize(L, e);
		return 1;
	}

	int PredicateInCircle(luaext::State L) {
		auto p = L.CheckPos(1);
		float r = L.CheckFloat(2);
		L.NewUserClass<CppLogic::Iterator::PredicateInCircle<EGL::CEffect>>(p, r * r);
		return 1;
	}

	int PredicateInRect(luaext::State L) {
		float x1 = L.CheckFloat(1);
		float y1 = L.CheckFloat(2);
		float x2 = L.CheckFloat(3);
		float y2 = L.CheckFloat(4);
		L.NewUserClass<CppLogic::Iterator::PredicateInRect<EGL::CEffect>>(x1, y1, x2, y2);
		return 1;
	}

	int PredicateOfType(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EffectPredicateOfType>(L.CheckEnum<shok::EffectTypeId>(1));
		return 1;
	}

	int PredicateOfPlayer(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EffectPredicateOfPlayer>(L.CheckPlayerId(1));
		return 1;
	}

	int PredicateIsArrow(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EffectPredicateIsArrow>();
		return 1;
	}

	int PredicateIsCannonBall(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EffectPredicateIsCannonBall>();
		return 1;
	}

	int PredicateIsArrowOrCannonBall(luaext::State L) {
		L.NewUserClass<CppLogic::Iterator::EffectPredicateIsArrowOrCannonBall>();
		return 1;
	}

	int PredicateAnd(luaext::State L) {
		const int num = L.GetTop();
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicateDynamicAnd<EGL::CEffect>>();
		p->preds.reserve(num);
		L.NewTable();
		for (int i = 1; i <= num; ++i) { // keep predicates, so they dont get gced
			p->preds.push_back(L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CEffect>>(i));
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
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicateDynamicOr<EGL::CEffect>>();
		p->preds.reserve(num);
		L.NewTable();
		for (int i = 1; i <= num; ++i) { // keep predicates, so they dont get gced
			p->preds.push_back(L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CEffect>>(i));
			L.PushValue(i);
			L.SetTableRaw(-2, i);
		}
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateNot(luaext::State L) {
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CEffect>>(1);
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicateNot<EGL::CEffect>>(pred);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}

	int PredicateSetPriority(luaext::State L) {
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CEffect>>(1);
		int pri = L.CheckInt(2);
		auto* p = L.NewUserClass<CppLogic::Iterator::PredicatePriority<EGL::CEffect>>(pred, pri);
		L.PushValue(1);// keep predicate, so they dont get gced
		p->L = L.GetState();
		p->r = L.Ref(L.REGISTRYINDEX);
		return 1;
	}


	int IteratorNext(luaext::State L) { // (state nil, last value) -> next value
		// no error checking here, cause that would cost speed
		// just expect no other c/c++ code will change the funcs upvalue
		// make sure ManagedIterator is the first class inherited from the actual iterator and we do not use luapp inheritance here
		// (both would break this simplified pointer)
		auto* it = static_cast<CppLogic::Iterator::ManagedIterator<EGL::CEffect>*>(L.ToUserdata(L.Upvalueindex(1)));
		float r = -1;
		int p = -1;
		EGL::CEffect* e = it->GetNext(&r, &p);
		if (e == nullptr) {
			L.Push();
			L.Push();
			L.Push();
		}
		else {
			L.Push(static_cast<int>(e->EffectID));
			L.Push(r);
			L.Push(p);
		}
		return 3;
	}
	int EffectIterator(luaext::State L) {
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAndAutoCreate(L);
		}
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CEffect>>(1);
		L.NewUserClass<CppLogic::Iterator::GlobalEffectIterator>(pred); // upvalue of func
		L.Push<IteratorNext>(1); // func
		L.Push(); // state
		L.Push(); // initial value
		return 3;
	}

	int EffectIteratorTableize(luaext::State L) {
		if (L.GetTop() > 1) { // auto create an and predicate
			PredicateAndAutoCreate(L);
		}
		auto* pred = L.CheckUserClass<CppLogic::Iterator::Predicate<EGL::CEffect>>(1);
		int index = 1;
		L.NewTable();
		CppLogic::Iterator::GlobalEffectIterator it{ pred };
		for (EGL::CEffect* e : it) {
			L.Push(static_cast<int>(e->EffectID));
			L.SetTableRaw(2, index);
			index++;
		}
		return 1;
	}

	constexpr std::array Effect{
			luaext::FuncReference::GetRef<CreateProjectile>("CreateProjectile"),
			luaext::FuncReference::GetRef<IsValidEffect>("IsValidEffect"),
			luaext::FuncReference::GetRef<GetProjectileCallbacks>("GetProjectileCallbacks"),
			luaext::FuncReference::GetRef<EnableEffectTriggers>("EnableEffectTriggers"),
			luaext::FuncReference::GetRef<GetType>("GetType"),
			luaext::FuncReference::GetRef<GetPosition>("GetPosition"),
			luaext::FuncReference::GetRef<GetPlayer>("GetPlayer"),
			luaext::FuncReference::GetRef<GetFlyingEffectStartEnd>("GetFlyingEffectStartEnd"),
			luaext::FuncReference::GetRef<GetArrowEffectData>("GetArrowEffectData"),
			luaext::FuncReference::GetRef<GetCannonBallEffectData>("GetCannonBallEffectData"),
			luaext::FuncReference::GetRef<IsArrow>("IsArrow"),
			luaext::FuncReference::GetRef<IsCannonBall>("IsCannonBall"),
			luaext::FuncReference::GetRef<EffectIterator>("EffectIterator"),
			luaext::FuncReference::GetRef<EffectIteratorTableize>("EffectIteratorTableize"),
			luaext::FuncReference::GetRef<EnableLightningFix>("EnableLightningFix"),
			luaext::FuncReference::GetRef<DumpEffectType>("DumpEffectType"),
			luaext::FuncReference::GetRef<DumpEffect>("DumpEffect"),
	};
	constexpr std::array Predicates{
			luaext::FuncReference::GetRef<PredicateInCircle>("InCircle"),
			luaext::FuncReference::GetRef<PredicateInRect>("InRect"),
			luaext::FuncReference::GetRef<PredicateOfType>("OfType"),
			luaext::FuncReference::GetRef<PredicateOfPlayer>("OfPlayer"),
			luaext::FuncReference::GetRef<PredicateIsArrow>("IsArrow"),
			luaext::FuncReference::GetRef<PredicateIsCannonBall>("IsCannonBall"),
			luaext::FuncReference::GetRef<PredicateIsArrowOrCannonBall>("IsArrowOrCannonBall"),
			luaext::FuncReference::GetRef<PredicateAnd>("And"),
			luaext::FuncReference::GetRef<PredicateOr>("Or"),
			luaext::FuncReference::GetRef<PredicateNot>("Not"),
			luaext::FuncReference::GetRef<PredicateSetPriority>("SetPriority"),
	};

	void Init(luaext::State L)
	{
		L.RegisterFuncs(Effect, -3);

		L.Push("Predicates");
		L.NewTable();
		L.RegisterFuncs(Predicates, -3);
		L.SetTableRaw(-3);


		if (L.GetState() != shok::LuaStateMainmenu) {
			L.GetSubTable("Events");
			L.Push("CPPLOGIC_EVENT_ON_EFFECT_CREATED");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnEffectCreated));
			L.SetTableRaw(-3);

			L.Push("CPPLOGIC_EVENT_ON_FLYINGEFFECT_HIT");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnFlyingEffectHit));
			L.SetTableRaw(-3);

			L.Push("CPPLOGIC_EVENT_ON_EFFECT_DESTROYED");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnEffectDestroyed));
			L.SetTableRaw(-3);

			L.Pop(1);
		}
	}

	void Cleanup(luaext::State L) {
		EGL::CFlyingEffect::FlyingEffectOnHitCallback = nullptr;
		EGL::CGLEGameLogic::CreateEffectHookCallback = nullptr;
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2 = nullptr;
		EGL::CEffect::OnDestroyCb = nullptr;
		GD::CDisplayEffectLightning::HookColorOverride(false);
	}

	void OnSaveLoaded(luaext::State L)
	{
		int t = L.GetTop();
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(EffectOnHitKey);
		L.GetTableRaw(-2);
		if (L.IsTable(-1)) {
			L.Push();
			if (L.Next(-2)) { // contains anything
				EGL::CFlyingEffect::HookOnHit();
				EGL::CFlyingEffect::FlyingEffectOnHitCallback = &FlyingEffectOnHitCallback;
			}
		}
		L.SetTop(t);
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.EffectTriggers)
			SetupEffectTriggers(true);
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.LightningEffectFix)
			GD::CDisplayEffectLightning::HookColorOverride(true);
	}
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, x-10000, y, x, y, 500, 1000, 0, 0, 1, 0, function(id) LuaDebugger.Log(id) end)
// local id = e(); local p = GetPosition(id); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, p.X-1000, p.Y, p.X, p.Y, 100, 1000, 0, 0, 1, 0)
// CppLogic.Effect.IsValidEffect(effid)

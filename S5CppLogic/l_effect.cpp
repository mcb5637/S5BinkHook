#include "pch.h"
#include "l_effect.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_effects.h"
#include "s5_scriptsystem.h"
#include "s5_entity.h"
#include "s5_maplogic.h"
#include "s5_entityandeffectmanager.h"
#include "s5_events.h"
#include "luaext.h"
#include "luaserializer.h"
#include "savegame_extra.h"

namespace CppLogic::Effect {
	void PushToHitCallbackReg(lua::State L) {
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.GetSubTable(EffectOnHitKey, -1);
		L.Remove(-2);
	}

	void FlyingEffectOnHitCallback(EGL::CFlyingEffect* eff) {
		int id = eff->EffectID;
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
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

	int CreateProjectile(lua::State l) { // (effecttype, startx, starty, tarx, tary, dmg, radius, tarid, attid, playerid, dmgclass, callback, source)
		luaext::EState L{ l };
		CProjectileEffectCreator data = CProjectileEffectCreator();
		data.EffectType = L.CheckInt(1);
		data.CurrentPos.X = data.StartPos.X = static_cast<float>(L.CheckNumber(2));
		data.CurrentPos.Y = data.StartPos.Y = static_cast<float>(L.CheckNumber(3));
		data.TargetPos.X = static_cast<float>(L.CheckNumber(4));
		data.TargetPos.Y = static_cast<float>(L.CheckNumber(5));
		data.Damage = L.OptInteger(6, 0);
		data.DamageRadius = static_cast<float>(L.OptNumber(7, -1));
		data.TargetID = L.OptEntityId(8);
		data.AttackerID = L.OptEntityId(9);
		int player = L.OptInteger(10, 0);
		data.PlayerID = player;
		data.SourcePlayer = player;
		int dmgclass = L.OptInteger(11, 0);
		data.DamageClass = dmgclass;
		data.AdvancedDamageSourceOverride = L.OptInteger(13, static_cast<int>(shok::AdvancedDealDamageSource::Script));
		EGL::CGLEGameLogic* gl = *EGL::CGLEGameLogic::GlobalObj;
		int id = gl->CreateEffect(&data);
		EGL::CEffect* ef = (*EGL::CGLEEffectManager::GlobalObj)->GetById(id);
		if (!GGL::CCannonBallEffect::FixDamageClass)
			if (GGL::CCannonBallEffect* cbeff = dynamic_cast<GGL::CCannonBallEffect*>(ef))
				cbeff->DamageClass = dmgclass;
		if (L.IsFunction(12)) {
			EGL::CFlyingEffect::HookOnHit();
			EGL::CFlyingEffect::FlyingEffectOnHitCallback = &FlyingEffectOnHitCallback;
			PushToHitCallbackReg(L);
			L.PushValue(12);
			L.SetTableRaw(-2, id);
		}
		L.Push(id);
		return 1;
	}

	int IsValidEffect(lua::State L) {
		int id = L.CheckInt(1);
		bool r = (*EGL::CGLEEffectManager::GlobalObj)->IsIdValid(id);
		L.Push(r);
		return 1;
	}

	int GetType(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = L.CheckEffect(1);
		L.Push(eff->EffectType);
		return 1;
	}
	int GetPosition(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = L.CheckEffect(1);
		L.PushPos(eff->Position);
		return 1;
	}
	int GetPlayer(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = L.CheckEffect(1);
		L.Push(eff->PlayerID);
		return 1;
	}

	int IsArrow(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = L.CheckEffect(1);
		L.Push(eff->GetClassIdentifier() == GGL::CArrowEffect::Identifier);
		return 1;
	}
	int IsCannonBall(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = L.CheckEffect(1);
		L.Push(eff->GetClassIdentifier() == GGL::CCannonBallEffect::Identifier);
		return 1;
	}

	int GetFlyingEffectStartEnd(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = dynamic_cast<EGL::CFlyingEffect*>(L.CheckEffect(1));
		if (eff == nullptr)
			throw lua::LuaException{ "no fyling effect at 1" };
		L.PushPos(eff->FlyingEffectSlot.StartPosition);
		L.PushPos(eff->FlyingEffectSlot.TargetPosition);
		return 2;
	}
	
	int GetArrowEffectData(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = dynamic_cast<GGL::CArrowEffect*>(L.CheckEffect(1));
		if (eff == nullptr)
			throw lua::LuaException{ "no arrow effect at 1" };
		L.Push(eff->AttackerID);
		L.Push(eff->TargetID);
		L.Push(eff->DamageAmount);
		L.Push(eff->Misses);
		return 4;
	}

	int GetCannonBallEffectData(lua::State ls) {
		luaext::EState L{ ls };
		auto* eff = dynamic_cast<GGL::CCannonBallEffect*>(L.CheckEffect(1));
		if (eff == nullptr)
			throw lua::LuaException{ "no cannonball effect at 1" };
		L.Push(eff->AttackerID);
		L.Push(eff->DamageAmount);
		L.Push(eff->AoERange);
		L.Push(eff->DamageClass);
		L.Push(eff->SourcePlayer);
		L.PushPos(eff->FlyingEffectSlot.TargetPosition);
		return 6;
	}

	int GetProjectileCallbacks(lua::State L) {
		PushToHitCallbackReg(L);
		return 1;
	}

	void EffectOnCreate(int id) {
		EGL::CEvent1Entity ev{ shok::EventIDs::CppLogicEvent_OnEffectCreated, id };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
	}
	void EffectOnHit(EGL::CFlyingEffect* eff) {
		EGL::CEvent1Entity ev{ shok::EventIDs::CppLogicEvent_OnFlyingEffectHit, eff->EffectID };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
	}
	void __stdcall EffectOnDestroy(EGL::CEffect* eff) {
		EGL::CEvent1Entity ev{ shok::EventIDs::CppLogicEvent_OnEffectDestroyed, eff->EffectID };
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
			CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.EffectTriggers = false;
		}
	}
	int EnableEffectTriggers(lua::State L) {
		SetupEffectTriggers(L.OptBool(1, true));
		return 0;
	}

	constexpr std::array<lua::FuncReference, 12> Effect{ {
			lua::FuncReference::GetRef<CreateProjectile>("CreateProjectile"),
			lua::FuncReference::GetRef<IsValidEffect>("IsValidEffect"),
			lua::FuncReference::GetRef<GetProjectileCallbacks>("GetProjectileCallbacks"),
			lua::FuncReference::GetRef<EnableEffectTriggers>("EnableEffectTriggers"),
			lua::FuncReference::GetRef<GetType>("GetType"),
			lua::FuncReference::GetRef<GetPosition>("GetPosition"),
			lua::FuncReference::GetRef<GetPlayer>("GetPlayer"),
			lua::FuncReference::GetRef<GetFlyingEffectStartEnd>("GetFlyingEffectStartEnd"),
			lua::FuncReference::GetRef<GetArrowEffectData>("GetArrowEffectData"),
			lua::FuncReference::GetRef<GetCannonBallEffectData>("GetCannonBallEffectData"),
			lua::FuncReference::GetRef<IsArrow>("IsArrow"),
			lua::FuncReference::GetRef<IsCannonBall>("IsCannonBall"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(Effect, -3);

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

	void Cleanup(lua::State L) {
		EGL::CFlyingEffect::FlyingEffectOnHitCallback = nullptr;
	}

	void CppLogic::Effect::OnSaveLoaded(lua::State L)
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
	}
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, x-10000, y, x, y, 500, 1000, 0, 0, 1, 0, function(id) LuaDebugger.Log(id) end)
// local id = e(); local p = GetPosition(id); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, p.X-1000, p.Y, p.X, p.Y, 100, 1000, 0, 0, 1, 0)
// CppLogic.Effect.IsValidEffect(effid)

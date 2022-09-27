#include "pch.h"
#include "l_effect.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_effects.h"
#include "s5_scriptsystem.h"
#include "s5_entity.h"
#include "s5_maplogic.h"
#include "s5_entityandeffectmanager.h"
#include "luaext.h"
#include "luaserializer.h"

namespace CppLogic::Effect {
	void PushToHitCallbackReg(lua::State L) {
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.GetSubTable(EffectOnHitKey);
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

	int GetProjectileCallbacks(lua::State L) {
		PushToHitCallbackReg(L);
		return 1;
	}

	constexpr std::array<lua::FuncReference, 3> Effect{ {
			lua::FuncReference::GetRef<CreateProjectile>("CreateProjectile"),
			lua::FuncReference::GetRef<IsValidEffect>("IsValidEffect"),
			lua::FuncReference::GetRef<GetProjectileCallbacks>("GetProjectileCallbacks"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(Effect, -3);
	}

	void Cleanup(lua::State L) {
		EGL::CFlyingEffect::FlyingEffectOnHitCallback = nullptr;
	}
}

// local x,y = GUI.Debug_GetMapPositionUnderMouse(); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, x-10000, y, x, y, 500, 1000, 0, 0, 1, 0, function(id) LuaDebugger.Log(id) end)
// local id = e(); local p = GetPosition(id); return CppLogic.Effect.CreateProjectile(GGL_Effects.FXCannonBallShrapnel, p.X-1000, p.Y, p.X, p.Y, 100, 1000, 0, 0, 1, 0)
// CppLogic.Effect.IsValidEffect(effid)

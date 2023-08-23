#include "pch.h"
#include "l_combat.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "luaext.h"
#include "s5_entity.h"
#include "s5_effects.h"
#include "savegame_extra.h"

namespace CppLogic::Combat {
	int DealDamage(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* targ = L.CheckEntity(1);
		int dmg = L.CheckInt(2);
		EGL::CGLEEntity* att = L.OptEntity(3);
		shok::AdvancedDealDamageSource sou = static_cast<shok::AdvancedDealDamageSource>(L.OptInteger(8, static_cast<int>(shok::AdvancedDealDamageSource::Script)));
		targ->AdvancedHurtEntityBy(att, dmg, L.OptPlayerId(4, shok::PlayerId::P0), L.OptBool(5, true), L.OptBool(6, true), L.OptBool(7, true), sou);
		return 0;
	}

	int DealAoEDamage(lua::State l) {
		luaext::EState L{ l };
		EGL::CGLEEntity* source = L.OptEntity(1);
		shok::Position pos = shok::Position();
		pos.X = static_cast<float>(L.CheckNumber(2));
		pos.Y = static_cast<float>(L.CheckNumber(3));
		float range = static_cast<float>(L.CheckNumber(4));
		int dmg = L.CheckInt(5);
		auto player = L.OptPlayerId(4, shok::PlayerId::P0);
		auto dmgclass = L.OptEnum<shok::DamageClassId>(7);
		shok::AdvancedDealDamageSource sou = static_cast<shok::AdvancedDealDamageSource>(L.OptInteger(11, static_cast<int>(shok::AdvancedDealDamageSource::Script)));
		EGL::CGLEEntity::AdvancedDealAoEDamage(source, pos, range, dmg, player, dmgclass, L.OptBool(8, true), L.OptBool(9, true), L.OptBool(10, true), sou);
		return 0;
	}

	// tested aoe damage: cannon/autocannon projectile, circularattack, trapcannon 
	int EnableAoEProjectileFix(lua::State L) {
		GGL::CCannonBallEffect::HookFromCreator();
		GGL::CCannonBallEffect::FixDamageClass = true;
		EGL::CGLEEntity::HookDamageMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.AoEDamageFix = true;
		return 0;
	}

	int DisableAoEProjectileFix(lua::State L) {
		GGL::CCannonBallEffect::FixDamageClass = false;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.AoEDamageFix = false;
		return 0;
	}

	void l_combat_ActivateCamo(GGL::CCamouflageBehavior* th) {
		EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
		if (e)
			e->ClearAttackers();
	}

	int EnableCamoFix(lua::State L) {
		GGL::CCamouflageBehavior::HookOnAttacked();
		GGL::CCamouflageBehavior::HookActivate();
		GGL::CCamouflageBehavior::CamoActivateCb = &l_combat_ActivateCamo;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CamoFix = true;
		return 0;
	}

	int DisableCamoFix(lua::State L) {
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2 = nullptr;
		GGL::CCamouflageBehavior::CamoActivateCb = nullptr;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CamoFix = false;
		return 0;
	}

	void Cleanup(lua::State L) {
		DisableAoEProjectileFix(L);
		DisableCamoFix(L);
	}

	constexpr std::array<lua::FuncReference, 6> Combat{ {
			lua::FuncReference::GetRef<DealDamage>("DealDamage"),
			lua::FuncReference::GetRef<DealAoEDamage>("DealAoEDamage"),
			lua::FuncReference::GetRef<EnableAoEProjectileFix>("EnableAoEProjectileFix"),
			lua::FuncReference::GetRef<DisableAoEProjectileFix>("DisableAoEProjectileFix"),
			lua::FuncReference::GetRef<EnableCamoFix>("EnableCamoFix"),
			lua::FuncReference::GetRef<DisableCamoFix>("DisableCamoFix"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(Combat, -3);
	}

	void CppLogic::Combat::OnSaveLoaded(lua::State L)
	{
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.AoEDamageFix)
			EnableAoEProjectileFix(L);
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CamoFix)
			EnableCamoFix(L);
	}
}
// CppLogic.Combat.DealDamage(GUI.GetEntityAtPosition(GUI.GetMousePosition()), 100)
// local id = e(); local p = GetPosition(id); CppLogic.Combat.DealAoEDamage(0, p.X, p.Y, 1000, 100, 0, 0)

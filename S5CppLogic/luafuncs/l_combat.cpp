#include "l_combat.h"
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_entity.h>
#include <shok/effect/s5_effects.h>
#include <utility/savegame_extra.h>

namespace CppLogic::Combat {
	int DealDamage(luaext::State L) {
		EGL::CGLEEntity* targ = L.CheckEntity(1);
		int dmg = L.CheckInt(2);
		EGL::CGLEEntity* att = L.OptEntity(3);
		auto sou = L.OptEnum<shok::AdvancedDealDamageSource>(8, shok::AdvancedDealDamageSource::Script);
		targ->AdvancedHurtEntityBy(att, dmg, L.OptPlayerId(4, shok::PlayerId::P0), L.OptBool(5, true), L.OptBool(6, true), L.OptBool(7, true), sou);
		return 0;
	}

	int DealAoEDamage(luaext::State L) {
		EGL::CGLEEntity* source = L.OptEntity(1);
		shok::Position pos = shok::Position();
		pos.X = static_cast<float>(L.CheckNumber(2));
		pos.Y = static_cast<float>(L.CheckNumber(3));
		auto range = static_cast<float>(L.CheckNumber(4));
		int dmg = L.CheckInt(5);
		auto player = L.OptPlayerId(4, shok::PlayerId::P0);
		auto dmgclass = L.OptEnum<shok::DamageClassId>(7);
		auto sou = L.OptEnum<shok::AdvancedDealDamageSource>(11, shok::AdvancedDealDamageSource::Script);
		EGL::CGLEEntity::AdvancedDealAoEDamage(source, pos, range, dmg, player, dmgclass, L.OptBool(8, true), L.OptBool(9, true), L.OptBool(10, true), sou);
		return 0;
	}

	// tested aoe damage: cannon/autocannon projectile, circularattack, trapcannon 
	int EnableAoEProjectileFix(luaext::State L) {
		GGL::CCannonBallEffect::HookFromCreator();
		GGL::CCannonBallEffect::FixDamageClass = true;
		EGL::CGLEEntity::HookDamageMod();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.AoEDamageFix = true;
		return 0;
	}

	int DisableAoEProjectileFix(luaext::State L) {
		GGL::CCannonBallEffect::FixDamageClass = false;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.AoEDamageFix = false;
		return 0;
	}

	static void l_combat_ActivateCamo(GGL::CCamouflageBehavior* th) {
		EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
		if (e)
			e->ClearAttackers();
	}

	int EnableCamoFix(luaext::State L) {
		GGL::CCamouflageBehavior::HookOnAttacked();
		GGL::CCamouflageBehavior::HookActivate();
		GGL::CCamouflageBehavior::CamoActivateCb = &l_combat_ActivateCamo;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CamoFix = true;
		return 0;
	}

	int DisableCamoFix(luaext::State L) {
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2 = nullptr;
		GGL::CCamouflageBehavior::CamoActivateCb = nullptr;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CamoFix = false;
		return 0;
	}

	void Cleanup(luaext::State L) {
		DisableAoEProjectileFix(L);
		DisableCamoFix(L);
	}

	constexpr std::array Combat{
			luaext::FuncReference::GetRef<DealDamage>("DealDamage"),
			luaext::FuncReference::GetRef<DealAoEDamage>("DealAoEDamage"),
			luaext::FuncReference::GetRef<EnableAoEProjectileFix>("EnableAoEProjectileFix"),
			luaext::FuncReference::GetRef<DisableAoEProjectileFix>("DisableAoEProjectileFix"),
			luaext::FuncReference::GetRef<EnableCamoFix>("EnableCamoFix"),
			luaext::FuncReference::GetRef<DisableCamoFix>("DisableCamoFix"),
	};

	void Init(luaext::State L)
	{
		L.RegisterFuncs(Combat, -3);
	}

	void OnSaveLoaded(luaext::State L)
	{
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.AoEDamageFix)
			EnableAoEProjectileFix(L);
		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CamoFix)
			EnableCamoFix(L);
	}
}
// CppLogic.Combat.DealDamage(GUI.GetEntityAtPosition(GUI.GetMousePosition()), 100)
// local id = e(); local p = GetPosition(id); CppLogic.Combat.DealAoEDamage(0, p.X, p.Y, 1000, 100, 0, 0)

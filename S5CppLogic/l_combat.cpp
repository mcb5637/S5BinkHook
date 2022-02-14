#include "pch.h"
#include "l_combat.h"
#include "s5data.h"
#include "luaext.h"
#include "l_entity.h"

int l_combat_dealDamage(lua_State* L) {
	EGL::CGLEEntity* targ = luaext_checkEntity(L, 1);
	int dmg = luaL_checkint(L, 2);
	EGL::CGLEEntity* att = luaext_optEntity(L, 3);
	CppLogic::AdvancedDealDamageSource sou = static_cast<CppLogic::AdvancedDealDamageSource>(luaL_optint(L, 8, static_cast<int>(CppLogic::AdvancedDealDamageSource::Script)));
	targ->AdvancedHurtEntityBy(att, dmg, luaL_optint(L, 4, 0), luaext_optbool(L, 5, true), luaext_optbool(L, 6, true), luaext_optbool(L, 7, true), sou);
	return 0;
}

int l_combat_dealAOEDamage(lua_State* L) {
	EGL::CGLEEntity* source = luaext_optEntity(L, 1);
	shok::Position pos = shok::Position();
	pos.X = luaL_checkfloat(L, 2);
	pos.Y = luaL_checkfloat(L, 3);
	float range = luaL_checkfloat(L, 4);
	int dmg = luaL_checkint(L, 5);
	int player = luaL_optint(L, 6, 0);
	int dmgclass = luaL_optint(L, 7, 0);
	CppLogic::AdvancedDealDamageSource sou = static_cast<CppLogic::AdvancedDealDamageSource>(luaL_optint(L, 11, static_cast<int>(CppLogic::AdvancedDealDamageSource::Script)));
	EGL::CGLEEntity::AdvancedDealAoEDamage(source, pos, range, dmg, player, dmgclass, luaext_optbool(L, 8, true), luaext_optbool(L, 9, true), luaext_optbool(L, 10, true), sou);
	return 0;
}

// tested aoe damage: cannon/autocannon projectile, circularattack, trapcannon 
int l_combat_EnableAoEProjectileFix(lua_State* L) {
	shok_GGL_CCannonBallEffect::HookFromCreator();
	shok_GGL_CCannonBallEffect::FixDamageClass = true;
	EGL::CGLEEntity::HookDamageMod();
	return 0;
}

int l_combat_DisableAoEProjectileFix(lua_State* L) {
	shok_GGL_CCannonBallEffect::FixDamageClass = false;
	return 0;
}

void l_combat_FlyingEffectOnHitCallback(shok_EGL_CFlyingEffect* eff, bool post) {
	if (post)
		EGL::CGLEEntity::ResetCamoIgnoreIfNotEntity = 0;
	else if (eff->IsArrowEffect())
		EGL::CGLEEntity::ResetCamoIgnoreIfNotEntity = ((shok_GGL_CArrowEffect*)eff)->AttackerID;
	else if (eff->IsCannonBallEffect())
		EGL::CGLEEntity::ResetCamoIgnoreIfNotEntity = ((shok_GGL_CCannonBallEffect*)eff)->AttackerID;
}
void l_combat_ActivateCamo(GGL::CCamouflageBehavior* th) {
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(th->EntityId);
	if (e)
		e->ClearAttackers();
}

int l_combat_EnableCamoFix(lua_State* L) {
	shok_EGL_CFlyingEffect::HookOnHit();
	shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2 = &l_combat_FlyingEffectOnHitCallback;
	EGL::CGLEEntity::HookResetCamo();
	EGL::CGLEEntity::HookCamoActivate();
	EGL::CGLEEntity::CamoActivateCb = &l_combat_ActivateCamo;
	return 0;
}

int l_combat_DisableCamoFix(lua_State* L) {
	shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2 = nullptr;
	EGL::CGLEEntity::CamoActivateCb = nullptr;
	EGL::CGLEEntity::ResetCamoIgnoreIfNotEntity = 0;
	return 0;
}

void l_combat_cleanup(lua_State* L) {
	l_combat_DisableAoEProjectileFix(L);
	l_combat_DisableCamoFix(L);
}

void l_combat_init(lua_State* L)
{
	luaext_registerFunc(L, "DealDamage", &l_combat_dealDamage);
	luaext_registerFunc(L, "DealAoEDamage", &l_combat_dealAOEDamage);
	luaext_registerFunc(L, "EnableAoEProjectileFix", &l_combat_EnableAoEProjectileFix);
	luaext_registerFunc(L, "DisableAoEProjectileFix", &l_combat_DisableAoEProjectileFix);
	luaext_registerFunc(L, "EnableCamoFix", &l_combat_EnableCamoFix);
	luaext_registerFunc(L, "DisableCamoFix", &l_combat_DisableCamoFix);
}

// CppLogic.Combat.DealDamage(GUI.GetEntityAtPosition(GUI.GetMousePosition()), 100)
// local id = e(); local p = GetPosition(id); CppLogic.Combat.DealAoEDamage(0, p.X, p.Y, 1000, 100, 0, 0)

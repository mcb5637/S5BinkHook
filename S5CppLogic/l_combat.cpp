#include "pch.h"
#include "l_combat.h"
#include "s5data.h"
#include "luaext.h"
#include "l_entity.h"

int l_combat_dealDamage(lua_State* L) {
	shok_EGL_CGLEEntity* targ = luaext_checkEntity(L, 1);
	int dmg = luaL_checkint(L, 2);
	shok_EGL_CGLEEntity* att = luaext_optEntity(L, 3);
	shok_EGL_CGLEEntity::EntityHurtEntity(att, targ, dmg);
	return 0;
}

int l_combat_dealAOEDamage(lua_State* L) {
	shok_EGL_CGLEEntity* source = luaext_optEntity(L, 1);
	shok_position pos = shok_position();
	pos.X = luaL_checkfloat(L, 2);
	pos.Y = luaL_checkfloat(L, 3);
	float range = luaL_checkfloat(L, 4);
	int dmg = luaL_checkint(L, 5);
	int player = luaL_optint(L, 6, 0);
	int dmgclass = luaL_optint(L, 7, 0);
	shok_EGL_CGLEEntity::EntityDealAoEDamage(source, &pos, range, dmg, player, dmgclass);
	return 0;
}

// tested aoe damage: cannon/autocannon projectile, circularattack, trapcannon 
int l_combat_EnableAoEProjectileFix(lua_State* L) {
	shok_GGL_CCannonBallEffect::HookFromCreator();
	shok_GGL_CCannonBallEffect::FixDamageClass = true;
	shok_EGL_CGLEEntity::HookDamageMod();
	return 0;
}

int l_combat_DisableAoEProjectileFix(lua_State* L) {
	shok_GGL_CCannonBallEffect::FixDamageClass = false;
	return 0;
}

void l_combat_FlyingEffectOnHitCallback(shok_EGL_CFlyingEffect* eff, bool post) {
	if (post)
		shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity = 0;
	else if (eff->IsArrowEffect())
		shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity = ((shok_GGL_CArrowEffect*)eff)->AttackerID;
	else if (eff->IsCannonBallEffect())
		shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity = ((shok_GGL_CCannonBallEffect*)eff)->AttackerID;
}
void l_combat_ActivateCamo(shok_GGL_CCamouflageBehavior* th) {
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(th->EntityId);
	if (e)
		e->ClearAttackers();
}

int l_combat_EnableCamoFix(lua_State* L) {
	shok_EGL_CFlyingEffect::HookOnHit();
	shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2 = &l_combat_FlyingEffectOnHitCallback;
	shok_EGL_CGLEEntity::HookResetCamo();
	shok_EGL_CGLEEntity::HookCamoActivate();
	shok_EGL_CGLEEntity::CamoActivateCb = &l_combat_ActivateCamo;
	return 0;
}

int l_combat_DisableCamoFix(lua_State* L) {
	shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2 = nullptr;
	shok_EGL_CGLEEntity::CamoActivateCb = nullptr;
	shok_EGL_CGLEEntity::ResetCamoIgnoreIfNotEntity = 0;
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

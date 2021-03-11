#include "pch.h"
#include "l_combat.h"
#include "s5data.h"
#include "luaext.h"
#include "l_entity.h"

int l_combat_dealDamage(lua_State* L) {
	shok_EGL_CGLEEntity* targ = luaext_checkEntity(L, 1);
	int dmg = luaL_checkint(L, 2);
	shok_EGL_CGLEEntity* att = luaext_optEntity(L, 3);
	shok_entityHurtEntity(att, targ, dmg);
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
	shok_entityDealAOEDamage(source, &pos, range, dmg, player, dmgclass);
	return 0;
}

void l_combatHookCreateEffect(int effectId, void* retAdr) {
	if (retAdr == CreatEffectReturnBattleBehaviorAttack) {
		shok_EGL_CEffect* ef = (*shok_EGL_CGLEEffectManagerObject)->GetEffectById(effectId);
		if (ef->IsCannonBallEffect()) {
			shok_GGL_CCannonBallEffect* cbeff = (shok_GGL_CCannonBallEffect*)ef;
			shok_EGL_CGLEEntity* e = shok_eid2obj(cbeff->AttackerID);
			cbeff->SourcePlayer = e->PlayerId;
			shok_GGlue_CGlueEntityProps* t = e->GetEntityType();
			shok_GGL_CBattleBehaviorProps* b = t->GetBattleBehaviorProps();
			if (b) {
				cbeff->DamageClass = b->DamageClass;
				return;
			}
			shok_GGL_CAutoCannonBehaviorProps* a = t->GetAutoCannonProps();
			if (a)
				cbeff->DamageClass = a->DamageClass;
		}
	}
}

int l_combat_EnableAoEProjectileFix(lua_State* L) {
	(*shok_EGL_CGLEGameLogicObject)->HookCreateEffect();
	CreateEffectHookCallback = &l_combatHookCreateEffect;
	return 0;
}

int l_combat_DisableAoEProjectileFix(lua_State* L) {
	CreateEffectHookCallback = nullptr;
	return 0;
}

void l_combat_init(lua_State* L)
{
	luaext_registerFunc(L, "DealDamage", &l_combat_dealDamage);
	luaext_registerFunc(L, "DealAoEDamage", &l_combat_dealAOEDamage);
	luaext_registerFunc(L, "EnableAoEProjectileFix", &l_combat_EnableAoEProjectileFix);
	luaext_registerFunc(L, "DisableAoEProjectileFix", &l_combat_DisableAoEProjectileFix);
}

// CppLogic.Combat.DealDamage(GUI.GetEntityAtPosition(GUI.GetMousePosition()), 100)
// local id = e(); local p = GetPosition(id); CppLogic.Combat.DealAoEDamage(0, p.X, p.Y, 1000, 100, 0, 0)

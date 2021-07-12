#include "pch.h"
#include "s5data.h"

struct shok_vtable_GGL_CBehaviorDefaultMovement {
	PADDINGI(3);
	int(__thiscall* AddHandlers)(shok_EGL_CGLEBehavior*, int); // behavior, just didnt make a extra vtable for it...
	PADDINGI(5);
	float(__thiscall* GetSpeed)(shok_GGL_CBehaviorDefaultMovement* m);
};

// vtable heroability 8 is ability(this, abilityid)

float shok_GGL_CBehaviorDefaultMovement::GetMovementSpeed()
{
	return reinterpret_cast<shok_vtable_GGL_CBehaviorDefaultMovement*>(vtable)->GetSpeed(this);
}

int (*shok_GGL_CSniperAbility::SnipeDamageOverride)(shok_EGL_CGLEEntity* sniper, shok_EGL_CGLEEntity* tar, int dmg) = nullptr;
int __fastcall sniperability_tasksnipeoverride(shok_GGL_CSniperAbility* thi, int _, int taskargs) {
	shok_EGL_CGLEEntity* thent = shok_EGL_CGLEEntity::GetEntityByID(thi->EntityId);
	shok_EGL_CGLEEntity* tar = shok_EGL_CGLEEntity::GetEntityByID(thi->TargetId);
	if (!tar || tar->Health <= 0)
		return 0;
	shok_GGL_CSniperAbilityProps* pr = thent->GetEntityType()->GetBehaviorProps<shok_GGL_CSniperAbilityProps>();
	if (thent->Position.GetDistanceSquaredTo(tar->Position) >= pr->Range * pr->Range)
		return 0;
	int dmg = static_cast<int>(tar->GetMaxHealth() * pr->DamageFactor);
	if (shok_GGL_CSniperAbility::SnipeDamageOverride)
		dmg = shok_GGL_CSniperAbility::SnipeDamageOverride(thent, tar, dmg);
	shok_GGL_CBattleBehaviorProps* bpr = thent->GetEntityType()->GetBehaviorProps< shok_GGL_CBattleBehaviorProps>();
	shok_CProjectileEffectCreator cr{};
	cr.EffectType = bpr->ProjectileEffectID;
	cr.PlayerID = thent->PlayerId;
	cr.StartPos = thent->Position;
	cr.CurrentPos = thent->Position;
	cr.TargetPos = tar->Position;
	cr.AttackerID = thent->EntityId;
	cr.TargetID = tar->EntityId;
	cr.Damage = dmg;
	cr.DamageRadius = -1;
	cr.DamageClass = 0;
	cr.SourcePlayer = thent->PlayerId;
	(*shok_EGL_CGLEGameLogic::GlobalObj)->CreateEffect(&cr);
	return 0;
}
bool OverrideSnipeTask_Hooked = false;
void shok_GGL_CSniperAbility::OverrideSnipeTask()
{
	if (OverrideSnipeTask_Hooked)
		return;
	OverrideSnipeTask_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4DB5B8), 10 };
	WriteJump(reinterpret_cast<void*>(0x4DB5B8), &sniperability_tasksnipeoverride);
}

static inline float(__thiscall* const battleBehaviorGetMaxRange)(shok_GGL_CBattleBehavior*) = reinterpret_cast<float(__thiscall*)(shok_GGL_CBattleBehavior *)>(0x50AB43);
float shok_GGL_CBattleBehavior::GetMaxRange()
{
	return battleBehaviorGetMaxRange(this);
}

static inline int(__thiscall* const leaderbehgettroophealth)(shok_GGL_CBattleBehavior*) = reinterpret_cast<int(__thiscall*)(shok_GGL_CBattleBehavior *)>(0x4EE1D6);
int shok_GGL_CLeaderBehavior::GetTroopHealth()
{
	return leaderbehgettroophealth(this);
}
static inline int(__thiscall* const leaderbehgettroophealthpersol)(shok_GGL_CBattleBehavior*) = reinterpret_cast<int(__thiscall*)(shok_GGL_CBattleBehavior *)>(0x4ECE77);
int shok_GGL_CLeaderBehavior::GetTroopHealthPerSoldier()
{
	return leaderbehgettroophealthpersol(this);
}

static inline float(__thiscall* const autocannonBehaviorGetMaxRange)(shok_GGL_CAutoCannonBehavior*) = reinterpret_cast<float(__thiscall*)(shok_GGL_CAutoCannonBehavior *)>(0x50F508);
float shok_GGL_CAutoCannonBehavior::GetMaxRange()
{
	return autocannonBehaviorGetMaxRange(this);
}

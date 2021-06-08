#include "pch.h"
#include "s5data.h"

struct shok_vtable_GGL_CBehaviorDefaultMovement {
	PADDINGI(9);
	float(__thiscall* GetSpeed)(shok_GGL_CBehaviorDefaultMovement* m);
};
float shok_GGL_CBehaviorDefaultMovement::GetMovementSpeed()
{
	return reinterpret_cast<shok_vtable_GGL_CBehaviorDefaultMovement*>(vtable)->GetSpeed(this);
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

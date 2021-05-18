#include "pch.h"
#include "s5data.h"

struct shok_vtable_GGL_CBehaviorDefaultMovement {
	PADDINGI(9);
	float(__thiscall* GetSpeed)(shok_GGL_CBehaviorDefaultMovement* m);
};
float shok_GGL_CBehaviorDefaultMovement::GetMovementSpeed()
{
	shok_vtable_GGL_CBehaviorDefaultMovement* vt = (shok_vtable_GGL_CBehaviorDefaultMovement*)vtable;
	return vt->GetSpeed(this);
}

bool shok_GGL_CCamouflageBehavior::IsThiefCamoBehavior()
{
	return vtable == shok_GGL_CThiefCamouflageBehavior::vtp;
}

static inline float(__thiscall* const battleBehaviorGetMaxRange)(shok_GGL_CBattleBehavior*) = (float(__thiscall*)(shok_GGL_CBattleBehavior * id))0x50AB43;
float shok_GGL_CBattleBehavior::GetMaxRange()
{
	return battleBehaviorGetMaxRange(this);
}

static inline int(__thiscall* const leaderbehgettroophealth)(shok_GGL_CBattleBehavior*) = (int(__thiscall*)(shok_GGL_CBattleBehavior * id))0x4EE1D6;
int shok_GGL_CLeaderBehavior::GetTroopHealth()
{
	return leaderbehgettroophealth(this);
}
static inline int(__thiscall* const leaderbehgettroophealthpersol)(shok_GGL_CBattleBehavior*) = (int(__thiscall*)(shok_GGL_CBattleBehavior * id))0x4ECE77;
int shok_GGL_CLeaderBehavior::GetTroopHealthPerSoldier()
{
	return leaderbehgettroophealthpersol(this);
}

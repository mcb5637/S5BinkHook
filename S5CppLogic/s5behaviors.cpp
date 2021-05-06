#include "pch.h"
#include "s5data.h"

struct shok_vtable_GGL_CBehaviorDefaultMovement {
	PADDINGI(9)
		float(__thiscall* GetSpeed)(shok_GGL_CBehaviorDefaultMovement* m);
};
float shok_GGL_CBehaviorDefaultMovement::GetMovementSpeed()
{
	shok_vtable_GGL_CBehaviorDefaultMovement* vt = (shok_vtable_GGL_CBehaviorDefaultMovement*)vtable;
	return vt->GetSpeed(this);
}

bool shok_GGL_CCamouflageBehavior::IsThiefCamoBehavior()
{
	return vtable == shok_vtp_GGL_CThiefCamouflageBehavior;
}

static inline float(__thiscall* battleBehaviorGetMaxRange)(shok_GGL_CBattleBehavior*) = (float(__thiscall*)(shok_GGL_CBattleBehavior * id))0x50AB43;
float shok_GGL_CBattleBehavior::GetMaxRange()
{
	return battleBehaviorGetMaxRange(this);
}
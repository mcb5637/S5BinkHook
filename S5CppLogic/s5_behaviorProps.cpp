#include "pch.h"
#include "s5_behaviorProps.h"

BB::SerializationData* EGL::CGLEBehaviorProps::SerializationData = reinterpret_cast<BB::SerializationData*>(0x86A768);

bool GGL::CRangedEffectAbilityProps::IsDefensive() const
{
	return (AffectsHostiles && DamageFactor > 0 && DamageFactor < 1) || ((AffectsFriends || AffectsOwn) && ArmorFactor > 1);
}

bool GGL::CRangedEffectAbilityProps::IsAggressive() const
{
	return (AffectsHostiles && ArmorFactor > 0 && ArmorFactor < 1) || ((AffectsFriends || AffectsOwn) && DamageFactor > 1);
}

bool GGL::CRangedEffectAbilityProps::IsHeal() const
{
	return (AffectsFriends || AffectsOwn) && HealthRecoveryFactor > 0;
}

inline int(__thiscall* const animexprops_getanimset)(GGL::CGLAnimationBehaviorExProps* th, int o) = reinterpret_cast<int(__thiscall*)(GGL::CGLAnimationBehaviorExProps*, int)>(0x4F5A4B);
int GGL::CGLAnimationBehaviorExProps::GetAnimSet(int modelOverride)
{
	return animexprops_getanimset(this, modelOverride);
}

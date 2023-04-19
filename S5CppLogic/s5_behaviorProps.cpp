#include "pch.h"
#include "s5_behaviorProps.h"

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

inline int(__thiscall* const foundryprops_gettl)(GGL::CFoundryBehaviorProperties* th, int ct) = reinterpret_cast<int(__thiscall*)(GGL::CFoundryBehaviorProperties*, int)>(0x50DEBA);
int GGL::CFoundryBehaviorProperties::GetTaskList(int cannonType)
{
	return foundryprops_gettl(this, cannonType);
}

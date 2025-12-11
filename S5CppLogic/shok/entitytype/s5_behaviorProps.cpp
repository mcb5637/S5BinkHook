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

inline shok::AnimSetId(__thiscall* const animexprops_getanimset)(GGL::CGLAnimationBehaviorExProps* th, shok::ModelId o) = reinterpret_cast<shok::AnimSetId(__thiscall*)(GGL::CGLAnimationBehaviorExProps*, shok::ModelId)>(0x4F5A4B);
shok::AnimSetId GGL::CGLAnimationBehaviorExProps::GetAnimSet(shok::ModelId modelOverride)
{
	return animexprops_getanimset(this, modelOverride);
}

inline shok::TaskListId(__thiscall* const foundryprops_gettl)(GGL::CFoundryBehaviorProperties* th, shok::EntityTypeId ct) = reinterpret_cast<shok::TaskListId(__thiscall*)(GGL::CFoundryBehaviorProperties*, shok::EntityTypeId)>(0x50DEBA);
shok::TaskListId GGL::CFoundryBehaviorProperties::GetTaskList(shok::EntityTypeId cannonType)
{
	return foundryprops_gettl(this, cannonType);
}

float GGL::CServiceBuildingBehaviorProperties::GetProgressAmount(shok::PlayerId p) const
{
	return reinterpret_cast<float(__thiscall*)(const CServiceBuildingBehaviorProperties*, shok::PlayerId)>(0x4DD186)(this, p);
}

#include "pch.h"
#include "s5data.h"

bool shok_GGL_CRangedEffectAbilityProps::IsDefensive()
{
	return (AffectsHostiles && DamageFactor > 0 && DamageFactor < 1) || ((AffectsFriends || AffectsOwn) && ArmorFactor > 1);
}

bool shok_GGL_CRangedEffectAbilityProps::IsAggressive()
{
	return (AffectsHostiles && ArmorFactor > 0 && ArmorFactor < 1) || ((AffectsFriends || AffectsOwn) && DamageFactor > 1);
}

bool shok_GGL_CRangedEffectAbilityProps::IsHeal()
{
	return (AffectsFriends || AffectsOwn) && HealthRecoveryFactor > 0;
}

#include "pch.h"
#include "s5data.h"

shok_BB_CClassFactory_serializationData* shok_EGL_CGLEBehaviorProps::SerializationData = reinterpret_cast<shok_BB_CClassFactory_serializationData*>(0x86A768);

bool shok_GGL_CRangedEffectAbilityProps::IsDefensive() const
{
	return (AffectsHostiles && DamageFactor > 0 && DamageFactor < 1) || ((AffectsFriends || AffectsOwn) && ArmorFactor > 1);
}

bool shok_GGL_CRangedEffectAbilityProps::IsAggressive() const
{
	return (AffectsHostiles && ArmorFactor > 0 && ArmorFactor < 1) || ((AffectsFriends || AffectsOwn) && DamageFactor > 1);
}

bool shok_GGL_CRangedEffectAbilityProps::IsHeal() const
{
	return (AffectsFriends || AffectsOwn) && HealthRecoveryFactor > 0;
}

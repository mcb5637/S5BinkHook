#include "pch.h"
#include "s5data.h"

shok_BB_CClassFactory_serializationData* EGL::CGLEBehaviorProps::SerializationData = reinterpret_cast<shok_BB_CClassFactory_serializationData*>(0x86A768);

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
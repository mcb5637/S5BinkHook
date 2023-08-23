#include "pch.h"
#include "s5_staticlist.h"

static inline void(__thiscall* const staticheroab_ctor)(GGL::CStaticHeroAbilityID* th, const char* n, shok::AbilityId id) = reinterpret_cast<void(__thiscall*)(GGL::CStaticHeroAbilityID*, const char*, shok::AbilityId)>(0x4F4B5B);
GGL::CStaticHeroAbilityID::CStaticHeroAbilityID(const char* name, shok::AbilityId id)
{
	staticheroab_ctor(this, name, id);
}

void GGL::CStaticHeroAbilityID::Apply() const
{
	throw 0;
}

void GGL::CStaticCategoryID::Apply() const
{
	throw 0;
}

static inline void(__thiscall* const staticcat_ctor)(GGL::CStaticCategoryID* th, const char* n) = reinterpret_cast<void(__thiscall*)(GGL::CStaticCategoryID*, const char*)>(0x44B1BF);
GGL::CStaticCategoryID::CStaticCategoryID(const char* name)
{
	staticcat_ctor(this, name);
}

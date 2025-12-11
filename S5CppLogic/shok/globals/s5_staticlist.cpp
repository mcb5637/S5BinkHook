#include "pch.h"
#include "s5_staticlist.h"

static inline void(__thiscall* const staticheroab_ctor)(GGL::CStaticHeroAbilityID* th, const char* n, shok::AbilityId id) = reinterpret_cast<void(__thiscall*)(GGL::CStaticHeroAbilityID*, const char*, shok::AbilityId)>(0x4F4B5B);
GGL::CStaticHeroAbilityID::CStaticHeroAbilityID(const char* name, shok::AbilityId id)
{
	staticheroab_ctor(this, name, id);
}

void GGL::CStaticHeroAbilityID::Apply() const
{
	const auto f = reinterpret_cast<void(__thiscall*)(const GGL::CStaticHeroAbilityID*)>(0x513fd6);
	f(this);
}

void GGL::CStaticCategoryID::Apply() const
{
	const auto f = reinterpret_cast<void(__thiscall*)(const GGL::CStaticCategoryID*)>(0x4b9248);
	f(this);
}

static inline void(__thiscall* const staticcat_ctor)(GGL::CStaticCategoryID* th, const char* n) = reinterpret_cast<void(__thiscall*)(GGL::CStaticCategoryID*, const char*)>(0x44B1BF);
GGL::CStaticCategoryID::CStaticCategoryID(const char* name)
{
	staticcat_ctor(this, name);
}

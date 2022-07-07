#include "pch.h"
#include "s5_staticlist.h"

static inline void(__thiscall* const staticheroab_ctor)(GGL::CStaticHeroAbilityID* th, const char* n, int id) = reinterpret_cast<void(__thiscall*)(GGL::CStaticHeroAbilityID*, const char*, int)>(0x4F4B5B);
GGL::CStaticHeroAbilityID::CStaticHeroAbilityID(const char* name, int id)
{
	staticheroab_ctor(this, name, id);
}

void GGL::CStaticHeroAbilityID::Apply() const
{
	throw 0;
}

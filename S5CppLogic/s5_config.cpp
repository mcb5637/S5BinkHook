#include "pch.h"
#include <stdexcept>
#include "s5_config.h"

float& GGL::CDamageClassProps::GetBonusVsArmorClass(int ac)
{
    --ac;
    if (ac < 0 || ac > 7)
        throw std::out_of_range{ "invalid armorclass" };
    return BonusVsArmorClass[ac];
}

GGL::CLogicProperties::SSettlerUpgradeCategory::SSettlerUpgradeCategory(int cat, int first)
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = cat;
    FirstSettler = first;
}
GGL::CLogicProperties::SSettlerUpgradeCategory::SSettlerUpgradeCategory(SSettlerUpgradeCategory&& o) noexcept
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = o.Category;
    FirstSettler = o.FirstSettler;
}
unsigned int GGL::CLogicProperties::SSettlerUpgradeCategory::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CLogicProperties::SBuildingUpgradeCategory::SBuildingUpgradeCategory(int cat, int first)
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = cat;
    FirstBuilding = first;
    NumScholars = 0;
}
GGL::CLogicProperties::SBuildingUpgradeCategory::SBuildingUpgradeCategory(SBuildingUpgradeCategory&& o) noexcept
{
    *reinterpret_cast<int*>(this) = vtp;
    Category = o.Category;
    FirstBuilding = o.FirstBuilding;
    NumScholars = o.NumScholars;
}
unsigned int GGL::CLogicProperties::SBuildingUpgradeCategory::GetClassIdentifier() const
{
    return Identifier;
}

static inline GGL::ExperienceClass::LevelData* (__thiscall* const xpclass_getlevel)(GGL::ExperienceClass* th, int l) = reinterpret_cast<GGL::ExperienceClass::LevelData * (__thiscall*)(GGL::ExperienceClass*, int)>(0x515270);
GGL::ExperienceClass::LevelData* GGL::ExperienceClass::GetLevel(int lvl)
{
    return xpclass_getlevel(this, lvl);
}

static inline GGL::ExperienceClass* (__thiscall* const xpclassholder_getclass)(GGL::ExperienceClassHolder* th, shok::ExperienceClass cl) = reinterpret_cast<GGL::ExperienceClass * (__thiscall*)(GGL::ExperienceClassHolder*, shok::ExperienceClass)>(0x515299);
GGL::ExperienceClass* GGL::ExperienceClassHolder::GetClass(shok::ExperienceClass cl)
{
    return xpclassholder_getclass(this, cl);
}

static inline void(__thiscall* const xpclassholder_load)(GGL::ExperienceClassHolder* th) = reinterpret_cast<void(__thiscall*)(GGL::ExperienceClassHolder*)>(0x515520);
GGL::ExperienceClassHolder* GGL::ExperienceClassHolder::GlobalObj()
{
    auto* r = reinterpret_cast<ExperienceClassHolder*>(0x868ED4);
    if (!r->Loaded)
        xpclassholder_load(r);
    return r;
}
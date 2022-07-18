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
    std::memcpy(this, &o, sizeof(SSettlerUpgradeCategory));
}
unsigned int GGL::CLogicProperties::SSettlerUpgradeCategory::GetClassIdentifier() const
{
    return Identifier;
}

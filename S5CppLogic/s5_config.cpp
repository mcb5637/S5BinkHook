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

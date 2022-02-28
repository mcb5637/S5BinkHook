#include "pch.h"
#include "s5data.h"

float& GGL::CDamageClassProps::GetBonusVsArmorClass(int ac)
{
    --ac;
    if (ac < 0 || ac > 7)
        throw std::out_of_range{ "invalid armorclass" };
    return BonusVsArmorClass[ac];
}

shok::Color::Color(int r, int g, int b, int a)
{
	R = r & 0xFF;
	G = g & 0xFF;
	B = b & 0xFF;
	A = a & 0xFF;
}

#include "pch.h"
#include "s5_RWE_2d.h"


static inline float* (__cdecl* const font_lengthdata_get)(void* ld) = reinterpret_cast<float* (__cdecl*)(void*)>(0x625D00);
float* RWE::P2D::Rt2dFont::GetLengthData()
{
    return font_lengthdata_get(LengthData);
}
static inline float(__cdecl* const font_getheight)(RWE::P2D::Rt2dFont* th) = reinterpret_cast<float(__cdecl*)(RWE::P2D::Rt2dFont*)>(0x708ED0);
float RWE::P2D::Rt2dFont::GetHeight()
{
    return font_getheight(this);
}
static inline float(__cdecl* const font_getstringwidth)(RWE::P2D::Rt2dFont* th, const char* s, float m) = reinterpret_cast<float(__cdecl*)(RWE::P2D::Rt2dFont*, const char*, float)>(0x708F00);
float RWE::P2D::Rt2dFont::GetStringWidth(const char* s, float height)
{
    return font_getstringwidth(this, s, height);
}
float RWE::P2D::Rt2dFont::GetStringWidth(const wchar_t* s, float height)
{
    return GetStringWidth(reinterpret_cast<const char*>(s), height);
}
void RWE::P2D::Rt2dFont::RenderText(const wchar_t* s, float height, RWE::RwV2d* anchor, Rt2dBrush* brush)
{
    RenderTxt(this, reinterpret_cast<const char*>(s), height, anchor, brush);
}
void RWE::P2D::Rt2dFont::RenderText(const char* s, float height, RWE::RwV2d* anchor, Rt2dBrush* brush)
{
    RenderTxt(this, s, height, anchor, brush);
}

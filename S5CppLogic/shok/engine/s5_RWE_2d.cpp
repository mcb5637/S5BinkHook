#include "pch.h"
#include "s5_RWE_2d.h"

inline RWE::P2D::Rt2dBrush* (__cdecl* const brush_settexture)(RWE::P2D::Rt2dBrush* th, RWE::RwTexture* tex) = reinterpret_cast<RWE::P2D::Rt2dBrush * (__cdecl*)(RWE::P2D::Rt2dBrush*, RWE::RwTexture*)>(0x707A00);
RWE::P2D::Rt2dBrush* RWE::P2D::Rt2dBrush::SetTexture(RwTexture* texture)
{
    return brush_settexture(this, texture);
}
inline RWE::P2D::Rt2dBrush* (__cdecl* const brush_setuv)(RWE::P2D::Rt2dBrush* th, RWE::RwV2d* uv0, RWE::RwV2d* uv1, RWE::RwV2d* uv2, RWE::RwV2d* uv3) = reinterpret_cast<RWE::P2D::Rt2dBrush * (__cdecl*)(RWE::P2D::Rt2dBrush*, RWE::RwV2d*, RWE::RwV2d*, RWE::RwV2d*, RWE::RwV2d*)>(0x707950);
RWE::P2D::Rt2dBrush* RWE::P2D::Rt2dBrush::SetUV(RwV2d* uv0, RwV2d* uv1, RwV2d* uv2, RwV2d* uv3)
{
    return brush_setuv(this, uv0, uv1, uv2, uv3);
}
inline RWE::P2D::Rt2dBrush* (__cdecl* const brush_setrgba)(RWE::P2D::Rt2dBrush* brush, RWE::RwRGBA* col0, RWE::RwRGBA* col1, RWE::RwRGBA* col2, RWE::RwRGBA* col3) = reinterpret_cast<RWE::P2D::Rt2dBrush * (__cdecl*)(RWE::P2D::Rt2dBrush*, RWE::RwRGBA*, RWE::RwRGBA*, RWE::RwRGBA*, RWE::RwRGBA*)>(0x7077F0);
RWE::P2D::Rt2dBrush* RWE::P2D::Rt2dBrush::SetRGBA(RwRGBA* col0, RwRGBA* col1, RwRGBA* col2, RwRGBA* col3)
{
    return brush_setrgba(this, col0, col1, col2, col3);
}
inline int (__cdecl* const brush_destroy)(RWE::P2D::Rt2dBrush* th) = reinterpret_cast<int (__cdecl*)(RWE::P2D::Rt2dBrush*)>(0x7076B0);
int RWE::P2D::Rt2dBrush::Destroy()
{
    return brush_destroy(this);
}

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
bool RWE::P2D::Rt2dFont::IsWchar()
{
    return Flags & 2;
}

inline void(__cdecl* const font_destroy)(RWE::P2D::Rt2dFont* f) = reinterpret_cast<void(__cdecl*)(RWE::P2D::Rt2dFont*)>(0x708FC0);
void RWE::P2D::Rt2dFont::Destroy()
{
    font_destroy(this);
}

#include "pch.h"
#include "s5_BBRw.h"

static inline BB::TResourceProxyResMgr<RWE::RwTexture*>** (__thiscall* const crwtextures_get)(BBRw::CRwTextures* th, const char* n, int z) = reinterpret_cast<BB::TResourceProxyResMgr<RWE::RwTexture*>**(__thiscall*)(BBRw::CRwTextures*, const char*, int)>(0x472CCA);
BB::TResourceProxyResMgr<RWE::RwTexture*>* BBRw::CRwTextures::Get(const char* name)
{
    return *crwtextures_get(this, name, 0);
}
static inline BB::TResourceProxyResMgr<RWE::RwTexture*>** (__stdcall* const crwtextures_getid)(BBRw::CRwTextures* th, int id) = reinterpret_cast<BB::TResourceProxyResMgr<RWE::RwTexture*>**(__stdcall*)(BBRw::CRwTextures*, int)>(0x467D46);
BB::TResourceProxyResMgr<RWE::RwTexture*>* BBRw::CRwTextures::Get(int id)
{
    return *crwtextures_getid(this, id);
}

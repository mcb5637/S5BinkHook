#include "s5_BBRw.h"

static inline BB::TResourceProxyResMgr<RWE::RwTexture*>** (__thiscall* const crwtextures_get)(BBRw::CRwTextures* th, const char* n, int z) = reinterpret_cast<BB::TResourceProxyResMgr<RWE::RwTexture*>**(__thiscall*)(BBRw::CRwTextures*, const char*, int)>(0x472CCA);
BB::TResourceProxyResMgr<RWE::RwTexture*>* BBRw::CRwTextures::Get(const char* name)
{
    return *crwtextures_get(this, name, 0);
}
static inline BB::TResourceProxyResMgr<RWE::RwTexture*>** (__stdcall* const crwtextures_getid)(BBRw::CRwTextures* th, shok::SelectionTextureId id) = reinterpret_cast<BB::TResourceProxyResMgr<RWE::RwTexture*>**(__stdcall*)(BBRw::CRwTextures*, shok::SelectionTextureId)>(0x467D46);
BB::TResourceProxyResMgr<RWE::RwTexture*>* BBRw::CRwTextures::Get(shok::SelectionTextureId id)
{
    return *crwtextures_getid(this, id);
}

inline BB::TResourceProxyResMgr<BBRw::CEffect*>** (__thiscall* const ieffects_get)(BBRw::IEffects* th, const char* n, int z) = reinterpret_cast<BB::TResourceProxyResMgr<BBRw::CEffect*>**(__thiscall*)(BBRw::IEffects*, const char*, int)>(0x46B83B);
BB::TResourceProxyResMgr<BBRw::CEffect*>* BBRw::IEffects::Get(const char* name)
{
    return *ieffects_get(this, name, 0);
}

#include "pch.h"
#include "s5_RWEngine.h"
#include "s5_mapdisplay.h"

static inline RWE::RwMatrix* (__cdecl* const matrix_optimize)(RWE::RwMatrix* m, const void* tolerance) = reinterpret_cast<RWE::RwMatrix* (__cdecl*)(RWE::RwMatrix*, const void*)>(0x41B2A0);
RWE::RwMatrix* RWE::RwMatrix::Optimize()
{
    return matrix_optimize(this, nullptr);
}
static inline RWE::RwMatrix* (__cdecl* const matrix_rotate)(RWE::RwMatrix* m, const RWE::RwV3d* ax, float an, RWE::RwOpCombineType c) = reinterpret_cast<RWE::RwMatrix * (__cdecl*)(RWE::RwMatrix*, const RWE::RwV3d*, float, RWE::RwOpCombineType)>(0x41B5F0);
RWE::RwMatrix* RWE::RwMatrix::Rotate(const RwV3d* axis, float angle, RwOpCombineType combineOp)
{
    return matrix_rotate(this, axis, angle, combineOp);
}
static inline RWE::RwMatrix* (__cdecl* const matrix_scale)(RWE::RwMatrix* m, const RWE::RwV3d* scale, RWE::RwOpCombineType c) = reinterpret_cast<RWE::RwMatrix * (__cdecl*)(RWE::RwMatrix*, const RWE::RwV3d*, RWE::RwOpCombineType)>(0x41B6E0);
RWE::RwMatrix* RWE::RwMatrix::Scale(const RwV3d* scale, RwOpCombineType combineOp)
{
    return matrix_scale(this, scale, combineOp);
}
static inline RWE::RwMatrix* (__cdecl* const matrix_translate)(RWE::RwMatrix* m, const RWE::RwV3d* t, RWE::RwOpCombineType c) = reinterpret_cast<RWE::RwMatrix * (__cdecl*)(RWE::RwMatrix*, const RWE::RwV3d*, RWE::RwOpCombineType)>(0x41B870);
RWE::RwMatrix* RWE::RwMatrix::Translate(const RwV3d* translation, RwOpCombineType combineOp)
{
    return matrix_translate(this, translation, combineOp);
}

static inline RWE::RwFrame* (__cdecl* const frame_updateobjects)(RWE::RwFrame* f) = reinterpret_cast<RWE::RwFrame* (__cdecl*)(RWE::RwFrame*)>(0x413FE0);
RWE::RwFrame* RWE::RwFrame::UpdateObjects()
{
    return frame_updateobjects(this);
}
static inline RWE::RwFrame* (__cdecl* const frame_rotate)(RWE::RwFrame* f, const RWE::RwV3d* axis, float angle, RWE::RwOpCombineType combineOp) = reinterpret_cast<RWE::RwFrame * (__cdecl*)(RWE::RwFrame*, const RWE::RwV3d*, float, RWE::RwOpCombineType)>(0x4141D0);
RWE::RwFrame* RWE::RwFrame::Rotate(const RwV3d* axis, float angle, RwOpCombineType combineOp)
{
    return frame_rotate(this, axis, angle, combineOp);
}
RWE::RwFrame* RWE::RwFrame::Rotate(float angle, RwOpCombineType combineOp)
{
    RwV3d axis{ 0,0,1 };
    return Rotate(&axis, angle, combineOp);
}
static inline RWE::RwFrame* (__cdecl* const frame_scale)(RWE::RwFrame* f, const RWE::RwV3d* scale, RWE::RwOpCombineType combineOp) = reinterpret_cast<RWE::RwFrame * (__cdecl*)(RWE::RwFrame*, const RWE::RwV3d*, RWE::RwOpCombineType)>(0x414170);
RWE::RwFrame* RWE::RwFrame::Scale(const RwV3d* scale, RwOpCombineType combineOp)
{
    return frame_scale(this, scale, combineOp);
}
RWE::RwFrame* RWE::RwFrame::Scale(float scale, RwOpCombineType combineOp)
{
    RwV3d s{ scale,scale,scale };
    return Scale(&s, combineOp);
}
static inline RWE::RwFrame* (__cdecl* const frame_translate)(RWE::RwFrame* f, const RWE::RwV3d* t, RWE::RwOpCombineType combineOp) = reinterpret_cast<RWE::RwFrame * (__cdecl*)(RWE::RwFrame*, const RWE::RwV3d*, RWE::RwOpCombineType)>(0x414140);
RWE::RwFrame* RWE::RwFrame::Translate(const RwV3d* translation, RwOpCombineType combineOp)
{
    return frame_translate(this, translation, combineOp);
}
static inline RWE::RwFrame* (__cdecl* const frame_forallchildren)(RWE::RwFrame* f, RWE::RwFrameCallBack cb, void* d) = reinterpret_cast<RWE::RwFrame* (__cdecl*)(RWE::RwFrame*, RWE::RwFrameCallBack, void*)>(0x414100);
RWE::RwFrame* RWE::RwFrame::ForAllChildren(RwFrameCallBack callBack, void* data)
{
    return frame_forallchildren(this, callBack, data);
}
static inline RWE::RwFrameCallBack frame_getanimhandler = reinterpret_cast<RWE::RwFrameCallBack>(0x720440);
RWE::RtAnimationFrameHandler* RWE::RwFrame::GetAnimFrameHandler()
{
    RWE::RtAnimationFrameHandler* h = nullptr;
    ForAllChildren(frame_getanimhandler, &h);
    return h;
}

static inline int(__cdecl* const raster_destroy)(RWE::RwRaster* r) = reinterpret_cast<int(__cdecl*)(RWE::RwRaster*)>(0x418A90);
void RWE::RwRaster::Destroy()
{
    raster_destroy(this);
}

static inline RWE::RpClump* (__cdecl* const clump_forallatomics)(RWE::RpClump* m, RWE::RpAtomicCallBack, void* data) = reinterpret_cast<RWE::RpClump* (__cdecl*)(RWE::RpClump*, RWE::RpAtomicCallBack, void*)>(0x628E30);
RWE::RpClump* RWE::RpClump::ForAllAtomics(RpAtomicCallBack callback, void* pData)
{
    return clump_forallatomics(this, callback, pData);
}
static inline RWE::RpWorld* (__cdecl* const clump_getworld)(const RWE::RpClump* c) = reinterpret_cast<RWE::RpWorld* (__cdecl*)(const RWE::RpClump*)>(0x626E00);
RWE::RpWorld* RWE::RpClump::GetWorld() const
{
    return clump_getworld(this);
}
static inline int(__cdecl* const clump_destroy)(RWE::RpClump* c) = reinterpret_cast<int(__cdecl*)(RWE::RpClump*)>(0x6294A0);
void RWE::RpClump::Destroy()
{
    auto* w = GetWorld();
    if (w)
        w->RemoveClump(this);
    clump_destroy(this);
}
void RWE::RpClump::AddToDefaultWorld()
{
    (*ED::CGlobalsBaseEx::GlobalObj)->DisplayWorld->World->AddClump(this);
}
static inline RWE::RpClump* (__cdecl* const clump_create)() = reinterpret_cast<RWE::RpClump* (__cdecl*)()>(0x629010);
RWE::RpClump* RWE::RpClump::Create()
{
    return clump_create();
}
static inline RWE::RpClump* (__cdecl* const clump_clone)(const RWE::RpClump* c) = reinterpret_cast<RWE::RpClump* (__cdecl*)(const RWE::RpClump*)>(0x6298E0);
RWE::RpClump* RWE::RpClump::Clone() const
{
    return clump_clone(this);
}
RWE::RwFrame* RWE::RpClump::GetFrame() const
{
    return static_cast<RWE::RwFrame*>(object.parent);
}
void RWE::RpClump::SetPlayerColor(int pl)
{
    ForAllAtomics(RpAtomic::SetPlayerColorCb, &pl);
}
void RWE::RpClump::DisableShadow()
{
    ForAllAtomics(RpAtomic::DisableShadowCb, nullptr);
}
void RWE::RpClump::DisableParticleEffects()
{
    ForAllAtomics(RpAtomic::DisableParticleEffectsCb, nullptr);
}
void RWE::RpClump::DisableTerrainDecal()
{
    ForAllAtomics(RpAtomic::DisableTerrainDecalCb, nullptr);
}
void RWE::RpClump::SetColorModulate(shok::Color argb)
{
    ForAllAtomics(RpAtomic::SetColorModulateCb, &argb);
}

static inline RWE::RpWorld* (__cdecl* const world_addclump)(RWE::RpWorld* w, RWE::RpClump* c) = reinterpret_cast<RWE::RpWorld * (__cdecl*)(RWE::RpWorld*, RWE::RpClump*)>(0x627130);
RWE::RpWorld* RWE::RpWorld::AddClump(RpClump* clump)
{
    return world_addclump(this, clump);
}
static inline RWE::RpWorld* (__cdecl* const world_remclump)(RWE::RpWorld* w, RWE::RpClump* c) = reinterpret_cast<RWE::RpWorld * (__cdecl*)(RWE::RpWorld*, RWE::RpClump*)>(0x6271C0);
RWE::RpWorld* RWE::RpWorld::RemoveClump(RpClump* clump)
{
    return world_remclump(this, clump);
}

static inline RWE::RtAnimationFrameHandler* (__cdecl* const animframehandler_clone)(RWE::RtAnimationFrameHandler* th, RWE::RtAnimationFrameHandler::AnimFlags d, void* da) = reinterpret_cast<RWE::RtAnimationFrameHandler * (__cdecl*)(RWE::RtAnimationFrameHandler*, RWE::RtAnimationFrameHandler::AnimFlags, void*)>(0x6EBB30);
RWE::RtAnimationFrameHandler* RWE::RtAnimationFrameHandler::Clone()
{
    return animframehandler_clone(this, Flags, Animation->Data);
}
static inline void(__cdecl* const animframehandler_anim_setanim)(RWE::RtAnimationFrameHandler::AnimData* th, RWE::RtAnimAnimation* a) = reinterpret_cast<void(__cdecl*)(RWE::RtAnimationFrameHandler::AnimData*, RWE::RtAnimAnimation*)>(0x6EAE10);
void RWE::RtAnimationFrameHandler::SetAnimation(RWE::RtAnimAnimation* a)
{
    animframehandler_anim_setanim(Animation, a);
}
static inline void(__cdecl* const animframehandler_destroy)(RWE::RtAnimationFrameHandler* th) = reinterpret_cast<void(__cdecl*)(RWE::RtAnimationFrameHandler*)>(0x6EB990);
void RWE::RtAnimationFrameHandler::Destroy()
{
    animframehandler_destroy(this);
}
static inline void(__cdecl* const animframehandler_anim_settime)(RWE::RtAnimationFrameHandler::AnimData* th, float t) = reinterpret_cast<void(__cdecl*)(RWE::RtAnimationFrameHandler::AnimData*, float t)>(0x6EAF10);
void RWE::RtAnimationFrameHandler::SetTimeOfAnim(float t)
{
    animframehandler_anim_settime(Animation, t);
}
static inline void(__cdecl* const animframehandler_apply)(RWE::RtAnimationFrameHandler* th) = reinterpret_cast<void(__cdecl*)(RWE::RtAnimationFrameHandler*)>(0x6EC080);
void RWE::RtAnimationFrameHandler::ApplyTransforms()
{
    animframehandler_apply(this);
}

void RWE::RtAnimationFrameHandler::SetupForModel(RWE::RpClump* c)
{
    struct data {
        RWE::RtAnimationFrameHandler* th;
        bool found = false;
    };
    data d{ this };
    c->ForAllAtomics(reinterpret_cast<RWE::RpAtomicCallBack>(0x487D1A), &d);
    struct unknownclumpaddon {
        PADDINGI(3);
        ED::ModelData* ModelData;
    };
    unknownclumpaddon* a = reinterpret_cast<unknownclumpaddon*>(reinterpret_cast<int>(c) + *reinterpret_cast<int*>(0x858228));
    ED::ModelData::ModelFlags flags = ED::ModelData::ModelFlags::None;
    if (a->ModelData) {
        flags = a->ModelData->Flags;
    }
    if (d.found && (flags & ED::ModelData::ModelFlags::HierarchyDestroyed) != ED::ModelData::ModelFlags::None) {
        //reinterpret_cast<byte*>(this)[1] &= 0xCFu; // ~0x30
        Flags = Flags & ~(AnimFlags::DoNotDestroyHierarchy | AnimFlags::HierarchyNotDestroyed);
    }
    else {
        //reinterpret_cast<byte*>(this)[1] |= 0x10;
        Flags = Flags | AnimFlags::DoNotDestroyHierarchy;
        reinterpret_cast<void(__cdecl*)(RWE::RtAnimationFrameHandler*)>(0x6EC7E0)(this);
    }
}

static inline RwTexture* (__cdecl* const texture_read)(const char* n, const char* m) = reinterpret_cast<RwTexture* (__cdecl*)(const char*, const char*)>(0x417DB0);
RwTexture* RwTexture::Read(const char* name, const char* mask)
{
    return texture_read(name, mask);
}
static inline int(__cdecl* const texture_destroy)(RwTexture* t) = reinterpret_cast<int(__cdecl*)(RwTexture * t)>(0x417C00);
void RwTexture::Destroy()
{
    texture_destroy(this);
}

static inline RtAnimAnimation* (__cdecl* const anim_read)(const char* name) = reinterpret_cast<RtAnimAnimation * (__cdecl*)(const char*)>(0x6EB180);
RtAnimAnimation* RtAnimAnimation::Read(const char* name)
{
    return anim_read(name);
}
static inline void(__cdecl* const anim_destroy)(RtAnimAnimation* a) = reinterpret_cast<void(__cdecl*)(RtAnimAnimation*)>(0x6EAC50);
void RtAnimAnimation::Destroy()
{
    anim_destroy(this);
}

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

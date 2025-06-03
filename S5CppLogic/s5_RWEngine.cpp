#include "pch.h"
#include "s5_RWEngine.h"
#include "s5_mapdisplay.h"

inline void(__cdecl* const vec3d_transform)(RWE::RwV3d* ou, const RWE::RwV3d* i, const RWE::RwMatrix* m) = reinterpret_cast<void(__cdecl*)(RWE::RwV3d*, const RWE::RwV3d*, const RWE::RwMatrix*)>(0x41C770);
RWE::RwV3d RWE::RwV3d::Transform(const RwMatrix* matrix) const
{
    RwV3d r{};
    vec3d_transform(&r, this, matrix);
    return r;
}
inline void(__cdecl* const vec3d_transformPoint)(RWE::RwV3d* ou, const RWE::RwV3d* i, const RWE::RwMatrix* m) = reinterpret_cast<void(__cdecl*)(RWE::RwV3d*, const RWE::RwV3d*, const RWE::RwMatrix*)>(0x41C710);
RWE::RwV3d RWE::RwV3d::TransformPoint(const RwMatrix* matrix) const
{
    RwV3d r{};
    vec3d_transformPoint(&r, this, matrix);
    return r;
}
inline void(__cdecl* const vec3d_normalize)(RWE::RwV3d* ou, const RWE::RwV3d* i) = reinterpret_cast<void(__cdecl*)(RWE::RwV3d*, const RWE::RwV3d*)>(0x41C920);
RWE::RwV3d RWE::RwV3d::Normalize() const
{
    RWE::RwV3d o{};
    vec3d_normalize(&o, this);
    return o;
}
inline float(__cdecl* const vec3d_len)(const RWE::RwV3d* i) = reinterpret_cast<float(__cdecl*)(const RWE::RwV3d*)>(0x41CA30);
float RWE::RwV3d::Length() const
{
    return vec3d_len(this);
}

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
inline RWE::RwMatrix* (__cdecl* const matrix_mul)(RWE::RwMatrix* out, const RWE::RwMatrix* i1, const RWE::RwMatrix* i2) = reinterpret_cast<RWE::RwMatrix * (__cdecl*)(RWE::RwMatrix*, const RWE::RwMatrix*, const RWE::RwMatrix*)>(0x41B370);
RWE::RwMatrix* RWE::RwMatrix::Multiply(const RwMatrix* MatrixIn1, const RwMatrix* matrixIn2)
{
    return matrix_mul(this, MatrixIn1, matrixIn2);
}
inline RWE::RwMatrix* (__cdecl* const matrix_inv)(RWE::RwMatrix* out, const RWE::RwMatrix* i) = reinterpret_cast<RWE::RwMatrix * (__cdecl*)(RWE::RwMatrix*, const RWE::RwMatrix*)>(0x41B690);
RWE::RwMatrix* RWE::RwMatrix::Invert(const RwMatrix* matrixIn)
{
    return matrix_inv(this, matrixIn);
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
inline RWE::RwFrame* (__cdecl* const frame_forallobj)(RWE::RwFrame* f, RWE::RwObjectCallBack cb, void* d) = reinterpret_cast<RWE::RwFrame * (__cdecl*)(RWE::RwFrame*, RWE::RwObjectCallBack, void*)>(0x414270);
RWE::RwFrame* RWE::RwFrame::ForAllObjects(RwObjectCallBack callBack, void* data)
{
    return frame_forallobj(this, callBack, data);
}
inline RWE::RpAtomic* (__cdecl* const frame_getfirstatomic)(RWE::RwFrame* f) = reinterpret_cast<RWE::RpAtomic * (__cdecl*)(RWE::RwFrame*)>(0x49305C);
RWE::RpAtomic* RWE::RwFrame::GetFirstAtomic()
{
    return frame_getfirstatomic(this);
}
static inline RWE::RwFrameCallBack frame_getanimhandler = reinterpret_cast<RWE::RwFrameCallBack>(0x720440);
RWE::Anim::RpHAnimHierarchy* RWE::RwFrame::GetAnimFrameHandler()
{
    RWE::Anim::RpHAnimHierarchy* h = nullptr;
    ForAllChildren(frame_getanimhandler, &h);
    return h;
}
inline RWE::RwMatrix* (__cdecl* const frame_getltm)(RWE::RwFrame* f) = reinterpret_cast<RWE::RwMatrix * (__cdecl*)(RWE::RwFrame*)>(0x414060);
RWE::RwMatrix* RWE::RwFrame::GetLTM()
{
    return frame_getltm(this);
}
inline void(__cdecl* const frame_destroy)(RWE::RwFrame* f) = reinterpret_cast<void(__cdecl*)(RWE::RwFrame*)>(0x414470);
void RWE::RwFrame::Destroy()
{
    frame_destroy(this);
}
bool RWE::RwFrame::Dirty() const
{
    int(__cdecl* const f)(const RWE::RwFrame * f) = reinterpret_cast<int(__cdecl*)(const RWE::RwFrame*)>(0x413E40);
    return f(this);
}

static inline int(__cdecl* const stream_close)(RWE::RwStream* s, void* d) = reinterpret_cast<int(__cdecl*)(RWE::RwStream*, void*)>(0x41A810);
bool RWE::RwStream::Close(void* data)
{
    return stream_close(this, data);
}
static inline RWE::RwStream*(__cdecl* const stream_skip)(RWE::RwStream* s, size_t l) = reinterpret_cast<RWE::RwStream*(__cdecl*)(RWE::RwStream*, size_t)>(0x41A700);
void RWE::RwStream::Skip(size_t len)
{
    stream_skip(this, len);
}
static inline int (__cdecl* const stream_findchunk)(RWE::RwStream* s, RWE::RwCorePluginID ty, size_t* l, unsigned int* v) = reinterpret_cast<int (__cdecl*)(RWE::RwStream*, RWE::RwCorePluginID ty, size_t*, unsigned int*)>(0x4290A0);
bool RWE::RwStream::FindChunk(RwCorePluginID ty, size_t* length, unsigned int* version)
{
    return stream_findchunk(this, ty, length, version);
}
static inline size_t(__cdecl* const stream_read)(RWE::RwStream* s, void* b, size_t l) = reinterpret_cast<size_t(__cdecl*)(RWE::RwStream*, void*, size_t)>(0x41A3E0);
size_t RWE::RwStream::Read(void* buff, size_t toRead)
{
    return stream_read(this, buff, toRead);
}

static inline int(__cdecl* const raster_destroy)(RWE::RwRaster* r) = reinterpret_cast<int(__cdecl*)(RWE::RwRaster*)>(0x418A90);
void RWE::RwRaster::Destroy()
{
    raster_destroy(this);
}
inline RWE::RwRaster* (__cdecl* const raster_show)(RWE::RwRaster* th, HWND w, int vs) = reinterpret_cast<RWE::RwRaster * (__cdecl*)(RWE::RwRaster*, HWND, int)>(0x418B70);
bool RWE::RwRaster::ShowRaster(HWND window, bool vsync)
{
    return raster_show(this, window, vsync ? 1 : 0);
}
static inline byte* (__cdecl* const raster_lock)(RWE::RwRaster* raster, byte level, RWE::RwRasterLockMode lockMode) = reinterpret_cast<byte * (__cdecl*)(RWE::RwRaster*, byte, RWE::RwRasterLockMode)>(0x418C40);
byte* RWE::RwRaster::Lock(byte level, RwRasterLockMode lockMode)
{
    return raster_lock(this, level, lockMode);
}
static inline byte* (__cdecl* const raster_lockpa)(RWE::RwRaster* raster, RWE::RwRasterLockMode lockMode) = reinterpret_cast<byte * (__cdecl*)(RWE::RwRaster*, RWE::RwRasterLockMode)>(0x418B00);
byte* RWE::RwRaster::LockPalette(RwRasterLockMode lockMode)
{
    return raster_lockpa(this, lockMode);
}
static inline RWE::RwRaster* (__cdecl* const raster_unlock)(RWE::RwRaster* raster) = reinterpret_cast<RWE::RwRaster* (__cdecl*)(RWE::RwRaster*)>(0x418A50);
void RWE::RwRaster::Unlock()
{
    raster_unlock(this);
}
static inline RWE::RwRaster* (__cdecl* const raster_unlockpal)(RWE::RwRaster* raster) = reinterpret_cast<RWE::RwRaster * (__cdecl*)(RWE::RwRaster*)>(0x418A70);
void RWE::RwRaster::UnlockPalette()
{
    raster_unlockpal(this);
}
static inline int (__cdecl* const raster_getmiplev)(RWE::RwRaster* raster) = reinterpret_cast<int (__cdecl*)(RWE::RwRaster*)>(0x418B30);
int RWE::RwRaster::GetMipLevels()
{
    return raster_getmiplev(this);
}

void RWE::RpGeometry::AddRef()
{
    RWE::RpGeometry*(__cdecl* const f)(RWE::RpGeometry * raster) = reinterpret_cast<RWE::RpGeometry*(__cdecl*)(RWE::RpGeometry*)>(0x62ED80);
    f(this);
}

void RWE::RpGeometry::Destroy()
{
    bool (__cdecl* const f)(RWE::RpGeometry * raster) = reinterpret_cast<bool (__cdecl*)(RWE::RpGeometry*)>(0x62EE60);
    f(this);
}
void RWE::RpGeometry::ForAllMaterials(RpMaterialCallBack fpCallBack, void* pData)
{
    RpGeometry* (__cdecl* const f)(RpGeometry * th, RpMaterialCallBack fpCallBack, void* pData) = reinterpret_cast<RpGeometry * (__cdecl*)(RpGeometry*, RpMaterialCallBack, void*)>(0x62E890);
    f(this, fpCallBack, pData);
}

static inline int(__cdecl* const atomic_isparticleemitter)(RWE::RpAtomic* a) = reinterpret_cast<int(__cdecl*)(RWE::RpAtomic*)>(0x5D9210);
bool RWE::RpAtomic::IsParticleEmitter()
{
    return atomic_isparticleemitter(this);;
}
static inline void(__cdecl* const atomic_destroy)(RWE::RpAtomic* a) = reinterpret_cast<void(__cdecl*)(RWE::RpAtomic*)>(0x628F90);
void RWE::RpAtomic::Destroy()
{
    atomic_destroy(this);
}
static inline RWE::RpAtomic* (__cdecl* const atomic_forallemitters)(RWE::RpAtomic* a, RWE::Particles::RpPrtStdEmitterCallBack cb, void* data) = reinterpret_cast<RWE::RpAtomic * (__cdecl*)(RWE::RpAtomic*, RWE::Particles::RpPrtStdEmitterCallBack, void*)>(0x5D9230);
void RWE::RpAtomic::ForAllEmitters(RWE::Particles::RpPrtStdEmitterCallBack cb, void* data)
{
    atomic_forallemitters(this, cb, data);
}
static inline RWE::RpAtomic* (__cdecl* const atomic_addemitter)(RWE::RpAtomic* th, RWE::Particles::RpPrtStdEmitter* ad) = reinterpret_cast<RWE::RpAtomic * (__cdecl*)(RWE::RpAtomic*, RWE::Particles::RpPrtStdEmitter*)>(0x5D9280);
void RWE::RpAtomic::AddEmitter(RWE::Particles::RpPrtStdEmitter* em)
{
    atomic_addemitter(this, em);
}
RWE::RpWorld* RWE::RpAtomic::GetWorld() const
{
    RWE::RpWorld* (__cdecl* const f)(const RWE::RpAtomic * th) = reinterpret_cast<RWE::RpWorld * (__cdecl*)(const RWE::RpAtomic*)>(0x626DA0);
    return f(this);
}
void RWE::RpAtomic::SetGeometry(RpGeometry* geometry, bool assumeSameBoundingSphere)
{
    RWE::RpAtomic* (__cdecl* const f)(RWE::RpAtomic* th, RpGeometry * g, uint32_t flags) = reinterpret_cast<RWE::RpAtomic * (__cdecl*)(RWE::RpAtomic*, RpGeometry * g, uint32_t flags)>(0x628F10);
    f(this, geometry, assumeSameBoundingSphere ? 1 : 0);
}

static inline RWE::RpClump* (__cdecl* const clump_forallatomics)(RWE::RpClump* m, RWE::RpAtomicCallBack, void* data) = reinterpret_cast<RWE::RpClump* (__cdecl*)(RWE::RpClump*, RWE::RpAtomicCallBack, void*)>(0x628E30);
RWE::RpClump* RWE::RpClump::ForAllAtomics(RpAtomicCallBack callback, void* pData)
{
    return clump_forallatomics(this, callback, pData);
}
RWE::RpClump* RWE::RpClump::ForAllLights(RpLightCallBack callback, void* pData)
{
    RWE::RpClump* (__cdecl* const clump_foralllights)(RWE::RpClump* m, RWE::RpLightCallBack, void* data) = reinterpret_cast<RWE::RpClump * (__cdecl*)(RWE::RpClump*, RWE::RpLightCallBack, void*)>(0x628EC0);
    return clump_foralllights(this, callback, pData);
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
static inline RWE::RpClump* (__cdecl* const clump_addAtomic)(RWE::RpClump* t, RWE::RpAtomic* a) = reinterpret_cast<RWE::RpClump * (__cdecl*)(RWE::RpClump*, RWE::RpAtomic*)>(0x629090);
void RWE::RpClump::AddAtomic(RWE::RpAtomic* a)
{
    clump_addAtomic(this, a);
}
static inline RWE::RpClump* (__cdecl* const clump_remAtomic)(RWE::RpClump* t, RWE::RpAtomic* a) = reinterpret_cast<RWE::RpClump * (__cdecl*)(RWE::RpClump*, RWE::RpAtomic*)>(0x6290C0);
void RWE::RpClump::RemoveAtomic(RpAtomic* atomic)
{
    clump_remAtomic(this, atomic);
}
void RWE::RpClump::AddLight(RWE::RpLight* a)
{
    RWE::RpClump* (__cdecl* const clump_addLight)(RWE::RpClump * t, RWE::RpLight * a) = reinterpret_cast<RWE::RpClump * (__cdecl*)(RWE::RpClump*, RWE::RpLight*)>(0x6290F0);
    clump_addLight(this, a);
}

inline void(__cdecl* const camera_setframe)(RWE::RwCamera* cam, RWE::RwFrame* f) = reinterpret_cast<void(__cdecl*)(RWE::RwCamera*, RWE::RwFrame*)>(0x42C290);
void RWE::RwCamera::SetFrame(RwFrame* f)
{
    camera_setframe(this, f);
}
inline void(__cdecl* const camera_destroy)(RWE::RwCamera* c) = reinterpret_cast<void(__cdecl*)(RWE::RwCamera*)>(0x41A070);
void RWE::RwCamera::Destroy()
{
    camera_destroy(this);
}
inline void(__cdecl* const camera_destroyext)(RWE::RwCamera* c) = reinterpret_cast<void(__cdecl*)(RWE::RwCamera*)>(0x467C09);
void RWE::RwCamera::ExtendedDestroy()
{
    camera_destroyext(this);
}
inline void(__cdecl* const camera_setnearclip)(RWE::RwCamera* c, float n) = reinterpret_cast<void(__cdecl*)(RWE::RwCamera*, float)>(0x419E60);
void RWE::RwCamera::SetNearClipPlane(float ne)
{
    camera_setnearclip(this, ne);
}
inline void(__cdecl* const camera_setfarclip)(RWE::RwCamera* c, float n) = reinterpret_cast<void(__cdecl*)(RWE::RwCamera*, float)>(0x419E90);
void RWE::RwCamera::SetFarClipPlane(float fa)
{
    camera_setfarclip(this, fa);
}
inline RWE::RwCamera*(__cdecl* const camera_setpro)(RWE::RwCamera* c, RWE::RwCameraProjection p) = reinterpret_cast<RWE::RwCamera*(__cdecl*)(RWE::RwCamera*, RWE::RwCameraProjection)>(0x419F90);
bool RWE::RwCamera::SetProjection(RwCameraProjection pro)
{
    return camera_setpro(this, pro) != nullptr;
}
inline void (__cdecl* const camera_setvwind)(RWE::RwCamera* c, RWE::RwV2d* v) = reinterpret_cast<void (__cdecl*)(RWE::RwCamera*, RWE::RwV2d*)>(0x41A000);
void RWE::RwCamera::SetViewWindow(RwV2d* viewWind)
{
    camera_setvwind(this, viewWind);
}
RWE::RwFrame* RWE::RwCamera::GetFrame() const
{
    return static_cast<RWE::RwFrame*>(object.object.parent);
}
inline RWE::RwCamera* (__cdecl* const camera_clear)(RWE::RwCamera* th, RWE::RwRGBA* color, RWE::RwCameraClearMode clearMode) = reinterpret_cast<RWE::RwCamera * (__cdecl*)(RWE::RwCamera*, RWE::RwRGBA*, RWE::RwCameraClearMode)>(0x419F30);
bool RWE::RwCamera::Clear(RwRGBA color, RwCameraClearMode clearMode)
{
    return camera_clear(this, &color, clearMode) != nullptr;
}
inline RWE::RwCamera* (__cdecl* const camera_beginupdate)(RWE::RwCamera* th) = reinterpret_cast<RWE::RwCamera * (__cdecl*)(RWE::RwCamera*)>(0x419E20);
bool RWE::RwCamera::BeginUpdate()
{
    return camera_beginupdate(this) != nullptr;
}
inline RWE::RwCamera* (__cdecl* const camera_endupdate)(RWE::RwCamera* th) = reinterpret_cast<RWE::RwCamera * (__cdecl*)(RWE::RwCamera*)>(0x419E10);
bool RWE::RwCamera::EndUpdate()
{
    return camera_endupdate(this) != nullptr;
}
inline RWE::RwCamera* (__cdecl* const camera_show)(RWE::RwCamera* th, HWND w, int vs) = reinterpret_cast<RWE::RwCamera * (__cdecl*)(RWE::RwCamera*, HWND, int)>(0x419F60);
bool RWE::RwCamera::ShowRaster(HWND window, bool vsync)
{
    return camera_show(this, window, vsync ? 1 : 0);
}

inline void(__cdecl* const light_destroy)(RWE::RpLight* l) = reinterpret_cast<void(__cdecl*)(RWE::RpLight*)>(0x6277B0);
void RWE::RpLight::Destroy()
{
    light_destroy(this);
}
inline void(__cdecl* const light_setcolor)(RWE::RpLight* l, const RWE::RwRGBAReal* color) = reinterpret_cast<void(__cdecl*)(RWE::RpLight*, const RWE::RwRGBAReal*)>(0x6274C0);
void RWE::RpLight::SetColor(const RwRGBAReal& color)
{
    light_setcolor(this, &color);
}
inline float(__cdecl* const light_getconeangle)(const RWE::RpLight* l) = reinterpret_cast<float(__cdecl*)(const RWE::RpLight*)>(0x627520);
float RWE::RpLight::GetConeAngle() const
{
    return light_getconeangle(this);
}
void RWE::RpLight::SetFrame(RwFrame* frame)
{
    RpLight* (__cdecl* const RpLightSetFrame)(RpLight * light, RwFrame * frame) = reinterpret_cast<RpLight* (__cdecl*)(RpLight*, RwFrame*)>(0x42C290);
    RpLightSetFrame(this, frame);
}
RWE::RwFrame* RWE::RpLight::GetFrame() const
{
    return static_cast<RWE::RwFrame*>(object.object.parent);
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
inline void(__cdecl* const world_addcam)(RWE::RpWorld* w, RWE::RwCamera* c) = reinterpret_cast<void(__cdecl*)(RWE::RpWorld*, RWE::RwCamera*)>(0x626CC0);
void RWE::RpWorld::AddCamera(RwCamera* cam)
{
    world_addcam(this, cam);
}
inline void(__cdecl* const world_remcam)(RWE::RpWorld* w, RWE::RwCamera* c) = reinterpret_cast<void(__cdecl*)(RWE::RpWorld*, RWE::RwCamera*)>(0x626CF0);
void RWE::RpWorld::RemoveCamera(RwCamera* cam)
{
    world_remcam(this, cam);
}
inline void(__cdecl* const world_addlight)(RWE::RpWorld* w, RWE::RpLight* c) = reinterpret_cast<void(__cdecl*)(RWE::RpWorld*, RWE::RpLight*)>(0x626E10);
void RWE::RpWorld::AddLight(RpLight* light)
{
    world_addlight(this, light);
}
inline void(__cdecl* const world_remlight)(RWE::RpWorld* w, RWE::RpLight* c) = reinterpret_cast<void(__cdecl*)(RWE::RpWorld*, RWE::RpLight*)>(0x626E60);
void RWE::RpWorld::RemoveLight(RpLight* light)
{
    world_remlight(this, light);
}

inline RWE::RwImage* (__cdecl* const image_allocpix)(RWE::RwImage* th) = reinterpret_cast<RWE::RwImage* (__cdecl*)(RWE::RwImage*)>(0x414D20);
RWE::RwImage* RWE::RwImage::AllocatePixels()
{
    return image_allocpix(this);
}
inline RWE::RwImage* (__cdecl* const image_freepix)(RWE::RwImage* th) = reinterpret_cast<RWE::RwImage* (__cdecl*)(RWE::RwImage*)>(0x414DE0);
RWE::RwImage* RWE::RwImage::FreePixels()
{
    return image_freepix(this);
}
inline int(__cdecl* const image_destroy)(RWE::RwImage* th) = reinterpret_cast<int(__cdecl*)(RWE::RwImage*)>(0x415B50);
int RWE::RwImage::Destroy()
{
    return image_destroy(this);
}

int RWE::RwDevice::SetTextureFilterMode(RwTextureFilterMode m)
{
    return fpRenderStateSet(RwRenderState::rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void*>(static_cast<int>(m)));
}
int RWE::RwDevice::SetCullMode(RwCullMode m)
{
    return fpRenderStateSet(RwRenderState::rwRENDERSTATECULLMODE, reinterpret_cast<void*>(static_cast<int>(m)));
}
RWE::RwCullMode RWE::RwDevice::GetCullMode()
{
    RwCullMode m;
    fpRenderStateGet(RwRenderState::rwRENDERSTATECULLMODE, &m);
    return m;
}
int RWE::RwDevice::SetTextureRaster(RWE::RwRaster* r)
{
    return fpRenderStateSet(RwRenderState::rwRENDERSTATETEXTURERASTER, r);
}

inline RWE::RwVideoMode* (__cdecl* const videomode_getinfo)(RWE::RwVideoMode* m, int mode) = reinterpret_cast<RWE::RwVideoMode * (__cdecl*)(RWE::RwVideoMode*, int)>(0x40FEC0);
bool RWE::RwVideoMode::GetInfo(int mode)
{
    return videomode_getinfo(this, mode) != nullptr;
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

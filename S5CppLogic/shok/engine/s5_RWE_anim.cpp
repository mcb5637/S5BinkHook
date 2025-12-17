#include "pch.h"
#include "s5_RWE_anim.h"
#include <shok/globals/s5_mapdisplay.h>


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

static inline void(__cdecl* const animinter_destroy)(RWE::Anim::RtAnimInterpolator* i) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RtAnimInterpolator*)>(0x70DE10);
void RWE::Anim::RtAnimInterpolator::Destroy()
{
    animinter_destroy(this);
}
static inline void(__cdecl* const animinter_addtime)(RWE::Anim::RtAnimInterpolator* th, float t) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RtAnimInterpolator*, float t)>(0x6EAF10);
void RWE::Anim::RtAnimInterpolator::AddTime(float t)
{
    animinter_addtime(this, t);
}
static inline void(__cdecl* const animinter_subtime)(RWE::Anim::RtAnimInterpolator* th, float t) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RtAnimInterpolator*, float t)>(0x6EB280);
void RWE::Anim::RtAnimInterpolator::SubTime(float t)
{
    animinter_subtime(this, t);
}
static inline void(__cdecl* const animinter_settime)(RWE::Anim::RtAnimInterpolator* th, float t) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RtAnimInterpolator*, float t)>(0x6EB400);
void RWE::Anim::RtAnimInterpolator::SetCurrentTime(float t)
{
    animinter_settime(this, t);
}
static inline void(__cdecl* const animinter_blend)(RWE::Anim::RtAnimInterpolator* th, RWE::Anim::RtAnimInterpolator* a1, RWE::Anim::RtAnimInterpolator* a2, float r) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RtAnimInterpolator*, RWE::Anim::RtAnimInterpolator*, RWE::Anim::RtAnimInterpolator*, float)>(0x6EB0E0);
void RWE::Anim::RtAnimInterpolator::Blend(RtAnimInterpolator* in1, RtAnimInterpolator* in2, float alpha)
{
    animinter_blend(this, in1, in2, alpha);
}

static inline void(__cdecl* const animinter_setanim)(RWE::Anim::RtAnimInterpolator* th, RWE::Anim::RtAnimAnimation* a) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RtAnimInterpolator*, RWE::Anim::RtAnimAnimation*)>(0x6EAE10);
void RWE::Anim::RtAnimInterpolator::SetAnimation(RWE::RtAnimAnimation* a)
{
    animinter_setanim(this, a);
}

static inline RWE::Anim::RpHAnimHierarchy* (__cdecl* const animhier_clone)(RWE::Anim::RpHAnimHierarchy* th, RWE::Anim::RpHAnimHierarchy::RpHAnimHierarchyFlag d, void* da) = reinterpret_cast<RWE::Anim::RpHAnimHierarchy * (__cdecl*)(RWE::Anim::RpHAnimHierarchy*, RWE::Anim::RpHAnimHierarchy::RpHAnimHierarchyFlag, void*)>(0x6EBB30);
RWE::Anim::RpHAnimHierarchy* RWE::Anim::RpHAnimHierarchy::Clone()
{
    return animhier_clone(this, flags, currentAnim->pNextFrame);
}
static inline void(__cdecl* const animhier_destroy)(RWE::Anim::RpHAnimHierarchy* th) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RpHAnimHierarchy*)>(0x6EB990);
void RWE::Anim::RpHAnimHierarchy::Destroy()
{
    animhier_destroy(this);
}
static inline void(__cdecl* const animhier_updatemat)(RWE::Anim::RpHAnimHierarchy* th) = reinterpret_cast<void(__cdecl*)(RWE::Anim::RpHAnimHierarchy*)>(0x6EC080);
void RWE::Anim::RpHAnimHierarchy::UpdateMatrices()
{
    animhier_updatemat(this);
}
static inline int(__cdecl* const animhier_idgetindex)(const RWE::Anim::RpHAnimHierarchy* th, int id) = reinterpret_cast<int(__cdecl*)(const RWE::Anim::RpHAnimHierarchy*, int)>(0x6EBC00);
int RWE::Anim::RpHAnimHierarchy::IDGetIndex(int id) const
{
    return animhier_idgetindex(this, id);
}
RWE::RwFrame* RWE::Anim::RpHAnimHierarchy::GetFromIndex(int idx) const
{
    return pNodeInfo[idx].pFrame;
}




void RWE::Anim::RpHAnimHierarchy::SetupForModel(RWE::RpClump* c)
{
    struct data { // NOLINT(*-pro-type-member-init)
        RWE::Anim::RpHAnimHierarchy* th;
        bool found = false;
    };
    data d{ this };
    c->ForAllAtomics(reinterpret_cast<RWE::RpAtomicCallBack>(0x487D1A), &d);
    struct unknownclumpaddon { // NOLINT(*-pro-type-member-init)
        PADDINGI(3);
        ED::ModelData* ModelData;
    };
    auto* a = reinterpret_cast<unknownclumpaddon*>(reinterpret_cast<int>(c) + *reinterpret_cast<int*>(0x858228));
    ED::ModelData::ModelFlags f = ED::ModelData::ModelFlags::None;
    if (a->ModelData) {
        f = a->ModelData->Flags;
    }
    if (d.found && (f & ED::ModelData::ModelFlags::HierarchyDestroyed) != ED::ModelData::ModelFlags::None) {
        //reinterpret_cast<byte*>(this)[1] &= 0xCFu; // ~0x30
        flags = flags & ~(RpHAnimHierarchyFlag::UpdateModellingMatrices | RpHAnimHierarchyFlag::UpdateLTMs);
    }
    else {
        //reinterpret_cast<byte*>(this)[1] |= 0x10;
        flags = flags | RpHAnimHierarchyFlag::UpdateModellingMatrices;
        reinterpret_cast<void(__cdecl*)(RWE::Anim::RpHAnimHierarchy*)>(0x6EC7E0)(this);
    }
}

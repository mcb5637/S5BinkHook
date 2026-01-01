#include "s5_RWE_particles.h"

static inline void(__cdecl* const proptab_destroy)(RWE::Particles::RpPrtStdPropertyTable* th) = reinterpret_cast<void(__cdecl*)(RWE::Particles::RpPrtStdPropertyTable*)>(0x5D96B0);
void RWE::Particles::RpPrtStdPropertyTable::Destroy()
{
    proptab_destroy(this);
}

static inline int(__cdecl* const proptab_getpoff)(RWE::Particles::RpPrtStdPropertyTable* th, int id) = reinterpret_cast<int(__cdecl*)(RWE::Particles::RpPrtStdPropertyTable*, int)>(0x5D97F0);
int RWE::Particles::RpPrtStdPropertyTable::GetPropOffset(EmitterProperties id)
{
    return proptab_getpoff(this, static_cast<int>(id));
}
int RWE::Particles::RpPrtStdPropertyTable::GetPropOffset(ParticleProperties id)
{
    return GetPropOffset(static_cast<EmitterProperties>(static_cast<int>(id)));
}

static inline void(__cdecl* const eclass_destroy)(RWE::Particles::RpPrtStdEmitterClass* th) = reinterpret_cast<void(__cdecl*)(RWE::Particles::RpPrtStdEmitterClass*)>(0x5DA340);
void RWE::Particles::RpPrtStdEmitterClass::Destroy()
{
    eclass_destroy(this);
}

static inline RWE::Particles::RpPrtStdEmitterClass* (__cdecl* const eclass_setprops)(RWE::Particles::RpPrtStdEmitterClass* th, RWE::Particles::RpPrtStdPropertyTable* p) = reinterpret_cast<RWE::Particles::RpPrtStdEmitterClass * (__cdecl*)(RWE::Particles::RpPrtStdEmitterClass*, RWE::Particles::RpPrtStdPropertyTable*)>(0x5DA470);
void RWE::Particles::RpPrtStdEmitterClass::SetProperties(RpPrtStdPropertyTable* p)
{
    eclass_setprops(this, p);
}

static inline RWE::Particles::RpPrtStdEmitterClass* (__cdecl* const eclass_setcb)(RWE::Particles::RpPrtStdEmitterClass* th, int n, RWE::Particles::RpPrtStdEmitterClass::RpPrtStdEmitterCallBackArray* c) = reinterpret_cast<RWE::Particles::RpPrtStdEmitterClass * (__cdecl*)(RWE::Particles::RpPrtStdEmitterClass*, int, RWE::Particles::RpPrtStdEmitterClass::RpPrtStdEmitterCallBackArray*)>(0x5DA3F0);
void RWE::Particles::RpPrtStdEmitterClass::SetCallback(int numCb, RpPrtStdEmitterCallBackArray* cbs)
{
    eclass_setcb(this, numCb, cbs);
}

RWE::Particles::RpPrtStdEmitterCallBack RWE::Particles::RpPrtStdEmitterClass::GetCallbackNum(RpPrtStdEmitterCallBackCode c, int cn)
{
    int co = static_cast<int>(c);
    return *(callback[cn * NumEmitterCallbacksInArray + co]);
}

static inline RWE::Particles::RpPrtStdParticleClass* (__cdecl* const particleclass_setprops)(RWE::Particles::RpPrtStdParticleClass* th, RWE::Particles::RpPrtStdPropertyTable* p) = reinterpret_cast<RWE::Particles::RpPrtStdParticleClass * (__cdecl*)(RWE::Particles::RpPrtStdParticleClass*, RWE::Particles::RpPrtStdPropertyTable*)>(0x5DA470);
void RWE::Particles::RpPrtStdParticleClass::SetProperties(RpPrtStdPropertyTable* p)
{
    particleclass_setprops(this, p);
}

static inline void(__cdecl* const pclass_destroy)(RWE::Particles::RpPrtStdParticleClass* th) = reinterpret_cast<void(__cdecl*)(RWE::Particles::RpPrtStdParticleClass*)>(0x5DA6B0);
void RWE::Particles::RpPrtStdParticleClass::Destroy()
{
    pclass_destroy(this);
}

static inline RWE::Particles::RpPrtStdEmitter* (__cdecl* const emitter_addemitter)(RWE::Particles::RpPrtStdEmitter* th, RWE::Particles::RpPrtStdEmitter* ad) = reinterpret_cast<RWE::Particles::RpPrtStdEmitter * (__cdecl*)(RWE::Particles::RpPrtStdEmitter*, RWE::Particles::RpPrtStdEmitter*)>(0x5DAB10);
void RWE::Particles::RpPrtStdEmitter::AddEmitter(RpPrtStdEmitter* em)
{
    emitter_addemitter(this, em);
}

RWE::Particles::RpPrtStdEmitterStandard* RWE::Particles::RpPrtStdEmitter::GetStandard()
{
    return static_cast<RpPrtStdEmitterStandard*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::STANDARD));
}

RWE::Particles::RpPrtStdEmitterPrtColor* RWE::Particles::RpPrtStdEmitter::GetColor()
{
    return static_cast<RpPrtStdEmitterPrtColor*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::PRTCOLOR));
}

RWE::Particles::RpPrtStdEmitterPrtTexCoords* RWE::Particles::RpPrtStdEmitter::GetTexCoords()
{
    return static_cast<RpPrtStdEmitterPrtTexCoords*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::PRTTEXCOORDS));
}

RWE::Particles::RpPrtStdEmitterPrt2DRotate* RWE::Particles::RpPrtStdEmitter::GetRotate()
{
    return static_cast<RpPrtStdEmitterPrt2DRotate*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::PRT2DROTATE));
}

RWE::Particles::RpPrtStdEmitterPrtSize* RWE::Particles::RpPrtStdEmitter::GetSize()
{
    return static_cast<RpPrtStdEmitterPrtSize*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::PRTSIZE));
}

RWE::Particles::RpPrtStdEmitterPTank* RWE::Particles::RpPrtStdEmitter::GetTank()
{
    return static_cast<RpPrtStdEmitterPTank*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::PTANK));
}

RWE::Particles::RpPrtStdEmitterPrtMatrix* RWE::Particles::RpPrtStdEmitter::GetMatrix()
{
    return static_cast<RpPrtStdEmitterPrtMatrix*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::PRTMATRIX));
}

RWE::Particles::RpPrtAdvEmtPrtEmt* RWE::Particles::RpPrtStdEmitter::GetAdvParticleEmitter()
{
    return static_cast<RpPrtAdvEmtPrtEmt*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::ADVPROPERTYCODEEMITTERPRTEMITTER));
}

RWE::Particles::RpPrtAdvEmtCircle* RWE::Particles::RpPrtStdEmitter::GetAdvCircleEmitter()
{
    return static_cast<RpPrtAdvEmtCircle*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::ADVPROPERTYCODEEMITTERCIRCLE));
}

RWE::Particles::RpPrtAdvEmtPointList* RWE::Particles::RpPrtStdEmitter::GetAdvPointListEmitter()
{
    return static_cast<RpPrtAdvEmtPointList*>(GetDataById(RpPrtStdPropertyTable::EmitterProperties::ADVPROPERTYCODEEMITTERPOINTLIST));
}

void* RWE::Particles::RpPrtStdEmitter::GetDataById(RpPrtStdPropertyTable::EmitterProperties p)
{
    int off = emtClass->propTab->GetPropOffset(p);
    if (off == 0)
        return nullptr;
    return reinterpret_cast<void*>(reinterpret_cast<int>(this) + off);
}

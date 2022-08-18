#include "pch.h"
#include "s5_RWE_particles.h"

static inline void(__cdecl* const proptab_destroy)(RWE::Particles::RpPrtStdPropertyTable* th) = reinterpret_cast<void(__cdecl*)(RWE::Particles::RpPrtStdPropertyTable*)>(0x5D96B0);
void RWE::Particles::RpPrtStdPropertyTable::Destroy()
{
    proptab_destroy(this);
}

static inline int(__cdecl* const proptab_getpoff)(RWE::Particles::RpPrtStdPropertyTable* th, int id) = reinterpret_cast<int(__cdecl*)(RWE::Particles::RpPrtStdPropertyTable*, int)>(0x5D97F0);
int RWE::Particles::RpPrtStdPropertyTable::GetPropOffset(Properties id)
{
    return proptab_getpoff(this, static_cast<int>(id));
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
    return static_cast<RpPrtStdEmitterStandard*>(GetDataById(RpPrtStdPropertyTable::Properties::STANDARD));
}

RWE::Particles::RpPrtStdEmitterPrtColor* RWE::Particles::RpPrtStdEmitter::GetColor()
{
    return static_cast<RpPrtStdEmitterPrtColor*>(GetDataById(RpPrtStdPropertyTable::Properties::PRTCOLOR));
}

RWE::Particles::RpPrtStdEmitterPrtTexCoords* RWE::Particles::RpPrtStdEmitter::GetTexCoords()
{
    return static_cast<RpPrtStdEmitterPrtTexCoords*>(GetDataById(RpPrtStdPropertyTable::Properties::PRTTEXCOORDS));
}

RWE::Particles::RpPrtStdEmitterPrt2DRotate* RWE::Particles::RpPrtStdEmitter::GetRotate()
{
    return static_cast<RpPrtStdEmitterPrt2DRotate*>(GetDataById(RpPrtStdPropertyTable::Properties::PRT2DROTATE));
}

RWE::Particles::RpPrtStdEmitterPrtSize* RWE::Particles::RpPrtStdEmitter::GetSize()
{
    return static_cast<RpPrtStdEmitterPrtSize*>(GetDataById(RpPrtStdPropertyTable::Properties::PRTSIZE));
}

RWE::Particles::RpPrtStdEmitterPTank* RWE::Particles::RpPrtStdEmitter::GetTank()
{
    return static_cast<RpPrtStdEmitterPTank*>(GetDataById(RpPrtStdPropertyTable::Properties::PTANK));
}

RWE::Particles::RpPrtStdEmitterPrtMatrix* RWE::Particles::RpPrtStdEmitter::GetMatrix()
{
    return static_cast<RpPrtStdEmitterPrtMatrix*>(GetDataById(RpPrtStdPropertyTable::Properties::PRTMATRIX));
}

RWE::Particles::Ex_CircularEmitter* RWE::Particles::RpPrtStdEmitter::GetCircularEmitter()
{
    return static_cast<Ex_CircularEmitter*>(GetDataById(RpPrtStdPropertyTable::Properties::Ex_CircularEmitter));
}

RWE::Particles::Ex_FogEmitter* RWE::Particles::RpPrtStdEmitter::GetFogEmitter()
{
    return static_cast<Ex_FogEmitter*>(GetDataById(RpPrtStdPropertyTable::Properties::Ex_FogEmitter));
}

void* RWE::Particles::RpPrtStdEmitter::GetDataById(RpPrtStdPropertyTable::Properties p)
{
    int off = emtClass->propTab->GetPropOffset(p);
    if (off == 0)
        return nullptr;
    return reinterpret_cast<void*>(reinterpret_cast<int>(this) + off);
}

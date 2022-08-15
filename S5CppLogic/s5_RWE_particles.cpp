#include "pch.h"
#include "s5_RWE_particles.h"

static inline void(__cdecl* const proptab_destroy)(RWE::Particles::RpPrtStdPropertyTable* th) = reinterpret_cast<void(__cdecl*)(RWE::Particles::RpPrtStdPropertyTable*)>(0x5D96B0);
void RWE::Particles::RpPrtStdPropertyTable::Destroy()
{
    proptab_destroy(this);
}

static inline RWE::Particles::RpPrtStdParticleClass* (__cdecl* const particleclass_setprops)(RWE::Particles::RpPrtStdParticleClass* th, RWE::Particles::RpPrtStdPropertyTable* p) = reinterpret_cast<RWE::Particles::RpPrtStdParticleClass * (__cdecl*)(RWE::Particles::RpPrtStdParticleClass*, RWE::Particles::RpPrtStdPropertyTable*)>(0x5DA470);
void RWE::Particles::RpPrtStdParticleClass::SetProperties(RpPrtStdPropertyTable* p)
{
    particleclass_setprops(this, p);
}

static inline RWE::Particles::RpPrtStdEmitter* (__cdecl* const emitter_addemitter)(RWE::Particles::RpPrtStdEmitter* th, RWE::Particles::RpPrtStdEmitter* ad) = reinterpret_cast<RWE::Particles::RpPrtStdEmitter * (__cdecl*)(RWE::Particles::RpPrtStdEmitter*, RWE::Particles::RpPrtStdEmitter*)>(0x5DAB10);
void RWE::Particles::RpPrtStdEmitter::AddEmitter(RpPrtStdEmitter* em)
{
    emitter_addemitter(this, em);
}


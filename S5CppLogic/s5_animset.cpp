#include "pch.h"
#include "s5_animset.h"

EGL::CGLEAnimSet::CGLEAnimSet()
{
	SetVT();
}
EGL::CGLEAnimSet::CGLEAnimSet(const CGLEAnimSet& o)
{
	SetVT();
}
EGL::CGLEAnimSet::CGLEAnimSet(CGLEAnimSet&& o) noexcept
{
	SetVT();
}


void* EGL::CGLEAnimSet::operator new(size_t s)
{
	return shok::Malloc(s);
}
void EGL::CGLEAnimSet::operator delete(void* p) {
	shok::Free(p);
}

inline int(__thiscall* const animset_getanim)(EGL::CGLEAnimSet* th, int c) = reinterpret_cast<int(__thiscall*)(EGL::CGLEAnimSet*, int)>(0x588464);
int EGL::CGLEAnimSet::GetAnim(int category)
{
	return animset_getanim(this, category);
}

unsigned int __stdcall EGL::CGLEAnimSet::GetClassIdentifier() const
{
	return Identifier;
}

void EGL::CGLEAnimSet::SetVT()
{
	*reinterpret_cast<int*>(this) = vtp;
}

unsigned int __stdcall GGL::CGLGoodDependentAnim::GetClassIdentifier() const
{
	return Identifier;
}

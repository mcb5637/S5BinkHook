#include "s5_animset.h"

EGL::CGLEAnimSet::CGLEAnimSet()
{
	SetVT();
}

EGL::CGLEAnimSet::~CGLEAnimSet()
{
	reinterpret_cast<void(__thiscall*)(CGLEAnimSet*, bool)>(0x5885A4)(this, false);
}

void* EGL::CGLEAnimSet::operator new(size_t s)
{
	return shok::Malloc(s);
}
void EGL::CGLEAnimSet::operator delete(void* p) {
	shok::Free(p);
}

inline shok::AnimationId(__thiscall* const animset_getanim)(EGL::CGLEAnimSet* th, shok::AnimationCategoryId c) = reinterpret_cast<shok::AnimationId(__thiscall*)(EGL::CGLEAnimSet*, shok::AnimationCategoryId)>(0x588464);
shok::AnimationId EGL::CGLEAnimSet::GetAnim(shok::AnimationCategoryId category)
{
	return animset_getanim(this, category);
}

shok::ClassId __stdcall EGL::CGLEAnimSet::GetClassIdentifier() const
{
	return Identifier;
}

void EGL::CGLEAnimSet::SetVT()
{
	*reinterpret_cast<int*>(this) = vtp;
}

shok::ClassId __stdcall GGL::CGLGoodDependentAnim::GetClassIdentifier() const
{
	return Identifier;
}

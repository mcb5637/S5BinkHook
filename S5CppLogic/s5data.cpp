#include "pch.h"

#include "s5data.h"

shok_EGL_CGLEEntity* (_stdcall* shok_eid2obj)(int id) = (shok_EGL_CGLEEntity * (_stdcall*)(int)) 0x5825B4;
void(_stdcall* shok_SetHighPrecFPU)() = (void(_stdcall *)()) 0x5C8451;

template<class T>
inline T* shok_vector<T>::Get(int i)
{
	if (i < 0 || i >= Size())
		return NULL;
	return start + i;
}

template<class T>
int shok_vector<T>::Size()
{
	return (this->end - this->start) / sizeof(T);
}

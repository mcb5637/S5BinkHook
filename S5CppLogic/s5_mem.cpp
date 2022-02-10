#include "pch.h"
#include "s5data.h"

shok::SaveVirtualProtect::SaveVirtualProtect() : shok::SaveVirtualProtect(reinterpret_cast<void*>(SHOK_SEGMENTSTART), SHOK_SEGMENTLENGTH)
{
}
shok::SaveVirtualProtect::SaveVirtualProtect(void* adr, size_t size)
{
	Adr = adr;
	Size = size;
	Prev = PAGE_EXECUTE_READWRITE;
	VirtualProtect(adr, size, PAGE_EXECUTE_READWRITE, &Prev);
}
shok::SaveVirtualProtect::~SaveVirtualProtect()
{
	VirtualProtect(Adr, Size, Prev, &Prev);
}
#include "pch.h"
#include "s5_datastructures.h"

static inline void(__thiscall* const str_ctor)(shok::String* th, const char* s) = reinterpret_cast<void(__thiscall*)(shok::String*, const char*)>(0x4018C6);
shok::String::String(const char* s)
{
	str_ctor(this, s);
}
static inline void(__thiscall* const str_ctorcopy)(shok::String* th, const shok::String* ot) = reinterpret_cast<void(__thiscall*)(shok::String*, const shok::String*)>(0x401808);
shok::String::String(const shok::String& c)
{
	str_ctorcopy(this, &c);
}
const char* shok::String::c_str() const
{
	if (allocated < 16)
		return reinterpret_cast<const char*>(&data);
	else
		return reinterpret_cast<const char*>(data);
}
size_t shok::String::size() const
{
	return size_v;
}
shok::String::~String()
{
	if (allocated >= 16)
		shok::Free(reinterpret_cast<void*>(data));
}
static inline void(__thiscall* const str_assign)(shok::String* th, const char* c) = reinterpret_cast<void(__thiscall*)(shok::String*, const char*)>(0x40182E);
void shok::String::assign(const char* s)
{
	str_assign(this, s);
}
shok::String::String() : shok::String("")
{
}

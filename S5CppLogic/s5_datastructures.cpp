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

std::strong_ordering shok::String::operator<=>(const String& r) const
{
	return std::string_view{ this->c_str(), this->size() } <=> std::string_view{ r.c_str(), r.size() };
}
bool shok::String::operator==(const String& r) const
{
	return (*this <=> r) == std::strong_ordering::equal;
}

std::strong_ordering shok::operator<=>(const String& a, const char* b)
{
	return std::string_view{ a.c_str(), a.size() } <=> std::string_view{ b };
}
bool shok::operator==(const String& a, const char* b)
{
	return (a <=> b) == std::strong_ordering::equal;
}
std::strong_ordering shok::operator<=>(const char* a, const String& b)
{
	return std::string_view{ a } <=> std::string_view{ b.c_str(), b.size() };
}
bool shok::operator==(const char* a, const String& b)
{
	return (a <=> b) == std::strong_ordering::equal;
}

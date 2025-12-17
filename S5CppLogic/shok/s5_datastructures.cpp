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
shok::String::String(const std::string& s)
{
	*this = s;
}
shok::String::String(const std::string_view& s)
{
	*this = s;
}
const char* shok::String::c_str() const
{
	if (allocated < 16)
		return data.inlin;
	else
		return data.alloc;
}
// ReSharper disable once CppDFAConstantFunctionResult
size_t shok::String::size() const
{
	return size_v;
}
shok::String::~String()
{
	// ReSharper disable once CppDFAConstantConditions
	if (allocated >= 16)
		// ReSharper disable once CppDFAUnreachableCode
		shok::Free(data.alloc);
}
static inline void(__thiscall* const str_assign)(shok::String* th, const char* c) = reinterpret_cast<void(__thiscall*)(shok::String*, const char*)>(0x40182E);
void shok::String::assign(const char* s)
{
	str_assign(this, s);
}
inline void(__thiscall* const str_assign_l)(shok::String* th, const char* c, size_t l) = reinterpret_cast<void(__thiscall*)(shok::String*, const char*, size_t)>(0x401799);
void shok::String::assign(const char* s, size_t len)
{
	str_assign_l(this, s, len);
}
inline shok::String* (__thiscall* const str_append)(shok::String* th, shok::String* a, size_t p, size_t c) = reinterpret_cast<shok::String * (__thiscall*)(shok::String*, shok::String*, size_t, size_t)>(0x405B62);
shok::String& shok::String::append(String& toappend, size_t pos, size_t count)
{
	return *str_append(this, &toappend, pos, count);
}
inline shok::String* (__thiscall* const str_append2)(shok::String* th, shok::String* a) = reinterpret_cast<shok::String * (__thiscall*)(shok::String*, shok::String*)>(0x459310);
shok::String& shok::String::append(String& toappend)
{
	return *str_append2(this, &toappend);
}
inline void(__thiscall* const str_resize)(shok::String* th, size_t l, char c) = reinterpret_cast<void(__thiscall*)(shok::String*, size_t, char)>(0x5540BA);
void shok::String::resize(size_t len, char c)
{
	str_resize(this, len, c);
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
shok::String& shok::String::operator=(const String& s)
{
	assign(s.c_str(), s.size());
	return *this;
}
shok::String& shok::String::operator=(const std::string& s)
{
	assign(s.c_str(), s.size());
	return *this;
}
shok::String& shok::String::operator=(const std::string_view& s)
{
	assign(s.data(), s.size());
	return *this;
}
shok::String& shok::String::operator=(const char* s)
{
	assign(s);
	return *this;
}
shok::String::operator std::string_view() const
{
	return { c_str(), size() };
}

std::strong_ordering shok::operator<=>(const String& a, std::string_view b)
{
	return std::string_view{ a.c_str(), a.size() } <=> b;
}
bool shok::operator==(const String& a, std::string_view b)
{
	return (a <=> b) == std::strong_ordering::equal;
}
std::strong_ordering shok::operator<=>(std::string_view a, const String& b)
{
	return a <=> std::string_view{ b.c_str(), b.size() };
}
bool shok::operator==(std::string_view a, const String& b)
{
	return (a <=> b) == std::strong_ordering::equal;
}

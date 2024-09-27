#include "pch.h"

#include "s5_baseDefs.h"


void shok::Position::FloorToBuildingPlacement()
{
	X = std::floorf(X / 100) * 100;
	Y = std::floorf(Y / 100) * 100;
}
void shok::Position::RoundToBuildingPlacement()
{
	X = std::roundf(X / 100) * 100;
	Y = std::roundf(Y / 100) * 100;
}

float shok::Position::GetDistanceSquaredTo(const shok::Position& p) const
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	return (dx * dx + dy * dy);
}

bool shok::Position::IsInRange(const shok::Position& p, float range) const
{
	return GetDistanceSquaredTo(p) <= (range * range);
}

float shok::Position::GetAngleBetween(const shok::Position& p) const
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	if (dx == 0 && dy == 0)
		return 0;
	float a = std::asinf(std::fabsf(dx) / (std::sqrtf(dx * dx + dy * dy)));
	a = CppLogic::RadiansToDegrees<float>(a);
	if (dx >= 0 && dy > 0)
		a = 270 - a;
	else if (dx < 0 && dy > 0)
		a = 270 + a;
	else if (dx < 0 && dy <= 0)
		a = 90 - a;
	else if (dx >= 0 && dy <= 0)
		a = 90 + a;
	return a;
}
shok::Position shok::Position::Rotate(float r) const
{
	float s = std::sinf(r);
	float c = std::cosf(r);
	return { X * c + Y * s, X * s + Y * c };
}
shok::Position shok::Position::RotateAround(float r, const shok::Position& center) const
{
	shok::Position p = *this - center;
	p = p.Rotate(r);
	return p + center;
}
inline void(__thiscall* const pos_normalize)(const shok::Position* p, shok::Position* r) = reinterpret_cast<void(__thiscall*)(const shok::Position*, shok::Position*)>(0x57D7F7);
shok::Position shok::Position::Normalize() const
{
	shok::Position r;
	pos_normalize(this, &r);
	return r;
}
shok::Position shok::Position::ClampToWorld(float border) const
{
	float size = static_cast<float>(*reinterpret_cast<int*>(0x898B74) * 100 - 100) - border;
	return { std::clamp(X, border, size), std::clamp(Y, border, size) };
}

shok::Position shok::Position::operator+(const shok::Position& other) const
{
	shok::Position r = *this;
	r += other;
	return r;
}
shok::Position& shok::Position::operator+=(const shok::Position& other)
{
	this->X += other.X;
	this->Y += other.Y;
	return *this;
}
shok::Position shok::Position::operator-(const shok::Position& other) const
{
	shok::Position r = *this;
	r -= other;
	return r;
}
shok::Position& shok::Position::operator-=(const shok::Position& other)
{
	this->X -= other.X;
	this->Y -= other.Y;
	return *this;
}
shok::Position shok::Position::operator*(float f) const
{
	shok::Position r = *this;
	r *= f;
	return r;
}
shok::Position& shok::Position::operator*=(float f)
{
	this->X *= f;
	this->Y *= f;
	return *this;
}
float shok::Position::Dot(const shok::Position& o) const
{
	return this->X * o.X + this->Y * o.Y;
}
BB::SerializationData* shok::Position::SerializationData = reinterpret_cast<BB::SerializationData*>(0x85D9B0);

shok::AARect shok::AARect::operator+(const shok::AARect& other) const
{
	return { this->low + other.low, this->high + other.high };
}
shok::AARect& shok::AARect::operator+=(const shok::AARect& other)
{
	this->low += other.low;
	this->high += other.high;
	return *this;
}
shok::AARect shok::AARect::operator-(const shok::AARect& other) const
{
	return { this->low - other.low, this->high - other.high };
}
shok::AARect& shok::AARect::operator-=(const shok::AARect& other)
{
	this->low -= other.low;
	this->high -= other.high;
	return *this;
}
shok::AARect shok::AARect::Rotate(float r) const
{
	return { low.Rotate(r), high.Rotate(r) };
}
shok::AARect shok::AARect::Sort() const
{
	shok::AARect r = *this;
	if (r.high.X < r.low.X)
		std::swap(r.high.X, r.low.X);
	if (r.high.Y < r.low.Y)
		std::swap(r.high.Y, r.low.Y);
	return r;
}
BB::SerializationData* shok::AARect::SerializationData = reinterpret_cast<BB::SerializationData*>(0x85DA90);

shok::Color::Color(int r, int g, int b, int a)
{
	R = r & 0xFF;
	G = g & 0xFF;
	B = b & 0xFF;
	A = a & 0xFF;
}

void* __stdcall BB::IObject::CastToIdentifier(shok::ClassId id)
{
	return nullptr;
}

bool __stdcall ECore::ICheckData::Check()
{
	return true;
}

static inline float(__thiscall* const costinfo_getres)(const shok::CostInfo* th, shok::ResourceType ty, bool addRaw) = reinterpret_cast<float(__thiscall*)(const shok::CostInfo*, shok::ResourceType, bool)>(0x4A9606);
float shok::CostInfo::GetResourceAmountFromType(shok::ResourceType ty, bool addRaw) const
{
	return costinfo_getres(this, ty, addRaw);
}

static inline void(__thiscall* const costinfo_add)(shok::CostInfo* th, shok::ResourceType ty, float a) = reinterpret_cast<void(__thiscall*)(shok::CostInfo*, shok::ResourceType, float)>(0x4A9774);
void shok::CostInfo::AddToType(shok::ResourceType ty, float toadd)
{
	costinfo_add(this, ty, toadd);
}

static inline void(__thiscall* const costinfo_sub)(shok::CostInfo* th, shok::ResourceType ty, float a, float b) = reinterpret_cast<void(__thiscall*)(shok::CostInfo*, shok::ResourceType, float, float)>(0x4A963D);
void shok::CostInfo::SubFromType(shok::ResourceType ty, float tosub)
{
	costinfo_sub(this, ty, tosub, 0.0f);
}

static inline bool(__thiscall* const constinfo_hasres)(const shok::CostInfo* th, const shok::CostInfo* has) = reinterpret_cast<bool(__thiscall*)(const shok::CostInfo*, const shok::CostInfo*)>(0x4A96D3);
bool shok::CostInfo::HasResources(const shok::CostInfo* has) const
{
	return constinfo_hasres(this, has);
}
inline bool(__thiscall* const costinfo_subci)(shok::CostInfo* th, const shok::CostInfo* s) = reinterpret_cast<bool(__thiscall*)(shok::CostInfo*, const shok::CostInfo*)>(0x4A971F);
bool shok::CostInfo::SubResources(const CostInfo& tosub)
{
	return costinfo_subci(this, &tosub);
}

bool CppLogic::CaselessStringComparator::operator()(const std::string& a, const std::string& b) const noexcept
{
	return _stricmp(a.c_str(), b.c_str()) < 0;
}

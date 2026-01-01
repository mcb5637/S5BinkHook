
#include "s5_baseDefs.h"

#include <algorithm>
#include <cctype>


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

static inline float(__thiscall* const pos_getangle)(const shok::Position* p) = reinterpret_cast<float(__thiscall*)(const shok::Position*)>(0x57D8F1);
float shok::Position::GetAngle() const
{
	return pos_getangle(this);
}

static inline float(__thiscall* const pos_getanglebetwe)(const shok::Position* p, const shok::Position* p2) = reinterpret_cast<float(__thiscall*)(const shok::Position*, const shok::Position*)>(0x57D98C);
float shok::Position::GetAngleBetweenR(const Position& p) const
{
	return pos_getanglebetwe(this, &p);
}
float shok::Position::GetAngleBetween(const shok::Position& p) const
{
	return CppLogic::RadiansToDegrees(GetAngleBetweenR(p));
}
shok::Position shok::Position::Rotate(float r) const
{
	float s = std::sin(r);
	float c = std::cos(r);
	return { X * c - Y * s, X * s + Y * c };
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
float shok::Position::DotSquare() const
{
	return Dot(*this);
}
shok::Position shok::Position::Abs() const noexcept
{
	return { std::abs(X), std::abs(Y) };
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

inline void(__thiscall* const eventhandlerlist_addhandlers)(EGL::EventHandlerList* th, shok::EventIDs id, EGL::EventHandler* h) = reinterpret_cast<void(__thiscall*)(EGL::EventHandlerList*, shok::EventIDs, EGL::EventHandler*)>(0x49E6A1);
void EGL::EventHandlerList::AddHandler(shok::EventIDs id, EGL::EventHandler* h)
{
	eventhandlerlist_addhandlers(this, id, h);
}

bool CppLogic::CaselessStringComparator::operator()(std::string_view a, std::string_view b) const noexcept
{
	return std::ranges::lexicographical_compare(a, b, [](char x, char y) { return std::tolower(x) < std::tolower(y); });
}

CppLogic::Rect::Rect(const shok::Position& start, const shok::Position& end, float width)
{
	float l = width;
	auto dir = (end - start);
	auto r = dir.Normalize().Rotate(DegreesToRadians(90.0f)) * l;
	A = start - r;
	B = start + r;
	C = end + r;
}

bool CppLogic::Rect::IsPosInRect(const shok::Position& p) const noexcept
{
	auto ab = B - A;
	auto bc = C - B;
	auto f = ab.Dot(p - A);
	auto g = bc.Dot(p - B);
	return 0 <= f && f <= ab.DotSquare() && 0 <= g && g <= bc.DotSquare();
}

shok::AARect CppLogic::Rect::BoundingBox() const noexcept
{
	auto d = C + (A - B);
	return { shok::Position{
			std::min(std::min(A.X, B.X), std::min(C.X, d.X)),
			std::min(std::min(A.Y, B.Y), std::min(C.Y, d.Y)),
		}, shok::Position {
			std::max(std::max(A.X, B.X), std::max(C.X, d.X)),
			std::max(std::max(A.Y, B.Y), std::max(C.Y, d.Y)),
		}
	};
}

#include "pch.h"

#include "s5data.h"
#include <stdexcept>
#include <libloaderapi.h>


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
	a = rad2deg<float>(a);
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

void* __stdcall BB::IObject::CastToIdentifier(unsigned int id)
{
	return nullptr;
}

void CppLogic::Hooks::RedirectCall(void* call, void* redirect) {
	byte* opcode = reinterpret_cast<byte*>(call);
	if (*opcode==0xFF && opcode[1]==0x15) { // call by address
		*opcode = 0xE8; // call
		opcode[5] = 0x90; // nop
	}
	if (*opcode != 0xE8) { // call
		DEBUGGER_BREAK;
	}
	int* adr = reinterpret_cast<int*>(opcode + 1);
	*adr = reinterpret_cast<int>(redirect) - reinterpret_cast<int>(adr + 1); // address relative to next instruction
}
void CppLogic::Hooks::RedirectCallVP(void* call, void* redirect)
{
	shok::SaveVirtualProtect vp{ call, 10 };
	CppLogic::Hooks::RedirectCall(call, redirect);
}
long long CppLogic::Hooks::WriteJump(void* adr, void* toJump) {
	long long r = *reinterpret_cast<long long*>(adr);
	byte* opcode = reinterpret_cast<byte*>(adr);
	*opcode = 0xE9; // jmp
	int* a = reinterpret_cast<int*>(opcode + 1);
	*a = reinterpret_cast<int>(toJump) - reinterpret_cast<int>(a + 1); // address relative to next instruction
	return r;
}

lua_State* shok::LuaStateMainmenu = nullptr;


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

bool CppLogic::HasSCELoader()
{
	return GetModuleHandle("SCELoader.dll");
}

const char* (*shok::GetStringTableTextOverride)(const char* s) = nullptr;
const char* __stdcall hookstt(const char* s) {
	if (!shok::GetStringTableTextOverride)
		return nullptr;
	return shok::GetStringTableTextOverride(s);
}
void __declspec(naked) hooksttasm() {
	__asm {
		//int 3
		push[esp + 4];
		call hookstt;
		test eax, eax;
		jz notfound;
		ret;

	notfound:
		mov eax, 0x894508;
		mov eax, [eax];
		push 0x556D33;
		ret;
	}
}
bool HookGetStringTableText_Hooked = false;
void shok::HookGetStringTableText()
{
	if (HookGetStringTableText_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		DEBUGGER_BREAK;
	HookGetStringTableText_Hooked = true;
	shok::SaveVirtualProtect vp{ shok::GetStringTableText , 10 };
	CppLogic::Hooks::WriteJump(shok::GetStringTableText, &hooksttasm);
}

void __declspec(naked) textprinting_getstringlen() {
	__asm {
		push 0;

	l_f60:
		cmp dword ptr[esp], 1;
		je l_skipOn;
		cmp ecx, 0x40; // @
		je l_turnSkipOn;
		jmp l_base;

	l_skipOn:
		cmp ecx, 0x20; // space
		je l_turnSkipOff;
		mov ecx, -1
		jmp l_f8e;

	l_turnSkipOn:
		mov dword ptr[esp], 1;
		jmp l_f8e;

	l_turnSkipOff:
		mov dword ptr[esp], 0;

	l_base:
		cmp ecx, 0x80; // base
		jl l_f89;
		sub ecx, [esi + 0x124];
		js l_f84;
		cmp ecx, [esi + 0x128];
		jge l_f84;
		mov eax, [esi + 0x12C];
		movsx ecx, word ptr[eax + ecx * 2];
		jmp l_f8e;
	l_f84:
		or ecx, -1;
		jmp l_f8e;
	l_f89:
		movsx ecx, word ptr[esi + ecx * 2 + 0x24];
	l_f8e:
		add edx, ebp; // next
		test ecx, ecx;
		jl l_fa3;
		mov eax, [esp + 0x14 + 8];
		shl ecx, 5;
		fld dword ptr[ecx + eax];
		fadd dword ptr[esi + 0xc];
		faddp st(1), st;
	l_fa3:
		movzx ecx, byte ptr[edx + edi];
		movzx eax, byte ptr[edx];
		and ecx, ebx;
		shl ecx, 8;
		or ecx, eax;
		jg l_f60;

		mov dword ptr[esp], 0x708FB3;
		ret;
	}
}
bool HookTextPrinting_Hooked = false;
void shok::HookTextPrinting()
{
	if (HookTextPrinting_Hooked)
		return;
	HookTextPrinting_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x557E47), 10 };
	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x708F60), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x557E47), reinterpret_cast<void*>(0x557DAA)); // continue checking @ after center,... (redirecting an existing jmp, and removing a push for a previous parameter)
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x708F60), &textprinting_getstringlen);
}

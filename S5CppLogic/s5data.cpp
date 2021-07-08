#include "pch.h"

#include "s5data.h"
#include <stdexcept>
#include <libloaderapi.h>


static inline void(__thiscall* const str_ctor)(shok_string* th, const char* s) = reinterpret_cast<void(__thiscall*)(shok_string*, const char*)>(0x4018C6);
shok_string::shok_string(const char* s)
{
	str_ctor(this, s);
}
static inline void(__thiscall* const str_ctorcopy)(shok_string* th, const shok_string* ot) = reinterpret_cast<void(__thiscall*)(shok_string*, const shok_string*)>(0x401808);
shok_string::shok_string(const shok_string& c)
{
	str_ctorcopy(this, &c);
}
const char* shok_string::c_str()
{
	if (size < 16)
		return reinterpret_cast<const char*>(&data);
	else
		return reinterpret_cast<const char*>(data);
}
shok_string::~shok_string()
{
	if (size >= 16)
		shok_free(reinterpret_cast<void*>(data));
}
static inline void(__thiscall* const str_assign)(shok_string* th, const char* c) = reinterpret_cast<void(__thiscall*)(shok_string*, const char*)>(0x40182E);
void shok_string::assign(const char* s)
{
	str_assign(this, s);
}
shok_string::shok_string() : shok_string("")
{
}

void shok_position::FloorToBuildingPlacement()
{
	X = std::floorf(X / 100) * 100;
	Y = std::floorf(Y / 100) * 100;
}

float shok_position::GetDistanceSquaredTo(shok_position& p)
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	return (dx * dx + dy * dy);
}

bool shok_position::IsInRange(shok_position& p, float range)
{
	return GetDistanceSquaredTo(p) <= (range * range);
}

float shok_position::GetAngleBetween(shok_position& p)
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	if (dx == 0 && dy == 0)
		return 0;
	float a = std::asinf(std::fabsf(dx) / (std::sqrtf(dx * dx + dy * dy)));
	a = (float)rad2deg((double)a);
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

void RedirectCall(void* call, void* redirect) {
	byte* opcode = reinterpret_cast<byte*>(call);
	if (*opcode != 0xE8) { // call
		DEBUGGER_BREAK;
	}
	int* adr = reinterpret_cast<int*>(opcode + 1);
	*adr = reinterpret_cast<int>(redirect) - reinterpret_cast<int>(adr + 1); // address relative to next instruction
}
void WriteJump(void* adr, void* toJump) {
	byte* opcode = reinterpret_cast<byte*>(adr);
	*opcode = 0xE9; // jmp
	int* a = reinterpret_cast<int*>(opcode + 1);
	*a = reinterpret_cast<int>(toJump) - reinterpret_cast<int>(a + 1); // address relative to next instruction
}

bool operator<(shok_attachment a, shok_attachment b) {
	if (a.AttachmentType == b.AttachmentType)
		return a.EntityId < b.EntityId;
	return a.AttachmentType < b.AttachmentType;
}

struct shok_BB_CFileStreamEx {
	int vtable = 0x761C60;
	int x = 0;
};
static inline bool(__thiscall* const shok_BB_CFileStreamEx_OpenFile)(shok_BB_CFileStreamEx* th, const char* name, int unk) = reinterpret_cast<bool(__thiscall*)(shok_BB_CFileStreamEx*, const char*, int)>(0x54924D);
static inline size_t(__stdcall* const shok_BB_CFileStreamEx_GetSize)(shok_BB_CFileStreamEx* th) = reinterpret_cast<size_t(__stdcall*)(shok_BB_CFileStreamEx*)>(0x549140);
static inline int(__stdcall* const shok_BB_CFileStreamEx_ReadToBuffer)(shok_BB_CFileStreamEx* th, void* buff, size_t buffsiz) = reinterpret_cast<int(__stdcall*)(shok_BB_CFileStreamEx*, void*, size_t)>(0x5491A8);
static inline void(__thiscall* const shok_BB_CFileStreamEx_Close)(shok_BB_CFileStreamEx* th) = reinterpret_cast<void(__thiscall*)(shok_BB_CFileStreamEx*)>(0x54920A);
static inline int(__thiscall* const shok_BB_CFileStreamEx_dtor)(shok_BB_CFileStreamEx* th) = reinterpret_cast<int(__thiscall*)(shok_BB_CFileStreamEx*)>(0x549215);

const char* ReadFileToString(const char* name, size_t* size)
{
	char* buff = nullptr;
	try
	{
		shok_BB_CFileStreamEx filestr{};
		if (shok_BB_CFileStreamEx_OpenFile(&filestr, name, 0x10113)) {
			size_t s = shok_BB_CFileStreamEx_GetSize(&filestr);
			if (size)
				*size = s;
			if (s > 0) {
				buff = new char[s + 1];
				memset(buff, 0, s + 1);
				shok_BB_CFileStreamEx_ReadToBuffer(&filestr, buff, s);
			}
			shok_BB_CFileStreamEx_Close(&filestr);
		}
		shok_BB_CFileStreamEx_dtor(&filestr);
	}
	catch (...)
	{
		if (buff)
			delete[] buff;
		return nullptr;
	}
	return buff;
}

bool DoesFileExist(const char* name)
{
	bool r = false;
	try
	{
		shok_BB_CFileStreamEx filestr{};
		if (shok_BB_CFileStreamEx_OpenFile(&filestr, name, 0x10113)) {
			size_t s = shok_BB_CFileStreamEx_GetSize(&filestr);
			if (s > 0) {
				r = true;
			}
			shok_BB_CFileStreamEx_Close(&filestr);
		}
		shok_BB_CFileStreamEx_dtor(&filestr);
	}
	catch (...)
	{
		return false;
	}
	return r;
}

static inline float(__thiscall* const costinfo_getres)(shok_costInfo* th, shok_ResourceType ty, bool addRaw) = reinterpret_cast<float(__thiscall*)(shok_costInfo*, shok_ResourceType, bool)>(0x4A9606);
float shok_costInfo::GetResourceAmountFromType(shok_ResourceType ty, bool addRaw)
{
	return costinfo_getres(this, ty, addRaw);
}

static inline void(__thiscall* const costinfo_add)(shok_costInfo* th, shok_ResourceType ty, float a) = reinterpret_cast<void(__thiscall*)(shok_costInfo*, shok_ResourceType, float)>(0x4A9774);
void shok_costInfo::AddToType(shok_ResourceType ty, float toadd)
{
	costinfo_add(this, ty, toadd);
}

static inline void(__thiscall* const costinfo_sub)(shok_costInfo* th, shok_ResourceType ty, float a, float b) = reinterpret_cast<void(__thiscall*)(shok_costInfo*, shok_ResourceType, float, float)>(0x4A963D);
void shok_costInfo::SubFromType(shok_ResourceType ty, float tosub)
{
	costinfo_sub(this, ty, tosub, 0.0f);
}

static inline bool(__thiscall* const constinfo_hasres)(shok_costInfo* th, shok_costInfo* has) = reinterpret_cast<bool(__thiscall*)(shok_costInfo*, shok_costInfo*)>(0x4A96D3);
bool shok_costInfo::HasResources(shok_costInfo* has)
{
	return constinfo_hasres(this, has);
}

bool HasSCELoader()
{
	return GetModuleHandle(L"SCELoader.dll");
}

const char* (*GetStringTableTextOverride)(const char* s) = nullptr;
const char* __stdcall hookstt(const char* s) {
	if (!GetStringTableTextOverride)
		return nullptr;
	return GetStringTableTextOverride(s);
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
void HookGetStringTableText()
{
	if (HasSCELoader())
		DEBUGGER_BREAK;
	WriteJump(shok_GetStringTableText, &hooksttasm);
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
void HookTextPrinting()
{
	WriteJump(reinterpret_cast<void*>(0x557E47), reinterpret_cast<void*>(0x557DAA)); // continue checking @ after center,... (redirecting an existing jmp, and removing a push for a previous parameter)

	WriteJump(reinterpret_cast<void*>(0x708F60), &textprinting_getstringlen);
}

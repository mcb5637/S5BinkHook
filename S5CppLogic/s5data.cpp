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

shok_saveVirtualProtect::shok_saveVirtualProtect() : shok_saveVirtualProtect(reinterpret_cast<void*>(SHOK_SEGMENTSTART), SHOK_SEGMENTLENGTH)
{
}
shok_saveVirtualProtect::shok_saveVirtualProtect(void* adr, size_t size)
{
	Adr = adr;
	Size = size;
	Prev = PAGE_EXECUTE_READWRITE;
	VirtualProtect(adr, size, PAGE_EXECUTE_READWRITE, &Prev);
}
shok_saveVirtualProtect::~shok_saveVirtualProtect()
{
	VirtualProtect(Adr, Size, Prev, &Prev);
}

void shok_position::FloorToBuildingPlacement()
{
	X = std::floorf(X / 100) * 100;
	Y = std::floorf(Y / 100) * 100;
}
void shok_position::RoundToBuildingPlacement()
{
	X = std::roundf(X / 100) * 100;
	Y = std::roundf(Y / 100) * 100;
}

float shok_position::GetDistanceSquaredTo(const shok_position& p) const
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	return (dx * dx + dy * dy);
}

bool shok_position::IsInRange(const shok_position& p, float range) const
{
	return GetDistanceSquaredTo(p) <= (range * range);
}

float shok_position::GetAngleBetween(const shok_position& p) const
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
shok_position shok_position::Rotate(float r) const
{
	float s = std::sinf(r);
	float c = std::cosf(r);
	return { X * c + Y * s, X * s + Y * c };
}
shok_position shok_position::RotateAround(float r, const shok_position& center) const
{
	shok_position p = *this - center;
	p = p.Rotate(r);
	return p + center;
}

shok_position shok_position::operator+(const shok_position& other) const
{
	return { this->X + other.X, this->Y + other.Y };
}
shok_position shok_position::operator-(const shok_position& other) const
{
	return { this->X - other.X, this->Y - other.Y };
}

void RedirectCall(void* call, void* redirect) {
	byte* opcode = reinterpret_cast<byte*>(call);
	if (*opcode != 0xE8) { // call
		DEBUGGER_BREAK;
	}
	int* adr = reinterpret_cast<int*>(opcode + 1);
	*adr = reinterpret_cast<int>(redirect) - reinterpret_cast<int>(adr + 1); // address relative to next instruction
}
long long WriteJump(void* adr, void* toJump) {
	long long r = *reinterpret_cast<long long*>(adr);
	byte* opcode = reinterpret_cast<byte*>(adr);
	*opcode = 0xE9; // jmp
	int* a = reinterpret_cast<int*>(opcode + 1);
	*a = reinterpret_cast<int>(toJump) - reinterpret_cast<int>(a + 1); // address relative to next instruction
	return r;
}

bool operator<(shok_attachment a, shok_attachment b) {
	if (a.AttachmentType == b.AttachmentType)
		return a.EntityId < b.EntityId;
	return a.AttachmentType < b.AttachmentType;
}

lua_State* mainmenu_state = nullptr;


static inline bool(__thiscall* const shok_BB_CFileStreamEx_OpenFile)(shok_BB_CFileStreamEx* th, const char* name, int unk) = reinterpret_cast<bool(__thiscall*)(shok_BB_CFileStreamEx*, const char*, int)>(0x54924D);
static inline size_t(__stdcall* const shok_BB_CFileStreamEx_GetSize)(shok_BB_CFileStreamEx* th) = reinterpret_cast<size_t(__stdcall*)(shok_BB_CFileStreamEx*)>(0x549140);
static inline int(__stdcall* const shok_BB_CFileStreamEx_ReadToBuffer)(shok_BB_CFileStreamEx* th, void* buff, size_t buffsiz) = reinterpret_cast<int(__stdcall*)(shok_BB_CFileStreamEx*, void*, size_t)>(0x5491A8);
static inline void(__thiscall* const shok_BB_CFileStreamEx_Close)(shok_BB_CFileStreamEx* th) = reinterpret_cast<void(__thiscall*)(shok_BB_CFileStreamEx*)>(0x54920A);
static inline int(__thiscall* const shok_BB_CFileStreamEx_dtor)(shok_BB_CFileStreamEx* th) = reinterpret_cast<int(__thiscall*)(shok_BB_CFileStreamEx*)>(0x549215);
shok_BB_CFileStreamEx::shok_BB_CFileStreamEx()
{
	vtable = shok_BB_CFileStreamEx::vtp;
}
shok_BB_CFileStreamEx::~shok_BB_CFileStreamEx()
{
	shok_BB_CFileStreamEx_dtor(this);
}
bool shok_BB_CFileStreamEx::OpenFile(const char* filename, int u)
{
	return shok_BB_CFileStreamEx_OpenFile(this, filename, u);
}
size_t shok_BB_CFileStreamEx::GetSize()
{
	return shok_BB_CFileStreamEx_GetSize(this);
}
int shok_BB_CFileStreamEx::ReadToBuffer(void* buff, size_t s)
{
	return shok_BB_CFileStreamEx_ReadToBuffer(this, buff, s);
}
void shok_BB_CFileStreamEx::Close()
{
	shok_BB_CFileStreamEx_Close(this);
}

const char* ReadFileToString(const char* name, size_t* size)
{
	char* buff = nullptr;
	try
	{
		shok_BB_CFileStreamEx filestr{};
		if (filestr.OpenFile(name, 0x10113)) {
			size_t s = filestr.GetSize();
			if (size)
				*size = s;
			if (s > 0) {
				buff = new char[s + 1];
				memset(buff, 0, s + 1);
				filestr.ReadToBuffer(buff, s);
			}
			filestr.Close();
		}
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
		if (filestr.OpenFile(name, 0x10113)) {
			size_t s = filestr.GetSize();
			if (s > 0) {
				r = true;
			}
			filestr.Close();
		}
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

void shok_BB_IObject::Destructor(bool free)
{
	reinterpret_cast<shok_vtable_BB_IObject*>(vtable)->dtor(this, free);
}
unsigned int shok_BB_IObject::GetIdentifier() const
{
	return reinterpret_cast<shok_vtable_BB_IObject*>(vtable)->GetClassIdentifier(this);
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
bool HookGetStringTableText_Hooked = false;
void HookGetStringTableText()
{
	if (HookGetStringTableText_Hooked)
		return;
	if (HasSCELoader())
		DEBUGGER_BREAK;
	HookGetStringTableText_Hooked = true;
	shok_saveVirtualProtect vp{ shok_GetStringTableText , 10 };
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
bool HookTextPrinting_Hooked = false;
void HookTextPrinting()
{
	if (HookTextPrinting_Hooked)
		return;
	HookTextPrinting_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x557E47), 10 };
	shok_saveVirtualProtect vp2{ reinterpret_cast<void*>(0x708F60), 10 };
	WriteJump(reinterpret_cast<void*>(0x557E47), reinterpret_cast<void*>(0x557DAA)); // continue checking @ after center,... (redirecting an existing jmp, and removing a push for a previous parameter)
	WriteJump(reinterpret_cast<void*>(0x708F60), &textprinting_getstringlen);
}

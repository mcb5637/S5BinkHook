#include "hooks.h"
#include <algorithm>
#include <stdexcept>
#include <Win.h>


CppLogic::Hooks::SaveVirtualProtect::SaveVirtualProtect() : CppLogic::Hooks::SaveVirtualProtect(reinterpret_cast<void*>(SHOK_SEGMENTSTART), SHOK_SEGMENTLENGTH)
{
}
CppLogic::Hooks::SaveVirtualProtect::SaveVirtualProtect(void* adr, size_t size)
{
	Adr = adr;
	Size = size;
	Prev = PAGE_EXECUTE_READWRITE;
	VirtualProtect(adr, size, PAGE_EXECUTE_READWRITE, &Prev);
}
CppLogic::Hooks::SaveVirtualProtect::SaveVirtualProtect(size_t size, std::initializer_list<void*> adrs) : SaveVirtualProtect(std::min<void*>(adrs), reinterpret_cast<int>(std::max<void*>(adrs)) - reinterpret_cast<int>(std::min<void*>(adrs)) + size)
{
}
CppLogic::Hooks::SaveVirtualProtect::~SaveVirtualProtect()
{
	VirtualProtect(Adr, Size, Prev, &Prev);
}

void CppLogic::Hooks::RedirectCall(void* call, void* redirect) {
	byte* opcode = static_cast<byte*>(call);
	if (*opcode == 0xFF && opcode[1] == 0x15) { // call by address
		*opcode = 0xE8; // call
		opcode[5] = 0x90; // nop
	}
	if (*opcode != 0xE8) { // call
		throw std::logic_error{ "trying to redirect something thats not a call" };
	}
	int* adr = reinterpret_cast<int*>(opcode + 1);
	*adr = reinterpret_cast<int>(redirect) - reinterpret_cast<int>(adr + 1); // address relative to next instruction
}
void CppLogic::Hooks::WriteJump(void* adr, void* toJump, void* nextvalid)
{
	if (reinterpret_cast<int>(nextvalid) - reinterpret_cast<int>(adr) < 5)
		throw std::logic_error{ "not enough space for jump" };
	byte* opcode = reinterpret_cast<byte*>(adr);
	*opcode = 0xE9; // jmp
	int* a = reinterpret_cast<int*>(opcode + 1);
	*a = reinterpret_cast<int>(toJump) - reinterpret_cast<int>(a + 1); // address relative to next instruction
	WriteNops(a + 1, nextvalid); // fill the rest with nops
}
void CppLogic::Hooks::WriteJump(void* adr, void* toJump, void* nextvalid, byte* backup)
{
	if (reinterpret_cast<int>(nextvalid) - reinterpret_cast<int>(adr) < 5)
		throw std::logic_error{ "not enough space for jump" };
	for (byte* a = static_cast<byte*>(adr); a < static_cast<byte*>(nextvalid); ++a) {
		*backup = *a;
		++backup;
	}
	WriteJump(adr, toJump, nextvalid);
}
void CppLogic::Hooks::RestoreJumpBackup(void* adr, byte* backup)
{
	byte* a = static_cast<byte*>(adr);
	for (int i = 0; i < 5; ++i) { // the jump
		*a = *backup;
		++a;
		++backup;
	}
	while (*a == 0x90) { // the nop padding
		*a = *backup;
		++a;
		++backup;
	}
}

void CppLogic::Hooks::WriteNops(void* adr, int num)
{
	byte* a = reinterpret_cast<byte*>(adr);
	for (int i = 0; i < num; ++i)
		a[i] = 0x90;
}
void CppLogic::Hooks::WriteNops(void* adr, void* nextvalid)
{
	for (byte* a = static_cast<byte*>(adr); a < static_cast<byte*>(nextvalid); ++a)
		*a = 0x90;
}

void CppLogic::Hooks::ReplaceOpcodes(void *adr, void *replacement, void *adrEnd) {
	auto* a = static_cast<byte*>(adr);
	auto* r = static_cast<byte*>(replacement);
	for (auto* i = a; i < adrEnd; ++i) {
		*i = *r;
		if (*r == 0xcc) // int3
			throw std::logic_error{"end of replacement reached?"};
		++r;
	}
}

std::vector<void*> CppLogic::Hooks::CheckMemFree::ToFree{};
void __stdcall CppLogic::Hooks::CheckMemFree::OnFree(void* p)
{
	ToFree.erase(std::remove(ToFree.begin(), ToFree.end(), p), ToFree.end());
}
void __declspec(naked) checkfreemem_freeasm() {
	__asm {
		mov eax, [esp + 4];
		push eax;
		call CppLogic::Hooks::CheckMemFree::OnFree;

		push 0xC;
		push 0x788F48;

		push 0x5C2E34;
		ret;
	};
}
void CppLogic::Hooks::CheckMemFree::EnableHook()
{
	SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5C2E2D), 0x5C2E34 - 0x5C2E2D };
	WriteJump(reinterpret_cast<void*>(0x5C2E2D), &checkfreemem_freeasm, reinterpret_cast<void*>(0x5C2E34));
}


bool CppLogic::HasSCELoader()
{
	return GetModuleHandle("SCELoader.dll");
}

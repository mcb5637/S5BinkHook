#include "pch.h"
#include "hooks.h"


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
CppLogic::Hooks::SaveVirtualProtect::~SaveVirtualProtect()
{
	VirtualProtect(Adr, Size, Prev, &Prev);
}

void CppLogic::Hooks::RedirectCall(void* call, void* redirect) {
	byte* opcode = reinterpret_cast<byte*>(call);
	if (*opcode == 0xFF && opcode[1] == 0x15) { // call by address
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
	CppLogic::Hooks::SaveVirtualProtect vp{ call, 10 };
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

void CppLogic::Hooks::WriteNops(void* adr, int num)
{
	byte* a = reinterpret_cast<byte*>(adr);
	for (int i = 0; i < num; ++i)
		a[i] = 0x90;
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
	SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5C2E2D), 10 };
	WriteJump(reinterpret_cast<void*>(0x5C2E2D), &checkfreemem_freeasm);
}


bool CppLogic::HasSCELoader()
{
	return GetModuleHandle("SCELoader.dll");
}

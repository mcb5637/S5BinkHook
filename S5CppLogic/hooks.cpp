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


bool CppLogic::HasSCELoader()
{
	return GetModuleHandle("SCELoader.dll");
}

#include "s5_effecttype.h"
#include <shok/globals/s5_classfactory.h>
#include <utility/hooks.h>

void __declspec(naked) fireprops_fix_ctor() {
	__asm {
		mov dword ptr[esi + 0x2C], 0x41480000;
		mov dword ptr[esi + 48], 0;

		push 0x4FAC4D;
		ret;
	}
}

bool CGLEffectFireProps_HookFixCtor = false;
void GGL::CGLEffectFireProps::HookFixCtor()
{
	if (CGLEffectFireProps_HookFixCtor)
		return;
	CGLEffectFireProps_HookFixCtor = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4FAC46), 0x10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FAC46), &fireprops_fix_ctor, reinterpret_cast<void*>(0x4FAC4D));
	auto f = reinterpret_cast<BB::SerializationData * (__stdcall* const)()>(0x4FB0C6);
	auto* sd = f();
	if (sd[4].SerializationName == std::string_view("SoundID")) {
		sd[4].SerializationName = "SoundIDAgain";
	}
}

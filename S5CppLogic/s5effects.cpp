#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGL_CFlyingEffect {
	PADDINGI(9);
	int(__thiscall* OnHit)(shok_EGL_CFlyingEffect* th);
};


shok_EGL_CGLEEffectCreator::shok_EGL_CGLEEffectCreator()
{
	vtable = 0x76E140;
}

shok_EGL_CFlyingEffectCreator::shok_EGL_CFlyingEffectCreator()
{
	vtable = 0x774688;
}

shok_CProjectileEffectCreator::shok_CProjectileEffectCreator()
{
	vtable = 0x774698;
}

bool shok_EGL_CEffect::IsCannonBallEffect()
{
	return vtable == shok_GGL_CCannonBallEffect::vtp;
}

bool shok_EGL_CEffect::IsArrowEffect()
{
	return vtable == shok_GGL_CArrowEffect::vtp;
}

bool shok_GGL_CCannonBallEffect::FixDamageClass = false;
void __fastcall hookcannonfromcreator(shok_GGL_CCannonBallEffect* th, shok_CProjectileEffectCreator* cr) {
	if (shok_GGL_CCannonBallEffect::FixDamageClass) {
		th->DamageClass = cr->DamageClass;
	}
}
void __declspec(naked) hookcannonfromcreatorasm() {
	__asm {
		mov[esi + 0x0C8], eax;
		mov eax, [edi + 0x38];
		mov[esi + 0x0CC], eax;

		mov ecx, esi;
		mov edx, edi;
		call hookcannonfromcreator;

		push 0x4FF951;
		ret;
	}
}
void shok_GGL_CCannonBallEffect::HookFromCreator()
{
	WriteJump((void*)0x4FF942, &hookcannonfromcreatorasm);
}

void (*FlyingEffectOnHitCallback2)(shok_EGL_CFlyingEffect* eff, bool post) = nullptr;
void (*FlyingEffectOnHitCallback)(shok_EGL_CFlyingEffect* eff) = nullptr;
int(__thiscall* CannonBallOnHit)(shok_EGL_CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(shok_EGL_CFlyingEffect* th);
int __fastcall CannonBallOnHitHook(shok_EGL_CFlyingEffect* th) {
	if (FlyingEffectOnHitCallback)
		FlyingEffectOnHitCallback(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, false);
	int i = CannonBallOnHit(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, true);
	// TODO remove rewriting vtable after kimichura removes the reset of it
	((shok_vtable_EGL_CFlyingEffect*)shok_GGL_CCannonBallEffect::vtp)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	((shok_vtable_EGL_CFlyingEffect*)shok_GGL_CArrowEffect::vtp)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
	return i;
}
int(__thiscall* ArrowOnHit)(shok_EGL_CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(shok_EGL_CFlyingEffect* th) {
	if (FlyingEffectOnHitCallback)
		FlyingEffectOnHitCallback(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, false);
	int i = ArrowOnHit(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, true);
	((shok_vtable_EGL_CFlyingEffect*)shok_GGL_CCannonBallEffect::vtp)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	((shok_vtable_EGL_CFlyingEffect*)shok_GGL_CArrowEffect::vtp)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
	return i;
}
void shok_EGL_CFlyingEffect::HookOnHit()
{
	if (CannonBallOnHit)
		return;
	shok_vtable_EGL_CFlyingEffect* vt = (shok_vtable_EGL_CFlyingEffect*)shok_GGL_CCannonBallEffect::vtp;
	CannonBallOnHit = vt->OnHit;
	vt->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	vt = (shok_vtable_EGL_CFlyingEffect*)shok_GGL_CArrowEffect::vtp;
	ArrowOnHit = vt->OnHit;
	vt->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
}

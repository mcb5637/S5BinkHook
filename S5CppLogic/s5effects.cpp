#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGL_CFlyingEffect {
	PADDINGI(9);
	int(__thiscall* OnHit)(shok_EGL_CFlyingEffect* th);
};


shok_EGL_CGLEEffectCreator::shok_EGL_CGLEEffectCreator()
{
	vtable = shok_EGL_CGLEEffectCreator::vtp;
}

shok_EGL_CFlyingEffectCreator::shok_EGL_CFlyingEffectCreator()
{
	vtable = shok_EGL_CFlyingEffectCreator::vtp;
}

shok_CProjectileEffectCreator::shok_CProjectileEffectCreator()
{
	vtable = shok_CProjectileEffectCreator::vtp;
}

bool shok_EGL_CEffect::IsCannonBallEffect()
{
	return shok_DynamicCast<shok_EGL_CEffect, shok_GGL_CCannonBallEffect>(this);
}

bool shok_EGL_CEffect::IsArrowEffect()
{
	return shok_DynamicCast<shok_EGL_CEffect, shok_GGL_CArrowEffect>(this);
}

bool shok_GGL_CCannonBallEffect::FixDamageClass = false;
bool shok_GGL_CCannonBallEffect::AddDamageSourceOverride = false;
void __fastcall hookcannonfromcreator(shok_GGL_CCannonBallEffect* th, shok_CProjectileEffectCreator* cr) {
	if (shok_GGL_CCannonBallEffect::FixDamageClass) {
		th->DamageClass = cr->DamageClass;
		if (shok_GGL_CCannonBallEffect::AddDamageSourceOverride)
			th->DamageClass |= static_cast<int>(cr->AdvancedDamageSourceOverride) << 24;
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
bool HookFromCreator_Hooked = false;
void shok_GGL_CCannonBallEffect::HookFromCreator()
{
	if (HookFromCreator_Hooked)
		return;
	HookFromCreator_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4FF942), 10 };
	WriteJump(reinterpret_cast<void*>(0x4FF942), &hookcannonfromcreatorasm);
}

shok_EGL_CFlyingEffect* shok_EGL_CFlyingEffect::CurrentHittingEffect = nullptr;
void (*shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2)(shok_EGL_CFlyingEffect* eff, bool post) = nullptr;
void (*shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback)(shok_EGL_CFlyingEffect* eff) = nullptr;
int(__thiscall* CannonBallOnHit)(shok_EGL_CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(shok_EGL_CFlyingEffect* th);
void WriteProjectlileOnHits();
int __fastcall CannonBallOnHitHook(shok_EGL_CFlyingEffect* th) {
	shok_EGL_CFlyingEffect::CurrentHittingEffect = th;
	if (shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback)
		shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback(th);
	if (shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2)
		shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2(th, false);
	int i = CannonBallOnHit(th);
	if (shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2)
		shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2(th, true);
	shok_EGL_CFlyingEffect::CurrentHittingEffect = nullptr;
	// TODO remove rewriting vtable after kimichura removes the reset of it
	WriteProjectlileOnHits();
	return i;
}
int(__thiscall* ArrowOnHit)(shok_EGL_CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(shok_EGL_CFlyingEffect* th) {
	shok_EGL_CFlyingEffect::CurrentHittingEffect = th;
	if (shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback)
		shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback(th);
	if (shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2)
		shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2(th, false);
	int i = ArrowOnHit(th);
	if (shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2)
		shok_EGL_CFlyingEffect::FlyingEffectOnHitCallback2(th, true);
	shok_EGL_CFlyingEffect::CurrentHittingEffect = nullptr;
	WriteProjectlileOnHits();
	return i;
}
void WriteProjectlileOnHits() {
	shok_vtable_EGL_CFlyingEffect* vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(shok_GGL_CCannonBallEffect::vtp);
	shok_saveVirtualProtect vp{ vt, 12 * 4 };
	vt->OnHit = reinterpret_cast<int(__thiscall*)(shok_EGL_CFlyingEffect*)>(&CannonBallOnHitHook);
	vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(shok_GGL_CArrowEffect::vtp);
	shok_saveVirtualProtect vp2{ vt, 12 * 4 };
	vt->OnHit = reinterpret_cast<int(__thiscall*)(shok_EGL_CFlyingEffect*)>(&ArrowOnHitHook);
}
void shok_EGL_CFlyingEffect::HookOnHit()
{
	if (CannonBallOnHit)
		return;
	shok_vtable_EGL_CFlyingEffect* vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(shok_GGL_CCannonBallEffect::vtp);
	CannonBallOnHit = vt->OnHit;
	vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(shok_GGL_CArrowEffect::vtp);
	ArrowOnHit = vt->OnHit;
	WriteProjectlileOnHits();
}

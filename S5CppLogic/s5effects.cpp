#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGL_CFlyingEffect {
	PADDINGI(9)
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
	return vtable == shok_vtp_GGL_CCannonBallEffect;
}

bool shok_EGL_CEffect::IsArrowEffect()
{
	return vtable == shok_vtp_GGL_CArrowEffect;
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
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CCannonBallEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CArrowEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
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
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CCannonBallEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CArrowEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
	return i;
}
void shok_EGL_CFlyingEffect::HookOnHit()
{
	if (CannonBallOnHit)
		return;
	shok_vtable_EGL_CFlyingEffect* vt = (shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CCannonBallEffect;
	CannonBallOnHit = vt->OnHit;
	vt->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	vt = (shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CArrowEffect;
	ArrowOnHit = vt->OnHit;
	vt->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
}
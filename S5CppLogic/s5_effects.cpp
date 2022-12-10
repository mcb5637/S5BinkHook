#include "pch.h"
#include "s5_effects.h"
#include "s5_effecttype.h"
#include "hooks.h"
#include "entityiterator.h"

struct shok_vtable_EGL_CFlyingEffect {
	PADDINGI(9);
	int(__thiscall* OnHit)(EGL::CFlyingEffect* th);
};


EGL::CGLEEffectCreator::CGLEEffectCreator()
{
	SetVT(EGL::CGLEEffectCreator::vtp);
}
void EGL::CGLEEffectCreator::SetVT(int vt) {
	*reinterpret_cast<int*>(this) = vt;
}
unsigned int __stdcall EGL::CGLEEffectCreator::GetClassIdentifier() const {
	return 0;
}
void* __stdcall EGL::CGLEEffectCreator::CastToIdentifier(unsigned int id) {
	return nullptr;
}

EGL::CFlyingEffectCreator::CFlyingEffectCreator()
{
	SetVT(EGL::CFlyingEffectCreator::vtp);
}

CProjectileEffectCreator::CProjectileEffectCreator()
{
	SetVT(CProjectileEffectCreator::vtp);
}

void __stdcall EGL::CFlyingEffectSlot::FillSlot(SSlotArgsFlyingEffect* data)
{
	reinterpret_cast<void(__stdcall*)(CFlyingEffectSlot*, SSlotArgsFlyingEffect*)>(0x589338)(this, data);
}
unsigned int __stdcall EGL::CFlyingEffectSlot::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall EGL::CFlyingEffectSlot::CastToIdentifier(unsigned int id)
{
	return nullptr;
}

void EGL::CFlyingEffect::FixOnLoad()
{
	auto* pr = (*EGL::CGLEEffectsProps::GlobalObj)->EffectTypes[this->EffectType];
	this->FlyingEffectProps = dynamic_cast<EGL::CFlyingEffectProps*>(pr);
	this->FlyingEffectSlot.Speed = this->FlyingEffectProps->Speed; // probably gets reset to 0 somewhere, cause it should get serialized
	this->FlyingEffectSlot.TargetPosition = this->TargetPosition;
	this->FlyingEffectSlot.LastPosition = this->Position; // not entirely correct, but seems to be better than nothing
}

void GGL::CArrowEffect::FixOnLoad()
{
	EGL::CFlyingEffect::FixOnLoad();
	this->ArrowEffectProps = dynamic_cast<GGL::CArrowEffectProps*>(this->FlyingEffectProps);
}

void GGL::CCannonBallEffect::FixOnLoad()
{
	EGL::CFlyingEffect::FixOnLoad();
	this->CannonBallEffectProps = dynamic_cast<GGL::CCannonBallEffectProps*>(this->FlyingEffectProps);
}

void EGL::CFlyingEffect::HookOnLoadFix()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 4, {
		reinterpret_cast<void*>(0x7775F8),
		reinterpret_cast<void*>(0x778E38),
		reinterpret_cast<void*>(0x7776A4),
	} };
	*reinterpret_cast<void**>(0x7775F8) = CppLogic::Hooks::MemberFuncPointerToVoid(&CFlyingEffect::FixOnLoad, 0);
	*reinterpret_cast<void**>(0x778E38) = CppLogic::Hooks::MemberFuncPointerToVoid(&GGL::CArrowEffect::FixOnLoad, 0);
	*reinterpret_cast<void**>(0x7776A4) = CppLogic::Hooks::MemberFuncPointerToVoid(&GGL::CCannonBallEffect::FixOnLoad, 0);
}


bool GGL::CCannonBallEffect::FixDamageClass = false;
bool GGL::CCannonBallEffect::AddDamageSourceOverride = false;
void __fastcall hookcannonfromcreator(GGL::CCannonBallEffect* th, CProjectileEffectCreator* cr) {
	if (GGL::CCannonBallEffect::FixDamageClass) {
		th->DamageClass = cr->DamageClass;
		if (GGL::CCannonBallEffect::AddDamageSourceOverride)
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
void GGL::CCannonBallEffect::HookFromCreator()
{
	if (HookFromCreator_Hooked)
		return;
	HookFromCreator_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4FF942), 0x4FF951 - 0x4FF942 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FF942), &hookcannonfromcreatorasm, reinterpret_cast<void*>(0x4FF951));
}

EGL::CFlyingEffect* EGL::CFlyingEffect::CurrentHittingEffect = nullptr;
void (*EGL::CFlyingEffect::FlyingEffectOnHitCallback2)(EGL::CFlyingEffect* eff, bool post) = nullptr;
void (*EGL::CFlyingEffect::FlyingEffectOnHitCallback)(EGL::CFlyingEffect* eff) = nullptr;
int(__thiscall* CannonBallOnHit)(EGL::CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(EGL::CFlyingEffect* th);
void WriteProjectlileOnHits();
int __fastcall CannonBallOnHitHook(EGL::CFlyingEffect* th) {
	EGL::CFlyingEffect::CurrentHittingEffect = th;
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback(th);
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback2)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2(th, false);
	int i = CannonBallOnHit(th);
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback2)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2(th, true);
	EGL::CFlyingEffect::CurrentHittingEffect = nullptr;
	// TODO remove rewriting vtable after kimichura removes the reset of it
	WriteProjectlileOnHits();
	return i;
}
int(__thiscall* ArrowOnHit)(EGL::CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(EGL::CFlyingEffect* th) {
	EGL::CFlyingEffect::CurrentHittingEffect = th;
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback(th);
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback2)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2(th, false);
	int i = ArrowOnHit(th);
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback2)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2(th, true);
	EGL::CFlyingEffect::CurrentHittingEffect = nullptr;
	WriteProjectlileOnHits();
	return i;
}
void WriteProjectlileOnHits() {
	shok_vtable_EGL_CFlyingEffect* vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(GGL::CCannonBallEffect::vtp);
	CppLogic::Hooks::SaveVirtualProtect vp{ vt, 12 * 4 };
	vt->OnHit = reinterpret_cast<int(__thiscall*)(EGL::CFlyingEffect*)>(&CannonBallOnHitHook);
	vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(GGL::CArrowEffect::vtp);
	CppLogic::Hooks::SaveVirtualProtect vp2{ vt, 12 * 4 };
	vt->OnHit = reinterpret_cast<int(__thiscall*)(EGL::CFlyingEffect*)>(&ArrowOnHitHook);
}
void EGL::CFlyingEffect::HookOnHit()
{
	if (CannonBallOnHit)
		return;
	shok_vtable_EGL_CFlyingEffect* vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(GGL::CCannonBallEffect::vtp);
	CannonBallOnHit = vt->OnHit;
	vt = reinterpret_cast<shok_vtable_EGL_CFlyingEffect*>(GGL::CArrowEffect::vtp);
	ArrowOnHit = vt->OnHit;
	WriteProjectlileOnHits();
}

ED::IEffect* ED::CDEVisibleEffectManager::GetDisplayForEffectID(int id)
{
	for (auto& e : Effects) {
		if (e.ID == id)
			return e.Effect;
	}
	return nullptr;
}

void ED::CDEVisibleEffectManager::DestroyDisplayForEffect(int id)
{
	struct data {
		data* p;
		data* n;
		VisEffect e;
	};
	data* d = nullptr;
	for (auto& e : Effects) {
		if (e.ID == id) {
			int v = reinterpret_cast<int>(&e.Effect);
			v -= offsetof(data, e.Effect);
			d = reinterpret_cast<data*>(v);
		}
	}
	if (d == nullptr)
		return;
	int nexout = 0;
	delete d->e.Effect;
	reinterpret_cast<void(__thiscall*)(int, void*, data*)>(0x7221EF)(reinterpret_cast<int>(this) + 8, &nexout, d);
}

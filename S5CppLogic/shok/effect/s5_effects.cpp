#include "pch.h"
#include "s5_effects.h"
#include <shok/effect/s5_effecttype.h>
#include <shok/globals/s5_classfactory.h>
#include <utility/hooks.h>
#include <utility/entityiterator.h>

shok::ClassId EGL::CEffect::GetClassIdentifier() const
{
	return Identifier;
}
void EGL::CEffect::FromCreator(EGL::CGLEEffectCreator* ct)
{
	auto p = reinterpret_cast<void(__thiscall* const)(EGL::CEffect*, EGL::CGLEEffectCreator*)>(0x588BE6);
	p(this, ct);
}
void EGL::CEffect::OnCreated()
{
	auto p = reinterpret_cast<void(__thiscall* const)(EGL::CEffect*)>(0x588D9C);
	p(this);
}
void EGL::CEffect::OnLoaded()
{
}
void EGL::CEffect::OnTick()
{
}
void EGL::CEffect::FireEvent(BB::CEvent* ev)
{
	auto p = reinterpret_cast<void(__thiscall* const)(EGL::CEffect*, BB::CEvent*)>(0x588EFA);
	p(this, ev);
}
void __stdcall EGL::CEffect::emptyfunc2()
{
}

shok::EffectTypeId __stdcall EGL::CEffect::GetEffectType() const
{
	return EffectType;
}
shok::PlayerId __stdcall EGL::CEffect::GetPlayerID() const
{
	return PlayerID;
}
EGL::ISlot* __stdcall EGL::CEffect::GetFlyingEffectSlot()
{
	return nullptr;
}


shok::ClassId __stdcall EGL::CFlyingEffect::GetClassIdentifier() const
{
	return Identifier;
}
void EGL::CFlyingEffect::FromCreator(EGL::CGLEEffectCreator* ct)
{
	auto p = reinterpret_cast<void(__thiscall* const)(EGL::CFlyingEffect*, EGL::CGLEEffectCreator*)>(0x589697);
	p(this, ct);
}
void EGL::CFlyingEffect::OnCreated()
{
	auto p = reinterpret_cast<void(__thiscall* const)(EGL::CFlyingEffect*)>(0x589687);
	p(this);
}
void EGL::CFlyingEffect::OnTick()
{
	auto p = reinterpret_cast<void(__thiscall* const)(EGL::CFlyingEffect*)>(0x589477);
	p(this);
}
EGL::ISlot* __stdcall EGL::CFlyingEffect::GetFlyingEffectSlot()
{
	return &FlyingEffectSlot;
}
void EGL::CFlyingEffect::OnHit()
{
	delete this;
}
void EGL::CFlyingEffect::CalculateGravityStuff()
{
	auto p = reinterpret_cast<void(__thiscall* const)(EGL::CFlyingEffect*)>(0x589787);
	p(this);
}
bool EGL::CFlyingEffect::OnTickMove()
{
	auto p = reinterpret_cast<bool(__thiscall* const)(EGL::CFlyingEffect*)>(0x58937E);
	return p(this);
}

void(__stdcall*EGL::CEffect::OnDestroyCb)(EGL::CEffect* th) = nullptr;
void __declspec(naked) effect_ondestroyhookasm() {
	__asm {
		mov dword ptr[edi], 0x784AE4;

		mov eax, EGL::CEffect::OnDestroyCb;
		cmp eax, 0;
		je retu;
		push ecx;
		call eax;

	retu:
		push 0x588FE0;
		ret;
	};
}
bool HookOnDestroyHooked = false;
void EGL::CEffect::HookOnDestroy()
{
	if (HookOnDestroyHooked)
		return;
	HookOnDestroyHooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x588FDA), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x588FDA), &effect_ondestroyhookasm, reinterpret_cast<void*>(0x588FE0));
}


EGL::CGLEEffectCreator::CGLEEffectCreator()
{
	SetVT(EGL::CGLEEffectCreator::vtp);
}
void EGL::CGLEEffectCreator::SetVT(int vt) {
	*reinterpret_cast<int*>(this) = vt;
}
shok::ClassId __stdcall EGL::CGLEEffectCreator::GetClassIdentifier() const {
	return Identifier;
}
void* __stdcall EGL::CGLEEffectCreator::CastToIdentifier(shok::ClassId id) {
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
shok::ClassId EGL::CFlyingEffectSlot::GetClassIdentifier() const
{
	return Identifier;
}
void* __stdcall EGL::CFlyingEffectSlot::CastToIdentifier(shok::ClassId id)
{
	return nullptr;
}

EGL::CGLEEffectProps* EGL::CEffect::LogicProps() const
{
	return (*EGL::CGLEEffectsProps::GlobalObj)->GetLogic(this->EffectType);
}

void EGL::CFlyingEffect::FixOnLoad()
{
	auto* pr = this->LogicProps();
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
void __fastcall GGL::CCannonBallEffect::CannonFromCreatorAdd(GGL::CCannonBallEffect* th, CProjectileEffectCreator* cr)
{
	if (GGL::CCannonBallEffect::FixDamageClass) {
		th->DamageClass = cr->DamageClass;
		th->DamageClassPadding = 0;
		if (GGL::CCannonBallEffect::AddDamageSourceOverride)
			th->AdvancedDamageSourceOverride = cr->AdvancedDamageSourceOverride;
	}
	else {
		th->DamageClass = shok::DamageClassId::Invalid;
		th->DamageClassPadding = 0;
		th->AdvancedDamageSourceOverride = shok::AdvancedDealDamageSource::Unknown;
	}
}

void NAKED_DEF GGL::CCannonBallEffect::CannonFromCreatorAddASM() {
	__asm {
		mov[esi + 0x0C8], eax;
		mov eax, [edi + 0x38];
		mov[esi + 0x0CC], eax;

		mov ecx, esi;
		mov edx, edi;
		call GGL::CCannonBallEffect::CannonFromCreatorAdd;

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
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FF942), &CannonFromCreatorAddASM, reinterpret_cast<void*>(0x4FF951));
}

void __declspec(naked) arrowonhit_damage() {
	__asm {
		mov ebx, 0;
		mov bl, [esi + 50 * 4 + 3]; // AdvancedDamageSourceOverride
		cmp ebx, 0;
		jne pu;
		mov ebx, 2; // arrow

	pu:
		push ebx;
		push 1;
		push 1;
		push 1;

		push[esi + 22 * 4]; // effect player

		push ecx;
		push eax;

		mov ecx, [ebp - 0x10];
		call EGL::CGLEEntity::AdvancedHurtEntityByStatic;

		push 0x5113CF;
		ret;
	};
}
void GGL::CArrowEffect::HookDealDamage()
{
	{
		CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
			reinterpret_cast<void*>(0x5113C2),
			reinterpret_cast<byte*>(0x4DBA20),
			reinterpret_cast<byte*>(0x511637),
		} };
		CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5113C2), &arrowonhit_damage, reinterpret_cast<void*>(0x5113CF));
		// projectile creator bigger zero for AdvancedDamageSourceOverride
		*reinterpret_cast<byte*>(0x4DBA20) = 0x89; // mov [esi+0x44], al -> mov [esi+0x44], eax
		// arrow projcetile AdvancedDamageSourceOverride
		*reinterpret_cast<byte*>(0x511634) = 0x8B; // mov al, [edi+0x44] -> mov eax, [edi+0x44]
		*reinterpret_cast<byte*>(0x511637) = 0x89; // mov [esi+0xC8], al -> mov [esi+0xC8], eax
	}
	{
		CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x87BC20), sizeof(BB::SerializationData) };
		auto* d = reinterpret_cast<BB::SerializationData*>(0x87BC20);
		assert(d->SerializationName == std::string_view{ "Misses" });
		d->DataConverter = BB::FieldSerializer::TypeInt;
		d->Size = sizeof(int);
	}
}

void __declspec(naked) cannonballhit_damage() {
	__asm {
		mov eax, [esi + 52 * 4];
		shr eax, 24;
		cmp eax, 0;
		jne pu;

		mov eax, 3;
	pu:
		push eax;

		push 1;
		push 1;
		push 1;
		mov eax, [esi + 52 * 4];
		and eax, 0xFFFF;
		push eax;
		push[esi + 48 * 4];
		push edx;
		push edx;
		fstp dword ptr [esp];

		lea eax, [esi + 16 * 4]; // pos
		push eax;
		push ebx;
		call EGL::CGLEEntity::AdvancedDealAoEDamage;

		mov eax, [esi + 0xC4];
		push 0x4FF51B;
		ret;
	};
}
void GGL::CCannonBallEffect::HookDealDamage()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
		reinterpret_cast<void*>(0x4FF4EB)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FF4EB), &cannonballhit_damage, reinterpret_cast<void*>(0x4FF51B));
}

EGL::CFlyingEffect* EGL::CFlyingEffect::CurrentHittingEffect = nullptr;
void (*EGL::CFlyingEffect::FlyingEffectOnHitCallback2)(EGL::CFlyingEffect* eff) = nullptr;
void (*EGL::CFlyingEffect::FlyingEffectOnHitCallback)(EGL::CFlyingEffect* eff) = nullptr;
inline void(__thiscall* const CannonBallOnHit)(GGL::CCannonBallEffect* th) = reinterpret_cast<void(__thiscall*)(GGL::CCannonBallEffect*)>(0x4FF476);
void GGL::CCannonBallEffect::OnHitHooked()
{
	EGL::CFlyingEffect::CurrentHittingEffect = this;
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback(this);
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback2)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2(this);
	CannonBallOnHit(this);
	EGL::CFlyingEffect::CurrentHittingEffect = nullptr;
	// TODO remove rewriting vtable after kimichura removes the reset of it
	HookOnHit();
}
inline void(__thiscall* const ArrowOnHit)(GGL::CArrowEffect* th) = reinterpret_cast<void(__thiscall*)(GGL::CArrowEffect*)>(0x511336);
void GGL::CArrowEffect::OnHitHooked()
{
	EGL::CFlyingEffect::CurrentHittingEffect = this;
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback(this);
	if (EGL::CFlyingEffect::FlyingEffectOnHitCallback2)
		EGL::CFlyingEffect::FlyingEffectOnHitCallback2(this);
	ArrowOnHit(this);
	EGL::CFlyingEffect::CurrentHittingEffect = nullptr;
	HookOnHit();
}
void EGL::CFlyingEffect::HookOnHit()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 4, {reinterpret_cast<void*>(0x778E48),
			reinterpret_cast<void*>(0x7776B4) }};
	*reinterpret_cast<void**>(0x778E48) = CppLogic::Hooks::MemberFuncPointerToVoid(&GGL::CArrowEffect::OnHitHooked, 0);
	*reinterpret_cast<void**>(0x7776B4) = CppLogic::Hooks::MemberFuncPointerToVoid(&GGL::CCannonBallEffect::OnHitHooked, 0);
}

static inline void(__stdcall* const rain_slot_fill)(GGL::CEffectRain::CSlotStartTurn* th, EGL::SSlotArgsStartTurn* d) = reinterpret_cast<void(__stdcall*)(GGL::CEffectRain::CSlotStartTurn*, EGL::SSlotArgsStartTurn*)>(0x5170E0);
void __stdcall GGL::CEffectRain::CSlotStartTurn::FillSlot(EGL::SSlotArgsStartTurn* data)
{
	return rain_slot_fill(this, data);
}

RWE::RpLight* __cdecl setcolor_nop(RWE::RpLight* l, void* color) {
	return l;
}
void __stdcall displayeff_light_updategametime(int, float) {

}
void GD::CDisplayEffectLightning::HookColorOverride(bool active)
{
	{
		CppLogic::Hooks::SaveVirtualProtect vp{ 5, {
			reinterpret_cast<void*>(0x4875FB),
		} };
		void* target = active ? static_cast<void*>(&setcolor_nop) : reinterpret_cast<void*>(0x6274C0);
		CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4875FB), target);
	}
	{
		CppLogic::Hooks::SaveVirtualProtect vp{ 5, {
			reinterpret_cast<void*>(0x76AE18),
		} };
		void* target = active ? static_cast<void*>(&displayeff_light_updategametime) : reinterpret_cast<void*>(0x4874BB);
		*reinterpret_cast<void**>(0x76AE18) = target;
	}
}

ED::IEffect* ED::CDEVisibleEffectManager::GetDisplayForEffectID(shok::EffectId id)
{
	for (auto& e : Effects) {
		if (e.ID == id)
			return e.Effect;
	}
	return nullptr;
}

void ED::CDEVisibleEffectManager::DestroyDisplayForEffect(shok::EffectId id)
{
	struct data { // NOLINT(*-pro-type-member-init)
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

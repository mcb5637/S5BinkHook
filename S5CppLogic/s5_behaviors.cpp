#include "pch.h"
#include "s5_behaviors.h"
#include "s5_entity.h"
#include "s5_entitytype.h"
#include "s5_behaviorProps.h"
#include "s5_effects.h"
#include "s5_maplogic.h"
#include "s5_defines.h"
#include "s5_entityandeffectmanager.h"
#include "s5_events.h"
#include "s5_tasklist.h"
#include "s5_effecttype.h"
#include "s5_scriptsystem.h"
#include "s5_netevents.h"
#include "hooks.h"
#include "EntityAddonData.h"
#include "savegame_extra.h"
#include "mod.h"

inline void(__thiscall* const path_waypoint_removelast)(EGL::CCoarsePath::WaypointData* th) = reinterpret_cast<void(__thiscall*)(EGL::CCoarsePath::WaypointData*)>(0x508988);
void EGL::CCoarsePath::WaypointData::RemoveLastWaypoint()
{
	path_waypoint_removelast(this);
}
shok::Position EGL::CCoarsePath::WaypointData::GetWaypoint(int i)
{
	if (i < 0 || i > CacheCount)
		throw std::out_of_range{ "invalid waypoint" };
	int c = CacheItem[i];
	int x = c & 0xFFFF;
	int y = (c >> 16) & 0xFFFF;
	return shok::Position{ static_cast<float>(x) * 100, static_cast<float>(y) * 100 };
}
inline bool(__thiscall* const path_navigate)(EGL::CCoarsePath* th, shok::Position* from, shok::Position* to, int one) = reinterpret_cast<bool(__thiscall*)(EGL::CCoarsePath*, shok::Position*, shok::Position*, int)>(0x5827A7);
bool EGL::CCoarsePath::Navigate(const shok::Position& from, const shok::Position& to)
{
	shok::Position f = from;
	shok::Position t = to;
	return path_navigate(this, &f, &t, 1);
}

inline void(__thiscall* const path_clear)(EGL::CCoarsePath* th) = reinterpret_cast<void(__thiscall*)(EGL::CCoarsePath*)>(0x582848);
void EGL::CCoarsePath::Clear()
{
	path_clear(this);
}

inline shok::TaskStateExecutionResult(__thiscall* const behanim_statehandlerwait)(EGL::CBehaviorAnimation* th, int i) = reinterpret_cast<shok::TaskStateExecutionResult(__thiscall*)(EGL::CBehaviorAnimation*, int)>(0x587E20);
shok::TaskStateExecutionResult EGL::CBehaviorAnimation::StateWaitForAnim(int i)
{
	return behanim_statehandlerwait(this, i);
}

inline void(__stdcall* const slotuvanims_fill)(EGL::CSlotUVAnims* th, EGL::SSlotArgsUVAnims* data) = reinterpret_cast<void(__stdcall*) (EGL::CSlotUVAnims*, EGL::SSlotArgsUVAnims*)> (0x5170E0);
void __stdcall EGL::CSlotUVAnims::FillSlot(SSlotArgsUVAnims* data)
{
	slotuvanims_fill(this, data);
}

inline void(__stdcall* const slotparticleeffatt_fill)(EGL::CSlotParticleEffectAttachment* th, EGL::SSlotArgsParticleEffectAttachment* data) = reinterpret_cast<void(__stdcall*) (EGL::CSlotParticleEffectAttachment*, EGL::SSlotArgsParticleEffectAttachment*)> (0x59084B);
void __stdcall EGL::CSlotParticleEffectAttachment::FillSlot(SSlotArgsParticleEffectAttachment* data)
{
	slotparticleeffatt_fill(this, data);
}

inline void(__stdcall* const slotpeffswitch_fill)(EGL::CSlotParticleEffectSwitch* th, EGL::SSlotArgsParticleEffectSwitch* data) = reinterpret_cast<void(__stdcall*) (EGL::CSlotParticleEffectSwitch*, EGL::SSlotArgsParticleEffectSwitch*)> (0x5170E0);
void __stdcall EGL::CSlotParticleEffectSwitch::FillSlot(SSlotArgsParticleEffectSwitch* data)
{
	slotpeffswitch_fill(this, data);
}

static inline void(__thiscall* const heroability_addhandlers)(GGL::CHeroAbility* th, shok::EntityId id) = reinterpret_cast<void(__thiscall*)(GGL::CHeroAbility*, shok::EntityId)>(0x4F4982);
void GGL::CHeroAbility::AddHandlers(shok::EntityId id)
{
	heroability_addhandlers(this, id);
}

static inline void(__thiscall* const heroability_oncreate)(GGL::CHeroAbility* th, EGL::CGLEBehaviorProps* p) = reinterpret_cast<void(__thiscall*)(GGL::CHeroAbility*, EGL::CGLEBehaviorProps*)>(0x4F477B);
void GGL::CHeroAbility::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	heroability_oncreate(this, p);
}

static inline void(__thiscall* const heroability_onload)(GGL::CHeroAbility* th, EGL::CGLEBehaviorProps* p) = reinterpret_cast<void(__thiscall*)(GGL::CHeroAbility*, EGL::CGLEBehaviorProps*)>(0x4F4799);
void GGL::CHeroAbility::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	heroability_onload(this, p);
}

static inline bool(__thiscall* const heroability_canuse)(GGL::CHeroAbility* th) = reinterpret_cast<bool(__thiscall*)(GGL::CHeroAbility*)>(0x4F47DC);
bool GGL::CHeroAbility::CanUseAbility()
{
	return heroability_canuse(this);
}

static inline bool(__thiscall* const heroability_checkandreset)(GGL::CHeroAbility* th) = reinterpret_cast<bool(__thiscall*)(GGL::CHeroAbility*)>(0x4F47ED);
bool GGL::CHeroAbility::CheckAndResetCooldown()
{
	return heroability_checkandreset(this);
}

void GGL::CCamouflageBehavior::EventOverrideOnAttacked(BB::CEvent* ev)
{
	if (ev->IsEvent(shok::EventIDs::OnAttackedBy) && CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CamoFix) {
		if (auto* a = dynamic_cast<CppLogic::Events::AdvHurtByEvent*>(ev)) {
			if (a->Source == shok::AdvancedDealDamageSource::Arrow || a->Source == shok::AdvancedDealDamageSource::Cannonball)
				return;
		}
	}
	InvisibilityRemaining = 0;
}
bool HookResetCamo_Hooked = false;
void GGL::CCamouflageBehavior::HookOnAttacked()
{
	EGL::CGLEEntity::HookHurtEntity();
	if (HookResetCamo_Hooked)
		return;
	HookResetCamo_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5011DF), 0x5011E6 - 0x5011DF };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5011DF), CppLogic::Hooks::MemberFuncPointerToVoid(&EventOverrideOnAttacked, 0), reinterpret_cast<void*>(0x5011E6));
}

static inline int(__thiscall* const activateCamoOrig)(GGL::CCamouflageBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CCamouflageBehavior*)>(0x501561);
void (*GGL::CCamouflageBehavior::CamoActivateCb)(GGL::CCamouflageBehavior* th);
int __thiscall GGL::CCamouflageBehavior::ActivateCamoOverride()
{
	int i = activateCamoOrig(this);
	if (GGL::CCamouflageBehavior::CamoActivateCb)
		GGL::CCamouflageBehavior::CamoActivateCb(this);
	return i;
}
bool HookCamoActivate_Hooked = false;
void GGL::CCamouflageBehavior::HookActivate()
{
	if (HookCamoActivate_Hooked)
		return;
	HookCamoActivate_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x10, { reinterpret_cast<void*>(0x4D51A4),
		reinterpret_cast<void*>(0x50163A)
	} };
	void* t = CppLogic::Hooks::MemberFuncPointerToVoid(&ActivateCamoOverride, 0);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4D51A4), t);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x50163A), t);
}


void __declspec(naked) bombattachment_fix() {
	__asm {
		push 0;
		push 0;
		push eax;
		push 43;
		push[esi + 8];
		call EGL::CGLEEntity::GetEntityByID;
		mov ecx, eax;
		call EGL::CGLEEntity::AttachEntity;


		or [ebp - 4], 0x0FFFFFFFF;
		lea ecx, [ebp - 0x3C];

		push 0x5062CD;
		ret;
	};
}
bool FixBombAttachment_Hooked = false;
void GGL::CBombPlacerBehavior::HookFixBombAttachment()
{

	if (FixBombAttachment_Hooked)
		return;
	FixBombAttachment_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5062C6), 0x5062CD - 0x5062C6 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5062C6), &bombattachment_fix, reinterpret_cast<void*>(0x5062CD));
}

void __declspec(naked) bombexplode_damage() {
	__asm {
		push 12;
		push 1;
		push 1;
		push 1;
		push 0;
		push edi;
		push ebx;

		push ebx;
		fstp[esp];

		lea eax, [eax + 22 * 4];
		push eax;
		push[ebp - 0x10];
		call EGL::CGLEEntity::AdvancedDealAoEDamage;

		push 0x506B3F;
		ret;
	};
}
void GGL::CBombBehavior::HookDealDamage()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
		reinterpret_cast<void*>(0x506B28)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x506B28), &bombexplode_damage, reinterpret_cast<void*>(0x506B3F));
}

shok::ClassId GGL::CCannonBuilderBehavior::GetClassIdentifier() const
{
	return Identifier;
}

void GGL::CCannonBuilderBehavior::AddHandlers(shok::EntityId id)
{
	reinterpret_cast<void(__thiscall*)(CCannonBuilderBehavior*, shok::EntityId)>(0x4FF053)(this, id);
}

void GGL::CCannonBuilderBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	reinterpret_cast<void(__thiscall*)(CCannonBuilderBehavior*, EGL::CGLEBehaviorProps*)>(0x4FE8C9)(this, p);
}

void GGL::CCannonBuilderBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	reinterpret_cast<void(__thiscall*)(CCannonBuilderBehavior*, EGL::CGLEBehaviorProps*)>(0x4FE8E6)(this, p);
}

bool GGL::CCannonBuilderBehavior::IsAbility(shok::AbilityId ability)
{
	return ability == shok::AbilityId::AbilityBuildCannon;
}

int GGL::CCannonBuilderBehavior::TaskGoToCannonPos(EGL::CGLETaskArgs* a)
{
	return reinterpret_cast<int(__thiscall*)(CCannonBuilderBehavior*, EGL::CGLETaskArgs*)>(0x4FECB3)(this, a);
}

int GGL::CCannonBuilderBehavior::TaskBuildCannon(EGL::CGLETaskArgs* a)
{
	return reinterpret_cast<int(__thiscall*)(CCannonBuilderBehavior*, EGL::CGLETaskArgs*)>(0x4FEE04)(this, a);
}

void GGL::CCannonBuilderBehavior::EventActivate(GGL::CEventPositionAnd2EntityTypes* e)
{
	return reinterpret_cast<void(__thiscall*)(CCannonBuilderBehavior*, GGL::CEventPositionAnd2EntityTypes*)>(0x4FEF27)(this, e);
}

void GGL::CCannonBuilderBehavior::EventCancel(BB::CEvent* e)
{
	return reinterpret_cast<void(__thiscall*)(CCannonBuilderBehavior*, BB::CEvent*)>(0x4FED34)(this, e);
}

void GGL::CCannonBuilderBehavior::EventOnFoundationDetach(EGL::CEvent1Entity* e)
{
	return reinterpret_cast<void(__thiscall*)(CCannonBuilderBehavior*, EGL::CEvent1Entity*)>(0x4FEDA5)(this, e);
}

void __thiscall GGL::CRangedEffectAbility::AdvHealAffected()
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	GGL::CRangedEffectAbilityProps* pr = e->GetEntityType()->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
	float hpfact = pr->HealthRecoveryFactor;
	if (hpfact <= 0)
		return;
	EGL::CGLEEffectCreator ecr{};
	ecr.PlayerID = e->PlayerId;
	ecr.EffectType = pr->HealEffect;
	for (const auto& a : e->ObservedEntities) {
		if (a.first == shok::AttachmentType::HERO_AFFECTED) {
			EGL::CGLEEntity* toheal = EGL::CGLEEntity::GetEntityByID(a.second.EntityId);
			if (!toheal)
				return;
			float heal = toheal->GetMaxHealth() * hpfact;
			if (ecr.EffectType != shok::EffectTypeId::Invalid) {
				ecr.StartPos.X = toheal->Position.X;
				ecr.StartPos.Y = toheal->Position.Y;
				(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&ecr);
			}
			if (toheal->GetBehavior<GGL::CSoldierBehavior>()) {
				toheal = EGL::CGLEEntity::GetEntityByID(toheal->GetFirstAttachedToMe(shok::AttachmentType::LEADER_SOLDIER));
			}
			if (!toheal)
				return;
			toheal->PerformHeal(static_cast<int>(heal), true);
		}
	}
}

void GGL::CRangedEffectAbility::HookHealAffected(bool active)
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4E3C78), 10 };
	if (active)
		CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4E3C78), CppLogic::Hooks::MemberFuncPointerToVoid(&AdvHealAffected, 0));
	else
		CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4E3C78), reinterpret_cast<void*>(0x4E39B4));
}

void __declspec(naked) circularatt_damage() {
	__asm {
		push 11;
		push 1;
		push 1;
		push 1;
		push 0;
		push eax;
		push esi;
		mov ecx, [ebp - 0x10];
		call EGL::CGLEEntity::AdvancedHurtEntityBy;

		push 0x4FE72F;
		ret;
	};
}
void GGL::CCircularAttack::HookDealDamage()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
		   reinterpret_cast<void*>(0x4FE722)
	   } };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FE722), &circularatt_damage, reinterpret_cast<void*>(0x4FE72F));
}

static inline void(__thiscall* const summonbeh_eventdie)(GGL::CSummonBehavior* th, BB::CEvent* ev) = reinterpret_cast<void(__thiscall*)(GGL::CSummonBehavior*, BB::CEvent*)>(0x4D6F25);
void GGL::CSummonBehavior::EventDie(BB::CEvent* ev)
{
	summonbeh_eventdie(this, ev);
}

static inline void(__thiscall* const summonbeh_eventactivate)(GGL::CSummonBehavior* th, BB::CEvent* ev) = reinterpret_cast<void(__thiscall*)(GGL::CSummonBehavior*, BB::CEvent*)>(0x4D6C2C);
void GGL::CSummonBehavior::EventActivate(BB::CEvent* ev)
{
	summonbeh_eventactivate(this, ev);
}

static inline int(__thiscall* const summonbeh_tasksummon)(GGL::CSummonBehavior* th, EGL::CGLETaskArgs* a) = reinterpret_cast<int(__thiscall*)(GGL::CSummonBehavior*, EGL::CGLETaskArgs*)>(0x4D6F8F);
int GGL::CSummonBehavior::TaskSummon(EGL::CGLETaskArgs* a)
{
	return summonbeh_tasksummon(this, a);
}

void __thiscall GGL::CConvertSettlerAbility::PerformConversion()
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto tid = e->GetFirstAttachedEntity(shok::AttachmentType::CONVERTER_SETTLER);
	auto* t = EGL::CGLEEntity::GetEntityByID(tid);
	if (t) {
		e->DetachObservedEntity(shok::AttachmentType::CONVERTER_SETTLER, tid, false);
		auto* n = t->AdvChangePlayer(e->PlayerId);
		CppLogic::Events::ConversionEvent ev{ shok::EventIDs::CppLogicEvent_OnConvert, e->EntityId, tid, n->EntityId };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
	}
}
void __declspec(naked) convertsettler_hookasm() {
	__asm {
		mov ecx, edi;
		call GGL::CConvertSettlerAbility::PerformConversion;

		push 0x4FCD2D;
		ret;
	};
}
bool HookConvert_Hooked = false;
void GGL::CConvertSettlerAbility::HookConvertEvent()
{
	if (HookConvert_Hooked)
		return;
	HookConvert_Hooked = true;
	EGL::CGLEEntity::ActivateEntityChangePlayerFix();
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4FCCFE), 0x4FCD05 - 0x4FCCFE };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FCCFE), &convertsettler_hookasm, reinterpret_cast<void*>(0x4FCD05));
}

static inline void(__thiscall* const summonbeh_addhandl)(GGL::CSummonBehavior* th, shok::EntityId id) = reinterpret_cast<void(__thiscall*)(GGL::CSummonBehavior*, shok::EntityId)>(0x4D6FBE);
void GGL::CSummonBehavior::AddHandlers(shok::EntityId id)
{
	summonbeh_addhandl(this, id);
}

static inline void(__thiscall* const summonbeh_oncr)(GGL::CSummonBehavior* th, EGL::CGLEBehaviorProps* p) = reinterpret_cast<void(__thiscall*)(GGL::CSummonBehavior*, EGL::CGLEBehaviorProps*)>(0x4D6B52);
void GGL::CSummonBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	summonbeh_oncr(this, p);
}

static inline void(__thiscall* const summonbeh_onlo)(GGL::CSummonBehavior* th, EGL::CGLEBehaviorProps* p) = reinterpret_cast<void(__thiscall*)(GGL::CSummonBehavior*, EGL::CGLEBehaviorProps*)>(0x4D6B6D);
void GGL::CSummonBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	summonbeh_onlo(this, p);
}

static inline bool(__thiscall* const summonbeh_isab)(GGL::CSummonBehavior* th, shok::AbilityId ab) = reinterpret_cast<bool(__thiscall*)(GGL::CSummonBehavior*, shok::AbilityId)>(0x4D6A64);
bool GGL::CSummonBehavior::IsAbility(shok::AbilityId ability)
{
	return summonbeh_isab(this, ability);
}

int (*GGL::CSniperAbility::SnipeDamageOverride)(EGL::CGLEEntity* sniper, EGL::CGLEEntity* tar, int dmg) = nullptr;
int __thiscall GGL::CSniperAbility::TaskOverrideSnipe(EGL::CGLETaskArgs* a)
{
	EGL::CGLEEntity* thent = EGL::CGLEEntity::GetEntityByID(EntityId);
	EGL::CGLEEntity* tar = EGL::CGLEEntity::GetEntityByID(TargetId);
	if (!tar || tar->Health <= 0)
		return 0;
	GGL::CSniperAbilityProps* pr = thent->GetEntityType()->GetBehaviorProps<GGL::CSniperAbilityProps>();
	if (thent->Position.GetDistanceSquaredTo(tar->Position) >= pr->Range * pr->Range)
		return 0;
	int dmg = static_cast<int>(tar->GetMaxHealth() * pr->DamageFactor);
	if (GGL::CSniperAbility::SnipeDamageOverride)
		dmg = GGL::CSniperAbility::SnipeDamageOverride(thent, tar, dmg);
	GGL::CBattleBehaviorProps* bpr = thent->GetEntityType()->GetBehaviorPropsDynamic< GGL::CBattleBehaviorProps>();
	CProjectileEffectCreator cr{};
	cr.EffectType = bpr->ProjectileEffectID;
	cr.PlayerID = thent->PlayerId;
	cr.StartPos = thent->Position;
	cr.CurrentPos = thent->Position;
	cr.TargetPos = tar->Position;
	cr.AttackerID = thent->EntityId;
	cr.TargetID = tar->EntityId;
	cr.Damage = dmg;
	cr.DamageRadius = -1;
	cr.DamageClass = shok::DamageClassId::Invalid;
	cr.SourcePlayer = thent->PlayerId;
	cr.AdvancedDamageSourceOverride = shok::AdvancedDealDamageSource::AbilitySnipe;
	(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&cr);
	return 0;
}
bool OverrideSnipeTask_Hooked = false;
void GGL::CSniperAbility::OverrideSnipeTask()
{
	if (OverrideSnipeTask_Hooked)
		return;
	OverrideSnipeTask_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4DB5B8), 0x4DB5BD - 0x4DB5B8 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4DB5B8), CppLogic::Hooks::MemberFuncPointerToVoid(&TaskOverrideSnipe, 0), reinterpret_cast<void*>(0x4DB5BD));
}

void __declspec(naked) shurikenthrow() {
	__asm {
		lea edx, [ebp - 0x5C];

		mov byte ptr[edx + 17 * 4 + 3], 15;

		mov[ebp - 0x20], eax;
		mov eax, [ecx];
		push edx;


		push 0x4DC6E2;
		ret;
	};
}
void GGL::CShurikenAbility::HookDealDamage()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
		   reinterpret_cast<void*>(0x4DC6D9)
	   } };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4DC6D9), &shurikenthrow, reinterpret_cast<void*>(0x4DC6E2));
}

static inline float(__thiscall* const battleBehaviorGetMaxRange)(const GGL::CBattleBehavior*) = reinterpret_cast<float(__thiscall*)(const GGL::CBattleBehavior*)>(0x50AB43);
float GGL::CBattleBehavior::GetMaxRange() const
{
	return battleBehaviorGetMaxRange(this);
}

int GGL::CBattleBehavior::GetDamage() const
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	const auto* d = e->GetAdditionalData();
	float base;
	if (d && d->DamageOverride >= 0)
		base = static_cast<float>(d->DamageOverride);
	else
		base = static_cast<float>(BattleProps->DamageAmount);
	base = e->ModifyDamage(base);
	float fact = e->GetTotalAffectedDamageModifier();
	return static_cast<int>(base * fact);
}

int GGL::CBattleBehavior::GetMaxRandomDamage() const
{
	return static_cast<int>(ModifierEntityDatabase::GlobalObj->GetModifiedStat(EntityId, GGL::CEntityProfile::ModifierType::DamageBonus, static_cast<float>(BattleProps->MaxDamageRandomBonus)));
}
int GGL::CBattleBehavior::GetRandomDamage()
{
	int r = GetMaxRandomDamage();
	if (r > 0) {
		std::uniform_int_distribution dist{ 0, r };
		r = dist((*EGL::CGLEGameLogic::GlobalObj)->RNG);
	}
	else {
		r = 0;
	}
	return r;
}

int __thiscall GGL::CBattleBehavior::GetDamageAgainst(EGL::CGLEEntity* target)
{
	return static_cast<int>(target->CalculateDamageAgainstMe(GetDamage() + GetRandomDamage(), BattleProps->DamageClass));
}

inline EGL::CGLEEntity* (__thiscall* const battlebeh_gettar)(const GGL::CBattleBehavior* th) = reinterpret_cast<EGL::CGLEEntity * (__thiscall*)(const GGL::CBattleBehavior*)>(0x50BB5D);
EGL::CGLEEntity* GGL::CBattleBehavior::GetTarget() const
{
	return battlebeh_gettar(this);
}

float GGL::CBattleBehavior::GetMissChance() const
{
	float c = ModifierEntityDatabase::GlobalObj->GetModifiedStat(EntityId, GGL::CEntityProfile::ModifierType::MissChance, static_cast<float>(BattleProps->MissChance));
	shok::WeatherState weather = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler->GetCurrentWeatherState();
	switch (weather) {
	case shok::WeatherState::Winter:
		c += (*GGL::CLogicProperties::GlobalObj)->WeatherMissChanceChangeSnow;
		break;
	case shok::WeatherState::Rain:
		c += (*GGL::CLogicProperties::GlobalObj)->WeatherMissChanceChangeRain;
		break;
	default:
		break;
	}
	return c;
}

bool GGL::CBattleBehavior::CheckMiss()
{
	float c = GetMissChance();
	if (c <= 0)
		return false;
	std::uniform_int_distribution dist{ 0, 100 };
	return c > dist((*EGL::CGLEGameLogic::GlobalObj)->RNG);
}

inline bool(__thiscall* const battlebeh_canattack)(GGL::CBattleBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CBattleBehavior*)>(0x50B8A0);
bool GGL::CBattleBehavior::CanAutoAttack()
{
	return battlebeh_canattack(this);
}

inline float(__thiscall* const battlebeh_getdmgclsfactvs)(GGL::CBattleBehavior* th, EGL::CGLEEntity* t) = reinterpret_cast<float(__thiscall*)(GGL::CBattleBehavior*, EGL::CGLEEntity*)>(0x50B668);
float GGL::CBattleBehavior::GetDamageClassFactorAgainst(EGL::CGLEEntity* target)
{
	return battlebeh_getdmgclsfactvs(this, target);
}

inline int (__thiscall*const battlebeh_getrandomattanim)(GGL::CBattleBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CBattleBehavior*)>(0x50AA39);
int GGL::CBattleBehavior::GetRandomAttackAnim()
{
	return battlebeh_getrandomattanim(this);
}

inline float(__thiscall* const battlebeh_getminrange)(const GGL::CBattleBehavior* th) = reinterpret_cast<float(__thiscall*)(const GGL::CBattleBehavior*)>(0x50AC09);
float GGL::CBattleBehavior::GetMinRange() const
{
	return battlebeh_getminrange(this);
}

float __thiscall GGL::CBattleBehavior::GetMaxRangeBase() const
{
	const EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	const auto* d = e->GetAdditionalData();
	if (d && d->MaxRangeOverride >= 0)
		return d->MaxRangeOverride;
	else
		return BattleProps->MaxRange;
}

void __thiscall GGL::CBattleBehavior::EventOverrideGetDamage(EGL::CEventGetValue_Int* ev)
{
	ev->Data = static_cast<int>(GetDamage());
}
int __thiscall GGL::CBattleBehavior::TaskOverrideFireProjctile(EGL::CGLETaskArgs* a)
{
	auto* en = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* tar = GetTarget();
	if (tar == nullptr) {
		EGL::CEvent1Entity ev{ shok::EventIDs::Leader_OnAttackTargetDetached, static_cast<shok::EntityId>(0) };
		en->FireEvent(&ev);
		return 2;
	}
	if (tar->Health <= 0) {
		en->DetachObservedEntity(shok::AttachmentType::ATTACKER_TARGET, tar->EntityId, false);
		EGL::CEvent1Entity ev{ shok::EventIDs::Leader_OnAttackTargetDetached, tar->EntityId };
		en->FireEvent(&ev);
		return 0;
	}

	CProjectileEffectCreator ct{};
	ct.EffectType = BattleProps->ProjectileEffectID;
	ct.PlayerID = en->PlayerId;
	ct.CurrentPos = ct.StartPos = en->Position;
	ct.TargetPos = tar->Position;
	ct.AttackerID = en->EntityId;
	ct.TargetID = tar->EntityId;
	ct.SourcePlayer = en->PlayerId;

	if (!dynamic_cast<GGL::CArrowEffectProps*>((*EGL::CGLEEffectsProps::GlobalObj)->GetLogic(BattleProps->ProjectileEffectID))) {
		ct.Damage = static_cast<int>(GetDamage()) + GetRandomDamage();
		ct.DamageRadius = BattleProps->DamageRange;
		ct.DamageClass = BattleProps->DamageClass;
	}
	else {
		int dmg = GetDamageAgainst(tar);

		if (dmg < 1)
			dmg = 1;

		ct.Damage = dmg;

		if (tar->GetAccessCategory() == shok::AccessCategory::AccessCategorySettler) {
			ct.Misses = CheckMiss();
		}
	}

	(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&ct);

	return 0;
}

bool BattleHookDamageMod_Hooked = false;
void GGL::CBattleBehavior::HookDamageOverride()
{
	if (BattleHookDamageMod_Hooked)
		return;
	BattleHookDamageMod_Hooked = true;

	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x50C77B),
		reinterpret_cast<void*>(0x50C1CD),
		reinterpret_cast<void*>(0x50C33C),
	} };
	// event
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50C77B), CppLogic::Hooks::MemberFuncPointerToVoid(&EventOverrideGetDamage, 0), reinterpret_cast<void*>(0x50C788));
	// task hurt
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50C1CD), CppLogic::Hooks::MemberFuncPointerToVoid(&GetDamageAgainst, 0), reinterpret_cast<void*>(0x50C1DC));
	// task fire projectile
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50C33C), CppLogic::Hooks::MemberFuncPointerToVoid(&TaskOverrideFireProjctile, 0), reinterpret_cast<void*>(0x50C34A));
}

void __declspec(naked) battlemaxrangeasm() {
	__asm {
		mov esi, ecx;

		pushad;
		call GGL::CBattleBehavior::GetMaxRangeBase;
		popad;

		mov eax, [esi + 0x30];
		push 0x50AB50;
		ret;
	}
}
bool BattleHookMaxRange_Hooked = false;
void GGL::CBattleBehavior::HookRangeOverride()
{
	if (BattleHookMaxRange_Hooked)
		return;
	BattleHookMaxRange_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x50AB48),
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50AB48), &battlemaxrangeasm, reinterpret_cast<void*>(0x50AB50));
}


void __declspec(naked) meleeonhit_damage() {
	__asm {
		push 1;
		push 1;
		push 1;
		push 1;
		push 0;
		push[ebp - 0x10];

		push[edi + 8];
		call EGL::CGLEEntity::GetEntityByID;
		push eax;

		mov ecx, esi;
		call EGL::CGLEEntity::AdvancedHurtEntityBy;

		push 0x50CA6D;
		ret;
	};
}
void GGL::CBattleBehavior::HookDealDamage()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
		   reinterpret_cast<void*>(0x50CA59),
	   } };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50CA59), &meleeonhit_damage, reinterpret_cast<void*>(0x50CA6D));
}

static inline int(__thiscall* const leaderbehgettroophealth)(GGL::CBattleBehavior*) = reinterpret_cast<int(__thiscall*)(GGL::CBattleBehavior*)>(0x4EE1D6);
int GGL::CLeaderBehavior::GetTroopHealth()
{
	return leaderbehgettroophealth(this);
}
static inline int(__thiscall* const leaderbehgettroophealthpersol)(GGL::CBattleBehavior*) = reinterpret_cast<int(__thiscall*)(GGL::CBattleBehavior*)>(0x4ECE77);
int GGL::CLeaderBehavior::GetTroopHealthPerSoldier()
{
	return leaderbehgettroophealthpersol(this);
}

inline float(__thiscall* const leaderbeh_getautoattackrange)(const GGL::CLeaderBehavior* th) = reinterpret_cast<float(__thiscall*)(const GGL::CLeaderBehavior*)>(0x4EAEFD);
float GGL::CLeaderBehavior::GetAutoAttackRange() const
{
	return leaderbeh_getautoattackrange(this);
}

inline float(__thiscall* const leaderbeh_getautoattackrangevsother)(const GGL::CLeaderBehavior* th) = reinterpret_cast<float(__thiscall*)(const GGL::CLeaderBehavior*)>(0x4EAF62);
float GGL::CLeaderBehavior::GetAutoAttackRangeVsOther() const
{
	return leaderbeh_getautoattackrangevsother(this);
}

inline float(__thiscall* const leaderbeh_getautoattackrangevsbuild)(const GGL::CLeaderBehavior* th) = reinterpret_cast<float(__thiscall*)(const GGL::CLeaderBehavior*)>(0x4EAF84);
float GGL::CLeaderBehavior::GetAutoAttackRangeVsBuildings() const
{
	return leaderbeh_getautoattackrangevsbuild(this);
}

inline float(__thiscall* const leaderbeh_getautoattackrangevsciv)(const GGL::CLeaderBehavior* th) = reinterpret_cast<float(__thiscall*)(const GGL::CLeaderBehavior*)>(0x4EAFC0);
float GGL::CLeaderBehavior::GetAutoAttackRangeVsCivillians() const
{
	return leaderbeh_getautoattackrangevsciv(this);
}

inline bool(__thiscall* const leaderbeh_hasmeleecat)(const GGL::CLeaderBehavior* th) = reinterpret_cast<bool(__thiscall*)(const GGL::CLeaderBehavior*)>(0x50AAB2);
bool GGL::CLeaderBehavior::HasMeleeCategory() const
{
	return leaderbeh_hasmeleecat(this);
}

inline int(__thiscall* const leaderbehsearchautoattacktar)(GGL::CLeaderBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CLeaderBehavior*)>(0x4EC894);
int GGL::CLeaderBehavior::SearchAutoAttackTarget()
{
	return leaderbehsearchautoattacktar(this);
}

void GGL::CLeaderBehavior::PerformRegeneration()
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	SecondsSinceHPRefresh = 0;
	int hp = e->Health;
	if (hp <= 0)
		return;
	int r = static_cast<GGL::CSettler*>(e)->LeaderGetRegenHealth();
	e->PerformHeal(r, GGL::CLeaderBehavior::LeaderRegenRegenerateSoldiers);
}

bool GGL::CLeaderBehavior::LeaderRegenRegenerateSoldiers = false;
void __thiscall GGL::CLeaderBehavior::CheckRegen()
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	int max = static_cast<GGL::CSettler*>(e)->LeaderGetRegenHealthSeconds();
	SecondsSinceHPRefresh++;
	if (SecondsSinceHPRefresh >= max)
		PerformRegeneration();
}
void __declspec(naked) leaderregensecondsasm() {
	__asm {
		mov ecx, esi;
		call GGL::CLeaderBehavior::CheckRegen;
		push 0x4EFC42;
		ret;
	}
}
bool HookLeaderRegen_Hooked = false;
void GGL::CLeaderBehavior::HookLeaderRegen()
{
	if (HookLeaderRegen_Hooked)
		return;
	HookLeaderRegen_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x4EFC29)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4EFC29), &leaderregensecondsasm, reinterpret_cast<void*>(0x4EFC42));
}

void GGL::CKegBehavior::AdvancedDealDamage()
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	{
		if (shok::EntityId id = e->GetFirstAttachedEntity(shok::AttachmentType::KEG_TARGET_BUILDING); id != shok::EntityId::Invalid) {
			if (EGL::CGLEEntity* t = EGL::CGLEEntity::GetEntityByID(id)) {
				if (GGL::CBuilding* b = dynamic_cast<GGL::CBuilding*>(t)) {
					int dmg;
					if (dynamic_cast<GGL::CBridgeEntity*>(b)) {
						dmg = b->Health;
					}
					else {
						float kegeff = static_cast<GGL::CGLBuildingProps*>(b->GetEntityType()->LogicProps)->KegEffectFactor;
						int dmgperc = static_cast<GGL::CKegBehaviorProperties*>(PropPointer)->DamagePercent;
						int mhp = b->GetMaxHealth();
						dmg = static_cast<int>(kegeff * dmgperc * mhp / 100);
					}
					b->AdvancedHurtEntityBy(e, dmg, shok::PlayerId::P0, false, false, true, shok::AdvancedDealDamageSource::AbilitySabotageSingleTarget);
				}
			}
		}
	}
	{
		if (shok::EntityId id = e->GetFirstAttachedToMe(shok::AttachmentType::DISARMING_THIEF_KEG); id != shok::EntityId::Invalid) {
			if (EGL::CGLEEntity* t = EGL::CGLEEntity::GetEntityByID(id)) {
				if (t->Position.IsInRange(e->Position, 3)) {
					t->AdvancedHurtEntityBy(e, t->Health, shok::PlayerId::P0, true, false, true, shok::AdvancedDealDamageSource::AbilitySabotageSingleTarget);
				}
			}
		}
	}
	{
		GGL::CKegBehaviorProperties* pr = static_cast<GGL::CKegBehaviorProperties*>(PropPointer);
		EGL::CGLEEntity::AdvancedDealAoEDamage(e, e->Position, pr->Radius, pr->Damage, e->PlayerId, static_cast<shok::DamageClassId>(0), true, false, true, shok::AdvancedDealDamageSource::AbilitySabotageBlast);
	}
}

void GGL::CKegBehavior::HookDealDamage()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x40, {
		   reinterpret_cast<void*>(0x4F1E77)
	   } };
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4F1E77), CppLogic::Hooks::MemberFuncPointerToVoid(&CKegBehavior::AdvancedDealDamage, 0));
}

inline EGL::CGLEAnimSet* (__thiscall* const behanimex_getanimset)(GGL::CGLBehaviorAnimationEx* th) = reinterpret_cast<EGL::CGLEAnimSet * (__thiscall*)(GGL::CGLBehaviorAnimationEx*)>(0x4F5D54);
EGL::CGLEAnimSet* GGL::CGLBehaviorAnimationEx::GetAnimSet()
{
	return behanimex_getanimset(this);
}

int GGL::CGLBehaviorAnimationEx::TaskWaitForAnimNonCancelable(EGL::CGLETaskArgsThousandths* a)
{
	int wait = a->Thousandths;
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	TurnToWaitFor = StartTurn + Duration * wait / 1000;
	e->SetTaskState(shok::TaskState::WaitForAnimNonCancelable);
	return 1;
}
void __thiscall GGL::CGLBehaviorAnimationEx::AddNonCancelableHandlers()
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	e->CreateTaskHandler<shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE>(this, &CGLBehaviorAnimationEx::TaskWaitForAnimNonCancelable);
	e->CreateStateHandler<shok::TaskState::WaitForAnimNonCancelable, EGL::CBehaviorAnimation>(this, &CGLBehaviorAnimationEx::StateWaitForAnim);
}
void __declspec(naked) anim_hooknoncancelanim_asm() {
	__asm {
		mov ecx, esi;
		call[eax + 0x48];

		mov ecx, edi;
		call GGL::CGLBehaviorAnimationEx::AddNonCancelableHandlers;

		mov ecx, [ebp + 0xC];
		push 0x588410;
		ret;
	};
}
bool HookNonCancelableAnim_Hooked = false;
void GGL::CGLBehaviorAnimationEx::HookNonCancelableAnim()
{
	if (HookNonCancelableAnim_Hooked)
		return;
	HookNonCancelableAnim_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x588408), 0x588410 - 0x588408 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x588408), &anim_hooknoncancelanim_asm, reinterpret_cast<void*>(0x588410));
}

inline GGL::CBuilding* (__thiscall* const workerbeh_gefirstattachedbuild)(GGL::CWorkerBehavior* th, shok::AttachmentType a) = reinterpret_cast<GGL::CBuilding * (__thiscall*)(GGL::CWorkerBehavior*, shok::AttachmentType)>(0x4CE720);
GGL::CBuilding* GGL::CWorkerBehavior::GetFirstAttachedBuilding(shok::AttachmentType a)
{
	return workerbeh_gefirstattachedbuild(this, a);
}

inline GGL::CBuilding* (__thiscall* const workerbeh_geworkplace)(GGL::CWorkerBehavior* th) = reinterpret_cast<GGL::CBuilding * (__thiscall*)(GGL::CWorkerBehavior*)>(0x4CEAA9);
GGL::CBuilding* GGL::CWorkerBehavior::GetWorkplace()
{
	return workerbeh_geworkplace(this);
}

inline GGL::CBuilding* (__thiscall* const workerbeh_getentered)(GGL::CWorkerBehavior* th) = reinterpret_cast<GGL::CBuilding * (__thiscall*)(GGL::CWorkerBehavior*)>(0x4CE7A1);
GGL::CBuilding* GGL::CWorkerBehavior::GetEnteredBuilding()
{
	return workerbeh_getentered(this);
}

inline bool (__thiscall* const workerbeh_isresearching)(GGL::CWorkerBehavior* th) = reinterpret_cast<bool (__thiscall*)(GGL::CWorkerBehavior*)>(0x4CFA8B);
bool GGL::CWorkerBehavior::IsResearchingSomething()
{
	return workerbeh_isresearching(this);
}

inline bool(__thiscall* const workerbeh_isbuildclosed)(GGL::CWorkerBehavior* th, shok::EntityId id) = reinterpret_cast<bool(__thiscall*)(GGL::CWorkerBehavior*, shok::EntityId)>(0x4CD865);
bool GGL::CWorkerBehavior::IsBuildingClosed(shok::EntityId bid)
{
	return workerbeh_isbuildclosed(this, bid);
}

inline void(__thiscall* const workerbeh_leaveifpossible)(GGL::CWorkerBehavior* th) = reinterpret_cast<void(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CF215);
void GGL::CWorkerBehavior::LeaveIfPossible()
{
	workerbeh_leaveifpossible(this);
}

inline bool(__thiscall* const workerbeh_checkhasworkplace)(GGL::CWorkerBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CF468);
bool GGL::CWorkerBehavior::CheckHasOpenedWorkplace()
{
	return workerbeh_checkhasworkplace(this);
}

inline void(__thiscall* const workerbeh_moticheck)(GGL::CWorkerBehavior* th, shok::WorkerReason r) = reinterpret_cast<void(__thiscall*)(GGL::CWorkerBehavior*, shok::WorkerReason)>(0x4CF317);
void GGL::CWorkerBehavior::MotivationFeedbackAndLeaveCheck(shok::WorkerReason r)
{
	workerbeh_moticheck(this, r);
}

inline bool(__thiscall* const workerbeh_isenteredbuilattacked)(GGL::CWorkerBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CEBBA);
bool GGL::CWorkerBehavior::IsEnteredBuildingRecentlyAttacked()
{
	return workerbeh_isenteredbuilattacked(this);
}

inline bool(__thiscall* const workerbeh_isovertime)(GGL::CWorkerBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CE7D3);
bool GGL::CWorkerBehavior::IsOvertimeActive()
{
	return workerbeh_isovertime(this);
}

inline void(__thiscall* const workerbeh_hide)(GGL::CWorkerBehavior* th) = reinterpret_cast<void(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CEBFC);
void GGL::CWorkerBehavior::HideInBuildingForRecentlyAttacked()
{
	workerbeh_hide(this);
}

inline bool(__thiscall* const workerbeh_noeat)(GGL::CWorkerBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CE80E);
bool GGL::CWorkerBehavior::DoesNotWantToEat()
{
	return workerbeh_noeat(this);
}

inline bool(__thiscall* const workerbeh_norest)(GGL::CWorkerBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CE83A);
bool GGL::CWorkerBehavior::DoesNotWantToRest()
{
	return workerbeh_norest(this);
}

inline bool(__thiscall* const workerbeh_canwork)(GGL::CWorkerBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CEC73);
bool GGL::CWorkerBehavior::CanWork()
{
	return workerbeh_canwork(this);
}

inline shok::TaskListId(__thiscall* const workerbeh_getworktl)(GGL::CWorkerBehavior* th) = reinterpret_cast<shok::TaskListId(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CE75A);
shok::TaskListId GGL::CWorkerBehavior::GetWorkTaskList()
{
	return workerbeh_getworktl(this);
}
inline int(__thiscall* const workerbeh_getworktimemax)(GGL::CWorkerBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CWorkerBehavior*)>(0x4CE866);
int GGL::CWorkerBehavior::GetWorktimeMax()
{
	return workerbeh_getworktimemax(this);
}
inline void(__thiscall* const workerbeh_goRestIgnoreworktime)(GGL::CWorkerBehavior* th, shok::EntityId bid) = reinterpret_cast<void(__thiscall*)(GGL::CWorkerBehavior*, shok::EntityId)>(0x4CFF56);
void GGL::CWorkerBehavior::GoRestIgnoreWorktime()
{
	workerbeh_goRestIgnoreworktime(this, EGL::CGLEEntity::GetEntityByID(EntityId)->GetFirstAttachedEntity(shok::AttachmentType::WORKER_WORKPLACE));
}

void __declspec(naked) workerbehavior_hooksupplierskipasm() {
	__asm {
		mov ecx, [ebp - 0x10];
		call GGL::CWorkerBehavior::AddSupplierSkip;

		mov ecx, [ebp - 0xC];
		pop edi;
		pop esi;
		pop ebx;

		push 0x4D168F;
		ret;
	};
}
bool HookSupplierSkip_Hooked = false;
void GGL::CWorkerBehavior::HookSupplierSkip()
{
	if (HookSupplierSkip_Hooked)
		return;
	HookSupplierSkip_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4D1689), 0x10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4D1689), &workerbehavior_hooksupplierskipasm, reinterpret_cast<void*>(0x4D168F));
}

int __thiscall GGL::CWorkerBehavior::DoWorkEvents(GGL::CBuilding* b, EGL::CGLETaskArgs* t)
{
	if (t->TaskType == shok::Task::TASK_DO_RESEARCH_STEP) {
		EGL::CEvent1Entity ev{ shok::EventIDs::University_ResearchStep, EntityId };
		b->FireEvent(&ev);
		return 0;
	}
	shok::TechnologyId tech = b->GetTechnologyInResearch();
	if (tech != shok::TechnologyId::Invalid) {
		(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId)->TechnologyStates.AddTechProgressWorker(tech, BehaviorProps2->AmountResearched);
	}
	else {
		EGL::CEvent1Entity ev{ shok::EventIDs::NoDetachEvent, EntityId };
		switch (t->TaskType) {
		case shok::Task::TASK_MINED_RESOURCE:
			ev.EventTypeId = static_cast<int>(shok::EventIDs::Mine_DoWorkStep);
			break;
		case shok::Task::TASK_REFINE_RESOURCE:
			ev.EventTypeId = static_cast<int>(shok::EventIDs::ResourceRefiner_Refine);
			break;
		case shok::Task::TASK_DO_TRADE_STEP:
			ev.EventTypeId = static_cast<int>(shok::EventIDs::Market_WorkStep);
			break;
		default:
			return 0;
		}
		b->FireEvent(&ev);
	}
	return 0;
}
void __declspec(naked) workerbeh_workeventsasm() {
	__asm {
		mov ecx, esi;
		push[ebp + 8];
		push eax;
		call GGL::CWorkerBehavior::DoWorkEvents;

		push 0x4CE70D;
		ret;
	};
}
void __thiscall GGL::CWorkerBehavior::TaskSupplyAdditional()
{
	if (ResourceTriggers) {
		float am = CarriedResourceAmount;
		auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
		auto* wp = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::WORKER_WORKPLACE));
		GGL::CEventGoodsTraded ev{ shok::EventIDs::CppLogicEvent_OnRefinerSupplyTaken, BehaviorProps2->ResourceToRefine, shok::ResourceType::None, 0.0f, EntityId, am };
		e->FireEvent(&ev);
		wp->FireEvent(&ev);
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
	}
}
void __declspec(naked) workerbeh_tasksupplyadd() {
	__asm {
		mov eax, 0x4A963D;
		call eax;

		mov ecx, esi;
		call GGL::CWorkerBehavior::TaskSupplyAdditional;

		push 0x4CFB73;
		ret;
	};
}

bool HookWorkEvents_Hooked = false;
void GGL::CWorkerBehavior::HookWorkEvents()
{
	if (HookWorkEvents_Hooked)
		return;
	HookWorkEvents_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x10, {
		reinterpret_cast<void*>(0x4CE641),
		reinterpret_cast<void*>(0x4CFB6E),
		reinterpret_cast<void*>(0x4CFAD2),
		reinterpret_cast<void*>(0x4CFB8C),
		reinterpret_cast<void*>(0x4CFC0D),
		reinterpret_cast<void*>(0x4D018A),
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4CE641), &workerbeh_workeventsasm, reinterpret_cast<void*>(0x4CE648));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4CFB6E), &workerbeh_tasksupplyadd, reinterpret_cast<void*>(0x4CFB73));
	void* over = CppLogic::Hooks::MemberFuncPointerToVoid(&CWorkerBehavior::IsResearchingOverride, 0);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4CFAD2), over); // take from stock
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4CFB8C), over); // set carry model
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4CFC0D), over); // goto supplier check
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4D018A), over); // goto supplier
}
bool GGL::CWorkerBehavior::ResourceTriggers = false;
bool GGL::CWorkerBehavior::RefinerFix = false;

int GGL::CWorkerBehavior::TaskSkipSupplierIfResearching(EGL::CGLETaskArgs* arg)
{
	if (IsResearchingSomething()) {
		auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
		auto* tl = e->GetCurrentTaskList();
		int i = 0;
		for (const auto* t : tl->Task) {
			if (t->TaskType == shok::Task::TASK_SKIP_SUPPLIER_IF_RESEARCHING_TARGET) {
				e->CurrentTaskIndex = i;
				return 0;
			}
			++i;
		}
	}
	return 0;
}

int GGL::CWorkerBehavior::TaskSkipSupplierIfResearchingTarget(EGL::CGLETaskArgs* arg)
{
	return 0;
}

int GGL::CWorkerBehavior::TaskResetCarriedResources(EGL::CGLETaskArgs* arg)
{
	CarriedResourceAmount = 0.0f;
	return 0;
}

int GGL::CWorkerBehavior::TaskCheckNeedsRes(EGL::CGLETaskArgs* arg)
{
	if (CarriedResourceAmount > 0 || IsResearchingSomething())
		return 0;
	if (WorkTimeRemaining < TargetWorkTime * 3)
		GoRestIgnoreWorktime();
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* b = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::WORKER_WORKPLACE));
	for (const auto& wt : static_cast<GGL::CGLBuildingProps*>(b->GetEntityType()->LogicProps)->WorkTaskList) {
		if (wt.Work == e->CurrentTaskListID) {
			e->SetTaskList(wt.Start);
			return 0;
		}
	}
	// fallback if for some reason the tasklist is not found
	GoRestIgnoreWorktime();
	return 0;
}

bool __thiscall GGL::CWorkerBehavior::IsResearchingOverride()
{
	if (GGL::CWorkerBehavior::RefinerFix)
		return false;
	return IsResearchingSomething();
}

void __thiscall GGL::CWorkerBehavior::AddSupplierSkip()
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	e->CreateTaskHandler<shok::Task::TASK_SKIP_SUPPLIER_IF_RESEARCHING>(this, &CWorkerBehavior::TaskSkipSupplierIfResearching);
	e->CreateTaskHandler<shok::Task::TASK_SKIP_SUPPLIER_IF_RESEARCHING_TARGET>(this, &CWorkerBehavior::TaskSkipSupplierIfResearchingTarget);
	e->CreateTaskHandler<shok::Task::TASK_REFINER_RESET_CARRIED_RESOURES>(this, &CWorkerBehavior::TaskResetCarriedResources);
	e->CreateTaskHandler<shok::Task::TASK_REFINER_CHECK_NEEDS_RESOURCES>(this, &CWorkerBehavior::TaskCheckNeedsRes);
}

static inline float(__thiscall* const autocannonBehaviorGetMaxRange)(const GGL::CAutoCannonBehavior*) = reinterpret_cast<float(__thiscall*)(const GGL::CAutoCannonBehavior*)>(0x50F508);
float GGL::CAutoCannonBehavior::GetMaxRange() const
{
	return autocannonBehaviorGetMaxRange(this);
}

int GGL::CAutoCannonBehavior::GetDamage() const
{
	EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* d = e->GetAdditionalData(false);
	if (d && d->DamageOverride >= 0)
		return d->DamageOverride;
	else
		return ACProps->DamageAmount;
}

float __thiscall GGL::CAutoCannonBehavior::GetMaxRangeBase() const
{
	const EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	const auto* d = e->GetAdditionalData();
	if (d && d->MaxRangeOverride >= 0)
		return d->MaxRangeOverride;
	else
		return ACProps->MaxAttackRange;
}

int __thiscall GGL::CAutoCannonBehavior::TaskFireProjectileOverride(EGL::CGLETaskArgs* a)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* base = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::TOP_ENTITY_FOUNDATION));
	if (base == nullptr || e->Health == 0 || base->Health == 0) {
		e->Hurt(e->Health);
		return 1;
	}
	auto* tar = EGL::CGLEEntity::GetEntityByID(e->GetFirstAttachedEntity(shok::AttachmentType::ATTACKER_TARGET));
	if (tar == nullptr)
		return 0;
	auto* p = ACProps;
	if (p->CannonBallEffectType != shok::EffectTypeId::Invalid) {
		CProjectileEffectCreator ct{};
		ct.EffectType = p->CannonBallEffectType;
		ct.PlayerID = e->PlayerId;
		ct.CurrentPos = ct.StartPos = e->Position;
		ct.TargetPos = tar->Position;
		ct.AttackerID = e->EntityId;
		ct.TargetID = tar->EntityId;

		if (!dynamic_cast<GGL::CArrowEffectProps*>((*EGL::CGLEEffectsProps::GlobalObj)->GetLogic(p->CannonBallEffectType))) {
			ct.Damage = GetDamage();
			ct.DamageRadius = p->DamageRange;
			ct.DamageClass = p->DamageClass;
			ct.SourcePlayer = e->PlayerId;
		}
		else {
			float dmg = tar->CalculateDamageAgainstMe(GetDamage(), p->DamageClass);

			if (dmg < 1)
				dmg = 1;

			ct.Damage = static_cast<int>(dmg);
		}

		(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&ct);
	}
	else {
		EGL::CGLEEntity::AdvancedDealAoEDamage(e, e->Position, p->DamageRange, GetDamage(), e->PlayerId, p->DamageClass, true, true, true, shok::AdvancedDealDamageSource::Cannonball);
		if (p->ImpactEffectType != shok::EffectTypeId::Invalid) {
			EGL::CGLEEffectCreator ct{};
			ct.EffectType = p->ImpactEffectType;
			ct.StartPos = e->Position;
			ct.PlayerID = e->PlayerId;
			(*EGL::CGLEGameLogic::GlobalObj)->CreateEffect(&ct);
		}
	}
	if (p->NumberOfShots > 0) {
		--ShotsLeft;
		if (ShotsLeft == 0) {
			e->Hurt(e->Health);
			e->EntityState = static_cast<shok::EntityStaus>(0x10003);
			if (p->SelfDestructTaskList != shok::TaskListId::Invalid) {
				e->SetTaskList(p->SelfDestructTaskList);
				return 2;
			}
			e->SetTaskState(shok::TaskState::Default);
			e->Destroy();
			return 1;
		}
	}
	return 0;
}

void __thiscall GGL::CAutoCannonBehavior::EventGetDamageOverride(EGL::CEventGetValue_Int* ev)
{
	ev->Data = GetDamage();
}

constexpr std::array autocannon_rotate_data{
	byte{0x51}, // push ecx
	byte{0x51}, // push ecx
	byte{0xDD}, byte{0x1C}, byte{0x24}, // fstp qword ptr ss:[esp],st(0)
	byte{0xE8}, byte{0x70}, byte{0x40}, byte{0x0B}, byte{0x00}, // call <settlershok.sub_5C4D2B>
	byte{0x59}, // pop ecx
	byte{0x59}, // pop ecx
	byte{0xD8}, byte{0x1D}, byte{0xEC}, byte{0x8B}, byte{0x77}, byte{0x00}, // fcomp st(0),dword ptr ds:[778BEC]
	byte{0xDF}, byte{0xE0}, // fnstsw ax
	byte{0xF6}, byte{0xC4}, byte{0x05}, // test ah,5
	byte{0x7B}, byte{0x33}, // jnp settlershok.510CFD
	byte{0xEB}, byte{0x21} // jmp settlershok.510CED
};

bool AutoCannonHookDamageMod_Hooked = false;
void GGL::CAutoCannonBehavior::HookDamageOverride()
{
	if (AutoCannonHookDamageMod_Hooked)
		return;
	AutoCannonHookDamageMod_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x510CED - 0x510CB1, {
		reinterpret_cast<void*>(0x510638),
		reinterpret_cast<void*>(0x50F5ED),
		reinterpret_cast<void*>(0x510CB1)
	} };
	// get damage event
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50F5ED), CppLogic::Hooks::MemberFuncPointerToVoid(&EventGetDamageOverride, 0), reinterpret_cast<void*>(0x50F5F3));
	// fire projectile (also used by trap)
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x510638), CppLogic::Hooks::MemberFuncPointerToVoid(&TaskFireProjectileOverride, 0), reinterpret_cast<void*>(0x510647));
	*reinterpret_cast<std::remove_const_t<decltype(autocannon_rotate_data)>*>(0x510CB1) = autocannon_rotate_data;
	CppLogic::Hooks::WriteNops(reinterpret_cast<void*>(0x510CCC), reinterpret_cast<void*>(0x510CED));
}


void __declspec(naked) autocannonmaxrangeasm() {
	__asm {
		mov esi, ecx;

		pushad;
		call GGL::CAutoCannonBehavior::GetMaxRangeBase;
		popad;

		mov eax, [esi + 0x14];
		push 0x50F515;
		ret;
	}
}
bool ACHookMaxRange_Hooked = false;
void GGL::CAutoCannonBehavior::HookRangeOverride()
{
	if (ACHookMaxRange_Hooked)
		return;
	ACHookMaxRange_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x50F50D)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50F50D), &autocannonmaxrangeasm, reinterpret_cast<void*>(0x50F515));
}

void GGL::CSerfBehavior::TaskExtractAdditional(int am, GGL::CResourceDoodad* d)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	if (GGL::CWorkerBehavior::ResourceTriggers) {
		GGL::CEventGoodsTraded ev3{ shok::EventIDs::CppLogicEvent_OnResourceMined, shok::ResourceType::None, d->ResourceType, static_cast<float>(am), EntityId, 0.0f };
		e->FireEvent(&ev3);
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev3);
	}
	auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId);
	auto rt = shok::CostInfo::RawToResourceType(d->ResourceType);
	if (rt != shok::ResourceType::None)
		pl->Statistics.OnResMined(rt, static_cast<float>(am));
}
void __declspec(naked) serfbeh_extractaddasm() {
	__asm {
		push ebx; // orig
		push edi; // add resdoodad
		mov ebx, [eax]; // orig
		push ebx; // add amount
		mov[ebp - 0x1c], ebx; // orig
		mov ecx, esi; // add this
		call GGL::CSerfBehavior::TaskExtractAdditional; // add

		mov ecx, esi; // orig

		push 0x4DEAEA;
		ret;
	};
}

bool HookMineTriggerSerf_Hooked = false;
void GGL::CSerfBehavior::HookMineTrigger()
{
	if (HookMineTriggerSerf_Hooked)
		return;
	HookMineTriggerSerf_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x4DEAE2)
	} };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4DEAE2), &serfbeh_extractaddasm, reinterpret_cast<void*>(0x4DEAE7));
}

inline void(__thiscall* const formationbeh_getfrompos)(GGL::CFormationBehavior* th, shok::Position* p, EGL::CGLEEntity* lead) = reinterpret_cast<void(__thiscall*)(GGL::CFormationBehavior*, shok::Position*, EGL::CGLEEntity*)>(0x4F7962);
void GGL::CFormationBehavior::GetFormationPosition(EGL::CGLEEntity* leader, shok::Position* out)
{
	formationbeh_getfrompos(this, out, leader);
}
shok::Position GGL::CFormationBehavior::GetFormationPosition()
{
	shok::Position p;
	shok::EntityId leaderid = EGL::CGLEEntity::GetEntityByID(EntityId)->GetFirstAttachedEntity(shok::AttachmentType::LEADER_SOLDIER);
	auto* lead = EGL::CGLEEntity::GetEntityByID(leaderid);
	GetFormationPosition(lead, &p);
	return p;
}

inline void(__thiscall* const formationbeh_addhandlers)(GGL::CFormationBehavior* th, shok::EntityId id) = reinterpret_cast<void(__thiscall*)(GGL::CFormationBehavior*, shok::EntityId)>(0x4F8C51);
void __thiscall GGL::CFormationBehavior::AddHandlers(shok::EntityId id)
{
	formationbeh_addhandlers(this, id);
}
void __thiscall GGL::CFormationBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}
void __thiscall GGL::CFormationBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}
inline void(__thiscall* const formationbeh_onupgrade)(GGL::CFormationBehavior* th, EGL::CGLEEntity* o) = reinterpret_cast<void(__thiscall*)(GGL::CFormationBehavior*, EGL::CGLEEntity*)>(0x4F71A9);
void __thiscall GGL::CFormationBehavior::OnEntityUpgrade(EGL::CGLEEntity* old)
{
	formationbeh_onupgrade(this, old);
}

inline int(__thiscall* const formationbeh_taskassumepos)(GGL::CFormationBehavior* th, EGL::CGLETaskArgs* t) = reinterpret_cast<int(__thiscall*)(GGL::CFormationBehavior*, EGL::CGLETaskArgs*)>(0x4F8780);
int GGL::CFormationBehavior::TaskAssumePositionInForation(EGL::CGLETaskArgs* t)
{
	return formationbeh_taskassumepos(this, t);
}
inline int(__thiscall* const formationbeh_idleinform)(GGL::CFormationBehavior* th, EGL::CGLETaskArgs* t) = reinterpret_cast<int(__thiscall*)(GGL::CFormationBehavior*, EGL::CGLETaskArgs*)>(0x4F7152);
int GGL::CFormationBehavior::TaskIdleInFormation(EGL::CGLETaskArgs* t)
{
	return formationbeh_idleinform(this, t);
}
inline void(__thiscall* const formationbeh_eventassumepos)(GGL::CFormationBehavior* th, BB::CEvent* ev) = reinterpret_cast<void(__thiscall*)(GGL::CFormationBehavior*, BB::CEvent*)>(0x4F7176);
void GGL::CFormationBehavior::EventAssumePositionInFormation(BB::CEvent* ev)
{
	formationbeh_eventassumepos(this, ev);
}
inline void(__thiscall* const formationbeh_eventgetposinform)(GGL::CFormationBehavior* th, EGL::CEventGetPosition* ev) = reinterpret_cast<void(__thiscall*)(GGL::CFormationBehavior*, EGL::CEventGetPosition*)>(0x4F884B);
void GGL::CFormationBehavior::EventGetPositionInFormation(EGL::CEventGetPosition* ev)
{
	formationbeh_eventgetposinform(this, ev);
}
inline shok::TaskStateExecutionResult(__thiscall* const formationbeh_stateidleinform)(GGL::CFormationBehavior* th, int u) = reinterpret_cast<shok::TaskStateExecutionResult(__thiscall*)(GGL::CFormationBehavior*, int)>(0x4F8898);
shok::TaskStateExecutionResult GGL::CFormationBehavior::StateIdleInFormation(int u)
{
	return formationbeh_stateidleinform(this, u);
}
inline shok::TaskStateExecutionResult(__thiscall* const formationbeh_stateassumepos)(GGL::CFormationBehavior* th, int u) = reinterpret_cast<shok::TaskStateExecutionResult(__thiscall*)(GGL::CFormationBehavior*, int)>(0x4F8957);
shok::TaskStateExecutionResult GGL::CFormationBehavior::StateAssumePositionInFormation(int u)
{
	return formationbeh_stateassumepos(this, u);
}

shok::Position* __thiscall GGL::CFormationBehavior::GetPosOverride(shok::Position* p, EGL::CGLEEntity* leader)
{
	auto* i = CastToIdentifier<CppLogic::Mod::IFormationBehaviorExtProvider>();
	if (i == nullptr)
		GetFormationPosition(leader, p);
	else
		*p = i->GetPosExt(leader);
	return p;
}
bool Form_HookGetPosExtIHooked = false;
void GGL::CFormationBehavior::HookGetPosExtI()
{
	if (Form_HookGetPosExtIHooked)
		return;
	Form_HookGetPosExtIHooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x10, {
		reinterpret_cast<void*>(0x4F8818),
		reinterpret_cast<void*>(0x4F8880),
		reinterpret_cast<void*>(0x4F892D),
		reinterpret_cast<void*>(0x4F89D1),
	} };
	void* tocall = CppLogic::Hooks::MemberFuncPointerToVoid(&CFormationBehavior::GetPosOverride, 0);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4F8818), tocall);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4F8880), tocall);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4F892D), tocall);
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4F89D1), tocall);
}

void GGL::CHeroBehavior::Resurrect()
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	ResurrectionTimePassed = 0;
	e->SetHealth(static_cast<int>(e->GetMaxHealth() * (*GGL::CLogicProperties::GlobalObj)->HeroResurrectionHealthFactor));
	for (auto at : { shok::AttachmentType::ATTACKER_TARGET, shok::AttachmentType::ATTACKER_COMMAND_TARGET }) {
		while (true) {
			auto t = e->GetFirstAttachedEntity(at);
			if (t == shok::EntityId::Invalid)
				break;
			e->DetachObservedEntity(at, t, false);
		}
	}
	e->EntityState = static_cast<shok::EntityStaus>(0x10000);
	BB::CEvent ev{ shok::EventIDs::HeroAbility_StandUp };
	e->FireEvent(&ev);
	e->CurrentState = shok::TaskState::Default;
}

bool GGL::CHeroBehavior::AdvanceProgress(int d)
{
	ResurrectionTimePassed += d;
	if (ResurrectionTimePassed >= (*GGL::CLogicProperties::GlobalObj)->HeroResurrectionTime) {
		Resurrect();
		return true;
	}
	return false;
}

static inline GGL::CPositionAtResourceFinder* (__cdecl* const shok_GGL_CPositionAtResourceFinder_greatebyent)(shok::EntityId id) = reinterpret_cast<GGL::CPositionAtResourceFinder * (__cdecl*)(shok::EntityId)>(0x4CB1C1);
GGL::CPositionAtResourceFinder* GGL::CPositionAtResourceFinder::CreateByEntity(shok::EntityId entityid)
{
	return shok_GGL_CPositionAtResourceFinder_greatebyent(entityid);
}
static inline void(__thiscall* const shok_GGL_CPositionAtResourceFinder_getposbyfloat)(GGL::CPositionAtResourceFinder* th, shok::Position* p, float f) = reinterpret_cast<void(__thiscall*)(GGL::CPositionAtResourceFinder*, shok::Position*, float)>(0x4CA89E);
shok::Position GGL::CPositionAtResourceFinder::CalcPositionFromFloat(float f)
{
	shok::Position p;
	//reinterpret_cast<shok_vtable_shok_GGL_CPositionAtResourceFinder*>(vtable)->GetPosition(this, &p, f);
	shok_GGL_CPositionAtResourceFinder_getposbyfloat(this, &p, f);
	return p;
}

inline float(__thiscall* const resrefiner_getam)(GGL::CResourceRefinerBehavior* th) = reinterpret_cast<float(__thiscall*)(GGL::CResourceRefinerBehavior*)>(0x4E1201);
float GGL::CResourceRefinerBehavior::GetRefineAmount()
{
	return resrefiner_getam(this);
}
inline shok::ResourceType(__thiscall* const resrefiner_getrt)(GGL::CResourceRefinerBehavior* th) = reinterpret_cast<shok::ResourceType(__thiscall*)(GGL::CResourceRefinerBehavior*)>(0x4E1035);
shok::ResourceType GGL::CResourceRefinerBehavior::GetResource()
{
	return resrefiner_getrt(this);
}
inline shok::ResourceType(__thiscall* const resrefiner_getrrt)(GGL::CResourceRefinerBehavior* th) = reinterpret_cast<shok::ResourceType(__thiscall*)(GGL::CResourceRefinerBehavior*)>(0x4E1052);
shok::ResourceType GGL::CResourceRefinerBehavior::GetRawResource()
{
	return resrefiner_getrrt(this);
}

void __thiscall GGL::CResourceRefinerBehavior::EventRefineOverride(BB::CEvent* ev)
{
	auto* e = EGL::CGLEEntity::GetEntityByID(EntityId);
	auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId);
	auto raw = GetRawResource();
	auto* ev2 = BB::IdentifierCast<EGL::CEvent1Entity>(ev);
	bool allow;
	if (GGL::CWorkerBehavior::RefinerFix && ev2)
		allow = EGL::CGLEEntity::GetEntityByID(ev2->EntityID)->GetBehavior<GGL::CWorkerBehavior>()->CarriedResourceAmount > 0;
	else
		allow = pl->CurrentResources.GetResourceAmountFromType(raw, false) > 0;

	if (allow) {
		float am = GetRefineAmount();
		auto rt = GetResource();

		if (GGL::CWorkerBehavior::ResourceTriggers && ev2) {
			GGL::CEventGoodsTraded ev3{ shok::EventIDs::CppLogicEvent_OnResourceRefined, raw, rt, am, ev2->EntityID, 0 };
			EGL::CGLEEntity::GetEntityByID(ev2->EntityID)->FireEvent(&ev3);
			e->FireEvent(&ev3);
			(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev3);
			am = ev3.BuyAmount;
		}

		pl->CurrentResources.AddToType(rt, am);
		{
			luaext::EState L{ *EScr::CScriptTriggerSystem::GameState };
			int t = L.GetTop();
			L.GetGlobal("GameCallback_GainedResources");
			L.Push(e->PlayerId);
			L.Push(static_cast<int>(rt));
			L.Push(am);
			EScr::CScriptTriggerSystem::HandleLuaError(L.GetState(), L.PCall(3, 0), "GameCallback_GainedResources");
			L.SetTop(t);
		}

		pl->Statistics.OnResRefined(rt, am);
		GGL::CFeedbackEventResource ev4{ shok::FeedbackEventIds::FEEDBACK_EVENT_RESOURCE_MINED, e->PlayerId, e->EntityId, rt, am };
		EGUIX::FeedbackEventHandler::GlobalObj()->FireEvent(&ev4);
	}
}

bool HookRefineTrigger_Hooked = false;
void GGL::CResourceRefinerBehavior::HookRefineTrigger()
{
	if (HookRefineTrigger_Hooked)
		return;
	HookRefineTrigger_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4E128B), 0x10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4E128B), CppLogic::Hooks::MemberFuncPointerToVoid(&CResourceRefinerBehavior::EventRefineOverride, 0), reinterpret_cast<void*>(0x4E1295));
}

static inline shok::EntityId(__thiscall* const raxbeh_buyleaderandattach)(GGL::CBarrackBehavior* th, shok::EntityTypeId ety) = reinterpret_cast<shok::EntityId(__thiscall*)(GGL::CBarrackBehavior*, shok::EntityTypeId)>(0x50EA18);
static inline int(__thiscall* const raxbeh_gettrainingtl)(GGL::CBarrackBehavior* th) = reinterpret_cast<int(__thiscall*)(GGL::CBarrackBehavior*)>(0x50EBCE);
shok::EntityId GGL::CBarrackBehavior::BuyLeaderByType(shok::EntityTypeId ety)
{
	shok::EntityId id = raxbeh_buyleaderandattach(this, ety);
	if (id != shok::EntityId::Invalid) {
		EGL::CEventValue_Int ev = { shok::EventIDs::Leader_SetTrainingTL, raxbeh_gettrainingtl(this) };
		EGL::CGLEEntity::GetEntityByID(id)->FireEvent(&ev);
	}
	return id;
}

bool HookBuyTriggers_Hooked = false;
void GGL::CBarrackBehavior::HookBuyTriggers()
{
	if (HookBuyTriggers_Hooked)
		return;
	HookBuyTriggers_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x50F020 - 0x50F016, { reinterpret_cast<void*>(0x50F016), reinterpret_cast<void*>(0x50EE4D) } };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50F016), CppLogic::Hooks::MemberFuncPointerToVoid(&CBarrackBehavior::EventBuyLeaderOverride, 0), reinterpret_cast<void*>(0x50F020));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50EE4D), CppLogic::Hooks::MemberFuncPointerToVoid(&CBarrackBehavior::EventBuySoldierOverride, 0), reinterpret_cast<void*>(0x50EE57));
}
void GGL::CBarrackBehavior::EventBuyLeaderOverride(EGL::CEventValue_Int* ev)
{
	auto* b = static_cast<GGL::CBuilding*>(EGL::CGLEEntity::GetEntityByID(EntityId));
	if (!b->IsIdle(true, true))
		return;
	auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	auto etyid = pl->SettlerUpgradeManager->GetTypeByUCat(static_cast<shok::UpgradeCategoryId>(ev->Data));
	auto* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(etyid);
	CppLogic::Events::CanBuySettlerEvent cev{ shok::EventIDs::CppLogicEvent_CanBuySettler, EntityId, shok::EntityId::Invalid, etyid,
		pl->PlayerAttractionHandler->AIPlayerFlag || (pl->PlayerAttractionHandler->GetAttractionUsage() < pl->PlayerAttractionHandler->GetAttractionLimit()),
		pl->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(ety->LogicProps)->Cost),
		!pl->PlayerAttractionHandler->IsMotivationLocked(), !b->WorkerAlarmModeActive };
	(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&cev);
	if (cev.Cost && cev.VCPop && cev.Alarm && cev.Motivation)
		BuyLeaderByType(etyid);
}
void GGL::CBarrackBehavior::EventBuySoldierOverride(EGL::CEvent1Entity* ev)
{
	auto* b = static_cast<GGL::CBuilding*>(EGL::CGLEEntity::GetEntityByID(EntityId));
	if (!b->IsConstructionFinished())
		return;
	if (b->IsUpgrading)
		return;
	if (EGL::CGLEEntity::EntityIDIsDead(ev->EntityID))
		return;
	EGL::CGLEEntity* leader = EGL::CGLEEntity::GetEntityByID(ev->EntityID);
	EGL::CEventGetValue_Int getcurr{ shok::EventIDs::Leader_GetCurrentNumSoldier };
	leader->FireEvent(&getcurr);
	EGL::CEventGetValue_Int getmax{ shok::EventIDs::Leader_GetMaxNumSoldier };
	leader->FireEvent(&getmax);
	if (getcurr.Data >= getmax.Data)
		return;
	EGL::CEventGetValue_Int getsolty{ shok::EventIDs::Leader_GetSoldierType };
	leader->FireEvent(&getsolty);
	auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	auto etyid = static_cast<shok::EntityTypeId>(getsolty.Data);
	auto* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(etyid);
	CppLogic::Events::CanBuySettlerEvent cev{ shok::EventIDs::CppLogicEvent_CanBuySettler, EntityId, ev->EntityID, etyid,
		pl->PlayerAttractionHandler->AIPlayerFlag || (pl->PlayerAttractionHandler->GetAttractionUsage() < pl->PlayerAttractionHandler->GetAttractionLimit()),
		pl->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(ety->LogicProps)->Cost),
		!pl->PlayerAttractionHandler->IsMotivationLocked(), !b->WorkerAlarmModeActive };
	(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&cev);
	if (cev.Cost && cev.VCPop && cev.Alarm && cev.Motivation)
	{
		shok::EntityId id = raxbeh_buyleaderandattach(this, etyid);
		if (id != shok::EntityId::Invalid) {
			EGL::CEvent1Entity ev = { shok::EventIDs::Leader_AttachSoldier, id };
			leader->FireEvent(&ev);
			auto sol = EGL::CGLEEntity::GetEntityByID(id);
			sol->AttachEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING, EntityId, shok::EventIDs::NoDetachEvent, shok::EventIDs::NoDetachEvent);
			static_cast<GGL::CSettler*>(sol)->Vanish();
			sol->SetTaskList(CppLogic::GetIdManager<shok::TaskListId>().GetIdByName("TL_SOLDIER_LEAVE_BARRACKS"));
		}
	}
}

inline EGL::CGLEEntity* (__thiscall*const defbuild_gettar)(GGL::CDefendableBuildingBehavior* th, const shok::Position* p) = reinterpret_cast<EGL::CGLEEntity * (__thiscall*)(GGL::CDefendableBuildingBehavior*, const shok::Position*)>(0x4FC22F);
EGL::CGLEEntity* GGL::CDefendableBuildingBehavior::GetAttackTarget(const shok::Position& pos)
{
	return defbuild_gettar(this, &pos);
}
inline bool(__thiscall* const defbuild_rollmiss)(GGL::CDefendableBuildingBehavior* th) = reinterpret_cast<bool(__thiscall*)(GGL::CDefendableBuildingBehavior*)>(0x4FBD49);
bool GGL::CDefendableBuildingBehavior::RollAttackMiss()
{
	return defbuild_rollmiss(this);;
}
inline int(__thiscall* const defbuild_calcdmg)(GGL::CDefendableBuildingBehavior* th, EGL::CGLEEntity* tar) = reinterpret_cast<int(__thiscall*)(GGL::CDefendableBuildingBehavior*, EGL::CGLEEntity*)>(0x4FC13B);
int GGL::CDefendableBuildingBehavior::CalcDamageAgainst(EGL::CGLEEntity* tar)
{
	return defbuild_calcdmg(this, tar);;
}
inline void(__thiscall* const defbuild_fire)(GGL::CDefendableBuildingBehavior* th) = reinterpret_cast<void(__thiscall*)(GGL::CDefendableBuildingBehavior*)>(0x4FC37F);
void GGL::CDefendableBuildingBehavior::FireAtNearestTarget()
{
	defbuild_fire(this);
}

static_assert(- 0x5C + static_cast<int>(offsetof(CProjectileEffectCreator, AdvancedDamageSourceOverride))==-21);
void __declspec(naked) defendbuildingbeh_dealdmgsrcasm() {
	__asm {
		mov byte ptr[ebp -21], 4;

		mov ecx, 0x895DAC;
		mov ecx, [ecx];

		push 0x4FC412;
		ret;
	};
}
bool HookDealDamageSource_Hooked = false;
void GGL::CDefendableBuildingBehavior::HookDealDamageSource()
{
	if (HookDealDamageSource_Hooked)
		return;
	HookDealDamageSource_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4FC40C), 0x10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4FC40C), &defendbuildingbeh_dealdmgsrcasm, reinterpret_cast<void*>(0x4FC412));
}

inline GGL::CResourceDoodad* (__thiscall* const minebeh_getresdoodad)(GGL::CMineBehavior* th) = reinterpret_cast<GGL::CResourceDoodad * (__thiscall*)(GGL::CMineBehavior*)>(0x4E68E9);
GGL::CResourceDoodad* GGL::CMineBehavior::GetResDoodad()
{
	return minebeh_getresdoodad(this);
}

int __thiscall GGL::CMineBehavior::TaskMineAdd(int* am, GGL::CResourceDoodad* d, BB::CEvent* ev)
{
	if (d->ResourceAmount < *am)
		*am = d->ResourceAmount;
	auto* ev2 = BB::IdentifierCast<EGL::CEvent1Entity>(ev);
	if (GGL::CWorkerBehavior::ResourceTriggers && ev2 && d->ResourceAmount > 0) {
		GGL::CEventGoodsTraded ev3{ shok::EventIDs::CppLogicEvent_OnResourceMined, shok::ResourceType::None, d->ResourceType, static_cast<float>(*am), ev2->EntityID, 0.0f};
		EGL::CGLEEntity::GetEntityByID(ev2->EntityID)->FireEvent(&ev3);
		EGL::CGLEEntity::GetEntityByID(EntityId)->FireEvent(&ev3);
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev3);
	}
	return d->ResourceAmount;
}
void __declspec(naked) minebeh_taskmineaddasm() {
	__asm {
		mov[ebp - 0x14], eax;

		push[ebp + 8];
		push eax;
		lea eax, [ebp - 0x18];
		push eax;
		mov ecx, esi;
		call GGL::CMineBehavior::TaskMineAdd;
		cmp eax, 0;
		je skip;

		mov ecx, [ebp - 0x14];

		push 0x4E6966;
		ret;

	skip:
		push 0x4E6B24;
		ret;
	};
}

bool HookMineTrigger_Hooked = false;
void GGL::CMineBehavior::HookMineTrigger()
{
	if (HookMineTrigger_Hooked)
		return;
	HookMineTrigger_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4E6961), 0x10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4E6961), &minebeh_taskmineaddasm, reinterpret_cast<void*>(0x4E6966));
}

inline int(__thiscall* const keepheb_maxserfs)(const GGL::CKeepBehavior* th) = reinterpret_cast<int(__thiscall*)(const GGL::CKeepBehavior*)>(0x4F23D2);
int GGL::CKeepBehavior::GetMaxNumberOfSerfs() const
{
	return keepheb_maxserfs(this);
}

inline GGL::CSettler*(__thiscall* const keepheb_createserf)(GGL::CKeepBehavior* th) = reinterpret_cast<GGL::CSettler * (__thiscall*)(GGL::CKeepBehavior*)>(0x4F24A0);
void __thiscall GGL::CKeepBehavior::EventBuySerfOverride(BB::CEvent* ev)
{
	auto* b = static_cast<GGL::CBuilding*>(EGL::CGLEEntity::GetEntityByID(EntityId));
	if (!b->IsIdle(false, true))
		return;
	auto* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(b->PlayerId);
	auto etyid = CppLogic::GetIdManager<shok::EntityTypeId>().GetIdByName("PU_Serf");
	auto* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(etyid);
	CppLogic::Events::CanBuySettlerEvent cev{ shok::EventIDs::CppLogicEvent_CanBuySettler, EntityId, shok::EntityId::Invalid, etyid,
		pl->PlayerAttractionHandler->AIPlayerFlag || (pl->PlayerAttractionHandler->GetAttractionUsage() < pl->PlayerAttractionHandler->GetAttractionLimit()),
		pl->CurrentResources.HasResources(&static_cast<GGL::CGLSettlerProps*>(ety->LogicProps)->Cost),
		!pl->PlayerAttractionHandler->IsMotivationLocked(), !b->WorkerAlarmModeActive,
		pl->PlayerAttractionHandler->SerfArray.size() < static_cast<size_t>(GetMaxNumberOfSerfs())};
	(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&cev);
	if (cev.Cost && cev.VCPop && cev.Alarm && cev.Motivation && cev.HQPop)
	{
		auto* serf = keepheb_createserf(this);
		if (serf != nullptr) {
			serf->Vanish();
			serf->AttachEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING, EntityId, shok::EventIDs::NoDetachEvent, shok::EventIDs::NoDetachEvent);
			serf->SetTaskList(CppLogic::GetIdManager<shok::TaskListId>().GetIdByName("TL_LEAVE_KEEP"));
			pl->CurrentResources.SubResources(static_cast<GGL::CGLSettlerProps*>(ety->LogicProps)->Cost);
		}
	}
}

bool HookBuySerf_Hooked = false;
void GGL::CKeepBehavior::HookBuySerf()
{
	if (HookBuySerf_Hooked)
		return;
	HookBuySerf_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4F2733), 0x4F273C- 0x4F2733 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4F2733), CppLogic::Hooks::MemberFuncPointerToVoid(&CKeepBehavior::EventBuySerfOverride, 0), reinterpret_cast<void*>(0x4F273C));
}

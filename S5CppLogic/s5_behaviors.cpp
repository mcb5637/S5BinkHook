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
#include "hooks.h"
#include "EntityAddonData.h"

void EGL::CGLEBehavior::unknownFuncBeh1(EGL::CGLEEntity* e)
{
}
void EGL::CGLEBehavior::OnEntityDestroy(bool uk)
{
}

BB::SerializationData* EGL::CGLEBehavior::SerializationData = reinterpret_cast<BB::SerializationData*>(0x86A828);


inline shok::TaskStateExecutionResult(__thiscall* const behanim_statehandlerwait)(EGL::CBehaviorAnimation* th, int i) = reinterpret_cast<shok::TaskStateExecutionResult(__thiscall*)(EGL::CBehaviorAnimation*, int)>(0x587E20);
shok::TaskStateExecutionResult EGL::CBehaviorAnimation::StateWaitForAnim(int i)
{
	return behanim_statehandlerwait(this, i);
}

static inline void(__thiscall* const heroability_addhandlers)(GGL::CHeroAbility* th, int id) = reinterpret_cast<void(__thiscall*)(GGL::CHeroAbility*, int)>(0x4F4982);
void GGL::CHeroAbility::AddHandlers(int id)
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
			if (ecr.EffectType) {
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
	int tid = e->GetFirstAttachedEntity(shok::AttachmentType::CONVERTER_SETTLER);
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

static inline void(__thiscall* const summonbeh_addhandl)(GGL::CSummonBehavior* th, int id) = reinterpret_cast<void(__thiscall*)(GGL::CSummonBehavior*, int)>(0x4D6FBE);
void GGL::CSummonBehavior::AddHandlers(int id)
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

static inline bool(__thiscall* const summonbeh_isab)(GGL::CSummonBehavior* th, int ab) = reinterpret_cast<bool(__thiscall*)(GGL::CSummonBehavior*, int)>(0x4D6A64);
bool GGL::CSummonBehavior::IsAbility(int ability)
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
	GGL::CBattleBehaviorProps* bpr = thent->GetEntityType()->GetBehaviorProps< GGL::CBattleBehaviorProps>();
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
	cr.DamageClass = 0;
	cr.SourcePlayer = thent->PlayerId;
	cr.AdvancedDamageSourceOverride = static_cast<byte>(shok::AdvancedDealDamageSource::AbilitySnipe);
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
	base = ModifierEntityDatabase::GlobalObj->GetModifiedStat(EntityId, GGL::CEntityProfile::ModifierType::Damage, base);
	float fact = 1.0f;
	for (const auto& af : e->ObserverEntities.ForKeys(shok::AttachmentType::HERO_AFFECTED)) {
		auto* h = EGL::CGLEEntity::GetEntityByID(af.second.EntityId);
		EGL::CEventGetValue_Float ev{ shok::EventIDs::RangedEffect_GetDamageFactor };
		h->FireEvent(&ev);
		fact += ev.Data - 1.0f;
	}
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
		EGL::CEvent1Entity ev{ shok::EventIDs::Leader_OnAttackTargetDetached, 0 };
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

	if (!dynamic_cast<GGL::CArrowEffectProps*>((*EGL::CGLEEffectsProps::GlobalObj)->EffectTypes[BattleProps->ProjectileEffectID])) {
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
		if (int id = e->GetFirstAttachedEntity(shok::AttachmentType::KEG_TARGET_BUILDING)) {
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
					b->AdvancedHurtEntityBy(e, dmg, 0, false, false, true, shok::AdvancedDealDamageSource::AbilitySabotageSingleTarget);
				}
			}
		}
	}
	{
		if (int id = e->GetFirstAttachedToMe(shok::AttachmentType::DISARMING_THIEF_KEG)) {
			if (EGL::CGLEEntity* t = EGL::CGLEEntity::GetEntityByID(id)) {
				if (t->Position.IsInRange(e->Position, 3)) {
					t->AdvancedHurtEntityBy(e, t->Health, 0, true, false, true, shok::AdvancedDealDamageSource::AbilitySabotageSingleTarget);
				}
			}
		}
	}
	{
		GGL::CKegBehaviorProperties* pr = static_cast<GGL::CKegBehaviorProperties*>(PropPointer);
		EGL::CGLEEntity::AdvancedDealAoEDamage(e, e->Position, pr->Radius, pr->Damage, e->PlayerId, 0, true, false, true, shok::AdvancedDealDamageSource::AbilitySabotageBlast);
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
	if (p->CannonBallEffectType) {
		CProjectileEffectCreator ct{};
		ct.EffectType = p->CannonBallEffectType;
		ct.PlayerID = e->PlayerId;
		ct.CurrentPos = ct.StartPos = e->Position;
		ct.TargetPos = tar->Position;
		ct.AttackerID = e->EntityId;
		ct.TargetID = tar->EntityId;

		if (!dynamic_cast<GGL::CArrowEffectProps*>((*EGL::CGLEEffectsProps::GlobalObj)->EffectTypes[p->CannonBallEffectType])) {
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
		if (p->ImpactEffectType != 0) {
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
			e->EntityState = 0x10003;
			if (p->SelfDestructTaskList != 0) {
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

bool AutoCannonHookDamageMod_Hooked = false;
void GGL::CAutoCannonBehavior::HookDamageOverride()
{
	if (AutoCannonHookDamageMod_Hooked)
		return;
	AutoCannonHookDamageMod_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
		reinterpret_cast<void*>(0x510638),
		reinterpret_cast<void*>(0x50F5ED),
	} };
	// get damage event
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x50F5ED), CppLogic::Hooks::MemberFuncPointerToVoid(&EventGetDamageOverride, 0), reinterpret_cast<void*>(0x50F5F3));
	// fire projectile (also used by trap)
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x510638), CppLogic::Hooks::MemberFuncPointerToVoid(&TaskFireProjectileOverride, 0), reinterpret_cast<void*>(0x510647));
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

static inline GGL::CPositionAtResourceFinder* (__cdecl* const shok_GGL_CPositionAtResourceFinder_greatebyent)(int id) = reinterpret_cast<GGL::CPositionAtResourceFinder * (__cdecl*)(int)>(0x4CB1C1);
GGL::CPositionAtResourceFinder* GGL::CPositionAtResourceFinder::CreateByEntity(int entityid)
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

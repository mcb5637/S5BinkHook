#include "pch.h"
#include "s5_behaviors.h"
#include "s5_entity.h"
#include "s5_entitytype.h"
#include "s5_behaviorProps.h"
#include "s5_effects.h"
#include "s5_maplogic.h"
#include "s5_defines.h"
#include "hooks.h"

void EGL::CGLEBehavior::unknownFuncBeh1(EGL::CGLEEntity* e)
{
}
void EGL::CGLEBehavior::unknownFuncBeh2(int uk)
{
}

BB::SerializationData* EGL::CGLEBehavior::SerializationData = reinterpret_cast<BB::SerializationData*>(0x86A828);


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
void GGL::CBombPlacerBehavior::FixBombAttachment()
{

	if (FixBombAttachment_Hooked)
		return;
	FixBombAttachment_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x5062C6), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x5062C6), &bombattachment_fix);
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
int __fastcall sniperability_tasksnipeoverride(GGL::CSniperAbility* thi, int _, int taskargs) {
	EGL::CGLEEntity* thent = EGL::CGLEEntity::GetEntityByID(thi->EntityId);
	EGL::CGLEEntity* tar = EGL::CGLEEntity::GetEntityByID(thi->TargetId);
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
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4DB5B8), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4DB5B8), &sniperability_tasksnipeoverride);
}

static inline float(__thiscall* const battleBehaviorGetMaxRange)(GGL::CBattleBehavior*) = reinterpret_cast<float(__thiscall*)(GGL::CBattleBehavior*)>(0x50AB43);
float GGL::CBattleBehavior::GetMaxRange()
{
	return battleBehaviorGetMaxRange(this);
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

static inline float(__thiscall* const autocannonBehaviorGetMaxRange)(GGL::CAutoCannonBehavior*) = reinterpret_cast<float(__thiscall*)(GGL::CAutoCannonBehavior*)>(0x50F508);
float GGL::CAutoCannonBehavior::GetMaxRange()
{
	return autocannonBehaviorGetMaxRange(this);
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

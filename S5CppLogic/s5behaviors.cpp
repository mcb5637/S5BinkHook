#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGL_CGLEBehavior : shok_vtable_BB_IObject {
	int(__thiscall* AddHandlers)(shok_EGL_CGLEBehavior*, int);
	int(__thiscall* OnEntityCreate)(shok_EGL_CGLEBehavior* th, shok_EGL_CGLEBehaviorProps* p);
	int(__thiscall* OnEntityLoad)(shok_EGL_CGLEBehavior* th, shok_EGL_CGLEBehaviorProps* p);
	PADDINGI(2); // nullsubs 0x52B509
};

struct shok_vtable_GGL_CBehaviorDefaultMovement : shok_vtable_EGL_CGLEBehavior {
	PADDINGI(1);
	float(__thiscall* GetSpeed)(shok_GGL_CBehaviorDefaultMovement* m); // 9
};

struct shok_vtable_GGL_CBattleBehavior : shok_vtable_EGL_CGLEBehavior { // behaviorfollow in between
	void(__thiscall* SetCommandInt)(shok_GGL_CBattleBehavior* th, shok_LeaderCommand cmd);
};
//constexpr int i = offsetof(shok_vtable_GGL_CBattleBehavior, SetCommandInt) / 4;

struct shok_vtable_shok_GGL_CPositionAtResourceFinder {
	void(__thiscall* Destructor)(shok_GGL_CPositionAtResourceFinder* th, bool free);
	float(__thiscall* SearchForPosition)(shok_GGL_CPositionAtResourceFinder* th, shok_EGL_CGLEEntity* e);
	void(__thiscall* GetPositionOffset)(shok_GGL_CPositionAtResourceFinder* th, shok_position* p, float f);
};

// vtable heroability 8 is ability(this, abilityid)

float shok_GGL_CBehaviorDefaultMovement::GetMovementSpeed()
{
	return reinterpret_cast<shok_vtable_GGL_CBehaviorDefaultMovement*>(vtable)->GetSpeed(this);
}


shok_BB_CClassFactory_serializationData* shok_EGL_CGLEBehavior::SerializationData = reinterpret_cast<shok_BB_CClassFactory_serializationData*>(0x86A828);

void __declspec(naked) bombattachment_fix() {
	__asm {
		push 0;
		push 0;
		push eax;
		push 43;
		push[esi + 8];
		call shok_EGL_CGLEEntity::GetEntityByID;
		mov ecx, eax;
		call shok_EGL_CGLEEntity::AttachEntity;


		or [ebp - 4], 0x0FFFFFFFF;
		lea ecx, [ebp - 0x3C];

		push 0x5062CD;
		ret;
	};
}
bool FixBombAttachment_Hooked = false;
void shok_GGL_CBombPlacerBehavior::FixBombAttachment()
{

	if (FixBombAttachment_Hooked)
		return;
	FixBombAttachment_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x5062C6), 10 };
	WriteJump(reinterpret_cast<void*>(0x5062C6), &bombattachment_fix);
}

int (*shok_GGL_CSniperAbility::SnipeDamageOverride)(shok_EGL_CGLEEntity* sniper, shok_EGL_CGLEEntity* tar, int dmg) = nullptr;
int __fastcall sniperability_tasksnipeoverride(shok_GGL_CSniperAbility* thi, int _, int taskargs) {
	shok_EGL_CGLEEntity* thent = shok_EGL_CGLEEntity::GetEntityByID(thi->EntityId);
	shok_EGL_CGLEEntity* tar = shok_EGL_CGLEEntity::GetEntityByID(thi->TargetId);
	if (!tar || tar->Health <= 0)
		return 0;
	shok_GGL_CSniperAbilityProps* pr = thent->GetEntityType()->GetBehaviorProps<shok_GGL_CSniperAbilityProps>();
	if (thent->Position.GetDistanceSquaredTo(tar->Position) >= pr->Range * pr->Range)
		return 0;
	int dmg = static_cast<int>(tar->GetMaxHealth() * pr->DamageFactor);
	if (shok_GGL_CSniperAbility::SnipeDamageOverride)
		dmg = shok_GGL_CSniperAbility::SnipeDamageOverride(thent, tar, dmg);
	shok_GGL_CBattleBehaviorProps* bpr = thent->GetEntityType()->GetBehaviorProps< shok_GGL_CBattleBehaviorProps>();
	shok_CProjectileEffectCreator cr{};
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
	cr.AdvancedDamageSourceOverride = static_cast<byte>(AdvancedDealDamageSource::AbilitySnipe);
	(*shok_EGL_CGLEGameLogic::GlobalObj)->CreateEffect(&cr);
	return 0;
}
bool OverrideSnipeTask_Hooked = false;
void shok_GGL_CSniperAbility::OverrideSnipeTask()
{
	if (OverrideSnipeTask_Hooked)
		return;
	OverrideSnipeTask_Hooked = true;
	shok_saveVirtualProtect vp{ reinterpret_cast<void*>(0x4DB5B8), 10 };
	WriteJump(reinterpret_cast<void*>(0x4DB5B8), &sniperability_tasksnipeoverride);
}

static inline float(__thiscall* const battleBehaviorGetMaxRange)(shok_GGL_CBattleBehavior*) = reinterpret_cast<float(__thiscall*)(shok_GGL_CBattleBehavior*)>(0x50AB43);
float shok_GGL_CBattleBehavior::GetMaxRange()
{
	return battleBehaviorGetMaxRange(this);
}
void shok_GGL_CBattleBehavior::SetCurrentCommand(shok_LeaderCommand cmd)
{
	reinterpret_cast<shok_vtable_GGL_CBattleBehavior*>(vtable)->SetCommandInt(this, cmd);
}

static inline int(__thiscall* const leaderbehgettroophealth)(shok_GGL_CBattleBehavior*) = reinterpret_cast<int(__thiscall*)(shok_GGL_CBattleBehavior*)>(0x4EE1D6);
int shok_GGL_CLeaderBehavior::GetTroopHealth()
{
	return leaderbehgettroophealth(this);
}
static inline int(__thiscall* const leaderbehgettroophealthpersol)(shok_GGL_CBattleBehavior*) = reinterpret_cast<int(__thiscall*)(shok_GGL_CBattleBehavior*)>(0x4ECE77);
int shok_GGL_CLeaderBehavior::GetTroopHealthPerSoldier()
{
	return leaderbehgettroophealthpersol(this);
}

void shok_GGL_CKegBehavior::AdvancedDealDamage()
{
	shok_EGL_CGLEEntity* e = shok_EGL_CGLEEntity::GetEntityByID(EntityId);
	{
		if (int id = e->GetFirstAttachedEntity(shok_AttachmentType::KEG_TARGET_BUILDING)) {
			if (shok_EGL_CGLEEntity* t = shok_EGL_CGLEEntity::GetEntityByID(id)) {
				if (shok_GGL_CBuilding* b = shok_DynamicCast<shok_EGL_CGLEEntity, shok_GGL_CBuilding>(t)) {
					int dmg;
					if (shok_DynamicCast<shok_GGL_CBuilding, shok_GGL_CBridgeEntity>(b)) {
						dmg = b->Health;
					}
					else {
						float kegeff = static_cast<shok_GGL_CGLBuildingProps*>(b->GetEntityType()->LogicProps)->KegEffectFactor;
						int dmgperc = static_cast<shok_GGL_CKegBehaviorProperties*>(PropPointer)->DamagePercent;
						int mhp = b->GetMaxHealth();
						dmg = static_cast<int>(kegeff * dmgperc * mhp / 100);
					}
					b->AdvancedHurtEntityBy(e, dmg, 0, false, false, true, AdvancedDealDamageSource::AbilitySabotageSingleTarget);
				}
			}
		}
	}
	{
		if (int id = e->GetFirstAttachedToMe(shok_AttachmentType::DISARMING_THIEF_KEG)) {
			if (shok_EGL_CGLEEntity* t = shok_EGL_CGLEEntity::GetEntityByID(id)) {
				if (t->Position.IsInRange(e->Position, 3)) {
					t->AdvancedHurtEntityBy(e, t->Health, 0, true, false, true, AdvancedDealDamageSource::AbilitySabotageSingleTarget);
				}
			}
		}
	}
	{
		shok_GGL_CKegBehaviorProperties* pr = static_cast<shok_GGL_CKegBehaviorProperties*>(PropPointer);
		shok_EGL_CGLEEntity::AdvancedDealAoEDamage(e, e->Position, pr->Radius, pr->Damage, e->PlayerId, 0, true, false, true, AdvancedDealDamageSource::AbilitySabotageBlast);
	}
}

static inline float(__thiscall* const autocannonBehaviorGetMaxRange)(shok_GGL_CAutoCannonBehavior*) = reinterpret_cast<float(__thiscall*)(shok_GGL_CAutoCannonBehavior*)>(0x50F508);
float shok_GGL_CAutoCannonBehavior::GetMaxRange()
{
	return autocannonBehaviorGetMaxRange(this);
}

static inline shok_GGL_CPositionAtResourceFinder* (__cdecl* const shok_GGL_CPositionAtResourceFinder_greatebyent)(int id) = reinterpret_cast<shok_GGL_CPositionAtResourceFinder * (__cdecl*)(int)>(0x4CB1C1);
shok_GGL_CPositionAtResourceFinder* shok_GGL_CPositionAtResourceFinder::CreateByEntity(int entityid)
{
	return shok_GGL_CPositionAtResourceFinder_greatebyent(entityid);
}
void shok_GGL_CPositionAtResourceFinder::Destroy()
{
	reinterpret_cast<shok_vtable_shok_GGL_CPositionAtResourceFinder*>(vtable)->Destructor(this, true);
}
float shok_GGL_CPositionAtResourceFinder::SearchForPosition(shok_EGL_CGLEEntity* e)
{
	return reinterpret_cast<shok_vtable_shok_GGL_CPositionAtResourceFinder*>(vtable)->SearchForPosition(this, e);
}
static inline void(__thiscall* const shok_GGL_CPositionAtResourceFinder_getposbyfloat)(shok_GGL_CPositionAtResourceFinder* th, shok_position* p, float f) = reinterpret_cast<void(__thiscall*)(shok_GGL_CPositionAtResourceFinder*, shok_position*, float)>(0x4CA89E);
shok_position shok_GGL_CPositionAtResourceFinder::CalcPositionFromFloat(float f)
{
	shok_position p;
	//reinterpret_cast<shok_vtable_shok_GGL_CPositionAtResourceFinder*>(vtable)->GetPosition(this, &p, f);
	shok_GGL_CPositionAtResourceFinder_getposbyfloat(this, &p, f);
	return p;
}

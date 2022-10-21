#include "pch.h"
#include "l_entitytype.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_behaviorProps.h"
#include "s5_entitytype.h"
#include "s5_entity.h"
#include "s5_maplogic.h"
#include "s5_player.h"
#include "luaext.h"
#include "hooks.h"

namespace CppLogic::EntityType {
	int LeaderTypeGetSoldierType(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		L.Push(p->SoldierType);
		return 1;
	}
	int LeaderTypeSetSoldierType(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		GGlue::CGlueEntityProps* t2 = L.CheckEntityType(2);
		if (!t2->IsSettlerType())
			throw lua::LuaException("no settler type at 2");
		if (!t2->GetBehaviorProps<GGL::CSoldierBehaviorProps>())
			throw lua::LuaException("no soldier type at 2");
		p->SoldierType = L.CheckInt(2);
		return 0;
	}

	int GetLimitedLifespanDuration(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CLimitedLifespanBehaviorProps* l = t->GetBehaviorProps<GGL::CLimitedLifespanBehaviorProps>();
		if (!l)
			throw lua::LuaException("no limited lifespan entitytype at 1");
		L.Push(l->LifespanSeconds);
		return 1;
	}
	int SetLimitedLifespanDuration(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CLimitedLifespanBehaviorProps* l = t->GetBehaviorProps<GGL::CLimitedLifespanBehaviorProps>();
		if(!l)
			throw lua::LuaException("no limited lifespan entitytype at 1");
		l->LifespanSeconds = L.CheckInt(2);
		return 0;
	}

	int LeaderTypeGetUpkeep(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		L.Push(p->UpkeepCosts);
		return 1;
	}
	int LeaderTypeSetUpkeep(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		GGL::CLeaderBehaviorProps* p = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		p->UpkeepCosts = L.CheckFloat(2);
		return 0;
	}

	int GetMaxHealth(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(t->LogicProps->MaxHealth);
		return 1;
	}
	int SetMaxHealth(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		t->LogicProps->MaxHealth = L.CheckInt(2);
		return 0;
	}

	int GetSabotageFactor(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.Push(bp->KegEffectFactor);
		return 1;
	}
	int SetSabotageFactor(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bp->KegEffectFactor = L.CheckFloat(2);
		return 0;
	}

	int GetAffectMotivation(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CAffectMotivationBehaviorProps* a = t->GetBehaviorProps<GGL::CAffectMotivationBehaviorProps>();
		if (!a)
			throw lua::LuaException("no affect motivation entitytype at 1");
		L.Push(a->MotivationEffect);
		return 1;
	}
	int SetAffectMotivation(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CAffectMotivationBehaviorProps* a = t->GetBehaviorProps<GGL::CAffectMotivationBehaviorProps>();
		if (!a)
			throw lua::LuaException("no affect motivation entitytype at 1");
		a->MotivationEffect = L.CheckFloat(2);
		return 0;
	}

	int GetSuspendedAnimation(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLAnimationBehaviorExProps* a = t->GetBehaviorProps<GGL::CGLAnimationBehaviorExProps>();
		if (!a)
			throw lua::LuaException("no animated entitytype at 1");
		L.Push(a->SuspensionAnimation);
		return 1;
	}
	int SetSuspendedAnimation(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLAnimationBehaviorExProps* a = t->GetBehaviorProps<GGL::CGLAnimationBehaviorExProps>();
		if (!a)
			throw lua::LuaException("no animated entitytype at 1");
		a->SuspensionAnimation = L.CheckInt(2);
		return 0;
	}

	int LeaderTypeGetRegeneration(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CLeaderBehaviorProps* l = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!l)
			throw lua::LuaException("no leader entitytype at 1");
		L.Push(l->HealingPoints);
		L.Push(l->HealingSeconds);
		return 2;
	}
	int LeaderTypeSetRegeneration(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CLeaderBehaviorProps* l = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!l)
			throw lua::LuaException("no leader entitytype at 1");
		if (L.IsNumber(2))
			l->HealingPoints = L.CheckInt(2);
		if (L.IsNumber(3))
			l->HealingSeconds = L.CheckInt(3);
		return 0;
	}

	int GetAutoAttackDamage(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			L.Push(b->DamageAmount);
			L.Push(b->DamageClass);
			return 2;
		}
		GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			L.Push(a->DamageAmount);
			L.Push(a->DamageClass);
			return 2;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}
	int SetAutoAttackDamage(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			b->DamageAmount = L.CheckInt(2);
			b->DamageClass = L.CheckInt(3);
			return 0;
		}
		GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			a->DamageAmount = L.CheckInt(2);
			a->DamageClass = L.CheckInt(3);
			return 0;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}

	int AddEntityCategory(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		shok::EntityCategory cat = static_cast<shok::EntityCategory>(L.CheckInt(2));
		if (t->IsOfCategory(cat))
			throw lua::LuaException("entitytype is already of this cat");
		auto v = t->LogicProps->Categories.SaveVector();
		v.Vector.push_back(cat);
		return 0;
	}
	int RemoveEntityCategory(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		shok::EntityCategory cat = static_cast<shok::EntityCategory>(L.CheckInt(2));
		auto v = t->LogicProps->Categories.SaveVector();
		auto e = std::remove_if(v.Vector.begin(), v.Vector.end(), [cat](shok::EntityCategory c) { return c == cat; });
		v.Vector.erase(e, v.Vector.end());
		return 0;
	}

	int GetAutoAttackMissChance(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		L.Push(b->MissChance);
		return 1;
	}
	int SetAutoAttackMissChance(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		b->MissChance = L.CheckInt(2);
		return 0;
	}

	int GetAutoAttackRange(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			L.Push(b->MaxRange);
			L.Push(b->MinRange);
			return 2;
		}
		GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			L.Push(a->MaxAttackRange);
			return 1;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}
	int SetAutoAttackRange(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			if (L.IsNumber(2))
				b->MaxRange = L.CheckFloat(2);
			if (L.IsNumber(3))
				b->MinRange = L.CheckFloat(3);
			return 0;
		}
		GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			if (L.IsNumber(2))
				a->MaxAttackRange = L.CheckFloat(2);
			return 0;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}

	int GetBattleWaitUntil(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			L.Push(b->BattleWaitUntil);
			return 1;
		}
		GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			L.Push(a->ReloadTime);
			return 1;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}
	int SetBattleWaitUntil(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CBattleBehaviorProps* b = t->GetBehaviorProps<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			b->BattleWaitUntil = L.CheckInt(2);
			return 0;
		}
		GGL::CAutoCannonBehaviorProps* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			a->ReloadTime = L.CheckInt(2);
			return 0;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}

	int LeaderTypeGetAutoAggressiveRange(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CLeaderBehaviorProps* b = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		L.Push(b->AutoAttackRange);
		return 1;
	}
	int LeaderTypeSetAutoAggressiveRange(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CLeaderBehaviorProps* b = t->GetBehaviorProps<GGL::CLeaderBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		b->AutoAttackRange = L.CheckFloat(2);
		return 0;
	}

	int GetVCAttractionSlotsProvided(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.Push(bp->NumberOfAttractableSettlers);
		return 1;
	}
	int SetVCAttractionSlotsProvided(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bp->NumberOfAttractableSettlers = L.CheckInt(2);
		return 0;
	}

	int ThiefTypeGetStealingInfo(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CThiefBehaviorProperties* p = t->GetBehaviorProps<GGL::CThiefBehaviorProperties>();
		if (!p)
			throw lua::LuaException("no thief entity at 1");
		L.Push(p->SecondsNeededToSteal);
		L.Push(p->MaximumAmountToSteal);
		L.Push(p->MinimumAmountToSteal);
		return 3;
	}
	int ThiefTypeSetStealingInfo(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CThiefBehaviorProperties* p = t->GetBehaviorProps<GGL::CThiefBehaviorProperties>();
		if (!p)
			throw lua::LuaException("no thief entity at 1");
		if (L.IsNumber(2))
			p->SecondsNeededToSteal = L.CheckInt(2);
		if (L.IsNumber(3))
			p->MaximumAmountToSteal = L.CheckInt(3);
		if (L.IsNumber(4))
			p->MinimumAmountToSteal = L.CheckInt(4);
		return 0;
	}

	int GetAbilityDataCamouflage(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CCamouflageBehaviorProps* p = t->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
		if (!p)
			throw lua::LuaException("no camoflague entity at 1");
		L.Push(p->DurationSeconds);
		L.Push(p->DiscoveryRange);
		L.Push(p->RechargeTimeSeconds);
		return 3;
	}
	int SetAbilityDataCamouflage(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CCamouflageBehaviorProps* p = t->GetBehaviorProps<GGL::CCamouflageBehaviorProps>();
		if (!p)
			throw lua::LuaException("no camoflague entity at 1");
		if (L.IsNumber(2))
			p->DurationSeconds = L.CheckInt(2);
		if (L.IsNumber(3))
			p->DiscoveryRange = L.CheckFloat(3);
		if (L.IsNumber(4))
			p->RechargeTimeSeconds = L.CheckInt(4);
		return 0;
	}

	int GetArmor(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (t->IsSettlerType()) {
			GGL::CGLSettlerProps* s = (GGL::CGLSettlerProps*)t->LogicProps;
			L.Push(s->ArmorAmount);
			L.Push(s->ArmorClass);
			return 2;
		}
		if (t->IsBuildingType()) {
			GGL::CGLBuildingProps* s = (GGL::CGLBuildingProps*)t->LogicProps;
			L.Push(s->ArmorAmount);
			L.Push(s->ArmorClass);
			return 2;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}
	int SetArmor(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)) {
			if (L.IsNumber(2))
				s->ArmorAmount = L.CheckInt(2);
			if (L.IsNumber(3))
				s->ArmorClass = L.CheckInt(3);
			return 0;
		}
		if (GGL::CGLBuildingProps* s = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)) {
			if (L.IsNumber(2))
				s->ArmorAmount = L.CheckInt(2);
			if (L.IsNumber(3))
				s->ArmorClass = L.CheckInt(3);
			return 0;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}

	int GetModels(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		for (int i = 0; i < 4; i++)
			L.Push(t->DisplayProps->Model[i]);
		return 4;
	}
	int SetModels(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		for (int i = 0; i < 4; i++)
			if (L.IsNumber(1 + i))
				t->DisplayProps->Model[i] = L.CheckInt(1 + i);
		return 0;
	}

	int GetAbilityDataCircularAttack(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CCircularAttackProps* p = t->GetBehaviorProps<GGL::CCircularAttackProps>();
		if (!p)
			throw lua::LuaException("no circularattack entity at 1");
		L.Push(p->Damage);
		L.Push(p->DamageClass);
		L.Push(p->Range);
		L.Push(p->RechargeTimeSeconds);
		return 4;
	}
	int SetAbilityDataCircularAttack(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CCircularAttackProps* p = t->GetBehaviorProps<GGL::CCircularAttackProps>();
		if (!p)
			throw lua::LuaException("no circularattack entity at 1");
		if (L.IsNumber(2))
			p->Damage = L.CheckInt(2);
		if (L.IsNumber(3))
			p->DamageClass = L.CheckInt(3);
		if (L.IsNumber(4))
			p->Range = L.CheckFloat(4);
		if (L.IsNumber(5))
			p->RechargeTimeSeconds = L.CheckInt(5);
		return 0;
	}

	int GetAbilityDataShuriken(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CShurikenAbilityProps* p = t->GetBehaviorProps<GGL::CShurikenAbilityProps>();
		if (!p)
			throw lua::LuaException("no shuriken entity at 1");
		L.Push(p->DamageAmount);
		L.Push(p->DamageClass);
		L.Push(p->NumberShuriken);
		L.Push(p->Range);
		L.Push(p->MaxArcDegree);
		L.Push(p->RechargeTimeSeconds);
		L.Push(p->ProjectileType);
		return 7;
	}
	int SetAbilityDataShuriken(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CShurikenAbilityProps* p = t->GetBehaviorProps<GGL::CShurikenAbilityProps>();
		if (!p)
			throw lua::LuaException("no shuriken entity at 1");
		if (L.IsNumber(2))
			p->DamageAmount = L.CheckInt(2);
		if (L.IsNumber(3))
			p->DamageClass = L.CheckInt(3);
		if (L.IsNumber(4))
			p->NumberShuriken = L.CheckInt(4);
		if (L.IsNumber(5))
			p->Range = L.CheckFloat(5);
		if (L.IsNumber(6))
			p->MaxArcDegree = L.CheckFloat(6);
		if (L.IsNumber(7))
			p->RechargeTimeSeconds = L.CheckInt(7);
		return 0;
	}

	int GetAbilityDataSniper(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CSniperAbilityProps* p = t->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!p)
			throw lua::LuaException("no sniper entity at 1");
		L.Push(p->DamageFactor);
		L.Push(p->Range);
		L.Push(p->RechargeTimeSeconds);
		return 3;
	}
	int SetAbilityDataSniper(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CSniperAbilityProps* p = t->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!p)
			throw lua::LuaException("no sniper entity at 1");
		if (L.IsNumber(2))
			p->DamageFactor = L.CheckFloat(2);
		if (L.IsNumber(3))
			p->Range = L.CheckFloat(3);
		if (L.IsNumber(4))
			p->RechargeTimeSeconds = L.CheckInt(4);
		return 0;
	}

	int GetAbilityDataRangedEffect(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CRangedEffectAbilityProps* p = t->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
		if (!p)
			throw lua::LuaException("no ranged effect entity at 1");
		L.Push(p->DamageFactor);
		L.Push(p->ArmorFactor);
		L.Push(p->HealthRecoveryFactor);
		L.Push(p->Range);
		L.Push(p->RechargeTimeSeconds);
		return 5;
	}
	int SetAbilityDataRangedEffect(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CRangedEffectAbilityProps* p = t->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
		if (!p)
			throw lua::LuaException("no ranged effect entity at 1");
		if (L.IsNumber(2))
			p->DamageFactor = L.CheckFloat(2);
		if (L.IsNumber(3))
			p->ArmorFactor = L.CheckFloat(3);
		if (L.IsNumber(4))
			p->HealthRecoveryFactor = L.CheckFloat(4);
		if (L.IsNumber(5))
			p->Range = L.CheckFloat(5);
		if (L.IsNumber(6))
			p->RechargeTimeSeconds = L.CheckInt(6);
		return 0;
	}

	int ResourceTreeTypeHasData(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CEntityProperties* l = dynamic_cast<GGL::CEntityProperties*>(t->LogicProps);
		L.Push(l != nullptr);
		return 1;
	}
	int ResourceTreeTypeGetData(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CEntityProperties* l = dynamic_cast<GGL::CEntityProperties*>(t->LogicProps);
		if (!l)
			throw lua::LuaException("no resource tree entity at 1");
		L.Push(l->ResourceEntity);
		L.Push(l->ResourceAmount);
		return 2;
	}
	int ResourceTreeTypeSetData(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CEntityProperties* l = dynamic_cast<GGL::CEntityProperties*>(t->LogicProps);
		if (!l)
			throw lua::LuaException("no resource tree entity at 1");
		if (L.IsNumber(2))
			l->ResourceEntity = L.CheckInt(2);
		if (L.IsNumber(3))
			l->ResourceAmount = L.CheckInt(3);
		return 0;
	}

	int GetFearless(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.Push(s->Fearless);
		return 1;
	}
	int SetFearless(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.CheckType(2, lua::LType::Boolean);
		s->Fearless = L.ToBoolean(2);
		return 0;
	}

	int SettlerGetCost(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.PushCostInfo(s->Cost);
		return 1;
	}
	int SettlerSetCost(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		bool ig = L.OptBool(3, false);
		L.ReadCostInfo(2, s->Cost, ig);
		return 0;
	}

	int GetConstructionCost(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.PushCostInfo(bp->ConstructionInfo.Cost);
		return 1;
	}
	int SetConstructionCost(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bool ig = L.OptBool(3, false);
		L.ReadCostInfo(2, bp->ConstructionInfo.Cost, ig);
		return 0;
	}

	int GetUpgradeCost(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.PushCostInfo(bp->Upgrade.Cost);
		return 1;
	}
	int SetUpgradeCost(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLBuildingProps* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bool ig = L.OptBool(3, false);
		L.ReadCostInfo(2, bp->Upgrade.Cost, ig);
		return 0;
	}

	int GetBlocking(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.NewTable();
		int i = 1;
		for (const shok::AARect& r : t->LogicProps->BlockingArea) {
			L.NewTable();
			L.PushPos(r.low);
			L.SetTableRaw(-2, 1);
			L.PushPos(r.high);
			L.SetTableRaw(-2, 2);
			L.SetTableRaw(-2, i);
			i++;
		}
		L.Push(t->LogicProps->NumBlockedPoints);
		if (GGL::CBuildBlockProperties* bb = dynamic_cast<GGL::CBuildBlockProperties*>(t->LogicProps)) {
			L.NewTable();
			L.PushPos(bb->BuildBlockArea.low);
			L.SetTableRaw(-2, 1);
			L.PushPos(bb->BuildBlockArea.high);
			L.SetTableRaw(-2, 2);
			return 3;
		}
		return 2;
	}

	int GetArmorModifierTechs(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (int i : s->ModifyArmor.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		if (GGL::CGLBuildingProps* s = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (int i : s->ModifyArmor.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}

	int GetExplorationModifierTechs(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (int i : s->ModifyExploration.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		if (GGL::CGLBuildingProps* s = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (int i : s->ModifyExploration.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}

	int GetDamageModifierTechs(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.NewTable();
		int c = 1;
		for (int i : s->ModifyDamage.TechList) {
			L.Push(i);
			L.SetTableRaw(-2, c);
			c++;
		}
		return 1;
	}

	int GetMaxRangeModifierTechs(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.NewTable();
		int c = 1;
		for (int i : s->ModifyMaxRange.TechList) {
			L.Push(i);
			L.SetTableRaw(-2, c);
			c++;
		}
		return 1;
	}

	int GetSpeedModifierTechs(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CGLSettlerProps* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.NewTable();
		int c = 1;
		for (int i : s->ModifySpeed.TechList) {
			L.Push(i);
			L.SetTableRaw(-2, c);
			c++;
		}
		return 1;
	}

	int AddHPTechMod(lua::State ls) {
		luaext::EState L{ ls };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supportet with SCELoader");
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsBuildingType())
			throw lua::LuaException("no building type at 1");
		int tech = L.CheckInt(2);
		shok::Technology* techo = (*GGL::CGLGameLogic::GlobalObj)->GetTech(tech);
		if (!techo)
			throw lua::LuaException("no tech at 2");
		EGL::CGLEEntity::BuildingMaxHpTechBoni.emplace(L.CheckInt(1), tech);
		return 0;
	}

	int GetBuildOnTypes(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsBuildingType())
			throw lua::LuaException("no building type at 1");
		L.NewTable();
		int i = 1;
		for (int ty : dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)->BuildOn) {
			L.Push(ty);
			L.SetTableRaw(-2, i);
			i++;
		}
		return 1;
	}

	int LeaderTypeGetMaxNumberOfSoldiers(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		GGL::CLimitedAttachmentBehaviorProperties* pr = t->GetBehaviorProps<GGL::CLimitedAttachmentBehaviorProperties>();
		if (!pr)
			throw lua::LuaException("no limited attachment");
		std::string_view v = "ATTACHMENT_LEADER_SOLDIER";
		for (auto& l : pr->Attachments) {
			if (v.compare(l.Type.c_str()) == 0) {
				L.Push(l.Limit);
				return 1;
			}
		}
		throw lua::LuaException("no attachment limit ATTACHMENT_LEADER_SOLDIER");
	}

	int GetUpgradeCategory(lua::State ls) {
		luaext::EState L{ ls };
		L.CheckEntityType(1);
		int ucat = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(1)->SettlerUpgradeManager->GetUpgradeCategoryOfEntityType(L.CheckInt(1));
		L.Push(ucat);
		return 1;
	}

	int GetDeleteWhenBuildOn(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(t->LogicProps->DeleteWhenBuiltOn);
		return 1;
	}
	int SetDeleteWhenBuildOn(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.CheckType(2, lua::LType::Boolean);
		t->LogicProps->DeleteWhenBuiltOn = L.ToBoolean(2);
		return 0;
	}

	int IsConvertible(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)) {
			L.Push(s->Convertible);
			return 1;
		}
		if (auto* s = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)) {
			L.Push(s->Convertible);
			return 1;
		}
		return 0;
	}

	int IsBuildingType(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)));
		return 1;
	}
	int IsSettlerType(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)));
		return 1;
	}
	int IsLeaderType(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(t->GetBehaviorProps<GGL::CLeaderBehaviorProps>()));
		return 1;
	}
	int IsSoldierType(lua::State ls) {
		luaext::EState L{ ls };
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(t->GetBehaviorProps<GGL::CSoldierBehaviorProps>()));
		return 1;
	}

	constexpr std::array<lua::FuncReference, 35> EntityType{ {
			lua::FuncReference::GetRef<GetLimitedLifespanDuration>("GetLimitedLifespanDuration"),
			lua::FuncReference::GetRef<SetLimitedLifespanDuration>("SetLimitedLifespanDuration"),
			lua::FuncReference::GetRef<GetMaxHealth>("GetMaxHealth"),
			lua::FuncReference::GetRef<SetMaxHealth>("SetMaxHealth"),
			lua::FuncReference::GetRef<GetAffectMotivation>("GetAffectMotivation"),
			lua::FuncReference::GetRef<SetAffectMotivation>("SetAffectMotivation"),
			lua::FuncReference::GetRef<GetSuspendedAnimation>("GetSuspendedAnimation"),
			lua::FuncReference::GetRef<SetSuspendedAnimation>("SetSuspendedAnimation"),
			lua::FuncReference::GetRef<GetAutoAttackDamage>("GetAutoAttackDamage"),
			lua::FuncReference::GetRef<SetAutoAttackDamage>("SetAutoAttackDamage"),
			lua::FuncReference::GetRef<AddEntityCategory>("AddEntityCategory"),
			lua::FuncReference::GetRef<RemoveEntityCategory>("RemoveEntityCategory"),
			lua::FuncReference::GetRef<GetAutoAttackMissChance>("GetAutoAttackMissChance"),
			lua::FuncReference::GetRef<SetAutoAttackMissChance>("SetAutoAttackMissChance"),
			lua::FuncReference::GetRef<GetAutoAttackRange>("GetAutoAttackRange"),
			lua::FuncReference::GetRef<SetAutoAttackRange>("SetAutoAttackRange"),
			lua::FuncReference::GetRef<GetArmor>("GetArmor"),
			lua::FuncReference::GetRef<SetArmor>("SetArmor"),
			lua::FuncReference::GetRef<GetModels>("GetModels"),
			lua::FuncReference::GetRef<SetModels>("SetModels"),
			lua::FuncReference::GetRef<ResourceTreeTypeHasData>("ResourceTreeTypeHasData"),
			lua::FuncReference::GetRef<ResourceTreeTypeGetData>("ResourceTreeTypeGetData"),
			lua::FuncReference::GetRef<ResourceTreeTypeSetData>("ResourceTreeTypeSetData"),
			lua::FuncReference::GetRef<GetBlocking>("GetBlocking"),
			lua::FuncReference::GetRef<GetArmorModifierTechs>("GetArmorModifierTechs"),
			lua::FuncReference::GetRef<GetExplorationModifierTechs>("GetExplorationModifierTechs"),
			lua::FuncReference::GetRef<GetDeleteWhenBuildOn>("GetDeleteWhenBuildOn"),
			lua::FuncReference::GetRef<SetDeleteWhenBuildOn>("SetDeleteWhenBuildOn"),
			lua::FuncReference::GetRef<GetBattleWaitUntil>("GetBattleWaitUntil"),
			lua::FuncReference::GetRef<SetBattleWaitUntil>("SetBattleWaitUntil"),
			lua::FuncReference::GetRef<IsConvertible>("IsConvertible"),
			lua::FuncReference::GetRef<IsBuildingType>("IsBuildingType"),
			lua::FuncReference::GetRef<IsSettlerType>("IsSettlerType"),
			lua::FuncReference::GetRef<IsLeaderType>("IsLeaderType"),
			lua::FuncReference::GetRef<IsSoldierType>("IsSoldierType"),
		} };

	constexpr std::array<lua::FuncReference, 29> Settler{ {
			lua::FuncReference::GetRef<LeaderTypeGetSoldierType>("LeaderTypeGetSoldierType"),
			lua::FuncReference::GetRef<LeaderTypeSetSoldierType>("LeaderTypeSetSoldierType"),
			lua::FuncReference::GetRef<LeaderTypeGetUpkeep>("LeaderTypeGetUpkeep"),
			lua::FuncReference::GetRef<LeaderTypeSetUpkeep>("LeaderTypeSetUpkeep"),
			lua::FuncReference::GetRef<LeaderTypeGetRegeneration>("LeaderTypeGetRegeneration"),
			lua::FuncReference::GetRef<LeaderTypeSetRegeneration>("LeaderTypeSetRegeneration"),
			lua::FuncReference::GetRef<LeaderTypeGetAutoAggressiveRange>("LeaderTypeGetAutoAggressiveRange"),
			lua::FuncReference::GetRef<LeaderTypeSetAutoAggressiveRange>("LeaderTypeSetAutoAggressiveRange"),
			lua::FuncReference::GetRef<ThiefTypeGetStealingInfo>("ThiefTypeGetStealingInfo"),
			lua::FuncReference::GetRef<ThiefTypeSetStealingInfo>("ThiefTypeSetStealingInfo"),
			lua::FuncReference::GetRef<GetAbilityDataCamouflage>("GetAbilityDataCamouflage"),
			lua::FuncReference::GetRef<SetAbilityDataCamouflage>("SetAbilityDataCamouflage"),
			lua::FuncReference::GetRef<GetAbilityDataCircularAttack>("GetAbilityDataCircularAttack"),
			lua::FuncReference::GetRef<SetAbilityDataCircularAttack>("SetAbilityDataCircularAttack"),
			lua::FuncReference::GetRef<GetAbilityDataShuriken>("GetAbilityDataShuriken"),
			lua::FuncReference::GetRef<SetAbilityDataShuriken>("SetAbilityDataShuriken"),
			lua::FuncReference::GetRef<GetAbilityDataSniper>("GetAbilityDataSniper"),
			lua::FuncReference::GetRef<SetAbilityDataSniper>("SetAbilityDataSniper"),
			lua::FuncReference::GetRef<GetAbilityDataRangedEffect>("GetAbilityDataRangedEffect"),
			lua::FuncReference::GetRef<SetAbilityDataRangedEffect>("SetAbilityDataRangedEffect"),
			lua::FuncReference::GetRef<GetFearless>("GetFearless"),
			lua::FuncReference::GetRef<SetFearless>("SetFearless"),
			lua::FuncReference::GetRef<SettlerGetCost>("GetCost"),
			lua::FuncReference::GetRef<SettlerSetCost>("SetCost"),
			lua::FuncReference::GetRef<GetDamageModifierTechs>("GetDamageModifierTechs"),
			lua::FuncReference::GetRef<GetMaxRangeModifierTechs>("GetMaxRangeModifierTechs"),
			lua::FuncReference::GetRef<GetSpeedModifierTechs>("GetSpeedModifierTechs"),
			lua::FuncReference::GetRef<LeaderTypeGetMaxNumberOfSoldiers>("LeaderTypeGetMaxNumberOfSoldiers"),
			lua::FuncReference::GetRef<GetUpgradeCategory>("GetUpgradeCategory"),
		} };

	constexpr std::array<lua::FuncReference, 10> Building{ {
			lua::FuncReference::GetRef<GetSabotageFactor>("GetSabotageFactor"),
			lua::FuncReference::GetRef<SetSabotageFactor>("SetSabotageFactor"),
			lua::FuncReference::GetRef<GetVCAttractionSlotsProvided>("GetVCAttractionSlotsProvided"),
			lua::FuncReference::GetRef<SetVCAttractionSlotsProvided>("SetVCAttractionSlotsProvided"),
			lua::FuncReference::GetRef<GetConstructionCost>("GetConstructionCost"),
			lua::FuncReference::GetRef<SetConstructionCost>("SetConstructionCost"),
			lua::FuncReference::GetRef<GetUpgradeCost>("GetUpgradeCost"),
			lua::FuncReference::GetRef<SetUpgradeCost>("SetUpgradeCost"),
			lua::FuncReference::GetRef<AddHPTechMod>("AddHPTechMod"),
			lua::FuncReference::GetRef<GetBuildOnTypes>("GetBuildOnTypes"),
		} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(EntityType, -3);

		L.Push("Settler");
		L.NewTable();
		L.RegisterFuncs(Settler, -3);
		L.SetTableRaw(-3);

		L.Push("Building");
		L.NewTable();
		L.RegisterFuncs(Building, -3);
		L.SetTableRaw(-3);
	}
}
// CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(Entities.PU_LeaderSword1)
// Logic.IsEntityInCategory(GUI.GetSelectedEntity(), 100)
// CppLogic.EntityType.AddEntityCategory(Entities.PU_LeaderSword1, 100)
// CppLogic.EntityType.GetAutoAttackMissChance(Entities.PU_LeaderBow1)
// CppLogic.EntityType.Settler.GetAbilityDataCamouflage(Entities.PU_Hero5)
// CppLogic.EntityType.GetArmor(Entities.PU_LeaderSword1
// CppLogic.EntityType.Building.AddHPTechMod(Entities.PB_Farm1, Technologies.T_Fletching)

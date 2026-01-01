#include "l_entitytype.h"
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entitytype/s5_behaviorProps.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/entity/s5_entity.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/player/s5_player.h>
#include <luaext.h>
#include <utility/hooks.h>
#include <utility/luaserializer.h>
#include <utility/ModBehavior.h>

namespace CppLogic::EntityType {
	int LeaderTypeGetSoldierType(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		auto* p = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		L.Push(p->SoldierType);
		return 1;
	}
	int LeaderTypeSetSoldierType(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		auto* p = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		GGlue::CGlueEntityProps* t2 = L.CheckEntityType(2);
		if (!t2->IsSettlerType())
			throw lua::LuaException("no settler type at 2");
		if (!t2->GetBehaviorProps<GGL::CSoldierBehaviorProps>())
			throw lua::LuaException("no soldier type at 2");
		p->SoldierType = L.CheckEnum<shok::EntityTypeId>(2);
		return 0;
	}

	int GetLimitedLifespanDuration(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* l = t->GetBehaviorProps<GGL::CLimitedLifespanBehaviorProps>();
		if (!l)
			throw lua::LuaException("no limited lifespan entitytype at 1");
		L.Push(l->LifespanSeconds);
		return 1;
	}
	int SetLimitedLifespanDuration(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* l = t->GetBehaviorProps<GGL::CLimitedLifespanBehaviorProps>();
		if(!l)
			throw lua::LuaException("no limited lifespan entitytype at 1");
		l->LifespanSeconds = L.CheckInt(2);
		return 0;
	}

	int LeaderTypeGetUpkeep(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		auto* p = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		L.Push(p->UpkeepCosts);
		return 1;
	}
	int LeaderTypeSetUpkeep(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsSettlerType())
			throw lua::LuaException("no settler type at 1");
		auto* p = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!p)
			throw lua::LuaException("no leader type at 1");
		p->UpkeepCosts = L.CheckFloat(2);
		return 0;
	}

	int GetMaxHealth(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(t->LogicProps->MaxHealth);
		return 1;
	}
	int SetMaxHealth(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		t->LogicProps->MaxHealth = L.CheckInt(2);
		return 0;
	}

	int GetSabotageFactor(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.Push(bp->KegEffectFactor);
		return 1;
	}
	int SetSabotageFactor(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bp->KegEffectFactor = L.CheckFloat(2);
		return 0;
	}

	int GetAffectMotivation(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* a = t->GetBehaviorProps<GGL::CAffectMotivationBehaviorProps>();
		if (!a)
			throw lua::LuaException("no affect motivation entitytype at 1");
		L.Push(a->MotivationEffect);
		return 1;
	}
	int SetAffectMotivation(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* a = t->GetBehaviorProps<GGL::CAffectMotivationBehaviorProps>();
		if (!a)
			throw lua::LuaException("no affect motivation entitytype at 1");
		a->MotivationEffect = L.CheckFloat(2);
		return 0;
	}

	int GetSuspendedAnimation(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* a = t->GetBehaviorProps<GGL::CGLAnimationBehaviorExProps>();
		if (!a)
			throw lua::LuaException("no animated entitytype at 1");
		L.Push(a->SuspensionAnimation);
		return 1;
	}
	int SetSuspendedAnimation(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* a = t->GetBehaviorProps<GGL::CGLAnimationBehaviorExProps>();
		if (!a)
			throw lua::LuaException("no animated entitytype at 1");
		a->SuspensionAnimation = L.CheckEnum<shok::AnimationId>(2);
		return 0;
	}

	int LeaderTypeGetRegeneration(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* l = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!l)
			throw lua::LuaException("no leader entitytype at 1");
		L.Push(l->HealingPoints);
		L.Push(l->HealingSeconds);
		return 2;
	}
	int LeaderTypeSetRegeneration(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* l = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!l)
			throw lua::LuaException("no leader entitytype at 1");
		if (L.IsNumber(2))
			l->HealingPoints = L.CheckInt(2);
		if (L.IsNumber(3))
			l->HealingSeconds = L.CheckInt(3);
		return 0;
	}

	int GetAutoAttackDamage(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			L.Push(b->DamageAmount);
			L.Push(b->DamageClass);
			return 2;
		}
		auto* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			L.Push(a->DamageAmount);
			L.Push(a->DamageClass);
			return 2;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}
	int SetAutoAttackDamage(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			b->DamageAmount = L.CheckInt(2);
			b->DamageClass = L.CheckEnum<shok::DamageClassId>(3);
			return 0;
		}
		auto* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			a->DamageAmount = L.CheckInt(2);
			a->DamageClass = L.CheckEnum<shok::DamageClassId>(3);
			return 0;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}

	int AddEntityCategory(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto cat = static_cast<shok::EntityCategory>(L.CheckInt(2));
		if (t->IsOfCategory(cat))
			throw lua::LuaException("entitytype is already of this cat");
		auto v = t->LogicProps->Categories.SaveVector();
		v.Vector.push_back(cat);
		return 0;
	}
	int RemoveEntityCategory(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto cat = static_cast<shok::EntityCategory>(L.CheckInt(2));
		auto v = t->LogicProps->Categories.SaveVector();
		auto e = std::remove_if(v.Vector.begin(), v.Vector.end(), [cat](shok::EntityCategory c) { return c == cat; });
		v.Vector.erase(e, v.Vector.end());
		return 0;
	}

	int GetAutoAttackMissChance(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		L.Push(b->MissChance);
		return 1;
	}
	int SetAutoAttackMissChance(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		b->MissChance = L.CheckInt(2);
		return 0;
	}

	int GetAutoAttackRange(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			L.Push(b->MaxRange);
			L.Push(b->MinRange);
			return 2;
		}
		auto* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			L.Push(a->MaxAttackRange);
			return 1;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}
	int SetAutoAttackRange(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			if (L.IsNumber(2))
				b->MaxRange = L.CheckFloat(2);
			if (L.IsNumber(3))
				b->MinRange = L.CheckFloat(3);
			return 0;
		}
		auto* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			if (L.IsNumber(2))
				a->MaxAttackRange = L.CheckFloat(2);
			return 0;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}

	int GetBattleWaitUntil(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			L.Push(b->BattleWaitUntil);
			return 1;
		}
		auto* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			L.Push(a->ReloadTime);
			return 1;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}
	int SetBattleWaitUntil(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (b != nullptr) {
			b->BattleWaitUntil = L.CheckInt(2);
			return 0;
		}
		auto* a = t->GetBehaviorProps<GGL::CAutoCannonBehaviorProps>();
		if (a != nullptr) {
			a->ReloadTime = L.CheckInt(2);
			return 0;
		}
		throw lua::LuaException("no battle or autocannon entity type at 1");
	}

	int LeaderTypeGetAutoAggressiveRange(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		L.Push(b->AutoAttackRange);
		return 1;
	}
	int LeaderTypeSetAutoAggressiveRange(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* b = t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>();
		if (!b)
			throw lua::LuaException("no battle entity at 1");
		b->AutoAttackRange = L.CheckFloat(2);
		return 0;
	}

	int GetVCAttractionSlotsProvided(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.Push(bp->NumberOfAttractableSettlers);
		return 1;
	}
	int SetVCAttractionSlotsProvided(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bp->NumberOfAttractableSettlers = L.CheckInt(2);
		return 0;
	}

	int ThiefTypeGetStealingInfo(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CThiefBehaviorProperties>();
		if (!p)
			throw lua::LuaException("no thief entity at 1");
		L.Push(p->SecondsNeededToSteal);
		L.Push(p->MaximumAmountToSteal);
		L.Push(p->MinimumAmountToSteal);
		return 3;
	}
	int ThiefTypeSetStealingInfo(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CThiefBehaviorProperties>();
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

	int GetAbilityDataCamouflage(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorPropsDynamic<GGL::CCamouflageBehaviorProps>();
		if (!p)
			throw lua::LuaException("no camoflague entity at 1");
		L.Push(p->DurationSeconds);
		L.Push(p->DiscoveryRange);
		L.Push(p->RechargeTimeSeconds);
		return 3;
	}
	int SetAbilityDataCamouflage(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorPropsDynamic<GGL::CCamouflageBehaviorProps>();
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

	int GetArmor(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (t->IsSettlerType()) {
			auto* s = static_cast<GGL::CGLSettlerProps *>(t->LogicProps); // NOLINT(*-pro-type-static-cast-downcast)
			L.Push(s->ArmorAmount);
			L.Push(s->ArmorClass);
			return 2;
		}
		if (t->IsBuildingType()) {
			auto* s = static_cast<GGL::CGLBuildingProps *>(t->LogicProps); // NOLINT(*-pro-type-static-cast-downcast)
			L.Push(s->ArmorAmount);
			L.Push(s->ArmorClass);
			return 2;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}
	int SetArmor(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)) {
			if (L.IsNumber(2))
				s->ArmorAmount = L.CheckInt(2);
			if (L.IsNumber(3))
				s->ArmorClass = L.CheckEnum<shok::ArmorClassId>(3);
			return 0;
		}
		if (auto* s = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)) {
			if (L.IsNumber(2))
				s->ArmorAmount = L.CheckInt(2);
			if (L.IsNumber(3))
				s->ArmorClass = L.CheckEnum<shok::ArmorClassId>(3);
			return 0;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}

	int GetModels(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		for (shok::ModelId & i : t->DisplayProps->Model)
			L.Push(i);
		return 4;
	}
	int SetModels(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		for (int i = 0; i < 4; i++)
			if (L.IsNumber(1 + i))
				t->DisplayProps->Model[i] = L.CheckEnum<shok::ModelId>(1 + i);
		return 0;
	}

	int GetAbilityDataCircularAttack(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CCircularAttackProps>();
		if (!p)
			throw lua::LuaException("no circularattack entity at 1");
		L.Push(p->Damage);
		L.Push(p->DamageClass);
		L.Push(p->Range);
		L.Push(p->RechargeTimeSeconds);
		return 4;
	}
	int SetAbilityDataCircularAttack(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CCircularAttackProps>();
		if (!p)
			throw lua::LuaException("no circularattack entity at 1");
		if (L.IsNumber(2))
			p->Damage = L.CheckInt(2);
		if (L.IsNumber(3))
			p->DamageClass = L.CheckEnum<shok::DamageClassId>(3);
		if (L.IsNumber(4))
			p->Range = L.CheckFloat(4);
		if (L.IsNumber(5))
			p->RechargeTimeSeconds = L.CheckInt(5);
		return 0;
	}

	int GetAbilityDataShuriken(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CShurikenAbilityProps>();
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
	int SetAbilityDataShuriken(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CShurikenAbilityProps>();
		if (!p)
			throw lua::LuaException("no shuriken entity at 1");
		if (L.IsNumber(2))
			p->DamageAmount = L.CheckInt(2);
		if (L.IsNumber(3))
			p->DamageClass = L.CheckEnum<shok::DamageClassId>(3);
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

	int GetAbilityDataSniper(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CSniperAbilityProps>();
		if (!p)
			throw lua::LuaException("no sniper entity at 1");
		L.Push(p->DamageFactor);
		L.Push(p->Range);
		L.Push(p->RechargeTimeSeconds);
		return 3;
	}
	int SetAbilityDataSniper(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CSniperAbilityProps>();
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

	int GetAbilityDataRangedEffect(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
		if (!p)
			throw lua::LuaException("no ranged effect entity at 1");
		L.Push(p->DamageFactor);
		L.Push(p->ArmorFactor);
		L.Push(p->HealthRecoveryFactor);
		L.Push(p->Range);
		L.Push(p->RechargeTimeSeconds);
		return 5;
	}
	int SetAbilityDataRangedEffect(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<GGL::CRangedEffectAbilityProps>();
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

	int GetAbilityDataResurrect(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<CppLogic::Mod::ResurrectAbilityProps>();
		if (!p)
			throw lua::LuaException("no resurrect entity at 1");
		L.Push(p->Range);
		L.Push(p->ProgressPerTick);
		L.Push(p->RechargeTimeSeconds);
		return 3;
	}

	int GetAbilityLightningStrikeData(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<CppLogic::Mod::LightningStrikeAbilityProps>();
		if (p == nullptr)
			throw lua::LuaException{ "no lightning strike ability" };
		L.Push(p->Range);
		L.Push(p->WeatherEnergyCost);
		L.Push(p->Damage);
		L.Push(p->DamageClass);
		L.Push(p->DamageRange);
		L.Push(p->Effect);
		L.Push(p->RechargeTimeSeconds);
		return 7;
	}

	int GetAbilityBombardmentData(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* p = t->GetBehaviorProps<CppLogic::Mod::BombardmentAbilityProps>();
		if (p == nullptr)
			throw lua::LuaException{ "no bombardment ability" };
		L.Push(p->AttackRange);
		L.Push(p->EffectType);
		L.Push(p->DamageRange);
		L.Push(p->Damage);
		L.Push(p->DamageClass);
		L.Push(p->TaskList);
		L.Push(p->DistanceOverride);
		L.Push(p->RechargeTimeSeconds);
		return 8;
	}

	int ResourceTreeTypeHasData(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* l = dynamic_cast<GGL::CEntityProperties*>(t->LogicProps);
		L.Push(l != nullptr);
		return 1;
	}
	int ResourceTreeTypeGetData(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* l = dynamic_cast<GGL::CEntityProperties*>(t->LogicProps);
		if (!l)
			throw lua::LuaException("no resource tree entity at 1");
		L.Push(l->ResourceEntity);
		L.Push(l->ResourceAmount);
		return 2;
	}
	int ResourceTreeTypeSetData(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* l = dynamic_cast<GGL::CEntityProperties*>(t->LogicProps);
		if (!l)
			throw lua::LuaException("no resource tree entity at 1");
		if (L.IsNumber(2))
			l->ResourceEntity = L.CheckEnum<shok::EntityTypeId>(2);
		if (L.IsNumber(3))
			l->ResourceAmount = L.CheckInt(3);
		return 0;
	}

	int GetFearless(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.Push(s->Fearless);
		return 1;
	}
	int SetFearless(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.CheckType(2, lua::LType::Boolean);
		s->Fearless = L.ToBoolean(2);
		return 0;
	}

	int SettlerGetCost(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.Push(s->Cost);
		return 1;
	}
	int SettlerSetCost(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		bool ig = L.OptBool(3, false);
		L.ReadCostInfo(2, s->Cost, ig);
		return 0;
	}

	int GetConstructionCost(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.Push(bp->ConstructionInfo.Cost);
		return 1;
	}
	int SetConstructionCost(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bool ig = L.OptBool(3, false);
		L.ReadCostInfo(2, bp->ConstructionInfo.Cost, ig);
		return 0;
	}

	int GetUpgradeCost(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		L.Push(bp->Upgrade.Cost);
		return 1;
	}
	int SetUpgradeCost(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* bp = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps);
		if (!bp)
			throw lua::LuaException("no building type at 1");
		bool ig = L.OptBool(3, false);
		L.ReadCostInfo(2, bp->Upgrade.Cost, ig);
		return 0;
	}

	int GetBlocking(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.NewTable();
		int i = 1;
		for (const shok::AARect& r : t->LogicProps->BlockingArea) {
			L.NewTable();
			L.Push(r.low);
			L.SetTableRaw(-2, 1);
			L.Push(r.high);
			L.SetTableRaw(-2, 2);
			L.SetTableRaw(-2, i);
			i++;
		}
		L.Push(t->LogicProps->NumBlockedPoints);
		if (auto* bb = dynamic_cast<GGL::CBuildBlockProperties*>(t->LogicProps)) {
			L.NewTable();
			L.Push(bb->BuildBlockArea.low);
			L.SetTableRaw(-2, 1);
			L.Push(bb->BuildBlockArea.high);
			L.SetTableRaw(-2, 2);
			return 3;
		}
		return 2;
	}

	int GetArmorModifierTechs(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (auto i : s->ModifyArmor.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		if (auto* s = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (auto i : s->ModifyArmor.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}

	int GetExplorationModifierTechs(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (auto i : s->ModifyExploration.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		if (auto* s = dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)) {
			L.NewTable();
			int c = 1;
			for (auto i : s->ModifyExploration.TechList) {
				L.Push(i);
				L.SetTableRaw(-2, c);
				c++;
			}
			return 1;
		}
		throw lua::LuaException("no settler or building entity type at 1");
	}

	int GetDamageModifierTechs(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.NewTable();
		int c = 1;
		for (auto i : s->ModifyDamage.TechList) {
			L.Push(i);
			L.SetTableRaw(-2, c);
			c++;
		}
		return 1;
	}

	int GetMaxRangeModifierTechs(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.NewTable();
		int c = 1;
		for (auto i : s->ModifyMaxRange.TechList) {
			L.Push(i);
			L.SetTableRaw(-2, c);
			c++;
		}
		return 1;
	}

	int GetSpeedModifierTechs(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* s = dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps);
		if (!s)
			throw lua::LuaException("no settler entity at 1");
		L.NewTable();
		int c = 1;
		for (auto i : s->ModifySpeed.TechList) {
			L.Push(i);
			L.SetTableRaw(-2, c);
			c++;
		}
		return 1;
	}

	int AddHPTechMod(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supportet with SCELoader");
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsBuildingType())
			throw lua::LuaException("no building type at 1");
		auto tech = L.CheckEnum<shok::TechnologyId>(2);
		shok::Technology* techo = CppLogic::GetTechnology(tech);
		if (!techo)
			throw lua::LuaException("no tech at 2");
		EGL::CGLEEntity::BuildingMaxHpTechBoni.emplace(L.CheckEnum<shok::EntityTypeId>(1), tech);
		return 0;
	}

	int GetBuildOnTypes(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		if (!t->IsBuildingType())
			throw lua::LuaException("no building type at 1");
		L.NewTable();
		int i = 1;
		for (auto ty : dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)->BuildOn) {
			L.Push(ty);
			L.SetTableRaw(-2, i);
			i++;
		}
		return 1;
	}

	int LeaderTypeGetMaxNumberOfSoldiers(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto* pr = t->GetBehaviorProps<GGL::CLimitedAttachmentBehaviorProperties>();
		if (!pr)
			throw lua::LuaException("no limited attachment");
		std::string_view v = "ATTACHMENT_LEADER_SOLDIER";
		for (auto& l : pr->Attachments) {
			if (v == l.Type) {
				L.Push(l.Limit);
				return 1;
			}
		}
		throw lua::LuaException("no attachment limit ATTACHMENT_LEADER_SOLDIER");
	}

	int GetUpgradeCategory(luaext::State L) {
		auto ucat = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(shok::PlayerId::P1)->SettlerUpgradeManager->GetUpgradeCategoryOfEntityType(L.CheckEnum<shok::EntityTypeId>(1));
		L.Push(ucat);
		return 1;
	}

	int DumpBattleBehavior(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		auto b = t->GetBehaviorPropsDynamic<GGL::CBattleBehaviorProps>();
		if (b == nullptr)
			L.Push();
		else
			CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, b);
		return 1;
	}

	int GetDeleteWhenBuildOn(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(t->LogicProps->DeleteWhenBuiltOn);
		return 1;
	}
	int SetDeleteWhenBuildOn(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.CheckType(2, lua::LType::Boolean);
		t->LogicProps->DeleteWhenBuiltOn = L.ToBoolean(2);
		return 0;
	}

	int IsConvertible(luaext::State L) {
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

	int IsBuildingType(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(dynamic_cast<GGL::CGLBuildingProps*>(t->LogicProps)));
		return 1;
	}
	int IsSettlerType(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(dynamic_cast<GGL::CGLSettlerProps*>(t->LogicProps)));
		return 1;
	}
	int IsLeaderType(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(t->GetBehaviorPropsDynamic<GGL::CLeaderBehaviorProps>()));
		return 1;
	}
	int IsSoldierType(luaext::State L) {
		GGlue::CGlueEntityProps* t = L.CheckEntityType(1);
		L.Push(static_cast<bool>(t->GetBehaviorProps<GGL::CSoldierBehaviorProps>()));
		return 1;
	}

	constexpr std::array EntityType{
			luaext::FuncReference::GetRef<GetLimitedLifespanDuration>("GetLimitedLifespanDuration"),
			luaext::FuncReference::GetRef<SetLimitedLifespanDuration>("SetLimitedLifespanDuration"),
			luaext::FuncReference::GetRef<GetMaxHealth>("GetMaxHealth"),
			luaext::FuncReference::GetRef<SetMaxHealth>("SetMaxHealth"),
			luaext::FuncReference::GetRef<GetAffectMotivation>("GetAffectMotivation"),
			luaext::FuncReference::GetRef<SetAffectMotivation>("SetAffectMotivation"),
			luaext::FuncReference::GetRef<GetSuspendedAnimation>("GetSuspendedAnimation"),
			luaext::FuncReference::GetRef<SetSuspendedAnimation>("SetSuspendedAnimation"),
			luaext::FuncReference::GetRef<GetAutoAttackDamage>("GetAutoAttackDamage"),
			luaext::FuncReference::GetRef<SetAutoAttackDamage>("SetAutoAttackDamage"),
			luaext::FuncReference::GetRef<AddEntityCategory>("AddEntityCategory"),
			luaext::FuncReference::GetRef<RemoveEntityCategory>("RemoveEntityCategory"),
			luaext::FuncReference::GetRef<GetAutoAttackMissChance>("GetAutoAttackMissChance"),
			luaext::FuncReference::GetRef<SetAutoAttackMissChance>("SetAutoAttackMissChance"),
			luaext::FuncReference::GetRef<GetAutoAttackRange>("GetAutoAttackRange"),
			luaext::FuncReference::GetRef<SetAutoAttackRange>("SetAutoAttackRange"),
			luaext::FuncReference::GetRef<GetArmor>("GetArmor"),
			luaext::FuncReference::GetRef<SetArmor>("SetArmor"),
			luaext::FuncReference::GetRef<GetModels>("GetModels"),
			luaext::FuncReference::GetRef<SetModels>("SetModels"),
			luaext::FuncReference::GetRef<ResourceTreeTypeHasData>("ResourceTreeTypeHasData"),
			luaext::FuncReference::GetRef<ResourceTreeTypeGetData>("ResourceTreeTypeGetData"),
			luaext::FuncReference::GetRef<ResourceTreeTypeSetData>("ResourceTreeTypeSetData"),
			luaext::FuncReference::GetRef<GetBlocking>("GetBlocking"),
			luaext::FuncReference::GetRef<GetArmorModifierTechs>("GetArmorModifierTechs"),
			luaext::FuncReference::GetRef<GetExplorationModifierTechs>("GetExplorationModifierTechs"),
			luaext::FuncReference::GetRef<GetDeleteWhenBuildOn>("GetDeleteWhenBuildOn"),
			luaext::FuncReference::GetRef<SetDeleteWhenBuildOn>("SetDeleteWhenBuildOn"),
			luaext::FuncReference::GetRef<GetBattleWaitUntil>("GetBattleWaitUntil"),
			luaext::FuncReference::GetRef<SetBattleWaitUntil>("SetBattleWaitUntil"),
			luaext::FuncReference::GetRef<IsConvertible>("IsConvertible"),
			luaext::FuncReference::GetRef<IsBuildingType>("IsBuildingType"),
			luaext::FuncReference::GetRef<IsSettlerType>("IsSettlerType"),
			luaext::FuncReference::GetRef<IsLeaderType>("IsLeaderType"),
			luaext::FuncReference::GetRef<IsSoldierType>("IsSoldierType"),
		};

	constexpr std::array Settler{
			luaext::FuncReference::GetRef<LeaderTypeGetSoldierType>("LeaderTypeGetSoldierType"),
			luaext::FuncReference::GetRef<LeaderTypeSetSoldierType>("LeaderTypeSetSoldierType"),
			luaext::FuncReference::GetRef<LeaderTypeGetUpkeep>("LeaderTypeGetUpkeep"),
			luaext::FuncReference::GetRef<LeaderTypeSetUpkeep>("LeaderTypeSetUpkeep"),
			luaext::FuncReference::GetRef<LeaderTypeGetRegeneration>("LeaderTypeGetRegeneration"),
			luaext::FuncReference::GetRef<LeaderTypeSetRegeneration>("LeaderTypeSetRegeneration"),
			luaext::FuncReference::GetRef<LeaderTypeGetAutoAggressiveRange>("LeaderTypeGetAutoAggressiveRange"),
			luaext::FuncReference::GetRef<LeaderTypeSetAutoAggressiveRange>("LeaderTypeSetAutoAggressiveRange"),
			luaext::FuncReference::GetRef<ThiefTypeGetStealingInfo>("ThiefTypeGetStealingInfo"),
			luaext::FuncReference::GetRef<ThiefTypeSetStealingInfo>("ThiefTypeSetStealingInfo"),
			luaext::FuncReference::GetRef<GetAbilityDataCamouflage>("GetAbilityDataCamouflage"),
			luaext::FuncReference::GetRef<SetAbilityDataCamouflage>("SetAbilityDataCamouflage"),
			luaext::FuncReference::GetRef<GetAbilityDataCircularAttack>("GetAbilityDataCircularAttack"),
			luaext::FuncReference::GetRef<SetAbilityDataCircularAttack>("SetAbilityDataCircularAttack"),
			luaext::FuncReference::GetRef<GetAbilityDataShuriken>("GetAbilityDataShuriken"),
			luaext::FuncReference::GetRef<SetAbilityDataShuriken>("SetAbilityDataShuriken"),
			luaext::FuncReference::GetRef<GetAbilityDataSniper>("GetAbilityDataSniper"),
			luaext::FuncReference::GetRef<SetAbilityDataSniper>("SetAbilityDataSniper"),
			luaext::FuncReference::GetRef<GetAbilityDataRangedEffect>("GetAbilityDataRangedEffect"),
			luaext::FuncReference::GetRef<SetAbilityDataRangedEffect>("SetAbilityDataRangedEffect"),
			luaext::FuncReference::GetRef<GetAbilityDataResurrect>("GetAbilityDataResurrect"),
			luaext::FuncReference::GetRef<GetAbilityLightningStrikeData>("GetAbilityLightningStrikeData"),
			luaext::FuncReference::GetRef<GetAbilityBombardmentData>("GetAbilityBombardmentData"),
			luaext::FuncReference::GetRef<GetFearless>("GetFearless"),
			luaext::FuncReference::GetRef<SetFearless>("SetFearless"),
			luaext::FuncReference::GetRef<SettlerGetCost>("GetCost"),
			luaext::FuncReference::GetRef<SettlerSetCost>("SetCost"),
			luaext::FuncReference::GetRef<GetDamageModifierTechs>("GetDamageModifierTechs"),
			luaext::FuncReference::GetRef<GetMaxRangeModifierTechs>("GetMaxRangeModifierTechs"),
			luaext::FuncReference::GetRef<GetSpeedModifierTechs>("GetSpeedModifierTechs"),
			luaext::FuncReference::GetRef<LeaderTypeGetMaxNumberOfSoldiers>("LeaderTypeGetMaxNumberOfSoldiers"),
			luaext::FuncReference::GetRef<GetUpgradeCategory>("GetUpgradeCategory"),
			luaext::FuncReference::GetRef<DumpBattleBehavior>("DumpBattleBehavior"),
		};

	constexpr std::array Building{
			luaext::FuncReference::GetRef<GetSabotageFactor>("GetSabotageFactor"),
			luaext::FuncReference::GetRef<SetSabotageFactor>("SetSabotageFactor"),
			luaext::FuncReference::GetRef<GetVCAttractionSlotsProvided>("GetVCAttractionSlotsProvided"),
			luaext::FuncReference::GetRef<SetVCAttractionSlotsProvided>("SetVCAttractionSlotsProvided"),
			luaext::FuncReference::GetRef<GetConstructionCost>("GetConstructionCost"),
			luaext::FuncReference::GetRef<SetConstructionCost>("SetConstructionCost"),
			luaext::FuncReference::GetRef<GetUpgradeCost>("GetUpgradeCost"),
			luaext::FuncReference::GetRef<SetUpgradeCost>("SetUpgradeCost"),
			luaext::FuncReference::GetRef<AddHPTechMod>("AddHPTechMod"),
			luaext::FuncReference::GetRef<GetBuildOnTypes>("GetBuildOnTypes"),
		};

	void Init(luaext::State L)
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

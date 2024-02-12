#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "luaext.h"
#include "s5_events.h"
#include "s5_entity.h"
#include <magic_enum_all.hpp>

namespace CppLogic::LuaEventInterface {
	template<class Ev>
	Ev CheckEvent(luaext::EState L, shok::EventIDs id, int off) {
		throw 0;
	}

	template<>
	inline EGL::CEventPosition CheckEvent<EGL::CEventPosition>(luaext::EState L, shok::EventIDs id, int off) {
		return { id, L.CheckPos(off+1) };
	}
	template<>
	inline BB::CEvent CheckEvent<BB::CEvent>(luaext::EState L, shok::EventIDs id, int off) {
		return { id };
	}
	template<>
	inline GGL::CEventPositionAnd2EntityTypes CheckEvent<GGL::CEventPositionAnd2EntityTypes>(luaext::EState L, shok::EventIDs id, int off) {
		return { id, L.CheckPos(off + 1), L.CheckEnum<shok::EntityTypeId>(off + 2), L.CheckEnum<shok::EntityTypeId>(off + 3) };
	}
	template<>
	inline EGL::CEvent1Entity CheckEvent<EGL::CEvent1Entity>(luaext::EState L, shok::EventIDs id, int off) {
		auto e = L.CheckEntity(off + 1);
		if (e->IsDead())
			throw lua::LuaException{ "target is dead" };
		return { id, e->EntityId };
	}

	template<shok::AbilityId Ab, class Ev>
	void CheckEntityAbility(EGL::CGLEEntity* e, Ev& ev) {
		GGL::CEventHeroAbilityInteger issup{ shok::EventIDs::HeroAbility_IsAbilitySupported, Ab };
		e->FireEvent(&issup);
		if (issup.Data == 0)
			throw lua::LuaException{ "ability not supported" };
		GGL::CEventHeroAbilityInteger max{ shok::EventIDs::HeroAbility_GetChargeCurrent, Ab };
		GGL::CEventHeroAbilityInteger curr{ shok::EventIDs::HeroAbility_GetChargeMax, Ab };
		e->FireEvent(&max);
		e->FireEvent(&curr);
		if (max.Data != curr.Data)
			throw lua::LuaException{ "ability not ready" };
	}

	template<shok::DiploState D>
	void CheckEntityDiploState(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if constexpr (D == shok::DiploState::Friendly) {
			if (e->PlayerId == oth->PlayerId)
				return;
		}
		GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(e->PlayerId);
		if (!ps)
			throw lua::LuaException{ "no player status???" };
		if (ps->GetDiploStateTo(oth->PlayerId) != D)
			throw lua::LuaException{ std::format("target is not {}", magic_enum::enum_name(D)) };
	}

	inline void CheckConvertible(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (!oth->EventIsConvertible())
			throw lua::LuaException{ "target not convertible" };
	}

	inline void CheckSettler(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (dynamic_cast<GGL::CSettler*>(oth) == nullptr)
			throw lua::LuaException{ "target not a settler" };
	}
	inline void CheckBuilding(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (dynamic_cast<GGL::CBuilding*>(oth) == nullptr)
			throw lua::LuaException{ "target not a building" };
	}

	template<shok::EntityCategory EC>
	void CheckTargetCategory(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (!oth->IsEntityInCategory(EC))
			throw lua::LuaException{ std::format("target not of category {}", magic_enum::enum_name(EC)) };
	}

	template<class Ev>
	using EntityChecker = void(*)(EGL::CGLEEntity* e, Ev& ev);
	template<class Ev, shok::EventIDs ID, EntityChecker<Ev>... Checkers>
	int EntityCommandEvent(lua::State l) {
		luaext::EState L{ l };
		auto* e = L.CheckEntity(1);
		if (e->IsDead())
			throw lua::LuaException{ "entity is dead" };
		auto ev = CheckEvent<Ev>(L, ID, 1);
		(Checkers(e, ev), ...);
		e->FireEvent(&ev);
		return 0;
	}
}

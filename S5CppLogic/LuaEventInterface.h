#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "luaext.h"
#include "s5_events.h"
#include "s5_netevents.h"
#include "s5_entity.h"
#include "s5_ui.h"
#include <magic_enum/magic_enum.hpp>

namespace CppLogic::LuaEventInterface {
	template<class Ev>
	concept HasEventType = requires { typename Ev::EventType; };

	template<class Ev>
	struct EventType {
	};
	template<class Ev>
	requires (!HasEventType<Ev>)
	struct EventType<Ev> {
		using T = Ev;
	};
	template<class Ev>
	requires HasEventType<Ev>
	struct EventType<Ev> {
		using T = typename Ev::EventType;
	};

	template<class Ev>
	typename EventType<Ev>::T CheckEvent(luaext::EState L, shok::EventIDs id, int off) {
		throw 0;
	}
	template<class Ev>
	typename EventType<Ev>::T CheckEvent(luaext::EState L, shok::NetEventIds id, int off) {
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
		if (id != shok::EventIDs::CppL_Resurrect_Activate && e->IsDead())
			throw lua::LuaException{ "target is dead" };
		return { id, e->EntityId };
	}

	struct CNetEventEntityIDAndPlayerID_AutoPlayer {
		using EventType = EGL::CNetEventEntityIDAndPlayerID;
	};
	template<>
	inline EGL::CNetEventEntityIDAndPlayerID CheckEvent<CNetEventEntityIDAndPlayerID_AutoPlayer>(luaext::EState L, shok::NetEventIds id, int off) {
		auto e = L.CheckEntity(off + 1);
		if (e->IsDead())
			throw lua::LuaException{ "target is dead" };
		return { id, e->PlayerId, e->EntityId };
	}
	static_assert(!HasEventType<BB::CEvent>);
	static_assert(HasEventType<CNetEventEntityIDAndPlayerID_AutoPlayer>);
	static_assert(std::same_as<EventType<CNetEventEntityIDAndPlayerID_AutoPlayer>::T, EGL::CNetEventEntityIDAndPlayerID>);
	static_assert(std::same_as<EventType<EGL::CNetEventEntityIDAndPlayerID>::T, EGL::CNetEventEntityIDAndPlayerID>);

	template<>
	inline EGL::CNetEventEntityID CheckEvent<EGL::CNetEventEntityID>(luaext::EState L, shok::NetEventIds id, int off) {
		auto e = L.CheckEntity(off + 1);
		if (e->IsDead())
			throw lua::LuaException{ "target is dead" };
		return { id, e->EntityId };
	}

	template<>
	inline EGL::CNetEvent2Entities CheckEvent<EGL::CNetEvent2Entities>(luaext::EState L, shok::NetEventIds id, int off) {
		auto e = L.CheckEntity(off + 1);
		auto t = L.CheckEntity(off + 2);
		if (e->IsDead())
			throw lua::LuaException{ "entity is dead" };
		if (id != shok::NetEventIds::CppL_Resurrect_Activate && t->IsDead())
			throw lua::LuaException{ "target is dead" };
		return { id, e->EntityId, t->EntityId };
	}

	template<>
	inline GGL::CNetEventExtractResource CheckEvent<GGL::CNetEventExtractResource>(luaext::EState L, shok::NetEventIds id, int off) {
		auto e = L.CheckEntity(off + 1);
		auto t = L.OptEntity(off + 2);
		if (auto* r = dynamic_cast<GGL::CResourceDoodad*>(t))
			return { id, e->EntityId, r->ResourceType, t->Position };
		if (t != nullptr) {
			if (auto* r = static_cast<GGL::CEntityProperties*>(t->GetEntityType()->LogicProps))
				return { id, e->EntityId, shok::ResourceType::WoodRaw, t->Position };
		}
		return { id, e->EntityId, L.CheckEnum<shok::ResourceType>(off + 2), L.CheckPos(off + 3) };
	}

	template<>
	inline EGL::CNetEventEntityAndPos CheckEvent<EGL::CNetEventEntityAndPos>(luaext::EState L, shok::NetEventIds id, int off) {
		auto e = L.CheckEntity(off + 1);
		if (e->IsDead())
			throw lua::LuaException{ "entity is dead" };
		return { id, e->EntityId, L.CheckPos(off + 2) };
	}

	template<>
	inline GGL::CNetEventCannonCreator CheckEvent<GGL::CNetEventCannonCreator>(luaext::EState L, shok::NetEventIds id, int off) {
		auto e = L.CheckEntity(off + 1);
		if (e->IsDead())
			throw lua::LuaException{ "entity is dead" };
		shok::Position p = L.CheckPos(off + 2);
		return { id, e->EntityId, L.CheckEnum<shok::EntityTypeId>(off + 3), L.CheckEnum<shok::EntityTypeId>(off + 4), {p.X, p.Y, 0.0f} };
	}

	template<shok::AbilityId Ab, class Ev>
	void CheckEntityAbility(EGL::CGLEEntity* e, Ev& ev) {
		GGL::CEventHeroAbilityGetInteger issup{ shok::EventIDs::HeroAbility_IsAbilitySupported, Ab };
		e->FireEvent(&issup);
		if (issup.Data == 0)
			throw lua::LuaException{ "ability not supported" };
		GGL::CEventHeroAbilityGetInteger max{ shok::EventIDs::HeroAbility_GetChargeCurrent, Ab };
		GGL::CEventHeroAbilityGetInteger curr{ shok::EventIDs::HeroAbility_GetChargeMax, Ab };
		e->FireEvent(&max);
		e->FireEvent(&curr);
		if (max.Data != curr.Data)
			throw lua::LuaException{ "ability not ready" };
	}
	template<shok::AbilityId Ab, class Ev>
	void CheckEntityAbility(Ev& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		GGL::CEventHeroAbilityGetInteger issup{ shok::EventIDs::HeroAbility_IsAbilitySupported, Ab };
		e->FireEvent(&issup);
		if (issup.Data == 0)
			throw lua::LuaException{ "ability not supported" };
		GGL::CEventHeroAbilityGetInteger max{ shok::EventIDs::HeroAbility_GetChargeCurrent, Ab };
		GGL::CEventHeroAbilityGetInteger curr{ shok::EventIDs::HeroAbility_GetChargeMax, Ab };
		e->FireEvent(&max);
		e->FireEvent(&curr);
		if (max.Data != curr.Data)
			throw lua::LuaException{ "ability not ready" };
	}
	template<shok::AbilityId Ab>
	void CheckActorAbility(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		GGL::CEventHeroAbilityGetInteger issup{ shok::EventIDs::HeroAbility_IsAbilitySupported, Ab };
		e->FireEvent(&issup);
		if (issup.Data == 0)
			throw lua::LuaException{ "ability not supported" };
		GGL::CEventHeroAbilityGetInteger max{ shok::EventIDs::HeroAbility_GetChargeCurrent, Ab };
		GGL::CEventHeroAbilityGetInteger curr{ shok::EventIDs::HeroAbility_GetChargeMax, Ab };
		e->FireEvent(&max);
		e->FireEvent(&curr);
		if (max.Data != curr.Data)
			throw lua::LuaException{ "ability not ready" };
	}

	template<class Bh, class Ev>
	void CheckEntityBehavior(EGL::CGLEEntity* e, Ev& ev) {
		if (e->GetBehavior<Bh>() == nullptr)
			throw lua::LuaException{ std::format("has no {} behavior", typename_details::type_name<Bh>()) };
	}
	template<class Bh, class Ev>
	void CheckEntityBehavior(Ev& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (e->GetBehavior<Bh>() == nullptr)
			throw lua::LuaException{ std::format("has no {} behavior", typename_details::type_name<Bh>()) };
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
	inline void CheckEntitySamePlayer(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (e->PlayerId != oth->PlayerId)
			throw lua::LuaException{ "target is not of same player" };
	}

	inline void CheckConvertible(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (!oth->EventIsConvertible())
			throw lua::LuaException{ "target not convertible" };
	}
	inline void CheckConvertible(EGL::CNetEvent2Entities& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (!oth->EventIsConvertible())
			throw lua::LuaException{ "target not convertible" };
	}

	inline void CheckSettler(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (dynamic_cast<GGL::CSettler*>(oth) == nullptr)
			throw lua::LuaException{ "target not a settler" };
	}
	inline void CheckSettler(EGL::CNetEvent2Entities& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (dynamic_cast<GGL::CSettler*>(oth) == nullptr)
			throw lua::LuaException{ "target not a settler" };
	}
	inline void CheckBuilding(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (dynamic_cast<GGL::CBuilding*>(oth) == nullptr)
			throw lua::LuaException{ "target not a building" };
	}
	inline void CheckBuilding(EGL::CNetEvent2Entities& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (dynamic_cast<GGL::CBuilding*>(oth) == nullptr)
			throw lua::LuaException{ "target not a building" };
	}

	template<shok::EntityCategory EC>
	void CheckTargetCategory(EGL::CGLEEntity* e, EGL::CEvent1Entity& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (!oth->IsEntityInCategory(EC))
			throw lua::LuaException{ std::format("target not of category {}", magic_enum::enum_name(EC)) };
	}
	template<shok::EntityCategory EC>
	void CheckTargetCategory(EGL::CNetEvent2Entities& ev) {
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (!oth->IsEntityInCategory(EC))
			throw lua::LuaException{ std::format("target not of category {}", magic_enum::enum_name(EC)) };
	}

	template<class Ev>
	requires std::derived_from<Ev, EGL::CNetEventPlayerID>
	inline void CheckLocalPlayer(Ev& ev) {
		if (ev.PlayerID != GGUI::CManager::GlobalObj()->ControlledPlayer)
			throw lua::LuaException{ "player not controlled" };
	}

	template<class Ev>
	inline void CheckBuilding(Ev& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (dynamic_cast<GGL::CBuilding*>(e) == nullptr)
			throw lua::LuaException{ "not a building" };
	}

	template<shok::EntityCategory EC, class Ev>
	void CheckEntityCategory(Ev& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (!e->IsEntityInCategory(EC))
			throw lua::LuaException{ std::format("entity not of category {}", magic_enum::enum_name(EC)) };
	}
	template<shok::EntityCategory EC>
	void CheckEntityActorCategory(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		if (!e->IsEntityInCategory(EC))
			throw lua::LuaException{ std::format("entity not of category {}", magic_enum::enum_name(EC)) };
	}


	template<class Ev>
	void CheckEntityOfLocalPlayer(Ev& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID);
		if (e->PlayerId != GGUI::CManager::GlobalObj()->ControlledPlayer)
			throw lua::LuaException{ "player not controlled"};
	}
	template<>
	inline void CheckEntityOfLocalPlayer<EGL::CNetEvent2Entities>(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		if (e->PlayerId != GGUI::CManager::GlobalObj()->ControlledPlayer)
			throw lua::LuaException{ "player not controlled" };
	}

	template<shok::DiploState D>
	void CheckEntityDiploState(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
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
	inline void CheckEntitySamePlayer(EGL::CNetEvent2Entities& ev) {
		auto* e = EGL::CGLEEntity::GetEntityByID(ev.EntityID1);
		auto* oth = EGL::CGLEEntity::GetEntityByID(ev.EntityID2);
		if (e->PlayerId != oth->PlayerId)
			throw lua::LuaException{ "not same player" };
	}

	template<class Ev>
	using EntityChecker = void(*)(EGL::CGLEEntity* e, Ev& ev);
	template<class Ev, shok::EventIDs ID, EntityChecker<typename EventType<Ev>::T>... Checkers>
	int EntityCommandEvent(lua::State l) {
		luaext::EState L{ l };
		auto* e = L.CheckEntity(1);
		if (e->IsDead())
			throw lua::LuaException{ "entity is dead" };
		typename EventType<Ev>::T ev = CheckEvent<Ev>(L, ID, 1);
		(Checkers(e, ev), ...);
		e->FireEvent(&ev);
		return 0;
	}

	template<class Ev>
	using NetChecker = void(*)(Ev& ev);
	template<class Ev, shok::NetEventIds ID, NetChecker<typename EventType<Ev>::T>... Checkers>
	int NetEvent(lua::State l) {
		luaext::EState L{ l };
		typename EventType<Ev>::T ev = CheckEvent<Ev>(L, ID, 0);
		(Checkers(ev), ...);
		GGUI::CManager::PostEventFromUI(&ev);
		return 0;
	}
}

#pragma once

#include <Luapp/luapp50.h>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/s5_defines.h>
#include <shok/globals/s5_idmanager.h>
#include <format>

namespace luaext {template<class S>
	class StateExtension;

	using State = lua::State::BindExtensions<StateExtension>;

	namespace detail {
		EGL::CGLEEntity* OptEntity(State L, int i);
		EGL::CEffect* OptEffect(State L, int i);
		shok::EntityId OptEntityId(State L, int i);
		void PushPos(State L, const shok::Position& p);
		shok::Position CheckPos(State L, int idx);
		shok::PositionRot CheckPosRot(State L, int idx, bool rad = false);
		void PushPosRot(State L, const shok::PositionRot& p, bool rad = false);
		void CheckEntityAlive(shok::EntityId id, const char* msg);
		void ReadCostInfo(State L, int idx, shok::CostInfo& c, bool ignorezeroes);
		void PushCostInfo(State L, const shok::CostInfo& c);
		GGlue::CGlueEntityProps* OptEntityType(State L, int idx);
		shok::Technology* CheckTech(State L, int idx);
		shok::PlayerId CheckPlayerId(State L, int idx, bool allowZero = true);
		EGUIX::CBaseWidget* CheckWidget(State L, int idx);
	}

	template<class S>
	class StateExtension {
	public:
		EGL::CGLEEntity* OptEntity(int i) {
			return detail::OptEntity(static_cast<S*>(this), i);
		}
		EGL::CGLEEntity* CheckEntity(int i) {
			EGL::CGLEEntity* d = OptEntity(i);
			if (!d)
				throw lua::LuaException{ std::format("no entity at argument {}", i) };
			return d;
		}
		GGL::CSettler* OptSettler(int i) {
			return dynamic_cast<GGL::CSettler*>(OptEntity(i));
		}
		GGL::CSettler* CheckSettler(int i) {
			GGL::CSettler* s = OptSettler(i);
			if (!s)
				throw lua::LuaException{ std::format("no settler at argument {}", i) };
			return s;
		}
		GGL::CBuilding* OptBuilding(int i) {
			return dynamic_cast<GGL::CBuilding*>(OptEntity(i));
		}
		GGL::CBuilding* CheckBuilding(int i) {
			GGL::CBuilding* b = OptBuilding(i);
			if (!b)
				throw lua::LuaException{ std::format("no building at argument {}", i) };
			return b;
		}
		GGL::CResourceDoodad* OptResourceDoodad(int i) {
			return dynamic_cast<GGL::CResourceDoodad*>(OptEntity(i));
		}
		GGL::CResourceDoodad* CheckResourceDoodad(int i) {
			GGL::CResourceDoodad* d = OptResourceDoodad(i);
			if (!d)
				throw lua::LuaException{ std::format("no resource entity at argument {}", i) };
			return d;
		}
		EGL::CEffect* OptEffect(int i) {
			return detail::OptEffect(static_cast<S*>(this), i);
		}
		EGL::CEffect* CheckEffect(int i) {
			EGL::CEffect* d = OptEffect(i);
			if (!d)
				throw lua::LuaException{ std::format("no effect at argument %d", i) };
			return d;
		}

		shok::EntityId OptEntityId(int i) {
			return detail::OptEntityId(static_cast<S*>(this), i);
		}

		void Push(const shok::Position& p) {
			return detail::PushPos(static_cast<S*>(this), p);
		}
		shok::Position CheckPos(int idx) {
			return detail::CheckPos(static_cast<S*>(this), idx);
		}
		shok::PositionRot CheckPosRot(int idx, bool rad = false) {
			return detail::CheckPosRot(static_cast<S*>(this), idx, rad);
		}
		void Push(const shok::PositionRot& p, bool rad = false) {
			return detail::PushPosRot(static_cast<S*>(this), p, rad);
		}

		static void CheckEntityAlive(shok::EntityId id, const char* msg) {
			detail::CheckEntityAlive(id, msg);
		}

		void ReadCostInfo(int idx, shok::CostInfo& c, bool ignorezeroes) {
			return detail::ReadCostInfo(static_cast<S*>(this), idx, c, ignorezeroes);
		}
		void Push(const shok::CostInfo& c) {
			return detail::PushCostInfo(static_cast<S*>(this), c);
		}

		GGlue::CGlueEntityProps* OptEntityType(int idx) {
			return detail::OptEntityType(static_cast<S*>(this), idx);
		}
		GGlue::CGlueEntityProps* CheckEntityType(int idx) {
			GGlue::CGlueEntityProps* t = OptEntityType(idx);
			if (t == nullptr)
				throw lua::LuaException{ std::format("no entitytype at {}", idx) };
			return t;
		}

		void StringToLower() {
			auto* th = static_cast<S*>(this);
			th->Push("string");
			th->GetGlobal();
			th->Push("lower");
			th->GetTableRaw(-2);
			th->Remove(-2);
			th->Insert(-2);
			th->TCall(1, 1, 0);
		}

		shok::Technology* CheckTech(int idx) {
			return detail::CheckTech(static_cast<S*>(this), idx);
		}
		shok::PlayerId CheckPlayerId(int idx, bool allowZero = true) {
			return detail::CheckPlayerId(static_cast<S*>(this), idx, allowZero);
		}
		shok::PlayerId OptPlayerId(int idx, shok::PlayerId def, bool allowZero = true) {
			auto* th = static_cast<S*>(this);
			if (!th->IsNoneOrNil(idx))
				return CheckPlayerId(idx, allowZero);
			return def;
		}

		EGUIX::CBaseWidget* CheckWidget(int idx) {
			return detail::CheckWidget(static_cast<S*>(this), idx);
		}

		template<class En>
		requires std::is_enum_v<En>
		void Push(En id) {
			auto* th = static_cast<S*>(this);
			th->Push(static_cast<std::underlying_type_t<En>>(id));
		}
		template<class En>
		requires std::is_enum_v<En>
		En CheckEnum(int idx, bool allowZero = false) {
			auto* th = static_cast<S*>(this);
			auto t = th->Type(idx);
			if (t == lua::LType::Number) {
				En id = static_cast<En>(th->CheckInt(idx));
				if (allowZero && id == static_cast<En>(0))
					return id;
				auto mng = CppLogic::GetIdManager<En>();
				if (mng.GetNameByID(id) == nullptr) {
					throw lua::LuaException{std::format("invalid {} at {}: {} does not exist", typename_details::type_name<En>(), idx, static_cast<int>(id))};
				}
				return id;
			}
			else if (t == lua::LType::String) {
				const char* s = th->ToString(idx);
				auto mng = CppLogic::GetIdManager<En>();
				En id = mng.GetIdByName(s);
				if constexpr (requires {
					{En::Invalid};
				}) {
					if (id == En::Invalid) {
						throw lua::LuaException{std::format("invalid {} at {}: {} does not exist", typename_details::type_name<En>(), idx, s)};
					}
				}
				return id;
			}
			else {
				throw lua::LuaException{std::format("invalid {} at {}: invalid type", typename_details::type_name<En>(), idx)};
			}
		}
		template<class En>
		requires std::is_enum_v<En>
		En OptEnum(int idx, En d = static_cast<En>(0)) {
			auto* th = static_cast<S*>(this);
			auto t = th->Type(idx);
			if (t == lua::LType::Number || t == lua::LType::String) {
				return th->template CheckEnum<En>(idx);
			}
			return d;
		}

		template<class T>
		requires std::same_as<T, shok::PlayerId>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckPlayerId(i);
		}
		template<class En>
		requires std::is_enum_v<En>
		En Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->template CheckEnum<En>(i);
		}
		template<class T>
		requires std::same_as<T, EGL::CGLEEntity>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckEntity(i);
		}
		template<class T>
		requires std::same_as<T, GGL::CSettler>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckSettler(i);
		}
		template<class T>
		requires std::same_as<T, GGL::CBuilding>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckBuilding(i);
		}
		template<class T>
		requires std::same_as<T, shok::Technology>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckTech(i);
		}
		template<class T>
		requires std::same_as<T, GGlue::CGlueEntityProps>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckEntityType(i);
		}
		template<class T>
		requires std::same_as<T, EGUIX::CBaseWidget>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckWidget(i);
		}
		template<class T>
		requires std::same_as<T, shok::Position>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckPos(i);
		}
		template<class T>
		requires std::same_as<T, shok::PositionRot>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckPosRot(i);
		}
		template<class T>
		requires std::same_as<T, EGL::CEffect>
		T Check(int i) {
			auto* th = static_cast<S*>(this);
			return th->CheckEffect(i);
		}
	};

	using CppFunction = State::CppFunction;
	using ComparisonOperator = State::ComparisonOperator;
	using ArihmeticOperator = State::ArihmeticOperator;
	using DebugInfo = State::DebugInfo;
	using DebugInfoOptions = State::DebugInfoOptions;
	using MetaEvent = State::MetaEvent;
	using FuncReference = State::FuncReference;
	using ErrorCode = State::ErrorCode;
	using ActivationRecord = State::ActivationRecord;
	using HookEvent = State::HookEvent;
	using UniqueState = lua::UniqueState::BindExtensions<StateExtension>;
	using Reference = State::Reference;
}

int str_ends_with(const char* str, const char* suffix);

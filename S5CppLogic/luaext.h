#pragma once

#include "luapp/luapp50.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_idmanager.h"
#include <format>

namespace luaext {
	class EState : public lua::State {
	public:
		EState(lua::State L);
		EState(lua_State* L);

		EGL::CGLEEntity* CheckEntity(int i);
		EGL::CGLEEntity* OptEntity(int i);
		GGL::CSettler* CheckSettler(int i);
		GGL::CSettler* OptSettler(int i);
		GGL::CBuilding* CheckBuilding(int i);
		GGL::CBuilding* OptBuilding(int i);
		GGL::CResourceDoodad* CheckResourceDoodad(int i);
		GGL::CResourceDoodad* OptResourceDoodad(int i);
		EGL::CEffect* OptEffect(int i);
		EGL::CEffect* CheckEffect(int i);

		shok::EntityId OptEntityId(int i);

		void PushPos(const shok::Position& p);
		shok::Position CheckPos(int idx);
		shok::PositionRot CheckPosRot(int idx, bool rad = false);
		void PushPosRot(const shok::PositionRot& p, bool rad = false);

		void CheckEntityAlive(shok::EntityId id, const char* msg);

		void ReadCostInfo(int idx, shok::CostInfo& c, bool ignorezeroes);
		void PushCostInfo(const shok::CostInfo& c);

		GGlue::CGlueEntityProps* OptEntityType(int idx);
		GGlue::CGlueEntityProps* CheckEntityType(int idx);

		void StringToLower();

		shok::Technology* CheckTech(int idx);
		shok::PlayerId CheckPlayerId(int idx, bool allowZero = true);
		shok::PlayerId OptPlayerId(int idx, shok::PlayerId def, bool allowZero = true);

		EGUIX::CBaseWidget* CheckWidget(int idx);

		using State::Push;
		template<class En>
		requires std::is_enum_v<En>
		void Push(En id) {
			Push(static_cast<std::underlying_type_t<En>>(id));
		}
		template<class En>
		requires std::is_enum_v<En>
		En CheckEnum(int idx, bool allowZero = false) {
			auto t = Type(idx);
			if (t == lua::LType::Number) {
				En id = static_cast<En>(ToInteger(idx));
				if (allowZero && id == static_cast<En>(0))
					return id;
				auto mng = CppLogic::GetIdManager<En>();
				if (mng.GetNameByID(id) == nullptr) {
					throw lua::LuaException{std::format("invalid {} at {}: {} does not exist", typename_details::type_name<En>(), idx, static_cast<int>(id))};
				}
				return id;
			}
			else if (t == lua::LType::String) {
				const char* s = ToString(idx);
				auto mng = CppLogic::GetIdManager<En>();
				En id = mng.GetIdByName(s);
				if (id == static_cast<En>(0)) {
					throw lua::LuaException{std::format("invalid {} at {}: {} does not exist", typename_details::type_name<En>(), idx, s)};
				}
				return id;
			}
			else {
				throw lua::LuaException{std::format("invalid {} at {}: invalid type", typename_details::type_name<En>(), idx)};
			}
		}
		template<class En>
		requires std::is_enum_v<En>
		En OptEnum(int idx) {
			auto t = Type(idx);
			if (t == lua::LType::Number || t == lua::LType::String) {
				return CheckEnum<En>(idx);
			}
			return static_cast<En>(0);
		}
	};
}

int str_ends_with(const char* str, const char* suffix);

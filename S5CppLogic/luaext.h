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

		int OptEntityId(int i);

		void PushPos(const shok::Position& p);
		shok::Position CheckPos(int idx);
		shok::PositionRot CheckPosRot(int idx, bool rad = false);
		void PushPosRot(const shok::PositionRot& p, bool rad = false);

		void CheckEntityAlive(int id, const char* msg);

		void ReadCostInfo(int idx, shok::CostInfo& c, bool ignorezeroes);
		void PushCostInfo(const shok::CostInfo& c);

		GGlue::CGlueEntityProps* OptEntityType(int idx);
		GGlue::CGlueEntityProps* CheckEntityType(int idx);

		void StringToLower();

		shok::ResourceType CheckResourceType(int idx);

		shok::Technology* CheckTech(int idx);

		EGUIX::CBaseWidget* CheckWidget(int idx);

		template<class En>
		requires std::is_enum_v<En>
		void PushEnum(En id) {
			Push(static_cast<std::underlying_type_t<En>>(id));
		}
		template<class En>
		requires std::is_enum_v<En>
		En CheckEnum(int idx) {
			En id = static_cast<En>(CheckInt(idx));
			auto mng = CppLogic::GetIdManager<En>();
			if (mng.GetNameByID(id) == nullptr) {
				throw lua::LuaException{std::format("invalid {} at {}: {} does not exist", typename_details::type_name<En>(), idx, static_cast<int>(id))};
			}
			return id;
		}
	};
}

int str_ends_with(const char* str, const char* suffix);

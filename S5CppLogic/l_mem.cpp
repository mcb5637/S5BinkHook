#include "pch.h"
#include "l_mem.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entitytype.h"
#include "s5_entity.h"
#include "s5_effecttype.h"
#include "s5_effects.h"
#include "s5_player.h"
#include "s5_maplogic.h"
#include "luaext.h"
#include "luaserializer.h"
#include <array>

namespace CppLogic::Mem {
	int SetFPU(lua::State L) {
		shok::SetHighPrecFPU();
		return 0;
	}

	int EntityType(lua::State l) {
		luaext::EState L{ l };
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.EntityType", L.CheckEntityType(1));
		return 1;
	}

	int Entity(lua::State l) {
		luaext::EState L{ l };
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.Entity", L.CheckEntity(1));
		return 1;
	}

	int EffectType(lua::State l) {
		luaext::EState L{ l };
		auto id = L.CheckEnum<shok::EffectTypeId>(1);
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.EffectType", GetEffectType(id), EGL::EffectType::SerializationData);
		return 1;
	}

	int Effect(lua::State l) {
		luaext::EState L{ l };
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.Effect", L.CheckEffect(1));
		return 1;
	}

	int Player(lua::State l) {
		luaext::EState L{ l };
		auto p = L.CheckPlayerId(1, false);
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.Player", (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p));
		return 1;
	}

	constexpr std::array Mem = {
		lua::FuncReference::GetRef<SetFPU>("SetFPU"),
		lua::FuncReference::GetRef<EntityType>("EntityType"),
		lua::FuncReference::GetRef<Entity>("Entity"),
		lua::FuncReference::GetRef<EffectType>("EffectType"),
		lua::FuncReference::GetRef<Effect>("Effect"),
		lua::FuncReference::GetRef<Player>("Player"),
	};

	void Init(lua::State L)
	{
		L.RegisterFuncs(Mem, -3);

		L.Push("ObjectAccessType");
		CppLogic::GetIdManager<CppLogic::Serializer::ObjectAccess::Type>().PushToState(L);
		L.SetTableRaw(-3);
	}
}
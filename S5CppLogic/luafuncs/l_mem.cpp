#include "l_mem.h"
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <shok/entitytype/s5_entitytype.h>
#include <shok/entity/s5_entity.h>
#include <shok/effect/s5_effecttype.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <shok/effect/s5_effects.h>
#include <shok/player/s5_player.h>
#include <shok/globals/s5_maplogic.h>
#include <luaext.h>
#include <utility/luaserializer.h>
#include <array>

namespace CppLogic::Mem {
	int SetFPU(luaext::State L) {
		shok::SetHighPrecFPU();
		return 0;
	}

	int EntityType(luaext::State L) {
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.EntityType", static_cast<BB::IObject*>(L.CheckEntityType(1)));
		return 1;
	}

	int Entity(luaext::State L) {
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.Entity", static_cast<BB::IObject*>(L.CheckEntity(1)));
		return 1;
	}

	int EffectType(luaext::State L) {
		auto id = L.CheckEnum<shok::EffectTypeId>(1);
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.EffectType", GetEffectType(id), EGL::EffectType::SerializationData);
		return 1;
	}

	int Effect(luaext::State L) {
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.Effect", static_cast<BB::IObject*>(L.CheckEffect(1)));
		return 1;
	}

	int Player(luaext::State L) {
		auto p = L.CheckPlayerId(1, false);
		CppLogic::Serializer::ObjectAccess::PushObject(L, "Mem.Player", (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p));
		return 1;
	}

	constexpr std::array Mem = {
		luaext::FuncReference::GetRef<SetFPU>("SetFPU"),
		luaext::FuncReference::GetRef<EntityType>("EntityType"),
		luaext::FuncReference::GetRef<Entity>("Entity"),
		luaext::FuncReference::GetRef<EffectType>("EffectType"),
		luaext::FuncReference::GetRef<Effect>("Effect"),
		luaext::FuncReference::GetRef<Player>("Player"),
	};

	void Init(luaext::State L)
	{
		L.RegisterFuncs(Mem, -3);

		L.Push("ObjectAccessType");
		CppLogic::GetIdManager<CppLogic::Serializer::ObjectAccess::Type>().PushToState(L);
		L.SetTableRaw(-3);
	}
}
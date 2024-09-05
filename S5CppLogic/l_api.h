#pragma once

#include <random>
#include "luaext.h"
#include "luaserializer.h"

namespace CppLogic::API {
	void Init(lua::State L);
	void OnSaveLoaded(lua::State L);
	void Cleanup(lua::State L);

	class RNG {
		std::minstd_rand Core;

		static int Int(lua::State L);
		static int Number(lua::State L);
		static int Serialize(lua::State L);
		static int Deserialize(lua::State L);

	public:
		RNG();
		RNG(unsigned int seed);

		static constexpr std::array<lua::FuncReference, 2> LuaMethods{ {
				lua::FuncReference::GetRef<Int>("Int"),
				lua::FuncReference::GetRef<Number>("Number"),
			}};
		static constexpr std::array<lua::FuncReference, 1> LuaMetaMethods{ {
				lua::FuncReference::GetRef<Serialize>(CppLogic::Serializer::AdvLuaStateSerializer::UserdataSerializerMetaEvent),
			} };

		static void Register(lua::State L);
	};
}


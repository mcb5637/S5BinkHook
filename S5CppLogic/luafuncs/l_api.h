#pragma once

#include <random>
#include <luaext.h>
#include <utility/luaserializer.h>

namespace CppLogic::API {
	void Init(luaext::State L);
	void OnSaveLoaded(luaext::State L);
	void Cleanup(luaext::State L);

	class RNG {
		std::minstd_rand Core;

		static int Int(luaext::State L);
		static int Number(luaext::State L);
		static int Deserialize(luaext::State L);

	public:
		RNG();

		explicit RNG(unsigned int seed);

		static int Serialize(luaext::State L);

		static constexpr std::array LuaMethods{
				luaext::FuncReference::GetRef<Int>("Int"),
				luaext::FuncReference::GetRef<Number>("Number"),
			};

		static void Register(luaext::State L);
	};
}


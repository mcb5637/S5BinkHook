#include "pch.h"
#include "l_mem.h"

#include "luaimport.h"
#include "luaext.h"
#include "s5data.h"
#include <array>

namespace CppLogic::Mem {

	int SetFPU(lua::State L) {
		shok_SetHighPrecFPU();
		return 0;
	}


	constexpr std::array<lua::FuncReference, 1> Mem = { {
		lua::FuncReference::GetRef<SetFPU>("SetFPU"),
		}
	};

	void Init(lua::State L)
	{
		L.RegisterFuncs(Mem, -3);
	}
}
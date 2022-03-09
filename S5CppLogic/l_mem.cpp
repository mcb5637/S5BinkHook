#include "pch.h"
#include "l_mem.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "luaext.h"
#include <array>

namespace CppLogic::Mem {
	int SetFPU(lua::State L) {
		shok::SetHighPrecFPU();
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
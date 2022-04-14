#pragma once
#include "s5_forwardDecls.h"

namespace CppLogic::Hooks {
	void RedirectCall(void* call, void* redirect);
	void RedirectCallVP(void* call, void* redirect);
	long long WriteJump(void* adr, void* toJump);
	static_assert(sizeof(long long) == 8);
	void WriteNops(void* adr, int num);


	// allows read/write/execute of the memory location pointed to until it goes out of scope.
	// using more than one at the same time works as expected, cause the destructors are called in reverse order.
	// use always as stack variable!
	struct SaveVirtualProtect {
		SaveVirtualProtect();
		SaveVirtualProtect(void* adr, size_t size);
		~SaveVirtualProtect();
	private:
		void* Adr;
		size_t Size;
		unsigned long Prev;
	};
}

namespace CppLogic {
	bool HasSCELoader();
}
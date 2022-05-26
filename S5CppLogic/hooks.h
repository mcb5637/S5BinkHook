#pragma once
#include "s5_forwardDecls.h"
#include <vector>

namespace CppLogic::Hooks {
	void RedirectCall(void* call, void* redirect);
	void WriteJump(void* adr, void* toJump, void* nextvalid);
	void WriteJump(void* adr, void* toJump, void* nextvalid, byte* backup);
	void RestoreJumpBackup(void* adr, byte* backup);
	void WriteNops(void* adr, int num);
	void WriteNops(void* adr, void* nextvalid);


	// allows read/write/execute of the memory location pointed to until it goes out of scope.
	// using more than one at the same time works as expected, cause the destructors are called in reverse order.
	// use always as stack variable!
	struct SaveVirtualProtect {
		SaveVirtualProtect();
		SaveVirtualProtect(void* adr, size_t size);
		~SaveVirtualProtect();
		SaveVirtualProtect(size_t size, std::initializer_list<void*> adrs);
	private:
		void* Adr;
		size_t Size;
		unsigned long Prev;
	};


	class CheckMemFree {
	public:
		static std::vector<void*> ToFree;

		static void __stdcall OnFree(void* p);
		static void EnableHook();
	};
}

namespace CppLogic {
	bool HasSCELoader();
}
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

	template<class PointerType>
	requires std::is_member_function_pointer_v<PointerType>
	union MemberFuncPointer {
		friend void* MemberFuncPointerToVoid<PointerType>(PointerType, ptrdiff_t);
		friend PointerType VoidToMemberFuncPointer<PointerType>(void*, ptrdiff_t);
	private:
		PointerType Pointer = nullptr;
		struct {
			void* Pointer;
			ptrdiff_t Off;
		} RawOff;
		struct {
			void* Pointer;
		} Raw;
	};
	template<class PointerType>
	requires std::is_member_function_pointer_v<PointerType>
	void* MemberFuncPointerToVoid(PointerType p, ptrdiff_t expectedOff)
	{
		MemberFuncPointer<PointerType> d{};
		d.Pointer = p;
		if constexpr (sizeof(PointerType) == 4) {
			if (expectedOff != 0)
				throw std::logic_error{ "unexpected offset" };
			return d.Raw.Pointer;
		}
		else if constexpr (sizeof(PointerType) == 8) {
			if (expectedOff != d.RawOff.Off)
				throw std::logic_error{ "unexpected offset" };
			return d.RawOff.Pointer;
		}
		else {
			throw std::logic_error{ "unexpected size" };
		}
	}
	template<class PointerType>
	requires std::is_member_function_pointer_v<PointerType>
	PointerType VoidToMemberFuncPointer(void* p, ptrdiff_t off)
	{
		MemberFuncPointer<PointerType> d{};
		if constexpr (sizeof(PointerType) == 4) {
			if (off != 0)
				throw std::logic_error{ "unexpected offset" };
			d.Raw.Pointer = p;
		}
		else if constexpr (sizeof(PointerType) == 8) {
			d.RawOff.Pointer = p;
			d.RawOff.Off = off;
		}
		else {
			throw std::logic_error{ "unexpected size" };
		}
		return d.Pointer;
	}
}

namespace CppLogic {
	bool HasSCELoader();
}

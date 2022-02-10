#pragma once
#include "s5data.h"


namespace shok {

	// allocator
	static inline void* (__cdecl* const Malloc)(size_t t) = reinterpret_cast<void* (__cdecl*)(size_t)>(0x5C4181);
	static inline void* (__cdecl* const New)(size_t t) = reinterpret_cast<void* (__cdecl*)(size_t)>(0x5C04FB);
	static inline void(__cdecl* const Free)(void* p) = reinterpret_cast<void(__cdecl*)(void* p)>(0x5C2E2D);


	template<class T>
	struct Allocator {
		typedef T value_type;
		Allocator() = default;
		template<class U> constexpr Allocator(const Allocator<U>&) noexcept {}

		[[nodiscard]] T* allocate(size_t n) noexcept
		{
			void* p = Malloc(n * sizeof(T));
			return static_cast<T*>(p);
		}
		void deallocate(T* p, size_t n) noexcept
		{
			Free(p);
		}
	};
	template<class T, class U>
	bool operator==(const Allocator<T>&, const Allocator<U>&) { return true; }
	template<class T, class U>
	bool operator!=(const Allocator<T>&, const Allocator<U>&) { return false; }


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

#pragma once

namespace shok {
	// allocator
	static inline void* (__cdecl* const Malloc)(size_t t) = reinterpret_cast<void* (__cdecl*)(size_t)>(0x5C4181);
	static inline void* (__cdecl* const New)(size_t t) = reinterpret_cast<void* (__cdecl*)(size_t)>(0x5C04FB);
	static inline void(__cdecl* const Free)(void* p) = reinterpret_cast<void(__cdecl*)(void* p)>(0x5C2E2D);

	using FreeCaller = decltype([](void* p) { Free(p); });

	template<class T>
	struct Allocator {
		typedef T value_type;
		Allocator() = default;
		template<class U> constexpr Allocator(const Allocator<U>&) noexcept {}

		// ReSharper disable once CppMemberFunctionMayBeStatic
		[[nodiscard]] T* allocate(size_t n) noexcept
		{
			void* p = Malloc(n * sizeof(T));
			return static_cast<T*>(p);
		}
		// ReSharper disable once CppMemberFunctionMayBeStatic
		void deallocate(T* p, size_t n) noexcept
		{
			Free(p);
		}
	};
	template<class T, class U>
	bool operator==(const Allocator<T>&, const Allocator<U>&) { return true; }
	template<class T, class U>
	bool operator!=(const Allocator<T>&, const Allocator<U>&) { return false; }
}

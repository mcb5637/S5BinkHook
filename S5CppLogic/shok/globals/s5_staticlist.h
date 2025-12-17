#pragma once
#include <shok/s5_baseDefs.h>
#include <magic_enum/magic_enum.hpp>

namespace EGL {
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticEntry {
	public:
		virtual void Apply() const = 0;

		int Id = 0;
		shok::String Name;
		CStaticEntry* Prev = nullptr;

		static inline constexpr int vtp = 0x766CDC;
	};
	static_assert(offsetof(CStaticEntry, Prev) == 4 * 9);

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticListEx {
	public:
		virtual void Add(CStaticEntry* e) = 0;
		virtual void ApplyAll() const = 0;

		CStaticEntry* First = nullptr;

		static inline constexpr int vtp = 0x766B44;

		static inline CStaticListEx* (__cdecl* const GlobalHeroAbilities)() = reinterpret_cast<CStaticListEx * (__cdecl*)()>(0x498312);
		// every category is in there multiple times ([26, 29] times). somebody probably put the initialization into a header xD
		static inline CStaticListEx* (__cdecl* const GlobalEntityCategories)() = reinterpret_cast<CStaticListEx * (__cdecl*)()>(0x449D04);
	};



	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticEntityTypeID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x766CEC;
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticEffectTypeID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x76EB20;
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticTaskListID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x76FF54;
	};
}

namespace GGL {
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticHeroAbilityID : public EGL::CStaticEntry {
	public:
		virtual void Apply() const override;

		static inline constexpr int vtp = 0x7768AC;

		// creates and adds it to the GlobalHeroAbilities list
		CStaticHeroAbilityID(const char* name, shok::AbilityId id);

		template<shok::AbilityId Id>
		static CStaticHeroAbilityID Create() {
			static_assert(!magic_enum::enum_name(Id).empty());
			return CStaticHeroAbilityID{ magic_enum::enum_name(Id).data(), Id };
		}
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticArmorClassID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x778E6C;
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticCategoryID : public EGL::CStaticEntry {
	public:
		virtual void Apply() const override;

		static inline constexpr int vtp = 0x766CE4;

		// creates and adds it to the GlobalHeroAbilities list (you cannot define the int value of the id, but it is getting set by Apply)
		CStaticCategoryID(const char* name);
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CStaticAttachmentTypeID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x770B20;
	};
}

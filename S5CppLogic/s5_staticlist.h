#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include <magic_enum/magic_enum.hpp>

namespace EGL {
	class CStaticEntry {
	public:
		virtual void Apply() const = 0;

		int Id = 0;
		shok::String Name;
		CStaticEntry* Prev = nullptr;

		static inline constexpr int vtp = 0x766CDC;
	};
#ifdef __INTELLISENSE__
#pragma diag_suppress 2784
#endif
	static_assert(offsetof(CStaticEntry, Prev) == 4 * 9);
#ifdef __INTELLISENSE__
#pragma diag_default 2784
#endif

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



	class CStaticEntityTypeID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x766CEC;
	};

	class CStaticEffectTypeID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x76EB20;
	};

	class CStaticTaskListID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x76FF54;
	};
}

namespace GGL {
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

	class CStaticArmorClassID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x778E6C;
	};

	class CStaticCategoryID : public EGL::CStaticEntry {
	public:
		virtual void Apply() const override;

		static inline constexpr int vtp = 0x766CE4;

		// creates and adds it to the GlobalHeroAbilities list (you cannot define the int value of the id, but it is getting set by Apply)
		CStaticCategoryID(const char* name);
	};

	class CStaticAttachmentTypeID : public EGL::CStaticEntry {
	public:
		static inline constexpr int vtp = 0x770B20;
	};
}

#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_player.h"

namespace EGL {
	class CGLEAnimCategory : public BB::IObject {
	public:
		int Name; // anim category
		int Animation;


		static inline constexpr int vtp = 0x776C6C;
		static inline constexpr unsigned int Identifier = 0xCEB17718;
	};

	class CGLEAnimSet : public BB::IObject {
	public:
		int Id = 0;
		shok::Vector<CGLEAnimCategory*> Anims;


		static inline constexpr int vtp = 0x784A90;
		static inline constexpr unsigned int Identifier = 0xA4C5BB88;

		CGLEAnimSet();
		CGLEAnimSet(const CGLEAnimSet& o) = delete;
		CGLEAnimSet(CGLEAnimSet&& o) noexcept = delete;

		int GetAnim(int category);

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual ~CGLEAnimSet() override;
		virtual unsigned int __stdcall GetClassIdentifier() const override;
	private:
		void SetVT();
	};
	static_assert(sizeof(CGLEAnimSet) == 4 * 6);
}

namespace GGL {
	class CGLGoodDependentAnim {
	public:
		int GoodType;
		int Animation;

		static inline constexpr int vtp = 0x776C64;
		static inline constexpr unsigned int Identifier = 0xB240C758;

		virtual unsigned int __stdcall GetClassIdentifier() const;
	};
	static_assert(sizeof(CGLGoodDependentAnim) == 4 * 3);
	class CGLAnimCategory : public EGL::CGLEAnimCategory {
	public:
		shok::Vector<CGLGoodDependentAnim> GoodDependentAnim;

		static inline constexpr int vtp = 0x776C7C;
		static inline constexpr unsigned int Identifier = 0xA7B5DFB8;
	};
}


#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_player.h"

namespace EGL {
	class CGLEAnimCategory : public BB::IObject {
	public:
		shok::AnimationCategoryId Name; // anim category
		shok::AnimationId Animation;


		static inline constexpr int vtp = 0x776C6C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xCEB17718);
	};

	class CGLEAnimSet : public BB::IObject {
	public:
		shok::AnimSetId Id = shok::AnimSetId::Invalid;
		shok::Vector<CGLEAnimCategory*> Anims;


		static inline constexpr int vtp = 0x784A90;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA4C5BB88);

		CGLEAnimSet();
		CGLEAnimSet(const CGLEAnimSet& o) = delete;
		CGLEAnimSet(CGLEAnimSet&& o) noexcept = delete;

		shok::AnimationId GetAnim(shok::AnimationCategoryId category);

		void* operator new(size_t s);
		void operator delete(void* p);

		virtual ~CGLEAnimSet() override;
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	private:
		void SetVT();
	};
	static_assert(sizeof(CGLEAnimSet) == 4 * 6);
}

namespace GGL {
	class CGLGoodDependentAnim {
	public:
		shok::Goods GoodType;
		shok::AnimationId Animation;

		static inline constexpr int vtp = 0x776C64;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB240C758);

		virtual shok::ClassId __stdcall GetClassIdentifier() const;
	};
	static_assert(sizeof(CGLGoodDependentAnim) == 4 * 3);
	class CGLAnimCategory : public EGL::CGLEAnimCategory {
	public:
		shok::Vector<CGLGoodDependentAnim> GoodDependentAnim;

		static inline constexpr int vtp = 0x776C7C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA7B5DFB8);
	};
}


#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace EGL {
	template<class ToManage, size_t MaxNum>
	requires (MaxNum == 65535) // funcs only defined for this size
	class TGLEItemManager {
	public:
		int Count;
		PADDINGI(3);
	private:
		struct {
			ToManage* Point;
			int Flags;
		} Data[MaxNum + 1];
		bool Unknown;

	public:
		bool IsIdValid(int id) const {
			bool(__thiscall* const f)(const TGLEItemManager<ToManage, MaxNum>*, int) = reinterpret_cast<bool(__thiscall*)(const TGLEItemManager<ToManage, MaxNum>*, int)>(0x4FAABD);
			return f(this, id);
		}
		ToManage* GetById(int id) {
			ToManage* (__thiscall* const f)(TGLEItemManager<ToManage, MaxNum>*, int) = reinterpret_cast<ToManage * (__thiscall*)(TGLEItemManager<ToManage, MaxNum>*, int)>(0x4FAAE3);
			return f(this, id);
		}
		ToManage* GetInSlot(int id) {
			if (Count <= id)
				return nullptr;
			return Data[id + 1].Point;
		}
		virtual ~TGLEItemManager() = default;
	private:
		virtual void UnknownFunc() = 0;
	};
	static_assert(sizeof(TGLEItemManager<EGL::CEffect, 65535>) == 524312);

	class CGLEEffectManager : public EGL::TGLEItemManager<EGL::CEffect, 65535> {
	public:
		static inline constexpr int vtp = 0x784528;

		static inline EGL::CGLEEffectManager** const GlobalObj = reinterpret_cast<EGL::CGLEEffectManager**>(0x898144);
	};

	class CGLEEntityManager : public EGL::TGLEItemManager<EGL::CGLEEntity, 65535> {
	public:
		static inline constexpr int vtp = 0x783B70;

		static inline EGL::CGLEEntityManager** const GlobalObj = reinterpret_cast<EGL::CGLEEntityManager**>(0x897558);
	};
}

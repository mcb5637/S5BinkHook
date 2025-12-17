#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>

namespace EGL {
	template<class ToManage, size_t MaxNum>
	requires (MaxNum == 65535) // funcs only defined for this size
	class TGLEItemManager {
	public:
		int HighestUsedSlot = 0;
		int FreeSlot = 1;
		int NumberOfItems = 0;
	private:
		struct {
			int ID;
			ToManage* Item;
		} Data[MaxNum + 1];
		int UnknownInt = 0;
		bool Unknown = false;

		// lower half of id ( & 0xFFFF) is position in Data array (offset 1?)
		// upper half of id ( & 0x3FFF0000) is counter (how many times this slot was used) (starts at 1, increases by 1 on each reuse via register)
		// last 2 bits unknown
		
	public:
		[[nodiscard]] bool IsIdValid(int id) const {
			auto f = reinterpret_cast<bool(__thiscall*)(const TGLEItemManager<ToManage, MaxNum>*, int)>(0x4FAABD);
			return f(this, id);
		}
		ToManage* GetById(int id) {
			auto f = reinterpret_cast<ToManage * (__thiscall*)(TGLEItemManager<ToManage, MaxNum>*, int)>(0x4FAAE3);
			return f(this, id);
		}
		ToManage* GetInSlot(int id) {
			if (HighestUsedSlot < id)
				return nullptr;
			return Data[id].Item;
		}
		virtual ~TGLEItemManager() = default;
		virtual void Tick() = 0;

		// register id 57B018(ToManage*, id_out*) id_out needs to be p to id in managed obj
	};
	static_assert(sizeof(TGLEItemManager<EGL::CEffect, 65535>) == 524312);

	class CGLEEffectManager : public EGL::TGLEItemManager<EGL::CEffect, 65535> {
	public:
		static inline constexpr int vtp = 0x784528;

		[[nodiscard]] bool IsIdValid(shok::EffectId id) const {
			return EGL::TGLEItemManager<EGL::CEffect, 65535>::IsIdValid(static_cast<int>(id));
		}
		EGL::CEffect* GetById(shok::EffectId id) {
			return EGL::TGLEItemManager<EGL::CEffect, 65535>::GetById(static_cast<int>(id));
		}

		static inline EGL::CGLEEffectManager** const GlobalObj = reinterpret_cast<EGL::CGLEEffectManager**>(0x898144);
	};

	class CGLEEntityManager : public EGL::TGLEItemManager<EGL::CGLEEntity, 65535> {
	public:
		shok::Map<shok::String, shok::EntityId> ScriptName;
		static inline BB::SerializationData* (__cdecl* const SerializationData)() = reinterpret_cast<BB::SerializationData * (__cdecl*)()>(0x574083);

		static inline constexpr int vtp = 0x783B70;

		static inline EGL::CGLEEntityManager** const GlobalObj = reinterpret_cast<EGL::CGLEEntityManager**>(0x897558);

		// only removes if id matches
		static inline void(__cdecl* const RemoveScriptName)(const char* name, shok::EntityId id) = reinterpret_cast<void(__cdecl*)(const char*, shok::EntityId)>(0x576C84);
		// overrides existing name
		static inline void(__cdecl* const SetScriptName)(const char* name, shok::EntityId id) = reinterpret_cast<void(__cdecl*)(const char*, shok::EntityId)>(0x576EA6);
	};
}

#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace EGL {
	class CGLEBehaviorProps : public BB::IObject, public ECore::ICheckData {
	public:
		int BehaviorIndex = 0;
		shok::ClassId BehaviorClass = {};

		static inline constexpr int vtp = 0x772A2C;
		static inline constexpr int TypeDesc = 0x813728;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x29B5CF4D);

		static inline const BB::SerializationData* SerializationData = reinterpret_cast<const BB::SerializationData*>(0x86A768);
	};
	static_assert(offsetof(EGL::CGLEBehaviorProps, BehaviorIndex) == 2 * 4);

	class CGLEBehavior : public BB::IObject { // no vtable
	public:
		int SlotIndex = 0;
		shok::EntityId EntityId = static_cast<shok::EntityId>(0);
		EGL::CGLEBehaviorProps* PropPointer = nullptr; // 3, warning this may not be set

	protected:
		virtual void __thiscall AddHandlers(shok::EntityId id) = 0; // 3
		virtual void __thiscall OnEntityCreate(EGL::CGLEBehaviorProps* p) = 0;
		virtual void __thiscall OnEntityLoad(EGL::CGLEBehaviorProps* p) = 0;
		virtual void __thiscall OnEntityUpgrade(EGL::CGLEEntity* old); // on movement seems to copy a lot of data, maybe change behavior?
		virtual void __thiscall OnEntityDestroy(bool ev); // 7 usually empty, ev is false when destroyed normally, entity is still valid when called (not the case in dtor)

	public:
		static inline constexpr int TypeDesc = 0x813778;
		static inline const BB::SerializationData* SerializationData = reinterpret_cast<const BB::SerializationData*>(0x86A828);
		static inline constexpr shok::ClassId Identifier = BB::InvalidIdentifier;
	};
}

namespace ED {
	class CBehaviorProps : public BB::IObject {
	public:
		shok::ClassId Class = static_cast<shok::ClassId>(0);
		int Index = 0;
		int Unknown = 100; // 100?? 3

		static inline constexpr int vtp = 0x76AB0C;
		static inline constexpr int TypeDesc = 0x80AC10;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1F78996D);

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x8586E8);
	};

	class IBehavior : public BB::IObject {
	public:
		//ED::CEntity* EntityDisplay = nullptr;
		//ED::CBehaviorProps* Props = nullptr; does seem to be only in subclasses

	protected:
		virtual void __stdcall OnAdd(ED::CEntity* edispl, ED::CBehaviorProps* props, int uk) = 0; // called before ED::CEntity init
		virtual void __stdcall Initialize(ED::CEntity* edispl, ED::CBehaviorProps* props) = 0; // called before ED::CEntity init
	public:
		virtual void __stdcall UpdateRenderNoTick(int count, float uk) = 0;
		virtual void __stdcall UpdateRenderOneTick(int count, float uk) = 0;
		virtual void __stdcall UpdateRenderManyTick(int count, float uk) = 0;
	private:
		virtual int __stdcall UnknownFuncRet100();

	public:
		static inline constexpr int vtp = 0x76A9E4;
		static inline constexpr int TypeDesc = 0x80AA54;
		static inline constexpr shok::ClassId Identifier = BB::InvalidIdentifier;
	};
}

#pragma once
#include <cstdint>
#include <compare>
#include <numbers>

#include "s5_forwardDecls.h"
#include "s5_datastructures.h"

namespace shok {
	// format i int, f float, x hex int, c char, s const char*
	static inline void (* const LogString)(const char* format, ...) = reinterpret_cast<void (*)(const char* format, ...)>(0x548268);

	static inline void(_stdcall* const SetHighPrecFPU)() = reinterpret_cast<void(_stdcall*)()>(0x5C8451);

	static inline int(__stdcall* const LoadBuffer)(lua_State* L, const char* buff, size_t bufflen, const char* name) = reinterpret_cast<int(__stdcall*)(lua_State*, const char*, size_t, const char*)>(0x59BE57);

	// generic structs
	struct Position {
		float X = -1;
		float Y = -1;

		void FloorToBuildingPlacement();
		void RoundToBuildingPlacement();
		float GetDistanceSquaredTo(const Position& p) const;
		bool IsInRange(const Position& p, float range) const;
		// returns deg, this is a hok specific angle, useful for a look at by settlers
		float GetAngleBetween(const Position& p) const;
		// requires rad
		Position Rotate(float r) const;
		// requires rad
		Position RotateAround(float r, const Position& center) const;

		Position operator+(const Position& other) const;
		Position& operator+=(const Position& other);
		Position operator-(const Position& other) const;
		Position& operator-=(const Position& other);
		Position operator*(float f) const;
		Position& operator*=(float f);
		float Dot(const Position& o) const;
		auto operator<=>(const Position& o) const = default;

		static BB::SerializationData* SerializationData;
	};
	struct PositionRot : Position {
		float r = 0;
	};


	struct AARect {
		shok::Position low, high;
		auto operator<=>(const AARect& o) const = default;
		AARect operator+(const AARect& other) const;
		AARect& operator+=(const AARect& other);
		AARect operator-(const AARect& other) const;
		AARect& operator-=(const AARect& other);

		// requires rad
		AARect Rotate(float r) const;
		AARect Sort() const;

		// uses Blocked1 and Blocked2 for the 2 members
		static BB::SerializationData* SerializationData;
	};

	struct CostInfo { // size 18
		PADDINGI(1);
		float Gold = 0, GoldRaw = 0;
		float Silver = 0, SilverRaw = 0;
		float Stone = 0, StoneRaw = 0;
		float Iron = 0, IronRaw = 0;
		float Sulfur = 0, SulfurRaw = 0;
		float Clay = 0, ClayRaw = 0;
		float Wood = 0, WoodRaw = 0;
		float WeatherEnergy = 0, Knowledge = 0, Faith = 0;

		float GetResourceAmountFromType(shok::ResourceType ty, bool addRaw) const;
		void AddToType(shok::ResourceType ty, float toadd);
		void SubFromType(shok::ResourceType ty, float tosub);
		bool HasResources(const CostInfo* has) const;

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x85D668);
	};
	static_assert(sizeof(CostInfo) == 18 * 4);


	struct Attachment {
		int EntityId, EventID;
	};

	struct Color {
		byte R = 255, G = 255, B = 255, A = 255;

		Color(int r, int g, int b, int a);
		Color() = default;

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x8594F8); // from rain effect
	};
	static_assert(sizeof(shok::Color) == 1 * 4);

	static inline char* (__cdecl* const CopyString)(const char* s) = reinterpret_cast<char* (__cdecl*)(const char*)>(0x547BD5);
	static inline void(__cdecl* const StringToLowerCase)(char* s) = reinterpret_cast<void(__cdecl*)(char* s)>(0x547C81);
}

namespace BB {
	class IObject {
	public:
		virtual ~IObject() = default;
		virtual unsigned int __stdcall GetClassIdentifier() const = 0;
		virtual void* __stdcall CastToIdentifier(unsigned int id);

		static constexpr int TypeDesc = 0x7FFE08;
		static constexpr int vtp = 0x7620F0;

		struct _vtableS {
			void(__thiscall* dtor)(BB::IObject* th, bool free);
			unsigned int(__stdcall* GetClassIdentifier)(const BB::IObject* th);
			void* (__stdcall* CastToIdentifier)(BB::IObject* th, unsigned int id);
		};
	};

	class IPostEvent {
	public:
		virtual void __stdcall PostEvent(BB::CEvent* ev) = 0;
	};

	class CNonCopyable {
	};
}

namespace ECore {
	class ICheckData {
	public:
		virtual ~ICheckData() = default;
		virtual bool __stdcall Check();
	};
}

namespace EGL {
	class CGLEAttachableBase {
	public:
		virtual ~CGLEAttachableBase() = default;
		// 8 more funcs
		// 7 is attached to (this, atype, id) ?

		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObserverEntities; // 8
		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObserverEffects; // 11
		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObservedEntities; // 14
		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObservedEffects; // 17
		bool SendEvent; // 20
		PADDING(3);

		static constexpr int vtp = 0x783D18;
		static constexpr int TypeDesc = 0x810B98;
	};
	class CEntityAttachmentProxy {
	};
	class CEffectAttachmentProxy {
	};
	template<class T, class V>
	class TGLEAttachable : public CGLEAttachableBase {

	};


	template<class EventType, class ReturnType>
	class IGLEHandler {
	public:
		virtual ReturnType Handle(EventType* t) = 0;
	};
	template<int HandlerID, class EventBase, class HandlerEvent, class ObjectType, class ReturnType>
	class THandler : public IGLEHandler<EventBase, ReturnType> {
	public:
		typedef ReturnType(ObjectType::* HandlerType)(HandlerEvent* ev);

		ObjectType* Object;
		HandlerType HandlerFunc;

		virtual ReturnType Handle(EventBase* ev) override {
			return std::invoke(HandlerFunc, Object, static_cast<HandlerEvent*>(ev));
		}

		THandler(ObjectType* o, HandlerType h) {
			Object = o;
			HandlerFunc = h;
		}
	};

	using EventHandler = EGL::IGLEHandler<BB::CEvent, void>;
	using TaskHandler = EGL::IGLEHandler<EGL::CGLETaskArgs, int>;

	class IGLEStateHandler {
		virtual int Handle(int i) = 0;
	};

	template<class ObjectType>
	class TStateHandler : public IGLEStateHandler {
	public:
		typedef int (ObjectType::* HandlerType)(int i);

		ObjectType* Object;
		HandlerType HandlerFunc;

		virtual int Handle(int i) override {
			return std::invoke(HandlerFunc, Object, i);
		}

		TStateHandler(ObjectType* o, HandlerType h) {
			Object = o;
			HandlerFunc = h;
		}
	};


	class ISlot {
	public:
		static inline constexpr int vtp = 0x7738CC;
		virtual ~ISlot() = default;
	};
	template<class T, unsigned int id>
	class TSlot : public ISlot {
	public:
		virtual void FillSlot(T* data) = 0;
	};
}

namespace CppLogic {
	template<class T>
	void* GetVTable(T* obj) {
		struct O {
			void* vt;
		};
		return reinterpret_cast<O*>(obj)->vt;
	}

	template<class T>
	bool ContainsValue(const T* data, const T search, int num) {
		for (int i = 0; i < num; i++)
			if (data[i] == search)
				return true;
		return false;
	}

	template<class T>
	constexpr inline T RadiansToDegrees(T r) {
		return static_cast<T>(static_cast<double>(r) * 180.0 / std::numbers::pi);
	}
	template<class T>
	constexpr inline T DegreesToRadians(T d) {
		return static_cast<T>(static_cast<double>(d) * std::numbers::pi / 180);
	}

}

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

	static inline float(__stdcall* const GetCurrentTimeFloat)() = reinterpret_cast<float(__stdcall*)()>(0x548B64);

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
		Position Normalize() const;

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

		// everything in rad, corrects angle diff to [-180,+180]
		static inline float(__cdecl* const AngleDifference)(float a1, float a2) = reinterpret_cast<float(__cdecl*)(float, float)>(0x57EF7E);
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

		static inline shok::ResourceType(__cdecl* const ResourceTypeToRaw)(shok::ResourceType rt) = reinterpret_cast<shok::ResourceType(__cdecl*)(shok::ResourceType)>(0x4A9299);

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

	// dynamic cast 0x5C36EE __cdecl(obj, vtoffset, from, to, reference)
}

namespace BB {
	constexpr unsigned int InvalidIdentifier = 0xEEFFFFFF; // guranteed to not appear in shok

	template<class T>
	concept HasValidIdentifier = std::is_same_v<decltype(T::Identifier), const unsigned int> && T::Identifier != BB::InvalidIdentifier;

	class IObject {
	public:
		virtual ~IObject() = default;
		virtual unsigned int __stdcall GetClassIdentifier() const = 0;
		virtual void* __stdcall CastToIdentifier(unsigned int id);

		static constexpr int TypeDesc = 0x7FFE08;
		static constexpr int vtp = 0x7620F0;
		static inline constexpr unsigned int Identifier = InvalidIdentifier;

		struct _vtableS {
			void(__thiscall* dtor)(BB::IObject* th, bool free);
			unsigned int(__stdcall* GetClassIdentifier)(const BB::IObject* th);
			void* (__stdcall* CastToIdentifier)(BB::IObject* th, unsigned int id);
		};
	};

	template<HasValidIdentifier CastTo, class CastFrom, HasValidIdentifier... AdditionalSubClasses>
	requires std::derived_from<CastTo, CastFrom> && std::derived_from<CastFrom, IObject> && (std::derived_from<AdditionalSubClasses, CastTo> && ...)
	CastTo* IdentifierCast(CastFrom* f) {
		const unsigned int id = f->GetClassIdentifier();
		if (id == CastTo::Identifier)
			return static_cast<CastTo*>(f);
		if constexpr (sizeof...(AdditionalSubClasses) != 0) {
			std::array<const unsigned int, sizeof...(AdditionalSubClasses)> additionalIdentifiers = { AdditionalSubClasses::Identifier... };
			for (const unsigned int addid : additionalIdentifiers) {
				if (id == addid)
					return static_cast<CastTo*>(f);
			}
		}
		return static_cast<CastTo*>(f->CastToIdentifier(CastTo::Identifier));
	}

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

		void* operator new(size_t s)
		{
			return shok::Malloc(s);
		}
		void operator delete(void* p) {
			shok::Free(p);
		}
	};

	using EventHandler = EGL::IGLEHandler<BB::CEvent, void>;
	using TaskHandler = EGL::IGLEHandler<EGL::CGLETaskArgs, int>;

	class IGLEStateHandler {
		virtual shok::TaskStateExecutionResult Handle(int i) = 0;
	};

	template<class ObjectType>
	class TStateHandler : public IGLEStateHandler {
	public:
		typedef shok::TaskStateExecutionResult(ObjectType::* HandlerType)(int i);

		ObjectType* Object;
		HandlerType HandlerFunc;

		virtual shok::TaskStateExecutionResult Handle(int i) override {
			return std::invoke(HandlerFunc, Object, i);
		}

		TStateHandler(ObjectType* o, HandlerType h) {
			Object = o;
			HandlerFunc = h;
		}

		void* operator new(size_t s)
		{
			return shok::Malloc(s);
		}
		void operator delete(void* p) {
			shok::Free(p);
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
		virtual void __stdcall FillSlot(T* data) = 0;
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

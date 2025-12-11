#pragma once
#include <cstdint>
#include <compare>
#include <numbers>

#include "s5_forwardDecls.h"
#include "s5_datastructures.h"
#include "s5_defines.h"

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
		// returns rad
		float GetAngle() const;
		// returns rad, this is a hok specific angle, useful for a look at by settlers
		float GetAngleBetweenR(const Position& p) const;
		// returns deg, this is a hok specific angle, useful for a look at by settlers
		float GetAngleBetween(const Position& p) const;
		// requires rad
		Position Rotate(float r) const;
		// requires rad
		Position RotateAround(float r, const Position& center) const;
		Position Normalize() const;
		Position ClampToWorld(float border = 0.0f) const;

		Position operator+(const Position& other) const;
		Position& operator+=(const Position& other);
		Position operator-(const Position& other) const;
		Position& operator-=(const Position& other);
		Position operator*(float f) const;
		Position& operator*=(float f);
		float Dot(const Position& o) const;
		float DotSquare() const;
		Position Abs() const noexcept;
		auto operator<=>(const Position& o) const = default;

		static BB::SerializationData* SerializationData;

		// 0x503B1E low res from hi res thiscall(const hi*) (modifies this)
	};
	struct PositionRot : Position {
		float r = 0;

		inline PositionRot(float x, float y, float r) : r(r) {
			X = x;
			Y = y;
		}
		inline explicit PositionRot(const Position& p) : Position(p) {}
		inline PositionRot& operator=(const Position& p) { // copes X and Y, leaves r
			Position::operator=(p);
			return *this;
		}
		inline PositionRot() = default;

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
		bool SubResources(const CostInfo& tosub); // returns hadRes, does not sub anything if one res type is missing

		static inline shok::ResourceType(__cdecl* const ResourceTypeToRaw)(shok::ResourceType rt) = reinterpret_cast<shok::ResourceType(__cdecl*)(shok::ResourceType)>(0x4A9299);
		static inline shok::ResourceType(__cdecl* const RawToResourceType)(shok::ResourceType rt) = reinterpret_cast<shok::ResourceType(__cdecl*)(shok::ResourceType)>(0x4A92D6);

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x85D668);
	};
	static_assert(sizeof(CostInfo) == 18 * 4);


	struct Attachment {
		shok::EntityId EntityId; // may aso be shok::EffectId (but that seems to be unused)
		shok::EventIDs EventID;
	};

	struct Color {
		byte R = 255, G = 255, B = 255, A = 255;

		Color(int r, int g, int b, int a);
		Color() = default;

		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0x8594F8); // from rain effect
	};
	static_assert(sizeof(shok::Color) == 1 * 4);

	class ConstructionInfo {
	public:
		PADDINGI(1);
		shok::Vector<shok::PositionRot> BuilderSlot;
		int Time;
		shok::CostInfo Cost;
		shok::EntityTypeId ConstructionSite;
	};
	static_assert(sizeof(ConstructionInfo) == 100);

	static inline char* (__cdecl* const CopyString)(const char* s) = reinterpret_cast<char* (__cdecl*)(const char*)>(0x547BD5);
	static inline void(__cdecl* const StringToLowerCase)(char* s) = reinterpret_cast<void(__cdecl*)(char* s)>(0x547C81);

	// dynamic cast 0x5C36EE __cdecl(obj, vtoffset, from, to, reference)
}

namespace BB {
	constexpr shok::ClassId InvalidIdentifier = shok::ClassId::Invalid; // guranteed to not appear in shok

	template<class T>
	struct IdentifierInvalid : std::false_type {};
	template<class T>
	concept HasValidIdentifier = std::is_same_v<decltype(T::Identifier), const shok::ClassId> && T::Identifier != BB::InvalidIdentifier && !IdentifierInvalid<T>::value;

	class IObject {
	public:
		virtual ~IObject() = default;
		virtual shok::ClassId __stdcall GetClassIdentifier() const = 0;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id);

		static constexpr int TypeDesc = 0x7FFE08;
		static constexpr int vtp = 0x7620F0;
		static constexpr shok::ClassId Identifier = InvalidIdentifier;

		template<HasValidIdentifier To>
		To* CastToIdentifier() {
			return static_cast<To*>(CastToIdentifier(To::Identifier));
		}
	};

	template<HasValidIdentifier CastTo, class CastFrom, HasValidIdentifier... AdditionalSubClasses>
	requires std::derived_from<CastTo, CastFrom> && std::derived_from<CastFrom, IObject> && (std::derived_from<AdditionalSubClasses, CastTo> && ...)
	CastTo* IdentifierCast(CastFrom* f) {
		if (f == nullptr)
			return nullptr;
		const shok::ClassId id = f->GetClassIdentifier();
		if (id == CastTo::Identifier)
			return static_cast<CastTo*>(f);
		if constexpr (sizeof...(AdditionalSubClasses) != 0) {
			std::array<const shok::ClassId, sizeof...(AdditionalSubClasses)> additionalIdentifiers = { AdditionalSubClasses::Identifier... };
			for (const shok::ClassId addid : additionalIdentifiers) {
				if (id == addid)
					return static_cast<CastTo*>(f);
			}
		}
		return static_cast<CastTo*>(f->CastToIdentifier(CastTo::Identifier));
	}

	class IObjectCreator {
	public:
		virtual ~IObjectCreator() = default;
		virtual shok::ClassId __stdcall GetIdOfCreated() = 0;
		virtual const char* __stdcall GetNameOfCreated() = 0;
		virtual BB::IObject* __stdcall Create() = 0;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x456C66DD);
	};

	class IPostEvent {
	public:
		virtual void __stdcall PostEvent(BB::CEvent* ev) = 0;
	};

	class CNonCopyable {
	};

	class CSlotBase {
	public:
		static inline constexpr int vtp = 0x769528;
		virtual ~CSlotBase() = default;
	};
	template<class ToHandle>
	class TSlot1 : public CSlotBase {
	public:
		virtual void __stdcall Handle(ToHandle toHandle) = 0;
	};
	template<class ObjectType, class ToHandle>
	class TSlotEx1 : public TSlot1<ToHandle> {
	public:
		typedef void(__stdcall ObjectType::* HandlerType)(ToHandle i);

		ObjectType* Object;
		HandlerType Handler;

		virtual void __stdcall Handle(ToHandle toHandle) override {
			std::invoke(Handler, Object, toHandle);
		}

		TSlotEx1(ObjectType* o, HandlerType h) {
			Object = o;
			Handler = h;
		}

		void* operator new(size_t s)
		{
			return shok::Malloc(s);
		}
		void operator delete(void* p) {
			shok::Free(p);
		}
	};
	class ISlot0 : public CSlotBase {
	public:
		virtual void __stdcall Handle() = 0;
	};
	template<class ObjectType>
	class TSlotEx0 : public ISlot0 {
	public:
		typedef void(__stdcall ObjectType::* HandlerType)();

		ObjectType* Object;
		HandlerType Handler;

		virtual void __stdcall Handle() override {
			std::invoke(Handler, Object);
		}

		TSlotEx0(ObjectType* o, HandlerType h) {
			Object = o;
			Handler = h;
		}

		void* operator new(size_t s)
		{
			return shok::Malloc(s);
		}
		void operator delete(void* p) {
			shok::Free(p);
		}
	};
	template<class ToHandle1, class ToHandle2>
	class TSlot2 : public CSlotBase {
	public:
		virtual void __stdcall Handle(ToHandle1 th1, ToHandle2 th2) = 0;
	};
	template<class ObjectType, class ToHandle1, class ToHandle2>
	class TSlotEx2 : public TSlot2<ToHandle1, ToHandle2> {
	public:
		typedef void(__stdcall ObjectType::* HandlerType)(ToHandle1 i, ToHandle2 j);

		ObjectType* Object;
		HandlerType Handler;

		virtual void __stdcall Handle(ToHandle1 th1, ToHandle2 th2) override {
			std::invoke(Handler, Object, th1, th2);
		}

		TSlotEx2(ObjectType* o, HandlerType h) {
			Object = o;
			Handler = h;
		}

		void* operator new(size_t s)
		{
			return shok::Malloc(s);
		}
		void operator delete(void* p) {
			shok::Free(p);
		}
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
	protected:
		virtual void OnOtherEntityAttachToMe_vt(shok::AttachmentType ty, shok::EntityId otherId, shok::EventIDs onThisDetachEvent) = 0; // other -> this
		virtual void OnOtherEffectAttachToMe_vt(shok::AttachmentType ty, shok::EffectId otherId, shok::EventIDs onThisDetachEvent) = 0; // other -> this
		virtual void OnOtherEntityDetachFromMe_vt(shok::AttachmentType ty, shok::EntityId otherId) = 0; // other -> this
		virtual void OnOtherEffectDetachFromMe_vt(shok::AttachmentType ty, shok::EffectId otherId) = 0; // other -> this
		virtual void DetachObservedEntity_vt(shok::AttachmentType ty, shok::EntityId otherId) = 0; // this -> other 5
		virtual void DetachObservedEffect_vt(shok::AttachmentType ty, shok::EffectId otherId) = 0; // this -> other
	public:
		virtual bool IsAttachedToEntity(shok::AttachmentType ty, shok::EntityId otherId) = 0; // this -> other
		virtual bool IsAttachedToEffect(shok::AttachmentType ty, shok::EffectId otherId) = 0; // this -> other

		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObserverEntities; // 8 other -> this
		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObserverEffects; // 11 other -> this
		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObservedEntities; // 14 this -> other
		shok::MultiMap<shok::AttachmentType, shok::Attachment> ObservedEffects; // 17 this -> other
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
	requires (std::same_as<T, EGL::CGLEEntity> && std::same_as<V, CEntityAttachmentProxy>) || (std::same_as<T, EGL::CEffect> && std::same_as<V, CEffectAttachmentProxy>)
	class TGLEAttachable : public CGLEAttachableBase {
	public:

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

	class EventHandlerList {
	public:
		shok::MultiMap<shok::EventIDs, EGL::EventHandler*> Handlers;
		bool AllowAdd;

		// dtor 49EB85
		void AddHandler(shok::EventIDs id, EGL::EventHandler* h);
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

	template<int NumBits>
	requires (NumBits == 1)
	class C2DBitArray : public BB::IObject {
	public:
		int SizeX, SizeY;
		int SizeXEx; // y coord multiplyer, (SizeX + 31) & 0xFFFFFFE0
		unsigned int BufferSize; // in bytes, 4 * ((SizeY * SizeXEx >> 5) + 1) & 0xFFFFFFFE
		shok::Vector<unsigned int> ValueArray; // likely binary data, just gets serialized as uints, rets reserved as BufferSize/4

		// NumBits==1 vtp 0x784EC0


		int Get(unsigned int x, unsigned int y) {
			unsigned int p = x + y * SizeXEx;
			unsigned int bit = p & 0x1F;
			return (ValueArray[p >> 5] & (1 << bit)) >> bit;
		}
		void Set(unsigned int x, unsigned int y, int v) {
			unsigned int p = x + y * SizeXEx;
			unsigned int bit = p & 0x1F;
			unsigned int d = 1 << bit;
			unsigned int& ar = ValueArray[p >> 5];
			if (v)
				ar |= d;
			else
				ar &= ~d;
		}
	};
	class C2DArray1Bit : public C2DBitArray<1> {
	public:

		static constexpr int vtp = 0x784ED0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xEE435293);
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

	template<class T>
	struct DestroyCaller {
		void operator()(T* o) {
			o->Destroy();
		}
	};


	template<int HandlerID, class EventBase, class HandlerEvent, class ReturnType>
	class StaticHandler : public EGL::IGLEHandler<EventBase, ReturnType> {
	public:
		typedef ReturnType(*HandlerType)(HandlerEvent* ev);

		HandlerType HandlerFunc;

		virtual ReturnType Handle(EventBase* ev) override {
			return HandlerFunc(static_cast<HandlerEvent*>(ev));
		}

		explicit StaticHandler(HandlerType h) {
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

	class CaselessStringComparator {
	public:
		using is_transparent = void;
		bool operator()(std::string_view a, std::string_view b) const noexcept;
	};

	class Rect {
		shok::Position A, B, C;
	public:

		inline Rect(const shok::Position& a, const shok::Position& b, const shok::Position& c) : A(a), B(b), C(c) {}
		Rect(const shok::Position& start, const shok::Position& end, float width);

		bool IsPosInRect(const shok::Position& p) const noexcept;
		shok::AARect BoundingBox() const noexcept;
	};
}

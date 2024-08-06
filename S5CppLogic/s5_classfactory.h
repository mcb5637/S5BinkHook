#pragma once
#include <memory>
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_idmanager.h"
#include "Luapp/constexprTypename.h"
#include "luaext.h"

namespace BB {
	class IXmlSerializer {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual void __stdcall Deserialize(BB::IStream* f, BB::IObject* ob) = 0; // open file in mode 0x113
	private:
		virtual void unknown() = 0;
	public:
		virtual void __stdcall Serialize(BB::IStream* f, BB::IObject* ob) = 0; // open file in mode 0x121
	protected:
		virtual ~IXmlSerializer() = default;
	public:
		virtual void __stdcall DeserializeByData(BB::IStream* f, void* ob, const BB::SerializationData* d) = 0;
		virtual void __stdcall SerializeByData(BB::IStream* f, void* ob, const BB::SerializationData* d, const char* xmlrootname = nullptr) = 0;
	};
	class CXmlSerializer : public IXmlSerializer {
	public:
		PADDINGI(45); // 43 + a bit of extra space

		static inline constexpr int vtp = 0x77FA2C;
		static inline constexpr int TypeDesc = 0x8311F0;

		using IXmlSerializer::Deserialize;
		using IXmlSerializer::Serialize;
		static BB::CXmlSerializer* Create();
		static std::unique_ptr<CXmlSerializer, CppLogic::DestroyCaller<CXmlSerializer>> CreateUnique();
		void Deserialize(const char* filename, BB::IObject* ob);
		void Deserialize(const char* filename, void* ob, const BB::SerializationData* d);
		void Serialize(const char* filename, BB::IObject* ob);

		static void HookWriteXSIType();
	private:
		BB::CXmlSerializer() = default;
		static void __stdcall WriteType(BB::IStream* f, const char* cn);
	};
	class IBinarySerializer {
	public:
		virtual ~IBinarySerializer() = default;
		virtual void __stdcall Destroy() = 0;
		virtual void __stdcall DeserializeById(BB::IStream* f, BB::IObject* b) = 0;
		virtual void __stdcall SerializeById(BB::IStream* f, BB::IObject* b) = 0;
		virtual void __stdcall DeserializeByData(BB::IStream* f, void* o, const BB::SerializationData* d) = 0;
		virtual void __stdcall SerializeByData(BB::IStream* f, void* o, const BB::SerializationData* d, shok::ClassId id) = 0; // id gets ignored

		static inline constexpr int vtp = 0x77F4C8;
	};
	class CBinarySerializer : public IBinarySerializer {
	public:

		static inline constexpr int vtp = 0x77F5A4;

		static CBinarySerializer* Create(int uk);

	private:
		CBinarySerializer() = default;
	};

	struct FieldSerilaizer {
		void(__stdcall* DeserializeFromString)(void* data, const char* buff) = nullptr;
		int(__stdcall* SerializeToString)(char* buff, size_t s, const void* data) = nullptr; // returns num chars written if buffer would be sufficient, negative on error
		void(__stdcall* DeserializeFromStream)(void* data, BB::IStream* str) = nullptr;
		void(__stdcall* SerializeToStream)(BB::IStream* str, const void* data) = nullptr;
		BB::CIDManagerEx* (__stdcall* GetIdManager)() = nullptr; // may be nullptr
		bool IsPrimitive = false;
		void* Buffer = nullptr; // p to a memory block of the serialized type, usually the buffer is directly after the serializer structure

		static inline BB::FieldSerilaizer* const TypeInt = reinterpret_cast<BB::FieldSerilaizer*>(0x810C98);
		static inline BB::FieldSerilaizer* const TypeUInt = reinterpret_cast<BB::FieldSerilaizer*>(0x810CD8);
		static inline BB::FieldSerilaizer* const TypeFloat = reinterpret_cast<BB::FieldSerilaizer*>(0x810C78);
		static inline BB::FieldSerilaizer* const TypeDouble = reinterpret_cast<BB::FieldSerilaizer*>(0x82BB38);
		static inline BB::FieldSerilaizer* const TypeBool = reinterpret_cast<BB::FieldSerilaizer*>(0x810C58);
		static inline BB::FieldSerilaizer* const TypeString = reinterpret_cast<BB::FieldSerilaizer*>(0x8640F0);
		static inline BB::FieldSerilaizer* const TypeCharBuff = reinterpret_cast<BB::FieldSerilaizer*>(0x810CB8);
		static inline BB::FieldSerilaizer* const TypeClassIdentifier = reinterpret_cast<BB::FieldSerilaizer*>(0x813978);
		static inline BB::FieldSerilaizer* const TypeTaskList = reinterpret_cast<BB::FieldSerilaizer*>(0x85D4AC);
		// does not add ids
		static inline BB::FieldSerilaizer* const TypeEntityType = reinterpret_cast<BB::FieldSerilaizer*>(0x85D4D0);
		static inline BB::FieldSerilaizer* const TypeModel = reinterpret_cast<BB::FieldSerilaizer*>(0x8585C0);
		static inline BB::FieldSerilaizer* const TypeAnim = reinterpret_cast<BB::FieldSerilaizer*>(0x86057C);
		static inline BB::FieldSerilaizer* const TypeAnimCategory = reinterpret_cast<BB::FieldSerilaizer*>(0x875494);
		static inline BB::FieldSerilaizer* const TypeDamageClass = reinterpret_cast<BB::FieldSerilaizer*>(0x860558);
		static inline BB::FieldSerilaizer* const TypeEffectType = reinterpret_cast<BB::FieldSerilaizer*>(0x8605A0);
		static inline BB::FieldSerilaizer* const TypeWidgetID = reinterpret_cast<BB::FieldSerilaizer*>(0x894708);
		static inline BB::FieldSerilaizer* const TypeArmorClassId = reinterpret_cast<BB::FieldSerilaizer*>(0x85D5CC);

		template<class T>
		static constexpr BB::FieldSerilaizer* GetSerilalizer() = delete;

		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<int>() {
			return TypeInt;
		}
		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<unsigned int>() {
			return TypeUInt;
		}
		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<float>() {
			return TypeFloat;
		}
		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<bool>() {
			return TypeBool;
		}
		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<double>() {
			return TypeDouble;
		}

		struct ExtendedInfo {
			std::string_view Name;
			void (* const Push)(lua::State L, void* data, const FieldSerilaizer* fs);
			void (* const Check)(lua::State L, void* data, int idx, const FieldSerilaizer* fs);
			std::string_view XSDType;
		};

		std::string GetTypeDescName() const;

		const ExtendedInfo& GetExtendedInfo() const;
		const ExtendedInfo* GetOptExtendedInfo() const;
	};

	struct SerializationListOptions {
		struct Iter {
		};
		struct Context {
			const char* GetAttribute(const char* a) const;
		};

		// use either AddToList or AddToListIdManaged
		void* (__stdcall* AddToList)(void* List) = nullptr; // gets list, allocates, then returns p to new object
		void* (__stdcall* AddToListIdManaged)(void* List, Context* context) = nullptr; // gets id from attribute, allocates, returns p to new object
		Iter* (__stdcall* AllocIter)(void* List) = nullptr;
		bool(__stdcall* IterNext)(Iter* i) = nullptr;
		void* (__stdcall* IterCurrent)(Iter* i) = nullptr;
		void(__stdcall* FreeIter)(Iter* i) = nullptr;
		void(__stdcall* FinalizeAddToList)(void* List) = nullptr; // used with AddToList, AddToListIdManaged unknown
		size_t(__stdcall* GetSize)(void* List) = nullptr;
	};

	struct SerializationData { // use a 0-terminated array (default constructed is 0)
		enum class Ty : int {
			Invalid = 0,
			Direct = 2, // uses dataconverter
			Embedded = 3, // uses subelement
			ObjectEmbedded = 5,
			ObjectPointer = 6, // uses getidentifier or subelement
		};

		Ty Type = Ty::Invalid;
		const char* SerializationName = nullptr; // if not set, automatically follows subelementdata with a position of 0
		size_t Position = 0;
		size_t Size = 0;
		const BB::FieldSerilaizer* DataConverter = nullptr;
		const BB::SerializationData* SubElementData = nullptr; //5
		shok::ClassId (__stdcall* GetIdentifier)(void* data) = nullptr;
		const BB::SerializationListOptions* ListOptions = nullptr;
		int Unknown3 = 0;

	private:
		static shok::ClassId __stdcall GetBBIdentifier(void* d);
	public:
		static constexpr SerializationData FieldData(const char* name, size_t pos, size_t size, const BB::FieldSerilaizer* converter, const BB::SerializationListOptions* list = nullptr)
		{
			return SerializationData{ Ty::Direct, name, pos, size, converter, nullptr, nullptr, list, 0 };
		}
		static constexpr SerializationData EmbeddedData(const char* name, size_t pos, size_t size, const SerializationData* subdata, const BB::SerializationListOptions* list = nullptr)
		{
			return SerializationData{ Ty::Embedded, name, pos, size, nullptr, subdata, nullptr, list, 0 };
		}
		static constexpr SerializationData GuardData()
		{
			return SerializationData{ Ty::Invalid, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, 0 };
		}
		static constexpr SerializationData ObjectPointerData(const char* name, size_t pos, size_t size, const BB::SerializationListOptions* list = nullptr) {
			return SerializationData{ Ty::ObjectPointer, name, pos, size, nullptr, nullptr, &GetBBIdentifier, list, 0 };
		}

		static const SerializationData* GetSerializationData(shok::ClassId id);
		template<class T>
		static const SerializationData* GetSerializationData() = delete;
		template<class T>
		requires requires { T::SerializationData; }
		static const SerializationData* GetSerializationData() {
			return T::SerializationData;
		}
		template<HasValidIdentifier T>
		static const SerializationData* GetSerializationData() {
			return GetSerializationData(T::Identifier);
		}
	};
	static_assert(sizeof(BB::SerializationData) == 4 * 9);
#define MemberSerializationSizeAndOffset(T,M) offsetof(T, M), sizeof(decltype(T::M))
#define MemberSerializationFieldData(T,M) MemberSerializationSizeAndOffset(T, M), BB::FieldSerilaizer::GetSerilalizer<decltype(T::M)>()
#define MemberSerializationEmbeddedData(T,M) MemberSerializationSizeAndOffset(T, M), BB::SerializationData::GetSerializationData<decltype(T::M)>()

	class CDefaultRuntimeClass : public BB::IObjectCreator, public BB::IObject {
	public:
		shok::ClassId Id; // 2
		shok::String Name;
		BB::IObject* (__stdcall* NewObj)(); // 10

		static constexpr int vtp = 0x77F738;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x74FECF20);

		// ctor 0x54CFA8
	};
	static_assert(offsetof(CDefaultRuntimeClass, Id) == 2 * 4);
	static_assert(offsetof(CDefaultRuntimeClass, NewObj) == 10 * 4);

	class CRuntimeClassEnumerator;

	class IClassFactory {
	public:
		virtual ~IClassFactory() = default;
	private:
		virtual void unknown0() = 0;
		virtual BB::IObject* __stdcall GetInfoObj(shok::ClassId id) = 0;
	public:
		virtual CRuntimeClassEnumerator* __stdcall GetEnumerator() = 0;
		virtual BB::IObject* __stdcall CreateObject(shok::ClassId id) = 0;
		virtual shok::ClassId __stdcall GetIdentifierByName(const char* name) = 0; // 5
		virtual const char* __stdcall GetClassDemangledName(shok::ClassId id) = 0;
		virtual const BB::SerializationData* __stdcall GetSerializationDataForClass(shok::ClassId id) = 0; // returns a 0-terminated array
		virtual void __stdcall AddClassManuallIObjectCreator(shok::ClassId id, BB::IObject* creator, const BB::SerializationData* data) = 0; // creator bust be CastToIdentifier able to BB::IObjectCreator, id may be 0 gets queried from BB::IObjectCreator
		virtual void __stdcall AddClassToFactory(shok::ClassId id, const char* name, BB::IObject* (__stdcall* NewObj)(), const BB::SerializationData* data) = 0;
	};

	class CClassFactory : public IClassFactory {
	public:
		struct ClassInfo {
			BB::IObject* InfoObj;
			PADDINGI(1); //bool true?
			BB::IObjectCreator* Creator;
			const BB::SerializationData* SData;
		};

		shok::Map<shok::ClassId, ClassInfo> Info;
		shok::Map<shok::String, shok::ClassId> NameToId;
		PADDINGI(1); //0

		static inline constexpr int vtp = 0x77F74C;
		static inline constexpr int TypeDesc = 0x830C80;

		using IClassFactory::CreateObject;
		template<HasValidIdentifier T>
		T* CreateObject()
		{
			return dynamic_cast<T*>(CreateObject(T::Identifier));
		}

		void LoadObject(BB::IObject* ob, const char* filename);
		template<HasValidIdentifier T>
		T* LoadObject(const char* filename)
		{
			T* ob = CreateObject<T>();
			if (!ob)
				return nullptr;
			LoadObject(ob, filename);
			return ob;
		}
		void LoadObject(void* ob, const char* filename, const BB::SerializationData* seri);

		using IClassFactory::GetSerializationDataForClass;
		template<HasValidIdentifier T>
		const BB::SerializationData* GetSerializationDataForClass()
		{
			return GetSerializationDataForClass(T::Identifier);
		}

	private:
		template<class T>
		static BB::IObject* __stdcall FactObjCreator() {
			return new T();
		}

	public:
		using IClassFactory::AddClassToFactory;
		// add class to factory, best called on mainmenu lua state initialization (will be called before anything serious gets loaded)
		// map lua state initialization will be to late, as most stuff is already loaded there
		// overload new and delete operators to allocate via shok::Malloc
		template<class T>
		requires std::is_default_constructible_v<T>
		void AddClassToFactory() {
			if (GetClassDemangledName(T::Identifier))
				throw std::invalid_argument("identifier already used");
			AddClassToFactory(T::Identifier, typename_details::type_name<T>().data(), &FactObjCreator<T>, T::SerializationData);
		}


		static inline BB::CClassFactory** const GlobalObj = reinterpret_cast<BB::CClassFactory**>(0x88F044);
	};
	static_assert(sizeof(CClassFactory) == 8 * 4);
}

namespace CppLogic {
	template<class T>
	class SerializationListOptions_ForVector : public BB::SerializationListOptions { // do only use with std::vector<T> !
		using VectT = std::vector<T>;
		using IterT = std::vector<T>::iterator;
		
		struct IterImpl : Iter {
			VectT* Vector;
			IterT It;
			bool First = true;

			IterImpl(VectT* v)
			{
				Vector = v;
			}
		};
		
		static void* __stdcall AddToListImp(void* l)
		{
			auto* v = static_cast<VectT*>(l);
			v->emplace_back();
			return &v->back();
		}
		static Iter* __stdcall AllocIterImp(void* l)
		{
			return new IterImpl{ static_cast<VectT*>(l) };
		}
		static bool __stdcall IterNextImp(Iter* i)
		{
			IterImpl* it = static_cast<IterImpl*>(i);
			if (it->First) {
				it->It = it->Vector->begin();
				it->First = false;
			}
			else {
				++it->It;
			}
			return it->It != it->Vector->end();
		}
		static void* __stdcall IterCurrentImp(Iter* i)
		{
			IterImpl* it = static_cast<IterImpl*>(i);
			return &*it->It;
		}
		static void __stdcall FreeIterImp(Iter* i)
		{
			IterImpl* it = static_cast<IterImpl*>(i);
			delete it;
		}
		static size_t __stdcall GetSizeImp(void* l)
		{
			auto* v = static_cast<VectT*>(l);
			return v->size();
		}
		static void __stdcall FinalizeAddToListImp(void* List)
		{

		}
		
	public:
		SerializationListOptions_ForVector()
		{
			AddToList = &AddToListImp;
			AllocIter = &AllocIterImp;
			IterNext = &IterNextImp;
			IterCurrent = &IterCurrentImp;
			FreeIter = &FreeIterImp;
			FinalizeAddToList = &FinalizeAddToListImp;
			GetSize = &GetSizeImp;
		}
	};


	template<class K, class V>
	class SerializationListOptions_ForMap : public BB::SerializationListOptions { // do only use with std::map<K, V> !
		using VectT = std::map<K, V>;
		using IterT = VectT::iterator;

		struct IterImpl : Iter {
			VectT* Vector;
			IterT It;
			bool First = true;

			IterImpl(VectT* v)
			{
				Vector = v;
			}
		};

		static inline std::pair<K, V> CurrentlyAdding{};

		static void* __stdcall AddToListImp(void* l)
		{
			return &CurrentlyAdding;
		}
		static Iter* __stdcall AllocIterImp(void* l)
		{
			return new IterImpl{ static_cast<VectT*>(l) };
		}
		static bool __stdcall IterNextImp(Iter* i)
		{
			IterImpl* it = static_cast<IterImpl*>(i);
			if (it->First) {
				it->It = it->Vector->begin();
				it->First = false;
			}
			else {
				++it->It;
			}
			return it->It != it->Vector->end();
		}
		static void* __stdcall IterCurrentImp(Iter* i)
		{
			IterImpl* it = static_cast<IterImpl*>(i);
			return &*it->It;
		}
		static void __stdcall FreeIterImp(Iter* i)
		{
			IterImpl* it = static_cast<IterImpl*>(i);
			delete it;
		}
		static size_t __stdcall GetSizeImp(void* l)
		{
			auto* v = static_cast<VectT*>(l);
			return v->size();
		}
		static void __stdcall FinalizeAddToListImp(void* l)
		{
			auto* v = static_cast<VectT*>(l);
			v->insert(CurrentlyAdding);
		}

	public:
		SerializationListOptions_ForMap()
		{
			AddToList = &AddToListImp;
			AllocIter = &AllocIterImp;
			IterNext = &IterNextImp;
			IterCurrent = &IterCurrentImp;
			FreeIter = &FreeIterImp;
			FinalizeAddToList = &FinalizeAddToListImp;
			GetSize = &GetSizeImp;
		}
	};

	class StringSerializer : public BB::FieldSerilaizer {
		std::string ActualBuffer;

		static void __stdcall DeserializeFromStringImp(void* data, const char* buff);
		static int __stdcall SerializeToStringImp(char* buff, size_t s, const void* data);
		static void __stdcall DeserializeFromStreamImp(void* data, BB::IStream* str);
		static void __stdcall SerializeToStreamImp(BB::IStream* str, const void* data);

	public:
		StringSerializer();
		static StringSerializer GlobalObj;
	};

	class ClassIdManager {
		BB::CClassFactory* Manager;

	public:
		inline ClassIdManager(BB::CClassFactory* mng) : Manager(mng) {
		}

		// does not add
		inline shok::ClassId GetIdByName(const char* name) const {
			return Manager->GetIdentifierByName(name);
		}
		inline const char* GetNameByID(shok::ClassId id) const {
			return Manager->GetClassDemangledName(id);
		}
		inline size_t size() const {
			return Manager->Info.size;
		}

		struct Iter {
		protected:
			friend class ClassIdManager;
			shok::Map<shok::ClassId, BB::CClassFactory::ClassInfo>::Iter I;

			inline Iter(shok::Map<shok::ClassId, BB::CClassFactory::ClassInfo>::Iter i) : I(i) {}

		public:
			auto operator<=>(const Iter&) const noexcept = default;
			inline shok::ClassId operator*() const noexcept {
				return I->first;
			}
			inline Iter& operator++() {
				++I;
				return *this;
			}
			inline Iter operator++(int) {
				Iter r = *this;
				++(*this);
				return r;
			}
		};
		inline Iter begin() const {
			return Iter{ Manager->Info.begin() };
		}
		inline Iter end() const {
			return Iter{ Manager->Info.end() };
		}
	};
	// iterates only over ids registered in BB::CClassFactory::GlobalObj
	template<>
	inline auto GetIdManager<shok::ClassId>() {
		auto mng = *BB::CClassFactory::GlobalObj;
		if (mng == nullptr)
			throw std::runtime_error{"shok::ClassId manager not yet initialized"};
		return ClassIdManager{mng};
	}
}

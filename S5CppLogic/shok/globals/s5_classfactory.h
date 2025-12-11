#pragma once
#include <memory>
#include <random>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/globals/s5_idmanager.h>
#include <Luapp/constexprTypename.h>
#include <luaext.h>
#include <utility/ConstexprString.h>

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
		CXmlSerializer() = default;
		static void __stdcall WriteType(BB::IStream* f, const char* cn);
		static void __attribute((naked)) WriteTypeASM();
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

		static CBinarySerializer* Create(int uk, int uk2);
		static std::unique_ptr<CBinarySerializer, CppLogic::DestroyCaller<CBinarySerializer>> CreateUnique(int uk = 0, int uk2 = 2);

	private:
		CBinarySerializer() = default;
	};

	struct FieldSerializer {
		void(__stdcall* DeserializeFromString)(void* data, const char* buff) = nullptr;
		int(__stdcall* SerializeToString)(char* buff, size_t s, const void* data) = nullptr; // returns num chars written if buffer would be sufficient, negative on error
		void(__stdcall* DeserializeFromStream)(void* data, BB::IStream* str) = nullptr;
		void(__stdcall* SerializeToStream)(BB::IStream* str, const void* data) = nullptr;
		BB::CIDManagerEx* (__stdcall* GetIdManager)() = nullptr; // may be nullptr
		bool IsPrimitive = false;
		void* Buffer = nullptr; // p to a memory block of the serialized type, usually the buffer is directly after the serializer structure

		static inline BB::FieldSerializer* const TypeInt = reinterpret_cast<BB::FieldSerializer*>(0x810C98);
		static inline BB::FieldSerializer* const TypeUInt = reinterpret_cast<BB::FieldSerializer*>(0x810CD8);
		static inline BB::FieldSerializer* const TypeFloat = reinterpret_cast<BB::FieldSerializer*>(0x810C78);
		static inline BB::FieldSerializer* const TypeDouble = reinterpret_cast<BB::FieldSerializer*>(0x82BB38);
		static inline BB::FieldSerializer* const TypeBool = reinterpret_cast<BB::FieldSerializer*>(0x810C58);
		static inline BB::FieldSerializer* const TypeString = reinterpret_cast<BB::FieldSerializer*>(0x8640F0);
		static inline BB::FieldSerializer* const TypeCharBuff = reinterpret_cast<BB::FieldSerializer*>(0x810CB8);
		static inline BB::FieldSerializer* const TypeClassIdentifier = reinterpret_cast<BB::FieldSerializer*>(0x813978);
		static inline BB::FieldSerializer* const TypeTaskList = reinterpret_cast<BB::FieldSerializer*>(0x85D4AC);
		static inline BB::FieldSerializer* const TypeUByte = reinterpret_cast<BB::FieldSerializer*>(0x80C380);
		// does not add ids
		static inline BB::FieldSerializer* const TypeEntityType = reinterpret_cast<BB::FieldSerializer*>(0x85D4D0);
		static inline BB::FieldSerializer* const TypeModel = reinterpret_cast<BB::FieldSerializer*>(0x8585C0);
		static inline BB::FieldSerializer* const TypeAnim = reinterpret_cast<BB::FieldSerializer*>(0x86057C);
		static inline BB::FieldSerializer* const TypeAnimCategory = reinterpret_cast<BB::FieldSerializer*>(0x875494);
		static inline BB::FieldSerializer* const TypeDamageClass = reinterpret_cast<BB::FieldSerializer*>(0x860558);
		static inline BB::FieldSerializer* const TypeEffectType = reinterpret_cast<BB::FieldSerializer*>(0x8605A0);
		static inline BB::FieldSerializer* const TypeWidgetID = reinterpret_cast<BB::FieldSerializer*>(0x894708);
		static inline BB::FieldSerializer* const TypeArmorClassId = reinterpret_cast<BB::FieldSerializer*>(0x85D5CC);
		static inline BB::FieldSerializer* const TypeEntityCategory = reinterpret_cast<BB::FieldSerializer*>(0x85D4F4);
		static inline BB::FieldSerializer* const TypeAttachmentType = reinterpret_cast<BB::FieldSerializer*>(0x868408);
		static inline BB::FieldSerializer* const TypeAccessCategories = reinterpret_cast<BB::FieldSerializer*>(0x810D18);

		template<class T>
		static constexpr BB::FieldSerializer* GetSerializer() = delete;

		template<>
		inline BB::FieldSerializer* GetSerializer<int>() {
			return TypeInt;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<unsigned int>() {
			return TypeUInt;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<float>() {
			return TypeFloat;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<bool>() {
			return TypeBool;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<double>() {
			return TypeDouble;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::ArmorClassId>() {
			return TypeArmorClassId;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::EntityId>() {
			return TypeInt; // no real serializer, but int should work
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::EntityTypeId>() {
			return TypeEntityType;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::PlayerId>() {
			return TypeInt; // no real serializer, but int should work
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::EffectTypeId>() {
			return TypeEffectType;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::DamageClassId>() {
			return TypeDamageClass;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::TaskListId>() {
			return TypeTaskList;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::EntityCategory>() {
			return TypeEntityCategory;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::EventIDs>() {
			return TypeInt; // no real serializer, but int should work
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::AttachmentType>() {
			return TypeAttachmentType;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::AdvancedDealDamageSource>() {
			return TypeUByte; // no real serializer, but int should work
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::ModelId>() {
			return TypeModel;
		}
		template<>
		inline BB::FieldSerializer* GetSerializer<shok::AnimationId>() {
			return TypeAnim;
		}

		struct ExtendedInfo {
			std::string_view Name;
			void (* const Push)(lua::State L, void* data, const FieldSerializer* fs);
			void (* const Check)(lua::State L, void* data, int idx, const FieldSerializer* fs);
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

		std::unique_ptr<BB::SerializationListOptions::Iter, void(__stdcall*)(BB::SerializationListOptions::Iter* i)> UniqueIter(void* list) const;

		struct ExtendedInfo {
			enum class Ty : int {
				Unknown,
				Vector,
				Map,
				Array,
				List,
			};

			Ty Type;
			std::string_view Name;
			void* (*IndexNumeric)(void* List, size_t index);
			void (*RemoveIf)(void* List, bool(*cond)(void* uv, const BB::SerializationData* sd, void* elem), void* uv, const BB::SerializationData* sd);
		};

		const ExtendedInfo& GetExtendedInfo() const;
		const ExtendedInfo* TryGetExtendedInfo() const;
	protected:
		void RegisterExtended(const ExtendedInfo& e) const;
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
		const BB::FieldSerializer* DataConverter = nullptr;
		const BB::SerializationData* SubElementData = nullptr; //5
		shok::ClassId (__stdcall* GetIdentifier)(void* data) = nullptr;
		const BB::SerializationListOptions* ListOptions = nullptr;
		int Unknown3 = 0;

	private:
		static shok::ClassId __stdcall GetBBIdentifier(void* d);
	public:
		static constexpr SerializationData FieldData(const char* name, size_t pos, size_t size, const BB::FieldSerializer* converter, const BB::SerializationListOptions* list = nullptr)
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

		static const SerializationData* GetSerializationDataFromId(shok::ClassId id);
		template<class T>
		static const SerializationData* GetSerializationData() = delete;
		template<class T>
		requires requires { T::SerializationData; }
		static const SerializationData* GetSerializationData() {
			return T::SerializationData;
		}
		template<HasValidIdentifier T>
		requires (!(requires { T::SerializationData; }))
		static const SerializationData* GetSerializationData() {
			return GetSerializationDataFromId(T::Identifier);
		}

		template<class T, CppLogic::ConstexprString ValueName = CppLogic::ConstexprString("Value"), CppLogic::ConstexprString KeyName = CppLogic::ConstexprString("Key")>
		struct AutoDataHolder {
			static constexpr SerializationData AutoData(const char* name, size_t pos, size_t size) {
				static_assert(std::same_as<T, void>, "needs specialization");
				static_assert(!std::same_as<T, void>, "needs specialization");
				return GuardData();
			}
		};

		template<class T, class B>
		requires std::derived_from<T, B>
		static constexpr SerializationData AutoBaseClass() {
			return EmbeddedData(nullptr, 0, sizeof(B), GetSerializationData<B>());
		}
	};
	static_assert(sizeof(BB::SerializationData) == 4 * 9);
	#define AutoMemberSerialization(T,M) BB::SerializationData::AutoDataHolder<decltype(T::M)>::AutoData(#M, offsetof(T, M), sizeof(decltype(T::M)))
	#define AutoMemberSerializationName(T,M,N) BB::SerializationData::AutoDataHolder<decltype(T::M)>::AutoData(N, offsetof(T, M), sizeof(decltype(T::M)))
	#define AutoMemberSerializationNameKeyVaue(T,M,N,K,V) BB::SerializationData::AutoDataHolder<decltype(T::M), CppLogic::ConstexprString(V), CppLogic::ConstexprString(K)>::AutoData(N, offsetof(T, M), sizeof(decltype(T::M)))
	template<>
	constexpr SerializationData SerializationData::AutoDataHolder<BB::IObject*>::AutoData(const char* name, size_t pos, size_t size) {
		return SerializationData::ObjectPointerData(name, pos, size);
	}
	template<class T>
	requires requires { SerializationData::GetSerializationData<T>(); }
	struct SerializationData::AutoDataHolder<T> {
		static constexpr SerializationData AutoData(const char* name, size_t pos, size_t size) {
			return SerializationData::EmbeddedData(name, pos, size, SerializationData::GetSerializationData<T>());
		}
	};
	template<class T>
	requires requires { FieldSerializer::GetSerializer<T>(); }
	struct SerializationData::AutoDataHolder<T> {
		static constexpr SerializationData AutoData(const char* name, size_t pos, size_t size) {
			return SerializationData::FieldData(name, pos, size, FieldSerializer::GetSerializer<T>());
		}
	};

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
		virtual void __stdcall RemoveCToolRuntimeClass(BB::IObject* creator) = 0;
	private:
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

namespace EToolsManager {
	class IClassInfo {
	public:
		virtual bool CheckSomething(int) = 0;
		virtual int* GetSomething() = 0;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x425513F0);
	};

	// this is some kind of fancy singleton
	template<BB::HasValidIdentifier T>
	class CToolRuntimeClass : public BB::IObject, public BB::IObjectCreator, public IClassInfo {
	public:
		PADDINGI(1); // looks to be a nullptr
		std::unique_ptr<T> Singleton; // not a unique_ptr, but it makes stuff easier

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x42543653); // looks like all use the same

		virtual shok::ClassId __stdcall GetClassIdentifier() const override{
			return Identifier;
		}
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override {
			if (id == IClassInfo::Identifier)
				return static_cast<IClassInfo*>(this);
			else if (id == BB::IObjectCreator::Identifier)
				return static_cast<BB::IObjectCreator*>(this);
			else
				return nullptr;
		}

		virtual shok::ClassId __stdcall GetIdOfCreated() override {
			return T::Identifier;
		}
		virtual const char* __stdcall GetNameOfCreated() override {
			return typeid(T).name();
		}
		virtual BB::IObject* __stdcall Create() override {
			if (Singleton == nullptr) {
				Singleton = new T();
			}
			// if casttoident to 0x151861E7 == 0, set to 0
			return Singleton.get();
		}

		virtual bool CheckSomething(int) override {
			return 0;
		}
		virtual int* GetSomething() override {
			return nullptr;
		}
	};
}

namespace CppLogic {
	template<class T, class Alloc>
	class SerializationListOptions_ForVector : public BB::SerializationListOptions { // do only use with std::vector<T> !
	public:
		using VectT = std::vector<T, Alloc>;
		using IterT = std::vector<T, Alloc>::iterator;
		
	private:
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
		static void* Index(void* List, size_t index) {
			auto* v = reinterpret_cast<VectT*>(List);
			T* r = nullptr;
			if (index >= 0 && index < v->size())
				r = &(*v)[index];
			return r;
		}
		static void RemoveIf(void* List, bool(*cond)(void* uv, const BB::SerializationData* sd, void* elem), void* uv, const BB::SerializationData* sd) {
			auto* v = reinterpret_cast<VectT*>(List);
			auto e = std::remove_if(v->begin(), v->end(), [&](T& v) {
				return cond(uv, sd, &v);
				});
			v->erase(e, v->end());
		}


		ExtendedInfo Extended;
		
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
			Extended.Type = ExtendedInfo::Ty::Vector;
			Extended.Name = typename_details::type_name<VectT>();
			Extended.IndexNumeric = &Index;
			Extended.RemoveIf = &RemoveIf;
			RegisterExtended(Extended);
		}
	};


	template<class K, class V, class Cmp, class Alloc>
	class SerializationListOptions_ForMap : public BB::SerializationListOptions { // do only use with std::map<K, V> !
	public:
		using VectT = std::map<K, V, Cmp, Alloc>;
		using IterT = VectT::iterator;

	private:
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

		static void RemoveIf(void* List, bool(*cond)(void* uv, const BB::SerializationData* sd, void* elem), void* uv, const BB::SerializationData* sd) {
			auto* v = reinterpret_cast<VectT*>(List);
			auto i = v->begin();
			while (i != v->end()) {
				if (cond(uv, sd, &*i))
					i = v->erase(i);
				else
					++i;
			}
		}


		ExtendedInfo Extended;

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
			Extended.Type = ExtendedInfo::Ty::Map;
			Extended.Name = typename_details::type_name<VectT>();
			Extended.IndexNumeric = nullptr;
			Extended.RemoveIf = &RemoveIf;
			RegisterExtended(Extended);
		}
	};

	template<class K, class V, ConstexprString A, class Cmp, class Alloc>
	class SerializationListOptions_ForMap_KeyAttribute : public BB::SerializationListOptions { // do only use with std::map<K, V> !
	public:
		using VectT = std::map<K, V, Cmp, Alloc>;

	private:
		static void* __stdcall AddToListIdManagedImpl(void* l, BB::SerializationListOptions::Context* context) {
			const char* id = context->GetAttribute(A.data());
			if (id == nullptr)
				return nullptr;
			VectT* map = static_cast<VectT*>(l);
			auto [it, s] = map->emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple());
			return &it->second;
		}

	public:
		SerializationListOptions_ForMap_KeyAttribute()
		{
			AddToListIdManaged = &AddToListIdManagedImpl;
		}
	};

	class StringSerializer : public BB::FieldSerializer {
		std::string ActualBuffer;

		static void __stdcall DeserializeFromStringImp(void* data, const char* buff);
		static int __stdcall SerializeToStringImp(char* buff, size_t s, const void* data);
		static void __stdcall DeserializeFromStreamImp(void* data, BB::IStream* str);
		static void __stdcall SerializeToStreamImp(BB::IStream* str, const void* data);

	public:
		StringSerializer();
		static StringSerializer GlobalObj;
	};

	template<class T, CppLogic::ConstexprString ValueName = CppLogic::ConstexprString(""), CppLogic::ConstexprString KeyName = CppLogic::ConstexprString("")>
	struct SerializationListOptions_Mapping {
		static constexpr bool Mapped = false;
		using VectT = void;

		static const SerializationListOptions_Mapping GlobalObj;
	};
	template<class K, class V, class Cmp, class Alloc, CppLogic::ConstexprString ValueName, CppLogic::ConstexprString KeyName>
	struct SerializationListOptions_Mapping<std::map<K, V, Cmp, Alloc>, ValueName, KeyName> {
		static constexpr bool Mapped = true;

		using KeyValue = std::pair<K, V>;
		static constexpr auto KN = KeyName;
		static constexpr auto VN = ValueName;

		using ListOpt = SerializationListOptions_ForMap<K, V, Cmp, Alloc>;
		using VectT = typename ListOpt::VectT;

		ListOpt ListOptions;
		const BB::SerializationData Seridata[3] = {
			AutoMemberSerializationName(KeyValue, first, KeyName.data()),
			AutoMemberSerializationName(KeyValue, second, ValueName.data()),
			BB::SerializationData::GuardData(),
		};

		static const SerializationListOptions_Mapping GlobalObj;
	};
	template<class T, class Alloc, CppLogic::ConstexprString ValueName, CppLogic::ConstexprString KeyName>
	struct SerializationListOptions_Mapping<std::vector<T, Alloc>, ValueName, KeyName> {
		static constexpr bool Mapped = true;

		using ListOpt = SerializationListOptions_ForVector<T, Alloc>;
		using VectT = typename ListOpt::VectT;

		ListOpt ListOptions;

		static const SerializationListOptions_Mapping GlobalObj;
	};
	template<class T, CppLogic::ConstexprString A = CppLogic::ConstexprString("")>
	struct SerializationListOptions_Mapping_KeyAttrib {
		static constexpr bool Mapped = false;
		using VectT = void;

		static const SerializationListOptions_Mapping_KeyAttrib GlobalObj;
	};
	template<class K, class V, ConstexprString A, class Cmp, class Alloc>
	struct SerializationListOptions_Mapping_KeyAttrib<std::map<K, V, Cmp, Alloc>, A> {
		static constexpr bool Mapped = true;

		using ListOpt = SerializationListOptions_ForMap_KeyAttribute<K, V, A, Cmp, Alloc>;
		using VectT = typename ListOpt::VectT;

		ListOpt ListOptions;

		static const SerializationListOptions_Mapping_KeyAttrib GlobalObj;
	};


	template<class K, class V, ConstexprString A, class Cmp, class Alloc>
	constexpr BB::SerializationData AutoData_KeyAttrib(const char* name, size_t pos, size_t size, const SerializationListOptions_Mapping_KeyAttrib<std::map<K, V, Cmp, Alloc>, A>&) {
		using M = SerializationListOptions_Mapping_KeyAttrib<std::map<K, V, Cmp, Alloc>, A>;
		static_assert(std::same_as<typename M::VectT, std::map<K, V, Cmp, Alloc>>);
		auto& mapping = M::GlobalObj;
		auto d = BB::SerializationData::AutoDataHolder<V>::AutoData(name, pos, size);
		d.ListOptions = &mapping.ListOptions;
		return d;
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
			bool operator==(const Iter&) const noexcept = default;
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

template<>
inline BB::FieldSerializer* BB::FieldSerializer::GetSerializer<std::string>() {
	return &CppLogic::StringSerializer::GlobalObj;
}
template<>
inline BB::FieldSerializer* BB::FieldSerializer::GetSerializer<std::minstd_rand>() {
	static_assert(sizeof(std::minstd_rand) == sizeof(int));
	return BB::FieldSerializer::TypeInt; // just serialize the state as int
}

template<class K, class V, class Cmp, class Alloc, CppLogic::ConstexprString ValueName, CppLogic::ConstexprString KeyName>
requires (CppLogic::SerializationListOptions_Mapping<std::map<K, V, Cmp, Alloc>>::Mapped)
struct BB::SerializationData::AutoDataHolder<std::map<K, V, Cmp, Alloc>, ValueName, KeyName> {
	static constexpr SerializationData AutoData(const char* name, size_t pos, size_t size) {
		using M = CppLogic::SerializationListOptions_Mapping<std::map<K, V, Cmp, Alloc>, ValueName, KeyName>;
		static_assert(std::same_as<typename M::VectT, std::map<K, V, Cmp, Alloc>>);
		auto& mapping = M::GlobalObj;
		return SerializationData::EmbeddedData(name, pos, size, mapping.Seridata, &mapping.ListOptions);
	}
};
template<class T, class Alloc, CppLogic::ConstexprString ValueName, CppLogic::ConstexprString KeyName>
requires (CppLogic::SerializationListOptions_Mapping<std::vector<T, Alloc>>::Mapped)
struct BB::SerializationData::AutoDataHolder<std::vector<T, Alloc>, ValueName, KeyName> {
	static constexpr SerializationData AutoData(const char* name, size_t pos, size_t size) {
		using M = CppLogic::SerializationListOptions_Mapping<std::vector<T, Alloc>, ValueName, KeyName>;
		static_assert(std::same_as<typename M::VectT, std::vector<T, Alloc>>);
		auto& mapping = M::GlobalObj;
		auto d = BB::SerializationData::AutoDataHolder<T>::AutoData(name, pos, size);
		d.ListOptions = &mapping.ListOptions;
		return d;
	}
};

#define CreateSerializationListForKeyValue(T,M,K,V) \
template<> \
const CppLogic::SerializationListOptions_Mapping<decltype(T::M), CppLogic::ConstexprString(V), CppLogic::ConstexprString(K)> \
	CppLogic::SerializationListOptions_Mapping<decltype(T::M), CppLogic::ConstexprString(V), CppLogic::ConstexprString(K)>::GlobalObj{}
#define CreateSerializationListFor(T,M) CreateSerializationListForKeyValue(T,M,"Key","Value")
#define CreateSerializationListForKeyAttrib(T,M,N) \
template<>\
const  CppLogic::SerializationListOptions_Mapping_KeyAttrib<decltype(T::M), CppLogic::ConstexprString(N)> \
	CppLogic::SerializationListOptions_Mapping_KeyAttrib<decltype(T::M), CppLogic::ConstexprString(N)>::GlobalObj{};
#define AutoMemberSerializationNameKeyAttrib(T,M,N,K) CppLogic::AutoData_KeyAttrib(N, offsetof(T, M), sizeof(decltype(T::M)), CppLogic::SerializationListOptions_Mapping_KeyAttrib<decltype(T::M), CppLogic::ConstexprString(K)>::GlobalObj)

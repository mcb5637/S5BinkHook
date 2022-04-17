#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "Luapp/constexprTypename.h"
#include "Luapp/luapp50.h"

namespace BB {
	class IXmlSerializer {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual void __stdcall Deserialize(BB::CFileStreamEx* f, BB::IObject* ob) = 0; // open file in mode 0x113
	private:
		virtual void unknown() = 0;
	public:
		virtual void __stdcall Serialize(BB::CFileStreamEx* f, BB::IObject* ob) = 0; // open file in mode 0x121
	protected:
		virtual ~IXmlSerializer() = default;
	public:
		virtual void __stdcall DeserializeByData(BB::CFileStreamEx* f, BB::IObject* ob, BB::SerializationData* d) = 0;
		virtual void __stdcall SerializeByData(BB::CFileStreamEx* f, BB::IObject* ob, BB::SerializationData* d, char* xmlrootname) = 0;
	};
	class CXmlSerializer : public IXmlSerializer {
	public:
		PADDINGI(45); // 43 + a bit of extra space

		static inline constexpr int vtp = 0x77FA2C;
		static inline constexpr int TypeDesc = 0x8311F0;

		using IXmlSerializer::Deserialize;
		using IXmlSerializer::Serialize;
		static BB::CXmlSerializer* Create();
		void Deserialize(const char* filename, BB::IObject* ob);
		// i doubt this will ever get used outside of debugging
		void Serialize(const char* filename, BB::IObject* ob);

	private:
		BB::CXmlSerializer() = default;
	};

	struct FieldSerilaizer {
		int(__stdcall* DeserializeFromString)(void* data, const char* buff);
		int(__stdcall* SerializeToString)(char* buff, size_t s, void* data);
		// probably the same for binary
		// int, int
		// data?

		static inline BB::FieldSerilaizer* const TypeInt = reinterpret_cast<BB::FieldSerilaizer*>(0x810C98);
		static inline BB::FieldSerilaizer* const TypeUInt = reinterpret_cast<BB::FieldSerilaizer*>(0x810CD8);
		static inline BB::FieldSerilaizer* const TypeFloat = reinterpret_cast<BB::FieldSerilaizer*>(0x810C78);
		static inline BB::FieldSerilaizer* const TypeBool = reinterpret_cast<BB::FieldSerilaizer*>(0x810C58);
		static inline BB::FieldSerilaizer* const TypeString = reinterpret_cast<BB::FieldSerilaizer*>(0x8640F0);
		static inline BB::FieldSerilaizer* const TypeCharBuff = reinterpret_cast<BB::FieldSerilaizer*>(0x810CB8);
		static inline BB::FieldSerilaizer* const TypeClassIdentifier = reinterpret_cast<BB::FieldSerilaizer*>(0x813978);
		static inline BB::FieldSerilaizer* const TypeTaskList = reinterpret_cast<BB::FieldSerilaizer*>(0x85D4AC);
		// does not add ids
		static inline BB::FieldSerilaizer* const TypeEntityType = reinterpret_cast<BB::FieldSerilaizer*>(0x85D4D0);
		static inline BB::FieldSerilaizer* const TypeModel = reinterpret_cast<BB::FieldSerilaizer*>(0x8585C0);
		static inline BB::FieldSerilaizer* const TypeAnim = reinterpret_cast<BB::FieldSerilaizer*>(0x86057C);
		static inline BB::FieldSerilaizer* const TypeDamageClass = reinterpret_cast<BB::FieldSerilaizer*>(0x860558);
		static inline BB::FieldSerilaizer* const TypeEffectType = reinterpret_cast<BB::FieldSerilaizer*>(0x8605A0);

		template<class T>
		static constexpr BB::FieldSerilaizer* GetSerilalizer() = delete;

		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<int>() {
			return TypeInt;
		}
		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<float>() {
			return TypeFloat;
		}
		template<>
		static constexpr BB::FieldSerilaizer* GetSerilalizer<bool>() {
			return TypeBool;
		}

		struct ExtendedInfo {
			const char* Name;
			void (* const Push)(lua::State L, void* data, const FieldSerilaizer* fs);
			void (* const Check)(lua::State L, void* data, int idx, const FieldSerilaizer* fs);
		};

		std::string GetTypeDescName() const;

		const ExtendedInfo& GetExtendedInfo() const;
	};

	struct SerializationListOptions {
		struct iter {
			void* List;
			unsigned int data; // usually a pointer
			bool first = true;
		};

		void* (__stdcall* AddToList)(void* List) = nullptr; // gets list, allocates, then returns p to new object
		int z = 0;
		iter* (__stdcall* AllocIter)(void* List) = nullptr;
		bool(__stdcall* IterNext)(iter* i) = nullptr;
		void* (__stdcall* IterCurrent)(iter* i) = nullptr;
		void(__stdcall* FreeIter)(iter* i) = nullptr;
		int nullsub = 0x52B509;
		size_t(__stdcall* GetSize)(void* List) = nullptr;
	};

	struct SerializationData { // use a 0-terminated array (default constructed is 0)
		int Type = 0; // 2 direct val (uses dataconverter), 3 embedded (uses subelement), 5 embedded bbobject, 6 p to bbobject (uses getidentifier or subelement)
		const char* SerializationName = nullptr; // if not set, automatically follows subelementdata with a position of 0
		size_t Position = 0;
		size_t Size = 0;
		const BB::FieldSerilaizer* DataConverter = nullptr;
		const BB::SerializationData* SubElementData = nullptr; //5
		unsigned int (__stdcall* GetIdentifier)(void* data) = nullptr;
		const BB::SerializationListOptions* ListOptions = nullptr;
		int Unknown3 = 0;

	private:
		static unsigned int __stdcall GetBBIdentifier(void* d);
	public:
		static constexpr SerializationData FieldData(const char* name, size_t pos, size_t size, const BB::FieldSerilaizer* converter, const BB::SerializationListOptions* list = nullptr)
		{
			return SerializationData{ 2, name, pos, size, converter, nullptr, nullptr, list, 0 };
		}
		static constexpr SerializationData EmbeddedData(const char* name, size_t pos, size_t size, const SerializationData* subdata, const BB::SerializationListOptions* list = nullptr)
		{
			return SerializationData{ 3, name, pos, size, nullptr, subdata, nullptr, list, 0 };
		}
		static constexpr SerializationData GuardData()
		{
			return SerializationData{ 0, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, 0 };
		}
		static constexpr SerializationData ObjectPointerData(const char* name, size_t pos, size_t size, const BB::SerializationListOptions* list = nullptr) {
			return SerializationData{ 6, name, pos, size, nullptr, nullptr, &GetBBIdentifier, list, 0 };
		}

		static const SerializationData* GetSerializationData(unsigned int id);
		template<class T>
		static const SerializationData* GetSerializationData() = delete;
		template<class T>
		requires requires { T::SerializationData; }
		static const SerializationData* GetSerializationData() {
			return T::SerializationData;
		}
		template<class T>
		requires requires { T::Identifier; }
		static const SerializationData* GetSerializationData() {
			return GetSerializationData(T::Identifier);
		}
	};
	static_assert(sizeof(BB::SerializationData) == 4 * 9);
#define MemberSerializationSizeAndOffset(T,M) offsetof(T, M), sizeof(decltype(T::M))
#define MemberSerializationFieldData(T,M) MemberSerializationSizeAndOffset(T, M), BB::FieldSerilaizer::GetSerilalizer<decltype(T::M)>()
#define MemberSerializationEmbeddedData(T,M) MemberSerializationSizeAndOffset(T, M), BB::SerializationData::GetSerializationData<decltype(T::M)>()

	class IClassFactory {
	public:
		virtual ~IClassFactory() = default;
	private:
		virtual void unknown0() = 0;
		virtual void unknown1() = 0;
		virtual void unknown2() = 0;
	public:
		virtual BB::IObject* __stdcall CreateObject(unsigned int id) = 0;
		virtual unsigned int __stdcall GetIdentifierByName(const char* name) = 0;
		virtual const char* __stdcall GetClassDemangledName(unsigned int id) = 0;
		virtual const BB::SerializationData* __stdcall GetSerializationDataForClass(unsigned int id) = 0; // returns a 0-terminated array
	private:
		virtual void unknown3() = 0;
	public:
		virtual void __stdcall AddClassToFactory(unsigned int id, const char* name, BB::IObject* (__stdcall* NewObj)(), const BB::SerializationData* data) = 0;
	};

	class CClassFactory : public IClassFactory {
	public:
		static inline constexpr int vtp = 0x77F74C;
		static inline constexpr int TypeDesc = 0x830C80;

		using IClassFactory::CreateObject;
		template<class T>
		T* CreateObject()
		{
			return dynamic_cast<T*>(CreateObject(T::Identifier));
		}

		void LoadObject(BB::IObject* ob, const char* filename);
		template<class T>
		T* LoadObject(const char* filename)
		{
			T* ob = CreateObject<T>();
			if (!ob)
				return nullptr;
			LoadObject(ob, filename);
			return ob;
		}

		using IClassFactory::GetSerializationDataForClass;
		template<class T>
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
			AddClassToFactory(T::Identifier, typename_details::type_name<T>(), &FactObjCreator<T>, T::SerializationData);
		}


		static inline BB::CClassFactory** const GlobalObj = reinterpret_cast<BB::CClassFactory**>(0x88F044);
	};
}

namespace CppLogic {
	template<class T>
	struct SerializationListOptions_ForVector : BB::SerializationListOptions { // do not use with shok::Vector !
		SerializationListOptions_ForVector() {
			AddToList = [](void* l) {
				std::vector<T>* v = static_cast<std::vector<T>*>(l);
				v->push_back(T{});
				return static_cast<void*>(v->data() + (v->size() - 1));
			};
			AllocIter = [](void* l) {
				return new iter{ l, 0, true };
			};
			IterNext = [](iter* i) {
				std::vector<T>* v = static_cast<std::vector<T>*>(i->List);
				if (i->first) {
					i->first = false;
					i->data = 0;
				}
				else {
					i->data++;
				}
				return i->data < v->size();
			};
			IterCurrent = [](iter* i) {
				std::vector<T>* v = static_cast<std::vector<T>*>(i->List);
				return static_cast<void*>(v->data() + i->data);
			};
			FreeIter = [](iter* i) {
				delete i;
			};
			GetSize = [](void* l) {
				std::vector<T>* v = static_cast<std::vector<T>*>(l);;
				return v->size();
			};
		}
	};
}

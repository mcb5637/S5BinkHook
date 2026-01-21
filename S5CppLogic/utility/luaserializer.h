#pragma once
#include <map>
#include <format>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/s5_defines.h>
#include <shok/globals/s5_classfactory.h>
#include <luaext.h>
#include <utility/EnumIdManagerMagic.h>

namespace CppLogic::Serializer {
	class ObjectToLuaSerializer {
		static void SerializeField(luaext::State L, void* o, const BB::SerializationData* s, bool keypushed = false);
		static void SerializeFields(luaext::State L, void* o, const BB::SerializationData* s);
		static void SerializePushField(luaext::State L, void* o, const BB::SerializationData* s);
		static void SerializeList(luaext::State L, void* o, const BB::SerializationData* s);

		static void DeserializeField(luaext::State L, void* o, const BB::SerializationData* s, bool valuepushed = false);
		static void DeserializeCheckField(luaext::State L, void* o, const BB::SerializationData* s);
		static void DeserializeFields(luaext::State L, void* o, const BB::SerializationData* s);
		static void DeserializeList(luaext::State L, void* o, const BB::SerializationData* s);
	public:
		static void Serialize(luaext::State L, void* o, const BB::SerializationData* seri, shok::ClassId id = shok::ClassId::Invalid);
		static void Serialize(luaext::State L, BB::IObject* o);

		// if o nullptr, gets created, if id 0, gets read from lua, in this case also checks seri for 0
		static void* Deserialize(luaext::State L, void* o, const BB::SerializationData* seri, shok::ClassId id = shok::ClassId::Invalid, std::initializer_list<shok::ClassId> whitelisted_ids = {});
		static void Deserialize(luaext::State L, BB::IObject* o);

		static void DumpClassSerializationData(luaext::State L, const BB::SerializationData* seri);
		static void DumpClassSerializationData(luaext::State L, shok::ClassId id);
	};

	class FastBinarySerializer {
		static void SerializeField(BB::IStream& s, void* o, const BB::SerializationData* seri);
		static void SerializeFields(BB::IStream& s, void* o, const BB::SerializationData* seri);
		static void SerializeList(BB::IStream& s, void* o, const BB::SerializationData* seri);

		static void DeserializeField(BB::IStream& s, void* o, const BB::SerializationData* seri);
		static void DeserializeFields(BB::IStream& s, void* o, const BB::SerializationData* seri);
		static void DeserializeList(BB::IStream& s, void* o, const BB::SerializationData* seri);

	public:
		static void Serialize(BB::IStream& s, void* o, const BB::SerializationData* seri);
		static void Serialize(BB::IStream& s, BB::IObject* o, shok::ClassId id = shok::ClassId::Invalid);

		// if o nullptr, gets created, if id 0, gets read from lua, in this case also checks seri for 0
		static void* Deserialize(BB::IStream& s, void* o, const BB::SerializationData* seri, shok::ClassId id = shok::ClassId::Invalid, std::initializer_list<shok::ClassId> whitelisted_ids = {});
		static void Deserialize(BB::IStream& s, BB::IObject* o);
	};

	/// <summary>
	/// better serialization of lua states.
	/// serialize writes the type (so it can write a reference instead).
	/// deserialize checks type then dispatches.
	/// <para>to serialize userdata: put a serializer function into metatable[UserdataSerializerMetaEvent], func(ud)->typenameString, anything.</para>
	/// <para>and registry a deserializer for that name in UserdataDeserializer[typenameString] = lua::CppToCFunction&lt;...&gt;, func(anything)->ud.
	/// anything can be any serializable lua value, including a table, but may not reference the to-be-serialized userdata (other serializable userdata is allowed).</para>
	/// <para>you may use CppLogic::Serializer::ObjectToLuaSerializer to turn a C++ object via BB::SerializationData into a table and back.</para>
	/// </summary>
	class AdvLuaStateSerializer {
		struct Reference {
			lua::LType Type;
			const void* Id;

			auto operator<=>(const Reference&) const = default;
		};
		struct UpvalueReference {
			int FuncReference;
			int UpvalueNum;
		};


		BB::IStream& IO;
		luaext::State L;
		std::unique_ptr<byte[]> Data;
		size_t DataLength = 0;
		std::map<Reference, int> RefToNumber;
		std::map<const void*, UpvalueReference> UpRefs;
		int NextRefereceNumber = 1;
		int IndexOfReferenceHolder = 0;
		void* (__cdecl*lua_upvalueid)(lua_State* L, int funcindex, int n) = nullptr;
		void(__cdecl* lua_upvaluejoin)(lua_State* L, int funcindex1, int n1, int funcindex2, int n2) = nullptr;
		bool DataOnly;

		// a reference to something already serialized/deserialized
		static constexpr lua::LType ReferenceType = static_cast<lua::LType>(-2);
		// a reference to a already serilaized/deserialized upvalue
		static constexpr lua::LType UpvalueReferenceType = static_cast<lua::LType>(-3);

		void WritePrimitive(const void* d, size_t len);
		size_t ReadPrimitive();
		template<class T>
		void WritePrimitive(const T d) {
			WritePrimitive(&d, sizeof(T));
		}
		template<class T>
		T ReadPrimitive(const char* ex) {
			if (ReadPrimitive() != sizeof(T))
				throw std::format_error{ ex };
			return *reinterpret_cast<T*>(Data.get());
		}

		void SerializeType(lua::LType t);
		lua::LType DeserializeType();
		void SerializeBool(int idx);
		void DeserializeBool();
		void SerializeNumber(int idx);
		void DeserializeNumber();
		void SerializeString(int idx);
		void DeserializeString();
		void SerializeReference(int ref);
		int DeserializeReference();
		void DeserializeReferencedValue();
		void SerializeTable(int idx, bool isglobal = false);
		void DeserializeTable(bool create);
		void SerializeFunction(int idx);
		void DeserializeFunction();
		void SerializeUserdata(int idx);
		void DeserializeUserdata();
		void SerializeAnything(int idx);
		void DeserializeAnything();
		void DeserializeAnything(lua::LType t);
		bool CanSerialize(int idx);
		static bool IsGlobalSkipped(const char* n);

		void PrepareSerialize();
		void CleanupSerialize();
		void PrepareDeserialize();
		void CleanupDeserialize(bool ret);

	public:
		AdvLuaStateSerializer(BB::IStream& io, lua_State* l, bool dataOnly = false);

		void SerializeState();
		void DeserializeState();

		void SerializeVariable(int i);
		void DeserializeVariable();

		void SerializeStack(int n = -1);
		void DeserializeStack();

		// pushes a registry subtable that will get serialized in savegames. (creates one if it does not exist)
		static void PushSerializedRegistry(luaext::State L);

		static constexpr const char* UserdataSerializerMetaEvent = "__serialize";
		static std::map<std::string, lua::CFunction> UserdataDeserializer;
		static constexpr int FileVersion = 2;
		static constexpr const char* RegistrySerializeKeys = "CppLogic::Serializer::AdvLuaStateSerializer_SerlializedRegistry";
	};

	class StructAccess;
	class BBObjectAccess;
	class ListAccess;
	class FieldAccess;
	class ObjectAccess {
	public:
		virtual ~ObjectAccess() = default;

		enum class Type : int {
			Field,
			Struct,
			BBObject,
			List,
		};

		ObjectAccess(std::string_view name, void* obj, const BB::SerializationData* sd, int id, void(*onWrite)(int id));

		std::string_view SDName;
		const BB::SerializationData* SeriData;
		void* Object;
		int Id;
		void(*OnWrite)(int id);

		static int Name(luaext::State L);
		template<class T>
		static int AsSubClass(luaext::State L) {
			L.CheckUserClass<T>(1);
			L.SetTop(1);
			return 1;
		}

	protected:
		static void PushSD(luaext::State L, std::string_view n, void* obj, const BB::SerializationData* sd, int id = 0, void(*onWrite)(int id) = nullptr, bool listElement = false);
	public:
		static void PushObject(luaext::State L, std::string_view n, BB::IObject* obj, int id = 0, void(*onWrite)(int id) = nullptr, shok::ClassId c = shok::ClassId::Invalid, void** owner = nullptr);
		static void PushObject(luaext::State L, std::string_view n, void* obj, const BB::SerializationData* sd, int id = 0, void(*onWrite)(int id) = nullptr);

		static constexpr std::array LuaMethods{
			luaext::FuncReference::GetRef<Name>("Name"),
			luaext::FuncReference::GetRef<AsSubClass<FieldAccess>>("AsFieldAccess"),
			luaext::FuncReference::GetRef<AsSubClass<StructAccess>>("AsStructAccess"),
			luaext::FuncReference::GetRef<AsSubClass<BBObjectAccess>>("AsObjectAccess"),
			luaext::FuncReference::GetRef<AsSubClass<ListAccess>>("AsListAccess"),
		};
	};

	class FieldAccess : public ObjectAccess {
	public:
		static int GetType(luaext::State L);
		static int DataType(luaext::State L);
		static int Get(luaext::State L);
		static int Set(luaext::State L);

		using ObjectAccess::ObjectAccess;

		using InheritsFrom = std::tuple<ObjectAccess>;

		static constexpr std::array LuaMethods{
			luaext::FuncReference::GetRef<GetType>("GetType"),
			luaext::FuncReference::GetRef<DataType>("DataType"),
			luaext::FuncReference::GetRef<Get>("Get"),
			luaext::FuncReference::GetRef<Set>("Set"),
		};
	};

	class StructAccess : public ObjectAccess {
	public:
		static int GetType(luaext::State L);
		static int Fields(luaext::State L);

		static int FieldsNext(luaext::State L);

		static int Index(luaext::State L);

		using ObjectAccess::ObjectAccess;

		using InheritsFrom = std::tuple<ObjectAccess>;

		static constexpr std::array LuaMethods{
			luaext::FuncReference::GetRef<GetType>("GetType"),
			luaext::FuncReference::GetRef<Fields>("Fields"),
			luaext::FuncReference::GetRef<Index>("Get"),
		};

		struct Iter {
			void* Obj;
			const BB::SerializationData* SeriData;
			const BB::SerializationData* CurrentSubObject;
			const BB::SerializationData* Root;
			bool First = true;

			Iter(void* o, const BB::SerializationData* sd);

			bool operator==(std::default_sentinel_t) const;
			[[nodiscard]] Iter begin() const {
				return *this;
			}
			[[nodiscard]] static auto end() {
				return std::default_sentinel;
			}

			std::pair<void*, const BB::SerializationData*> operator*() const;
			Iter& operator++();
			Iter operator++(int) {
				Iter r = *this;
				++(*this);
				return r;
			}
		};
	};

	class BBObjectAccess : public StructAccess {
		void** Owner;
	public:
		static int GetType(luaext::State L);
		static int ObjectType(luaext::State L);
		static int New(luaext::State L);
		static int IsNullptr(luaext::State L);

		using InheritsFrom = std::tuple<StructAccess>;

		BBObjectAccess(std::string_view name, void* obj, const BB::SerializationData* sd, int id, void(*onWrite)(int id), void** owner = nullptr);

		static constexpr std::array LuaMethods{
			luaext::FuncReference::GetRef<GetType>("GetType"),
			luaext::FuncReference::GetRef<ObjectType>("ObjectType"),
			luaext::FuncReference::GetRef<New>("New"),
			luaext::FuncReference::GetRef<IsNullptr>("IsNullptr"),
		};
	};

	class ListAccess : public ObjectAccess {
	public:
		static int GetType(luaext::State L);
		static int Elements(luaext::State L);
		static int Size(luaext::State L);
		static int First(luaext::State L);
		static int Insert(luaext::State L);
		static int Remove(luaext::State L);
		static int ListType(luaext::State L);
		static int InsertAt(luaext::State L);

		static int ElementsNext(luaext::State L);

		static int Index(luaext::State L);

		using ObjectAccess::ObjectAccess;

		using InheritsFrom = std::tuple<ObjectAccess>;

		static constexpr std::array LuaMethods{
			luaext::FuncReference::GetRef<GetType>("GetType"),
			luaext::FuncReference::GetRef<Elements>("Elements"),
			luaext::FuncReference::GetRef<Size>("Size"),
			luaext::FuncReference::GetRef<First>("First"),
			luaext::FuncReference::GetRef<Insert>("Insert"),
			luaext::FuncReference::GetRef<Remove>("Remove"),
			luaext::FuncReference::GetRef<ListType>("ListType"),
			luaext::FuncReference::GetRef<InsertAt>("InsertAt"),
		};

	private:
		struct ElementIter {
			std::unique_ptr<BB::SerializationListOptions::Iter, void(__stdcall*)(BB::SerializationListOptions::Iter* i)> Iter;
		};
	};
}

namespace CppLogic {
	template<>
	inline auto GetIdManager<Serializer::ObjectAccess::Type>() {
		return CppLogic::MagicEnum::EnumIdManager<Serializer::ObjectAccess::Type>{};
	}
}

#pragma once
#include <map>
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_classfactory.h"
#include "Luapp/luapp50.h"

namespace CppLogic::Serializer {
	class ObjectToLuaSerializer {
		static void SerializeField(lua::State L, void* o, const BB::SerializationData* s, bool keypushed = false);
		static void SerializeFields(lua::State L, void* o, const BB::SerializationData* s);
		static void SerializePushField(lua::State L, void* o, const BB::SerializationData* s);
		static void SerializeList(lua::State L, void* o, const BB::SerializationData* s);

		static void DeserializeField(lua::State L, void* o, const BB::SerializationData* s, bool valuepushed = false);
		static void DeserializeCheckField(lua::State L, void* o, const BB::SerializationData* s);
		static void DeserializeFields(lua::State L, void* o, const BB::SerializationData* s);
		static void DeserializeList(lua::State L, void* o, const BB::SerializationData* s);
	public:
		static void Serialize(lua::State L, void* o, const BB::SerializationData* seri, unsigned int id = 0);
		static void Serialize(lua::State L, BB::IObject* o);

		// if o nullptr, gets created, if id 0, gets read from lua, in this case also checks seri for 0
		static void Deserialize(lua::State L, void* o, const BB::SerializationData* seri, unsigned int id = 0);
		static void Deserialize(lua::State L, BB::IObject* o);

		static void DumpClassSerializationData(lua::State L, const BB::SerializationData* seri);
		static void DumpClassSerializationData(lua::State L, unsigned int id);
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


		BB::CFileStreamEx& IO;
		lua::State L;
		void* Data = nullptr;
		size_t DataLength = 0;
		std::map<Reference, int> RefToNumber;
		int NextRefereceNumber = 1;
		int IndexOfReferenceHolder = 0;

		// a reference to something already serialized/deserialized
		static constexpr lua::LType ReferenceType = static_cast<lua::LType>(-2);

		void WritePrimitive(const void* d, size_t len);
		size_t ReadPrimitive();

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
		void SerializeTable(int idx);
		void DeserializeTable(bool create);
		void SerializeFunction(int idx);
		void DeserializeFunction();
		void SerializeUserdata(int idx);
		void DeserializeUserdata();
		void SerializeAnything(int idx);
		void DeserializeAnything();
		bool CanSerialize(int idx);
		bool IsGlobalSkipped(const char* n);

	public:
		AdvLuaStateSerializer(BB::CFileStreamEx& io, lua_State* l);
		~AdvLuaStateSerializer();

		void SerializeState();
		void DeserializeState();

		static constexpr const char* UserdataSerializerMetaEvent = "__serialize";
		static std::map<std::string, lua::CFunction> UserdataDeserializer;
	};
}

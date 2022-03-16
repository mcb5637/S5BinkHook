#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_classfactory.h"
#include "Luapp/luapp50.h"

namespace CppLogic::Serializer {
	class LuaSerializer {
		static void SerializeField(lua::State L, void* o, const BB::SerializationData* s, bool keypushed = false);
		static void SerializeFields(lua::State L, void* o, const BB::SerializationData* s);
		static void SerializePushField(lua::State L, void* o, const BB::SerializationData* s);
		static void SerializeList(lua::State L, void* o, const BB::SerializationData* s);
	public:
		static void Serialize(lua::State L, void* o, const BB::SerializationData* seri, unsigned int id = 0);
		static void Serialize(lua::State L, BB::IObject* o);

		static void DumpClassSerializationData(lua::State L, const BB::SerializationData* seri);
		static void DumpClassSerializationData(lua::State L, unsigned int id);
	};
}

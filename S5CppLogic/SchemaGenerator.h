#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_classfactory.h"
#include "luaext.h"

namespace CppLogic::Serializer {
	class SchemaGenerator {
	public:
		static void PushUnknownFieldSerializers(lua::State L, const BB::SerializationData* d);
		static void PushUnknownFieldSerializers(lua::State L);
	};
}

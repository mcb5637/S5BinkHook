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

		static size_t PredictNumberOfFields(const BB::SerializationData* data);
		static std::string EscapeClassname(std::string_view name);
		static shok::ClassId TryFindBaseClass(const BB::SerializationData* data);
		static void WriteSeriData(BB::IStream& f, size_t indent, const BB::SerializationData* data, bool skipas = false);
		static void WriteClassSchema(BB::IStream& f, shok::ClassId id, bool alwaysInheritBBObject = true);
		static void WriteRegisteredClassesSchema(BB::IStream& f);

	private:
		static void WriteNewClassSchema(BB::IStream& f, std::string_view name, const BB::SerializationData* seridata);
		static void WriteSubclassSchema(BB::IStream& f, std::string_view name, std::string_view basename, const BB::SerializationData* seridata);
	};
}

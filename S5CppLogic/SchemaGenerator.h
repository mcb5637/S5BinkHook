#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_classfactory.h"
#include "luaext.h"

namespace CppLogic::Serializer {
	class SchemaGenerator {
		static void PushUnknownFieldSerializers(lua::State L, const BB::SerializationData* d);
	public:
		static void PushUnknownFieldSerializers(lua::State L);

		static void WriteAllClassesSchema(BB::IStream& f);

	private:
		static size_t PredictNumberOfFields(const BB::SerializationData* data);
		static std::string EscapeClassname(std::string_view name);
		static std::string EscapeString(std::string_view name);
		static shok::ClassId TryFindBaseClass(const BB::SerializationData* data);
		static void WriteSeriData(BB::IStream& f, size_t indent, const BB::SerializationData* data, bool skipas = false);
		static void WriteClassSchema(BB::IStream& f, shok::ClassId id, bool alwaysInheritBBObject = true);
		static void WriteRegisteredClassesSchema(BB::IStream& f);
		static void WriteChosenClassesSchema(BB::IStream& f);
		static void WriteNewClassSchema(BB::IStream& f, std::string_view name, const BB::SerializationData* seridata, bool skipClassname = false);
		static void WriteSubclassSchema(BB::IStream& f, std::string_view name, std::string_view basename, const BB::SerializationData* seridata, bool skipClassname = false);
		static void WriteClassnameClassSchema(BB::IStream& f, std::string_view name);
		template<class C, class Parent = void, bool hasParentSeridata = true>
		static void WriteChosenClassSchema(BB::IStream& f, bool skipClassname = true);
	};
}

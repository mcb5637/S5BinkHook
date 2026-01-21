#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/s5_defines.h>
#include <luaext.h>

namespace CppLogic::Serializer {
	class SchemaGenerator {
		static void PushUnknownFieldSerializers(luaext::State L, const BB::SerializationData* d);
		static void PushUnknownFieldSerializersSpecific(std::string_view name, const BB::SerializationData* cl, std::string_view parent_name, bool hasParentSeridata, bool skipClassname, luaext::State L);
		static void PushUnknownListOptions(luaext::State L, const BB::SerializationData* d);
		static void PushUnknownListOptionsSpecific(std::string_view name, const BB::SerializationData* cl, std::string_view parent_name, bool hasParentSeridata, bool skipClassname, luaext::State L);

	public:
		static void PushUnknownFieldSerializers(luaext::State L);
		static void PushUnknownListOptions(luaext::State L);

		static void WriteAllClassesSchema(BB::IStream& f);

		static std::string EscapeClassname(std::string_view name);
	private:
		static size_t PredictNumberOfFields(const BB::SerializationData* data);
		static std::string EscapeString(std::string_view name);
		static std::string_view TryFindClassOfSeriData(const BB::SerializationData* data, bool alwaysInheritBBObject);
		static void WriteSeriData(BB::IStream& f, size_t indent, const BB::SerializationData* data, bool skipas = false);
		static void WriteClassSchema(BB::IStream& f, shok::ClassId id, bool alwaysInheritBBObject = true);
		static void WriteRegisteredClassesSchema(BB::IStream& f);
		static void WriteChosenClassesSchema(BB::IStream& f);
		static void WriteNewClassSchema(BB::IStream& f, std::string_view name, const BB::SerializationData* seridata, bool skipClassname, shok::ClassId id = shok::ClassId::Invalid);
		static void WriteSubclassSchema(BB::IStream& f, std::string_view name, std::string_view basename, const BB::SerializationData* seridata, bool skipClassname, shok::ClassId id = shok::ClassId::Invalid);
		static void WriteClassnameClassSchema(BB::IStream& f, std::string_view name, shok::ClassId id);
		static void WriteChosenClassSchema(std::string_view name, const BB::SerializationData* cl, std::string_view parent_name, bool hasParentSeridata, bool skipClassname, BB::IStream& f);
		static void PreRegisterExtraClasses();
		template<auto F, class... Arg>
		static void ForAllChoosesClasses(Arg&... arg);
		template<auto F, class C, class... Arg>
		static void ForSimpleChoosenClass(Arg&... arg);
	};
}

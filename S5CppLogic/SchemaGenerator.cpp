#include "pch.h"
#include "SchemaGenerator.h"
#include "s5_filesystem.h"
#include "s5_effecttype.h"
#include "s5_tech.h"
#include "s5_mapdisplay.h"
#include "modloader.h"

template<class T>
static bool CheckFieldSerializerType(const BB::FieldSerializer* f) {
	const BB::FieldSerializer* c = BB::FieldSerializer::GetSerializer<T>();
	return c->DeserializeFromString == f->DeserializeFromString && c->SerializeToString == f->SerializeToString;
}
static std::string CheckFieldSerializerTypes(const BB::FieldSerializer* f) {
	if (CheckFieldSerializerType<int>(f))
		return std::format("{{{}, &InfoInt}},", reinterpret_cast<const void*>(f));
	if (CheckFieldSerializerType<unsigned int>(f))
		return std::format("{{{}, &InfoUInt}},", reinterpret_cast<const void*>(f));
	if (CheckFieldSerializerType<float>(f))
		return std::format("{{{}, &InfoFloat}},", reinterpret_cast<const void*>(f));
	if (CheckFieldSerializerType<bool>(f))
		return std::format("{{{}, &InfoBool}},", reinterpret_cast<const void*>(f));
	return "";
}

void CppLogic::Serializer::SchemaGenerator::PushUnknownFieldSerializers(lua::State L, const BB::SerializationData* d)
{
	if (d == nullptr)
		return;
	while (d->Type != BB::SerializationData::Ty::Invalid) {
		if (d->Type == BB::SerializationData::Ty::Direct) {
			auto* fieldseri = d->DataConverter;
			if (fieldseri) {
				auto* ext = fieldseri->GetOptExtendedInfo();
				if (ext == nullptr) {
					L.Push(std::format("{}", reinterpret_cast<const void*>(fieldseri)));
					auto s = CheckFieldSerializerTypes(fieldseri);
					if (s.empty())
						L.PushValue(-1);
					else
						L.Push(s);
					L.SetTableRaw(-3);
				}
			}
		}
		else if (d->SerializationName != nullptr || d->Type == BB::SerializationData::Ty::Embedded) {
			PushUnknownFieldSerializers(L, d->SubElementData);
		}
		++d;
	}
}

void CppLogic::Serializer::SchemaGenerator::PushUnknownFieldSerializers(lua::State L)
{
	L.NewTable();
	auto* f = *BB::CClassFactory::GlobalObj;
	for (const auto& [id, info] : f->Info) {
		if (info.SData == nullptr)
			continue;
		PushUnknownFieldSerializers(L, info.SData);
	}
	PushUnknownFieldSerializers(L, shok::Technology::SerializationData);
	PushUnknownFieldSerializers(L, ED::CModelsProps::SerializationData);
	PushUnknownFieldSerializers(L, GGL::ExperienceClass::SerializationData);
	PushUnknownFieldSerializers(L, ED::CDisplayProps::SerializationData);
	PushUnknownFieldSerializers(L, GGlue::CTerrainPropsMgr::SerializationData);
	PushUnknownFieldSerializers(L, GGlue::CGlueWaterPropsMgr::SerializationData);
}

size_t CppLogic::Serializer::SchemaGenerator::PredictNumberOfFields(const BB::SerializationData* data)
{
	size_t r = 0;
	while (data->Type != BB::SerializationData::Ty::Invalid) {
		if (data->Type == BB::SerializationData::Ty::Direct) {
			++r;
		}
		else if (data->Type == BB::SerializationData::Ty::Embedded) {
			if (data->SerializationName == nullptr) {
				r += PredictNumberOfFields(data->SubElementData);
			}
			else {
				++r;
			}
		}
		else if (data->Type == BB::SerializationData::Ty::ObjectPointer || data->Type == BB::SerializationData::Ty::ObjectEmbedded) {
			++r;
		}

		++data;
	}
	return r;
}

std::string CppLogic::Serializer::SchemaGenerator::EscapeClassname(std::string_view name)
{
	auto rm_pref = [&name](std::string_view p) {
		if (name.starts_with(p))
			name.remove_prefix(p.length());
	};

	rm_pref("class ");
	rm_pref("struct ");

	std::string n{ name };
	std::replace(n.begin(), n.end(), '<', '_');
	std::replace(n.begin(), n.end(), '>', '_');
	std::replace(n.begin(), n.end(), ':', '_');
	std::replace(n.begin(), n.end(), ',', '_');
	return n;
}

std::string CppLogic::Serializer::SchemaGenerator::EscapeString(std::string_view name)
{
	std::string n{ name };
	auto repl_str = [&n](std::string_view search, std::string_view replace) {
		size_t pos = 0;
		while ((pos = n.find(search, pos)) != std::string::npos) {
			n.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	};
	repl_str("<", "&lt;");
	repl_str(">", "&gt;");
	repl_str("&", "&amp;");
	repl_str("\"", "&quot;");
	repl_str("`", "&apos;");
	return n;
}

std::map<const BB::SerializationData*, std::string_view> ExtraClasses{};

std::string_view CppLogic::Serializer::SchemaGenerator::TryFindClassOfSeriData(const BB::SerializationData* data, bool alwaysInheritBBObject)
{
	auto* cf = *BB::CClassFactory::GlobalObj;
	for (const auto & [id, info] : cf->Info) {
		if (info.SData == data)
			return cf->GetClassDemangledName(id);
	}
	auto e = ExtraClasses.find(data);
	if (e != ExtraClasses.end())
		return e->second;
	if (alwaysInheritBBObject)
		return "BB::IObject";
	return "";
}

void CppLogic::Serializer::SchemaGenerator::WriteSeriData(BB::IStream& f, size_t indent, const BB::SerializationData* data, bool skipas)
{
	std::string_view as = "xs:sequence";// (PredictNumberOfFields(data) > 1) ? "xs:all" : "xs:sequence";

	if (!skipas) {
		f.Indent(indent - 1);
		f.Write("<");
		f.Write(as);
		f.Write(">\n");
	}

	while (data->Type != BB::SerializationData::Ty::Invalid) {
		if (data->Type == BB::SerializationData::Ty::Direct) {
			f.Indent(indent);
			f.Write("<xs:element name=\"");
			f.Write(data->SerializationName);
			f.Write("\"");

			auto* ext = data->DataConverter->GetOptExtendedInfo();
			if (ext != nullptr && !ext->XSDType.empty()) {
				f.Write(" type=\"");
				f.Write(ext->XSDType);
				f.Write("\"");
			}
			if (data->ListOptions != nullptr) {
				f.Write(" minOccurs=\"0\" maxOccurs=\"unbounded\"");
			}
			f.Write("/>\n");
		}
		else if (data->Type == BB::SerializationData::Ty::Embedded) {
			if (data->SerializationName == nullptr) {
				WriteSeriData(f, indent, data->SubElementData, true);
			}
			else {
				f.Indent(indent);
				f.Write("<xs:element name=\"");
				f.Write(data->SerializationName);
				f.Write("\"");

				if (data->ListOptions != nullptr) {
					f.Write(" minOccurs=\"0\" maxOccurs=\"unbounded\"");
				}

				auto ty = TryFindClassOfSeriData(data->SubElementData, false);
				if (!ty.empty()) {
					f.Write(" type=\"");
					f.Write(EscapeClassname(ty));
					f.Write("\"/>\n");
				}
				else {
					f.Write(">\n");

					f.Indent(indent + 1);
					f.Write("<xs:complexType>\n");

					WriteSeriData(f, indent + 3, data->SubElementData);

					f.Indent(indent + 1);
					f.Write("</xs:complexType>\n");

					f.Indent(indent);
					f.Write("</xs:element>\n");
				}
			}
		}
		else if (data->Type == BB::SerializationData::Ty::ObjectPointer || data->Type == BB::SerializationData::Ty::ObjectEmbedded) {
			f.Indent(indent);
			f.Write("<xs:element name=\"");
			f.Write(data->SerializationName);
			f.Write("\" type=\"BB__IObject_NoClassname\"");

			if (data->ListOptions != nullptr) {
				f.Write(" minOccurs=\"0\" maxOccurs=\"unbounded\"");
			}
			f.Write("/>\n");
		}

		++data;
	}

	if (!skipas) {
		f.Indent(indent - 1);
		f.Write("</");
		f.Write(as);
		f.Write(">\n");
	}
}

void CppLogic::Serializer::SchemaGenerator::WriteSubclassSchema(BB::IStream& f, std::string_view name, std::string_view basename, const BB::SerializationData* seridata, bool skipClassname, shok::ClassId id)
{
	f.Write("\t<xs:complexType name=\"");
	f.Write(EscapeClassname(name));
	if (!skipClassname)
		f.Write("_NoClassname");
	f.Write("\">\n\t\t\t<xs:complexContent>\n\t\t\t\t<xs:extension base=\"");
	f.Write(EscapeClassname(basename));
	f.Write("_NoClassname\">\n");

	WriteSeriData(f, 6, seridata);

	f.Write("\t\t\t\t</xs:extension>\n\t\t\t</xs:complexContent>\n\t</xs:complexType>\n");

	if (!skipClassname)
		WriteClassnameClassSchema(f, name, id);
}

void CppLogic::Serializer::SchemaGenerator::WriteNewClassSchema(BB::IStream& f, std::string_view name, const BB::SerializationData* seridata, bool skipClassname, shok::ClassId id)
{
	f.Write("\t<xs:complexType name=\"");
	f.Write(EscapeClassname(name));
	if (!skipClassname)
		f.Write("_NoClassname");
	f.Write("\">\n");

	WriteSeriData(f, 4, seridata);

	f.Write("\t</xs:complexType>\n");

	if (!skipClassname)
		WriteClassnameClassSchema(f, name, id);
}

void CppLogic::Serializer::SchemaGenerator::WriteClassnameClassSchema(BB::IStream& f, std::string_view name, shok::ClassId id)
{
	f.Write("\t<xs:complexType name=\"");
	f.Write(EscapeClassname(name));
	f.Write("\">\n\t\t\t<xs:complexContent>\n\t\t\t\t<xs:extension base=\"");
	f.Write(EscapeClassname(name));
	f.Write("_NoClassname\">\n");

	f.Write("\t\t\t\t\t<xs:attribute name=\"classname\" fixed=\"");
	f.Write(EscapeString(name));
	f.Write("\" use=\"required\"/>\n");

	if (id != shok::ClassId::Invalid) {
		f.Write("\t\t\t\t\t<xs:attribute name=\"classid\" fixed=\"");
		f.Write(std::format("{:#x}", static_cast<unsigned int>(id)));
		f.Write("\" use=\"optional\"/>\n");
	}

	f.Write("\t\t\t\t</xs:extension>\n\t\t\t</xs:complexContent>\n\t</xs:complexType>\n");
}

void CppLogic::Serializer::SchemaGenerator::WriteClassSchema(BB::IStream& f, shok::ClassId id, bool alwaysInheritBBObject)
{
	auto* cf = *BB::CClassFactory::GlobalObj;

	std::string_view name = cf->GetClassDemangledName(id);
	auto* seridata = cf->GetSerializationDataForClass(id);

	if (seridata->Type != BB::SerializationData::Ty::Invalid && seridata->SerializationName == nullptr) {
		auto basename = TryFindClassOfSeriData(seridata->SubElementData, alwaysInheritBBObject);

		if (basename.empty()) {
			WriteNewClassSchema(f, name, seridata, false, id);
		}
		else {
			WriteSubclassSchema(f, name, basename, seridata + 1, false, id);
		}
	}
	else {
		if (alwaysInheritBBObject)
			WriteSubclassSchema(f, name, "BB::IObject", seridata, false, id);
		else
			WriteNewClassSchema(f, name, seridata, false, id);
	}
}

template<class C, class Parent, bool hasParentSeridata>
inline void CppLogic::Serializer::SchemaGenerator::WriteChosenClassSchema(BB::IStream& f, bool skipClassname)
{
	if constexpr (std::same_as<Parent, void>) {
		WriteNewClassSchema(f, typename_details::type_name<C>(), C::SerializationData, skipClassname);
	}
	else {
		const BB::SerializationData* sd = C::SerializationData;
		if constexpr (hasParentSeridata)
			++sd;
		WriteSubclassSchema(f, typename_details::type_name<C>(), typename_details::type_name<Parent>(), sd, skipClassname);
	}
}

void CppLogic::Serializer::SchemaGenerator::WriteRegisteredClassesSchema(BB::IStream& f)
{
	auto* cf = *BB::CClassFactory::GlobalObj;
	for (const auto& [id, info] : cf->Info) {
		if (info.SData == nullptr)
			continue;
		WriteClassSchema(f, id);
	}
}

void CppLogic::Serializer::SchemaGenerator::WriteChosenClassesSchema(BB::IStream& f)
{
	WriteChosenClassSchema<shok::Position>(f);
	WriteChosenClassSchema<shok::CostInfo>(f);
	WriteChosenClassSchema<EGL::EffectsProps::EffectData>(f);
	WriteChosenClassSchema<shok::Technology>(f);
	WriteChosenClassSchema<ED::CModelsProps::ModelData>(f);
	WriteChosenClassSchema<GGL::ExperienceClass>(f);
	WriteChosenClassSchema<ED::CDisplayProps>(f);
	WriteChosenClassSchema<GGlue::TerrainTypeData>(f);
	WriteChosenClassSchema<GGlue::WaterTypeData>(f);
	WriteChosenClassSchema<GGlue::CTerrainPropsMgr>(f);
	WriteChosenClassSchema<GGlue::CGlueWaterPropsMgr>(f);
	WriteChosenClassSchema<CppLogic::ModLoader::ModpackDesc>(f);
}

void CppLogic::Serializer::SchemaGenerator::PreRegisterExtraClasses()
{
	if (!ExtraClasses.empty())
		return;
	ExtraClasses[GGlue::TerrainTypeData::SerializationData] = typename_details::type_name<GGlue::TerrainTypeData>();
	ExtraClasses[GGlue::WaterTypeData::SerializationData] = typename_details::type_name<GGlue::WaterTypeData>();
	ExtraClasses[shok::Position::SerializationData] = typename_details::type_name<shok::Position>();
	ExtraClasses[shok::CostInfo::SerializationData] = typename_details::type_name<shok::CostInfo>();
}

void CppLogic::Serializer::SchemaGenerator::WriteAllClassesSchema(BB::IStream& f)
{
	PreRegisterExtraClasses();

	f.Write("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">\n"); //  xmlns:vc=\"http://www.w3.org/2007/XMLSchema-versioning\" vc:minVersion=\"1.1\"

	f.Write("\t<xs:complexType name=\"BB__IObject_NoClassname\" abstract=\"true\">\n\t\t<xs:sequence>\n\t\t</xs:sequence>\n\t</xs:complexType>\n");

	WriteRegisteredClassesSchema(f);
	WriteChosenClassesSchema(f);

	f.Write("</xs:schema>\n");
}

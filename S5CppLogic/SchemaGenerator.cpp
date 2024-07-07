#include "pch.h"
#include "SchemaGenerator.h"
#include "s5_filesystem.h"

template<class T>
static bool CheckFieldSerializerType(const BB::FieldSerilaizer* f) {
	const BB::FieldSerilaizer* c = BB::FieldSerilaizer::GetSerilalizer<T>();
	return c->DeserializeFromString == f->DeserializeFromString && c->SerializeToString == f->SerializeToString;
}
static std::string CheckFieldSerializerTypes(const BB::FieldSerilaizer* f) {
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
	std::string n{ name };
	std::replace(n.begin(), n.end(), '<', '_');
	std::replace(n.begin(), n.end(), '>', '_');
	std::replace(n.begin(), n.end(), ':', '_');
	std::replace(n.begin(), n.end(), ',', '_');
	return n;
}

shok::ClassId CppLogic::Serializer::SchemaGenerator::TryFindBaseClass(const BB::SerializationData* data)
{
	auto* cf = *BB::CClassFactory::GlobalObj;
	for (const auto & [id, info] : cf->Info) {
		if (info.SData == data)
			return id;
	}
	return shok::ClassId::Invalid;
}

void CppLogic::Serializer::SchemaGenerator::WriteSeriData(BB::IStream& f, size_t indent, const BB::SerializationData* data, bool skipas)
{
	std::string_view as = "xs:sequence"; //(PredictNumberOfFields(data) > 1) ? "xs:all" : "xs:sequence";

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
		else if (data->Type == BB::SerializationData::Ty::ObjectPointer || data->Type == BB::SerializationData::Ty::ObjectEmbedded) {
			f.Indent(indent);
			f.Write("<xs:element name=\"");
			f.Write(data->SerializationName);
			f.Write("\" type=\"BB__IObject\"");

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

void CppLogic::Serializer::SchemaGenerator::WriteSubclassSchema(BB::IStream& f, std::string_view name, std::string_view basename, const BB::SerializationData* seridata)
{
	f.Write("\t<xs:complexType name=\"");
	f.Write(name);
	f.Write("\">\n\t\t\t<xs:complexContent>\n\t\t\t\t<xs:extension base=\"");
	f.Write(EscapeClassname(basename));
	f.Write("\">\n");

	WriteSeriData(f, 6, seridata);

	f.Write("\t\t\t\t</xs:extension>\n\t\t\t</xs:complexContent>\n\t</xs:complexType>\n");
}

void CppLogic::Serializer::SchemaGenerator::WriteNewClassSchema(BB::IStream& f, std::string_view name, const BB::SerializationData* seridata)
{
	f.Write("\t<xs:complexType name=\"");
	f.Write(name);
	f.Write("\">\n");

	WriteSeriData(f, 4, seridata);

	f.Write("\t</xs:complexType>\n");
}

void CppLogic::Serializer::SchemaGenerator::WriteClassSchema(BB::IStream& f, shok::ClassId id, bool alwaysInheritBBObject)
{
	auto* cf = *BB::CClassFactory::GlobalObj;

	std::string name = EscapeClassname(cf->GetClassDemangledName(id));
	auto* seridata = cf->GetSerializationDataForClass(id);

	if (seridata->Type != BB::SerializationData::Ty::Invalid && seridata->SerializationName == nullptr) {
		auto base = TryFindBaseClass(seridata->SubElementData);

		std::string_view basename = "";
		if (base == shok::ClassId::Invalid) {
			if (alwaysInheritBBObject)
				basename = "BB::IObject";
		}
		else {
			basename = cf->GetClassDemangledName(base);
		}

		if (basename.empty()) {
			WriteNewClassSchema(f, name, seridata);
		}
		else {
			WriteSubclassSchema(f, name, basename, seridata + 1);
		}
	}
	else {
		if (alwaysInheritBBObject)
			WriteSubclassSchema(f, name, "BB__IObject", seridata);
		else
			WriteNewClassSchema(f, name, seridata);
	}
}

void CppLogic::Serializer::SchemaGenerator::WriteRegisteredClassesSchema(BB::IStream& f)
{
	f.Write("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">\n");

	f.Write("\t<xs:complexType name=\"BB__IObject\" abstract=\"true\">\n\t\t<xs:sequence>\n\t\t\</xs:sequence>\n\t\t<xs:attribute name=\"classname\" type=\"xs:string\" use=\"required\"/>\n\t</xs:complexType>\n");

	auto* cf = *BB::CClassFactory::GlobalObj;
	for (const auto& [id, info] : cf->Info) {
		if (info.SData == nullptr)
			continue;
		WriteClassSchema(f, id);
	}

	f.Write("</xs:schema>\n");
}

#include "pch.h"
#include "SchemaGenerator.h"

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
	while (d->Type != 0) {
		if (d->Type == 2) {
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
		else if (d->SerializationName != nullptr || d->Type == 3) {
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

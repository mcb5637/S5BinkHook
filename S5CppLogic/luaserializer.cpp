#include "pch.h"
#include "luaserializer.h"

void CppLogic::Serializer::LuaSerializer::SerializeField(lua::State L, void* o, const BB::SerializationData* s, bool keypushed)
{
	switch (s->Type) {
	case 2:
		if (!keypushed)
			L.Push(s->SerializationName);
		SerializePushField(L, o, s);
		L.SetTableRaw(-3);
		break;
	case 3:
		if (!keypushed)
		L.Push(s->SerializationName);
		L.NewTable();

		SerializeFields(L, o, s->SubElementData);

		L.SetTableRaw(-3);
		break;
	case 6:
		o = *static_cast<void**>(o);
	case 5:
		{
			if (!keypushed)
				L.Push(s->SerializationName);
			const BB::SerializationData* os = nullptr;
			unsigned int id = 0;
			if (s->GetIdentifier) {
				id = s->GetIdentifier(o);
				os = (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
			}
			else {
				os = s->SubElementData;
			}

			if (os)
				Serialize(L, o, os, id);
			else
				L.Push("unknown object");

			L.SetTableRaw(-3);
		}
		break;
	default:
		if (keypushed)
			L.Push("unknown serialization type");
		break;
	}
}

void CppLogic::Serializer::LuaSerializer::SerializeFields(lua::State L, void* o, const BB::SerializationData* s)
{
	while (s->Size) {
		if (s->SerializationName) {
			void* data = reinterpret_cast<void*>(reinterpret_cast<int>(o) + static_cast<int>(s->Position));
			if (s->ListOptions)
				SerializeList(L, data, s);
			else
				SerializeField(L, data, s);
		}
		else if (s->SubElementData) {
			SerializeFields(L, o, s->SubElementData);
		}
		s++;
	}
}

void CppLogic::Serializer::LuaSerializer::SerializePushField(lua::State L, void* data, const BB::SerializationData* s)
{
	s->DataConverter->GetExtendedInfo().Push(L, data, s->DataConverter);
}

void CppLogic::Serializer::LuaSerializer::SerializeList(lua::State L, void* o, const BB::SerializationData* s)
{
	L.Push(s->SerializationName);
	L.NewTable();

	auto* it = s->ListOptions->AllocIter(o);

	int i = 1;
	while (s->ListOptions->IterNext(it)) {
		L.Push(i);
		SerializeField(L, s->ListOptions->IterCurrent(it), s, true);
		i++;
	}

	s->ListOptions->FreeIter(it);

	L.SetTableRaw(-3);
}

void CppLogic::Serializer::LuaSerializer::Serialize(lua::State L, void* o, const BB::SerializationData* seri, unsigned int id)
{
	L.NewTable();
	if (id) {
		L.Push("ObjectClassIdentification");
		L.Push(static_cast<double>(id));
		L.SetTableRaw(-3);
		L.Push("ObjectClassName");
		L.Push((*BB::CClassFactory::GlobalObj)->GetClassDemangledName(id));
		L.SetTableRaw(-3);
	}
	SerializeFields(L, o, seri);
}
void CppLogic::Serializer::LuaSerializer::Serialize(lua::State L, BB::IObject* o)
{
	unsigned int id = o->GetClassIdentifier();
	Serialize(L, o, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id), id);
}

void CppLogic::Serializer::LuaSerializer::DumpClassSerializationData(lua::State L, const BB::SerializationData* d)
{
	if (!d) {
		L.Push("nullptr");
		return;
	}
	L.NewTable();
	while (d->Size) {
		L.Push(static_cast<int>(d->Position));
		L.NewTable();

		if (d->SerializationName) {
			L.Push("name");
			L.Push(d->SerializationName);
			L.SetTableRaw(-3);
		}
		L.Push("size");
		L.Push(static_cast<int>(d->Size));
		L.SetTableRaw(-3);

		if (d->Type == 2) {
			L.Push("DataType");
			L.Push("Field");
			L.SetTableRaw(-3);
			L.Push("FieldType");
			L.Push(d->DataConverter->GetTypeDescName());
			L.SetTableRaw(-3);
		}
		else if (d->Type == 3) {
			L.Push("DataType");
			L.Push("Embedded Object/Base Class");
			L.SetTableRaw(-3);
			L.Push("SubElement");
			DumpClassSerializationData(L, d->SubElementData);
			L.SetTableRaw(-3);
		}
		else if (d->Type == 5) {
			L.Push("DataType");
			L.Push("Embedded BB::IObject");
			L.SetTableRaw(-3);
		}
		else if (d->Type == 6) {
			L.Push("DataType");
			L.Push("BB::IObject *");
			L.SetTableRaw(-3);
		}
		else {
			L.Push("DataType");
			L.PushFString("unknown %d", d->Type);
			L.SetTableRaw(-3);
		}

		if (d->ListOptions) {
			L.Push("ListOpions");
			L.Push(reinterpret_cast<int>(d->ListOptions));
			L.SetTableRaw(-3);
		}

		L.SetTableRaw(-3);
		d++;
	}
}
void CppLogic::Serializer::LuaSerializer::DumpClassSerializationData(lua::State L, unsigned int id)
{
	DumpClassSerializationData(L, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id));
}

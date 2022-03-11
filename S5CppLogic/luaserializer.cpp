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
	char buff[201]{};
	s->DataConverter->SerializeToString(buff, 200, data);
	L.Push(buff);
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

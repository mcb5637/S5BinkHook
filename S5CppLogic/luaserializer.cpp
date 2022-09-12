#include "pch.h"
#include "luaserializer.h"
#include <format>
#include "s5_filesystem.h"
#include "s5_scriptsystem.h"

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

void CppLogic::Serializer::LuaSerializer::DeserializeField(lua::State L, void* o, const BB::SerializationData* s, bool valuepushed)
{
	switch (s->Type) {
	case 2:
		if (!valuepushed) {
			L.Push(s->SerializationName);
			L.GetTableRaw(-2);
		}

		if (!L.IsNil(-1))
			DeserializeCheckField(L, o, s);

		if (!valuepushed)
			L.Pop(1);
		break;
	case 3:
		if (!valuepushed) {
			L.Push(s->SerializationName);
			L.GetTableRaw(-2);
		}

		if (!L.IsNil(-1))
			DeserializeFields(L, o, s->SubElementData);

		if (!valuepushed)
			L.Pop(1);
		break;
	case 6:
		o = *static_cast<void**>(o);
	case 5:
	{
		if (!valuepushed) {
			L.Push(s->SerializationName);
			L.GetTableRaw(-2);
		}

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
			Deserialize(L, o, os, id);

		if (!valuepushed)
			L.Pop(1);
		break;
	}
	default:
		break;
	}
}

void CppLogic::Serializer::LuaSerializer::DeserializeCheckField(lua::State L, void* o, const BB::SerializationData* s)
{
	s->DataConverter->GetExtendedInfo().Check(L, o, -1, s->DataConverter);
}

void CppLogic::Serializer::LuaSerializer::DeserializeFields(lua::State L, void* o, const BB::SerializationData* s)
{
	while (s->Size) {
		if (s->SerializationName) {
			void* data = reinterpret_cast<void*>(reinterpret_cast<int>(o) + static_cast<int>(s->Position));
			if (s->ListOptions)
				DeserializeList(L, data, s);
			else
				DeserializeField(L, data, s);
		}
		else if (s->SubElementData) {
			DeserializeFields(L, o, s->SubElementData);
		}
		s++;
	}
}

void CppLogic::Serializer::LuaSerializer::DeserializeList(lua::State L, void* o, const BB::SerializationData* s)
{
	L.Push(s->SerializationName);
	L.GetTableRaw(-2);

	if (L.IsTable(-1)) {
		for (int k : L.IPairs(-1)) {
			void* d = s->ListOptions->AddToList(o);
			DeserializeField(L, d, s, true);
		}
	}

	L.Pop(1);
}

void CppLogic::Serializer::LuaSerializer::Deserialize(lua::State L, void* o, const BB::SerializationData* seri, unsigned int id)
{
	if (o == nullptr) {
		if (id == 0) {
			L.Push("ObjectClassIdentification");
			L.GetTableRaw(-2);
			if (!L.IsNil(-1))
				id = static_cast<unsigned int>(L.CheckNumber(-1));
			L.Pop(1);
			if (id == 0) {
				L.Push("ObjectClassName");
				L.GetTableRaw(-2);
				if (!L.IsNil(-1))
					id = (*BB::CClassFactory::GlobalObj)->GetIdentifierByName(L.CheckString(-1));
				L.Pop(1);
			}
		}
		if (id == 0)
			throw std::invalid_argument{ "no object and no id provided" };
		o = (*BB::CClassFactory::GlobalObj)->CreateObject(id);
		if (o == nullptr)
			throw std::invalid_argument{ "invalid id provided" };
		if (seri == nullptr) {
			seri = (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
		}
	}
	DeserializeFields(L, o, seri);
}
void CppLogic::Serializer::LuaSerializer::Deserialize(lua::State L, BB::IObject* o)
{
	unsigned int id = o->GetClassIdentifier();
	Deserialize(L, o, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id), id);
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

CppLogic::Serializer::AdvLuaStateSerializer::AdvLuaStateSerializer(BB::CFileStreamEx& io, lua_State* l)
	: IO{ io }, L{ l }
{
}
CppLogic::Serializer::AdvLuaStateSerializer::~AdvLuaStateSerializer()
{
	if (Data)
		delete[] Data;
}

void CppLogic::Serializer::AdvLuaStateSerializer::WritePrimitive(const void* d, size_t len)
{
	IO.Write(&len, sizeof(size_t));
	IO.Write(d, len);
}

size_t CppLogic::Serializer::AdvLuaStateSerializer::ReadPrimitive()
{
	size_t len = 0;
	IO.Read(&len, sizeof(size_t));
	if (len > DataLength) {
		delete[] Data;
		Data = new byte[len];
	}
	IO.Read(Data, len);
	return len;
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeType(lua::LType t)
{
	WritePrimitive(&t, sizeof(lua::LType));
}
lua::LType CppLogic::Serializer::AdvLuaStateSerializer::DeserializeType()
{
	if (ReadPrimitive() != sizeof(lua::LType))
		throw std::format_error{ "error reading lua type, size mismatch" };
	lua::LType r = *static_cast<lua::LType*>(Data);
	if ((static_cast<int>(r) < static_cast<int>(lua::LType::Nil) || static_cast<int>(r) > static_cast<int>(lua::LType::Thread)) && r != ReferenceType)
		throw std::format_error{ "error reading lua type, not a valid type" };
	return r;
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeBool(int idx)
{
	SerializeType(lua::LType::Boolean);
	int b = L.ToBoolean(idx);
	WritePrimitive(&b, sizeof(int));
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeBool()
{
	if (ReadPrimitive() != sizeof(int))
		throw std::format_error{ "error reading bool, size mismatch" };
	L.Push(static_cast<bool>(*static_cast<int*>(Data)));
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeNumber(int idx)
{
	SerializeType(lua::LType::Number);
	double n = L.ToNumber(idx);
	WritePrimitive(&n, sizeof(double));
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeNumber()
{
	if (ReadPrimitive() != sizeof(double))
		throw std::format_error{ "error reading number, size mismatch" };
	L.Push(*static_cast<double*>(Data));
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeString(int idx)
{
	SerializeType(lua::LType::String);
	size_t len = 0;
	const char* s = L.ToString(idx, &len);
	WritePrimitive(s, len);
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeString()
{
	size_t len = ReadPrimitive();
	L.Push(static_cast<const char*>(Data), len);
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeReference(int ref)
{
	WritePrimitive(&ref, sizeof(int));
}
int CppLogic::Serializer::AdvLuaStateSerializer::DeserializeReference()
{
	if (ReadPrimitive() != sizeof(int))
		throw std::format_error{ "error reading reference, size mismatch" };
	return *static_cast<int*>(Data);
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeReferencedValue()
{
	int ref = DeserializeReference();
	L.GetTableRaw(IndexOfReferenceHolder, ref);
	if (L.IsNil(-1))
		throw std::format_error{ "error reading reference, invalid" };
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeTable(int idx)
{
	Reference r{ lua::LType::Table, L.ToPointer(idx) };
	auto refn = RefToNumber.find(r);
	if (refn == RefToNumber.end()) {
		L.CheckStack(5);
		int refnum = NextRefereceNumber;
		++NextRefereceNumber;
		RefToNumber.insert(std::make_pair(r, refnum));
		SerializeType(lua::LType::Table);
		SerializeReference(refnum);
		idx = L.ToAbsoluteIndex(idx);
		for (auto kv : L.Pairs(idx)) {
			if (CanSerialize(-1) && CanSerialize(-2)) {
				if (idx == L.GLOBALSINDEX && L.IsString(-2) && IsGlobalSkipped(L.ToString(-2)))
					continue;
				SerializeAnything(-2);
				SerializeAnything(-1);
			}
		}
		SerializeType(lua::LType::Nil); // i use serialized nil as end of table
		if (L.GetMetatable(-1)) {
			SerializeAnything(-1);
			L.Pop(1);
		}
		else
			SerializeType(lua::LType::Nil);
	}
	else {
		SerializeType(ReferenceType);
		SerializeReference(refn->second);
	}
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeTable(bool create)
{
	int ref = DeserializeReference();
	if (create)
		L.NewTable();

	L.PushValue(-1);
	L.SetTableRaw(IndexOfReferenceHolder, ref);

	while (true) {
		DeserializeAnything();
		if (L.IsNil(-1)) { // i use serialized nil as end of table
			L.Pop(1);
			break;
		}
		DeserializeAnything();
		L.SetTableRaw(-3);
	}

	DeserializeAnything();
	if (L.IsTable(-1)) // table or nil
		L.SetMetatable(-2);
	else
		L.Pop(1);
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeFunction(int idx)
{
	L.PushValue(idx);
	lua::DebugInfo i = L.Debug_GetInfoForFunc(lua::DebugInfoOptions::Upvalues);
	if (i.NumUpvalues > 0)
		throw std::format_error{ "cannot serialize a lua function with upvalues" };

	Reference r{ lua::LType::Function, L.ToPointer(idx) };
	auto refn = RefToNumber.find(r);
	if (refn == RefToNumber.end()) {
		int refnum = NextRefereceNumber;
		++NextRefereceNumber;
		RefToNumber.insert(std::make_pair(r, refnum));
		SerializeType(lua::LType::Function);
		SerializeReference(refnum);
		L.Dump([](lua_State*, const void* d, size_t len, void* ud) {
			AdvLuaStateSerializer* th = static_cast<AdvLuaStateSerializer*>(ud);
			th->WritePrimitive(d, len);
			return 0;
			}, this);
	}
	else {
		SerializeType(ReferenceType);
		SerializeReference(refn->second);
	}
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeFunction()
{
	int ref = DeserializeReference();
	L.Load([](lua_State*, void* ud, size_t* len) {
		AdvLuaStateSerializer* th = static_cast<AdvLuaStateSerializer*>(ud);
		*len = th->ReadPrimitive();
		return static_cast<const char*>(th->Data);
		}, this, nullptr);
	L.PushValue(-1);
	L.SetTableRaw(IndexOfReferenceHolder, ref);
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeAnything(int idx)
{
	switch (L.Type(idx)) {
	case lua::LType::Nil:
		SerializeType(lua::LType::Nil);
		break;
	case lua::LType::Boolean:
		SerializeBool(idx);
		break;
	case lua::LType::Number:
		SerializeNumber(idx);
		break;
	case lua::LType::String:
		SerializeString(idx);
		break;
	case lua::LType::Table:
		SerializeTable(idx);
		break;
	case lua::LType::Function:
		SerializeFunction(idx);
		break;
	default:
		throw std::format_error{ "invalid type" };
	}
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeAnything()
{
	switch (DeserializeType()) {
	case lua::LType::Nil:
		L.Push();
		break;
	case lua::LType::Boolean:
		DeserializeBool();
		break;
	case lua::LType::Number:
		DeserializeNumber();
		break;
	case lua::LType::String:
		DeserializeString();
		break;
	case lua::LType::Table:
		DeserializeTable(true);
		break;
	case lua::LType::Function:
		DeserializeFunction();
		break;
	case ReferenceType:
		DeserializeReferencedValue();
		break;
	default:
		throw std::format_error{ "invalid type" };
	}
}
bool CppLogic::Serializer::AdvLuaStateSerializer::CanSerialize(int idx)
{
	switch (L.Type(idx)) {
	case lua::LType::Nil:
	case lua::LType::Boolean:
	case lua::LType::Number:
	case lua::LType::String:
	case lua::LType::Table:
		return true;
	case lua::LType::Function:
	{
		if (L.IsCFunction(idx))
			return false;
		L.PushValue(idx);
		lua::DebugInfo i = L.Debug_GetInfoForFunc(lua::DebugInfoOptions::Upvalues);
		if (i.NumUpvalues > 0) {
			shok::LogString("AdvLuaStateSerializer: cannot serialize function with upvalues");
			return false;
		}
		return true;
	}
	default:
		return false;
	}
}

bool CppLogic::Serializer::AdvLuaStateSerializer::IsGlobalSkipped(const char* n)
{
	auto* nse = EScr::LuaStateSerializer::DoNotSerializeGlobals();
	std::string_view glob{ n };
	for (const auto& n : *nse) {
		std::string_view c{ n.c_str(), n.size() };
		if (glob == c)
			return true;
	}
	return false;
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeState()
{
	SerializeTable(L.GLOBALSINDEX);
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeState()
{
	L.NewTable();
	IndexOfReferenceHolder = L.GetTop();
	L.PushValue(L.GLOBALSINDEX);
	if (DeserializeType() != lua::LType::Table)
		throw std::format_error{ "_G is not a table" };
	DeserializeTable(false);
	L.SetTop(IndexOfReferenceHolder - 1);
}

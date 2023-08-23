#include "pch.h"
#include "luaserializer.h"
#include <format>
#include "s5_filesystem.h"
#include "s5_scriptsystem.h"

void CppLogic::Serializer::ObjectToLuaSerializer::SerializeField(lua::State L, void* o, const BB::SerializationData* s, bool keypushed)
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
			shok::ClassId id = BB::InvalidIdentifier;
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

void CppLogic::Serializer::ObjectToLuaSerializer::SerializeFields(lua::State L, void* o, const BB::SerializationData* s)
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

void CppLogic::Serializer::ObjectToLuaSerializer::SerializePushField(lua::State L, void* data, const BB::SerializationData* s)
{
	s->DataConverter->GetExtendedInfo().Push(L, data, s->DataConverter);
}

void CppLogic::Serializer::ObjectToLuaSerializer::SerializeList(lua::State L, void* o, const BB::SerializationData* s)
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

void CppLogic::Serializer::ObjectToLuaSerializer::Serialize(lua::State L, void* o, const BB::SerializationData* seri, shok::ClassId id)
{
	L.NewTable();
	if (id != shok::ClassId::Invalid) {
		L.Push("ObjectClassIdentification");
		L.Push(static_cast<double>(static_cast<unsigned int>(id)));
		L.SetTableRaw(-3);
		L.Push("ObjectClassName");
		L.Push((*BB::CClassFactory::GlobalObj)->GetClassDemangledName(id));
		L.SetTableRaw(-3);
	}
	SerializeFields(L, o, seri);
}
void CppLogic::Serializer::ObjectToLuaSerializer::Serialize(lua::State L, BB::IObject* o)
{
	shok::ClassId id = o->GetClassIdentifier();
	Serialize(L, o, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id), id);
}

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeField(lua::State L, void* o, const BB::SerializationData* s, bool valuepushed)
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
		shok::ClassId id = shok::ClassId::Invalid;
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

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeCheckField(lua::State L, void* o, const BB::SerializationData* s)
{
	s->DataConverter->GetExtendedInfo().Check(L, o, -1, s->DataConverter);
}

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeFields(lua::State L, void* o, const BB::SerializationData* s)
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

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeList(lua::State L, void* o, const BB::SerializationData* s)
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

void CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(lua::State L, void* o, const BB::SerializationData* seri, shok::ClassId id)
{
	if (o == nullptr) {
		if (id == shok::ClassId::Invalid) {
			L.Push("ObjectClassIdentification");
			L.GetTableRaw(-2);
			if (!L.IsNil(-1))
				id = static_cast<shok::ClassId>(static_cast<unsigned int>(L.CheckNumber(-1)));
			L.Pop(1);
			if (id == shok::ClassId::Invalid) {
				L.Push("ObjectClassName");
				L.GetTableRaw(-2);
				if (!L.IsNil(-1))
					id = (*BB::CClassFactory::GlobalObj)->GetIdentifierByName(L.CheckString(-1));
				L.Pop(1);
			}
		}
		if (id == shok::ClassId::Invalid)
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
void CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(lua::State L, BB::IObject* o)
{
	shok::ClassId id = o->GetClassIdentifier();
	Deserialize(L, o, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id), id);
}

void CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(lua::State L, const BB::SerializationData* d)
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
void CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(lua::State L, shok::ClassId id)
{
	DumpClassSerializationData(L, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id));
}

CppLogic::Serializer::AdvLuaStateSerializer::AdvLuaStateSerializer(BB::CFileStreamEx& io, lua_State* l)
	: IO{ io }, L{ l }
{
	HMODULE h = GetModuleHandle("S5Lua5.dll"); // get the already loaded dll, and then the (possibly existing) funcs
	if (h) {
		lua_upvalueid = reinterpret_cast<void* (__cdecl*)(lua_State*, int, int)>(GetProcAddress(h, "lua_upvalueid"));
		lua_upvaluejoin = reinterpret_cast<void(__cdecl*)(lua_State*, int, int, int, int)>(GetProcAddress(h, "lua_upvaluejoin"));
	}
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
	WritePrimitive(t);
}
lua::LType CppLogic::Serializer::AdvLuaStateSerializer::DeserializeType()
{
	lua::LType r = ReadPrimitive<lua::LType>("error reading lua type, size mismatch");
	if ((static_cast<int>(r) < static_cast<int>(lua::LType::Nil) || static_cast<int>(r) > static_cast<int>(lua::LType::Thread)) && r != ReferenceType && r != UpvalueReferenceType)
		throw std::format_error{ "error reading lua type, not a valid type" };
	return r;
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeBool(int idx)
{
	SerializeType(lua::LType::Boolean);
	bool b = L.ToBoolean(idx);
	WritePrimitive(b);
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeBool()
{
	L.Push(ReadPrimitive<bool>("error reading bool, size mismatch"));
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeNumber(int idx)
{
	SerializeType(lua::LType::Number);
	double n = L.ToNumber(idx);
	WritePrimitive(n);
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeNumber()
{
	L.Push(ReadPrimitive<double>("error reading number, size mismatch"));
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
	WritePrimitive(ref);
}
int CppLogic::Serializer::AdvLuaStateSerializer::DeserializeReference()
{
	return ReadPrimitive<int>("error reading reference, size mismatch");
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeReferencedValue()
{
	int ref = DeserializeReference();
	L.GetTableRaw(IndexOfReferenceHolder, ref);
	if (L.IsNil(-1))
		throw std::format_error{ "error reading reference, invalid" };
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeTable(int idx, bool isglobal)
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
				if (isglobal && L.IsString(-2) && IsGlobalSkipped(L.ToString(-2)))
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
	if (!(lua_upvalueid && lua_upvaluejoin)) {
		if (i.NumUpvalues > 0)
			throw std::format_error{ "cannot serialize a lua function with upvalues" };
	}

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
		WritePrimitive(i.NumUpvalues);
		if (lua_upvalueid && lua_upvaluejoin) {
			for (int n = 1; n <= i.NumUpvalues; ++n) {
				const void* id = lua_upvalueid(L.GetState(), idx, n);
				auto uref = UpRefs.find(id);
				if (uref == UpRefs.end()) {
					UpRefs.insert(std::make_pair(id, UpvalueReference{ refnum, n }));
					L.Debug_GetUpvalue(idx, n);
					SerializeAnything(-1);
					L.Pop(1);
				}
				else {
					SerializeType(UpvalueReferenceType);
					WritePrimitive(uref->second.FuncReference);
					WritePrimitive(uref->second.UpvalueNum);
				}
			}
		}
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
	int nups = ReadPrimitive<int>("deserialze func invalid upvalue number");
	if (lua_upvalueid && lua_upvaluejoin) {
		for (int n = 1; n <= nups; ++n) {
			lua::LType t = DeserializeType();
			if (t == UpvalueReferenceType) {
				int funcref = ReadPrimitive<int>("deserialze func invalid upvalue reference func");
				int upnum = ReadPrimitive<int>("deserialze func invalid upvalue reference up");
				L.GetTableRaw(IndexOfReferenceHolder, funcref);
				if (L.IsNil(-1))
					throw std::format_error{ "error reading upvalue reference, invalid" };
				lua_upvaluejoin(L.GetState(), -2, n, -1, upnum);
				L.Pop(1);
			}
			else {
				DeserializeAnything(t);
				L.Debug_SetUpvalue(-2, n);
			}
		}
	}
	else {
		if (nups > 0)
			throw std::format_error{ "attempting to deserialize func with upvalues when upvalue serilaization is not available" };
	}
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeUserdata(int idx)
{
	Reference r{ lua::LType::Userdata, L.ToPointer(idx) };
	auto refn = RefToNumber.find(r);
	if (refn == RefToNumber.end()) {
		idx = L.ToAbsoluteIndex(idx);
		int refnum = NextRefereceNumber;
		++NextRefereceNumber;
		RefToNumber.insert(std::make_pair(r, refnum));
		SerializeType(lua::LType::Userdata);
		SerializeReference(refnum);
		if (!L.GetMetatable(idx))
			throw std::format_error{ "cannot serialize a userdata without serializer" };
		L.Push(UserdataSerializerMetaEvent);
		L.GetTableRaw(-2);
		if (!L.IsFunction(-1))
			throw std::format_error{ "cannot serialize a userdata without serializer function" };
		L.PushValue(idx);
		L.TCall(1, 2);
		SerializeString(-2); // name
		SerializeAnything(-1); // probably a table or function
		L.Pop(3);
	}
	else {
		SerializeType(ReferenceType);
		SerializeReference(refn->second);
	}
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeUserdata()
{
	int ref = DeserializeReference();

	if (DeserializeType() != lua::LType::String)
		throw std::format_error{ "deserialize udata name error" };
	size_t len = ReadPrimitive();
	std::string tname{ static_cast<const char*>(Data), len };
	auto f = UserdataDeserializer.find(tname);
	if (f == UserdataDeserializer.end())
		throw std::format_error{ "deserialize udata name not found" };
	L.Push(f->second);
	DeserializeAnything();
	L.TCall(1, 1);

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
	case lua::LType::Userdata:
		SerializeUserdata(idx);
		break;
	default:
		throw std::format_error{ "invalid type" };
	}
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeAnything()
{
	DeserializeAnything(DeserializeType());
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeAnything(lua::LType t)
{
	switch (t) {
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
	case lua::LType::Userdata:
		DeserializeUserdata();
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
		if (!(lua_upvalueid && lua_upvaluejoin)) {
			L.PushValue(idx);
			lua::DebugInfo i = L.Debug_GetInfoForFunc(lua::DebugInfoOptions::Upvalues);
			if (i.NumUpvalues > 0) {
				shok::LogString("AdvLuaStateSerializer: cannot serialize function with upvalues");
				return false;
			}
		}
		return true;
	}
	case lua::LType::Userdata:
	{
		if (!L.GetMetatable(idx))
			return false;
		L.Push(UserdataSerializerMetaEvent);
		L.GetTableRaw(-2);
		bool r = L.IsFunction(-1);
		L.Pop(2);
		return r;
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

void CppLogic::Serializer::AdvLuaStateSerializer::PrepareSerialize()
{
	WritePrimitive(FileVersion);
	WritePrimitive(L.Version());
	IndexOfReferenceHolder = L.GetTop();
}
void CppLogic::Serializer::AdvLuaStateSerializer::CleanupSerialize()
{
	L.SetTop(IndexOfReferenceHolder);
}
void CppLogic::Serializer::AdvLuaStateSerializer::PrepareDeserialize()
{
	int v = ReadPrimitive<int>("fileversion invalid size");
	if (v == 1) { // legacy save format
		lua_upvalueid = nullptr;
		lua_upvaluejoin = nullptr;
		// try to load anyway, will crash on invalid version
	}
	else if (v == FileVersion) {
		double luaver = ReadPrimitive<double>("luaversion invalid size");
		if (L.Version() != luaver)
			throw std::format_error{ "file version missmatch" };
	}
	else {
		throw std::format_error{ "invalid fileversion" };
	}
	L.NewTable();
	IndexOfReferenceHolder = L.GetTop();
}
void CppLogic::Serializer::AdvLuaStateSerializer::CleanupDeserialize(bool ret)
{
	int t = L.GetTop();
	if (ret) {
		L.Insert(IndexOfReferenceHolder);
		L.SetTop(IndexOfReferenceHolder);
	}
	else {
		L.SetTop(IndexOfReferenceHolder - 1);
	}
}

std::map<std::string, lua::CFunction> CppLogic::Serializer::AdvLuaStateSerializer::UserdataDeserializer{};

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeState()
{
	PrepareSerialize();

	L.PushGlobalTable();
	SerializeTable(-1, true);

	PushSerializedRegistry(L);
	SerializeTable(-1, false);

	CleanupSerialize();
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeState()
{
	PrepareDeserialize();

	L.PushGlobalTable();
	if (DeserializeType() != lua::LType::Table)
		throw std::format_error{ "_G is not a table" };
	DeserializeTable(false);

	L.Push(RegistrySerializeKeys);
	DeserializeAnything();
	L.SetTableRaw(L.REGISTRYINDEX);

	CleanupDeserialize(false);
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeVariable(int i)
{
	PrepareSerialize();

	SerializeAnything(i);

	CleanupSerialize();
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeVariable()
{
	PrepareDeserialize();

	DeserializeAnything();

	CleanupDeserialize(true);
}

void CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(lua::State L)
{
	L.GetSubTable(RegistrySerializeKeys, L.REGISTRYINDEX);
}

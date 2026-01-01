#include "luaserializer.h"
#include <format>
#include <shok/globals/s5_filesystem.h>
#include <shok/s5_scriptsystem.h>
#include <utility/OnScopeExit.h>

void CppLogic::Serializer::ObjectToLuaSerializer::SerializeField(luaext::State L, void* o, const BB::SerializationData* s, bool keypushed)
{
	switch (s->Type) {
	case BB::SerializationData::Ty::Direct:
		if (!keypushed)
			L.Push(s->SerializationName);
		SerializePushField(L, o, s);
		L.SetTableRaw(-3);
		break;
	case BB::SerializationData::Ty::Embedded:
		if (!keypushed)
		L.Push(s->SerializationName);
		L.NewTable();

		SerializeFields(L, o, s->SubElementData);

		L.SetTableRaw(-3);
		break;
	case BB::SerializationData::Ty::ObjectPointer:
		o = *static_cast<void**>(o);
		[[fallthrough]];
	case BB::SerializationData::Ty::ObjectEmbedded:
		{
			if (!keypushed)
				L.Push(s->SerializationName);

			if (o == nullptr) {
				L.Push();
			}
			else {
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
			}

			L.SetTableRaw(-3);
		}
		break;
	default:
		if (keypushed)
			L.Push("unknown serialization type");
		break;
	}
}

void CppLogic::Serializer::ObjectToLuaSerializer::SerializeFields(luaext::State L, void* o, const BB::SerializationData* s)
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

void CppLogic::Serializer::ObjectToLuaSerializer::SerializePushField(luaext::State L, void* data, const BB::SerializationData* s)
{
	s->DataConverter->GetExtendedInfo().Push(L, data, s->DataConverter);
}

void CppLogic::Serializer::ObjectToLuaSerializer::SerializeList(luaext::State L, void* o, const BB::SerializationData* s)
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

void CppLogic::Serializer::ObjectToLuaSerializer::Serialize(luaext::State L, void* o, const BB::SerializationData* seri, shok::ClassId id)
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
void CppLogic::Serializer::ObjectToLuaSerializer::Serialize(luaext::State L, BB::IObject* o)
{
	shok::ClassId id = o->GetClassIdentifier();
	Serialize(L, o, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id), id);
}

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeField(luaext::State L, void* o, const BB::SerializationData* s, bool valuepushed)
{
	switch (s->Type) {
	case BB::SerializationData::Ty::Direct:
		if (!valuepushed) {
			L.Push(s->SerializationName);
			L.GetTableRaw(-2);
		}

		if (!L.IsNil(-1))
			DeserializeCheckField(L, o, s);

		if (!valuepushed)
			L.Pop(1);
		break;
	case BB::SerializationData::Ty::Embedded:
		if (!valuepushed) {
			L.Push(s->SerializationName);
			L.GetTableRaw(-2);
		}

		if (!L.IsNil(-1))
			DeserializeFields(L, o, s->SubElementData);

		if (!valuepushed)
			L.Pop(1);
		break;
	case BB::SerializationData::Ty::ObjectPointer:
		o = *static_cast<void**>(o);
		[[fallthrough]];
	case BB::SerializationData::Ty::ObjectEmbedded:
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

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeCheckField(luaext::State L, void* o, const BB::SerializationData* s)
{
	s->DataConverter->GetExtendedInfo().Check(L, o, -1, s->DataConverter);
}

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeFields(luaext::State L, void* o, const BB::SerializationData* s)
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

void CppLogic::Serializer::ObjectToLuaSerializer::DeserializeList(luaext::State L, void* o, const BB::SerializationData* s)
{
	L.Push(s->SerializationName);
	L.GetTableRaw(-2);

	if (L.IsTable(-1)) {
		for ([[maybe_unused]] int _ : L.IPairs(-1)) {
			void* d = s->ListOptions->AddToList(o);
			DeserializeField(L, d, s, true);
			s->ListOptions->FinalizeAddToList(o);
		}
	}

	L.Pop(1);
}

void* CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(luaext::State L, void* o, const BB::SerializationData* seri, shok::ClassId id, std::initializer_list<shok::ClassId> whitelisted_ids)
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
		if (whitelisted_ids.size() > 0) {
			if (std::find(whitelisted_ids.begin(), whitelisted_ids.end(), id) == whitelisted_ids.end())
				throw std::invalid_argument{std::format("class id {} not allowed", static_cast<int>(id))};
		}
		o = (*BB::CClassFactory::GlobalObj)->CreateObject(id);
		if (o == nullptr)
			throw std::invalid_argument{ "invalid id provided" };
		if (seri == nullptr) {
			seri = (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
		}
	}
	DeserializeFields(L, o, seri);
	return o;
}
void CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(luaext::State L, BB::IObject* o)
{
	shok::ClassId id = o->GetClassIdentifier();
	Deserialize(L, o, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id), id);
}

void CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(luaext::State L, const BB::SerializationData* d)
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

		if (d->Type == BB::SerializationData::Ty::Direct) {
			L.Push("DataType");
			L.Push("Field");
			L.SetTableRaw(-3);
			L.Push("FieldType");
			L.Push(d->DataConverter->GetTypeDescName());
			L.SetTableRaw(-3);
		}
		else if (d->Type == BB::SerializationData::Ty::Embedded) {
			L.Push("DataType");
			L.Push("Embedded Object/Base Class");
			L.SetTableRaw(-3);
			L.Push("SubElement");
			DumpClassSerializationData(L, d->SubElementData);
			L.SetTableRaw(-3);
		}
		else if (d->Type == BB::SerializationData::Ty::ObjectEmbedded) {
			L.Push("DataType");
			L.Push("Embedded BB::IObject");
			L.SetTableRaw(-3);
		}
		else if (d->Type == BB::SerializationData::Ty::ObjectPointer) {
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
void CppLogic::Serializer::ObjectToLuaSerializer::DumpClassSerializationData(luaext::State L, shok::ClassId id)
{
	DumpClassSerializationData(L, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id));
}



void CppLogic::Serializer::FastBinarySerializer::SerializeField(BB::IStream& s, void* o, const BB::SerializationData* seri)
{
	switch (seri->Type) {
	case BB::SerializationData::Ty::Direct:
	{
		const BB::FieldSerializer* fs = seri->DataConverter;
		if (fs == BB::FieldSerializer::TypeAccessCategories)
			fs = BB::FieldSerializer::TypeInt;
		if (fs->SerializeToStream == nullptr)
			throw std::logic_error{ "SerializeToStream missing" };
		fs->SerializeToStream(&s, o);
		break;
	}
	case BB::SerializationData::Ty::Embedded:
		SerializeFields(s, o, seri->SubElementData);
		break;
	case BB::SerializationData::Ty::ObjectPointer:
		o = *static_cast<void**>(o);
		[[fallthrough]];
	case BB::SerializationData::Ty::ObjectEmbedded:
		if (o == nullptr) {
			s.Write("0");
		}
		else {
			s.Write("1");
			if (seri->GetIdentifier)
				Serialize(s, static_cast<BB::IObject*>(o), seri->GetIdentifier(o));
			else
				SerializeFields(s, o, seri->SubElementData);
		}
		break;
	default:
		break;
	}
}

void CppLogic::Serializer::FastBinarySerializer::SerializeFields(BB::IStream& s, void* o, const BB::SerializationData* seri)
{
	while (seri->Size) {
		if (seri->SerializationName) {
			void* data = reinterpret_cast<void*>(reinterpret_cast<int>(o) + static_cast<int>(seri->Position));
			if (seri->ListOptions)
				SerializeList(s, data, seri);
			else
				SerializeField(s, data, seri);
		}
		else if (seri->SubElementData) {
			SerializeFields(s, o, seri->SubElementData);
		}
		seri++;
	}
}

void CppLogic::Serializer::FastBinarySerializer::SerializeList(BB::IStream& s, void* o, const BB::SerializationData* seri)
{
	auto* it = seri->ListOptions->AllocIter(o);

	size_t size = seri->ListOptions->GetSize(o);
	s.Write(&size, sizeof(size));
	while (seri->ListOptions->IterNext(it)) {
		SerializeField(s, seri->ListOptions->IterCurrent(it), seri);
	}

	seri->ListOptions->FreeIter(it);
}

void CppLogic::Serializer::FastBinarySerializer::Serialize(BB::IStream& s, void* o, const BB::SerializationData* seri)
{
	SerializeFields(s, o, seri);
}

void CppLogic::Serializer::FastBinarySerializer::Serialize(BB::IStream& s, BB::IObject* o, shok::ClassId id)
{
	if (id == shok::ClassId::Invalid)
		id = o->GetClassIdentifier();
	auto* seri = (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
	s.Write(&id, sizeof(id));
	Serialize(s, o, seri);
}

void CppLogic::Serializer::FastBinarySerializer::DeserializeField(BB::IStream& s, void* o, const BB::SerializationData* seri)
{

	switch (seri->Type) {
	case BB::SerializationData::Ty::Direct:
	{
		const BB::FieldSerializer* fs = seri->DataConverter;
		if (fs == BB::FieldSerializer::TypeAccessCategories)
			fs = BB::FieldSerializer::TypeInt;
		if (fs->DeserializeFromStream == nullptr)
			throw std::logic_error{ "DeserializeFromStream missing" };
		fs->DeserializeFromStream(o, &s);
		break;
	}
	case BB::SerializationData::Ty::Embedded:
		DeserializeFields(s, o, seri->SubElementData);
		break;
	case BB::SerializationData::Ty::ObjectPointer:
		o = *static_cast<void**>(o);
		[[fallthrough]];
	case BB::SerializationData::Ty::ObjectEmbedded:
	{
		char c = '\0';
		s.Read(&c, sizeof(c));
		if (c != '\0') {
			if (seri->GetIdentifier)
				Deserialize(s, static_cast<BB::IObject*>(o), nullptr, shok::ClassId::Invalid);
			else
				DeserializeFields(s, o, seri->SubElementData);
		}
		else {
			*static_cast<void**>(o) = nullptr;
		}
		break;
	}
	default:
		break;
	}
}

void CppLogic::Serializer::FastBinarySerializer::DeserializeFields(BB::IStream& s, void* o, const BB::SerializationData* seri)
{
	while (seri->Size) {
		if (seri->SerializationName) {
			void* data = reinterpret_cast<void*>(reinterpret_cast<int>(o) + static_cast<int>(seri->Position));
			if (seri->ListOptions)
				DeserializeList(s, data, seri);
			else
				DeserializeField(s, data, seri);
		}
		else if (seri->SubElementData) {
			DeserializeFields(s, o, seri->SubElementData);
		}
		seri++;
	}
}

void CppLogic::Serializer::FastBinarySerializer::DeserializeList(BB::IStream& s, void* o, const BB::SerializationData* seri)
{
	size_t size = 0;
	s.Read(&size, sizeof(size));
	for (size_t k = 0; k < size; ++k) {
		void* d = seri->ListOptions->AddToList(o);
		DeserializeField(s, d, seri);
		seri->ListOptions->FinalizeAddToList(o);
	}
}

void* CppLogic::Serializer::FastBinarySerializer::Deserialize(BB::IStream& s, void* o, const BB::SerializationData* seri, shok::ClassId id, std::initializer_list<shok::ClassId> whitelisted_ids)
{
	if (o == nullptr) {
		if (id == shok::ClassId::Invalid) {
			s.Read(&id, sizeof(id));
		}
		if (id == shok::ClassId::Invalid)
			throw std::invalid_argument{ "no object and no id provided" };
		if (whitelisted_ids.size() > 0) {
			if (std::find(whitelisted_ids.begin(), whitelisted_ids.end(), id) == whitelisted_ids.end())
				throw std::invalid_argument{ std::format("class id {} not allowed", static_cast<int>(id)) };
		}
		o = (*BB::CClassFactory::GlobalObj)->CreateObject(id);
		if (o == nullptr)
			throw std::invalid_argument{ "invalid id provided" };
		if (seri == nullptr) {
			seri = (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
		}
	}
	DeserializeFields(s, o, seri);
	return o;
}

void CppLogic::Serializer::FastBinarySerializer::Deserialize(BB::IStream& s, BB::IObject* o)
{
	shok::ClassId id = o->GetClassIdentifier();
	Deserialize(s, o, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id), id);
}



CppLogic::Serializer::AdvLuaStateSerializer::AdvLuaStateSerializer(BB::IStream& io, lua_State* l, bool dataOnly)
	: IO{ io }, L{ l }, DataOnly(dataOnly)
{
	HMODULE h = GetModuleHandle("S5Lua5.dll"); // get the already loaded dll, and then the (possibly existing) funcs
	if (h) {
		lua_upvalueid = reinterpret_cast<void* (__cdecl*)(lua_State*, int, int)>(GetProcAddress(h, "lua_upvalueid"));
		lua_upvaluejoin = reinterpret_cast<void(__cdecl*)(lua_State*, int, int, int, int)>(GetProcAddress(h, "lua_upvaluejoin"));
	}
}

void CppLogic::Serializer::AdvLuaStateSerializer::WritePrimitive(const void* d, size_t len)
{
	IO.Write(&len, sizeof(size_t));
	IO.Write(d, static_cast<long>(len));
}

size_t CppLogic::Serializer::AdvLuaStateSerializer::ReadPrimitive()
{
	size_t len = 0;
	IO.Read(&len, sizeof(size_t));
	if (len > DataLength) {
		Data = std::unique_ptr<byte[]>(new byte[len]);
		DataLength = len;
	}
	IO.Read(Data.get(), static_cast<long>(len));
	return len;
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeType(lua::LType t)
{
	WritePrimitive(t);
}
lua::LType CppLogic::Serializer::AdvLuaStateSerializer::DeserializeType()
{
	auto r = ReadPrimitive<lua::LType>("error reading lua type, size mismatch");
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
	double n = *L.ToNumber(idx);
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
	L.Push(reinterpret_cast<const char*>(Data.get()), len);
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
		for ([[maybe_unused]] auto _ : L.Pairs(idx)) {
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
// ReSharper disable once CppDFAConstantParameter
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeTable(bool create)
{
	int ref = DeserializeReference();
	// ReSharper disable once CppDFAConstantConditions
	if (create) {
		// ReSharper disable once CppDFAUnreachableCode
		L.NewTable();
	}

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
	if (DataOnly)
		throw std::format_error{ "functions not allowed" };
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
		L.PushValue(idx);
		L.Dump([](lua_State*, const void* d, size_t len, void* ud) {
			auto* th = static_cast<AdvLuaStateSerializer*>(ud);
			th->WritePrimitive(d, len);
			return 0;
			}, this);
		L.Pop(1);
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
	if (DataOnly)
		throw std::format_error{ "functions not allowed" };
	int ref = DeserializeReference();
	L.Load([](lua_State*, void* ud, size_t* len) {
		auto* th = static_cast<AdvLuaStateSerializer*>(ud);
		*len = th->ReadPrimitive();
		return reinterpret_cast<const char*>(th->Data.get());
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
			throw std::format_error{ "attempting to deserialize func with upvalues when upvalue serialization is not available" };
	}
}

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeUserdata(int idx)
{
	if (DataOnly)
		throw std::format_error{ "functions not allowed" };
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
	if (DataOnly)
		throw std::format_error{ "functions not allowed" };
	int ref = DeserializeReference();

	if (DeserializeType() != lua::LType::String)
		throw std::format_error{ "deserialize udata name error" };
	size_t len = ReadPrimitive();
	std::string tname{ reinterpret_cast<const char*>(Data.get()), len };
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
	for (const auto& i : *nse) {
		std::string_view c{ i.c_str(), i.size() };
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
		auto luaver = ReadPrimitive<double>("luaversion invalid size");
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
	if (ret) {
		L.Remove(IndexOfReferenceHolder);
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

void CppLogic::Serializer::AdvLuaStateSerializer::SerializeStack(int n)
{
	if (n < 0)
		n = L.GetTop();

	PrepareSerialize();

	WritePrimitive(n);

	for (int i = 1; i <= n; ++i)
		SerializeAnything(i);

	CleanupSerialize();
}
void CppLogic::Serializer::AdvLuaStateSerializer::DeserializeStack()
{
	PrepareDeserialize();

	int n = ReadPrimitive<int>("number of elements size missmatch");

	for (int i = 1; i <= n; ++i) {
		DeserializeAnything();
	}

	CleanupDeserialize(true);
}

void CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(luaext::State L)
{
	L.GetSubTable(RegistrySerializeKeys, L.REGISTRYINDEX);
}

CppLogic::Serializer::ObjectAccess::ObjectAccess(std::string_view name, void* obj, const BB::SerializationData* sd, int id, void(*onWrite)(int id))
	: SDName(name), Object(obj), SeriData(sd), Id(id), OnWrite(onWrite == nullptr ? [](int) {} : onWrite)
{
}

int CppLogic::Serializer::ObjectAccess::Name(luaext::State L)
{
	auto* th = L.CheckUserClass<ObjectAccess>(1);
	L.Push(th->SDName);
	return 1;
}

void CppLogic::Serializer::ObjectAccess::PushSD(luaext::State L, std::string_view n, void* obj, const BB::SerializationData* sd, int id, void(*onWrite)(int id), bool listElement)
{
	if (sd->Size == 0) { // NOLINT(*-branch-clone)
		L.Push();
	}
	else if (sd->ListOptions != nullptr && !listElement) {
		L.NewUserClass<ListAccess>(n, obj, sd, id, onWrite);
	}
	else if (sd->Type == BB::SerializationData::Ty::Embedded) {
		L.NewUserClass<StructAccess>(n, obj, sd->SubElementData, id, onWrite);
	}
	else if (sd->Type == BB::SerializationData::Ty::Direct) {
		L.NewUserClass<FieldAccess>(n, obj, sd, id, onWrite);
	}
	else if (sd->Type == BB::SerializationData::Ty::ObjectPointer) {
		shok::ClassId c;
		void** owner = reinterpret_cast<void**>(obj);
		obj = *owner;
		if (obj == nullptr) {
			L.NewUserClass<BBObjectAccess>(n, obj, nullptr, id, onWrite, owner);
		}
		else {
			if (sd->GetIdentifier != nullptr) {
				c = sd->GetIdentifier(obj);
			}
			else {
				c = (reinterpret_cast<const BB::IObject*>(obj))->GetClassIdentifier();
			}
			PushObject(L, n, reinterpret_cast<BB::IObject*>(obj), id, onWrite, c, owner);
		}
	}
	else {
		L.Push();
	}
}

void CppLogic::Serializer::ObjectAccess::PushObject(luaext::State L, std::string_view n, BB::IObject* obj, int id, void(*onWrite)(int id), shok::ClassId c, void** owner)
{
	if (c == shok::ClassId::Invalid)
		c = obj->GetClassIdentifier();
	L.NewUserClass<BBObjectAccess>(n, obj, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(c), id, onWrite, owner);
}

void CppLogic::Serializer::ObjectAccess::PushObject(luaext::State L, std::string_view n, void* obj, const BB::SerializationData* sd, int id, void(*onWrite)(int id))
{
	L.NewUserClass<StructAccess>(n, obj, sd, id, onWrite);
}

int CppLogic::Serializer::FieldAccess::GetType(luaext::State L)
{
	L.Push(Type::Field);
	return 1;
}

int CppLogic::Serializer::FieldAccess::DataType(luaext::State L)
{
	auto* th = L.CheckUserClass<FieldAccess>(1);
	L.Push(th->SeriData->DataConverter->GetTypeDescName());
	return 1;
}

int CppLogic::Serializer::FieldAccess::Get(luaext::State L)
{
	auto* th = L.CheckUserClass<FieldAccess>(1);
	th->SeriData->DataConverter->GetExtendedInfo().Push(L, th->Object, th->SeriData->DataConverter);
	return 1;
}

int CppLogic::Serializer::FieldAccess::Set(luaext::State L)
{
	auto* th = L.CheckUserClass<FieldAccess>(1);
	th->OnWrite(th->Id);
	th->SeriData->DataConverter->GetExtendedInfo().Check(L, th->Object, 2, th->SeriData->DataConverter);
	return 0;
}

int CppLogic::Serializer::StructAccess::GetType(luaext::State L)
{
	L.Push(Type::Struct);
	return 1;
}

int CppLogic::Serializer::StructAccess::Index(luaext::State L)
{
	auto* th = L.CheckUserClass<StructAccess>(1);
	if (th->Object == nullptr)
		throw lua::LuaException("object null");
	auto m = L.CheckStringView(2);
	Iter i{ th->Object, th->SeriData };
	for (const auto& [obj, sd] : i) {
		if (sd->SerializationName == m) {
			PushSD(L, sd->SerializationName, obj, sd, th->Id, th->OnWrite);
			return 1;
		}
	}
	return 0;
}

int CppLogic::Serializer::StructAccess::Fields(luaext::State L)
{
	auto* th = L.CheckUserClass<StructAccess>(1);
	if (th->Object == nullptr)
		throw lua::LuaException("object null");
	L.NewUserClass<Iter>(th->Object, th->SeriData);
	L.PushValue(1);
	L.Push<FieldsNext>(2);
	return 1;
}

int CppLogic::Serializer::StructAccess::FieldsNext(luaext::State L)
{
	auto* i = L.CheckUserClass<Iter>(L.Upvalueindex(1));
	auto* th = L.CheckUserClass<StructAccess>(L.Upvalueindex(2));
	if (i->First) {
		i->First = false;
	}
	else {
		++(*i);
	}
	if (*i == i->end()) {
		return 0;
	}
	else {
		const auto& [obj, sd] = **i;
		PushSD(L, sd->SerializationName, obj, sd, th->Id, th->OnWrite);
		return 1;
	}
}

bool CppLogic::Serializer::StructAccess::Iter::operator==(std::default_sentinel_t) const
{
	return SeriData->Size == 0;
}

std::pair<void*, const BB::SerializationData*> CppLogic::Serializer::StructAccess::Iter::operator*() const
{
	return {reinterpret_cast<void*>(reinterpret_cast<int>(Obj) + SeriData->Position), SeriData}; // all child objects have offset 0
}

CppLogic::Serializer::StructAccess::Iter& CppLogic::Serializer::StructAccess::Iter::operator++()
{
	++SeriData;
	while (SeriData->Size == 0 && CurrentSubObject != Root) {
		SeriData = Root;
		while (SeriData->Type == BB::SerializationData::Ty::Embedded && SeriData->SerializationName == nullptr && SeriData->SubElementData != CurrentSubObject) {
			SeriData = SeriData->SubElementData;
		}
		CurrentSubObject = SeriData;
		++SeriData;
	}
	return *this;
}

CppLogic::Serializer::StructAccess::Iter::Iter(void* o, const BB::SerializationData* sd) : Obj(o), SeriData(sd), Root(sd), CurrentSubObject(nullptr)
{
	while (SeriData->Type == BB::SerializationData::Ty::Embedded && SeriData->SerializationName == nullptr) {
		SeriData = SeriData->SubElementData;
	}
	CurrentSubObject = SeriData;
}

CppLogic::Serializer::BBObjectAccess::BBObjectAccess(std::string_view name, void* obj, const BB::SerializationData* sd, int id, void(*onWrite)(int id), void** owner)
	: StructAccess(name, obj, sd, id, onWrite), Owner(owner)
{
}

int CppLogic::Serializer::BBObjectAccess::GetType(luaext::State L)
{
	L.Push(Type::BBObject);
	return 1;
}

int CppLogic::Serializer::BBObjectAccess::ObjectType(luaext::State L)
{
	auto* th = L.CheckUserClass<BBObjectAccess>(1);
	if (th->Object == nullptr) {
		return 0;
	}
	shok::ClassId id;
	if (th->SeriData->GetIdentifier != nullptr) {
		id = th->SeriData->GetIdentifier(th->Object);
	}
	else {
		id = (reinterpret_cast<const BB::IObject*>(th->Object))->GetClassIdentifier();
	}
	L.Push((*BB::CClassFactory::GlobalObj)->GetClassDemangledName(id));
	L.Push(static_cast<double>(static_cast<unsigned int>(id)));
	return 2;
}

int CppLogic::Serializer::BBObjectAccess::New(luaext::State L)
{
	auto* th = L.CheckUserClass<BBObjectAccess>(1);
	if (th->Owner == nullptr) {
		throw lua::LuaException{ "no owner" };
	}
	shok::ClassId id;
	if (L.IsNoneOrNil(2))
		id = shok::ClassId::Invalid;
	else
		id = L.CheckEnum<shok::ClassId>(2);
	bool free = L.OptBool(3, true);
	th->OnWrite(th->Id);
	if (free && th->Object != nullptr) {
		delete static_cast<BB::IObject*>(th->Object);
		th->Object = nullptr;
		*th->Owner = nullptr;
	}
	if (id != shok::ClassId::Invalid)
	{
		BB::CClassFactory* f = *BB::CClassFactory::GlobalObj;
		th->Object = f->CreateObject(id);
		*th->Owner = th->Object;
		th->SeriData = f->GetSerializationDataForClass(id);
	}
	L.SetTop(1);
	return 1;
}

int CppLogic::Serializer::BBObjectAccess::IsNullptr(luaext::State L)
{
	auto* th = L.CheckUserClass<BBObjectAccess>(1);
	L.Push(th->Object == nullptr);
	L.Push(th->Owner == nullptr);
	return 2;
}

int CppLogic::Serializer::ListAccess::GetType(luaext::State L)
{
	L.Push(Type::List);
	return 1;
}

int CppLogic::Serializer::ListAccess::Elements(luaext::State L)
{
	auto* th = L.CheckUserClass<ListAccess>(1);
	L.NewUserClass<ElementIter>(th->SeriData->ListOptions->UniqueIter(th->Object));
	L.PushValue(1);
	L.Push<ElementsNext>(2);
	return 1;
}

int CppLogic::Serializer::ListAccess::ElementsNext(luaext::State L)
{
	auto* i = L.CheckUserClass<ElementIter>(L.Upvalueindex(1));
	auto* th = L.CheckUserClass<ListAccess>(L.Upvalueindex(2));
	if (th->SeriData->ListOptions->IterNext(i->Iter.get())) {
		void* obj = th->SeriData->ListOptions->IterCurrent(i->Iter.get());
		PushSD(L, th->SeriData->SerializationName, obj, th->SeriData, th->Id, th->OnWrite, true);
		return 1;
	}
	return 0;
}

int CppLogic::Serializer::ListAccess::Size(luaext::State L)
{
	auto* th = L.CheckUserClass<ListAccess>(1);
	L.Push(static_cast<double>(th->SeriData->ListOptions->GetSize(th->Object)));
	return 1;
}

int CppLogic::Serializer::ListAccess::First(luaext::State L)
{
	auto* th = L.CheckUserClass<ListAccess>(1);
	auto i = th->SeriData->ListOptions->UniqueIter(th->Object);
	while (th->SeriData->ListOptions->IterNext(i.get())) {
		void* obj = th->SeriData->ListOptions->IterCurrent(i.get());
		PushSD(L, th->SeriData->SerializationName, obj, th->SeriData, th->Id, th->OnWrite, true);
		L.PushValue(2);
		L.PushValue(-2);
		L.TCall(1, 1);
		if (L.ToBoolean(-1))
		{
			L.Pop(1);
			return 1;
		}
		L.SetTop(2);
	}
	return 0;
}

int CppLogic::Serializer::ListAccess::Insert(luaext::State L)
{
	auto* th = L.CheckUserClass<ListAccess>(1);
	th->OnWrite(th->Id);
	void* o = th->SeriData->ListOptions->AddToList(th->Object);
	L.PushValue(2);
	PushSD(L, th->SeriData->SerializationName, o, th->SeriData, th->Id, th->OnWrite, true);
	CppLogic::OnScopeExit ex{ [&]() {
		th->SeriData->ListOptions->FinalizeAddToList(th->Object);
	} };
	L.TCall(1, 0);
	return 0;
}

int CppLogic::Serializer::ListAccess::Remove(luaext::State L)
{
	auto* th = L.CheckUserClass<ListAccess>(1);
	const auto& inf = th->SeriData->ListOptions->GetExtendedInfo();
	if (inf.RemoveIf == nullptr)
		throw lua::LuaException{ "no remove available" };
	struct Data {
		luaext::State L;
		ListAccess* A = nullptr;
	} d{ L, th };
	inf.RemoveIf(th->Object, [](void* uv, const BB::SerializationData* sd, void* el) {
		Data* d = static_cast<Data*>(uv);
		int t = d->L.GetTop();
		d->L.PushValue(2);
		PushSD(d->L, sd->SerializationName, el, sd, d->A->Id, d->A->OnWrite, true);
		bool r = false;
		if (d->L.PCall(1, 1) == luaext::State::ErrorCode::Success)
			r = d->L.ToBoolean(-1);
		d->L.SetTop(t);
		return r;
		}, &d, th->SeriData);
	return 0;
}

int CppLogic::Serializer::ListAccess::ListType(luaext::State L)
{
	auto* th = L.CheckUserClass<ListAccess>(1);
	const auto* inf = th->SeriData->ListOptions->TryGetExtendedInfo();
	if (inf == nullptr) {
		L.Push(static_cast<int>(BB::SerializationListOptions::ExtendedInfo::Ty::Unknown));
		L.Push("?");
	}
	else {
		L.Push(static_cast<int>(inf->Type));
		L.Push(inf->Name);
	}
	return 2;
}

int CppLogic::Serializer::ListAccess::InsertAt(luaext::State L) {
	auto* th = L.CheckUserClass<ListAccess>(1);
	const auto& inf = th->SeriData->ListOptions->GetExtendedInfo();
	if (inf.InsertAt == nullptr)
		throw lua::LuaException{ "no insert at available" };
	struct Data {
		luaext::State L;
		ListAccess* A = nullptr;
	} d{ L, th };
	auto write = [](void* uv, const BB::SerializationData* sd, void* elem) {
		Data* d = static_cast<Data*>(uv);
		auto r = d->L.AutoCleanStack();
		d->L.PushValue(3);
		PushSD(d->L, sd->SerializationName, elem, sd, d->A->Id, d->A->OnWrite, true);
		d->L.TCall(1, 0);
	};
	bool l;
	if (L.IsNumber(2)) {
		l = inf.InsertAt(th->Object, L.Check<size_t>(2), write, &d, th->SeriData);
	}
	else {
		l = inf.InsertAt(th->Object, [](void* uv, const BB::SerializationData* sd, void* el) {
			Data* d = static_cast<Data*>(uv);
			int t = d->L.GetTop();
			d->L.PushValue(2);
			PushSD(d->L, sd->SerializationName, el, sd, d->A->Id, d->A->OnWrite, true);
			bool r = false;
			if (d->L.PCall(1, 1) == luaext::State::ErrorCode::Success)
				r = d->L.ToBoolean(-1);
			d->L.SetTop(t);
			return r;
		}, write, &d, th->SeriData);
	}
	L.Push(l);
	return 1;
}

int CppLogic::Serializer::ListAccess::Index(luaext::State L)
{
	auto* th = L.CheckUserClass<ListAccess>(1);
	const auto& inf = th->SeriData->ListOptions->GetExtendedInfo();
	if (inf.IndexNumeric != nullptr) {
		int i = L.CheckInt(2);
		void* obj = inf.IndexNumeric(th->Object, static_cast<size_t>(i));
		if (obj == nullptr)
			throw lua::LuaException{ "invalid index" };
		PushSD(L, th->SeriData->SerializationName, obj, th->SeriData, th->Id, th->OnWrite, true);
		return 1;
	}
	throw lua::LuaException{ "no index available" };
}

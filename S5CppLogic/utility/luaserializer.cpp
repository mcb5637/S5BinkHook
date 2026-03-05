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

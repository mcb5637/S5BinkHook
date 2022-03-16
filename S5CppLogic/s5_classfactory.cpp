#include "pch.h"
#include <map>
#include "s5_classfactory.h"
#include "s5_filesystem.h"

static inline BB::CXmlSerializer*(__stdcall* const xmlserializer_new)(int d) = reinterpret_cast<BB::CXmlSerializer*(__stdcall* const)(int)>(0x550731);
BB::CXmlSerializer* BB::CXmlSerializer::Create()
{
    return xmlserializer_new(0);
}
void BB::CXmlSerializer::Deserialize(const char* filename, BB::IObject* ob)
{
    BB::CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, 0x113)) {
        Deserialize(&filestr, ob);
        filestr.Close();
    }
}

void BB::CXmlSerializer::Serialize(const char* filename, BB::IObject* ob)
{
    BB::CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, 0x121)) {
        auto* s = BB::CXmlSerializer::Create();

        s->Serialize(&filestr, ob);

        s->Destroy();
        filestr.Close();
    }
}

void PushUnknownValue(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    char buff[201]{};
    fs->SerializeToString(buff, 200, data);
    L.Push(buff);
}
void CheckUnknownValue(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    const char* s = L.CheckString(idx);
    fs->DeserializeFromString(data, s);
}
BB::FieldSerilaizer::ExtendedInfo InfoUnknown{ "Unknown Type", &PushUnknownValue, &CheckUnknownValue };


void PushInt(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(*static_cast<int*>(data));
}
void CheckInt(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<int*>(data) = L.CheckInt(idx);
}
BB::FieldSerilaizer::ExtendedInfo InfoInt{ "Int", &PushInt, &CheckInt };
BB::FieldSerilaizer::ExtendedInfo InfoTasklist{ "shok::TaskListID", &PushInt, &CheckInt };
BB::FieldSerilaizer::ExtendedInfo InfoEntityType{ "shok::EntityTypeID", &PushInt, &CheckInt };
BB::FieldSerilaizer::ExtendedInfo InfoModel{ "shok::ModelID", &PushInt, &CheckInt };

void PushUInt(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(static_cast<double>(*static_cast<unsigned int*>(data)));
}
void CheckUInt(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<unsigned int*>(data) = static_cast<unsigned int>(L.CheckNumber(idx));
}
BB::FieldSerilaizer::ExtendedInfo InfoUInt{ "unsigned Int", &PushUInt, &CheckUInt };

void PushBool(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(*static_cast<bool*>(data));
}
void CheckBool(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<bool*>(data) = L.CheckBool(idx);
}
BB::FieldSerilaizer::ExtendedInfo InfoBool{ "Bool", &PushBool, &CheckBool };

void PushFloat(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(*static_cast<float*>(data));
}
void CheckFloat(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<float*>(data) = L.CheckFloat(idx);
}
BB::FieldSerilaizer::ExtendedInfo InfoFloat{ "Float", &PushFloat, &CheckFloat };

void PushSString(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(static_cast<shok::String*>(data)->c_str());
}
void CheckSString(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    static_cast<shok::String*>(data)->assign(L.CheckString(idx));
}
BB::FieldSerilaizer::ExtendedInfo InfoString{ typename_details::type_name<shok::String>(), &PushSString, &CheckSString };

void PushCharBuff(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(*static_cast<const char**>(data));
}
BB::FieldSerilaizer::ExtendedInfo InfoCharBuff{ "char*" , &PushCharBuff, &CheckUnknownValue }; // let the original xml variant alloc the mem


BB::FieldSerilaizer::ExtendedInfo InfoClassID{ "shok::ClassIdentifier", &PushUnknownValue, &CheckUnknownValue };

const std::map<int, const BB::FieldSerilaizer::ExtendedInfo*> KnownSerializers{ {
    {0x810C58, &InfoBool},

    {0x810C98, &InfoInt},
    {0x800108, &InfoInt},

    {0x85D4AC, &InfoTasklist},
    {0x85D4D0, &InfoEntityType},
    {0x8585C0, &InfoModel},

    {0x810CD8, &InfoUInt},

    {0x810C78, &InfoFloat},

    {0x8640F0, &InfoString},
    {0x894954, &InfoString},
    {0x810CB8, &InfoCharBuff},

    {0x813978, &InfoClassID},
} };

std::string BB::FieldSerilaizer::GetTypeDescName() const
{
    int p = reinterpret_cast<int>(this);

    auto it = KnownSerializers.find(p);
    if (it != KnownSerializers.end()) {
        return it->second->Name;
    }

    char b[50]{};
    std::snprintf(b, 50, "unknown type 0x%x", p);
    return b;
}

const BB::FieldSerilaizer::ExtendedInfo& BB::FieldSerilaizer::GetExtendedInfo() const
{
    int p = reinterpret_cast<int>(this);

    auto it = KnownSerializers.find(p);
    if (it != KnownSerializers.end()) {
        return *it->second;
    }

    return InfoUnknown;
}

const BB::SerializationData* BB::SerializationData::GetSerializationData(unsigned int id)
{
    return (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
}

unsigned int __stdcall BB::SerializationData::GetBBIdentifier(void* d)
{
    return static_cast<BB::IObject*>(d)->GetClassIdentifier();
}

void BB::CClassFactory::LoadObject(BB::IObject* ob, const char* filename)
{
    BB::CXmlSerializer* s = BB::CXmlSerializer::Create();
    s->Deserialize(filename, ob);
    s->Destroy();
}

#include "pch.h"
#include <map>
#include "s5_classfactory.h"
#include "s5_filesystem.h"
#include "s5_entity.h"
#include "s5_idmanager.h"
#include "s5_mapdisplay.h"

static inline BB::CXmlSerializer*(__stdcall* const xmlserializer_new)(int d) = reinterpret_cast<BB::CXmlSerializer*(__stdcall* const)(int)>(0x550731);
BB::CXmlSerializer* BB::CXmlSerializer::Create()
{
    return xmlserializer_new(0);
}
std::unique_ptr<BB::CXmlSerializer, CppLogic::DestroyCaller<BB::CXmlSerializer>> BB::CXmlSerializer::CreateUnique()
{
    return std::unique_ptr<CXmlSerializer, CppLogic::DestroyCaller<CXmlSerializer>>(Create());
}
void BB::CXmlSerializer::Deserialize(const char* filename, BB::IObject* ob)
{
    BB::CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, BB::CFileStreamEx::Flags::DefaultRead)) {
        Deserialize(&filestr, ob);
        filestr.Close();
    }
}

void BB::CXmlSerializer::Deserialize(const char* filename, void* ob, const BB::SerializationData* d)
{
    BB::CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, BB::CFileStreamEx::Flags::DefaultRead)) {
        DeserializeByData(&filestr, ob, d);
        filestr.Close();
    }
}

void BB::CXmlSerializer::Serialize(const char* filename, BB::IObject* ob)
{
    BB::CFileStreamEx filestr{};
    if (filestr.OpenFile(filename, BB::CFileStreamEx::Flags::DefaultWrite)) {
        auto s = BB::CXmlSerializer::CreateUnique();

        s->Serialize(&filestr, ob);

        filestr.Close();
    }
}

inline BB::CBinarySerializer* (__stdcall* const binaryseri_create)(int z, int uk) = reinterpret_cast<BB::CBinarySerializer * (__stdcall*)(int, int)>(0x54C272);
BB::CBinarySerializer* BB::CBinarySerializer::Create(int uk)
{
    return binaryseri_create(0, uk);
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

template<class En>
void CheckEnum(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<En*>(data) = luaext::EState{ L }.CheckEnum<En>(idx, true);
}

BB::FieldSerilaizer::ExtendedInfo InfoTasklist{ "shok::TaskListId", &PushInt, &CheckEnum<shok::TaskListId> };
BB::FieldSerilaizer::ExtendedInfo InfoEntityType{ "shok::ModelId", &PushInt, &CheckEnum<shok::ModelId> };
BB::FieldSerilaizer::ExtendedInfo InfoModel{ "shok::ModelId", &PushInt, &CheckEnum<shok::ModelId> };
BB::FieldSerilaizer::ExtendedInfo InfoAnim{ "shok::AnimationId", &PushInt, &CheckEnum<shok::AnimationId> };
BB::FieldSerilaizer::ExtendedInfo InfoDamageClass{ "shok::DamageClassId", &PushInt, &CheckEnum<shok::DamageClassId> };
BB::FieldSerilaizer::ExtendedInfo InfoEffectType{ "shok::EffectTypeId", &PushInt, &CheckEnum<shok::EffectTypeId> };
BB::FieldSerilaizer::ExtendedInfo InfoResourceType{ "shok::ResourceType", &PushInt, &CheckEnum<shok::ResourceType> };
BB::FieldSerilaizer::ExtendedInfo InfoUpgradeCategory{ "shok::UpgradeCategoryId", &PushInt, &CheckEnum<shok::UpgradeCategoryId> };
BB::FieldSerilaizer::ExtendedInfo InfoTechnologyID{ "shok::TechnologyId", &PushInt, &CheckEnum<shok::TechnologyId> };
BB::FieldSerilaizer::ExtendedInfo InfoSoundID{ "shok::SoundId", &PushInt, &CheckEnum<shok::SoundId> };
BB::FieldSerilaizer::ExtendedInfo InfoAmbientSoundID{ "shok::AmbientSoundId", &PushInt, &CheckEnum<shok::AmbientSoundId> };
BB::FieldSerilaizer::ExtendedInfo InfoWeatherEffectTextureID{ "shok::WeatherEffectTextureId", &PushInt, &CheckEnum<shok::WeatherEffectTextureId> };
BB::FieldSerilaizer::ExtendedInfo InfoTerrainTextureTextureID{ "shok::TerrainTextureId", &PushInt, &CheckEnum<shok::TerrainTextureId> };
BB::FieldSerilaizer::ExtendedInfo InfoEntityCategory{ "shok::EntityCategory", &PushInt, &CheckEnum<shok::EntityCategory> };
BB::FieldSerilaizer::ExtendedInfo InfoAccessCategory{ "shok::AccessCategory", &PushInt, &CheckInt };
BB::FieldSerilaizer::ExtendedInfo InfoAnimCategory{ "shok::AnimationCategoryId", &PushInt, &CheckEnum<shok::AnimationCategoryId> };
BB::FieldSerilaizer::ExtendedInfo InfoGoods{ "shok::Goods", &PushInt, &CheckEnum<shok::Goods> };
BB::FieldSerilaizer::ExtendedInfo InfoWidgetID{ "shok::WidgetId", &PushInt, &CheckEnum<shok::WidgetId> };
BB::FieldSerilaizer::ExtendedInfo InfoFeedbackEventId{ "shok::FeedbackEventIds", &PushInt, &CheckEnum<shok::FeedbackEventIds> };
BB::FieldSerilaizer::ExtendedInfo InfoFeedbackStateId{ "shok::FeedbackStateId", &PushInt, &CheckEnum<shok::FeedbackStateId> };
BB::FieldSerilaizer::ExtendedInfo InfoAbilityId{ "shok::AbilityId", &PushInt, &CheckEnum<shok::AbilityId> };
BB::FieldSerilaizer::ExtendedInfo InfoArmorClassId{ "shok::ArmorClassId", &PushInt, &CheckEnum<shok::ArmorClassId> };

void PushUInt(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(static_cast<double>(*static_cast<unsigned int*>(data)));
}
void CheckUInt(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<unsigned int*>(data) = static_cast<unsigned int>(L.CheckNumber(idx));
}
BB::FieldSerilaizer::ExtendedInfo InfoUInt{ "unsigned Int", &PushUInt, &CheckUInt };

void PushUByte(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(static_cast<double>(*static_cast<byte*>(data)));
}
void CheckUByte(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<unsigned int*>(data) = static_cast<byte>(L.CheckNumber(idx));
}
BB::FieldSerilaizer::ExtendedInfo InfoUByte{ "unsigned Byte", &PushUByte, &CheckUByte };

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

void PushDouble(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(*static_cast<double*>(data));
}
void CheckDouble(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<double*>(data) = L.CheckNumber(idx);
}
BB::FieldSerilaizer::ExtendedInfo InfoDouble{ "Double", &PushDouble, &CheckDouble };

void PushSString(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(static_cast<shok::String*>(data)->c_str());
}
void CheckSString(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    static_cast<shok::String*>(data)->assign(L.CheckString(idx));
}
BB::FieldSerilaizer::ExtendedInfo InfoString{ typename_details::type_name<shok::String>(), &PushSString, &CheckSString};

void PushCharBuff(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(*static_cast<const char**>(data));
}
BB::FieldSerilaizer::ExtendedInfo InfoCharBuff{ "char*" , &PushCharBuff, &CheckUnknownValue }; // let the original xml variant alloc the mem


void PushEntId(lua::State L, void* data, const BB::FieldSerilaizer* fs) {
    L.Push(static_cast<int>((*static_cast<EGL::CGLEEntity**>(data))->EntityId));
}
void CheckEntId(lua::State L, void* data, int idx, const BB::FieldSerilaizer* fs) {
    *static_cast<EGL::CGLEEntity**>(data) = luaext::EState{ L }.CheckEntity(idx);
}
// only to be used after entities are loaded
BB::FieldSerilaizer::ExtendedInfo EntitySerializedById{ "EGL::CGLEEntity (by Id)", &PushEntId, &CheckEntId };

BB::FieldSerilaizer::ExtendedInfo InfoClassID{ "shok::ClassIdentifier", &PushUnknownValue, &CheckUnknownValue };

const std::map<int, const BB::FieldSerilaizer::ExtendedInfo*> KnownSerializers{ {
    {0x810C58, &InfoBool},
    {0x82BAF4, &InfoBool},
    {0x822334, &InfoBool},
    {0x84D164, &InfoBool},
    {0x84CC44, &InfoBool},
    {0x84CCE8, &InfoBool},
    {0x81182C, &InfoBool},
    {0x83BA08, &InfoBool},
    {0x83CDC8, &InfoBool},
    {0x83CCF8, &InfoBool},
    {0x80DC80, &InfoBool},
    {0x839AB0, &InfoBool},
    {0x83578c, &InfoBool},
    {0x82F6E0, &InfoBool},

    {0x810C98, &InfoInt},
    {0x800108, &InfoInt},
    {0x8231C4, &InfoInt},
    {0x82BB60, &InfoInt},
    {0x812688, &InfoInt},
    {0x822374, &InfoInt},
    {0x8236C0, &InfoInt},
    {0x828A2C, &InfoInt},
    {0x829F34, &InfoInt},
    {0x80AE58, &InfoInt},
    {0x84CD28, &InfoInt},
    {0x83BA50, &InfoInt},
    {0x813408, &InfoInt},
    {0x813514, &InfoInt},
    {0x83CE08, &InfoInt},
    {0x83CD38, &InfoInt},
    {0x80DCE8, &InfoInt},
    {0x7FF498, &InfoInt},
    {0x823ca8, &InfoInt},
    {0x829b14, &InfoInt},
    {0x8395e8, &InfoInt},
    {0x83acb0, &InfoInt},
    {0x8394b8, &InfoInt},
    {0x837254, &InfoInt},
    {0x82CAE8, &InfoInt},
    {0x82D348, &InfoInt},
    {0x82F700, &InfoInt},
    {0x839f30, &InfoInt},
    {0x828fb0, &InfoInt},

    {0x85D4AC, &InfoTasklist},
    {0x85D4D0, &InfoEntityType},
    {0x8585C0, &InfoModel},
    {0x86057C, &InfoAnim},
    {0xA0C868, &InfoAnim},
    {0x860558, &InfoDamageClass},
    {0x8605A0, &InfoEffectType},
    {0x860510, &InfoResourceType},
    {0x86414C, &InfoUpgradeCategory},
    {0x85D53C, &InfoUpgradeCategory},
    {0x85D560, &InfoTechnologyID},
    {0x85A108, &InfoSoundID},
    {0x85A12C, &InfoAmbientSoundID},
    {0x8587D4, &InfoWeatherEffectTextureID},
    {0xA0D634, &InfoTerrainTextureTextureID},
    {0x85D4F4, &InfoEntityCategory},
    {0x875494, &InfoAnimCategory},
    {0x8754B8, &InfoGoods},
    {0x8683e4, &InfoGoods},
    {0x894708, &InfoWidgetID},
    {0x880494, &InfoFeedbackEventId},
    {0x8804B8, &InfoFeedbackStateId},
    {0x883088, &InfoAbilityId},
    {0x85D5CC, &InfoArmorClassId},

    {0x810D18, &InfoAccessCategory},

    {reinterpret_cast<int(__cdecl*)()>(0x58A92E)(), &EntitySerializedById},

    {0x810CD8, &InfoUInt},
    {0x80AD90, &InfoUInt},
    {0x84CD48, &InfoUInt},
    {0x80DE60, &InfoUInt},
    {0x83CE48, &InfoUInt},
    {0x839628, &InfoUInt},
    {0x83acd0, &InfoUInt},
    {0x8394f8, &InfoUInt},
    {0x839f70, &InfoUInt},

    {0x80C380, &InfoUByte},
    {0x84DD48, &InfoUByte},

    {0x810C78, &InfoFloat},
    {0x8231A4, &InfoFloat},
    {0x82BB14, &InfoFloat},
    {0x812668, &InfoFloat},
    {0x822354, &InfoFloat},
    {0x8236A0, &InfoFloat},
    {0x829F14, &InfoFloat},
    {0x80AD70, &InfoFloat},
    {0x80AE38, &InfoFloat},
    {0x84CC64, &InfoFloat},
    {0x84CD08, &InfoFloat},
    {0x80A994, &InfoFloat},
    {0x8133E8, &InfoFloat},
    {0x829844, &InfoFloat},
    {0x83CDE8, &InfoFloat},
    {0x83CD18, &InfoFloat},
    {0x80DCA0, &InfoFloat},
    {0x83B65C, &InfoFloat},
    {0x829AF4, &InfoFloat},
    {0x82D62C, &InfoFloat},
    {0x82D328, &InfoFloat},
    {0x82CAC8, &InfoFloat},
    {0x83CBFC, &InfoFloat},
    {0x828f90, &InfoFloat},
    {0x8171f0, &InfoFloat},

    {0x82BB38, &InfoDouble},
    {0x8000E0, &InfoDouble},
    {0x83BA28, &InfoDouble},

    {0x8640F0, &InfoString},
    {0x894954, &InfoString},
    {0xA1A588, &InfoString},
    {0xA0654C, &InfoString},
    {0x87CEF8, &InfoString},
    {0xA0D9CC, &InfoString},
    {0xA0D5D4, &InfoString},
    {0x84E9B4, &InfoString},
    {0xA06454, &InfoString},
    {0x810CB8, &InfoCharBuff},
    {0x84CC84, &InfoCharBuff},
    {0x83CE28, &InfoCharBuff},
    {0x83CD58, &InfoCharBuff},

    {0x813978, &InfoClassID},
    {0x8223B4, &InfoClassID},
    {0x80ADB0, &InfoClassID},
    {0x810CF8, &InfoClassID},
} };

std::string BB::FieldSerilaizer::GetTypeDescName() const
{
    int p = reinterpret_cast<int>(this);

    auto it = KnownSerializers.find(p);
    if (it != KnownSerializers.end()) {
        return std::string{ it->second->Name };
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

inline const char* (__stdcall* const serilistctx_get)(const BB::SerializationListOptions::Context* c, const char* a) = reinterpret_cast<const char* (__stdcall*)(const BB::SerializationListOptions::Context*, const char*)>(0x554600);
const char* BB::SerializationListOptions::Context::GetAttribute(const char* a) const
{
    return serilistctx_get(this, a);
}

const BB::SerializationData* BB::SerializationData::GetSerializationData(shok::ClassId id)
{
    return (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(id);
}

shok::ClassId __stdcall BB::SerializationData::GetBBIdentifier(void* d)
{
    return static_cast<BB::IObject*>(d)->GetClassIdentifier();
}

void BB::CClassFactory::LoadObject(BB::IObject* ob, const char* filename)
{
    auto s = BB::CXmlSerializer::CreateUnique();
    s->Deserialize(filename, ob);
}

void BB::CClassFactory::LoadObject(void* ob, const char* filename, const BB::SerializationData* seri)
{
    auto s = BB::CXmlSerializer::Create();
    s->Deserialize(filename, ob, seri);
}

void __stdcall CppLogic::StringSerializer::DeserializeFromStringImp(void* data, const char* buff)
{
    static_cast<std::string*>(data)->assign(buff);
}

int __stdcall CppLogic::StringSerializer::SerializeToStringImp(char* buff, size_t s, const void* data)
{
    const auto* d = static_cast<const std::string*>(data);
    std::memcpy(buff, d->c_str(), std::min(s, d->size()));
    return d->size();
}

void __stdcall CppLogic::StringSerializer::DeserializeFromStreamImp(void* data, BB::IStream* str)
{
    auto* d = static_cast<std::string*>(data);
    size_t s = 0;
    str->Read(&s, sizeof(s));
    char* buff = new char[s];
    str->Read(buff, s);
    d->assign(buff, s);
    delete[] buff;
}

void __stdcall CppLogic::StringSerializer::SerializeToStreamImp(BB::IStream* str, const void* data)
{
    const auto* d = static_cast<const std::string*>(data);
    size_t s = d->size();
    str->Write(&s, sizeof(s));
    str->Write(d->c_str(), s);
}

CppLogic::StringSerializer::StringSerializer()
{
    DeserializeFromString = &DeserializeFromStringImp;
    SerializeToString = &SerializeToStringImp;
    DeserializeFromStream = &DeserializeFromStreamImp;
    SerializeToStream = &SerializeToStreamImp;
    Buffer = &ActualBuffer;
}

CppLogic::StringSerializer CppLogic::StringSerializer::GlobalObj{};

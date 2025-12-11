#include "pch.h"
#include <map>
#include "s5_classfactory.h"
#include <shok/globals/s5_filesystem.h>
#include <shok/entity/s5_entity.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/ui/s5_widget.h>
#include <shok/entitytype/s5_behaviorProps.h>
#include <utility/SchemaGenerator.h>
#include <utility/hooks.h>

static inline BB::CXmlSerializer* (__stdcall* const xmlserializer_new)(int d) = reinterpret_cast<BB::CXmlSerializer * (__stdcall* const)(int)>(0x550731);
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

void BB::CXmlSerializer::WriteTypeASM() {
	__asm {
		mov eax, 0x54FA1E;
		call eax;

		push[ebp + 0x10];
		push esi;
		call BB::CXmlSerializer::WriteType;

		push 0x54FC5D;
		ret;
	}
}

void BB::CXmlSerializer::HookWriteXSIType()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x54FC58), 0x54FC5D - 0x54FC58 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x54FC58), &WriteTypeASM, reinterpret_cast<void*>(0x54FC5D));
}

void __stdcall BB::CXmlSerializer::WriteType(BB::IStream* f, const char* cn)
{
	f->Write(" xsi:type=\"");
	auto c = CppLogic::Serializer::SchemaGenerator::EscapeClassname(cn);
	f->Write(c);
	f->Write("\"");
}

inline BB::CBinarySerializer* (__stdcall* const binaryseri_create)(int z, int uk) = reinterpret_cast<BB::CBinarySerializer * (__stdcall*)(int, int)>(0x54C272);
BB::CBinarySerializer* BB::CBinarySerializer::Create(int uk, int uk2)
{
	return binaryseri_create(uk, uk2);
}
std::unique_ptr<BB::CBinarySerializer, CppLogic::DestroyCaller<BB::CBinarySerializer>> BB::CBinarySerializer::CreateUnique(int uk, int uk2)
{
	return std::unique_ptr<CBinarySerializer, CppLogic::DestroyCaller<CBinarySerializer>>(Create(uk, uk2));
}

void PushUnknownValue(lua::State L, void* data, const BB::FieldSerializer* fs) {
	char buff[201]{};
	fs->SerializeToString(buff, 200, data);
	L.Push(buff);
}
void CheckUnknownValue(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	const char* s = L.CheckString(idx);
	fs->DeserializeFromString(data, s);
}
BB::FieldSerializer::ExtendedInfo InfoUnknown{ "Unknown Type", &PushUnknownValue, &CheckUnknownValue };


void PushInt(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(*static_cast<int*>(data));
}
void CheckInt(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<int*>(data) = L.CheckInt(idx);
}
BB::FieldSerializer::ExtendedInfo InfoInt{ "Int", &PushInt, &CheckInt, "xs:int" };

template<class En>
void CheckEnum(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<En*>(data) = luaext::EState{ L }.CheckEnum<En>(idx, true);
}

BB::FieldSerializer::ExtendedInfo InfoTasklist{ "shok::TaskListId", &PushInt, &CheckEnum<shok::TaskListId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoTask{ "shok::Task", &PushInt, &CheckEnum<shok::Task>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoEntityType{ "shok::EntityTypeId", &PushInt, &CheckEnum<shok::EntityTypeId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoModel{ "shok::ModelId", &PushInt, &CheckEnum<shok::ModelId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoAnim{ "shok::AnimationId", &PushInt, &CheckEnum<shok::AnimationId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoDamageClass{ "shok::DamageClassId", &PushInt, &CheckEnum<shok::DamageClassId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoEffectType{ "shok::EffectTypeId", &PushInt, &CheckEnum<shok::EffectTypeId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoResourceType{ "shok::ResourceType", &PushInt, &CheckEnum<shok::ResourceType>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoUpgradeCategory{ "shok::UpgradeCategoryId", &PushInt, &CheckEnum<shok::UpgradeCategoryId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoTechnologyID{ "shok::TechnologyId", &PushInt, &CheckEnum<shok::TechnologyId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoSoundID{ "shok::SoundId", &PushInt, &CheckEnum<shok::SoundId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoAmbientSoundID{ "shok::AmbientSoundId", &PushInt, &CheckEnum<shok::AmbientSoundId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoWeatherEffectTextureID{ "shok::WeatherEffectTextureId", &PushInt, &CheckEnum<shok::WeatherEffectTextureId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoTerrainTextureTextureID{ "shok::TerrainTextureId", &PushInt, &CheckEnum<shok::TerrainTextureId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoEntityCategory{ "shok::EntityCategory", &PushInt, &CheckEnum<shok::EntityCategory>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoAccessCategory{ "shok::AccessCategory", &PushInt, &CheckInt, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoAnimCategory{ "shok::AnimationCategoryId", &PushInt, &CheckEnum<shok::AnimationCategoryId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoGoods{ "shok::Goods", &PushInt, &CheckEnum<shok::Goods>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoWidgetID{ "shok::WidgetId", &PushInt, &CheckEnum<shok::WidgetId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoFeedbackEventId{ "shok::FeedbackEventIds", &PushInt, &CheckEnum<shok::FeedbackEventIds>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoFeedbackStateId{ "shok::FeedbackStateId", &PushInt, &CheckEnum<shok::FeedbackStateId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoAbilityId{ "shok::AbilityId", &PushInt, &CheckEnum<shok::AbilityId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoArmorClassId{ "shok::ArmorClassId", &PushInt, &CheckEnum<shok::ArmorClassId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoTerrainTypeId{ "shok::TerrainTypeId", &PushInt, &CheckEnum<shok::TerrainTypeId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoBuildingBanner{ "shok::BuildingBannerGroupId", &PushInt, &CheckEnum<shok::BuildingBannerGroupId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoBlessCategoryId{ "shok::BlessCategoryId", &PushInt, &CheckEnum<shok::BlessCategoryId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoAttachmentType{ "shok::AttachmentType", &PushInt, &CheckEnum<shok::AttachmentType>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoAnimSetId{ "shok::AnimSetId", &PushInt, &CheckEnum<shok::AnimSetId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoWidgetGroupId{ "shok::WidgetGroupId", &PushInt, &CheckEnum<shok::WidgetGroupId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoGUITextureId{ "shok::GUITextureId", &PushInt, &CheckEnum<shok::GUITextureId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoFontId{ "shok::FontId", &PushInt, &CheckEnum<shok::FontId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoPrincipalTaskId{ "shok::PrincipalTaskId", &PushInt, &CheckEnum<shok::PrincipalTaskId>, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoRaceId{ "shok::RaceId", &PushInt, &CheckInt, "xs:string" };
BB::FieldSerializer::ExtendedInfo InfoTechnologyCategoryId{ "shok::TechnologyCategoryId", &PushInt, &CheckEnum<shok::TechnologyCategoryId>, "xs:string" };

void PushUInt(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(static_cast<double>(*static_cast<unsigned int*>(data)));
}
void CheckUInt(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<unsigned int*>(data) = static_cast<unsigned int>(L.CheckNumber(idx));
}
BB::FieldSerializer::ExtendedInfo InfoUInt{ "unsigned Int", &PushUInt, &CheckUInt, "xs:unsignedInt" };

void PushUByte(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(static_cast<double>(*static_cast<byte*>(data)));
}
void CheckUByte(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<unsigned int*>(data) = static_cast<byte>(L.CheckNumber(idx));
}
BB::FieldSerializer::ExtendedInfo InfoUByte{ "unsigned Byte", &PushUByte, &CheckUByte, "xs:unsignedByte" };

void PushBool(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(*static_cast<bool*>(data));
}
void CheckBool(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<bool*>(data) = L.CheckBool(idx);
}
BB::FieldSerializer::ExtendedInfo InfoBool{ "Bool", &PushBool, &CheckBool, "xs:boolean" };

void PushFloat(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(*static_cast<float*>(data));
}
void CheckFloat(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<float*>(data) = L.CheckFloat(idx);
}
BB::FieldSerializer::ExtendedInfo InfoFloat{ "Float", &PushFloat, &CheckFloat, "xs:float" };

void PushDouble(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(*static_cast<double*>(data));
}
void CheckDouble(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<double*>(data) = L.CheckNumber(idx);
}
BB::FieldSerializer::ExtendedInfo InfoDouble{ "Double", &PushDouble, &CheckDouble, "xs:double" };

void PushSString(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(static_cast<shok::String*>(data)->c_str());
}
void CheckSString(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	static_cast<shok::String*>(data)->assign(L.CheckString(idx));
}
BB::FieldSerializer::ExtendedInfo InfoString{ typename_details::type_name<shok::String>(), &PushSString, &CheckSString, "xs:string" };

void PushCharBuff(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(*static_cast<const char**>(data));
}
BB::FieldSerializer::ExtendedInfo InfoCharBuff{ "char*" , &PushCharBuff, &CheckUnknownValue, "xs:string" }; // let the original xml variant alloc the mem

void PushStdString(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(static_cast<std::string*>(data)->c_str());
}
void CheckStdString(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	static_cast<std::string*>(data)->assign(L.CheckString(idx));
}
BB::FieldSerializer::ExtendedInfo InfoStdString{ typename_details::type_name<std::string>(), &PushStdString, &CheckStdString, "xs:string" };

void PushEntId(lua::State L, void* data, const BB::FieldSerializer* fs) {
	L.Push(static_cast<int>((*static_cast<EGL::CGLEEntity**>(data))->EntityId));
}
void CheckEntId(lua::State L, void* data, int idx, const BB::FieldSerializer* fs) {
	*static_cast<EGL::CGLEEntity**>(data) = luaext::EState{ L }.CheckEntity(idx);
}
// only to be used after entities are loaded
BB::FieldSerializer::ExtendedInfo EntitySerializedById{ "EGL::CGLEEntity (by Id)", &PushEntId, &CheckEntId, "xs:int" };

BB::FieldSerializer::ExtendedInfo InfoClassID{ "shok::ClassIdentifier", &PushUnknownValue, &CheckUnknownValue, "xs:string" };

BB::FieldSerializer::ExtendedInfo InfoLowResTerrainNode{ "EGL::CGLETerrainLowRes::TerrainNode (no binary)", &PushUnknownValue, &CheckUnknownValue };
BB::FieldSerializer::ExtendedInfo InfoHiResTerrainNode{ "EGL::CGLETerrainHiRes::TerrainNode (no binary)", &PushUnknownValue, &CheckUnknownValue };
BB::FieldSerializer::ExtendedInfo InfoIntVector{ "shok::Vector<int>", &PushUnknownValue, &CheckUnknownValue };
BB::FieldSerializer::ExtendedInfo InfoFloatRad{ "float (rad serialized as deg)", &PushFloat, &CheckFloat };

const std::map<int, const BB::FieldSerializer::ExtendedInfo*> KnownSerializers{ {
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
	{0x8267a4, &InfoBool},
	{0x8112dc, &InfoBool},
	{0x812a38, &InfoBool},
	{0x8132f8, &InfoBool},
	{0x815de8, &InfoBool},
	{0x8163f4, &InfoBool},
	{0x817088, &InfoBool},
	{0x818eb4, &InfoBool},
	{0x81b6e0, &InfoBool},
	{0x81bae4, &InfoBool},
	{0x81bd3c, &InfoBool},
	{0x81d744, &InfoBool},
	{0x81efa4, &InfoBool},
	{0x81f5fc, &InfoBool},
	{0x81f974, &InfoBool},
	{0x8203b8, &InfoBool},
	{0x820de8, &InfoBool},
	{0x821e48, &InfoBool},
	{0x822a88, &InfoBool},
	{0x8234bc, &InfoBool},
	{0x8257bc, &InfoBool},
	{0x82783c, &InfoBool},
	{0x827fa8, &InfoBool},
	{0x8289ec, &InfoBool},
	{0x829178, &InfoBool},
	{0x829460, &InfoBool},
	{0x829ef4, &InfoBool},
	{0x8327c8, &InfoBool},
	{0x832940, &InfoBool},
	{0x833ce0, &InfoBool},
	{0x833ff8, &InfoBool},
	{0x834170, &InfoBool},
	{0x8342e0, &InfoBool},
	{0x834580, &InfoBool},
	{0x836ae0, &InfoBool},
	{0x83ae78, &InfoBool},
	{0x83c940, &InfoBool},
	{0x856e90, &InfoBool},
	{0x82b9a0, &InfoBool},
	{0x811e8c, &InfoBool},
	{0x82a8d0, &InfoBool},

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
	{0x825e74, &InfoInt},
	{0x8267e4, &InfoInt},
	{0x80ac9c, &InfoInt},
	{0x810e78, &InfoInt},
	{0x81131c, &InfoInt},
	{0x811514, &InfoInt},
	{0x81186c, &InfoInt},
	{0x812a78, &InfoInt},
	{0x813338, &InfoInt},
	{0x813938, &InfoInt},
	{0x815e28, &InfoInt},
	{0x816434, &InfoInt},
	{0x8170c8, &InfoInt},
	{0x81737c, &InfoInt},
	{0x817b24, &InfoInt},
	{0x818ef4, &InfoInt},
	{0x819268, &InfoInt},
	{0x817f64, &InfoInt},
	{0x81959c, &InfoInt},
	{0x81a430, &InfoInt},
	{0x81ace8, &InfoInt},
	{0x81b3ac, &InfoInt},
	{0x81bb24, &InfoInt},
	{0x81c3b8, &InfoInt},
	{0x81c58c, &InfoInt},
	{0x81c81c, &InfoInt},
	{0x81d020, &InfoInt},
	{0x81d268, &InfoInt},
	{0x81d784, &InfoInt},
	{0x81efe4, &InfoInt},
	{0x81f9b4, &InfoInt},
	{0x81fb0c, &InfoInt},
	{0x81fdf8, &InfoInt},
	{0x8203f8, &InfoInt},
	{0x8209ec, &InfoInt},
	{0x820e28, &InfoInt},
	{0x8214e0, &InfoInt},
	{0x821e88, &InfoInt},
	{0x8220c0, &InfoInt},
	{0x822680, &InfoInt},
	{0x822ac8, &InfoInt},
	{0x822e44, &InfoInt},
	{0x822f98, &InfoInt},
	{0x8240f4, &InfoInt},
	{0x824ae8, &InfoInt},
	{0x824e98, &InfoInt},
	{0x825940, &InfoInt},
	{0x826c1c, &InfoInt},
	{0x827fe8, &InfoInt},
	{0x828908, &InfoInt},
	{0x8291b8, &InfoInt},
	{0x8294a0, &InfoInt},
	{0x829cb4, &InfoInt},
	{0x8303e0, &InfoInt},
	{0x8318e8, &InfoInt},
	{0x832b18, &InfoInt},
	{0x8345e8, &InfoInt},
	{0x8348b0, &InfoInt},
	{0x835318, &InfoInt},
	{0x835e1c, &InfoInt},
	{0x836b20, &InfoInt},
	{0x83932c, &InfoInt},
	{0x83940c, &InfoInt},
	{0x83ab08, &InfoInt},
	{0x83abec, &InfoInt},
	{0x83ac70, &InfoInt},
	{0x83aeb8, &InfoInt},
	{0x84d3a8, &InfoInt},
	{0x855708, &InfoInt},
	{0x856e70, &InfoInt},
	{0x857128, &InfoInt},
	{0x82ba08, &InfoInt},
	{0x811ecc, &InfoInt},
	{0x82a938, &InfoInt},

	{0x85D4AC, &InfoTasklist},
	{0x86cb50, &InfoTasklist},
	{0x868370, &InfoTask},
	{0x85D4D0, &InfoEntityType},
	{0x85f258, &InfoEntityType},
	{0x8585C0, &InfoModel},
	{0x85d5f0, &InfoModel},
	{0x86057C, &InfoAnim},
	{0xA0C868, &InfoAnim},
	{0x860558, &InfoDamageClass},
	{0x85f27c, &InfoDamageClass},
	{0x8605A0, &InfoEffectType},
	{0x860510, &InfoResourceType},
	{0x86414C, &InfoUpgradeCategory},
	{0x85D53C, &InfoUpgradeCategory},
	{0x85D560, &InfoTechnologyID},
	{0x85A108, &InfoSoundID},
	{0x85A12C, &InfoAmbientSoundID},
	{0x876a08, &InfoAmbientSoundID},
	{0x8587D4, &InfoWeatherEffectTextureID},
	{0xA0D634, &InfoTerrainTextureTextureID},
	{0x85D4F4, &InfoEntityCategory},
	{0x875494, &InfoAnimCategory},
	{0x874fe8, &InfoAnimCategory},
	{0x8754B8, &InfoGoods},
	{0x8683e4, &InfoGoods},
	{0x894708, &InfoWidgetID},
	{0x880494, &InfoFeedbackEventId},
	{0x8804B8, &InfoFeedbackStateId},
	{0x883088, &InfoAbilityId},
	{0x85D5CC, &InfoArmorClassId},
	{0x8792e0, &InfoTerrainTypeId},
	{0x8586c0, &InfoBuildingBanner},
	{0x860534, &InfoBlessCategoryId},
	{0x868408, &InfoAttachmentType},
	{0x87500c, &InfoAnimSetId},
	{0x875030, &InfoAnimSetId},
	{0x89472c, &InfoWidgetGroupId},
	{0x894b48, &InfoGUITextureId},
	{0x89518c, &InfoFontId},
	{0x898214, &InfoPrincipalTaskId},
	{0x85D518, &InfoRaceId},
	{0x864128, &InfoTechnologyCategoryId},
	//0x85d584, 0x85d5a8 something head related

	{0x810D18, &InfoAccessCategory},

	{0x83938c, &InfoLowResTerrainNode},
	{0x83942c, &InfoLowResTerrainNode},

	{reinterpret_cast<int(__cdecl*)()>(0x58A92E)(), &EntitySerializedById},
	{0x867440, &InfoIntVector},
	{0xa02b10, &InfoIntVector},

	{0x810CD8, &InfoUInt},
	{0x80AD90, &InfoUInt},
	{0x84CD48, &InfoUInt},
	{0x80DE60, &InfoUInt},
	{0x83CE48, &InfoUInt},
	{0x839628, &InfoUInt},
	{0x83acd0, &InfoUInt},
	{0x8394f8, &InfoUInt},
	{0x839f70, &InfoUInt},
	{0x80b140, &InfoUInt},
	{0x80b250, &InfoUInt},
	{0x81188c, &InfoUInt},
	{0x811a20, &InfoUInt},
	{0x817250, &InfoUInt},
	{0x81739c, &InfoUInt},
	{0x817b64, &InfoUInt},
	{0x81a470, &InfoUInt},
	{0x81f024, &InfoUInt},
	{0x81f67c, &InfoUInt},
	{0x81f9f4, &InfoUInt},
	{0x822394, &InfoUInt},
	{0x825590, &InfoUInt},
	{0x836b60, &InfoUInt},
	{0x83ab48, &InfoUInt},
	{0x83ac0c, &InfoUInt},
	{0x83ac90, &InfoUInt},

	{0x80C380, &InfoUByte},
	{0x84DD48, &InfoUByte},
	{0x8557cc, &InfoUByte},

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
	{0x825e54, &InfoFloat},
	{0x800c88, &InfoFloat},
	{0x8112fc, &InfoFloat},
	{0x8114f4, &InfoFloat},
	{0x81184c, &InfoFloat},
	{0x8119c0, &InfoFloat},
	{0x813318, &InfoFloat},
	{0x815e08, &InfoFloat},
	{0x816414, &InfoFloat},
	{0x819248, &InfoFloat},
	{0x818ed4, &InfoFloat},
	{0x81957c, &InfoFloat},
	{0x819a7c, &InfoFloat},
	{0x81a410, &InfoFloat},
	{0x81acc8, &InfoFloat},
	{0x81af90, &InfoFloat},
	{0x81bb04, &InfoFloat},
	{0x81bd5c, &InfoFloat},
	{0x81c398, &InfoFloat},
	{0x81c56c, &InfoFloat},
	{0x81d000, &InfoFloat},
	{0x81efc4, &InfoFloat},
	{0x81f61c, &InfoFloat},
	{0x81f994, &InfoFloat},
	{0x818ed4, &InfoFloat},
	{0x81957c, &InfoFloat},
	{0x819a7c, &InfoFloat},
	{0x81a410, &InfoFloat},
	{0x81acc8, &InfoFloat},
	{0x81af90, &InfoFloat},
	{0x81bb04, &InfoFloat},
	{0x81bd5c, &InfoFloat},
	{0x81c398, &InfoFloat},
	{0x81c56c, &InfoFloat},
	{0x81d000, &InfoFloat},
	{0x81efc4, &InfoFloat},
	{0x81f61c, &InfoFloat},
	{0x81f994, &InfoFloat},
	{0x81fdd8, &InfoFloat},
	{0x81ffc8, &InfoFloat},
	{0x8209cc, &InfoFloat},
	{0x820e08, &InfoFloat},
	{0x821224, &InfoFloat},
	{0x822aa8, &InfoFloat},
	{0x822e24, &InfoFloat},
	{0x822f78, &InfoFloat},
	{0x823c88, &InfoFloat},
	{0x8240d4, &InfoFloat},
	{0x825550, &InfoFloat},
	{0x825920, &InfoFloat},
	{0x827fc8, &InfoFloat},
	{0x8288e8, &InfoFloat},
	{0x828e14, &InfoFloat},
	{0x829198, &InfoFloat},
	{0x829480, &InfoFloat},
	{0x8327e8, &InfoFloat},
	{0x834018, &InfoFloat},
	{0x834300, &InfoFloat},
	{0x8345a0, &InfoFloat},
	{0x836b00, &InfoFloat},
	{0x837c90, &InfoFloat},
	{0x83ae98, &InfoFloat},
	{0x8556a0, &InfoFloat},
	{0x8570c0, &InfoFloat},
	{0x82b9c0, &InfoFloat},
	{0x811eac, &InfoFloat},

	{0x8987dc, &InfoFloatRad},

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
	{0x84ef64, &InfoString},
	{0x84fea4, &InfoString},
	{0x855774, &InfoString},
	{0x856ed0, &InfoString},
	{0x862d38, &InfoString},
	{0x872ba4, &InfoString},
	{0x878dcc, &InfoString},
	{0x895ac8, &InfoString},
	{0x87ebe8, &InfoString},
	{0x810CB8, &InfoCharBuff},
	{0x84CC84, &InfoCharBuff},
	{0x83CE28, &InfoCharBuff},
	{0x83CD58, &InfoCharBuff},
	{0x822fb8, &InfoCharBuff},
	{0x837028, &InfoCharBuff},

	{reinterpret_cast<int>(&CppLogic::StringSerializer::GlobalObj), &InfoStdString},

	{0x813978, &InfoClassID},
	{0x8223B4, &InfoClassID},
	{0x80ADB0, &InfoClassID},
	{0x810CF8, &InfoClassID},
	{0x80acbc, &InfoClassID},
	{0x83c9e0, &InfoClassID},
} };

std::string BB::FieldSerializer::GetTypeDescName() const
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

const BB::FieldSerializer::ExtendedInfo& BB::FieldSerializer::GetExtendedInfo() const
{
	int p = reinterpret_cast<int>(this);

	auto it = KnownSerializers.find(p);
	if (it != KnownSerializers.end()) {
		return *it->second;
	}

	return InfoUnknown;
}
const BB::FieldSerializer::ExtendedInfo* BB::FieldSerializer::GetOptExtendedInfo() const
{
	int p = reinterpret_cast<int>(this);

	auto it = KnownSerializers.find(p);
	if (it != KnownSerializers.end()) {
		return it->second;
	}

	return nullptr;
}

inline const char* (__stdcall* const serilistctx_get)(const BB::SerializationListOptions::Context* c, const char* a) = reinterpret_cast<const char* (__stdcall*)(const BB::SerializationListOptions::Context*, const char*)>(0x554600);
const char* BB::SerializationListOptions::Context::GetAttribute(const char* a) const
{
	return serilistctx_get(this, a);
}

std::unique_ptr<BB::SerializationListOptions::Iter, void(__stdcall*)(BB::SerializationListOptions::Iter* i)> BB::SerializationListOptions::UniqueIter(void* list) const
{
	return std::unique_ptr<Iter, void(__stdcall*)(BB::SerializationListOptions::Iter * i)>(AllocIter(list), FreeIter);
}

template<class T>
BB::SerializationListOptions::ExtendedInfo MakeLOEVec() {
	return BB::SerializationListOptions::ExtendedInfo{
		BB::SerializationListOptions::ExtendedInfo::Ty::Vector,
		typename_details::type_name<shok::Vector<T>>(),
		[](void* List, size_t index) -> void* {
			auto* v = reinterpret_cast<shok::Vector<T>*>(List);
			T* r = nullptr;
			if (index >= 0 && index < v->size())
				r = &(*v)[index];
			return r;
		},
		[](void* List, bool(*cond)(void* uv, const BB::SerializationData* sd, void* elem), void* uv, const BB::SerializationData* sd) {
			auto* v = reinterpret_cast<shok::Vector<T>*>(List);
			auto sv = v->SaveVector();
			auto e = std::remove_if(sv.Vector.begin(), sv.Vector.end(), [&](T& v) {
				return cond(uv, sd, &v);
			});
			sv.Vector.erase(e, sv.Vector.end());
		},
	};
}
template<class K, class V>
BB::SerializationListOptions::ExtendedInfo MakeLOEMap() {
	return BB::SerializationListOptions::ExtendedInfo{
		BB::SerializationListOptions::ExtendedInfo::Ty::Map,
		typename_details::type_name<shok::Map<K, V>>(),
		nullptr,
		[](void* List, bool(*cond)(void* uv, const BB::SerializationData* sd, void* elem), void* uv, const BB::SerializationData* sd) {
			auto* v = reinterpret_cast<shok::Map<K, V>*>(List);
			auto i = v->begin();
			while (i != v->end()) {
				if (cond(uv, sd, &*i))
					i = v->erase(i);
				else
					++i;
			}
		},
	};
}

BB::SerializationListOptions::ExtendedInfo LOE_VecInt = MakeLOEVec<int>();
BB::SerializationListOptions::ExtendedInfo LOE_VecBBObj = MakeLOEVec<BB::IObject*>();
BB::SerializationListOptions::ExtendedInfo LOE_VecAARect = MakeLOEVec<shok::AARect>();
//BB::SerializationListOptions::ExtendedInfo LOE_VecBuildingUp = MakeLOEVec<GGL::CLogicProperties::SBuildingUpgradeCategory>();
//BB::SerializationListOptions::ExtendedInfo LOE_VecSettlerUp = MakeLOEVec<GGL::CLogicProperties::SSettlerUpgradeCategory>();
//BB::SerializationListOptions::ExtendedInfo LOE_VecTaxLvl = MakeLOEVec<GGL::CLogicProperties::STaxationLevel>();
//BB::SerializationListOptions::ExtendedInfo LOE_VecTradeRes = MakeLOEVec<GGL::CLogicProperties::STradeResource>();
//BB::SerializationListOptions::ExtendedInfo LOE_VecBlessCat = MakeLOEVec<GGL::CLogicProperties::SBlessCategory>();
BB::SerializationListOptions::ExtendedInfo LOE_VecPos = MakeLOEVec<shok::Position>();
BB::SerializationListOptions::ExtendedInfo LOE_VecPosR = MakeLOEVec<shok::PositionRot>();
BB::SerializationListOptions::ExtendedInfo LOE_VecTradeResData = MakeLOEVec<GGL::CTradeManager::ResData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecWorkModifier = MakeLOEVec<GGL::CServiceBuildingBehaviorProperties::WorkModifier>();
BB::SerializationListOptions::ExtendedInfo LOE_VecSerfExtract = MakeLOEVec<GGL::CSerfBehaviorProps::ExtractionInfoData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecRefinerEff = MakeLOEVec<GGL::CResourceRefinerBehaviorProperties::EfficencyUpgrade>();
BB::SerializationListOptions::ExtendedInfo LOE_VecResDepBuilding = MakeLOEVec<GGL::CResourceDependentBuildingBehaviorProperties::TypeData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecLimitedAttachment = MakeLOEVec<GGL::CLimitedAttachmentBehaviorProperties::LimitedAttachmentProps>();
BB::SerializationListOptions::ExtendedInfo LOE_VecIdleAnim = MakeLOEVec<GGL::CFormationBehaviorProperties::IdleAnimProps>();
BB::SerializationListOptions::ExtendedInfo LOE_VecCannonInfo = MakeLOEVec<GGL::CFoundryBehaviorProperties::CannonInfoData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecXPLevel = MakeLOEVec<GGL::ExperienceClass::LevelData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecETyArrival = MakeLOEVec<EGL::CPlayerFeedbackHandler::SingleArrival>();
BB::SerializationListOptions::ExtendedInfo LOE_VecResOut = MakeLOEVec<EGL::CPlayerFeedbackHandler::SingleResOut>();
BB::SerializationListOptions::ExtendedInfo LOE_VecExploCircle = MakeLOEVec<EGL::CPlayerExplorationHandler::ExCircle>();
BB::SerializationListOptions::ExtendedInfo LOE_VecTriggerParam = MakeLOEVec<EScr::CScriptTrigger::Parameter>();
BB::SerializationListOptions::ExtendedInfo LOE_VecBehProps = MakeLOEVec<GGlue::CGlueEntityProps::BehaviorData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecShorewave = MakeLOEVec<GGlue::WaterPropsDisplay::ShoreWaveData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecShorewaveKF = MakeLOEVec<GGlue::WaterPropsDisplay::ShoreWaveKeyFrameData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecEvaderWait = MakeLOEVec<GGL::CEvadingEntity::EvaderWaitData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecWorkTL = MakeLOEVec<GGL::CGLBuildingProps::WorkTL>();
BB::SerializationListOptions::ExtendedInfo LOE_VecStatTech = MakeLOEVec<GGL::CGameStatistics::TechResearchData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecStatUp = MakeLOEVec<GGL::CGameStatistics::BuildingUpgradedData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecTechState = MakeLOEVec<GGL::PlayerTechManager::Tech>();
BB::SerializationListOptions::ExtendedInfo LOE_VecTribute = MakeLOEVec<GGL::PlayerTributesManager::Tribute>();
BB::SerializationListOptions::ExtendedInfo LOE_VecResStat = MakeLOEVec<GGL::CResourceStatistics::ResData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecTechReq = MakeLOEVec<shok::Technology::TechReq>();
BB::SerializationListOptions::ExtendedInfo LOE_VecECatReq = MakeLOEVec<shok::Technology::ECatReq>();
BB::SerializationListOptions::ExtendedInfo LOE_VecETypeReq = MakeLOEVec<shok::Technology::ETypeReq>();
BB::SerializationListOptions::ExtendedInfo LOE_VecUCatReq = MakeLOEVec<shok::Technology::UCatReq>();
BB::SerializationListOptions::ExtendedInfo LOE_VecEInSystem = MakeLOEVec<GGL::CPlayerAttractionHandler::EntityInSystemData>();
BB::SerializationListOptions::ExtendedInfo LOE_VecEntityP = MakeLOEVec<EGL::CGLEEntity*>();
BB::SerializationListOptions::ExtendedInfo LOE_VecUInt = MakeLOEVec<unsigned int>();

BB::SerializationListOptions::ExtendedInfo LOE_MapIntInt = MakeLOEMap<int, int>();
BB::SerializationListOptions::ExtendedInfo LOE_MapBuildingUp = MakeLOEMap<shok::UpgradeCategoryId, GGL::CBuildingUpgradeManager::ScholarInfo>();
BB::SerializationListOptions::ExtendedInfo LOE_MapLimitedAttachment = MakeLOEMap<shok::AttachmentType, GGL::CLimitedAttachmentBehavior::AttachmentInfo>();
BB::SerializationListOptions::ExtendedInfo LOE_MapTrigger = MakeLOEMap<shok::TriggerId, BB::IObject*>();
BB::SerializationListOptions::ExtendedInfo LOE_MapGDBList = MakeLOEMap<shok::String, BB::IObject*>();
BB::SerializationListOptions::ExtendedInfo LOE_MapEtypeVector = MakeLOEMap<shok::EntityTypeId, shok::Vector<EGL::CGLEEntity*>>();

BB::SerializationListOptions::ExtendedInfo LOE_ArrInt{
	BB::SerializationListOptions::ExtendedInfo::Ty::Array,
	typename_details::type_name<shok::Array<int, 3>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ArrFloat{
	BB::SerializationListOptions::ExtendedInfo::Ty::Array,
	typename_details::type_name<shok::Array<float, 8>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ArrMaterial{
	BB::SerializationListOptions::ExtendedInfo::Ty::Array,
	typename_details::type_name<shok::Array<EGUIX::CMaterial, 5>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ArrSettlerFeedback{
	BB::SerializationListOptions::ExtendedInfo::Ty::Array,
	typename_details::type_name<shok::Array<shok::Set<EGL::CPlayerFeedbackHandler::SingleFeedack>, 7>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ArrGenMessage{
	BB::SerializationListOptions::ExtendedInfo::Ty::Array,
	typename_details::type_name<shok::Array<EGL::CPlayerFeedbackHandler::GenMessageData, 4>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ArrBool{
	BB::SerializationListOptions::ExtendedInfo::Ty::Array,
	typename_details::type_name<shok::Array<bool, 20>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ArrDiplo{
	BB::SerializationListOptions::ExtendedInfo::Ty::Array,
	typename_details::type_name<shok::Array<shok::DiploState, 9>>(),
	nullptr, // TODO?
	nullptr,
};

BB::SerializationListOptions::ExtendedInfo LOE_ListQuest{
	BB::SerializationListOptions::ExtendedInfo::Ty::List,
	typename_details::type_name<shok::List<GGL::PlayerQuestManager::Quest>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ListEtyLeft{
	BB::SerializationListOptions::ExtendedInfo::Ty::List,
	typename_details::type_name<shok::List<GGL::CPlayerAttractionHandler::TypeThatLeftData>>(),
	nullptr, // TODO?
	nullptr,
};
BB::SerializationListOptions::ExtendedInfo LOE_ListBBObj{
	BB::SerializationListOptions::ExtendedInfo::Ty::List,
	typename_details::type_name<shok::List<BB::IObject*>>(),
	nullptr, // TODO?
	nullptr,
};


auto& KnownListInfos() {
	static std::map<int, const BB::SerializationListOptions::ExtendedInfo*> i{
		{0x84e8f8, &LOE_VecInt},
		{0x85e214, &LOE_VecBBObj},
		{0x878b88, &LOE_VecBBObj},
		{0x88363c, &LOE_VecBBObj},
		{0x898240, &LOE_VecBBObj},
		{0x8989d4, &LOE_VecBBObj},
		{0x85dcc8, &LOE_VecAARect},
		/*{0x860868, &LOE_VecBuildingUp}, needs copy operators
		{0x86088c, &LOE_VecSettlerUp},
		{0x8608b0, &LOE_VecTaxLvl},
		{0x8608d4, &LOE_VecTradeRes},
		{0x86098c, &LOE_VecBlessCat},*/
		{0x865fc8, &LOE_VecPos},
		{0x869100, &LOE_VecTradeResData},
		{0x86f2b0, &LOE_VecWorkModifier},
		{0x86fd50, &LOE_VecSerfExtract},
		{0x8701a4, &LOE_VecRefinerEff},
		{0x87060c, &LOE_VecResDepBuilding},
		{0x8732b8, &LOE_VecLimitedAttachment},
		//{0x8755c0, &}, vec animset GoodDependentAnim
		{0x875a1c, &LOE_VecIdleAnim},
		{0x87ade8, &LOE_VecCannonInfo},
		{0x87ce64, &LOE_VecXPLevel},
		{0x89866c, &LOE_VecETyArrival},
		{0x898690, &LOE_VecResOut},
		{0x8b17dc, &LOE_VecExploCircle},
		{0xa06b14, &LOE_VecTriggerParam},
		{0xa0c9e8, &LOE_VecBehProps},
		{0x84cb28, &LOE_VecShorewave},
		{0xa0dc84, &LOE_VecShorewaveKF},
		{0x85e238, &LOE_VecEvaderWait},
		{0x85f4a8, &LOE_VecPosR},
		{0x85f4cc, &LOE_VecWorkTL},
		{0x8622f0, &LOE_VecStatTech},
		{0x862314, &LOE_VecStatUp},
		{0x862338, &LOE_VecTechState},
		{0x862488, &LOE_VecTribute},
		{0x862520, &LOE_VecResStat},
		{0x8643f0, &LOE_VecTechReq},
		{0x864414, &LOE_VecETypeReq},
		{0x864438, &LOE_VecECatReq},
		{0x86445c, &LOE_VecUCatReq},
		{0x867494, &LOE_VecEInSystem},
		{0x8970d0, &LOE_VecEntityP},
		{0x862178, &LOE_VecUInt},

		{0x85f484, &LOE_MapIntInt},
		{0x86a5c8, &LOE_MapBuildingUp},
		{0x873220, &LOE_MapLimitedAttachment},
		{0xa06a58, &LOE_MapTrigger},
		{0x84fce8, &LOE_MapGDBList},
		{0x8b21b8, &LOE_MapEtypeVector},

		{0x85dbc0, &LOE_ArrInt},
		{0x87cff0, &LOE_ArrFloat},
		{0x89559c, &LOE_ArrMaterial},
		{0x8985b4, &LOE_ArrSettlerFeedback},
		{0x8985d8, &LOE_ArrGenMessage},
		{0x861fe0, &LOE_ArrBool},
		{0x862004, &LOE_ArrDiplo},

		{0x8623f4, &LOE_ListQuest},
		{0x867470, &LOE_ListEtyLeft},
		{0x894d4c, &LOE_ListBBObj},
	};
	return i;
};


const BB::SerializationListOptions::ExtendedInfo* BB::SerializationListOptions::TryGetExtendedInfo() const
{
	int k = reinterpret_cast<int>(this);

	auto& i = KnownListInfos();
	auto it = i.find(k);
	if (it != i.end()) {
		return it->second;
	}

	return nullptr;
}

const BB::SerializationListOptions::ExtendedInfo& BB::SerializationListOptions::GetExtendedInfo() const
{
	auto* inf = TryGetExtendedInfo();
	if (inf == nullptr)
		throw std::invalid_argument{ "no extended list info known" };
	return *inf;
}

void BB::SerializationListOptions::RegisterExtended(const ExtendedInfo& e) const
{
	KnownListInfos()[reinterpret_cast<int>(this)] = &e;
}

const BB::SerializationData* BB::SerializationData::GetSerializationDataFromId(shok::ClassId id)
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

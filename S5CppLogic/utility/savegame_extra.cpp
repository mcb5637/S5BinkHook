#include <filesystem>
#include "savegame_extra.h"
#include <shok/globals/s5_classfactory.h>
#include <shok/globals/s5_filesystem.h>
#include <shok/s5_exception.h>
#include <shok/ui/s5_ui.h>
#include <shok/s5_scriptsystem.h>

void CppLogic::SavegameExtra::SerializedMapdata::SerializeTo(const char* path, const char* savename)
{
	SavegameName = savename;
	std::string p{ path };
	p.append("\\CppLogic.xml");
	auto s = BB::CXmlSerializer::CreateUnique();
	try {
		BB::CFileStreamEx f{};
		if (f.OpenFile(p.c_str(), BB::IStream::Flags::DefaultWrite)) {
			s->SerializeByData(&f, this, SerializationData, nullptr);
		}
		f.Close();
	}
	catch (const BB::CFileException& e) {
		char buff[201]{};
		e.CopyMessage(buff, 200);
		shok::LogString(__FUNCSIG__" error: %s\n", buff);
	}
	try {
		std::filesystem::path pa{ path };
		pa.concat("\\CppLogic.bin");
		if (std::filesystem::exists(pa)) {
			std::filesystem::remove(pa);
		}
	}
	catch (const std::exception& e) {
		shok::LogString(__FUNCSIG__" error: %s\n", e.what());
	}
}

void CppLogic::SavegameExtra::SerializedMapdata::DeserializeFrom(const char* path, const char* savename)
{
	std::string p{ path };
	p.append("\\CppLogic.xml");
	auto s = BB::CXmlSerializer::CreateUnique();
	try {
		BB::CFileStreamEx f{};
		if (f.OpenFile(p.c_str(), BB::IStream::Flags::DefaultRead)) {
			s->DeserializeByData(&f, this, SerializationData);
		}
		f.Close();
	}
	catch (const BB::CFileException& e) {
		char buff[201]{};
		e.CopyMessage(buff, 200);
		shok::LogString(__FUNCSIG__" error: %s\n", buff);
	}
	if (SavegameName != savename)
	{
		shok::LogString(__FUNCSIG__" save name does not match, is %s, should be %s\n", SavegameName.c_str(), savename);
		Clear();
	}
}

const char* SetStringTableText_GetText(const char* s, const std::map<std::string, std::string, CppLogic::CaselessStringComparator>& map)
{
	if (!s)
		return s;

	std::string_view sfor{ s };
	auto f = map.find(sfor);

	if (f != map.end())
		return f->second.c_str();
	else
		return nullptr;
}
const char* SetStringTableText_GetTextIngame(const char* s) {
	return SetStringTableText_GetText(s, CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.StringTableTextOverride);
}
const char* SetStringTableText_GetTextMainmenu(const char* s) {
	return SetStringTableText_GetText(s, CppLogic::SavegameExtra::SerializedMapdata::StringTableTextOverrideMainmenu);
}
void stt_remove() {

}
void stt_enable(const char* (*f)(const char*)) {
	BB::StringTableText::HookGetStringTableText();
	BB::StringTableText::GetStringTableTextOverride = f;
}
void CppLogic::SavegameExtra::SerializedMapdata::STTToMainmenu()
{
	if (StringTableTextOverrideMainmenu.empty())
		stt_remove();
	else
		stt_enable(SetStringTableText_GetTextMainmenu);
}

void CppLogic::SavegameExtra::SerializedMapdata::STTToIngame()
{
	if (GlobalObj.StringTableTextOverride.empty())
		stt_remove();
	else
		stt_enable(SetStringTableText_GetTextIngame);
}

std::map<std::string, std::string, CppLogic::CaselessStringComparator>& CppLogic::SavegameExtra::SerializedMapdata::GetActiveOverrides(lua_State* s)
{
	if (s == *EScr::CScriptTriggerSystem::GameState)
		return GlobalObj.StringTableTextOverride;
	else
		return StringTableTextOverrideMainmenu;
}
void CppLogic::SavegameExtra::SerializedMapdata::STTHasChanged(lua_State* s)
{
	if (s == *EScr::CScriptTriggerSystem::GameState)
		return STTToIngame();
	else
		return STTToMainmenu();
}

CppLogic::SavegameExtra::SerializedMapdata CppLogic::SavegameExtra::SerializedMapdata::GlobalObj{};
std::map<std::string, std::string, CppLogic::CaselessStringComparator>  CppLogic::SavegameExtra::SerializedMapdata::StringTableTextOverrideMainmenu{};

CreateSerializationListFor(CppLogic::SavegameExtra::SerializedMapdata, StringTableTextOverride);

BB::SerializationData CppLogic::SavegameExtra::SerializedMapdata::SerializationData[]{
	AutoMemberSerialization(SerializedMapdata, SavegameName),
	AutoMemberSerialization(SerializedMapdata, HurtEntityCallWithNoAttacker),
	AutoMemberSerialization(SerializedMapdata, UseMaxHPTechBoni),
	AutoMemberSerialization(SerializedMapdata, PaydayTrigger),
	AutoMemberSerialization(SerializedMapdata, LeaderRegenRegenerateSoldiers),
	AutoMemberSerialization(SerializedMapdata, ChangeTaskListCheckUncancelable),
	AutoMemberSerialization(SerializedMapdata, BuildOnMovementFix),
	AutoMemberSerialization(SerializedMapdata, ExperienceClassFix),
	AutoMemberSerialization(SerializedMapdata, AoEDamageFix),
	AutoMemberSerialization(SerializedMapdata, CamoFix),
	AutoMemberSerialization(SerializedMapdata, ConversionTrigger),
	AutoMemberSerialization(SerializedMapdata, HookMaxHP),
	AutoMemberSerialization(SerializedMapdata, HookDamage),
	AutoMemberSerialization(SerializedMapdata, HookArmor),
	AutoMemberSerialization(SerializedMapdata, HookExploration),
	AutoMemberSerialization(SerializedMapdata, HookRegen),
	AutoMemberSerialization(SerializedMapdata, HookMaxRange),
	AutoMemberSerialization(SerializedMapdata, HookDisplayName),
	AutoMemberSerialization(SerializedMapdata, RangedEffectSoldierHeal),
	AutoMemberSerialization(SerializedMapdata, EffectTriggers),
	AutoMemberSerialization(SerializedMapdata, ResourceTriggers),
	AutoMemberSerialization(SerializedMapdata, RefinerFix),
	AutoMemberSerialization(SerializedMapdata, SettlerBuyTriggers),
	AutoMemberSerialization(SerializedMapdata, StringTableTextOverride),
	AutoMemberSerialization(SerializedMapdata, ScriptTriggerEval),
	AutoMemberSerialization(SerializedMapdata, CannonInProgressAttraction),
	AutoMemberSerialization(SerializedMapdata, LightningEffectFix),
	AutoMemberSerialization(SerializedMapdata, ResDoodad_RefillableCategory),
	BB::SerializationData::GuardData(),
};

void CppLogic::SavegameExtra::SerializedMapdata::Clear()
{
	SavegameName.clear();
	HurtEntityCallWithNoAttacker = false;
	UseMaxHPTechBoni = false;
	PaydayTrigger = false;
	LeaderRegenRegenerateSoldiers = false;
	ChangeTaskListCheckUncancelable = false;
	BuildOnMovementFix = false;
	ExperienceClassFix = false;
	AoEDamageFix = false;
	CamoFix = false;
	ConversionTrigger = false;
	HookMaxHP = false;
	HookDamage = false;
	HookArmor = false;
	HookExploration = false;
	HookRegen = false;
	HookMaxRange = false;
	HookDisplayName = false;
	RangedEffectSoldierHeal = false;
	EffectTriggers = false;
	ResourceTriggers = false;
	RefinerFix = false;
	SettlerBuyTriggers = false;
	ScriptTriggerEval = false;
	CannonInProgressAttraction = false;
	LightningEffectFix = false;
	ResDoodad_RefillableCategory = {};
	StringTableTextOverride.clear();
}

CreateSerializationListForKeyAttrib(CppLogic::SavegameExtra::StringTableTextOverride, StringTableTextOverrideData, "id");

BB::SerializationData CppLogic::SavegameExtra::StringTableTextOverride::SerializationData[]{
	AutoMemberSerializationNameKeyAttrib(CppLogic::SavegameExtra::StringTableTextOverride, StringTableTextOverrideData, "string", "id"),
	BB::SerializationData::GuardData(),
};

void CppLogic::SavegameExtra::StringTableTextOverride::Merge(std::string_view prefix, lua_State* s) const
{
	auto& target = SerializedMapdata::GetActiveOverrides(s);
	for (const auto& [k, v] : StringTableTextOverrideData) {
		target[std::format("{}/{}", prefix, k)] = v;
	}
	SerializedMapdata::STTHasChanged(s);
}

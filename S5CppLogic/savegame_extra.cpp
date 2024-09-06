#include "pch.h"
#include <filesystem>
#include "savegame_extra.h"
#include "s5_classfactory.h"
#include "s5_filesystem.h"
#include "s5_exception.h"

void CppLogic::SavegameExtra::SerializedMapdata::SerializeTo(const char* path, const char* savename)
{
	SavegameName = savename;
	std::string p{ path };
	p.append("\\CppLogic.xml");
	auto s = BB::CXmlSerializer::CreateUnique();
	try {
		BB::CFileStreamEx f{};
		if (f.OpenFile(p.c_str(), BB::IStream::Flags::DefaultWrite)) {
			s->SerializeByData(&f, this, SerializationData, 0);
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

CppLogic::SavegameExtra::SerializedMapdata CppLogic::SavegameExtra::SerializedMapdata::GlobalObj{};

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
	StringTableTextOverride.clear();
}
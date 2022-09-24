#include "pch.h"
#include "savegame_extra.h"
#include "s5_classfactory.h"
#include "s5_filesystem.h"
#include "s5_exception.h"

bool CppLogic::SavegameExtra::SerializedMapdata::StringComparator::operator()(const std::string& a, const std::string& b) const noexcept
{
	return _stricmp(a.c_str(), b.c_str()) < 0;
}

void CppLogic::SavegameExtra::SerializedMapdata::SerializeTo(const char* path, const char* savename)
{
	SavegameName = savename;
	std::string p{ path };
	p.append("\\CppLogic.xml");
	auto* s = BB::CXmlSerializer::Create();
	try {
		BB::CFileStreamEx f{};
		if (f.OpenFile(p.c_str(), BB::IStream::Flags::GenericWrite)) {
			s->SerializeByData(&f, this, SerializationData, "SerializedMapdata");
		}
		f.Close();
	}
	catch (const BB::CFileException& e) {
		char buff[201]{};
		e.CopyMessage(buff, 200);
		shok::LogString(__FUNCSIG__" error: %s\n", buff);
	}
	s->Destroy();
}

void CppLogic::SavegameExtra::SerializedMapdata::DeserializeFrom(const char* path, const char* savename)
{
	std::string p{ path };
	p.append("\\CppLogic.xml");
	try {
		(*BB::CClassFactory::GlobalObj)->LoadObject(this, p.c_str(), SerializationData);
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

CppLogic::SerializationListOptions_ForMap<const std::string, std::string> StringMap{};
using ptype = std::pair<const std::string, std::string>;
BB::SerializationData PairStrings[]{
	BB::SerializationData::FieldData("Key", MemberSerializationSizeAndOffset(ptype, first), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::FieldData("Value", MemberSerializationSizeAndOffset(ptype, second), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::GuardData(),
};

BB::SerializationData CppLogic::SavegameExtra::SerializedMapdata::SerializationData[]{
	BB::SerializationData::FieldData("SavegameName", MemberSerializationSizeAndOffset(SerializedMapdata, SavegameName), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::FieldData("HurtEntityCallWithNoAttacker", MemberSerializationFieldData(SerializedMapdata, HurtEntityCallWithNoAttacker)),
	BB::SerializationData::FieldData("UseMaxHPTechBoni", MemberSerializationFieldData(SerializedMapdata, UseMaxHPTechBoni)),
	BB::SerializationData::FieldData("PaydayTrigger", MemberSerializationFieldData(SerializedMapdata, PaydayTrigger)),
	BB::SerializationData::FieldData("LeaderRegenRegenerateSoldiers", MemberSerializationFieldData(SerializedMapdata, LeaderRegenRegenerateSoldiers)),
	BB::SerializationData::FieldData("ChangeTaskListCheckUncancelable", MemberSerializationFieldData(SerializedMapdata, ChangeTaskListCheckUncancelable)),
	BB::SerializationData::FieldData("BuildOnMovementFix", MemberSerializationFieldData(SerializedMapdata, BuildOnMovementFix)),
	BB::SerializationData::FieldData("ExperienceClassFix", MemberSerializationFieldData(SerializedMapdata, ExperienceClassFix)),
	BB::SerializationData::FieldData("AoEDamageFix", MemberSerializationFieldData(SerializedMapdata, AoEDamageFix)),
	BB::SerializationData::FieldData("CamoFix", MemberSerializationFieldData(SerializedMapdata, CamoFix)),
	BB::SerializationData::FieldData("ConversionTrigger", MemberSerializationFieldData(SerializedMapdata, ConversionTrigger)),
	BB::SerializationData::EmbeddedData("StringTableTextOverride", MemberSerializationSizeAndOffset(SerializedMapdata ,StringTableTextOverride), PairStrings, &StringMap),
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
	StringTableTextOverride.clear();
}
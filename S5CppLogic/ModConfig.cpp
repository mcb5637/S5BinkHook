#include "pch.h"
#include "ModConfig.h"
#include "s5_classfactory.h"

void CppLogic::Mod::Config::RegisterClasses()
{
    (*BB::CClassFactory::GlobalObj)->AddClassToFactory<DamageClassExt>();
}

CppLogic::SerializationListOptions_ForMap<shok::ArmorClassId, float>  ArmorclassToFloatMap_ListOptions;
using pair_ac_float = std::pair<const shok::ArmorClassId, float>;
const BB::SerializationData PairACFloat_SerializationData[]{
	BB::SerializationData::FieldData("ArmorClass", MemberSerializationSizeAndOffset(pair_ac_float, first), BB::FieldSerilaizer::TypeArmorClassId),
	BB::SerializationData::FieldData("Factor", MemberSerializationFieldData(pair_ac_float, second)),
	BB::SerializationData::GuardData(),
};

const BB::SerializationData CppLogic::Mod::Config::DamageClassExt::SerializationData[3] {
    BB::SerializationData::EmbeddedData(nullptr, 0, sizeof(GGL::CDamageClassProps), GGL::CDamageClassProps::SerializationData),
    BB::SerializationData::EmbeddedData("ExtraArmorClass", MemberSerializationSizeAndOffset(DamageClassExt, ExtraArmorClasses), PairACFloat_SerializationData, &ArmorclassToFloatMap_ListOptions),
    BB::SerializationData::GuardData(),
};

shok::ClassId CppLogic::Mod::Config::DamageClassExt::GetClassIdentifier() const
{
	return Identifier;
}

void* CppLogic::Mod::Config::DamageClassExt::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::Config::DamageClassExt::operator delete(void* p) {
    shok::Free(p);
}

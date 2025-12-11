#include "pch.h"
#include "ModConfig.h"
#include <shok/globals/s5_classfactory.h>

void CppLogic::Mod::Config::RegisterClasses()
{
    (*BB::CClassFactory::GlobalObj)->AddClassToFactory<DamageClassExt>();
}

CreateSerializationListForKeyValue(CppLogic::Mod::Config::DamageClassExt, ExtraArmorClasses, "ArmorClass", "Factor");

const BB::SerializationData CppLogic::Mod::Config::DamageClassExt::SerializationData[] {
    BB::SerializationData::AutoBaseClass<DamageClassExt, GGL::CDamageClassProps>(),
    AutoMemberSerializationNameKeyVaue(DamageClassExt, ExtraArmorClasses, "ExtraArmorClass", "ArmorClass", "Factor"),
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

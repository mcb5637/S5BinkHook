#include "pch.h"
#include "ModBehavior.h"
#include "s5_events.h"
#include "s5_classfactory.h"

void CppLogic::Mod::RegisterClasses()
{
    BB::CClassFactory* f = *BB::CClassFactory::GlobalObj;
    f->AddClassToFactory<FormationSpacedBehaviorProps>();
    f->AddClassToFactory<FormationSpacedBehavior>();
}

unsigned int __stdcall CppLogic::Mod::FormationSpacedBehaviorProps::GetClassIdentifier() const
{
    return Identifier;
}

void* CppLogic::Mod::FormationSpacedBehaviorProps::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::FormationSpacedBehaviorProps::operator delete(void* p) {
    shok::Free(p);
}

const BB::SerializationData CppLogic::Mod::FormationSpacedBehaviorProps::SerializationData[]{
    BB::SerializationData::EmbeddedData(nullptr, 0, sizeof(GGL::CFormationBehaviorProperties), GGL::CFormationBehaviorProperties::SerializationData),
    BB::SerializationData::FieldData("SpaceFactor", MemberSerializationFieldData(FormationSpacedBehaviorProps, SpaceFactor)),
    BB::SerializationData::GuardData(),
};

unsigned int __stdcall CppLogic::Mod::FormationSpacedBehavior::GetClassIdentifier() const
{
    return Identifier;
}

void* __stdcall CppLogic::Mod::FormationSpacedBehavior::CastToIdentifier(unsigned int id)
{
    if (id == IFormationBehaviorExtProvider::Identifier)
        return static_cast<IFormationBehaviorExtProvider*>(this);
    return nullptr;
}

shok::Position CppLogic::Mod::FormationSpacedBehavior::GetPosExt(EGL::CGLEEntity* leader)
{
    shok::Position n;
    GetFormationPosition(leader, &n);
    EGL::CEventGetPosition ev{ shok::EventIDs::Movement_GetNextFineWaypoint };
    leader->FireEvent(&ev);
    shok::Position diff = n - ev.Position;
    diff *= static_cast<FormationSpacedBehaviorProps*>(PropPointer)->SpaceFactor;
    return ev.Position + diff;
}

CppLogic::Mod::FormationSpacedBehavior::FormationSpacedBehavior()
{
    HookGetPosExtI();
}

const BB::SerializationData CppLogic::Mod::FormationSpacedBehavior::SerializationData[] {
    BB::SerializationData::EmbeddedData(nullptr, 0, sizeof(GGL::CFormationBehavior), GGL::CFormationBehavior::SerializationData),
    BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::FormationSpacedBehavior::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::FormationSpacedBehavior::operator delete(void* p) {
    shok::Free(p);
}

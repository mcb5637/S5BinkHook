#include "pch.h"
#include "ModBehavior.h"
#include "s5_events.h"
#include "s5_classfactory.h"

void CppLogic::Mod::RegisterClasses()
{
    BB::CClassFactory* f = *BB::CClassFactory::GlobalObj;
    f->AddClassToFactory<FormationSpacedBehaviorProps>();
    f->AddClassToFactory<FormationSpacedBehavior>();
    f->AddClassToFactory<ResourceTrackerBehavior>();
}

shok::ClassId __stdcall CppLogic::Mod::FormationSpacedBehaviorProps::GetClassIdentifier() const
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
    BB::SerializationData::AutoBaseClass<FormationSpacedBehaviorProps, GGL::CFormationBehaviorProperties>(),
    AutoMemberSerialization(FormationSpacedBehaviorProps, SpaceFactor),
    BB::SerializationData::GuardData(),
};

shok::ClassId __stdcall CppLogic::Mod::FormationSpacedBehavior::GetClassIdentifier() const
{
    return Identifier;
}

void* __stdcall CppLogic::Mod::FormationSpacedBehavior::CastToIdentifier(shok::ClassId id)
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
    BB::SerializationData::AutoBaseClass<FormationSpacedBehavior, GGL::CFormationBehavior>(),
    BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::FormationSpacedBehavior::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::FormationSpacedBehavior::operator delete(void* p) {
    shok::Free(p);
}

shok::ClassId __stdcall CppLogic::Mod::ResourceTrackerBehavior::GetClassIdentifier() const
{
    return Identifier;
}

const BB::SerializationData CppLogic::Mod::ResourceTrackerBehavior::SerializationData[]{
    BB::SerializationData::AutoBaseClass<ResourceTrackerBehavior, EGL::CGLEBehavior>(),
    AutoMemberSerialization(ResourceTrackerBehavior, Produced),
    AutoMemberSerialization(ResourceTrackerBehavior, Used),
    BB::SerializationData::GuardData(),
};

void* CppLogic::Mod::ResourceTrackerBehavior::operator new(size_t s)
{
    return shok::Malloc(s);
}
void CppLogic::Mod::ResourceTrackerBehavior::operator delete(void* p) {
    shok::Free(p);
}

void __thiscall CppLogic::Mod::ResourceTrackerBehavior::AddHandlers(shok::EntityId id)
{
    EntityId = id;
    auto* e = EGL::CGLEEntity::GetEntityByID(id);
    e->CreateEventHandler<shok::EventIDs::CppLogicEvent_OnResourceMined>(this, &ResourceTrackerBehavior::EventMinedOrRefined);
    e->CreateEventHandler<shok::EventIDs::CppLogicEvent_OnResourceRefined>(this, &ResourceTrackerBehavior::EventMinedOrRefined);
    e->CreateEventHandler<shok::EventIDs::CppLogicEvent_OnRefinerSupplyTaken>(this, &ResourceTrackerBehavior::EventSupplied);
}
void __thiscall CppLogic::Mod::ResourceTrackerBehavior::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
    PropPointer = p;
}
void __thiscall CppLogic::Mod::ResourceTrackerBehavior::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
    PropPointer = p;
}
void __thiscall CppLogic::Mod::ResourceTrackerBehavior::OnEntityUpgrade(EGL::CGLEEntity* old)
{
    auto* o = old->GetBehavior<ResourceTrackerBehavior>();
    if (o) {
        Produced = o->Produced;
        Used = o->Used;
    }
}

void CppLogic::Mod::ResourceTrackerBehavior::EventMinedOrRefined(GGL::CEventGoodsTraded* ev)
{
    Produced.AddToType(ev->BuyType, ev->BuyAmount);
}
void CppLogic::Mod::ResourceTrackerBehavior::EventSupplied(GGL::CEventGoodsTraded* ev)
{
    Used.AddToType(ev->SellType, ev->SellAmount);
}

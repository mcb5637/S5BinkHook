#include "pch.h"
#include "s5data.h"

struct shok_vtable_ED_CBehaviorProps : shok_vtable_BB_IObject {
    void(__stdcall* OnAdd)(shok_ED_IBehavior* th, shok_ED_CEntity* edisplay, shok_ED_CBehaviorProps* props, int _2); // called before shok_ED_CEntity init
    void(__stdcall* Initialize)(shok_ED_IBehavior* th, shok_ED_CEntity* edisplay, shok_ED_CBehaviorProps* props); // called after shok_ED_CEntity init
    void(__stdcall* UpdateRenderNoTick)(shok_ED_IBehavior* th, int count, float uk);
    void(__stdcall* UpdateRenderOneTick)(shok_ED_IBehavior* th, int count, float uk);
    void(__stdcall* UpdateRenderManyTick)(shok_ED_IBehavior* th, int count, float uk);
    PADDINGI(1); // 1 param, ret 100
};

struct shok_vtable_ED_CEntity : shok_vtable_BB_IObject {
    void(__thiscall* Initialize)(shok_ED_CEntity* th, int* m); // m might be p to model id?
    void(__stdcall* OnRenderUpdate)(shok_ED_CEntity* th, int tick, float seconds); // tick seems to be logic ticks, seconds in gametime (ticks/10)
    PADDINGI(1); // maybe update something
    PADDINGI(1); // get some data to pointer, return bool?
    PADDINGI(2); // empty funcs
    PADDINGI(1); // stdcall? set some float?
    PADDINGI(1); // empty func, ret 0
    void(__stdcall* SetPositionData)(shok_ED_CEntity* th, EGL::IEntityDisplay::posdata* data);
    PADDINGI(1); // return some float
};

shok_BB_CClassFactory_serializationData* shok_ED_CBehaviorProps::SerializationData = reinterpret_cast<shok_BB_CClassFactory_serializationData*>(0x8586E8);


void shok_ED_CEntity::SetPositionData(EGL::IEntityDisplay::posdata* d)
{
    reinterpret_cast<shok_vtable_ED_CEntity*>(vtable)->SetPositionData(this, d);
}
void shok_ED_CEntity::ResetPositionData()
{
    EGL::IEntityDisplay::posdata d = Entity->GetPosData();
    SetPositionData(&d);
}

static inline shok_ED_CEntity* (__thiscall* const shok_ED_CVisibleEntityManager_create)(shok_ED_CVisibleEntityManager* th, EGL::IEntityDisplay* e) = reinterpret_cast<shok_ED_CEntity * (__thiscall*)(shok_ED_CVisibleEntityManager*, EGL::IEntityDisplay*)>(0x4762EA);
shok_ED_CEntity* shok_ED_CVisibleEntityManager::CreateDisplayForEntity(EGL::IEntityDisplay* e)
{
    return shok_ED_CVisibleEntityManager_create(this, e);
}
shok_ED_CEntity* shok_ED_CVisibleEntityManager::GetDisplayForEntity(int eid)
{
    return Entities[eid & 0xFFFF];
}
static inline void(__thiscall* const shok_ED_CVisibleEntityManager_destroy)(shok_ED_CVisibleEntityManager* th, int id) = reinterpret_cast<void(__thiscall*)(shok_ED_CVisibleEntityManager*, int)>(0x476066);
void shok_ED_CVisibleEntityManager::DestroyDisplayForEntity(int eid)
{
    shok_ED_CVisibleEntityManager_destroy(this, eid & 0xFFFF);
}

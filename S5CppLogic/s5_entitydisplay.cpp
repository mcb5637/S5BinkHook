#include "pch.h"
#include "s5_entitydisplay.h"

struct shok_vtable_ED_CBehaviorProps : BB::IObject::_vtableS {
    void(__stdcall* OnAdd)(ED::IBehavior* th, ED::CEntity* edisplay, ED::CBehaviorProps* props, int _2); // called before ED::CEntity init
    void(__stdcall* Initialize)(ED::IBehavior* th, ED::CEntity* edisplay, ED::CBehaviorProps* props); // called after ED::CEntity init
    void(__stdcall* UpdateRenderNoTick)(ED::IBehavior* th, int count, float uk);
    void(__stdcall* UpdateRenderOneTick)(ED::IBehavior* th, int count, float uk);
    void(__stdcall* UpdateRenderManyTick)(ED::IBehavior* th, int count, float uk);
    PADDINGI(1); // 1 param, ret 100
};

struct shok_vtable_ED_CEntity : BB::IObject::_vtableS {
    void(__thiscall* Initialize)(ED::CEntity* th, int* m); // m might be p to model id?
    void(__stdcall* OnRenderUpdate)(ED::CEntity* th, int tick, float seconds); // tick seems to be logic ticks, seconds in gametime (ticks/10)
    PADDINGI(1); // maybe update something
    PADDINGI(1); // get some data to pointer, return bool?
    PADDINGI(2); // empty funcs
    PADDINGI(1); // stdcall? set some float?
    PADDINGI(1); // empty func, ret 0
    void(__stdcall* SetPositionData)(ED::CEntity* th, EGL::IEntityDisplay::posdata* data);
    PADDINGI(1); // return some float
};

void ED::CEntity::ResetPositionData()
{
    EGL::IEntityDisplay::posdata d = Entity->GetPosData();
    SetPositionData(&d);
}

static inline ED::CEntity* (__thiscall* const shok_ED_CVisibleEntityManager_create)(ED::CVisibleEntityManager* th, EGL::IEntityDisplay* e) = reinterpret_cast<ED::CEntity * (__thiscall*)(ED::CVisibleEntityManager*, EGL::IEntityDisplay*)>(0x4762EA);
ED::CEntity* ED::CVisibleEntityManager::CreateDisplayForEntity(EGL::IEntityDisplay* e)
{
    return shok_ED_CVisibleEntityManager_create(this, e);
}
static inline void(__thiscall* const shok_ED_CVisibleEntityManager_destroy)(ED::CVisibleEntityManager* th, int id) = reinterpret_cast<void(__thiscall*)(ED::CVisibleEntityManager*, int)>(0x476066);
void ED::CVisibleEntityManager::DestroyDisplayForEntity(shok::EntityId eid)
{
    shok_ED_CVisibleEntityManager_destroy(this, static_cast<int>(eid) & 0xFFFF);
}

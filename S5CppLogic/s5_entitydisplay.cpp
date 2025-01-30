#include "pch.h"
#include "s5_entitydisplay.h"

void ED::CEntity::ResetPositionData()
{
    EGL::IEntityDisplay::posdata d = Entity->GetPosData();
    SetPositionData(&d);
}

ED::ModelData* ED::CEntity::GetModelData()
{
    int m = reinterpret_cast<int>(Model);
    if (m == 0)
        return nullptr;
    return *reinterpret_cast<ED::ModelData**>(m + *reinterpret_cast<int*>(0x858228) + 0xc);
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

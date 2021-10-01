#include "pch.h"
#include "s5data.h"

static inline shok_ED_CEntity* (__thiscall* const shok_ED_CVisibleEntityManager_create)(shok_ED_CVisibleEntityManager* th, shok_IEntityDisplay* e) = reinterpret_cast<shok_ED_CEntity * (__thiscall*)(shok_ED_CVisibleEntityManager*, shok_IEntityDisplay*)>(0x4762EA);
shok_ED_CEntity* shok_ED_CVisibleEntityManager::CreateDisplayForEntity(shok_IEntityDisplay* e)
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

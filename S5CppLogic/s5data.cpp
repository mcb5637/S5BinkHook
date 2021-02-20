#include "pch.h"

#include "s5data.h"

shok_EGL_CGLEEntity* (_stdcall* shok_eid2obj)(int id) = (shok_EGL_CGLEEntity * (_stdcall*)(int)) 0x5825B4;
void(_stdcall* shok_SetHighPrecFPU)() = (void(_stdcall *)()) 0x5C8451;
shok_ECS_CManager*** shok_ECS_CManagerObject = (shok_ECS_CManager***)0x0A0344C;
shok_EGL_CGLEGameLogic** shok_EGL_CGLEGameLogicObject = (shok_EGL_CGLEGameLogic**)0x895DAC;
int shok_effectCreatorData_CreatorType_Projectile = 0x774698;
shok_EGL_CGLEEffectManager** shok_EGL_CGLEEffectManagerObject = (shok_EGL_CGLEEffectManager**)0x898144;
bool(__thiscall* shok_EGL_CGLEEffectManager_IsEffectValid)(shok_EGL_CGLEEffectManager* th, int id) = (bool(__thiscall*)(shok_EGL_CGLEEffectManager*, int))0x4FAABD;
void(* shok_entityHurtEntity)(shok_EGL_CGLEEntity* attackerObj, shok_EGL_CGLEEntity* targetObj, int damage) = (void(*)(shok_EGL_CGLEEntity*, shok_EGL_CGLEEntity*, int)) 0x49F358;
void (*shok_logString)(const char* format, const char* string) = (void (*)(const char* format, const char* string)) 0x548268;
shok_BB_CIDManagerEx** shok_BB_CIDManagerExObj = (shok_BB_CIDManagerEx**)0xA0C838;
int(__thiscall* shok_getAnimIdByName)(shok_BB_CIDManagerEx* th, const char* name) = (int(__thiscall*)(shok_BB_CIDManagerEx * th, const char* name)) 0x54F19E;
shok_EGL_CGLEEntityManager** shok_EGL_CGLEEntityManagerObj = (shok_EGL_CGLEEntityManager**)0x897558;
int(*shok_getEntityIdByScriptName)(const char* str) = (int(*)(const char* str)) 0x576624;
bool(_cdecl* shok_EntityIsSettler)(shok_EGL_CGLEEntity* e) = (bool(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4A2B62;
bool(_cdecl* shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e) = (bool(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x449432;

template<class T>
inline T* shok_vector<T>::Get(int i)
{
	if (i < 0 || i >= Size())
		return nullptr;
	return start + i;
}

template<class T>
int shok_vector<T>::Size()
{
	return end - start;
}

shok_EGL_CGLEEntity* shok_EGL_CGLEEntityManager::GetEntityByNum(int num)
{
	if (EntityCount <= num)
		return nullptr;
	return Entities[num].entity;
}

shok_EGL_CGLEBehavior* EntitySearchBehavior(shok_EGL_CGLEEntity* e, void* vt) {
	for (int i = 0; i < e->BehaviorList.Size(); i++) {
		shok_EGL_CGLEBehavior** b = e->BehaviorList.Get(i);
		if (b != nullptr && (*b) != nullptr && (*b)->vtable == vt)
			return *b;
	}
	return nullptr;
}

shok_GGL_CSoldierBehavior* EntityGetSoldierBehavior(shok_EGL_CGLEEntity* e)
{
	return (shok_GGL_CSoldierBehavior*)EntitySearchBehavior(e, (void*)0x773CC8);
}

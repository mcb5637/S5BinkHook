#include "pch.h"

#include "s5data.h"

shok_EGL_CGLEEntity* (_stdcall* shok_eid2obj)(int id) = (shok_EGL_CGLEEntity * (_stdcall*)(int)) 0x5825B4;
void(_stdcall* shok_SetHighPrecFPU)() = (void(_stdcall *)()) 0x5C8451;
shok_ECS_CManager*** shok_ECS_CManagerObject = (shok_ECS_CManager***)0x0A0344C;
shok_EGL_CGLEGameLogic** shok_EGL_CGLEGameLogicObject = (shok_EGL_CGLEGameLogic**)0x895DAC;
int shok_effectCreatorData_CreatorType_Projectile = 0x774698;
shok_EGL_CGLEEffectManager** shok_EGL_CGLEEffectManagerObject = (shok_EGL_CGLEEffectManager**)0x898144;
void(* shok_entityHurtEntity)(shok_EGL_CGLEEntity* attackerObj, shok_EGL_CGLEEntity* targetObj, int damage) = (void(*)(shok_EGL_CGLEEntity*, shok_EGL_CGLEEntity*, int)) 0x49F358;
void (*shok_logString)(const char* format, const char* string) = (void (*)(const char* format, const char* string)) 0x548268;
shok_BB_CIDManagerEx** shok_BB_CIDManagerExObj = (shok_BB_CIDManagerEx**)0xA0C838;
shok_EGL_CGLEEntityManager** shok_EGL_CGLEEntityManagerObj = (shok_EGL_CGLEEntityManager**)0x897558;
int(*shok_getEntityIdByScriptName)(const char* str) = (int(*)(const char* str)) 0x576624;
bool(_cdecl* shok_EntityIsSettler)(shok_EGL_CGLEEntity* e) = (bool(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4A2B62;
bool(_cdecl* shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e) = (bool(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x449432;
bool(_cdecl* shok_EntityIsResourceDoodad)(shok_EGL_CGLEEntity* e) = (bool(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4B82C7;
int(_cdecl* shok_EntityGetProvidedResourceByID)(int id) = (int(_cdecl*)(int id)) 0x4B8489;

bool(__thiscall* shok_EGL_CGLEEffectManager_IsEffectValid)(shok_EGL_CGLEEffectManager* th, int id) = (bool(__thiscall*)(shok_EGL_CGLEEffectManager*, int))0x4FAABD;
bool shok_EGL_CGLEEffectManager::IsEffectValid(int id)
{
	return shok_EGL_CGLEEffectManager_IsEffectValid(this, id);
}

int(__thiscall* shok_getAnimIdByName)(shok_BB_CIDManagerEx* th, const char* name) = (int(__thiscall*)(shok_BB_CIDManagerEx * th, const char* name)) 0x54F19E;
int shok_BB_CIDManagerEx::GetAnimIdByName(const char* name) {
	return shok_getAnimIdByName(this, name);
}

bool(__thiscall* shok_IsEntityInCategory)(shok_EGL_CGLEEntity* e, int cat) = (bool(__thiscall*)(shok_EGL_CGLEEntity * e, int cat)) 0x57BBEB;
bool shok_EGL_CGLEEntity::IsEntityInCategory(int cat)
{
	return shok_IsEntityInCategory(this, cat);
}


struct shok_vtable_EGL_CGLEGameLogic {
private:
	int u[23];
public:
	int(__thiscall* CreateEffect)(shok_EGL_CGLEGameLogic* th, shok_effectCreatorData* data);
};
int shok_EGL_CGLEGameLogic::CreateEffect(shok_effectCreatorData* data) {
	shok_vtable_EGL_CGLEGameLogic* vt = (shok_vtable_EGL_CGLEGameLogic*)this->vtable;
	return vt->CreateEffect(this, data);
}

struct shok_vtable_ECS_CManager {
private:
	int u, u2;
public:
	void(__thiscall* ReloadCutscene)(shok_ECS_CManager* th, const char* path);
};
void shok_ECS_CManager::ReloadCutscene(const char* path)
{
	((shok_vtable_ECS_CManager*)this->vtable)->ReloadCutscene(this, path);
}

shok_EGL_CGLEEntity* shok_EGL_CGLEEntityManager::GetEntityByNum(int num)
{
	if (EntityCount <= num)
		return nullptr;
	return Entities[num].entity;
}

template<class T>
bool contains(T* data, T search, int num) {
	for (int i = 0; i < num; i++)
		if (data[i] == search)
			return true;
	return false;
}

shok_EGL_CGLEBehavior* shok_EGL_CGLEEntity::SearchBehavior(void** vts, int num)
{
	for (shok_EGL_CGLEBehavior* b : BehaviorList) {
		if (b != nullptr && contains(vts, b->vtable, num))
			return b;
	}
	return nullptr;
}

shok_EGL_CGLEBehavior* shok_EGL_CGLEEntity::SearchBehavior(void* vt) {
	return SearchBehavior(&vt, 1);
}

shok_GGL_CSoldierBehavior* shok_EGL_CGLEEntity::GetSoldierBehavior()
{
	return (shok_GGL_CSoldierBehavior*)SearchBehavior(shok_vtp_GGL_CSoldierBehavior);
}

shok_GGL_CLeaderBehavior* shok_EGL_CGLEEntity::GetLeaderBehavior()
{
	void* data[2] = { shok_vtp_GGL_CLeaderBehavior, shok_vtp_GGL_CBattleSerfBehavior };
	return (shok_GGL_CLeaderBehavior*)SearchBehavior(data, 2);
}

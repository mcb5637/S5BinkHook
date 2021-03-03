#include "pch.h"

#include "s5data.h"

void* (__cdecl* shok_malloc)(size_t t) = (void* (__cdecl*)(size_t)) 0x5C4181;
void(__cdecl* shok_free)(void* p) = (void(__cdecl*)(void* p)) 0x5C2E2D;
void* (__cdecl* shok_new)(size_t t) = (void* (__cdecl*)(size_t))0x5C04FB;

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
shok_EGL_CGLEEntitiesProps** shok_EGL_CGLEEntitiesPropsObj = (shok_EGL_CGLEEntitiesProps**)0x895DB0;

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

shok_GGL_CBehaviorDefaultMovement* shok_EGL_CGLEEntity::GetMovementBehavior()
{
	void* data[4] = { shok_vtp_GGL_CBehaviorDefaultMovement, shok_vtp_GGL_CLeaderMovement, shok_vtp_GGL_CSoldierMovement, shok_vtp_GGL_CSettlerMovement };
	return (shok_GGL_CBehaviorDefaultMovement*)SearchBehavior(data, 4);
}

shok_GGL_CBarrackBehavior* shok_EGL_CGLEEntity::GetBarrackBehavior()
{
	return (shok_GGL_CBarrackBehavior*)SearchBehavior(shok_vtp_GGL_CBarrackBehavior);
}

shok_GGlue_CGlueEntityProps* shok_EGL_CGLEEntitiesProps::GetEntityType(int i)
{
	if (i <= 0 || i >= (int) EntityTypes.size())
		return nullptr;
	return EntityTypes.data() + i;
}

shok_EGL_CGLEBehaviorProps* shok_GGlue_CGlueEntityProps::SearchBehaviorProp(void** vts, int num)
{
	for (shok_EGL_CGLEBehaviorProps* b : BehaviorProps) {
		if (b != nullptr && contains(vts, b->vtable, num))
			return b;
	}
	return nullptr;
}

shok_EGL_CGLEBehaviorProps* shok_GGlue_CGlueEntityProps::SearchBehaviorProp(void* vt)
{
	return SearchBehaviorProp(&vt, 1);
}

shok_GGL_CLeaderBehaviorProps* shok_GGlue_CGlueEntityProps::GetLeaderBehaviorProp()
{
	return (shok_GGL_CLeaderBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CLeaderBehaviorProps);
}

shok_GGL_CSoldierBehaviorProps* shok_GGlue_CGlueEntityProps::GetSoldierBehaviorProp()
{
	return (shok_GGL_CSoldierBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CSoldierBehaviorProps);
}

shok_GGL_CBattleBehaviorProps* shok_GGlue_CGlueEntityProps::GetBattleBehaviorProp()
{
	void* data[5] = { shok_vtp_GGL_CBattleBehaviorProps, shok_vtp_GGL_CLeaderBehaviorProps, shok_vtp_GGL_CSoldierBehaviorProps, shok_vtp_GGL_CSerfBattleBehaviorProps, shok_vtp_GGL_CBattleSerfBehaviorProps };
	return (shok_GGL_CBattleBehaviorProps*)SearchBehaviorProp(data, 5);
}

bool shok_GGlue_CGlueEntityProps::IsSettlerType()
{
	return LogicProps->vtable == shok_vtp_GGL_CGLSettlerProps;
}

bool shok_GGlue_CGlueEntityProps::IsBuildingType()
{
	void* data[2] = { shok_vtp_GGL_CGLBuildingProps, shok_vtp_GGL_CBridgeProperties };
	return contains(data, LogicProps->vtable, 2);
}

bool shok_EGL_CGLEEntity::IsMovingEntity()
{
	void* data[4] = { shok_vtp_EGL_CMovingEntity, shok_vtp_GGL_CEvadingEntity, shok_vtp_GGL_CSettler, shok_vtp_GGL_CAnimal };
	return contains(data, vtable, 4);
}

shok_GGL_CCamouflageBehavior* shok_EGL_CGLEEntity::GetCamoAbilityBehavior()
{
	void* data[2] = { shok_vtp_GGL_CCamouflageBehavior, shok_vtp_GGL_CThiefCamouflageBehavior };
	return (shok_GGL_CCamouflageBehavior*)SearchBehavior(data, 2);
}

shok_GGL_CThiefCamouflageBehavior* shok_EGL_CGLEEntity::GetCamoThiefBehavior()
{
	return (shok_GGL_CThiefCamouflageBehavior*)SearchBehavior(shok_vtp_GGL_CThiefCamouflageBehavior);
}

shok_GGL_CHeroBehavior* shok_EGL_CGLEEntity::GetHeroBehavior()
{
	return (shok_GGL_CHeroBehavior*)SearchBehavior(shok_vtp_GGL_CHeroBehavior);
}

shok_GGL_CLimitedLifespanBehavior* shok_EGL_CGLEEntity::GetLimitedLifespanBehavior()
{
	return (shok_GGL_CLimitedLifespanBehavior*)SearchBehavior(shok_vtp_GGL_CLimitedLifespanBehavior);
}

shok_GGL_CLimitedLifespanBehaviorProps* shok_GGlue_CGlueEntityProps::GetLimitedLifespanBehaviorProp()
{
	return (shok_GGL_CLimitedLifespanBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CLimitedLifespanBehaviorProps);
}

shok_GGL_CAffectMotivationBehaviorProps* shok_GGlue_CGlueEntityProps::GetAffectMotivationBehaviorProp()
{
	return (shok_GGL_CAffectMotivationBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CAffectMotivationBehaviorProps);
}

shok_GGL_CGLAnimationBehaviorExProps* shok_GGlue_CGlueEntityProps::GetAnimationExProp()
{
	return (shok_GGL_CGLAnimationBehaviorExProps*)SearchBehaviorProp(shok_vtp_GGL_CGLAnimationBehaviorExProps);
}

shok_GGL_CAutoCannonBehaviorProps* shok_GGlue_CGlueEntityProps::GetAutoCannonProp()
{
	return (shok_GGL_CAutoCannonBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CAutoCannonBehaviorProps);
}

bool shok_GGlue_CGlueEntityProps::IsOfCategory(int cat)
{
	for (int i : LogicProps->Categories)
		if (i == cat)
			return true;
	return false;
}

shok_GGL_CThiefBehaviorProperties* shok_GGlue_CGlueEntityProps::GetThiefBehaviorProp()
{
	return (shok_GGL_CThiefBehaviorProperties*)SearchBehaviorProp(shok_vtp_GGL_CThiefBehaviorProperties);
}

shok_GGL_CCamouflageBehaviorProps* shok_GGlue_CGlueEntityProps::GetCamouflageBehaviorProp()
{
	return (shok_GGL_CCamouflageBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CCamouflageBehaviorProps);
}

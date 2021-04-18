#include "pch.h"

#include "s5data.h"
#include <stdexcept>

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
void (*shok_logString)(const char* format, ...) = (void (*)(const char* format, ...)) 0x548268;
shok_BB_CIDManagerEx** shok_BB_CIDManagerExObj = (shok_BB_CIDManagerEx**)0xA0C838;
shok_EGL_CGLEEntityManager** shok_EGL_CGLEEntityManagerObj = (shok_EGL_CGLEEntityManager**)0x897558;
int(*shok_getEntityIdByScriptName)(const char* str) = (int(*)(const char* str)) 0x576624;
int(_cdecl* shok_EntityIsSettler)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4A2B62;
int(_cdecl* shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x449432;
int(_cdecl* shok_EntityIsResourceDoodad)(shok_EGL_CGLEEntity* e) = (int(_cdecl*)(shok_EGL_CGLEEntity * e)) 0x4B82C7;
int(_cdecl* shok_EntityGetProvidedResourceByID)(int id) = (int(_cdecl*)(int id)) 0x4B8489;
shok_EGL_CGLEEntitiesProps** shok_EGL_CGLEEntitiesPropsObj = (shok_EGL_CGLEEntitiesProps**)0x895DB0;
shok_damageClassHolder** shok_DamageClassHolderObj = (shok_damageClassHolder**)0x85A3DC;
shok_GGL_CLogicProperties** shok_GGL_CLogicPropertiesObj = (shok_GGL_CLogicProperties**)0x8758240;
shok_GGL_CPlayerAttractionProps** shok_GGL_CPlayerAttractionPropsObj = (shok_GGL_CPlayerAttractionProps**)0x866A80;
shok_GGL_CGLGameLogic** shok_GGL_CGLGameLogicObj = (shok_GGL_CGLGameLogic**)0x85A3A0;
void(__cdecl* shok_entityDealAOEDamage)(shok_EGL_CGLEEntity* attacker, shok_position* center, float range, int damage, int player, int damageclass) = (void(__cdecl*)(shok_EGL_CGLEEntity*, shok_position*, float, int, int, int))0x49F82A;
lua_State** shok_luastate_game = (lua_State**)0x853A9C;
shok_GGUI_CManager* (*shok_GetGuiManager)() = (shok_GGUI_CManager * (*)()) 0x525622;
bool(_cdecl* shok_entityIsDead)(int id) = (bool(_cdecl*)(int)) 0x44B096;
bool(_cdecl* shok_canPlaceBuilding)(int entitytype, int player, shok_position* pos, float rotation, int buildOnId) = (bool(_cdecl*)(int entitytype, int player, shok_position * pos, float rotation, int buildOnId)) 0x4B442C;
int(_cdecl* shok_entityChangePlayer)(int entityid, int player) = (int(_cdecl*)(int, int)) 0x49A6A7;
int(__stdcall* shok_loadBuffer)(lua_State* L, const char* buff, size_t bufflen, const char* name) = (int(__stdcall*)(lua_State*, const char*, size_t, const char*)) 0x59BE57;

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


int(__thiscall* CreateEffectHookedOrig)(shok_EGL_CGLEGameLogic* th, shok_effectCreatorData* data) = nullptr;
void(*CreateEffectHookCallback)(int id, void* ret) = nullptr;
struct shok_vtable_EGL_CGLEGameLogic {
private:
	int u[22];
public:
	int(__thiscall* CreateEntity)(shok_EGL_CGLEGameLogic* th, shok_EGL_CGLEEntityCreator* data, int i); // 22
	int(__thiscall* CreateEffect)(shok_EGL_CGLEGameLogic* th, shok_effectCreatorData* data); // 23
};
int shok_EGL_CGLEGameLogic::CreateEffect(shok_effectCreatorData* data) {
	shok_vtable_EGL_CGLEGameLogic* vt = (shok_vtable_EGL_CGLEGameLogic*)this->vtable;
	return vt->CreateEffect(this, data);
}
int __fastcall CreateEffectHook(shok_EGL_CGLEGameLogic* th, int _, shok_effectCreatorData* data)
{
	void** ebp_var = (void**) 1;
	_asm {
		mov ebp_var, ebp
	}
	int id = CreateEffectHookedOrig(th, data);
	if (CreateEffectHookCallback)
		CreateEffectHookCallback(id, ebp_var[1]);
	return id;
}
void shok_EGL_CGLEGameLogic::HookCreateEffect()
{
	if (CreateEffectHookedOrig)
		return;
	shok_vtable_EGL_CGLEGameLogic* vt = (shok_vtable_EGL_CGLEGameLogic*)this->vtable;
	CreateEffectHookedOrig = vt->CreateEffect;
	vt->CreateEffect = (int(__thiscall *)(shok_EGL_CGLEGameLogic * th, shok_effectCreatorData * data)) &CreateEffectHook;
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

shok_GGL_CLeaderBehaviorProps* shok_GGlue_CGlueEntityProps::GetLeaderBehaviorProps()
{
	return (shok_GGL_CLeaderBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CLeaderBehaviorProps);
}

shok_GGL_CSoldierBehaviorProps* shok_GGlue_CGlueEntityProps::GetSoldierBehaviorProps()
{
	return (shok_GGL_CSoldierBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CSoldierBehaviorProps);
}

shok_GGL_CBattleBehaviorProps* shok_GGlue_CGlueEntityProps::GetBattleBehaviorProps()
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

shok_GGL_CLimitedLifespanBehaviorProps* shok_GGlue_CGlueEntityProps::GetLimitedLifespanBehaviorProps()
{
	return (shok_GGL_CLimitedLifespanBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CLimitedLifespanBehaviorProps);
}

shok_GGL_CAffectMotivationBehaviorProps* shok_GGlue_CGlueEntityProps::GetAffectMotivationBehaviorProps()
{
	return (shok_GGL_CAffectMotivationBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CAffectMotivationBehaviorProps);
}

shok_GGL_CGLAnimationBehaviorExProps* shok_GGlue_CGlueEntityProps::GetAnimationExProps()
{
	return (shok_GGL_CGLAnimationBehaviorExProps*)SearchBehaviorProp(shok_vtp_GGL_CGLAnimationBehaviorExProps);
}

shok_GGL_CAutoCannonBehaviorProps* shok_GGlue_CGlueEntityProps::GetAutoCannonProps()
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

shok_GGL_CThiefBehaviorProperties* shok_GGlue_CGlueEntityProps::GetThiefBehaviorProps()
{
	return (shok_GGL_CThiefBehaviorProperties*)SearchBehaviorProp(shok_vtp_GGL_CThiefBehaviorProperties);
}

shok_GGL_CCamouflageBehaviorProps* shok_GGlue_CGlueEntityProps::GetCamouflageBehaviorProps()
{
	return (shok_GGL_CCamouflageBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CCamouflageBehaviorProps);
}

shok_GGL_CCircularAttackProps* shok_GGlue_CGlueEntityProps::GetCircularAttackBehaviorProps()
{
	return (shok_GGL_CCircularAttackProps*)SearchBehaviorProp(shok_vtp_GGL_CCircularAttackProps);
}

shok_GGL_CShurikenAbilityProps* shok_GGlue_CGlueEntityProps::GetShurikenBehaviorProps()
{
	return (shok_GGL_CShurikenAbilityProps*)SearchBehaviorProp(shok_vtp_GGL_CShurikenAbilityProps);
}

shok_GGL_CSniperAbilityProps* shok_GGlue_CGlueEntityProps::GetSniperBehaviorProps()
{
	return (shok_GGL_CSniperAbilityProps*)SearchBehaviorProp(shok_vtp_GGL_CSniperAbilityProps);
}

shok_GGL_CRangedEffectAbilityProps* shok_GGlue_CGlueEntityProps::GetRangedEffectBehaviorProps()
{
	return (shok_GGL_CRangedEffectAbilityProps*)SearchBehaviorProp(shok_vtp_GGL_CRangedEffectAbilityProps);
}

bool shok_GGlue_CGlueEntityProps::IsCEntityProperties()
{
	return LogicProps->vtable == shok_vtp_GGL_CEntityProperties;
}

shok_GGL_CWorkerBehaviorProps* shok_GGlue_CGlueEntityProps::GetWorkerBehaviorProps()
{
	return (shok_GGL_CWorkerBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CWorkerBehaviorProps);
}

shok_GGL_CWorkerBehavior* shok_EGL_CGLEEntity::GetWorkerBehavior()
{
	return (shok_GGL_CWorkerBehavior*)SearchBehavior(shok_vtp_GGL_CWorkerBehavior);
}

shok_GGL_CMarketBehavior* shok_EGL_CGLEEntity::GetMarketBehavior()
{
	return (shok_GGL_CMarketBehavior*)SearchBehavior(shok_vtp_GGL_CMarketBehavior);
}

shok_GGlue_CGlueEntityProps* shok_EGL_CGLEEntity::GetEntityType()
{
	return (*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(EntityType);
}

shok_GGL_CThiefBehavior* shok_EGL_CGLEEntity::GetThiefBehavior()
{
	return (shok_GGL_CThiefBehavior*)SearchBehavior(shok_vtp_GGL_CThiefBehavior);
}

int shok_EGL_CGLEEntity::EventGetIntById(int id)
{
	shok_event_data_EGL_CEventGetValue_int_1211121895 d = shok_event_data_EGL_CEventGetValue_int_1211121895();
	d.id = id;
	shok_vtable_EGL_CGLEEntity* vt = (shok_vtable_EGL_CGLEEntity*)vtable;
	vt->FireEvent(this, &d);
	return d.result;
}

int shok_EGL_CGLEEntity::EventGetDamage()
{
	return EventGetIntById(0x1503D);
}
int shok_EGL_CGLEEntity::EventGetArmor()
{
	return EventGetIntById(0x1503E);
}
int shok_EGL_CGLEEntity::EventGetMaxWorktime()
{
	return EventGetIntById(0x1301A);
}

shok_GGL_CBattleBehavior* shok_EGL_CGLEEntity::GetBattleBehavior()
{
	void* v[5] = { shok_vtp_GGL_CBattleBehavior, shok_vtp_GGL_CLeaderBehavior, shok_vtp_GGL_CSoldierBehavior, shok_vtp_GGL_CSerfBattleBehavior, shok_vtp_GGL_CBattleSerfBehavior };
	return (shok_GGL_CBattleBehavior*)SearchBehavior(v, 5);
}

float(__thiscall* battleBehaviorGetMaxRange)(shok_GGL_CBattleBehavior*) = (float(__thiscall*)(shok_GGL_CBattleBehavior * id))0x50AB43;
float shok_GGL_CBattleBehavior::GetMaxRange()
{
	return battleBehaviorGetMaxRange(this);
}


shok_GGL_CPlayerStatus* (__thiscall* shok_GGL_CGLGameLogic_GetPlayer)(shok_GGL_CPlayerStatus** pl, int p) = (shok_GGL_CPlayerStatus* (__thiscall*) (shok_GGL_CPlayerStatus **, int)) 0x4A91BC;
shok_GGL_CPlayerStatus* shok_GGL_CGLGameLogic::GetPlayer(int i)
{
	return shok_GGL_CGLGameLogic_GetPlayer(this->players, i);
}

float shok_EGL_CGLEEntity::GetExploration()
{
	shok_vtable_EGL_CGLEEntity* vt = (shok_vtable_EGL_CGLEEntity*)vtable;
	return vt->GetExploration(this);
}

float shok_GGL_CBehaviorDefaultMovement::GetMovementSpeed()
{
	shok_vtable_GGL_CBehaviorDefaultMovement* vt = (shok_vtable_GGL_CBehaviorDefaultMovement*)vtable;
	return vt->GetSpeed(this);
}

bool shok_EGL_CEffect::IsCannonBallEffect()
{
	return vtable == shok_vtp_GGL_CCannonBallEffect;
}

bool shok_EGL_CEffect::IsArrowEffect()
{
	return vtable == shok_vtp_GGL_CArrowEffect;
}

shok_EGL_CEffect* (__thiscall* shok_effid2obj)(shok_EGL_CGLEEffectManager* th, int id) = (shok_EGL_CEffect * (__thiscall*)(shok_EGL_CGLEEffectManager*, int))0x04FAAE3;
shok_EGL_CEffect* shok_EGL_CGLEEffectManager::GetEffectById(int id)
{
	return shok_effid2obj(this, id);
}

void logAdress(const char* name, void* adr)
{
	shok_logString("%s %x\n", name, adr);
}

struct shok_vtable_EGL_CFlyingEffect {
	PADDINGI(9)
	int(__thiscall* OnHit)(shok_EGL_CFlyingEffect* th);
};

void (*FlyingEffectOnHitCallback2)(shok_EGL_CFlyingEffect* eff, bool post) = nullptr;
void (*FlyingEffectOnHitCallback)(shok_EGL_CFlyingEffect* eff) = nullptr;
int(__thiscall* CannonBallOnHit)(shok_EGL_CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(shok_EGL_CFlyingEffect* th);
int __fastcall CannonBallOnHitHook(shok_EGL_CFlyingEffect* th) {
	if (FlyingEffectOnHitCallback)
		FlyingEffectOnHitCallback(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, false);
	int i = CannonBallOnHit(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, true);
	// TODO remove rewriting vtable after kimichura removes the reset of it
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CCannonBallEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CArrowEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
	return i;
}
int(__thiscall* ArrowOnHit)(shok_EGL_CFlyingEffect* th) = nullptr;
int __fastcall ArrowOnHitHook(shok_EGL_CFlyingEffect* th) {
	if (FlyingEffectOnHitCallback)
		FlyingEffectOnHitCallback(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, false);
	int i = ArrowOnHit(th);
	if (FlyingEffectOnHitCallback2)
		FlyingEffectOnHitCallback2(th, true);
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CCannonBallEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	((shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CArrowEffect)->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
	return i;
}
void shok_EGL_CFlyingEffect::HookOnHit()
{
	if (CannonBallOnHit)
		return;
	shok_vtable_EGL_CFlyingEffect* vt = (shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CCannonBallEffect;
	CannonBallOnHit = vt->OnHit;
	vt->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & CannonBallOnHitHook;
	vt = (shok_vtable_EGL_CFlyingEffect*)shok_vtp_GGL_CArrowEffect;
	ArrowOnHit = vt->OnHit;
	vt->OnHit = (int(__thiscall*)(shok_EGL_CFlyingEffect*)) & ArrowOnHitHook;
}

struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(shok_BB_IPostEvent* th, shok_BB_CEvent* ev);
};
void(__stdcall* PostEventOrig)(shok_BB_IPostEvent* th, shok_BB_CEvent* ev) = nullptr;
shok_vtable_BB_IPostEvent* BB_IPostEvent_vtableHooked = nullptr;
bool(*PostEventCallback)(shok_BB_CEvent* ev) = nullptr;
void __stdcall PostEventHook(shok_BB_IPostEvent* th, shok_BB_CEvent* ev) {
	if (PostEventCallback)
		if (PostEventCallback(ev))
			return;
	PostEventOrig(th, ev);
}
void shok_GGUI_CManager::HackPostEvent()
{
	if (PostEventOrig) {
		BB_IPostEvent_vtableHooked->PostEvent = PostEventOrig;
	}
	BB_IPostEvent_vtableHooked = (shok_vtable_BB_IPostEvent*)PostEvent->vtable;
	PostEventOrig = BB_IPostEvent_vtableHooked->PostEvent;
	BB_IPostEvent_vtableHooked->PostEvent = (void(__stdcall*)(shok_BB_IPostEvent * th, shok_BB_CEvent* ev)) & PostEventHook;
}

shok_technology* shok_GGL_CGLGameLogic::GetTech(int i)
{
	try {
		return TechList->TechList.at(i-1);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

void shok_EGL_CMovingEntity::AttackMove(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x1502E; // attack move
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	//shok_event_data_EGL_CEventValue_bool_703333479 e2 = shok_event_data_EGL_CEventValue_bool_703333479();
	//e2.id = 0x1503B; // set target rotation state
	//e2.b = 0;
	//((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &e2);
	TargetRotationValid = 0;
}

void shok_EGL_CMovingEntity::AttackEntity(int targetId)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x15004;
	ev.entityId = targetId;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::Move(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x11002; // move
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	TargetRotationValid = 0;
}

int(__thiscall* shok_GGL_CPlayerStatus_getDiploState)(int* d, int p) = (int(__thiscall*)(int* d, int p)) 0x4B4D5B;
int shok_GGL_CPlayerStatus::GetDiploStateTo(int p)
{
	return shok_GGL_CPlayerStatus_getDiploState(&DiploData, p);
}

int shok_EGL_CGLEEntity::EventLeaderGetCurrentCommand()
{
	if (GetLeaderBehavior() == nullptr)
		return -1;
	return EventGetIntById(0x1502D);
}

int shok_EGL_CGLEGameLogic::GetTimeMS()
{
	return InGameTime[0] * InGameTime[1];
}

void shok_EGL_CMovingEntity::HeroAbilitySendHawk(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x16002;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityInflictFear()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x16026;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceBomb(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x15033;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityPlaceCannon(shok_position& p, int FoundationType, int CannonType)
{
	shok_event_data_GGL_CEventPositionAnd2EntityTypes ev = shok_event_data_GGL_CEventPositionAnd2EntityTypes();
	ev.id = 0x1600A;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	ev.EntityType1 = FoundationType;
	ev.EntityType2 = CannonType;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityRangedEffect()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x1601C;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityCircularAttack()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x16022;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilitySummon()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x1601A;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityConvert(int target)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x16027;
	ev.entityId = target;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilitySnipe(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x1602D;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::HeroAbilityShuriken(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x1602F;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

shok_GGL_CHeroHawkBehavior* shok_EGL_CGLEEntity::GetHeroHawkBehavior()
{
	return (shok_GGL_CHeroHawkBehavior*)SearchBehavior(shok_vtp_GGL_CHeroHawkBehavior);
}

shok_GGL_CHeroHawkBehaviorProps* shok_GGlue_CGlueEntityProps::GetHeroHawkBehaviorProps()
{
	return (shok_GGL_CHeroHawkBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CHeroHawkBehaviorProps);
}

shok_GGL_CInflictFearAbilityProps* shok_GGlue_CGlueEntityProps::GetInflictFearBehaviorProps()
{
	return (shok_GGL_CInflictFearAbilityProps*)SearchBehaviorProp(shok_vtp_GGL_CInflictFearAbilityProps);
}

shok_GGL_CInflictFearAbility* shok_EGL_CGLEEntity::GetInflictFearBehavior()
{
	return (shok_GGL_CInflictFearAbility*)SearchBehavior(shok_vtp_GGL_CInflictFearAbility);
}

shok_GGL_CBombPlacerBehavior* shok_EGL_CGLEEntity::GetBombPlacerBehavior()
{
	return (shok_GGL_CBombPlacerBehavior*)SearchBehavior(shok_vtp_GGL_CBombPlacerBehavior);
}

shok_GGL_CHeroAbilityProps* shok_GGlue_CGlueEntityProps::GetBombPlacerBehaviorProps()
{
	return (shok_GGL_CHeroAbilityProps*)SearchBehaviorProp(shok_vtp_GGL_CHeroAbilityProps);
}

shok_GGL_CCannonBuilderBehaviorProps* shok_GGlue_CGlueEntityProps::GetCannonBuilderBehaviorProps()
{
	return (shok_GGL_CCannonBuilderBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CCannonBuilderBehaviorProps);
}

shok_GGL_CCannonBuilderBehavior* shok_EGL_CGLEEntity::GetCannonBuilderBehavior()
{
	return (shok_GGL_CCannonBuilderBehavior*)SearchBehavior(shok_vtp_GGL_CCannonBuilderBehavior);
}

shok_GGL_CRangedEffectAbility* shok_EGL_CGLEEntity::GetRangedEffectBehavior()
{
	return (shok_GGL_CRangedEffectAbility*)SearchBehavior(shok_vtp_GGL_CRangedEffectAbility);
}

shok_GGL_CCircularAttack* shok_EGL_CGLEEntity::GetCircularAttackBehavior()
{
	return (shok_GGL_CCircularAttack*)SearchBehavior(shok_vtp_GGL_CCircularAttack);
}

shok_GGL_CSummonBehavior* shok_EGL_CGLEEntity::GetSummonBehavior()
{
	return (shok_GGL_CSummonBehavior*)SearchBehavior(shok_vtp_GGL_CSummonBehavior);
}

shok_GGL_CSummonBehaviorProps* shok_GGlue_CGlueEntityProps::GetSummonBehaviorProps()
{
	return (shok_GGL_CSummonBehaviorProps*)SearchBehaviorProp(shok_vtp_GGL_CSummonBehaviorProps);
}

shok_GGL_CConvertSettlerAbilityProps* shok_GGlue_CGlueEntityProps::GetConvertSettlersBehaviorProps()
{
	return (shok_GGL_CConvertSettlerAbilityProps*)SearchBehaviorProp(shok_vtp_GGL_CConvertSettlerAbilityProps);
}

shok_GGL_CConvertSettlerAbility* shok_EGL_CGLEEntity::GetConvertSettlerBehavior()
{
	return (shok_GGL_CConvertSettlerAbility*)SearchBehavior(shok_vtp_GGL_CConvertSettlerAbility);
}

bool ArePlayersHostile(int p1, int p2)
{
	return (*shok_GGL_CGLGameLogicObj)->GetPlayer(p1)->GetDiploStateTo(p2) == shok_DIPLOSTATE_HOSTILE;
}

shok_GGL_CSniperAbility* shok_EGL_CGLEEntity::GetSniperBehavior()
{
	return (shok_GGL_CSniperAbility*)SearchBehavior(shok_vtp_GGL_CSniperAbility);
}

shok_GGL_CShurikenAbility* shok_EGL_CGLEEntity::GetShurikenBehavior()
{
	return (shok_GGL_CShurikenAbility*)SearchBehavior(shok_vtp_GGL_CShurikenAbility);
}

void shok_EGL_CMovingEntity::ThiefSabotage(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10107;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefDefuse(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x1010E;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefStealFrom(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10102;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ThiefSecureGoods(int tid)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10103;
	ev.entityId = tid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutBinoculars(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x11102;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutFindResource()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x11104;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::ScoutPlaceTorch(shok_position& p)
{
	shok_event_data_EGL_CEventPosition ev = shok_event_data_EGL_CEventPosition();
	ev.id = 0x11103;
	ev.pos.X = p.X;
	ev.pos.Y = p.Y;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

shok_GGL_CKegPlacerBehaviorProperties* shok_GGlue_CGlueEntityProps::GetKegPlacerBehaviorProps()
{
	return (shok_GGL_CKegPlacerBehaviorProperties*)SearchBehaviorProp(shok_vtp_GGL_CKegPlacerBehaviorProperties);
}

shok_GGL_CKegPlacerBehavior* shok_EGL_CGLEEntity::GetKegPlacerBehavior()
{
	return (shok_GGL_CKegPlacerBehavior*)SearchBehavior(shok_vtp_GGL_CKegPlacerBehavior);
}

shok_GGL_CAbilityScoutBinocularProps* shok_GGlue_CGlueEntityProps::GetBinocularBehaviorProps()
{
	return (shok_GGL_CAbilityScoutBinocularProps*)SearchBehaviorProp(shok_vtp_GGL_CAbilityScoutBinocularProps);
}

shok_GGL_CAbilityScoutBinocular* shok_EGL_CGLEEntity::GetBinocularBehavior()
{
	return (shok_GGL_CAbilityScoutBinocular*)SearchBehavior(shok_vtp_GGL_CAbilityScoutBinocular);
}

shok_GGL_CTorchPlacerBehavior* shok_EGL_CGLEEntity::GetTorchPlacerBehavior()
{
	return (shok_GGL_CTorchPlacerBehavior*)SearchBehavior(shok_vtp_GGL_CTorchPlacerBehavior);
}

shok_GGL_CTorchPlacerBehaviorProperties* shok_GGlue_CGlueEntityProps::GetTorchPlacerBehaviorProps()
{
	return (shok_GGL_CTorchPlacerBehaviorProperties*)SearchBehaviorProp(shok_vtp_GGL_CTorchPlacerBehaviorProperties);
}

shok_GGL_CPointToResourceBehaviorProperties* shok_GGlue_CGlueEntityProps::GetPointToResBehaviorProps()
{
	return (shok_GGL_CPointToResourceBehaviorProperties*)SearchBehaviorProp(shok_vtp_GGL_CPointToResourceBehaviorProperties);
}

shok_GGL_CPointToResourceBehavior* shok_EGL_CGLEEntity::GetPointToResBehavior()
{
	return (shok_GGL_CPointToResourceBehavior*)SearchBehavior(shok_vtp_GGL_CPointToResourceBehavior);
}

shok_GGL_CKegBehavior* shok_EGL_CGLEEntity::GetKegBehavior()
{
	return (shok_GGL_CKegBehavior*)SearchBehavior(shok_vtp_GGL_CKegBehavior);
}

int shok_EGL_CGLEGameLogic::GetTick()
{
	return InGameTime[0];
}

void shok_position::FloorToBuildingPlacement()
{
	X = std::floorf(X / 100) * 100;
	Y = std::floorf(Y / 100) * 100;
}

float shok_position::GetDistanceSquaredTo(shok_position& p)
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	return (dx * dx + dy * dy);
}

bool shok_position::IsInRange(shok_position& p, float range)
{
	return GetDistanceSquaredTo(p) <= (range * range);
}

float shok_position::GetAngleBetween(shok_position& p)
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	if (dx == 0 && dy == 0)
		return 0;
	float a = std::asinf(std::fabsf(dx) / (std::sqrtf(dx * dx + dy * dy)));
	a = (float)rad2deg((double)a);
	if (dx >= 0 && dy > 0)
		a = 270 - a;
	else if (dx < 0 && dy > 0)
		a = 270 + a;
	else if (dx < 0 && dy <= 0)
		a = 90 - a;
	else if (dx >= 0 && dy <= 0)
		a = 90 + a;
	return a;
}

bool shok_EGL_CGLEEntity::IsSettler()
{
	return shok_EntityIsSettler(this);
}

bool shok_EGL_CMovingEntity::IsMoving()
{
	shok_event_data_EGL_CEventValue_bool_703333479 d = shok_event_data_EGL_CEventValue_bool_703333479();
	d.id = 0x20009;
	d.b = false;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &d);
	return d.b;
}

void RedirectCall(void* call, void* redirect) {
	byte* opcode = (byte*)call;
	if (*opcode != 0xE8) { // call
		DEBUGGER_BREAK
	}
	int* adr = (int*)(opcode+1);
	*adr = ((int)redirect) - ((int)(adr + 1)); // address relative to next instruction
}
void WriteJump(void* adr, void* toJump) {
	byte* opcode = (byte*)adr;
	*opcode = 0xE9; // jmp
	int* a = (int*)(opcode + 1);
	*a = ((int)toJump) - ((int)(a + 1)); // address relative to next instruction
}

void (*Hero6ConvertHookCb)(int id, int pl, bool post, int converter) = nullptr;
int _cdecl hero6convertchangeplayer(int id, int pl) {
	shok_EGL_CGLEEntity* c = (shok_EGL_CGLEEntity*)1;
	_asm { mov c, esi }
	if (Hero6ConvertHookCb)
		Hero6ConvertHookCb(id, pl, false, c->EntityId);
	int r = shok_entityChangePlayer(id, pl);
	if (Hero6ConvertHookCb)
		Hero6ConvertHookCb(id, pl, true, c->EntityId);
	return r;
}
void HookHero6Convert()
{
	RedirectCall((void*)0x4FCD26, &hero6convertchangeplayer);
}

bool shok_GGL_CRangedEffectAbilityProps::IsDefensive()
{
	return (AffectsHostiles && DamageFactor > 0 && DamageFactor < 1) || ((AffectsFriends || AffectsOwn) && ArmorFactor > 1);
}
bool shok_GGL_CRangedEffectAbilityProps::IsAggressive()
{
	return (AffectsHostiles && ArmorFactor > 0 && ArmorFactor < 1) || ((AffectsFriends || AffectsOwn) && DamageFactor > 1);
}

bool shok_GGL_CRangedEffectAbilityProps::IsHeal()
{
	return (AffectsFriends || AffectsOwn) && HealthRecoveryFactor > 0;
}

bool operator<(Attachment a, Attachment b) {
	if (a.AttachmentType == b.AttachmentType)
		return a.EntityId < b.EntityId;
	return a.AttachmentType < b.AttachmentType;
}

int shok_EGL_CGLEEntity::GetFirstAttachedToMe(int attachmentId)
{
	Attachment* r = EntitiesAttachedToMe.GetFirstMatch([attachmentId](Attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}

int shok_EGL_CGLEEntity::GetFirstAttachedEntity(int attachmentId)
{
	Attachment* r = AttachedToEntities.GetFirstMatch([attachmentId](Attachment* a) {return a->AttachmentType == attachmentId; });
	return r == nullptr ? 0 : r->EntityId;
}

int ResetCamoIgnoreIfNotEntity = 0;
void __fastcall camo_behaviorReset(shok_GGL_CCamouflageBehavior* th, int _, int a) {
	if (ResetCamoIgnoreIfNotEntity == 0 || th->EntityId == ResetCamoIgnoreIfNotEntity)
		th->InvisibilityRemaining = 0;
}
void HookResetCamo() {
	WriteJump((void*)0x5011DF, &camo_behaviorReset);
}

void shok_EGL_CMovingEntity::HoldPosition()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x1502F;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CMovingEntity::Defend()
{
	shok_event_data_BB_CEvent ev = shok_event_data_BB_CEvent();
	ev.id = 0x15032;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void shok_EGL_CGLEEntity::ClearAttackers()
{
	std::vector<shok_GGL_CSettler*> tomove = std::vector<shok_GGL_CSettler*>();
	EntitiesAttachedToMe.ForAll([&tomove](Attachment* a) {
		if (a->AttachmentType == AttachmentType_ATTACKER_COMMAND_TARGET || a->AttachmentType == AttachmentType_LEADER_TARGET || a->AttachmentType == AttachmentType_FOLLOWER_FOLLOWED) {
			shok_EGL_CGLEEntity* at = shok_eid2obj(a->EntityId);
			if (shok_EntityIsSettler(at)) {
				shok_GGL_CSettler* s = (shok_GGL_CSettler*)at;
				tomove.emplace_back(s);
			}
		}
		});
	for (shok_GGL_CSettler* s : tomove)
		s->Defend();
}

int(__thiscall* activateCamoOrig)(shok_GGL_CCamouflageBehavior* th) = (int(__thiscall*)(shok_GGL_CCamouflageBehavior*)) 0x501561;
void (*CamoActivateCb)(shok_GGL_CCamouflageBehavior* th);
int __fastcall camoActivateHook(shok_GGL_CCamouflageBehavior* th) {
	int i = activateCamoOrig(th);
	if (CamoActivateCb)
		CamoActivateCb(th);
	return i;
}
void HookCamoActivate()
{
	RedirectCall((void*)0x4D51A4, &camoActivateHook);
	RedirectCall((void*)0x50163A, &camoActivateHook);
}

void shok_GGL_CBuilding::CommandBuildCannon(int entitytype)
{
	shok_event_data_EGL_CEventValue_int_27574121 e = shok_event_data_EGL_CEventValue_int_27574121();
	e.id = 0x17016;
	e.i = entitytype;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &e);
}

shok_GGL_CFoundryBehavior* shok_EGL_CGLEEntity::GetFoundryBehavior()
{
	return (shok_GGL_CFoundryBehavior*)SearchBehavior(shok_vtp_GGL_CFoundryBehavior);
}

bool shok_GGL_CCamouflageBehavior::IsThiefCamoBehavior()
{
	return vtable == shok_vtp_GGL_CThiefCamouflageBehavior;
}

void shok_EGL_CMovingEntity::HeroAbilityActivateCamoflage()
{
	shok_event_data_BB_CEvent e = shok_event_data_BB_CEvent();
	e.id = 0x16015;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &e);
}

struct shok_BB_CFileStreamEx {
	int vtable = 0x761C60;
	int x = 0;
};
bool(__thiscall* shok_BB_CFileStreamEx_OpenFile)(shok_BB_CFileStreamEx* th, const char* name, int unk) = (bool(__thiscall*)(shok_BB_CFileStreamEx*, const char*, int)) 0x54924D;
size_t(__stdcall* shok_BB_CFileStreamEx_GetSize)(shok_BB_CFileStreamEx* th) = (size_t(__stdcall*)(shok_BB_CFileStreamEx*)) 0x549140;
int(__stdcall* shok_BB_CFileStreamEx_ReadToBuffer)(shok_BB_CFileStreamEx* th, void* buff, size_t buffsiz) = (int(__stdcall*)(shok_BB_CFileStreamEx*, void*, size_t)) 0x5491A8;
void(__thiscall* shok_BB_CFileStreamEx_Close)(shok_BB_CFileStreamEx* th) = (void(__thiscall*)(shok_BB_CFileStreamEx*))0x54920A;
int(__thiscall* shok_BB_CFileStreamEx_dtor)(shok_BB_CFileStreamEx* th) = (int(__thiscall*)(shok_BB_CFileStreamEx*))0x549215;

const char* ReadFileToString(const char* name, size_t* size)
{
	char* buff = nullptr;
	try
	{
		shok_BB_CFileStreamEx filestr = shok_BB_CFileStreamEx();
		if (shok_BB_CFileStreamEx_OpenFile(&filestr, name, 0x10113)) {
			size_t s = shok_BB_CFileStreamEx_GetSize(&filestr);
			if (size)
				*size = s;
			if (s > 0) {
				buff = new char[s + 1];
				memset(buff, 0, s + 1);
				shok_BB_CFileStreamEx_ReadToBuffer(&filestr, buff, s);
			}
			shok_BB_CFileStreamEx_Close(&filestr);
		}
		shok_BB_CFileStreamEx_dtor(&filestr);
	}
	catch (...)
	{
		if (buff)
			delete[] buff;
		return nullptr;
	}
	return buff;
}

bool DoesFileExist(const char* name)
{

	bool r = false;
	try
	{
		shok_BB_CFileStreamEx filestr = shok_BB_CFileStreamEx();
		if (shok_BB_CFileStreamEx_OpenFile(&filestr, name, 0x10113)) {
			size_t s = shok_BB_CFileStreamEx_GetSize(&filestr);
			if (s > 0) {
				r = true;
			}
			shok_BB_CFileStreamEx_Close(&filestr);
		}
		shok_BB_CFileStreamEx_dtor(&filestr);
	}
	catch (...)
	{
		return false;
	}
	return r;
}

bool shok_GGL_CSettler::IsIdle()
{
	int com = EventLeaderGetCurrentCommand();
	return com == shok_LeaderCommandDefend || com == shok_LeaderCommandHoldPos || TaskListId == ((shok_GGL_CGLSettlerProps*)GetEntityType())->IdleTaskList;
}

int shok_GGL_CBuilding::GetConstructionSite()
{
	Attachment* a = EntitiesAttachedToMe.GetFirstMatch([](Attachment* a) { return a->AttachmentType == AttachmentType_ATTACHMENT_CONSTRUCTION_SITE_BUILDING; });
	if (a)
		return a->EntityId;
	return 0;
}

int(__thiscall* shok_build_getnearestfreeslot)(shok_EGL_CGLEEntity* th, shok_position* p) = (int(__thiscall*) (shok_EGL_CGLEEntity*, shok_position*)) 0x4AB02D;
int shok_GGL_CBuilding::GetNearestFreeConstructionSlotFor(shok_position* p)
{
	int cid = GetConstructionSite();
	if (!cid)
		return -1;
	shok_EGL_CGLEEntity* consi = shok_eid2obj(cid);
	if (!consi)
		return -1;
	return shok_build_getnearestfreeslot(consi, p);
}

int shok_GGL_CBuilding::GetNearestFreeRepairSlotFor(shok_position* p)
{
	return shok_build_getnearestfreeslot(this, p);
}

bool shok_EGL_CMovingEntity::SerfConstructBuilding(shok_GGL_CBuilding* build)
{
	int cid = build->GetNearestFreeConstructionSlotFor(&this->Position);
	if (cid < 0)
		return false;
	shok_event_data_GGL_CEventEntityIndex ev = shok_event_data_GGL_CEventEntityIndex();
	ev.id = 0x14003;
	ev.entity = build->GetConstructionSite();
	ev.index = cid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return true;
}

bool shok_EGL_CMovingEntity::SerfRepairBuilding(shok_GGL_CBuilding* build)
{
	int cid = build->GetNearestFreeRepairSlotFor(&this->Position);
	if (cid < 0)
		return false;
	shok_event_data_GGL_CEventEntityIndex ev = shok_event_data_GGL_CEventEntityIndex();
	ev.id = 0x14003; // repair and build are the same event, just different target entity
	ev.entity = build->EntityId;
	ev.index = cid;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return true;
}

bool shok_GGL_CBuilding::IsConstructionFinished()
{
	return BuildingHeight >= 1;
}

int(__thiscall* entitygetmaxhealth)(shok_EGL_CGLEEntity* th) = (int(__thiscall*)(shok_EGL_CGLEEntity*)) 0x57B798;
int shok_EGL_CGLEEntity::GetMaxHealth()
{
	return entitygetmaxhealth(this);
}

void shok_EGL_CMovingEntity::SerfExtractResource(int id)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x10009;
	ev.entityId = id;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

void(__thiscall* shok_EGL_CGLEEntityCreator_ctor)(shok_EGL_CGLEEntityCreator* th) = (void(__thiscall*)(shok_EGL_CGLEEntityCreator*))0x4493A4;
shok_EGL_CGLEEntityCreator::shok_EGL_CGLEEntityCreator(bool _)
{
	shok_EGL_CGLEEntityCreator_ctor(this);
}
shok_EGL_CGLEEntityCreator::shok_EGL_CGLEEntityCreator()
{
}

shok_GGL_CGLConstructionSiteCreator::shok_GGL_CGLConstructionSiteCreator()
{

}
void(__thiscall* shok_GGL_CResourceDoodadCreator_ctor)(shok_GGL_CResourceDoodadCreator* th) = (void(__thiscall*)(shok_GGL_CResourceDoodadCreator*)) 0x4DD7E2;
shok_GGL_CResourceDoodadCreator::shok_GGL_CResourceDoodadCreator()
{
	shok_GGL_CResourceDoodadCreator_ctor(this);
}

int shok_EGL_CGLEGameLogic::CreateEntity(shok_EGL_CGLEEntityCreator* cr)
{
	return ((shok_vtable_EGL_CGLEGameLogic*)vtable)->CreateEntity(this, cr, 1);
}

void(__thiscall* shok_EGL_CGLEEntityCreator_dtor)(shok_EGL_CGLEEntityCreator* th) = (void(__thiscall*)(shok_EGL_CGLEEntityCreator*))0x449E23;
shok_EGL_CGLEEntityCreator::~shok_EGL_CGLEEntityCreator()
{
	shok_EGL_CGLEEntityCreator_dtor(this);
}

void shok_EGL_CGLEEntity::Destroy()
{
	((shok_vtable_EGL_CGLEEntity*)vtable)->Destroy(this, 0);
}

shok_EGL_CGLEEntity* ReplaceEntityWithResourceEntity(shok_EGL_CGLEEntity* e)
{
	shok_EGL_CGLEEntityProps* ty = e->GetEntityType()->LogicProps;
	if (ty->vtable != shok_vtp_GGL_CEntityProperties)
		return nullptr;
	shok_GGL_CEntityProperties* t = (shok_GGL_CEntityProperties*)ty;
	shok_GGL_CResourceDoodadCreator c = shok_GGL_CResourceDoodadCreator();
	c.EntityType = t->ResourceEntity;
	c.PlayerId = e->PlayerId;
	c.Pos = e->Position;
	c.ResourceAmount = t->ResourceAmount;
	c.Scale = e->Scale;
	if (e->ScriptName) {
		size_t len = strlen(e->ScriptName) + 1;
		c.ScriptName = (char*)shok_malloc(sizeof(char) * len);
		strcpy_s(c.ScriptName, len, e->ScriptName);
	}
	else {
		c.ScriptName = nullptr;
	}
	int id = (*shok_EGL_CGLEGameLogicObject)->CreateEntity(&c);
	shok_EGL_CGLEEntity* r = shok_eid2obj(id);
	shok_event_data_EGL_CEventValue_int_27574121 ev = shok_event_data_EGL_CEventValue_int_27574121();
	ev.id = 0x1000C;
	ev.i = e->EntityType;
	((shok_vtable_EGL_CGLEEntity*)r->vtable)->FireEvent(r, &ev);
	e->Destroy();
	return r;
}

shok_GGL_CSerfBehavior* shok_EGL_CGLEEntity::GetSerfBehavior()
{
	return (shok_GGL_CSerfBehavior*)SearchBehavior(shok_vtp_GGL_CSerfBehavior);
}

void(__thiscall* building_startUpgrade)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*)) 0x4AF1B5;
void shok_GGL_CBuilding::StartUpgrade()
{
	building_startUpgrade(this);
}

void(__thiscall* buildingCancelUpgrade)(shok_GGL_CBuilding* th) = (void(__thiscall*)(shok_GGL_CBuilding*))0x4AF228;
void shok_GGL_CBuilding::CancelUpgrade()
{
	buildingCancelUpgrade(this);
}

bool shok_GGL_CBuilding::IsIdle()
{
	if (!IsConstructionFinished())
		return false;
	if (IsUpgrading)
		return false;
	if (CurrentState == 0x13) // alarm mode
		return false;
	if (GetTechnologyInResearch())
		return false;
	{
		shok_GGL_CFoundryBehavior* f = GetFoundryBehavior();
		if (f && (f->CannonType != 0 || GetCannonProgress() != 100))
			return false;
	}
	if (GetMarketBehavior() && GetMarketProgress() < 1.0f)
		return false;
	if (GetBarrackBehavior()) {
		if (EntitiesAttachedToMe.GetFirstMatch([](Attachment* a) { return a->AttachmentType == AttachmentType_FIGHTER_BARRACKS; }))
			return false;
	}
	return true;
}

int(__thiscall* building_gettechinres)(shok_GGL_CBuilding* th) = (int(__thiscall*)(shok_GGL_CBuilding*))0x4AAD09;
int shok_GGL_CBuilding::GetTechnologyInResearch()
{
	return building_gettechinres(this);
}

int shok_GGL_CBuilding::GetCannonProgress()
{
	return EventGetIntById(0x17014);
}

float shok_GGL_CBuilding::GetMarketProgress()
{
	shok_event_data_EGL_CEventGetValue_float_1468983543 ev = shok_event_data_EGL_CEventGetValue_float_1468983543();
	ev.id = 0x17009;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return ev.f;
}

void(__thiscall* shok_entity_expellSettler)(shok_EGL_CGLEEntity* th, int i) = (void(__thiscall*)(shok_EGL_CGLEEntity*, int))0x4A39BB;
void shok_EGL_CMovingEntity::SettlerExpell()
{
	if (GetLeaderBehavior() && AttachedToEntities.GetFirstMatch([](Attachment* a) { return a->AttachmentType == AttachmentType_LEADER_SOLDIER; })) {
		shok_event_data_EGL_CEventValue_bool_703333479 ev = shok_event_data_EGL_CEventValue_bool_703333479();
		ev.id = 0x15046;
		ev.b = true;
		((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	}
	else {
		//((shok_vtable_EGL_CGLEEntity*)vtable)->Destroy(this, 1);
		shok_entity_expellSettler(this, 1);
	}
}

void shok_GGL_CBuilding::CommandRecruitSoldierForLeader(int id)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x15037;
	ev.entityId = id;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
}

int shok_EGL_CMovingEntity::LeaderGetNearbyBarracks()
{
	return EventGetIntById(0x15036);
}

int(__thiscall* upgrademanager_getucatbybuilding)(shok_GGL_CBuildingUpgradeManager* th, int id) = (int(__thiscall*)(shok_GGL_CBuildingUpgradeManager*, int))0x4B3CA6;
int shok_GGL_CBuildingUpgradeManager::GetUpgradeCategoryOfBuildingType(int etype)
{
	return upgrademanager_getucatbybuilding(this, etype);
}

int shok_EGL_CGLEEntity::LimitedAttachmentGetMaximum(int attachType)
{
	shok_event_data_GGL_CEventAttachmentTypeGetInteger ev = shok_event_data_GGL_CEventAttachmentTypeGetInteger();
	ev.id = 0x1A007;
	ev.AttachmentType = attachType;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	return ev.i;
}

int(__thiscall* plattracthandlerGetAttLimit)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*))0x4C216F;
int shok_GGL_CPlayerAttractionHandler::GetAttractionLimit()
{
	return plattracthandlerGetAttLimit(this);
}

int(__thiscall* plattracthandlerGetAttUsage)(shok_GGL_CPlayerAttractionHandler* th) = (int(__thiscall*)(shok_GGL_CPlayerAttractionHandler*))0x4C278B;
int shok_GGL_CPlayerAttractionHandler::GetAttractionUsage()
{
	return plattracthandlerGetAttUsage(this);
}

void shok_EGL_CMovingEntity::LeaderAttachSoldier(int soldierId)
{
	shok_event_data_EGL_CEvent1Entity ev = shok_event_data_EGL_CEvent1Entity();
	ev.id = 0x15009;
	ev.entityId = soldierId;
	((shok_vtable_EGL_CGLEEntity*)vtable)->FireEvent(this, &ev);
	shok_event_data_BB_CEvent e2 = shok_event_data_BB_CEvent();
	e2.id = 0x15017;
	shok_EGL_CGLEEntity* sol = shok_eid2obj(soldierId);
	((shok_vtable_EGL_CGLEEntity*)sol->vtable)->FireEvent(sol, &e2);
}

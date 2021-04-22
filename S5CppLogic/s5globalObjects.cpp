#include "pch.h"
#include "s5data.h"
#include <stdexcept>


struct shok_vtable_ECS_CManager {
private:
	int u, u2;
public:
	void(__thiscall* ReloadCutscene)(shok_ECS_CManager* th, const char* path);
};

struct shok_vtable_EGL_CGLEGameLogic {
private:
	int u[22];
public:
	int(__thiscall* CreateEntity)(shok_EGL_CGLEGameLogic* th, shok_EGL_CGLEEntityCreator* data, int i); // 22
	int(__thiscall* CreateEffect)(shok_EGL_CGLEGameLogic* th, shok_EGL_CGLEEffectCreator* data); // 23
};

struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(shok_BB_IPostEvent* th, shok_BB_CEvent* ev);
};

struct shok_vtable_GGL_CWeatherHandler {
	PADDINGI(1)
	int(__thiscall* GetCurrentWeather)(shok_GGL_CWeatherHandler* th);
};

shok_GGlue_CGlueEntityProps* shok_EGL_CGLEEntitiesProps::GetEntityType(int i)
{
	if (i <= 0 || i >= (int)EntityTypes.size())
		return nullptr;
	return EntityTypes.data() + i;
}

void shok_ECS_CManager::ReloadCutscene(const char* path)
{
	((shok_vtable_ECS_CManager*)this->vtable)->ReloadCutscene(this, path);
}

static inline bool(__thiscall* shok_EGL_CGLEEffectManager_IsEffectValid)(shok_EGL_CGLEEffectManager* th, int id) = (bool(__thiscall*)(shok_EGL_CGLEEffectManager*, int))0x4FAABD;
bool shok_EGL_CGLEEffectManager::IsEffectValid(int id)
{
	return shok_EGL_CGLEEffectManager_IsEffectValid(this, id);
}

static inline shok_EGL_CEffect* (__thiscall* shok_effid2obj)(shok_EGL_CGLEEffectManager* th, int id) = (shok_EGL_CEffect * (__thiscall*)(shok_EGL_CGLEEffectManager*, int))0x04FAAE3;
shok_EGL_CEffect* shok_EGL_CGLEEffectManager::GetEffectById(int id)
{
	return shok_effid2obj(this, id);
}

static inline int(__thiscall* shok_getAnimIdByName)(shok_BB_CIDManagerEx* th, const char* name) = (int(__thiscall*)(shok_BB_CIDManagerEx * th, const char* name)) 0x54F19E;
int shok_BB_CIDManagerEx::GetAnimIdByName(const char* name) {
	return shok_getAnimIdByName(this, name);
}

int shok_EGL_CGLEGameLogic::CreateEffect(shok_EGL_CGLEEffectCreator* data) {
	shok_vtable_EGL_CGLEGameLogic* vt = (shok_vtable_EGL_CGLEGameLogic*)this->vtable;
	return vt->CreateEffect(this, data);
}

int shok_EGL_CGLEGameLogic::CreateEntity(shok_EGL_CGLEEntityCreator* cr)
{
	return ((shok_vtable_EGL_CGLEGameLogic*)vtable)->CreateEntity(this, cr, 1);
}

int shok_EGL_CGLEGameLogic::GetTimeMS()
{
	return InGameTime[0] * InGameTime[1];
}

int shok_EGL_CGLEGameLogic::GetTick()
{
	return InGameTime[0];
}

int(__thiscall* CreateEffectHookedOrig)(shok_EGL_CGLEGameLogic* th, shok_EGL_CGLEEffectCreator* data) = nullptr;
void(*CreateEffectHookCallback)(int id, void* ret) = nullptr;
int __fastcall CreateEffectHook(shok_EGL_CGLEGameLogic* th, int _, shok_EGL_CGLEEffectCreator* data)
{
	void** ebp_var = (void**)1;
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
	vt->CreateEffect = (int(__thiscall*)(shok_EGL_CGLEGameLogic * th, shok_EGL_CGLEEffectCreator * data)) & CreateEffectHook;
}

shok_EGL_CGLEEntity* shok_EGL_CGLEEntityManager::GetEntityByNum(int num)
{
	if (EntityCount <= num)
		return nullptr;
	return Entities[num].entity;
}

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
	BB_IPostEvent_vtableHooked->PostEvent = (void(__stdcall*)(shok_BB_IPostEvent * th, shok_BB_CEvent * ev)) & PostEventHook;
}

static inline shok_GGL_CPlayerStatus* (__thiscall* shok_GGL_CGLGameLogic_GetPlayer)(shok_GGL_CPlayerStatus** pl, int p) = (shok_GGL_CPlayerStatus * (__thiscall*) (shok_GGL_CPlayerStatus**, int)) 0x4A91BC;
shok_GGL_CPlayerStatus* shok_GGL_CGLGameLogic::GetPlayer(int i)
{
	return shok_GGL_CGLGameLogic_GetPlayer(this->players, i);
}

shok_technology* shok_GGL_CGLGameLogic::GetTech(int i)
{
	try {
		return TechList->TechList.at(i - 1);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

static inline void(__thiscall* cglgamelogic_enablealarm)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventPlayerID* d) = (void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventPlayerID*))0x49FA14;
void shok_GGL_CGLGameLogic::EnableAlarmForPlayer(int pl)
{
	shok_EGL_CNetEventPlayerID ev = { shok_vtp_EGL_CNetEventPlayerID, 69680, pl };
	cglgamelogic_enablealarm(this, &ev);
}

static inline void(__thiscall* cglgamelogic_disablealarm)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventPlayerID* d) = (void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventPlayerID*))0x49FAD7;
void shok_GGL_CGLGameLogic::DisableAlarmForPlayer(int pl)
{
	shok_EGL_CNetEventPlayerID ev = { shok_vtp_EGL_CNetEventPlayerID, 69681, pl };
	cglgamelogic_disablealarm(this, &ev);
}

static inline void(__thiscall* cglgamelogic_upgradesettlercat)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventIntegerAndPlayerID* d) = (void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventIntegerAndPlayerID*))0x4985C5;
void shok_GGL_CGLGameLogic::UpgradeSettlerCategory(int pl, int ucat)
{
	shok_EGL_CNetEventIntegerAndPlayerID ev = { shok_vtp_EGL_CNetEventIntegerAndPlayerID , 69642, pl, ucat };
	cglgamelogic_upgradesettlercat(this, &ev);
}

static inline void(__thiscall* cglgamelogic_activateweatherm)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventIntegerAndPlayerID* d) = (void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventIntegerAndPlayerID*))0x49BF7A;
void shok_GGL_CGLGameLogic::PlayerActivateWeathermachine(int player, int weathertype)
{
	shok_EGL_CNetEventIntegerAndPlayerID ev = { shok_vtp_EGL_CNetEventIntegerAndPlayerID , 69686, player, weathertype };
	cglgamelogic_activateweatherm(this, &ev);
}

static inline void(__thiscall* cglgamelogic_blesssettlers)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventIntegerAndPlayerID* d) = (void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventIntegerAndPlayerID*))0x49B7E6;
void shok_GGL_CGLGameLogic::PlayerBlessSettlers(int player, int blessCat)
{
	shok_EGL_CNetEventIntegerAndPlayerID ev = { shok_vtp_EGL_CNetEventIntegerAndPlayerID , 69686, player, blessCat };
	cglgamelogic_blesssettlers(this, &ev);
}

static inline int(__thiscall* weatherdata_getnext)(shok_GGL_CWeatherHandler* th) = (int(__thiscall*)(shok_GGL_CWeatherHandler*))0x4B93BD;
int shok_GGL_CWeatherHandler::GetNextWeatherState()
{
	return weatherdata_getnext(this);
}

int shok_GGL_CWeatherHandler::GetCurrentWeatherState()
{
	return ((shok_vtable_GGL_CWeatherHandler*)vtable)->GetCurrentWeather(this);
}

static inline int(__thiscall* weatherdata_gettimetonext)(shok_GGL_CWeatherHandler* th) = (int(__thiscall*)(shok_GGL_CWeatherHandler*))0x4B9309;
int shok_GGL_CWeatherHandler::GetTicksToNextPeriodicWeatherChange()
{
	return weatherdata_gettimetonext(this);
}

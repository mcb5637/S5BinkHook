#pragma once
#include <cstdint>

#define SHOK_Import_LUA_OPEN 0x761284
#define SHOK_SEGMENTSTART 0x401000
#define SHOK_SEGMENTLENGTH 0x64B000

typedef uint8_t byte;

template <class T> struct shok_vector {
private:
	T* start;
	T* end;
	T* endData;
public:
	T* Get(int i);
	int Size();
};

struct shok_position {
	float X;
	float Y;
};

struct shok_positionRot : shok_position {
	float r;
};

struct shok_object {
public:
	void* vtable;
};

struct shok_EGL_CGLEEntity : shok_object {
private:
	int u1;
public:
	int EntityId;
private:
	int u2;
public:
	int EntityType;
	int ModelOverride;
	int PlayerId;
private:
	int u3, u4, u5, u6, u7, u8, u9, u10, u11, u12, u13, u14, u15, u16, u17;
public:
	shok_positionRot Position; // 22
private:
	int u18, u19;
public:
	float Scale; // 25
	byte DefaultBehavourFlag, UserControlFlag, UnattackableFlag;
private:
	byte u20;
public:
	byte SelectableFlag, f1, VisibleFlag;
private:
	byte u21;
	int u22, u23;
public:
	shok_vector<int*> BehaviorList; // 31
};

struct shok_ECS_CManager : shok_object {

};

struct shok_vtable_ECS_CManager {
private:
	int u, u2;
public:
	void(__thiscall* ReloadCutscene)(shok_ECS_CManager* th, const char* path);
};

struct shok_effectCreatorData {
public:
	int CreatorType, EffectType;
private:
	int Zero1 = 0;
public:
	int PlayerID;
	shok_position StartPos, CurrentPos, TargetPos;
private:
	int Zero2 = 0;
public:
	int AttackerID, TargetID, Damage;
	float DamageRadius;
private:
	int Zero3 = 0, Zero4 = 0, Zero5 = 0;
};

struct shok_EGL_CGLEGameLogic : shok_object {

};

struct shok_vtable_EGL_CGLEGameLogic {
private:
	int u[23];
public:
	int(__thiscall* CreateEffect)(shok_EGL_CGLEGameLogic* th, shok_effectCreatorData* data);
};

struct shok_EGL_CGLEEffectManager : shok_object {

};

struct shok_BB_CIDManagerEx : shok_object {

};

struct shok_EGL_CGLEEntityManager : shok_object {
	friend shok_EGL_CGLEEntity * shok_EGL_CGLEEntityManager_GetEntityByNum(shok_EGL_CGLEEntityManager* man, int num);
public:
	int EntityCount;
private:
	int u[4];
	struct {
	private:
		int u;
	public:
		shok_EGL_CGLEEntity* entity;
	} Entities[1];
};

shok_EGL_CGLEEntity* shok_EGL_CGLEEntityManager_GetEntityByNum(shok_EGL_CGLEEntityManager* man, int num);

extern shok_EGL_CGLEEntity* (_stdcall *shok_eid2obj)(int id);

extern void(_stdcall* shok_SetHighPrecFPU)();

extern shok_ECS_CManager*** shok_ECS_CManagerObject;

extern shok_EGL_CGLEGameLogic** shok_EGL_CGLEGameLogicObject;

extern int shok_effectCreatorData_CreatorType_Projectile;

extern shok_EGL_CGLEEffectManager** shok_EGL_CGLEEffectManagerObject;

extern bool(__thiscall* shok_EGL_CGLEEffectManager_IsEffectValid)(shok_EGL_CGLEEffectManager* th, int id);

extern void(* shok_entityHurtEntity)(shok_EGL_CGLEEntity* attackerObj, shok_EGL_CGLEEntity* targetObj, int damage);

extern void (*shok_logString)(const char* format, const char* string);

extern shok_BB_CIDManagerEx** shok_BB_CIDManagerExObj;

extern int(__thiscall* shok_getAnimIdByName)(shok_BB_CIDManagerEx* th, const char* name);

extern shok_EGL_CGLEEntityManager** shok_EGL_CGLEEntityManagerObj;

#pragma once
#include <cstdint>

#define DEBUGGER_BREAK _asm int 3

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
	int u3[15];
public:
	shok_positionRot Position; // 22
private:
	int u4[2];
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
private:
	int u24;
public:
	int CurrentState, EntityStateBehaviors, TaskListId, TaskIndex; // la37
private:
	int u25[13]; // la50
public:
	int CurrentHealth;
	char* ScriptName;
	char* ScriptCommandLine;
	float Exploration;
	int TaskListStart;
private:
	int u26[3];
public:
	int InvulnerabilityAndSuspended; // 57
	int SuspensionTurn;
	int ScriptingValue;
private:
	int u27[4];
public:
	int StateChangeCounter;
	int TaskListChangeCounter;
	int NumberOfAuraEffects;
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

public:
	shok_EGL_CGLEEntity* GetEntityByNum(int num);
};

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

extern int(*shok_getEntityIdByScriptName)(const char* str);

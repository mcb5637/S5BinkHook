#pragma once
#include <cstdint>
#include <vector>

#define DEBUGGER_BREAK _asm int 3

#define SHOK_Import_LUA_OPEN 0x761284
#define SHOK_SEGMENTSTART 0x401000
#define SHOK_SEGMENTLENGTH 0x64B000

typedef uint8_t byte;

template <class T> struct shok_vector {
public:
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

struct shok_EGL_CGLEBehavior : shok_object {
	int BehaviorIndex, EntityId;
private:
	int PropPointer; // 3
};

struct shok_GGL_CBattleBehavior : shok_EGL_CGLEBehavior {
	float SuccessDistance, FailureDistance;
	int TimeOutTime, StartTurn;
	shok_position TargetPosition;
	byte StartFollowing, StopFollowing;
private:
	byte u1[2];
public:
	int FollowStatus, LatestHitTurn;
private:
	int u2;
public:
	int LatestAttackerID, BattleStatus;
	byte NoMoveNecessary, NormalRangeCheckNecessary;
private:
	byte u3[2];
public:
	int Command;
	shok_position AttackMoveTarget;
private:
	int u4;
public:
	int MilliSecondsToWait, MSToPlayHitAnimation, HitPlayed;
};

struct shok_GGL_CSoldierBehavior : shok_GGL_CBattleBehavior {
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
public:
	float Scale; // 25
private:
	int u4;
public:
	byte DefaultBehavourFlag, UserControlFlag, UnattackableFlag;
private:
	byte u20;
public:
	byte SelectableFlag, f1, VisibleFlag;
private:
	byte u21;
	int u22;
public:
	std::vector<shok_EGL_CGLEBehavior*> BehaviorList; // 30, first field in 31
	int CurrentState, EntityStateBehaviors, TaskListId, TaskIndex; // la37
private:
	int u25[12]; // la49
public:
	int CurrentHealth;
	char* ScriptName;
	char* ScriptCommandLine;
	float Exploration;
	int TaskListStart; // 54
private:
	int u26[2];
public:
	int InvulnerabilityAndSuspended; // 57
	int SuspensionTurn;
	int ScriptingValue; // 59
private:
	int u27[3];
public:
	int StateChangeCounter; // 63
	int TaskListChangeCounter;
	int NumberOfAuraEffects; // 65

private:
	shok_EGL_CGLEBehavior* SearchBehavior(void* vt);
public:
	shok_GGL_CSoldierBehavior* GetSoldierBehavior();
};

struct shok_EGL_CMovingEntity : shok_EGL_CGLEEntity {
	shok_position TargetPosition; // la67
	int TargetRotationValid;
	float TargetRotation;
	int MovementState; // 70
};

struct shok_GGL_CEvadingEntity : shok_EGL_CMovingEntity {};

struct shok_GGL_CSettler : shok_GGL_CEvadingEntity {
private:
	int u[4];
public:
	int TimeToWait, HeadIndex, HeadParams; // la77
private:
	int u2[7];
public:
	float ExperiencePoints; // 85
	int ExperienceLevel;
private:
	int u3[2];
public:
	float MovingSpeed; //89
private:
	int u4;
public:
	float Damage;
private:
	int u5;
public:
	float Dodge;
private:
	int u6;
public:
	float Motivation;
private:
	int u7;
public:
	float Armor;
private:
	int u8;
public:
	float CurrentAmountSoldiers;
private:
	int u9;
public:
	float MaxAmountSoldiers;
private:
	int u10;
public:
	float DamageBonus;
private:
	int u11;
public:
	float ExplorationCache;
private:
	int u12;
public:
	float MaxAttackRange;
private:
	int u13;
public:
	float AutoAttackRange;
private:
	int u14;
public:
	float HealingPoints;
private:
	int u15;
public:
	float MissChance;
private:
	int u16;
public:
	float Healthbar; // 115
private:
	int u17[7];
public:
	float SomeIntRegardingTaskLists; // 123
private:
	int u18[3];
public:
	int LeaderId;
private:
	int u19[2];
public:
	int OverheadWidget;
	int ExperienceClass;
private:
	int u20[2];
public:
	int BlessBuff, NPCMarker, LeaveBuildingTurn; //la136
};

struct shok_GGL_CResourceDoodad : shok_EGL_CGLEEntity {
	int ResourceType, ResourceAmount, ResourceAmountAdd;
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

extern bool(_cdecl* shok_EntityIsSettler)(shok_EGL_CGLEEntity* e);

extern bool(_cdecl* shok_EntityIsBuilding)(shok_EGL_CGLEEntity* e);

extern bool(__thiscall* shok_IsEntityInCategory)(shok_EGL_CGLEEntity* e, int cat);

extern bool(_cdecl* shok_EntityIsResourceDoodad)(shok_EGL_CGLEEntity* e);

extern int(_cdecl* shok_EntityGetProvidedResourceByID)(int id);

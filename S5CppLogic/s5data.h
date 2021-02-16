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

struct shok_positionRot {
	float X;
	float Y;
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

extern shok_EGL_CGLEEntity* (_stdcall *shok_eid2obj)(int id);

extern void(_stdcall* shok_SetHighPrecFPU)();

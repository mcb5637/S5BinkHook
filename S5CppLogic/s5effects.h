#pragma once
#include "s5data.h"

struct shok_EGL_CEffect : shok_object {
	PADDINGI(15);
	shok_position Position; // 16
	int SpawnedTurn; // 18
	PADDINGI(1);
	int EffectType; // 20
	PADDINGI(2);
	int EffectID; // 23

	static inline constexpr int vtp = 0x784B28;

	bool IsCannonBallEffect();
	bool IsArrowEffect();
};
struct shok_EGL_CFlyingEffect : shok_EGL_CEffect {
	PADDINGI(6);
	int SpawnedTurnAgain; //30
	float GravityFactor;
	PADDINGI(2);
	shok_position StartPosition, TargetPosition, CurrentPosition, NextPosition;
	PADDINGI(1);
	float StrangeFloat;
	PADDINGI(3);
	int AttackerID; // 47

	static inline constexpr int vtp = 0x7775E4;

	static void HookOnHit();
};
extern void (*FlyingEffectOnHitCallback)(shok_EGL_CFlyingEffect* eff);
extern void (*FlyingEffectOnHitCallback2)(shok_EGL_CFlyingEffect* eff, bool post);

struct shok_GGL_CArrowEffect : shok_EGL_CFlyingEffect {
	int TargetID;
	int DamageAmount;

	static inline constexpr int vtp = 0x778E24;
};
struct shok_GGL_CCannonBallEffect : shok_EGL_CFlyingEffect {
	int SourcePlayer; // 48
	void* Props;
	int DamageAmount; // 50
	float AoERange;
	int DamageClass; // 52

	static inline constexpr int vtp = 0x777690;
};

struct shok_EGL_CGLEEffectCreator { // 16 source player, 13 dmg
protected:
	int vtable;
public:
	int EffectType = 0;
private:
	int Zero1 = 0;
public:
	int PlayerID = 0;
	shok_position StartPos = { 0,0 }, CurrentPos = { 0,0 }, TargetPos = { 0,0 };
private:
	int Zero2 = 0;
public:
	int AttackerID = 0, TargetID = 0, Damage = 0;
	float DamageRadius = 0;
private:
	int Zero3 = 0, Zero4 = 0, Zero5 = 0;

public:
	shok_EGL_CGLEEffectCreator();
};

struct shok_EGL_CFlyingEffectCreator : shok_EGL_CGLEEffectCreator {
	shok_EGL_CFlyingEffectCreator();
};

struct shok_CProjectileEffectCreator : shok_EGL_CFlyingEffectCreator {
	shok_CProjectileEffectCreator();
};

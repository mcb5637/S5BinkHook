#pragma once
#include "s5data.h"

struct shok_EGL_CEffect : shok_object {
	int vtable_EGL_IEffectDisplay;
	int attachmentvt;
	shok_set<shok_attachment> ObserverEntities;
	shok_set<shok_attachment> ObserverEffects;
	shok_set<shok_attachment> ObservedEntities;
	shok_set<shok_attachment> ObservedEffects;
	byte SendEvent; // 20
	PADDING(3);
	shok_position Position; // 16
	int StartTurn; // 18
	int Duration;
	int EffectType; // 20
	int EffectFlags;
	int PlayerID;
	int EffectID; // 23

	static inline constexpr int vtp = 0x784B28;

	bool IsCannonBallEffect();
	bool IsArrowEffect();
};
struct shok_EGL_CFlyingEffectSlot : shok_object {
	int vtable_bbobject;
	int StartTurn;
	float a; // gravity factor
	float b;
	float c;
	shok_position StartPosition, TargetPosition, Position, LastPosition;
	float x; // 6.28 ?
	float Speed;

	static inline constexpr int vtp = 0x777640;
};
struct shok_EGL_CFlyingEffect : shok_EGL_CEffect {
	PADDINGI(4);
	shok_EGL_CFlyingEffectSlot FlyingEffectSlot; // 28
	shok_position TargetPosition; // 44 la 45
	PADDINGI(2);

	static inline constexpr int vtp = 0x7775E4;

	static void HookOnHit();
};
extern void (*FlyingEffectOnHitCallback)(shok_EGL_CFlyingEffect* eff);
extern void (*FlyingEffectOnHitCallback2)(shok_EGL_CFlyingEffect* eff, bool post);

struct shok_GGL_CArrowEffect : shok_EGL_CFlyingEffect {
	int AttackerID; // 47
	int TargetID;
	int DamageAmount;
	byte Misses;
	PADDING(3);

	static inline constexpr int vtp = 0x778E24;
};
struct shok_GGL_CCannonBallEffect : shok_EGL_CFlyingEffect {
	int AttackerID; // 47
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

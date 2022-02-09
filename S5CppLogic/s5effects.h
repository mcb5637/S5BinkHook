#pragma once
#include "s5data.h"

struct shok_EGL_CEffect : shok_BB_IObject {
	int vtable_EGL_IEffectDisplay;
	int attachmentvt;
	shok::Set<shok_attachment> ObserverEntities;
	shok::Set<shok_attachment> ObserverEffects;
	shok::Set<shok_attachment> ObservedEntities;
	shok::Set<shok_attachment> ObservedEffects;
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
	static inline constexpr int TypeDesc = 0x822284;

	bool IsCannonBallEffect();
	bool IsArrowEffect();
};
struct shok_EGL_CFlyingEffectSlot : shok_object { // technically bbiobj, but we have to dynamiccast it
	int vtable_bbobject;
	int StartTurn;
	float a; // gravity factor
	float b;
	float c;
	shok_position StartPosition, TargetPosition, Position, LastPosition;
	float x; // 6.28 ?
	float Speed;

	static inline constexpr int vtp = 0x777640;
	static inline constexpr int TypeDesc = 0x823638;
};
struct shok_EGL_CFlyingEffect : shok_EGL_CEffect {
	PADDINGI(4);
	shok_EGL_CFlyingEffectSlot FlyingEffectSlot; // 28
	shok_position TargetPosition; // 44 la 45
	PADDINGI(1);

	static inline constexpr int vtp = 0x7775E4;
	static inline constexpr int TypeDesc = 0x8235EC;

	static void HookOnHit();
	static void (*FlyingEffectOnHitCallback)(shok_EGL_CFlyingEffect* eff);
	static void (*FlyingEffectOnHitCallback2)(shok_EGL_CFlyingEffect* eff, bool post);
	static shok_EGL_CFlyingEffect* CurrentHittingEffect;
};
//constexpr int i = offsetof(shok_EGL_CFlyingEffect, PlayerID);

struct shok_GGL_CArrowEffect : shok_EGL_CFlyingEffect {
	int AttackerID; // 47
	int TargetID;
	int DamageAmount;
	byte Misses; // 50
	PADDING(2);
	byte AdvancedDamageSourceOverride;

	static inline constexpr int vtp = 0x778E24;
	static inline constexpr int TypeDesc = 0x8289CC;
};
struct shok_GGL_CCannonBallEffect : shok_EGL_CFlyingEffect {
	int AttackerID; // 47
	int SourcePlayer; // 48
	void* Props;
	int DamageAmount; // 50
	float AoERange;
	int DamageClass; // 52

	static bool FixDamageClass;
	static bool AddDamageSourceOverride;
	static void HookFromCreator();

	static inline constexpr int vtp = 0x777690;
	static inline constexpr int TypeDesc = 0x82365C;
};

struct shok_EGL_CGLEEffectCreator {
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
	int Zero2 = 0; // height offset unused
public:
	int AttackerID = 0, TargetID = 0, Damage = 0; // la 13
	float DamageRadius = 0;
	int DamageClass = 0; // unused
	int SourcePlayer = 0; // 16
	bool Misses = false; // 17
private:
	bool Zero5[2] = { false, false };
public:
	byte AdvancedDamageSourceOverride = 0;


public:
	static inline constexpr int vtp = 0x76E140;
	static inline constexpr int TypeDesc = 0x810254;
	shok_EGL_CGLEEffectCreator();
};

struct shok_EGL_CFlyingEffectCreator : shok_EGL_CGLEEffectCreator {

	static inline constexpr int vtp = 0x774688;
	static inline constexpr int TypeDesc = 0x819334;

	shok_EGL_CFlyingEffectCreator();
};

struct shok_CProjectileEffectCreator : shok_EGL_CFlyingEffectCreator {

	static inline constexpr int vtp = 0x774698;
	static inline constexpr int TypeDesc = 0x81935C;

	shok_CProjectileEffectCreator();
};

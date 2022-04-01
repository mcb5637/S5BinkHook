#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace EGL {
	class IEffectDisplay {
	public:
		virtual ~IEffectDisplay() = default;
		// 5 more funcs
	};

	class CEffect : public BB::IObject, public IEffectDisplay, public EGL::TGLEAttachable<EGL::CEffect, EGL::CEffectAttachmentProxy> {
	public:
		shok::Position Position; // 16
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
	static_assert(offsetof(CEffect, StartTurn) == 18 * 4);

	class SSlotArgsFlyingEffect {
	};
	class CFlyingEffectSlot : public EGL::TSlot<EGL::SSlotArgsFlyingEffect, 1944101197>, public BB::IObject {
		int StartTurn;
		float a; // gravity factor
		float b;
		float c;
		shok::Position StartPosition, TargetPosition, Position, LastPosition;
		float x; // 6.28 ?
		float Speed;

		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(unsigned int id) override;

		static inline constexpr int vtp = 0x777640;
		static inline constexpr int TypeDesc = 0x823638;
		static inline constexpr unsigned int Identifier = 0xD1E5044D;
	};

	class CFlyingEffect : public EGL::CEffect {
	public:
		PADDINGI(4);
		EGL::CFlyingEffectSlot FlyingEffectSlot; // 28
		shok::Position TargetPosition; // 44 la 45
		PADDINGI(1);

		static inline constexpr int vtp = 0x7775E4;
		static inline constexpr int TypeDesc = 0x8235EC;

		static void HookOnHit();
		static void (*FlyingEffectOnHitCallback)(EGL::CFlyingEffect* eff);
		static void (*FlyingEffectOnHitCallback2)(EGL::CFlyingEffect* eff, bool post);
		static EGL::CFlyingEffect* CurrentHittingEffect;
	};
}

namespace GGL {
	class CArrowEffect : public EGL::CFlyingEffect {
	public:
		int AttackerID; // 47
		int TargetID;
		int DamageAmount;
		byte Misses; // 50
		PADDING(2);
		byte AdvancedDamageSourceOverride;

		static inline constexpr int vtp = 0x778E24;
		static inline constexpr int TypeDesc = 0x8289CC;
	};

	class CCannonBallEffect : public EGL::CFlyingEffect {
	public:
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
}

namespace EGL {
	class CGLEEffectCreator : public BB::IObject {
	public:
		int EffectType = 0;
	private:
		int Zero1 = 0;
	public:
		int PlayerID = 0;
		shok::Position StartPos = { 0,0 }, CurrentPos = { 0,0 }, TargetPos = { 0,0 };
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
		CGLEEffectCreator();

		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(unsigned int id) override;
	protected:
		void SetVT(int vt);
	};

	class CFlyingEffectCreator : public EGL::CGLEEffectCreator {
	public:

		static inline constexpr int vtp = 0x774688;
		static inline constexpr int TypeDesc = 0x819334;

		CFlyingEffectCreator();
	};
}

class CProjectileEffectCreator : public EGL::CFlyingEffectCreator {
public:

	static inline constexpr int vtp = 0x774698;
	static inline constexpr int TypeDesc = 0x81935C;

	CProjectileEffectCreator();
};
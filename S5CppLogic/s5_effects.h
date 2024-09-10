#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_RWEngine.h"

namespace EGL {
	class IEffectDisplay {
	public:
		struct TypeAndID {
			shok::EffectTypeId EffectType = shok::EffectTypeId::Invalid;
			shok::EffectId EffectID = shok::EffectId::Invalid;
		};
		struct TurnDurationAndPos {
			int StartTurn = 0;
			int Duration = 0;
			shok::Position P;
		};


		virtual ~IEffectDisplay() = default;
		virtual shok::EffectTypeId __stdcall GetEffectType() const = 0;
		virtual void __stdcall FillTypeAndID(TypeAndID* d) const = 0;
		virtual void __stdcall FillTurnDurationAndPos(TurnDurationAndPos* d) const = 0;
		virtual void __stdcall GetPlayerID() const = 0;
		virtual EGL::ISlot* __stdcall GetFlyingEffectSlot() = 0;
	};

	class CEffect : public BB::IObject, public IEffectDisplay, public EGL::TGLEAttachable<EGL::CEffect, EGL::CEffectAttachmentProxy> {
	public:
		shok::Position Position; // 16
		int StartTurn; // 18
		int Duration;
		shok::EffectTypeId EffectType; // 20
		int EffectFlags; // 1->has duration
		shok::PlayerId PlayerID;
		shok::EffectId EffectID; // 23
		PADDINGI(4); // set of eventhandlers? then bool?

		static inline constexpr int vtp = 0x784B28;
		static inline constexpr int TypeDesc = 0x822284;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF8536CED);

		virtual void FromCreator(EGL::CGLEEffectCreator* ct) = 0; // 3
		virtual void OnCreated() = 0;
		virtual void OnLoaded() = 0; // 5
		virtual void OnTick() = 0;
		virtual void FireEvent(BB::CEvent* ev) = 0;
	private:
		virtual void emptyfunc2() = 0; // 8

	public:

		CGLEEffectProps* LogicProps() const;

		static void(__stdcall*OnDestroyCb)(CEffect* th);
		static void HookOnDestroy();
	};
	static_assert(offsetof(CEffect, StartTurn) == 18 * 4);

	class SSlotArgsFlyingEffect {
	};
	class CFlyingEffectSlot : public EGL::TSlot<EGL::SSlotArgsFlyingEffect, 1944101197>, public BB::IObject {
	public:
		int StartTurn; // in effect 30
		float GravityFactor; // gravity factor, "a" in seridata
		float b; // current height?
		float HeightStart; // , "c" in seridata
		shok::Position StartPosition; // in effect, 34
		shok::Position TargetPosition; // in effect, 36
		shok::Position Position; // in effect, 38
		shok::Position LastPosition; // in effect, 40
		float x; // angle rad, flight direction?
		float Speed; // in effect 43

		// dtor empty
		virtual void __stdcall FillSlot(SSlotArgsFlyingEffect* data) override;
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		static inline constexpr int vtp = 0x777640;
		static inline constexpr int TypeDesc = 0x823638;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD1E5044D);
	};

	class CFlyingEffect : public EGL::CEffect {
	public:
		EGL::CFlyingEffectSlot FlyingEffectSlot; // 28
		shok::Position TargetPosition; // 44 la 45
		EGL::CFlyingEffectProps* FlyingEffectProps; // 46

		static inline constexpr int vtp = 0x7775E4;
		static inline constexpr int TypeDesc = 0x8235EC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8B5120CD);

		virtual void OnHit() = 0;
		virtual void CalculateGravityStuff() = 0;

	protected:
		void FixOnLoad();
	public:
		static void HookOnLoadFix();

		static void HookOnHit();
		static void (*FlyingEffectOnHitCallback)(EGL::CFlyingEffect* eff);
		static void (*FlyingEffectOnHitCallback2)(EGL::CFlyingEffect* eff);
		static EGL::CFlyingEffect* CurrentHittingEffect;
	};
	//constexpr int i = offsetof(CFlyingEffect, FlyingEffectSlot.Speed) / 4;
}

namespace GGL {
	class CArrowEffect : public EGL::CFlyingEffect {
		friend class EGL::CFlyingEffect;
	public:
		shok::EntityId AttackerID; // 47
		shok::EntityId TargetID;
		int DamageAmount;
		bool Misses; // 50
		PADDING(2);
		byte AdvancedDamageSourceOverride;
		GGL::CArrowEffectProps* ArrowEffectProps;

	protected:
		void FixOnLoad();
		void OnHitHooked();

	public:
		static inline constexpr int vtp = 0x778E24;
		static inline constexpr int TypeDesc = 0x8289CC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2320F01D);

		static void HookDealDamage();
	};

	class CCannonBallEffect : public EGL::CFlyingEffect {
		friend class EGL::CFlyingEffect;
	public:
		shok::EffectId AttackerID; // 47
		shok::PlayerId SourcePlayer; // 48
		GGL::CCannonBallEffectProps* CannonBallEffectProps;
		int DamageAmount; // 50
		float AoERange;
		shok::DamageClassId DamageClass; // 52

	protected:
		void FixOnLoad();
		void OnHitHooked();

	public:
		static bool FixDamageClass;
		static bool AddDamageSourceOverride;
		static void HookFromCreator();
		static void HookDealDamage();

		static inline constexpr int vtp = 0x777690;
		static inline constexpr int TypeDesc = 0x82365C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xDF09D9FD);
	};

	class CEffectLightning : public EGL::CEffect {
	public:
		bool HasSpawnedFire = false; // 28
		GGL::CGLEffectLightningProps* Props; //29

		static inline constexpr int vtp = 0x779CE0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x284C6B9D);
	};
	static_assert(sizeof(CEffectLightning) == 30 * 4);
	static_assert(offsetof(CEffectLightning, HasSpawnedFire) == 28*4);
}

namespace EGL {
	class CGLEEffectCreator : public BB::IObject {
	public:
		shok::EffectTypeId EffectType = shok::EffectTypeId::Invalid;
	private:
		int Zero1 = 0;
	public:
		shok::PlayerId PlayerID = shok::PlayerId::Invalid;
		shok::Position StartPos = { 0,0 }, CurrentPos = { 0,0 }, TargetPos = { 0,0 };
	private:
		int Zero2 = 0; // height offset unused
	public:
		shok::EntityId AttackerID = shok::EntityId::Invalid, TargetID = shok::EntityId::Invalid;
		int Damage = 0; // 13
		float DamageRadius = 0;
		shok::DamageClassId DamageClass = shok::DamageClassId::Invalid; // unused
		shok::PlayerId SourcePlayer = shok::PlayerId::Invalid; // 16
		bool Misses = false; // 17
	private:
		bool Zero5[2] = { false, false };
	public:
		byte AdvancedDamageSourceOverride = 0;


	public:
		static inline constexpr int vtp = 0x76E140;
		static inline constexpr int TypeDesc = 0x810254;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x78BD3AED);
		CGLEEffectCreator();

		// dtor empty
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;
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

namespace ED {
	class IEffect : public BB::IObject {
	public:
		// no vtable
		virtual void Initialze(EGL::IEffectDisplay* eff, ED::CEffectProps* props) = 0;
		virtual void UpdateGameTimeSynced(int ticks, float seconds) = 0;
		virtual void UpdateRealTimeSynced() = 0;

		EGL::IEffectDisplay* Effect;
	};

	class CEffectParticleSystem : public IEffect {
	public:
		RWE::RpClump* Clump;

		static inline constexpr int vtp = 0x7AE790;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x22C757DD);
	};

	class CHAnimEffect : public IEffect {
	public:
		RWE::RpClump* Clump; // 2
		RWE::Anim::RtAnimAnimation* Anim;
		RWE::Anim::RpHAnimHierarchy* AnimHandler; // 4

	public:
		static inline constexpr int vtp = 0x7AE98C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x11E11B07);
	};
	//constexpr int i = offsetof(CHAnimEffect, Clump) / 4;

	class CFlyingEffect : public IEffect {
	public:
		RWE::RpClump* Clump;

		static inline constexpr int vtp = 0x7AE9D0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFCD5C1E7);
	};
}

namespace GD {
	class CEffectSnow : public ED::IEffect {

	public:
		static inline constexpr int vtp = 0x76AB88;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE6F4B223);
	};

	class CDisplayEffectRain : public ED::IEffect {

	public:
		static inline constexpr int vtp = 0x76AD38;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8D16CD43);
	};

	class CDisplayEffectLightning : public ED::IEffect {
	public:
		PADDINGI(1);
		PADDINGI(3); // allocated pointers?
		ED::CEffectProps* Props;
		RWE::RwRGBAReal AmbientColorBackup, DiffuseColorBackup;

		static inline constexpr int vtp = 0x76AE08;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x72C52353);
	};
	static_assert(offsetof(CDisplayEffectLightning, Props) == 6 * 4);
	static_assert(sizeof(CDisplayEffectLightning) == 15 * 4);
}

namespace ED {
	class IDEVisibleEffectManager {
	public:
		virtual ~IDEVisibleEffectManager() = default;
		virtual void UpdateGameTimeSynced(int ticks, float sec) = 0;
		virtual void UpdateRealTypeSynced() = 0;
	};

	class CDEVisibleEffectManager : public IDEVisibleEffectManager {
		bool unknown;

	public:
		struct VisEffect {
			shok::EffectId ID;
		private:
			bool unknown;
			shok::EffectId ID2;
		public:
			ED::IEffect* Effect;
		};

		PADDINGI(1);
		shok::List<VisEffect> Effects;
		PADDINGI(4); // vector of list elements ???
		PADDINGI(2);


		static inline constexpr int vtp = 0x7AE368;

		static inline CDEVisibleEffectManager** const GlobalObj = reinterpret_cast<CDEVisibleEffectManager**>(0xA19EA0);

		ED::IEffect* GetDisplayForEffectID(shok::EffectId id);
		void DestroyDisplayForEffect(shok::EffectId id);
	};
	static_assert(sizeof(CDEVisibleEffectManager) == 12 * 4);
}

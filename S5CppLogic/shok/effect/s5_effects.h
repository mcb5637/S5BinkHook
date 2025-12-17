#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/engine/s5_RWEngine.h>
#include <shok/engine/s5_RWE_3d.h>

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
		[[nodiscard]] virtual shok::EffectTypeId __stdcall GetEffectType() const = 0;
		virtual void __stdcall FillTypeAndID(TypeAndID* d) const = 0;
		virtual void __stdcall FillTurnDurationAndPos(TurnDurationAndPos* d) const = 0;
		[[nodiscard]] virtual shok::PlayerId __stdcall GetPlayerID() const = 0;
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
		EventHandlerList EventHandlers;

		static inline constexpr int vtp = 0x784B28;
		static inline constexpr int TypeDesc = 0x822284;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF8536CED);

		virtual ~CEffect() override;
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void FromCreator(EGL::CGLEEffectCreator* ct); // 3
		virtual void OnCreated();
		virtual void OnLoaded(); // 5
		virtual void OnTick();
		virtual void FireEvent(BB::CEvent* ev);
	private:
		virtual void __stdcall emptyfunc2(); // 8

	public:

		[[nodiscard]] virtual shok::EffectTypeId __stdcall GetEffectType() const override;
		virtual void __stdcall FillTypeAndID(TypeAndID* d) const override;
		virtual void __stdcall FillTurnDurationAndPos(TurnDurationAndPos* d) const override;
		[[nodiscard]] virtual shok::PlayerId __stdcall GetPlayerID() const override;
		virtual EGL::ISlot* __stdcall GetFlyingEffectSlot() override;
	protected:
		virtual void OnOtherEntityAttachToMe_vt(shok::AttachmentType ty, shok::EntityId otherId, shok::EventIDs onThisDetachEvent) override; // other -> this
		virtual void OnOtherEffectAttachToMe_vt(shok::AttachmentType ty, shok::EffectId otherId, shok::EventIDs onThisDetachEvent) override; // other -> this
		virtual void OnOtherEntityDetachFromMe_vt(shok::AttachmentType ty, shok::EntityId otherId) override; // other -> this
		virtual void OnOtherEffectDetachFromMe_vt(shok::AttachmentType ty, shok::EffectId otherId) override; // other -> this
		virtual void DetachObservedEntity_vt(shok::AttachmentType ty, shok::EntityId otherId) override; // this -> other 5
		virtual void DetachObservedEffect_vt(shok::AttachmentType ty, shok::EffectId otherId) override; // this -> other
	public:
		virtual bool IsAttachedToEntity(shok::AttachmentType ty, shok::EntityId otherId) override; // this -> other
		virtual bool IsAttachedToEffect(shok::AttachmentType ty, shok::EffectId otherId) override; // this -> other

		CEffect();

		[[nodiscard]] CGLEEffectProps* LogicProps() const;

		static void(__stdcall*OnDestroyCb)(CEffect* th);
		static void HookOnDestroy();

		// set pos 588BB0 thiscall(pos*)
	};
	static_assert(offsetof(CEffect, StartTurn) == 18 * 4);

	struct SSlotArgsFlyingEffect { // NOLINT(*-pro-type-member-init)
		int StartTurn;
		shok::Position StartPosition;
		shok::Position TargetPosition;
		float GravityFactor;
		float b; // current height?
		float HeightStart;
		float Speed;
		float x; // angle rad, flight direction?
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
		float FlightDirection; // rad, "x" in seridata
		float Speed; // in effect 43

		// dtor empty
		virtual void __stdcall FillSlot(SSlotArgsFlyingEffect* data) override;
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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


		virtual ~CFlyingEffect() override;
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void FromCreator(EGL::CGLEEffectCreator* ct) override;
		virtual void OnCreated() override;
		virtual void OnTick() override;

		virtual EGL::ISlot* __stdcall GetFlyingEffectSlot() override;

		virtual void OnHit(); // 9
		virtual void CalculateGravityStuff(); // 10

		CFlyingEffect();

	protected:
		// ReSharper disable once CppHiddenFunction
		void FixOnLoad();

		bool OnTickMove();
	public:
		static void HookOnLoadFix();

		static void HookOnHit();
		static void (*FlyingEffectOnHitCallback)(EGL::CFlyingEffect* eff);
		static void (*FlyingEffectOnHitCallback2)(EGL::CFlyingEffect* eff);
		static EGL::CFlyingEffect* CurrentHittingEffect;
	};
	//constexpr int i = offsetof(CFlyingEffect, FlyingEffectSlot.Speed) / 4;

	struct SSlotArgsStartTurn {
		int StartTurn;
	};
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
		shok::AdvancedDealDamageSource AdvancedDamageSourceOverride;
		GGL::CArrowEffectProps* ArrowEffectProps;

	protected:
		// ReSharper disable once CppHidingFunction
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
		struct CompactedDamageClass {
		private:
			uint16_t Data;
		public:
			// ReSharper disable once CppDFAConstantParameter
			constexpr CompactedDamageClass(shok::DamageClassId d) : Data(static_cast<uint16_t>(static_cast<int>(d))) {}
			constexpr operator shok::DamageClassId() const {
				return static_cast<shok::DamageClassId>(static_cast<int>(Data));
			}
		};


		shok::EntityId AttackerID; // 47
		shok::PlayerId SourcePlayer; // 48
		GGL::CCannonBallEffectProps* CannonBallEffectProps;
		int DamageAmount; // 50
		float AoERange;
		CompactedDamageClass DamageClass{shok::DamageClassId{}}; // 52 originally full damageclass, but i need storage ;)
	private:
		byte DamageClassPadding = 0;
	public:
		shok::AdvancedDealDamageSource AdvancedDamageSourceOverride; // originally part of damageclass

	protected:
		// ReSharper disable once CppHidingFunction
		void FixOnLoad();
		void OnHitHooked();

		static void __fastcall CannonFromCreatorAdd(GGL::CCannonBallEffect* th, CProjectileEffectCreator* cr);
		static void __attribute((naked)) CannonFromCreatorAddASM();

	public:
		static bool FixDamageClass;
		static bool AddDamageSourceOverride;
		static void HookFromCreator();
		static void HookDealDamage();

		static inline constexpr int vtp = 0x777690;
		static inline constexpr int TypeDesc = 0x82365C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xDF09D9FD);
	};
	static_assert(sizeof(CCannonBallEffect) == 4 * 53);

	class CEffectLightning : public EGL::CEffect {
	public:
		bool HasSpawnedFire = false; // 28
		GGL::CGLEffectLightningProps* Props; //29

		static inline constexpr int vtp = 0x779CE0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x284C6B9D);

		// bug: no OnLoaded override to reset Props!
	};
	static_assert(sizeof(CEffectLightning) == 30 * 4);
	static_assert(offsetof(CEffectLightning, HasSpawnedFire) == 28*4);

	class CEffectRain : public EGL::CEffect {
	public:
		class CSlotStartTurn : public EGL::TSlot<EGL::SSlotArgsStartTurn, 268588221> {
			int StartTurn = 0;

			virtual void __stdcall FillSlot(EGL::SSlotArgsStartTurn* data) override;
		};

		CSlotStartTurn StartTurnSlot;

		// ctor 51711F

		static inline constexpr int vtp = 0x779C74;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x817186ED);
	};

	class CEffectFire : public EGL::CEffect {
	public:
		int LastSpreadTime;
		bool HasSpread;
		GGL::CGLEffectFireProps* FireProps;

		virtual void BurnEntity(shok::EntityId id) = 0;

		static inline constexpr int vtp = 0x7770D4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3448BED);

		// bug: no OnLoaded override to reset FireProps!
	};
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
		shok::AdvancedDealDamageSource AdvancedDamageSourceOverride = shok::AdvancedDealDamageSource::Unknown;


	public:
		static inline constexpr int vtp = 0x76E140;
		static inline constexpr int TypeDesc = 0x810254;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x78BD3AED);
		CGLEEffectCreator();

		// dtor empty
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;
	protected:
		void SetVT(int vt);
	};

	class CFlyingEffectCreator : public EGL::CGLEEffectCreator {
	public:

		static inline constexpr int vtp = 0x774688;
		static inline constexpr int TypeDesc = 0x819334;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3EA0B0BD);

		CFlyingEffectCreator();
	};
}

class CProjectileEffectCreator : public EGL::CFlyingEffectCreator {
public:

	static inline constexpr int vtp = 0x774698;
	static inline constexpr int TypeDesc = 0x81935C;
	static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9E562D7D);

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
		RWE::RpClump* Clump = nullptr;

		static inline constexpr int vtp = 0x7AE790;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x22C757DD);
	};

	class CHAnimEffect : public IEffect {
	public:
		RWE::RpClump* Clump = nullptr; // 2
		RWE::Anim::RtAnimAnimation* Anim = nullptr;
		RWE::Anim::RpHAnimHierarchy* AnimHandler = nullptr; // 4

	public:
		static inline constexpr int vtp = 0x7AE98C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x11E11B07);
	};
	//constexpr int i = offsetof(CHAnimEffect, Clump) / 4;

	class CFlyingEffect : public IEffect {
	public:
		RWE::RpClump* Clump = nullptr;

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

	class CDisplayEffectLightning : public ED::IEffect { // NOLINT(*-pro-type-member-init)
	public:
		size_t NumElements; // 2
		RWE::I3D::RwIm3DVertex* Vertex; // 4 per element?
		RWE::I3D::RwImVertexIndex* Indexes; // advanced by 12?
		RWE::RwRaster** Textures; // 5
		ED::CEffectProps* Props;
		RWE::RwRGBAReal AmbientColorBackup, DiffuseColorBackup;

		static inline constexpr int vtp = 0x76AE08;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x72C52353);

		// add element 0x487265 thiscaall(shok::Position*, shok::Position*, RwMatrix* ltm, RWE::RwRaster*)

		static void HookColorOverride(bool active);
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
		bool unknown = false;

	public:
		struct VisEffect {
			shok::EffectId ID{};
		private:
			bool unknown = false;
			shok::EffectId ID2{};
		public:
			ED::IEffect* Effect = nullptr;
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

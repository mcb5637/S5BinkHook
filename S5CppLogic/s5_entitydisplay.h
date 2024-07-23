#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_baseprops.h"
#include "s5_entity.h"

namespace ED {
	class CUVAnimBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AE8EC;
		static inline constexpr int TypeDesc = 0x84CF0C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x257D793D);
	};

	class CPermanentUVAnimBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AE914;
		static inline constexpr int TypeDesc = 0x84CFD0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x162356CD);
	};

	class CEffectParticleSystem;
	class CParticleEffectAttachmentBehavior : public ED::IBehavior {
	public:
		struct SAttach {
			shok::ModelId Model;
			int FrameId; // hardcoded 500 to 502
			PADDING(1); // bool?
			PADDING(3);
			ED::CEffectParticleSystem* Attached;

			// clear 0x719F35 __thiscall()
			// attach 0x4881A0 __thiscall(ED::CEntity*, shok::ModelId, int frameid)
		} Attach[3];
		ED::CEntity* EntityDisplay = nullptr;
		EGL::TSlot<EGL::SSlotArgsParticleEffectAttachment, -803571449>* Slot; //14

		static inline constexpr int vtp = 0x7AE93C;
		static inline constexpr int TypeDesc = 0x84D01C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2DB43D17);
	};
	static_assert(sizeof(CParticleEffectAttachmentBehavior::SAttach) == 16);
	static_assert(offsetof(CParticleEffectAttachmentBehavior, EntityDisplay) == 13 * 4);
	static_assert(sizeof(CParticleEffectAttachmentBehavior) == 15 * 4);

	class CParticleEffectSwitchBehavior : public ED::IBehavior {
	public:
		unsigned int PrevOnOffBits;
		ED::CEntity* EntityDisplay = nullptr; // 2
		EGL::TSlot<EGL::SSlotArgsParticleEffectSwitch, 591789671>* Slot;

		static inline constexpr int vtp = 0x7AE964;
		static inline constexpr int TypeDesc = 0x84D050;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD6E2E097);

		// uses frames [100,131] (if existing)
		// getAtomicEmitterOfFrame 0x720031 __thiscall(int frameid)
		// disable 0x7200DE __thiscall(int frameid)
		// enable 0x7200C2 __thiscall(int frameid)
	};
	static_assert(offsetof(CParticleEffectSwitchBehavior, EntityDisplay) == 2 * 4);
	static_assert(sizeof(CParticleEffectSwitchBehavior) == 4 * 4);

	class CDisplayBehaviorWaterfallAnimation : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AEA44;
		static inline constexpr int TypeDesc = 0x84D354;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x29B4B323);
	};

	class CDisplayStaticEntity;
	class CDisplayBehaviorMovement : public ED::IBehavior {
	public:
		ED::CDisplayStaticEntity* Entity;
		EGL::TSlot<EGL::SSlotArgsMovingEntity, 1383452519>* Slot;

		static inline constexpr int vtp = 0x7AEAA4;
		static inline constexpr int TypeDesc = 0x84D3E8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x166A8A03);
	};
	static_assert(offsetof(CDisplayBehaviorMovement, Entity) == 4 * 1);

	class CDisplayBehaviorBuildingAnimation : public ED::IBehavior {
	public:
		ED::CEntity* EntityDisplay = nullptr;
		EGL::CBehaviorAnimation::CSlotAnimation* Slot;


		static inline constexpr int vtp = 0x7AEAD4;
		static inline constexpr int TypeDesc = 0x84D4CC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x72DD7A54);
	};
	static_assert(offsetof(CDisplayBehaviorBuildingAnimation, Slot) == 2 * 4);

	class CDisplayStaticWithHierarchy;
	class CDisplayBehaviorAnimationNoBlending : public ED::IBehavior { // size 9
	public:
		ED::CDisplayStaticWithHierarchy* EntityDisplay = nullptr;
		EGL::TSlot<EGL::SSlotArgsAnimation, 796165920>* Slot = nullptr; // 2
		RWE::Anim::RpHAnimHierarchy* AnimHandlerData;
		shok::AnimationId CurrentAnimId; // 4
		float AnimSpeed; //?
		float StartTime; // 6
		float SpeedModifier;
		bool PlayBackwards;



		static inline constexpr int vtp = 0x7AEB08;
		static inline constexpr int TypeDesc = 0x84D5B4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1304C08D);
	};

	class CDisplayBehaviorAnimation : public CDisplayBehaviorAnimationNoBlending { // size 15
	public:
		RWE::Anim::RpHAnimHierarchy* AnimHandlerData2; // 9

		static inline constexpr int vtp = 0x7AEB40;
		static inline constexpr int TypeDesc = 0x84D6A4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB79FC943);
	};
	//constexpr int i = offsetof(CDisplayBehaviorAnimationNoBlending, Slot) / 4;





	class CEntity : public BB::IObject {
	public:
		EGL::IEntityDisplay* Entity;
		shok::EntityId EntityID;
		shok::ModelId ModelOverrideID;
		RWE::RpClump* Model;
		PADDINGI(1); // some kind of flags? // 5
		PADDINGI(1); // some counter
		shok::Vector<ED::IBehavior*> DisplayBehaviors; // 7
		PADDINGI(1); // unk p // 11
		PADDINGI(1); // 0
		PADDINGI(1); // unk p
		PADDINGI(1); // -1, color? // 14

		struct SelectionData {
			shok::Position Pos;
			float Radius;
			shok::SelectionTextureId Texture;
		};

	protected:
		virtual void __thiscall Initialize(shok::ModelId* modelOverride) = 0;
	public:
		virtual void __stdcall OnRenderUpdate(int tick, float seconds) = 0; // tick seems to be logic ticks, seconds in gametime (ticks/10)
	private:
		virtual void UnknownEDisplayFunc1() = 0; // 5 maybe update something
	public:
		virtual bool GetSelectionData(SelectionData* d) = 0;
	private:
		virtual void UnknownEDisplayFunc3() = 0;
		virtual void UnknownEDisplayFunc4() = 0;
		virtual void UnknownEDisplayFunc5(float) = 0; // stdcall? set some float?
	public:
		virtual RWE::Anim::RpHAnimHierarchy* GetAnimHierarchy() = 0; // 10
		virtual void __stdcall SetPositionData(EGL::IEntityDisplay::posdata* data) = 0;
	private:
		virtual float UnknownEDisplayFunc7() = 0; // return some float

	public:
		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, ED::IBehavior>
		T* GetDisplayBehavior() {
			for (ED::IBehavior* b : DisplayBehaviors) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
		template<typename T>
		requires std::derived_from<T, ED::IBehavior>
		T* GetDisplayBehaviorDynamic() {
			for (ED::IBehavior* b : DisplayBehaviors) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}
		// checks identifier, only returns exact class, no subclasses
		template<BB::HasValidIdentifier T>
		requires std::derived_from<T, ED::IBehavior>
		const T* GetDisplayBehavior() const {
			for (const ED::IBehavior* b : DisplayBehaviors) {
				if (b && b->GetClassIdentifier() == T::Identifier) {
					return static_cast<const T*>(b);
				}
			}
			return nullptr;
		}
		// dynamic_cast, may return subclasses
		template<typename T>
		requires std::derived_from<T, ED::IBehavior>
		const T* GetDisplayBehaviorDynamic() const {
			for (const ED::IBehavior* b : DisplayBehaviors) {
				if (b) {
					const T* r = dynamic_cast<const T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}

		void ResetPositionData();

		ED::ModelData* GetModelData();

		static inline constexpr int vtp = 0x76A494;
		static inline constexpr int TypeDesc = 0x8099C0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2A5844BD);
	};
	static_assert(sizeof(ED::CEntity) == 4 * 15);

	class CDisplayStaticEntity : public ED::CEntity {
	public:
		ED::CTerrainDecalBase* TerrainDecal; // 15 (maybe only aligned)
		PADDINGI(1); // 0

		static inline constexpr int vtp = 0x7AE3C4;
		static inline constexpr int TypeDesc = 0x8099DC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x814643);
	};
	static_assert(sizeof(CDisplayStaticEntity) == 4 * 17);

	class CDisplayStaticWithHierarchy : public CDisplayStaticEntity {
	public:
		RWE::Anim::RpHAnimHierarchy* Handler; // 17

		static inline constexpr int vtp = 0x76AE30;
		static inline constexpr int TypeDesc = 0x80AF54;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3D31B843);
	};
	static_assert(sizeof(CDisplayStaticWithHierarchy) == 4 * 18);
}

namespace GD {
	class CLimitedAttachmentBannerBehaviorProps : public ED::CBehaviorProps { // residence
	public:
		int Banner, Node;

		static inline constexpr int vtp = 0x76AB1C;
		static inline constexpr int TypeDesc = 0x80AC30;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x156D5E5D);
	};

	class CCamouflageBehaviorProps : public ED::CBehaviorProps {
	public:
		shok::ModelId CamouflageModelID; //4
		int CamouflageAlpha;

		static inline constexpr int vtp = 0x76AEA0;
		static inline constexpr int TypeDesc = 0x80B048;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x15CB25AD);
	};

	class CBuildingBehaviorProps : public ED::CBehaviorProps {
	public:
		int Banner, NumDamageEffects, NumDestroyEffects;

		static inline constexpr int vtp = 0x76AF1C;
		static inline constexpr int TypeDesc = 0x80B1C0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2A2142BD);
	};

	class CWaterfallAnimationBehaviorProps : public ED::CBehaviorProps { // seems to be unused
	public:
		int Layer01Speed, Layer02Speed, Layer03Speed;

		static inline constexpr int vtp = 0x7AEA34;
		static inline constexpr int TypeDesc = 0x84D320;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x38F70CA3);
	};




	class CAlphaBlendingBehavior : public ED::IBehavior {
	public:
		ED::CEntity* EntityDisplay = nullptr; // 1
		EGL::TSlot<GGL::SSlotArgsAlphaBlending, 765384995>* Slot;
		int uk;

		static inline constexpr int vtp = 0x76AA0C;
		static inline constexpr int TypeDesc = 0x80AA70;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xBDB4C703);
	};

	class CLimitedAttachmentBannerBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x76AB2C;
		static inline constexpr int TypeDesc = 0x80AC68;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1DE7EFFD);
	};

	class CCamouflageBehavior : public ED::IBehavior {
	public:
		ED::CEntity* EntityDisplay = nullptr;
		ED::CBehaviorProps* Props = nullptr;
		void* Slot; // EGL::TSlot<GGL::SSlotArgsCamouflage,983570077>
		bool UseAltModel;
		PADDING(3);

		static inline constexpr int vtp = 0x76AEB0;
		static inline constexpr int TypeDesc = 0x80B0B8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x26C048ED);
	};
	static_assert(sizeof(CCamouflageBehavior) == 5 * 4);

	class CBuildingBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x76AF54;
		static inline constexpr int TypeDesc = 0x80B1E8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x76786DD);
	};

}


namespace ED {
	class IVisibleEntityManager {
	public:
		virtual void Destroy() = 0;
	private:
		virtual void uk1() = 0;
		virtual void uk2() = 0;
	public:
		virtual ED::CEntity* GetDisplayForEntity(shok::EntityId eid) = 0;
		// 3 more funcs
		// dtor

		static inline constexpr int vtp = 0x76A180;
	};

	// creates ED::CEntity objects as the corresponding entity appears on screen, and deletes them when they go off screen.
	// (a entity hidden by FoW counts as offscreen in this case).
	class CVisibleEntityManager : public IVisibleEntityManager {
	public:
		PADDINGI(1); // p EGL::CGLEEntitiesDisplay
		PADDINGI(1); // p to ?
		PADDINGI(1); // p to ED::CRenderSettingsEx
		ED::CEntity* Entities[1]; // indexed by lower half of eid

		static inline constexpr int vtp = 0x76A1A4;

		ED::CEntity* CreateDisplayForEntity(EGL::IEntityDisplay* e);
		void DestroyDisplayForEntity(shok::EntityId eid);
	};
}


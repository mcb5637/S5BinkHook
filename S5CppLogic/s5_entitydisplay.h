#pragma once
#include "s5data.h"

namespace ED {
	class CBehaviorProps : public BB::IObject {
	public:
		int Class = 0;
		int Index = 0;
		int Unknown = 100; // 100?? 3

		static inline constexpr int vtp = 0x76AB0C;
		static inline constexpr int TypeDesc = 0x80AC10;
		static inline constexpr unsigned int Identifier = 0x1F78996D;

		static BB::SerializationData* SerializationData;
	};




	class IBehavior : public BB::IObject {
	public:
		ED::CEntity* EntityDisplay = nullptr;
		ED::CBehaviorProps* Props = nullptr;

	protected:
		virtual void __stdcall OnAdd(ED::CEntity* edispl, ED::CBehaviorProps* props, int uk) = 0; // called before ED::CEntity init
		virtual void __stdcall Initialize(ED::CEntity* edispl, ED::CBehaviorProps* props) = 0; // called before ED::CEntity init
	public:
		virtual void __stdcall UpdateRenderNoTick(int count, float uk) = 0;
		virtual void __stdcall UpdateRenderOneTick(int count, float uk) = 0;
		virtual void __stdcall UpdateRenderManyTick(int count, float uk) = 0;
	private:
		virtual void __stdcall UnknownFuncRet100() = 0;

	public:
		static inline constexpr int vtp = 0x76A9E4;
		static inline constexpr int TypeDesc = 0x80AA54;
	};

	class CUVAnimBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AE8EC;
		static inline constexpr int TypeDesc = 0x84CF0C;
	};

	class CPermanentUVAnimBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AE914;
		static inline constexpr int TypeDesc = 0x84CFD0;
	};

	class CParticleEffectAttachmentBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AE93C;
		static inline constexpr int TypeDesc = 0x84D01C;
	};

	class CParticleEffectSwitchBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AE964;
		static inline constexpr int TypeDesc = 0x84D050;
	};

	class CDisplayBehaviorWaterfallAnimation : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AEA44;
		static inline constexpr int TypeDesc = 0x84D354;
	};

	class CDisplayBehaviorMovement : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AEAA4;
		static inline constexpr int TypeDesc = 0x84D3E8;
	};

	class CDisplayBehaviorBuildingAnimation : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AEAD4;
		static inline constexpr int TypeDesc = 0x84D4CC;
	};

	class CDisplayBehaviorAnimationNoBlending : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x7AEB08;
		static inline constexpr int TypeDesc = 0x84D5B4;
	};

	class CDisplayBehaviorAnimation : public CDisplayBehaviorAnimationNoBlending {
	public:

		static inline constexpr int vtp = 0x7AEB40;
		static inline constexpr int TypeDesc = 0x84D6A4;
	};





	class CEntity : public BB::IObject {
	public:
		EGL::IEntityDisplay* Entity;
		int EntityID;
		int ModelOverrideID;
		ED::ModelInstance* Model;
		PADDINGI(1); // some kind of flags? // 5
		PADDINGI(1); // some counter
		shok::Vector<ED::IBehavior*> DisplayBehaviors; // 7
		PADDINGI(1); // unk p // 11
		PADDINGI(1); // 0
		PADDINGI(1); // unk p
		PADDINGI(1); // -1, color? // 14

	protected:
		virtual void __thiscall Initialize(int* m) = 0; // m might be p to model id?
	public:
		virtual void __stdcall OnRenderUpdate(int tick, float seconds) = 0; // tick seems to be logic ticks, seconds in gametime (ticks/10)
	private:
		virtual void UnknownEDisplayFunc1() = 0; // maybe update something
		virtual bool UnknownEDisplayFunc2(void*) = 0; // get some data to pointer, return bool?
		virtual void UnknownEDisplayFunc3() = 0;
		virtual void UnknownEDisplayFunc4() = 0;
		virtual void UnknownEDisplayFunc5(float) = 0; // stdcall? set some float?
		virtual int UnknownEDisplayFunc6() = 0; // empty func, ret 0
		virtual void __stdcall SetPositionData(EGL::IEntityDisplay::posdata* data) = 0;
		virtual float UnknownEDisplayFunc7() = 0; // return some float

		template<typename T>
		requires std::derived_from<T, ED::IBehavior>
		T* GetDisplayBehavior() {
			for (ED::IBehavior* b : DisplayBehaviors) {
				if (b) {
					T* r = dynamic_cast<T*>(b);
					if (r)
						return r;
				}
			}
			return nullptr;
		}

		void ResetPositionData();

		static inline constexpr int vtp = 0x76A494;
	};
	static_assert(sizeof(ED::CEntity) == 4 * 15);

	class CDisplayStaticEntity : public ED::CEntity {
	public:
		PADDINGI(1); // p ED::CTerrainDecalAligned // 15
		PADDINGI(1); // 0

		static inline constexpr int vtp = 0x7AE3C4;
	};
	static_assert(sizeof(CDisplayStaticEntity) == 4 * 17);

}

namespace GD {
	class CLimitedAttachmentBannerBehaviorProps : public ED::CBehaviorProps { // residence
	public:
		int Banner, Node;

		static inline constexpr int vtp = 0x76AB1C;
		static inline constexpr int TypeDesc = 0x80AC30;
		static inline constexpr unsigned int Identifier = 0x156D5E5D;
	};

	class CCamouflageBehaviorProps : public ED::CBehaviorProps {
	public:
		int CamouflageModelID; //4
		int CamouflageAlpha;

		static inline constexpr int vtp = 0x76AEA0;
		static inline constexpr int TypeDesc = 0x80B048;
		static inline constexpr unsigned int Identifier = 0x15CB25AD;
	};

	class CBuildingBehaviorProps : public ED::CBehaviorProps {
	public:
		int Banner, NumDamageEffects, NumDestroyEffects;

		static inline constexpr int vtp = 0x76AF1C;
		static inline constexpr int TypeDesc = 0x80B1C0;
		static inline constexpr unsigned int Identifier = 0x2A2142BD;
	};

	class CWaterfallAnimationBehaviorProps : public ED::CBehaviorProps { // seems to be unused
	public:
		int Layer01Speed, Layer02Speed, Layer03Speed;

		static inline constexpr int vtp = 0x7AEA34;
		static inline constexpr int TypeDesc = 0x84D320;
		static inline constexpr unsigned int Identifier = 0x38F70CA3;
	};




	class CAlphaBlendingBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x76AA0C;
		static inline constexpr int TypeDesc = 0x80AA70;
	};

	class CLimitedAttachmentBannerBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x76AB2C;
		static inline constexpr int TypeDesc = 0x80AC68;
	};

	class CCamouflageBehavior : public ED::IBehavior {
	public:
		void* Slot; // EGL::TSlot<GGL::SSlotArgsCamouflage,983570077>
		bool UseAltModel;
		PADDING(3);

		static inline constexpr int vtp = 0x76AEB0;
		static inline constexpr int TypeDesc = 0x80B0B8;
		static inline constexpr unsigned int Identifier = 0x26C048ED;
	};
	static_assert(sizeof(CCamouflageBehavior) == 5 * 4);

	class CBuildingBehavior : public ED::IBehavior {
	public:

		static inline constexpr int vtp = 0x76AF54;
		static inline constexpr int TypeDesc = 0x80B1E8;
	};

}


namespace ED {
	// creates ED::CEntity objects as the corresponding entity appears on screen, and deletes them when they go off screen.
	// (a entity hidden by FoW counts as offscreen in this case).
	class CVisibleEntityManager {
	public:
		PADDINGI(1); // p EGL::CGLEEntitiesDisplay
		PADDINGI(1); // p to ?
		PADDINGI(1); // p to ED::CRenderSettingsEx
		ED::CEntity* Entities[1]; // indexed by lower half of eid

		static inline constexpr int vtp = 0x76A1A4;

	private:
		virtual void UnknownVEntManFunc1() = 0;

	public:
		ED::CEntity* CreateDisplayForEntity(EGL::IEntityDisplay* e);
		ED::CEntity* GetDisplayForEntity(int eid);
		void DestroyDisplayForEntity(int eid);
	};
}

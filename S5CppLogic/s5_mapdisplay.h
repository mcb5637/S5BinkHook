#pragma once
#include "s5data.h"

namespace ED {
	class ILandscape {
	public:
		virtual ~ILandscape() = default;
	private:
		virtual void unknown() = 0;
	public:
		virtual float GetTerrainHeightAtPosF(float x, float y) = 0;
		virtual float GetWaterHeightAtPosF(float x, float y) = 0;
		virtual float GetHigherTerrainOrWaterHeightAtPos(float x, float y) = 0;
	protected:
		virtual bool GetLandscapePosFromMousePos(void* cam, float* mousepos, shok::PositionRot* outpos, int flag) = 0;
	};

	class CLandscape : public ILandscape {
	public:
		EGL::CGLETerrainHiRes* TerrainHiRes;
		EGL::CGLETerrainLowRes* TerrainLowRes;
		EGL::LandscapeBlockingData* Blocking;
		int SizeDiv32X, SizeDiv32Y;
		int HiResSizeX, HiResSizeY;
		float WorldSizeX, WorldSizeY;

		static inline constexpr int vtp = 0x76A404;

		bool GetTerrainPosAtScreenCoords(shok::PositionRot& outpos, int x, int y); // r in this case is the terrain height at the position
		float GetTerrainHeightAtPos(const shok::Position& p);
		float GetWaterHeightAtPos(const shok::Position& p);

	private:
		virtual float retzero(int, int) = 0;
	};
	static_assert(sizeof(ED::CLandscape) == 10 * 4);

	class CGlobalsLogic {
	public:
		virtual ~CGlobalsLogic() = default;
	};
	class CGlobalsLogicEx : public CGlobalsLogic {
	public:
		EGL::CGLEGameLogic* GameLogic;
		PADDINGI(4); // p EGL::CGLEEntitiesDisplay, p EGL::CGLEEffectsDisplay, p EGL::CGLETerrainHiRes, p EGL::CGLETerrainLowRes
		EGL::LandscapeBlockingData* Blocking; // 6
		PADDINGI(2); // p EGL::CGLELandscape, p EGL::CTerrainVertexColors
		EGL::CRegionInfo* RegionInfo; // 9
		PADDINGI(1); // p EGL::CPlayerExplorationHandler
		ED::CLandscape* Landscape;
		// p ED::CLandscapeFogOfWar, 
		PADDINGI(7);
		ED::CVisibleEntityManager* VisibleEntityManager; // 19

		static inline constexpr int vtp = 0x769F74;

		void ToTerrainCoord(const shok::Position& p, int* out);
		bool IsCoordValid(int* out);
		EGL::CGLELandscape::BlockingMode GetBlocking(const shok::Position& p);

		static inline ED::CGlobalsLogicEx** const GlobalObj = reinterpret_cast<ED::CGlobalsLogicEx**>(0x8581EC);
	};

	class CModelsProps { // models.xml gets loaded into this 71D6BD
	public:
		virtual ~CModelsProps() = default;


		static inline constexpr int vtp = 0x7AE60C;
	};

	class CCamera {
	public:
		virtual ~CCamera() = default;
	};
	class CCameraEx : public CCamera {
	public:
		PADDINGI(1); //1
		void* SomeCameraData;

		static inline constexpr int vtp = 0x769E64;
	};

	class IPlayerColors {
	public:
		virtual ~IPlayerColors() = default;
	};
	class CPlayerColors : public IPlayerColors {
	public:
		PADDINGI(63);
		shok::Color Colors[17]; // 8 player

		static inline constexpr int vtp = 0x76964C;

		shok::Color GetColorByIndex(int i);
		void SetColorByIndex(int i, shok::Color c);
		void RefreshPlayerColors();
	};

	class ICommandAcknowledgements {
	public:
		virtual unsigned int __stdcall GetIdentifier() = 0;
		virtual ~ICommandAcknowledgements() = default;
	private:
		virtual void unknown0() = 0;
	public:
		virtual void ShowAck(float x, float y) = 0;
	};
	class CCommandAcknowledgements : public ICommandAcknowledgements {
	public:
		using ICommandAcknowledgements::ShowAck;

		static inline constexpr int vtp = 0x7695C0;

		void ShowAck(const shok::Position& pos);
	};

	struct ModelInstance {
		enum class TransformOperation : int {
			Set = 0, // sets to identity, then performs the operation
			Multiply = 1, // the new matrix is performed last
			ReverseMultiply = 2, // the new matrix is performed first
		};


		PADDINGI(1);
		struct {
			PADDINGI(4);
			float Matrix[4 * 4];
		}*Transform;

		void Register();
		void Destroy();
		void Rotate(float r, TransformOperation op);
		void Rotate(float r, float* axis, TransformOperation op);
		void Scale(float* s, TransformOperation op); // 3 coordinates (order?)
		void Scale(float s, TransformOperation op);
		void Translate(const shok::Position& p, float height, TransformOperation op);
		void SetColorByPlayerID(int pl);
		void DisableShadow();
		void DisableParticleEffects();
		void DisableTerrainDecal();
		//argb =  r | (g << 8) | (b << 16) | (a << 24)
		void SetColorModulate(shok::Color argb);
		void SetColorModulate(int a, int r, int g, int b);
	};
	struct ModelData {

		ED::ModelInstance* Instanciate() const;
	};

	class IResourceManager {
	public:
		virtual ~IResourceManager() = default;
	private:
		virtual void unknown0() = 0;
		virtual void unknown1() = 0;
		virtual void unknown2() = 0;
		virtual void unknown3() = 0;
		virtual void unknown4() = 0;
	public:
		virtual const ED::ModelData* GetModelDataByEntityType(int ety) = 0; //6
		virtual const ED::ModelData* GetModelData(int modelid) = 0;
	private:
		virtual void unknown5() = 0;
		virtual void unknown6() = 0;
		virtual void unknown7() = 0;
	};

	class CResourceManager : public IResourceManager {
		PADDINGI(1); // conat char**?
		void* ModelData; // most likely a vector
		PADDINGI(3);

		static inline constexpr int vtp = 0x769824;
	};

	class IWorld {
	public:
		virtual ~IWorld() = default;
	};
	class CWorld : public IWorld {
	public:
		void* SomeRenderObj;

		static inline constexpr int vtp = 0x769E94;
	};

	class CGlobalsBase {
	public:
		virtual ~CGlobalsBase() = default;
	};
	class CGlobalsBaseEx : public CGlobalsBase {
	public:
		BB::CIDManagerEx* AnimManager;
		BB::CIDManagerEx* ModelManager;
		BB::CIDManagerEx* SpecialEffectManager; // lightning and snow textures?
		PADDINGI(1); // empty BB::CIDManagerEx
		ED::CDisplayProps* DisplayProps; // 5
		BB::CIDManagerEx** EffectManager; // probably an object without vtable
		BB::CIDManagerEx** EntityTypeManager; // probably an object without vtable
		ED::CModelsProps* ModelProps;
		PADDINGI(1); // p to something terrain related?
		PADDINGI(1); // p to something water related? // 10
		BBRw::CEngine* RWEngine; // p to BBRw::CEngine
		PADDINGI(1); // p to ED::CAuras
		ED::CCameraEx* Camera;
		ED::CCommandAcknowledgements* CommandAcks;
		PADDINGI(1); // p to ED::CEntitiesTypeFlags // 15
		PADDINGI(1); // p to ED::CGUIScene
		PADDINGI(1); // unknown
		PADDINGI(1); // p to ED::CLight
		PADDINGI(1); // unknown
		PADDINGI(1); // p to ED::COcclusionEffect
		PADDINGI(1); // p to ED::COrnamentalItems
		ED::CPlayerColors* PlayerColors; // 22
		PADDINGI(3); // 25 p ED::CRenderSettingsEx
		ED::CResourceManager* ResManager; // 26
		PADDINGI(7);
		ED::CWorld* DisplayWorld;

		static inline constexpr int vtp = 0x769478;

		static inline ED::CGlobalsBaseEx** const GlobalObj = reinterpret_cast<ED::CGlobalsBaseEx**>(0x857E8C);
	};
	//constexpr int i = offsetof(ED::CGlobalsBaseEx, DisplayWorld) / 4;

}

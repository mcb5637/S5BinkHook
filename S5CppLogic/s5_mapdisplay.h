#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_maplogic.h"
#include "s5_RWEngine.h"

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

	class CModelsProps {
	public:
		virtual ~CModelsProps() = default;

		struct ModelData {
			char* Effect = nullptr;
			char* OrnamentalItemEffect = nullptr;
			char* SelectionTexture = nullptr;
			float OnePassAlphaBlendingDistance = 0;
			float TerrainLightColorPortion = 0;
			float TerrainVertexColorPortion = 0;
			float SelectionOffsetX = 0; // 6
			float SelectionOffsetY = 0;
			float SelectionRadius = 0;
			bool CastShadow = true, DoNotDestroyHierarchy = false, HasUVAnim = false, OcclusionCaster = false;
			bool OcclusionReceiver = false, OrnamentalItem = false, ShareShadowRaster = false, UseAlphaBlending = false;
			bool UseSurfaceHeight = false, TwoPassAlphaBlending = false, AlphaBlendHighQualityOnly = false; PADDING(1);
			int DefaultAnimID = 0; // 12

			ModelData() = default;
			ModelData(ModelData&& o) noexcept;
			~ModelData();
			void operator=(const ModelData&) noexcept;

			static inline const BB::SerializationData* const SerializationData = reinterpret_cast<const BB::SerializationData*>(0xA19F90);
		};

		shok::Vector<ModelData> Model;
		BB::CIDManagerEx* ModelIdManager;

		static inline const BB::SerializationData* const SerializationData = reinterpret_cast<const BB::SerializationData*>(0xA1A2B0);

		static inline constexpr int vtp = 0x7AE60C;

		void LoadModelDataFromExtraFile(int id);
		// remember to free in idmanager and CResourceManager, free last id first
		void PopModel(int id);
		void ReloadAllModels();
	};
	static_assert(sizeof(CModelsProps::ModelData) == 13 * 4);

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

	struct ModelData {
		RWE::RpClump* Clump = nullptr;
		PADDINGI(5);
		void* UVAnim = nullptr; // 6
		PADDINGI(2);
		char* ModelName = nullptr; // 9
		PADDINGI(7);
		float SelectionOffsetX = 0; // 17
		float SelectionOffsetY = 0;
		float SelectionRadius = 0;
		void* SelectionTexture = nullptr; // texture loaded via RWE?
		int DefaultAnimID = 0;


		RWE::RpClump* Instanciate() const;

		ModelData() = default;
		ModelData(ModelData&& o) noexcept;
		~ModelData();

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	//constexpr int i = offsetof(ModelData, ModelName) / 4;
	static_assert(sizeof(ModelData) == 22 * 4);

	template<class T>
	class CDEResourceList {
	public:
		shok::Map<int, T*> Map;
		//check params
		virtual void Load(int id, shok::String* name) {
			throw 0;
		}
		virtual T* Get(int id) {
			throw 0;
		}
	};

	class IResourceManager {
	public:
		virtual ~IResourceManager() = default;
		virtual void Destroy() = 0;
		virtual RWE::RtAnimAnimation* GetAnimData(int animId) = 0;
	private:
		virtual void unknown2() = 0; // get from UnknownManager, then return float?
	public:
		virtual RWE::RwTexture* GetTextureData(int textureId) = 0;
		virtual RWE::RwTexture* GetSelectionTexture() = 0;
		virtual const RWE::RpClump* GetModelDataByEntityType(int ety) = 0; //6
		virtual const ED::ModelData* GetModelData(int modelid) = 0;
		virtual const RWE::RpClump* GetModelClumpByModelId(int id) = 0;
	private:
		virtual void unknown6() = 0;// get from UnknownManager, return something?
		virtual void* GetModelManager() = 0;
	};

	class CResourceManager : public IResourceManager {
		PADDINGI(1); // const char**?
		struct {
			shok::Vector<ModelData*> Models; // get lazily loaded
			BB::CIDManagerEx* ModelIDManager;
		} ModelManager;
		struct {
			shok::Vector<void*> Data;
			BB::CIDManagerEx* IDManager;
		} UnknownManager; // maybe also models? the idmanager seems to be the same
		CDEResourceList<RWE::RtAnimAnimation> AnimManager;
		CDEResourceList<RWE::RwTexture> TextureManager; // seems to be only lightning and snowflake textures
		RWE::RwTexture* SelectionTexture;

		static inline constexpr int vtp = 0x769824;

		// gets reloaded when accessed next
		void FreeModel(int id);
		// remember to free in idmanager and CModelsProps, free last id first
		void PopModel(int id);

		// remember to free in idmanager too, free last id first
		void FreeAnim(int id);


	private:
		ModelData* LoadModel(const char* name);
	};

	class IWorld {
	public:
		virtual ~IWorld() = default;
	};
	class CWorld : public IWorld {
	public:
		RWE::RpWorld* World;

		static inline constexpr int vtp = 0x769E94;
	};

	class TerrainTextureManager { // size 25
	public:
		struct TerrainType {
			int Id = 0;
			int Priority = 0;
			int BaseTextureId = 0;
			int SnowTextureId = 0;
			int TransitionTextureId = 0;
			float OneDiv4TimesQuads = 0;
			int ReplacementTerrainType = 0;
			bool TransitionsColorModulate = false;
			bool TransitionTextureIs_Transitions01 = false;
		};

		GGlue::TerrainPropsDisplay* DisplayProps;
		shok::Vector<TerrainType> TerrainTypes; // 1 modified if lower graphics settings
		shok::Vector<TerrainType> OriginalTerrainTypes; // 5 stays as it is
		shok::Vector<RWE::RwTexture*> Textures; // 9, lazily loaded
		shok::Vector<int> TextureIdsOrderedByPriority; // 13
		int BlackInternalUseOnlyId; // 17 terrainid
		int Transitions01Id; // 18 textureid
		shok::Vector<int> TextureReplacement; // 19 terrainindex->terrainindex
		int TextureQualityOption;
		int TextureQualityOptionChangedCounter; // ?

		void ReloadAllTextures();
		void ApplyTextureQuality();

		void PopTexture(int id);
		void LoadTexture(int id);
		void ReApplyTerrainType(int id);
		void ReApplyAllTerrainTypes();
	private:
		TerrainType CreateTerrainType(int id);
	};
	static_assert(sizeof(TerrainTextureManager::TerrainType) == 8 * 4);
	static_assert(sizeof(TerrainTextureManager) == 25 * 4);
	class TerrainDisplay { // size 9
	public:
		PADDINGI(2);
		TerrainTextureManager* TextureManager;
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
		EGL::EffectsDisplayProps* EffectDisplayProps;
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
		PADDINGI(3);
		TerrainDisplay* TerrainManager; // 30
		PADDINGI(3);
		ED::CWorld* DisplayWorld; // 34

		static inline constexpr int vtp = 0x769478;

		static inline ED::CGlobalsBaseEx** const GlobalObj = reinterpret_cast<ED::CGlobalsBaseEx**>(0x857E8C);
	};
	//constexpr int i = offsetof(ED::CGlobalsBaseEx, TerrainTextureManager) / 4;
	static_assert(sizeof(CGlobalsBaseEx) == 35 * 4);

	class IDisplayBase {
	public:
		virtual ~IDisplayBase() = default;
		virtual void Destroy() = 0;
		virtual void ReloadAllTerrainTextures() = 0;
	};

	class CDisplayBase : public IDisplayBase {
	public:
		CGlobalsBaseEx* GlobalsBase;
		PADDINGI(1);
		CDisplayProps* DisplayProps; // 3
		CModelsProps* ModelsProps;

		static inline constexpr int vtp = 0x7AE3AC;
	};
	static_assert(sizeof(CDisplayBase) == 5 * 4);

	class IDisplayRenderCallbacks {
		virtual void __stdcall unknown(float a) = 0;
	};
}

namespace GD {
	class CBuildingEffectsProps : public BB::IObject {
	public:
		float HealthNoDamageEffects;
		float HealthAllDamageEffects;
		shok::Vector<int> DamageEffect;
		shok::Vector<int> DestroyEffect;
		PADDINGI(1);

		virtual unsigned int __stdcall GetClassIdentifier() const override;

		static inline constexpr int vtp = 0x76A810;
		static inline constexpr unsigned int Identifier = 0xFC5B7F67;
	};
	static_assert(sizeof(CBuildingEffectsProps) == 12 * 4);

	class CGlobalEffect {
	public:
		virtual ~CGlobalEffect() = default;

		PADDINGI(3); // this+2 == ED::CEffectDisplay ??
		int EffectID; // 3

		static inline constexpr int vtp = 0x76AFA4;
	};


	class CGlobalEffects {
	public:
		virtual ~CGlobalEffects() = default;
		CGlobalEffect* RainEfect;
		CGlobalEffect* SnowEffect;

		static inline constexpr int vtp = 0x76A984;
	};

	class IDDisplay {
	public:
		virtual ~IDDisplay() = default;
		// 14 more methods
	};

	class CDDisplay : public IDDisplay, public ED::IDisplayRenderCallbacks {
	public:
		PADDINGI(2); // bool int?
		CBuildingEffectsProps BuildingEffectsProps;
		PADDINGI(3);
		ED::CDisplayBase* DisplayBase;
		PADDINGI(1);
		PADDINGI(1); // p to GD::CGlobalsBase, size 8
		CGlobalEffects* GlobalEfects;

		static inline constexpr int vtp = 0x76A828;

		static inline CDDisplay** const GlobalObj = reinterpret_cast<CDDisplay**>(0x8585B8);
	};
	static_assert(offsetof(CDDisplay, DisplayBase) == 19 * 4);
	static_assert(offsetof(CDDisplay, GlobalEfects) == 22 * 4);
	static_assert(sizeof(CDDisplay) == 23 * 4);
	//constexpr int i = offsetof(CDDisplay, GlobalEfects) / 4;
}

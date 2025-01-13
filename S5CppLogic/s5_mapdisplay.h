#pragma once
#include "enumflags.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_maplogic.h"
#include "s5_BBRw.h"
#include "s5_idmanager.h"
#include "s5_glue.h"
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
		virtual bool GetLandscapePosFromMousePos(RWE::RwCamera* cam, float* mousepos, shok::PositionRot* outpos, int flag) = 0;
	};

	class CLandscape : public ILandscape {
		friend class EGL::CGLETerrainLowRes;
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

		float __thiscall GetWaterHeightAtPosOverride(float x, float y);
	};
	static_assert(sizeof(ED::CLandscape) == 10 * 4);

	class ITerrainDecal : public BB::CNonCopyable {
	public:
		static inline constexpr int vtp = 0x76A1F4;
		virtual ~ITerrainDecal() = default;
		virtual void Destroy() = 0;
		virtual void SetPos(float x, float y) = 0;
		// 1 more funcs, empty in alignment
	};
	class CTerrainDecalBase : public ITerrainDecal {
	public:
		PADDINGI(1); // int enum? 1st param
		RWE::RwRaster* Texture;
		PADDINGI(1); // 0
		PADDINGI(6); // memset 0

		static inline constexpr int vtp = 0x76A238;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5A4A43C1); // from entitydisplay casttoident

		//ctor 47826F thiscall(int, texraster)
	};
	#ifdef __INTELLISENSE__
	#pragma diag_suppress 2784
	#endif
	static_assert(sizeof(CTerrainDecalBase) == 10 * 4);
	#ifdef __INTELLISENSE__
	#pragma diag_default 2784
	#endif
	class CTerrainDecalAligned : public CTerrainDecalBase { // size 10
	public:
		static inline constexpr int vtp = 0x76A260;
	};
	class CTerrainDecal : public CTerrainDecalBase { // size 14
	public:
		shok::Position Pos;
		shok::Position Size;


		static inline constexpr int vtp = 0x76A24C;

		// ctor 4782BC thiscall(int, textraster, x, y, sx, sy)
	};
	#ifdef __INTELLISENSE__
	#pragma diag_suppress 2784
	#endif
	static_assert(offsetof(CTerrainDecal, Pos.X) == 10 * 4);
	static_assert(sizeof(CTerrainDecal) == 14 * 4);
	#ifdef __INTELLISENSE__
	#pragma diag_default 2784
	#endif

	class ITerrainDecals : public BB::CNonCopyable {
	public:
		static inline constexpr int vtp = 0x76A208;

		virtual ~ITerrainDecals() = default;
		virtual void Destroy() = 0;
		virtual CTerrainDecal* CreateTerrainDecal(int, RWE::RwRaster* texture, float x, float y, float sx, float ys) = 0;
		virtual CTerrainDecalAligned* CreateAlignedDecal(int, const shok::Position* p, void*) = 0;
		// 6 more funcs

	};
	class CTerrainDecals : public ITerrainDecals {
	public:
		float MapsizeX;
		float MapsizeY;
		float MapsizeX2;
		float MapsizeY2;
		int MapsizeXBase;
		int MapsizeYBase;
		int MapsizeXDiv32;
		int MapsizeYDiv32; // 8
		struct {
			PADDINGI(4);
		} Data[576];
		void* EmptyDecal; // 2313
		void* DecalShadow;
		void* DecalSelection;
		void* DecalDoodad;
		RWE::RwTexture* XD_Decals;


		static inline constexpr int vtp = 0x76A274;
	};
	//constexpr int i = offsetof(CTerrainDecals, DecalShadow) / 4;

	class CSelectionList {
	public:
		virtual ~CSelectionList() = default;

		shok::Vector<shok::EntityId> Selected;

		static inline constexpr int vtp = 0x76A1C8;
	};
	static_assert(sizeof(CSelectionList) == 4 * 5);

	class CSelectionDecalList {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual ~CSelectionDecalList() = default;

		struct Data {
			bool Toggle;
			shok::EntityId Entity;
			ITerrainDecal* Selection;
		};
		shok::Map<shok::EntityId, Data*> Selected;
		bool Toggle;

		// update 4776CB stdcall()


		static inline constexpr int vtp = 0x76A1D4;
	};
	static_assert(sizeof(CSelectionDecalList) == 4 * 5);

	class CGlobalsLogic {
	public:
		virtual ~CGlobalsLogic() = default;
	};
	class CGlobalsLogicEx : public CGlobalsLogic {
	public:
		EGL::CGLEGameLogic* GameLogic;
		PADDINGI(2); // p EGL::CGLEEntitiesDisplay, p EGL::CGLEEffectsDisplay, 
		EGL::CGLETerrainHiRes* TerrainHi;
		EGL::CGLETerrainLowRes* TerrainLow; // 5
		EGL::LandscapeBlockingData* Blocking; // 6
		EGL::CGLELandscape* EGL_Landscape;
		EGL::CTerrainVertexColors* VertexColors;
		EGL::CRegionInfo* RegionInfo; // 9
		EGL::CPlayerExplorationHandler* ExplorationHandler; // 10
		ED::CLandscape* Landscape;
		PADDINGI(3); // p ED::CLandscapeFogOfWar, p ED::CLandscapeRegions, p ED::CPlayers
		CSelectionDecalList* SelectionDecals; // 15
		CSelectionList* SelectionList;
		CTerrainDecals* TerrainDecalsManager; // 17
		PADDINGI(1); // unknown obj
		ED::CVisibleEntityManager* VisibleEntityManager; // 19

		static inline constexpr int vtp = 0x769F74;

		void ToTerrainCoord(const shok::Position& p, int* out);
		bool IsCoordValid(int* out);
		EGL::CGLELandscape::BlockingMode GetBlocking(const shok::Position& p);

		// 71AD6D update?

		static inline ED::CGlobalsLogicEx** const GlobalObj = reinterpret_cast<ED::CGlobalsLogicEx**>(0x8581EC);
	};
	//constexpr int i = offsetof(CGlobalsLogicEx, TerrainTecalsManager) / 4;
	static_assert(offsetof(CGlobalsLogicEx, SelectionDecals) == 15 * 4);
	static_assert(offsetof(CGlobalsLogicEx, TerrainDecalsManager) == 17 * 4);

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
			bool CastShadow = true, DoNotDestroyHierarchy = false, HasUVAnim = false, OcclusionCaster = false; // 9
			bool OcclusionReceiver = false, OrnamentalItem = false, ShareShadowRaster = false, UseAlphaBlending = false; // 10
			bool UseSurfaceHeight = false, TwoPassAlphaBlending = false, AlphaBlendHighQualityOnly = false; PADDING(1); // 11
			shok::AnimationId DefaultAnimID = {}; // 12

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

		void LoadModelDataFromExtraFile(shok::ModelId id);
		// remember to free in idmanager and CResourceManager, free last id first
		void PopModel(shok::ModelId id);
		void ReloadAllModels();
		ModelData& Get(shok::ModelId id);
	};
	static_assert(sizeof(CModelsProps::ModelData) == 13 * 4);

	class CCamera {
	public:
		virtual ~CCamera() = default;
		// 1 more func

		PADDINGI(1); //1
		RWE::RwCamera* Camera;
		RWE::RwRGBA ClearColor;

		void Clear();
		bool BeginRender();
		bool EndRender();
		void Show(HWND window);
	};
	class CCameraEx : public CCamera {
	public:
		RWE::RpWorld* World;

		static inline constexpr int vtp = 0x769E64;

		// ctor 46FCF4 (x, y, bit, ?)
	};
	class CGUICamera : public CCamera {
	public:

		static constexpr int vtp = 0x769EB0;

		// ctor 4700B3 (x, y, bit)
	};

	class IPlayerColors {
	public:
		virtual ~IPlayerColors() = default;
		virtual void Destroy() = 0;
		virtual void Init() = 0;
		virtual void SetPlayerColorMapping(shok::PlayerId pid, int col) = 0;
	};
	class CPlayerColors : public IPlayerColors {
	public:
		struct PlayerUIColor {
			byte B, G, R, A;
		};

		PlayerUIColor CurrentPlayerUIColors[9];
		shok::Color UnknownColors[9]; // probably unused
		RWE::RwRGBAReal CurrentPlayerModelColors[9];

		shok::Color CurrentPlayerMiniMapColors[9];

		shok::Color ConfigColors[17]; // 64 8 player
		int PlayerColorMapping[9]; // 81

		static inline constexpr int vtp = 0x76964C;

		shok::Color GetColorByIndex(int i);
		void SetColorByIndex(int i, shok::Color c);
		void RefreshPlayerColors();
	};
	static_assert(offsetof(CPlayerColors, CurrentPlayerUIColors) == 1 * 4);
	static_assert(offsetof(CPlayerColors, CurrentPlayerModelColors) == 19 * 4);
	static_assert(offsetof(CPlayerColors, CurrentPlayerMiniMapColors) == 55 * 4);
	static_assert(offsetof(CPlayerColors, ConfigColors) == 64 * 4);
	static_assert(offsetof(CPlayerColors, PlayerColorMapping) == 81 * 4);
	static_assert(sizeof(CPlayerColors) == 90 * 4);

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
		enum class ModelFlags : unsigned int {
			None = 0,
			HasUnknownAddon = 1,
			ShareShadowRaster = 2,
			HasAnotherUnknownAddon = 4,
			CastShadow = 8,
			UseAlphaBlending = 0x10,
			UseAlphaBlending_1 = 0x20,
			UseAlphaBlending_DistanceGreater0 = 0x40,
			UseSurfaceHeight = 0x200,
			HierarchyDestroyed = 0x400,
			AlphaBlendHighQualityOnly = 0x800,
		};

		RWE::RpClump* Clump = nullptr;
		PADDINGI(4);
		ModelFlags Flags;
		void* UVAnim = nullptr; // 6
		float OnePassAlphaBlendingDistanceSquared = 0;
		PADDINGI(1);
		char* ModelName = nullptr; // 9
		PADDINGI(6);
		void* TerrainDecal = nullptr;
		float SelectionOffsetX = 0; // 17
		float SelectionOffsetY = 0;
		float SelectionRadius = 0;
		shok::SelectionTextureId SelectionTexture = {};
		shok::AnimationId DefaultAnimID = {};


		RWE::RpClump* Instanciate() const;

		ModelData() = default;
		ModelData(ModelData&& o) noexcept;
		~ModelData();

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	//constexpr int i = offsetof(ModelData, ModelName) / 4;
	static_assert(sizeof(ModelData) == 22 * 4);
	template<>
	class ::enum_is_flags<ED::ModelData::ModelFlags> : public std::true_type {};

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
		virtual RWE::RtAnimAnimation* GetAnimData(shok::AnimationId animId) = 0;
		virtual float GetAnimationDuration(shok::AnimationId animid) = 0;
		virtual RWE::RwTexture* GetTextureData(int textureId) = 0;
		virtual RWE::RwTexture* GetSelectionTexture() = 0;
		virtual const RWE::RpClump* GetModelDataByEntityType(shok::EntityTypeId ety) = 0; //6
		virtual const ED::ModelData* GetModelData(shok::ModelId modelid) = 0;
		virtual const RWE::RpClump* GetModelClumpByModelId(shok::ModelId id) = 0;
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
		void FreeModel(shok::ModelId id);
		// remember to free in idmanager and CModelsProps, free last id first
		void PopModel(shok::ModelId id);
		const ED::ModelData* LoadModel(shok::ModelId id);

		// remember to free in idmanager too, free last id first
		void FreeAnim(shok::AnimationId id);


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
			shok::TerrainTypeId Id = {};
			int Priority = 0;
			shok::TerrainTextureId BaseTextureId = {};
			shok::TerrainTextureId SnowTextureId = {};
			shok::TerrainTextureId TransitionTextureId = {};
			float OneDiv4TimesQuads = 0;
			shok::TerrainTypeId ReplacementTerrainType = {};
			bool TransitionsColorModulate = false;
			bool TransitionTextureIs_Transitions01 = false;
		};

		GGlue::TerrainPropsDisplay* DisplayProps;
		shok::Vector<TerrainType> TerrainTypes; // 1 modified if lower graphics settings
		shok::Vector<TerrainType> OriginalTerrainTypes; // 5 stays as it is
		shok::Vector<RWE::RwTexture*> Textures; // 9, lazily loaded
		shok::Vector<shok::TerrainTypeId> TextureIdsOrderedByPriority; // 13
		shok::TerrainTypeId BlackInternalUseOnlyId; // 17 terrainid
		shok::TerrainTextureId Transitions01Id; // 18 textureid
		shok::Vector<int> TextureReplacement; // 19 terrainindex->terrainindex
		int TextureQualityOption;
		int TextureQualityOptionChangedCounter; // ?

		void ReloadAllTextures();
		void ApplyTextureQuality();

		void PopTexture(shok::TerrainTextureId id);
		void LoadTexture(shok::TerrainTextureId id);
		void ReApplyTerrainType(shok::TerrainTypeId id);
		void ReApplyAllTerrainTypes();
	private:
		TerrainType CreateTerrainType(shok::TerrainTypeId id);
	};
	static_assert(sizeof(TerrainTextureManager::TerrainType) == 8 * 4);
	static_assert(sizeof(TerrainTextureManager) == 25 * 4);
	class TerrainDisplay { // size 9
	public:
		PADDINGI(2);
		TerrainTextureManager* TextureManager;
	};

	class CEntitiesTypeFlags {
	public:
		virtual ~CEntitiesTypeFlags() = default;

		enum class Flags : byte {
			RenderInFoW = 1,
			RenderNormalQuality = 2,
			HighQualityOnly = 4,
		};

		Flags* Data = nullptr;
		size_t NumberOfFlags = 0;

		CEntitiesTypeFlags(ED::EntityTypeDisplayProps* entityTypeDisplays);

		void* operator new(size_t s);
		void operator delete(void* p);

		static inline constexpr int vtp = 0x769E70;
	};
	static_assert(sizeof(CEntitiesTypeFlags) == 3 * 4);

	class IAuras {
	public:
		static inline constexpr int vtp = 0x7695EC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x17F36327); // iauras and cauras ?

		virtual unsigned int __stdcall GetClassIdentifier() const = 0;
		virtual ~IAuras() = default;
		virtual void Destroy() = 0;
		virtual void ClearAuras() = 0;
		virtual void AddAura(RWE::RpClump* clump) = 0;
		virtual void RenderAuras() = 0;
	};
	class CAuras : public IAuras {
	public:
		shok::Vector<RWE::RpClump*> Clumps;
		void* Shader; //BB::TResourceProxyResMgr<BBRw::CEffect *>
		RWE::RwTexture* Texture;

		static inline constexpr int vtp = 0x769608;
	};
	static_assert(sizeof(CAuras) == 4 * 7);

	struct CGfxSetTransitionStatus {
		shok::WeatherGFXSet To;
		shok::WeatherGFXSet From;
		int TransitionStart;
		int TransitionDuration;
		float Progress;
	};

	class ILight {
	public:
		virtual ~ILight() = default;
		virtual void Destroy() = 0;
		virtual void SetSunDir(float x, float y, float z) = 0;
		virtual void SetAmbientColor(int r, int g, int b) = 0;
		virtual void SetDiffuseColor(int r, int g, int b) = 0;

		PADDINGI(1); // counter, starts at 1?
		RWE::RwV3d SunDir; // 2
		RWE::RwV3d NormalizedSunDir; // 5
		RWE::RwRGBAReal AmbientColor; // 8
		RWE::RwRGBAReal DiffuseColor; // 12
		PADDINGI(1); // 16 float 1
		RWE::RpLight* AmbientLight;
		RWE::RpLight* DiffuseLight;
		PADDINGI(1); // 19 float 0

		static inline constexpr int vtp = 0x769964;
		// ctor 46D3A7()
	};
	class CLight : public ILight {
	public:
		struct LightData {
			RWE::RwV3d SunDir; // pos or direction?
			RWE::RwRGBAReal AmbientColor, DiffuseColor;

			// ctor 46D624()
		};
		struct GFXSetLight {
			float TransitionStart;
			float TransitionEnd;
			LightData Data;
		};

		struct GFXDataHandler {
			shok::Vector<GFXSetLight> Data; // indexed directy by id, 0 is likely placeholder

			void Interpolate(CGfxSetTransitionStatus* status, LightData* output);
		} GFX; // 20

		void Update();

		static inline constexpr int vtp = 0x76999C;
		// ctor 46E1F4()
		// handle transtion status 46DA60 stdcall(ts)
	};
	static_assert(sizeof(CLight::GFXSetLight) == 52);
	static_assert(offsetof(CLight, GFX) == 20 * 4);
	static_assert(offsetof(CLight, AmbientLight) == 17 * 4);
	static_assert(offsetof(CLight, DiffuseColor) == 12 * 4);

	class CRenderSettings {
	public:
		virtual ~CRenderSettings() = default;
		virtual void Destroy() = 0;
		virtual void SetWeatherStatus(shok::WeatherGFXSet to, shok::WeatherGFXSet from, int startTurn, int duration) = 0;
		virtual void Tick(float time, int uk) = 0;

		enum class RenderFlags : int {
			RenderDecals = 0x1,
			RenderDecalsAtomics = 0x2,
			RenderDecalsDoodads = 0x4,
			RenderDecalShadows = 0x8,

			RenderDecalsSelections = 0x10,
			RenderEffects = 0x20,
			RenderFogOfWar = 0x40,

			RenderLandscape = 0x80,
			RenderLandscapeFogofWar = 0x100,
			RenderLandscapeTerrain = 0x200,
			RenderLandscapeTerrainLayer1 = 0x400,
			RenderLandscapeTerrainLayer2 = 0x800,

			RenderLandscapeTerrainLayer3 = 0x1000,
			RenderLandscapeWater = 0x2000,

			RenderObjects = 0x10000,
			RenderObjectsAlphaBlendPass = 0x20000,
			RenderObjectsAlphaTestPass = 0x40000,
			RenderObjectsNonAlpha = 0x80000,

			RenderParticles = 0x400000,
			RenderSky = 0x800000,

			RenderFog = 0x1000000,
			RenderInvisibleObjects = 0x2000000,
			RenderLandscapeDebugInfo = 0x4000000,
			UseGFXSet = 0x8000000,

			RenderUseOrnamentalItemsSystem = 0x10000000,
			RenderUpdateMorphAnim = 0x20000000,
			RenderUpdateParticles = 0x40000000,
		};

		RenderFlags Flags = RenderFlags::RenderUseOrnamentalItemsSystem; // 1
		PADDINGI(1); // int 1
		shok::List<BB::ISlot0*> UnknownCallbacks; // 2
		float FarClipPlaneMin = 0, FarClipPlaneMax = 1000000.0f; // 6,7
		RWE::RwRGBA FogColor = { 0,0,0,0 };
		float FogStart = 5000.0f, FogEnd = 10000.0f; // 9,10
		float IceStatus = 0, IceStatusClamped = 0; // 11,12
		float IceStatusCoeffA = 1, IceStatusCoeffB = 0; // IceStatus=IceStatusCoeffA*SnowStatus+IceStatusCoeffB 13,14
		bool IceStatusFromSnowStatus = true; // 15
		float SnowStatus = 0, SnowStatusClamped = 0; // 16,17
		float SkyBoxOffset = 2000.0f; // 18
		shok::List<BB::TSlot2<unsigned int, unsigned int>*> UnknownCallbacks2;
		shok::List<BB::TSlot1<ED::CGfxSetTransitionStatus const&>*> TransitionCallbacks; // 22
		PADDINGI(1); // 25 f 0.5

		static inline constexpr int vtp = 0x769A10;
	};
	static_assert(offsetof(CRenderSettings, FarClipPlaneMin) == 6 * 4);
	static_assert(offsetof(CRenderSettings, SkyBoxOffset) == 18 * 4);
	template<>
	class ::enum_is_flags<CRenderSettings::RenderFlags> : public std::true_type {};
	class CRenderSettingsEx : public CRenderSettings {
	public:
		struct GFXSetFog {
			float TransitionStart;
			float TransitionEnd;
			bool FogActive; // ?
			RWE::RwRGBAReal FogColor;
			float FogStart, FogEnd;
		};
		struct GFXSetSnowStatus {
			float TransitionStart;
			float TransitionEnd;
			float Status; // alpha or just a fancy bool?
		};

		CGfxSetTransitionStatus WeatherStatus; // 26 progress is not set here
		shok::Vector<GFXSetSnowStatus> SnowData; // indexed by id directly, 0 likely placeholder
		shok::Vector<GFXSetFog> FogData; // indexed by id directly, 0 likely placeholder

		static inline constexpr int vtp = 0x769A24;

		// gets from ED::CGlobalsBaseEx
		static inline CRenderSettingsEx* (__stdcall* const GlobalObj)() = reinterpret_cast<CRenderSettingsEx * (__stdcall*)()>(0x46E4EC);
	};
	static_assert(sizeof(CRenderSettingsEx) == 39 * 4);
	static_assert(offsetof(CRenderSettingsEx, FogData) == 35 * 4);

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
		ED::EntityTypeDisplayProps* EntityTypeDisplays;
		ED::CModelsProps* ModelProps;
		PADDINGI(1); // p to something terrain related?
		PADDINGI(1); // p to something water related? // 10
		BBRw::CEngine* RWEngine; // p to BBRw::CEngine
		CAuras* Auras;
		ED::CCameraEx* Camera;
		ED::CCommandAcknowledgements* CommandAcks;
		ED::CEntitiesTypeFlags* EntityTypeFlags; // 15
		ED::CGUIScene* Scene;
		PADDINGI(1); // unknown
		ED::CLight* Light; // 18
		shok::List<BB::TSlot1<bool>*>* SomeCallback; // some reset callback?
		PADDINGI(1); // p to ED::COcclusionEffect
		PADDINGI(1); // p to ED::COrnamentalItems
		ED::CPlayerColors* PlayerColors; // 22
		PADDINGI(1);
		PADDINGI(1);
		ED::CRenderSettingsEx* RenderSettings; // 25
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
	static_assert(offsetof(CGlobalsBaseEx, RenderSettings) == 25 * 4);

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
		shok::Vector<shok::ModelId> DamageEffect;
		shok::Vector<shok::ModelId> DestroyEffect;
		PADDINGI(1);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static inline constexpr int vtp = 0x76A810;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFC5B7F67);
	};
	static_assert(sizeof(CBuildingEffectsProps) == 12 * 4);

	class CGlobalEffect {
	public:
		virtual ~CGlobalEffect() = default;

		PADDINGI(3); // this+2 == ED::CEffectDisplay ??
		shok::EffectTypeId EffectID; // 3

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
		// 14 more methods, // 7 anim get duration (jmp resmanager getduration)
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

template<>
inline auto CppLogic::GetIdManager<shok::TerrainTextureId>() {
	auto mng = (*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->DisplayProps->TerrainTextureManager;
	return CppLogic::EnumIdManager<shok::TerrainTextureId>{mng};
}

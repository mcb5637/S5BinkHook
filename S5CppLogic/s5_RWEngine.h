#pragma once
#include "enumflags.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_RWE_enums.h"

// a lot of info about renderware comes from here: https://github.com/DK22Pac/vice-37
// this is c code, i just c++ified it to clean up my global namespace
// need these for templates, so RTTI works
struct RtAnimAnimation;
struct RwTexture;
namespace RWE {
	// real forward decls
	struct RpClump;
	struct RpAtomic;
	struct RpWorld;
	struct RwFrame;
	using RwTexture = ::RwTexture;
	using RtAnimAnimation = ::RtAnimAnimation;

	// not done
	struct RwResEntry;
	struct RpGeometry;
	struct RpMaterial;
	struct RpSector;
	struct RxPipeline;
	struct RpWorldSector;
	struct RwTexDictionary;
	struct RwMatrix;

	struct RwLinkList {
		struct RwLLLink {
			RwLLLink* next;
			RwLLLink* prev;
		};
		RwLLLink link;
	};

	struct RwObject {
		RwObjectType type;
	private:
		byte subType, flags, privateFlags;
	public:
		void* parent;
	};
	static_assert(sizeof(RwObject) == 8);

	struct RwRGBA
	{
		byte red;
		byte green;
		byte blue;
		byte alpha;
	};
	struct RwRGBAReal
	{
		float red;
		float green;
		float blue;
		float alpha;
	};
	struct RwTexCoords
	{
		float u;
		float v;
	};

	struct RwV3d {
		float x = 0, y = 0, z = 0;

		static const inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA05D50);

		RwV3d Transform(const RwMatrix* matrix) const;
		RwV3d TransformPoint(const RwMatrix* matrix) const;
	};
	struct RwV2d {
		float x = 0, y = 0;
	};
	struct RwMatrix {
		RwV3d right;
		unsigned int flags;
		RwV3d up;
		unsigned int pad1;
		RwV3d at;
		unsigned int pad2;
		RwV3d pos;
		unsigned int pad3;

		// returns this
		RwMatrix* Optimize();
		// returns this, in degrees
		RwMatrix* Rotate(const RwV3d* axis, float angle, RwOpCombineType combineOp);
		// returns this
		RwMatrix* Scale(const RwV3d* scale, RwOpCombineType combineOp);
		// returns this
		RwMatrix* Translate(const RwV3d* translation, RwOpCombineType combineOp);
		// returns this (result)
		RwMatrix* Multiply(const RwMatrix* MatrixIn1, const RwMatrix* matrixIn2);
		// returns this (result)
		RwMatrix* Invert(const RwMatrix* matrixIn);

		static inline RwMatrix* (__cdecl* const Create)() = reinterpret_cast<RwMatrix * (__cdecl*)()>(0x41BA80);
	};
	struct RwSphere {
		RwV3d center;
		float radius;
	};
	struct RwBBox {
		RwV3d sup;
		RwV3d inf;
	};

	typedef RpClump* (*RpClumpCallBack)(RpClump* clump, void* data);
	typedef RpAtomic* (*RpAtomicCallBack)(RpAtomic* atomic, void* data);
	typedef RwFrame* (*RwFrameCallBack)(RwFrame* frame, void* data);
	typedef RwObject* (*RwObjectCallBack)(RwObject* object, void* data);

	struct RwFrame {
		RwObject object;

		RwLinkList::RwLLLink inDirtyListLink;

		RwMatrix modelling;
	private:
		RwMatrix ltm;
	public:

		RwLinkList objectList; // 36

		RwFrame* child;
		RwFrame* next;
		RwFrame* root; //40

		// returns this
		RwFrame* UpdateObjects();
		// returns this, in degrees
		RwFrame* Rotate(const RwV3d* axis, float angle, RwOpCombineType combineOp);
		// returns this, in degrees
		RwFrame* Rotate(float angle, RwOpCombineType combineOp);
		// returns this
		RwFrame* Scale(const RwV3d* scale, RwOpCombineType combineOp);
		// returns this
		RwFrame* Scale(float scale, RwOpCombineType combineOp);
		// returns this
		RwFrame* Translate(const RwV3d* translation, RwOpCombineType combineOp);
		// returns this
		RwFrame* ForAllChildren(RwFrameCallBack callBack, void* data);
		// returns this
		RwFrame* ForAllObjects(RwObjectCallBack callBack, void* data);
		RpAtomic* GetFirstAtomic();
		RWE::Anim::RpHAnimHierarchy* GetAnimFrameHandler();
		RwMatrix* GetLTM();
		static inline RwFrame* (__cdecl* const Create)() = reinterpret_cast<RwFrame * (__cdecl*)()>(0x413F10);
		void Destroy();
	};
	//constexpr int i = offsetof(RwFrame, objectList)/4;

	struct RwObjectHasFrame {
		RwObject object;
		RwLinkList::RwLLLink lFrame;
		RwObjectHasFrame* (*sync)(RwObjectHasFrame* object);
	};

	struct RwStreamMemory
	{
		int position; /* Current 'memory' position 0 is first byte */
		int nSize;  /* Space allocated currently */
		byte* memBlock; /* Current memory block pointer */
	};
	union RwStreamFile
	{
		void* fpFile;               /**< file pointer */
		const void* constfpFile;    /**< const file pointer */
	};
	struct RwStreamCustom
	{
		int(*sfnclose)(void* d);
		unsigned int (*sfnread)(void* d, void* buff, unsigned int l);
		int(*sfnwrite)(void* d, const void* buff, unsigned int l);
		int(*sfnskip)(void* d, unsigned int l);
		void* data;
	};
	union RwStreamUnion
	{
		RwStreamMemory memory; /**< memory */
		RwStreamFile file; /**< file */
		RwStreamCustom custom; /**< custom */
	};
	struct RwMemory
	{
		byte* start; /**< Starting address. */
		unsigned int length; /**< Length in bytes. */
	};
	struct RwStream {
		RwStreamType type;
		RwStreamAccessType accessType;
		int position;
		RwStreamUnion Type;
		int rwOwned;

		static inline RwStream* (__cdecl* const Open)(RwStreamType type, RwStreamAccessType ac, void* data) = reinterpret_cast<RwStream * (__cdecl*)(RwStreamType, RwStreamAccessType, void*)>(0x41A8E0);
		bool Close(void* data = nullptr);
		void Skip(size_t len);
		bool FindChunk(RwCorePluginID ty, size_t* length, unsigned int* version);
		size_t Read(void* buff, size_t toRead);
	};

	struct RwRaster {
		// check this, all of this is device dependend according to RW docu
		RwRaster* parent; /* Top level raster if a sub raster */
		byte* cpPixels; /* Pixel pointer when locked */
		byte* palette; /* Raster palette */
		int width, height, depth; /* Dimensions of raster */
		int stride; /* Lines bytes of raster */
		short nOffsetX, nOffsetY; /* Sub raster offset */
		byte cType;  /* Type of raster */
		byte cFlags; /* Raster flags */
		byte privateFlags; /* Raster private flags */
		byte cFormat; /* Raster format */

		byte* originalPixels;
		int originalWidth;
		int originalHeight;
		int originalStride;

		// destroys, no ref counting here
		// destroy parent? (docu)
		void Destroy();
		static inline RwRaster* (__cdecl* const Create)(int width, int height, int depth, int flags) = reinterpret_cast<RwRaster * (__cdecl*)(int, int, int, int)>(0x418BA0);
		bool ShowRaster(HWND window, bool vsync);
	};

	struct RpInterpolator {
		int flags;
		short startMorphTarget, endMorphTarget;
		float time, recipTime, position;
	};
	static_assert(sizeof(RpInterpolator) == 20);

	struct RpAtomic {
		RwObjectHasFrame object;
		RwResEntry* repEntry;
		RpGeometry* geometry;
		RwSphere boundingSphere;
		RwSphere worldBoundingSphere;
		RpClump* clump; // 15
		RwLinkList::RwLLLink inClumpLink;

		RpAtomic* (*renderCallBack)(RpAtomic* atomic);

		RpInterpolator interpolator;
		short renderFrame, pad;

		RwLinkList llWorldSectorsInAtomic;
		void* pipeline;

		enum class Flags : int {
			CollisionTest = 1,
			RenderShadow = 2,
			Render = 4,
		};

		bool IsParticleEmitter();
		void Destroy();
		void ForAllEmitters(RWE::Particles::RpPrtStdEmitterCallBack cb, void* data);
		void AddEmitter(RWE::Particles::RpPrtStdEmitter* em);

		static inline const RpAtomicCallBack SetPlayerColorCb = reinterpret_cast<RpAtomicCallBack>(0x48F361);
		static inline const RpAtomicCallBack DisableShadowCb = reinterpret_cast<RpAtomicCallBack>(0x721FD8);
		static inline const RpAtomicCallBack DisableParticleEffectsCb = reinterpret_cast<RpAtomicCallBack>(0x721F87); // destroys clumps that are IsParticleEmitter
		static inline const RpAtomicCallBack DisableTerrainDecalCb = reinterpret_cast<RpAtomicCallBack>(0x721FB2);
		static inline const RpAtomicCallBack SetColorModulateCb = reinterpret_cast<RpAtomicCallBack>(0x47B6E3);
	};
	//constexpr int i = offsetof(RpAtomic, clump) / 4;

	struct RpClump {

		RwObject object;

		RwLinkList atomicList, lightList, cameraList;
		RwLinkList::RwLLLink inWorldLink;
		RpClumpCallBack callback; // 0x6EF940?

		// returns this
		RpClump* ForAllAtomics(RpAtomicCallBack callback, void* pData);

		RpWorld* GetWorld() const;

		// destroys atomics, loghts, cameras and frames in the clump
		// destroys geometry if not used elsewhere
		// detaches from world, if exists
		void Destroy();
		void AddToDefaultWorld();
		static RpClump* Create();
		RpClump* Clone() const;
		RwFrame* GetFrame() const;
		void SetPlayerColor(int pl);
		void DisableShadow();
		void DisableParticleEffects();
		void DisableTerrainDecal();
		void SetColorModulate(shok::Color argb);
		void AddAtomic(RpAtomic* atomic);
		void RemoveAtomic(RpAtomic* atomic);

		static inline RpClump* (__cdecl* const Read)(RwStream* s) = reinterpret_cast<RpClump * (__cdecl*)(RwStream*)>(0x629990);
	};

	/**
	 * \ingroup rwcamera
	 * RwCameraProjection
	 * This type represents the options available for
	 * setting the camera projection model, either perspective projection or
	* parallel projection (see API function \ref RwCameraSetProjection)*/
	enum class RwCameraProjection : int
	{
		rwNACAMERAPROJECTION = 0,   /**<Invalid projection */
		rwPERSPECTIVE = 1,          /**<Perspective projection */
		rwPARALLEL = 2,             /**<Parallel projection */
	};
	/*
	 * This type represents a plane
	 */
	struct RwPlane
	{
		RwV3d normal;    /**< Normal to the plane */
		float distance; /**< Distance to plane from origin in normal direction*/
	};
	/*
	 * Structure describing a frustrum plane.
	 */
	struct RwFrustumPlane
	{
		RwPlane             plane;
		uint8_t             closestX;
		uint8_t             closestY;
		uint8_t             closestZ;
		uint8_t             pad;
	};
	struct RwCamera
	{
		RwObjectHasFrame    object;

		/* Parallel or perspective projection */
		RwCameraProjection  projectionType;

		/* Start/end update functions */
		RwCamera*(__cdecl* beginUpdate)(RwCamera*); // 6
		RwCamera* (__cdecl* endUpdate)(RwCamera*);

		/* The view matrix */
		RwMatrix            viewMatrix;

		/* The cameras image buffer */
		RwRaster* frameBuffer; // 24

		/* The Z buffer */
		RwRaster* zBuffer;

		/* Cameras mathmatical characteristics */
		RwV2d               viewWindow; // 16
		RwV2d               recipViewWindow;
		RwV2d               viewOffset; // 30
		float              nearPlane;
		float              farPlane; // 33
		float              fogPlane;

		/* Transformation to turn camera z or 1/z into a Z buffer z */
		float              zScale, zShift; // 35

		/* The clip-planes making up the viewing frustum */
		RwFrustumPlane      frustumPlanes[6];
		RwBBox              frustumBoundBox;

		/* Points on the tips of the view frustum */
		RwV3d               frustumCorners[8];

		static inline RwCamera* (__cdecl* const Create)() = reinterpret_cast<RwCamera * (__cdecl*)()>(0x41A0B0);
		static inline RwCamera* (__cdecl* const CreateExtended)(int width, int height, int bit) = reinterpret_cast<RwCamera * (__cdecl*)(int, int, int)>(0x467C76); // shok, creates with all rasters+frame
		void SetFrame(RwFrame* f);
		void Destroy();
		void ExtendedDestroy(); // shok, also destroys rasters+frame
		void SetNearClipPlane(float ne);
		void SetFarClipPlane(float fa);
		bool SetProjection(RwCameraProjection pro);
		void SetViewWindow(RwV2d* viewWind);
		RwFrame* GetFrame() const;
		bool Clear(RwRGBA color, RwCameraClearMode clearMode); // clears camera to color, needs to be outside of update
		bool BeginUpdate();
		bool EndUpdate();
		bool ShowRaster(HWND window, bool vsync);
	};
	//constexpr int i = offsetof(RwCamera, farPlane) / 4;

	struct RpMaterialList {
		RpMaterial** materials;
		int numMaterials;
		int space;
	};

	struct RpWorld {
		enum class RpWorldRenderOrder : int {
			rpWORLDRENDERNARENDERORDER = 0,
			rpWORLDRENDERFRONT2BACK,
			rpWORLDRENDERBACK2FRONT,
		};
		
		RwObject object;

		unsigned int flags;

		RpWorldRenderOrder  renderOrder;

		RpMaterialList      matList;

		/* The world stored as a BSP tree */
		RpSector* rootSector;

		/* The number of texture coordinate sets in each sector */
		int numTexCoordSets;

		int numClumpsInWorld;
		RwLinkList::RwLLLink* currentClumpLink;

		RwLinkList clumpList;

		RwLinkList lightList;

		RwLinkList directionalLightList;

		RwV3d worldOrigin;

		RwBBox boundingBox;

		/* The callbacks functions */
		RpWorldSector* (*renderCallBack) (RpWorldSector* worldSector);

		RxPipeline* pipeline;

		// returns this
		RpWorld* AddClump(RpClump* clump);
		// returns this
		RpWorld* RemoveClump(RpClump* clump);
		void AddCamera(RwCamera* cam);
		void RemoveCamera(RwCamera* cam);
		static inline RpWorld* (__cdecl* const Create)(RwBBox* boundingBox) = reinterpret_cast<RpWorld * (__cdecl*)(RwBBox*)>(0x628370);
	};

	/**
	 * \ingroup rwimage
	 * \struct RwImage
	 * Image containing device-independent pixels.
	 * This should be considered an opaque type.
	 * Use the RwImage API functions to access.
	 */
	struct RwImage
	{
		int flags;

		int width;  /* Device may have different ideas */
		int height; /* internally !! */

		int depth;  /* Of referenced image */
		int stride;

		uint8_t* cpPixels;
		RwRGBA* palette;

		static inline RwImage* (__cdecl* const Create)(int width, int height, int depth) = reinterpret_cast<RwImage * (__cdecl*)(int, int, int)>(0x414CC0);
		RwImage* AllocatePixels();
		RwImage* FreePixels();
		int Destroy();
	};


	/**
	 * \ingroup rwcoredriverd3d9
	 * \struct RwD3D9Vertex
	 * D3D9 vertex structure definition for 2D geometry
	 */
	struct RwIm2DVertex
	{
		float x; /**< Screen X */
		float y; /**< Screen Y */
		float z; /**< Screen Z */
		float rhw; /**< Reciprocal of homogeneous W */

		unsigned int emissiveColor; /**< Vertex color */

		float u; /**< Texture coordinate U */
		float v; /**< Texture coordinate V */
	};

	struct RwDevice {
		float gammaCorrection; // 4
		int(__cdecl* fpSystem)(int nOption, void* pOut, void* pInOut, int nIn);
		float zBufferNear, zBufferFar;
		int(__cdecl* fpRenderStateSet)(RwRenderState nState, void* pParam); // 8
		int(__cdecl* fpRenderStateGet)(RwRenderState nState, void* pParam);

		void* fpIm2DRenderLine; // 10
		void* fpIm2DRenderTriangle;
		int(__cdecl* fpIm2DRenderPrimitive)(RwPrimitiveType primType, RwIm2DVertex* vertices, int numVertices);// 12
		void* fpIm2DRenderIndexedPrimitive;

		void* fpIm3DRenderLine;
		void* fpIm3DRenderTriangle;
		void* fpIm3DRenderPrimitive;
		void* fpIm3DRenderIndexedPrimitive;

		int SetTextureFilterMode(RwTextureFilterMode m);
		int SetCullMode(RwCullMode m);
		RwCullMode GetCullMode();
		int SetTextureRaster(RWE::RwRaster* r);
	};
	static_assert(sizeof(RwDevice) == 56);

	struct RwStringFunctions {
		int (__cdecl*vecSprintf)(char* buffer, const char* format, ...);
		int (__cdecl*vecVsprintf)(char* buffer, const char* format, va_list argptr); // 50
		char* (__cdecl*vecStrcpy)(char* dest, const char* srce);
		char* (__cdecl*vecStrncpy)(char* dest, const char* srce, size_t size);
		char* (__cdecl*vecStrcat)(char* dest, const char* srce);
		char* (__cdecl*vecStrncat)(char* dest, const char* srce, size_t size);
		char* (__cdecl*vecStrrchr)(const char* string, int findThis); // 55
		char* (__cdecl*vecStrchr)(const char* string, int findThis);
		char* (__cdecl*vecStrstr)(const char* string, const char* findThis);
		int (__cdecl*vecStrcmp)(const char* string1, const char* string2); // 0 on equal
		int (__cdecl*vecStrncmp)(const char* string1, const char* string2, size_t max_size);
		int (__cdecl*vecStricmp)(const char* string1, const char* string2); // 60
		size_t(__cdecl*vecStrlen)(const char* string);
		char* (__cdecl*vecStrupr)(char* string);
		char* (__cdecl*vecStrlwr)(char* string);
		char* (__cdecl*vecStrtok)(char* string, const char* delimit);
		int (__cdecl*vecSscanf)(const char* buffer, const char* format, ...); // 65
	};
	static_assert(sizeof(RwStringFunctions) == 68);

	struct RwMemoryFunctions {
		void* (*rwmalloc)(size_t size, unsigned int hint); // 66
		void  (*rwfree)(void* mem);
		void* (*rwrealloc)(void* mem, size_t newSize, unsigned int hint);
		void* (*rwcalloc)(size_t numObj, size_t sizeObj, unsigned int hint); //69
	};
	struct RwFreeList {
		size_t entrySize;
		unsigned int entriesPerBlock;
		size_t heapSize;
		unsigned int alignment;
		RwLinkList blockList; // data blocks (full?)
		unsigned int flags;
		RwLinkList::RwLLLink link; // free
	};
	struct RwMetrics;

	struct RwGlobals {
		enum class RwEngineStatus : int {
			rwENGINESTATUSIDLE = 0,
			rwENGINESTATUSINITED = 1,
			rwENGINESTATUSOPENED = 2,
			rwENGINESTATUSSTARTED = 3,
		};


		void* curCamera;
		void* curWorld;

		unsigned short renderFrame, lightFrame;
		unsigned short pad[2];

		RwDevice dOpenDevice; // 4

		int(__cdecl* stdFunc[29])(void* pout, void* pinout, int inint); // 18

		RwLinkList dirtyFrameList;

		RwStringFunctions stringFuncs; // 49

		RwMemoryFunctions memoryFuncs; // 66

		void* (*memoryAlloc)(RwFreeList* fl, unsigned int hint); // 70
		RwFreeList* (*memoryFree)(RwFreeList* fl, void* pData);

		RwMetrics* metrics;
		RwEngineStatus engineStatus;

		unsigned int resArenaInitSize;

		static inline RwGlobals** const GlobalObj = reinterpret_cast<RwGlobals**>(0x8501C8);
	};
	static_assert(offsetof(RwGlobals, memoryFuncs) == 264);
	static_assert(offsetof(RwGlobals, dOpenDevice.fpRenderStateSet) == 4*8);
	//constexpr int i = offsetof(RwGlobals, stdFunc)/4;

	/**
	 * \ingroup rwengine
	 * \struct RwVideoMode
	 * This type represents a video mode available on a device specified
	 * by the frame buffer resolution (width and height) and depth,
	 * and a flag indicating  whether the device has exclusive use of
	 * the mode (see API function \ref RwEngineGetVideoModeInfo): */
	struct RwVideoMode
	{
		int         width = 0;   /**< Width  */
		int         height = 0;  /**< Height */
		int         depth = 0;   /**< Depth  */
		RwVideoModeFlag flags = static_cast<RwVideoModeFlag>(0);   /**< Flags  */
		int         refRate = 0; /**< Approximate refresh rate */
		int         format = 0;  /**< Raster format \see RwRasterFormat */

		static inline int(__cdecl*const GetNumVideoModes)() = reinterpret_cast<int(__cdecl*)()>(0x40FE70);
		bool GetInfo(int mode); // returns sucess
		static inline int(__cdecl* const SetVideoMode)(int mode) = reinterpret_cast<int(__cdecl*)(int mode)>(0x40FF70);
		static inline int(__cdecl* const GetVideoMode)() = reinterpret_cast<int(__cdecl*)()>(0x40FF20);
	};
}

struct RwTexture {
	RWE::RwRaster* raster;
	RWE::RwTexDictionary* dict;
	RWE::RwLinkList::RwLLLink lInDictionary;
	char name[32];
	char mask[32];
	unsigned int filterAddressing; // 20
	int refCount;

	static RwTexture* Read(const char* name, const char* mask);
	// destroys if no reference left
	void Destroy();
};

// misc renderware funcs:
// 0x41CD50 RwChar* StringStreamRead(RwChar* nativeString, RwStream* stream, RwUInt32 length) (strange calling conv?)
// 0x41CE10 RwChar* UnicodeStringStreamRead(RwChar* nativeString, RwStream* stream, RwUInt32 length) (strange calling conv?)
// 0x41CF00 RwChar* _rwStringStreamFindAndRead(RwChar* string, RwStream* stream)
// 0x417A50 RwBool RwTextureGetMipmapping(void)
// 0x417A80 RwBool RwTextureGetAutoMipmapping(void)
// 0x417A30 RwBool RwTextureSetMipmapping(RwBool enable)
// 0x417A60 RwBool RwTextureSetAutoMipmapping(RwBool enable)
// 0x626100 const RwPluginRegistry* _rwPluginRegistrySkipDataChunks(const RwPluginRegistry* reg, RwStream* stream)
// 0x41CCB0 RwUInt32 _rwStringStreamGetSize(const RwChar* string)
// 0x41A530 RwStream* RwStreamWrite(RwStream* stream, void const* buffer, RwUInt32 length)
// 0x429040 RwStream* _rwStreamWriteVersionedChunkHeader(RwStream* stream, RwInt32 type, RwInt32 size, RwUInt32 version, RwUInt32 buildNum)
// 0x41CCE0 const RwChar* _rwStringStreamWrite(const RwChar* string, RwStream* stream)

// directX
// sdk version 0x20u
// 0x410B60 D3D9DeviceSystemOpen (optimized calling convention?)
// 0x850224 WindowHandle
// 0x8018B8 _RwD3DAdapterType
// 0x85022C _RwD3DAdapterIndex
// 0xA1B280 _RwD3D9DeviceCaps
// 0x410AA0 D3D9CalculateMaxMultisamplingLevels
// 0x420190 _rwCPUHaveMMX
// 0x4201F0 _rwCPUHaveSSE
// 0x420250 _rwCPUHaveSSE2
// 0x4202B0 _rwCPUHave3DNow
// 0x411700 D3D9DeviceReleaseVideoMemory
// 0x411830 D3D9DeviceRestoreVideoMemory
// 0x424360 _rxD3D9VideoMemoryRasterListRestore
// 0x420F50 _rwD3D9DynamicVertexBufferRestore
// 0x424EE0 _rwD3D9Im3DRenderOpen
// 0x41E090 _rwD3D9Im2DRenderOpen
// 0x410A40 D3D9RestoreCacheLights
// 0x41FF70 _rwD3D9RenderStateReset

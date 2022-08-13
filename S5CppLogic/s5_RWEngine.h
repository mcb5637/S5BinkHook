#pragma once
#include "enumflags.h"
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

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

	struct RwLinkList {
		struct RwLLLink {
			RwLLLink* next;
			RwLLLink* prev;
		};
		RwLLLink link;
	};

	enum class RwObjectType : byte {
		FRAME = 0,
		ATOMIC = 1,
		CLUMP = 2,
		LIGHT = 3,
		CAMERA = 4,
		TEXDICTIONARY = 6,
		WORLD = 7,
		GEOMETRY = 8,
		WorldSector = 0xFF, // not a true type?
	};
	enum class RwCorePluginID : int {
		NAOBJECT = 0,
		STRUCT = 1,
		STRING = 2,
		EXTENSION = 3,
		CAMERA = 5,
		TEXTURE = 6,
		MATERIAL = 7,
		MATLIST = 8,
		ATOMICSECT = 9,
		PLANESECT = 0xA,
		WORLD = 0xB,
		SPLINE = 0xC,
		MATRIX = 0xD,
		FRAMELIST = 0xE,
		GEOMETRY = 0xF,
		CLUMP = 0x10,
		LIGHT = 0x12,
		UNICODESTRING = 0x13,
		ATOMIC = 0x14,
		TEXTURENATIVE = 0x15,
		TEXDICTIONARY = 0x16,
		ANIMDATABASE = 0x17,
		IMAGE = 0x18,
		SKINANIMATION = 0x19,
		GEOMETRYLIST = 0x1A,
		ANIMANIMATION = 0x1B,
		HANIMANIMATION = 0x1B,
		TEAM = 0x1C,
		CROWD = 0x1D,
		DMORPHANIMATION = 0x1E,
		RIGHTTORENDER = 0x1f,
		MTEFFECTNATIVE = 0x20,
		MTEFFECTDICT = 0x21,
		TEAMDICTIONARY = 0x22,
		PITEXDICTIONARY = 0x23,
		TOC = 0x24,
		PRTSTDGLOBALDATA = 0x25,
		ALTPIPE = 0x26,
		PIPEDS = 0x27,
		PATCHMESH = 0x28,
		CHUNKGROUPSTART = 0x29,
		CHUNKGROUPEND = 0x2A,
		UVANIMDICT = 0x2B,
		COLLTREE = 0x2C,
		ENVIRONMENT = 0x2D,
	};

	struct RwObject {
		RwObjectType type;
	private:
		byte subType, flags, privateFlags;
	public:
		void* parent;
	};
	static_assert(sizeof(RwObject) == 8);

	enum class RwOpCombineType : int {
		Replace = 0, // completely replaces the old matrix
		Preconcat, // transformation in object space, M = S*M
		Postconcat, // transformation in world space, M = M*S
	};

	struct RwV3d {
		float x, y, z;
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

	struct RwFrame {
		RwObject object;

		RwLinkList::RwLLLink inDirtyListLink;

		RwMatrix modelling;
	private:
		RwMatrix ltm;
	public:

		RwLinkList objectList;

		RwFrame* child;
		RwFrame* next;
		RwFrame* root;

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
		RtAnimationFrameHandler* GetAnimFrameHandler();
	};

	struct RwObjectHasFrame {
		RwObject object;
		RwLinkList::RwLLLink lFrame;
		RwObjectHasFrame* (*sync)(RwObjectHasFrame* object);
	};

	enum class RwStreamType : int
	{
		rwNASTREAM = 0,     /**<Invalid stream type */
		rwSTREAMFILE,       /**<File */
		rwSTREAMFILENAME,   /**<File name */
		rwSTREAMMEMORY,     /**<Memory*/
		rwSTREAMCUSTOM,     /**<Custom */
	};
	enum class RwStreamAccessType : int
	{
		rwNASTREAMACCESS = 0,   /**<Invalid stream access */
		rwSTREAMREAD,           /**<Read */
		rwSTREAMWRITE,          /**<Write */
		rwSTREAMAPPEND,         /**<Append */
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
		RpClump* clump;
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

		static inline const RpAtomicCallBack SetPlayerColorCb = reinterpret_cast<RpAtomicCallBack>(0x48F361);
		static inline const RpAtomicCallBack DisableShadowCb = reinterpret_cast<RpAtomicCallBack>(0x721FD8);
		static inline const RpAtomicCallBack DisableParticleEffectsCb = reinterpret_cast<RpAtomicCallBack>(0x721F87);
		static inline const RpAtomicCallBack DisableTerrainDecalCb = reinterpret_cast<RpAtomicCallBack>(0x721FB2);
		static inline const RpAtomicCallBack SetColorModulateCb = reinterpret_cast<RpAtomicCallBack>(0x47B6E3);
	};

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

		static inline RpClump* (__cdecl* const Read)(RwStream* s) = reinterpret_cast<RpClump * (__cdecl*)(RwStream*)>(0x629990);
	};

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
	};


	struct RwDevice {
		float gammaCorrection;
		void* fpSystem;
		float zBufferNear, zBufferFar;
	private:
		void* fpRenderStateSet;
		void* fpRenderStateGet;

		void* fpIm2DRenderLine;
		void* fpIm2DRenderTriangle;
		void* fpIm2DRenderPrimitive;
		void* fpIm2DRenderIndexedPrimitive;

		void* fpIm3DRenderLine;
		void* fpIm3DRenderTriangle;
		void* fpIm3DRenderPrimitive;
		void* fpIm3DRenderIndexedPrimitive;
	};
	static_assert(sizeof(RwDevice) == 56);

	struct RwStringFunctions {
		PADDINGI(17);
	};
	static_assert(sizeof(RwStringFunctions) == 68);

	struct RwMemoryFunctions {
		void* (*rwmalloc)(size_t size, unsigned int hint);
		void  (*rwfree)(void* mem);
		void* (*rwrealloc)(void* mem, size_t newSize, unsigned int hint);
		void* (*rwcalloc)(size_t numObj, size_t sizeObj, unsigned int hint);
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

		RwDevice dOpenDevice;

		void* stdFunc[29];

		RwLinkList dirtyFrameList;

		RwStringFunctions stringFuncs; // 49

		RwMemoryFunctions memoryFuncs; // 66

		void* (*memoryAlloc)(RwFreeList* fl, unsigned int hint); // 70
		RwFreeList* (*memoryFree)(RwFreeList* fl, void* pData);

		RwMetrics* metrics;
		RwEngineStatus engineStatus;

		unsigned int resArenaInitSize;

		static inline RwGlobals* const GlobalObj = reinterpret_cast<RwGlobals*>(0x8501C8);
	};
	static_assert(offsetof(RwGlobals, memoryFuncs) == 264);
	//constexpr int i = offsetof(RwGlobals, stringFuncs)/4;

	struct RtAnimationFrameHandler { // attached to a frame, plugin? not sure what this does size 9?
		enum class AnimFlags : unsigned int {
			None = 0,
			DoNotDestroyHierarchy = 0x1000,
			HierarchyNotDestroyed = 0x2000,
		};


		AnimFlags Flags;
		PADDINGI(1);
		PADDINGI(5); // pointer?
		PADDINGI(1);
		struct AnimData {
			RWE::RtAnimAnimation* Anim;
			float Time;
			void* Data;
		}* Animation; // 8

		RtAnimationFrameHandler* Clone();
		void SetAnimation(RWE::RtAnimAnimation* a);
		void Destroy();
		void AdvanceTime(float t);
		void SubstractTime(float t);
		void SetTimeOfAnim(float t);
		void ApplyTransforms();
		void SetupForModel(RWE::RpClump* c);
		void FuseAnimations(RtAnimationFrameHandler* anim1, RtAnimationFrameHandler* anim2, float ratio);
	};
	template<>
	class ::enum_is_flags<RtAnimationFrameHandler::AnimFlags> : public std::true_type {};
	//constexpr int i = offsetof(RtAnimationFrameHandler, Animation)/4;
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

struct RtAnimAnimation {
	PADDINGI(3);
	float Duration; // 3


	static RtAnimAnimation* Read(const char* name);
	void Destroy();
};

namespace BB {
	template<class T> requires std::same_as<T, RWE::RwTexture*> class TResourceManager;

	template<class T>
	requires std::same_as<T, RWE::RwTexture*> // not sure if others exist
	class TResourceProxyBase {
	public:
		virtual ~TResourceProxyBase() = default;
		virtual T __stdcall Load() = 0;
		virtual void __stdcall Free(T d) = 0;
		virtual void __stdcall Override(T d) {
			FreeCache();
			Cache = d;
		};

		T Cache;
		PADDINGI(1); // refCounter?
		TResourceManager<T>* Manager; // 3
		int Id;

		T Get() { // 0x46CFAD
			if (!Cache) {
				Cache = Load();
			}
			return Cache;
		}
		void __stdcall FreeCache() { // 0x4728BE
			if (Cache) {
				Free(Cache);
				Cache = nullptr;
			}
		}

		// vtp for RWE::RwTexture* 0x769F9C
	};
	template<class T>
	requires std::same_as<T, RWE::RwTexture*> // not sure if others exist
	class TResourceProxyResMgr : public TResourceProxyBase<T> {

		// vtp for RWE::RwTexture* 0x769FD0
	};
#ifdef __INTELLISENSE__
#pragma diag_suppress 2784
#endif
	static_assert(offsetof(TResourceProxyBase<RWE::RwTexture*>, Cache) == 4 * 1);
	static_assert(offsetof(TResourceProxyResMgr<RWE::RwTexture*>, Cache) == 4 * 1);
#ifdef __INTELLISENSE__
#pragma diag_default 2784
#endif

	template<class T>
	requires std::same_as<T, RWE::RwTexture*> // not sure if others exist
	class TResourceManager {
	public:
		virtual ~TResourceManager() = default;
		virtual T __stdcall Read(const char* name) = 0;
		virtual void __stdcall Destroy(T) = 0;


		BB::CIDManagerEx* IdManager;
		shok::Vector<TResourceProxyResMgr<T>*> Data;

		void PopId(int id) {
			if (id + 1 != static_cast<int>(Data.size()))
				throw std::out_of_range{ "invaild id" };
			auto v = Data.SaveVector();
			v.Vector[id]->FreeCache();
			delete v.Vector[id];
			v.Vector.pop_back();
			IdManager->RemoveID(id);
		}

		// vtp for RWE::RwTexture* 0x76BD90
	};
}

namespace BBRw {
	class CRwTextures : public BB::TResourceManager<RWE::RwTexture*> {
	public:
		static inline constexpr int vtp = 0x76BDA0;

		BB::TResourceProxyResMgr<RWE::RwTexture*>* Get(const char* name);
		BB::TResourceProxyResMgr<RWE::RwTexture*>* Get(int id);
	};

	class IEngine {
	public:
		virtual ~IEngine() = default;
	};

	class CEngine : public IEngine {
	public:
		static inline constexpr int vtp = 0x76B534;
		PADDINGI(3);
		CRwTextures* SelectionTextures; // also shorewave
	};
}

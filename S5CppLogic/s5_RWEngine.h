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
	struct RwMatrix;
	struct RwResEntry;
	struct RpGeometry;
	struct RpMaterial;
	struct RpMaterialList;
	namespace dict {
		struct RtDictSchema;
		struct RtDict;
	}

	// not done
	struct RpSector;
	struct RxPipeline;
	struct RpWorldSector;
	struct RwTexDictionary;
	struct RxVertexIndex; //?? typedef int?

	static constexpr int rwMAXTEXTURECOORDS = 8;
	static constexpr int rwLIBRARYCURRENTVERSION = 0x37002;
	static constexpr int rwLIBRARYBASEVERSION = 0x35000;

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
		/*
		 * \ref RwV3dTransformPoint uses the given matrix describing a
		 * transformation and applies it to the specified point.  The
		 * result is placed in another point (which may be the same point as
		 * the source).
		 *
		 * \param pointOut  A pointer to an RwV3d that is to receive the results of
		 *                  the point transformation
		 * \param pointIn   A pointer to an RwV3d that is to supply the point to be
		 *                   transformed
		 * \param matrix    A pointer to the matrix supplying the transformation
		 *
		 * \return Returns a pointer to the transformed point if successful or NULL
		 *        if there is an error
		 */
		RwV3d TransformPoint(const RwMatrix* matrix) const;
		RwV3d Normalize() const;
		/*
		 * \ref RwV3dTransformPoints uses the given matrix describing a
		 * transformation and applies it to the specified array of points. The
		 * results are then placed in another array (which may be the same array as
		 * the source).
		 *
		 * \note The source and destination array may be separate or coincident, but
		 * the result is not defined if the source and destination overlap in any
		 * other way (i.e. if they are offset from one another but still overlap).
		 *
		 * \param pointsOut  A pointer to an array of RwV3ds that are to
		 * receive the results of the point transformation
		 * \param pointsIn   A pointer to an array of RwV3ds that are to supply
		 * to points to be transformed
		 * \param numPoints  A number of points to transform
		 * \param matrix     A pointer to the matrix supplying the transformation
		 *
		 * \return Returns a pointer to the transformed point array if successful or NULL
		 *        if there is an error
		 */
		static inline RwV3d* (__cdecl* const RwV3dTransformPoints)(RwV3d* pointsOut, const RwV3d* pointsIn, int numPoints, const RwMatrix* matrix) = reinterpret_cast<RwV3d* (__cdecl*)(RwV3d*, const RwV3d*, int, const RwMatrix*)>(0x41C740);
		float Length() const;
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

		// 41BA50 RwMatrixDestroy
		// 41B9B0 RwMatrixTransform
		// 41B400 RwMatrixRotateOneMinusCosineSine
		// 41B360 RwMatrixUpdate
		// 41A940 MatrixMultiply
		// 41B3E0 RwMatrixOrthoNormalize
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
	typedef RpMaterial* (*RpMaterialCallBack)(RpMaterial* material, void* data);

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
		bool Dirty() const;

		// 413F50 rwFrameDestroyRecurse
		// 4142B0 RwFrameRegisterPlugin
		// 4142E0 rwFrameCloneRecurse
		// 729830 RwFrameRegisterPluginStream

		// 717D90 RwFrameGetUserDataArray
		// 717D60 RwFrameGetUserDataArrayCount
	};
	//constexpr int i = offsetof(RwFrame, objectList)/4;

	struct RwObjectHasFrame {
		RwObject object;
		RwLinkList::RwLLLink lFrame;
		RwObjectHasFrame* (*sync)(RwObjectHasFrame* object);

		// 629210 _rwObjectHasFrameSetFrame
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
		static inline RwRaster* (__cdecl* const Create)(int width, int height, int depth, RwRasterCreateFlags flags) = reinterpret_cast<RwRaster * (__cdecl*)(int, int, int, RwRasterCreateFlags)>(0x418BA0);
		bool ShowRaster(HWND window, bool vsync);
		byte* Lock(byte level, RwRasterLockMode lockMode);
		byte* LockPalette(RwRasterLockMode lockMode);
		void Unlock();
		void UnlockPalette();
		int GetMipLevels();
	};

	struct RpInterpolator {
		int flags;
		short startMorphTarget, endMorphTarget;
		float time, recipTime, position;
	};
	static_assert(sizeof(RpInterpolator) == 20);

	struct RpTriangle
	{
		uint16_t            vertIndex[3];   /**< vertex indices */
		uint16_t            matIndex;       /**< Index into material list */
	};

	/**
	 * \ingroup rpmesh
	 * \struct RpMesh
	 * This type represents a single polygon mesh.
	 * A mesh is defined as a collection of triangles derived from an RpGeometry
	 * or RpWorldSector which have a common material.
	 *
	 * See API functions \see RpGeometryForAllMeshes and
	 * \see RpWorldSectorForAllMeshes and
	 * the corresponding function callback types:
	 */
	struct RpMesh
	{
		RxVertexIndex* indices;    /**< vertex indices defining the mesh */
		uint32_t            numIndices; /**< number of vertices in mesh */
		RpMaterial* material;   /**< pointer to material used to
										 *   render the mesh. */
	};

	/**
	 * \ingroup rpmesh
	 * \struct RpMeshHeader
	 * Header for all meshes that constitute a single RpGeometry or RpWorldSector
	 */
	struct RpMeshHeader
	{
		RpMeshHeaderFlags            flags;    /**< \see RpMeshHeaderFlags */
		uint16_t            numMeshes; /**< Number of meshes in object */
		uint16_t            serialNum; /**< Determine if mesh has changed
										* since last instance */
		uint32_t            totalIndicesInMesh; /**< Total triangle index
												 * count in all meshes
												 */
		uint32_t            firstMeshOffset; /**< Offset in bytes from end this
											  * structure RpMeshHeader
											  * to the first mesh
											  */

		// 634BB0 _rpMeshRead
	};

	struct RpMorphTarget
	{
		RpGeometry* parentGeom;
		RwSphere   boundingSphere;
		RwV3d* verts; // 5
		RwV3d* normals;
	};

	struct RpMaterialList {
		RpMaterial** materials;
		int numMaterials;
		int space;

		// 637CD0 _rpMaterialListStreamRead
		// 637AD0 _rpMaterialListDeinitialize
		// 637B50 _rpMaterialListSetSize
		// 637BE0 _rpMaterialListAppendMaterial
	};

	struct RpGeometry
	{
		RwObject            object;     /* Generic type */

		RpGeometryFlag            flags;      /* Geometry flags */

		uint16_t            lockedSinceLastInst; /* What has been locked since we last instanced - for re-instancing */
		int16_t             refCount;   /* Reference count (for keeping track of atomics referencing geometry) */

		int32_t             numTriangles; /* 4 Quantity of various things (polys, verts and morph targets) */
		int32_t             numVertices;
		int32_t             numMorphTargets;
		int32_t             numTexCoordSets; // 7

		RpMaterialList      matList;

		RpTriangle* triangles;  /* The triangles */

		RwRGBA* preLitLum;  /* 12 The pre-lighting values */

		RwTexCoords* texCoords[rwMAXTEXTURECOORDS]; /* Texture coordinates */

		RpMeshHeader* mesh;   /* The mesh - groups polys of the same material */

		RwResEntry* repEntry;       /* Information for an instance */

		RpMorphTarget* morphTarget;    /* 23 The Morph Target */

		void AddRef();
		void Destroy();
		void ForAllMaterials(RpMaterialCallBack fpCallBack, void* pData);

		// 62EEA0 RpGeometryStreamRead
		// 62EB60 RpGeometryCreate
		// 62E6B0 RpGeometryAddMorphTargets

		// 6ED580 RpSkinGeometryGetSkin
		// 6ED590 RpSkinGeometrySetSkin

		// 5D8D30 RpMorphGeometryCreateInterpolators
	};

	typedef void        (*RwResEntryDestroyNotify) (RwResEntry* resEntry);
	struct RwResEntry
	{
		RwLinkList::RwLLLink            link;   /* Node in the list of resource elements */
		int32_t             size;   /* Size of this node */
		void* owner;  /* Owner of this node */
		RwResEntry** ownerRef; /* Pointer to pointer to this (enables de-alloc) */
		RwResEntryDestroyNotify destroyNotify; /* This is called right before destruction */
	};

	struct RwSurfaceProperties
	{
		float ambient;   /**< ambient reflection coefficient */
		float specular;  /**< specular reflection coefficient */
		float diffuse;   /**< reflection coefficient */
	};

	struct RpMaterial
	{
		RwTexture* texture; /**< texture */
		RwRGBA              color; /**< color */
		RxPipeline* pipeline; /**< pipeline */
		RwSurfaceProperties surfaceProps; /**< surfaceProps */
		int16_t             refCount;          /* C.f. rwsdk/world/bageomet.h:RpGeometry */
		int16_t             pad;

		// 62A000 RpMaterialDestroy
		// 62A060 RpMaterialStreamRead
		// 629EE0 RpMaterialCreate

		// 629FA0 RpMaterialRegisterPlugin
		// 629FD0 RpMaterialRegisterPluginStream

		// 7138F0 RpMatFXMaterialSetEffects
		// 713310 RpMatFXMaterialSetBumpMapCoefficient
		// 713140 RpMatFXMaterialSetBumpMapTexture
		// 7133E0 RpMatFXMaterialSetEnvMapTexture
		// 7134D0 RpMatFXMaterialSetEnvMapCoefficient
		// 713480 RpMatFXMaterialSetEnvMapFrameBufferAlpha
		// 713620 RpMatFXMaterialSetDualTexture
		// 713690 RpMatFXMaterialSetDualBlendModes
		// 713790 RpMatFXMaterialSetUVTransformMatrices

		// 5E29F0 RpMaterialUVAnimApplyUpdate
		// 5E2CB0 RpMaterialUVAnimSetCurrentTime
		// 5E2CF0 RpMaterialUVAnimAddAnimTime
		// 5E2D30 RpMaterialUVAnimExists

		// 5D8DB0 RpMorphAtomicAddTime
	};

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
		RpWorld* GetWorld() const;
		void SetGeometry(RpGeometry* geometry, bool assumeSameBoundingSphere);

		// RpAtomicGetWorldBoundingSphere 628B90

		// 629320 RpAtomicCreate
		// 6293F0 RpAtomicClone

		// 6ED550 RpSkinAtomicSetHAnimHierarchy
		// 6ED570 RpSkinAtomicGetHAnimHierarchy

		// 492184 BB get "3dsmax User Properties" userdata of frame (ret RpUserDataArray*)

		static inline const RpAtomicCallBack SetPlayerColorCb = reinterpret_cast<RpAtomicCallBack>(0x48F361);
		static inline const RpAtomicCallBack DisableShadowCb = reinterpret_cast<RpAtomicCallBack>(0x721FD8);
		static inline const RpAtomicCallBack DisableParticleEffectsCb = reinterpret_cast<RpAtomicCallBack>(0x721F87); // destroys clumps that are IsParticleEmitter
		static inline const RpAtomicCallBack DisableTerrainDecalCb = reinterpret_cast<RpAtomicCallBack>(0x721FB2);
		static inline const RpAtomicCallBack SetColorModulateCb = reinterpret_cast<RpAtomicCallBack>(0x47B6E3);
	};
	//constexpr int i = offsetof(RpAtomic, clump) / 4;

	struct rwFrameList
	{
		RwFrame** frames;
		int32_t numFrames;

		// 729890 _rwFrameListStreamRead
		// 729860 _rwFrameListDeinitialize
	};

	struct RpClump {

		RwObject object;

		RwLinkList atomicList, lightList, cameraList;
		RwLinkList::RwLLLink inWorldLink;
		RpClumpCallBack callback; // 0x6EF940?

		// returns this
		RpClump* ForAllAtomics(RpAtomicCallBack callback, void* pData);
		// RpClumpForAllLights 628EC0
		// RpClumpForAllCameras 628E70
		// RpClumpRender 628DE0
		// RpClumpAddLight 6290F0

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


		// 626D20 RwCameraGetWorld
	};
	//constexpr int i = offsetof(RwCamera, farPlane) / 4;

	struct RpLight {
		RwObjectHasFrame object; /**< object */
		float radius; /**< radius */
		RwRGBAReal color; /**< color */  /* Light color */ // 6
		float minusCosAngle; /**< minusCosAngle */
		RwLinkList WorldSectorsInLight; /**< WorldSectorsInLight */
		RwLinkList::RwLLLink inWorld; /**< inWorld */
		uint16_t lightFrame; /**< lightFrame */
		uint16_t pad;

		static inline RpLight* (__cdecl* const Create)(RpLightType type) = reinterpret_cast<RpLight * (__cdecl* const)(RpLightType)>(0x6277F0);
		void Destroy();
		void SetColor(const RwRGBAReal& color);
		float GetConeAngle() const;
	};
	static_assert(offsetof(RpLight, color) == 6 * 4);

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
		void AddLight(RpLight* light);
		void RemoveLight(RpLight* light);
		static inline RpWorld* (__cdecl* const Create)(RwBBox* boundingBox) = reinterpret_cast<RpWorld * (__cdecl*)(RwBBox*)>(0x628370);

		// 626D30 RpWorldAddAtomic
		// 626D60 RpWorldRemoveAtomic
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
		int(__cdecl* fpIm2DRenderIndexedPrimitive)(RwPrimitiveType primType, RwIm2DVertex* vertices, int numVertices, uint16_t* indices, int numIndices);

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

	enum class RwStandardFunc : int;
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

		// lookup func in RwStandardFunc
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

	struct RpUserDataArray
	{
		char* name;          /**< Identifier for this data array */
		RpUserDataFormat    format;         /**< Data format of this array */
		int32_t             numElements;    /**< Number of elements in this array */
		void* data;          /**< Pointer to the array data */

		// 717DC0 RpUserDataArrayGetName
		// 717DD0 RpUserDataArrayGetFormat
		// 717DE0 RpUserDataArrayGetNumElements
		// 717DF0 RpUserDataArrayGetInt

		// 4921F3 BB getStringPropertyStaringWith(const char*)
	};
	struct UserDataObject // this is whats stored in the frame/...
	{
		int NumUserDatas;
		RpUserDataArray* UDs;

		// 717CC0 UserDataObjectConstruct
		// 717CE0 UserDataObjectDestruct
		// 717FC0 UserDataObjectCopy
		// 717D00 UserDataObjectStreamRead
		// 717D20 UserDataObjectStreamWrite
		// 717D40 UserDataObjectGetSize
	};
	// 717FE0 RpUserDataPluginAttach


	struct RpMatFXMaterial { // material stores p to this
		enum class DataType : int {
			BumpMap = 1,
			EnvMap = 2,
			DualTexture = 4,
			UVTransformMat = 5,
		};
		struct Data {
			union {
				struct {
					RwFrame* Frame;
					RwTexture* Texture;
					RwTexture* SourceTexture;
					float Coefficient;
					PADDINGI(1);
				} BumpMap;
				struct {
					RwFrame* Frame;
					RwTexture* Texture;
					float Coefficient;
					int UseFrameBufferAlpha;
					PADDINGI(1);
				} EnvMap;
				struct {
					RwTexture* Texture;
					RwBlendFunction SrcBlendMode;
					RwBlendFunction DstBlendMode;
				} DualTexture;
				struct {
					RwMatrix* BaseTransform;
					RwMatrix* DualTransform;
				} UVTransform;
			} Source;
			DataType Type;
		};

		Data D[2];
		RpMatFXMaterialFlags Flags;
	};
	// 713E60 RpMatFXPluginAttach

	struct RwSList
	{
		uint8_t* listElements;
		int32_t     numElementsFilled;
		int32_t     numElementsAlloced;
		int32_t     entrySize;
	};

	namespace dict {
		/**
		 * \ingroup rtdict
		 *  The following callbacks are needed for each dictionary schema.
		 *  See \ref RtDictSchema.
		 *
		 *
		 * \ref RtDictEntryAddRefCallBack
		 * defines a callback function for registering that the caller has a
		 * reference to a particular entry.
		 *
		 * \param Entry the entry of which to increment the reference count
		 *
		 * \return The entry
		 */
		typedef void* (RtDictEntryAddRefCallBack)(void* entry);

		/**
		 * \ingroup rtdict
		 * \ref RtDictEntryDestroyCallBack
		 * defines a callback function for destroying an entry in the dictionary.
		 *
		 * \param entry The entry to be destroyed
		 *
		 * \return Unused, \ref RwBool to match most RenderWare object destruction
		 * functions
		 */
		typedef int(RtDictEntryDestroyCallBack)(void* entry);

		/**
		 * \ingroup rtdict
		 * \ref RtDictEntryGetNameCallBack
		 * defines a callback function to get the name of an entry in the
		 * dictionary.
		 *
		 * \param Entry The entry
		 *
		 * \return A pointer to the name
		 */
		typedef const char* (RtDictEntryGetNameCallBack)(const void* entry);

		/**
		 * \ingroup rtdict
		 * \ref RtDictEntryStreamGetSizeCallBack
		 * gets the streamable size of an entry in the dictionary.
		 *
		 * \param entry The entry
		 *
		 * \return The size of the entry when streamed
		 */
		typedef uint32_t(RtDictEntryStreamGetSizeCallBack)(const void* entry);

		/**
		 * \ingroup rtdict
		 * \ref RtDictEntryStreamReadCallBack
		 * reads a new entry from a stream. This function
		 * should return a pointer to a freshly allocated and initialized entry,
		 * with a nominal reference count of 1 (which will be assigned to the
		 * dictionary).
		 *
		 * \param stream The stream to read from
		 *
		 * \return A pointer to the new entry on success; NULL on failure.
		 */
		typedef void* (RtDictEntryStreamReadCallBack)(RwStream* stream);

		/**
		 * \ingroup rtdict
		 * \ref RtDictStreamReadCompatibilityCallBack
		 * enables older dictionaries to be retrofitted to use the new generic
		 * dictionary code. This callback is for internal use.
		 *
		 * \param stream The stream to read from
		 *
		 * \return A pointer to the new dictionary on success; NULL on failure.
		 */
		typedef RtDict* (RtDictStreamReadCompatibilityCallBack)(RwStream* stream);

		/**
		 * \ingroup rtdict
		 * \ref RtDictEntryStreamWriteCallBack
		 * writes an entry of the dictionary to a stream.
		 *
		 * \param entry The entry to write
		 * \param stream The stream to write to
		 *
		 * \return The entry on success; NULL on failure.
		 */
		typedef const void* (RtDictEntryStreamWriteCallBack)(const void* entry, RwStream* stream);


		/**
		 *  The following callbacks are used by utility functions that work with dictionaries
		 *  See \ref RtDictSchema and \ref RtDict.
		 */

		 /**
		  * \ingroup rtdict
		  * \ref RtDictCallBack
		  * is to be applied to each dictionary in a \ref RtDictSchema.
		  *
		  * \param dictionary The dictionary to apply the callback to
		  * \param data User supplied data
		  *
		  * \return The dictionary the callback was applied to.
		  * \see RtDictSchemaForAllDictionaries
		  */
		typedef RtDict* (RtDictCallBack)(RtDict* dictionary, void* data);

		/**
		 * \ingroup rtdict
		 * \ref RtDictEntryCallBack
		 * is to be applied to each entry in a dictionary.
		 *
		 * \param entry The entry to apply the callback to
		 * \param data User supplied data
		 *
		 * \return The entry the callback was applied to.
		 * \see RtDictForAllEntries
		 */
		typedef void* (RtDictEntryCallBack)(void* entry, void* data);

		/**
		 * \ingroup rtdict
		 * \struct RtDict
		 * A container of objects; objects which may be obtained by name. A dictionary
		 * is always created and managed via a schema object, \ref RtDictSchema.
		 *
		 * \see RtDictSchema
		 * \see RtDictSchemaCreateDict
		 * \see RtDictSchemaStreamReadDict
		 */
		struct RtDict
		{
			RtDictSchema* schema;
			/**< The schema that defines the layout of the dictionary. */
			RwSList* entries;
			/**< The entries held within the dictionary. */

			// 5E3C10 RtDictDestroy
			// 5E3B90 RtDictForAllEntries
		};

		/**
		 * \ingroup rtdict
		 * \struct RtDictSchema
		 * Defines the layout and management functions for a dictionary, or set of
		 * dictionaries. Maintains a 'current' dictionary for each layout. Provides a
		 * way of creating, or streaming in, dictionaries.
		 *
		 * \see RtDict
		 */
		struct RtDictSchema
		{
			const char* name;
			/**< Name of the schema.  Handy for debugging. */
			uint32_t dictChunkType;
			/**< Chunk type of the dictionary in a stream */
			uint32_t entryChunkType;
			/**< Chunk type of the dictionary entries in a stream */
			uint32_t compatibilityVersion;
			/**< Used for inner version checking when streaming in dictionaries. If
			  *< the dictionary's version is less than this, \ref streamReadCompatibilityCB
			  *< will be used for reading in the contents. */
			RwSList* dictionaries;
			/**< List of dictionaries that have been created using this layout */
			RtDict* current;
			/**< Current dictionary of this layout type */
			RtDictEntryAddRefCallBack* addRefCB;
			/**< Callback used to register that the dictionary has a reference to an entry */
			RtDictEntryDestroyCallBack* destroyCB;
			/**< Callback used to register that the dictionary no longer has a reference to an entry */
			RtDictEntryGetNameCallBack* getNameCB;
			/**< Callback used to obtain the name of an entry */
			RtDictEntryStreamGetSizeCallBack* streamGetSizeCB;
			/**< Callback used to obtain the size of an entry */
			RtDictEntryStreamReadCallBack* streamReadCB;
			/**< Callback used to stream in an entry */
			RtDictStreamReadCompatibilityCallBack* streamReadCompatibilityCB;
			/**< Callback used to stream in an old version of the dictionary. For internal use; may be set to NULL */
			RtDictEntryStreamWriteCallBack* streamWriteCB;
			/**< Callback used to stream out an entry */

			// 5E3CE0 RtDictSchemaStreamReadDict
			// 5E3C50 RtDictSchemaCreateDict
			// 5E3A70 RtDictSchemaRemoveDict
			// 5E3AD0 RtDictSchemaGetCurrentDict
			// 5E3A50 RtDictSchemaDestruct
			// 5E3A20 RtDictSchemaInit
		};
	}
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

	// 625880 RwTextureStreamRead
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

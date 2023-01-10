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

		PrtStd = 304,
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
		float x, y, z;
	};
	struct RwV2d {
		float x, y;
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
		RWE::Anim::RpHAnimHierarchy* GetAnimFrameHandler();
		RwMatrix* GetLTM();
	};
	//constexpr int i = offsetof(RwFrame, root)/4;

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


	/**
	 * \ingroup rwrenderstate
	 * RwRenderState represents the global state variables that control
	 * rendering. These may be set and queried using the
	 * \ref RwRenderStateSet and \ref RwRenderStateGet functions respectively.
	 *
	 * Refer to the \ref rwrenderstateoverview for an overview of this system.
	 *
	 * \note The texture render states (raster, address & filter modes) would
	 * normally just be used when rendering in immediate mode and should be
	 * specificied completely every time a texture is used. Retained mode
	 * pipelines will frequently set theses states internally, usually based on
	 * \ref RwTexture objects.
	 */
	enum class RwRenderState : int {
		rwRENDERSTATENARENDERSTATE = 0,

		rwRENDERSTATETEXTURERASTER,
		/**<Raster used for texturing (normally used in immediate mode).
		 *  The value is a pointer to an \ref RwRaster.
		 * Default: NULL.
		 */
		 rwRENDERSTATETEXTUREADDRESS,
		 /**<\ref RwTextureAddressMode: wrap, clamp, mirror or border.
		  * Default: rwTEXTUREADDRESSWRAP.
		  */
		  rwRENDERSTATETEXTUREADDRESSU,
		  /**<\ref RwTextureAddressMode in u only.
		   * Default: rwTEXTUREADDRESSWRAP.
		   */
		   rwRENDERSTATETEXTUREADDRESSV,
		   /**<\ref RwTextureAddressMode in v only.
			* Default: rwTEXTUREADDRESSWRAP.
			*/
			rwRENDERSTATETEXTUREPERSPECTIVE,
			/**<Perspective correction on/off (always enabled on many platforms).
			 */
			 rwRENDERSTATEZTESTENABLE,
			 /**<Z-buffer test on/off.
			  * Default: TRUE.
			  */
			  rwRENDERSTATESHADEMODE,
			  /**<\ref RwShadeMode: flat or gouraud shading.
			   * Default: rwSHADEMODEGOURAUD.
			   */
			   rwRENDERSTATEZWRITEENABLE,
			   /**<Z-buffer write on/off.
				* Default: TRUE.
				*/
				rwRENDERSTATETEXTUREFILTER,
				/**<\ref RwTextureFilterMode: point sample, bilinear, trilinear, etc.
				 * Default: rwFILTERLINEAR.
				 */
				 rwRENDERSTATESRCBLEND,
				 /**<\ref RwBlendFunction used to modulate the source pixel color
				  *  when blending to the frame buffer.
				  * Default: rwBLENDSRCALPHA.
				  */
				  rwRENDERSTATEDESTBLEND,
				  /**<\ref RwBlendFunction used to modulate the destination pixel
				   *  color in the frame buffer when blending. The resulting pixel
				   *  color is given by the formula
				   *  (SRCBLEND * srcColor + DESTBLEND * destColor) for each RGB
				   *  component. For a particular platform, not all combinations
				   *  of blend function are allowed (see platform specific
				   *  restrictions).
				   * Default: rwBLENDINVSRCALPHA.
				   */
				   rwRENDERSTATEVERTEXALPHAENABLE,
				   /**<Alpha blending on/off (always enabled on some platforms).
					*  This is normally used in immediate mode to enable alpha blending
					*  when vertex colors or texture rasters have transparency. Retained
					*  mode pipelines will usually set this state based on material colors
					*  and textures.
					* Default: FALSE.
					*/
					rwRENDERSTATEBORDERCOLOR,
					/**<Border color for \ref RwTextureAddressMode
					 *  \ref rwTEXTUREADDRESSBORDER. The value should be a packed
					 *  RwUInt32 in a platform specific format. The macro
					 *  RWRGBALONG(r, g, b, a) may be used to construct this using
					 *  8-bit color components.
					 * Default: RWRGBALONG(0, 0, 0, 0).
					 */
					 rwRENDERSTATEFOGENABLE,
					 /**<Fogging on/off (all polygons will be fogged).
					  * Default: FALSE.
					  */
					  rwRENDERSTATEFOGCOLOR,
					  /**<Color used for fogging. The value should be a packed RwUInt32
					   *  in a platform specific format. The macro RWRGBALONG(r, g, b, a)
					   *  may be used to construct this using 8-bit color components.
					   * Default: RWRGBALONG(0, 0, 0, 0).
					   */
					   rwRENDERSTATEFOGTYPE,
					   /**<\ref RwFogType, the type of fogging to use.
						* Default: rwFOGTYPELINEAR.
						*/
						rwRENDERSTATEFOGDENSITY,
						/**<Fog density for \ref RwFogType of
						 *  \ref rwFOGTYPEEXPONENTIAL or \ref rwFOGTYPEEXPONENTIAL2.
						 *  The value should be a pointer to an RwReal in the
						 *  range 0 to 1.
						 * Default: 1.
						 */
						 rwRENDERSTATECULLMODE = 20,
						 /**<\ref RwCullMode, for selecting front/back face culling, or
						  *  no culling.
						  * Default: rwCULLMODECULLBACK.
						  */
						  rwRENDERSTATESTENCILENABLE,
						  /**<Stenciling on/off.
						   *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
						   * Default: FALSE.
						   */
						   rwRENDERSTATESTENCILFAIL,
						   /**<\ref RwStencilOperation used when the stencil test passes.
							*  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
							* Default: rwSTENCILOPERATIONKEEP.
							*/
							rwRENDERSTATESTENCILZFAIL,
							/**<\ref RwStencilOperation used when the stencil test passes and
							 *  the depth test (z-test) fails.
							 *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
							 * Default: rwSTENCILOPERATIONKEEP.
							 */
							 rwRENDERSTATESTENCILPASS,
							 /**<\ref RwStencilOperation used when both the stencil and the depth
							  *  (z) tests pass.
							  *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
							  * Default: rwSTENCILOPERATIONKEEP.
							  */
							  rwRENDERSTATESTENCILFUNCTION,
							  /**<\ref RwStencilFunction for the stencil test.
							   *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
							   * Default: rwSTENCILFUNCTIONALWAYS.
							   */
							   rwRENDERSTATESTENCILFUNCTIONREF,
							   /**<Integer reference value for the stencil test.
								*  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
								* Default: 0.
								*/
								rwRENDERSTATESTENCILFUNCTIONMASK,
								/**<Mask applied to the reference value and each stencil buffer
								 *  entry to determine the significant bits for the stencil test.
								 *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
								 * Default: 0xffffffff.
								 */
								 rwRENDERSTATESTENCILFUNCTIONWRITEMASK,
								 /**<Write mask applied to values written into the stencil buffer.
								  *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
								  * Default: 0xffffffff.
								  */
								  rwRENDERSTATEALPHATESTFUNCTION,
								  /**<\ref RwAlphaTestFunction for the alpha test. When a pixel fails,
								   * neither the frame buffer nor the Z-buffer are updated.
								   * Default: rwALPHATESTFUNCTIONGREATER (GameCube, Xbox, D3D8, D3D9
								   * and OpenGL). The default PS2 behaviour is to always update the
								   * frame buffer and update the Z-buffer only if a greater than or
								   * equal test passes.
								   */
								   rwRENDERSTATEALPHATESTFUNCTIONREF,
								   /**<Integer reference value for the alpha test.
									*  <i> Range is 0 to 255, mapped to the platform's actual range </i>
									* Default: 128 (PS2) 0 (GameCube, Xbox, D3D8, D3D9 and OpenGL).
									*/

	};
	/**
	 * \ingroup rwrenderstate
	 * RwTextureFilterMode represents the texture filtering modes that may
	 * be set using the \ref RwRenderState \ref rwRENDERSTATETEXTUREFILTER in
	 * immediate mode, or \ref RwTextureSetFilterMode in retained mode.
	 */
	enum class RwTextureFilterMode : int
	{
		rwFILTERNAFILTERMODE = 0,
		rwFILTERNEAREST,                /**<Point sampled */
		rwFILTERLINEAR,                 /**<Bilinear */
		rwFILTERMIPNEAREST,             /**<Point sampled per pixel mip map */
		rwFILTERMIPLINEAR,              /**<Bilinear per pixel mipmap */
		rwFILTERLINEARMIPNEAREST,       /**<MipMap interp point sampled */
		rwFILTERLINEARMIPLINEAR,        /**<Trilinear */
	};

	struct RwDevice {
		float gammaCorrection; // 4
		int(__cdecl* fpSystem)(int nOption, void* pOut, void* pInOut, int nIn);
		float zBufferNear, zBufferFar;
		int(__cdecl* fpRenderStateSet)(RwRenderState nState, void* pParam); // 8
		int(__cdecl* fpRenderStateGet)(RwRenderState nState, void* pParam);

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
	//constexpr int i = offsetof(RwGlobals, dOpenDevice)/4;
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

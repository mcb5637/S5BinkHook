#pragma once

namespace RWE {
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
	enum class RwOpCombineType : int {
		Replace = 0, // completely replaces the old matrix
		Preconcat, // transformation in object space, M = S*M
		Postconcat, // transformation in world space, M = M*S
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
	/**
	 * \ingroup rwrenderstate
	 * RwCullMode represents the options available for culling polygons
	 * during rendering. The cull mode may be set via the \ref RwRenderState
	 * setting \ref rwRENDERSTATECULLMODE.
	 */
	enum class RwCullMode : int
	{
		rwCULLMODENACULLMODE = 0,
		rwCULLMODECULLNONE,
		/**<Both front and back-facing triangles are drawn. */
		rwCULLMODECULLBACK,
		/**<Only front-facing triangles are drawn */
		rwCULLMODECULLFRONT,
		/**<Only back-facing triangles are drawn */
	};
	/**
	 * \ingroup immediatemodedatatypes
	 * RwPrimitiveType
	 *  This type represents the different types of indexed
	 * line and indexed triangle primitives that are available when rendering 2D
	 * and 3D immediate mode objects (see API functions \ref RwIm2DRenderIndexedPrimitive,
	 * \ref RwIm2DRenderPrimitive, \ref RwIm3DRenderIndexedPrimitive and \ref RwIm3DRenderPrimitive).
	 * Indices are into a vertex list and must be defined in a counter-clockwise order
	 * (as seen from the camera) to be visible.*/
	enum class RwPrimitiveType : int
	{
		rwPRIMTYPENAPRIMTYPE = 0,   /**<Invalid primative type */
		rwPRIMTYPELINELIST = 1,     /**<Unconnected line segments, each line is specified by
									 * both its start and end index, independently of other lines
									 * (for example, 3 segments specified as 0-1, 2-3, 4-5) */
		rwPRIMTYPEPOLYLINE = 2,     /**<Connected line segments, each line's start index
									* (except the first) is specified by the index of the end of
									* the previous segment (for example, 3 segments specified as
									* 0-1, 1-2, 2-3) */
		rwPRIMTYPETRILIST = 3,      /**<Unconnected triangles: each triangle is specified by
									* three indices, independently of other triangles (for example,
									* 3 triangles specified as 0-1-2, 3-4-5, 6-7-8) */
		rwPRIMTYPETRISTRIP = 4,     /**<Connected triangles sharing an edge with, at most, one
									* other forming a series (for example, 3 triangles specified
									* as 0-2-1, 1-2-3-, 2-4-3) */
		rwPRIMTYPETRIFAN = 5,      /**<Connected triangles sharing an edge with, at most,
										* two others forming a fan (for example, 3 triangles specified
										* as 0-2-1, 0-3-2, 0-4-3) */
		rwPRIMTYPEPOINTLIST = 6,    /**<Points 1, 2, 3, etc. This is not
									* supported by the default RenderWare
									* immediate or retained-mode pipelines
									* (except on PlayStation 2), it is intended
									* for use by user-created pipelines */
	};
}

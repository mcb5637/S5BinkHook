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
	enum class RwPluginVendor : int
	{
		rwVENDORID_CORE = 0x000000L,
		rwVENDORID_CRITERIONTK = 0x000001L,
		rwVENDORID_REDLINERACER = 0x000002L,
		rwVENDORID_CSLRD = 0x000003L,
		rwVENDORID_CRITERIONINT = 0x000004L,
		rwVENDORID_CRITERIONWORLD = 0x000005L,
		rwVENDORID_BETA = 0x000006L,
		rwVENDORID_CRITERIONRM = 0x000007L,
		rwVENDORID_CRITERIONRWA = 0x000008L, /* RenderWare Audio */
		rwVENDORID_CRITERIONRWP = 0x000009L, /* RenderWare Physics */
	};
	constexpr int MAKECHUNKID(RwPluginVendor vendorID, int chunkID) {
		return (((static_cast<int>(vendorID) & 0xFFFFFF) << 8) | (chunkID & 0xFF));
	}
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

		// these are defines for some reason, making copying a lot harder
		// lots more...
		HANIMPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONTK, 0x1e),
		USERDATAPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONTK, 0x1f),
		MATERIALEFFECTSPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONTK, 0x20),
		PRTSTDPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONTK, 0x30),
		PRTADVPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONTK, 0x32),

		/**< RpMaterial pluginID */
		MATERIALMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x01),
		/**< RpMesh pluginID */
		MESHMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x02),
		/**< RpGeometry pluginID */
		GEOMETRYMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x03),
		/**< RpClump pluginID */
		CLUMPMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x04),
		/**< RpLight pluginID */
		LIGHTMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x05),
		/* Not used */
		COLLISIONMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x06),
		/**< RpWorld pluginID */
		WORLDMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x07),
		/* Not used */
		RANDOMMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x08),
		/**< PluginID for RpWorld's objects */
		WORLDOBJMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x09),
		/**< RpWorldSector pluginID */
		SECTORMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x0A),
		/**< Binary RpWorld pluginID */
		BINWORLDMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x0B),
		/**< RpWorld pipeline pluginID */
		WORLDPIPEMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x0D),
		/**< Binary RpMesh pluginID */
		BINMESHPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x0E),
		/**< RpWorld device pluginID */
		RXWORLDDEVICEMODULE = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x0F),
		/**< PluginID for platform specific serialization data */
		NATIVEDATAPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x10),
		/**< \if xbox Vertex format pluginID \endif */
		/**< \if gcn  Vertex format pluginID \endif */
		VERTEXFMTPLUGIN = MAKECHUNKID(RwPluginVendor::rwVENDORID_CRITERIONWORLD, 0x11),
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

	enum class RwStandardFunc : int {
		rwSTANDARDNASTANDARD = 0,
		rwSTANDARDCAMERABEGINUPDATE = 1,   /* Start 3d camera update */
		rwSTANDARDRGBTOPIXEL = 2,   /* For an RGB value return a pixel value */
		rwSTANDARDPIXELTORGB = 3,   /* For a pixel value returns a RGB value */
		rwSTANDARDRASTERCREATE = 4,   /* Create an raster */
		rwSTANDARDRASTERDESTROY = 5,   /* Raster destroy */
		rwSTANDARDIMAGEGETRASTER = 6,   /* Get image from a raster */
		rwSTANDARDRASTERSETIMAGE = 7,   /* Set raster from an image */
		rwSTANDARDTEXTURESETRASTER = 8,   /* Set texture's raster */
		rwSTANDARDIMAGEFINDRASTERFORMAT = 9,   /* Find a suitable raster format for an image */
		rwSTANDARDCAMERAENDUPDATE = 10,  /* End 3d camera update */
		rwSTANDARDSETRASTERCONTEXT = 11,  /* Start destination of 2d operations */
		rwSTANDARDRASTERSUBRASTER = 12,  /* Make a raster inside another raster */
		rwSTANDARDRASTERCLEARRECT = 13,  /* Clear a rectangle of the current dest raster */
		rwSTANDARDRASTERCLEAR = 14,  /* Clear the current dest raster */
		rwSTANDARDRASTERLOCK = 15, /* Lock a raster to get it's pixels */
		rwSTANDARDRASTERUNLOCK = 16,  /* Unlock a raster to return it's pixels */
		rwSTANDARDRASTERRENDER = 17,  /* Render a raster (not scaled, but masked) */
		rwSTANDARDRASTERRENDERSCALED = 18,  /* Render a raster (scaled and masked) */
		rwSTANDARDRASTERRENDERFAST = 19,  /* Render a raster (not scaled or masked) */
		rwSTANDARDRASTERSHOWRASTER = 20,  /* Show a camera raster */
		rwSTANDARDCAMERACLEAR = 21,  /* Clear a camera's raster and/or Z raster */
		rwSTANDARDHINTRENDERF2B = 22,  /* Set hint for rendering direction in the world */
		rwSTANDARDRASTERLOCKPALETTE = 23,  /* Lock a raster to get it's palette */
		rwSTANDARDRASTERUNLOCKPALETTE = 24,  /* Unlock a raster to return it's palette */
		rwSTANDARDNATIVETEXTUREGETSIZE = 25,  /* Get size of native texture when written to a stream */
		rwSTANDARDNATIVETEXTUREREAD = 26,  /* Read native texture from the stream */
		rwSTANDARDNATIVETEXTUREWRITE = 27,  /* Write native texture to the stream */
		rwSTANDARDRASTERGETMIPLEVELS = 28,  /* Get the number of mip levels in a raster */
		rwSTANDARDNUMOFSTANDARD = 29,
	};

	enum class RwCameraClearMode : int
	{
		rwCAMERACLEARIMAGE = 0x1,   /**<Clear the frame buffer */
		rwCAMERACLEARZ = 0x2,       /**<Clear the Z buffer */
		rwCAMERACLEARSTENCIL = 0x4, /**<\if xbox   Clear the stencil buffer \endif
									  * \if d3d8   Clear the stencil buffer \endif
									  * \if d3d9   Clear the stencil buffer \endif
									  * \if opengl Clear the stencil buffer \endif
									  */
	};
	template<>
	class ::enum_is_flags<RwCameraClearMode> : public std::true_type {};
	enum class RwVideoModeFlag : int
	{
		rwVIDEOMODEEXCLUSIVE = 0x0001, /**<Exclusive (i.e. full-screen) */
		rwVIDEOMODEINTERLACE = 0x0002, /**<Interlaced                   */
		rwVIDEOMODEFFINTERLACE = 0x0004, /**<Flicker Free Interlaced      */

		/* Platform specific video mode flags. */

		rwVIDEOMODE_PS2_FSAASHRINKBLIT = 0x0100,
		/**< \if sky2
		 *   Full-screen antialiasing mode 0
		 *   \endif
		 */
		rwVIDEOMODE_PS2_FSAAREADCIRCUIT = 0x0200,
		/**< \if sky2
		 *   Full-screen antialiasing mode 1
		 *   \endif
		 */

		rwVIDEOMODE_XBOX_WIDESCREEN = 0x0100,
		/**< \if xbox
		 *   Wide screen.
		 *   \endif
		 */
		rwVIDEOMODE_XBOX_PROGRESSIVE = 0x0200,
		/**< \if xbox
		 *   Progressive.
		 *   \endif
		 */
		rwVIDEOMODE_XBOX_FIELD = 0x0400,
		/**< \if xbox
		 *   Field rendering.
		 *   \endif
		 */
		rwVIDEOMODE_XBOX_10X11PIXELASPECT = 0x0800,
		/**< \if xbox
		 *   The frame buffer is centered on the display.
		 *   On a TV that is 704 pixels across, this would leave 32 pixels of black
		 *   border on the left and 32 pixels of black border on the right.
		 *   \endif
		 */
	};
	template<>
	class ::enum_is_flags<RwVideoModeFlag> : public std::true_type {};

	enum class RpLightType : int
	{
		rpNALIGHTTYPE = 0,

		/* These don't take part in the tie mechanism (no position) */
		rpLIGHTDIRECTIONAL, /**<Directional Light */
		rpLIGHTAMBIENT, /**<Ambient Light */

		/* These do take part in the tie mechanism (do have position) */
		rpLIGHTPOINT = 0x80, /**<Point Light */
		rpLIGHTSPOT, /**<Spot Light */
		rpLIGHTSPOTSOFT, /**<Soft Spot Light */
	};
	/**
	 * \ingroup rwim3d
	 * RwIm3DTransformFlags
	 *  The bit-field type  RwIm3DTransformFlags
	 * specifies options available for controlling execution of the 3D immediate
	 * mode pipeline (see API function \ref RwIm3DTransform):*/
	enum class RwIm3DTransformFlags : int
	{
		rwIM3D_VERTEXUV = 1,   /**<Texture coordinates in source vertices should be used */
		rwIM3D_ALLOPAQUE = 2,   /**<All source vertices are opaque (alpha is 255) */
		rwIM3D_NOCLIP = 4,   /**<No clipping should be performed on the geometry (the
									 * app may know it is all onscreen or within the guard band clipping
									 * region for the current hardware, so clipping can be skipped) */
		rwIM3D_VERTEXXYZ = 8,   /**<Vertex positions */
		rwIM3D_VERTEXRGBA = 16,  /**<Vertex color */
	};
	template<>
	class ::enum_is_flags<RwIm3DTransformFlags> : public std::true_type {};

	/**
	 * \ingroup rwraster
	 *  RwRasterType
	 *  This type represents the options available for creating a new
	 * raster (se API function \ref RwRasterCreate)*/
	enum class RwRasterType : int
	{
		rwRASTERTYPENORMAL = 0x00,          /**<Normal */
		rwRASTERTYPEZBUFFER = 0x01,         /**<Z Buffer */
		rwRASTERTYPECAMERA = 0x02,          /**<Camera */
		rwRASTERTYPETEXTURE = 0x04,         /**<Texture */
		rwRASTERTYPECAMERATEXTURE = 0x05,   /**<Camera texture */
		rwRASTERTYPEMASK = 0x07,            /**<Mask for finding type */

		rwRASTERPALETTEVOLATILE = 0x40,        /**<If set, hints that the palette will change often */
		rwRASTERDONTALLOCATE = 0x80,        /**<If set the raster is not allocated */
	};
	/**
	 * \ingroup rwraster
	 * \ref RwRasterFormat is a set of values and flags which may be combined to
	 * specify a raster format. The format chosen for a particular raster depends
	 * on the hardware device and the raster type specified at creation time
	 * (see API function \ref RwRasterCreate). The format may be retrieved using
	 * API function \ref RwRasterGetFormat.
	 *
	 * The raster format is a packed set of bits which contains the following
	 * four pieces of information (these may be combined with bitwise OR):
	 *
	 * <ol>
	 * <li> The pixel color format corresponding to one of the following values:
	 *      <ul>
	 *      <li> rwRASTERFORMAT1555
	 *      <li> rwRASTERFORMAT565
	 *      <li> rwRASTERFORMAT4444
	 *      <li> rwRASTERFORMATLUM8
	 *      <li> rwRASTERFORMAT8888
	 *      <li> rwRASTERFORMAT888
	 *      <li> rwRASTERFORMAT16
	 *      <li> rwRASTERFORMAT24
	 *      <li> rwRASTERFORMAT32
	 *      <li> rwRASTERFORMAT555
	 *      </ul>
	 *      This value may be masked out of the raster format using
	 *      rwRASTERFORMATPIXELFORMATMASK.
	 * <li> The palette depth if the raster is palettized:
	 *      <ul>
	 *      <li> rwRASTERFORMATPAL4
	 *      <li> rwRASTERFORMATPAL8
	 *      </ul>
	 *      In these cases, the color format refers to that of the palette.
	 * <li> Flag rwRASTERFORMATMIPMAP. Set if the raster contains mipmap levels.
	 * <li> Flag rwRASTERFORMATAUTOMIPMAP. Set if the mipmap levels were generated
	 *      automatically by RenderWare.
	 * </ol>
	 */
	enum class RwRasterFormat : int {
		rwRASTERFORMATDEFAULT = 0x0000, /* Whatever the hardware likes best */

		rwRASTERFORMAT1555 = 0x0100,    /**<16 bits - 1 bit alpha, 5 bits red, green and blue */
		rwRASTERFORMAT565 = 0x0200,     /**<16 bits - 5 bits red and blue, 6 bits green */
		rwRASTERFORMAT4444 = 0x0300,    /**<16 bits - 4 bits per component */
		rwRASTERFORMATLUM8 = 0x0400,    /**<Gray scale */
		rwRASTERFORMAT8888 = 0x0500,    /**<32 bits - 8 bits per component */
		rwRASTERFORMAT888 = 0x0600,     /**<24 bits - 8 bits per component */
		rwRASTERFORMAT16 = 0x0700,      /**<16 bits - undefined: useful for things like Z buffers */
		rwRASTERFORMAT24 = 0x0800,      /**<24 bits - undefined: useful for things like Z buffers */
		rwRASTERFORMAT32 = 0x0900,      /**<32 bits - undefined: useful for things like Z buffers */
		rwRASTERFORMAT555 = 0x0a00,     /**<16 bits - 5 bits red, green and blue */

		rwRASTERFORMATAUTOMIPMAP = 0x1000, /**<RenderWare generated the mip levels */

		rwRASTERFORMATPAL8 = 0x2000,    /**<8 bit palettised */
		rwRASTERFORMATPAL4 = 0x4000,    /**<4 bit palettised */

		rwRASTERFORMATMIPMAP = 0x8000,  /**<Mip mapping on */

		rwRASTERFORMATPIXELFORMATMASK = 0x0f00, /**<The pixel color format
												 *  (excluding palettised bits) */
		rwRASTERFORMATMASK = 0xff00     /**<The whole format */,
	};
	enum class RwRasterCreateFlags : int {
		rwRASTERTYPENORMAL = 0x00,          /**<Normal */
		rwRASTERTYPEZBUFFER = 0x01,         /**<Z Buffer */
		rwRASTERTYPECAMERA = 0x02,          /**<Camera */
		rwRASTERTYPETEXTURE = 0x04,         /**<Texture */
		rwRASTERTYPECAMERATEXTURE = 0x05,   /**<Camera texture */
		rwRASTERTYPEMASK = 0x07,            /**<Mask for finding type */

		rwRASTERPALETTEVOLATILE = 0x40,        /**<If set, hints that the palette will change often */
		rwRASTERDONTALLOCATE = 0x80,        /**<If set the raster is not allocated */

		rwRASTERFORMATDEFAULT = 0x0000, /* Whatever the hardware likes best */

		rwRASTERFORMAT1555 = 0x0100,    /**<16 bits - 1 bit alpha, 5 bits red, green and blue */
		rwRASTERFORMAT565 = 0x0200,     /**<16 bits - 5 bits red and blue, 6 bits green */
		rwRASTERFORMAT4444 = 0x0300,    /**<16 bits - 4 bits per component */
		rwRASTERFORMATLUM8 = 0x0400,    /**<Gray scale */
		rwRASTERFORMAT8888 = 0x0500,    /**<32 bits - 8 bits per component */
		rwRASTERFORMAT888 = 0x0600,     /**<24 bits - 8 bits per component */
		rwRASTERFORMAT16 = 0x0700,      /**<16 bits - undefined: useful for things like Z buffers */
		rwRASTERFORMAT24 = 0x0800,      /**<24 bits - undefined: useful for things like Z buffers */
		rwRASTERFORMAT32 = 0x0900,      /**<32 bits - undefined: useful for things like Z buffers */
		rwRASTERFORMAT555 = 0x0a00,     /**<16 bits - 5 bits red, green and blue */

		rwRASTERFORMATAUTOMIPMAP = 0x1000, /**<RenderWare generated the mip levels */

		rwRASTERFORMATPAL8 = 0x2000,    /**<8 bit palettised */
		rwRASTERFORMATPAL4 = 0x4000,    /**<4 bit palettised */

		rwRASTERFORMATMIPMAP = 0x8000,  /**<Mip mapping on */

		rwRASTERFORMATPIXELFORMATMASK = 0x0f00, /**<The pixel color format
												 *  (excluding palettised bits) */
		rwRASTERFORMATMASK = 0xff00     /**<The whole format */,
	};
	template<>
	class ::enum_is_flags<RwRasterCreateFlags> : public std::true_type {};
	/**
	 * \ingroup rwraster
	 * \ref RwRasterLockMode represents the options available for locking
	 * a raster so that it may be modified (see API function \ref RwRasterLock). An
	 * application may wish to write to the raster, read from the raster or
	 * simultaneously write and read a raster (rwRASTERLOCKWRITE | rwRASTERLOCKREAD).
	 */
	enum class RwRasterLockMode : int
	{
		rwRASTERLOCKWRITE = 0x01,   /**<Lock for writing */
		rwRASTERLOCKREAD = 0x02,    /**<Lock for reading */
		rwRASTERLOCKNOFETCH = 0x04, /**<When used in combination with
									 *  rwRASTERLOCKWRITE, asks the driver not to
									 *  fetch the pixel data. This is only useful
									 *  if it is known that ALL the raster data is
									 *  going to be overwritten before the raster
									 *  is unlocked, i.e. from an
									 *  \ref RwRasterSetFromImage call. This flag
									 *  is not supported by all drivers. */
		rwRASTERLOCKRAW = 0x08,    /**<When used in combination with
									   rwRASTERLOCKWRITE or rwRASTERLOCKREAD
									   allows access to the raw platform specific
									   pixel format */
	};
	template<>
	class ::enum_is_flags<RwRasterLockMode> : public std::true_type {};

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
	/**
	 * \ingroup rpuserdata
	 *  User data formats
	 */
	enum class RpUserDataFormat : int
	{
		rpNAUSERDATAFORMAT = 0,
		rpINTUSERDATA,          /**< 32 bit int data */
		rpREALUSERDATA,         /**< 32 bit float data */
		rpSTRINGUSERDATA,       /**< unsigned byte pointer data */
	};

	enum class RpGeometryFlag : int
	{
		rpGEOMETRYTRISTRIP = 0x00000001, /**<This geometry's meshes can be
											  rendered as strips.
											  \ref RpMeshSetTriStripMethod is
											  used to change this method.*/
		rpGEOMETRYPOSITIONS = 0x00000002, /**<This geometry has positions */
		rpGEOMETRYTEXTURED = 0x00000004, /**<This geometry has only one set of
											  texture coordinates. Texture
											  coordinates are specified on a per
											  vertex basis */
		rpGEOMETRYPRELIT = 0x00000008, /**<This geometry has pre-light colors */
		rpGEOMETRYNORMALS = 0x00000010, /**<This geometry has vertex normals */
		rpGEOMETRYLIGHT = 0x00000020, /**<This geometry will be lit */
		rpGEOMETRYMODULATEMATERIALCOLOR = 0x00000040, /**<Modulate material color
														  with vertex colors
														  (pre-lit + lit) */

		rpGEOMETRYTEXTURED2 = 0x00000080, /**<This geometry has at least 2 sets of
											  texture coordinates. */

		NumTexCoordSetsStoredInFlags = 0xFF0000,

											  /*
											   * These above flags were stored in the flags field in an RwObject, they
											   * are now stored in the flags file of the RpGeometry.
											   */

		rpGEOMETRYNATIVE = 0x01000000,
		rpGEOMETRYNATIVEINSTANCE = 0x02000000,

		rpGEOMETRYFLAGSMASK = 0x000000FF,
		rpGEOMETRYNATIVEFLAGSMASK = 0x0F000000,
	};
	template<>
	class ::enum_is_flags<RpGeometryFlag> : public std::true_type {};

	/**
	 * \ingroup rpmatfx
	 * RpMatFXMaterialFlags, this type represents the different types of
	 * material effects that can be used on a material. The effects are
	 * initialized with \ref RpMatFXMaterialSetEffects:
	 */
	enum class RpMatFXMaterialFlags : int
	{
		rpMATFXEFFECTNULL = 0, /**<No material effect.           */
		rpMATFXEFFECTBUMPMAP = 1, /**<Bump mapping                  */
		rpMATFXEFFECTENVMAP = 2, /**<Environment mapping           */
		rpMATFXEFFECTBUMPENVMAP = 3, /**<Bump and environment mapping  */
		rpMATFXEFFECTDUAL = 4, /**<Dual pass                     */
		rpMATFXEFFECTUVTRANSFORM = 5, /**<Base UV transform             */
		rpMATFXEFFECTDUALUVTRANSFORM = 6, /**<Dual UV transform (2 pass)    */

		rpMATFXEFFECTMAX,
		rpMATFXNUMEFFECTS = rpMATFXEFFECTMAX - 1,
	};

	/**
	 * \ingroup rwrenderstate
	 * RwBlendFunction represents the options available when changing the
	 * \ref RwRenderState setting for \ref rwRENDERSTATESRCBLEND and
	 * \ref rwRENDERSTATEDESTBLEND. The values are factors used to modulate
	 * either the source or destination pixel color when blending to
	 * the frame buffer. Note the some  combinations are not allowed on
	 * certain platforms (see platform specific restrictions).
	 *
	 * In the following list, the factors applied to each color component are
	 * listed explicitly. A subscript s refers to a source value while a subscript
	 * d refers to a destination value. Note that blending in the alpha channel is
	 * only applicable when the frame buffer actually contains alpha, and that
	 * the precise operation is platform specific. For instance, the
	 * source alpha value might simply be written to the alpha channel with
	 * no blending.
	 */
	enum RwBlendFunction
	{
		rwBLENDNABLEND = 0,
		rwBLENDZERO,            /**<(0,    0,    0,    0   ) */
		rwBLENDONE,             /**<(1,    1,    1,    1   ) */
		rwBLENDSRCCOLOR,        /**<(Rs,   Gs,   Bs,   As  ) */
		rwBLENDINVSRCCOLOR,     /**<(1-Rs, 1-Gs, 1-Bs, 1-As) */
		rwBLENDSRCALPHA,        /**<(As,   As,   As,   As  ) */
		rwBLENDINVSRCALPHA,     /**<(1-As, 1-As, 1-As, 1-As) */
		rwBLENDDESTALPHA,       /**<(Ad,   Ad,   Ad,   Ad  ) */
		rwBLENDINVDESTALPHA,    /**<(1-Ad, 1-Ad, 1-Ad, 1-Ad) */
		rwBLENDDESTCOLOR,       /**<(Rd,   Gd,   Bd,   Ad  ) */
		rwBLENDINVDESTCOLOR,    /**<(1-Rd, 1-Gd, 1-Bd, 1-Ad) */
		rwBLENDSRCALPHASAT,     /**<(f,    f,    f,    1   )  f = min (As, 1-Ad) */
	};

	enum class RpMeshHeaderFlags : int
	{
		/* NOTE: trilists are denoted by absence of any other
		 *       primtype flags, so be careful that you test:
		 *        (triListFlag == (flags&triListFlag))
		 *       or:
		 *        (0 == (flags&rpMESHHEADERPRIMMASK))
		 *       and not:
		 *        (flags&triListFlag)
		 */
		rpMESHHEADERTRISTRIP = 0x0001, /**< Render as tristrips */
		rpMESHHEADERTRIFAN = 0x0002, /**< On PlayStation 2 these will be converted to trilists */
		rpMESHHEADERLINELIST = 0x0004, /**< Render as linelists */
		rpMESHHEADERPOLYLINE = 0x0008, /**< On PlayStation 2 these will be converted to linelists */
		rpMESHHEADERPOINTLIST = 0x0010, /**< Pointlists are supported only if rendered by
										 *   custom pipelines; there is no default RenderWare
										 *   way to render pointlists. */

		rpMESHHEADERPRIMMASK = 0x00FF, /**< All bits reserved for specifying primitive type */
		rpMESHHEADERUNINDEXED = 0x0100, /**< Topology is defined implicitly by vertex
										 *   order, ergo the mesh contains no indices */
	};
}

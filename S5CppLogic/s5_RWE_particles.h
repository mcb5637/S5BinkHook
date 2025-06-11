#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_RWEngine.h"

namespace RWE::Particles {
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdPropertyTable
	 * A structure representing properties's data present in an emitter or
	 * particle.
	 * An emitter, or particle, are treated as a single block of memory. The
	 * property table is used to describe the data presents in the memory block.
	 * It stores the property's id, offset entry from the start of the memory
	 * block and size in bytes.
	 */
	struct RpPrtStdPropertyTable
	{
		enum class EmitterProperties : int {
			EMITTER = 0,
			STANDARD = 1,
			PRTCOLOR = 2,
			PRTTEXCOORDS = 3,
			PRT2DROTATE = 4,
			PRTSIZE = 5,
			PTANK = 6,
			PRTVELOCITY = 7,
			PRTMATRIX = 8,

			ADVPROPERTYCODEEMITTERPRTCHAIN = 0x1000000,
			ADVPROPERTYCODEEMITTERPRTEMITTER = 0x1000001,
			ADVPROPERTYCODEEMITTERPRTMULTICOLOR = 0x1000002,
			ADVPROPERTYCODEEMITTERPRTMULTITEXCOORDS = 0x1000003,
			ADVPROPERTYCODEEMITTERPRTMULTITEXCOORDSSTEP = 0x1000004,
			ADVPROPERTYCODEEMITTERPRTMULTISIZE = 0x1000005,
			ADVPROPERTYCODEEMITTERPOINTLIST = 0x1000006,
			ADVPROPERTYCODEEMITTERCIRCLE = 0x1000007,
			ADVPROPERTYCODEEMITTERSPHERE = 0x1000008,
		};
		enum class ParticleProperties : int {
			STANDARD = 0,
			POSITION = 1,
			COLOR = 2,
			TEXCOORDS = 3,
			PRT2DROTATE = 4,
			SIZE = 5,
			VELOCITY = 6,
			MATRIX = 7,

			ADVPROPERTYCODEPARTICLECHAIN = 0x1000000,
			ADVPROPERTYCODEPARTICLEEMITTER = 0x1000001,
			ADVPROPERTYCODEPARTICLEMULTICOLOR = 0x1000002,
			ADVPROPERTYCODEPARTICLEMULTITEXCOORDS = 0x1000003,
			ADVPROPERTYCODEPARTICLEMULTITEXCOORDSSTEP = 0x1000004,
			ADVPROPERTYCODEPARTICLEMULTISIZE = 0x1000005,
		};


		unsigned int* propPtr; /**< Internal usage */
		RpPrtStdPropertyTable* next; /**< Internal usage */
		int id; /**< Property table's id */
		int refCount; /**< Reference count. Internal usage */
		int numProp; /**< Number of properties in the table */
		int maxProp; /**< Internal usage */
		int* propID; /**< Array of properties's id */
		int* propOffset; /**< Array of properties's offset */
		int* propSize; /**< Array of properties's size */
		int* propSizeUA; /**< Internal usage */

		static inline RpPrtStdPropertyTable* (__cdecl* const Get)(int id) = reinterpret_cast<RpPrtStdPropertyTable * (__cdecl*)(int)>(0x5D9820);
		void Destroy(); // decrements refcount, destroys when unused
		int GetPropOffset(EmitterProperties id);
		int GetPropOffset(ParticleProperties id);
	};
	
	/**
	 * \ingroup rpprtstd
	 * \ref RpPrtStdEmitterCallBackCode, this type represents the different callbacks for
	 * processing a \ref RpPrtStdEmitter. These callbacks will be called at various point for
	 * processing the emitter's custom data.
	 */
	enum class RpPrtStdEmitterCallBackCode : int
	{
		PRTSTDEMITTERCALLBACKEMIT = 0, /**< Emitter particle emission callback */
		PRTSTDEMITTERCALLBACKBEGINUPDATE,              /**< Emitter begin update call back */
		PRTSTDEMITTERCALLBACKENDUPDATE,                /**< Emitter end update callback */
		PRTSTDEMITTERCALLBACKBEGINRENDER,              /**< Emitter begin render callback */
		PRTSTDEMITTERCALLBACKENDRENDER,                /**< Emitter end render callback */
		PRTSTDEMITTERCALLBACKCREATE,                   /**< Emitter create callback */
		PRTSTDEMITTERCALLBACKDESTROY,                  /**< Emitter destroy callback */
		PRTSTDEMITTERCALLBACKSTREAMREAD,               /**< Emitter stream input callback */
		PRTSTDEMITTERCALLBACKSTREAMWRITE,              /**< Emitter stream output callback */
		PRTSTDEMITTERCALLBACKSTREAMGETSIZE,            /**< Emitter stream get size callback */
		PRTSTDEMITTERCALLBACKCLONE,                    /**< Emitter clone  callback */
	};

	enum class EmitterClassId : int {
		STANDARD = 0x00000001,
		PRTCOLOR = 0x00000002,
		PRTTEXCOORDS = 0x00000004,
		PRT2DROTATE = 0x00000008,
		PRTSIZE = 0x00000010,
		PTANK = 0x00000020,
		PRTMATRIX = 0x00000040,
		ADVPRTEMITTER = 0x00000100,
		ADVMULTICOLOR = 0x00000200,
		ADVMULTITEXCOORDS = 0x00000400,
		ADVMULTITEXCOORDSSTEP = 0x00000800,
		ADVMULTISIZE = 0x00001000,
		ADVPOINTLIST = 0x00010000,
		ADVCIRCLE = 0x00020000,
		ADVSPHERE = 0x00030000,
	};
	template<>
	class ::enum_is_flags<EmitterClassId> : public std::true_type {};
	enum class ParticleClassId : int {
		STANDARD = 0x00000001,
		POSITION = 0x00000002,
		COLOR = 0x00000004,
		TEXCOORDS = 0x00000008,
		PRT2DROTATE = 0x00000010,
		SIZE = 0x00000020,
		VELOCITY = 0x00000040,
		MATRIX = 0x00000080,

		ADVPRTEMITTER = 0x00000100,
		ADVMULTICOLOR = 0x00000200,
		ADVMULTITEXCOORDS = 0x00000400,
		ADVMULTITEXCOORDSSTEP = 0x00000800,
		ADVMULTISIZE = 0x00001000,
	};
	template<>
	class ::enum_is_flags<ParticleClassId> : public std::true_type {};

	/**
	 * \ingroup rpprtstd
	 * \ref RpPrtStdEmitterCallBack
	 * \ref RpPrtStdEmitterCallBack represents the function called for processing
	 * a \ref RpPrtStdEmitter. There can several types of the functions, each performing a
	 * specific task defined by \ref RpPrtStdEmitterCallBackCode.
	 *
	 * \param atomic        A pointer to the emitter's parent atomic.
	 * \param emt           A pointer to the emitter.
	 * \param data          A pointer to user data.
	 *
	 * \return A pointer to the emitter on success. NULL otherwise.
	 */
	typedef RpPrtStdEmitter* (*RpPrtStdEmitterCallBack) (RWE::RpAtomic* atomic, RpPrtStdEmitter* emt, void* data);
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterClass
	 * A structure representing the collection of data and callbacks for an emitter. An emitter
	 * class is used to create emitters that share the same properties and callback functions.
	 * Once created, an emitter class cannot be altered. An emitter class should also not be
	 * destroyed if emitters of the class are still in use.
	 */
	struct RpPrtStdEmitterClass
	{
		static constexpr int NumEmitterCallbacksInArray = 11;
		typedef RpPrtStdEmitterCallBack RpPrtStdEmitterCallBackArray[NumEmitterCallbacksInArray];

		RpPrtStdEmitterClass* next; /**< Internal usage */
		int id; /**< Emitter class's id */
		int refCount; /**< Reference count. Internal usage */
		int objSize; /**< Size of the emitter */
		RpPrtStdPropertyTable* propTab; /**< Reference to a table of emitter properties */
		int numCallback; /**< Number of callback arrays */
		RpPrtStdEmitterCallBackArray* callback; /**< Array of emitter callback functions */


		void Destroy();
		void SetProperties(RpPrtStdPropertyTable* p);
		void SetCallback(int numCb, RpPrtStdEmitterCallBackArray* cbs); // copies array
		static inline RpPrtStdEmitterClass* (__cdecl* const GetByPropTab)(RpPrtStdPropertyTable* p) = reinterpret_cast<RpPrtStdEmitterClass * (__cdecl*)(RpPrtStdPropertyTable*)>(0x5DA4E0);
		static inline RpPrtStdEmitterClass* (__cdecl* const Get)(int id) = reinterpret_cast<RpPrtStdEmitterClass * (__cdecl*)(int)>(0x5DA4B0);
		// setup callback 5DE1A0
		RpPrtStdEmitterCallBack GetCallbackNum(RpPrtStdEmitterCallBackCode c, int cn);
	};

	struct RpPrtStdParticleBatch;
	/**
	 * \ingroup rpprtstd
	 * \ref RpPrtStdParticleCallBack represents the function called for processing
	 * a \ref RpPrtStdParticleBatch. There can be several types of the functions, each
	 * performing a specific task defined by \ref RpPrtStdParticleCallBackCode.
	 *
	 * \param emt           A pointer to the particle batch's parent emitter.
	 * \param prtBatch      A pointer to the particle batch.
	 * \param data          A pointer to user data.
	 *
	 * \return A pointer to the particle batch on success. NULL otherwise.
	 */
	typedef RpPrtStdParticleBatch* (*RpPrtStdParticleCallBack) (RpPrtStdEmitter* emt, RpPrtStdParticleBatch* prtBatch, void* data);
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdParticleClass
	 * A structure representing the collection of data and callbacks for a particle. A particle
	 * class is used to create particle that share the same properties and callback functions.
	 * Once created, a particle class cannot be altered. A particle class should also not be
	 * destroyed if particles of the class are still in use.
	 */
	struct RpPrtStdParticleClass
	{
		RpPrtStdParticleClass* next; /**< Internal usage */
		int id; /**< Particle class's id */
		int refCount; /**< Reference count. Internal usage */
		int objSize; /**< Size of a particle */
		RpPrtStdPropertyTable* propTab; /**< Reference to a table of particle properties */
		int numCallback; /**< Number of callback sets */
		RpPrtStdParticleCallBack** callback; /**< Array of particle batch callback functions */

		void SetProperties(RpPrtStdPropertyTable* p);
		void Destroy();
		static inline RpPrtStdParticleClass* (__cdecl* const GetByPropTab)(RpPrtStdPropertyTable* p) = reinterpret_cast<RpPrtStdParticleClass * (__cdecl*)(RpPrtStdPropertyTable*)>(0x5DA810);
		static inline RpPrtStdParticleClass* (__cdecl* const Get)(int id) = reinterpret_cast<RpPrtStdParticleClass * (__cdecl*)(int)>(0x5DA7E0);
		// setup callback 5DA740
	};

	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterStandard
	 * A structure represent the set of properties for a basic emitter. The structure is stored as
	 * a single optional data in the emitter property table.
	 *
	 * The basic emitter uses a 'time' variable to control particle emissions. A large time gap between
	 * emission can give pulse effects where as a small time gap produces a streaming effects. Particles
	 * can be emitted from a single point, line, area or a volume.
	 *
	 * Particles emitted from an emitter are of the same type. An emitter cannot emit different types of
	 * particles. Once an emitter has reached its maximum number of particles, no further particles are
	 * emitted until some of the existing particles have died.
	 *
	 * Most properties have a bias value to vary the property's value. This uses the seed field
	 * to give a degreee of randomness.
	 */
	struct RpPrtStdEmitterStandard
	{
		int seed; /**< Seed for random number generator */
		int maxPrt; /**< Maximum number of active particles */
		float currTime; /**< Current timestamp for emitter */
		float prevTime; /**< Previous timestamp for emitter */
		RwV3d force; /**< External force applied to particles */
		RwV3d emtPos; /**< Emitter position */
		RwV3d emtSize; /**< 10 Emitter size. This is the volume where new particles are emitted */
		float emtEmitGap, /**< 13 Time gap between emission */
			emtEmitGapBias, /**< Time gap bias */
			emtPrevEmitTime, /**< Previous particle emission time */
			emtEmitTime; /**< Next particle emission time */
		int emtPrtEmit, /**< 17 Number of particle emit per emission */
			emtPrtEmitBias; /**< Particle emission bias */

		float prtInitVel, /**< Particle's initial speed */
			prtInitVelBias; /**< 20 Particle's initial speed bias */
		float prtLife, /**< Particle's duration */
			prtLifeBias; /**< Particle's duration bias */

		RwV3d prtInitDir, /**<23 Particle's initial direction */
			prtInitDirBias; /**<26 particle's initial direction bias */

		RwV2d prtSize; /**<29 Particle's size. This is ignored if each particle has its own size. */

		RwRGBA prtColor; /**<31 Particle's color. This is ignored if each particle has its own colors. */

		float prtDelta2DRotate; /**<32 Particle's 2D rotation in radians. This is ignored if each particle has its own rotation. */

		RwTexCoords prtUV[4]; /**<33 Particle's texture coordindates */
		RwTexture* texture; /**< Particle's texture */

		RwMatrix* ltm; /**< LTM to transform particle coordinates from local space to world space */
	};
	//constexpr int i = offsetof(RpPrtStdEmitterStandard, prtDelta2DRotate) / 4;
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdParticleStandard
	 * A structure represent the set of properties for a basic particle. This structure is mandatory
	 * for all particles.
	 * The structure contain the life duration of the particle. The particle will be removed when the current
	 * time is greater than the end time. The particle timer always starts at zero.
	 *
	 * The inverted end time is for interpolating optional particle properties.
	 */
	struct RpPrtStdParticleStandard
	{
		int                     flag;                               /**< Particle's property flag */
		float                      currTime,                           /**< Particle's curr time  */
			endTime,                            /**< Particle's end time */
			invEndTime;                         /**< Particle's inv end for interpolation */
	};

	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterPrtColor
	 * An optional structure to represent the change in color of a particle from birth to death.
	 * The particles will start with the prtStartCol and ends with endPrtColor. The particle's
	 * life is used to interpolate the colors.
	 *
	 * If this structure is not present, then the particles will have a constant color.
	 */
	struct RpPrtStdEmitterPrtColor
	{
		RwRGBAReal prtStartCol, /**< Particle start color */
			prtStartColBias; /**< Particle start color bias */
		RwRGBAReal prtEndCol, /**< Particle end color */
			prtEndColBias; /**< Particle end color bias */
	};
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdParticleColor
	 * A structure representing the change in a particle's color during its life time. The structure is optional
	 * and should not be used if particles have a constant color.
	 */
	struct RpPrtStdParticleColor
	{
		RwRGBAReal                  startCol,                           /**< Particle's end size bias */
			deltaCol;                           /**< Particle's color rate of change */
	};

	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterPrtTexCoords
	 * An optional structure to represent the change in texcoords of a particle from birth to death.
	 * The particles will start with prtStartUV0 and prtStartUV1 and ends with endPrtEndUV0 and endPrtEndUV1.
	 * The particle's life is used to interpolate the texcoords.
	 *
	 * A particle can have two texcoords representing the top left and bottom right respectively. By varying
	 * the texcoords can give an animated texture effect on a particle.
	 *
	 * If this structure is not present, then the particles will have a constant texcoords.
	 */
	struct RpPrtStdEmitterPrtTexCoords
	{
		RwTexCoords prtStartUV0, /**< Particle start top left texcoords */
			prtStartUV0Bias; /**< Particle start top left texcoords bias */
		RwTexCoords prtEndUV0, /**< Particle end top left texcoords */
			prtEndUV0Bias; /**< Particle end top left texcoords bias */
		RwTexCoords prtStartUV1, /**< Particle start bottom right texcoords */
			prtStartUV1Bias; /**< Particle start botton right texcoords bias */
		RwTexCoords prtEndUV1, /**< Particle end bottom right texcoords */
			prtEndUV1Bias; /**< Particle end bottom right texcoords bias */
	};
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdParticleTexCoords
	 * A structure representing the change in a particle's texcoords during its life time. The structure is optional
	 * and should not be used if particles have a constant texcoords.
	 */
	struct RpPrtStdParticleTexCoords
	{
		RwTexCoords                 startUV0,                           /**< Particle's starting top left texcoords */
			startUV1;                           /**< Particle's starting bottom right texcoords */
		RwTexCoords                 deltaUV0,                           /**< Particle's top left texcoords rate of change */
			deltaUV1;                           /**< Particle's bottom right texcoords rate of change */
	};

	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterPrt2DRotate
	 * An optional structure to represent the 2D rotation of a particle from birth to death.
	 * The particles will start with being rotated by prtStart2DRotate and ends with being rotated by prtEnd2DRotate.
	 * The rotation is specified in radians. The starting rotation must be between -PI and PI.
	 * Multiple revolutions can be obtained by specifying an end rotation > the start rotation + 2 PI.
	 *
	 * The particle's life is used to interpolate the size.
	 *
	 * If this structure is not present, then the particles will either have a global rotation or no rotation.
	 */
	struct RpPrtStdEmitterPrt2DRotate
	{
		float prtStart2DRotate, /**< Particle start 2D rotate */
			prtStart2DRotateBias; /**< Particle start 2D rotate bias */
		float prtEnd2DRotate, /**< Particle end 2D rotate */
			prtEnd2DRotateBias; /**< Particle end 2D rotate bias */
	};
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdParticle2DRotate
	 * A structure representing the changed in the 2D rotation of a particle during its life time. The structure is optional
	 * and should not be used if particles does not rotate or uses a matrix.
	 */
	struct RpPrtStdParticle2DRotate
	{
		float                      start2DRotate,                      /**< Particle's starting 2D rotation */
			delta2DRotate;                      /**< Particle's 2D rotation rate of change */
	};

	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterPrtSize
	 * An optional structure to represent the change in size of a particle from birth to death.
	 * The particles will start with prtStartSize and ends with prtEndSize. The particle's life
	 * is used to interpolate the size.
	 *
	 * If this structure is not present, then the particles will have a constant size.
	 */
	struct RpPrtStdEmitterPrtSize
	{
		RwV2d prtStartSize, /**< Particle start size */
			prtStartSizeBias; /**< Particle start size bias */
		RwV2d prtEndSize, /**< Particle end size */
			prtEndSizeBias; /**< Particle end size bias */
	};
	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdParticleSize
	 * A structure representing the change in a particle's size during its life time. The structure is optional
	 * and should not be used if particles have a constant size.
	 */
	struct RpPrtStdParticleSize
	{
		RwV2d                       startSize,                          /**< Particle's starting size */
			deltaSize;                          /**< Particle's size rate of change */
		RwV2d                       currSize,                           /**< Particle's current size. Internal use only */
			invCurrSize;                        /**< Particle's inverse current size. Internal use only */
	};

	/**
	 * \ingroup rpptank
	 * Passed to \ref RpPTankAtomicCreate, these flags specify
	 * the type and properties held by the particles.
	 * Some flags are mutually exclusive and should not be mixed.
	 * The debug version of the library will assert and signal these problems.
	 */
	enum class RpPTankDataFlags : int
	{
		NONE = ((int)0x00000000),
		POSITION = ((int)0x00000001),   /**<Uses a position per particle*/
		COLOR = ((int)0x00000002),   /**<Uses a color per particle*/
		SIZE = ((int)0x00000004),   /**<Uses a size per particle. This flag is ignored if the particle is a matrix.*/
		MATRIX = ((int)0x00000008),   /**<Uses a matrix per particle*/
		NORMAL = ((int)0x00000010),   /**<Uses a normal per particle*/
		F2DROTATE = ((int)0x00000020),   /**<Uses a 2D rotation per particle*/
		VTXCOLOR = ((int)0x00000040),   /**<Uses a color per Billboard vertex*/
		VTX2TEXCOORDS = ((int)0x00000080),   /**<Uses Top-Left and Bottom-Right Texture coordinates*/
		VTX4TEXCOORDS = ((int)0x00000100),   /**<Uses a texture UV per vertex*/
		CNSMATRIX = ((int)0x00008000),   /**<Uses a constant matrix*/
		CNS2DROTATE = ((int)0x00020000),   /**<Uses a constant 2D rotation*/
		CNSVTXCOLOR = ((int)0x00040000),   /**<Uses a constant color per Billboard vertex*/
		CNSVTX2TEXCOORDS = ((int)0x00080000),   /**<Uses constant Top-Left and Bottom-Right Texture coordinates*/
		CNSVTX4TEXCOORDS = ((int)0x00100000),   /**<Uses a constant texture UV per vertex*/
		USECENTER = ((int)0x01000000),    /**<The position of the particles are shifted*/
		ARRAY = ((int)0x10000000),    /**<Data is organized in an array */
		STRUCTURE = ((int)0x20000000),    /**<Data is organized in a structure */
	};
	template<>
	class ::enum_is_flags<RpPTankDataFlags> : public std::true_type {};

	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterPTank
	 *
	 * A structure for storing the data required to create a RpPTank for use
	 * with the emitter. The structure allows the user to create a RpPTank
	 * manually.
	 */
	struct RpPrtStdEmitterPTank
	{
		int dataFlags, /**< Data flag used in RpPTank creation. See \ref RpPTankAtomicCreate  */
			platFlags, /**< Platform flag used in RpPTank creation. See \ref RpPTankAtomicCreate */
			numPrt, /**< An integer representing the current number of active * particles */
			maxPrt; /**< An integer representing the maxiumum number of particles * stored in the RpPTank */
		RpPTankDataFlags updateFlags, /**< A flag representing the properties to be updated by the particle emiiter during update. A user may select to
							 * update some properties manually by unsetting the relevant bits in the flag. The flag settings are the same as \ref RpPTankDataFlags */
			emitFlags; /**< A flag representing the properties to be initialized * by the particle emitter during particles emission. A user
							 * may select to initialize some properties manually by unsetting the relevant bits in the flag.
							 * The flag settings are the same as \ref RpPTankDataFlags */
		RpAtomic* pTank; /**< Pointer to the RpPTank \ref RpAtomic. */
		char** dataInPtrs, /**< An array of pointers pointing to the particle's property buffers in RpPTank. This array points to the input values to
							  * be updated. The pointers must be incremented after each particle is processed.
							  * The size of the array is dependant on the number of particle properties. The order of the properties is constant.
							  * \see RpPTankDataLockFlags. */
			** dataOutPtrs;  /**< An array of pointers pointing to the particle's property
							 * buffers in RpPTank. This array points to where updated values
							 * are to be written. This can be in the input buffer or a
							 * seperate output buffer. Dead particles are removed by being
							 * overwritten by live particles. So the pointers should only
							 * be incremented for each live particle updated.
							 * The size of the array is dependant on the number of particle
							 * properties. The order of the properties is constant.
							 * \see RpPTankDataLockFlags. */
		int* dataStride;    /**< An array of \ref RwInt32 to store the stride of the
													 * particle properties in the RpPTank. This is the value the
													 * data pointers need to be incremented to the next particle.
													 * The size of the array is dependant on the number of particle
													 * properties. The order of the stride is constant.
													 * \see RpPTankDataLockFlags. */
		unsigned int strSrcBlend;    /**< This is used to set the source blend mode for the RpPTank
													 * \ref RpAtomic when it is created for the first time.
													 * \see RpPTankAtomicSetBlendModes. */
		unsigned int strDstBlend;    /**< This is used to set the destination blend mode for the RpPTank
													 * \ref RpAtomic when it is created for the first time.
													 * \see RpPTankAtomicSetBlendModes. */
		int strVtxABlend;   /**< This is used to enable vertex alpha blending for the
													 * RpTank \ref RpAtomic when it is created for the first time.
													 * \see RpPTankAtomicSetVertexAlpha. */
	};

	/**
	 * \ingroup rpprtstd
	 * \struct RpPrtStdEmitterPrtMatrix
	 *
	 * An optional structure to construct a matrix for each particle during emissions. A particle
	 * can be represented as a single matrix. This gives the particles an orientation rather than
	 * just a simple position.
	 *
	 * This allows transformation to be applied to the particles, such as rotation. If
	 * \ref rpPRTSTDEMITTERPRTMTXFLAGSCNSMTX is set in the flag, then the prtCnsMatrix is applied to each
	 * particle during particle update.
	 *
	 * If this structure is not present, then it assumes the  particles will have just a position
	 * property.
	 */
	struct RpPrtStdEmitterPrtMatrix
	{
		RwMatrix prtCnsMtx; /**< Transformation matrix to be applied to each particle */

		RwV3d prtPosMtxAt, /**< Particle initial look at vector */
			prtPosMtxAtBias; /**< Particle initial look at vector bias */
		RwV3d prtPosMtxUp, /**< Particle initial up vector. */
			prtPosMtxUpBias; /**< Particle initial up vector bias */

		int flags; /**< Particle matrix flag. See \ref RpPrtStdEmitterPrtMatrixFlags */
	};

	/**
	 * \ingroup prtadvprtemitter
	 * \struct RpPrtAdvEmtPrtEmt
	 *
	 * Properties for the PrtEmitter emitter type, that allow each particle to
	 * control a sub-emitter.
	 */
	struct RpPrtAdvEmtPrtEmt
	{
		float                              time;       /**<Time before first
														 * emission of the
														 * sub-emitter.*/
		float                              timeBias;   /**<Time bias.*/
		float                              timeGap;    /**<Time gap between each
														 * emission of the
														 * sub-emitter.*/
		float                              timeGapBias;/**<Time gap bias.*/
		RpPrtStdEmitter* emitter;   /**<Pointer to the
														 * sub-emitter.*/
	};
	/**
	 * \ingroup prtadvprtemitter
	 * \struct RpPrtAdvPrtEmitter
	 *
	 * Properties for the PrtEmitter particle type, that allow each particle to
	 * control a sub-emitter.
	 */
	struct RpPrtAdvPrtEmitter
	{
		float                              emtEmitTime;        /**<Time before
																 * next emission of
																 * the sub-emitter.*/
		float                              emtEmitTimeGap;     /**<Time gap between
																 *  each emission of
																 *  the sub-emitter.*/
	};

	/**
	 * \ingroup prtadvmultiprop
	 * \struct RpPrtAdvEmtPrtMultiColor
	 *
	 * Properties for the MultiColor emitter type, that allow particles' colors to
	 * be interpolated through several colors.
	 *
	 */
	struct RpPrtAdvEmtPrtMultiColor
	{
		/**
		 * \ingroup prtadvmultiprop
		 * \struct RpPrtAdvEmtPrtColorItem
		 *
		 * Used to store each color keyframe when using the MultiColors particles.
		 */
		struct RpPrtAdvEmtPrtColorItem
		{
			float                              time;       /**<Time.*/
			float                              timeBias;   /**<Time bias.*/
			RwRGBAReal                          midCol;     /**<Color.*/
			RwRGBAReal                          midColBias; /**<Color bias.*/
		};

		uint32_t                            numCol; /**<Number of steps.*/
		RpPrtAdvEmtPrtColorItem* list;  /**<Pointer to the
													 * \ref RpPrtAdvEmtPrtColorItem
													 * describing the different step
													 *  of the color animation.*/

		uint32_t                            offsetTime;         /**< Internal usage */
		uint32_t                            offsetStartCol;     /**< Internal usage */
		uint32_t                            offsetDeltaCol;     /**< Internal usage */
	};
	/**
	 * \ingroup prtadvmultiprop
	 * \struct RpPrtAdvPrtMultiColor
	 *
	 * Properties for the MultiColor particle type, that allow particle's colors to
	 * be interpolated through several colors.
	 * These properties are generated by the MultiColor emitter.
	 */
	struct RpPrtAdvPrtMultiColor
	{
		uint32_t                            currIndex;  /**<Current position in the animation.*/
	};

	/**
	 * \ingroup prtadvmultiprop
	 * \struct RpPrtAdvEmtPrtMultiTexCoords
	 *
	 * Properties for the MultiTexCoords emitter type, that allow particles' texture
	 * coordinates to be interpolated through several texture coordinates.
	 */
	struct RpPrtAdvEmtPrtMultiTexCoords
	{
		/**
		 * \ingroup prtadvmultiprop
		 * \struct RpPrtAdvEmtPrtTexCoordsItem
		 *
		 * Used to store each Texture Coordinates keyframe when using the MultiTexCoords
		 * particles.
		 */
		struct RpPrtAdvEmtPrtTexCoordsItem
		{
			float                              time;       /**<Time.*/
			float                              timeBias;   /**<Time bias.*/
			RwTexCoords                         midUV0;     /**<Top-Left UV set.*/
			RwTexCoords                         midUV0Bias; /**<Top-Left UV set bias.*/
			RwTexCoords                         midUV1;     /**<Bottom-Right UV set.*/
			RwTexCoords                         midUV1Bias; /**<Bottom-Right UV set
															 * bias.*/
		};


		uint32_t                            numTex; /**<Number of steps.*/
		RpPrtAdvEmtPrtTexCoordsItem* list;  /**<Pointer to the
													 * \ref RpPrtAdvEmtPrtTexCoordsItem
													 * describing the different step
													 *  of the texture coordinates
													 * animation.*/

		uint32_t                            offsetTime;         /**< Internal usage */
		uint32_t                            offsetStartUV0;     /**< Internal usage */
		uint32_t                            offsetDeltaUV0;     /**< Internal usage */
		uint32_t                            offsetStartUV1;     /**< Internal usage */
		uint32_t                            offsetDeltaUV1;     /**< Internal usage */
	};
	/**
	 * \ingroup prtadvmultiprop
	 * \struct RpPrtAdvPrtMultiTexCoords
	 *
	 * Properties for the MultiTexCoords particle type, that allow particle's
	 * texture coordinates to be interpolated through several texture coordinates.
	 * These properties are generated by the MultiTexCoords emitter.
	 */
	struct RpPrtAdvPrtMultiTexCoords
	{
		uint32_t                            currIndex;  /**<Current position in the animation.*/
	};
	/**
	 * \ingroup prtadvtexstep
	 * \struct RpPrtAdvPrtTexStep
	 *
	 * Properties for the MultiTexCoordsStep particle type, that allow particle's
	 * texture coordinates to be set one by one.
	 * These properties are generated by the MultiTexCoordsStep emitter.
	 */
	struct RpPrtAdvPrtTexStep
	{
		uint32_t                            currIndex;  /**<Current position in the animation.*/
	};

	/**
	 * \ingroup prtadvmultiprop
	 * \struct RpPrtAdvEmtPrtMultiSize
	 *
	 * Properties for the MultiSize emitter type, that allow particles' size to be
	 * interpolated through several sizes.
	 */
	struct RpPrtAdvEmtPrtMultiSize
	{
		/**
		 * \ingroup prtadvmultiprop
		 * \struct RpPrtAdvEmtPrtSizeItem
		 *
		 * Used to store each size keyframe when using the MultiSize particles.
		 */
		struct RpPrtAdvEmtPrtSizeItem
		{
			float                              time;           /**<Time.*/
			float                              timeBias;       /**<Time bias.*/
			RwV2d                               midSize;        /**<Size.*/
			RwV2d                               midSizeBias;    /**<Size bias.*/
		};


		uint32_t                            numSize;    /**<Number of steps.*/
		RpPrtAdvEmtPrtSizeItem* list;      /**<Pointer to the
														 * \ref RpPrtAdvEmtPrtSizeItem
														 * describing the different
														 * step of the size
														 * animation.*/

		uint32_t                            offsetTime;         /**< Internal usage */
		uint32_t                            offsetStartSize;    /**< Internal usage */
		uint32_t                            offsetDeltaSize;    /**< Internal usage */
	};
	/**
	 * \ingroup prtadvmultiprop
	 * \struct RpPrtAdvPrtMultiSize
	 *
	 * Properties for the MultiSize particle type, that allow particle's sizes to be
	 * interpolated through several sizes.
	 * These properties are generated by the MultiSize emitter.
	 */
	struct RpPrtAdvPrtMultiSize
	{
		uint32_t                            currIndex;  /**<Current position in the animation.*/
	};

	/**
	 * \ingroup prtadvcircle
	 * \struct RpPrtAdvEmtCircle
	 *
	 * Properties for the circular emitter type, that allow to use a circular, disc or
	 * cylindrical shaped emitter.
	 */
	struct RpPrtAdvEmtCircle {
		float                              radius;             /**<Radius.*/
		float                              radiusGap;          /**<Width of the
																 * disc.*/
		float                              height;             /**<Height of the
																 * shape.*/

		int                              useCircleEmission;  /**<Set to generate
																 * the particle
																 * initial velocity
																 * using it's
																 * position on the
																 * shape.*/
		// rad
		float                              dirRotation;        /**<Rotate the
																 * initial velocity
																 * around the circle
																 *  (used when
																 * \ref useCircleEmission
																 * is set to TRUE).*/
	};

	/**
	 * \ingroup prtadvpointlist
	 * \struct RpPrtAdvEmtPointList
	 *
	 * Properties for the PointList emitter type, that allow to use a list of point
	 * (and an optional direction) to vary the particle's position
	 * (and optionally it's initial velocity).
	 */
	struct RpPrtAdvEmtPointList {
		uint32_t                            numPoint;       /**<Number of points
													 * in the list.*/
		uint32_t                            lastPoint;      /**<Index of the
															 * last point used to
															 * emit particles.*/
		int                              useDirection;   /**<Use or not the
															 * direction list to
															 * generate the initial
															 * velocity of the
															 * particles.*/
		int                              random;         /**<Set if the points
															 * are to be used
															 * randomly.*/
		RwV3d* pointList;     /**<Pointer to the
															 * list of point.*/
		RwV3d* directionList; /**<Pointer to the
															 * list of directions.*/
	};

	/**
	 * \ingroup prtadvsphere
	 * \struct RpPrtAdvEmtSphere
	 *
	 * Properties for the spherical emitter type, that allow to use a spherical shaped emitter.
	 */
	struct RpPrtAdvEmtSphere
	{
		float                              radius;             /**<Radius.*/
		float                              radiusGap;          /**<Width of the sphere.*/

		int                              useSphereEmission;  /**<Set to set the
																 * particle initial
																 * velocity using
																 * it's position on
																 * the shape.*/
	};

	enum class RpPrtStdEmitterFlags : unsigned int {
		rpPRTSTDEMITTERFLAGALIVE = 0x00000001, /**< The emitter is alive and should not be removed */
		rpPRTSTDEMITTERFLAGACTIVE = 0x00000002, /**< This indicates the emitter is active or idle */
		rpPRTSTDEMITTERFLAGPTANK = 0x00000004, /**< The emitter uses the \ref rpptank */
		rpPRTSTDEMITTERFLAGEMIT = 0x00000008, /**< This indicates the emitter can emit new particles */
		rpPRTSTDEMITTERFLAGUPDATE = 0x00000010, /**< This indicates if the emitter is to be updated. */
		rpPRTSTDEMITTERFLAGUPDATEPARTICLE = 0x00000020, /**< This indicated if the emitter's particles are updated. */
		rpPRTSTDEMITTERFLAGRENDER = 0x00000040, /**< This indicates if the emitter is rendered. */
		rpPRTSTDEMITTERFLAGRENDERPARTICLE = 0x00000080, /**< This indicates if the emitter's particles are rendered. */
		rpPRTSTDEMITTERFLAGNOBUFFERSWAP = 0x00000100, /**< Internal usage */
		rpPRTSTDEMITTERFLAGSTREAMREAD = 0x00000200, /**< Internal usage */
	};

	struct RpPrtStdEmitter {


		RpPrtStdEmitter* next; /**< Pointer to the next emitter */

		RpPrtStdEmitterFlags flag; /**< Emitter's property flag */

		RpAtomic* atomic; /**< Pointer to emitter's parent atomic */

		RpPrtStdEmitterClass* emtClass; /**< Pointer the emitter's class */

		int prtActive; /**< Number of particles emitted */
		int prtBatchMaxPrt; /**< Maximum number of particles per batch */
		RpPrtStdParticleClass* prtClass;                  /**< Pointer to the particle's class */

		RpPrtStdParticleBatch* inactiveBatch; /**< Circular linked list of inactive particle batches */
		RpPrtStdParticleBatch* activeBatch; /**< Circular linked list of active particle batches */

		float boundingSphere; /**< Emitter's bounding sphere. */

		void AddEmitter(RpPrtStdEmitter* em);

		RpPrtStdEmitterStandard* GetStandard();
		RpPrtStdEmitterPrtColor* GetColor();
		RpPrtStdEmitterPrtTexCoords* GetTexCoords();
		RpPrtStdEmitterPrt2DRotate* GetRotate();
		RpPrtStdEmitterPrtSize* GetSize();
		RpPrtStdEmitterPTank* GetTank();
		RpPrtStdEmitterPrtMatrix* GetMatrix();
		RpPrtAdvEmtPrtEmt* GetAdvParticleEmitter();
		RpPrtAdvEmtCircle* GetAdvCircleEmitter();
		RpPrtAdvEmtPointList* GetAdvPointListEmitter();

		static inline const RpPrtStdEmitterCallBack* DisableAll = reinterpret_cast<RpPrtStdEmitterCallBack*>(0x720028);
		static inline const RpPrtStdEmitterCallBack* EnableAll = reinterpret_cast<RpPrtStdEmitterCallBack*>(0x72001F);
		static inline const RpPrtStdEmitterCallBack* InitSeed = reinterpret_cast<RpPrtStdEmitterCallBack*>(0x48F053);
	private:
		void* GetDataById(RpPrtStdPropertyTable::EmitterProperties p);
	};

}

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
	};
	
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
		RpPrtStdEmitterClass* next; /**< Internal usage */
		int id; /**< Emitter class's id */
		int refCount; /**< Reference count. Internal usage */
		int objSize; /**< Size of the emitter */
		RpPrtStdPropertyTable* propTab; /**< Reference to a table of emitter properties */
		int numCallback; /**< Number of callback arrays */
		RpPrtStdEmitter** callback; /**< Array of emitter callback functions */
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
		// set callback 5DA740
	};

	struct RpPrtStdEmitter {
		RpPrtStdEmitter* next; /**< Pointer to the next emitter */

		unsigned int flag; /**< Emitter's property flag */

		RpAtomic* atomic; /**< Pointer to emitter's parent atomic */

		RpPrtStdEmitterClass* emtClass; /**< Pointer the emitter's class */

		int prtActive; /**< Number of particles emitted */
		int prtBatchMaxPrt; /**< Maximum number of particles per batch */
		RpPrtStdParticleClass* prtClass;                  /**< Pointer to the particle's class */

		RpPrtStdParticleBatch* inactiveBatch; /**< Circular linked list of inactive particle batches */
		RpPrtStdParticleBatch* activeBatch; /**< Circular linked list of active particle batches */

		float boundingSphere; /**< Emitter's bounding sphere. */

		void AddEmitter(RpPrtStdEmitter* em);
	};

}

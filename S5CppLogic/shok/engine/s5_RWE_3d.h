#pragma once
#include <shok/engine/s5_RWEngine.h>

namespace RWE::I3D {
	/**
	 * \ingroup cored3d9
	 * \struct RxObjSpace3DVertex
	 * Structure representing object space vertex.
	 */
	struct RxObjSpace3DVertex // NOLINT(*-pro-type-member-init)
	{
		RwV3d objVertex; /**< position */
		RwV3d objNormal; /**< normal */
		unsigned int color; /**< emissive color*/
		float u; /**< u */
		float v; /**< v */
	};
	using RwIm3DVertex = RxObjSpace3DVertex;
	using RwImVertexIndex = uint16_t;

	struct RxPipeline {
		// RwIm3DSetRenderPipeline 0x416CB0
		// RwIm3DSetTransformPipeline 0x416C60
	};

	struct Im3D {
		/*
		*	\ref RwIm3DTransform is used to transform the given vertices into the
		 * current camera in preparation for rendering. Following transformation there
		 * exists an array of camera space vertices, together with an array of screen
		 * space (device-dependent) vertices for those which lie in the camera's
		 * frustum.
		 *
		 * This function executes the immediate mode transform pipeline, which is
		 * accessible through \ref RwIm3DGetTransformPipeline. This transforms
		 * the vertices into world space and caches them ready for rendering by
		 * subsequent calls to RwIm3D render functions. These vertices are cached
		 * and can be used by any number of RwIm3D rendering functions until \ref RwIm3DEnd
		 * is called, at which point the vertices are discarded.
		 *
		 * Note that because RwIm3D behaves (from the point of view of the API and
		 * the application) exactly as it did with the old RenderWare Graphics pipeline,
		 * there are some restrictions due to the details of the implementation using the
		 * new flexible pipeline code. In-between a \ref RwIm3DTransform and \ref RwIm3DEnd
		 * pair, no other pipeline execution, that is, anything which calls
		 * \ref RxPipelineExecute -  can take place. This is because the caching
		 * of the vertices requires the global heap - see \ref RxHeapGetGlobalHeap - to
		 * remain uncleared. Executing other pipelines would clear the heap. There
		 * will be a successor to RwIm3D which will work more cleanly with the new
		 * PowerPipe functionality and which will not have these restrictions, but
		 * that is not yet available.
		 *
		 * This function takes an optional matrix which is concatenated onto the
		 * world-to-camera matrix before vertices are transformed. This is useful if
		 * the vertices are in object space, in which case the argument should be the
		 * local transformation matrix.
		 *
		 * \note The \ref RwIm3DTransformFlags flags \ref rwIM3D_VERTEXXYZ and
		 *       \ref rwIM3D_VERTEXRGBA are added automatically by this function, thus
		 *       requiring at least positional and color information in the \e pVerts data.
		 *
		 * \param pVerts  A pointer to the \ref RwIm3DVertex array to transform
		 * \param numVerts  An \ref RwInt32 value equal to the number of vertices to transform
		 * \param ltm  A pointer to the object-to-world matrix, or
		 *       NULL if the vertices are provided in world space
		 * \param flags  flags of type \ref RwIm3DTransformFlags.
		 *
		 * \return Returns the vertex array if successful, otherwise NULL.
		*/
		static inline void* (__cdecl* const Transform)(RwIm3DVertex* pVerts, size_t numVerts, RwMatrix* ltm, RwIm3DTransformFlags flags) = reinterpret_cast<void* (__cdecl*) (RwIm3DVertex*, size_t, RwMatrix*, RwIm3DTransformFlags)>(0x416A00);
		/*
		* \ref RwIm3DRenderIndexedPrimitive executes the remainder of the
		 * RwIm3D transform pipeline, (internally passing in the cached packets
		 * containing the transformed vertices) and specifying the primitive
		 * type and index data in the (void *)data parameter of \ref RxPipelineExecute.
		 *
		 * \param primType  \ref RwPrimitiveType primitive type
		 * \param indices  pointer to \ref RwImVertexIndex array of indices
		 * \param numIndices  \ref RwInt32 number of Indices in the primitive.
		 *
		 * \return Returns TRUE on success, FALSE on error.*/
		static inline int(__cdecl* const RenderIndexedPrimitive)(RwPrimitiveType primType, RwImVertexIndex* indices, int numIndices) = reinterpret_cast<int(__cdecl*)(RwPrimitiveType, RwImVertexIndex*, int)>(0x416B00);
		/*
		* \ref RwIm3DEnd is used to indicate that the application has finished
		 * using the transformed 3D immediate mode vertices and they can be discarded
		 * by the engine. After this is called, other pipelines may be executed since
		 * the global heap (see \ref RxHeapGetGlobalHeap) will have been cleared.
		 *
		 * \return TRUE on success, FALSE otherwise.*/
		static inline int(__cdecl* const End)() = reinterpret_cast<int(__cdecl*)()>(0x416AD0);
	};
}

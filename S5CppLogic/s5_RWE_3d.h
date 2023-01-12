#pragma once
#include "s5_RWEngine.h"

namespace RWE::I3D {
	/**
	 * \ingroup cored3d9
	 * \struct RxObjSpace3DVertex
	 * Structure representing object space vertex.
	 */
	struct RxObjSpace3DVertex
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
		static inline void* (__cdecl* const Transform)(RwIm3DVertex* pVerts, size_t numVerts, RwMatrix* ltm, unsigned int flags) = reinterpret_cast<void* (__cdecl*) (RwIm3DVertex*, size_t, RwMatrix*, unsigned int)>(0x416A00);
		static inline int(__cdecl* const RenderIndexedPrimitive)(RwPrimitiveType primType, RwImVertexIndex* indices, int numIndices) = reinterpret_cast<int(__cdecl*)(RwPrimitiveType, RwImVertexIndex*, int)>(0x416B00);
		static inline int(__cdecl* const End)() = reinterpret_cast<int(__cdecl*)()>(0x416AD0);
	};
}

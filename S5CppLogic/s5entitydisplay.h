#pragma once
#include "s5data.h"

struct shok_ED_IBehavior : shok_object {

};

struct shok_ED_CEntity : shok_object {
	void* Entity_IEntityDisplay;
	int EntityID;
	PADDINGI(1); // 0
	PADDINGI(1); // unk p
	PADDINGI(1); // player? // 5
	PADDINGI(1); // some counter
	vector_padding;
	std::vector<shok_ED_IBehavior*, shok_allocator<shok_ED_IBehavior*>> DisplayBehaviors;
	PADDINGI(1); // unk p // 1
	PADDINGI(1); // 0
	PADDINGI(1); // unk p
	PADDINGI(1); // -1 // 14


	static inline constexpr int vtp = 0x76A494;
};
static_assert(sizeof(shok_ED_CEntity) == 4 * 15);

struct shok_ED_CDisplayStaticEntity : shok_ED_CEntity {
	PADDINGI(1); // p ED::CTerrainDecalAligned // 15
	PADDINGI(1); // 0

	static inline constexpr int vtp = 0x7AE3C4;
};
static_assert(sizeof(shok_ED_CDisplayStaticEntity) == 4 * 17);

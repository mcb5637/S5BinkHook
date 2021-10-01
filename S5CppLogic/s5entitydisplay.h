#pragma once
#include "s5data.h"

struct shok_ED_IBehavior : shok_object {

};

struct shok_ED_CEntity : shok_object {
	shok_IEntityDisplay* Entity;
	int EntityID;
	int ModelOverrideID;
	PADDINGI(1); // unk p might be model data, some sort of initilaized model data
	PADDINGI(1); // some kind of flags? // 5
	PADDINGI(1); // some counter
	vector_padding; // 7
	std::vector<shok_ED_IBehavior*, shok_allocator<shok_ED_IBehavior*>> DisplayBehaviors;
	PADDINGI(1); // unk p // 1
	PADDINGI(1); // 0
	PADDINGI(1); // unk p
	PADDINGI(1); // -1, color? // 14


	static inline constexpr int vtp = 0x76A494;
};
static_assert(sizeof(shok_ED_CEntity) == 4 * 15);

struct shok_ED_CDisplayStaticEntity : shok_ED_CEntity {
	PADDINGI(1); // p ED::CTerrainDecalAligned // 15
	PADDINGI(1); // 0

	static inline constexpr int vtp = 0x7AE3C4;
};
static_assert(sizeof(shok_ED_CDisplayStaticEntity) == 4 * 17);


struct shok_ED_CVisibleEntityManager : shok_object {
	PADDINGI(1); // p EGL::CGLEEntitiesDisplay
	PADDINGI(1); // p to ?
	PADDINGI(1); // p to ED::CRenderSettingsEx
	shok_ED_CEntity* Entities[1]; // indexed by lower half of eid

	static inline constexpr int vtp = 0x76A1A4;

	shok_ED_CEntity* CreateDisplayForEntity(shok_IEntityDisplay* e);
	shok_ED_CEntity* GetDisplayForEntity(int eid);
	void DestroyDisplayForEntity(int eid);
};

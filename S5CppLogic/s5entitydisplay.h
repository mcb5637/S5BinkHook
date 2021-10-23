#pragma once
#include "s5data.h"

struct shok_ED_CBehaviorProps : shok_BB_IObject {
	int Class = 0;
	int Index = 0;
	int Unknown = 100; // 100?? 3

	static inline constexpr int vtp = 0x76AB0C;
	static inline constexpr int TypeDesc = 0x80AC10;
	static inline constexpr unsigned int Identifier = 0x1F78996D;

	static shok_BB_CClassFactory_serializationData* SerializationData;
};

struct shok_GD_CLimitedAttachmentBannerBehaviorProps : shok_ED_CBehaviorProps { // residence
	int Banner, Node;

	static inline constexpr int vtp = 0x76AB1C;
	static inline constexpr int TypeDesc = 0x80AC30;
	static inline constexpr unsigned int Identifier = 0x156D5E5D;
};

struct shok_GD_CCamouflageBehaviorProps : shok_ED_CBehaviorProps {
	int CamouflageModelID; //4
	int CamouflageAlpha;

	static inline constexpr int vtp = 0x76AEA0;
	static inline constexpr int TypeDesc = 0x80B048;
	static inline constexpr unsigned int Identifier = 0x15CB25AD;
};

struct shok_GD_CBuildingBehaviorProps : shok_ED_CBehaviorProps {
	int Banner, NumDamageEffects, NumDestroyEffects;

	static inline constexpr int vtp = 0x76AF1C;
	static inline constexpr int TypeDesc = 0x80B1C0;
	static inline constexpr unsigned int Identifier = 0x2A2142BD;
};

struct shok_GD_CWaterfallAnimationBehaviorProps : shok_ED_CBehaviorProps { // seems to be unused
	int Layer01Speed, Layer02Speed, Layer03Speed;

	static inline constexpr int vtp = 0x7AEA34;
	static inline constexpr int TypeDesc = 0x84D320;
	static inline constexpr unsigned int Identifier = 0x38F70CA3;
};

struct shok_ED_CEntity;
struct shok_ED_IBehavior : shok_BB_IObject {
	shok_ED_CEntity* EntityDisplay = nullptr;
	shok_ED_CBehaviorProps* Props = nullptr;

	static inline constexpr int vtp = 0x76A9E4;
	static inline constexpr int TypeDesc = 0x80AA54;
};

struct shok_GD_CAlphaBlendingBehavior : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x76AA0C;
	static inline constexpr int TypeDesc = 0x80AA70;
};

struct shok_GD_CLimitedAttachmentBannerBehavior : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x76AB2C;
	static inline constexpr int TypeDesc = 0x80AC68;
};

struct shok_GD_CCamouflageBehavior : shok_ED_IBehavior {
	void* Slot; // EGL::TSlot<GGL::SSlotArgsCamouflage,983570077>
	bool UseAltModel;
	PADDING(3);

	static inline constexpr int vtp = 0x76AEB0;
	static inline constexpr int TypeDesc = 0x80B0B8;
	static inline constexpr unsigned int Identifier = 0x26C048ED;
};
static_assert(sizeof(shok_GD_CCamouflageBehavior) == 5 * 4);

struct shok_GD_CBuildingBehavior : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x76AF54;
	static inline constexpr int TypeDesc = 0x80B1E8;
};

struct shok_ED_CUVAnimBehavior : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AE8EC;
	static inline constexpr int TypeDesc = 0x84CF0C;
};

struct shok_ED_CPermanentUVAnimBehavior : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AE914;
	static inline constexpr int TypeDesc = 0x84CFD0;
};

struct shok_ED_CParticleEffectAttachmentBehavior : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AE93C;
	static inline constexpr int TypeDesc = 0x84D01C;
};

struct shok_ED_CParticleEffectSwitchBehavior : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AE964;
	static inline constexpr int TypeDesc = 0x84D050;
};

struct shok_ED_CDisplayBehaviorWaterfallAnimation : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AEA44;
	static inline constexpr int TypeDesc = 0x84D354;
};

struct shok_ED_CDisplayBehaviorMovement : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AEAA4;
	static inline constexpr int TypeDesc = 0x84D3E8;
};

struct shok_ED_CDisplayBehaviorBuildingAnimation : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AEAD4;
	static inline constexpr int TypeDesc = 0x84D4CC;
};

struct shok_ED_CDisplayBehaviorAnimationNoBlending : shok_ED_IBehavior {

	static inline constexpr int vtp = 0x7AEB08;
	static inline constexpr int TypeDesc = 0x84D5B4;
};

struct shok_ED_CDisplayBehaviorAnimation : shok_ED_CDisplayBehaviorAnimationNoBlending {

	static inline constexpr int vtp = 0x7AEB40;
	static inline constexpr int TypeDesc = 0x84D6A4;
};

struct shok_modelinstance;
struct shok_ED_CEntity : shok_BB_IObject {
	shok_IEntityDisplay* Entity;
	int EntityID;
	int ModelOverrideID;
	shok_modelinstance* Model;
	PADDINGI(1); // some kind of flags? // 5
	PADDINGI(1); // some counter
	vector_padding; // 7
	std::vector<shok_ED_IBehavior*, shok_allocator<shok_ED_IBehavior*>> DisplayBehaviors;
	PADDINGI(1); // unk p // 11
	PADDINGI(1); // 0
	PADDINGI(1); // unk p
	PADDINGI(1); // -1, color? // 14

	template<typename T, typename std::enable_if<std::is_base_of<shok_ED_IBehavior, T>::value>::type* = nullptr>
	T* GetDisplayBehavior() {
		for (shok_ED_IBehavior* b : DisplayBehaviors) {
			if (b) {
				T* r = shok_DynamicCast<shok_ED_IBehavior, T>(b);
				if (r)
					return r;
			}
		}
		return nullptr;
	}

	static inline constexpr int vtp = 0x76A494;
};
static_assert(sizeof(shok_ED_CEntity) == 4 * 15);

struct shok_ED_CDisplayStaticEntity : shok_ED_CEntity {
	PADDINGI(1); // p ED::CTerrainDecalAligned // 15
	PADDINGI(1); // 0

	static inline constexpr int vtp = 0x7AE3C4;
};
static_assert(sizeof(shok_ED_CDisplayStaticEntity) == 4 * 17);


// creates shok_ED_CEntity objects as the corresponding entity appears on screen, and deletes them when they go off screen.
// (a entity hidden by FoW counts as offscreen in this case).
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

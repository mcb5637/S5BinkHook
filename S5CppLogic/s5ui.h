#pragma once
#include "s5data.h"

struct shok_guistate_targetingdata { // size 8
	int TargetID;
	shok_position TargetPos;
	shok_positionRot TargetPosWithZ;
	PADDINGI(2);
};
static_assert(sizeof(shok_guistate_targetingdata) == 8 * 4);

struct shok_GGUI_C3DViewHandler;

struct shok_GGUI_SStateParameters : shok_object {

};

struct shok_GGUI_CState : shok_BB_IObject { // no vtable
	shok_GGUI_C3DViewHandler* C3DViewHandler;

	static inline constexpr int TypeDesc = 0x82CC10;
};
struct shok_GGUI_CBasicState : shok_GGUI_CState { // no vtable
	static inline constexpr int TypeDesc = 0x82CC2C;
};
struct shok_GGUI_CCommandState : shok_GGUI_CBasicState { // no vtable
	static inline constexpr int TypeDesc = 0x82CC88;
};

struct shok_GGUI_CPositionCommandState : shok_GGUI_CCommandState { // no vtable
	static inline constexpr int TypeDesc = 0x82CDFC;
};
struct shok_GGUI_CWalkCommandState : shok_GGUI_CPositionCommandState {
	static inline constexpr int vtp = 0x77B300;
	static inline constexpr int TypeDesc = 0x82CE74;
	static inline constexpr unsigned int Identifier = 0xC6BF8157;
};
struct shok_GGUI_CPlaceTorchState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D4CC;
	static inline constexpr int TypeDesc = 0x82E30C;
	static inline constexpr unsigned int Identifier = 0xA55B7457;
};
struct shok_GGUI_CPatrolCommandState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D698;
	static inline constexpr int TypeDesc = 0x82E50C;
	static inline constexpr unsigned int Identifier = 0xC2DE9816;
};
struct shok_GGUI_CPlaceBombCommandState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D700;
	static inline constexpr int TypeDesc = 0x82E5A8;
	static inline constexpr unsigned int Identifier = 0x2AEB8F66;
};
struct shok_GGUI_CAttackMoveCommandState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D7D4;
	static inline constexpr int TypeDesc = 0x82E6A4;
	static inline constexpr unsigned int Identifier = 0x10BEA906;
};
struct shok_GGUI_CScoutBinocularsCommandState : shok_GGUI_CPositionCommandState {
	static inline constexpr int vtp = 0x77CCD8;
	static inline constexpr int TypeDesc = 0x82DC54;
	static inline constexpr unsigned int Identifier = 0x19F674B7;
};
struct shok_GGUI_CSendHawkCommandState : shok_GGUI_CPositionCommandState {
	static inline constexpr int vtp = 0x77CD50;
	static inline constexpr int TypeDesc = 0x82DCD8;
	static inline constexpr unsigned int Identifier = 0xC7454326;
};


struct shok_GGUI_CEntityCommandState : shok_GGUI_CCommandState { // no vtable
	static inline constexpr int TypeDesc = 0x82CCAC;
};
struct shok_GGUI_CStealGoodsState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B00C;
	static inline constexpr int TypeDesc = 0x82CCD4;
	static inline constexpr unsigned int Identifier = 0x112C3B07;
};
struct shok_GGUI_CSecureStolenGoodsState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B058;
	static inline constexpr int TypeDesc = 0x82CCF8;
	static inline constexpr unsigned int Identifier = 0xC455B817;
};
struct shok_GGUI_CDisarmKegCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B0AC;
	static inline constexpr int TypeDesc = 0x82CD24;
	static inline constexpr unsigned int Identifier = 0x7355BF67;
};
struct shok_GGUI_CNPCInteractionState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B0F8;
	static inline constexpr int TypeDesc = 0x82CD50;
	static inline constexpr unsigned int Identifier = 0xE1E52B17;
};
struct shok_GGUI_CAttackCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B140;
	static inline constexpr int TypeDesc = 0x82CD78;
	static inline constexpr unsigned int Identifier = 0x816584DD;
};
struct shok_GGUI_CSerfConstructBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B188;
	static inline constexpr int TypeDesc = 0x82CDA0;
	static inline constexpr unsigned int Identifier = 0x9992B87;
};
struct shok_GGUI_CSerfRepairBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B1E0;
	static inline constexpr int TypeDesc = 0x82CDD0;
	static inline constexpr unsigned int Identifier = 0x69AEF107;
};
struct shok_GGUI_CSerfExtractResourceState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B234;
	static inline constexpr int TypeDesc = 0x82CE28;
	static inline constexpr unsigned int Identifier = 0xE9DAEB87;
};
struct shok_GGUI_CShurikenCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77BF80;
	static inline constexpr int TypeDesc = 0x82D840;
	static inline constexpr unsigned int Identifier = 0xCE36524D;
};
struct shok_GGUI_CSnipeCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77BFE8;
	static inline constexpr int TypeDesc = 0x82D8C0;
	static inline constexpr unsigned int Identifier = 0x7A00C4AD;
};
struct shok_GGUI_CPlaceKegCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D538;
	static inline constexpr int TypeDesc = 0x82E384;
	static inline constexpr unsigned int Identifier = 0xEB1BB807;
};
struct shok_GGUI_CConvertSettlerCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D5A0;
	static inline constexpr int TypeDesc = 0x82E404;
	static inline constexpr unsigned int Identifier = 0x50E03DCD;
};
struct shok_GGUI_CConvertBuildingCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D614;
	static inline constexpr int TypeDesc = 0x82E488;
	static inline constexpr unsigned int Identifier = 0x5E868D4D;
};
struct shok_GGUI_CGuardCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D770;
	static inline constexpr int TypeDesc = 0x82E628;
	static inline constexpr unsigned int Identifier = 0x4ABFC876;
};
struct shok_GGUI_CBlessSettlerState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D844;
	static inline constexpr int TypeDesc = 0x82E724;
	static inline constexpr unsigned int Identifier = 0x6027D046;
};
struct shok_GGUI_CForceWorkerToWorkState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D8F0;
	static inline constexpr int TypeDesc = 0x82E818;
	static inline constexpr unsigned int Identifier = 0xDF71DC86;
};
struct shok_GGUI_CExpelSettlerState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D948;
	static inline constexpr int TypeDesc = 0x82E898;
	static inline constexpr unsigned int Identifier = 0xF8794A47;
};
struct shok_GGUI_CBuySoldierState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D9AC;
	static inline constexpr int TypeDesc = 0x82E914;
	static inline constexpr unsigned int Identifier = 0xB982B848;
};
struct shok_GGUI_CBuyLeaderState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DA08;
	static inline constexpr int TypeDesc = 0x82E98C;
	static inline constexpr unsigned int Identifier = 0x45B1258;
};
struct shok_GGUI_CUpgradeSingleBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DA64;
	static inline constexpr int TypeDesc = 0x82EA04;
	static inline constexpr unsigned int Identifier = 0x39B1467;
};
struct shok_GGUI_CUpgradeBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DAE0;
	static inline constexpr int TypeDesc = 0x82EA88;
	static inline constexpr unsigned int Identifier = 0x95FAD777;
};
struct shok_GGUI_CSellBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DB48;
	static inline constexpr int TypeDesc = 0x82EB08;
	static inline constexpr unsigned int Identifier = 0x8FF51827;
};

struct shok_GGUI_CPlaceCannonState : shok_GGUI_CCommandState {
	static inline constexpr int vtp = 0x77DC0C;
	static inline constexpr int TypeDesc = 0x82EBD8;
	static inline constexpr unsigned int Identifier = 0x8BF90FCD;
};
struct shok_GGUI_CPlaceBuildingState : shok_GGUI_CBasicState {
	int UpgradeCategory;
	shok_position PosToBuild;
	int MouseX, MouseY; // screenpos

	static inline constexpr int vtp = 0x77DCA8;
	static inline constexpr int TypeDesc = 0x82ECC0;
	static inline constexpr unsigned int Identifier = 0x956CDD97;
};
static_assert(sizeof(shok_GGUI_CPlaceBuildingState) / 4 == 7);

struct shok_GGUI_CSelectionState : shok_GGUI_CState {
	static inline constexpr int vtp = 0x77B90C;
	static inline constexpr int TypeDesc = 0x82D03C;
	static inline constexpr unsigned int Identifier = 0xD3F98EE7;
	// 1 extra (empty) func in vtable
};
struct shok_GGUI_CCutsceneState : shok_GGUI_CState {
	static inline constexpr int vtp = 0x77D8A8;
	static inline constexpr int TypeDesc = 0x82E7A0;
	static inline constexpr unsigned int Identifier = 0xD33F4188;
};

struct shok_ERwTools_CRpClumpRenderable : shok_object {

	static inline constexpr int vtp = 0x7AEBF8;

	void SetModelData(const shok_modeldata* modeldata, float rotation);
	void SetPosition(const shok_position& p, float z);
	void SetBuildingRedColor(bool r);
};

struct shok_stateidandcommandstate {
	int Id;
	shok_GGUI_CState* State;
};
struct shok_GGUI_C3DViewHandler : shok_BB_IObject {
	shok_GGUI_CState* CurrentState;
	shok_BB_CIDManager* StateIdManager;
	int* IPicker; // ERwTools::IPicker
	int* IPlacer; // ERwTools::IPlacer
	shok_ERwTools_CRpClumpRenderable* ClumpRenerable;
	PADDINGI(2); // probably pos mouseover
	int MouseX, MouseY;

	void SetGUIStateByIdentifier(unsigned int identifier);
	template<class T>
	void SetGUIState() {
		SetGUIStateByIdentifier(T::Identifier);
	}
	void SetGUIStateByIdentfierOnNextUpdate(unsigned int identifier);
	template<class T>
	void SetGUIStateByIdentfierOnNextUpdate() {
		SetGUIStateByIdentfierOnNextUpdate(T::Identifier);
	}

	static inline constexpr int vtp = 0x77B8E0;
};
//constexpr int i = offsetof(shok_GGUI_C3DViewHandler, MouseX) / 4;
struct shok_EGL_CGLEGUIInterface : shok_object {

	static inline constexpr int vtp = 0x783840;
};
struct shok_GGL_CGLGUIInterface : shok_object {

	bool GetNearestFreePosForBuildingPlacement(int ety, const shok_position& inp, shok_position& outp);

	static inline constexpr int vtp = 0x76D9A4;
};
struct shok_GGUI_CMouseEffect : shok_BB_IPostEvent {

	static inline constexpr int vtp = 0x77BEE0;
};
struct shok_CMouseCursorManager : shok_object { // extends GGUI::IMouseCursorAppearance

	static inline constexpr int vtp = 0x761BBC;
};
struct shok_GGUI_CManager : shok_object { // size 26*4
	PADDINGI(5);
	shok_GGUI_C3DViewHandler* C3DViewHandler; // 6
	shok_EGL_CGLEGUIInterface* EGUIInterface;
	shok_GGL_CGLGUIInterface* GUIInterface; // 8
	shok_EGL_CGLETerrainHiRes* TerrainHiRes;
private:
	shok_BB_IPostEvent* PostEvent;
public:
	vector_padding;
	std::vector<int, shok_allocator<int>> SelectedEntities; //11
	int ControlledPlayer; // 15
	PADDINGI(3);
	vector_padding;
	std::vector<shok_stateidandcommandstate*, shok_allocator<shok_stateidandcommandstate*>> CommandStates;
	shok_GGUI_CMouseEffect* MouseEffect;
	lua_State* GameState;
	shok_CMouseCursorManager* MouseCursorManager;

public:
	static inline constexpr int vtp = 0x77B2F8;

	void HackPostEvent();

	static inline shok_GGUI_CManager* (__cdecl* const GlobalObj)() = reinterpret_cast<shok_GGUI_CManager * (__cdecl*)()>(0x525622);
	static bool(*PostEventCallback)(shok_BB_CEvent* ev);
};
//constexpr int i = offsetof(shok_GGUI_CManager, GameState) / 4;
//constexpr int i = sizeof(shok_GGUI_CManager) / 4;

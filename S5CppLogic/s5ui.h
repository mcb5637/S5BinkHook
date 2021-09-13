#pragma once
#include "s5data.h"

struct shok_GGUI_SStateParameters : shok_object {

};

struct shok_GGUI_CState : shok_object { // no vtable

};
struct shok_GGUI_CBasicState : shok_GGUI_CState { // no vtable

};
struct shok_GGUI_CCommandState : shok_GGUI_CBasicState { // no vtable

};

struct shok_GGUI_CPositionCommandState : shok_GGUI_CCommandState { // no vtable
};
struct shok_GGUI_CWalkCommandState : shok_GGUI_CPositionCommandState {
	static inline constexpr int vtp = 0x77B300;
};
struct shok_GGUI_CPlaceTorchState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D4CC;
};
struct shok_GGUI_CPatrolCommandState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D698;
};
struct shok_GGUI_CPlaceBombCommandState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D700;
};
struct shok_GGUI_CAttackMoveCommandState : shok_GGUI_CWalkCommandState {
	static inline constexpr int vtp = 0x77D7D4;
};
struct shok_GGUI_CScoutBinocularsCommandState : shok_GGUI_CPositionCommandState {
	static inline constexpr int vtp = 0x77CCD8;
};
struct shok_GGUI_CSendHawkCommandState : shok_GGUI_CPositionCommandState {
	static inline constexpr int vtp = 0x77CD50;
};


struct shok_GGUI_CEntityCommandState : shok_GGUI_CCommandState { // no vtable
};
struct shok_GGUI_CStealGoodsState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B8E0;
};
struct shok_GGUI_CSecureStolenGoodsState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B058;
};
struct shok_GGUI_CDisarmKegCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B0AC;
};
struct shok_GGUI_CNPCInteractionState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B0F8;
};
struct shok_GGUI_CAttackCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B140;
};
struct shok_GGUI_CSerfConstructBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B188;
};
struct shok_GGUI_CSerfRepairBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B1E0;
};
struct shok_GGUI_CSerfExtractResourceState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77B234;
};
struct shok_GGUI_CShurikenCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77BF80;
};
struct shok_GGUI_CSnipeCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77BFE8;
};
struct shok_GGUI_CPlaceKegCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D538;
};
struct shok_GGUI_CConvertSettlerCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D5A0;
};
struct shok_GGUI_CConvertBuildingCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D614;
};
struct shok_GGUI_CGuardCommandState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D770;
};
struct shok_GGUI_CBlessSettlerState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D844;
};
struct shok_GGUI_CForceWorkerToWorkState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D8F0;
};
struct shok_GGUI_CExpelSettlerState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D948;
};
struct shok_GGUI_CBuySoldierState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77D9AC;
};
struct shok_GGUI_CBuyLeaderState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DA08;
};
struct shok_GGUI_CUpgradeSingleBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DA64;
};
struct shok_GGUI_CUpgradeBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DAE0;
};
struct shok_GGUI_CSellBuildingState : shok_GGUI_CEntityCommandState {
	static inline constexpr int vtp = 0x77DB48;
};

struct shok_GGUI_CPlaceCannonState : shok_GGUI_CCommandState {
	static inline constexpr int vtp = 0x77DC0C;
};
struct shok_GGUI_CPlaceBuildingState : shok_GGUI_CBasicState {
	static inline constexpr int vtp = 0x77DCA8;
};

struct shok_GGUI_CSelectionState : shok_GGUI_CState {
	static inline constexpr int vtp = 0x77B90C;
	// 1 extra (empty) func in vtable
};
struct shok_GGUI_CCutsceneState : shok_GGUI_CState {
	static inline constexpr int vtp = 0x77D8A8;
};

struct shok_stateidandcommandstate {
	int Id;
	shok_GGUI_CState* State;
};
struct shok_GGUI_C3DViewHandler : shok_object {

	static inline constexpr int vtp = 0x77B8E0;
};
struct shok_EGL_CGLEGUIInterface : shok_object {

	static inline constexpr int vtp = 0x783840;
};
struct shok_GGL_CGLGUIInterface : shok_object {

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
	shok_GGL_CGLGUIInterface* GUIInterface;
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
	PADDINGI(1); // p to unknown
	shok_CMouseCursorManager* MouseCursorManager;


	// last p to CMouseCursorManager (no namespace?), before, p to unknown, p to GGUI::CMouseEffect, vector of p to ?

public:
	static inline constexpr int vtp = 0x77B2F8;

	void HackPostEvent();

	static inline shok_GGUI_CManager* (__cdecl* const GlobalObj)() = reinterpret_cast<shok_GGUI_CManager * (__cdecl*)()>(0x525622);
	static bool(*PostEventCallback)(shok_BB_CEvent* ev);
};
//constexpr int i = offsetof(shok_GGUI_CManager, SelectedEntities) / 4;
//constexpr int i = sizeof(shok_GGUI_CManager) / 4;

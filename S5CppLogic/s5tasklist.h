#pragma once
#include "s5data.h"

struct shok_EGL_CGLETaskArgs : shok_object {
	shok_Task TaskType;

	static inline constexpr int vtp = 0x76E10C;
	static inline constexpr int TypeDesc = 0x8101E8;
};

struct shok_EGL_CGLETaskArgsThousandths : shok_EGL_CGLETaskArgs {
	int Thousandths;

	static inline constexpr int vtp = 0x76E120;
	static inline constexpr int TypeDesc = 0x810208;
};

struct shok_EGL_CGLETaskArgsPosition : shok_EGL_CGLETaskArgs {
	shok_position Position;

	static inline constexpr int vtp = 0x7713F4;
	static inline constexpr int TypeDesc = 0x812DA4;
};

struct shok_EGL_CGLETaskArgsPosAndOrientation : shok_EGL_CGLETaskArgsPosition {
	float Orientation;

	static inline constexpr int vtp = 0x784228;
	static inline constexpr int TypeDesc = 0x8368D4;
};

struct shok_GGL_CGLTaskArgsPositionAndTarget : shok_EGL_CGLETaskArgsPosition {
	//shok_AttachmentType TargetType; probably, unused

	static inline constexpr int vtp = 0x771430;
	static inline constexpr int TypeDesc = 0x812E1C;
};

struct shok_GGL_CGLTaskArgsTargetType : shok_EGL_CGLETaskArgs {
	shok_AttachmentType TargetType;

	static inline constexpr int vtp = 0x77141C;
	static inline constexpr int TypeDesc = 0x812DF4;
};

struct shok_EGL_CGLETaskArgsAnimation : shok_EGL_CGLETaskArgs {
	int AnimID;
	byte PlayBackwards;
	PADDING(3);
	int Category;

	static inline constexpr int vtp = 0x772AE8;
	static inline constexpr int TypeDesc = 0x813A88;
};

struct shok_EGL_CGLETaskArgsSubAnim : shok_EGL_CGLETaskArgsAnimation {
	byte IsLooped;
	PADDING(3);
	int SubAnimIdx;

	static inline constexpr int vtp = 0x78423C;
	static inline constexpr int TypeDesc = 0x836904;
};

struct shok_EGL_CTaskArgsParticleEffectIndex : shok_EGL_CGLETaskArgs {
	int EffectIndex;

	static inline constexpr int vtp = 0x784250;
	static inline constexpr int TypeDesc = 0x83692C;
};

struct shok_EGL_CTaskArgsParticleEffectIndexAndType : shok_EGL_CTaskArgsParticleEffectIndex {
	int EffectType;

	static inline constexpr int vtp = 0x784264;
	static inline constexpr int TypeDesc = 0x83695C;
};

struct shok_EGL_CTaskArgsEffectType : shok_EGL_CGLETaskArgs {
	int EffectType;

	static inline constexpr int vtp = 0x784278;
	static inline constexpr int TypeDesc = 0x836994;
};

struct shok_EGL_CGLETaskArgsTaskListID : shok_EGL_CGLETaskArgs {
	int TaskList;

	static inline constexpr int vtp = 0x78428C;
	static inline constexpr int TypeDesc = 0x8369BC;
};

struct shok_EGL_CTaskArgsRandomWaitForAnim : shok_EGL_CGLETaskArgs {
	int LowerBound, UpperBound; // float?

	static inline constexpr int vtp = 0x7842A0;
	static inline constexpr int TypeDesc = 0x8369E8;
};

struct shok_EGL_CGLETaskArgsSound : shok_EGL_CGLETaskArgs {
	int SoundID;
	float Volume; // int?
	byte Looped;
	PADDING(3);

	static inline constexpr int vtp = 0x7842B4;
	static inline constexpr int TypeDesc = 0x836A18;
};

struct shok_EGL_CGLETaskArgsEntityType : shok_EGL_CGLETaskArgs {
	// ??

	static inline constexpr int vtp = 0x7842C8;
	static inline constexpr int TypeDesc = 0x836A3C;
};

struct shok_EGL_CGLETaskArgsEntityTypeAndPos : shok_EGL_CGLETaskArgsEntityType {
	// ??

	static inline constexpr int vtp = 0x7842DC;
	static inline constexpr int TypeDesc = 0x836A68;
};

struct shok_EGL_CTaskArgsModelID : shok_EGL_CGLETaskArgs {
	int Model;

	static inline constexpr int vtp = 0x7842F0;
	static inline constexpr int TypeDesc = 0x836A98;
};

struct shok_EGL_CTaskArgsUVAnim : shok_EGL_CGLETaskArgs {
	int SlotIndex;
	byte Activate;
	PADDING(3);

	static inline constexpr int vtp = 0x784304;
	static inline constexpr int TypeDesc = 0x836ABC;
};

struct shok_EGL_CTaskArgsInteger : shok_EGL_CGLETaskArgs {
	int Value;

	static inline constexpr int vtp = 0x772AFC;
	static inline constexpr int TypeDesc = 0x813AB0;
};

struct shok_EGL_CTaskArgsFloat : shok_EGL_CGLETaskArgs {
	float Value;

	static inline constexpr int vtp = 0x772B10;
	static inline constexpr int TypeDesc = 0x813AD4;
};

struct shok_GGL_CGLTaskArgsGoodType : shok_EGL_CGLETaskArgs { // unsed

	static inline constexpr int vtp = 0x771408;
	static inline constexpr int TypeDesc = 0x812DCC;
};

struct shok_EGL_CGLETaskList : shok_object {
	vector_padding;
	std::vector<shok_EGL_CGLETaskArgs*> Task;
	int TaskListID;
	int PrincipalTask;
	char* Script;

	int GetNumberOfTasks();
	shok_EGL_CGLETaskArgs* GetTask(int i);

	static inline constexpr int vtp = 0x7845FC;
	static inline constexpr int TypeDesc = 0x836FA8;
};

struct shok_EGL_CGLETaskListMgr : shok_object {
	PADDINGI(3); // pointer to 3 different BB::CIDManagerEx?
	vector_padding;
	std::vector<shok_EGL_CGLETaskList*> TaskLists;

	shok_EGL_CGLETaskList* GetTaskListByID(int id);
	const char* GetTaskListNameByID(int i);

	static inline constexpr int vtp = 0x7845EC;
	static inline constexpr int TypeDesc = 0x836F84;
	static inline shok_EGL_CGLETaskListMgr** const GlobalObj = reinterpret_cast<shok_EGL_CGLETaskListMgr**>(0x895DD8);
};

struct shok_taskData {
	const char* TaskName;
	PADDINGI(1); // no idea what this is, big hex values like 23F8242D, but no pointer/float
	int ID;

	static inline std::vector<shok_taskData, shok_allocator<shok_taskData>>** GlobalVector = reinterpret_cast<std::vector<shok_taskData, shok_allocator<shok_taskData>>**>(0x898238);
};

struct shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int : shok_object {
	shok_EGL_CGLEBehavior* Object;
	int(__thiscall* Func)(shok_EGL_CGLEBehavior* th, shok_EGL_CGLETaskArgs* args);

	int ExecuteTask(shok_EGL_CGLETaskArgs* args);
};


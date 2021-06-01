#pragma once
#include "s5data.h"

struct shok_EGL_CGLETaskArgs : shok_object {
	int TaskType;

	static inline constexpr int vtp = 0x76E10C;
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
};

struct shok_EGL_CGLETaskListMgr : shok_object {
	PADDINGI(3); // pointer to 3 different BB::CIDManagerEx?
	vector_padding;
	std::vector<shok_EGL_CGLETaskList*> TaskLists;

	shok_EGL_CGLETaskList* GetTaskListByID(int id);
	const char* GetTaskListNameByID(int i);

	static inline constexpr int vtp = 0x7845EC;
	static inline shok_EGL_CGLETaskListMgr** const GlobalObj = reinterpret_cast<shok_EGL_CGLETaskListMgr**>(0x895DD8);
};

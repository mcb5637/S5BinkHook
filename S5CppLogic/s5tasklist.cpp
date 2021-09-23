#include "pch.h"
#include "s5data.h"

struct shok_vtable_EGL_IGLEHandler_EGL_CGLETaskArgs_int {
    int(__thiscall* ExecuteTask)(shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int* th, shok_EGL_CGLETaskArgs* args);
};

shok_EGL_CGLETaskArgs::shok_EGL_CGLETaskArgs()
{
    vtable = shok_EGL_CGLETaskArgs::vtp;
}
shok_EGL_CGLETaskArgsAnimation::shok_EGL_CGLETaskArgsAnimation()
{
    vtable = shok_EGL_CGLETaskArgsAnimation::vtp;
}

int shok_EGL_CGLETaskList::GetNumberOfTasks()
{
    return this->Task.size();
}
static inline shok_EGL_CGLETaskArgs* (__thiscall* const shok_EGL_CGLETaskList_gettask)(shok_EGL_CGLETaskList* th, int i) = reinterpret_cast<shok_EGL_CGLETaskArgs * (__thiscall* const)(shok_EGL_CGLETaskList*, int)>(0x57B196);
shok_EGL_CGLETaskArgs* shok_EGL_CGLETaskList::GetTask(int i)
{
    return shok_EGL_CGLETaskList_gettask(this, i);
}

static inline shok_EGL_CGLETaskList* (__thiscall* const shok_CGLETaskListMgr_gettasklistbyid)(shok_EGL_CGLETaskListMgr* th, int id) = reinterpret_cast<shok_EGL_CGLETaskList * (__thiscall*)(shok_EGL_CGLETaskListMgr*, int)>(0x5832BF);
shok_EGL_CGLETaskList* shok_EGL_CGLETaskListMgr::GetTaskListByID(int id)
{
    return shok_CGLETaskListMgr_gettasklistbyid(this, id);
}

static inline const char* (__thiscall* const shok_CGLETaskListMgr_gettasklistnamebyid)(shok_EGL_CGLETaskListMgr* th, int id) = reinterpret_cast<const char* (__thiscall* const)(shok_EGL_CGLETaskListMgr*, int)>(0x583303);
const char* shok_EGL_CGLETaskListMgr::GetTaskListNameByID(int i)
{
    return shok_CGLETaskListMgr_gettasklistnamebyid(this, i);
}

const char* TaskLuaFunc = "TASK_LUA_FUNC";
const char* TaskWaitForAnimNonCancel = "TASK_WAIT_FOR_ANIM_NON_CANCELABLE";
void shok_taskData::AddExtraTasks()
{
    shok_saveVector<shok_taskData>(&(*shok_taskData::GlobalVector)->TaskData, [](std::vector<shok_taskData, shok_allocator<shok_taskData>>& v) {
        v.push_back(shok_taskData{ TaskLuaFunc, 0xB3F8356D, shok_Task::TASK_LUA_FUNC });
        v.push_back(shok_taskData{ TaskWaitForAnimNonCancel, 0x230862d8, shok_Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE });
        });
}
void shok_taskData::RemoveExtraTasks()
{
    shok_saveVector<shok_taskData>(&(*shok_taskData::GlobalVector)->TaskData, [](std::vector<shok_taskData, shok_allocator<shok_taskData>>& v) {
        v.erase(std::remove_if(v.begin(), v.end(), [](shok_taskData& a) {
            return a.TaskName == TaskLuaFunc || a.TaskName == TaskWaitForAnimNonCancel;
            }), v.end());
        });
}

int shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int::ExecuteTask(shok_EGL_CGLETaskArgs* args)
{
    return reinterpret_cast<shok_vtable_EGL_IGLEHandler_EGL_CGLETaskArgs_int*>(vtable)->ExecuteTask(this, args);
}

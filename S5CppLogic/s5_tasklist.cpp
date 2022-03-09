#include "pch.h"
#include "s5_tasklist.h"
#include "s5_idmanager.h"

struct shok_vtable_EGL_IGLEHandler_EGL_CGLETaskArgs_int {
    int(__thiscall* ExecuteTask)(EGL::TaskHandler* th, EGL::CGLETaskArgs* args);
};

void EGL::CGLETaskArgs::SetVT(int vtp)
{
    *reinterpret_cast<int*>(this) = vtp;
}
EGL::CGLETaskArgs::CGLETaskArgs()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgs::GetClassIdentifier() const
{
    return Identifier;
}
bool EGL::CGLETaskArgs::RetZero()
{
    return false;
}

EGL::CGLETaskArgsThousandths::CGLETaskArgsThousandths()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsThousandths::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsPosition::CGLETaskArgsPosition()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsPosition::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsPosAndOrientation::CGLETaskArgsPosAndOrientation()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsPosAndOrientation::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsAnimation::CGLETaskArgsAnimation()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsAnimation::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsSubAnim::CGLETaskArgsSubAnim()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsSubAnim::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsParticleEffectIndex::CTaskArgsParticleEffectIndex()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsParticleEffectIndex::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsParticleEffectIndexAndType::CTaskArgsParticleEffectIndexAndType()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsParticleEffectIndexAndType::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsEffectType::CTaskArgsEffectType()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsEffectType::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsTaskListID::CGLETaskArgsTaskListID()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsTaskListID::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsRandomWaitForAnim::CTaskArgsRandomWaitForAnim()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsRandomWaitForAnim::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsSound::CGLETaskArgsSound()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsSound::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsEntityType::CGLETaskArgsEntityType()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsEntityType::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsEntityTypeAndPos::CGLETaskArgsEntityTypeAndPos()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CGLETaskArgsEntityTypeAndPos::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsModelID::CTaskArgsModelID()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsModelID::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsUVAnim::CTaskArgsUVAnim()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsUVAnim::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsInteger::CTaskArgsInteger()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsInteger::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsFloat::CTaskArgsFloat()
{
    SetVT(vtp);
}
unsigned int __stdcall EGL::CTaskArgsFloat::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CGLTaskArgsPositionAndTarget::CGLTaskArgsPositionAndTarget()
{
    SetVT(vtp);
}
unsigned int __stdcall GGL::CGLTaskArgsPositionAndTarget::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CGLTaskArgsTargetType::CGLTaskArgsTargetType()
{
    SetVT(vtp);
}
unsigned int __stdcall GGL::CGLTaskArgsTargetType::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CGLTaskArgsGoodType::CGLTaskArgsGoodType()
{
    SetVT(vtp);
}
unsigned int __stdcall GGL::CGLTaskArgsGoodType::GetClassIdentifier() const
{
    return Identifier;
}


int EGL::CGLETaskList::GetNumberOfTasks()
{
    return this->Task.size();
}
static inline EGL::CGLETaskArgs* (__thiscall* const shok_EGL_CGLETaskList_gettask)(EGL::CGLETaskList* th, int i) = reinterpret_cast<EGL::CGLETaskArgs * (__thiscall* const)(EGL::CGLETaskList*, int)>(0x57B196);
EGL::CGLETaskArgs* EGL::CGLETaskList::GetTask(int i)
{
    return shok_EGL_CGLETaskList_gettask(this, i);
}

static inline EGL::CGLETaskList* (__thiscall* const shok_CGLETaskListMgr_gettasklistbyid)(EGL::CGLETaskListMgr* th, int id) = reinterpret_cast<EGL::CGLETaskList * (__thiscall*)(EGL::CGLETaskListMgr*, int)>(0x5832BF);
EGL::CGLETaskList* EGL::CGLETaskListMgr::GetTaskListByID(int id)
{
    return shok_CGLETaskListMgr_gettasklistbyid(this, id);
}

static inline const char* (__thiscall* const shok_CGLETaskListMgr_gettasklistnamebyid)(EGL::CGLETaskListMgr* th, int id) = reinterpret_cast<const char* (__thiscall* const)(EGL::CGLETaskListMgr*, int)>(0x583303);
const char* EGL::CGLETaskListMgr::GetTaskListNameByID(int i)
{
    return shok_CGLETaskListMgr_gettasklistnamebyid(this, i);
}

int EGL::CGLETaskListMgr::RegisterTaskList(EGL::CGLETaskList* tl, const char* name)
{
    int tid = TaskListManager->GetIDByNameOrCreate(name);
    if (static_cast<int>(TaskLists.size()) != tid)
        throw std::exception("ids dont match!");
    tl->TaskListID = tid;
    auto v = TaskLists.SaveVector();
    v.Vector.push_back(tl);
    return tid;
}
void EGL::CGLETaskListMgr::RemoveTaskList(int tid)
{
    if (tid) {
        if (static_cast<int>(TaskLists.size()) != tid + 1)
            throw std::exception("ids dont match!");
        EGL::CGLETaskList* tl = GetTaskListByID(tid);
        {
            auto v = tl->Task.SaveVector();
            for (EGL::CGLETaskArgs* p : v.Vector) {
                delete p;
            }
            v.Vector.clear();
        }
        delete tl;
        {
            auto v = TaskLists.SaveVector();
            v.Vector.pop_back();
        }
        TaskListManager->RemoveID(tid);
    }
}


const char* TaskLuaFunc = "TASK_LUA_FUNC";
const char* TaskWaitForAnimNonCancel = "TASK_WAIT_FOR_ANIM_NON_CANCELABLE";
void EGL::TaskData::AddExtraTasks()
{
    auto v = (*EGL::TaskData::GlobalVector)->SaveVector();
    v.Vector.push_back(EGL::TaskData{ TaskLuaFunc, 0xB3F8356D, shok::Task::TASK_LUA_FUNC });
    v.Vector.push_back(EGL::TaskData{ TaskWaitForAnimNonCancel, 0x230862d8, shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE });
}
void EGL::TaskData::RemoveExtraTasks()
{
    auto v = (*EGL::TaskData::GlobalVector)->SaveVector();
    v.Vector.erase(std::remove_if(v.Vector.begin(), v.Vector.end(), [](EGL::TaskData& a) {
        return a.TaskName == TaskLuaFunc || a.TaskName == TaskWaitForAnimNonCancel;
        }), v.Vector.end());
}

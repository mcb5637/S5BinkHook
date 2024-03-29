#include "pch.h"
#include "s5_tasklist.h"
#include "s5_idmanager.h"
#include "s5_classfactory.h"
#include "s5_filesystem.h"

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
shok::ClassId __stdcall EGL::CGLETaskArgs::GetClassIdentifier() const
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
shok::ClassId __stdcall EGL::CGLETaskArgsThousandths::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsPosition::CGLETaskArgsPosition()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsPosition::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsPosAndOrientation::CGLETaskArgsPosAndOrientation()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsPosAndOrientation::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsAnimation::CGLETaskArgsAnimation()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsAnimation::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsSubAnim::CGLETaskArgsSubAnim()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsSubAnim::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsParticleEffectIndex::CTaskArgsParticleEffectIndex()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsParticleEffectIndex::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsParticleEffectIndexAndType::CTaskArgsParticleEffectIndexAndType()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsParticleEffectIndexAndType::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsEffectType::CTaskArgsEffectType()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsEffectType::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsTaskListID::CGLETaskArgsTaskListID()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsTaskListID::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsRandomWaitForAnim::CTaskArgsRandomWaitForAnim()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsRandomWaitForAnim::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsSound::CGLETaskArgsSound()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsSound::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsEntityType::CGLETaskArgsEntityType()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsEntityType::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CGLETaskArgsEntityTypeAndPos::CGLETaskArgsEntityTypeAndPos()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CGLETaskArgsEntityTypeAndPos::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsModelID::CTaskArgsModelID()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsModelID::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsUVAnim::CTaskArgsUVAnim()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsUVAnim::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsInteger::CTaskArgsInteger()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsInteger::GetClassIdentifier() const
{
    return Identifier;
}

EGL::CTaskArgsFloat::CTaskArgsFloat()
{
    SetVT(vtp);
}
shok::ClassId __stdcall EGL::CTaskArgsFloat::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CGLTaskArgsPositionAndTarget::CGLTaskArgsPositionAndTarget()
{
    SetVT(vtp);
}
shok::ClassId __stdcall GGL::CGLTaskArgsPositionAndTarget::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CGLTaskArgsTargetType::CGLTaskArgsTargetType()
{
    SetVT(vtp);
}
shok::ClassId __stdcall GGL::CGLTaskArgsTargetType::GetClassIdentifier() const
{
    return Identifier;
}

GGL::CGLTaskArgsGoodType::CGLTaskArgsGoodType()
{
    SetVT(vtp);
}
shok::ClassId __stdcall GGL::CGLTaskArgsGoodType::GetClassIdentifier() const
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

static inline EGL::CGLETaskList* (__thiscall* const shok_CGLETaskListMgr_gettasklistbyid)(EGL::CGLETaskListMgr* th, shok::TaskListId id) = reinterpret_cast<EGL::CGLETaskList * (__thiscall*)(EGL::CGLETaskListMgr*, shok::TaskListId)>(0x5832BF);
EGL::CGLETaskList* EGL::CGLETaskListMgr::GetTaskListByID(shok::TaskListId id)
{
    return shok_CGLETaskListMgr_gettasklistbyid(this, id);
}

static inline const char* (__thiscall* const shok_CGLETaskListMgr_gettasklistnamebyid)(EGL::CGLETaskListMgr* th, shok::TaskListId id) = reinterpret_cast<const char* (__thiscall* const)(EGL::CGLETaskListMgr*, shok::TaskListId)>(0x583303);
const char* EGL::CGLETaskListMgr::GetTaskListNameByID(shok::TaskListId i)
{
    return shok_CGLETaskListMgr_gettasklistnamebyid(this, i);
}

shok::TaskListId EGL::CGLETaskListMgr::RegisterTaskList(EGL::CGLETaskList* tl, const char* name)
{
    int tid = TaskListManager->GetIDByNameOrCreate(name);
    if (static_cast<int>(TaskLists.size()) != tid)
        throw std::exception("ids dont match!");
    tl->TaskListID = static_cast<shok::TaskListId>(tid);
    auto v = TaskLists.SaveVector();
    v.Vector.push_back(tl);
    return static_cast<shok::TaskListId>(tid);
}
void EGL::CGLETaskListMgr::RemoveTaskList(shok::TaskListId tid)
{
    int id = static_cast<int>(tid);
    if (id) {
        if (static_cast<int>(TaskLists.size()) != id + 1)
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
        TaskListManager->RemoveID(id);
    }
}

void EGL::CGLETaskListMgr::FreeTaskList(shok::TaskListId id)
{
    EGL::CGLETaskList* tl = GetTaskListByID(id);
    {
        auto v = tl->Task.SaveVector();
        for (EGL::CGLETaskArgs* p : v.Vector) {
            delete p;
        }
        v.Vector.clear();
    }
    delete tl;
    TaskLists[static_cast<int>(id)] = nullptr;
}

void EGL::CGLETaskListMgr::LoadTaskList(shok::TaskListId id)
{
    int i = static_cast<int>(id);
    const char* n = TaskListManager->GetNameByID(i);
    if (!n)
        throw std::logic_error{ "invalid tasklist id" };
    std::string filename = "Data\\Config\\TaskLists\\";
    filename.append(n);
    filename.append(".xml");
    EGL::CGLETaskList* tl = (*BB::CClassFactory::GlobalObj)->LoadObject<EGL::CGLETaskList>(filename.c_str());
    tl->TaskListID = id;
    if (i == static_cast<int>(TaskLists.size())) {
        auto v = TaskLists.SaveVector();
        v.Vector.push_back(tl);
    }
    else {
        TaskLists[i] = tl;
    }
}


const char* TaskLuaFunc = "TASK_LUA_FUNC";
const char* TaskWaitForAnimNonCancel = "TASK_WAIT_FOR_ANIM_NON_CANCELABLE";
const char* TaskSkipSupplier = "TASK_SKIP_SUPPLIER_IF_RESEARCHING";
const char* TaskResetCarriedRes = "TASK_REFINER_RESET_CARRIED_RESOURES";
const char* TaskRefinerCheck = "TASK_REFINER_CHECK_NEEDS_RESOURCES";
void EGL::TaskData::AddExtraTasks()
{
    auto v = (*EGL::TaskData::GlobalVector)->SaveVector();
    v.Vector.push_back(EGL::TaskData{ TaskLuaFunc, EGL::CTaskArgsInteger::Identifier, shok::Task::TASK_LUA_FUNC });
    v.Vector.push_back(EGL::TaskData{ TaskWaitForAnimNonCancel, EGL::CGLETaskArgsThousandths::Identifier, shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE });
    v.Vector.push_back(EGL::TaskData{ TaskSkipSupplier, EGL::CTaskArgsInteger::Identifier, shok::Task::TASK_SKIP_SUPPLIER_IF_RESEARCHING });
    v.Vector.push_back(EGL::TaskData{ TaskResetCarriedRes, EGL::CTaskArgsInteger::Identifier, shok::Task::TASK_REFINER_RESET_CARRIED_RESOURES });
    v.Vector.push_back(EGL::TaskData{ TaskRefinerCheck, EGL::CTaskArgsInteger::Identifier, shok::Task::TASK_REFINER_CHECK_NEEDS_RESOURCES });
}
void EGL::TaskData::RemoveExtraTasks()
{
    auto v = (*EGL::TaskData::GlobalVector)->SaveVector();
    v.Vector.erase(std::remove_if(v.Vector.begin(), v.Vector.end(), [](EGL::TaskData& a) {
        return a.TaskName == TaskLuaFunc || a.TaskName == TaskWaitForAnimNonCancel || a.TaskName == TaskSkipSupplier || a.TaskName == TaskResetCarriedRes
            || a.TaskName == TaskRefinerCheck;
        }), v.Vector.end());
}


std::map<shok::TaskState, std::string_view> shok::TaskStateToName{
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Default, "Default"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::WaitForAnim, "WaitForAnim"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Move, "Move"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Follow, "Follow"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Wait, "Wait"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Rotate, "Rotate"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::SetNextTaskList, "SetNextTaskList"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Move_NonCancelable, "Move_NonCancelable"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::IdleInFormation, "IdleInFormation"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::AssumePositionInFormation, "AssumePositionInFormation"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::SerfSearchResource, "SerfSearchResource"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Train, "Train"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::HeroComatose, "HeroComatose"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::BuildingAlarmDefend, "BuildingAlarmDefend"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::BattleWait, "BattleWait"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::LeaderGetCloseTotarget, "LeaderGetCloseTotarget"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::MoveToTarget, "MoveToTarget"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::HeroGoToNPC, "HeroGoToNPC"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::ConvertBuilding, "ConvertBuilding"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::ConvertSettler, "ConvertSettler"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::Merchant_WaitForHero, "Merchant_WaitForHero"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::ThiefStealGoods, "ThiefStealGoods"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::ThiefSabotage, "ThiefSabotage"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::ThiefDisarm, "ThiefDisarm"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::ScoutPointToRes, "ScoutPointToRes"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::LuaFunc, "LuaFunc"},
    std::pair<const shok::TaskState, std::string_view>{shok::TaskState::WaitForAnimNonCancelable, "WaitForAnimNonCancelable"},
};
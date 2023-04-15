#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"

namespace EGL {
    class CGLETaskArgs : public BB::IObject {
    public:
        shok::Task TaskType = static_cast<shok::Task>(0);

        CGLETaskArgs();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

    protected:
        virtual bool RetZero(); // on CGLETaskArgsSubAnim returns anim==0
        void SetVT(int vtp);

    public:
        static inline constexpr int vtp = 0x76E10C;
        static inline constexpr int TypeDesc = 0x8101E8;
        static inline constexpr unsigned int Identifier = 0x7CE66308;
    };

    class CGLETaskArgsThousandths : public EGL::CGLETaskArgs {
    public:
        int Thousandths = 0;

        CGLETaskArgsThousandths();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x76E120;
        static inline constexpr int TypeDesc = 0x810208;
        static inline constexpr unsigned int Identifier = 0x230862D8;
    };

    class CGLETaskArgsPosition : public EGL::CGLETaskArgs {
    public:
        shok::Position Position{};

        CGLETaskArgsPosition();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x7713F4;
        static inline constexpr int TypeDesc = 0x812DA4;
        static inline constexpr unsigned int Identifier = 0x5CC25F38;
    };

    class CGLETaskArgsPosAndOrientation : public EGL::CGLETaskArgsPosition {
    public:
        float Orientation = 0;

        CGLETaskArgsPosAndOrientation();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x784228;
        static inline constexpr int TypeDesc = 0x8368D4;
        static inline constexpr unsigned int Identifier = 0x2F706FDD;
    };

    class CGLETaskArgsAnimation : public EGL::CGLETaskArgs {
    public:
        int AnimID = 0;
        bool PlayBackwards = false;
        PADDING(3);
        int Category = 0;

        CGLETaskArgsAnimation();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x772AE8;
        static inline constexpr int TypeDesc = 0x813A88;
        static inline constexpr unsigned int Identifier = 0x4D90ECB8;
    };

    class CGLETaskArgsSubAnim : public EGL::CGLETaskArgsAnimation {
    public:
        bool IsLooped = false;
        PADDING(3);
        int SubAnimIdx = 0;

        CGLETaskArgsSubAnim();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x78423C;
        static inline constexpr int TypeDesc = 0x836904;
        static inline constexpr unsigned int Identifier = 0x23F8242D;
    };

    class CTaskArgsParticleEffectIndex : public EGL::CGLETaskArgs {
    public:
        int EffectIndex = 0;

        CTaskArgsParticleEffectIndex();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x784250;
        static inline constexpr int TypeDesc = 0x83692C;
        static inline constexpr unsigned int Identifier = 0xBFC2EBB7;
    };

    class CTaskArgsParticleEffectIndexAndType : public EGL::CTaskArgsParticleEffectIndex {
    public:
        int EffectType = 0;

        CTaskArgsParticleEffectIndexAndType();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x784264;
        static inline constexpr int TypeDesc = 0x83695C;
        static inline constexpr unsigned int Identifier = 0xF92B2027;
    };

    class CTaskArgsEffectType : public EGL::CGLETaskArgs {
    public:
        int EffectType = 0;

        CTaskArgsEffectType();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x784278;
        static inline constexpr int TypeDesc = 0x836994;
        static inline constexpr unsigned int Identifier = 0x442AFA07;
    };

    class CGLETaskArgsTaskListID : public EGL::CGLETaskArgs {
    public:
        int TaskList = 0;

        CGLETaskArgsTaskListID();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x78428C;
        static inline constexpr int TypeDesc = 0x8369BC;
        static inline constexpr unsigned int Identifier = 0x6385EA0D;
    };

    class CTaskArgsRandomWaitForAnim : public EGL::CGLETaskArgs {
    public:
        int LowerBound = 0, UpperBound = 0;

        CTaskArgsRandomWaitForAnim();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x7842A0;
        static inline constexpr int TypeDesc = 0x8369E8;
        static inline constexpr unsigned int Identifier = 0xEAA8FC97;
    };

    class CGLETaskArgsSound : public EGL::CGLETaskArgs {
    public:
        int SoundID = 0;
        int Volume = 0;
        bool Looped = false;
        PADDING(3);

        CGLETaskArgsSound();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x7842B4;
        static inline constexpr int TypeDesc = 0x836A18;
        static inline constexpr unsigned int Identifier = 0x2CFEDFCD;
    };

    class CGLETaskArgsEntityType : public EGL::CGLETaskArgs {
    public:
        int EntityType = 0;

        CGLETaskArgsEntityType();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x7842C8;
        static inline constexpr int TypeDesc = 0x836A3C;
        static inline constexpr unsigned int Identifier = 0x999044AD;
    };

    class CGLETaskArgsEntityTypeAndPos : public EGL::CGLETaskArgsEntityType {
    public:
        shok::Position Position{};

        CGLETaskArgsEntityTypeAndPos();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x7842DC;
        static inline constexpr int TypeDesc = 0x836A68;
        static inline constexpr unsigned int Identifier = 0x4C44439D;
    };

    class CTaskArgsModelID : public EGL::CGLETaskArgs {
    public:
        int Model = 0;

        CTaskArgsModelID();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x7842F0;
        static inline constexpr int TypeDesc = 0x836A98;
        static inline constexpr unsigned int Identifier = 0x3D17C8CD;
    };

    class CTaskArgsUVAnim : public EGL::CGLETaskArgs {
    public:
        int SlotIndex = 0;
        bool Activate = false;
        PADDING(3);

        CTaskArgsUVAnim();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x784304;
        static inline constexpr int TypeDesc = 0x836ABC;
        static inline constexpr unsigned int Identifier = 0x3631D59D;
    };

    class CTaskArgsInteger : public EGL::CGLETaskArgs {
    public:
        int Value = 0;

        CTaskArgsInteger();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x772AFC;
        static inline constexpr int TypeDesc = 0x813AB0;
        static inline constexpr unsigned int Identifier = 0xB3F8356D;
    };

    class CTaskArgsFloat : public EGL::CGLETaskArgs {
    public:
        float Value = 0;

        CTaskArgsFloat();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x772B10;
        static inline constexpr int TypeDesc = 0x813AD4;
        static inline constexpr unsigned int Identifier = 0xA2787EED;
    };
}

namespace GGL {
    class CGLTaskArgsPositionAndTarget : public EGL::CGLETaskArgsPosition {
    public:
        //shok::AttachmentType TargetType; probably, unused

        CGLTaskArgsPositionAndTarget();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x771430;
        static inline constexpr int TypeDesc = 0x812E1C;
        static inline constexpr unsigned int Identifier = 0xC9A11128;
    };

    class CGLTaskArgsTargetType : public EGL::CGLETaskArgs {
    public:
        shok::AttachmentType TargetType = shok::AttachmentType::APPROACHING_BUILDER_BRIDGE;

        CGLTaskArgsTargetType();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x77141C;
        static inline constexpr int TypeDesc = 0x812DF4;
        static inline constexpr unsigned int Identifier = 0x9B0B7F68;
    };

    class CGLTaskArgsGoodType : public EGL::CGLETaskArgs { // unused
    public:

        CGLTaskArgsGoodType();
        virtual unsigned int __stdcall GetClassIdentifier() const override;

        static inline constexpr int vtp = 0x771408;
        static inline constexpr int TypeDesc = 0x812DCC;
        static inline constexpr unsigned int Identifier = 0x1E3B7668;
    };
}

namespace EGL {
    enum class PrincipalTasks : int {
        Work = 1,
        Idle = 2,
        Eat = 3,
        Rest = 4,
    };

    class CGLETaskList : public BB::IObject {
    public:
        shok::Vector<EGL::CGLETaskArgs*> Task;
        int TaskListID;
        PrincipalTasks PrincipalTask;
        char* Script;

        int GetNumberOfTasks();
        EGL::CGLETaskArgs* GetTask(int i);

        static inline constexpr int vtp = 0x7845FC;
        static inline constexpr int TypeDesc = 0x836FA8;
        static inline constexpr unsigned int Identifier = 0xA7170A28;
    };
    static_assert(sizeof(EGL::CGLETaskList) == 8 * 4);

    class CGLETaskListMgr : public BB::IObject { // tasklist objects get reloaded every time, idmanager not
    public:
        BB::CIDManagerEx* TaskListManager;
        BB::CIDManagerEx* TaskManager;
        BB::CIDManagerEx* WorkCycleManager;
        shok::Vector<EGL::CGLETaskList*> TaskLists;

        EGL::CGLETaskList* GetTaskListByID(int id);
        const char* GetTaskListNameByID(int i);
        int RegisterTaskList(EGL::CGLETaskList* tl, const char* name);
        void RemoveTaskList(int tid); // has to be the last tasklist. do not use if anything still uses that tasklist (only intended for cleanup)
        void FreeTaskList(int id);
        void LoadTaskList(int id);

        static inline constexpr int vtp = 0x7845EC;
        static inline constexpr int TypeDesc = 0x836F84;
        static inline EGL::CGLETaskListMgr** const GlobalObj = reinterpret_cast<EGL::CGLETaskListMgr**>(0x895DD8);
    };

    struct TaskData {
        const char* TaskName;
        unsigned int TaskArgsClassID;
        shok::Task ID;

        static inline shok::Vector<TaskData>** GlobalVector = reinterpret_cast<shok::Vector<TaskData>**>(0x898238);

        static void AddExtraTasks();
        static void RemoveExtraTasks();
    };
}

namespace shok {
    enum class Task : int {
        TASK_SET_POS = 1, //EGL::CGLETaskArgsPosAndOrientation (GGL::CSettler)
        //EGL::CGLETaskArgsPosition (GGL::CWorkerBehavior pos relative to workplace,
        //  GGL::CSoldierBehavior+GGL::CeaderBehavior pos relative to barracks, GGL::CSettlerMerchantBehavior pos relative to merch building,
        //  GGL::CHawkBehavior pos relative to hero)
        //  ignores blocking
        TASK_GO_TO_POS = 2,
        TASK_SET_ANIM = 3, //EGL::CGLETaskArgsAnimation (GGL::CGLBehaviorAnimationEx, EGL::CBehaviorAnimation)
        TASK_SET_SUB_ANIM = 4, //EGL::CGLETaskArgsSubAnim (EGL::GLEBehaviorMultiSubAnims)
        TASK_WAIT_FOR_ANIM = 5, //EGL::CGLETaskArgsThousands (EGL::CBehaviorAnimation) wait to part of anims duration 1000->1, always timed from start of animation.
        TASK_RANDOM_WAIT_FOR_ANIM = 6, //EGL::CTaskArgsRandomWaitForAnim (EGL::CBehaviorAnimation)
        TASK_WAIT = 7, //EGL::CGLETaskArgsThousandths (GGL::CSettler) wait predefined amount of time
        TASK_DELETE_SELF = 8, //EGL::CGLETaskArgs (GGL::CGLBehaviorDying)
        TASK_PLAY_SOUND = 9, //EGL::CGLETaskArgsSound (EGL::CGLEEntity)
        TASK_SET_TASK_LIST = 10, //EGL::CGLETaskArgsTaskListID (GGL::CSettler, GGL::CAnimal)
        TASK_STOP_SOUND = 11, //EGL::CGLETaskArgsSound (EGL::CGLEEntity)
        TASK_RESET_TASK_LIST_TIMER = 12, //EGL::CGLETaskArgs (EGL::CGLEEntity)
        TASK_TURN_TO_TARGET_ORIENTATION = 13, //EGL::CGLETaskArgs (EGL::CMovementBehavior)
        TASK_SET_MODEL = 14, //EGL::CTaskArgsModelID (EGL::CGLEEntity)
        TASK_SET_TARGET_ORIENTATION_RELATIVE = 15, //EGL::CTaskArgsFloat (EGL::CMovementBehavior)
        TASK_ACTIVATE_PARTICLE_EFFECT = 16, //EGL::CTaskArgsParticleEffectIndex (GGL::CWorkerBehavior)
        TASK_DEACTIVATE_PARTICLE_EFFECT = 17, //EGL::CTaskArgsParticleEffectIndex (GGL::CWorkerBehavior)
        TASK_SPAWN_PARTICLE_EFFECT = 18, //EGL::CTaskArgsParticleEffectIndexAndType (EGL::CParticleEffectAttachmentBehavior)
        TASK_ACTIVATE_UVANIM = 19, //EGL::CTaskArgsUVAnim (GGL::CWorkerBehavior, GGL::CLeaderBehavior)
        TASK_SET_ANIM_AT_BUILDING = 20,
        TASK_VANISH = 21, //EGL::CGLETaskArgs (GGL::CSettler)
        TASK_APPEAR = 22, //EGL::CGLETaskArgs (GGL::CSettler)
        TASK_LIST_DONE = 23, //EGL::CGLETaskArgs (GGL::CSettler)
        TASK_CREATE_OBJ = 24,
        TASK_DELETE_OBJ = 25,
        TASK_ENTER_BUILDING = 26, //GGL::CGLTaskArgsTargetType (GGL::CSettler)
        TASK_LEAVE_BUILDING = 27, //EGL::CGLETaskArgs (GGL::CSettler)
        TASK_GO_TO_MAIN_HOUSE = 28, //EGL::CGLETaskArgs (GGL::CSerfBehavior, GGL::CBattleSerfBehavior)
        TASK_CHANGE_TYPE = 29,
        TASK_ATTACH_TOOL = 30,
        TASK_PUT_ON_PILE = 31,
        TASK_GET_FROM_PILE = 32,
        TASK_GO_TO_ENTITY = 33,
        TASK_GO_TO_WORK_AREA = 34,
        TASK_GO_TO_BLOCKED_PILE = 35,
        TASK_GO_TO_PILE = 36,
        TASK_GO_TO_PILE_COMMAND_POS = 37,
        TASK_CREATE_PILE_AND_PUT_GOOD_TO_IT = 38,
        TASK_ATTACH_GOOD = 39,
        TASK_DETACH_GOOD = 40,
        TASK_DECREASE_RESOURCE = 41,
        TASK_SEARCH_FOR_RESOURCE = 42,
        TASK_SEARCH_FOR_PILE = 43,
        TASK_WALK = 44, //EGL::CGLETaskArgs (GGL::CBehaviorWalkCommand)
        TASK_DECREASE_COLLECTED_RESOURCE = 45,
        TASK_DESTROY = 46,
        TASK_GET_FROM_PILE_OF_TYPE = 47,
        TASK_CHECK_START_HARVESTING = 48,
        TASK_START_FARMING = 49,
        TASK_SOW = 50,

        TASK_PERFORM_ATTACK = 51,
        TASK_CREATE_ENTITY = 52, //EGL::CGLETaskArgsEntityTypeAndPos (GGL::CGLBehaviorDying)
        TASK_REPAIR = 53,
        TASK_RETURN_TO_EMPLOYER = 54,
        TASK_GO_TO_POS_EX = 55,
        TASK_WANDER = 56, //EGL::CGLETaskArgs (GGL::CAnimal)
        TASK_BACK_TO_DEFAULT_TASKLIST = 57, //EGL::CGLETaskArgs (GGL::CAnimal)
        TASK_WAIT_UNTIL = 58, //EGL::CGLETaskArgsThousandths (GGL::CSettler) wait for definedTime + 100*(entity.TaskListStart - currentTick) (time since TASK_RESET_TASK_LIST_TIMER)
        TASK_GO_TO_CAMP = 59, //EGL::CGLETaskArgs (GGL::CCamperBehavior)
        TASK_LEAVE_CAMP = 60, //EGL::CGLETaskArgs (GGL::CCamperBehavior)
        TASK_LEAVE_SETTLEMENT = 61, //EGL::CGLETaskArgs (GGL::CWorkerBehavior, GGL::CSerfBehavior)
        TASK_ADVANCE_IN_CYCLE = 62, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_GOLD = 63, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_GOLDRAW = 64, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_SILVER = 65, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_SILVERRAW = 66, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_STONE = 67, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_STONERAW = 68, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_IRON = 69, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_IRONRAW = 70, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_SULFUR = 71, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_SULFURRAW = 72, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_CLAY = 73, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_CLAYRAW = 74, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_WOOD = 75, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_WOODRAW = 76, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_WEATHERENERGY = 77, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_KNOWLEDGE = 78, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_INCREASE_PLAYER_FAITH = 79, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_CONSUME_RESOURCE = 80,
        TASK_MINED_RESOURCE = 81, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_GO_TO_WORK_BUILDING = 82, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_GO_TO_EAT_BUILDING = 83,
        TASK_GO_TO_REST_BUILDING = 84, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_GO_TO_LEAVE_BUILDING = 85, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_NEW_MOTIVATION_MODIFIER = 86,
        TASK_CHECK_MOTIVATION = 87,
        TASK_WORK_WAIT_UNTIL = 88, //EGL::CGLETaskArgs (GGL::CWorkerBehavior) TASK_WAIT_UNTIL with CWorkerBehaviorProps->WorkWaitUntil
        TASK_EAT_WAIT = 89, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_REST_WAIT = 90, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_GO_TO_CONSTRUCTION_SITE = 91, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_TURN_TO_CONSTRUCTION_SITE = 92, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_CHANGE_ATTACHMENT_TO_CONSTRUCTION_SITE = 93, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_HURT = 94, //EGL::CGLETaskArgs (GGL::CBattleBehavior)
        TASK_MOVE_TO_TARGET = 95, //EGL::CGLETaskArgs (GGL::CBattleBehavior)
        TASK_SET_ORIENTATION_TO_TARGET = 96, //EGL::CGLETaskArgs (GGL::CBattleBehavior)
        TASK_SET_ATTACK_ANIM = 97, //EGL::CGLETaskArgs (GGL::CBattleBehavior)
        TASK_SET_BATTLE_IDLE_ANIM = 98, //EGL::CGLETaskArgs (GGL::CBattleBehavior)
        TASK_ASSUME_POSITION_IN_FORMATION = 99, //EGL::CGLETaskArgs (GGL::CFormationBehavior)

        TASK_IDLE_IN_FORMATION = 100, //EGL::CGLETaskArgs (GGL::CFormationBehavior)
        TASK_FIND_RESEARCH_BUILDING = 101,
        TASK_GO_TO_RESEARCH_BUILDING = 102,
        TASK_DO_RESEARCH = 103,
        TASK_CHECK_RANGE = 104, //EGL::CGLETaskArgs (GGL::CBattleBehavior, GGL::CAutoCannonBehavior)
        TASK_ABANDON_CURRENT_JOB = 105, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_GO_TO_RESOURCE = 106, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_TURN_TO_RESOURCE = 107, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_WAIT_EXTRACTION_DELAY = 108, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_EXTRACT_RESOURCE = 109, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_FIRE_PROJECTILE = 110, //EGL::CGLETaskArgs (GGL::CBattleBehavior, GGL::CAutoCannonBehavior)
        TASK_REFINE_RESOURCE = 111, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_SET_DEFAULT_REACTION_TYPE = 112, //EGL::CGLETaskArgs (GGL::CWorkerBattleBehavior, GGL::CSerfBattleBehavior, GGL::CLeaderBehavior)
        TASK_DO_TRADE_STEP = 113, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_DO_RESEARCH_STEP = 114, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_PLACE_BOMB = 115, //EGL::CGLETaskArgs (GGL::CBombPlacerBehavior)
        TASK_GO_TO_BOMB_TARGET = 116, //EGL::CGLETaskArgs (GGL::CBombPlacerBehavior)
        TASK_RETREAT_FROM_BOMB_TARGET = 117, //EGL::CGLETaskArgs (GGL::CBombPlacerBehavior)
        TASK_LEAVE_BARRACKS = 118, //EGL::CGLETaskArgs (GGL::CSoldierBehavior, GGL::CLeaderBehavior)
        TASK_RESOLVE_COLLISION = 119, //EGL::CGLETaskArgs (GGL::CSettler)
        TASK_CHANGE_WORK_TIME_ABSOLUTE = 120, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_CHANGE_WORK_TIME_RELATIVE = 121, //EGL::CTaskArgsFloat (GGL::CWorkerBehavior)
        TASK_MOVE_TO_EXPLORATION_POINT = 122, //EGL::CGLETaskArgs (GGL::CHawkBehavior)
        TASK_CHECK_HERO_POSITION = 123, //EGL::CGLETaskArgs (GGL::CHawkBehavior)
        TASK_SET_ANIM_AT_WORKPLACE = 124, //EGL::CGLETaskArgsSubAnim (GGL::CWorkerBehavior)
        TASK_SET_ANIM_AT_FARM = 125, //EGL::CGLETaskArgsSubAnim (GGL::CWorkerBehavior)
        TASK_SET_ANIM_AT_RESIDENCE = 126, //EGL::CGLETaskArgsSubAnim (GGL::CWorkerBehavior)
        TASK_SET_ANIM_AT_BARRACKS = 127, //EGL::CGLETaskArgsSubAnim (GGL::CLeaderBehavior)
        TASK_SET_ANIM_AT_VILLAGE_CENTER = 128, //EGL::CGLETaskArgsSubAnim (GGL::CWorkerBehavior)
        TASK_BATTLE_WAIT_UNTIL = 129, //EGL::CGLETaskArgs (GGL::CBattleBehavior, GGL::CAutoCannonBehavior) waits BattleWaitUntil from time since TASK_RESET_TASK_LIST_TIMER
        TASK_TRAIN = 130, //EGL::CGLETaskArgs (GGL::CLeaderBehavior)
        TASK_TURN_INTO_BATTLE_SERF = 131, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_TURN_INTO_SERF = 132, //EGL::CGLETaskArgs (GGL::CBattleSerfBehavior)
        TASK_BECOME_COMATOSE = 133, //EGL::CGLETaskArgs (GGL::CHeroBehavior)
        TASK_GO_TO_CANNON_POSITION = 134, //EGL::CGLETaskArgs (GGL::CCannonBuilderBehavior)
        TASK_BUILD_CANNON = 135, //EGL::CGLETaskArgs (GGL::CCannonBuilderBehavior)
        TASK_GO_TO_RESOURCE_SLOT = 136, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_CHANGE_DEFENDABLE_BUILDING_ATTACHMENT = 137, //EGL::CGLETaskArgs (GGL::CWorkerAlarmModeBehavior)
        TASK_MOVE_TO_DEFENDABLE_BUILDING = 138, //EGL::CGLETaskArgs (GGL::CWorkerAlarmModeBehavior)
        TASK_DEFEND = 139, //EGL::CGLETaskArgs (GGL::CWorkerAlarmModeBehavior)
        TASK_GO_TO_CONSTRUCTION_SITE_SLOT = 140, //EGL::CGLETaskArgs (GGL::CSerfBehavior)
        TASK_FLEE = 141, //EGL::CGLETaskArgs (GGL::CWorkerFleeBehavior, GGL::CAnimal)
        TASK_RETURN_TO_CYCLE = 142, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHECK_FEAR = 143, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHANGE_WORK_TIME_WORK = 144, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHANGE_WORK_TIME_FARM = 145, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHANGE_WORK_TIME_RESIDENCE = 146, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHANGE_WORK_TIME_CAMP = 147, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_GET_CLOSE_TO_TARGET = 148, //EGL::CGLETaskArgs (GGL::CLeaderBehavior)
        TASK_ATTACK = 149, //EGL::CGLETaskArgs (GGL::CLeaderBehavior)

        TASK_GO_TO_SUPPLIER = 150, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_DO_WORK_AT_FOUNDRY = 151, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CREATE_CANNON = 152, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_SET_CANNON_PROGRESS = 153, //EGL::CTaskArgsInteger (GGL::CWorkerBehavior)
        TASK_CHECK_GO_TO_REST_BUILDING_SUCCESS = 154, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHECK_GO_TO_EAT_BUILDING_SUCCESS = 155, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHECK_GO_TO_WORK_BUILDING_SUCCESS = 156, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHECK_GO_TO_VILLAGE_CENTER_SUCCESS = 157, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHECK_GO_TO_DEFENDABLE_BUILDING_SUCCESS = 158, //EGL::CGLETaskArgs (GGL::CWorkerAlarmModeBehavior)
        TASK_TAKE_FROM_STOCK = 159,
        TASK_SET_CARRIER_MODEL = 160, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHECK_GO_TO_SUPPLIER_SUCCESS = 161, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_LEFT_BUILDING = 162, //EGL::CGLETaskArgs (GGL::CSettler)
        TASK_GO_TO_FREE_POSITION = 163, //EGL::CGLETaskArgs (GGL::CSettler)
        TASK_SUMMON_ENTITIES = 164, //EGL::CGLETaskArgs (GGL::CSummonBehavior)
        TASK_SPAWN_PLAYER_EFFECT = 165, //EGL::CTaskArgsEffectType (GGL::CGLBehaviorDying)
        TASK_GO_TO_NPC = 166, //EGL::CGLETaskArgs (GGL::CHeroBehavior)
        TASK_INTERACT_WITH_NPC = 167, //EGL::CGLETaskArgs (GGL::CHeroBehavior)
        TASK_SET_SPECIAL_ATTACK_ANIM = 168, //EGL::CGLETaskArgs (GGL::CInflictFearAbility, GGL::CCircularAttack)
        TASK_PERFORM_SPECIAL_ATTACK = 169, //EGL::CGLETaskArgs (GGL::CInflictFearAbility, GGL::CCircularAttack)
        TASK_RETURN_TO_OLD_TASK_LIST = 170,
        TASK_CHECK_MIN_RADIUS = 171, //EGL::CGLETaskArgs (GGL::CLeaderBehavior)
        TASK_MOVE_TO_BUILDING_TO_CONVERT = 172, //EGL::CGLETaskArgs (GGL::CConvertBuildingAbility)
        TASK_CONVERT_BUILDING = 173, //EGL::CGLETaskArgs (GGL::CConvertBuildingAbility)
        TASK_SET_LATEST_ATTACK_TURN = 174, //EGL::CGLETaskArgs (GGL::CBattleBehavior, GGL::CAutoCannonBehavior)
        TASK_WAIT_FOR_LATEST_ATTACK = 175, //EGL::CGLETaskArgs (GGL::CBattleBehavior, GGL::CAutoCannonBehavior) waits BattleWaitUntil from the tick last set by TASK_SET_LATEST_ATTACK_TURN
        TASK_RESOLVE_BATTLE_COLLISION = 176, //EGL::CGLETaskArgs (GGL::CBattleBehavior)
        TASK_START_WORK_IF_AT_WORKPLACE = 177, //EGL::CGLETaskArgsTaskListID (GGL::CWorkerBehavior)
        TASK_MOVE_TO_SETTLER_TO_CONVERT = 178, //EGL::CGLETaskArgs (GGL::CConverSettlerAbility)
        TASK_CONVERT_SETTLER = 179, //EGL::CGLETaskArgs (GGL::CConverSettlerAbility)
        TASK_TRADER_OFFER_POS_REACHED = 180, //EGL::CGLETaskArgs (GGL::CSettlerMerchantBehavior)
        TASK_GO_TO_NEAREST_NEUTRAL_BRIDGE = 181, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_CHECK_GO_TO_BRIDGE_SUCCESS = 182, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_INCREASE_BRIDGE_PROGRESS = 183, //EGL::CGLETaskArgs (GGL::CWorkerBehavior)
        TASK_EXPLORE = 184, //EGL::CGLETaskArgs (GGL::CAbilityScoutBinocular)
        TASK_TURN_TO_EXPLORE_POSITION = 185, //EGL::CGLETaskArgs (GGL::CAbilityScoutBinocular)
        TASK_GO_TO_TARGET_BUILDING = 186, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_CHECK_GO_TO_TARGET_BUILDING_SUCCESS = 187, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_GO_TO_SECURE_BUILDING = 188, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_CHECK_GO_TO_SECURE_BUILDING_SUCCESS = 189, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_STEAL_GOODS = 190, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_SECURE_STOLEN_GOODS = 191, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_SET_THIEF_MODEL = 192, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_THIEF_IDLE = 193, //EGL::CGLETaskArgs (GGL::CThiefBehavior)
        TASK_PLACE_KEG = 194, //EGL::CGLETaskArgs (GGL::CKegPlacerBehavior)
        TASK_GO_TO_KEG_TARGET = 195, //EGL::CGLETaskArgs (GGL::CKegPlacerBehavior)
        TASK_CHECK_GO_TO_KEG_TARGET_SUCCESS = 196, //EGL::CGLETaskArgs (GGL::CKegPlacerBehavior)
        TASK_RETREAT_FROM_KEG_TARGET = 197, //EGL::CGLETaskArgs (GGL::CKegPlacerBehavior)
        TASK_DISARM_KEG = 198, //EGL::CGLETaskArgs (GGL::CKegPlacerBehavior)
        TASK_GO_TO_KEG = 199, //EGL::CGLETaskArgs (GGL::CKegPlacerBehavior)
        TASK_SET_MOTIVATE_ANIM = 200, //EGL::CGLETaskArgs (GGL::CMotivateWorkersAbility)

        TASK_PERFORM_MOTIVATION = 201, //EGL::CGLETaskArgs (GGL::CMotivateWorkersAbility)
        TASK_SET_SNIPE_ANIM = 202, //EGL::CGLETaskArgs (GGL::CSniperAbility)
        TASK_SNIPE = 203, //EGL::CGLETaskArgs (GGL::CSniperAbility)
        TASK_GO_TO_TORCH_DESTINATION = 204, //EGL::CGLETaskArgs (GGL::CTorchPlacerBehavior)
        TASK_PLACE_TORCH = 205, //EGL::CGLETaskArgs (GGL::CTorchPlacerBehavior)
        TASK_POINT_TO_RESOURCE = 206, //EGL::CGLETaskArgs (GGL::CPointToResourceBehavior)
        TASK_SET_THROW_SHURIKEN_ANIM = 207, //EGL::CGLETaskArgs (GGL::CShurikenAbility)
        TASK_THROW_SHURIKEN = 208, //EGL::CGLETaskArgs (GGL::CShurikenAbility)
        TASK_TURN_TO_SNIPER_TARGET = 209, //EGL::CGLETaskArgs (GGL::CSniperAbility)
        TASK_TURN_TO_SHURIKEN_TARGET = 210, //EGL::CGLETaskArgs (GGL::CShurikenAbility)

        TASK_LUA_FUNC = 1500,
        TASK_WAIT_FOR_ANIM_NON_CANCELABLE = 1501,
    };

    enum class TaskState : int {
        Default = 0,
        // 1 something leader related (detach attack target, then defend?), also soldier related, also serf (to stop event)
        WaitForAnim = 2,
        Move = 3,
        Follow = 4,
        Wait = 5, // confusingly gets resused by autocannon wait until
        Rotate = 6,
        SetNextTaskList = 7,
        // 9 & 10 building behavior
        Move_NonCancelable = 12,
        // 13 something worker
        IdleInFormation = 14,
        AssumePositionInFormation = 15,
        SerfSearchResource = 16,
        Train = 17,
        HeroComatose = 18,
        BuildingAlarmDefend = 19,
        BattleWait = 20,
        // 21 something worker
        LeaderGetCloseTotarget = 22,
        MoveToTarget = 23,
        // 24 soldier formation?
        HeroGoToNPC = 25,
        ConvertBuilding = 26,
        // 27 settler entered building?
        ConvertSettler = 28,
        Merchant_WaitForHero = 29,
        ThiefStealGoods = 30,
        ThiefSabotage = 31,
        ThiefDisarm = 32,
        ScoutPointToRes = 33, // maybe unused?


        LuaFunc = 500,
        WaitForAnimNonCancelable = 501,
    };
}

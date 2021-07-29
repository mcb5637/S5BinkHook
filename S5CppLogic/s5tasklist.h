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

struct shok_taskData_vect;
struct shok_taskData {
	const char* TaskName;
    unsigned int TaskArgsClassID;
	shok_Task ID;

	static inline shok_taskData_vect** GlobalVector = reinterpret_cast<shok_taskData_vect**>(0x898238);

    static void AddExtraTasks();
    static void RemoveExtraTasks();
};
struct shok_taskData_vect {
    vector_padding;
    std::vector<shok_taskData, shok_allocator<shok_taskData>> TaskData;
};

struct shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int : shok_object {
	void* Object;
	int(__thiscall* Func)(void* th, shok_EGL_CGLETaskArgs* args);
    int Zero;

	int ExecuteTask(shok_EGL_CGLETaskArgs* args);
};
struct shok_EGL_IGLEStateHandler : shok_object {
    void* Object;
    int(__thiscall* Func)(void* th, int args);
    int Zero;
};


enum class shok_Task : int {
    TASK_SET_POS = 1,
    TASK_GO_TO_POS = 2,
    TASK_SET_ANIM = 3,
    TASK_SET_SUB_ANIM = 4,
    TASK_WAIT_FOR_ANIM = 5, // wait to % of anims duration, always timed from start of animation.
    TASK_RANDOM_WAIT_FOR_ANIM = 6,
    TASK_WAIT = 7,
    TASK_DELETE_SELF = 8,
    TASK_PLAY_SOUND = 9,
    TASK_SET_TASK_LIST = 10,
    TASK_STOP_SOUND = 11,
    TASK_RESET_TASK_LIST_TIMER = 12,
    TASK_TURN_TO_TARGET_ORIENTATION = 13,
    TASK_SET_MODEL = 14,
    TASK_SET_TARGET_ORIENTATION_RELATIVE = 15,
    TASK_ACTIVATE_PARTICLE_EFFECT = 16,
    TASK_DEACTIVATE_PARTICLE_EFFECT = 17,
    TASK_SPAWN_PARTICLE_EFFECT = 18,
    TASK_ACTIVATE_UVANIM = 19,
    TASK_SET_ANIM_AT_BUILDING = 20,
    TASK_VANISH = 21,
    TASK_APPEAR = 22,
    TASK_LIST_DONE = 23,
    TASK_CREATE_OBJ = 24,
    TASK_DELETE_OBJ = 25,
    TASK_ENTER_BUILDING = 26,
    TASK_LEAVE_BUILDING = 27,
    TASK_GO_TO_MAIN_HOUSE = 28,
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
    TASK_WALK = 44,
    TASK_DECREASE_COLLECTED_RESOURCE = 45,
    TASK_DESTROY = 46,
    TASK_GET_FROM_PILE_OF_TYPE = 47,
    TASK_CHECK_START_HARVESTING = 48,
    TASK_START_FARMING = 49,
    TASK_SOW = 50,

    TASK_PERFORM_ATTACK = 51,
    TASK_CREATE_ENTITY = 52,
    TASK_REPAIR = 53,
    TASK_RETURN_TO_EMPLOYER = 54,
    TASK_GO_TO_POS_EX = 55,
    TASK_WANDER = 56,
    TASK_BACK_TO_DEFAULT_TASKLIST = 57,
    TASK_WAIT_UNTIL = 58,
    TASK_GO_TO_CAMP = 59,
    TASK_LEAVE_CAMP = 60,
    TASK_LEAVE_SETTLEMENT = 61,
    TASK_ADVANCE_IN_CYCLE = 62,
    TASK_INCREASE_PLAYER_GOLD = 63,
    TASK_INCREASE_PLAYER_GOLDRAW = 64,
    TASK_INCREASE_PLAYER_SILVER = 65,
    TASK_INCREASE_PLAYER_SILVERRAW = 66,
    TASK_INCREASE_PLAYER_STONE = 67,
    TASK_INCREASE_PLAYER_STONERAW = 68,
    TASK_INCREASE_PLAYER_IRON = 69,
    TASK_INCREASE_PLAYER_IRONRAW = 70,
    TASK_INCREASE_PLAYER_SULFUR = 71,
    TASK_INCREASE_PLAYER_SULFURRAW = 72,
    TASK_INCREASE_PLAYER_CLAY = 73,
    TASK_INCREASE_PLAYER_CLAYRAW = 74,
    TASK_INCREASE_PLAYER_WOOD = 75,
    TASK_INCREASE_PLAYER_WOODRAW = 76,
    TASK_INCREASE_PLAYER_WEATHERENERGY = 77,
    TASK_INCREASE_PLAYER_KNOWLEDGE = 78,
    TASK_INCREASE_PLAYER_FAITH = 79,
    TASK_CONSUME_RESOURCE = 80,
    TASK_MINED_RESOURCE = 81,
    TASK_GO_TO_WORK_BUILDING = 82,
    TASK_GO_TO_EAT_BUILDING = 83,
    TASK_GO_TO_REST_BUILDING = 84,
    TASK_GO_TO_LEAVE_BUILDING = 85,
    TASK_NEW_MOTIVATION_MODIFIER = 86,
    TASK_CHECK_MOTIVATION = 87,
    TASK_WORK_WAIT_UNTIL = 88,
    TASK_EAT_WAIT = 89,
    TASK_REST_WAIT = 90,
    TASK_GO_TO_CONSTRUCTION_SITE = 91,
    TASK_TURN_TO_CONSTRUCTION_SITE = 92,
    TASK_CHANGE_ATTACHMENT_TO_CONSTRUCTION_SITE = 93,
    TASK_HURT = 94,
    TASK_MOVE_TO_TARGET = 95,
    TASK_SET_ORIENTATION_TO_TARGET = 96,
    TASK_SET_ATTACK_ANIM = 97,
    TASK_SET_BATTLE_IDLE_ANIM = 98,
    TASK_ASSUME_POSITION_IN_FORMATION = 99,

    TASK_IDLE_IN_FORMATION = 100,
    TASK_FIND_RESEARCH_BUILDING = 101,
    TASK_GO_TO_RESEARCH_BUILDING = 102,
    TASK_DO_RESEARCH = 103,
    TASK_CHECK_RANGE = 104,
    TASK_ABANDON_CURRENT_JOB = 105,
    TASK_GO_TO_RESOURCE = 106,
    TASK_TURN_TO_RESOURCE = 107,
    TASK_WAIT_EXTRACTION_DELAY = 108,
    TASK_EXTRACT_RESOURCE = 109,
    TASK_FIRE_PROJECTILE = 110,
    TASK_REFINE_RESOURCE = 111,
    TASK_SET_DEFAULT_REACTION_TYPE = 112,
    TASK_DO_TRADE_STEP = 113,
    TASK_DO_RESEARCH_STEP = 114,
    TASK_PLACE_BOMB = 115,
    TASK_GO_TO_BOMB_TARGET = 116,
    TASK_RETREAT_FROM_BOMB_TARGET = 117,
    TASK_LEAVE_BARRACKS = 118,
    TASK_RESOLVE_COLLISION = 119,
    TASK_CHANGE_WORK_TIME_ABSOLUTE = 120,
    TASK_CHANGE_WORK_TIME_RELATIVE = 121,
    TASK_MOVE_TO_EXPLORATION_POINT = 122,
    TASK_CHECK_HERO_POSITION = 123,
    TASK_SET_ANIM_AT_WORKPLACE = 124,
    TASK_SET_ANIM_AT_FARM = 125,
    TASK_SET_ANIM_AT_RESIDENCE = 126,
    TASK_SET_ANIM_AT_BARRACKS = 127,
    TASK_SET_ANIM_AT_VILLAGE_CENTER = 128,
    TASK_BATTLE_WAIT_UNTIL = 129,
    TASK_TRAIN = 130,
    TASK_TURN_INTO_BATTLE_SERF = 131,
    TASK_TURN_INTO_SERF = 132,
    TASK_BECOME_COMATOSE = 133,
    TASK_GO_TO_CANNON_POSITION = 134,
    TASK_BUILD_CANNON = 135,
    TASK_GO_TO_RESOURCE_SLOT = 136,
    TASK_CHANGE_DEFENDABLE_BUILDING_ATTACHMENT = 137,
    TASK_MOVE_TO_DEFENDABLE_BUILDING = 138,
    TASK_DEFEND = 139,
    TASK_GO_TO_CONSTRUCTION_SITE_SLOT = 140,
    TASK_FLEE = 141,
    TASK_RETURN_TO_CYCLE = 142,
    TASK_CHECK_FEAR = 143,
    TASK_CHANGE_WORK_TIME_WORK = 144,
    TASK_CHANGE_WORK_TIME_FARM = 145,
    TASK_CHANGE_WORK_TIME_RESIDENCE = 146,
    TASK_CHANGE_WORK_TIME_CAMP = 147,
    TASK_GET_CLOSE_TO_TARGET = 148,
    TASK_ATTACK = 149,

    TASK_GO_TO_SUPPLIER = 150,
    TASK_DO_WORK_AT_FOUNDRY = 151,
    TASK_CREATE_CANNON = 152,
    TASK_SET_CANNON_PROGRESS = 153,
    TASK_CHECK_GO_TO_REST_BUILDING_SUCCESS = 154,
    TASK_CHECK_GO_TO_EAT_BUILDING_SUCCESS = 155,
    TASK_CHECK_GO_TO_WORK_BUILDING_SUCCESS = 156,
    TASK_CHECK_GO_TO_VILLAGE_CENTER_SUCCESS = 157,
    TASK_CHECK_GO_TO_DEFENDABLE_BUILDING_SUCCESS = 158,
    TASK_TAKE_FROM_STOCK = 159,
    TASK_SET_CARRIER_MODEL = 160,
    TASK_CHECK_GO_TO_SUPPLIER_SUCCESS = 161,
    TASK_LEFT_BUILDING = 162,
    TASK_GO_TO_FREE_POSITION = 163,
    TASK_SUMMON_ENTITIES = 164,
    TASK_SPAWN_PLAYER_EFFECT = 165,
    TASK_GO_TO_NPC = 166,
    TASK_INTERACT_WITH_NPC = 167,
    TASK_SET_SPECIAL_ATTACK_ANIM = 168,
    TASK_PERFORM_SPECIAL_ATTACK = 169,
    TASK_RETURN_TO_OLD_TASK_LIST = 170,
    TASK_CHECK_MIN_RADIUS = 171,
    TASK_MOVE_TO_BUILDING_TO_CONVERT = 172,
    TASK_CONVERT_BUILDING = 173,
    TASK_SET_LATEST_ATTACK_TURN = 174,
    TASK_WAIT_FOR_LATEST_ATTACK = 175,
    TASK_RESOLVE_BATTLE_COLLISION = 176,
    TASK_START_WORK_IF_AT_WORKPLACE = 177,
    TASK_MOVE_TO_SETTLER_TO_CONVERT = 178,
    TASK_CONVERT_SETTLER = 179,
    TASK_TRADER_OFFER_POS_REACHED = 180,
    TASK_GO_TO_NEAREST_NEUTRAL_BRIDGE = 181,
    TASK_CHECK_GO_TO_BRIDGE_SUCCESS = 182,
    TASK_INCREASE_BRIDGE_PROGRESS = 183,
    TASK_EXPLORE = 184,
    TASK_TURN_TO_EXPLORE_POSITION = 185,
    TASK_GO_TO_TARGET_BUILDING = 186,
    TASK_CHECK_GO_TO_TARGET_BUILDING_SUCCESS = 187,
    TASK_GO_TO_SECURE_BUILDING = 188,
    TASK_CHECK_GO_TO_SECURE_BUILDING_SUCCESS = 189,
    TASK_STEAL_GOODS = 190,
    TASK_SECURE_STOLEN_GOODS = 191,
    TASK_SET_THIEF_MODEL = 192,
    TASK_THIEF_IDLE = 193,
    TASK_PLACE_KEG = 194,
    TASK_GO_TO_KEG_TARGET = 195,
    TASK_CHECK_GO_TO_KEG_TARGET_SUCCESS = 196,
    TASK_RETREAT_FROM_KEG_TARGET = 197,
    TASK_DISARM_KEG = 198,
    TASK_GO_TO_KEG = 199,
    TASK_SET_MOTIVATE_ANIM = 200,

    TASK_PERFORM_MOTIVATION = 201,
    TASK_SET_SNIPE_ANIM = 202,
    TASK_SNIPE = 203,
    TASK_GO_TO_TORCH_DESTINATION = 204,
    TASK_PLACE_TORCH = 205,
    TASK_POINT_TO_RESOURCE = 206,
    TASK_SET_THROW_SHURIKEN_ANIM = 207,
    TASK_THROW_SHURIKEN = 208,
    TASK_TURN_TO_SNIPER_TARGET = 209,
    TASK_TURN_TO_SHURIKEN_TARGET = 210,

    TASK_LUA_FUNC = 1500,
    TASK_WAIT_FOR_ANIM_NON_CANCELABLE = 1501,
};

enum class shok_TaskState : int {
    Default = 0,
    // 1 something leader related (detach attack target, then defend?), also soldier related, also serf (to stop event)
    WaitForAnim = 2,
    Move = 3,
    Follow = 4,
    BattleWaitUntilAutoCannon = 5,
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
    ComvertSettler = 28,
    Merchant_WaitForHero = 29,
    ThiefStealGoods = 30,
    ThiefSabotage = 31,
    ThiefDisarm = 32,
    ScoutPointToRes = 33, // maybe unused?


    LuaFunc = 500,
    WaitForAnimNonCancelable = 501,
};

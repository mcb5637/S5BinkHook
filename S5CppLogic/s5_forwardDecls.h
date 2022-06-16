#pragma once

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define PADDING(size) private: char TOKENPASTE2(padding_, __LINE__) [size]={}; public:
#define PADDINGI(size) PADDING(size*4)

#define DEBUGGER_BREAK _asm int 3

constexpr int SHOK_Import_LUA_OPEN = 0x761284;
constexpr int SHOK_Import_LUA_CLOSE = 0x76126C;
constexpr int SHOK_SEGMENTSTART = 0x401000;
constexpr int SHOK_SEGMENTLENGTH = 0x64B000;

typedef uint8_t byte;

struct lua_State;

namespace shok {
	enum class ResourceType : int;
	enum class AttachmentType : int;
	enum class NetEventIds : int;
	enum class FeedbackEventIds : int;
	enum class EventIDs : int;
	enum class InputEventIds : int;
	enum class Task : int;
	enum class TaskState : int;
	enum class LeaderCommand : int;
	enum class EntityCategory : int;
	enum class DiploState : int;
	enum class TechState : int;
	enum class Keys : int;

	class Technology;
	class TechCategoryManager;
	class TechManager;
}
namespace EGL {
	class CGLEEntity;
	class CGLEEntityCreator;
	class LandscapeBlockingData;
	class CGLELandscape;
	class CGLETaskArgs;
	class CGLETaskList;
	class CTaskArgsInteger;
	class CGLEBehaviorProps;
	class IEntityDisplay;
	class CEffect;
	class CGLEEffectCreator;
}
namespace GGL {
	class CBuilding;
	class CHeroAbilityProps;
	class CCamouflageBehaviorProps;
	class CCircularAttackProps;
	class CSummonBehaviorProps;
	class CGLBuildingProps;
	class CPlayerStatus;
	class CResourceDoodad;
	class CSettler;
}
namespace ED {
	class CBehaviorProps;
	class CEntity;
	class CDisplayProps;
	class CVisibleEntityManager;
	struct ModelData;
}
namespace BB {
	class CEvent;
	class CIDManagerEx;
	class CFileStreamEx;
	struct SerializationData;
	class CBinarySerializer;
	class CIDManager;
	class IStream;
}
namespace GGUI {
	class C3DViewHandler;
	class CState;
}
namespace GGlue {
	class CGlueEntityProps;
}
namespace EGUIX {
	class CBaseWidget;
	struct Color;
}
namespace RWE {
	struct RpClump;
}

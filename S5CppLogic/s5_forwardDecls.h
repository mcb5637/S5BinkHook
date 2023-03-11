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
	enum class ExperienceClass : int;
	enum class TechModifierType : int;
	enum class TaskStateExecutionResult : int;
	enum class TaskExecutionResult : int;
	enum class AdvancedDealDamageSource : int;
	enum class AccessCategory : int;
	enum class WeatherState : int;
	enum class WorkerReason : int;

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
	class EffectsDisplayProps;
	class CGLEAnimProps;
	class CGLEAnimSet;
	class CFlyingEffectProps;
	class CGLETaskArgsThousandths;
	class CGLETerrainLowRes;

	template<class T, unsigned int id>
	requires (std::same_as<T, bool>&& id == 0x65DE8317) || (std::same_as<T, int> && id == 0x483040E7) || (std::same_as<T, float> && id == 0x578EE8F7)
	class CEventGetValue;
	using CEventGetValue_Bool = CEventGetValue<bool, 0x65DE8317>;
	using CEventGetValue_Int = CEventGetValue<int, 0x483040E7>;
	using CEventGetValue_Float = CEventGetValue<float, 0x578EE8F7>;

	class CEventGetPosition;
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
	class CArrowEffectProps;
	class CCannonBallEffectProps;
	class CAutoCannonBehaviorProps;
	class CBattleBehaviorProps;
	class CGLGameLogic;
}
namespace ED {
	class CBehaviorProps;
	class CEntity;
	class CDisplayProps;
	class CVisibleEntityManager;
	struct ModelData;
	class CEffectProps;
	class EntityTypeDisplayProps;
	class CTerrainDecalBase;
}
namespace GD {
	class CDDisplay;
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
	struct SStateParameters;
}
namespace GGlue {
	class CGlueEntityProps;
	class TerrainPropsDisplay;
}
namespace EGUIX {
	class CBaseWidget;
	class CMaterial;
	struct Rect;
	struct Color;
}
struct RwTexture;
struct RtAnimAnimation;
namespace RWE {
	struct RpClump;
	struct RpAtomic;
	using RwTexture = ::RwTexture;
}
namespace RWE::Particles {
	struct RpPrtStdEmitter;
	typedef RpPrtStdEmitter* (__cdecl* RpPrtStdEmitterCallBack) (RWE::RpAtomic* atomic, RpPrtStdEmitter* emt, void* data);
}
namespace RWE::Anim {
	using RtAnimAnimation = ::RtAnimAnimation;
	struct RtAnimationFrameHandler;
	struct RtAnimInterpolator;
	struct RpHAnimHierarchy;
}
namespace RWE::P2D {
	struct Rt2dFont;
	struct Rt2dBrush;
}
namespace EScr {
	class CScriptTrigger;
}
namespace CppLogic::EntityAddon {
	class EntityAddonData;
}

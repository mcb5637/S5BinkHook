#pragma once

namespace shok {
	enum class ResourceType : int;
	enum class AttachmentType : int;
	enum class NetEventIds : int;
	enum class FeedbackEventIds : int;
	enum class EventIDs : int;
	enum class InputEventIds : int;
}
namespace EGL {
	class CGLEEntity;
	class CGLEEntityCreator;
	class LandscapeBlockingData;
	class CGLELandscape;
}
namespace GGL {
	class CBuilding;
}
namespace ED {
	class CBehaviorProps;
	class CEntity;
	struct ModelInstance;
	class CDisplayProps;
}
namespace BB {
	class CEvent;
	class CIDManagerEx;
	class CFileStreamEx;
}
namespace GGUI {
	class C3DViewHandler;
}

struct shok_BB_CClassFactory_serializationData;

struct shok_EGL_CGLETaskArgs;

enum class shok_Task;
enum class shok_TaskState;

struct shok_EGL_CGLETaskList;
struct shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int;
struct shok_EGL_IGLEStateHandler;

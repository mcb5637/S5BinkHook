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
}
namespace GGL {
	class CBuilding;
}
namespace ED {
	class CBehaviorProps;
	class CEntity;
}
namespace BB {
	class CEvent;
}

struct shok_BB_CClassFactory_serializationData;

struct shok_EGL_CGLETaskArgs;

enum class shok_Task;
enum class shok_TaskState;

struct shok_EGL_CGLETaskList;
struct shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int;
struct shok_EGL_IGLEStateHandler;

struct shok_modelinstance;
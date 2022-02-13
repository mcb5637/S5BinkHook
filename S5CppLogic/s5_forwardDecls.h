#pragma once

namespace shok {
	enum class ResourceType;
	enum class AttachmentType;
}
namespace EGL {
	class CGLEEntity;
	class CGLEEntityCreator;
}
namespace GGL {
	class CBuilding;
}

struct shok_BB_CClassFactory_serializationData;

struct shok_BB_CEvent;
enum class shok_EventIDs : int;
enum class shok_NetEventIds : int;
enum class shok_FeedbackEventIds : int;
enum class shok_InputEventIds : int;
struct shok_EGL_CGLETaskArgs;

enum class shok_Task;
enum class shok_TaskState;

struct shok_ED_CBehaviorProps;

struct shok_EGL_CGLETaskList;
struct shok_EGL_IGLEHandler_EGL_CGLETaskArgs_int;
struct shok_EGL_IGLEStateHandler;
struct shok_EGL_IGLEHandler_BB_CEvent_void;

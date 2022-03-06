#pragma once

namespace shok {
	enum class ResourceType : int;
	enum class AttachmentType : int;
	enum class NetEventIds : int;
	enum class FeedbackEventIds : int;
	enum class EventIDs : int;
	enum class InputEventIds : int;
	enum class Task : int;
	enum class TaskState : int;
}
namespace EGL {
	class CGLEEntity;
	class CGLEEntityCreator;
	class LandscapeBlockingData;
	class CGLELandscape;
	class CGLETaskArgs;
	class CGLETaskList;
	class CTaskArgsInteger;
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
	struct SerializationData;
}
namespace GGUI {
	class C3DViewHandler;
}

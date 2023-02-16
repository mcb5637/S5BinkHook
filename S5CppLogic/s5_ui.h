#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_maplogic.h"
#include "s5_defines.h"
#include "s5_player.h"
#include "s5_guistates.h"

enum class win_mouseEvents : int {
	KeyDown = 0x100,
	KeyUp = 0x101,
	Char = 0x102,
	SysKeyDown = 0x104,
	SysKeyUp = 0x105,
	MouseMove = 0x200,
	LButtonDown = 0x201,
	LButtonUp = 0x202,
	LButtonDBl = 0x203,
	RButtonDown = 0x204,
	RButtonUp = 0x205,
	RButtonDBl = 0x206,
	MButtonDown = 0x207,
	MButtonUp = 0x208,
	MButtonDBl = 0x209,
	MouseWheel = 0x20A,
	XButtonDown = 0x20B,
	XButtonUp = 0x20C,
	XButtonDBl = 0x20D,
};

namespace BB {
	class ITextSet {
	public:
		static inline constexpr int vtp = 0x7AECB0;
		virtual ~ITextSet() = default;
		virtual void __stdcall Destroy() = 0;
		virtual const char* __stdcall GetTextById(int id) = 0;
		virtual const char* __stdcall GetText(const char* key) = 0;
		virtual void __stdcall CopyText(const char* targetKey, const char* sourceKey) = 0;
		virtual void __stdcall Load(const char* dir) = 0;
		virtual BB::CIDManagerEx* __stdcall GetIdManager() = 0;
	};
	class CTextSet : public ITextSet {
	public:
		static inline constexpr int vtp = 0x7AED08;
		char Language[4];
		char* Directory; // ingame
		PADDINGI(1);
		BB::CIDManagerEx* IdManager;
		shok::Vector<char*> Strings; // 5

		void Load();
		void Merge(CTextSet* other);
	};
	static_assert(sizeof(CTextSet) == 4 * 9);
	class IText {
	public:
		static inline constexpr int vtp = 0x7AECD0;
		virtual ~IText() = default;
		virtual void __stdcall Destroy() = 0;
		virtual void __stdcall SetLanguage(const char* l) = 0; // 2 letters code
		virtual CTextSet* __stdcall CreateTextSet() = 0;
		virtual CTextSet* __stdcall CreateTextSetAndLoadFromDir(const char* dir) = 0;
		virtual CTextSet* __stdcall CreateSetAndLoadFrom2Dirs(const char* dir1, const char* dir2) = 0;
	};
	class CText : public IText {
	public:
		static inline constexpr int vtp = 0x7AECEC;
		char Language[4];
	};
	static_assert(sizeof(CText) == 2 * 4);
	struct StringTableText {
		CText* Text;
		CTextSet* TextSet;

		static inline const char* (__cdecl* const GetStringTableText)(const char* key) = reinterpret_cast<const char* (__cdecl* const)(const char*)>(0x556D2E);
		static const char* (*GetStringTableTextOverride)(const char* s);
		static void HookGetStringTableText();

		static inline StringTableText** const GlobalObj = reinterpret_cast<StringTableText**>(0x894508);

	private:
		static const char* __stdcall GetSTTOverride(const char* s);
	};
}

namespace shok {
	static inline HWND* MainWindowHandle = reinterpret_cast<HWND*>(0x84ECC4);


	class UIRenderer {
	public:
		PADDINGI(1);
		bool ShouldRenderText;
		PADDING(3);
		PADDINGI(1);
		float RenderSizeX, RenderSizeY; // 3
		bool SomeTextBool;
		PADDING(3);
		PADDINGI(1);
		RWE::P2D::Rt2dBrush* Brush;

		void RenderText(const char* txt, int fontid, float x, float y, float xend, const EGUIX::Color* color, float linedistancefactor);
		void SetTextRenderColor(shok::Color c);
		void RenderMaterial(const EGUIX::CMaterial* mat, bool scale, const EGUIX::Rect* pos);

		static constexpr shok::Position ScaledScreenSize{ 1024.0f, 768.0f };

		static inline size_t(__cdecl* const MultibyteToWString)(const char* in, wchar_t* out, size_t len) = reinterpret_cast<size_t(__cdecl*)(const char*, wchar_t*, size_t)>(0x55B73C);

		static inline UIRenderer* (__cdecl* const GlobalObj)() = reinterpret_cast<UIRenderer * (__cdecl*)()>(0x556F5B);
	};
	class UIRenderCustomColorContext {
	public:
		const EGUIX::Color* GetColorByInt(int i);
	};
	//constexpr int i = offsetof(UIRenderer, RenderSizeX) / 4;


	void HookTextPrinting();
}

namespace EToolsManager {
	class CSimpleTool {
		virtual void getsecmemb() = delete;

		void* Data; // GS3DTools::CAppCamera
		unsigned int Ident; // 0xCA334412
	public:
		static inline constexpr unsigned int Identifier = 0x425576F4; // from CRwCameraHandler casttoident
		static inline constexpr int vtp = 0x78061C;
	};
}

namespace ERwTools {
	class IRenderable {
	protected:
		virtual void SetBuildingRedColorI(int r) = 0;
	private:
		virtual void uk0() = 0;
		virtual void uk1() = 0;
		virtual void uk2() = 0;
	public:
		virtual void SetPosition(float x, float y, float z) = 0;
	private:
		virtual void uk3() = 0;
	protected:
		virtual void SetModelData(RWE::RpClump* model, float rot) = 0;
	};
	class CRpClumpRenderable : public IRenderable {
	public:
		int RenderMode; // red color?
		RWE::RpClump* Model;
		void* RenderObj; // from ED::CWorld

		static inline constexpr int vtp = 0x7AEBF8;

		using IRenderable::SetModelData;
		using IRenderable::SetPosition;

		void SetModelData(const ED::ModelData* modeldata, float rotation);
		void SetPosition(const shok::Position& p, float z);
		void SetBuildingRedColor(bool r);
	};

	struct CameraInfo {
		float LookAtX, LookAtY, LookAtZ;
		float Distance;
	};
	class ICameraHandle {
	public:
		virtual void SetDirty() = 0;
		virtual int GetUpsdateZMode() = 0;
		virtual CameraInfo* GetCameraInfo() = 0;
		virtual float GetZoomFactor() = 0;
	private:
		virtual void Noop() = 0;
	public:
		virtual void ScrollSetLookAt(float x, float y, bool uk) = 0;
		virtual void SetScrollLookAtTime(float t) = 0;
		virtual void ScrollGetLookAt(float* x, float* y) = 0;
		virtual void ScrollSetSpeed(float sp) = 0;
		virtual void SetScrollLeft(int flag) = 0;
		virtual void SetScrollRight(int flag) = 0; //10
		virtual void SetScrollUp(int flag) = 0;
		virtual void SetScrollDown(int flag) = 0;
		virtual void SetScrollBorderFlag(int flag) = 0;
		virtual void SetScrollMouseMode(int flag) = 0;
		virtual void GetScrollMouseMode() = 0;
		virtual void GetScrollBorderFlag() = 0;
		virtual void SetUpdateZMode(int mode) = 0; // >= 0 && < 4
		virtual void GetUpdateZMode() = 0;
		virtual void SetLookAtZ(float z) = 0;
		virtual void SetScrollSpeedZ(float a) = 0; // 20
		virtual void SetScrollZUp(int flag) = 0;
		virtual void SetScrollZDown(int flag) = 0;
		virtual void ScrollGameTimeSynced(int currentTurn, float x, float y) = 0;
		virtual void FlyToLookAt(int currentTurn, float x, float y, float time) = 0;
		virtual void SetZoomSpeed(float s) = 0;
		virtual void SetZoomFactor(float f) = 0;
		virtual void SetZoomFactorMin(float f) = 0;
		virtual void SetZoomFactorMax(float f) = 0;
		virtual void SetFOV(float fov) = 0;
		virtual void SetVerticalAngle(float a) = 0; // 30
		virtual void SetCameraDistance(float d) = 0;
		virtual void SetZoomIn(int flag) = 0;
		virtual void SetZoomOut(int flag) = 0;
		virtual void ScrollWheelZoom(int dir) = 0;
		virtual void ZoomSetAngleGameTimeSynced(int currentTurn, float z) = 0;
		virtual void ZoomSetDistanceGameTimeSynced(int currentTurn, float d) = 0;
		virtual float GetVerticalAngle() = 0;
		virtual float GetLookAtZ() = 0;
		virtual void ZoomSetAngleFlight(int currentTurn, float angle, float time) = 0;
		virtual void ZoomSetDistanceFlight(int currentTurn, float distance, float time) = 0; // 40
		virtual void SetRotationSpeed(float speed) = 0;
		virtual void SetRotMaxAngle(float a) = 0;
		virtual void SetRotFlipBack(int flag) = 0;
		virtual void SetRotFlipBackSpeed(float speed) = 0;
		virtual void SetHorizontalAngle(float a) = 0;
		virtual float GetHorizontalAngle() = 0;
		virtual void SetRotClockwise(int flag) = 0;
		virtual void SetRotCounterClockwise(int flag) = 0;
		virtual void SetRotDefaultAngle(float a) = 0;
		virtual void RotGameTimeSynced(int currentTurn, float rot) = 0; // 50
		virtual void RotFlight(int currentTurn, float rot, float time) = 0;
	private:
		virtual void set49() = 0;
		virtual void set50() = 0;
		virtual void set51() = 0;
		virtual void set52() = 0;
	public:
		virtual void SetEntityIDToFollow(int id) = 0;
		virtual void SetControlMode(int mode) = 0;
		virtual bool GetScreenCoord(float x, float y, float z, float* xout, float* yout) = 0; // 58
	private:
		virtual void get46() = 0;
	public:
		virtual void InitCameraFlight() = 0;
		// get something from lookatxelem, then 2 file funcs



		static inline constexpr unsigned int Identifier = 0xCB15D84;
	};
	class ICameraMovement {
	private:
		virtual void uk2() = 0;
	public:
		static inline constexpr unsigned int Identifier = 0x3D8FC4E4;
	};
	class ICameraSettings {
	public:
		virtual void CS_SetVerticalAngle(float a) = 0;
		virtual void CS_SetFOV(float fov) = 0;
	private:
		virtual void setsomepos() = 0;
	public:
		virtual void CS_SetRotMaxAngle(float a) = 0;
	private:
		virtual void nop() = 0;
	public:
		virtual void SetUpLimit(float upLowLimit, float upHighLimit) = 0;
		virtual void ResetFOV() = 0;


		static inline constexpr unsigned int Identifier = 0x758445A4;
	};
	class CRwCameraHandler : public BB::IObject, public ICameraHandle, public ICameraMovement, public ICameraSettings {
	public:
		bool Dirty; // 4
		int UpdateZMode; // >= 0 && < 4
		bool bScrolling;
		float FOV;
		CameraInfo CameraInfo; // 8
		float HorizontalAngle; // 12
		float VerticalAngle;
		float ZoomFactor;
		float ZoomFactorMin;
		float ZoomFactorMax;
		float SpeedZ;
		float UpLowLimit;
		float UpHighLimit;
		double LastRealTime; // 20
		double GameTurnDuration;
		int LastGameTurn;
		bool bRotFlipBack;
		float RotDir;
		float RotSpeed;
		float RotFlipBackSpeed;
		float RotMaxAngle;
		float RotDefaultAngle; // 30
		float ScrollSpeed;
		float ScrollSpeedX;
		float ScrollSpeedY;
		float ScrollTargetX;
		float ScrollTargetY;
		float ScrollLookAtTime;
		float ScrollXDir;
		float ScrollYDir;
		float ScrollZDir;
		bool ScrollBorderFlag; // 40
		float ScrollSpeedZ;
		int ScrollMouseMode;
		float ZoomDir;
		float ZoomSpeed;
		float ZoomWheelFactor; // 45
		PADDINGI(7);
		EToolsManager::CSimpleTool SimpleTool; // 53
		int EntityIDToFollow;
		int ScrollGTSLastGameTurn;
		float ScrollGTSLastLookAtX;
		float ScrollGTSLastLookAtY;
		int ScrollGTSTargetGameTurn; // 60
		float ScrollGTSTargetLookAtX;
		float ScrollGTSTargetLookAtY;
		int ZoomAngleGTSLastGameTurn;
		float ZoomAngleGTSLastAngle;
		int ZoomAngleGTSTargetGameTurn;
		float ZoomAngleGTSTargetAngle;
		int ZoomDistanceGTSLastGameTurn;
		float ZoomDistanceGTSLastDistance;
		int ZoomDistanceGTSTargetGameTurn;
		float ZoomDistanceGTSTargetDistance; // 70
		int RotateGTSLastGameTurn;
		float RotateGTSLastAngle;
		int RotateGTSTargetGameTurn;
		float RotateGTSTargetAngle;
		int ControlMode;
		struct {
			struct {
				// all Time -> Data;
				shok::Map<float, float> LookAtXElement;
				shok::Map<float, float> LookAtYElement;
				shok::Map<float, float> ZoomAngleElement;
				shok::Map<float, float> ZoomDistanceElement;
				shok::Map<float, float> AngleElement;
			} Handler;
		} Flight;
		PADDINGI(1);

	public:
		static inline constexpr int vtp = 0x77AD98;
		static inline constexpr unsigned int Identifier = 0x45CAFEB1;

		static inline ICameraHandle** const GlobalObj = reinterpret_cast<ICameraHandle**>(0x87EC68);
		static inline float* const CutsceneFarClipPlaneMax = reinterpret_cast<float*>(0x77A7E8);
		static inline float* const CutsceneFarClipPlaneMin = reinterpret_cast<float*>(0x77A7F0);
	};
	static_assert(sizeof(CRwCameraHandler) == 92*4);
	constexpr int i = offsetof(CRwCameraHandler, CameraInfo.LookAtZ) / 4;
}

namespace EGL {
	class IGLEGUIInterface {
	public:
		virtual ~IGLEGUIInterface() = default;
		virtual bool GetEntityPosition(int id, float* x, float* y, float* r) = 0;
		virtual int GetEntityPlayer(int id) = 0;
		virtual int GetEntityTypeByName(const char* typeName) = 0;
		virtual int GetEntityType(int id) = 0;
		virtual float GetDistanceTo(int id, float x, float y) = 0; // 5
		virtual const char* GetEntityTypeName(int ety) = 0;
		virtual int GetEntityHealth(int id) = 0;
		virtual bool IsEntityValid(int id) = 0;
		virtual bool IsEntityState0x10003(int id) = 0;
		virtual bool IsEntityOfCategory(int id, shok::EntityCategory cat) = 0; // 10
	private:
		virtual bool unknownfunc1(int pl) = 0; // check exploration active for player?
	public:
		virtual int GetGameTick() = 0;

		static inline constexpr int vtp = 0x7837D0;
	};
	class CGLEGUIInterface : public IGLEGUIInterface {
	public:
		static inline constexpr int vtp = 0x783840;
	};
}

namespace GGL {
	class IGLGUIInterface {
	public:
		struct MerchantData {
			int Entity;
			bool HasOffers = 0;
			PADDING(3);
			int BlockingPlayer = 0;
			PADDINGI(2); // unknown entries of beh props, strings?
		};
		struct MineData {
			int Entity;
			int ResAmount = 0;
			int ResType = 0;
		};
		struct MotivationData {
			int Entity;
			float Max, ThresholdSad, ThresholdAngry, ThresholdLeave, VCLockedThreshold, ThresholdHappy;
		};
		struct RefinerData {
			int Entity;
			shok::ResourceType ResType = shok::ResourceType::None;
			float ResAmount = 0, RawAmount = 0;
		};
		struct UIData {
			float CamouflageBar = 0;
			float LimitedLifespanBar = 0;
			bool IsMine = false, BuildingIsMotivationLocked = false;
			float HeroResurrectionTimePassed = 0;
			float HeroResurrectionTimeMax = 0;
			bool WorkerIsFleeing = false, IsHero = false; // 5
			int KegTimeToExplode = 0;
			int NPCMarker = 0;
			bool BuildingOvertimeActive = false;
			float ThiefSabotageOrDefuseBar = 0;
			float BuildingAttachedThiefStealProgress = 0; // 10
			float WorkerWorktimeBar = 0;
			float LeaderCurrentSoldierAmount = 0;
			float LeaderMaxSoldierAmount = 0;
			float HealthBar = 0;
			float BuildingProgressNumber = 0; // 15 upgrade, research, market trade
			int LeaderExperienceLevels = 0;//float?
			int EntityId = 0;
			float WorkerMotivation = 0;
			bool IsRefiner = false, IsWorkerOverhead = false, IsSerfOverhead = false, IsSoldierOverhead = false;
			bool IsLeaderOverhead = false, IsBuildingAlarmActive = false; //20
			shok::String TaskListName;
			PADDINGI(14);
			ED::CEntity* Entity = nullptr; // 42
			GGL::CSettler* Settler = nullptr;
			EGL::CGLEEntity* BuildingToDisplay = nullptr; // mine, constructionsite, building
			GGL::CSettler* Leader = nullptr; //45
			GGL::CBuilding* ConstructionSite = nullptr;
			float Unknown = 0; //600 building, 310 cav experience class, 200 other settlers
			int WorkplaceNumberOfWorkersAttached = 0;
			int NumberOfSerfsAttachedToConstructionSite = 0;
			int WorkplaceMaxNumberOfWorkers = 0; //50
			int ResidenceNumberOfSleepers = 0;
			int NumberOfSettlersEnteredBuilding = 0;
			int ResidenceMaxNumberOfSleepers = 0;
			int MaxNumberOfSerfsAttachedToConstructionSite = 0;
			int FarmNumberOfEaters = 0; // 55
			int FarmMaxNumberOfEaters = 0;
			float BuildingProgressBar = 0; // bridge progress, building height
			bool IsMercenaryBuilding = false;
		};

		virtual ~IGLGUIInterface() = default;
		virtual bool CanPlaceBuildingAtPos(int ety, float x, float y, float r) = 0; // checks building func or buildblock of entity blocking
		virtual bool CanPlaceEntityAtPos(int ety, float x, float y, float r) = 0; // checks buildblock+block of entity blocking
		virtual bool IsBuildOnBuilding(int ety) = 0;
		virtual bool GetNearestFreePosForBuildingPlacement(int ety, float x, float y, float* xout, float* yout, float range) = 0;
		virtual bool DoesBuildOnTypeMatches(int ety, int eid) = 0;
		virtual bool CheckBuildingPlacementAndCost(int pl, int ety, float x, float y, float r) = 0;
		virtual int GetSettlerTypeByUCat(int pl, int ucat) = 0;
		virtual bool IsWall(int ety) = 0;
		virtual bool IsValidPos(float x, float y) = 0;
		virtual bool IsPosBlocked(float x, float y) = 0; // 10
		virtual shok::ResourceType GetResourceTypeProvidedBy(int eid) = 0;
		virtual void FillMerchantData(MerchantData* d) = 0;
		virtual void FillMineData(MineData* d) = 0; // works with mine building or resentity
		virtual void FillMotivationData(MotivationData* d) = 0;
		virtual void FillRefinerData(RefinerData* d) = 0;
		virtual void FillUIData(int player, UIData* d) = 0;
	private:
		virtual void unknown1() = 0; // get workplace string???
		virtual void unknown2() = 0; // get residence string???
		virtual void unknown3() = 0; // get farm string???
	public:
		virtual int GetModelOverride(int eid) = 0; // 20
		virtual int GetCurrentResourceAmount(int eid) = 0;
		virtual bool DoesSetTaskListDepthMatch(int eid, int tlde) = 0;
		virtual bool IsWorker(int eid) = 0;
		virtual bool IsSettler(int eid) = 0;
		virtual bool IsSoldier(int eid) = 0;
		virtual int GetEntityOrBaseEntity(int eid) = 0; // BuildOn base
		virtual int GetLeaderOfSoldier(int eid) = 0;
		virtual bool IsLeader(int eid) = 0;
		virtual bool IsHero(int eid) = 0;
		virtual bool IsNPCMarkerOn(int eid) = 0; // 30
		virtual bool IsSerf(int eid) = 0;
	private:
		virtual void unknown4() = 0; // maybe player related
	public:
		virtual bool IsBuilding(int eid) = 0;
		virtual bool IsConstructionSite(int eid) = 0;
		virtual bool IsBuildingSellable(int eid, int player) = 0;
	private:
		virtual void unknown5() = 0; // something buildingupgrademanager
	public:
		virtual bool IsConvertible(int eid, int player) = 0;
		virtual bool IsAnimal(int eid) = 0;
		virtual bool IsMercenaryFree(int eid, int player) = 0;
		virtual int MercenarySettlerGetBuilding(int id) = 0; //40
		virtual int MouseoverEntityToTargetEntity(int id, int player) = 0;
		virtual int CheckSniperTarget(int sniper, int target, int sniperplayer) = 0;
		virtual int CheckShurikenTarget(int shur, int tar, int shurplay) = 0;
		virtual bool IsNotHostile(int id, int player) = 0;
		virtual int GetHealth(int id) = 0; // 45
		virtual int GetMaxHealth(int id) = 0;
		virtual int GetPlayer(int id) = 0;
		virtual int BuildingGetTargetableId(int id) = 0;
	private:
		virtual void unknown6() = 0; // something entityiterator
		virtual void unknown7() = 0; // 50
		virtual void unknown8() = 0; // get some serf extract res
	public:
		virtual EGL::CGLEEntity* GetEntity(int id) = 0;
	private:
		virtual void unknown9() = 0; // get something building related
		virtual void unknown10() = 0; // building get military info 4
	public:
		virtual void RoundPosToBuildingPlacement(float x, float y, float* xout, float* yout) = 0; //55
		virtual void FillSoldiersOfLaeder(int id, shok::Vector<int>* soldiers) = 0;
	private:
		virtual void unknown12() = 0;
		virtual void unknown13() = 0;
	public:
		virtual bool IsBuildingType(int id) = 0;
		virtual GGL::PlayerTributesManager* GetTributeManager(int player) = 0; //60
		virtual GGL::PlayerQuestManager* GetQuestManager(int player) = 0;
		virtual int* GetDiplomacyData(int player) = 0;
		virtual GGL::CPlayerStatus* GetPlayerStatus(int player) = 0;
	private:
		virtual shok::ResourceType GetResourceTypeProvidedBy2(int eid) = 0; // same func as above
		virtual void unknown14() = 0; //65
		virtual void unknown15() = 0;
		virtual void unknown16() = 0;
	public:
		virtual int GetSector(int entityid) = 0;
		virtual int GetSector(const shok::Position* p) = 0;
		virtual bool IsEntityInSector(int eid, int secor) = 0; //70
		virtual bool GetTechUsedForStatistics(int techid) = 0;
	private:
		virtual void unknown17() = 0; //search some pos?
	public:
		virtual bool IsPositionExploredByPlayer(int pid, const shok::Position* p) = 0;
		virtual int GetWorkCycleId(const char* name) = 0;
		virtual int GetPrincipalTask(int tid) = 0; //75
		virtual bool IsThief(int id) = 0;
		virtual bool IsThiefCarryingSomething(int id) = 0;
	private:
		virtual void unknown18() = 0;
		virtual void unknown19() = 0;
	public:
		virtual bool IsBridge(int id) = 0; //80
		virtual bool IsBridgeTargetedBySabotage(int id) = 0;
		virtual bool CanKegGetDisarmed(int id) = 0;

		static inline constexpr int vtp = 0x76D79C;
	};
	//constexpr int i = offsetof(IGLGUIInterface::UIData, Entity) / 4;
	class CGLGUIInterface : public IGLGUIInterface {
	public:

		using IGLGUIInterface::GetNearestFreePosForBuildingPlacement;

		bool GetNearestFreePosForBuildingPlacement(int ety, const shok::Position& inp, shok::Position& outp);

		static inline constexpr int vtp = 0x76D9A4;

		static void HookFillDataHealth();
	};
}

namespace GGUI {
	class C3DViewHandler : public BB::IObject {
	public:
		GGUI::CState* CurrentState;
		BB::CIDManager* StateIdManager;
		int* IPicker; // ERwTools::IPicker
		int* IPlacer; // ERwTools::IPlacer
		ERwTools::CRpClumpRenderable* ClumpRenerable;
		PADDINGI(2); // probably pos mouseover
		int MouseX, MouseY;

		void SetGUIStateByIdentifier(unsigned int identifier, const GGUI::SStateParameters* p = nullptr);
		template<class T>
		requires std::derived_from<T, GGUI::CState>
		void SetGUIState(const GGUI::SStateParameters* p = nullptr) {
			SetGUIStateByIdentifier(T::Identifier, p);
		}
		void SetGUIStateByIdentfierOnNextUpdate(unsigned int identifier);
		template<class T>
		requires std::derived_from<T, GGUI::CState>
		void SetGUIStateByIdentfierOnNextUpdate() {
			SetGUIStateByIdentfierOnNextUpdate(T::Identifier);
		}

		static inline constexpr int vtp = 0x77B8E0;
	};

	class IMouseEffect : public BB::IPostEvent {

	};
	class CMouseEffect : public IMouseEffect {
	public:
		static inline constexpr int vtp = 0x77BEE0;
	};
	class IMouseCursorAppearance {
		virtual void unknown() = 0;
	};
}

class CMouseCursorManager : public GGUI::IMouseCursorAppearance {
public:
	static inline constexpr int vtp = 0x761BBC;
};

namespace GGUI {
	class CManager { // size 26*4
		virtual void unknown() = 0;
	public:

		struct StateIdData {
			shok::EntityCategory ApplyTo;
			GGUI::CBasicState* State;
		};
		struct SelectionData {
			int Id;
			bool IsSoldier;
		};

		PADDINGI(5);
		GGUI::C3DViewHandler* C3DViewHandler; // 6
		EGL::CGLEGUIInterface* EGUIInterface;
		GGL::CGLGUIInterface* GUIInterface; // 8
		EGL::CGLETerrainHiRes* TerrainHiRes;
	private:
		BB::IPostEvent* PostEvent;
	public:
		shok::Vector<SelectionData> SelectedEntities; //11
		int ControlledPlayer; // 15
		bool CanSelectEntitiesOfPlayer[9];
		shok::Vector<StateIdData*> CommandStates; // 19
		CMouseEffect* MouseEffect;
		lua_State* GameState; // 24
		CMouseCursorManager* MouseCursorManager;

	public:
		static inline constexpr int vtp = 0x77B2F8;

		// select entity standard thiscall 0x721EF6(selectedVector*, id)
		// select entity solier of leader thiscall 0x721F12(selectedVector*, id)

		void SetControlledPlayer(int pl);
		bool IsEntitySelected(int id) const;
		bool SelectEntity(int id); // returns successful
		bool DeselectEntity(int id); // returns successful
		bool ClearSelection(); // returns successful
		void OnSelectionChanged(); // calls lua+guistate, has to be called manually after changing selection
		// checks if entity is of category and GGUI::CBasicState::CheckCommandValid
		bool IsCommandStateValid(StateIdData* s, int entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata);
		// goes through CommandStates and returns the first valid one
		GGUI::CBasicState* GetCommandStateFor(int entity, GGUI::CBasicState::TargetData* tdata, GGUI::CBasicState::ExecuteData* edata);

		void HackPostEvent();

		static inline GGUI::CManager* (__cdecl* const GlobalObj)() = reinterpret_cast<GGUI::CManager * (__cdecl*)()>(0x525622);
		static bool(*PostEventCallback)(BB::CEvent* ev);

		static inline void(__cdecl* const PostEventFromUI)(BB::CEvent* ev) = reinterpret_cast<void(__cdecl*)(BB::CEvent*)>(0x525D77);

		// checks if entity is selectable, including player, selectable flag, ...
		static inline int(__cdecl* const GetSelectableEntity)(int id, bool soldierToLeader) = reinterpret_cast<int(__cdecl*)(int, bool)>(0x525B3E);

		static bool IsModifierPressed(shok::Keys modif);
	};
	//constexpr int i = offsetof(CManager, CommandStates) / 4;
}

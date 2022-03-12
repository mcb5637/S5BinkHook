#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_maplogic.h"
#include "s5_defines.h"
#include "s5_player.h"

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

namespace shok {
	static inline HWND* MainWindowHandle = reinterpret_cast<HWND*>(0x84ECC4);

	const char* (__cdecl* const GetStringTableText)(const char* key) = reinterpret_cast<const char* (__cdecl* const)(const char*)>(0x556D2E);
	extern const char* (*GetStringTableTextOverride)(const char* s);
	void HookGetStringTableText();

	void HookTextPrinting();
}

namespace ERwTools {
	class IRenderable {
	public:
		virtual void SetBuildingRedColor(bool r) = 0;
	private:
		virtual void uk0() = 0;
		virtual void uk1() = 0;
		virtual void uk2() = 0;
	public:
		virtual void SetPosition(float x, float y, float z) = 0;
	private:
		virtual void uk3() = 0;
	protected:
		virtual void SetModelData(void* model, float rot) = 0;
	};
	class CRpClumpRenderable : public IRenderable {
	public:
		int RenderMode; // red color?
		ED::ModelInstance* Model;
		void* RenderObj; // from ED::CWorld

		static inline constexpr int vtp = 0x7AEBF8;

		using IRenderable::SetModelData;
		using IRenderable::SetPosition;

		void SetModelData(const ED::ModelData* modeldata, float rotation);
		void SetPosition(const shok::Position& p, float z);
	};

}

namespace EGL {
	class IGLEGUIInterface {
	public:
		virtual ~IGLEGUIInterface() = default;
		// 12 more vfunc
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
	private:
		virtual void unknown0() = 0; // fill some data, maybe name related?
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
	private:
		virtual void unknown11() = 0;
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
	class CGLGUIInterface : public IGLGUIInterface {
	public:

		using IGLGUIInterface::GetNearestFreePosForBuildingPlacement;

		bool GetNearestFreePosForBuildingPlacement(int ety, const shok::Position& inp, shok::Position& outp);

		static inline constexpr int vtp = 0x76D9A4;
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

		void SetGUIStateByIdentifier(unsigned int identifier);
		template<class T>
		requires std::derived_from<T, GGUI::CState>
		void SetGUIState() {
			SetGUIStateByIdentifier(T::Identifier);
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
			int Id;
			GGUI::CState* State;
		};

		PADDINGI(5);
		GGUI::C3DViewHandler* C3DViewHandler; // 6
		EGL::CGLEGUIInterface* EGUIInterface;
		GGL::CGLGUIInterface* GUIInterface; // 8
		EGL::CGLETerrainHiRes* TerrainHiRes;
	private:
		BB::IPostEvent* PostEvent;
	public:
		shok::Vector<int> SelectedEntities; //11
		int ControlledPlayer; // 15
		PADDINGI(3);
		shok::Vector<StateIdData*> CommandStates;
		CMouseEffect* MouseEffect;
		lua_State* GameState;
		CMouseCursorManager* MouseCursorManager;

	public:
		static inline constexpr int vtp = 0x77B2F8;

		void HackPostEvent();

		static inline GGUI::CManager* (__cdecl* const GlobalObj)() = reinterpret_cast<GGUI::CManager * (__cdecl*)()>(0x525622);
		static bool(*PostEventCallback)(BB::CEvent* ev);


		static bool IsModifierPressed(shok::Keys modif);
	};
}

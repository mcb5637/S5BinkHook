#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace GGUI {
	struct SStateParameters {
		virtual ~SStateParameters() = default;

		static inline constexpr int vtp = 0x77DBAC;
		static inline constexpr int TypeDesc = 0x82EB84;
	};
	struct SPlaceCannonStateParameters : public SStateParameters {
		shok::EntityTypeId TopType;
		shok::EntityTypeId FoundationType;

		static inline constexpr int vtp = 0x77DC04;
		static inline constexpr int TypeDesc = 0x82EBA8;
	};
	struct SPlaceBuildingStateParameters : public SStateParameters {
		shok::UpgradeCategoryId UCat;

		static inline constexpr int vtp = 0x77DCA0;
		static inline constexpr int TypeDesc = 0x82EC8C;

		SPlaceBuildingStateParameters(shok::UpgradeCategoryId ucat);
	};


	class CState : public BB::IObject { // no vtable
	public:
		GGUI::C3DViewHandler* C3DViewHandler = nullptr;

		static inline constexpr int TypeDesc = 0x82CC10;

		virtual bool OnMouseEvent(BB::CEvent* ev) = 0; // return event handled?
		virtual void SetStateParameters(SStateParameters* p);
		virtual bool Cancel() = 0; // no idea what the return actually is for, seems to return always 1
		virtual const char* GetName() = 0;
		virtual void OnSelectionChanged(int z); //on selection changed call with 0 -> cancel state
	};

	class CBasicState : public GGUI::CState { // no vtable
	public:
		static inline constexpr int TypeDesc = 0x82CC2C;

		struct TargetData {
			shok::EntityId TargetID = {};
			shok::Position TargetPos;
			shok::PositionRot TargetPosWithZ;
			PADDINGI(2);

			void FillPosWithZFromPos();
		};
		struct ExecuteData {
			shok::EntityId CurrentID;
			unsigned int CurrentNumber;
			shok::EntityId FirstID;
			unsigned int NumberOfEntities;
		};

		virtual bool OnMouseEvent(BB::CEvent* ev) override;
		virtual bool Cancel() override;

		virtual bool CheckCommandValid(TargetData* d, int z) = 0;
		virtual void ExecuteCommand(TargetData* d, ExecuteData* selectedID) = 0; // check if that id is actually the parameter, might also be 0?
		virtual TargetData* GetTargetData(TargetData* d, int x, int y) = 0; // write data into d, then return d
		virtual void OnMouseMove(int x, int y);
		virtual bool OnCancel() = 0; // Cancel redirects here, no idea why we need a 2nd method for that...

		void FillPosData(TargetData* d, int x, int y);
		void FillEntityData(TargetData* d, int x, int y);
	};
	static_assert(sizeof(CBasicState::TargetData) == 8 * 4);

	class CCommandState : public GGUI::CBasicState { // no vtable
	public:
		shok::EntityId ExclusiveRecipientId;

		static inline constexpr int TypeDesc = 0x82CC88;
	};

	class CPositionCommandState : public GGUI::CCommandState { // no vtable
	public:
		static inline constexpr int TypeDesc = 0x82CDFC;

		virtual TargetData* GetTargetData(TargetData* d, int x, int y) override;

		virtual void ExecuteForPosAndEntity(int id, float x, float y, float r) = 0;
	};
	class CWalkCommandState : public GGUI::CPositionCommandState {
	public:
		static inline constexpr int vtp = 0x77B300;
		static inline constexpr int TypeDesc = 0x82CE74;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC6BF8157);
	};
	class CPlaceTorchState : public GGUI::CWalkCommandState {
	public:
		static inline constexpr int vtp = 0x77D4CC;
		static inline constexpr int TypeDesc = 0x82E30C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xA55B7457);
	};
	class CPatrolCommandState : public GGUI::CWalkCommandState {
	public:
		static inline constexpr int vtp = 0x77D698;
		static inline constexpr int TypeDesc = 0x82E50C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC2DE9816);
	};
	class CPlaceBombCommandState : public GGUI::CWalkCommandState {
	public:
		static inline constexpr int vtp = 0x77D700;
		static inline constexpr int TypeDesc = 0x82E5A8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2AEB8F66);
	};
	class CAttackMoveCommandState : public GGUI::CWalkCommandState {
	public:
		static inline constexpr int vtp = 0x77D7D4;
		static inline constexpr int TypeDesc = 0x82E6A4;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x10BEA906);
	};
	class CScoutBinocularsCommandState : public GGUI::CPositionCommandState {
	public:
		static inline constexpr int vtp = 0x77CCD8;
		static inline constexpr int TypeDesc = 0x82DC54;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x19F674B7);
	};
	class CSendHawkCommandState : public GGUI::CPositionCommandState {
	public:
		static inline constexpr int vtp = 0x77CD50;
		static inline constexpr int TypeDesc = 0x82DCD8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC7454326);
	};

	class CEntityCommandState : public GGUI::CCommandState { // no vtable
	public:
		static inline constexpr int TypeDesc = 0x82CCAC;

		virtual TargetData* GetTargetData(TargetData* d, int x, int y) override;
	};
	class CStealGoodsState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B00C;
		static inline constexpr int TypeDesc = 0x82CCD4;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x112C3B07);
	};
	class CSecureStolenGoodsState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B058;
		static inline constexpr int TypeDesc = 0x82CCF8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC455B817);
	};
	class CDisarmKegCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B0AC;
		static inline constexpr int TypeDesc = 0x82CD24;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7355BF67);
	};
	class CNPCInteractionState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B0F8;
		static inline constexpr int TypeDesc = 0x82CD50;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE1E52B17);
	};
	class CAttackCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B140;
		static inline constexpr int TypeDesc = 0x82CD78;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x816584DD);
	};
	class CSerfConstructBuildingState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B188;
		static inline constexpr int TypeDesc = 0x82CDA0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9992B87);
	};
	class CSerfRepairBuildingState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B1E0;
		static inline constexpr int TypeDesc = 0x82CDD0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x69AEF107);
	};
	class CSerfExtractResourceState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77B234;
		static inline constexpr int TypeDesc = 0x82CE28;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xE9DAEB87);
	};
	class CShurikenCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77BF80;
		static inline constexpr int TypeDesc = 0x82D840;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xCE36524D);
	};
	class CSnipeCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77BFE8;
		static inline constexpr int TypeDesc = 0x82D8C0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7A00C4AD);
	};
	class CPlaceKegCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D538;
		static inline constexpr int TypeDesc = 0x82E384;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xEB1BB807);
	};
	class CConvertSettlerCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D5A0;
		static inline constexpr int TypeDesc = 0x82E404;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x50E03DCD);
	};
	class CConvertBuildingCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D614;
		static inline constexpr int TypeDesc = 0x82E488;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5E868D4D);
	};
	class CGuardCommandState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D770;
		static inline constexpr int TypeDesc = 0x82E628;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x4ABFC876);
	};
	class CBlessSettlerState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D844;
		static inline constexpr int TypeDesc = 0x82E724;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x6027D046);
	};
	class CForceWorkerToWorkState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D8F0;
		static inline constexpr int TypeDesc = 0x82E818;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xDF71DC86);
	};
	class CExpelSettlerState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D948;
		static inline constexpr int TypeDesc = 0x82E898;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF8794A47);
	};
	class CBuySoldierState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77D9AC;
		static inline constexpr int TypeDesc = 0x82E914;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xB982B848);
	};
	class CBuyLeaderState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77DA08;
		static inline constexpr int TypeDesc = 0x82E98C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x45B1258);
	};
	class CUpgradeSingleBuildingState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77DA64;
		static inline constexpr int TypeDesc = 0x82EA04;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x39B1467);
	};
	class CUpgradeBuildingState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77DAE0;
		static inline constexpr int TypeDesc = 0x82EA88;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x95FAD777);
	};
	class CSellBuildingState : public GGUI::CEntityCommandState {
	public:
		static inline constexpr int vtp = 0x77DB48;
		static inline constexpr int TypeDesc = 0x82EB08;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8FF51827);
	};

	class CPlaceCannonState : public GGUI::CCommandState {
	public:
		PADDINGI(2);
		shok::EntityTypeId TopType; // 5
		shok::EntityTypeId FoundationType;

		static inline constexpr int vtp = 0x77DC0C;
		static inline constexpr int TypeDesc = 0x82EBD8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8BF90FCD);
	};
	class CPlaceBuildingState : public GGUI::CBasicState {
	public:
		shok::UpgradeCategoryId UpgradeCategory = {};
		shok::Position PosToBuild;
		int MouseX = 0, MouseY = 0; // screenpos

		void UpdateModel();
		void SetModelToRender();
		void SetModelToRender(RWE::RpClump* c, float r);

		CPlaceBuildingState();
		virtual ~CPlaceBuildingState() override;
		virtual void SetStateParameters(SStateParameters* p) override;
		virtual void OnSelectionChanged(int z) override;
		virtual bool CheckCommandValid(TargetData* d, int z) override;
		virtual void ExecuteCommand(TargetData* d, ExecuteData* selectedID) override;
		virtual TargetData* GetTargetData(TargetData* d, int x, int y) override;
		virtual void OnMouseMove(int x, int y) override;
		virtual bool OnCancel() override;

		static inline constexpr int vtp = 0x77DCA8;
		static inline constexpr int TypeDesc = 0x82ECC0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x956CDD97);

		// maybe increase the objects size and make this not global?
		static float PlacementRotation;
		static [[deprecated]] void HookPlacementRotation();

		static shok::PositionRot GetNearestPlacementPosBuildOn(shok::EntityTypeId ety, const shok::Position& p, float range);
		static shok::PositionRot GetNearestPlacementPosFree(shok::EntityTypeId ety, const shok::PositionRot& p, float range);
		static shok::PositionRot GetNearestPlacementPos(shok::EntityTypeId ety, const shok::PositionRot& p, float range);
	};
	static_assert(sizeof(GGUI::CPlaceBuildingState) / 4 == 7);

	class CSelectionState : public GGUI::CState {
	public:
		static inline constexpr int vtp = 0x77B90C;
		static inline constexpr int TypeDesc = 0x82D03C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD3F98EE7);
		// 1 extra (empty) func in vtable

		static void HookFixDoubleClickSelection();
	};
	class CCutsceneState : public GGUI::CState {
	public:
		static inline constexpr int vtp = 0x77D8A8;
		static inline constexpr int TypeDesc = 0x82E7A0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xD33F4188);
	};

}

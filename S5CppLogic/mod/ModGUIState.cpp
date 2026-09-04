#include "ModGUIState.h"

#include <shok/s5_scriptsystem.h>
#include <shok/engine/s5_RWEngine.h>
#include <shok/events/s5_events.h>
#include <shok/events/s5_netevents.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/globals/s5_config.h>
#include <shok/ui/s5_ui.h>

namespace CppLogic::Mod::GUIState {
	void* GUIState_LuaSelection::operator new(size_t s) {
		return shok::Malloc(s);
	}
	void GUIState_LuaSelection::operator delete(void* p) {
		shok::Free(p);
	}

	GUIState_LuaSelection::~GUIState_LuaSelection() {
		luaext::State L{*EScr::CScriptTriggerSystem::GameState};
		if (RefOnKlick != luaext::State::NoRef)
			L.UnRef(RefOnKlick, luaext::State::RegistryIndex);
		if (RefOnCancel != luaext::State::NoRef)
			L.UnRef(RefOnCancel, luaext::State::RegistryIndex);
	}

	shok::ClassId __stdcall GUIState_LuaSelection::GetClassIdentifier() const {
		return GUIState_LuaSelection::Identifier;
	}

	bool GUIState_LuaSelection::OnMouseEvent(BB::CEvent* ev) {
		auto* mev = BB::IdentifierCast<BB::CMouseEvent>(ev);
		if (mev && mev->IsEvent(shok::InputEventIds::MouseButtonDown)) {
			if (mev->IsKey(shok::Keys::MouseLButton)) {
				bool r = true;
				if (RefOnKlick != luaext::State::NoRef) {
					luaext::State L{*EScr::CScriptTriggerSystem::GameState};
					int i = L.GetTop();
					L.Push(RefOnKlick, luaext::State::RegistryIndex);
					L.Push(mev->X);
					L.Push(mev->Y);
					L.PCall(2, 1, 0);
					if (L.IsBoolean(-1))
						r = L.ToBoolean(-1);
					L.SetTop(i);
				}
				if (r)
					Cancel(false);
				return r;
			}
			else if (mev->IsKey(shok::Keys::MouseRButton)) { // rmb
				Cancel(true);
				return true;
			}
		}
		return false;
	}

	bool GUIState_LuaSelection::Cancel() {
		Cancel(true);
		return true;
	}

	const char* GUIState_LuaSelection::GetName() {
		return Name;
	}


	void GUIState_LuaSelection::Initialize() {
		(*BB::CClassFactory::GlobalObj)->AddClassToFactory<GUIState_LuaSelection>();
	}

	void GUIState_LuaSelection::Cancel(bool calllua) {
		if (calllua && RefOnCancel != luaext::State::NoRef) {
			luaext::State L{*EScr::CScriptTriggerSystem::GameState};
			int i = L.GetTop();
			L.Push(RefOnCancel, luaext::State::RegistryIndex);
			L.PCall(0, 0, 0);
			L.SetTop(i);
		}
		C3DViewHandler->SetGUIStateByIdentfierOnNextUpdate<GGUI::CSelectionState>();
	}

	void GUIState_PlaceBuildingEx::Initialize() {
		(*BB::CClassFactory::GlobalObj)->AddClassToFactory<GUIState_PlaceBuildingEx>();
	}

	void* GUIState_PlaceBuildingEx::operator new(size_t s) {
		return shok::Malloc(s);
	}
	void GUIState_PlaceBuildingEx::operator delete(void* p) {
		shok::Free(p);
	}

	shok::ClassId __stdcall GUIState_PlaceBuildingEx::GetClassIdentifier() const {
		return Identifier;
	}

	const char* GUIState_PlaceBuildingEx::GetName() {
		return Name;
	}

	void GUIState_PlaceBuildingEx::SetStateParameters(GGUI::SStateParameters* p) {
		GGUI::CPlaceBuildingState::SetStateParameters(p);
		ERwTools::CDefCameraBehaviour::HookEnableZoom(false);
	}

	bool GUIState_PlaceBuildingEx::OnMouseEvent(BB::CEvent* ev) {
		if (auto* e = BB::IdentifierCast<BB::CMouseEvent>(ev)) {
			if (e->IsEvent(shok::InputEventIds::MouseWheel)) {
				bool rotate = false;
				if (ScrollRotate) {
					if (ScrollModifiers == shok::Keys::None) {
						rotate = !e->IsModifier(shok::Keys::ModifierControl);
					}
					else {
						rotate = e->IsModifier(ScrollModifiers);
					}
				}
				if (!rotate) {
					(*ERwTools::CRwCameraHandler::GlobalObj)->ScrollWheelZoom(e->Delta);
					e->EventHandled = true;
					return true;
				}
				else {
					if (e->Delta < 0) {
						CurrentStep = (CurrentStep + 1) % NumSteps;
					}
					else {
						CurrentStep--;
						while (CurrentStep < 0)
							CurrentStep += NumSteps;
					}
					PosToBuild = {};
					UpdateModel(e->X, e->Y);
					e->EventHandled = true;
					return true;
				}
			}
			else if (e->IsEvent(shok::InputEventIds::MouseButtonDown)) {
				Chain = e->IsModifier(shok::Keys::ModifierControl);
			}
		}

		return GGUI::CPlaceBuildingState::OnMouseEvent(ev);
	}

	bool GUIState_PlaceBuildingEx::CheckCommandValid(TargetData* d, int z) {
		if (UpgradeCategory == shok::UpgradeCategoryId::Invalid)
			return false;
		if (d->TargetPos.X <= 0 || d->TargetPos.Y <= 0)
			return false;
		auto* m = GGUI::CManager::GlobalObj();
		auto* i = m->GUIInterface;
		auto ety = i->GetBuildingTypeByUCat(m->ControlledPlayer, UpgradeCategory);
		if (!i->CheckBuildingPlacementAndCost(m->ControlledPlayer, ety, d->TargetPos.X, d->TargetPos.Y, CppLogic::DegreesToRadians(GetRotation())))
			return false;
		bool hasSector = false;
		auto sector = i->GetSector(&d->TargetPos);
		for (const auto& e : m->SelectedEntities) {
			if (i->IsSerf(e.Id)) {
				auto serf_sect = i->GetSectorOfEntity(e.Id);
				if (d->TargetID != shok::EntityId::Invalid) {
					if (i->IsEntityInSector(d->TargetID, serf_sect)) {
						hasSector = true;
						break;
					}
				}
				else {
					if (sector == serf_sect) {
						hasSector = true;
						break;
					}
				}
			}
		}
		if (!hasSector)
			return false;
		if (!i->IsPositionExploredByPlayer(m->ControlledPlayer, &d->TargetPos))
			return false;
		return true;
	}

	void GUIState_PlaceBuildingEx::ExecuteCommand(TargetData* d, ExecuteData* selectedID) {
		if (selectedID->CurrentID == selectedID->FirstID) {
			if (CheckCommandValid(d, 0)) {
				auto m = GGUI::CManager::GlobalObj();
				size_t num = std::numeric_limits<size_t>::max();
				if (Chain) {
					auto i = m->GUIInterface;
					auto ety = i->GetBuildingTypeByUCat(m->ControlledPlayer, UpgradeCategory);
					auto* bt = GetEntityType(ety);
					if (auto* l = dynamic_cast<GGL::CGLBuildingProps*>(bt->LogicProps)) {
						num = l->ConstructionInfo.BuilderSlot.size();
					}
				}
				GGL::CNetEventBuildingCreator ev{shok::NetEventIds::Player_BuyBuilding, m->ControlledPlayer, UpgradeCategory,
												 shok::PositionRot{d->TargetPos.X, d->TargetPos.Y, CppLogic::DegreesToRadians(GetRotation())}};
				{
					auto v = ev.Serf.SaveVector();
					for (const auto& se : m->SelectedEntities) {
						if (m->GUIInterface->IsSerf(se.Id)) {
							v.Vector.push_back(se.Id);
							if (--num == 0)
								break;
						}
					}
				}
				GGUI::CManager::PostEventFromUI(&ev);
				if (Chain) {
					for (auto s : ev.Serf) {
						m->DeselectEntity(s);
					}
					m->OnSelectionChanged();
					PosToBuild = {};
					UpdateModel(MouseX, MouseY);
					return;
				}
				luaext::State L{m->GameState};
				int top = L.GetTop();
				L.GetGlobal("GameCallback_GUI_AfterBuildingPlacement");
				L.PCall(0, 0);
				L.SetTop(top);
				SetModelToRender(nullptr, 0);
			}
		}
	}

	GGUI::CBasicState::TargetData* GUIState_PlaceBuildingEx::GetTargetData(TargetData* d, int x, int y) {
		auto m = GGUI::CManager::GlobalObj();
		FillPosData(d, x, y);
		{
			TargetData d2{};
			FillEntityData(&d2, x, y);
			d->TargetID = d2.TargetID;
		}
		d->TargetPos.FloorToBuildingPlacement();
		if (UpgradeCategory != shok::UpgradeCategoryId::Invalid) {
			auto ety = m->GUIInterface->GetBuildingTypeByUCat(m->ControlledPlayer, UpgradeCategory);
			if (static_cast<int>(d->TargetPos.X) == static_cast<int>(PosToBuild.X) && static_cast<int>(d->TargetPos.Y) == static_cast<int>(PosToBuild.X)) {
				d->TargetPos = PosToBuild;
			}
			else {
				auto* lp = *GGL::CLogicProperties::GlobalObj;
				float snap = lp->BuildingPlacementSnapDistance;
				auto [p, id] = GetNearestPlacementPos(ety, shok::PositionRot{d->TargetPos.X, d->TargetPos.Y, CppLogic::DegreesToRadians(GetRotation())}, snap);
				if (p.X >= 0) {
					d->TargetPos.X = p.X;
					d->TargetPos.Y = p.Y;
					SetRotation(CppLogic::RadiansToDegrees(p.r));
					if (id != shok::EntityId::Invalid)
						d->TargetID = id;
				}
				PosToBuild = d->TargetPos;
			}
		}
		d->FillPosWithZFromPos();
		return d;
	}

	void GUIState_PlaceBuildingEx::OnMouseMove(int x, int y) {
		MouseX = x;
		MouseY = y;
		UpdateModel(x, y);
	}

	bool GUIState_PlaceBuildingEx::OnCancel() {
		ERwTools::CDefCameraBehaviour::HookEnableZoom(true);
		return GGUI::CPlaceBuildingState::OnCancel();
	}

	void GUIState_PlaceBuildingEx::UpdateModel(int x, int y) {
		if (UpgradeCategory == shok::UpgradeCategoryId::Invalid)
			return;
		SetModelToRender();
		TargetData d{};
		GetTargetData(&d, x, y);
		C3DViewHandler->ClumpRenerable->Model->GetFrame()->Rotate(GetRotation(), RWE::RwOpCombineType::Replace);
		C3DViewHandler->ClumpRenerable->SetBuildingRedColor(!CheckCommandValid(&d, 0));
		C3DViewHandler->ClumpRenerable->SetPosition(d.TargetPos, d.TargetPosWithZ.r);
	}

	float GUIState_PlaceBuildingEx::GetRotation() const {
		return StepToDegrees * static_cast<float>(CurrentStep);
	}

	void GUIState_PlaceBuildingEx::SetRotation(float deg) {
		CurrentStep = static_cast<int>(std::roundf(deg / StepToDegrees));
	}

	void GUIState_PlaceBuildingEx::OnRotationChanged() {
		PosToBuild = {};
		UpdateModel(MouseX, MouseY);
	}
} // namespace CppLogic::Mod::GUIState

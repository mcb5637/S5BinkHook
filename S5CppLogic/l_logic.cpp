#include "pch.h"
#include "l_logic.h"
#include <assert.h>
#include <filesystem>
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_config.h"
#include "s5_idmanager.h"
#include "s5_maplogic.h"
#include "s5_player.h"
#include "s5_cutscene.h"
#include "s5_ui.h"
#include "s5_scriptsystem.h"
#include "s5_entitytype.h"
#include "s5_entity.h"
#include "s5_netevents.h"
#include "s5_filesystem.h"
#include "s5_exception.h"
#include "s5_mapdisplay.h"
#include "s5_defines.h"
#include "s5_guistates.h"
#include "s5_tasklist.h"
#include "s5_RWEngine.h"
#include "luaext.h"
#include "hooks.h"
#include "luaserializer.h"

namespace CppLogic::Logic {
	int GetDamageFactor(lua::State ls) {
		luaext::EState L{ ls };
		int dmgclass = L.CheckInt(1);
		int size = static_cast<int>((*GGL::DamageClassesHolder::GlobalObj)->DamageClassList.size());
		if (dmgclass <= 0 || dmgclass >= size)
			throw lua::LuaException("invalid damagecass");
		int amclass = L.CheckInt(2);
		L.Push((*GGL::DamageClassesHolder::GlobalObj)->DamageClassList[dmgclass]->GetBonusVsArmorClass(amclass));
		return 1;
	}
	int SetDamageFactor(lua::State ls) {
		luaext::EState L{ ls };
		int dmgclass = L.CheckInt(1);
		int size = static_cast<int>((*GGL::DamageClassesHolder::GlobalObj)->DamageClassList.size());
		if (dmgclass <= 0 || dmgclass >= size)
			throw lua::LuaException("invalid damagecass");
		int amclass = L.CheckInt(2);
		(*GGL::DamageClassesHolder::GlobalObj)->DamageClassList[dmgclass]->GetBonusVsArmorClass(amclass) = L.CheckFloat(2);
		return 0;
	}

	int ReloadCutscene(lua::State ls) {
		luaext::EState L{ ls };
		const char* data = L.OptString(1, "Maps\\ExternalMap");
		(**ECS::CManager::GlobalObj)->ReloadCutscene(data);
		return 0;
	}

	int GetAnimIdFromName(lua::State ls) {
		luaext::EState L{ ls };
		const char* data = L.CheckString(1);
		int id = (*BB::CIDManagerEx::AnimManager)->GetIdByName(data);
		L.Push(id);
		return 1;
	}

	int PlayerGetPaydayStartetTick(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (p->PlayerAttractionHandler->PaydayActive)
			L.Push(p->PlayerAttractionHandler->PaydayFirstOccuraceGameTurn);
		else
			L.Push(-1);
		return 1;
	}
	int PlayerSetPaydayStartetTick(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		int st = L.CheckInt(2);
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (st < 0) {
			p->PlayerAttractionHandler->PaydayActive = 0;
		}
		else {
			p->PlayerAttractionHandler->PaydayActive = 1;
			p->PlayerAttractionHandler->PaydayFirstOccuraceGameTurn = st;
		}
		return 0;
	}
	int SetPaydayFrequency(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0)
			throw lua::LuaException("freq has to be > 0");
		(*GGL::CPlayerAttractionProps::GlobalObj)->PaydayFrequency = i;
		return 0;
	}


	int NetEventReadBack(lua::State L) {
		BB::CEvent* ev = static_cast<BB::CEvent*>(L.ToUserdata(L.Upvalueindex(1)));
		L.PushValue(1);
		L.Push("EventType");
		L.Push();
		L.SetTableRaw(-3);
		if (EGL::CNetEventEntityAndPosArray* e = dynamic_cast<EGL::CNetEventEntityAndPosArray*>(ev)) {
			L.Push("Position");
			L.GetTableRaw(-2);
			if (!L.IsNil(-1)) {
				auto v = e->Position.SaveVector();
				v.Vector.clear();
			}
			L.Pop(1);
		}
		if (GGL::CNetEventBuildingCreator* e = dynamic_cast<GGL::CNetEventBuildingCreator*>(ev)) {
			L.Push("Serf");
			L.GetTableRaw(-2);
			if (!L.IsNil(-1)) {
				auto v = e->Serf.SaveVector();
				v.Vector.clear();
			}
			L.Pop(1);
		}
		CppLogic::Serializer::LuaSerializer::Deserialize(L, ev);
		return 0;
	}

	int NetEventSetHook(lua::State L) {
		if (!L.IsFunction(1))
			throw lua::LuaException("no func");
		L.PushLightUserdata(&NetEventSetHook);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);
		GGUI::CManager::PostEventCallback = [](BB::CEvent* ev) {
			int id = ev->EventTypeId;
			lua::State L{ *EScr::CScriptTriggerSystem::GameState };
			int top = L.GetTop();
			L.PushLightUserdata(&NetEventSetHook);
			L.GetTableRaw(L.REGISTRYINDEX);
			bool r = false;
			if (L.IsFunction(-1)) {
				L.Push(id);
				CppLogic::Serializer::LuaSerializer::Serialize(L, ev);
				L.PushLightUserdata(ev);
				L.Push<NetEventReadBack>(1);
				L.PCall(3, 1, 0);
				if (L.IsBoolean(-1))
					r = L.ToBoolean(-1);
			}
			L.SetTop(top);
			return r;
		};
		GGUI::CManager::GlobalObj()->HackPostEvent();
		return 0;
	}
	int NetEventUnSetHook(lua::State L) {
		GGUI::CManager::PostEventCallback = nullptr;
		return 0;
	}


	int PlayerGetKillStatistics(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		L.Push(p->Statistics.NumberOfUnitsKilled);
		L.Push(p->Statistics.NumberOfUnitsDied);
		L.Push(p->Statistics.NumberOfBuildingsDestroyed);
		L.Push(p->Statistics.NumberOfBuildingsLost);
		return 4;
	}

	int CanPlaceBuildingAt(lua::State ls) {
		luaext::EState L{ ls };
		int ty = L.CheckInt(1);
		GGlue::CGlueEntityProps* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ty);
		if (!ety)
			throw lua::LuaException("no entitytype");
		if (!ety->IsBuildingType())
			throw lua::LuaException("not a building");
		int pl = L.CheckInt(2);
		shok::Position p = L.CheckPos(3);
		p.FloorToBuildingPlacement();
		float r = CppLogic::DegreesToRadians(L.CheckFloat(4));
		if (L.IsNumber(5))
			L.Push(GGL::CPlayerStatus::CanPlaceBuilding(ty, pl, &p, r, L.CheckInt(5)));
		else
			L.Push(GGL::CPlayerStatus::CanPlaceBuildingAtPos(ty, pl, &p, r));
		return 1;
	}

	int PlayerActivateAlarm(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (p->WorkerAlarmMode)
			throw lua::LuaException("alarm active");
		if (p->AlarmRechargeTime > 0)
			throw lua::LuaException("alarm cooldown");
		(*GGL::CGLGameLogic::GlobalObj)->EnableAlarmForPlayer(i);
		return 0;
	}
	int PlayerDeactivateAlarm(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (!p->WorkerAlarmMode)
			throw lua::LuaException("alarm not active");
		(*GGL::CGLGameLogic::GlobalObj)->DisableAlarmForPlayer(i);
		return 0;
	}

	int PlayerUpgradeSettlerCategory(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		int ucat = L.CheckInt(2);
		auto u = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->SettlerUpgradeManager->UpgradeCategories.find(ucat);
		if (u == (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->SettlerUpgradeManager->UpgradeCategories.end())
			throw lua::LuaException("invalid ucat");
		(*GGL::CGLGameLogic::GlobalObj)->UpgradeSettlerCategory(i, ucat);
		return 0;
	}

	int PlayerSetTaxLevel(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		int tl = L.CheckInt(2);
		if (tl < 0 || tl >= 5)
			throw lua::LuaException("invalid taxlevel");
		(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->TaxLevel = tl;
		return 0;
	}

	int PlayerActivateWeatherMachine(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		int w = L.CheckInt(2);
		if (w < 1 || w >= 4)
			throw lua::LuaException("invalid weathertype");
		if ((*GGL::CGLGameLogic::GlobalObj)->WeatherHandler->WeatherChange.StateToChangeTo)
			throw lua::LuaException("weather currently changing");
		if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->CurrentResources.WeatherEnergy < (*GGL::CLogicProperties::GlobalObj)->EnergyRequiredForWeatherChange)
			throw lua::LuaException("not enough weather energy");
		(*GGL::CGLGameLogic::GlobalObj)->PlayerActivateWeathermachine(i, w);
		return 0;
	}

	int PlayerBlessSettlers(lua::State ls) {
		luaext::EState L{ ls };
		int i = L.CheckInt(1);
		if (i <= 0 || i >= 9)
			throw lua::LuaException("invalid player");
		int b = L.CheckInt(2);
		float faithneeded = -1;
		for (GGL::CLogicProperties::SBlessCategory& bcat : (*GGL::CLogicProperties::GlobalObj)->BlessCategories) {
			if (bcat.Name == b) {
				faithneeded = bcat.RequiredFaith;
				break;
			}
		}
		if (faithneeded < 0)
			throw lua::LuaException("invalid blesscategory");
		if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->CurrentResources.Faith < faithneeded)
			throw lua::LuaException("not enough faith");
		(*GGL::CGLGameLogic::GlobalObj)->PlayerBlessSettlers(i, b);
		return 0;
	}

	int LandscapeGetSector(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p));
		return 1;
	}

	int LandscapeGetNearestUnblockedPosInSector(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		int s = L.CheckInt(2);
		float r = L.CheckFloat(3);
		shok::Position po;
		if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetNearestPositionInSector(&p, r, s, &po))
			L.PushPos(po);
		else
			L.Push();
		return 1;
	}

	int EnableAllHurtEntityTrigger(lua::State ls) {
		luaext::EState L{ ls };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity::HookHurtEntity();
		if (L.IsBoolean(1))
			EGL::CGLEEntity::HurtEntityCallWithNoAttacker = L.ToBoolean(1);
		else
			EGL::CGLEEntity::HurtEntityCallWithNoAttacker = true;
		if (L.IsFunction(2)) {
			L.PushLightUserdata(&EnableAllHurtEntityTrigger);
			L.PushValue(2);
			L.SetTableRaw(L.REGISTRYINDEX);

			EGL::CGLEEntity::HurtEntityOnKillCb = [](EGL::CGLEEntity* att, EGL::CGLEEntity* kill, int attpl, shok::AdvancedDealDamageSource sourc) {
				lua::State L{ *EScr::CScriptTriggerSystem::GameState };

				int t = L.GetTop();

				L.PushLightUserdata(&EnableAllHurtEntityTrigger);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(att ? att->EntityId : 0);
				L.Push(kill->EntityId);
				L.Push(attpl);
				L.Push(static_cast<int>(sourc));
				L.PCall(4, 0, 0);

				L.SetTop(t);
			};
		}
		else {
			EGL::CGLEEntity::HurtEntityOnKillCb = nullptr;
			L.PushLightUserdata(&EnableAllHurtEntityTrigger);
			L.Push();
			L.SetTableRaw(L.REGISTRYINDEX);
		}
		return 0;
	}

	int HurtEntityGetDamage(lua::State ls) {
		luaext::EState L{ ls };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		if (!EGL::CGLEEntity::HurtEntityDamagePointer)
			throw lua::LuaException("not in trigger");
		L.Push(*EGL::CGLEEntity::HurtEntityDamagePointer);
		L.Push(static_cast<int>(EGL::CGLEEntity::HurtEntityDamageSource));
		L.Push(EGL::CGLEEntity::HurtEntityAttackerPlayer);
		return 3;
	}
	int HurtEntitySetDamage(lua::State ls) {
		luaext::EState L{ ls };
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		if (!EGL::CGLEEntity::HurtEntityDamagePointer)
			throw lua::LuaException("not in trigger");
		*EGL::CGLEEntity::HurtEntityDamagePointer = L.CheckInt(1);
		return 0;
	}

	int GetLoadOrder(lua::State L) {
		L.NewTable();
		int r = 1;
		for (BB::IFileSystem* a : (*BB::CFileSystemMgr::GlobalObj)->LoadOrder) {
			if (BB::CDirectoryFileSystem* a2 = dynamic_cast<BB::CDirectoryFileSystem*>(a)) {
				L.Push(a2->Path);
				L.SetTableRaw(-2, r);
				r++;
			}
			else if (BB::CBBArchiveFile* a2 = dynamic_cast<BB::CBBArchiveFile*>(a)) {
				L.Push(a2->Path);
				L.SetTableRaw(-2, r);
				r++;
			}
			else {
				L.Push(typeid(*a).name());
				L.SetTableRaw(-2, r);
				r++;
			}
		}
		return 1;
	}

	int AddArchive(lua::State L) {
		const char* s = L.CheckString(1);
		if (!std::filesystem::exists(s))
			throw lua::LuaException("file doesnt exist");
		try {
			(*BB::CFileSystemMgr::GlobalObj)->AddArchive(s);
		}
		catch (const BB::CFileException& e) {
			char buff[201]{};
			e.CopyMessage(buff, 200);
			throw lua::LuaException(buff);
		}
		return 0;
	}

	int RemoveTopArchive(lua::State L) {
		if ((*BB::CFileSystemMgr::GlobalObj)->LoadOrder.size() <= 0)
			throw lua::LuaException("loadorder empty");
		BB::CBBArchiveFile* a = dynamic_cast<BB::CBBArchiveFile*>((*BB::CFileSystemMgr::GlobalObj)->LoadOrder[0]);
		if (!a)
			throw lua::LuaException("may only remove archives");
		std::string_view path = a->Path;
		if (path.starts_with("base") || path.starts_with("extra1") || path.starts_with("extra2"))
			throw lua::LuaException("may not remove BB archives");
		(*BB::CFileSystemMgr::GlobalObj)->RemoveTopArchive();
		return 0;
	}

	int AddFolder(lua::State L) {
		const char* s = L.CheckString(1);
		(*BB::CFileSystemMgr::GlobalObj)->AddFolder(s);
		return 0;
	}

	int EnableMaxHPTechMod(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supportet with SCELoader");
		EGL::CGLEEntity::HookMaxHP();
		EGL::CGLEEntity::UseMaxHPTechBoni = true;
		return 0;
	}

	int LandscapeGetTerrainType(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetTerrainTypeAt(p));
		return 1;
	}
	int LandscapeGetWaterType(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetWaterTypeAt(p));
		return 1;
	}
	int LandscapeGetWaterHeight(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetWaterHeightAt(p));
		return 1;
	}
	int LandscapeGetTerrainHeight(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes->GetTerrainHeight(p));
		return 1;
	}
	int LandscapeGetTerrainVertexColor(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->VertexColors->GetTerrainVertexColor(p));
		return 1;
	}
	int LandscapeGetBlocking(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push(static_cast<int>((*ED::CGlobalsLogicEx::GlobalObj)->GetBlocking(p)));
		return 1;
	}
	int LandscapeGetBridgeHeight(lua::State ls) {
		luaext::EState L{ ls };
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetBridgeHeight(p));
		return 1;
	}

	int GetColorByColorIndex(lua::State L) { // ind -> r,g,b,a
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		int i = L.CheckInt(1);
		shok::Color c = (*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->GetColorByIndex(i);
		L.Push(c.R);
		L.Push(c.G);
		L.Push(c.B);
		L.Push(c.A);
		return 4;
	}
	int SetColorByColorIndex(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		int i = L.CheckInt(1);
		int r = L.CheckInt(2), g = L.CheckInt(3), b = L.CheckInt(4);
		int a = L.OptInteger(5, 255);
		(*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->SetColorByIndex(i, shok::Color(r, g, b, a));
		(*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->RefreshPlayerColors();
		return 0;
	}

	int SetPaydayCallback(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use global GameCallback_PaydayPayed instead");
		if (!L.IsFunction(1))
			throw lua::LuaException("no func");
		GGL::CPlayerAttractionHandler::HookCheckPayday();
		L.PushLightUserdata(&SetPaydayCallback);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);
		GGL::CPlayerAttractionHandler::OnCheckPayDayCallback = [](GGL::CPlayerAttractionHandler* th) {
			lua::State L{ *EScr::CScriptTriggerSystem::GameState };
			int t = L.GetTop();
			L.PushLightUserdata(&SetPaydayCallback);
			L.GetTableRaw(L.REGISTRYINDEX);
			L.Push(th->PlayerID);
			L.Push(th->GetWorkerPaydayIncome());
			L.PCall(2, 1, 0);
			if (L.IsNumber(-1)) {
				float add = L.CheckFloat(-1);
				if (add > 0)
					(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(th->PlayerID)->CurrentResources.AddToType(shok::ResourceType::GoldRaw, add);
				else if (add < 0)
					(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(th->PlayerID)->CurrentResources.SubFromType(shok::ResourceType::Gold, -add);
			}
			L.SetTop(t);
		};
		return 0;
	}

	int SetLeadersRegenerateTroopHealth(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		EGL::CGLEEntity::HookLeaderRegen();
		EGL::CGLEEntity::LeaderRegenRegenerateSoldiers = L.ToBoolean(1);
		return 0;
	}

	int SetStringTableText(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.GetState() != *EScr::CScriptTriggerSystem::GameState)
			throw lua::LuaException("does only work ingame");
		if (!shok::GetStringTableTextOverride) {
			shok::HookGetStringTableText();
			L.PushLightUserdata(&SetStringTableText);
			L.NewTable();
			L.SetTableRaw(L.REGISTRYINDEX);
			shok::GetStringTableTextOverride = [](const char* s) {
				if (!s)
					return s;
				const char* r = nullptr;
				lua::State L{ *EScr::CScriptTriggerSystem::GameState };
				int t = L.GetTop();

				L.PushLightUserdata(&SetStringTableText);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(s);
				luaext::EState{ L }.StringToLower();
				L.GetTableRaw(-2);
				if (L.IsString(-1)) {
					r = L.ToString(-1); // stored in registry, so its ok to return it
				}

				L.SetTop(t);
				return r;
			};
		}

		if (!L.IsString(1))
			throw lua::LuaException("key not string");
		if (!L.IsString(2) && !L.IsNil(2))
			throw lua::LuaException("replacement not string or nil");

		L.PushLightUserdata(&SetStringTableText);
		L.GetTableRaw(L.REGISTRYINDEX);
		L.PushValue(1);
		luaext::EState{ L }.StringToLower();
		L.PushValue(2);
		L.SetTableRaw(-3);
		return 0;
	}

	int SetPlaceBuildingAdditionalCheck(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			GGL::CPlayerStatus::CanPlaceBuildingCallback = nullptr;
			L.PushLightUserdata(&SetPlaceBuildingAdditionalCheck);
			L.Push();
			L.SetTableRaw(L.REGISTRYINDEX);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		L.PushLightUserdata(&SetPlaceBuildingAdditionalCheck);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);


		if (!GGL::CPlayerStatus::CanPlaceBuildingCallback) {
			GGL::CPlayerStatus::CanPlaceBuildingCallback = [](int entitytype, int player, shok::Position* pos, float rotation, int buildOnId) {
				luaext::EState L{ *EScr::CScriptTriggerSystem::GameState };
				int t = L.GetTop();
				bool r = true;

				L.PushLightUserdata(&SetPlaceBuildingAdditionalCheck);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(entitytype);
				L.Push(player);
				L.PushPos(*pos);
				L.Push(CppLogic::RadiansToDegrees(rotation));
				L.Push(buildOnId);
				L.PCall(5, 1, 0);
				if (L.IsBoolean(-1))
					r = L.ToBoolean(-1);

				L.SetTop(t);
				return r;
			};
			GGL::CPlayerStatus::HookCanPlaceBuilding();
		}
		return 0;
	}

	int SetPlaceBuildingRotation(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		GGUI::CPlaceBuildingState::HookPlacementRotation();
		GGUI::CPlaceBuildingState::PlacementRotation = CppLogic::DegreesToRadians(L.CheckFloat(1));
		GGUI::CPlaceBuildingState* s = dynamic_cast<GGUI::CPlaceBuildingState*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
		if (s)
			s->UpdateModel();
		return 0;
	}
	int GetPlaceBuildingRotation(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		L.Push(CppLogic::RadiansToDegrees(GGUI::CPlaceBuildingState::PlacementRotation));
		return 1;
	}

	int FixSnipeDamage(lua::State L) {
		GGL::CSniperAbility::OverrideSnipeTask();
		if (L.IsNil(1)) {
			GGL::CSniperAbility::SnipeDamageOverride = nullptr;
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		L.PushLightUserdata(&FixSnipeDamage);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);


		if (!GGL::CSniperAbility::SnipeDamageOverride) {
			GGL::CSniperAbility::SnipeDamageOverride = [](EGL::CGLEEntity* sniper, EGL::CGLEEntity* tar, int dmg) {
				lua::State L{ *EScr::CScriptTriggerSystem::GameState };
				int t = L.GetTop();

				L.PushLightUserdata(&FixSnipeDamage);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(sniper->EntityId);
				L.Push(tar->EntityId);
				L.Push(dmg);
				L.PCall(3, 1, 0);
				if (L.IsNumber(-1))
					dmg = L.CheckInt(-1);

				L.SetTop(t);
				return dmg;
			};
		}
	
		return 0;
	}

	int GetCurrentWeatherGFXState(lua::State L) {
		int state = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler->WeatherChange.CurrentWeatherGFXState;
		L.Push(state);
		return 1;
	}
	int GetWeatherQueue(lua:: State L) {
		GGL::CWeatherHandler* wh = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler;
		L.NewTable();
		for (const auto& kae : wh->Elements) {
			L.NewTable();
			L.Push("State");
			L.Push(kae.second.State);
			L.SetTableRaw(-3);
			L.Push("GFX");
			L.Push(kae.second.GfxSet);
			L.SetTableRaw(-3);
			L.Push("IsPeriodic");
			L.Push(kae.second.IsPeriodic);
			L.SetTableRaw(-3);
			L.Push("Length");
			L.Push(kae.second.Length);
			L.SetTableRaw(-3);
			L.Push("Forerun");
			L.Push(kae.second.Forerun);
			L.SetTableRaw(-3);

			L.SetTableRaw(-2, kae.second.StartTimeOffset);
		}
		L.Push(wh->CurrentWeatherOffset);
		L.Push(wh->NextPeriodicWeatherStartTimeOffset);
		return 3;
	}
	int ClearWeatherQueueAndAddInitial(lua::State L) {
		GGL::CWeatherHandler* wh = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler;
		int state = L.CheckInt(1);
		if (state < 1 || state > 3)
			throw lua::LuaException("no weather state");
		int dur = L.CheckInt(2) * 10;
		if (dur <= 50)
			throw lua::LuaException("duration too short");
		int fore = L.CheckInt(3) * 10;
		if (fore <= 0 || fore > 100)
			throw lua::LuaException("forerun to small or too big");
		int gfx = L.CheckInt(4);
		if (gfx <= 0)
			throw lua::LuaException("gfx <=0");
		int trans = L.CheckInt(5) * 10;
		if (trans <= fore || trans > 200)
			throw lua::LuaException("transition too small or too big");
		if (wh->Elements.size == 0) {
			wh->AddWeatherElement(state, dur, true, fore, gfx, trans);
		}
		else {
			wh->ClearQueue(state, dur, fore, gfx, trans);
		}
		return 0;
	}

	struct SetLuaTaskListFunc_Info {
		EGL::CGLEEntity* e;
		bool HasSetTl, HasMoved, Ret;
	};
	int SetLuaTaskListFunc_Move(lua::State ls) {
		luaext::EState L{ ls };
		SetLuaTaskListFunc_Info* d = static_cast<SetLuaTaskListFunc_Info*>(L.ToUserdata(L.Upvalueindex(1)));
		shok::Position p = L.CheckPos(1);
		EGL::CEventPosition e{ shok::EventIDs::Movement_TaskMoveToPos, p };
		d->e->FireEvent(&e);
		d->HasMoved = true;
		bool usepathing = true;
		if (L.ToBoolean(2))
			usepathing = false;
		d->e->GetBehavior<GGL::CBehaviorDefaultMovement>()->IsPathingUsed = usepathing;
		if (L.ToBoolean(3) && d->e->CurrentState == shok::TaskState::Move)
			d->e->CurrentState = shok::TaskState::Move_NonCancelable;
		return 0;
	}
	int SetLuaTaskListFunc_SetTl(lua::State L) {
		SetLuaTaskListFunc_Info* d = static_cast<SetLuaTaskListFunc_Info*>(L.ToUserdata(L.Upvalueindex(1)));
		int tid = L.CheckInt(1);
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(tid);
		if (!tl)
			throw lua::LuaException("no tasklist");
		d->e->SetTaskList(tl);
		d->HasSetTl = true;
		return 0;
	}
	int SetLuaTaskListFunc(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");

		if (L.IsNil(1)) {
			EGL::CGLEEntity::LuaTaskListCallback = nullptr;
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		L.PushLightUserdata(&SetLuaTaskListFunc);
		L.PushValue(1);
		L.SetTableRaw(L.REGISTRYINDEX);


		if (!EGL::CGLEEntity::LuaTaskListCallback) {
			EGL::CGLEEntity::LuaTaskListCallback = [](EGL::CGLEEntity* e, int val) {
				lua::State L{ *EScr::CScriptTriggerSystem::GameState };
				int t = L.GetTop();

				SetLuaTaskListFunc_Info d{ e, false, false, false };
				L.PushLightUserdata(&SetLuaTaskListFunc);
				L.GetTableRaw(L.REGISTRYINDEX);
				L.Push(e->EntityId);
				L.Push(val);
				L.PushLightUserdata(&d);
				L.Push<SetLuaTaskListFunc_Move>(1);
				L.PushLightUserdata(&d);
				L.Push<SetLuaTaskListFunc_SetTl>(1);
				L.PCall(4, 1, 0);
				if (L.ToBoolean(-1))
					d.Ret = true;

				L.SetTop(t);
				if (d.HasMoved)
					return 1;
				else if (d.HasSetTl)
					return 2;
				else if (d.Ret)
					return 3;
				else
					return 0;
			};
		}

		return 0;
	}

	int TaskListMakeWaitForAnimsUnCancelable(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");

		int i = L.CheckInt(1);
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(i);
		if (!tl)
			throw lua::LuaException("invalid tasklist");
		i = L.OptInteger(2, -1);
		if (i < 0) {
			for (EGL::CGLETaskArgs* p : tl->Task) {
				if (p->TaskType == shok::Task::TASK_WAIT_FOR_ANIM)
					p->TaskType = shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE;
			}
		}
		else {
			if (i >= static_cast<int>(tl->Task.size()))
				throw lua::LuaException("invalid task");
			if (tl->Task[i]->TaskType != shok::Task::TASK_WAIT_FOR_ANIM)
				throw lua::LuaException("task not waitforanim");
			tl->Task[i]->TaskType = shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE;
		}
		return 0;
	}
	int TaskListMakeWaitForAnimsCancelable(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");

		int i = L.CheckInt(1);
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(i);
		if (!tl)
			throw lua::LuaException("invalid tasklist");
		i = L.OptInteger(2, -1);
		if (i < 0) {
			for (EGL::CGLETaskArgs* p : tl->Task) {
				if (p->TaskType == shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE)
					p->TaskType = shok::Task::TASK_WAIT_FOR_ANIM;
			}
		}
		else {
			if (i >= static_cast<int>(tl->Task.size()))
				throw lua::LuaException("invalid task");
			if (tl->Task[i]->TaskType != shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE)
				throw lua::LuaException("task not waitforanim_nocancel");
			tl->Task[i]->TaskType = shok::Task::TASK_WAIT_FOR_ANIM;
		}
		return 0;
	}
	int TaskListSetChangeTaskListCheckUncancelable(lua::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");
		EGL::CGLEEntity::HookSetTaskListNonCancelable(L.OptBool(1, false));
		return 0;
	}

	int EnableBuildOnMovementFix(lua::State L) {
		EGL::CGLEEntity::BuildOnSetPosFixMovement = L.ToBoolean(1);
		if (EGL::CGLEEntity::BuildOnSetPosFixMovement)
			EGL::CGLEEntity::HookBuildOnSetPos();
		return 0;
	}

	int GetNearestFreePosForBuilding(lua::State ls) {
		luaext::EState L{ ls };
		if (!L.CheckEntityType(1)->IsBuildingType())
			throw lua::LuaException("no building type");
		int ty = L.CheckInt(1);
		shok::PositionRot pin = L.CheckPosRot(2);
		float range = L.OptFloat(3, 0);
		if (range <= 0)
			range = (*GGL::CLogicProperties::GlobalObj)->BuildingPlacementSnapDistance;
		shok::PositionRot pout = GGUI::CPlaceBuildingState::GetNearestPlacementPos(ty, pin, range);
		L.PushPosRot(pout);
		return 1;
	}

	int BlockingUpdateWeatherChange(lua::State L) {
		(*EGL::CGLEGameLogic::GlobalObj)->Landscape->WeatherChangeBlockingUpdate();
		return 0;
	}

	int EnableExperienceClassFix(lua::State L) {
		GGL::CEntityProfile::HookExperience(L.CheckBool(1));
		return 0;
	}

	RWE::RwOpCombineType LogicModel_CheckTO(lua::State L, int idx) {
		int i = L.OptInteger(idx, static_cast<int>(RWE::RwOpCombineType::Preconcat));
		if (!(i >= 0 && i < 3))
			throw lua::LuaException("invalid transform operation");
		return static_cast<RWE::RwOpCombineType>(i);
	}
	struct LogicModel {
		RWE::RpClump* Model = nullptr;
		RWE::RtAnimationFrameHandler* AnimHandler = nullptr;
		float Lasttime = 0;

		static int Clear(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (m->Model) {
				m->Model->Destroy();
				m->Model = nullptr;
				m->AnimHandler = nullptr;
			}
			return 0;
		}
		static int SetModel(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			int mid = L.CheckInt(2);
			if (!(*ED::CGlobalsBaseEx::GlobalObj)->ModelManager->GetNameByID(mid))
				throw lua::LuaException("invalid model");
			if (m->Model) {
				m->Model->Destroy();
				m->Model = nullptr;
				m->AnimHandler = nullptr;
			}
			auto* mdata = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(mid);
			m->Model = mdata->Instanciate();
			m->Model->AddToDefaultWorld();
			return 0;
		}
		static int SetAnim(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			int anim = L.CheckInt(2);
			m->AnimHandler = m->Model->GetFrame()->GetAnimFrameHandler();
			if (!m->AnimHandler)
				throw lua::LuaException{ "no animhandler?" };
			//reinterpret_cast<void(__cdecl*)(void*)>(0x6EC7E0)(m->AnimHandler);
			//reinterpret_cast<byte*>(m->AnimHandler)[1] |= 0x10;
			auto* adata = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetAnimData(anim);
			m->AnimHandler->SetAnimation(adata);
			m->AnimHandler->SetTimeOfAnim(0.0f);
			m->AnimHandler->ApplyTransforms();
			m->Lasttime = static_cast<float>((*EGL::CGLEGameLogic::GlobalObj)->InGameTime->Tick) * (*EGL::CGLEGameLogic::GlobalObj)->InGameTime->TicksPerMS / 1000;
			return 0;
		}
		static int AdvanceAnim(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			if (!m->AnimHandler)
				throw lua::LuaException{ "set an anim first" };
			float t = static_cast<float>((*EGL::CGLEGameLogic::GlobalObj)->InGameTime->Tick) * (*EGL::CGLEGameLogic::GlobalObj)->InGameTime->TicksPerMS / 1000;
			m->AnimHandler->SetTimeOfAnim(t - m->Lasttime);
			m->AnimHandler->ApplyTransforms();
			m->Lasttime = t;
			return 0;
		}
		static int Translate(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			shok::Position p = luaext::EState{ L }.CheckPos(2);
			float h = L.OptFloat(3, 0);
			if (L.OptBool(5, true)) {
				float t = (*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainHeightAtPos(p);
				if (L.OptBool(6, false)) {
					float w = (*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetWaterHeightAtPos(p);
					h += std::max(t, w);
				}
				else {
					h += t;
				}
			}
			RWE::RwV3d tr{ p.X, p.Y, h };
			m->Model->GetFrame()->Translate(&tr, LogicModel_CheckTO(L, 4));
			return 0;
		}
		static int Rotate(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			float r = L.CheckFloat(2);
			m->Model->GetFrame()->Rotate(r, LogicModel_CheckTO(L, 3));
			return 0;
		}
		static int Scale(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			float s = L.CheckFloat(2);
			m->Model->GetFrame()->Scale(s, LogicModel_CheckTO(L, 3));
			return 0;
		}
		static int ResetTransform(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->GetFrame()->Rotate(0, RWE::RwOpCombineType::Replace);
			return 0;
		}
		static int SetColorByPlayer(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			int p = L.CheckInt(2);
			if (!(p >= 0 && p <= 9))
				throw lua::LuaException("invalid player");
			m->Model->SetPlayerColor(p);
			return 0;
		}
		static int DisableShadow(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->DisableShadow();
			return 0;
		}
		static int DisableParticleEffects(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->DisableParticleEffects();
			return 0;
		}
		static int DisableTerrainDecal(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->DisableTerrainDecal();
			return 0;
		}
		static int SetColorModulate(lua::State L) {
			LogicModel* m = L.GetUserData<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			int r = L.CheckInt(2);
			int g = L.CheckInt(3);
			int b = L.CheckInt(4);
			int a = L.OptInteger(5, 255);
			m->Model->SetColorModulate(shok::Color{ r, g, b, a });
			return 0;
		}
	
		static constexpr const std::array<lua::FuncReference,13> LuaMethods = {
			lua::FuncReference::GetRef<Clear>("Clear"),
			lua::FuncReference::GetRef<SetModel>("SetModel"),
			lua::FuncReference::GetRef<Translate>("Translate"),
			lua::FuncReference::GetRef<Rotate>("Rotate"),
			lua::FuncReference::GetRef<Scale>("Scale"),
			lua::FuncReference::GetRef<ResetTransform>("ResetTransform"),
			lua::FuncReference::GetRef<SetColorByPlayer>("SetColorByPlayer"),
			lua::FuncReference::GetRef<DisableShadow>("DisableShadow"),
			lua::FuncReference::GetRef<DisableParticleEffects>("DisableParticleEffects"),
			lua::FuncReference::GetRef<DisableTerrainDecal>("DisableTerrainDecal"),
			lua::FuncReference::GetRef<SetColorModulate>("SetColorModulate"),
			lua::FuncReference::GetRef<SetAnim>("SetAnim"),
			lua::FuncReference::GetRef<AdvanceAnim>("AdvanceAnim"),
		};

		~LogicModel() {
			if (Model) {
				Model->Destroy();
				Model = nullptr;
			}
		};
	};
	int CreateFreeModel(lua::State L) {
		L.NewUserData<LogicModel>();
		return 1;
	}

	void Loadtasks()
	{
		EGL::TaskData::AddExtraTasks();
		EGL::CGLEEntity::HookLuaTaskList();
		EGL::CGLEEntity::HookNonCancelableAnim();
	}

	void OnLoad()
	{
		if (!CppLogic::HasSCELoader())
			Loadtasks();
	}

	void Cleanup(lua::State L) {
		NetEventUnSetHook(L);
		GGL::CPlayerAttractionHandler::OnCheckPayDayCallback = nullptr;
		EGL::CGLEEntity::LeaderRegenRegenerateSoldiers = false;
		shok::GetStringTableTextOverride = nullptr;
		GGL::CPlayerStatus::CanPlaceBuildingCallback = nullptr;
		GGUI::CPlaceBuildingState::PlacementRotation = 0.0f;
		EGL::CGLEEntity::UseMaxHPTechBoni = false;
		GGL::CSniperAbility::SnipeDamageOverride = nullptr;
		EGL::CGLEEntity::LuaTaskListCallback = nullptr;
		EGL::CGLEEntity::BuildOnSetPosFixMovement = false;
		EGL::CGLEEntity::HurtEntityCallWithNoAttacker = false;
		EGL::CGLEEntity::HookSetTaskListNonCancelable(false);
		GGL::CEntityProfile::HookExperience(false);
	}

	constexpr std::array<lua::FuncReference, 52> Logic{ {
			lua::FuncReference::GetRef<GetDamageFactor>("GetDamageFactor"),
			lua::FuncReference::GetRef<SetDamageFactor>("SetDamageFactor"),
			lua::FuncReference::GetRef<ReloadCutscene>("ReloadCutscene"),
			lua::FuncReference::GetRef<GetAnimIdFromName>("GetAnimIdFromName"),
			lua::FuncReference::GetRef<PlayerGetPaydayStartetTick>("PlayerGetPaydayStartetTick"),
			lua::FuncReference::GetRef<PlayerSetPaydayStartetTick>("PlayerSetPaydayStartetTick"),
			lua::FuncReference::GetRef<SetPaydayFrequency>("SetPaydayFrequency"),
			lua::FuncReference::GetRef<PlayerGetKillStatistics>("PlayerGetKillStatistics"),
			lua::FuncReference::GetRef<CanPlaceBuildingAt>("CanPlaceBuildingAt"),
			lua::FuncReference::GetRef<PlayerActivateAlarm>("PlayerActivateAlarm"),
			lua::FuncReference::GetRef<PlayerDeactivateAlarm>("PlayerDeactivateAlarm"),
			lua::FuncReference::GetRef<PlayerUpgradeSettlerCategory>("PlayerUpgradeSettlerCategory"),
			lua::FuncReference::GetRef<PlayerSetTaxLevel>("PlayerSetTaxLevel"),
			lua::FuncReference::GetRef<PlayerActivateWeatherMachine>("PlayerActivateWeatherMachine"),
			lua::FuncReference::GetRef<PlayerBlessSettlers>("PlayerBlessSettlers"),
			lua::FuncReference::GetRef<LandscapeGetSector>("LandscapeGetSector"),
			lua::FuncReference::GetRef<LandscapeGetNearestUnblockedPosInSector>("LandscapeGetNearestUnblockedPosInSector"),
			lua::FuncReference::GetRef<EnableAllHurtEntityTrigger>("EnableAllHurtEntityTrigger"),
			lua::FuncReference::GetRef<HurtEntityGetDamage>("HurtEntityGetDamage"),
			lua::FuncReference::GetRef<HurtEntitySetDamage>("HurtEntitySetDamage"),
			lua::FuncReference::GetRef<GetLoadOrder>("GetLoadOrder"),
			lua::FuncReference::GetRef<AddArchive>("AddArchive"),
			lua::FuncReference::GetRef<RemoveTopArchive>("RemoveTopArchive"),
			lua::FuncReference::GetRef<EnableMaxHPTechMod>("EnableMaxHPTechMod"),
			lua::FuncReference::GetRef<LandscapeGetTerrainType>("LandscapeGetTerrainType"),
			lua::FuncReference::GetRef<LandscapeGetWaterType>("LandscapeGetWaterType"),
			lua::FuncReference::GetRef<LandscapeGetWaterHeight>("LandscapeGetWaterHeight"),
			lua::FuncReference::GetRef<LandscapeGetTerrainHeight>("LandscapeGetTerrainHeight"),
			lua::FuncReference::GetRef<LandscapeGetTerrainVertexColor>("LandscapeGetTerrainVertexColor"),
			lua::FuncReference::GetRef<LandscapeGetBlocking>("LandscapeGetBlocking"),
			lua::FuncReference::GetRef<LandscapeGetBridgeHeight>("LandscapeGetBridgeHeight"),
			lua::FuncReference::GetRef<GetColorByColorIndex>("GetColorByColorIndex"),
			lua::FuncReference::GetRef<SetColorByColorIndex>("SetColorByColorIndex"),
			lua::FuncReference::GetRef<SetPaydayCallback>("SetPaydayCallback"),
			lua::FuncReference::GetRef<SetLeadersRegenerateTroopHealth>("SetLeadersRegenerateTroopHealth"),
			lua::FuncReference::GetRef<SetStringTableText>("SetStringTableText"),
			lua::FuncReference::GetRef<SetPlaceBuildingAdditionalCheck>("SetPlaceBuildingAdditionalCheck"),
			lua::FuncReference::GetRef<SetPlaceBuildingRotation>("SetPlaceBuildingRotation"),
			lua::FuncReference::GetRef<GetPlaceBuildingRotation>("GetPlaceBuildingRotation"),
			lua::FuncReference::GetRef<FixSnipeDamage>("FixSnipeDamage"),
			lua::FuncReference::GetRef<GetCurrentWeatherGFXState>("GetCurrentWeatherGFXState"),
			lua::FuncReference::GetRef<GetWeatherQueue>("GetWeatherQueue"),
			lua::FuncReference::GetRef<ClearWeatherQueueAndAddInitial>("ClearWeatherQueueAndAddInitial"),
			lua::FuncReference::GetRef<SetLuaTaskListFunc>("SetLuaTaskListFunc"),
			lua::FuncReference::GetRef<TaskListMakeWaitForAnimsUnCancelable>("TaskListMakeWaitForAnimsUnCancelable"),
			lua::FuncReference::GetRef<TaskListMakeWaitForAnimsCancelable>("TaskListMakeWaitForAnimsCancelable"),
			lua::FuncReference::GetRef<TaskListSetChangeTaskListCheckUncancelable>("TaskListSetChangeTaskListCheckUncancelable"),
			lua::FuncReference::GetRef<EnableBuildOnMovementFix>("EnableBuildOnMovementFix"),
			lua::FuncReference::GetRef<CreateFreeModel>("CreateFreeModel"),
			lua::FuncReference::GetRef<GetNearestFreePosForBuilding>("GetNearestFreePosForBuilding"),
			lua::FuncReference::GetRef<BlockingUpdateWeatherChange>("BlockingUpdateWeatherChange"),
			lua::FuncReference::GetRef<EnableExperienceClassFix>("EnableExperienceClassFix"),
		} };

	constexpr std::array<lua::FuncReference, 2> UICmd{ {
			lua::FuncReference::GetRef<NetEventSetHook>("SetCallback"),
			lua::FuncReference::GetRef<NetEventUnSetHook>("UnSetCallback"),
	} };

	void Init(lua::State L)
	{
		L.RegisterFuncs(Logic, -3);
	#ifdef _DEBUG
		L.RegisterFunc<AddFolder>("AddFolder", -3);
	#endif

		L.Push( "UICommands");
		L.NewTable();
		L.RegisterFuncs(UICmd, -3);
		L.SetTableRaw(-3);

		if (L.GetState() != shok::LuaStateMainmenu) {
			L.PrepareUserDataType<LogicModel>();
		}
	}
}

// CppLogic.Logic.UICommands.SetCallback(function(id, ev) LuaDebugger.Log(id) LuaDebugger.Log(ev) return false end)
// CppLogic.Logic.AddArchive("extra2\\shr\\maps\\user\\test.s5x")
// CppLogic.Logic.AddArchive("test.bba")
// CppLogic.Logic.AddFolder("test")
// CppLogic.Logic.GetLoadOrder()
// CppLogic.Logic.RemoveTopArchive()
// CppLogic.Logic.EnableMaxHPTechMod() ResearchTechnology(Technologies.T_Fletching)
// CppLogic.Logic.GetColorByColorIndex(1)
// CppLogic.Logic.SetColorByColorIndex(1, 255, 255, 255, 255)
// CppLogic.Logic.SetPaydayCallback(GameCallback_PaydayPayed)
// CppLogic.Logic.SetStringTableText("MainMenu/StartExtras", "test")
// CppLogic.Logic.SetStringTableText("names/pu_hero1a", "test")
// CppLogic.Logic.SetPlaceBuildingAdditionalCheck(function(...) LuaDebugger.Log(arg) return true end)
// CppLogic.Logic.SetPlaceBuildingRotation(90)
// CppLogic.Logic.SetLuaTaskListFunc(function(id, i) LuaDebugger.Log(id) LuaDebugger.Log(i) end)

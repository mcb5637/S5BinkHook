#include "pch.h"
#include "l_logic.h"
#include <filesystem>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/globals/s5_config.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/player/s5_player.h>
#include <shok/globals/s5_cutscene.h>
#include <shok/ui/s5_ui.h>
#include <shok/s5_scriptsystem.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/entity/s5_entity.h>
#include <shok/events/s5_netevents.h>
#include <shok/globals/s5_filesystem.h>
#include <shok/s5_exception.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/s5_defines.h>
#include <shok/ui/s5_guistates.h>
#include <shok/entity/s5_tasklist.h>
#include <shok/engine/s5_RWEngine.h>
#include <shok/engine/s5_RWE_anim.h>
#include <shok/events/s5_events.h>
#include <shok/globals/s5_classfactory.h>
#include <luaext.h>
#include <utility/hooks.h>
#include <utility/luaserializer.h>
#include <utility/savegame_extra.h>
#include <utility/modloader.h>
#include <luafuncs/l_ui.h>
#include <utility/ModFilesystem.h>

namespace CppLogic::Logic {
	int GetDamageFactor(luaext::State L) {
		auto dmgclass = L.CheckEnum<shok::DamageClassId>(1);
		auto amclass = L.CheckEnum<shok::ArmorClassId>(2);
		L.Push(CppLogic::GetDamageClass(dmgclass)->GetBonusVsArmorClass(amclass));
		return 1;
	}
	int SetDamageFactor(luaext::State L) {
		auto dmgclass = L.CheckEnum<shok::DamageClassId>(1);
		auto amclass = L.CheckEnum<shok::ArmorClassId>(2);
		CppLogic::GetDamageClass(dmgclass)->GetBonusVsArmorClass(amclass) = L.CheckFloat(3);
		return 0;
	}

	int ReloadCutscene(luaext::State L) {
		const char* data = L.OptString(1, "Maps\\ExternalMap");
		(**ECS::CManager::GlobalObj)->ReloadCutscene(data);
		return 0;
	}

	int GetAnimIdFromName(luaext::State L) {
		const char* data = L.CheckString(1);
		int id = (*BB::CIDManagerEx::AnimManager)->GetIdByName(data);
		L.Push(id);
		return 1;
	}

	int PlayerGetPaydayStartetTick(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (p->PlayerAttractionHandler->PaydayActive)
			L.Push(p->PlayerAttractionHandler->PaydayFirstOccuraceGameTurn);
		else
			L.Push(-1);
		return 1;
	}
	int PlayerSetPaydayStartetTick(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		int st = L.CheckInt(2);
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (st < 0) {
			p->PlayerAttractionHandler->PaydayActive = false;
		}
		else {
			p->PlayerAttractionHandler->PaydayActive = true;
			p->PlayerAttractionHandler->PaydayFirstOccuraceGameTurn = st;
		}
		return 0;
	}
	int SetPaydayFrequency(luaext::State L) {
		int i = L.CheckInt(1);
		if (i <= 0)
			throw lua::LuaException("freq has to be > 0");
		(*GGL::CPlayerAttractionProps::GlobalObj)->PaydayFrequency = i;
		return 0;
	}


	int NetEventReadBack(luaext::State L) {
		auto* ev = static_cast<BB::CEvent*>(L.ToUserdata(L.Upvalueindex(1)));
		L.PushValue(1);
		L.Push("EventType");
		L.Push();
		L.SetTableRaw(-3);
		if (auto* e = dynamic_cast<EGL::CNetEventEntityAndPosArray*>(ev)) {
			L.Push("Position");
			L.GetTableRaw(-2);
			if (!L.IsNil(-1)) {
				auto v = e->Position.SaveVector();
				v.Vector.clear();
			}
			L.Pop(1);
		}
		if (auto* e = dynamic_cast<GGL::CNetEventBuildingCreator*>(ev)) {
			L.Push("Serf");
			L.GetTableRaw(-2);
			if (!L.IsNil(-1)) {
				auto v = e->Serf.SaveVector();
				v.Vector.clear();
			}
			L.Pop(1);
		}
		CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(L, ev);
		return 0;
	}

	bool NetEventSetHook_Hook(BB::CEvent* ev) {
		int id = ev->EventTypeId;
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int top = L.GetTop();
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(NetEventSetHookRegKey);
		L.GetTableRaw(-2);
		bool r = false;
		if (L.IsFunction(-1)) {
			L.Push(id);
			CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, ev);
			L.PushLightUserdata(ev);
			L.Push<NetEventReadBack>(1);
			L.PCall(3, 1, 0);
			if (L.IsBoolean(-1))
				r = L.ToBoolean(-1);
		}
		L.SetTop(top);
		return r;
	};
	int NetEventSetHook(luaext::State L) {
		if (!L.IsFunction(1))
			throw lua::LuaException("no func");
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(NetEventSetHookRegKey);
		L.PushValue(1);
		L.SetTableRaw(-3);
		GGUI::CManager::PostEventCallback = &NetEventSetHook_Hook;
		GGUI::CManager::GlobalObj()->HackPostEvent();
		return 0;
	}
	int NetEventUnSetHook(luaext::State L) {
		GGUI::CManager::PostEventCallback = nullptr;
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(NetEventSetHookRegKey);
		L.Push();
		L.SetTableRaw(-3);
		return 0;
	}


	int PlayerGetKillStatistics(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		L.Push(p->Statistics.NumberOfUnitsKilled);
		L.Push(p->Statistics.NumberOfUnitsDied);
		L.Push(p->Statistics.NumberOfBuildingsDestroyed);
		L.Push(p->Statistics.NumberOfBuildingsLost);
		return 4;
	}

	int CanPlaceBuildingAt(luaext::State L) {
		auto ty = L.CheckEnum<shok::EntityTypeId>(1);
		GGlue::CGlueEntityProps* ety = CppLogic::GetEntityType(ty);
		if (!ety)
			throw lua::LuaException("no entitytype");
		if (!ety->IsBuildingType())
			throw lua::LuaException("not a building");
		auto pl = L.CheckPlayerId(2);
		shok::Position p = L.CheckPos(3);
		if (static_cast<GGL::CGLBuildingProps*>(ety->LogicProps)->BuildOn.size() == 0) // NOLINT(*-pro-type-static-cast-downcast)
			p.FloorToBuildingPlacement();
		float r = CppLogic::DegreesToRadians(L.CheckFloat(4));
		if (L.IsNumber(5))
			L.Push(GGL::CPlayerStatus::CanPlaceBuilding(ty, pl, &p, r, L.OptEntityId(5)));
		else
			L.Push(GGL::CPlayerStatus::CanPlaceBuildingAtPos(ty, pl, &p, r));
		return 1;
	}

	int PlayerActivateAlarm(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (p->WorkerAlarmMode)
			throw lua::LuaException("alarm active");
		if (p->AlarmRechargeTime > 0)
			throw lua::LuaException("alarm cooldown");
		(*GGL::CGLGameLogic::GlobalObj)->EnableAlarmForPlayer(i);
		return 0;
	}
	int PlayerDeactivateAlarm(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		if (!p->WorkerAlarmMode)
			throw lua::LuaException("alarm not active");
		(*GGL::CGLGameLogic::GlobalObj)->DisableAlarmForPlayer(i);
		return 0;
	}

	int PlayerUpgradeSettlerCategory(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		auto ucat = L.CheckEnum<shok::UpgradeCategoryId>(2);
		auto u = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->SettlerUpgradeManager->UpgradeCategories.find(ucat);
		if (u == (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->SettlerUpgradeManager->UpgradeCategories.end())
			throw lua::LuaException("invalid ucat");
		(*GGL::CGLGameLogic::GlobalObj)->UpgradeSettlerCategory(i, ucat);
		return 0;
	}

	int PlayerSetTaxLevel(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		int tl = L.CheckInt(2);
		if (tl < 0 || tl >= 5)
			throw lua::LuaException("invalid taxlevel");
		(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->TaxLevel = tl;
		return 0;
	}

	int PlayerActivateWeatherMachine(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		int w = L.CheckInt(2);
		if (w < 1 || w >= 4)
			throw lua::LuaException("invalid weathertype");
		if ((*GGL::CGLGameLogic::GlobalObj)->WeatherHandler->WeatherChange.StateToChangeTo != shok::WeatherState::Invalid)
			throw lua::LuaException("weather currently changing");
		if ((*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->CurrentResources.WeatherEnergy < (*GGL::CLogicProperties::GlobalObj)->EnergyRequiredForWeatherChange)
			throw lua::LuaException("not enough weather energy");
		(*GGL::CGLGameLogic::GlobalObj)->PlayerActivateWeathermachine(i, static_cast<shok::WeatherState>(w));
		return 0;
	}

	int PlayerBlessSettlers(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		auto b = L.CheckEnum<shok::BlessCategoryId>(2);
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

	int LandscapeGetSector(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p));
		return 1;
	}

	int LandscapeGetNearestUnblockedPosInSector(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		int s = L.CheckInt(2);
		float r = L.CheckFloat(3);
		shok::Position po;
		if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetNearestPositionInSector(&p, r, static_cast<shok::SectorId>(s), &po))
			L.Push(po);
		else
			L.Push();
		return 1;
	}

	int EnableAllHurtEntityTrigger(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		EGL::CGLEEntity::HookHurtEntity();
		if (L.IsBoolean(1))
			EGL::CGLEEntity::HurtEntityCallWithNoAttacker = L.ToBoolean(1);
		else
			EGL::CGLEEntity::HurtEntityCallWithNoAttacker = true;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HurtEntityCallWithNoAttacker = EGL::CGLEEntity::HurtEntityCallWithNoAttacker;
		return 0;
	}

	int HurtEntityGetDamage(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		auto* ev = dynamic_cast<CppLogic::Events::AdvHurtEvent*>(*EScr::CScriptTriggerSystem::CurrentRunningEventGet);
		if (!ev)
			throw lua::LuaException("not in trigger");
		L.Push(ev->Damage);
		L.Push(static_cast<int>(ev->Source));
		L.Push(ev->AttackerPlayer);
		return 3;
	}
	int HurtEntitySetDamage(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use CEntity instead");
		auto* ev = dynamic_cast<CppLogic::Events::AdvHurtEvent*>(*EScr::CScriptTriggerSystem::CurrentRunningEventGet);
		if (!ev)
			throw lua::LuaException("not in trigger");
		ev->Damage = L.CheckInt(1);
		return 0;
	}

	int GetLoadOrder(luaext::State L) {
		L.NewTable();
		int r = 1;
		for (BB::IFileSystem* a : (*BB::CFileSystemMgr::GlobalObj)->LoadOrder) {
			if (auto* dir = dynamic_cast<BB::CDirectoryFileSystem*>(a)) {
				L.Push(dir->Path);
				L.SetTableRaw(-2, r);
				r++;
			}
			else if (auto* arch = dynamic_cast<BB::CBBArchiveFile*>(a)) {
				L.Push(arch->ArchiveFile.Filename);
				L.SetTableRaw(-2, r);
				r++;
			}
			else if (auto* redir = dynamic_cast<CppLogic::Mod::FileSystem::RedirectFileSystem*>(a)) {
				L.Push(redir->Name);
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

	int AddArchive(luaext::State L) {
		auto s = L.CheckStringView(1);
		if (!std::filesystem::exists(s))
			throw lua::LuaException("file doesnt exist");
		try {
			(*BB::CFileSystemMgr::GlobalObj)->AddArchive(s.data());
		}
		catch (const BB::CFileException& e) {
			char buff[201]{};
			e.CopyMessage(buff, 200);
			throw lua::LuaException(buff);
		}
		L.NewUserClass<CppLogic::ModLoader::ArchivePopHelper>(s, (*BB::CFileSystemMgr::GlobalObj)->LoadOrder[0]);
		return 1;
	}

	int RemoveTopArchive(luaext::State L) {
		if ((*BB::CFileSystemMgr::GlobalObj)->LoadOrder.size() <= 0)
			throw lua::LuaException("loadorder empty");
		if ((*BB::CFileSystemMgr::GlobalObj)->LoadOrder[0] == BB::CFileSystemMgr::LoadorderTop)
			throw lua::LuaException("may not remove BB archives");
		(*BB::CFileSystemMgr::GlobalObj)->RemoveTopArchive();
		return 0;
	}

	int AddFolder(luaext::State L) {
		auto s = L.CheckStringView(1);
		(*BB::CFileSystemMgr::GlobalObj)->AddFolder(s.data());
		L.NewUserClass<CppLogic::ModLoader::ArchivePopHelper>(s, (*BB::CFileSystemMgr::GlobalObj)->LoadOrder[0]);
		return 1;
	}

	int AddRedirectLayer(luaext::State L) {
		auto s = L.CheckStringView(1);
		auto* r = CppLogic::Mod::FileSystem::RedirectFileSystem::CreateRedirectLayer(s);
		L.NewUserClass<CppLogic::ModLoader::ArchiveRedirectHelper>(s, r);
		return 1;
	}

	int GetArchiveOfFile(luaext::State L) {
		const char* s = L.CheckString(1);
		auto* mng = (*BB::CFileSystemMgr::GlobalObj);
		BB::IFileSystem::FileInfo info{};
		for (unsigned int i = 0; i < mng->LoadOrder.size(); ++i) {
			auto fs = mng->LoadOrder[i];
			fs->GetFileInfo(&info, s);
			if (info.Found) {
				if (auto* dir = dynamic_cast<BB::CDirectoryFileSystem*>(fs)) {
					L.Push(dir->Path);
				}
				else if (auto* arch = dynamic_cast<BB::CBBArchiveFile*>(fs)) {
					L.Push(arch->ArchiveFile.Filename);
				}
				else {
					L.Push(typeid(*fs).name());
				}
				L.Push(static_cast<int>(i + 1));
				return 2;
			}
		}
		return 0;
	}

	int EnableMaxHPTechMod(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supportet with SCELoader");
		EGL::CGLEEntity::HookMaxHP();
		EGL::CGLEEntity::UseMaxHPTechBoni = true;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.UseMaxHPTechBoni = EGL::CGLEEntity::UseMaxHPTechBoni;
		return 0;
	}

	int LandscapeGetTerrainType(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetTerrainTypeAt(p));
		return 1;
	}
	int LandscapeGetWaterType(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetWaterTypeAt(p));
		return 1;
	}
	int LandscapeGetWaterHeight(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetWaterHeightAt(p));
		return 1;
	}
	int LandscapeGetTerrainHeight(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes->GetTerrainHeight(p));
		return 1;
	}
	int LandscapeGetTerrainVertexColor(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		int c = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->VertexColors->GetTerrainVertexColor(p);
		L.Push(c);
		L.Push((c >> 24) & 0xFF);
		L.Push((c >> 16) & 0xFF);
		L.Push((c >> 8) & 0xFF);
		L.Push(c & 0xFF);
		return 5;
	}
	int LandscapeGetBlocking(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		L.Push(static_cast<int>((*ED::CGlobalsLogicEx::GlobalObj)->GetBlocking(p)));
		return 1;
	}
	int LandscapeGetBridgeHeight(luaext::State L) {
		shok::Position p = L.CheckPos(1);
		L.Push((*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetBridgeHeight(p));
		return 1;
	}

	int GetColorByColorIndex(luaext::State L) { // ind -> r,g,b,a
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
	int SetColorByColorIndex(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		int i = L.CheckInt(1);
		int r = L.CheckInt(2), g = L.CheckInt(3), b = L.CheckInt(4);
		int a = L.OptInt(5, 255);
		(*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->SetColorByIndex(i, shok::Color(r, g, b, a));
		(*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->RefreshPlayerColors();
		return 0;
	}

	int SetPaydayCallback(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("use global GameCallback_PaydayPayed instead");
		GGL::CPlayerAttractionHandler::HookCheckPayday();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.PaydayTrigger = true;
		return 0;
	}

	int SetLeadersRegenerateTroopHealth(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		GGL::CLeaderBehavior::HookLeaderRegen();
		GGL::CLeaderBehavior::LeaderRegenRegenerateSoldiers = L.ToBoolean(1);
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.LeaderRegenRegenerateSoldiers = GGL::CLeaderBehavior::LeaderRegenRegenerateSoldiers;
		return 0;
	}

	int SetStringTableText(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		
		if (!L.IsString(1))
			throw lua::LuaException("key not string");
		if (!L.IsString(2) && !L.IsNil(2))
			throw lua::LuaException("replacement not string or nil");

		auto& map = CppLogic::SavegameExtra::SerializedMapdata::GetActiveOverrides(L.GetState());

		if (L.IsNil(2)) {
			auto r = map.find(L.CheckStringView(1));
			if (r != map.end())
				map.erase(r);
			return 0;
		}
		map[L.CheckStdString(1)] = L.CheckStdString(2);

		CppLogic::SavegameExtra::SerializedMapdata::STTHasChanged(L.GetState());
		return 0;
	}

	bool CanPlaceBuildingCb(shok::EntityTypeId entitytype, shok::PlayerId player, shok::Position* pos, float rotation, shok::EntityId buildOnId) {
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int t = L.GetTop();
		bool r = true;

		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(CanPlaceBuildingCallbackRegKey);
		L.GetTableRaw(-2);
		L.Push(entitytype);
		L.Push(player);
		L.Push(*pos);
		L.Push(CppLogic::RadiansToDegrees(rotation));
		L.Push(buildOnId);
		L.PCall(5, 1, 0);
		if (L.IsBoolean(-1))
			r = L.ToBoolean(-1);

		L.SetTop(t);
		return r;
	}
	int SetPlaceBuildingAdditionalCheck(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		if (L.IsNil(1)) {
			GGL::CPlayerStatus::CanPlaceBuildingCallback = nullptr;
			CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
			L.Push(CanPlaceBuildingCallbackRegKey);
			L.Push();
			L.SetTableRaw(-3);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(CanPlaceBuildingCallbackRegKey);
		L.PushValue(1);
		L.SetTableRaw(-3);


		if (!GGL::CPlayerStatus::CanPlaceBuildingCallback) {
			GGL::CPlayerStatus::CanPlaceBuildingCallback = &CanPlaceBuildingCb;
			GGL::CPlayerStatus::HookCanPlaceBuilding();
		}
		return 0;
	}

	int SetPlaceBuildingRotation(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		//GGUI::CPlaceBuildingState::HookPlacementRotation();
		auto* s = dynamic_cast<CppLogic::UI::GUIState_PlaceBuildingEx*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
		if (s)
			s->SetRotation(L.CheckFloat(1));
		return 0;
	}
	int GetPlaceBuildingRotation(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("not supported with SCELoader");
		auto* s = dynamic_cast<CppLogic::UI::GUIState_PlaceBuildingEx*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
		if (!s)
			return 0;
		L.Push(s->GetRotation());
		return 1;
	}

	int SnipeDamageCb(EGL::CGLEEntity* sniper, EGL::CGLEEntity* tar, int dmg) {
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int t = L.GetTop();

		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(SnipeDamageOverrideRegKey);
		L.GetTableRaw(-2);
		L.Push(sniper->EntityId);
		L.Push(tar->EntityId);
		L.Push(dmg);
		L.PCall(3, 1, 0);
		if (L.IsNumber(-1))
			dmg = L.CheckInt(-1);

		L.SetTop(t);
		return dmg;
	}
	int FixSnipeDamage(luaext::State L) {
		GGL::CSniperAbility::OverrideSnipeTask();
		if (L.IsNil(1)) {
			GGL::CSniperAbility::SnipeDamageOverride = nullptr;
			CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
			L.Push(SnipeDamageOverrideRegKey);
			L.Push(true);
			L.SetTableRaw(-3);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(SnipeDamageOverrideRegKey);
		L.PushValue(1);
		L.SetTableRaw(-3);

		GGL::CSniperAbility::SnipeDamageOverride = &SnipeDamageCb;
	
		return 0;
	}

	int GetCurrentWeatherGFXState(luaext::State L) {
		int state = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler->WeatherChange.CurrentWeatherGFXState;
		L.Push(state);
		return 1;
	}
	int GetWeatherQueue(luaext::State L) {
		GGL::CWeatherHandler* wh = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler;
		L.NewTable();
		for (const auto& kae : wh->Elements) {
			L.NewTable();
			L.Push("State");
			L.Push(static_cast<int>(kae.second.State));
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
	int ClearWeatherQueueAndAddInitial(luaext::State L) {
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
			wh->AddWeatherElement(static_cast<shok::WeatherState>(state), dur, true, fore, static_cast<shok::WeatherGFXSet>(gfx), trans);
		}
		else {
			wh->ClearQueue(static_cast<shok::WeatherState>(state), dur, fore, static_cast<shok::WeatherGFXSet>(gfx), trans);
		}
		return 0;
	}

	struct SetLuaTaskListFunc_Info {
		EGL::CGLEEntity* e;
		bool HasSetTl, HasMoved, Ret;
	};
	int SetLuaTaskListFunc_Move(luaext::State L) {
		auto* d = static_cast<SetLuaTaskListFunc_Info*>(L.ToUserdata(L.Upvalueindex(1)));
		shok::Position p = L.CheckPos(1);
		EGL::CEventPosition e{ shok::EventIDs::Movement_TaskMoveToPos, p };
		d->e->FireEvent(&e);
		d->HasMoved = true;
		bool usepathing = true;
		if (L.ToBoolean(2))
			usepathing = false;
		d->e->GetBehaviorDynamic<GGL::CBehaviorDefaultMovement>()->IsPathingUsed = usepathing;
		if (L.ToBoolean(3) && d->e->CurrentState == shok::TaskState::Move)
			d->e->CurrentState = shok::TaskState::Move_NonCancelable;
		return 0;
	}
	int SetLuaTaskListFunc_SetTl(luaext::State L) {
		auto* d = static_cast<SetLuaTaskListFunc_Info*>(L.ToUserdata(L.Upvalueindex(1)));
		auto tid = L.CheckEnum<shok::TaskListId>(1);
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(tid);
		if (!tl)
			throw lua::LuaException("no tasklist");
		d->e->SetTaskList(tl);
		d->HasSetTl = true;
		return 0;
	}
	shok::TaskExecutionResult SetLuaTaskListFunc_Func(EGL::CGLEEntity* e, int val)
	{
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		int t = L.GetTop();

		SetLuaTaskListFunc_Info d{ e, false, false, false };
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(SetLuaTaskListFuncRegKey);
		L.GetTableRaw(-2);
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
			return shok::TaskExecutionResult::StateChanged;
		else if (d.HasSetTl)
			return shok::TaskExecutionResult::TaskListChanged;
		else if (d.Ret)
			return shok::TaskExecutionResult::LuaTaskState;
		else
			return shok::TaskExecutionResult::NextTask;
	}
	int SetLuaTaskListFunc(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");

		if (L.IsNil(1)) {
			EGL::CGLEEntity::LuaTaskListCallback = nullptr;
			CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
			L.Push(SetLuaTaskListFuncRegKey);
			L.Push();
			L.SetTableRaw(-3);
			return 0;
		}

		L.CheckType(1, lua::LType::Function);
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		L.Push(SetLuaTaskListFuncRegKey);
		L.PushValue(1);
		L.SetTableRaw(-3);

		if (!EGL::CGLEEntity::LuaTaskListCallback) {
			EGL::CGLEEntity::LuaTaskListCallback = &SetLuaTaskListFunc_Func;
		}

		return 0;
	}

	int TaskListMakeWaitForAnimsUnCancelable(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");

		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(L.CheckEnum<shok::TaskListId>(1));
		if (!tl)
			throw lua::LuaException("invalid tasklist");
		int i = L.OptInt(2, -1);
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
	int TaskListMakeWaitForAnimsCancelable(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");

		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(L.CheckEnum<shok::TaskListId>(1));
		if (!tl)
			throw lua::LuaException("invalid tasklist");
		int i = L.OptInt(2, -1);
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
	int TaskListSetChangeTaskListCheckUncancelable(luaext::State L) {
		if (CppLogic::HasSCELoader())
			throw lua::LuaException("does not work with SCELoader");
		bool b = L.OptBool(1, false);
		EGL::CGLEEntity::HookSetTaskListNonCancelable(b);
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ChangeTaskListCheckUncancelable = b;
		return 0;
	}

	int TaskListInsertWaitForLatestAttack(luaext::State L) {
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(L.CheckEnum<shok::TaskListId>(1));
		if (!tl)
			throw lua::LuaException("invalid tasklist");
		int i = L.CheckInt(2);
		if (i < 0 || i > static_cast<int>(tl->Task.size()))
			throw lua::LuaException("invalid task");
		auto* t = (*BB::CClassFactory::GlobalObj)->CreateObject<EGL::CGLETaskArgs>();
		t->TaskType = shok::Task::TASK_WAIT_FOR_LATEST_ATTACK;
		auto vec = tl->Task.SaveVector();
		vec.Vector.insert(vec.Vector.begin() + i, t);
		return 0;
	}
	int TaskListInsertSetLatestAttack(luaext::State L) {
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(L.CheckEnum<shok::TaskListId>(1));
		if (!tl)
			throw lua::LuaException("invalid tasklist");
		int i = L.CheckInt(2);
		if (i < 0 || i > static_cast<int>(tl->Task.size()))
			throw lua::LuaException("invalid task");
		auto* t = (*BB::CClassFactory::GlobalObj)->CreateObject<EGL::CGLETaskArgs>();
		t->TaskType = shok::Task::TASK_SET_LATEST_ATTACK_TURN;
		auto vec = tl->Task.SaveVector();
		vec.Vector.insert(vec.Vector.begin() + i, t);
		return 0;
	}
	int TaskListRemoveLatestAttack(luaext::State L) {
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(L.CheckEnum<shok::TaskListId>(1));
		if (!tl)
			throw lua::LuaException("invalid tasklist");
		auto vec = tl->Task.SaveVector();
		vec.Vector.erase(std::remove_if(vec.Vector.begin(), vec.Vector.end(), 
			[](EGL::CGLETaskArgs* a) {
				if (a->TaskType == shok::Task::TASK_WAIT_FOR_LATEST_ATTACK || a->TaskType == shok::Task::TASK_SET_LATEST_ATTACK_TURN) {
					delete a;
					return true;
				}
				return false;
			}), vec.Vector.end());
		return 0;
	}

	int EnableBuildOnMovementFix(luaext::State L) {
		EGL::CGLEEntity::BuildOnSetPosFixMovement = L.ToBoolean(1);
		if (EGL::CGLEEntity::BuildOnSetPosFixMovement)
			EGL::CGLEEntity::HookBuildOnSetPos();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.BuildOnMovementFix = EGL::CGLEEntity::BuildOnSetPosFixMovement;
		return 0;
	}

	int GetNearestFreePosForBuilding(luaext::State L) {
		if (!L.CheckEntityType(1)->IsBuildingType())
			throw lua::LuaException("no building type");
		auto ty = L.CheckEnum<shok::EntityTypeId>(1);
		shok::PositionRot pin = L.CheckPosRot(2);
		float range = L.OptFloat(3, 0);
		if (range <= 0)
			range = (*GGL::CLogicProperties::GlobalObj)->BuildingPlacementSnapDistance;
		shok::PositionRot pout = GGUI::CPlaceBuildingState::GetNearestPlacementPos(ty, pin, range);
		L.Push(pout);
		return 1;
	}

	int BlockingUpdateWeatherChange(luaext::State L) {
		(*EGL::CGLEGameLogic::GlobalObj)->Landscape->WeatherChangeBlockingUpdate();
		return 0;
	}

	int EnableExperienceClassFix(luaext::State L) {
		bool b = L.CheckBool(1);
		GGL::CEntityProfile::HookExperience(b);
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ExperienceClassFix = b;
		return 0;
	}

	int GetAnimationDuration(luaext::State L) {
		auto id = L.CheckEnum<shok::AnimationId>(1);
		L.Push((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetAnimationDuration(id));
		return 1;
	}

	int GetTradeDataForResource(luaext::State L) {
		auto rt = L.Check<shok::ResourceType>(1);
		if (rt != shok::ResourceType::Gold && rt != shok::ResourceType::Stone && rt != shok::ResourceType::Iron
			&& rt != shok::ResourceType::Sulfur && rt != shok::ResourceType::Clay && rt != shok::ResourceType::Wood)
			throw lua::LuaException{ "invalid resource type" };
		auto* t = (*GGL::CLogicProperties::GlobalObj)->GetResource(rt);
		L.Push(t->BasePrice);
		L.Push(t->MinPrice);
		L.Push(t->MaxPrice);
		L.Push(t->Inflation);
		L.Push(t->Deflation);
		L.Push(t->WorkAmount);
		return 6;
	}
	int SetTradeDataForResource(luaext::State L) {
		auto rt = L.Check<shok::ResourceType>(1);
		if (rt != shok::ResourceType::Gold && rt != shok::ResourceType::Stone && rt != shok::ResourceType::Iron
			&& rt != shok::ResourceType::Sulfur && rt != shok::ResourceType::Clay && rt != shok::ResourceType::Wood)
			throw lua::LuaException{ "invalid resource type" };
		auto* t = (*GGL::CLogicProperties::GlobalObj)->GetResource(rt);
		if (!L.IsNoneOrNil(2))
			t->BasePrice = L.CheckFloat(2);
		if (!L.IsNoneOrNil(3))
			t->MinPrice = L.CheckFloat(3);
		if (!L.IsNoneOrNil(4))
			t->MaxPrice = L.CheckFloat(4);
		if (!L.IsNoneOrNil(5))
			t->Inflation = L.CheckFloat(5);
		if (!L.IsNoneOrNil(6))
			t->Deflation = L.CheckFloat(6);
		if (!L.IsNoneOrNil(7))
			t->WorkAmount = L.CheckFloat(7);
		return 0;
	}

	int Navigate(luaext::State L) {
		shok::Position from = L.CheckPos(1);
		shok::Position to = L.CheckPos(2);
		if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->IsPosBlockedInMode(&from, EGL::CGLELandscape::BlockingMode::Blocked))
			throw lua::LuaException{ "from is blocked" };
		if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->IsPosBlockedInMode(&to, EGL::CGLELandscape::BlockingMode::Blocked))
			throw lua::LuaException{ "to is blocked" };
		EGL::CCoarsePath p{};
		if (p.Navigate(from, to)) {
			L.NewTable();
			int i = 1;
			for (int k = 0; k < p.WayPoints.CacheCount; ++k) {
				L.Push(p.WayPoints.GetWaypoint(k));
				L.SetTableRaw(-2, i);
				++i;
			}
			return 1;
		}
		return 0;
	}

	void EnableResourceTriggers(bool e, bool ref) {
		GGL::CWorkerBehavior::HookWorkEvents();
		GGL::CResourceRefinerBehavior::HookRefineTrigger();
		GGL::CMineBehavior::HookMineTrigger();
		GGL::CSerfBehavior::HookMineTrigger();
		GGL::CWorkerBehavior::ResourceTriggers = e;
		GGL::CWorkerBehavior::RefinerFix = ref;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ResourceTriggers = e;
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.RefinerFix = ref;
	}
	int EnableResourceTriggers(luaext::State L) {
		EnableResourceTriggers(L.OptBool(1, true), L.OptBool(2, false));
		return 0;
	}
	int EnableSettlerBuyTriggers(luaext::State L) {
		GGL::CPlayerAttractionHandler::HookWorkerSpawn();
		GGL::CBarrackBehavior::HookBuyTriggers();
		GGL::CKeepBehavior::HookBuySerf();
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.SettlerBuyTriggers = true;
		return 0;
	}

	int GetSettlerBuyTriggerData(luaext::State L) {
		auto* ev = BB::IdentifierCast<CppLogic::Events::CanBuySettlerEvent>(*EScr::CScriptTriggerSystem::CurrentRunningEventGet);
		if (ev == nullptr)
			throw lua::LuaException{ "invalid event" };
		L.Push(ev->ToBuy);
		L.Push(ev->Target);
		L.Push(ev->VCPop);
		L.Push(ev->Cost);
		L.Push(ev->Motivation);
		L.Push(ev->Alarm);
		L.Push(ev->HQPop);
		return 7;
	}
	int SetSettlerBuyTriggerData(luaext::State L) {
		auto* ev = BB::IdentifierCast<CppLogic::Events::CanBuySettlerEvent>(*EScr::CScriptTriggerSystem::CurrentRunningEventGet);
		if (ev == nullptr)
			throw lua::LuaException{ "invalid event" };
		ev->VCPop = L.CheckBool(1);
		ev->Cost = L.CheckBool(2);
		ev->Motivation = L.CheckBool(3);
		ev->Alarm = L.CheckBool(4);
		ev->HQPop = L.CheckBool(5);
		return 0;
	}

	int IsPositionExplored(luaext::State L) {
		auto pl = L.CheckPlayerId(1, false);
		auto pos = L.CheckPos(2);
		EGL::PlayerManager* p = (*EGL::CGLEGameLogic::GlobalObj)->PlayerMng;
		auto* ex = p->GetExplorationHandlerByPlayer(pl);
		auto x = static_cast<unsigned int>(pos.X / 100.0f);
		auto y = static_cast<unsigned int>(pos.Y / 100.0f);
		L.Push(static_cast<bool>(ex->ExplorationMapCurrent->Get(x, y)));
		L.Push(static_cast<bool>(ex->ExplorationMapSeen->Get(x, y)));
		L.Push(static_cast<bool>(ex->ExplorationMapWork->Get(x, y)));
		return 3;
	}

	int SetPositionExploration(luaext::State L) {
		auto pl = L.CheckPlayerId(1, false);
		auto rect = shok::AARect{ L.CheckPos(2), L.CheckPos(3) }.Sort();
		EGL::PlayerManager* p = (*EGL::CGLEGameLogic::GlobalObj)->PlayerMng;
		auto* ex = p->GetExplorationHandlerByPlayer(pl);
		auto xl = static_cast<unsigned int>(rect.low.X / 100.0f);
		auto yl = static_cast<unsigned int>(rect.low.Y / 100.0f);
		auto xh = static_cast<unsigned int>(rect.high.X / 100.0f);
		auto yh = static_cast<unsigned int>(rect.high.Y / 100.0f);
		bool b = L.CheckBool(4);
		for (unsigned int x = xl; x <= xh; ++x)
			for (unsigned int y = yl; y <= yh; ++y)
				ex->ExplorationMapSeen->Set(x, y, b);
		return 0;
	}

	int DumpTaskList(luaext::State L) {
		EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(L.CheckEnum<shok::TaskListId>(1));
		CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, tl);
		return 1;
	}

	int PlayerGetWorkerAttraction(luaext::State L) {
		auto pid = L.CheckPlayerId(1, false);
		auto* ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pid)->PlayerAttractionHandler;
		L.Push(ah->GetAttractionUsageOfVector(ah->FreeWorkerArray) + ah->GetAttractionUsageOfVector(ah->EmployedWorkerArray));
		return 1;
	}
	int PlayerGetMilitaryAttraction(luaext::State L) {
		auto pid = L.CheckPlayerId(1, false);
		auto* ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pid)->PlayerAttractionHandler;
		L.Push(ah->GetAttractionUsageOfVector(ah->LeaderArray) + ah->GetAttractionUsageOfVector(ah->SoldierArray));
		return 1;
	}
	int PlayerGetSerfAttraction(luaext::State L) {
		auto pid = L.CheckPlayerId(1, false);
		auto* ah = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pid)->PlayerAttractionHandler;
		L.Push(ah->GetAttractionUsageOfVector(ah->SerfArray));
		return 1;
	}

	int EnableCannonInProgressAttraction(luaext::State L) {
		GGL::CPlayerAttractionHandler::AttractionCannonInProgress = L.CheckBool(1);
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CannonInProgressAttraction = GGL::CPlayerAttractionHandler::AttractionCannonInProgress;
		GGL::CPlayerAttractionHandler::HookAttractionCannonInProgress();
		return 0;
	}

	int EnableRefillabeMineNoAutoDestroy(luaext::State L) {
		CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ResDoodad_RefillableCategory = L.IsNoneOrNil(1) ? shok::EntityCategory::Invalid : L.CheckEnum<shok::EntityCategory>(1);
		GGL::CResourceDoodad::HookAutoDestroyIfEmpty();
		GGL::CResourceDoodad::RefillableCategory = CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ResDoodad_RefillableCategory;
		return 0;
	}

	int GetPlayerName(luaext::State L) {
		auto pid = L.CheckPlayerId(1);
		GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pid);
		if (p->PlayerNameStringRaw.size() > 0)
		{
			L.Push(p->PlayerNameStringRaw);
			L.Push(false);
		}
		else
		{
			L.Push(p->PlayerNameStringTableKey);
			L.Push(true);
		}
		return 2;
	}

	int DumpGameLogic(luaext::State L) {
		L.NewTable();
		L.Push("GGL");
		CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, *GGL::CGLGameLogic::GlobalObj, GGL::CGLGameLogic::SerializationData());
		L.SetTableRaw(-3);

		auto* l = *EGL::CGLEGameLogic::GlobalObj;
		L.Push("EGL::LogicGameTime");
		CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, l->InGameTime, EGL::LogicGameTime::SerializationData());
		L.SetTableRaw(-3);

		L.Push("EGL::PlayerManager");
		CppLogic::Serializer::ObjectToLuaSerializer::Serialize(L, l->PlayerMng, EGL::PlayerManager::SerializationData);
		L.SetTableRaw(-3);

		return 1;
	}

	RWE::RwOpCombineType LogicModel_CheckTO(luaext::State L, int idx) {
		int i = L.OptInt(idx, static_cast<int>(RWE::RwOpCombineType::Preconcat));
		if (!(i >= 0 && i < 3))
			throw lua::LuaException("invalid transform operation");
		return static_cast<RWE::RwOpCombineType>(i);
	}
	struct LogicModel {
		shok::ModelId ModelId = {};
		shok::AnimationId AnimId = {};
		RWE::RpClump* Model = nullptr;
		RWE::Anim::RpHAnimHierarchy* AnimHandler = nullptr;
		float StartTime = 0;
		float CurrentTime = 0;
		int PlayerColor = -1;
		bool NoShadow = false, NoParticleEffects = false, NoTerrainDecal = false;
		shok::Color Modulate = shok::Color{ 255,255,255,255 };

		static int Clear(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (m->Model) {
				m->Model->Destroy();
				m->Model = nullptr;
				m->AnimHandler = nullptr;
			}
			m->ModelId = shok::ModelId::Invalid;
			m->AnimId = shok::AnimationId::Invalid;
			m->PlayerColor = -1;
			m->NoShadow = false;
			m->NoParticleEffects = false;
			m->NoTerrainDecal = false;
			m->Modulate = shok::Color{ 255,255,255,255 };
			return 0;
		}
		static int SetModel(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			auto mid = L.CheckEnum<shok::ModelId>(2);
			if (m->Model) {
				m->Model->Destroy();
				m->Model = nullptr;
				m->AnimHandler = nullptr;
			}
			auto* mdata = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(mid);
			m->Model = mdata->Instanciate();
			m->Model->AddToDefaultWorld();
			m->ModelId = mid;
			m->AnimId = shok::AnimationId::Invalid;
			m->PlayerColor = -1;
			m->NoShadow = false;
			m->NoParticleEffects = false;
			m->NoTerrainDecal = false;
			m->Modulate = shok::Color{ 255,255,255,255 };
			return 0;
		}
		static int SetAnim(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			auto anim = L.CheckEnum<shok::AnimationId>(2);
			m->AnimHandler = m->Model->GetFrame()->GetAnimFrameHandler();
			if (!m->AnimHandler)
				throw lua::LuaException{ "no animhandler?" };
			auto* adata = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetAnimData(anim);
			m->AnimHandler->SetupForModel(m->Model);
			m->AnimHandler->currentAnim->SetAnimation(adata);
			m->AnimHandler->currentAnim->SetCurrentTime(0.0f);
			m->AnimHandler->UpdateMatrices();
			m->StartTime = (*EGL::CGLEGameLogic::GlobalObj)->GetTimeSeconds();
			m->AnimId = anim;
			m->CurrentTime = m->StartTime;
			return 0;
		}
		static int SetTimeOfAnim(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			if (!m->AnimHandler)
				throw lua::LuaException{ "set an anim first" };
			float t = L.OptFloat(2, (*EGL::CGLEGameLogic::GlobalObj)->GetTimeSeconds());
			if (L.OptBool(3, true))
				t -= m->StartTime;
			m->CurrentTime = t;
			m->AnimHandler->currentAnim->SetCurrentTime(t);
			m->AnimHandler->UpdateMatrices();
			return 0;
		}
		static int Translate(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			shok::Position p = L.CheckPos(2);
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
		static int Rotate(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			float r = L.CheckFloat(2);
			m->Model->GetFrame()->Rotate(r, LogicModel_CheckTO(L, 3));
			return 0;
		}
		static int Scale(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			float s = L.CheckFloat(2);
			m->Model->GetFrame()->Scale(s, LogicModel_CheckTO(L, 3));
			return 0;
		}
		static int ResetTransform(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->GetFrame()->Rotate(0, RWE::RwOpCombineType::Replace);
			return 0;
		}
		static int SetColorByPlayer(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			int p = L.CheckInt(2);
			if (!(p >= 0 && p <= 9))
				throw lua::LuaException("invalid player");
			m->Model->SetPlayerColor(p);
			m->PlayerColor = p;
			return 0;
		}
		static int DisableShadow(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->DisableShadow();
			m->NoShadow = true;
			return 0;
		}
		static int DisableParticleEffects(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->DisableParticleEffects();
			m->NoParticleEffects = true;
			return 0;
		}
		static int DisableTerrainDecal(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			m->Model->DisableTerrainDecal();
			m->NoTerrainDecal = true;
			return 0;
		}
		static int SetColorModulate(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			if (!m->Model)
				throw lua::LuaException("set a model first");
			int r = L.CheckInt(2);
			int g = L.CheckInt(3);
			int b = L.CheckInt(4);
			int a = L.OptInt(5, 255);
			m->Modulate = shok::Color{ r, g, b, a };
			m->Model->SetColorModulate(m->Modulate);
			return 0;
		}
		static int Serialize(luaext::State L) {
			auto* m = L.CheckUserClass<LogicModel>(1);
			L.Push(typename_details::type_name<LogicModel>());
			L.NewTable();

			L.Push("Model");
			L.Push(m->ModelId);
			L.SetTableRaw(-3);

			L.Push("Anim");
			L.Push(m->AnimId);
			L.SetTableRaw(-3);

			L.Push("StartTime");
			L.Push(m->StartTime);
			L.SetTableRaw(-3);

			L.Push("CurrentTime");
			L.Push(m->CurrentTime);
			L.SetTableRaw(-3);

			if (m->Model) {
				auto* f = m->Model->GetFrame();
				auto* mat = reinterpret_cast<float*>(f->GetLTM());
				for (int i = 0; i < (sizeof(RWE::RwMatrix) / sizeof(float)); ++i) {
					L.Push(mat[i]);
					L.SetTableRaw(-2, i + 1);
				}
			}
			L.Push("PlayerColor");
			L.Push(m->PlayerColor);
			L.SetTableRaw(-3);

			L.Push("NoShadow");
			L.Push(m->NoShadow);
			L.SetTableRaw(-3);

			L.Push("NoParticleEffects");
			L.Push(m->NoParticleEffects);
			L.SetTableRaw(-3);

			L.Push("NoTerrainDecal");
			L.Push(m->NoTerrainDecal);
			L.SetTableRaw(-3);

			L.Push("ModulateR");
			L.Push(m->Modulate.R);
			L.SetTableRaw(-3);

			L.Push("ModulateG");
			L.Push(m->Modulate.G);
			L.SetTableRaw(-3);

			L.Push("ModulateB");
			L.Push(m->Modulate.B);
			L.SetTableRaw(-3);

			L.Push("ModulateA");
			L.Push(m->Modulate.A);
			L.SetTableRaw(-3);

			return 2;
		}
		static int Deserialize(luaext::State L) {
			auto* m = L.NewUserClass<LogicModel>();
			L.Push("Model");
			L.GetTableRaw(1);
			m->ModelId = L.CheckEnum<shok::ModelId>(-1);
			L.Pop(1);

			L.Push("Anim");
			L.GetTableRaw(1);
			m->AnimId = L.CheckEnum<shok::AnimationId>(-1);
			L.Pop(1);

			L.Push("StartTime");
			L.GetTableRaw(1);
			m->StartTime = L.CheckFloat(-1);
			L.Pop(1);

			L.Push("CurrentTime");
			L.GetTableRaw(1);
			m->CurrentTime = L.CheckFloat(-1);
			L.Pop(1);

			L.Push("PlayerColor");
			L.GetTableRaw(1);
			m->PlayerColor = L.CheckInt(-1);
			L.Pop(1);

			L.Push("NoShadow");
			L.GetTableRaw(1);
			m->NoShadow = L.CheckBool(-1);
			L.Pop(1);

			L.Push("NoParticleEffects");
			L.GetTableRaw(1);
			m->NoParticleEffects = L.CheckBool(-1);
			L.Pop(1);

			L.Push("NoTerrainDecal");
			L.GetTableRaw(1);
			m->NoTerrainDecal = L.CheckBool(-1);
			L.Pop(1);

			L.Push("ModulateR");
			L.GetTableRaw(1);
			m->Modulate.R = static_cast<byte>(L.CheckInt(-1));
			L.Pop(1);

			L.Push("ModulateG");
			L.GetTableRaw(1);
			m->Modulate.G = static_cast<byte>(L.CheckInt(-1));
			L.Pop(1);

			L.Push("ModulateB");
			L.GetTableRaw(1);
			m->Modulate.B = static_cast<byte>(L.CheckInt(-1));
			L.Pop(1);

			L.Push("ModulateA");
			L.GetTableRaw(1);
			m->Modulate.A = static_cast<byte>(L.CheckInt(-1));
			L.Pop(1);

			if (m->ModelId != shok::ModelId::Invalid) {
				auto* mdata = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(m->ModelId);
				m->Model = mdata->Instanciate();
				m->Model->AddToDefaultWorld();
				if (m->AnimId != shok::AnimationId::Invalid) {
					m->AnimHandler = m->Model->GetFrame()->GetAnimFrameHandler();
					if (!m->AnimHandler)
						throw lua::LuaException{ "no animhandler?" };
					auto* adata = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetAnimData(m->AnimId);
					m->AnimHandler->SetupForModel(m->Model);
					m->AnimHandler->currentAnim->SetAnimation(adata);
					m->AnimHandler->currentAnim->SetCurrentTime(m->CurrentTime);
					m->AnimHandler->UpdateMatrices();
				}
				auto* f = m->Model->GetFrame();
				auto* mat = reinterpret_cast<float*>(f->GetLTM());
				for (int i = 0; i < (sizeof(RWE::RwMatrix) / sizeof(float)); ++i) {
					L.GetTableRaw(1, i + 1);
					if (L.IsNumber(-1))
						mat[i] = L.CheckFloat(-1);
					L.Pop(1);
				}
				f->UpdateObjects();
				if (m->PlayerColor >= 0)
					m->Model->SetPlayerColor(m->PlayerColor);
				if (m->NoParticleEffects)
					m->Model->DisableParticleEffects();
				if (m->NoShadow)
					m->Model->DisableShadow();
				if (m->NoTerrainDecal)
					m->Model->DisableTerrainDecal();
				m->Model->SetColorModulate(m->Modulate);
			}
			return 1;
		}
	
		static constexpr const std::array LuaMethods = {
			luaext::FuncReference::GetRef<Clear>("Clear"),
			luaext::FuncReference::GetRef<SetModel>("SetModel"),
			luaext::FuncReference::GetRef<Translate>("Translate"),
			luaext::FuncReference::GetRef<Rotate>("Rotate"),
			luaext::FuncReference::GetRef<Scale>("Scale"),
			luaext::FuncReference::GetRef<ResetTransform>("ResetTransform"),
			luaext::FuncReference::GetRef<SetColorByPlayer>("SetColorByPlayer"),
			luaext::FuncReference::GetRef<DisableShadow>("DisableShadow"),
			luaext::FuncReference::GetRef<DisableParticleEffects>("DisableParticleEffects"),
			luaext::FuncReference::GetRef<DisableTerrainDecal>("DisableTerrainDecal"),
			luaext::FuncReference::GetRef<SetColorModulate>("SetColorModulate"),
			luaext::FuncReference::GetRef<SetAnim>("SetAnim"),
			luaext::FuncReference::GetRef<SetTimeOfAnim>("SetTimeOfAnim"),
		};
		static constexpr const std::array LuaMetaMethods{
				luaext::FuncReference::GetRef<Serialize>(CppLogic::Serializer::AdvLuaStateSerializer::UserdataSerializerMetaEvent),
		};

		~LogicModel() {
			if (Model) {
				Model->Destroy();
				Model = nullptr;
			}
		};
	};
	int CreateFreeModel(luaext::State L) {
		L.NewUserClass<LogicModel>();
		return 1;
	}

	void Loadtasks()
	{
		EGL::TaskData::AddExtraTasks();
		EGL::CGLEEntity::HookLuaTaskList();
		GGL::CGLBehaviorAnimationEx::HookNonCancelableAnim();
		GGL::CWorkerBehavior::HookSupplierSkip();
	}

	void OnLoad()
	{
		if (!CppLogic::HasSCELoader())
			Loadtasks();
	}

	void Cleanup(luaext::State L) {
		NetEventUnSetHook(L);
		GGL::CPlayerAttractionHandler::OnCheckPayDayCallback = nullptr;
		GGL::CLeaderBehavior::LeaderRegenRegenerateSoldiers = false;
		CppLogic::SavegameExtra::SerializedMapdata::STTToMainmenu();
		GGL::CPlayerStatus::CanPlaceBuildingCallback = nullptr;
		GGUI::CPlaceBuildingState::PlacementRotation = 0.0f;
		EGL::CGLEEntity::UseMaxHPTechBoni = false;
		GGL::CSniperAbility::SnipeDamageOverride = nullptr;
		EGL::CGLEEntity::LuaTaskListCallback = nullptr;
		EGL::CGLEEntity::BuildOnSetPosFixMovement = false;
		EGL::CGLEEntity::HurtEntityCallWithNoAttacker = false;
		EGL::CGLEEntity::HookSetTaskListNonCancelable(false);
		GGL::CEntityProfile::HookExperience(false);
		GGL::CWorkerBehavior::ResourceTriggers = false;
		GGL::CWorkerBehavior::RefinerFix = false;
		GGL::CPlayerAttractionHandler::AttractionCannonInProgress = false;
	}

	constexpr std::array Logic{
			luaext::FuncReference::GetRef<GetDamageFactor>("GetDamageFactor"),
			luaext::FuncReference::GetRef<SetDamageFactor>("SetDamageFactor"),
			luaext::FuncReference::GetRef<ReloadCutscene>("ReloadCutscene"),
			luaext::FuncReference::GetRef<GetAnimIdFromName>("GetAnimIdFromName"),
			luaext::FuncReference::GetRef<PlayerGetPaydayStartetTick>("PlayerGetPaydayStartetTick"),
			luaext::FuncReference::GetRef<PlayerSetPaydayStartetTick>("PlayerSetPaydayStartetTick"),
			luaext::FuncReference::GetRef<SetPaydayFrequency>("SetPaydayFrequency"),
			luaext::FuncReference::GetRef<PlayerGetKillStatistics>("PlayerGetKillStatistics"),
			luaext::FuncReference::GetRef<CanPlaceBuildingAt>("CanPlaceBuildingAt"),
			luaext::FuncReference::GetRef<PlayerActivateAlarm>("PlayerActivateAlarm"),
			luaext::FuncReference::GetRef<PlayerDeactivateAlarm>("PlayerDeactivateAlarm"),
			luaext::FuncReference::GetRef<PlayerUpgradeSettlerCategory>("PlayerUpgradeSettlerCategory"),
			luaext::FuncReference::GetRef<PlayerSetTaxLevel>("PlayerSetTaxLevel"),
			luaext::FuncReference::GetRef<PlayerActivateWeatherMachine>("PlayerActivateWeatherMachine"),
			luaext::FuncReference::GetRef<PlayerBlessSettlers>("PlayerBlessSettlers"),
			luaext::FuncReference::GetRef<LandscapeGetSector>("LandscapeGetSector"),
			luaext::FuncReference::GetRef<LandscapeGetNearestUnblockedPosInSector>("LandscapeGetNearestUnblockedPosInSector"),
			luaext::FuncReference::GetRef<EnableAllHurtEntityTrigger>("EnableAllHurtEntityTrigger"),
			luaext::FuncReference::GetRef<HurtEntityGetDamage>("HurtEntityGetDamage"),
			luaext::FuncReference::GetRef<HurtEntitySetDamage>("HurtEntitySetDamage"),
			luaext::FuncReference::GetRef<GetLoadOrder>("GetLoadOrder"),
			luaext::FuncReference::GetRef<AddArchive>("AddArchive"),
			luaext::FuncReference::GetRef<RemoveTopArchive>("RemoveTopArchive"),
			luaext::FuncReference::GetRef<GetArchiveOfFile>("GetArchiveOfFile"),
			luaext::FuncReference::GetRef<EnableMaxHPTechMod>("EnableMaxHPTechMod"),
			luaext::FuncReference::GetRef<LandscapeGetTerrainType>("LandscapeGetTerrainType"),
			luaext::FuncReference::GetRef<LandscapeGetWaterType>("LandscapeGetWaterType"),
			luaext::FuncReference::GetRef<LandscapeGetWaterHeight>("LandscapeGetWaterHeight"),
			luaext::FuncReference::GetRef<LandscapeGetTerrainHeight>("LandscapeGetTerrainHeight"),
			luaext::FuncReference::GetRef<LandscapeGetTerrainVertexColor>("LandscapeGetTerrainVertexColor"),
			luaext::FuncReference::GetRef<LandscapeGetBlocking>("LandscapeGetBlocking"),
			luaext::FuncReference::GetRef<LandscapeGetBridgeHeight>("LandscapeGetBridgeHeight"),
			luaext::FuncReference::GetRef<GetColorByColorIndex>("GetColorByColorIndex"),
			luaext::FuncReference::GetRef<SetColorByColorIndex>("SetColorByColorIndex"),
			luaext::FuncReference::GetRef<SetPaydayCallback>("SetPaydayCallback"),
			luaext::FuncReference::GetRef<SetLeadersRegenerateTroopHealth>("SetLeadersRegenerateTroopHealth"),
			luaext::FuncReference::GetRef<SetStringTableText>("SetStringTableText"),
			luaext::FuncReference::GetRef<SetPlaceBuildingAdditionalCheck>("SetPlaceBuildingAdditionalCheck"),
			luaext::FuncReference::GetRef<SetPlaceBuildingRotation>("SetPlaceBuildingRotation"),
			luaext::FuncReference::GetRef<GetPlaceBuildingRotation>("GetPlaceBuildingRotation"),
			luaext::FuncReference::GetRef<FixSnipeDamage>("FixSnipeDamage"),
			luaext::FuncReference::GetRef<GetCurrentWeatherGFXState>("GetCurrentWeatherGFXState"),
			luaext::FuncReference::GetRef<GetWeatherQueue>("GetWeatherQueue"),
			luaext::FuncReference::GetRef<ClearWeatherQueueAndAddInitial>("ClearWeatherQueueAndAddInitial"),
			luaext::FuncReference::GetRef<SetLuaTaskListFunc>("SetLuaTaskListFunc"),
			luaext::FuncReference::GetRef<TaskListMakeWaitForAnimsUnCancelable>("TaskListMakeWaitForAnimsUnCancelable"),
			luaext::FuncReference::GetRef<TaskListMakeWaitForAnimsCancelable>("TaskListMakeWaitForAnimsCancelable"),
			luaext::FuncReference::GetRef<TaskListSetChangeTaskListCheckUncancelable>("TaskListSetChangeTaskListCheckUncancelable"),
			luaext::FuncReference::GetRef<EnableBuildOnMovementFix>("EnableBuildOnMovementFix"),
			luaext::FuncReference::GetRef<CreateFreeModel>("CreateFreeModel"),
			luaext::FuncReference::GetRef<GetNearestFreePosForBuilding>("GetNearestFreePosForBuilding"),
			luaext::FuncReference::GetRef<BlockingUpdateWeatherChange>("BlockingUpdateWeatherChange"),
			luaext::FuncReference::GetRef<EnableExperienceClassFix>("EnableExperienceClassFix"),
			luaext::FuncReference::GetRef<GetAnimationDuration>("GetAnimationDuration"),
			luaext::FuncReference::GetRef<GetTradeDataForResource>("GetTradeDataForResource"),
			luaext::FuncReference::GetRef<SetTradeDataForResource>("SetTradeDataForResource"),
			luaext::FuncReference::GetRef<TaskListInsertWaitForLatestAttack>("TaskListInsertWaitForLatestAttack"),
			luaext::FuncReference::GetRef<TaskListInsertSetLatestAttack>("TaskListInsertSetLatestAttack"),
			luaext::FuncReference::GetRef<TaskListRemoveLatestAttack>("TaskListRemoveLatestAttack"),
			luaext::FuncReference::GetRef<Navigate>("Navigate"),
			luaext::FuncReference::GetRef<EnableResourceTriggers>("EnableResourceTriggers"),
			luaext::FuncReference::GetRef<EnableSettlerBuyTriggers>("EnableSettlerBuyTriggers"),
			luaext::FuncReference::GetRef<GetSettlerBuyTriggerData>("GetSettlerBuyTriggerData"),
			luaext::FuncReference::GetRef<SetSettlerBuyTriggerData>("SetSettlerBuyTriggerData"),
			luaext::FuncReference::GetRef<IsPositionExplored>("IsPositionExplored"),
			luaext::FuncReference::GetRef<SetPositionExploration>("SetPositionExploration"),
			luaext::FuncReference::GetRef<DumpTaskList>("DumpTaskList"),
			luaext::FuncReference::GetRef<PlayerGetWorkerAttraction>("PlayerGetWorkerAttraction"),
			luaext::FuncReference::GetRef<PlayerGetMilitaryAttraction>("PlayerGetMilitaryAttraction"),
			luaext::FuncReference::GetRef<PlayerGetSerfAttraction>("PlayerGetSerfAttraction"),
			luaext::FuncReference::GetRef<EnableCannonInProgressAttraction>("EnableCannonInProgressAttraction"),
			luaext::FuncReference::GetRef<EnableRefillabeMineNoAutoDestroy>("EnableRefillabeMineNoAutoDestroy"),
			luaext::FuncReference::GetRef<GetPlayerName>("GetPlayerName"),
			luaext::FuncReference::GetRef<DumpGameLogic>("DumpGameLogic"),
#ifdef DEBUG_FUNCS
			luaext::FuncReference::GetRef<AddRedirectLayer>("AddRedirectLayer"),
			luaext::FuncReference::GetRef<AddFolder>("AddFolder"),
#endif
		};

	constexpr std::array UICmd{
			luaext::FuncReference::GetRef<NetEventSetHook>("SetCallback"),
			luaext::FuncReference::GetRef<NetEventUnSetHook>("UnSetCallback"),
	};

	void Init(luaext::State L)
	{
		L.RegisterFuncs(Logic, -3);

		L.Push( "UICommands");
		L.NewTable();
		L.RegisterFuncs(UICmd, -3);
		L.SetTableRaw(-3);

		if (L.GetState() != shok::LuaStateMainmenu) {
			L.PrepareUserClassType<LogicModel>();
			CppLogic::Serializer::AdvLuaStateSerializer::UserdataDeserializer[std::string{ typename_details::type_name<LogicModel>() }] = &luaext::State::CppToCFunction<LogicModel::Deserialize>;

			L.GetSubTable("Events");
			L.Push("CPPLOGIC_EVENT_ON_ENTITY_KILLS_ENTITY");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnEntityKilled));
			L.SetTableRaw(-3);
			L.Push("CPPLOGIC_EVENT_ON_PAYDAY");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnPayday));
			L.SetTableRaw(-3);
			L.Push("CPPLOGIC_EVENT_ON_RESOURCE_REFINED");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnResourceRefined));
			L.SetTableRaw(-3);
			L.Push("CPPLOGIC_EVENT_ON_REFINER_SUPPLY_TAKEN");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnRefinerSupplyTaken));
			L.SetTableRaw(-3);
			L.Push("CPPLOGIC_EVENT_ON_RESOURCE_MINED");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnResourceMined));
			L.SetTableRaw(-3);
			L.Push("CPPLOGIC_EVENT_CAN_BUY_SETTLER");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_CanBuySettler));
			L.SetTableRaw(-3);
			L.Push("CPPLOGIC_EVENT_ON_MAP_STARTED");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnMapStarted));
			L.SetTableRaw(-3);
			L.Push("CPPLOGIC_EVENT_ON_SAVEGAME_LOADED");
			L.Push(static_cast<int>(shok::EventIDs::CppLogicEvent_OnSavegameLoaded));
			L.SetTableRaw(-3);
			L.Pop(1);
		}
	}

	void OnSaveLoaded(luaext::State L)
	{
		CppLogic::Serializer::AdvLuaStateSerializer::PushSerializedRegistry(L);
		if (!CppLogic::HasSCELoader())
		{
			EGL::CGLEEntity::HurtEntityCallWithNoAttacker = CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.HurtEntityCallWithNoAttacker;
			if (EGL::CGLEEntity::HurtEntityCallWithNoAttacker)
				EGL::CGLEEntity::HookHurtEntity();

			EGL::CGLEEntity::UseMaxHPTechBoni = CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.UseMaxHPTechBoni;
			if (EGL::CGLEEntity::UseMaxHPTechBoni)
				EGL::CGLEEntity::HookMaxHP();

			if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.PaydayTrigger)
				GGL::CPlayerAttractionHandler::HookCheckPayday();

			L.Push(CanPlaceBuildingCallbackRegKey);
			L.GetTableRaw(-2);
			if (!L.IsNil(-1)) {
				GGL::CPlayerStatus::CanPlaceBuildingCallback = &CanPlaceBuildingCb;
				GGL::CPlayerStatus::HookCanPlaceBuilding();
			}
			L.Pop(1);

			GGL::CLeaderBehavior::LeaderRegenRegenerateSoldiers = CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.LeaderRegenRegenerateSoldiers;
			if (GGL::CLeaderBehavior::LeaderRegenRegenerateSoldiers)
				GGL::CLeaderBehavior::HookLeaderRegen();

			L.Push(SetLuaTaskListFuncRegKey);
			L.GetTableRaw(-2);
			if (!L.IsNil(-1)) {
				EGL::CGLEEntity::LuaTaskListCallback = &SetLuaTaskListFunc_Func;
			}
			L.Pop(1);

			if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ChangeTaskListCheckUncancelable)
				EGL::CGLEEntity::HookSetTaskListNonCancelable(true);

			CppLogic::SavegameExtra::SerializedMapdata::STTToIngame();
		}
		L.Push(SnipeDamageOverrideRegKey);
		L.GetTableRaw(-2);
		if (!L.IsNil(-1)) {
			GGL::CSniperAbility::OverrideSnipeTask();
			if (L.IsFunction(-1))
				GGL::CSniperAbility::SnipeDamageOverride = &SnipeDamageCb;
		}
		L.Pop(1);

		EGL::CGLEEntity::BuildOnSetPosFixMovement = CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.BuildOnMovementFix;
		if (EGL::CGLEEntity::BuildOnSetPosFixMovement)
			EGL::CGLEEntity::HookBuildOnSetPos();

		GGL::CEntityProfile::HookExperience(CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ExperienceClassFix);

		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ResourceTriggers || CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.RefinerFix)
			EnableResourceTriggers(CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ResourceTriggers, CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.RefinerFix);

		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.SettlerBuyTriggers)
			EnableSettlerBuyTriggers(luaext::State{ nullptr });

		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.CannonInProgressAttraction) {
			GGL::CPlayerAttractionHandler::HookAttractionCannonInProgress();
			GGL::CPlayerAttractionHandler::AttractionCannonInProgress = true;
		}

		if (CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ResDoodad_RefillableCategory != shok::EntityCategory::Invalid) {
			GGL::CResourceDoodad::HookAutoDestroyIfEmpty();
			GGL::CResourceDoodad::RefillableCategory = CppLogic::SavegameExtra::SerializedMapdata::GlobalObj.ResDoodad_RefillableCategory;
		}

		L.Pop(1);
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

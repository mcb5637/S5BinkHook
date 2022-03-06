#include "pch.h"
#include "l_logic.h"
#include "luaext.h"
#include <assert.h>
#include <filesystem>
#include "s5_netevents.h"

int l_logicGetDamageModifier(lua_State* L) {
	int dmgclass = luaL_checkint(L, 1);
	int size = (int)(*GGL::DamageClassesHolder::GlobalObj)->DamageClassList.size();
	luaext_assert(L, dmgclass > 0 && dmgclass < size, "invalid damagecass");
	int amclass = luaL_checkint(L, 2);
	luaext_assert(L, amclass >= 1 && amclass < 8, "invalid armorclass");
	lua_pushnumber(L, (*GGL::DamageClassesHolder::GlobalObj)->DamageClassList[dmgclass]->GetBonusVsArmorClass(amclass));
	return 1;
}
int l_logicSetDamageModifier(lua_State* L) {
	int dmgclass = luaL_checkint(L, 1);
	int size = (int)(*GGL::DamageClassesHolder::GlobalObj)->DamageClassList.size();
	luaext_assert(L, dmgclass > 0 && dmgclass < size, "invalid damagecass");
	int amclass = luaL_checkint(L, 2);
	luaext_assert(L, amclass >= 1 && amclass < 8, "invalid armorclass");
	(*GGL::DamageClassesHolder::GlobalObj)->DamageClassList[dmgclass]->GetBonusVsArmorClass(amclass) = luaL_checkfloat(L, 2);
	return 0;
}

int l_logicReloadCutscene(lua_State* L) {
	const char* data = luaL_optlstring(L, 1, "Maps\\ExternalMap", nullptr);
	(**ECS::CManager::GlobalObj)->ReloadCutscene(data);
	return 0;
}

int l_logicGetAnimIdFromName(lua_State* L) {
	const char* data = luaL_checkstring(L, 1);
	int id = (*BB::CIDManagerEx::AnimManager)->GetIdByName(data);
	lua_pushnumber(L, id);
	return 1;
}

int l_playerGetPaydayStatus(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
	if (p->PlayerAttractionHandler->PaydayActive)
		lua_pushnumber(L, p->PlayerAttractionHandler->PaydayFirstOccuraceGameTurn);
	else
		lua_pushnumber(L, -1);
	return 1;
}
int l_playerSetPaydayStatus(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int st = luaL_checkint(L, 2);
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
int l_logic_setpaydayfrequency(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0, "freq has to be > 0");
	(*GGL::CPlayerAttractionProps::GlobalObj)->PaydayFrequency = i;
	return 0;
}


void l_netPushEvent(lua_State* L, BB::CEvent* ev) {
	lua_newtable(L);
	if (EGL::CNetEventEntityID* e = dynamic_cast<EGL::CNetEventEntityID*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		if (GGL::CNetEventCannonCreator* e = dynamic_cast<GGL::CNetEventCannonCreator*>(ev)) {
			lua_pushstring(L, "BottomType");
			lua_pushnumber(L, e->BottomType);
			lua_rawset(L, -3);
			lua_pushstring(L, "TopType");
			lua_pushnumber(L, e->TopType);
			lua_rawset(L, -3);
			lua_pushstring(L, "Position");
			luaext_pushPos(L, e->Position);
			lua_rawset(L, -3);
		}
		else if (GGL::CNetEventEntityIDAndUpgradeCategory* e = dynamic_cast<GGL::CNetEventEntityIDAndUpgradeCategory*>(ev)) {
			lua_pushstring(L, "UpgradeCategory");
			lua_pushnumber(L, e->UpgradeCategory);
			lua_rawset(L, -3);
		}
		else if (EGL::CNetEventEntityIDAndInteger* e = dynamic_cast<EGL::CNetEventEntityIDAndInteger*>(ev)) {
			lua_pushstring(L, "Data");
			lua_pushnumber(L, e->Data);
			lua_rawset(L, -3);
		}
		else if (GGL::CNetEventTechnologyAndEntityID* e = dynamic_cast<GGL::CNetEventTechnologyAndEntityID*>(ev)) {
			lua_pushstring(L, "Technology");
			lua_pushnumber(L, e->Technology);
			lua_rawset(L, -3);
		}
	}
	else if (EGL::CNetEvent2Entities* e = dynamic_cast<EGL::CNetEvent2Entities*>(ev)) {
		lua_pushstring(L, "ActorId");
		lua_pushnumber(L, e->ActorId);
		lua_rawset(L, -3);
		lua_pushstring(L, "TargetId");
		lua_pushnumber(L, e->TargetId);
		lua_rawset(L, -3);
	}
	else if (EGL::CNetEventEntityAndPos* e = dynamic_cast<EGL::CNetEventEntityAndPos*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Position");
		luaext_pushPos(L, e->Position);
		lua_rawset(L, -3);
	}
	else if (EGL::CNetEventEntityAndPosArray* e = dynamic_cast<EGL::CNetEventEntityAndPosArray*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Positions");
		lua_newtable(L);
		int i = 1;
		for (shok::Position p : e->Positions) {
			luaext_pushPos(L, p);
			lua_rawseti(L, -2, i);
			i++;
		}
		lua_rawset(L, -3);
		lua_pushstring(L, "Rotation");
		lua_pushnumber(L, e->Rotation == -1.0f ? -1.0 : rad2deg<double>(e->Rotation));
		lua_rawset(L, -3);
	}
	else if (GGL::CNetEventExtractResource* e = dynamic_cast<GGL::CNetEventExtractResource*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "ResourceType");
		lua_pushnumber(L, static_cast<int>(e->ResourceType));
		lua_rawset(L, -3);
		lua_pushstring(L, "Position");
		luaext_pushPos(L, e->Position);
		lua_rawset(L, -3);
	}
	else if (GGL::CNetEventTransaction* e = dynamic_cast<GGL::CNetEventTransaction*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "BuyType");
		lua_pushnumber(L, static_cast<int>(e->BuyType));
		lua_rawset(L, -3);
		lua_pushstring(L, "BuyAmount");
		lua_pushnumber(L, e->BuyAmount);
		lua_rawset(L, -3);
		lua_pushstring(L, "SellType");
		lua_pushnumber(L, static_cast<int>(e->SellType));
		lua_rawset(L, -3);
	}
	else if (EGL::CNetEventPlayerID* e = dynamic_cast<EGL::CNetEventPlayerID*>(ev)) {
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		if (GGL::CNetEventBuildingCreator* e = dynamic_cast<GGL::CNetEventBuildingCreator*>(ev)) {
			lua_pushstring(L, "UpgradeCategory");
			lua_pushnumber(L, e->UpgradeCategory);
			lua_rawset(L, -3);
			lua_pushstring(L, "Position");
			luaext_pushPosRot(L, e->Position, true);
			lua_rawset(L, -3);
			lua_pushstring(L, "Serfs");
			lua_newtable(L);
			int i = 1;
			for (int s : e->Serfs) {
				lua_pushnumber(L, s);
				lua_rawseti(L, -2, i);
				i++;
			}
			lua_rawset(L, -3);
		}
		else if (EGL::CNetEventIntegerAndPlayerID* e = dynamic_cast<EGL::CNetEventIntegerAndPlayerID*>(ev)) {
			lua_pushstring(L, "Data");
			lua_pushnumber(L, e->Data);
			lua_rawset(L, -3);
		}
		else if (EGL::CNetEventPlayerIDAndInteger* e = dynamic_cast<EGL::CNetEventPlayerIDAndInteger*>(ev)) {
			lua_pushstring(L, "Data");
			lua_pushnumber(L, e->Data);
			lua_rawset(L, -3);
		}
		else if (EGL::CNetEventEntityIDAndPlayerID* e = dynamic_cast<EGL::CNetEventEntityIDAndPlayerID*>(ev)) {
			lua_pushstring(L, "EntityId");
			lua_pushnumber(L, e->EntityId);
			lua_rawset(L, -3);
			if (EGL::CNetEventEntityIDAndPlayerIDAndEntityType* e = dynamic_cast<EGL::CNetEventEntityIDAndPlayerIDAndEntityType*>(ev)) {
				lua_pushstring(L, "EntityType");
				lua_pushnumber(L, e->EntityType);
				lua_rawset(L, -3);
			}
			else if (GGL::CNetEventEntityIDPlayerIDAndInteger* e = dynamic_cast<GGL::CNetEventEntityIDPlayerIDAndInteger*>(ev)) {
				lua_pushstring(L, "Data");
				lua_pushnumber(L, e->Data);
				lua_rawset(L, -3);
			}
		}
	}
	else if (Framework::CEventGameSpeed* e = dynamic_cast<Framework::CEventGameSpeed*>(ev)) {
		lua_pushstring(L, "Speed");
		lua_pushnumber(L, e->SpeedFactor);
		lua_rawset(L, -3);
	}
}
bool l_netReadEvent(lua_State* L, BB::CEvent* ev) {
	bool allRead = true;
	if (EGL::CNetEventEntityID* e = dynamic_cast<EGL::CNetEventEntityID*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		if (GGL::CNetEventCannonCreator* e = dynamic_cast<GGL::CNetEventCannonCreator*>(ev)) {
			lua_pushstring(L, "BottomType");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->BottomType = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
			lua_pushstring(L, "TopType");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->TopType = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
			lua_pushstring(L, "Position");
			lua_rawget(L, -2);
			if (lua_istable(L, -1))
				luaext_checkPos(L, e->Position, -1);
			else
				allRead = false;
			lua_pop(L, 1);
		}
		else if (GGL::CNetEventEntityIDAndUpgradeCategory* e = dynamic_cast<GGL::CNetEventEntityIDAndUpgradeCategory*>(ev)) {
			lua_pushstring(L, "UpgradeCategory");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->UpgradeCategory = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
		}
		else if (EGL::CNetEventEntityIDAndInteger* e = dynamic_cast<EGL::CNetEventEntityIDAndInteger*>(ev)) {
			lua_pushstring(L, "Data");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->Data = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
		}
		else if (GGL::CNetEventTechnologyAndEntityID* e = dynamic_cast<GGL::CNetEventTechnologyAndEntityID*>(ev)) {
			lua_pushstring(L, "Technology");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->Technology = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
		}
	}
	else if (EGL::CNetEvent2Entities* e = dynamic_cast<EGL::CNetEvent2Entities*>(ev)) {
		lua_pushstring(L, "ActorId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->ActorId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "TargetId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->TargetId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (EGL::CNetEventEntityAndPos* e = dynamic_cast<EGL::CNetEventEntityAndPos*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Position");
		lua_rawget(L, -2);
		if (lua_istable(L, -1))
			luaext_checkPos(L, e->Position, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (EGL::CNetEventEntityAndPosArray* e = dynamic_cast<EGL::CNetEventEntityAndPosArray*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Positions");
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			int i = 1;
			auto v = e->Positions.SaveVector();
			shok::Position p{};
			v.Vector.clear();
			while (true) {
				lua_rawgeti(L, -1, i);
				if (!lua_istable(L, -1)) {
					lua_pop(L, 1);
					break;
				}
				luaext_checkPos(L, p, -1);
				lua_pop(L, 1);
				v.Vector.push_back(p);
				i++;
			}
		}
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Rotation");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1)) {
			e->Rotation = luaL_checkfloat(L, -1);
			if (e->Rotation != -1.0f) {
				e->Rotation = static_cast<float>(deg2rad<double>(e->Rotation));
			}
		}
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (GGL::CNetEventExtractResource* e = dynamic_cast<GGL::CNetEventExtractResource*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "ResourceType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->ResourceType = luaext_getresourcetype(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Position");
		lua_rawget(L, -2);
		if (lua_istable(L, -1))
			luaext_checkPos(L, e->Position, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (GGL::CNetEventTransaction* e = dynamic_cast<GGL::CNetEventTransaction*>(ev)) {
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "BuyType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->BuyType = luaext_getresourcetype(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "BuyAmount");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->BuyAmount = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "SellType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->SellType = luaext_getresourcetype(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (EGL::CNetEventPlayerID* e = dynamic_cast<EGL::CNetEventPlayerID*>(ev)) {
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = luaL_checkint(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		if (GGL::CNetEventBuildingCreator* e = dynamic_cast<GGL::CNetEventBuildingCreator*>(ev)) {
			lua_pushstring(L, "UpgradeCategory");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->UpgradeCategory = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
			lua_pushstring(L, "Position");
			lua_rawget(L, -2);
			if (lua_istable(L, -1))
				luaext_checkPosRot(L, e->Position, -1, true);
			else
				allRead = false;
			lua_pop(L, 1);
			lua_pushstring(L, "Serfs");
			lua_rawget(L, -2);
			if (lua_istable(L, -1)) {
				int i = 1;
				auto v = e->Serfs.SaveVector();
				v.Vector.clear();
				while (true) {
					lua_rawgeti(L, -1, i);
					if (!lua_isnumber(L, -1)) {
						lua_pop(L, 1);
						break;
					}
					v.Vector.push_back(luaL_checkint(L, -1));
					lua_pop(L, 1);
					i++;
				}
			}
			else
				allRead = false;
			lua_pop(L, 1);
		}
		else if (EGL::CNetEventIntegerAndPlayerID* e = dynamic_cast<EGL::CNetEventIntegerAndPlayerID*>(ev)) {
			lua_pushstring(L, "Data");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->Data = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
		}
		else if (EGL::CNetEventPlayerIDAndInteger* e = dynamic_cast<EGL::CNetEventPlayerIDAndInteger*>(ev)) {
			lua_pushstring(L, "Data");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->Data = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
		}
		else if (EGL::CNetEventEntityIDAndPlayerID* e = dynamic_cast<EGL::CNetEventEntityIDAndPlayerID*>(ev)) {
			lua_pushstring(L, "EntityId");
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
				e->EntityId = luaL_checkint(L, -1);
			else
				allRead = false;
			lua_pop(L, 1);
			if (EGL::CNetEventEntityIDAndPlayerIDAndEntityType* e = dynamic_cast<EGL::CNetEventEntityIDAndPlayerIDAndEntityType*>(ev)) {
				lua_pushstring(L, "EntityType");
				lua_rawget(L, -2);
				if (lua_isnumber(L, -1))
					e->EntityType = luaL_checkint(L, -1);
				else
					allRead = false;
				lua_pop(L, 1);
			}
			else if (GGL::CNetEventEntityIDPlayerIDAndInteger* e = dynamic_cast<GGL::CNetEventEntityIDPlayerIDAndInteger*>(ev)) {
				lua_pushstring(L, "Data");
				lua_rawget(L, -2);
				if (lua_isnumber(L, -1))
					e->Data = luaL_checkint(L, -1);
				else
					allRead = false;
				lua_pop(L, 1);
			}
		}
	}
	// read back speed change?
	return allRead;
}

int l_netEventReadBack(lua_State* L) {
	BB::CEvent* ev = static_cast<BB::CEvent*>(lua_touserdata(L, lua_upvalueindex(1)));
	lua_pushboolean(L, l_netReadEvent(L, ev));
	return 1;
}

int l_netEventSetHook(lua_State* L) {
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	lua_pushlightuserdata(L, &l_netEventSetHook);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	GGUI::CManager::PostEventCallback = [](BB::CEvent* ev) {
		int id = ev->EventTypeId;
		lua_State* L = *EScr::CScriptTriggerSystem::GameState;
		int top = lua_gettop(L);
		luaL_checkstack(L, 10, "");
		lua_pushlightuserdata(L, &l_netEventSetHook);
		lua_rawget(L, LUA_REGISTRYINDEX);
		bool r = false;
		if (lua_isfunction(L, -1)) {
			lua_pushnumber(L, id);
			l_netPushEvent(L, ev);
			lua_pushlightuserdata(L, ev);
			lua_pushcclosure(L, &l_netEventReadBack, 1);
			lua_pcall(L, 3, 1, 0);
			if (lua_isboolean(L, -1))
				r = lua_toboolean(L, -1);
		}
		lua_settop(L, top);
		return r;
	};
	GGUI::CManager::GlobalObj()->HackPostEvent();
	return 0;
}
int l_netEventUnSetHook(lua_State* L) {
	GGUI::CManager::PostEventCallback = nullptr;
	return 0;
}


int l_playerGetKillStatistics(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
	lua_pushnumber(L, p->Statistics.NumberOfUnitsKilled);
	lua_pushnumber(L, p->Statistics.NumberOfUnitsDied);
	lua_pushnumber(L, p->Statistics.NumberOfBuildingsDestroyed);
	lua_pushnumber(L, p->Statistics.NumberOfBuildingsLost);
	return 4;
}

int l_logicCanPLaceBuildingAt(lua_State* L) {
	int ty = luaL_checkint(L, 1);
	GGlue::CGlueEntityProps* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ty);
	luaext_assertPointer(L, ety, "no entitytype");
	luaext_assert(L, ety->IsBuildingType(), "not a building");
	int pl = luaL_checkint(L, 2);
	shok::Position p;
	luaext_checkPos(L, p, 3);
	p.FloorToBuildingPlacement();
	float r = deg2rad(luaL_checkfloat(L, 4));
	if (lua_isnumber(L, 5))
		lua_pushboolean(L, GGL::CPlayerStatus::CanPlaceBuilding(ty, pl, &p, r, luaL_checkint(L, 5)));
	else
		lua_pushboolean(L, GGL::CPlayerStatus::CanPlaceBuildingAtPos(ty, pl, &p, r));
	return 1;
}

int l_logicPlayerActivateAlarm(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
	luaext_assert(L, !p->WorkerAlarmMode, "alarm active");
	luaext_assert(L, p->AlarmRechargeTime <= 0, "alarm cooldown");
	(*GGL::CGLGameLogic::GlobalObj)->EnableAlarmForPlayer(i);
	return 0;
}
int l_logicPlayerDeactivateAlarm(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	GGL::CPlayerStatus* p = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
	luaext_assert(L, p->WorkerAlarmMode, "alarm not active");
	(*GGL::CGLGameLogic::GlobalObj)->DisableAlarmForPlayer(i);
	return 0;
}

int l_logicPlayerUpgradeSettlerCategory(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int ucat = luaL_checkint(L, 2);
	auto* u = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->SettlerUpgradeManager->UpgradeCategories.GetFirstMatch([ucat](GGL::CSettlerUpgradeManager::UCatEntry* u) {return u->UCat == ucat; });
	luaext_assertPointer(L, u, "invalid ucat");
	(*GGL::CGLGameLogic::GlobalObj)->UpgradeSettlerCategory(i, ucat);
	return 0;
}

int l_logicPlayerSetTaxLevel(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int tl = luaL_checkint(L, 2);
	luaext_assert(L, tl >= 0 && tl < 5, "invalid taxlevel");
	(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->TaxLevel = tl;
	return 0;
}

int l_logicPlayerActivateWeatherMachine(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int w = luaL_checkint(L, 2);
	luaext_assert(L, w > 0 && w < 4, "invalid weathertype");
	luaext_assert(L, !(*GGL::CGLGameLogic::GlobalObj)->WeatherHandler->WeatherChange.StateToChangeTo, "weather currently changing");
	luaext_assert(L, (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->CurrentResources.WeatherEnergy >= (*GGL::CLogicProperties::GlobalObj)->EnergyRequiredForWeatherChange, "not enough weather energy");
	(*GGL::CGLGameLogic::GlobalObj)->PlayerActivateWeathermachine(i, w);
	return 0;
}

int l_logicPlayerBlessSettlers(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int b = luaL_checkint(L, 2);
	float faithneeded = -1;
	for (GGL::CLogicProperties::SBlessCategory& bcat : (*GGL::CLogicProperties::GlobalObj)->BlessCategories) {
		if (bcat.Name == b) {
			faithneeded = bcat.RequiredFaith;
			break;
		}
	}
	luaext_assert(L, faithneeded >= 0, "invalid blesscategory");
	luaext_assert(L, (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i)->CurrentResources.Faith >= faithneeded, "not enough faith");
	(*GGL::CGLGameLogic::GlobalObj)->PlayerBlessSettlers(i, b);
	return 0;
}

int l_logicLandscapeGetSector(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetSector(&p));
	return 1;
}

int l_logicLandscapeGetNearesUnblockedPosInSector(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	int s = luaL_checkint(L, 2);
	float r = luaL_checkfloat(L, 3);
	shok::Position po;
	if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->GetNearestPositionInSector(&p, r, s, &po))
		luaext_pushPos(L, po);
	else
		lua_pushnil(L);
	return 1;
}

int l_logicHookHurtEntity(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "use CEntity instead");
	EGL::CGLEEntity::HookHurtEntity();
	if (lua_isboolean(L, 1))
		EGL::CGLEEntity::HurtEntityCallWithNoAttacker = lua_toboolean(L, 1);
	else
		EGL::CGLEEntity::HurtEntityCallWithNoAttacker = true;
	if (lua_isfunction(L, 2)) {
		lua_pushlightuserdata(L, &l_logicHookHurtEntity);
		lua_pushvalue(L, 2);
		lua_rawset(L, LUA_REGISTRYINDEX);

		EGL::CGLEEntity::HurtEntityOnKillCb = [](EGL::CGLEEntity* att, EGL::CGLEEntity* kill, int attpl, shok::AdvancedDealDamageSource sourc) {
			lua_State* L = *EScr::CScriptTriggerSystem::GameState;

			int t = lua_gettop(L);

			lua_pushlightuserdata(L, &l_logicHookHurtEntity);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, att ? att->EntityId : 0);
			lua_pushnumber(L, kill->EntityId);
			lua_pushnumber(L, attpl);
			lua_pushnumber(L, static_cast<int>(sourc));
			lua_pcall(L, 4, 0, 0);

			lua_settop(L, t);
		};
	}
	else {
		EGL::CGLEEntity::HurtEntityOnKillCb = nullptr;
		lua_pushlightuserdata(L, &l_logicHookHurtEntity);
		lua_pushnil(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
	}
	return 0;
}

int l_logicHurtEntityGetDamage(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "use CEntity instead");
	luaext_assertPointer(L, EGL::CGLEEntity::HurtEntityDamagePointer, "not in trigger");
	lua_pushnumber(L, *EGL::CGLEEntity::HurtEntityDamagePointer);
	lua_pushnumber(L, static_cast<int>(EGL::CGLEEntity::HurtEntityDamageSource));
	lua_pushnumber(L, EGL::CGLEEntity::HurtEntityAttackerPlayer);
	return 3;
}
int l_logicHurtEntitySetDamage(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "use CEntity instead");
	luaext_assertPointer(L, EGL::CGLEEntity::HurtEntityDamagePointer, "not in trigger");
	*EGL::CGLEEntity::HurtEntityDamagePointer = luaL_checkint(L, 1);
	return 0;
}

int l_logicGetLoadOrder(lua_State* L) {
	lua_newtable(L);
	int r = 1;
	for (BB::IFileSystem* a : (*BB::CFileSystemMgr::GlobalObj)->LoadOrder) {
		if (BB::CDirectoryFileSystem* a2 = dynamic_cast<BB::CDirectoryFileSystem*>(a)) {
			lua_pushstring(L,a2->Path);
			lua_rawseti(L, -2, r);
			r++;
		}
		else if (BB::CBBArchiveFile* a2 = dynamic_cast<BB::CBBArchiveFile*>(a)) {
			lua_pushstring(L, a2->Path);
			lua_rawseti(L, -2, r);
			r++;
		}
		else {
			lua_pushstring(L, typeid(*a).name());
			lua_rawseti(L, -2, r);
			r++;
		}
	}
	return 1;
}

int l_logicAddArchive(lua_State* L) {
	const char* s = luaL_checkstring(L, 1);
	luaext_assert(L, std::filesystem::exists(s), "file doesnt exist");
	try {
		(*BB::CFileSystemMgr::GlobalObj)->AddArchive(s);
	}
	catch (const BB::CFileException& e) {
		char buff[201]{};
		e.CopyMessage(buff, 200);
		lua_pushstring(L, buff);
		lua_error(L);
	}
	return 0;
}

int l_logicRemoveTop(lua_State* L) {
	if ((*BB::CFileSystemMgr::GlobalObj)->LoadOrder.size() <= 0)
		return 0;
	BB::CBBArchiveFile* a = dynamic_cast<BB::CBBArchiveFile*>((*BB::CFileSystemMgr::GlobalObj)->LoadOrder[0]);
	luaext_assert(L, a, "may only remove archives");
	luaext_assert(L, str_ends_with(a->Path, ".s5x"), "may only remove maps");
	(*BB::CFileSystemMgr::GlobalObj)->RemoveTopArchive();
	return 0;
}

int l_logicAddFolder(lua_State* L) {
	const char* s = luaL_checkstring(L, 1);
	(*BB::CFileSystemMgr::GlobalObj)->AddFolder(s);
	return 0;
}

int l_logicEnableMaxHpTechMod(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supportet with SCELoader");
	EGL::CGLEEntity::HookMaxHP();
	EGL::CGLEEntity::UseMaxHPTechBoni = true;
	return 0;
}

int l_logicLandscapeGetTerrainType(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetTerrainTypeAt(p));
	return 1;
}
int l_logicLandscapeGetWaterType(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetWaterTypeAt(p));
	return 1;
}
int l_logicLandscapeGetWaterHeight(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetWaterHeightAt(p));
	return 1;
}
int l_logicLandscapeGetTerrainHeight(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes->GetTerrainHeight(p));
	return 1;
}
int l_logicLandscapeGetTerrainVertexColor(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*EGL::CGLEGameLogic::GlobalObj)->Landscape->VertexColors->GetTerrainVertexColor(p));
	return 1;
}
int l_logicLandscapeGetBlocking(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, static_cast<int>((*ED::CGlobalsLogicEx::GlobalObj)->GetBlocking(p)));
	return 1;
}
int l_logicLandscapeGetBridgeHeight(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes->GetBridgeHeight(p));
	return 1;
}

int l_logicGetColor(lua_State* L) { // ind -> r,g,b,a
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	int i = luaL_checkint(L, 1);
	shok::Color c = (*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->GetColorByIndex(i);
	lua_pushnumber(L, c.R);
	lua_pushnumber(L, c.G);
	lua_pushnumber(L, c.B);
	lua_pushnumber(L, c.A);
	return 4;
}
int l_logicSetColor(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	int i = luaL_checkint(L, 1);
	int r = luaL_checkint(L, 2), g = luaL_checkint(L, 3), b = luaL_checkint(L, 4);
	int a = luaL_optint(L, 5, 255);
	(*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->SetColorByIndex(i, shok::Color(r, g, b, a));
	(*ED::CGlobalsBaseEx::GlobalObj)->PlayerColors->RefreshPlayerColors();
	return 0;
}

int l_logicEnablePlayerPaydayCallback(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "use global GameCallback_PaydayPayed instead");
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	GGL::CPlayerAttractionHandler::HookCheckPayday();
	lua_pushlightuserdata(L, &l_logicEnablePlayerPaydayCallback);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	GGL::CPlayerAttractionHandler::OnCheckPayDayCallback = [](GGL::CPlayerAttractionHandler* th) {
		lua_State* L2 = *EScr::CScriptTriggerSystem::GameState;
		int t = lua_gettop(L2);
		lua_pushlightuserdata(L2, &l_logicEnablePlayerPaydayCallback);
		lua_rawget(L2, LUA_REGISTRYINDEX);
		lua_pushnumber(L2, th->PlayerID);
		lua_pushnumber(L2, th->GetWorkerPaydayIncome());
		lua_pcall(L2, 2, 1, 0);
		if (lua_isnumber(L2, -1)) {
			float add = luaL_checkfloat(L2, -1);
			if (add > 0)
				(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(th->PlayerID)->CurrentResources.AddToType(shok::ResourceType::GoldRaw, add);
			else if (add < 0)
				(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(th->PlayerID)->CurrentResources.SubFromType(shok::ResourceType::Gold, -add);
		}
		lua_settop(L2, t);
	};
	return 0;
}

int l_logicSetRegenerateSoldiers(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	EGL::CGLEEntity::HookLeaderRegen();
	EGL::CGLEEntity::LeaderRegenRegenerateSoldiers = lua_toboolean(L, 1);
	return 0;
}

int l_logicOverrideStringTableText(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	if (L != *EScr::CScriptTriggerSystem::GameState)
		luaL_error(L, "does only work ingame");
	if (!shok::GetStringTableTextOverride) {
		shok::HookGetStringTableText();
		lua_pushlightuserdata(L, &l_logicOverrideStringTableText);
		lua_newtable(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		shok::GetStringTableTextOverride = [](const char* s) {
			if (!s)
				return s;
			const char* r = nullptr;
			int t = lua_gettop(*EScr::CScriptTriggerSystem::GameState);

			lua_pushlightuserdata(*EScr::CScriptTriggerSystem::GameState, &l_logicOverrideStringTableText);
			lua_rawget(*EScr::CScriptTriggerSystem::GameState, LUA_REGISTRYINDEX);
			lua_pushstring(*EScr::CScriptTriggerSystem::GameState, s);
			luaext_tolower(*EScr::CScriptTriggerSystem::GameState);
			lua_rawget(*EScr::CScriptTriggerSystem::GameState, -2);
			if (lua_isstring(*EScr::CScriptTriggerSystem::GameState, -1)) {
				r = lua_tostring(*EScr::CScriptTriggerSystem::GameState, -1); // stored in registry, so its ok to return it
			}

			lua_settop(*EScr::CScriptTriggerSystem::GameState, t);
			return r;
		};
	}

	luaext_assert(L, lua_isstring(L, 1), "key not string");
	luaext_assert(L, lua_isstring(L, 2) || lua_isnil(L, 2), "replacement not string or nil");

	lua_pushlightuserdata(L, &l_logicOverrideStringTableText);
	lua_rawget(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, 1);
	luaext_tolower(L);
	lua_pushvalue(L, 2);
	lua_rawset(L, -3);
	return 0;
}

int l_logic_SetPlaceBuildingCb(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	if (lua_isnil(L, 1)) {
		GGL::CPlayerStatus::CanPlaceBuildingCallback = nullptr;
		lua_pushlightuserdata(L, &l_logic_SetPlaceBuildingCb);
		lua_pushnil(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		return 0;
	}

	luaext_assert(L, lua_isfunction(L, 1), "no func at 1");
	lua_pushlightuserdata(L, &l_logic_SetPlaceBuildingCb);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);


	if (!GGL::CPlayerStatus::CanPlaceBuildingCallback) {
		GGL::CPlayerStatus::CanPlaceBuildingCallback = [](int entitytype, int player, shok::Position* pos, float rotation, int buildOnId) {
			lua_State* L = *EScr::CScriptTriggerSystem::GameState;
			int t = lua_gettop(L);
			bool r = true;

			lua_pushlightuserdata(L, &l_logic_SetPlaceBuildingCb);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, entitytype);
			lua_pushnumber(L, player);
			luaext_pushPos(L, *pos);
			lua_pushnumber(L, rad2deg(rotation));
			lua_pushnumber(L, buildOnId);
			lua_pcall(L, 5, 1, 0);
			if (lua_isboolean(L, -1))
				r = lua_toboolean(L, -1);

			lua_settop(L, t);
			return r;
		};
		GGL::CPlayerStatus::HookCanPlaceBuilding();
	}
	return 0;
}

int l_logic_SetPlaceBuildingRotation(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	GGUI::CPlaceBuildingState::HookPlacementRotation();
	GGUI::CPlaceBuildingState::PlacementRotation = deg2rad(luaL_checkfloat(L, 1));
	GGUI::CPlaceBuildingState* s = dynamic_cast<GGUI::CPlaceBuildingState*>(GGUI::CManager::GlobalObj()->C3DViewHandler->CurrentState);
	if (s)
		s->UpdateModel();
	return 0;
}
int l_logic_GetPlaceBuildingRotation(lua_State* L) {
	if (CppLogic::HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	lua_pushnumber(L, rad2deg(GGUI::CPlaceBuildingState::PlacementRotation));
	return 1;
}

int l_logic_FixSnipeDamage(lua_State* L) {
	GGL::CSniperAbility::OverrideSnipeTask();
	if (lua_isnil(L, 1)) {
		GGL::CSniperAbility::SnipeDamageOverride = nullptr;
		return 0;
	}

	luaext_assert(L, lua_isfunction(L, 1), "no func at 1");
	lua_pushlightuserdata(L, &l_logic_FixSnipeDamage);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);


	if (!GGL::CSniperAbility::SnipeDamageOverride) {
		GGL::CSniperAbility::SnipeDamageOverride = [](EGL::CGLEEntity* sniper, EGL::CGLEEntity* tar, int dmg) {
			lua_State* L = *EScr::CScriptTriggerSystem::GameState;
			int t = lua_gettop(L);

			lua_pushlightuserdata(L, &l_logic_FixSnipeDamage);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, sniper->EntityId);
			lua_pushnumber(L, tar->EntityId);
			lua_pushnumber(L, dmg);
			lua_pcall(L, 3, 1, 0);
			if (lua_isnumber(L, -1))
				dmg = luaL_checkint(L, -1);

			lua_settop(L, t);
			return dmg;
		};
	}
	
	return 0;
}

int l_logic_GetCurrentWeatherGFXState(lua_State* L) {
	int state = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler->WeatherChange.CurrentWeatherGFXState;
	lua_pushnumber(L, state);
	return 1;
}
int l_logic_GetWeatherQueue(lua_State* L) {
	GGL::CWeatherHandler* wh = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler;
	lua_newtable(L);
	for (const auto& kae : wh->Elements) {
		lua_newtable(L);
		lua_pushstring(L, "State");
		lua_pushnumber(L, kae.WeatherElement.State);
		lua_rawset(L, -3);
		lua_pushstring(L, "GFX");
		lua_pushnumber(L, kae.WeatherElement.GfxSet);
		lua_rawset(L, -3);
		lua_pushstring(L, "IsPeriodic");
		lua_pushboolean(L, kae.WeatherElement.IsPeriodic);
		lua_rawset(L, -3);
		lua_pushstring(L, "Length");
		lua_pushnumber(L, kae.WeatherElement.Length);
		lua_rawset(L, -3);
		lua_pushstring(L, "Forerun");
		lua_pushnumber(L, kae.WeatherElement.Forerun);
		lua_rawset(L, -3);

		lua_rawseti(L, -2, kae.WeatherElement.StartTimeOffset);
	}
	lua_pushnumber(L, wh->CurrentWeatherOffset);
	lua_pushnumber(L, wh->NextPeriodicWeatherStartTimeOffset);
	return 3;
}
int l_logic_ClearWeatherAndAddInitial(lua_State* L) {
	GGL::CWeatherHandler* wh = (*GGL::CGLGameLogic::GlobalObj)->WeatherHandler;
	int state = luaL_checkint(L, 1);
	luaext_assert(L, state >= 1 && state <= 3, "no weather state");
	int dur = luaL_checkint(L, 2) * 10;
	luaext_assert(L, dur > 50, "duration too short");
	int fore = luaL_checkint(L, 3) * 10;
	luaext_assert(L, fore > 0 && fore <= 100, "forerun to small or too big");
	int gfx = luaL_checkint(L, 4);
	luaext_assert(L, gfx > 0, "gfx <=0");
	int trans = luaL_checkint(L, 5) * 10;
	luaext_assert(L, trans > fore && trans <= 200, "transition too small or too big");
	if (wh->Elements.size == 0) {
		wh->AddWeatherElement(state, dur, true, fore, gfx, trans);
	}
	else {
		wh->ClearQueue(state, dur, fore, gfx, trans);
	}
	return 0;
}

struct l_logic_setluataskfunc_info {
	EGL::CGLEEntity* e;
	bool HasSetTl, HasMoved, Ret;
};
int l_logic_setluataskfunc_move(lua_State* L) {
	l_logic_setluataskfunc_info* d = static_cast<l_logic_setluataskfunc_info*>(lua_touserdata(L, lua_upvalueindex(1)));
	shok::Position p;
	luaext_checkPos(L, p, 1);
	EGL::CEventPosition e{ shok::EventIDs::Movement_TaskMoveToPos, p };
	d->e->FireEvent(&e);
	d->HasMoved = true;
	bool usepathing = true;
	if (lua_toboolean(L, 2))
		usepathing = false;
	d->e->GetBehavior<GGL::CBehaviorDefaultMovement>()->IsPathingUsed = usepathing;
	if (lua_toboolean(L, 3) && d->e->CurrentState == shok::TaskState::Move)
		d->e->CurrentState = shok::TaskState::Move_NonCancelable;
	return 0;
}
int l_logic_setluataskfunc_settl(lua_State* L) {
	l_logic_setluataskfunc_info* d = static_cast<l_logic_setluataskfunc_info*>(lua_touserdata(L, lua_upvalueindex(1)));
	int tid = luaL_checkint(L, 1);
	EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(tid);
	luaext_assertPointer(L, tl, "no tasklist");
	d->e->SetTaskList(tl);
	d->HasSetTl = true;
	return 0;
}
int l_logic_setluataskfunc(lua_State* L) {
	luaext_assert(L, !CppLogic::HasSCELoader(), "does not work with SCELoader");

	if (lua_isnil(L, 1)) {
		EGL::CGLEEntity::LuaTaskListCallback = nullptr;
		return 0;
	}

	luaext_assert(L, lua_isfunction(L, 1), "no func at 1");
	lua_pushlightuserdata(L, &l_logic_setluataskfunc);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);


	if (!EGL::CGLEEntity::LuaTaskListCallback) {
		EGL::CGLEEntity::LuaTaskListCallback = [](EGL::CGLEEntity* e, int val) {
			lua_State* L = *EScr::CScriptTriggerSystem::GameState;
			int t = lua_gettop(L);

			l_logic_setluataskfunc_info d{ e, false, false, false };
			lua_pushlightuserdata(L, &l_logic_setluataskfunc);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, e->EntityId);
			lua_pushnumber(L, val);
			lua_pushlightuserdata(L, &d);
			lua_pushcclosure(L, &l_logic_setluataskfunc_move, 1);
			lua_pushlightuserdata(L, &d);
			lua_pushcclosure(L, &l_logic_setluataskfunc_settl, 1);
			lua_pcall(L, 4, 1, 0);
			if (lua_toboolean(L, -1))
				d.Ret = true;

			lua_settop(L, t);
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

int l_logic_makeTaskListWaitForAnimUncancelable(lua_State* L) {
	luaext_assert(L, !CppLogic::HasSCELoader(), "does not work with SCELoader");

	int i = luaL_checkint(L, 1);
	EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(i);
	luaext_assertPointer(L, tl, "invalid tasklist");
	i = luaL_optint(L, 2, -1);
	if (i < 0) {
		for (EGL::CGLETaskArgs* p : tl->Task) {
			if (p->TaskType == shok::Task::TASK_WAIT_FOR_ANIM)
				p->TaskType = shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE;
		}
	}
	else {
		luaext_assert(L, i < static_cast<int>(tl->Task.size()), "invalid task");
		luaext_assert(L, tl->Task[i]->TaskType == shok::Task::TASK_WAIT_FOR_ANIM, "task not waitforanim");
		tl->Task[i]->TaskType = shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE;
	}
	return 0;
}
int l_logic_makeTaskListWaitForAnimCancelable(lua_State* L) {
	luaext_assert(L, !CppLogic::HasSCELoader(), "does not work with SCELoader");

	int i = luaL_checkint(L, 1);
	EGL::CGLETaskList* tl = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(i);
	luaext_assertPointer(L, tl, "invalid tasklist");
	i = luaL_optint(L, 2, -1);
	if (i < 0) {
		for (EGL::CGLETaskArgs* p : tl->Task) {
			if (p->TaskType == shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE)
				p->TaskType = shok::Task::TASK_WAIT_FOR_ANIM;
		}
	}
	else {
		luaext_assert(L, i < static_cast<int>(tl->Task.size()), "invalid task");
		luaext_assert(L, tl->Task[i]->TaskType == shok::Task::TASK_WAIT_FOR_ANIM_NON_CANCELABLE, "task not waitforanim_nocancel");
		tl->Task[i]->TaskType = shok::Task::TASK_WAIT_FOR_ANIM;
	}
	return 0;
}
int l_logic_setTaskListSetCheckUncancelable(lua_State* L) {
	luaext_assert(L, !CppLogic::HasSCELoader(), "does not work with SCELoader");
	EGL::CGLEEntity::HookSetTaskListNonCancelable(luaext_optbool(L, 1, false));
	return 0;
}

int l_logic_FixBuildOnMovement(lua_State* L) {
	EGL::CGLEEntity::BuildOnSetPosFixMovement = lua_toboolean(L, 1);
	if (EGL::CGLEEntity::BuildOnSetPosFixMovement)
		EGL::CGLEEntity::HookBuildOnSetPos();
	return 0;
}

int l_logic_GetNearestFreePosForBuilding(lua_State* L) {
	luaext_assert(L, luaext_checkEntityType(L, 1)->IsBuildingType(), "no building type");
	int ty = luaL_checkint(L, 1);
	shok::PositionRot pin;
	luaext_checkPosRot(L, pin, 2);
	float range = luaL_optfloat(L, 3, 0);
	if (range <= 0)
		range = (*GGL::CLogicProperties::GlobalObj)->BuildingPlacementSnapDistance;
	shok::PositionRot pout = GGUI::CPlaceBuildingState::GetNearestPlacementPos(ty, pin, range);
	luaext_pushPosRot(L, pout);
	return 1;
}

ED::ModelInstance::TransformOperation l_logicModel_checkTO(lua_State* L, int idx) {
	int i = luaL_optint(L, idx, static_cast<int>(ED::ModelInstance::TransformOperation::Multiply));
	luaext_assert(L, i >= 0 && i < 3, "invalid transform operation");
	return static_cast<ED::ModelInstance::TransformOperation>(i);
}
struct l_logicModel {
	ED::ModelInstance* Model = nullptr;

	static int Clear(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		if (m->Model) {
			m->Model->Destroy();
			m->Model = nullptr;
		}
		return 0;
	}
	static int SetModel(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		int mid = luaL_checkint(L, 2);
		luaext_assert(L, (*ED::CGlobalsBaseEx::GlobalObj)->ModelManager->GetNameByID(mid), "invalid model");
		if (m->Model) {
			m->Model->Destroy();
			m->Model = nullptr;
		}
		m->Model = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(mid)->Instanciate();
		m->Model->Register();
		return 0;
	}
	static int Translate(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		shok::Position p;
		luaext_checkPos(L, p, 2);
		float h = luaL_optfloat(L, 3, 0);
		if (luaext_optbool(L, 5, true)) {
			float t = (*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainHeightAtPos(p);
			if (luaext_optbool(L, 6, false)) {
				float w = (*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetWaterHeightAtPos(p);
				h += max(t, w);
			}
			else {
				h += t;
			}
		}
		m->Model->Translate(p, h, l_logicModel_checkTO(L, 4));
		return 0;
	}
	static int Rotate(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		float r = luaL_checkfloat(L, 2);
		m->Model->Rotate(r, l_logicModel_checkTO(L, 3));
		return 0;
	}
	static int Scale(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		float s = luaL_checkfloat(L, 2);
		m->Model->Scale(s, l_logicModel_checkTO(L, 3));
		return 0;
	}
	static int ResetTransform(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		m->Model->Rotate(0, ED::ModelInstance::TransformOperation::Set);
		return 0;
	}
	static int SetColorByPlayer(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		int p = luaL_checkint(L, 2);
		luaext_assert(L, p >= 0 && p <= 9, "invalid player");
		m->Model->SetColorByPlayerID(p);
		return 0;
	}
	static int DisableShadow(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		m->Model->DisableShadow();
		return 0;
	}
	static int DisableParticleEffects(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		m->Model->DisableParticleEffects();
		return 0;
	}
	static int DisableTerrainDecal(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		m->Model->DisableTerrainDecal();
		return 0;
	}
	static int SetColorModulate(lua_State* L) {
		l_logicModel* m = luaext_GetUserData<l_logicModel>(L, 1);
		luaext_assertPointer(L, m->Model, "set a model first");
		int r = luaL_checkint(L, 2);
		int g = luaL_checkint(L, 3);
		int b = luaL_checkint(L, 4);
		int a = luaL_optint(L, 5, 255);
		m->Model->SetColorModulate(a, r, g, b);
		return 0;
	}
	
	static constexpr const luaL_reg LuaMethods[] = {
		{"Clear", &Clear},
		{"SetModel", &SetModel},
		{"Translate", &Translate},
		{"Rotate", &Rotate},
		{"Scale", &Scale},
		{"ResetTransform", &ResetTransform},
		{"SetColorByPlayer", &SetColorByPlayer},
		{"DisableShadow", &DisableShadow},
		{"DisableParticleEffects", &DisableParticleEffects},
		{"DisableTerrainDecal", &DisableTerrainDecal},
		{"SetColorModulate", &SetColorModulate},
		{nullptr, nullptr},
	};

	~l_logicModel() {
		if (Model) {
			Model->Destroy();
			Model = nullptr;
		}
	};
};
int l_logicModel_create(lua_State* L) {
	luaext_newUserData<l_logicModel>(L);
	return 1;
}

int l_logic_loadtasks(lua_State* L)
{
	EGL::TaskData::AddExtraTasks();
	EGL::CGLEEntity::HookLuaTaskList();
	EGL::CGLEEntity::HookNonCancelableAnim();
	return 0;
}

void l_logic_onload()
{
	if (!CppLogic::HasSCELoader())
		l_logic_loadtasks(nullptr);
}

void l_logic_cleanup(lua_State* L) {
	l_netEventUnSetHook(L);
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
}

void l_logic_init(lua_State* L)
{
	luaext_registerFunc(L, "GetDamageFactor", l_logicGetDamageModifier);
	luaext_registerFunc(L, "SetDamageFactor", l_logicSetDamageModifier);
	luaext_registerFunc(L, "ReloadCutscene", &l_logicReloadCutscene);
	luaext_registerFunc(L, "GetAnimIdFromName", &l_logicGetAnimIdFromName);
	luaext_registerFunc(L, "PlayerGetPaydayStartetTick", &l_playerGetPaydayStatus);
	luaext_registerFunc(L, "PlayerSetPaydayStartetTick", &l_playerSetPaydayStatus);
	luaext_registerFunc(L, "SetPaydayFrequency", &l_logic_setpaydayfrequency);
	luaext_registerFunc(L, "PlayerGetKillStatistics", &l_playerGetKillStatistics);
	luaext_registerFunc(L, "CanPlaceBuildingAt", &l_logicCanPLaceBuildingAt);
	luaext_registerFunc(L, "PlayerActivateAlarm", &l_logicPlayerActivateAlarm);
	luaext_registerFunc(L, "PlayerDeactivateAlarm", &l_logicPlayerDeactivateAlarm);
	luaext_registerFunc(L, "PlayerUpgradeSettlerCategory", &l_logicPlayerUpgradeSettlerCategory);
	luaext_registerFunc(L, "PlayerSetTaxLevel", &l_logicPlayerSetTaxLevel);
	luaext_registerFunc(L, "PlayerActivateWeatherMachine", &l_logicPlayerActivateWeatherMachine);
	luaext_registerFunc(L, "PlayerBlessSettlers", &l_logicPlayerBlessSettlers);
	luaext_registerFunc(L, "LandscapeGetSector", &l_logicLandscapeGetSector);
	luaext_registerFunc(L, "LandscapeGetNearestUnblockedPosInSector", &l_logicLandscapeGetNearesUnblockedPosInSector);
	luaext_registerFunc(L, "EnableAllHurtEntityTrigger", &l_logicHookHurtEntity);
	luaext_registerFunc(L, "HurtEntityGetDamage", &l_logicHurtEntityGetDamage);
	luaext_registerFunc(L, "HurtEntitySetDamage", &l_logicHurtEntitySetDamage);
	luaext_registerFunc(L, "GetLoadOrder", &l_logicGetLoadOrder);
	luaext_registerFunc(L, "AddArchive", &l_logicAddArchive);
	luaext_registerFunc(L, "RemoveTopArchive", &l_logicRemoveTop);
#ifdef _DEBUG
	luaext_registerFunc(L, "AddFolder", &l_logicAddFolder);
#endif
	luaext_registerFunc(L, "EnableMaxHPTechMod", &l_logicEnableMaxHpTechMod);
	luaext_registerFunc(L, "LandscapeGetTerrainType", &l_logicLandscapeGetTerrainType);
	luaext_registerFunc(L, "LandscapeGetWaterType", &l_logicLandscapeGetWaterType);
	luaext_registerFunc(L, "LandscapeGetWaterHeight", &l_logicLandscapeGetWaterHeight);
	luaext_registerFunc(L, "LandscapeGetTerrainHeight", &l_logicLandscapeGetTerrainHeight);
	luaext_registerFunc(L, "LandscapeGetTerrainVertexColor", &l_logicLandscapeGetTerrainVertexColor);
	luaext_registerFunc(L, "LandscapeGetBlocking", &l_logicLandscapeGetBlocking);
	luaext_registerFunc(L, "LandscapeGetBridgeHeight", &l_logicLandscapeGetBridgeHeight);
	luaext_registerFunc(L, "GetColorByColorIndex", &l_logicGetColor);
	luaext_registerFunc(L, "SetColorByColorIndex", &l_logicSetColor);
	luaext_registerFunc(L, "SetPaydayCallback", &l_logicEnablePlayerPaydayCallback);
	luaext_registerFunc(L, "SetLeadersRegenerateTroopHealth", &l_logicSetRegenerateSoldiers);
	luaext_registerFunc(L, "SetStringTableText", &l_logicOverrideStringTableText);
	luaext_registerFunc(L, "SetPlaceBuildingAdditionalCheck", &l_logic_SetPlaceBuildingCb);
	luaext_registerFunc(L, "SetPlaceBuildingRotation", &l_logic_SetPlaceBuildingRotation);
	luaext_registerFunc(L, "GetPlaceBuildingRotation", &l_logic_GetPlaceBuildingRotation);
	luaext_registerFunc(L, "FixSnipeDamage", &l_logic_FixSnipeDamage);
	luaext_registerFunc(L, "GetCurrentWeatherGFXState", &l_logic_GetCurrentWeatherGFXState);
	luaext_registerFunc(L, "GetWeatherQueue", &l_logic_GetWeatherQueue);
	luaext_registerFunc(L, "ClearWeatherQueueAndAddInitial", &l_logic_ClearWeatherAndAddInitial);
	luaext_registerFunc(L, "SetLuaTaskListFunc", &l_logic_setluataskfunc);
	luaext_registerFunc(L, "TaskListMakeWaitForAnimsUnCancelable", &l_logic_makeTaskListWaitForAnimUncancelable);
	luaext_registerFunc(L, "TaskListMakeWaitForAnimsCancelable", &l_logic_makeTaskListWaitForAnimCancelable);
	luaext_registerFunc(L, "TaskListSetChangeTaskListCheckUncancelable", &l_logic_setTaskListSetCheckUncancelable);
	luaext_registerFunc(L, "EnableBuildOnMovementFix", &l_logic_FixBuildOnMovement);
	luaext_registerFunc(L, "CreateFreeModel", &l_logicModel_create);
	luaext_registerFunc(L, "GetNearestFreePosForBuilding", &l_logic_GetNearestFreePosForBuilding);

	lua_pushstring(L, "UICommands");
	lua_newtable(L);
	luaext_registerFunc(L, "SetCallback", &l_netEventSetHook);
	luaext_registerFunc(L, "UnSetCallback", &l_netEventUnSetHook);
	lua_rawset(L, -3);

	if (L != shok::LuaStateMainmenu) {
		luaext_prepareUserDataType<l_logicModel>(L);
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

#include "pch.h"
#include "l_logic.h"
#include "luaext.h"
#include <assert.h>

int l_logicGetDamageModifier(lua_State* L) {
	int dmgclass = luaL_checkint(L, 1);
	int size = (int)(*shok_DamageClassHolderObj)->DamageClassList.size();
	luaext_assert(L, dmgclass > 0 && dmgclass < size, "invalid damagecass");
	int amclass = luaL_checkint(L, 2);
	amclass--;
	luaext_assert(L, amclass >= 0 && amclass < 7, "invalid armorclass");
	assert(amclass >= 0 && amclass < 7);
	lua_pushnumber(L, (*shok_DamageClassHolderObj)->DamageClassList[dmgclass]->BonusVsArmorClass[amclass]);
	return 1;
}
int l_logicSetDamageModifier(lua_State* L) {
	int dmgclass = luaL_checkint(L, 1);
	int size = (int)(*shok_DamageClassHolderObj)->DamageClassList.size();
	luaext_assert(L, dmgclass > 0 && dmgclass < size, "invalid damagecass");
	int amclass = luaL_checkint(L, 2);
	amclass--;
	luaext_assert(L, amclass >= 0 && amclass < 7, "invalid armorclass");
	assert(amclass >= 0 && amclass < 7);
	(*shok_DamageClassHolderObj)->DamageClassList[dmgclass]->BonusVsArmorClass[amclass] = luaL_checkfloat(L, 2);
	return 0;
}

int l_logicReloadCutscene(lua_State* L) {
	const char* data = luaL_optlstring(L, 1, "Maps\\ExternalMap", nullptr);
	(**shok_ECS_CManagerObject)->ReloadCutscene(data);
	return 0;
}

int l_logicGetAnimIdFromName(lua_State* L) {
	const char* data = luaL_checkstring(L, 1);
	int id = (*shok_BB_CIDManagerExObj)->GetAnimIdByName(data);
	lua_pushnumber(L, id);
	return 1;
}

int l_playerGetPaydayStatus(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	if (p->PlayerAttractionHandler->PaydayStarted)
		lua_pushnumber(L, p->PlayerAttractionHandler->PaydayStartTick);
	else
		lua_pushnumber(L, -1);
	return 1;
}
int l_playerSetPaydayStatus(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int st = luaL_checkint(L, 2);
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	if (st < 0) {
		p->PlayerAttractionHandler->PaydayStarted = 0;
	}
	else {
		p->PlayerAttractionHandler->PaydayStarted = 1;
		p->PlayerAttractionHandler->PaydayStartTick = st;
	}
	return 0;
}


void l_netPushEvent(lua_State* L, shok_BB_CEvent* ev) {
	lua_newtable(L);
	if (ev->vtable == shok_vtp_EGL_CNetEventEntityID) {
		shok_EGL_CNetEventEntityID* e = (shok_EGL_CNetEventEntityID*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEvent2Entities) {
		shok_EGL_CNetEvent2Entities* e = (shok_EGL_CNetEvent2Entities*)ev;
		lua_pushstring(L, "ActorId");
		lua_pushnumber(L, e->ActorId);
		lua_rawset(L, -3);
		lua_pushstring(L, "TargetId");
		lua_pushnumber(L, e->TargetId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityAndPos) {
		shok_EGL_CNetEventEntityAndPos* e = (shok_EGL_CNetEventEntityAndPos*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Position");
		luaext_pushPos(L, e->Position);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityAndPosArray) {
		shok_EGL_CNetEventEntityAndPosArray* e = (shok_EGL_CNetEventEntityAndPosArray*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Positions");
		lua_newtable(L);
		int i = 1;
		for (shok_position p : e->Positions) {
			luaext_pushPos(L, p);
			lua_rawseti(L, -2, i);
			i++;
		}
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventExtractResource) {
		shok_GGL_CNetEventExtractResource* e = (shok_GGL_CNetEventExtractResource*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "ResourceType");
		lua_pushnumber(L, e->ResourceType);
		lua_rawset(L, -3);
		lua_pushstring(L, "Position");
		luaext_pushPos(L, e->Position);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventTransaction) {
		shok_GGL_CNetEventTransaction* e = (shok_GGL_CNetEventTransaction*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "BuyType");
		lua_pushnumber(L, e->BuyType);
		lua_rawset(L, -3);
		lua_pushstring(L, "BuyAmount");
		lua_pushnumber(L, e->BuyAmount);
		lua_rawset(L, -3);
		lua_pushstring(L, "SellType");
		lua_pushnumber(L, e->SellType);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventCannonCreator) {
		shok_GGL_CNetEventCannonCreator* e = (shok_GGL_CNetEventCannonCreator*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
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
	else if (ev->vtable == shok_vtp_GGL_CNetEventEntityIDAndUpgradeCategory) {
		shok_GGL_CNetEventEntityIDAndUpgradeCategory* e = (shok_GGL_CNetEventEntityIDAndUpgradeCategory*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "UpgradeCategory");
		lua_pushnumber(L, e->UpgradeCategory);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityIDAndInteger) {
		shok_EGL_CNetEventEntityIDAndInteger* e = (shok_EGL_CNetEventEntityIDAndInteger*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Data");
		lua_pushnumber(L, e->Data);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventTechnologyAndEntityID) {
		shok_GGL_CNetEventTechnologyAndEntityID* e = (shok_GGL_CNetEventTechnologyAndEntityID*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Technology");
		lua_pushnumber(L, e->Technology);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventPlayerID) {
		shok_EGL_CNetEventPlayerID* e = (shok_EGL_CNetEventPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventBuildingCreator) {
		shok_GGL_CNetEventBuildingCreator* e = (shok_GGL_CNetEventBuildingCreator*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "UpgradeCategory");
		lua_pushnumber(L, e->UpgradeCategory);
		lua_rawset(L, -3);
		lua_pushstring(L, "Position");
		luaext_pushPosRot(L, e->Position);
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
	else if (ev->vtable == shok_vtp_EGL_CNetEventIntegerAndPlayerID) {
		shok_EGL_CNetEventIntegerAndPlayerID* e = (shok_EGL_CNetEventIntegerAndPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Data");
		lua_pushnumber(L, e->Data);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventPlayerIDAndInteger) {
		shok_EGL_CNetEventPlayerIDAndInteger* e = (shok_EGL_CNetEventPlayerIDAndInteger*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Data");
		lua_pushnumber(L, e->Data);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityIDAndPlayerID) {
		shok_EGL_CNetEventEntityIDAndPlayerID* e = (shok_EGL_CNetEventEntityIDAndPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityIDAndPlayerIDAndEntityType) {
		shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType* e = (shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "EntityType");
		lua_pushnumber(L, e->EntityType);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventEntityIDPlayerIDAndInteger) {
		shok_GGL_CNetEventEntityIDPlayerIDAndInteger* e = (shok_GGL_CNetEventEntityIDPlayerIDAndInteger*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Data");
		lua_pushnumber(L, e->Data);
		lua_rawset(L, -3);
	}
}
bool l_netReadEvent(lua_State* L, shok_BB_CEvent* ev) {
	bool allRead = true;
	if (ev->vtable == shok_vtp_EGL_CNetEventEntityID) {
		shok_EGL_CNetEventEntityID* e = (shok_EGL_CNetEventEntityID*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEvent2Entities) {
		shok_EGL_CNetEvent2Entities* e = (shok_EGL_CNetEvent2Entities*)ev;
		lua_pushstring(L, "ActorId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->ActorId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "TargetId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->TargetId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityAndPos) {
		shok_EGL_CNetEventEntityAndPos* e = (shok_EGL_CNetEventEntityAndPos*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
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
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityAndPosArray) {
		shok_EGL_CNetEventEntityAndPosArray* e = (shok_EGL_CNetEventEntityAndPosArray*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Positions");
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			int i = 1;
			shok_saveVector<shok_position>(&e->Positions, [L, &i](std::vector<shok_position, shok_allocator<shok_position>>& v) {
				shok_position p = shok_position();
				v.clear();
				while (true) {
					lua_rawgeti(L, -1, i);
					if (!lua_istable(L, -1)) {
						lua_pop(L, 1);
						break;
					}
					luaext_checkPos(L, p, -1);
					lua_pop(L, 1);
					v.push_back(p);
					i++;
				}
				});
		}
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventExtractResource) {
		shok_GGL_CNetEventExtractResource* e = (shok_GGL_CNetEventExtractResource*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "ResourceType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->ResourceType = (int)lua_tonumber(L, -1);
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
	else if (ev->vtable == shok_vtp_GGL_CNetEventTransaction) {
		shok_GGL_CNetEventTransaction* e = (shok_GGL_CNetEventTransaction*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "BuyType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->BuyType = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "BuyAmount");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->BuyAmount = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "SellType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->SellType = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventCannonCreator) {
		shok_GGL_CNetEventCannonCreator* e = (shok_GGL_CNetEventCannonCreator*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "BottomType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->BottomType = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "TopType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->TopType = (int)lua_tonumber(L, -1);
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
	else if (ev->vtable == shok_vtp_GGL_CNetEventEntityIDAndUpgradeCategory) {
		shok_GGL_CNetEventEntityIDAndUpgradeCategory* e = (shok_GGL_CNetEventEntityIDAndUpgradeCategory*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "UpgradeCategory");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->UpgradeCategory = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityIDAndInteger) {
		shok_EGL_CNetEventEntityIDAndInteger* e = (shok_EGL_CNetEventEntityIDAndInteger*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Data");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->Data = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventTechnologyAndEntityID) {
		shok_GGL_CNetEventTechnologyAndEntityID* e = (shok_GGL_CNetEventTechnologyAndEntityID*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Technology");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->Technology = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventPlayerID) {
		shok_EGL_CNetEventPlayerID* e = (shok_EGL_CNetEventPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventBuildingCreator) {
		shok_GGL_CNetEventBuildingCreator* e = (shok_GGL_CNetEventBuildingCreator*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "UpgradeCategory");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->UpgradeCategory = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Position");
		lua_rawget(L, -2);
		if (lua_istable(L, -1))
			luaext_checkPosRot(L, e->Position, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Serfs");
		lua_rawget(L, -2);
		if (lua_istable(L, -1)) {
			int i = 1;
			shok_saveVector<int>(&e->Serfs, [L, &i](std::vector<int, shok_allocator<int>>& v) {
				v.clear();
				while (true) {
					lua_rawgeti(L, -1, i);
					if (!lua_isnumber(L, -1)) {
						lua_pop(L, 1);
						break;
					}
					v.push_back((int)lua_tonumber(L, -1));
					lua_pop(L, 1);
					i++;
				}
				});
		}
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventIntegerAndPlayerID) {
		shok_EGL_CNetEventIntegerAndPlayerID* e = (shok_EGL_CNetEventIntegerAndPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Data");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->Data = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventPlayerIDAndInteger) {
		shok_EGL_CNetEventPlayerIDAndInteger* e = (shok_EGL_CNetEventPlayerIDAndInteger*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Data");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->Data = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityIDAndPlayerID) {
		shok_EGL_CNetEventEntityIDAndPlayerID* e = (shok_EGL_CNetEventEntityIDAndPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_EGL_CNetEventEntityIDAndPlayerIDAndEntityType) {
		shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType* e = (shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "EntityType");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityType = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_vtp_GGL_CNetEventEntityIDPlayerIDAndInteger) {
		shok_GGL_CNetEventEntityIDPlayerIDAndInteger* e = (shok_GGL_CNetEventEntityIDPlayerIDAndInteger*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
		lua_pushstring(L, "Data");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->Data = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	return allRead;
}


int l_netEventCbRef = LUA_NOREF;
bool l_netEventPostCallback(shok_BB_CEvent* ev) {
	int id = ev->EventTypeId;
	lua_State* L = *shok_luastate_game;
	int top = lua_gettop(L);
	luaL_checkstack(L, 10, "");
	lua_rawgeti(L, LUA_REGISTRYINDEX, l_netEventCbRef);
	bool r = false;
	if (lua_isfunction(L, -1)) {
		lua_pushnumber(L, id);
		l_netPushEvent(L, ev);
		lua_pcall(L, 2, 1, 0);
		if (lua_isboolean(L, -1))
			r = lua_toboolean(L, -1);
		else if (lua_istable(L, -1))
			l_netReadEvent(L, ev);
	}
	lua_settop(L, top);
	return r;
}

int l_netEventSetHook(lua_State* L) {
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	luaL_unref(L, LUA_REGISTRYINDEX, l_netEventCbRef);
	l_netEventCbRef = luaL_ref(L, LUA_REGISTRYINDEX);
	PostEventCallback = &l_netEventPostCallback;
	shok_GetGuiManager()->HackPostEvent();
	return 0;
}
int l_netEventUnSetHook(lua_State* L) {
	luaL_unref(L, LUA_REGISTRYINDEX, l_netEventCbRef);
	PostEventCallback = nullptr;
	return 0;
}


int l_playerGetKillStatistics(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	lua_pushnumber(L, p->NumberOfSettlersKilled);
	lua_pushnumber(L, p->NumberOfSettlersLost);
	lua_pushnumber(L, p->NumberOfBuildingsKilled);
	lua_pushnumber(L, p->NumberOfBuildingsLost);
	return 4;
}

int l_logicCanPLaceBuildingAt(lua_State* L) {
	int ty = luaL_checkint(L, 1);
	shok_GGlue_CGlueEntityProps* ety = (*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(ty);
	luaext_assertPointer(L, ety, "no entitytype");
	luaext_assert(L, ety->IsBuildingType(), "not a building");
	int pl = luaL_checkint(L, 2);
	shok_position p;
	luaext_checkPos(L, p, 3);
	p.FloorToBuildingPlacement();
	float r = luaL_checkfloat(L, 4);
	int bon = luaL_checkint(L, 5);
	lua_pushboolean(L, shok_canPlaceBuilding(ty, pl, &p, r, bon));
	return 1;
}

int l_logicPlayerActivateAlarm(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	luaext_assert(L, !p->AlarmActive, "alarm active");
	luaext_assert(L, p->AlarmRechargeTime <= 0, "alarm cooldown");
	(*shok_GGL_CGLGameLogicObj)->EnableAlarmForPlayer(i);
	return 0;
}
int l_logicPlayerDeactivateAlarm(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	luaext_assert(L, p->AlarmActive, "alarm not active");
	(*shok_GGL_CGLGameLogicObj)->DisableAlarmForPlayer(i);
	return 0;
}

int l_logicPlayerUpgradeSettlerCategory(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int ucat = luaL_checkint(L, 2);
	shok_GGL_CSettlerUpgradeManager_UCatEntry* u = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i)->SettlerUpgradeManager->UpgradeCategories.GetFirstMatch([ucat](shok_GGL_CSettlerUpgradeManager_UCatEntry* u) {return u->UCat == ucat; });
	luaext_assertPointer(L, u, "invalid ucat");
	(*shok_GGL_CGLGameLogicObj)->UpgradeSettlerCategory(i, ucat);
	return 0;
}

int l_logicPlayerSetTaxLevel(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int tl = luaL_checkint(L, 2);
	luaext_assert(L, tl >= 0 && tl < 5, "invalid taxlevel");
	(*shok_GGL_CGLGameLogicObj)->GetPlayer(i)->TaxLevel = tl;
	return 0;
}

int l_logicPlayerActivateWeatherMachine(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int w = luaL_checkint(L, 2);
	luaext_assert(L, w > 0 && w < 4, "invalid weathertype");
	luaext_assert(L, !(*shok_GGL_CGLGameLogicObj)->WeatherHandler->WeatherChangeActive, "weather currently changing");
	luaext_assert(L, (*shok_GGL_CGLGameLogicObj)->GetPlayer(i)->CurrentResources.WeatherEnergy >= (*shok_GGL_CLogicPropertiesObj)->EnergyRequiredForWeatherChange, "not enough weather energy");
	(*shok_GGL_CGLGameLogicObj)->PlayerActivateWeathermachine(i, w);
	return 0;
}

int l_logicPlayerBlessSettlers(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int b = luaL_checkint(L, 2);
	float faithneeded = -1;
	for (shok_GGL_CLogicProperties_SBlessCategory& bcat : (*shok_GGL_CLogicPropertiesObj)->BlessCategories) {
		if (bcat.Name == b) {
			faithneeded = bcat.RequiredFaith;
			break;
		}
	}
	luaext_assert(L, faithneeded >= 0, "invalid blesscategory");
	luaext_assert(L, (*shok_GGL_CGLGameLogicObj)->GetPlayer(i)->CurrentResources.Faith >= faithneeded, "not enough faith");
	(*shok_GGL_CGLGameLogicObj)->PlayerBlessSettlers(i, b);
	return 0;
}

void l_logic_cleanup(lua_State* L) {
	l_netEventUnSetHook(L);
}

void l_logic_init(lua_State* L)
{
	luaext_registerFunc(L, "GetDamageFactor", l_logicGetDamageModifier);
	luaext_registerFunc(L, "SetDamageFactor", l_logicSetDamageModifier);
	luaext_registerFunc(L, "ReloadCutscene", &l_logicReloadCutscene);
	luaext_registerFunc(L, "GetAnimIdFromName", &l_logicGetAnimIdFromName);
	luaext_registerFunc(L, "PlayerGetPaydayStartetTick", &l_playerGetPaydayStatus);
	luaext_registerFunc(L, "PlayerSetPaydayStartetTick", &l_playerSetPaydayStatus);
	luaext_registerFunc(L, "PlayerGetKillStatistics", &l_playerGetKillStatistics);
	luaext_registerFunc(L, "CanPlaceBuildingAt", &l_logicCanPLaceBuildingAt);
	luaext_registerFunc(L, "PlayerActivateAlarm", &l_logicPlayerActivateAlarm);
	luaext_registerFunc(L, "PlayerDeactivateAlarm", &l_logicPlayerDeactivateAlarm);
	luaext_registerFunc(L, "PlayerUpgradeSettlerCategory", &l_logicPlayerUpgradeSettlerCategory);
	luaext_registerFunc(L, "PlayerSetTaxLevel", &l_logicPlayerSetTaxLevel);
	luaext_registerFunc(L, "PlayerActivateWeatherMachine", &l_logicPlayerActivateWeatherMachine);
	luaext_registerFunc(L, "PlayerBlessSettlers", &l_logicPlayerBlessSettlers);


	lua_pushstring(L, "UICommands");
	lua_newtable(L);
	luaext_registerFunc(L, "SetCallback", &l_netEventSetHook);
	luaext_registerFunc(L, "UnSetCallback", &l_netEventUnSetHook);
	lua_rawset(L, -3);
}

// CppLogic.Logic.UICommands.SetCallback(function(id, ev) LuaDebugger.Log(id) LuaDebugger.Log(ev) return false end)

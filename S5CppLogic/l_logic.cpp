#include "pch.h"
#include "l_logic.h"
#include "luaext.h"
#include <assert.h>
#include <filesystem>

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
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	if (st < 0) {
		p->PlayerAttractionHandler->PaydayActive = 0;
	}
	else {
		p->PlayerAttractionHandler->PaydayActive = 1;
		p->PlayerAttractionHandler->PaydayFirstOccuraceGameTurn = st;
	}
	return 0;
}


void l_netPushEvent(lua_State* L, shok_BB_CEvent* ev) {
	lua_newtable(L);
	if (ev->vtable == shok_EGL_CNetEventEntityID::vtp) {
		shok_EGL_CNetEventEntityID* e = (shok_EGL_CNetEventEntityID*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEvent2Entities::vtp) {
		shok_EGL_CNetEvent2Entities* e = (shok_EGL_CNetEvent2Entities*)ev;
		lua_pushstring(L, "ActorId");
		lua_pushnumber(L, e->ActorId);
		lua_rawset(L, -3);
		lua_pushstring(L, "TargetId");
		lua_pushnumber(L, e->TargetId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEventEntityAndPos::vtp) {
		shok_EGL_CNetEventEntityAndPos* e = (shok_EGL_CNetEventEntityAndPos*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Position");
		luaext_pushPos(L, e->Position);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEventEntityAndPosArray::vtp) {
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
		lua_pushstring(L, "Rotation");
		lua_pushnumber(L, e->Rotation==-1.0f ? -1.0 : rad2deg((double)e->Rotation));
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_GGL_CNetEventExtractResource::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventTransaction::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventCannonCreator::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventEntityIDAndUpgradeCategory::vtp) {
		shok_GGL_CNetEventEntityIDAndUpgradeCategory* e = (shok_GGL_CNetEventEntityIDAndUpgradeCategory*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "UpgradeCategory");
		lua_pushnumber(L, e->UpgradeCategory);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEventEntityIDAndInteger::vtp) {
		shok_EGL_CNetEventEntityIDAndInteger* e = (shok_EGL_CNetEventEntityIDAndInteger*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Data");
		lua_pushnumber(L, e->Data);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_GGL_CNetEventTechnologyAndEntityID::vtp) {
		shok_GGL_CNetEventTechnologyAndEntityID* e = (shok_GGL_CNetEventTechnologyAndEntityID*)ev;
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Technology");
		lua_pushnumber(L, e->Technology);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEventPlayerID::vtp) {
		shok_EGL_CNetEventPlayerID* e = (shok_EGL_CNetEventPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_GGL_CNetEventBuildingCreator::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventIntegerAndPlayerID::vtp) {
		shok_EGL_CNetEventIntegerAndPlayerID* e = (shok_EGL_CNetEventIntegerAndPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Data");
		lua_pushnumber(L, e->Data);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEventPlayerIDAndInteger::vtp) {
		shok_EGL_CNetEventPlayerIDAndInteger* e = (shok_EGL_CNetEventPlayerIDAndInteger*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "Data");
		lua_pushnumber(L, e->Data);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEventEntityIDAndPlayerID::vtp) {
		shok_EGL_CNetEventEntityIDAndPlayerID* e = (shok_EGL_CNetEventEntityIDAndPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_pushnumber(L, e->PlayerId);
		lua_rawset(L, -3);
		lua_pushstring(L, "EntityId");
		lua_pushnumber(L, e->EntityId);
		lua_rawset(L, -3);
	}
	else if (ev->vtable == shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventEntityIDPlayerIDAndInteger::vtp) {
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
	if (ev->vtable == shok_EGL_CNetEventEntityID::vtp) {
		shok_EGL_CNetEventEntityID* e = (shok_EGL_CNetEventEntityID*)ev;
		lua_pushstring(L, "EntityId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->EntityId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_EGL_CNetEvent2Entities::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventEntityAndPos::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventEntityAndPosArray::vtp) {
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
		lua_pushstring(L, "Rotation");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1)) {
			e->Rotation = (float)lua_tonumber(L, -1);
			if (e->Rotation != -1.0f) {
				e->Rotation = (float)deg2rad((double)e->Rotation);
			}
		}
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_GGL_CNetEventExtractResource::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventTransaction::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventCannonCreator::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventEntityIDAndUpgradeCategory::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventEntityIDAndInteger::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventTechnologyAndEntityID::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventPlayerID::vtp) {
		shok_EGL_CNetEventPlayerID* e = (shok_EGL_CNetEventPlayerID*)ev;
		lua_pushstring(L, "PlayerId");
		lua_rawget(L, -2);
		if (lua_isnumber(L, -1))
			e->PlayerId = (int)lua_tonumber(L, -1);
		else
			allRead = false;
		lua_pop(L, 1);
	}
	else if (ev->vtable == shok_GGL_CNetEventBuildingCreator::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventIntegerAndPlayerID::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventPlayerIDAndInteger::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventEntityIDAndPlayerID::vtp) {
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
	else if (ev->vtable == shok_EGL_CNetEventEntityIDAndPlayerIDAndEntityType::vtp) {
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
	else if (ev->vtable == shok_GGL_CNetEventEntityIDPlayerIDAndInteger::vtp) {
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


int l_netEventSetHook(lua_State* L) {
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	lua_pushlightuserdata(L, &l_netEventSetHook);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	PostEventCallback = [](shok_BB_CEvent* ev) {
		int id = ev->EventTypeId;
		lua_State* L = *shok_luastate_game;
		int top = lua_gettop(L);
		luaL_checkstack(L, 10, "");
		lua_pushlightuserdata(L, &l_netEventSetHook);
		lua_rawget(L, LUA_REGISTRYINDEX);
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
	};
	shok_GetGuiManager()->HackPostEvent();
	return 0;
}
int l_netEventUnSetHook(lua_State* L) {
	PostEventCallback = nullptr;
	return 0;
}


int l_playerGetKillStatistics(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	lua_pushnumber(L, p->Statistics.NumberOfUnitsKilled);
	lua_pushnumber(L, p->Statistics.NumberOfUnitsDied);
	lua_pushnumber(L, p->Statistics.NumberOfBuildingsDestroyed);
	lua_pushnumber(L, p->Statistics.NumberOfBuildingsLost);
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
	luaext_assert(L, !p->WorkerAlarmMode, "alarm active");
	luaext_assert(L, p->AlarmRechargeTime <= 0, "alarm cooldown");
	(*shok_GGL_CGLGameLogicObj)->EnableAlarmForPlayer(i);
	return 0;
}
int l_logicPlayerDeactivateAlarm(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	shok_GGL_CPlayerStatus* p = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i);
	luaext_assert(L, p->WorkerAlarmMode, "alarm not active");
	(*shok_GGL_CGLGameLogicObj)->DisableAlarmForPlayer(i);
	return 0;
}

int l_logicPlayerUpgradeSettlerCategory(lua_State* L) {
	int i = luaL_checkint(L, 1);
	luaext_assert(L, i > 0 && i < 9, "invalid player");
	int ucat = luaL_checkint(L, 2);
	shok_GGL_CUpgradeManager_UCatEntry* u = (*shok_GGL_CGLGameLogicObj)->GetPlayer(i)->SettlerUpgradeManager->UpgradeCategories.GetFirstMatch([ucat](shok_GGL_CUpgradeManager_UCatEntry* u) {return u->UCat == ucat; });
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

int l_logicLandscapeGetSector(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*shok_EGL_CGLEGameLogicObject)->Landscape->GetSector(&p));
	return 1;
}

int l_logicLandscapeGetNearesUnblockedPosInSector(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	int s = luaL_checkint(L, 2);
	float r = luaL_checkfloat(L, 3);
	shok_position po;
	if ((*shok_EGL_CGLEGameLogicObject)->Landscape->GetNearestPositionInSector(&p, r, s, &po))
		luaext_pushPos(L, po);
	else
		lua_pushnil(L);
	return 1;
}

int l_logicHookHurtEntity(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	HookHurtEntity();
	return 0;
}

int l_logicHurtEntityGetDamage(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	luaext_assertPointer(L, HurtEntityDamagePointer, "not in trigger");
	lua_pushnumber(L, *HurtEntityDamagePointer);
	return 1;
}
int l_logicHurtEntitySetDamage(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use CEntity instead");
	luaext_assertPointer(L, HurtEntityDamagePointer, "not in trigger");
	*HurtEntityDamagePointer = luaL_checkint(L, 1);
	return 0;
}

int l_logicGetLoadOrder(lua_State* L) {
	lua_newtable(L);
	int r = 1;
	for (shok_BB_IFileSystem* a : (*shok_BB_CFileSystemMgrObj)->LoadOrder) {
		if (a->vtable == shok_BB_CDirectoryFileSystem::vtp) {
			lua_pushstring(L, ((shok_BB_CDirectoryFileSystem*)a)->Path);
			lua_rawseti(L, -2, r);
			r++;
		}
		else if (a->vtable == shok_BB_CBBArchiveFile::vtp) {
			lua_pushstring(L, ((shok_BB_CBBArchiveFile*)a)->Path);
			lua_rawseti(L, -2, r);
			r++;
		}
	}
	return 1;
}

int l_logicAddArchive(lua_State* L) {
	const char* s = luaL_checkstring(L, 1);
	luaext_assert(L, std::filesystem::exists(s), "file doesnt exist");
	(*shok_BB_CFileSystemMgrObj)->AddArchive(s);
	return 0;
}

int l_logicRemoveTop(lua_State* L) {
	if ((*shok_BB_CFileSystemMgrObj)->LoadOrder.size() <= 0)
		return 0;
	shok_BB_IFileSystem* a = (*shok_BB_CFileSystemMgrObj)->LoadOrder[0];
	luaext_assert(L, a->vtable == shok_BB_CBBArchiveFile::vtp, "may only remove archives");
	luaext_assert(L, str_ends_with(((shok_BB_CBBArchiveFile*)a)->Path, ".s5x"), "may only remove maps");
	(*shok_BB_CFileSystemMgrObj)->RemoveTopArchive();
	return 0;
}

int l_logicAddFolder(lua_State* L) { // works, but adds to the bottom
	const char* s = luaL_checkstring(L, 1);
	(*shok_BB_CFileSystemMgrObj)->AddFolder(s);
	return 0;
}

int l_logicEnableMaxHpTechMod(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "not supportet with SCELoader");
	EnableMaxHealthTechBoni();
	return 0;
}

int l_logicLandscapeGetTerrainType(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*shok_EGL_CGLEGameLogicObject)->Landscape->LowRes->GetTerrainTypeAt(p));
	return 1;
}
int l_logicLandscapeGetWaterType(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*shok_EGL_CGLEGameLogicObject)->Landscape->LowRes->GetWaterTypeAt(p));
	return 1;
}
int l_logicLandscapeGetWaterHeight(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*shok_EGL_CGLEGameLogicObject)->Landscape->LowRes->GetWaterHeightAt(p));
	return 1;
}
int l_logicLandscapeGetTerrainHeight(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*shok_EGL_CGLEGameLogicObject)->Landscape->HiRes->GetTerrainHeight(p));
	return 1;
}
int l_logicLandscapeGetTerrainVertexColor(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*shok_EGL_CGLEGameLogicObject)->Landscape->VertexColors->GetTerrainVertexColor(p));
	return 1;
}
int l_logicLandscapeGetBlocking(lua_State* L) {
	shok_position p;
	luaext_checkPos(L, p, 1);
	lua_pushnumber(L, (*shok_ED_CGlobalsLogicExObj)->GetBlocking(p));
	return 1;
}

int l_logicGetColor(lua_State* L) { // ind -> r,g,b,a
	int i = luaL_checkint(L, 1);
	int c = (*shok_ED_CGlobalsBaseExObj)->PlayerColors->GetColorByIndex(i);
	lua_pushnumber(L, c & 0xFF);
	lua_pushnumber(L, c >> 8 & 0xFF);
	lua_pushnumber(L, c >> 16 & 0xFF);
	lua_pushnumber(L, c >> 24 & 0xFF);
	return 4;
}
int l_logicSetColor(lua_State* L) {
	int i = luaL_checkint(L, 1);
	int r = luaL_checkint(L, 2), g = luaL_checkint(L, 3), b = luaL_checkint(L, 4);
	int a = luaL_optint(L, 5, 255);
	int c = r & 0xFF;
	c |= (g & 0xFF) << 8;
	c |= (b & 0xFF) << 16;
	c |= (a & 0xFF) << 24;
	(*shok_ED_CGlobalsBaseExObj)->PlayerColors->SetColorByIndex(i, c);
	(*shok_ED_CGlobalsBaseExObj)->PlayerColors->RefreshPlayerColors();
	return 0;
}

int l_logicEnablePlayerPaydayCallback(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "use global GameCallback_PaydayPayed instead");
	if (!lua_isfunction(L, 1))
		luaL_error(L, "no func");
	shok_GGL_CPlayerAttractionHandler_HookCheckPayday();
	lua_pushlightuserdata(L, &l_logicEnablePlayerPaydayCallback);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);
	shok_GGL_CPlayerAttractionHandler_OnCheckPayDay = [](shok_GGL_CPlayerAttractionHandler* th) {
		lua_State* L2 = *shok_luastate_game;
		int t = lua_gettop(L2);
		lua_pushlightuserdata(L2, &l_logicEnablePlayerPaydayCallback);
		lua_rawget(L2, LUA_REGISTRYINDEX);
		lua_pushnumber(L2, th->PlayerID);
		lua_pushnumber(L2, th->GetWorkerPaydayIncome());
		lua_pcall(L2, 2, 0, 0);
		lua_settop(L2, t);
	};
	return 0;
}

void l_logic_cleanup(lua_State* L) {
	l_netEventUnSetHook(L);
	shok_GGL_CPlayerAttractionHandler_OnCheckPayDay = nullptr;
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
	luaext_registerFunc(L, "LandscapeGetSector", &l_logicLandscapeGetSector);
	luaext_registerFunc(L, "LandscapeGetNearestUnblockedPosInSector", &l_logicLandscapeGetNearesUnblockedPosInSector);
	luaext_registerFunc(L, "EnableAllHurtEntityTrigger", &l_logicHookHurtEntity);
	luaext_registerFunc(L, "HurtEntityGetDamage", &l_logicHurtEntityGetDamage);
	luaext_registerFunc(L, "HurtEntitySetDamage", &l_logicHurtEntitySetDamage);
	luaext_registerFunc(L, "GetLoadOrder", &l_logicGetLoadOrder);
	luaext_registerFunc(L, "AddArchive", &l_logicAddArchive);
	luaext_registerFunc(L, "RemoveTopArchive", &l_logicRemoveTop);
	//luaext_registerFunc(L, "AddFolder", &l_logicAddFolder);
	luaext_registerFunc(L, "EnableMaxHPTechMod", &l_logicEnableMaxHpTechMod);
	luaext_registerFunc(L, "LandscapeGetTerrainType", &l_logicLandscapeGetTerrainType);
	luaext_registerFunc(L, "LandscapeGetWaterType", &l_logicLandscapeGetWaterType);
	luaext_registerFunc(L, "LandscapeGetWaterHeight", &l_logicLandscapeGetWaterHeight);
	luaext_registerFunc(L, "LandscapeGetTerrainHeight", &l_logicLandscapeGetTerrainHeight);
	luaext_registerFunc(L, "LandscapeGetTerrainVertexColor", &l_logicLandscapeGetTerrainVertexColor);
	luaext_registerFunc(L, "LandscapeGetBlocking", &l_logicLandscapeGetBlocking);
	luaext_registerFunc(L, "GetColorByColorIndex", &l_logicGetColor);
	luaext_registerFunc(L, "SetColorByColorIndex", &l_logicSetColor);
	luaext_registerFunc(L, "SetPaydayCallback", &l_logicEnablePlayerPaydayCallback);


	lua_pushstring(L, "UICommands");
	lua_newtable(L);
	luaext_registerFunc(L, "SetCallback", &l_netEventSetHook);
	luaext_registerFunc(L, "UnSetCallback", &l_netEventUnSetHook);
	lua_rawset(L, -3);
}

// CppLogic.Logic.UICommands.SetCallback(function(id, ev) LuaDebugger.Log(id) LuaDebugger.Log(ev) return false end)
// CppLogic.Logic.AddArchive("extra2\\shr\\maps\\user\\test.s5x")
// CppLogic.Logic.AddArchive("test.bba")
// CppLogic.Logic.RemoveTopArchive()
// CppLogic.Logic.EnableMaxHPTechMod() ResearchTechnology(Technologies.T_Fletching)
// CppLogic.Logic.GetColorByColorIndex(1)
// CppLogic.Logic.SetColorByColorIndex(1, 255, 255, 255, 255)
// CppLogic.Logic.SetPaydayCallback(GameCallback_PaydayPayed)

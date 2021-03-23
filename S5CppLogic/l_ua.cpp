#include "pch.h"
#include "l_ua.h"
#include "luaext.h"

UnlimitedArmy::~UnlimitedArmy()
{
}
UnlimitedArmy::UnlimitedArmy(int p)
{
	Player = p;
	LastPos.X = -1;
	LastPos.Y = -1;
}
void UnlimitedArmy::CalculatePos()
{
	float x = 0, y = 0;
	int num = 0;
	for (int id : Entities) {
		shok_EGL_CGLEEntity* e = shok_eid2obj(id);
		if (e != nullptr) {
			x += e->Position.X;
			y += e->Position.Y;
			num++;
		}
	}
	if (num == 0) {
		LastPos.X = -1;
		LastPos.Y = -1;
	}
	else {
		LastPos.X = x / num;
		LastPos.Y = y / num;
	}
}
void UnlimitedArmy::CleanDead()
{
	auto e = std::remove_if(Entities.begin(), Entities.end(), shok_entityIsDead);
	Entities.erase(e, Entities.end());
}
void UnlimitedArmy::Tick()
{
	CleanDead();
}

const char* udname = "UnlimitedArmy";

int l_uam_gc(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->~UnlimitedArmy();
	return 0;
}

int l_uam_AddEntity(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->Entities.push_back(luaext_checkEntity(L, 2)->EntityId);
	return 0;
}

int l_ual_IterateNext(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	int i = luaL_checkint(L, 2);
	i++;
	if (i >= (int)a->Entities.size()) {
		lua_pushnil(L);
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, i);
		lua_pushnumber(L, a->Entities[i]);
	}
	return 2;
}

int l_uam_Iterate(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	lua_pushcfunction(L, &l_ual_IterateNext);
	lua_pushvalue(L, 1);
	lua_pushnumber(L, -1);
	return 3;
}

int l_uam_GetPos(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	int tick = luaL_optint(L, 2, -1);
	if (tick != a->PosLastUpdatedTick) {
		if (tick >= 0)
			a->PosLastUpdatedTick = tick;
		a->CalculatePos();
	}
	luaext_pushPos(L, a->LastPos);
	return 1;
}

int l_uam_Tick(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	a->Tick();
	return 0;
}

int l_uam_IdChanged(lua_State* L) {
	UnlimitedArmy* a = (UnlimitedArmy*)luaL_checkudata(L, 1, udname);
	int old = luaL_checkint(L, 2);
	int ne = luaL_checkint(L, 3);
	for (int i = 0; i < (int)a->Entities.size(); i++) {
		if (a->Entities[i] == old)
			a->Entities[i] = ne;
	}
	return 0;
}

int l_uaNew(lua_State* L) {
	int pl = luaL_checkint(L, 1);
	UnlimitedArmy* a = (UnlimitedArmy*)lua_newuserdata(L, sizeof(UnlimitedArmy));
	new (a) UnlimitedArmy(pl);
	luaL_getmetatable(L, udname);
	lua_setmetatable(L, -2);
	return 1;
}

void l_ua_init(lua_State* L)
{
	luaext_registerFunc(L, "New", &l_uaNew);

	luaL_newmetatable(L, udname);
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, &l_uam_gc);
	lua_settable(L, -3);

	lua_pushstring(L, "__index");
	lua_newtable(L);
	luaext_registerFunc(L, "AddLeader", &l_uam_AddEntity);
	luaext_registerFunc(L, "GetPos", &l_uam_GetPos);
	luaext_registerFunc(L, "Tick", &l_uam_Tick);
	luaext_registerFunc(L, "Iterate", &l_uam_Iterate);
	luaext_registerFunc(L, "OnIdChanged", &l_uam_IdChanged);
	lua_settable(L, -3);
	lua_pop(L, 1);
}

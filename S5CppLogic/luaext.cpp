#include "pch.h"
#include "luaext.h"
#include "luaimport.h"
#include "s5data.h"

void luaext_registerFunc(lua_State* L, const char* name, lua_CFunction func)
{
	lua_pushstring(L, name);
	lua_pushcfunction(L, func);
	lua_settable(L, -3);
}

void* luaext_checkudata(lua_State* L, int ind)
{
	if (!lua_isuserdata(L, ind))
		luaL_error(L, "no userdata at %d", ind);
	return lua_touserdata(L, ind);
}

EGL::CGLEEntity* luaext_checkEntity(lua_State* L, int ind)
{
	EGL::CGLEEntity* d = luaext_optEntity(L, ind);
	if (d == nullptr)
		luaL_error(L, "no entity at argument %d", ind);
	return d;
}

EGL::CGLEEntity* luaext_optEntity(lua_State* L, int ind)
{
	int id = 0;
	if (lua_type(L, ind) == LUA_TSTRING) {
		id = EGL::CGLEEntity::GetEntityIDByScriptName(lua_tostring(L, ind));
	}
	else if (lua_type(L, ind) == LUA_TNUMBER) {
		id = (int)lua_tonumber(L, ind);
	}
	if (id == 0)
		return nullptr;
	return EGL::CGLEEntity::GetEntityByID(id);
}

int luaext_optEntityId(lua_State* L, int ind) {
	EGL::CGLEEntity* e = luaext_optEntity(L, ind);
	return e == nullptr ? 0 : e->EntityId;
}

int luaext_checkEntityId(lua_State* L, int ind) {
	EGL::CGLEEntity* e = luaext_checkEntity(L, ind);
	return e == nullptr ? 0 : e->EntityId;
}

GGL::CSettler* luaext_optSettler(lua_State* L, int ind) {
	EGL::CGLEEntity* d = luaext_optEntity(L, ind);
	if (d == nullptr)
		return nullptr;
	return shok_DynamicCast<EGL::CGLEEntity, GGL::CSettler>(d);
}

GGL::CSettler* luaext_checkSettler(lua_State* L, int ind) {
	GGL::CSettler* d = luaext_optSettler(L, ind);
	if (d == nullptr)
		luaL_error(L, "no settler at argument %d", ind);
	return d;
}

GGL::CBuilding* luaext_optBuilding(lua_State* L, int ind) {
	EGL::CGLEEntity* d = luaext_optEntity(L, ind);
	if (d == nullptr)
		return nullptr;
	return shok_DynamicCast<EGL::CGLEEntity, GGL::CBuilding>(d);
}

GGL::CBuilding* luaext_checkBulding(lua_State* L, int ind) {
	GGL::CBuilding* d = luaext_optBuilding(L, ind);
	if (d == nullptr)
		luaL_error(L, "no building at argument %d", ind);
	return d;
}

GGL::CResourceDoodad* luaext_optResourceDoodad(lua_State* L, int ind) {
	EGL::CGLEEntity* d = luaext_optEntity(L, ind);
	if (d == nullptr)
		return nullptr;
	return shok_DynamicCast<EGL::CGLEEntity, GGL::CResourceDoodad>(d);
}

GGL::CResourceDoodad* luaext_checkResourceDoodad(lua_State* L, int ind) {
	GGL::CResourceDoodad* d = luaext_optResourceDoodad(L, ind);
	if (d == nullptr)
		luaL_error(L, "no resource entity at argument %d", ind);
	return d;
}

int luaext_assertPointer(lua_State* L, void* p, const char* msg) {
	if (p == nullptr)
		luaL_error(L, msg);
	return 0;
}

int luaext_assert(lua_State* L, bool b, const char* msg) {
	if (!b)
		luaL_error(L, msg);
	return 0;
}

GGlue::CGlueEntityProps* luaext_optEntityType(lua_State* L, int i) {
	if (!lua_isnumber(L, i))
		return nullptr;
	int t = luaL_checkint(L, i);
	return (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(t);
}

GGlue::CGlueEntityProps* luaext_checkEntityType(lua_State* L, int i) {
	GGlue::CGlueEntityProps* t = luaext_optEntityType(L, i);
	if (t == nullptr)
		luaL_error(L, "no entitytype at %d", i);
	return t;
}

void luaext_pushPos(lua_State* L, const shok::Position& p) {
	lua_newtable(L);
	lua_pushstring(L, "X");
	lua_pushnumber(L, p.X);
	lua_rawset(L, -3);
	lua_pushstring(L, "Y");
	lua_pushnumber(L, p.Y);
	lua_rawset(L, -3);
}

void luaext_pushPosRot(lua_State* L, const shok::PositionRot& p, bool rad) {
	luaext_pushPos(L, p);
	lua_pushstring(L, "r");
	if (rad)
		lua_pushnumber(L, rad2deg(p.r));
	else
		lua_pushnumber(L, p.r);
	lua_rawset(L, -3);
}

void luaext_checkPos(lua_State* L, shok::Position& p, int i) {
	i = lua_abs_index(L, i);
	lua_pushstring(L, "X");
	lua_gettable(L, i);
	float x = luaL_checkfloat(L, -1);
	lua_pushstring(L, "Y");
	lua_gettable(L, i);
	float y = luaL_checkfloat(L, -1);
	int size = *EGL::CGLEGameLogic::MapSize * 100;
	luaext_assert(L, x >= 0 && y >= 0 && x < size && y < size, "position outside of map");
	p.X = x;
	p.Y = y;
	lua_pop(L, 2);
}

void luaext_checkPosRot(lua_State* L, shok::PositionRot& p, int i, bool rad) {
	i = lua_abs_index(L, i);
	lua_pushstring(L, "X");
	lua_gettable(L, i);
	float x = luaL_checkfloat(L, -1);
	lua_pushstring(L, "Y");
	lua_gettable(L, i);
	float y = luaL_checkfloat(L, -1);
	lua_pushstring(L, "r");
	lua_gettable(L, i);
	float r = luaL_checkfloat(L, -1);
	if (rad)
		r = deg2rad(r);
	int size = *EGL::CGLEGameLogic::MapSize * 100;
	luaext_assert(L, x >= 0 && y >= 0 && x < size && y < size, "position outside of map");
	p.X = x;
	p.Y = y;
	p.r = r;
	lua_pop(L, 3);
}

void luaext_writeCostInfo(lua_State* L, const shok::CostInfo& c) {
	lua_newtable(L);
	for (int i = 1; i <= shok::ResourceType_MaxValue; i++) {
		lua_pushnumber(L, 0);
		lua_rawseti(L, -2, i);
	}
	lua_pushnumber(L, c.Clay);
	lua_rawseti(L, -2, static_cast<int>(shok::ResourceType::Clay));
	lua_pushnumber(L, c.Gold);
	lua_rawseti(L, -2, static_cast<int>(shok::ResourceType::Gold));
	lua_pushnumber(L, c.Iron);
	lua_rawseti(L, -2, static_cast<int>(shok::ResourceType::Iron));
	lua_pushnumber(L, c.Silver);
	lua_rawseti(L, -2, static_cast<int>(shok::ResourceType::Silver));
	lua_pushnumber(L, c.Stone);
	lua_rawseti(L, -2, static_cast<int>(shok::ResourceType::Stone));
	lua_pushnumber(L, c.Sulfur);
	lua_rawseti(L, -2, static_cast<int>(shok::ResourceType::Sulfur));
	lua_pushnumber(L, c.Wood);
	lua_rawseti(L, -2, static_cast<int>(shok::ResourceType::Wood));
}

void luaext_readCostInfo(lua_State* L, int index, shok::CostInfo& c, bool ignoreZeroes) {
	float i = 0;
	lua_rawgeti(L, index, static_cast<int>(shok::ResourceType::Clay));
	if (lua_isnumber(L, -1)) {
		i = luaL_checkfloat(L, -1);
		if (i != 0 || !ignoreZeroes)
			c.Clay = i;
	}
	lua_pop(L, 1);
	lua_rawgeti(L, index, static_cast<int>(shok::ResourceType::Gold));
	if (lua_isnumber(L, -1)) {
		i = luaL_checkfloat(L, -1);
		if (i != 0 || !ignoreZeroes)
			c.Gold = i;
	}
	lua_pop(L, 1);
	lua_rawgeti(L, index, static_cast<int>(shok::ResourceType::Iron));
	if (lua_isnumber(L, -1)) {
		i = luaL_checkfloat(L, -1);
		if (i != 0 || !ignoreZeroes)
			c.Iron = i;
	}
	lua_pop(L, 1);
	lua_rawgeti(L, index, static_cast<int>(shok::ResourceType::Silver));
	if (lua_isnumber(L, -1)) {
		i = luaL_checkfloat(L, -1);
		if (i != 0 || !ignoreZeroes)
			c.Silver = i;
	}
	lua_pop(L, 1);
	lua_rawgeti(L, index, static_cast<int>(shok::ResourceType::Stone));
	if (lua_isnumber(L, -1)) {
		i = luaL_checkfloat(L, -1);
		if (i != 0 || !ignoreZeroes)
			c.Stone = i;
	}
	lua_pop(L, 1);
	lua_rawgeti(L, index, static_cast<int>(shok::ResourceType::Sulfur));
	if (lua_isnumber(L, -1)) {
		i = luaL_checkfloat(L, -1);
		if (i != 0 || !ignoreZeroes)
			c.Sulfur = i;
	}
	lua_pop(L, 1);
	lua_rawgeti(L, index, static_cast<int>(shok::ResourceType::Wood));
	if (lua_isnumber(L, -1)) {
		i = luaL_checkfloat(L, -1);
		if (i != 0 || !ignoreZeroes)
			c.Wood = i;
	}
	lua_pop(L, 1);
}

shok::ResourceType luaext_getresourcetype(lua_State* L, int i) {
	int r = luaL_checkint(L, i);
	luaext_assert(L, r > 0 && r <= shok::ResourceType_MaxValue, "no resource type");
	return static_cast<shok::ResourceType>(r);
}

void luaext_assertEntityAlive(lua_State* L, int id, const char* msg)
{
	if (EGL::CGLEEntity::EntityIDIsDead(id))
		luaL_error(L, msg);
}

int str_ends_with(const char* str, const char* suffix) {

	if (str == NULL || suffix == NULL)
		return 0;

	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);

	if (suffix_len > str_len)
		return 0;

	return 0 == strncmp(str + str_len - suffix_len, suffix, suffix_len);
}

void luaext_tolower(lua_State* L) {
	lua_getglobal(L, "string");
	lua_pushstring(L, "lower");
	lua_rawget(L, -2);
	lua_remove(L, -2);
	lua_insert(L, -2);
	lua_pcall(L, 1, 1, 0);
}

bool luaext_optbool(lua_State* L, int i, bool b) {
	if (lua_isnoneornil(L, i))
		return b;
	return lua_toboolean(L, i);
}

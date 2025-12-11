#include "pch.h"
#include <luaext.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/entity/s5_entity.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/ui/s5_widget.h>
#include <shok/effect/s5_effects.h>
#include <shok/globals/s5_entityandeffectmanager.h>

luaext::EState::EState(lua::State L) : lua::State(L.GetState())
{
}
luaext::EState::EState(lua_State* L) : lua::State(L)
{
}

EGL::CGLEEntity* luaext::EState::CheckEntity(int i)
{
	EGL::CGLEEntity* d = OptEntity(i);
	if (!d)
		throw lua::LuaException{ std::format("no entity at argument {}", i) };
	return d;
}
EGL::CGLEEntity* luaext::EState::OptEntity(int i)
{
	shok::EntityId id = {};
	if (Type(i) == lua::LType::String) {
		id = EGL::CGLEEntity::GetEntityIDByScriptName(ToString(i));
	}
	else if (IsNumber(i)) {
		id = static_cast<shok::EntityId>(CheckInt(i));
	}
	if (id == shok::EntityId::Invalid)
		return nullptr;
	return EGL::CGLEEntity::GetEntityByID(id);
}

GGL::CSettler* luaext::EState::CheckSettler(int i)
{
	GGL::CSettler* s = OptSettler(i);
	if (!s)
		throw lua::LuaException{ std::format("no settler at argument {}", i) };
	return s;
}
GGL::CSettler* luaext::EState::OptSettler(int i)
{
	return dynamic_cast<GGL::CSettler*>(OptEntity(i));
}

GGL::CBuilding* luaext::EState::CheckBuilding(int i)
{
	GGL::CBuilding* b = OptBuilding(i);
	if (!b)
		throw lua::LuaException{ std::format("no building at argument {}", i) };
	return b;
}
GGL::CBuilding* luaext::EState::OptBuilding(int i)
{
	return dynamic_cast<GGL::CBuilding*>(OptEntity(i));
}

GGL::CResourceDoodad* luaext::EState::CheckResourceDoodad(int i)
{
	GGL::CResourceDoodad* d = OptResourceDoodad(i);
	if (!d)
		throw lua::LuaException{ std::format("no resource entity at argument {}", i) };
	return d;
}
GGL::CResourceDoodad* luaext::EState::OptResourceDoodad(int i)
{
	return dynamic_cast<GGL::CResourceDoodad*>(OptEntity(i));
}

EGL::CEffect* luaext::EState::OptEffect(int i)
{
	int id = OptInt(i, 0);
	if (id == 0)
		return nullptr;
	return (*EGL::CGLEEffectManager::GlobalObj)->GetById(static_cast<shok::EffectId>(id));
}
EGL::CEffect* luaext::EState::CheckEffect(int i)
{
	EGL::CEffect* d = OptEffect(i);
	if (!d)
		throw lua::LuaException{ std::format("no effect at argument %d", i) };
	return d;
}

shok::EntityId luaext::EState::OptEntityId(int i)
{
	EGL::CGLEEntity* e = OptEntity(i);
	return e ? e->EntityId : static_cast<shok::EntityId>(0);
}

void luaext::EState::PushPos(const shok::Position& p)
{
	NewTable();
	Push("X");
	Push(p.X);
	SetTableRaw(-3);
	Push("Y");
	Push(p.Y);
	SetTableRaw(-3);
}
shok::Position luaext::EState::CheckPos(int idx)
{
	int i = ToAbsoluteIndex(idx);
	CheckType(i, lua::LType::Table);
	Push("X");
	GetTableRaw(i);
	float x = CheckFloat(-1);
	Push("Y");
	GetTableRaw(i);
	float y = CheckFloat(-1);
	Pop(2);
	int size = *EGL::CGLEGameLogic::MapSize * 100;
	if (!(x >= 0 && y >= 0 && x < size && y < size))
		throw lua::LuaException("position outside of map");
	return shok::Position(x, y);
}
shok::PositionRot luaext::EState::CheckPosRot(int i, bool rad)
{
	i = ToAbsoluteIndex(i);
	Push("X");
	GetTableRaw(i);
	float x = CheckFloat(-1);
	Push("Y");
	GetTableRaw(i);
	float y = CheckFloat(-1);
	Push("r");
	GetTableRaw(i);
	float r = CheckFloat(-1);
	if (rad)
		r = CppLogic::DegreesToRadians(r);
	Pop(3);
	int size = *EGL::CGLEGameLogic::MapSize * 100;
	if (!(x >= 0 && y >= 0 && x < size&& y < size))
		throw lua::LuaException("position outside of map");
	return { x,y,r };
}
void luaext::EState::PushPosRot(const shok::PositionRot& p, bool rad)
{
	PushPos(p);
	Push("r");
	if (rad)
		Push(CppLogic::RadiansToDegrees(p.r));
	else
		Push(p.r);
	SetTableRaw(-3);
}

void luaext::EState::CheckEntityAlive(shok::EntityId id, const char* msg)
{
	if (EGL::CGLEEntity::EntityIDIsDead(id))
		throw lua::LuaException(msg);
}

void luaext::EState::ReadCostInfo(int index, shok::CostInfo& c, bool ignoreZeroes)
{
	float i = 0;
	GetTableRaw(index, static_cast<int>(shok::ResourceType::Clay));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Clay = i;
	}
	Pop(1);
	GetTableRaw(index, static_cast<int>(shok::ResourceType::Gold));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Gold = i;
	}
	Pop(1);
	GetTableRaw(index, static_cast<int>(shok::ResourceType::Iron));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Iron = i;
	}
	Pop(1);
	GetTableRaw(index, static_cast<int>(shok::ResourceType::Silver));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Silver = i;
	}
	Pop(1);
	GetTableRaw(index, static_cast<int>(shok::ResourceType::Stone));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Stone = i;
	}
	Pop(1);
	GetTableRaw(index, static_cast<int>(shok::ResourceType::Sulfur));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Sulfur = i;
	}
	Pop(1);
	GetTableRaw(index, static_cast<int>(shok::ResourceType::Wood));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Wood = i;
	}
	Pop(1);
}
void luaext::EState::PushCostInfo(const shok::CostInfo& c)
{
	NewTable();
	for (int i = 1; i <= shok::ResourceType_MaxValue; i++) {
		Push(c.GetResourceAmountFromType(static_cast<shok::ResourceType>(i), false));
		SetTableRaw(-2, i);
	}
}

GGlue::CGlueEntityProps* luaext::EState::OptEntityType(int idx)
{
	if (!IsNumber(idx) && !IsString(idx))
		return nullptr;
	auto t = CheckEnum<shok::EntityTypeId>(idx);
	return CppLogic::GetEntityType(t);
}
GGlue::CGlueEntityProps* luaext::EState::CheckEntityType(int idx)
{
	GGlue::CGlueEntityProps* t = OptEntityType(idx);
	if (t == nullptr)
		throw lua::LuaException{ std::format("no entitytype at {}", idx) };
	return t;
}

void luaext::EState::StringToLower()
{
	Push("string");
	GetGlobal();
	Push("lower");
	GetTableRaw(-2);
	Remove(-2);
	Insert(-2);
	PCall(1, 1, 0);
}

shok::Technology* luaext::EState::CheckTech(int idx)
{
	auto tid = CheckEnum<shok::TechnologyId>(idx);
	shok::Technology* tech = CppLogic::GetTechnology(tid);
	if (!tech)
		throw lua::LuaException{ std::format("no tech at {}", idx) };
	return tech;
}
shok::PlayerId luaext::EState::CheckPlayerId(int idx, bool allowZero)
{
	int p = CheckInt(idx);
	if (!allowZero && p == 0)
		throw lua::LuaException{ std::format("player 0 invalid at {}", idx) };
	if (p < 0 || p > 8)
		throw lua::LuaException{ std::format("player {} invalid at {}", p, idx) };
	return static_cast<shok::PlayerId>(p);
}
shok::PlayerId luaext::EState::OptPlayerId(int idx, shok::PlayerId def, bool allowZero)
{
	if (!IsNoneOrNil(idx))
		return CheckPlayerId(idx, allowZero);
	return def;
}

EGUIX::CBaseWidget* luaext::EState::CheckWidget(int idx)
{
	shok::WidgetId id;
	EGUIX::WidgetManager* wm = EGUIX::WidgetManager::GlobalObj();
	if (Type(idx) == lua::LType::String)
		id = wm->GetIdByName(ToString(idx));
	else
		id = static_cast<shok::WidgetId>(CheckInt(idx));
	EGUIX::CBaseWidget* r = wm->GetWidgetByID(id);
	if (!r)
		throw lua::LuaException{ std::format("no widget at {}", idx) };
	return r;
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

#include "pch.h"
#include "luaext.h"
#include "s5_entitytype.h"
#include "s5_entity.h"
#include "s5_maplogic.h"
#include "s5_widget.h"

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
		ThrowLuaFormatted("no entity at argument %d", i);
	return d;
}
EGL::CGLEEntity* luaext::EState::OptEntity(int i)
{
	int id = 0;
	if (Type(i) == lua::LType::String) {
		id = EGL::CGLEEntity::GetEntityIDByScriptName(ToString(i));
	}
	else if (IsNumber(i)) {
		id = ToInteger(i);
	}
	if (id == 0)
		return nullptr;
	return EGL::CGLEEntity::GetEntityByID(id);
}

GGL::CSettler* luaext::EState::CheckSettler(int i)
{
	GGL::CSettler* s = OptSettler(i);
	if (!s)
		ThrowLuaFormatted("no settler at argument %d", i);
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
		ThrowLuaFormatted("no building at argument %d", i);
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
		ThrowLuaFormatted("no resource entity at argument %d", i);
	return d;
}
GGL::CResourceDoodad* luaext::EState::OptResourceDoodad(int i)
{
	return dynamic_cast<GGL::CResourceDoodad*>(OptEntity(i));
}

int luaext::EState::OptEntityId(int i)
{
	EGL::CGLEEntity* e = OptEntity(i);
	return e ? e->EntityId : 0;
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

void luaext::EState::CheckEntityAlive(int id, const char* msg)
{
	if (EGL::CGLEEntity::EntityIDIsDead(id))
		throw lua::LuaException(msg);
}

void luaext::EState::ReadCostInfo(int index, shok::CostInfo& c, bool ignoreZeroes)
{
	float i = 0;
	GetTableRawI(index, static_cast<int>(shok::ResourceType::Clay));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Clay = i;
	}
	Pop(1);
	GetTableRawI(index, static_cast<int>(shok::ResourceType::Gold));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Gold = i;
	}
	Pop(1);
	GetTableRawI(index, static_cast<int>(shok::ResourceType::Iron));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Iron = i;
	}
	Pop(1);
	GetTableRawI(index, static_cast<int>(shok::ResourceType::Silver));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Silver = i;
	}
	Pop(1);
	GetTableRawI(index, static_cast<int>(shok::ResourceType::Stone));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Stone = i;
	}
	Pop(1);
	GetTableRawI(index, static_cast<int>(shok::ResourceType::Sulfur));
	if (IsNumber(-1)) {
		i = CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Sulfur = i;
	}
	Pop(1);
	GetTableRawI(index, static_cast<int>(shok::ResourceType::Wood));
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
		Push(0);
		SetTableRawI(-2, i);
	}
	Push(c.Clay);
	SetTableRawI(-2, static_cast<int>(shok::ResourceType::Clay));
	Push(c.Gold);
	SetTableRawI(-2, static_cast<int>(shok::ResourceType::Gold));
	Push(c.Iron);
	SetTableRawI(-2, static_cast<int>(shok::ResourceType::Iron));
	Push(c.Silver);
	SetTableRawI(-2, static_cast<int>(shok::ResourceType::Silver));
	Push(c.Stone);
	SetTableRawI(-2, static_cast<int>(shok::ResourceType::Stone));
	Push(c.Sulfur);
	SetTableRawI(-2, static_cast<int>(shok::ResourceType::Sulfur));
	Push(c.Wood);
	SetTableRawI(-2, static_cast<int>(shok::ResourceType::Wood));
}

GGlue::CGlueEntityProps* luaext::EState::OptEntityType(int idx)
{
	if (!IsNumber(idx))
		return nullptr;
	int t = CheckInt(idx);
	return (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(t);
}
GGlue::CGlueEntityProps* luaext::EState::CheckEntityType(int idx)
{
	GGlue::CGlueEntityProps* t = OptEntityType(idx);
	if (t == nullptr)
		ThrowLuaFormatted("no entitytype at %d", idx);
	return t;
}

void luaext::EState::StringToLower()
{
	Push("string");
	GetTableRaw(GLOBALSINDEX);
	Push("lower");
	GetTableRaw(-2);
	Remove(-2);
	Insert(-2);
	PCall(1, 1, 0);
}

shok::ResourceType luaext::EState::CheckResourceType(int i)
{
	int r = CheckInt(i);
	if (r <= 0 || r > shok::ResourceType_MaxValue)
		throw lua::LuaException("no resource type");
	return static_cast<shok::ResourceType>(r);
}

shok::Technology* luaext::EState::CheckTech(int idx)
{
	int tid = CheckInt(idx);
	shok::Technology* tech = (*GGL::CGLGameLogic::GlobalObj)->GetTech(tid);
	if (!tech)
		ThrowLuaFormatted("no tech at %d", idx);
	return tech;
}

EGUIX::CBaseWidget* luaext::EState::CheckWidget(int idx)
{
	int id;
	EGUIX::WidgetManager* wm = EGUIX::WidgetManager::GlobalObj();
	if (Type(idx) == lua::LType::String)
		id = wm->GetIdByName(ToString(idx));
	else
		id = CheckInt(idx);
	EGUIX::CBaseWidget* r = wm->GetWidgetByID(id);
	if (!r)
		ThrowLuaFormatted("no widget at %i", idx);
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

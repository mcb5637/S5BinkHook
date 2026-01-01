#include <luaext.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/entity/s5_entity.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/ui/s5_widget.h>
#include <shok/effect/s5_effects.h>
#include <shok/globals/s5_entityandeffectmanager.h>

EGL::CGLEEntity* luaext::detail::OptEntity(State L, int i)
{
	shok::EntityId id = {};
	if (L.Type(i) == lua::LType::String) {
		id = EGL::CGLEEntity::GetEntityIDByScriptName(L.ToString(i));
	}
	else if (L.IsNumber(i)) {
		id = static_cast<shok::EntityId>(L.CheckInt(i));
	}
	if (id == shok::EntityId::Invalid)
		return nullptr;
	return EGL::CGLEEntity::GetEntityByID(id);
}


EGL::CEffect* luaext::detail::OptEffect(State L, int i)
{
	int id = L.OptInt(i, 0);
	if (id == 0)
		return nullptr;
	return (*EGL::CGLEEffectManager::GlobalObj)->GetById(static_cast<shok::EffectId>(id));
}

shok::EntityId luaext::detail::OptEntityId(State L, int i)
{
	EGL::CGLEEntity* e = L.OptEntity(i);
	return e ? e->EntityId : static_cast<shok::EntityId>(0);
}

void luaext::detail::PushPos(State L, const shok::Position& p)
{
	L.NewTable();
	L.Push("X");
	L.Push(p.X);
	L.SetTableRaw(-3);
	L.Push("Y");
	L.Push(p.Y);
	L.SetTableRaw(-3);
}
shok::Position luaext::detail::CheckPos(State L, int idx)
{
	int i = L.ToAbsoluteIndex(idx);
	L.CheckType(i, lua::LType::Table);
	L.Push("X");
	L.GetTableRaw(i);
	float x = L.CheckFloat(-1);
	L.Push("Y");
	L.GetTableRaw(i);
	float y = L.CheckFloat(-1);
	L.Pop(2);
	auto size = static_cast<float>(*EGL::CGLEGameLogic::MapSize * 100);
	if (!(x >= 0 && y >= 0 && x < size && y < size))
		throw lua::LuaException("position outside of map");
	return shok::Position(x, y);
}
shok::PositionRot luaext::detail::CheckPosRot(State L, int i, bool rad)
{
	i = L.ToAbsoluteIndex(i);
	L.Push("X");
	L.GetTableRaw(i);
	float x = L.CheckFloat(-1);
	L.Push("Y");
	L.GetTableRaw(i);
	float y = L.CheckFloat(-1);
	L.Push("r");
	L.GetTableRaw(i);
	float r = L.CheckFloat(-1);
	if (rad)
		r = CppLogic::DegreesToRadians(r);
	L.Pop(3);
	auto size = static_cast<float>(*EGL::CGLEGameLogic::MapSize * 100);
	if (!(x >= 0 && y >= 0 && x < size && y < size))
		throw lua::LuaException("position outside of map");
	return { x,y,r };
}
void luaext::detail::PushPosRot(State L, const shok::PositionRot& p, bool rad)
{
	PushPos(L, p);
	L.Push("r");
	if (rad)
		L.Push(CppLogic::RadiansToDegrees(p.r));
	else
		L.Push(p.r);
	L.SetTableRaw(-3);
}

void luaext::detail::CheckEntityAlive(shok::EntityId id, const char* msg)
{
	if (EGL::CGLEEntity::EntityIDIsDead(id))
		throw lua::LuaException(msg);
}

void luaext::detail::ReadCostInfo(State L, int index, shok::CostInfo& c, bool ignoreZeroes)
{
	float i = 0;
	L.GetTableRaw(index, static_cast<int>(shok::ResourceType::Clay));
	if (L.IsNumber(-1)) {
		i = L.CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Clay = i;
	}
	L.Pop(1);
	L.GetTableRaw(index, static_cast<int>(shok::ResourceType::Gold));
	if (L.IsNumber(-1)) {
		i = L.CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Gold = i;
	}
	L.Pop(1);
	L.GetTableRaw(index, static_cast<int>(shok::ResourceType::Iron));
	if (L.IsNumber(-1)) {
		i = L.CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Iron = i;
	}
	L.Pop(1);
	L.GetTableRaw(index, static_cast<int>(shok::ResourceType::Silver));
	if (L.IsNumber(-1)) {
		i = L.CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Silver = i;
	}
	L.Pop(1);
	L.GetTableRaw(index, static_cast<int>(shok::ResourceType::Stone));
	if (L.IsNumber(-1)) {
		i = L.CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Stone = i;
	}
	L.Pop(1);
	L.GetTableRaw(index, static_cast<int>(shok::ResourceType::Sulfur));
	if (L.IsNumber(-1)) {
		i = L.CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Sulfur = i;
	}
	L.Pop(1);
	L.GetTableRaw(index, static_cast<int>(shok::ResourceType::Wood));
	if (L.IsNumber(-1)) {
		i = L.CheckFloat(-1);
		if (i != 0 || !ignoreZeroes)
			c.Wood = i;
	}
	L.Pop(1);
}
void luaext::detail::PushCostInfo(State L, const shok::CostInfo& c)
{
	L.NewTable();
	for (int i = 1; i <= shok::ResourceType_MaxValue; i++) {
		L.Push(c.GetResourceAmountFromType(static_cast<shok::ResourceType>(i), false));
		L.SetTableRaw(-2, i);
	}
}

GGlue::CGlueEntityProps* luaext::detail::OptEntityType(State L, int idx)
{
	if (!L.IsNumber(idx) && !L.IsString(idx))
		return nullptr;
	auto t = L.CheckEnum<shok::EntityTypeId>(idx);
	return CppLogic::GetEntityType(t);
}


shok::Technology* luaext::detail::CheckTech(State L, int idx)
{
	auto tid = L.CheckEnum<shok::TechnologyId>(idx);
	shok::Technology* tech = CppLogic::GetTechnology(tid);
	if (!tech)
		throw lua::LuaException{ std::format("no tech at {}", idx) };
	return tech;
}
shok::PlayerId luaext::detail::CheckPlayerId(State L, int idx, bool allowZero)
{
	int p = L.CheckInt(idx);
	if (!allowZero && p == 0)
		throw lua::LuaException{ std::format("player 0 invalid at {}", idx) };
	if (p < 0 || p > 8)
		throw lua::LuaException{ std::format("player {} invalid at {}", p, idx) };
	return static_cast<shok::PlayerId>(p);
}

EGUIX::CBaseWidget* luaext::detail::CheckWidget(State L, int idx)
{
	shok::WidgetId id;
	auto t = L.GetTop();
	EGUIX::WidgetManager* wm = EGUIX::WidgetManager::GlobalObj();
	if (L.Type(idx) == lua::LType::String)
		id = wm->GetIdByName(L.ToString(idx));
	else
		id = static_cast<shok::WidgetId>(L.CheckInt(idx));
	EGUIX::CBaseWidget* r = wm->GetWidgetByID(id);
	if (!r)
		throw lua::LuaException{ std::format("no widget at {}", idx) };
	return r;
}

int str_ends_with(const char* str, const char* suffix) {

	if (str == nullptr || suffix == nullptr)
		return 0;

	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);

	if (suffix_len > str_len)
		return 0;

	return 0 == strncmp(str + str_len - suffix_len, suffix, suffix_len);
}

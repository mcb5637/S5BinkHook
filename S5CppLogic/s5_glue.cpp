#include "pch.h"
#include "s5_glue.h"
#include "s5_behaviorProps.h"
#include "s5_entitydisplay.h"

static inline void(__thiscall* const epropsmng_loadentitytypebyid)(GGlue::CEntitiesPropsMgr* th, int id) = reinterpret_cast<void(__thiscall*)(GGlue::CEntitiesPropsMgr*, int)>(0x5B931B);
void GGlue::CEntitiesPropsMgr::LoadEntityTypeByID(int id)
{
	if (id >= static_cast<int>(CGLEEntitiesProps.EntityTypes.size()))
	{
		auto glue = CGLEEntitiesProps.EntityTypes.SaveVector();
		glue.Vector.push_back({});
		auto log = CGLEEntitiesProps.EntityTypesLogicProps.SaveVector();
		log.Vector.push_back(nullptr);
		auto dis = CGLEEntitiesProps.EntityTypesDisplayProps.SaveVector();
		dis.Vector.push_back(nullptr);
	}
	epropsmng_loadentitytypebyid(this, id);
}

void GGlue::CEntitiesPropsMgr::PopEntityType(int id)
{
	if (id + 1 != static_cast<int>(CGLEEntitiesProps.EntityTypes.size()))
		throw std::out_of_range("invalid id");
	FreeEntityType(id);
	auto glue = CGLEEntitiesProps.EntityTypes.SaveVector();
	glue.Vector.pop_back();
	auto log = CGLEEntitiesProps.EntityTypesLogicProps.SaveVector();
	log.Vector.pop_back();
	auto dis = CGLEEntitiesProps.EntityTypesDisplayProps.SaveVector();
	dis.Vector.pop_back();
}

void GGlue::CEntitiesPropsMgr::FreeEntityType(int id)
{
	GGlue::CGlueEntityProps& e = CGLEEntitiesProps.EntityTypes[id];
	for (auto& b : e.BehaviorProps) {
		delete b.Logic;
		delete b.Display;
	}
	{
		auto b = e.BehaviorProps.SaveVector();
		b.Vector.clear();
	}
	delete e.LogicProps;
	delete e.DisplayProps;
	e.LogicProps = nullptr;
	e.DisplayProps = nullptr;
}

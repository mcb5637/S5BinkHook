#include "pch.h"
#include "s5_glue.h"
#include "s5_behaviorProps.h"
#include "s5_entitydisplay.h"
#include "s5_effecttype.h"
#include "s5_idmanager.h"
#include "s5_classfactory.h"
#include "s5_filesystem.h"

static inline void(__thiscall* effectlogic_init)(EGL::CGLEEffectProps* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEffectProps*)>(0x589297);
void GGlue::CEffectsPropsMgr::LoadEffectTypeFromExtraFile(int id)
{
	if (id > static_cast<int>(EffectsProps.Effects.size()))
		throw std::logic_error{ "somehow the id is too big" };
	if (id == static_cast<int>(EffectsProps.Effects.size())) {
		auto v = EffectsProps.Effects.SaveVector();
		v.Vector.emplace_back();
		auto vl = EffectsLogicProps.EffectTypes.SaveVector();
		vl.Vector.push_back(nullptr);
		auto vd = EffectsDisplayProps.EffectTypes.SaveVector();
		vd.Vector.push_back(nullptr);
	}
	auto& t = EffectsProps.Effects[id];
	std::string filename = "Data/Config/Effects/";
	filename.append(EffectTypeManager->GetNameByID(id));
	filename.append(".xml");
	(*BB::CClassFactory::GlobalObj)->LoadObject(&t, filename.c_str(), t.SerializationData);
	if (!t.Display || !t.Logic) {
		FreeEffectType(id);
		throw std::invalid_argument{ "invalid file" };
	}
	effectlogic_init(t.Logic);
	EffectsLogicProps.EffectTypes[id] = t.Logic;
	EffectsDisplayProps.EffectTypes[id] = t.Display;
}
void GGlue::CEffectsPropsMgr::FreeEffectType(int id)
{
	auto& t = EffectsProps.Effects[id];
	delete t.Display;
	delete t.Logic;
	t.Display = nullptr;
	t.Logic = nullptr;
	EffectsLogicProps.EffectTypes[id] = nullptr;
	EffectsDisplayProps.EffectTypes[id] = nullptr;
}

void GGlue::CEffectsPropsMgr::ReloadAllEffectTypes()
{
	for (unsigned int i = EffectsProps.Effects.size() - 1; i > 0; --i) {
		FreeEffectType(i);
		EffectTypeManager->RemoveID(i);
	}
	FreeEffectType(0); // there is no effect 0, just a placeholder. not sure if i need to remove it
	{
		auto v = EffectsProps.Effects.SaveVector();
		v.Vector.clear();
		auto vl = EffectsLogicProps.EffectTypes.SaveVector();
		vl.Vector.clear();
		auto vd = EffectsDisplayProps.EffectTypes.SaveVector();
		vd.Vector.clear();
	}
	LoadData("Data\\Config\\Effects.xml");
}

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
	CGLEEntitiesProps.EntityTypesLogicProps[id] = nullptr;
	CGLEEntitiesProps.EntityTypesDisplayProps[id] = nullptr;
}

#include "pch.h"
#include "s5_glue.h"
#include <cstring>
#include "s5_behaviorProps.h"
#include "s5_entitydisplay.h"
#include "s5_effecttype.h"
#include "s5_idmanager.h"
#include "s5_classfactory.h"
#include "s5_filesystem.h"
#include "s5_mapdisplay.h"
#include "s5_animset.h"

void GGlue::CGlueAnimsPropsMgr::CreateAnimProps(int id)
{
	if (id > static_cast<int>(Data.size()))
		throw std::logic_error{ "somehow the id is too big" };
	if (id == static_cast<int>(Data.size())) {
		auto v = Data.SaveVector();
		v.Vector.push_back(new EGL::CGLEAnimProps());
	}
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	char buff[0x101]{};
	strcpy_s(buff, 0x100, (*BB::CIDManagerEx::AnimManager)->GetNameByID(id));
	shok::StringToLowerCase(buff);
	auto* t = Data.at(id);
	t->Duration = static_cast<int>(mng->GetAnimationDuration(id) * 10.0f); // * 1000 / 100
	if (strstr(buff, "_walk") != nullptr || strstr(buff, "_run") != nullptr)
		t->IsWalkOrRun = true;
	else
		t->IsWalkOrRun = false;
}

void GGlue::CGlueAnimsPropsMgr::PopAnimPops(int id)
{
	if (id + 1 != static_cast<int>(Data.size()))
		throw std::out_of_range{ "invalid id" };
	auto v = Data.SaveVector();
	delete v.Vector.back();
	v.Vector.pop_back();
}

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
	if (id >= static_cast<int>(EntityTypes.size()))
	{
		auto glue = EntityTypes.SaveVector();
		glue.Vector.emplace_back();
		auto log = CGLEEntitiesProps.EntityTypesLogicProps.SaveVector();
		log.Vector.push_back(nullptr);
		auto dis = DisplayProps.EntityTypesDisplayProps.SaveVector();
		dis.Vector.push_back(nullptr);
	}
	epropsmng_loadentitytypebyid(this, id);
	auto* ty = CGLEEntitiesProps.GetEntityType(id);
	ty->LogicProps->InitializeBlocking();
	RefreshDisplayFlags();
}

void GGlue::CEntitiesPropsMgr::PopEntityType(int id)
{
	if (id + 1 != static_cast<int>(EntityTypes.size()))
		throw std::out_of_range("invalid id");
	FreeEntityType(id);
	auto glue = EntityTypes.SaveVector();
	glue.Vector.pop_back();
	auto log = CGLEEntitiesProps.EntityTypesLogicProps.SaveVector();
	log.Vector.pop_back();
	auto dis = DisplayProps.EntityTypesDisplayProps.SaveVector();
	dis.Vector.pop_back();
	RefreshDisplayFlags();
}

void GGlue::CEntitiesPropsMgr::FreeEntityType(int id)
{
	GGlue::CGlueEntityProps& e = EntityTypes[id];
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
	DisplayProps.EntityTypesDisplayProps[id] = nullptr;
}

void GGlue::CEntitiesPropsMgr::RefreshDisplayFlags()
{
	delete (*ED::CGlobalsBaseEx::GlobalObj)->EntityTypeFlags;
	(*ED::CGlobalsBaseEx::GlobalObj)->EntityTypeFlags = new ED::CEntitiesTypeFlags((*ED::CGlobalsBaseEx::GlobalObj)->EntityTypeDisplays);
}

void GGlue::CTerrainPropsMgr::ReloadTerrainTypes()
{
	auto lv = Logic.LogicProps.SaveVector();
	lv.Vector.clear();
	auto dv = Display.DisplayProps.SaveVector();
	dv.Vector.clear();
	if (Display.TransitionTexture) {
		shok::Free(Display.TransitionTexture);
		Display.TransitionTexture = nullptr;
	}
	if (Display.SnowTransitionTexture) {
		shok::Free(Display.SnowTransitionTexture);
		Display.SnowTransitionTexture = nullptr;
	}
	TerrainTypeManager->clear();
	(*BB::CClassFactory::GlobalObj)->LoadObject(this, "Data\\Config\\Terrain.xml", SerializationData);

	for (const auto& e : TerrainType) {
		lv.Vector.push_back(e.Logic);
		dv.Vector.push_back(e.Display);
	}
	TerrainType.SaveVector().Vector.clear();
}

void GGlue::CTerrainPropsMgr::LoadTerrainTypeFromExtraFile(int id)
{
	if (id > static_cast<int>(Display.DisplayProps.size()))
		throw std::logic_error{ "somehow the id is too big" };
	TerrainTypeData d{};
	 
	std::string filename = "Data/Config/TerrainTypes/";
	filename.append(TerrainTypeManager->GetNameByID(id));
	filename.append(".xml");
	(*BB::CClassFactory::GlobalObj)->LoadObject(&d, filename.c_str(), d.SerializationData);

	auto lv = Logic.LogicProps.SaveVector();
	auto dv = Display.DisplayProps.SaveVector();

	while (id > static_cast<int>(dv.Vector.size())) {
		lv.Vector.emplace_back();
		dv.Vector.emplace_back();
	}

	lv.Vector.at(id) = d.Logic;
	dv.Vector.at(id) = d.Display;
}

static inline void(__thiscall* const watertype_display_init)(GGlue::WaterTypeData::DisplayData* th) = reinterpret_cast<void(__thiscall*)(GGlue::WaterTypeData::DisplayData*)>(0x5BE750);
void GGlue::WaterTypeData::DisplayData::Initialize()
{
	watertype_display_init(this);
}

void GGlue::CGlueWaterPropsMgr::ReloadWaterTypes()
{
	WaterTypeManager->clear();
	Display.ShoreWaveTypeManager->clear();
	shok::Free(Display.WaterBumpTexName);
	Display.WaterBumpTexName = nullptr;
	shok::Free(Display.SkyTexName);
	Display.SkyTexName = nullptr;
	{
		auto v = Display.ShoreWaveKeyFrame.SaveVector();
		v.Vector.clear();
	}
	{
		auto v = Display.ShoreWave.SaveVector();
		v.Vector.clear();
	}
	// main vecors get cleared by load
	Load("Data\\Config\\Water.xml");
}

void GGlue::CGlueWaterPropsMgr::LoadWaterTypeFromExtraFile(int id)
{
	if (id > static_cast<int>(Display.WaterDisplay.size()))
		throw std::logic_error{ "somehow the id is too big" };
	WaterTypeData d{};

	std::string filename = "Data/Config/WaterTypes/";
	filename.append(WaterTypeManager->GetNameByID(id));
	filename.append(".xml");
	(*BB::CClassFactory::GlobalObj)->LoadObject(&d, filename.c_str(), d.SerializationData);
	d.Display.Initialize();
	
	auto lv = Logic.WaterLogic.SaveVector();
	auto dv = Display.WaterDisplay.SaveVector();
	while (id > static_cast<int>(dv.Vector.size())) {
		lv.Vector.emplace_back();
		dv.Vector.emplace_back();
	}

	lv.Vector.at(id) = d.Logic;
	dv.Vector.at(id) = d.Display;
}

inline EGL::CGLEAnimSet* (__thiscall* const animsetmng_get)(EGL::AnimSetManager* th, int id) = reinterpret_cast<EGL::CGLEAnimSet * (__thiscall*)(EGL::AnimSetManager*, int)>(0x588487);
EGL::CGLEAnimSet* EGL::AnimSetManager::GetAnimSet(int id)
{
	return animsetmng_get(this, id);
}

void EGL::AnimSetManager::FreeAnimSet(int id)
{
	--id;
	delete AnimSets[id];
	AnimSets[id] = nullptr;
}
void EGL::AnimSetManager::LoadAnimSet(int id)
{
	int off = id - 1;
	if (off > static_cast<int>(AnimSets.size()))
		throw std::logic_error{ "somehow the id is too big" };

	CGLEAnimSet* set = new CGLEAnimSet();
	set->Id = id;
	std::string file{ "Data\\Config\\AnimSets\\" };
	file.append((*BB::CIDManagerEx::AnimSetManager)->GetNameByID(id));
	file.append(".xml");
	(*BB::CClassFactory::GlobalObj)->LoadObject(set, file.c_str());

	if (off == static_cast<int>(AnimSets.size())) {
		auto v = AnimSets.SaveVector();
		v.Vector.push_back(set);
	}
	else {
		AnimSets[off] = set;
	}
}
void EGL::AnimSetManager::PopAnimSet(int id)
{
	--id;
	if (id + 1 != static_cast<int>(AnimSets.size()))
		throw std::out_of_range("invalid id");
	FreeAnimSet(id);
	auto v = AnimSets.SaveVector();
	v.Vector.pop_back();
}

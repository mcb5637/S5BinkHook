#include "s5_glue.h"
#include <cstring>
#include <shok/entitytype/s5_behaviorProps.h>
#include <shok/entity/s5_entitydisplay.h>
#include <shok/effect/s5_effecttype.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/entity/s5_animset.h>
#include <shok/globals/s5_framework.h>

void GGlue::CGlueAnimsPropsMgr::CreateAnimProps(shok::AnimationId i)
{
	int id = static_cast<int>(i);
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
	t->Duration = static_cast<int>(mng->GetAnimationDuration(i) * 10.0f); // * 1000 / 100
	if (strstr(buff, "_walk") != nullptr || strstr(buff, "_run") != nullptr)
		t->IsWalkOrRun = true;
	else
		t->IsWalkOrRun = false;
}

void GGlue::CGlueAnimsPropsMgr::PopAnimPops(shok::AnimationId i)
{
	int id = static_cast<int>(i);
	if (id + 1 != static_cast<int>(Data.size()))
		throw std::out_of_range{ "invalid id" };
	auto v = Data.SaveVector();
	delete v.Vector.back();
	v.Vector.pop_back();
}

static inline void(__thiscall* effectlogic_init)(EGL::CGLEEffectProps* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEffectProps*)>(0x589297);
void GGlue::CEffectsPropsMgr::LoadEffectTypeFromExtraFile(shok::EffectTypeId i)
{
	int id = static_cast<int>(i);
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
		FreeEffectType(i);
		throw std::invalid_argument{ "invalid file" };
	}
	effectlogic_init(t.Logic);
	EffectsLogicProps.EffectTypes[id] = t.Logic;
	EffectsDisplayProps.EffectTypes[id] = t.Display;
}
void GGlue::CEffectsPropsMgr::FreeEffectType(shok::EffectTypeId i)
{
	int id = static_cast<int>(i);
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
		FreeEffectType(static_cast<shok::EffectTypeId>(i));
		EffectTypeManager->RemoveID(static_cast<int>(i));
	}
	FreeEffectType(static_cast<shok::EffectTypeId>(0)); // there is no effect 0, just a placeholder. not sure if i need to remove it
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

EGL::EffectsProps::EffectType* GGlue::CEffectsPropsMgr::Get(shok::EffectTypeId id)
{
	int i = static_cast<int>(id);
	if (i > 0 && i <= static_cast<int>(EffectsProps.Effects.size()))
		return &EffectsProps.Effects[i];
	return nullptr;
}

EGL::EffectType* CppLogic::GetEffectType(shok::EffectTypeId id)
{
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager;
	return mng->Get(id);
}

static inline void(__thiscall* const epropsmng_loadentitytypebyid)(GGlue::CEntitiesPropsMgr* th, int id) = reinterpret_cast<void(__thiscall*)(GGlue::CEntitiesPropsMgr*, int)>(0x5B931B);
void GGlue::CEntitiesPropsMgr::LoadEntityTypeByID(shok::EntityTypeId i)
{
	int id = static_cast<int>(i);
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
	auto* ty = CppLogic::GetEntityType(i);
	ty->LogicProps->InitializeBlocking();
	RefreshDisplayFlags();
}

void GGlue::CEntitiesPropsMgr::PopEntityType(shok::EntityTypeId i)
{
	int id = static_cast<int>(i);
	if (id + 1 != static_cast<int>(EntityTypes.size()))
		throw std::out_of_range("invalid id");
	FreeEntityType(i);
	auto glue = EntityTypes.SaveVector();
	glue.Vector.pop_back();
	auto log = CGLEEntitiesProps.EntityTypesLogicProps.SaveVector();
	log.Vector.pop_back();
	auto dis = DisplayProps.EntityTypesDisplayProps.SaveVector();
	dis.Vector.pop_back();
	RefreshDisplayFlags();
}

void GGlue::CEntitiesPropsMgr::FreeEntityType(shok::EntityTypeId i)
{
	int id = static_cast<int>(i);
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

void GGlue::CDamageClassesPropsMgr::AddDamageClass(shok::DamageClassId id, GGL::CDamageClassProps* c)
{
	auto v = Logic.DamageClassList.SaveVector();
	if (v.Vector.size() == static_cast<int>(id))
		v.Vector.push_back(c);
	else if (static_cast<int>(v.Vector.size()) > static_cast<int>(id))
		v.Vector[static_cast<int>(id)] = c;
	else
		throw std::invalid_argument{"invalid damageclass id"};
}
void GGlue::CDamageClassesPropsMgr::ResetDamageClasses()
{
	auto v = Logic.DamageClassList.SaveVector();
	for (int id = static_cast<int>(v.Vector.size())-1; id >= static_cast<int>(DamageClass.size()); --id) {
		DamageClassManager->RemoveID(id);
		delete v.Vector[id];
		v.Vector.pop_back();
	}
	for (int id = 1; id < static_cast<int>(DamageClass.size()); ++id) {
		if (DamageClass[id] != v.Vector[id]) {
			if (DamageClass[id] == nullptr) {
				delete v.Vector[id];
				v.Vector[id] = (*BB::CClassFactory::GlobalObj)->CreateObject<GGL::CDamageClassProps>();
			}
			else {
				delete v.Vector[id];
				v.Vector[id] = DamageClass[id];
			}
		}
	}
}

GGL::CDamageClassProps* CppLogic::GetDamageClass(shok::DamageClassId id)
{
	return (*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager->Logic.Get(id);
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

void GGlue::CTerrainPropsMgr::LoadTerrainTypeFromExtraFile(shok::TerrainTypeId i)
{
	int id = static_cast<int>(i);
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

std::pair<GGlue::TerrainTypeLogicData*, GGlue::TerrainTypeDisplayData*> CppLogic::GetTerrainType(shok::TerrainTypeId id)
{
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager;
	int i = static_cast<int>(id);
	if (i > 0 && i <= static_cast<int>(mng->Logic.LogicProps.size()) && i <= static_cast<int>(mng->Display.DisplayProps.size()))
		return { &mng->Logic.LogicProps[i], &mng->Display.DisplayProps[i] };
	return { nullptr, nullptr };
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

void GGlue::CGlueWaterPropsMgr::LoadWaterTypeFromExtraFile(shok::WaterTypeId i)
{
	int id = static_cast<int>(i);
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

std::pair<GGlue::WaterTypeLogicData*, GGlue::WaterTypeDisplayData*> CppLogic::GetWaterType(shok::WaterTypeId id)
{
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager;
	int i = static_cast<int>(id);
	if (i > 0 && i <= static_cast<int>(mng->Logic.WaterLogic.size()) && i <= static_cast<int>(mng->Display.WaterDisplay.size()))
		return { &mng->Logic.WaterLogic[i], &mng->Display.WaterDisplay[i] };
	return { nullptr, nullptr };
}

inline EGL::CGLEAnimSet* (__thiscall* const animsetmng_get)(EGL::AnimSetManager* th, shok::AnimSetId id) = reinterpret_cast<EGL::CGLEAnimSet * (__thiscall*)(EGL::AnimSetManager*, shok::AnimSetId)>(0x588487);
EGL::CGLEAnimSet* EGL::AnimSetManager::GetAnimSet(shok::AnimSetId id)
{
	return animsetmng_get(this, id);
}

EGL::CGLEAnimSet* CppLogic::GetAnimSet(shok::AnimSetId id) {
	return (*EGL::AnimSetManager::GlobalObj)->GetAnimSet(id);
}

void EGL::AnimSetManager::FreeAnimSet(shok::AnimSetId i)
{
	int id = static_cast<int>(i);
	--id;
	delete AnimSets[id];
	AnimSets[id] = nullptr;
}
void EGL::AnimSetManager::LoadAnimSet(shok::AnimSetId i)
{
	int id = static_cast<int>(i);
	int off = id - 1;
	if (off > static_cast<int>(AnimSets.size()))
		throw std::logic_error{ "somehow the id is too big" };

	auto* set = new CGLEAnimSet();
	set->Id = i;
	std::string file{ R"(Data\Config\AnimSets\)" };
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
void EGL::AnimSetManager::PopAnimSet(shok::AnimSetId i)
{
	int id = static_cast<int>(i);
	--id;
	if (id + 1 != static_cast<int>(AnimSets.size()))
		throw std::out_of_range("invalid id");
	FreeAnimSet(i);
	auto v = AnimSets.SaveVector();
	v.Vector.pop_back();
}

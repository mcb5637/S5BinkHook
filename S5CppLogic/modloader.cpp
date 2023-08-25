#include "pch.h"
#include "modloader.h"

#include <regex>

#include "s5_filesystem.h"
#include "s5_glue.h"
#include "s5_entitytype.h"
#include "s5_idmanager.h"
#include "s5_entityandeffectmanager.h"
#include "s5_tasklist.h"
#include "s5_maplogic.h"
#include "s5_mapdisplay.h"
#include "s5_tech.h"
#include "s5_widget.h"
#include "s5_mapdisplay.h"
#include "s5_entitydisplay.h"
#include "s5_config.h"
#include "s5_exception.h"
#include "s5_sound.h"
#include "entityiterator.h"
#include "luaext.h"
#include "dump_guitextures.h"
#include "luaserializer.h"

void CppLogic::ModLoader::ModLoader::Init(lua::State L, const char* mappath, const char* func)
{
	Log(L, "Initializing ModLoader");

	Log(L, mappath);
	if (!BB::CFileSystemMgr::DoesFileExist(mappath)) {
		Log(L, "no ModLoader.lua, aborting");
		return;
	}
	int t = L.GetTop();
	EScr::CScriptTriggerSystem::LoadFileToLuaState(L.GetState(), mappath);
	AddLib(L);
	bool keeparchive = false;
	L.Push("ModLoader");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.Push("Initialize");
		L.GetTableRaw(-2);
		if (L.IsFunction(-1))
			L.PCall(0, 0);
		else
			L.Pop(1);
		L.Push("KeepArchive");
		L.GetTableRaw(-2);
		keeparchive = L.ToBoolean(-1);
		L.Pop(1);
		L.Push(func);
		L.GetTableRaw(-2);
		if (L.IsFunction(-1))
			L.PCall(0, 0);
		else
			L.Pop(1);
	}
	Framework::AGameModeBase::DoNotRemoveNextArchive = keeparchive;
	RemoveLib(L);
	L.SetTop(t);
	Log(L, "Done");
}

void CppLogic::ModLoader::ModLoader::PreMapStart(lua_State* ingame, const char* name, const char* path, bool externalmap)
{
	lua::State L{ ingame };
	static constexpr size_t bufflen = 0x3FF;
	char mappath[bufflen];
	snprintf(mappath, bufflen, "%s\\%s\\ModLoader.lua", externalmap ? "Maps" : path, externalmap ? "ExternalMap" : name);
	Init(L, mappath, "MapStart");
}
const char* internalmap_getpath(GS3DTools::CMapData* d) {
	Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->CampagnInfoHandler.GetCampagnInfo(d);
	if (!ci)
		return "";
	Framework::MapInfo* i = ci->GetMapInfoByName(d->MapName.c_str());
	if (!i)
		return "";
	return i->MapFilePath.c_str();
}
void CppLogic::ModLoader::ModLoader::PreSaveLoad(lua_State* ingame, Framework::GameModeStartMapData* data, bool externalmap)
{
	Framework::SavegameSystem* sdata = Framework::SavegameSystem::GlobalObj();
	if (!sdata->LoadSaveData(data->Folder))
		throw 0;
	lua::State L{ ingame };
	static constexpr size_t bufflen = 0x3FF;
	char mappath[bufflen];
	snprintf(mappath, bufflen, "%s\\ModLoader.lua", externalmap ? "Maps\\ExternalMap" : internalmap_getpath(&sdata->CurrentSave->MapData));
	Init(L, mappath, "LoadSave");
}
void CppLogic::ModLoader::ModLoader::AddLib(lua::State L)
{
	L.Push("CppLogic");
	L.GetGlobal();
	L.Push("ModLoader");
	L.NewTable();

	for (auto l : Loaders)
		l->RegisterFuncs(L);

	L.RegisterFuncs(LuaFuncs, -3);

	L.SetTableRaw(-3);
	L.Pop(1);

	// preload selection textures, so modloader doesnt break them
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures;
	mng->Get("Selection");
	mng->Get("Selection_civilian");
	mng->Get("Selection_hero");
	mng->Get("Selection_Soldier");
	mng->Get("Selection_Building");
}
void CppLogic::ModLoader::ModLoader::RemoveLib(lua::State L)
{
	L.Push("CppLogic");
	L.GetGlobal();
	L.Push("ModLoader");
	L.NewTable();

	L.RegisterFuncs(NoLoaderFuncs, -3);

	L.SetTableRaw(-3);
	L.Pop(1);
}

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityTypeId>::Load(shok::EntityTypeId id, luaext::EState L) {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
	if (static_cast<int>(id) < static_cast<int>(mng->CGLEEntitiesProps.EntityTypesLogicProps.size())) {
		mng->FreeEntityType(id);
	}
	mng->LoadEntityTypeByID(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityTypeId>::TableName() {
	return "Entities";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityTypeId>::FuncName() {
	return "EntityType";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::SanityCheck() {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
	if (mng->EntityTypes.size() != mng->EntityTypeManager->size())
		throw lua::LuaException{"not all entitytypes loaded"};
	for (int id = 1; id < static_cast<int>(mng->EntityTypes.size()); ++id) {
		auto n = mng->EntityTypeManager->GetNameByID(id);
		if (n == nullptr || *n == '\0')
			continue;
		if (mng->EntityTypes[id].LogicProps == nullptr)
			throw lua::LuaException{std::format("entitytype {}={} missing LogicProps", n, id)};
		if (mng->EntityTypes[id].DisplayProps == nullptr)
			throw lua::LuaException{std::format("entitytype {}={} missing DisplayProps", n, id)};
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::UnLoad(shok::EntityTypeId id) {
	(*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager->PopEntityType(id);
	(*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->RemoveID(static_cast<int>(id));
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EffectTypeId>::Load(shok::EffectTypeId id, luaext::EState L) {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager;
	if (static_cast<int>(id) < static_cast<int>(mng->EffectsProps.Effects.size()))
		mng->FreeEffectType(id);
	mng->LoadEffectTypeFromExtraFile(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EffectTypeId>::TableName() {
	return "GGL_Effects";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EffectTypeId>::FuncName() {
	return "EffectType";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::SanityCheck() {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager;
	if (mng->EffectsProps.Effects.size() != mng->EffectTypeManager->size())
		throw lua::LuaException{"not all effect types loaded"};
	for (int id = 1; id < static_cast<int>(mng->EffectsProps.Effects.size()); ++id) {
		auto n = mng->EffectTypeManager->GetNameByID(id);
		if (n == nullptr || *n == '\0')
			continue;
		if (mng->EffectsProps.Effects[id].Logic == nullptr)
			throw lua::LuaException{std::format("effecttype {}={} missing Logic", n, id)};
		if (mng->EffectsProps.Effects[id].Display == nullptr)
			throw lua::LuaException{std::format("effecttype {}={} missing Display", n, id)};
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::Reset() {
	if (NeedsReload)
		(*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager->ReloadAllEffectTypes();
	NeedsReload = false;
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TaskListId>::Load(shok::TaskListId id, luaext::EState L) {
	auto* m = *EGL::CGLETaskListMgr::GlobalObj;
	if (static_cast<int>(id) < static_cast<int>(m->TaskLists.size()))
		m->FreeTaskList(id);
	m->LoadTaskList(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TaskListId>::TableName() {
	return "TaskLists";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TaskListId>::FuncName() {
	return "TaskList";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TaskListId>::SanityCheck() {
	auto* mng = *EGL::CGLETaskListMgr::GlobalObj;
	if (mng->TaskLists.size() != mng->TaskListManager->size())
		throw lua::LuaException{"not all effect types loaded"};
	for (int id = 1; id < static_cast<int>(mng->TaskLists.size()); ++id) {
		auto n = mng->TaskListManager->GetNameByID(id);
		if (n == nullptr || *n == '\0')
			continue;
		if (mng->TaskLists[id] == nullptr)
			throw lua::LuaException{std::format("tasklist {}={} missing", n, id)};
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TaskListId>::OnIdLoaded(shok::TaskListId id) {
	// make sure ToReload stays empty
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TaskListId>::UnLoad(shok::TaskListId id) {
	if (*EGL::CGLETaskListMgr::GlobalObj)
		(*EGL::CGLETaskListMgr::GlobalObj)->RemoveTaskList(id);
	else
		CppLogic::GetIdManager<shok::TaskListId>().RemoveID(id);
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TaskListId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TaskListId>::Obj{};



void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::DamageClassId>::Load(shok::DamageClassId id, luaext::EState L) {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager;
	L.PushValue(2);
	void* o = CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(L, nullptr, nullptr);
	mng->AddDamageClass(id, static_cast<GGL::CDamageClassProps*>(o));
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::DamageClassId>::TableName() {
	return "DamageClasses";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::DamageClassId>::FuncName() {
	return "DamageClass";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::SanityCheck() {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager;
	if (mng->Logic.DamageClassList.size() != mng->DamageClassManager->size())
		throw lua::LuaException{"not all effect types loaded"};
	for (int id = 1; id < static_cast<int>(mng->Logic.DamageClassList.size()); ++id) {
		auto n = mng->DamageClassManager->GetNameByID(id);
		if (n == nullptr || *n == '\0')
			continue;
		if (mng->Logic.DamageClassList[id] == nullptr)
			throw lua::LuaException{std::format("damageclass {}={} is missing", n, id)};
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Reset() {
	if (NeedsReload)
		(*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager->ResetDamageClasses();
	NeedsReload = false;
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Obj{};


std::array<CppLogic::ModLoader::ModLoader::DataTypeLoader*, 4> CppLogic::ModLoader::ModLoader::Loaders{{
		&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::Obj,
			& CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::Obj,
			& CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TaskListId>::Obj,
			& CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Obj,
	}};

bool CppLogic::ModLoader::ModLoader::Initialized = false;
std::vector<shok::TechnologyId> CppLogic::ModLoader::ModLoader::TechsToRemove{};
std::vector<shok::ModelId> CppLogic::ModLoader::ModLoader::ModelsToRemove{};
bool CppLogic::ModLoader::ModLoader::ReloadModels = false;
std::vector<shok::GUITextureId> CppLogic::ModLoader::ModLoader::TexturesToRemove{};
std::vector<shok::GUITextureId> CppLogic::ModLoader::ModLoader::TexturesToReload{};
std::vector<shok::AnimationId> CppLogic::ModLoader::ModLoader::AnimsToRemove{};
std::vector<shok::AnimationId> CppLogic::ModLoader::ModLoader::AnimsToReload{};
std::vector<shok::UpgradeCategoryId> CppLogic::ModLoader::ModLoader::SettlerUCatsToRemove{};
std::vector<shok::UpgradeCategoryId> CppLogic::ModLoader::ModLoader::BuildingUCatsToRemove{};
bool CppLogic::ModLoader::ModLoader::ReloadWaterTypes = false;
std::vector<int> CppLogic::ModLoader::ModLoader::SelectionTexturesToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::SelectionTexturesToReload{};
std::vector<shok::TerrainTextureId> CppLogic::ModLoader::ModLoader::TerrainTexturesToRemove{};
std::vector<shok::TerrainTextureId> CppLogic::ModLoader::ModLoader::TerrainTexturesToReload{};
bool CppLogic::ModLoader::ModLoader::ReloadTerrainTypes = false;
std::vector<shok::ExperienceClass> CppLogic::ModLoader::ModLoader::ExperienceClassesToRemove{};
std::vector<shok::ExperienceClass> CppLogic::ModLoader::ModLoader::ExperienceClassesToReload{};
std::vector<shok::SoundId> CppLogic::ModLoader::ModLoader::SoundGroupsToRemove{};
std::vector<shok::AnimSetId> CppLogic::ModLoader::ModLoader::AnimSetsToRemove{};
std::vector<shok::AnimSetId> CppLogic::ModLoader::ModLoader::AnimSetsToReload{};
std::vector<int> CppLogic::ModLoader::ModLoader::DirectXEffectsToFree{};

int CppLogic::ModLoader::ModLoader::SetEntityTypeToReload(lua::State L)
{
	auto id = luaext::EState{ L }.CheckEnum<shok::EntityTypeId>(1);
	CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::Obj.OnIdLoaded(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::PreLoadTechnology(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = (*GGL::CGLGameLogic::GlobalObj)->TechManager;
	if ((*BB::CIDManagerEx::TechnologiesManager)->GetIdByName(t))
		throw lua::LuaException{ "tasklist already exists" };
	int id = (*BB::CIDManagerEx::TechnologiesManager)->GetIDByNameOrCreate(t);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadTechnology(lua::State L)
{
	int id = L.CheckInt(1);
	auto* m = (*GGL::CGLGameLogic::GlobalObj)->TechManager;
	if (id <= 0 || id >= static_cast<int>(m->Techs.size()))
		throw lua::LuaException("invalid id");
	m->FreeTech(static_cast<shok::TechnologyId>(id));
	m->LoadTech(static_cast<shok::TechnologyId>(id));
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddTechnology(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = (*GGL::CGLGameLogic::GlobalObj)->TechManager;
	int id = (*BB::CIDManagerEx::TechnologiesManager)->GetIdByName(t);
	if (id != 0 && id < static_cast<int>(m->Techs.size()))
		throw lua::LuaException{ "tasklist already exists" };
	id = (*BB::CIDManagerEx::TechnologiesManager)->GetIDByNameOrCreate(t);
	m->LoadTech(static_cast<shok::TechnologyId>(id));
	TechsToRemove.push_back(static_cast<shok::TechnologyId>(id));
	L.Push("Technologies");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::AddModel(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* mp = (*ED::CGlobalsBaseEx::GlobalObj)->ModelProps;
	if (mp->ModelIdManager->GetIdByName(t))
		throw lua::LuaException{ "model already exists" };
	int id = mp->ModelIdManager->GetIDByNameOrCreate(t);
	try {
		mp->LoadModelDataFromExtraFile(static_cast<shok::ModelId>(id));
		(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->LoadModel(static_cast<shok::ModelId>(id));
	}
	catch (const BB::CException& e) {
		char buff[201];
		e.CopyMessage(buff, 200);
		throw lua::LuaException{ buff };
	}
	ModelsToRemove.push_back(static_cast<shok::ModelId>(id));
	L.Push("Models");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadModel(lua::State L)
{
	int id = L.CheckInt(1);
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	if (!m->ModelManager.ModelIDManager->GetNameByID(id))
		throw lua::LuaException{ "invalid id" };
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->LoadModelDataFromExtraFile(static_cast<shok::ModelId>(id));
	m->FreeModel(static_cast<shok::ModelId>(id));
	m->LoadModel(static_cast<shok::ModelId>(id));
	ReloadModels = true;
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddGUITexture(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = EGUIX::TextureManager::GlobalObj();
	if (m->GetTextureIDNoAdd(t) != shok::GUITextureId::Invalid)
		throw lua::LuaException{ "texture already exists" };
	auto id = m->GetTextureID(t);
	m->GetTextureByID(id);
	TexturesToRemove.push_back(id);
	L.Push(static_cast<int>(id));
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadGUITexture(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = EGUIX::TextureManager::GlobalObj();
	auto id = m->GetTextureIDNoAdd(t);
	if (id == shok::GUITextureId::Invalid) {
		for (const char* tex : dump::GUITextures) {
			if (std::strcmp(tex, t) == 0) {
				id = m->GetTextureID(t);
				break;
			}
		}
		if (id == shok::GUITextureId::Invalid)
			throw lua::LuaException{ "texture does not exist" };
	}
	m->ReloadTexture(id);
	TexturesToReload.push_back(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddAnimation(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	if ((*BB::CIDManagerEx::AnimManager)->GetIdByName(t))
		throw lua::LuaException{ "animation already exists" };
	int id = (*BB::CIDManagerEx::AnimManager)->GetIDByNameOrCreate(t);
	shok::String s{ t };
	m->AnimManager.Load(id, &s);
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(static_cast<shok::AnimationId>(id));
	AnimsToRemove.push_back(static_cast<shok::AnimationId>(id));
	L.Push("Animations");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.PushValue(1);
		L.Push(id);
		L.SetTableRaw(-3);
	}
	// func exit cleans stack anyway, so no need to pop
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadAnimation(lua::State L)
{
	int id = L.CheckInt(1);
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	const char* n = (*BB::CIDManagerEx::AnimManager)->GetNameByID(id);
	if (!n)
		throw lua::LuaException{ "invalid animation" };
	m->FreeAnim(static_cast<shok::AnimationId>(id));
	shok::String s{ n };
	m->AnimManager.Load(id, &s);
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(static_cast<shok::AnimationId>(id));
	AnimsToReload.push_back(static_cast<shok::AnimationId>(id));
	return 0;
}

int CppLogic::ModLoader::ModLoader::PreLoadUpgradeCategory(lua::State L)
{
	const char* t = L.CheckString(1);
	if ((*BB::CIDManagerEx::UpgradeCategoryManager)->GetIdByName(t))
		throw lua::LuaException{ "upgradecategory already exists" };
	int id = (*BB::CIDManagerEx::UpgradeCategoryManager)->GetIDByNameOrCreate(t);
	L.Push(id);
	return 1;
}
int CppLogic::ModLoader::ModLoader::AddSettlerUpgradeCategory(lua::State l)
{
	luaext::EState L{ l };
	const char* t = L.CheckString(1);
	auto* sty = L.CheckEntityType(2);
	if (!sty->IsSettlerType())
		throw lua::LuaException{ "not a settler type" };
	int id = (*BB::CIDManagerEx::UpgradeCategoryManager)->GetIdByName(t);
	if (id != 0 && id < static_cast<int>((*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.size()))
		throw lua::LuaException{ "upgrade categoty already exists" };
	id = (*BB::CIDManagerEx::UpgradeCategoryManager)->GetIDByNameOrCreate(t);
	auto first = L.CheckEnum<shok::EntityTypeId>(2);
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.SaveVector();
		v.Vector.emplace_back(static_cast<shok::UpgradeCategoryId>(id), first);
	}
	auto ctype = first;
	while (ctype != shok::EntityTypeId::Invalid) {
		auto* cty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ctype);
		auto* sty = dynamic_cast<GGL::CGLSettlerProps*>(cty->LogicProps);
		if (sty == nullptr)
			throw lua::LuaException{ "non settler type in settler ucat" };
		sty->Upgrade.Category = static_cast<shok::UpgradeCategoryId>(id);
		ctype = sty->Upgrade.Type;
	}
	// players are not created yet, so the upgrademanagers in there read the correct data from GGL::CLogicProperties::GlobalObj
	SettlerUCatsToRemove.push_back(static_cast<shok::UpgradeCategoryId>(id));
	L.Push("UpgradeCategories");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::AddBuildingUpgradeCategory(lua::State l)
{
	luaext::EState L{ l };
	const char* t = L.CheckString(1);
	auto* sty = L.CheckEntityType(2);
	if (!sty->IsBuildingType())
		throw lua::LuaException{ "not a building type" };
	int id = (*BB::CIDManagerEx::UpgradeCategoryManager)->GetIdByName(t);
	if (id != 0 && id < static_cast<int>((*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.size()))
		throw lua::LuaException{ "upgrade categoty already exists" };
	id = (*BB::CIDManagerEx::UpgradeCategoryManager)->GetIDByNameOrCreate(t);
	auto first = L.CheckEnum<shok::EntityTypeId>(2);
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->BuildingUpgrades.SaveVector();
		v.Vector.emplace_back(static_cast<shok::UpgradeCategoryId>(id), first);
	}
	auto ctype = first;
	while (ctype != shok::EntityTypeId::Invalid) {
		auto* cty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ctype);
		auto* bty = dynamic_cast<GGL::CGLBuildingProps*>(cty->LogicProps);
		if (bty == nullptr)
			throw lua::LuaException{ "non building type in building ucat" };
		bty->Upgrade.Category = static_cast<shok::UpgradeCategoryId>(id);
		ctype = bty->Upgrade.Type;
	}
	// players are not created yet, so the upgrademanagers in there read the correct data from GGL::CLogicProperties::GlobalObj
	BuildingUCatsToRemove.push_back(static_cast<shok::UpgradeCategoryId>(id));
	L.Push("UpgradeCategories");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::AddWaterType(lua::State L)
{
	const char* t = L.CheckString(1);
	int id = L.OptInteger(2, 0);
	if ((*BB::CIDManagerEx::WaterTypeManager)->GetIdByName(t) != 0)
		throw lua::LuaException{ "water type already exists" };
	try {
		id = (*BB::CIDManagerEx::WaterTypeManager)->GetIDByNameOrCreate(t, id);
	}
	catch (const BB::CInvalidIDException& e) {
		char buff[201]{};
		e.CopyMessage(buff, 200);
		throw lua::LuaException{ buff };
	}
	(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->LoadWaterTypeFromExtraFile(static_cast<shok::WaterTypeId>(id));
	ReloadWaterTypes = true;
	L.Push("WaterTypes");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.PushValue(1);
		L.Push(id);
		L.SetTableRaw(-3);
	}
	// func exit cleans stack anyway, so no need to pop
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadWaterType(lua::State L)
{
	int id = L.CheckInt(1);
	if ((*BB::CIDManagerEx::WaterTypeManager)->GetNameByID(id) == nullptr)
		throw lua::LuaException{ "water type does not exists" };
	(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->LoadWaterTypeFromExtraFile(static_cast<shok::WaterTypeId>(id));
	ReloadWaterTypes = true;
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddSelectionTexture(lua::State L)
{
	const char* n = L.CheckString(1);
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures;
	if (mng->IdManager->GetIdByName(n) != 0)
		throw lua::LuaException{ "selection texture already exists" };
	auto d = mng->Get(n);
	SelectionTexturesToRemove.push_back(d->Id);
	d->Get();
	return 0;
}

int CppLogic::ModLoader::ModLoader::ReloadSelectionTexture(lua::State L)
{
	const char* n = L.CheckString(1);
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures;
	if (mng->IdManager->GetIdByName(n) == 0)
		throw lua::LuaException{ "selection texture does not exist" };
	auto d = mng->Get(n);
	SelectionTexturesToReload.push_back(d->Id);
	d->FreeCache();
	d->Get();
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddTerrainTexture(lua::State L)
{
	const char* n = L.CheckString(1);
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager;
	if (mng->DisplayProps->TerrainTextureManager->GetIdByName(n) != 0)
		throw lua::LuaException{ "terrain texture already exists" };
	int id = mng->DisplayProps->TerrainTextureManager->GetIDByNameOrCreate(n);
	mng->LoadTexture(static_cast<shok::TerrainTextureId>(id));
	TerrainTexturesToRemove.push_back(static_cast<shok::TerrainTextureId>(id));
	return 0;
}

int CppLogic::ModLoader::ModLoader::ReloadTerrainTexture(lua::State L)
{
	const char* n = L.CheckString(1);
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager;
	int id = mng->DisplayProps->TerrainTextureManager->GetIdByName(n);
	if (id == 0)
		throw lua::LuaException{ "terrain texture does not exists" };
	mng->LoadTexture(static_cast<shok::TerrainTextureId>(id));
	TerrainTexturesToReload.push_back(static_cast<shok::TerrainTextureId>(id));
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddTerrainType(lua::State L)
{
	const char* t = L.CheckString(1);
	int id = L.OptInteger(2, 0);
	if ((*BB::CIDManagerEx::TerrainTypeManager)->GetIdByName(t) != 0)
		throw lua::LuaException{ "terrain type already exists" };
	try {
		id = (*BB::CIDManagerEx::TerrainTypeManager)->GetIDByNameOrCreate(t, id);
	}
	catch (const BB::CInvalidIDException& e) {
		char buff[201]{};
		e.CopyMessage(buff, 200);
		throw lua::LuaException{ buff };
	}
	(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->LoadTerrainTypeFromExtraFile(static_cast<shok::TerrainTypeId>(id));
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyTerrainType(static_cast<shok::TerrainTypeId>(id));
	ReloadTerrainTypes = true;
	L.Push("TerrainTypes");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.PushValue(1);
		L.Push(id);
		L.SetTableRaw(-3);
	}
	// func exit cleans stack anyway, so no need to pop
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadTerrainType(lua::State L)
{
	int id = L.CheckInt(1);
	if ((*BB::CIDManagerEx::TerrainTypeManager)->GetNameByID(id) == nullptr)
		throw lua::LuaException{ "terrain type does not exists" };
	(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->LoadTerrainTypeFromExtraFile(static_cast<shok::TerrainTypeId>(id));
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyTerrainType(static_cast<shok::TerrainTypeId>(id));
	ReloadTerrainTypes = true;
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddExperienceClass(lua::State L)
{
	GGL::CEntityProfile::HookExperienceClassAssignment(true);
	const char* n = L.CheckString(1);
	shok::EntityCategory cat = static_cast<shok::EntityCategory>(L.CheckInt(2));
	auto* mng = GGL::ExperienceClassHolder::GlobalObj();
	for (auto* cl : mng->Classes) {
		if (std::strcmp(cl->Table.c_str(), n) == 0)
			throw lua::LuaException{ "already exists" };
	}
	shok::ExperienceClass id = mng->AddExperienceClass(n, cat);
	ExperienceClassesToRemove.push_back(id);
	L.Push("ExperienceClasses");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.PushValue(1);
		L.Push(static_cast<int>(id));
		L.SetTableRaw(-3);
	}
	// func exit cleans stack anyway, so no need to pop
	L.Push(static_cast<int>(id));
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadExperienceClass(lua::State L)
{
	int id = L.CheckInt(1);
	auto* mng = GGL::ExperienceClassHolder::GlobalObj();
	if (id <= 0 || id >= static_cast<int>(mng->Classes.size()))
		throw lua::LuaException{ "invalid experienceclass" };
	mng->LoadExperienceClass(static_cast<shok::ExperienceClass>(id));
	ExperienceClassesToReload.push_back(static_cast<shok::ExperienceClass>(id));
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddSounds(lua::State L)
{
	int num = L.GetTop();
	for (int i = 1; i <= num; ++i) {
		const char* n = L.CheckString(i);
		if ((*BB::CIDManagerEx::SoundsManager)->GetIdByName(n) != 0)
			throw lua::LuaException{ "sound already exists" };
	}
	auto* mng = *ESnd::CSoESound::GlobalObj;
	L.Push("Sounds");
	L.GetGlobal();
	int firstid;
	{
		const char* n = L.CheckString(1);
		firstid = mng->AddSoundToNewGroup(n);
		std::string s{ n };
		std::replace(s.begin(), s.end(), '\\', '_');
		L.Push(s);
		L.Push(firstid);
		L.SetTableRaw(-3);
	}
	for (int i = 2; i <= num; ++i) {
		const char* n = L.CheckString(i);
		int id = mng->AddSoundToLastGroup(n);
		std::string s{ n };
		std::replace(s.begin(), s.end(), '\\', '_');
		L.Push(s);
		L.Push(id);
		L.SetTableRaw(-3);
	}
	L.Push(firstid);
	SoundGroupsToRemove.push_back(static_cast<shok::SoundId>(firstid));
	return 1;
}

int CppLogic::ModLoader::ModLoader::AddAnimSet(lua::State L)
{
	const char* name = L.CheckString(1);
	auto* idm = *BB::CIDManagerEx::AnimSetManager;
	if (idm->GetIdByName(name))
		throw lua::LuaException{ "animset already exists" };
	auto* mng = *EGL::AnimSetManager::GlobalObj;
	int id = idm->GetIDByNameOrCreate(name);
	mng->LoadAnimSet(static_cast<shok::AnimSetId>(id));
	AnimSetsToRemove.push_back(static_cast<shok::AnimSetId>(id));
	L.Push("AnimSets");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
}
int CppLogic::ModLoader::ModLoader::ReloadAnimSet(lua::State L)
{
	int id = L.CheckInt(1);
	auto* mng = *EGL::AnimSetManager::GlobalObj;
	auto* idm = *BB::CIDManagerEx::AnimSetManager;
	if (idm->GetNameByID(id) == nullptr)
		throw lua::LuaException{ "anim set does not exists" };
	mng->FreeAnimSet(static_cast<shok::AnimSetId>(id));
	mng->LoadAnimSet(static_cast<shok::AnimSetId>(id));
	AnimSetsToReload.push_back(static_cast<shok::AnimSetId>(id));
	return 0;
}

int CppLogic::ModLoader::ModLoader::LoadDirectXEffect(lua::State L)
{
	const char* name = L.CheckString(1);
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->Effects;
	if (!mng->IdManager->GetIdByName(name))
		mng->IdManager->GetIDByNameOrCreate(name);
	auto* s = mng->Get(name);
	s->FreeCache();
	s->Get();
	DirectXEffectsToFree.push_back(s->Id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::RefreshEntityCategoryCache(lua::State L)
{
	EGL::EntityCategoryCache::RefreshCache(*EGL::CGLEEntitiesProps::GlobalObj);
	return 0;
}

int CppLogic::ModLoader::ModLoader::SanityCheck(lua::State L)
{
	for (auto* l : Loaders) {
		l->SanityCheck();
	}
	return 0;
}

void CppLogic::ModLoader::ModLoader::Log(lua::State L, const char* log)
{
	shok::LogString("ModLoader: %s\n", log);
	int t = L.GetTop();
	L.Push("LuaDebugger");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.Push("Log");
		L.GetTableRaw(-2);
		if (L.IsFunction(-1)) {
			L.PushFString("ModLoader: %s", log);
			L.PCall(1, 0);
		}
	}
	L.SetTop(t);
}

void CppLogic::ModLoader::ModLoader::Initialize()
{
	Framework::AGameModeBase::HookStartMap();
	Framework::AGameModeBase::PreStartMap = &PreMapStart;
	Framework::AGameModeBase::HookRemoveArchive();
	Framework::AGameModeBase::HookLoadSave();
	Framework::AGameModeBase::PreLoadSave = &PreSaveLoad;
	Initialized = true;
}

void CppLogic::ModLoader::ModLoader::Cleanup(Framework::CMain::NextMode n)
{
	if (((*Framework::CMain::GlobalObj)->CurrentMode != Framework::CMain::Mode::MainMenu)) {
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };

		Log(L, "Cleanup");
		int t = L.GetTop();
		AddLib(L);
		L.Push("ModLoader");
		L.GetGlobal();
		if (L.IsTable(-1)) {
			L.Push("Cleanup");
			L.GetTableRaw(-2);
			if (L.IsFunction(-1))
				L.PCall(0, 0);
			else
				L.Pop(1);
		}
		// no need to remove lib, state gets destroyed anyway
		L.SetTop(t);

		Log(L, "Removing extra archives");
		BB::CFileSystemMgr* fm = (*BB::CFileSystemMgr::GlobalObj);
		while (fm->LoadOrder[0] != BB::CFileSystemMgr::LoadorderTop) {
			fm->RemoveTopArchive();
		}
		Log(L, "Resetting data");

		// make sure there is no entity/effect left that can access a type we are deleting
		if (*EGL::CGLEEntityManager::GlobalObj) {
			CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity> p{ [](const EGL::CGLEEntity*, float*, int*) { return true; } };
			CppLogic::Iterator::GlobalEntityIterator it{ &p };
			auto* man = (*ED::CGlobalsLogicEx::GlobalObj)->VisibleEntityManager;
			for (EGL::CGLEEntity* a : it) {
				if (man->GetDisplayForEntity(a->EntityId))
					man->DestroyDisplayForEntity(a->EntityId);
				a->Destroy();
			}
			(*EGL::CGLEGameLogic::GlobalObj)->ClearToDestroy();
		}
		if (*EGL::CGLEEffectManager::GlobalObj) {
			CppLogic::Iterator::PredicateFunc<EGL::CEffect> p{ [](const EGL::CEffect*, float*, int*) { return true; } };
			CppLogic::Iterator::GlobalEffectIterator it{ &p };
			for (EGL::CEffect* a : it) {
				delete a;
			}
		}

		for (auto* l : Loaders) {
			l->Reset();
		}

		while (TechsToRemove.size() != 0) {
			auto id = TechsToRemove.back();
			TechsToRemove.pop_back();
			if (*GGL::CGLGameLogic::GlobalObj)
				(*GGL::CGLGameLogic::GlobalObj)->TechManager->PopTech(id);
			if ((*BB::CIDManagerEx::TechnologiesManager))
				(*BB::CIDManagerEx::TechnologiesManager)->RemoveID(static_cast<int>(id));
		}

		while (ModelsToRemove.size() != 0) {
			auto id = ModelsToRemove.back();
			ModelsToRemove.pop_back();
			(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->PopModel(id);
			(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->PopModel(id);
			(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ModelIdManager->RemoveID(static_cast<int>(id));
		}
		if (ReloadModels) {
			for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
				(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(static_cast<shok::ModelId>(id));
				//(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(id);
				// gets reloaded on next use anyway
			}
			(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ReloadAllModels();
		}
		ReloadModels = false;

		while (TexturesToRemove.size() != 0) {
			auto id = TexturesToRemove.back();
			TexturesToRemove.pop_back();
			EGUIX::TextureManager::GlobalObj()->FreeTexture(id);
			EGUIX::TextureManager::GlobalObj()->IdManager->RemoveID(static_cast<int>(id));
		}
		for (auto id : TexturesToReload) {
			if (static_cast<unsigned int>(id) < EGUIX::TextureManager::GlobalObj()->Textures.size())
				EGUIX::TextureManager::GlobalObj()->ReloadTexture(id);
		}
		TexturesToReload.clear();

		while (AnimsToRemove.size() != 0) {
			auto id = AnimsToRemove.back();
			AnimsToRemove.pop_back();
			(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeAnim(id);
			(*BB::CIDManagerEx::AnimManager)->RemoveID(static_cast<int>(id));
			(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->PopAnimPops(id);
		}
		for (auto id : AnimsToReload) {
			auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
			const char* n = (*BB::CIDManagerEx::AnimManager)->GetNameByID(static_cast<int>(id));
			if (n) {
				m->FreeAnim(id);
				shok::String s{ n };
				m->AnimManager.Load(static_cast<int>(id), &s);
				(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(id);
			}
		}
		AnimsToReload.clear();

		{
			auto v = (*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.SaveVector();
			while (SettlerUCatsToRemove.size() != 0) {
				auto id = SettlerUCatsToRemove.back();
				SettlerUCatsToRemove.pop_back();
				if (v.Vector.back().Category == id) {
					v.Vector.pop_back();
				}
				(*BB::CIDManagerEx::UpgradeCategoryManager)->RemoveID(static_cast<int>(id));
			}
		}
		{
			auto v = (*GGL::CLogicProperties::GlobalObj)->BuildingUpgrades.SaveVector();
			while (BuildingUCatsToRemove.size() != 0) {
				auto id = BuildingUCatsToRemove.back();
				BuildingUCatsToRemove.pop_back();
				if (v.Vector.back().Category == id) {
					v.Vector.pop_back();
				}
				(*BB::CIDManagerEx::UpgradeCategoryManager)->RemoveID(static_cast<int>(id));
			}
		}

		if (ReloadWaterTypes) {
			(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->ReloadWaterTypes();
			ReloadWaterTypes = false;
		}

		while (SelectionTexturesToRemove.size() != 0) {
			int id = SelectionTexturesToRemove.back();
			SelectionTexturesToRemove.pop_back();
			auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures;
			mng->PopId(id);
		}
		for (int id : SelectionTexturesToReload) {
			auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures;
			auto* d = mng->Get(id);
			d->FreeCache();
			d->Get();
		}
		SelectionTexturesToReload.clear();

		while (TerrainTexturesToRemove.size() != 0) {
			auto id = TerrainTexturesToRemove.back();
			TerrainTexturesToRemove.pop_back();
			(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->PopTexture(id);
			(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->TerrainTextureManager->RemoveID(static_cast<int>(id));
		}
		for (auto id : TerrainTexturesToReload) {
			(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->LoadTexture(id);
		}
		TerrainTexturesToReload.clear();

		if (ReloadTerrainTypes) {
			(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->ReloadTerrainTypes();
			(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyAllTerrainTypes();
			ReloadTerrainTypes = false;
		}

		while (ExperienceClassesToRemove.size() != 0) {
			shok::ExperienceClass id = ExperienceClassesToRemove.back();
			ExperienceClassesToRemove.pop_back();
			GGL::ExperienceClassHolder::GlobalObj()->PopExpeienceClass(id);
		}
		for (shok::ExperienceClass id : ExperienceClassesToReload) {
			GGL::ExperienceClassHolder::GlobalObj()->LoadExperienceClass(id);
		}
		ExperienceClassesToReload.clear();
		GGL::CEntityProfile::HookExperienceClassAssignment(false);

		while (SoundGroupsToRemove.size() != 0) {
			auto id = SoundGroupsToRemove.back();
			SoundGroupsToRemove.pop_back();
			(*ESnd::CSoESound::GlobalObj)->PopSoundGroup(static_cast<int>(id));
		}

		while (AnimSetsToRemove.size() != 0) {
			auto id = AnimSetsToRemove.back();
			AnimSetsToRemove.pop_back();
			(*EGL::AnimSetManager::GlobalObj)->PopAnimSet(id);
			(*BB::CIDManagerEx::AnimSetManager)->RemoveID(static_cast<int>(id));
		}
		for (auto id : AnimSetsToReload) {
			auto* mng = *EGL::AnimSetManager::GlobalObj;
			mng->FreeAnimSet(id);
			mng->LoadAnimSet(id);
		}
		AnimSetsToReload.clear();

		if (DirectXEffectsToFree.size() > 0) {
			for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
				(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(static_cast<shok::ModelId>(id));
				// gets reloaded on next use anyway
			}
			auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->Effects;
			while (DirectXEffectsToFree.size() > 0) {
				int id = DirectXEffectsToFree.back();
				DirectXEffectsToFree.pop_back();
				mng->Data[id]->FreeCache();
				if (id + 1 == static_cast<int>(mng->Data.size()))
					mng->PopId(id);
				else
					mng->IdManager->RemoveID(id);
			}
		}

		Log(L, "Done");
	}
}

bool CppLogic::ModLoader::ModLoader::IsInitialized()
{
	return Initialized;
}

void CppLogic::ModLoader::ModLoader::AddTaskListToRemove(shok::TaskListId id)
{
	CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TaskListId>::Obj.OnIdAllocated(id);
}

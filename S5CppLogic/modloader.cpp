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
	Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->GetCampagnInfo(d);
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

bool CppLogic::ModLoader::ModLoader::Initialized = false;
std::vector<int> CppLogic::ModLoader::ModLoader::EntityTypesToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::EntityTypesToReload{};
bool CppLogic::ModLoader::ModLoader::ReloadEffectTypes = false;
std::vector<int> CppLogic::ModLoader::ModLoader::TaskListsToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::TechsToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::ModelsToRemove{};
bool CppLogic::ModLoader::ModLoader::ReloadModels = false;
std::vector<int> CppLogic::ModLoader::ModLoader::TexturesToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::TexturesToReload{};
std::vector<int> CppLogic::ModLoader::ModLoader::AnimsToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::AnimsToReload{};
std::vector<int> CppLogic::ModLoader::ModLoader::SettlerUCatsToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::BuildingUCatsToRemove{};
bool CppLogic::ModLoader::ModLoader::ReloadWaterTypes = false;
std::vector<int> CppLogic::ModLoader::ModLoader::SelectionTexturesToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::SelectionTexturesToReload{};
std::vector<int> CppLogic::ModLoader::ModLoader::TerrainTexturesToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::TerrainTexturesToReload{};
bool CppLogic::ModLoader::ModLoader::ReloadTerrainTypes = false;
std::vector<shok::ExperienceClass> CppLogic::ModLoader::ModLoader::ExperienceClassesToRemove{};
std::vector<shok::ExperienceClass> CppLogic::ModLoader::ModLoader::ExperienceClassesToReload{};
std::vector<int> CppLogic::ModLoader::ModLoader::SoundGroupsToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::AnimSetsToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::AnimSetsToReload{};
std::vector<int> CppLogic::ModLoader::ModLoader::DirectXEffectsToFree{};

int CppLogic::ModLoader::ModLoader::PreLoadEntityType(lua::State L)
{
	const char* t = L.CheckString(1);
	if ((*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->GetIdByName(t))
		throw lua::LuaException{ "entitytype already exists" };
	int id = (*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->GetIDByNameOrCreate(t);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::AddEntityType(lua::State L)
{
	const char* t = L.CheckString(1);
	int id = (*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->GetIdByName(t);
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
	if (id != 0 && id < static_cast<int>(mng->EntityTypes.size()))
		throw lua::LuaException{ "entitytype already exists" };
	id = (*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->GetIDByNameOrCreate(t);
	mng->LoadEntityTypeByID(id);
	EntityTypesToRemove.push_back(id);
	L.Push("Entities");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadEntityType(lua::State L)
{
	int id = L.CheckInt(1);
	auto* m = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
	if (id <= 0 || id >= static_cast<int>(m->EntityTypes.size()))
		throw lua::LuaException("invalid id");
	m->FreeEntityType(id);
	m->LoadEntityTypeByID(id);
	EntityTypesToReload.push_back(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::SetEntityTypeToReload(lua::State L)
{
	int id = L.CheckInt(1);
	auto* m = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
	if (id <= 0 || id >= static_cast<int>(m->EntityTypes.size()))
		throw lua::LuaException("invalid id");
	EntityTypesToReload.push_back(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::PreLoadEffectType(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = (*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager;
	if (m->EffectTypeManager->GetIdByName(t))
		throw lua::LuaException{ "effecttype already exists" };
	int id = m->EffectTypeManager->GetIDByNameOrCreate(t);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadEffectType(lua::State L)
{
	int id = L.CheckInt(1);
	auto* m = (*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager;
	if (id <= 0 || id >= static_cast<int>(m->EffectsProps.Effects.size()))
		throw lua::LuaException("invalid id");
	m->FreeEffectType(id);
	m->LoadEffectTypeFromExtraFile(id);
	ReloadEffectTypes = true;
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddEffectType(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = (*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager;
	int id = m->EffectTypeManager->GetIdByName(t);
	if (id != 0 && id < static_cast<int>(m->EffectsProps.Effects.size()))
		throw lua::LuaException{ "effecttype already exists" };
	id = m->EffectTypeManager->GetIDByNameOrCreate(t);
	m->LoadEffectTypeFromExtraFile(id);
	L.Push("GGL_Effects");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	ReloadEffectTypes = true;
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::PreLoadTaskList(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = *EGL::CGLETaskListMgr::GlobalObj;
	if (m->TaskListManager->GetIdByName(t))
		throw lua::LuaException{ "tasklist already exists" };
	int id = m->TaskListManager->GetIDByNameOrCreate(t);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadTaskList(lua::State L)
{
	int id = L.CheckInt(1);
	auto* m = *EGL::CGLETaskListMgr::GlobalObj;
	if (id <= 0 || id >= static_cast<int>(m->TaskLists.size()))
		throw lua::LuaException("invalid id");
	m->FreeTaskList(id);
	m->LoadTaskList(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddTaskList(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = *EGL::CGLETaskListMgr::GlobalObj;
	int id = m->TaskListManager->GetIdByName(t);
	if (id != 0 && id < static_cast<int>(m->TaskLists.size()))
		throw lua::LuaException{ "tasklist already exists" };
	id = m->TaskListManager->GetIDByNameOrCreate(t);
	m->LoadTaskList(id);
	TaskListsToRemove.push_back(id);
	L.Push("TaskLists");
	L.GetGlobal();
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
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
	m->FreeTech(id);
	m->LoadTech(id);
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
	m->LoadTech(id);
	TechsToRemove.push_back(id);
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
	mp->LoadModelDataFromExtraFile(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->LoadModel(id);
	ModelsToRemove.push_back(id);
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
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->LoadModelDataFromExtraFile(id);
	m->FreeModel(id);
	m->LoadModel(id);
	ReloadModels = true;
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddGUITexture(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = EGUIX::TextureManager::GlobalObj();
	if (m->GetTextureIDNoAdd(t))
		throw lua::LuaException{ "texture already exists" };
	int id = m->GetTextureID(t);
	m->GetTextureByID(id);
	TexturesToRemove.push_back(id);
	L.Push(id);
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReloadGUITexture(lua::State L)
{
	const char* t = L.CheckString(1);
	auto* m = EGUIX::TextureManager::GlobalObj();
	int id = m->GetTextureIDNoAdd(t);
	if (id == 0) {
		for (const char* tex : dump::GUITextures) {
			if (std::strcmp(tex, t) == 0) {
				id = m->GetTextureID(t);
				break;
			}
		}
		if (id == 0)
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
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(id);
	AnimsToRemove.push_back(id);
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
	m->FreeAnim(id);
	shok::String s{ n };
	m->AnimManager.Load(id, &s);
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(id);
	AnimsToReload.push_back(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::AddSettlerUpgradeCategory(lua::State l)
{
	luaext::EState L{ l };
	const char* t = L.CheckString(1);
	auto* sty = L.CheckEntityType(2);
	if (!sty->IsSettlerType())
		throw lua::LuaException{ "not a settler type" };
	if ((*BB::CIDManagerEx::UpgradeCategoryManager)->GetIdByName(t) != 0)
		throw lua::LuaException{ "upgrade categoty already exists" };
	int id = (*BB::CIDManagerEx::UpgradeCategoryManager)->GetIDByNameOrCreate(t);
	int first = L.CheckInt(2);
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.SaveVector();
		v.Vector.emplace_back(id, first);
	}
	int ctype = first;
	while (ctype != 0) {
		auto* cty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ctype);
		auto* sty = dynamic_cast<GGL::CGLSettlerProps*>(cty->LogicProps);
		if (sty == nullptr)
			throw lua::LuaException{ "non settler type in settler ucat" };
		sty->Upgrade.Category = id;
		ctype = sty->Upgrade.Type;
	}
	// players are not created yet, so the upgrademanagers in there read the correct data from GGL::CLogicProperties::GlobalObj
	SettlerUCatsToRemove.push_back(id);
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
	if ((*BB::CIDManagerEx::UpgradeCategoryManager)->GetIdByName(t) != 0)
		throw lua::LuaException{ "upgrade categoty already exists" };
	int id = (*BB::CIDManagerEx::UpgradeCategoryManager)->GetIDByNameOrCreate(t);
	int first = L.CheckInt(2);
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->BuildingUpgrades.SaveVector();
		v.Vector.emplace_back(id, first);
	}
	int ctype = first;
	while (ctype != 0) {
		auto* cty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ctype);
		auto* bty = dynamic_cast<GGL::CGLBuildingProps*>(cty->LogicProps);
		if (bty == nullptr)
			throw lua::LuaException{ "non building type in building ucat" };
		bty->Upgrade.Category = id;
		ctype = bty->Upgrade.Type;
	}
	// players are not created yet, so the upgrademanagers in there read the correct data from GGL::CLogicProperties::GlobalObj
	BuildingUCatsToRemove.push_back(id);
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
	(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->LoadWaterTypeFromExtraFile(id);
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
	(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->LoadWaterTypeFromExtraFile(id);
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
	mng->LoadTexture(id);
	TerrainTexturesToRemove.push_back(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::ReloadTerrainTexture(lua::State L)
{
	const char* n = L.CheckString(1);
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager;
	int id = mng->DisplayProps->TerrainTextureManager->GetIdByName(n);
	if (id == 0)
		throw lua::LuaException{ "terrain texture does not exists" };
	mng->LoadTexture(id);
	TerrainTexturesToReload.push_back(id);
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
	(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->LoadTerrainTypeFromExtraFile(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyTerrainType(id);
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
	(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->LoadTerrainTypeFromExtraFile(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyTerrainType(id);
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
	SoundGroupsToRemove.push_back(firstid);
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
	mng->LoadAnimSet(id);
	AnimSetsToRemove.push_back(id);
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
	mng->FreeAnimSet(id);
	mng->LoadAnimSet(id);
	AnimSetsToReload.push_back(id);
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
	DirectXEffectsToFree.push_back(s->Id);
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
		}
		if (*EGL::CGLEEffectManager::GlobalObj) {
			CppLogic::Iterator::PredicateFunc<EGL::CEffect> p{ [](const EGL::CEffect*, float*, int*) { return true; } };
			CppLogic::Iterator::GlobalEffectIterator it{ &p };
			for (EGL::CEffect* a : it) {
				delete a;
			}
		}


		while (EntityTypesToRemove.size() != 0) {
			int id = EntityTypesToRemove.back();
			EntityTypesToRemove.pop_back();
			(*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager->PopEntityType(id);
			(*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->RemoveID(id);
		}

		for (int id : EntityTypesToReload) {
			auto* m = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
			if (id <= 0 || id >= static_cast<int>(m->EntityTypes.size())) // might got popped above
				continue;
			m->FreeEntityType(id);
			m->LoadEntityTypeByID(id);
		}
		EntityTypesToReload.clear();

		if (ReloadEffectTypes)
			(*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager->ReloadAllEffectTypes();
		ReloadEffectTypes = false;

		while (TaskListsToRemove.size() != 0) {
			int id = TaskListsToRemove.back();
			TaskListsToRemove.pop_back();
			if (*EGL::CGLETaskListMgr::GlobalObj)
				(*EGL::CGLETaskListMgr::GlobalObj)->RemoveTaskList(id);
			else
				(*BB::CIDManagerEx::TaskListManager)->RemoveID(id);
		}

		while (TechsToRemove.size() != 0) {
			int id = TechsToRemove.back();
			TechsToRemove.pop_back();
			if (*GGL::CGLGameLogic::GlobalObj)
				(*GGL::CGLGameLogic::GlobalObj)->TechManager->PopTech(id);
			if ((*BB::CIDManagerEx::TechnologiesManager))
				(*BB::CIDManagerEx::TechnologiesManager)->RemoveID(id);
		}

		while (ModelsToRemove.size() != 0) {
			int id = ModelsToRemove.back();
			ModelsToRemove.pop_back();
			(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->PopModel(id);
			(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->PopModel(id);
			(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ModelIdManager->RemoveID(id);
		}
		if (ReloadModels) {
			for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
				(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(id);
				//(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(id);
				// gets reloaded on next use anyway
			}
			(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ReloadAllModels();
		}
		ReloadModels = false;

		while (TexturesToRemove.size() != 0) {
			int id = TexturesToRemove.back();
			TexturesToRemove.pop_back();
			EGUIX::TextureManager::GlobalObj()->FreeTexture(id);
			EGUIX::TextureManager::GlobalObj()->IdManager->RemoveID(id);
		}
		for (int id : TexturesToReload) {
			if (static_cast<unsigned int>(id) < EGUIX::TextureManager::GlobalObj()->Textures.size())
				EGUIX::TextureManager::GlobalObj()->ReloadTexture(id);
		}
		TexturesToReload.clear();

		while (AnimsToRemove.size() != 0) {
			int id = AnimsToRemove.back();
			AnimsToRemove.pop_back();
			(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeAnim(id);
			(*BB::CIDManagerEx::AnimManager)->RemoveID(id);
			(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->PopAnimPops(id);
		}
		for (int id : AnimsToReload) {
			auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
			const char* n = (*BB::CIDManagerEx::AnimManager)->GetNameByID(id);
			if (n) {
				m->FreeAnim(id);
				shok::String s{ n };
				m->AnimManager.Load(id, &s);
				(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(id);
			}
		}
		AnimsToReload.clear();

		{
			auto v = (*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.SaveVector();
			while (SettlerUCatsToRemove.size() != 0) {
				int id = SettlerUCatsToRemove.back();
				SettlerUCatsToRemove.pop_back();
				if (v.Vector.back().Category == id) {
					v.Vector.pop_back();
				}
				(*BB::CIDManagerEx::UpgradeCategoryManager)->RemoveID(id);
			}
		}
		{
			auto v = (*GGL::CLogicProperties::GlobalObj)->BuildingUpgrades.SaveVector();
			while (BuildingUCatsToRemove.size() != 0) {
				int id = BuildingUCatsToRemove.back();
				BuildingUCatsToRemove.pop_back();
				if (v.Vector.back().Category == id) {
					v.Vector.pop_back();
				}
				(*BB::CIDManagerEx::UpgradeCategoryManager)->RemoveID(id);
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
			int id = TerrainTexturesToRemove.back();
			TerrainTexturesToRemove.pop_back();
			(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->PopTexture(id);
			(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->TerrainTextureManager->RemoveID(id);
		}
		for (int id : TerrainTexturesToReload) {
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
			int id = SoundGroupsToRemove.back();
			SoundGroupsToRemove.pop_back();
			(*ESnd::CSoESound::GlobalObj)->PopSoundGroup(id);
		}

		while (AnimSetsToRemove.size() != 0) {
			int id = AnimSetsToRemove.back();
			AnimSetsToRemove.pop_back();
			(*EGL::AnimSetManager::GlobalObj)->PopAnimSet(id);
			(*BB::CIDManagerEx::AnimSetManager)->RemoveID(id);
		}
		for (int id : AnimSetsToReload) {
			auto* mng = *EGL::AnimSetManager::GlobalObj;
			mng->FreeAnimSet(id);
			mng->LoadAnimSet(id);
		}
		AnimSetsToReload.clear();

		if (DirectXEffectsToFree.size() > 0) {
			for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
				(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(id);
				// gets reloaded on next use anyway
			}
			auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->Effects;
			for (int id : DirectXEffectsToFree) {
				mng->Data[id]->FreeCache();
			}
			DirectXEffectsToFree.clear();
		}

		Log(L, "Done");
	}
}

bool CppLogic::ModLoader::ModLoader::IsInitialized()
{
	return Initialized;
}

void CppLogic::ModLoader::ModLoader::AddTaskListToRemove(int id)
{
	TaskListsToRemove.push_back(id);
}

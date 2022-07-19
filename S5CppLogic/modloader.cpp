#include "pch.h"
#include "modloader.h"
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
#include "entityiterator.h"
#include "luaext.h"

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
	L.GetTableRaw(L.GLOBALSINDEX);
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
	Framework::SaveData* sdata = Framework::SaveData::GlobalObj();
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
	L.GetTableRaw(L.GLOBALSINDEX);
	L.Push("ModLoader");
	L.NewTable();

	L.RegisterFuncs(LuaFuncs, -3);

	L.SetTableRaw(-3);
	L.Pop(1);
}
void CppLogic::ModLoader::ModLoader::RemoveLib(lua::State L)
{
	L.Push("CppLogic");
	L.GetTableRaw(L.GLOBALSINDEX);
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

int CppLogic::ModLoader::ModLoader::AddEntityType(lua::State L)
{
	const char* t = L.CheckString(1);
	if ((*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->GetIdByName(t))
		throw lua::LuaException{ "entitytype already exists" };
	int id = (*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->GetIDByNameOrCreate(t);
	(*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager->LoadEntityTypeByID(id);
	EntityTypesToRemove.push_back(id);
	L.Push("Entities");
	L.GetTableRaw(L.GLOBALSINDEX);
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
	if (id <= 0 || id >= static_cast<int>(m->CGLEEntitiesProps.EntityTypes.size()))
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
	if (id <= 0 || id >= static_cast<int>(m->CGLEEntitiesProps.EntityTypes.size()))
		throw lua::LuaException("invalid id");
	EntityTypesToReload.push_back(id);
	return 0;
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
	if (m->EffectTypeManager->GetIdByName(t))
		throw lua::LuaException{ "effecttype already exists" };
	int id = m->EffectTypeManager->GetIDByNameOrCreate(t);
	m->LoadEffectTypeFromExtraFile(id);
	L.Push("GGL_Effects");
	L.GetTableRaw(L.GLOBALSINDEX);
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	ReloadEffectTypes = true;
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
	if (m->TaskListManager->GetIdByName(t))
		throw lua::LuaException{ "tasklist already exists" };
	int id = m->TaskListManager->GetIDByNameOrCreate(t);
	m->LoadTaskList(id);
	TaskListsToRemove.push_back(id);
	L.Push("TaskLists");
	L.GetTableRaw(L.GLOBALSINDEX);
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
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
	if ((*BB::CIDManagerEx::TechnologiesManager)->GetIdByName(t))
		throw lua::LuaException{ "tasklist already exists" };
	int id = (*BB::CIDManagerEx::TechnologiesManager)->GetIDByNameOrCreate(t);
	m->LoadTech(id);
	TechsToRemove.push_back(id);
	L.Push("Technologies");
	L.GetTableRaw(L.GLOBALSINDEX);
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
	(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->GetModelData(id);
	ModelsToRemove.push_back(id);
	L.Push("Models");
	L.GetTableRaw(L.GLOBALSINDEX);
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
	m->GetModelData(id);
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
	if (id == 0)
		throw lua::LuaException{ "texture does not exist" };
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
	AnimsToRemove.push_back(id);
	L.Push("Animations");
	L.GetTableRaw(L.GLOBALSINDEX);
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
	L.GetTableRaw(L.GLOBALSINDEX);
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
	L.GetTableRaw(L.GLOBALSINDEX);
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
	L.Push(id);
	return 1;
}

void CppLogic::ModLoader::ModLoader::Log(lua::State L, const char* log)
{
	shok::LogString("ModLoader: %s\n", log);
	int t = L.GetTop();
	L.Push("LuaDebugger");
	L.GetTableRaw(L.GLOBALSINDEX);
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

BB::IFileSystem* loadordertop = nullptr;
void CppLogic::ModLoader::ModLoader::Initialize()
{
	Framework::AGameModeBase::HookStartMap();
	Framework::AGameModeBase::PreStartMap = &PreMapStart;
	Framework::AGameModeBase::HookRemoveArchive();
	Framework::AGameModeBase::HookLoadSave();
	Framework::AGameModeBase::PreLoadSave = &PreSaveLoad;
	loadordertop = (*BB::CFileSystemMgr::GlobalObj)->LoadOrder[0];
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
		L.GetTableRaw(L.GLOBALSINDEX);
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
		while (fm->LoadOrder[0] != loadordertop) {
			fm->RemoveTopArchive();
		}
		Log(L, "Done");

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
			if (id <= 0 || id >= static_cast<int>(m->CGLEEntitiesProps.EntityTypes.size())) // might got popped above
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
		}
		for (int id : AnimsToReload) {
			auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
			const char* n = (*BB::CIDManagerEx::AnimManager)->GetNameByID(id);
			if (n) {
				m->FreeAnim(id);
				shok::String s{ n };
				m->AnimManager.Load(id, &s);
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

#include "pch.h"
#include "modloader.h"
#include "s5_filesystem.h"
#include "s5_glue.h"
#include "s5_entitytype.h"
#include "s5_idmanager.h"
#include "s5_entityandeffectmanager.h"
#include "s5_tasklist.h"
#include "s5_maplogic.h"
#include "s5_tech.h"
#include "entityiterator.h"

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
	L.Push();
	L.SetTableRaw(-3);
	L.Pop(1);
}

bool CppLogic::ModLoader::ModLoader::Initialized = false;
std::vector<int> CppLogic::ModLoader::ModLoader::EntityTypesToRemove{};
std::vector<int> CppLogic::ModLoader::ModLoader::EntityTypesToReload{};
bool CppLogic::ModLoader::ModLoader::ReloadEffectTypes = false;
std::vector<int> CppLogic::ModLoader::ModLoader::TaskListsToRemove{};

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
	return 0;
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
	return 0;
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
	return 0;
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
		L.SetTop(t);

		Log(L, "Removing extra archives");
		BB::CFileSystemMgr* fm = (*BB::CFileSystemMgr::GlobalObj);
		while (fm->LoadOrder[0] != loadordertop) {
			fm->RemoveTopArchive();
		}
		Log(L, "Done");
	}

	if (n != Framework::CMain::NextMode::ToMainMenu) {
		if (*EGL::CGLEEntityManager::GlobalObj) { // make sure there is no entity left that can access an entitytype we are deleting
			CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity> p{ [](const EGL::CGLEEntity*, float*, int*) { return true; } };
			CppLogic::Iterator::GlobalEntityIterator it{ &p };
			for (EGL::CGLEEntity* a : it) {
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

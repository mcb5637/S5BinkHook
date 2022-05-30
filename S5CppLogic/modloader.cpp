#include "pch.h"
#include "modloader.h"
#include "s5_filesystem.h"
#include "s5_glue.h"
#include "s5_entitytype.h"
#include "s5_idmanager.h"

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

int CppLogic::ModLoader::ModLoader::AddEntityType(lua::State L)
{
	const char* t = L.CheckString(1);
	int id = (*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->GetIDByNameOrCreate(t);
	(*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager->LoadEntityTypeByID(id);
	L.Push("Entities");
	L.GetTableRaw(L.GLOBALSINDEX);
	L.PushValue(1);
	L.Push(id);
	L.SetTableRaw(-3);
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
}

void CppLogic::ModLoader::ModLoader::Cleanup(lua::State L)
{
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

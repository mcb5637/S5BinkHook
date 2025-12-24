#include "pch.h"
#include "modloader.h"

#include <regex>
#include <filesystem>

#include <shok/globals/s5_filesystem.h>
#include <shok/globals/s5_glue.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_entityandeffectmanager.h>
#include <shok/entity/s5_tasklist.h>
#include <shok/globals/s5_maplogic.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/tech/s5_tech.h>
#include <shok/ui/s5_widget.h>
#include <shok/entity/s5_entitydisplay.h>
#include <shok/globals/s5_config.h>
#include <shok/s5_exception.h>
#include <shok/sound/s5_sound.h>
#include <shok/events/s5_netevents.h>
#include <utility/entityiterator.h>
#include <luaext.h>
#include <utility/dump_guitextures.h>
#include <utility/luaserializer.h>
#include <utility/ModConfig.h>
#include <utility/StringUtility.h>
#include <utility/WinAPIUtil.h>
#include <utility/savegame_extra.h>

#ifdef DEBUG_FUNCS
#define RESERIALIZE_NO_DIALOG
#endif

shok::String CppLogic::ModLoader::ModLoader::ModPackList{};
size_t CppLogic::ModLoader::ModLoader::GUIDLength = 0;

void CppLogic::ModLoader::ModLoader::Init(luaext::State L, const char* mappath, std::string_view func, const std::function<void(luaext::State)>& pushMapInfo)
{
	Log(L, "Initializing ModLoader");
	InitExtraECats();

	auto logpath = StringUtil::ANSIToUTF8(mappath);
	Log(L, logpath.c_str());
	if (!BB::CFileSystemMgr::DoesFileExist(mappath)) {
		Log(L, "no ModLoader.lua, use default instead");
		mappath = "data/maps/default/ModLoader.lua";
	}
	int t = L.GetTop();
	EScr::CScriptTriggerSystem::LoadFileToLuaState(L.GetState(), mappath);
	AddLib(L);
	bool keeparchive = false;
	L.Push("ModLoader");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.Push("MapInfo");
		pushMapInfo(L);
		L.SetTableRaw(-3);
		L.Push("Initialize");
		L.GetTableRaw(-2);
		if (L.IsFunction(-1)) {
			if (L.PCall(0, 0) != lua::ErrorCode::Success) {
				auto s = std::format("Initialize error: {}", L.ToDebugString(-1));
				Log(L, s.c_str());
				L.Pop(1);
			}
		}
		else {
			L.Pop(1);
		}
		L.Push("KeepArchive");
		L.GetTableRaw(-2);
		keeparchive = L.ToBoolean(-1);
		L.Pop(1);
		L.Push(func);
		L.GetTableRaw(-2);
		if (L.IsFunction(-1)) {
			if (L.PCall(0, 0) != lua::ErrorCode::Success) {
				auto s = std::format("{} error: {}", func, L.ToDebugString(-1));
				Log(L, s.c_str());
				L.Pop(1);
			}
		}
		else {
			L.Pop(1);
		}
	}
	Framework::AGameModeBase::DoNotRemoveNextArchive = keeparchive;
	RemoveLib(L);
	L.SetTop(t);
	Log(L, "Done");
}

void CppLogic::ModLoader::ModLoader::PreMapStart(lua_State* ingame, const char* name, const char* path, bool externalmap)
{
	luaext::State L{ ingame };
	auto mappath = std::format("{}\\{}\\ModLoader.lua", externalmap ? "Maps" : path, externalmap ? "ExternalMap" : name);
	Init(L, mappath.c_str(), "MapStart", [](luaext::State l) {
		const GS3DTools::CMapData& m = (*Framework::CMain::GlobalObj)->CurrentMap;
		l.NewTable();
		l.Push("MapName");
		l.Push(m.MapName);
		l.SetTableRaw(-3);
		l.Push("MapType");
		l.Push(static_cast<int>(m.MapType));
		l.SetTableRaw(-3);
		l.Push("MapCampagnName");
		l.Push(m.MapCampagnName);
		l.SetTableRaw(-3);
		l.Push("MapGUID");
		l.Push(m.MapGUID);
		l.SetTableRaw(-3);
		l.Push("IsSavegame");
		l.Push(false);
		l.SetTableRaw(-3);
	});
}
void CppLogic::ModLoader::ModLoader::PostMapscriptLoaded()
{
	luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
	int t = L.GetTop();

	Log(L, "ModLoader on Mapscript loaded");
	L.Push("ModLoader");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.Push("LoadModScripts");
		L.GetTableRaw(-2);
		if (L.IsFunction(-1)) {
			if (L.PCall(0, 0) != lua::ErrorCode::Success) {
				auto s = std::format("LoadModScripts error: {}", L.ToDebugString(-1));
				Log(L, s.c_str());
				L.Pop(1);
			}
		}
		else {
			L.Pop(1);
		}
	}
	Log(L, "Done");

	L.SetTop(t);
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
		throw std::logic_error("no save???");
	luaext::State L{ ingame };
	auto mappath = std::format("{}\\ModLoader.lua", externalmap ? "Maps\\ExternalMap" : internalmap_getpath(&sdata->CurrentSave->MapData));
	Init(L, mappath.c_str(), "LoadSave", [&](luaext::State l) {
		l.NewTable();
		const GS3DTools::CMapData& m = sdata->CurrentSave->MapData;
		l.Push("MapName");
		l.Push(m.MapName);
		l.SetTableRaw(-3);
		l.Push("MapType");
		l.Push(static_cast<int>(m.MapType));
		l.SetTableRaw(-3);
		l.Push("MapCampagnName");
		l.Push(m.MapCampagnName);
		l.SetTableRaw(-3);
		l.Push("MapGUID");
		l.Push(m.MapGUID);
		l.SetTableRaw(-3);
		l.Push("IsSavegame");
		l.Push(true);
		l.SetTableRaw(-3);
		l.Push("SaveLoading");
		l.Push(data->Folder);
		l.SetTableRaw(-3);
	});
}
void CppLogic::ModLoader::ModLoader::AddLib(luaext::State L)
{
	L.Push("CppLogic");
	L.GetGlobal();
	L.Push(ModLoaderLib);
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
void CppLogic::ModLoader::ModLoader::RemoveLib(luaext::State L)
{
	L.Push("CppLogic");
	L.GetGlobal();
	L.Push(ModLoaderLib);
	L.NewTable();


	for (auto l : LoadersIngame)
		l->RegisterFuncs(L);

	L.RegisterFuncs(NoLoaderFuncs, -3);

	L.SetTableRaw(-3);
	L.Pop(1);
}
void CppLogic::ModLoader::ModLoader::PreSave(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name)
{
	GUIDLength = mapdata->MapGUID.size();
	mapdata->MapGUID.append(ModPackList);
}
void CppLogic::ModLoader::ModLoader::PostSave(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name)
{
	mapdata->MapGUID.resize(GUIDLength);


}

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityTypeId>::Load(shok::EntityTypeId id, luaext::State L) {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
	if (static_cast<int>(id) < static_cast<int>(mng->CGLEEntitiesProps.EntityTypesLogicProps.size())) {
		mng->FreeEntityType(id);
	}
	mng->LoadEntityTypeByID(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityTypeId>::TableName() {
	return "Entities";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityTypeId>::FuncName() {
	return "EntityType";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::SanityCheck() {
	auto idm = CppLogic::GetIdManager<shok::EntityTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		auto et = CppLogic::GetEntityType(id);
		if (et == nullptr)
			throw lua::LuaException{ std::format("entitytype {}={} missing", n, static_cast<int>(id)) };
		if (et->LogicProps == nullptr)
			throw lua::LuaException{ std::format("entitytype {}={} missing LogicProps", n, static_cast<int>(id)) };
		if (et->DisplayProps == nullptr)
			throw lua::LuaException{ std::format("entitytype {}={} missing DisplayProps", n, static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::UnLoad(shok::EntityTypeId id) {
	(*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager->PopEntityType(id);
	(*EGL::CGLEEntitiesProps::GlobalObj)->EntityTypeManager->RemoveID(static_cast<int>(id));
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EffectTypeId>::Load(shok::EffectTypeId id, luaext::State L) {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager;
	if (static_cast<int>(id) < static_cast<int>(mng->EffectsProps.Effects.size()))
		mng->FreeEffectType(id);
	mng->LoadEffectTypeFromExtraFile(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EffectTypeId>::TableName() {
	return "GGL_Effects";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EffectTypeId>::FuncName() {
	return "EffectType";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::SanityCheck() {
	auto idm = CppLogic::GetIdManager<shok::EffectTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (CppLogic::GetEffectType(id)->Logic == nullptr)
			throw lua::LuaException{ std::format("effecttype {}={} missing Logic", n, static_cast<int>(id)) };
		if (CppLogic::GetEffectType(id)->Display == nullptr)
			throw lua::LuaException{ std::format("effecttype {}={} missing Display", n, static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::Reset() {
	if (NeedsReload)
		(*Framework::CMain::GlobalObj)->GluePropsManager->EffectPropsManager->ReloadAllEffectTypes();
	NeedsReload = false;
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TaskListId>::Load(shok::TaskListId id, luaext::State L) {
	auto* m = *EGL::CGLETaskListMgr::GlobalObj;
	if (static_cast<int>(id) < static_cast<int>(m->TaskLists.size()))
		m->FreeTaskList(id);
	m->LoadTaskList(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TaskListId>::TableName() {
	return "TaskLists";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TaskListId>::FuncName() {
	return "TaskList";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::SanityCheck() {
	auto idm = CppLogic::GetIdManager<shok::TaskListId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (CppLogic::GetTaskList(id) == nullptr)
			throw lua::LuaException{ std::format("tasklist {}={} missing", n, static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Reload() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::UnLoad(shok::TaskListId id) {
	if (*EGL::CGLETaskListMgr::GlobalObj)
		(*EGL::CGLETaskListMgr::GlobalObj)->RemoveTaskList(id);
	else
		CppLogic::GetIdManager<shok::TaskListId>().RemoveID(id);
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ArmorClassId>::Load(shok::ArmorClassId id, luaext::State L) {

}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ArmorClassId>::TableName() {
	return "ArmorClasses";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ArmorClassId>::FuncName() {
	return "ArmorClass";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::SanityCheck() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::Reload() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::UnLoad(shok::ArmorClassId id) {
	CppLogic::GetIdManager<shok::ArmorClassId>().RemoveID(id);
}
template<>
constexpr bool CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::RegisterFuncLoad = false;
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::Obj{};


template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::DamageClassId>::Load(shok::DamageClassId id, luaext::State L) {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager;
	L.PushValue(2);
	void* o = CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(L, nullptr, nullptr, shok::ClassId::Invalid, {
		GGL::CDamageClassProps::Identifier,
		CppLogic::Mod::Config::DamageClassExt::Identifier,
		});
	mng->AddDamageClass(id, static_cast<GGL::CDamageClassProps*>(o));
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::DamageClassId>::TableName() {
	return "DamageClasses";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::DamageClassId>::FuncName() {
	return "DamageClass";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::SanityCheck() {
	auto idm = CppLogic::GetIdManager<shok::DamageClassId>();
	auto acmng = CppLogic::GetIdManager<shok::ArmorClassId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		auto* dc = CppLogic::GetDamageClass(id);
		if (dc == nullptr)
			throw lua::LuaException{ std::format("damageclass {}={} is missing", n, static_cast<int>(id)) };
		for (auto ac : acmng) {
			if (dc->GetBonusVsArmorClass(ac) < 0)
				throw lua::LuaException{ std::format("damageclass {}={} has invalid factor {}", n, static_cast<int>(id), static_cast<int>(ac)) };
		}
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Reset() {
	if (NeedsReload)
		(*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager->ResetDamageClasses();
	NeedsReload = false;
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TechnologyId>::Load(shok::TechnologyId id, luaext::State L) {
	auto* m = (*GGL::CGLGameLogic::GlobalObj)->TechManager;
	if (static_cast<int>(id) < static_cast<int>(m->Techs.size()))
		m->FreeTech(id);
	m->LoadTech(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TechnologyId>::TableName() {
	return "Technologies";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TechnologyId>::FuncName() {
	return "Technology";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::SanityCheck() {
	auto idm = CppLogic::GetIdManager<shok::TechnologyId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (CppLogic::GetTechnology(id) == nullptr)
			throw lua::LuaException{ std::format("tech {}={} missing", n, static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::Reload() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::UnLoad(shok::TechnologyId id) {
	if (*GGL::CGLGameLogic::GlobalObj)
		(*GGL::CGLGameLogic::GlobalObj)->TechManager->PopTech(id);
	if ((*BB::CIDManagerEx::TechnologiesManager))
		(*BB::CIDManagerEx::TechnologiesManager)->RemoveID(static_cast<int>(id));
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ModelId>::Load(shok::ModelId id, luaext::State L) {
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->LoadModelDataFromExtraFile(id);
	if (static_cast<int>(id) < static_cast<int>(m->ModelManager.Models.size()))
		m->FreeModel(id);
	m->LoadModel(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ModelId>::TableName() {
	return "Models";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ModelId>::FuncName() {
	return "Model";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::SanityCheck() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::Reload() {
	for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
		(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(static_cast<shok::ModelId>(id));
	}
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ReloadAllModels();
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::UnLoad(shok::ModelId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->PopModel(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->PopModel(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ModelIdManager->RemoveID(static_cast<int>(id));
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::GUITextureId>::Load(shok::GUITextureId id, luaext::State L) {
	auto* mng = EGUIX::TextureManager::GlobalObj();
	mng->ReloadTexture(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::GUITextureId>::TableName() {
	return nullptr;
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::GUITextureId>::FuncName() {
	return "GUITexture";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::SanityCheck() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::UnLoad(shok::GUITextureId id) {
	EGUIX::TextureManager::GlobalObj()->FreeTexture(id);
	EGUIX::TextureManager::GlobalObj()->IdManager->RemoveID(static_cast<int>(id));
}
template<>
constexpr bool CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::RegisterFuncPreLoad = false;
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimationId>::Load(shok::AnimationId id, luaext::State L) {
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	if (m->AnimManager.Map.find(static_cast<int>(id)) != m->AnimManager.Map.end())
		m->FreeAnim(id);
	shok::String s{ CppLogic::GetIdManager<shok::AnimationId>().GetNameByID(id) };
	m->AnimManager.Load(static_cast<int>(id), &s);
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(static_cast<shok::AnimationId>(id));
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimationId>::TableName() {
	return "Animations";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimationId>::FuncName() {
	return "Animation";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId>::SanityCheck() {
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	auto idm = CppLogic::GetIdManager<shok::AnimationId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (m->AnimManager.Map.find(static_cast<int>(id)) == m->AnimManager.Map.end())
			throw lua::LuaException{ std::format("anim {}={} missing", n, static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId>::UnLoad(shok::AnimationId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeAnim(id);
	CppLogic::GetIdManager<shok::AnimationId>().RemoveID(id);
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->PopAnimPops(id);
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::UpgradeCategoryId>::Load(shok::UpgradeCategoryId id, luaext::State L) {

}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::UpgradeCategoryId>::TableName() {
	return "UpgradeCategories";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::UpgradeCategoryId>::FuncName() {
	return nullptr;
}
void CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::Reset()
{
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.SaveVector();
		while (!ToRemoveSettler.empty()) {
			auto id = ToRemoveSettler.back();
			ToRemoveSettler.pop_back();
			if (v.Vector.back().Category == id) {
				v.Vector.pop_back();
			}
			CppLogic::GetIdManager<shok::UpgradeCategoryId>().RemoveID(id);
		}
	}
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->BuildingUpgrades.SaveVector();
		while (!ToRemoveBuilding.empty()) {
			auto id = ToRemoveBuilding.back();
			ToRemoveBuilding.pop_back();
			if (v.Vector.back().Category == id) {
				v.Vector.pop_back();
			}
			CppLogic::GetIdManager<shok::UpgradeCategoryId>().RemoveID(id);
		}
	}
}
void CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::SanityCheck()
{
	static auto find = [](const auto& v, shok::UpgradeCategoryId id) -> bool {
		for (const auto& u : v)
			if (u.Category == id)
				return true;
		return false;
		};
	auto* m = *GGL::CLogicProperties::GlobalObj;
	auto idm = CppLogic::GetIdManager<shok::UpgradeCategoryId>();
	std::array<std::string_view, 2> toignore{
		"Bridge3",
		"Bridge4",
	};
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (std::find(toignore.begin(), toignore.end(), n) != toignore.end())
			continue;
		if (!find(m->BuildingUpgrades, id) && !find(m->SettlerUpgrades, id))
			throw lua::LuaException{ std::format("upgrade category {}={} missing", n, static_cast<int>(id)) };
	}
}
void CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::RegisterFuncs(luaext::State L)
{
	L.Push("PreLoadUpgradeCategory");
	L.Push<Prel>();
	L.SetTableRaw(-3);
	L.Push("AddSettlerUpgradeCategory");
	L.Push<AddSettlerUpgradeCategory>();
	L.SetTableRaw(-3);
	L.Push("AddBuildingUpgradeCategory");
	L.Push<AddBuildingUpgradeCategory>();
	L.SetTableRaw(-3);
}
void CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::OnIdAllocated(shok::UpgradeCategoryId id)
{
}
void CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::OnIdLoaded(shok::UpgradeCategoryId id)
{
}
int CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::Prel(luaext::State L)
{
	return Obj.Preload(luaext::State{ L });
}
int CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::AddSettlerUpgradeCategory(luaext::State l)
{
	luaext::State L{ l };
	shok::UpgradeCategoryId id = Obj.GetId(L, 1);
	auto* sty = L.CheckEntityType(2);
	if (!sty->IsSettlerType())
		throw lua::LuaException{ "not a settler type" };
	auto first = L.CheckEnum<shok::EntityTypeId>(2);
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.SaveVector();
		for (const auto& u : v.Vector) {
			if (u.Category == id)
				throw lua::LuaException{ "upgrade category already exists" };
		}
		v.Vector.emplace_back(id, first);
	}
	auto ctype = first;
	while (ctype != shok::EntityTypeId::Invalid) {
		auto* cty = CppLogic::GetEntityType(ctype);
		auto* sprops = dynamic_cast<GGL::CGLSettlerProps*>(cty->LogicProps);
		if (sprops == nullptr)
			throw lua::LuaException{ "non settler type in settler ucat" };
		sprops->Upgrade.Category = id;
		ctype = sprops->Upgrade.Type;
	}
	// players are not created yet, so the upgrademanagers in there read the correct data from GGL::CLogicProperties::GlobalObj
	Obj.ToRemoveSettler.push_back(id);
	L.Push(id);
	return 1;
}
int CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::AddBuildingUpgradeCategory(luaext::State l)
{
	luaext::State L{ l };
	shok::UpgradeCategoryId id = Obj.GetId(L, 1);
	auto* sty = L.CheckEntityType(2);
	if (!sty->IsBuildingType())
		throw lua::LuaException{ "not a building type" };
	auto first = L.CheckEnum<shok::EntityTypeId>(2);
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->BuildingUpgrades.SaveVector();
		for (const auto& u : v.Vector) {
			if (u.Category == id)
				throw lua::LuaException{ "upgrade category already exists" };
		}
		v.Vector.emplace_back(id, first);
	}
	auto ctype = first;
	while (ctype != shok::EntityTypeId::Invalid) {
		auto* cty = CppLogic::GetEntityType(ctype);
		auto* bty = dynamic_cast<GGL::CGLBuildingProps*>(cty->LogicProps);
		if (bty == nullptr)
			throw lua::LuaException{ "non building type in building ucat" };
		bty->Upgrade.Category = id;
		ctype = bty->Upgrade.Type;
	}
	// players are not created yet, so the upgrademanagers in there read the correct data from GGL::CLogicProperties::GlobalObj
	Obj.ToRemoveBuilding.push_back(id);
	L.Push(id);
	return 1;
}
CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::WaterTypeId>::Load(shok::WaterTypeId id, luaext::State L) {
	(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->LoadWaterTypeFromExtraFile(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::WaterTypeId>::TableName() {
	return "WaterTypes";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::WaterTypeId>::FuncName() {
	return "WaterType";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId>::SanityCheck() {
	auto idm = CppLogic::GetIdManager<shok::WaterTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		auto [l, d] = CppLogic::GetWaterType(id);
		if (l == nullptr)
			throw lua::LuaException{ std::format("watertype {}={} missing", n, static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId>::Reset() {
	if (NeedsReload)
		(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->ReloadWaterTypes();
	NeedsReload = false;
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::SelectionTextureId>::Load(shok::SelectionTextureId id, luaext::State L) {
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures;
	const char* n = CppLogic::GetIdManager<shok::SelectionTextureId>().GetNameByID(id);
	auto d = mng->Get(n);
	d->FreeCache(); // nop if empty
	d->Get();
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::SelectionTextureId>::TableName() {
	return nullptr;
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::SelectionTextureId>::FuncName() {
	return "SelectionTexture";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId>::SanityCheck() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId>::UnLoad(shok::SelectionTextureId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures->PopId(static_cast<int>(id));
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTextureId>::Load(shok::TerrainTextureId id, luaext::State L) {
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->LoadTexture(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTextureId>::TableName() {
	return nullptr;
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTextureId>::FuncName() {
	return "TerrainTexture";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId>::SanityCheck() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId>::UnLoad(shok::TerrainTextureId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->PopTexture(id);
	CppLogic::GetIdManager<shok::TerrainTextureId>().RemoveID(id);
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTypeId>::Load(shok::TerrainTypeId id, luaext::State L) {
	(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->LoadTerrainTypeFromExtraFile(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyTerrainType(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTypeId>::TableName() {
	return "TerrainTypes";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTypeId>::FuncName() {
	return "TerrainType";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId>::SanityCheck() {
	auto idm = CppLogic::GetIdManager<shok::TerrainTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		auto [l, d] = CppLogic::GetTerrainType(id);
		if (l == nullptr)
			throw lua::LuaException{ std::format("terraintype {}={} missing", n, static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId>::Reset() {
	if (NeedsReload) {
		(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->ReloadTerrainTypes();
		(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyAllTerrainTypes();
	}
	NeedsReload = false;
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId>::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityCategory>::Load(shok::EntityCategory id, luaext::State L) {

}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityCategory>::TableName() {
	return "EntityCategories";
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityCategory>::FuncName() {
	return "EntityCategory";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::SanityCheck() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::Reload() {

}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::UnLoad(shok::EntityCategory id) {
	CppLogic::GetIdManager<shok::EntityCategory>().RemoveID(id);
}
template<>
constexpr bool CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::RegisterFuncLoad = false;
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::Obj{};

void CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::Reset()
{
	while (!ToRemove.empty()) {
		shok::ExperienceClass id = ToRemove.back();
		ToRemove.pop_back();
		GGL::ExperienceClassHolder::GlobalObj()->PopExpeienceClass(id);
	}
	for (shok::ExperienceClass id : ToReload) {
		GGL::ExperienceClassHolder::GlobalObj()->LoadExperienceClass(id);
	}
	ToReload.clear();
	GGL::CEntityProfile::HookExperienceClassAssignment(false);
}
void CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::SanityCheck()
{
}
void CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::RegisterFuncs(luaext::State L)
{
	L.RegisterFunc<Add>("AddExperienceClass", -3);
	L.RegisterFunc<Add>("ReloadExperienceClass", -3);
}
int CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::Add(luaext::State L)
{
	GGL::CEntityProfile::HookExperienceClassAssignment(true);
	const char* n = L.CheckString(1);
	auto* mng = GGL::ExperienceClassHolder::GlobalObj();
	shok::ExperienceClass id = CppLogic::GetIdManager<shok::ExperienceClass>().GetIdByName(n);
	if (id != shok::ExperienceClass::Invalid) {
		if (std::find(Obj.ToRemove.begin(), Obj.ToRemove.end(), id) == Obj.ToRemove.end() && std::find(Obj.ToReload.begin(), Obj.ToReload.end(), id) == Obj.ToReload.end())
			Obj.ToReload.push_back(id);
		mng->LoadExperienceClass(id);
		L.Push(static_cast<int>(id));
		return 1;
	}
	shok::EntityCategory cat = L.IsNumber(2) ? static_cast<shok::EntityCategory>(L.CheckInt(2)) : luaext::State{ L }.CheckEnum<shok::EntityCategory>(2);
	id = mng->AddExperienceClass(n, cat);
	Obj.ToRemove.push_back(id);
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
CppLogic::ModLoader::ModLoader::ExperienceClassesLoader CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::Obj{};

void CppLogic::ModLoader::ModLoader::SoundGroupsLoader::Reset()
{
	while (!ToRemove.empty()) {
		auto id = ToRemove.back();
		ToRemove.pop_back();
		(*ESnd::CSoESound::GlobalObj)->PopSoundGroup(id);
	}
}
void CppLogic::ModLoader::ModLoader::SoundGroupsLoader::SanityCheck()
{
	/*auto im = CppLogic::GetIdManager<shok::SoundId>();
	auto* mng = *ESnd::CSoESound::GlobalObj;
	for (auto i : im) {
		auto s = std::format("data\\sounds\\{}.wav", im.GetNameByID(i));
		BB::IFileSystem::FileInfo info{};
		(*BB::CFileSystemMgr::GlobalObj)->GetFileInfo(&info, s.c_str());
		if (!info.Found)
			throw lua::LuaException{ std::format("sound {}={} does nox exist", im.GetNameByID(i), static_cast<int>(i)) };
	}*/
}
void CppLogic::ModLoader::ModLoader::SoundGroupsLoader::RegisterFuncs(luaext::State L)
{
	L.RegisterFunc<Add>("AddSounds", -3);
}
int CppLogic::ModLoader::ModLoader::SoundGroupsLoader::Add(luaext::State l)
{
	luaext::State L{ l };
	int num = L.GetTop();
	for (int i = 1; i <= num; ++i) {
		const char* n = L.CheckString(i);
		if ((*BB::CIDManagerEx::SoundsManager)->GetIdByName(n) != 0)
			throw lua::LuaException{ "sound already exists" };
	}
	auto* mng = *ESnd::CSoESound::GlobalObj;
	L.Push("Sounds");
	L.GetGlobal();
	shok::SoundId firstid;
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
		shok::SoundId id = mng->AddSoundToLastGroup(n);
		std::string s{ n };
		std::replace(s.begin(), s.end(), '\\', '_');
		L.Push(s);
		L.Push(id);
		L.SetTableRaw(-3);
	}
	L.Push(firstid);
	Obj.ToRemove.push_back(static_cast<shok::SoundId>(firstid));
	return 1;
}
CppLogic::ModLoader::ModLoader::SoundGroupsLoader CppLogic::ModLoader::ModLoader::SoundGroupsLoader::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimSetId>::Load(shok::AnimSetId id, luaext::State L) {
	auto* mng = *EGL::AnimSetManager::GlobalObj;
	if (static_cast<int>(id) < static_cast<int>(mng->AnimSets.size()))
		mng->FreeAnimSet(id);
	mng->LoadAnimSet(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimSetId>::TableName() {
	return nullptr;
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimSetId>::FuncName() {
	return "AnimSet";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId>::SanityCheck() {
	auto* mng = *EGL::AnimSetManager::GlobalObj;
	auto idm = CppLogic::GetIdManager<shok::AnimSetId>();
	for (auto id : idm) {
		if (static_cast<int>(id) > static_cast<int>(mng->AnimSets.size()) || mng->GetAnimSet(id) == nullptr)
			throw lua::LuaException{ std::format("animset {}={} does not exist", idm.GetNameByID(id), static_cast<int>(id)) };
	}
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId>::UnLoad(shok::AnimSetId id) {
	(*EGL::AnimSetManager::GlobalObj)->PopAnimSet(id);
	CppLogic::GetIdManager<shok::AnimSetId>().RemoveID(id);
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId>::Obj{};

void CppLogic::ModLoader::ModLoader::DirectXEffectLoader::Reset()
{
	if (!ToRemove.empty()) {
		for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
			(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(static_cast<shok::ModelId>(id));
			// gets reloaded on next use anyway
		}
		auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->Effects;
		while (!ToRemove.empty()) {
			int id = ToRemove.back();
			ToRemove.pop_back();
			mng->Data[id]->FreeCache();
			if (id + 1 == static_cast<int>(mng->Data.size()))
				mng->PopId(id);
			else
				mng->IdManager->RemoveID(id);
		}
	}
}
void CppLogic::ModLoader::ModLoader::DirectXEffectLoader::SanityCheck()
{
}
void CppLogic::ModLoader::ModLoader::DirectXEffectLoader::RegisterFuncs(luaext::State L)
{
	L.RegisterFunc<Add>("LoadDirectXEffect", -3);
}
int CppLogic::ModLoader::ModLoader::DirectXEffectLoader::Add(luaext::State L)
{
	const char* name = L.CheckString(1);
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->Effects;
	if (!mng->IdManager->GetIdByName(name))
		mng->IdManager->GetIDByNameOrCreate(name);
	auto* s = mng->Get(name);
	s->FreeCache();
	s->Get();
	Obj.ToRemove.push_back(s->Id);
	return 0;
}
CppLogic::ModLoader::ModLoader::DirectXEffectLoader CppLogic::ModLoader::ModLoader::DirectXEffectLoader::Obj{};

void CppLogic::ModLoader::ModLoader::FeedbackEventSoundDataLoader::Reset() {
}

void CppLogic::ModLoader::ModLoader::FeedbackEventSoundDataLoader::SanityCheck() {
}

void CppLogic::ModLoader::ModLoader::FeedbackEventSoundDataLoader::RegisterFuncs(luaext::State L) {
	L.RegisterFunc<Add>("LoadFeedbackEventSoundData", -3);
}

int CppLogic::ModLoader::ModLoader::FeedbackEventSoundDataLoader::Add(luaext::State l) {
	luaext::State L{l};
	auto* m = GGUI::SoundFeedback::GlobalObj();
	auto id = L.CheckEnum<shok::FeedbackEventShortenedId>(1);
	m->SD.ReloadData(id);
	return 0;
}

CppLogic::ModLoader::ModLoader::FeedbackEventSoundDataLoader CppLogic::ModLoader::ModLoader::FeedbackEventSoundDataLoader::Obj{};

template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::FontId>::Load(shok::FontId id, luaext::State L) {
	auto* mng = EGUIX::FontManager::GlobalObj();
	mng->ClearFont(id);
	mng->GetFontObj(id);
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::FontId>::TableName() {
	return nullptr;
}
template<>
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::FontId>::FuncName() {
	return "Font";
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::FontId>::SanityCheck() {
	
}
template<>
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::FontId>::UnLoad(shok::FontId id) {
	auto* mng = EGUIX::FontManager::GlobalObj();
	mng->PopFont(id);
}
template<>
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::FontId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::FontId>::Obj{};

std::array<CppLogic::ModLoader::ModLoader::DataTypeLoader*, 21> CppLogic::ModLoader::ModLoader::Loaders{ {
		&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::EffectTypeId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId>::Obj,
			&CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId>::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::Obj,
			&CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::Obj,
			&CppLogic::ModLoader::ModLoader::SoundGroupsLoader::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId>::Obj,
			&CppLogic::ModLoader::ModLoader::DirectXEffectLoader::Obj,
			&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::FontId>::Obj,
			&FeedbackEventSoundDataLoader::Obj,
	} };
std::array<CppLogic::ModLoader::ModLoader::DataTypeLoader*, 1> CppLogic::ModLoader::ModLoader::LoadersIngame{ {
		&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::Obj,
	} };


bool CppLogic::ModLoader::ModLoader::Initialized = false;

int CppLogic::ModLoader::ModLoader::SetEntityTypeToReload(luaext::State L)
{
	auto id = luaext::State{ L }.CheckEnum<shok::EntityTypeId>(1);
	CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::Obj.OnIdLoaded(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::SetTaskListToReload(luaext::State L)
{
	auto id = luaext::State{ L }.CheckEnum<shok::TaskListId>(1);
	CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Obj.OnIdLoaded(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::SetDamageclassesToReload(luaext::State L)
{
	CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Obj.OnIdLoaded(shok::DamageClassId::Invalid);
	return 0;
}

int CppLogic::ModLoader::ModLoader::RefreshEntityCategoryCache(luaext::State L)
{
	EGL::EntityCategoryCache::RefreshCache(*EGL::CGLEEntitiesProps::GlobalObj);
	return 0;
}

int CppLogic::ModLoader::ModLoader::SanityCheck(luaext::State L)
{
	for (auto* l : Loaders) {
		l->SanityCheck();
	}
	return 0;
}

void CppLogic::ModLoader::ModLoader::InitExtraECats()
{
	auto m = CppLogic::GetIdManager<shok::EntityCategory>();
	for (shok::EntityCategory ec : {shok::EntityCategory::TargetFilter_TargetType, shok::EntityCategory::TargetFilter_TargetTypeLeader,
		shok::EntityCategory::TargetFilter_CustomRanged, shok::EntityCategory::TargetFilter_NonCombat}) {
		m.GetIDByNameOrCreate(magic_enum::enum_name(ec).data(), ec);
		CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::Obj.OnIdAllocated(ec);
	}
}


int CppLogic::ModLoader::ModLoader::GetModpackInfo(luaext::State L)
{
	ModpackDesc d{};
	d.Name = L.CheckStringView(1);
	d.BBAPath = GetModPackPath(d.Name);
	try {
		auto ar = BB::CBBArchiveFile::CreateUnique();
		ar->OpenArchive(d.BBAPath.c_str());
		auto fil = std::format("ModPack\\{}\\ModPack.xml", d.Name);
		auto file = ar->OpenFileStreamUnique(fil.c_str(), BB::IStream::Flags::DefaultRead);
		auto seri = BB::CXmlSerializer::CreateUnique();
		seri->DeserializeByData(file.get(), &d, ModpackDesc::SerializationData);
		Serializer::ObjectToLuaSerializer::Serialize(L, &d, ModpackDesc::SerializationDataEx);
	}
	catch (const BB::CFileException& e) {
		char buff[200]{};
		e.CopyMessage(buff, 200);
		L.Push(buff);
	}
	return 1;
}

int CppLogic::ModLoader::ModLoader::LoadModpackBBA(luaext::State L)
{
	auto p = GetModPackPath(L.CheckStringView(1));
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	mng->AddArchive(p.c_str());
	auto* a = mng->LoadOrder[0];
	if (mng->LoadOrder.size() >= 2) {
		if (auto* ar = dynamic_cast<BB::CBBArchiveFile*>(mng->LoadOrder[1])) {
			if (std::string_view{ ar->ArchiveFile.Filename }.ends_with(".s5x")) {
				mng->LoadOrder[1] = mng->LoadOrder[0];
				mng->LoadOrder[0] = ar;
			}
		}
	}
	L.NewUserClass<ArchivePopHelper>(std::move(p), a);
	return 1;
}

int CppLogic::ModLoader::ModLoader::CreateModpackRedirectLayer(luaext::State L)
{
	auto p = std::format("{} redirect layer", L.CheckStringView(1));
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	auto* r = CppLogic::Mod::FileSystem::RedirectFileSystem::CreateRedirectLayer(p);
	if (mng->LoadOrder.size() >= 2) {
		if (auto* ar = dynamic_cast<BB::CBBArchiveFile*>(mng->LoadOrder[1])) {
			if (std::string_view{ ar->ArchiveFile.Filename }.ends_with(".s5x")) {
				mng->LoadOrder[1] = mng->LoadOrder[0];
				mng->LoadOrder[0] = ar;
			}
		}
	}
	L.NewUserClass<ArchiveRedirectHelper>(std::move(p), r);
	return 1;
}

int CppLogic::ModLoader::ModLoader::InvalidModPackPanic(luaext::State L)
{
	auto m = L.CheckStringView(1);
	MessageBoxA(nullptr, m.data(), "ModLoader failure", MB_OK);
	std::exit(0);
	return 0;
}

int CppLogic::ModLoader::ModLoader::GetModpacks(luaext::State L)
{
	L.NewTable();
	std::filesystem::directory_iterator it{ std::filesystem::path(ModpackFolder) };
	int k = 1;
	for (const auto& i : it) {
		if (i.is_regular_file()) {
			const auto& p = i.path();
			if (p.extension().string() == ".bba") {
				L.Push(p.stem().string());
				L.SetTableRaw(-2, k);
				++k;
			}
		}
	}
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReserializeEntityType(luaext::State l)
{
	luaext::State L{ l };
	CppLogic::WinAPI::FileDialog dlg{};
	auto etyid = L.CheckEnum<shok::EntityTypeId>(1);
	auto* ety = CppLogic::GetEntityType(etyid);
	auto t = std::format("Write {}", CppLogic::GetIdManager<shok::EntityTypeId>().GetNameByID(etyid));
	dlg.Title = t.c_str();
	dlg.Filter = "xml\0*.xml\0";
	dlg.SelectedPath = L.OptStringView(2, "");
#ifndef RESERIALIZE_NO_DIALOG
	if (!dlg.Show()) {
		return 0;
	}
#endif
	BB::CFileStreamEx f{};
	if (f.OpenFile(dlg.SelectedPath.c_str(), BB::IStream::Flags::DefaultWrite)) {
		BB::CXmlSerializer::HookWriteXSIType();
		auto s = BB::CXmlSerializer::CreateUnique();
		s->SerializeByData(&f, ety, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(ety->GetClassIdentifier()));
		f.Close();
	}
	return 0;
}

int CppLogic::ModLoader::ModLoader::ReserializeTaskList(luaext::State l)
{
	luaext::State L{ l };
	CppLogic::WinAPI::FileDialog dlg{};
	auto etyid = L.CheckEnum<shok::TaskListId>(1);
	auto* ety = (*EGL::CGLETaskListMgr::GlobalObj)->GetTaskListByID(etyid);
	auto t = std::format("Write {}", CppLogic::GetIdManager<shok::TaskListId>().GetNameByID(etyid));
	dlg.Title = t.c_str();
	dlg.Filter = "xml\0*.xml\0";
	dlg.SelectedPath = L.OptStringView(2, "");
#ifndef RESERIALIZE_NO_DIALOG
	if (!dlg.Show()) {
		return 0;
	}
#endif
	BB::CFileStreamEx f{};
	if (f.OpenFile(dlg.SelectedPath.c_str(), BB::IStream::Flags::DefaultWrite)) {
		BB::CXmlSerializer::HookWriteXSIType();
		auto s = BB::CXmlSerializer::CreateUnique();
		s->SerializeByData(&f, ety, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(ety->GetClassIdentifier()));
		f.Close();
	}
	return 0;
}

int CppLogic::ModLoader::ModLoader::ReserializeModel(luaext::State l)
{
	luaext::State L{ l };
	CppLogic::WinAPI::FileDialog dlg{};
	auto etyid = L.CheckEnum<shok::ModelId>(1);
	auto& ety = (*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->Get(etyid);
	auto t = std::format("Write {}", CppLogic::GetIdManager<shok::ModelId>().GetNameByID(etyid));
	dlg.Title = t.c_str();
	dlg.Filter = "xml\0*.xml\0";
	dlg.SelectedPath = L.OptStringView(2, "");
#ifndef RESERIALIZE_NO_DIALOG
	if (!dlg.Show()) {
		return 0;
	}
#endif
	BB::CFileStreamEx f{};
	if (f.OpenFile(dlg.SelectedPath.c_str(), BB::IStream::Flags::DefaultWrite)) {
		BB::CXmlSerializer::HookWriteXSIType();
		auto s = BB::CXmlSerializer::CreateUnique();
		s->SerializeByData(&f, &ety, ED::CModelsProps::ModelData::SerializationData);
		f.Close();
	}
	return 0;
}

int CppLogic::ModLoader::ModLoader::SetModPackList(luaext::State L)
{
	ModPackList = L.CheckStringView(1);
	return 0;
}

int CppLogic::ModLoader::ModLoader::GetModPackList(luaext::State L)
{
	L.Push(ModPackList);
	return 1;
}

luaext::Reference CppLogic::ModLoader::ModLoader::SavegameValidOverride{};
int CppLogic::ModLoader::ModLoader::OverrideSavegameValid(luaext::State L)
{
	if (!L.IsFunction(1))
		throw lua::LuaException{ "no func at 1" };
	L.UnRef(SavegameValidOverride);
	L.PushValue(1);
	SavegameValidOverride = L.Ref();
	Framework::SavegameSystem::IsSaveValidOverride = [](std::string_view save) {
		luaext::State L{ shok::LuaStateMainmenu };
		int t = L.GetTop();
		bool r = false;

		L.Push(SavegameValidOverride);
		L.Push(save);
		if (L.PCall(1, 1) == lua::ErrorCode::Success) {
			r = L.ToBoolean(-1);
		}

		L.SetTop(t);
		return r;
		};
	Framework::SavegameSystem::HookSavegameValid();
	return 0;
}

int CppLogic::ModLoader::ModLoader::LoadStringTableTextOverrides(luaext::State L)
{
	std::string_view file = L.CheckStringView(1);
	std::string_view lang = L.CheckStringView(2);
	auto f = std::format("data/text/{}/override/{}.xml", lang, file);
	try {
		auto seri = BB::CXmlSerializer::CreateUnique();
		BB::CFileStreamEx fs{};
		CppLogic::SavegameExtra::StringTableTextOverride o{};
		if (fs.OpenFile(f.data(), BB::IStream::Flags::DefaultRead)) {
			seri->DeserializeByData(&fs, &o, CppLogic::SavegameExtra::StringTableTextOverride::SerializationData);
			o.Merge(L.OptStringView(3, file), L.GetState());
			L.Push(static_cast<int>(o.StringTableTextOverrideData.size()));
		}
		else {
			L.Push(-1);
		}
	}
	catch (const BB::CFileException&) {
		L.Push(-1);
	}
	return 1;
}

int CppLogic::ModLoader::ModLoader::MapGetModPacks(luaext::State l)
{
	luaext::State L{ l };
	const char* n = L.CheckString(1);
	auto ty = L.CheckEnum<shok::MapType>(2);
	const char* cn = L.OptString(3, nullptr); // optional
	Framework::CampagnInfo* ci = (*Framework::CMain::GlobalObj)->CampagnInfoHandler.GetCampagnInfo(ty, cn);
	if (ci == nullptr)
		throw lua::LuaException("invalid map type/campagn");
	Framework::MapInfo* i = ci->GetMapInfoByName(n);
	if (i == nullptr)
		throw lua::LuaException("invalid");

	ExtendedMapInfo inf{};
	auto seri = BB::CXmlSerializer::CreateUnique();

	if (i->IsExternalmap) {
		auto ar = BB::CBBArchiveFile::CreateUnique();
		ar->OpenArchive(i->MapFilePath.c_str());
		auto file = ar->OpenFileStreamUnique("Maps\\ExternalMap\\Info.xml", BB::IStream::Flags::DefaultRead);
		seri->DeserializeByData(file.get(), &inf, ExtendedMapInfo::SerializationData);
		ar->CloseArchive();
	}
	else {
		auto infoxml = std::format("{}\\{}", i->MapFilePath.c_str(), "Info.xml");
		BB::CFileStreamEx file{};
		file.OpenFile(infoxml.c_str(), BB::IStream::Flags::DefaultRead);
		seri->DeserializeByData(&file, &inf, ExtendedMapInfo::SerializationData);
		file.Close();
	}

	L.NewTable();
	int j = 1;
	for (const auto& s : inf.ModPacks.Required) {
		L.Push(s);
		L.SetTableRaw(-2, j);
		++j;
	}

	L.NewTable();
	j = 1;
	for (const auto& s : inf.ModPacks.Incompatible) {
		L.Push(s);
		L.SetTableRaw(-2, j);
		++j;
	}

	L.NewTable();
	j = 1;
	for (const auto& s : inf.ModPacks.UserRequestable) {
		L.Push(s);
		L.SetTableRaw(-2, j);
		++j;
	}

	return 3;
}

int CppLogic::ModLoader::ModLoader::GetEntityTypeMem(luaext::State l) {
	luaext::State L{ l };
	Serializer::ObjectAccess::PushObject(L, "ModLoader.GetEntityTypeMem", L.CheckEntityType(1),
		static_cast<int>(L.CheckEnum<shok::EntityTypeId>(1)), [](int id) {
			DataTypeLoaderTracking<shok::EntityTypeId>::Obj.OnIdLoaded(static_cast<shok::EntityTypeId>(id));
		});
	return 1;
}

int CppLogic::ModLoader::ModLoader::GetTaskListMem(luaext::State l) {
	luaext::State L{ l };
	auto id = L.CheckEnum<shok::TaskListId>(1);
	Serializer::ObjectAccess::PushObject(L, "ModLoader.GetTaskListMem", GetTaskList(id),
		static_cast<int>(id), [](int id) {
			DataTypeLoaderHalf<shok::TaskListId>::Obj.OnIdLoaded(static_cast<shok::TaskListId>(id));
		});
	return 1;
}

int CppLogic::ModLoader::ModLoader::GetTechnologyMem(luaext::State l) {
	luaext::State L{ l };
	auto id = L.CheckEnum<shok::TechnologyId>(1);
	Serializer::ObjectAccess::PushObject(L, "ModLoader.GetTechnologyMem", GetTechnology(id), shok::Technology::SerializationData,
		static_cast<int>(id), [](int id) {
			DataTypeLoaderHalf<shok::TechnologyId>::Obj.OnIdLoaded(static_cast<shok::TechnologyId>(id));
		});
	return 1;
}

int CppLogic::ModLoader::ModLoader::GetEffectTypeMem(luaext::State l) {
	luaext::State L{ l };
	auto id = L.CheckEnum<shok::EffectTypeId>(1);
	Serializer::ObjectAccess::PushObject(L, "ModLoader.GetEffectTypeMem", GetEffectType(id), EGL::EffectType::SerializationData,
		static_cast<int>(id), [](int id) {
			DataTypeLoaderReload<shok::EffectTypeId>::Obj.OnIdLoaded(static_cast<shok::EffectTypeId>(id));
		});
	return 1;
}

int CppLogic::ModLoader::ModLoader::GetFeedbackEventMem(luaext::State l) {
	luaext::State L{ l };
	auto& sd = GGUI::SoundFeedback::GlobalObj()->SD;
	auto id = L.CheckEnum<shok::FeedbackEventShortenedId>(1);
	auto ev = sd.SoundData.find(id);
	if (ev == sd.SoundData.end())
		throw lua::LuaException{"could not find SoundData"};
	auto* e = ev->second;
	Serializer::ObjectAccess::PushObject(L, "ModLoader.GetFeedbackEventMem", e, GGUI::SoundFeedback::FeedbackEventSoundData::SerializationData);
	return 1;
}

void CppLogic::ModLoader::ModLoader::Log(luaext::State L, const char* log)
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
			if (L.PCall(1, 0) != lua::ErrorCode::Success)
				L.Pop(1);
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
	Framework::SavegameSystem::PreGameSavedTo = &PreSave;
	Framework::SavegameSystem::PostGameSavedTo = &PostSave;
	EGL::CGLEGameLogic::HookOnMapscriptLoaded();
	EGL::CGLEGameLogic::OnMapscriptLoaded = &PostMapscriptLoaded;
	(*BB::CFileSystemMgr::GlobalObj)->AddArchive("ModPacks\\CppLogic.bba");
	Initialized = true;
}

void CppLogic::ModLoader::ModLoader::Cleanup(Framework::CMain::NextMode n)
{
	if (((*Framework::CMain::GlobalObj)->CurrentMode != Framework::CMain::Mode::MainMenu)) {
		luaext::State L{ *EScr::CScriptTriggerSystem::GameState };
		ModPackList = "";

		Log(L, "Cleanup");
		int t = L.GetTop();
		AddLib(L);
		L.Push("ModLoader");
		L.GetGlobal();
		if (L.IsTable(-1)) {
			L.Push("Cleanup");
			L.GetTableRaw(-2);
			if (L.IsFunction(-1)) {
				if (L.PCall(0, 0) != lua::ErrorCode::Success) {
					auto s = std::format("Cleanup error: {}", L.ToStringView(-1));
					Log(L, s.c_str());
					L.Pop(1);
				}
			}
			else {
				L.Pop(1);
			}
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
			auto p = CppLogic::Iterator::PredicateFunc<EGL::CGLEEntity>([](const EGL::CGLEEntity*, float*, int*) { return true; });
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
			auto p = CppLogic::Iterator::PredicateFunc<EGL::CEffect>([](const EGL::CEffect*, float*, int*) { return true; });
			CppLogic::Iterator::GlobalEffectIterator it{ &p };
			for (EGL::CEffect* a : it) {
				delete a;
			}
		}

		for (auto* l : Loaders) {
			l->Reset();
		}

		Log(L, "Done");
	}
}

void CppLogic::ModLoader::ModLoader::InitMainmenu(luaext::State L)
{
	L.Push(ModLoaderLib);
	L.NewTable();
	L.RegisterFuncs(Mainmenu, -3);
	L.SetTableRaw(-3);
}

bool CppLogic::ModLoader::ModLoader::IsInitialized()
{
	return Initialized;
}

void CppLogic::ModLoader::ModLoader::AddTaskListToRemove(shok::TaskListId id)
{
	CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Obj.OnIdAllocated(id);
}

std::string CppLogic::ModLoader::ModLoader::GetModPackPath(std::string_view n)
{
	return std::format("{}\\{}.bba", ModpackFolder, n);
}

CreateSerializationListFor(CppLogic::ModLoader::ModpackDesc, Required);

const BB::SerializationData CppLogic::ModLoader::ModpackDesc::SerializationData[]{
	AutoMemberSerialization(ModpackDesc, LoaderPath),
	AutoMemberSerialization(ModpackDesc, ScriptPath),
	AutoMemberSerialization(ModpackDesc, MainmenuPath),
	AutoMemberSerialization(ModpackDesc, Version),
	AutoMemberSerialization(ModpackDesc, Description),
	AutoMemberSerialization(ModpackDesc, Required),
	AutoMemberSerialization(ModpackDesc, Incompatible),
	AutoMemberSerialization(ModpackDesc, Override),
	AutoMemberSerialization(ModpackDesc, DataMod),
	AutoMemberSerialization(ModpackDesc, ScriptMod),
	AutoMemberSerialization(ModpackDesc, MainmenuMod),
	AutoMemberSerialization(ModpackDesc, KeepArchive),
	AutoMemberSerialization(ModpackDesc, UserRequestable),
	AutoMemberSerialization(ModpackDesc, ScriptLib),
	BB::SerializationData::GuardData(),
};
const BB::SerializationData CppLogic::ModLoader::ModpackDesc::SerializationDataEx[]{
	BB::SerializationData::EmbeddedData(nullptr, 0, sizeof(ModpackDesc), SerializationData),
	AutoMemberSerialization(ModpackDesc, Name),
	AutoMemberSerialization(ModpackDesc, BBAPath),
	BB::SerializationData::GuardData(),
};

const BB::SerializationData CppLogic::ModLoader::ExtendedMapInfo::SModPacks::SerializationData[]{
	AutoMemberSerialization(ExtendedMapInfo::SModPacks, Required),
	AutoMemberSerialization(ExtendedMapInfo::SModPacks, Incompatible),
	AutoMemberSerialization(ExtendedMapInfo::SModPacks, UserRequestable),
	BB::SerializationData::GuardData(),
};

const BB::SerializationData CppLogic::ModLoader::ExtendedMapInfo::SerializationData[]{
	BB::SerializationData::AutoBaseClass<ExtendedMapInfo, Framework::MapInfo>(),
	AutoMemberSerialization(ExtendedMapInfo, ModPacks),
	BB::SerializationData::GuardData(),
};


int CppLogic::ModLoader::ArchivePopHelper::Remove(luaext::State L)
{
	auto t = L.CheckUserClass<ArchivePopHelper>(1);
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	auto v = mng->LoadOrder.SaveVector();
	auto it = v.Vector.begin();
	while (it != v.Vector.end()) {
		if (*it == t->FS) {
			(*it)->Destroy();
			t->FS = nullptr;
			v.Vector.erase(it);
			L.Push(true);
			return 1;
		}
		++it;
	}
	L.Push(false);
	return 1;
}

int CppLogic::ModLoader::ArchivePopHelper::IsLoaded(luaext::State L)
{
	auto t = L.CheckUserClass<ArchivePopHelper>(1);
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	for (const auto* f : mng->LoadOrder) {
		if (f == t->FS) {
			L.Push(true);
			return 1;
		}
	}
	L.Push(false);
	return 1;
}

int CppLogic::ModLoader::ArchivePopHelper::ToString(luaext::State L)
{
	auto t = L.CheckUserClass<ArchivePopHelper>(1);
	L.Push(t->Archive);
	if (t->FS == nullptr) {
		L.Push();
	}
	else if (auto* ar = dynamic_cast<BB::CBBArchiveFile*>(t->FS)) {
		L.Push(ar->ArchiveFile.Filename);
	}
	else if (auto* fs = dynamic_cast<BB::CDirectoryFileSystem*>(t->FS)) {
		L.Push(fs->Path);
	}
	else if (auto* rd = dynamic_cast<CppLogic::Mod::FileSystem::RedirectFileSystem*>(t->FS)) {
		L.Push(rd->Name);
	}
	else {
		L.Push(typeid(*t->FS).name());
	}
	return 2;
}

CppLogic::Mod::FileSystem::RedirectFileSystem* CppLogic::ModLoader::ArchiveRedirectHelper::CheckStillValid()
{
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	for (const auto* f : mng->LoadOrder) {
		if (f == FS) {
			if (auto* r = dynamic_cast<CppLogic::Mod::FileSystem::RedirectFileSystem*>(FS))
				return r;
		}
	}
	throw lua::LuaException{ "not in loadorder" };
}

int CppLogic::ModLoader::ArchiveRedirectHelper::Get(luaext::State L)
{
	auto t = L.CheckUserClass<ArchiveRedirectHelper>(1);
	auto* r = t->CheckStillValid();
	L.NewTable();
	for (const auto& [n, re] : r->Redirects) {
		L.Push(n);
		L.Push(re);
		L.SetTableRaw(-3);
	}
	return 1;
}

int CppLogic::ModLoader::ArchiveRedirectHelper::Set(luaext::State L)
{
	auto t = L.CheckUserClass<ArchiveRedirectHelper>(1);
	auto* r = t->CheckStillValid();
	std::string_view n = L.CheckStringView(2);
	auto it = r->Redirects.find(n);
	if (L.IsNoneOrNil(3)) {
		if (it != r->Redirects.end())
			r->Redirects.erase(it);
		return 0;
	}
	std::string_view re = L.CheckStringView(3);
	if (it == r->Redirects.end()) {
		r->Redirects.emplace(std::piecewise_construct, std::forward_as_tuple(n), std::forward_as_tuple(re));
	}
	else {
		it->second = re;
	}
	return 0;
}

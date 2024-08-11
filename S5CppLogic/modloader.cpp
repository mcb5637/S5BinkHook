#include "pch.h"
#include "modloader.h"

#include <regex>
#include <filesystem>

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
#include "ModConfig.h"
#include "StringUtility.h"
#include "WinAPIUtil.h"

shok::String CppLogic::ModLoader::ModLoader::ModPackList{};
size_t CppLogic::ModLoader::ModLoader::GUIDLength = 0;

void CppLogic::ModLoader::ModLoader::Init(lua::State L, const char* mappath, std::string_view func)
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
		L.Push("Initialize");
		L.GetTableRaw(-2);
		if (L.IsFunction(-1)) {
			if (L.PCall(0, 0) != lua::ErrorCode::Success) {
				auto s = std::format("Initialize error: {}", L.ToStringView(-1));
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
				auto s = std::format("{} error: {}", func, L.ToStringView(-1));
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
	lua::State L{ ingame };
	static constexpr size_t bufflen = 0x3FF;
	char mappath[bufflen];
	snprintf(mappath, bufflen, "%s\\%s\\ModLoader.lua", externalmap ? "Maps" : path, externalmap ? "ExternalMap" : name);
	Init(L, mappath, "MapStart");
}
void CppLogic::ModLoader::ModLoader::PostMapscriptLoaded()
{
	lua::State L{ *EScr::CScriptTriggerSystem::GameState };
	int t = L.GetTop();

	Log(L, "ModLoader on Mapscript loaded");
	L.Push("ModLoader");
	L.GetGlobal();
	if (L.IsTable(-1)) {
		L.Push("LoadModScripts");
		L.GetTableRaw(-2);
		if (L.IsFunction(-1)) {
			if (L.PCall(0, 0) != lua::ErrorCode::Success) {
				auto s = std::format("LoadModScripts error: {}", L.ToStringView(-1));
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
void CppLogic::ModLoader::ModLoader::RemoveLib(lua::State L)
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
	auto idm = CppLogic::GetIdManager<shok::EntityTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		auto et = EGL::CGLEEntitiesProps::GetEntityType(id);
		if (et == nullptr)
			throw lua::LuaException{ std::format("entitytype {}={} missing", n, static_cast<int>(id)) };
		if (et->LogicProps == nullptr)
			throw lua::LuaException{ std::format("entitytype {}={} missing LogicProps", n, static_cast<int>(id)) };
		if (et->DisplayProps == nullptr)
			throw lua::LuaException{ std::format("entitytype {}={} missing DisplayProps", n, static_cast<int>(id)) };
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
	auto idm = CppLogic::GetIdManager<shok::EffectTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (mng->Get(id)->Logic == nullptr)
			throw lua::LuaException{ std::format("effecttype {}={} missing Logic", n, static_cast<int>(id)) };
		if (mng->Get(id)->Display == nullptr)
			throw lua::LuaException{ std::format("effecttype {}={} missing Display", n, static_cast<int>(id)) };
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
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::SanityCheck() {
	auto* mng = *EGL::CGLETaskListMgr::GlobalObj;
	auto idm = CppLogic::GetIdManager<shok::TaskListId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (mng->TaskLists.at(static_cast<int>(id)) == nullptr)
			throw lua::LuaException{ std::format("tasklist {}={} missing", n, static_cast<int>(id)) };
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Reload() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::UnLoad(shok::TaskListId id) {
	if (*EGL::CGLETaskListMgr::GlobalObj)
		(*EGL::CGLETaskListMgr::GlobalObj)->RemoveTaskList(id);
	else
		CppLogic::GetIdManager<shok::TaskListId>().RemoveID(id);
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ArmorClassId>::Load(shok::ArmorClassId id, luaext::EState L) {

}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ArmorClassId>::TableName() {
	return "ArmorClasses";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ArmorClassId>::FuncName() {
	return "ArmorClass";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::SanityCheck() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::Reload() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::UnLoad(shok::ArmorClassId id) {
	CppLogic::GetIdManager<shok::ArmorClassId>().RemoveID(id);
}
template<>
constexpr bool CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::RegisterFuncLoad = false;
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ArmorClassId>::Obj{};


void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::DamageClassId>::Load(shok::DamageClassId id, luaext::EState L) {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager;
	L.PushValue(2);
	void* o = CppLogic::Serializer::ObjectToLuaSerializer::Deserialize(L, nullptr, nullptr, shok::ClassId::Invalid, {
		GGL::CDamageClassProps::Identifier,
		CppLogic::Mod::Config::DamageClassExt::Identifier,
		});
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
	auto idm = CppLogic::GetIdManager<shok::DamageClassId>();
	auto acmng = CppLogic::GetIdManager<shok::ArmorClassId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		auto* dc = mng->Logic.TryGet(id);
		if (dc == nullptr)
			throw lua::LuaException{ std::format("damageclass {}={} is missing", n, static_cast<int>(id)) };
		for (auto ac : acmng) {
			if (dc->GetBonusVsArmorClass(ac) < 0)
				throw lua::LuaException{ std::format("damageclass {}={} has invalid factor {}", n, static_cast<int>(id), static_cast<int>(ac)) };
		}
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Reset() {
	if (NeedsReload)
		(*Framework::CMain::GlobalObj)->GluePropsManager->DamageClassesPropsManager->ResetDamageClasses();
	NeedsReload = false;
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TechnologyId>::Load(shok::TechnologyId id, luaext::EState L) {
	auto* m = (*GGL::CGLGameLogic::GlobalObj)->TechManager;
	if (static_cast<int>(id) < static_cast<int>(m->Techs.size()))
		m->FreeTech(id);
	m->LoadTech(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TechnologyId>::TableName() {
	return "Technologies";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TechnologyId>::FuncName() {
	return "Technology";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::SanityCheck() {
	auto* mng = (*GGL::CGLGameLogic::GlobalObj)->TechManager;
	auto idm = CppLogic::GetIdManager<shok::TechnologyId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (mng->Get(id) == nullptr)
			throw lua::LuaException{ std::format("tech {}={} missing", n, static_cast<int>(id)) };
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::Reload() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::UnLoad(shok::TechnologyId id) {
	if (*GGL::CGLGameLogic::GlobalObj)
		(*GGL::CGLGameLogic::GlobalObj)->TechManager->PopTech(id);
	if ((*BB::CIDManagerEx::TechnologiesManager))
		(*BB::CIDManagerEx::TechnologiesManager)->RemoveID(static_cast<int>(id));
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TechnologyId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ModelId>::Load(shok::ModelId id, luaext::EState L) {
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->LoadModelDataFromExtraFile(id);
	if (static_cast<int>(id) < static_cast<int>(m->ModelManager.Models.size()))
		m->FreeModel(id);
	m->LoadModel(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ModelId>::TableName() {
	return "Models";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::ModelId>::FuncName() {
	return "Model";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::SanityCheck() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::Reload() {
	for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
		(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(static_cast<shok::ModelId>(id));
	}
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ReloadAllModels();
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::UnLoad(shok::ModelId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->PopModel(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->PopModel(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->ModelProps->ModelIdManager->RemoveID(static_cast<int>(id));
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::ModelId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::GUITextureId>::Load(shok::GUITextureId id, luaext::EState L) {
	auto* mng = EGUIX::TextureManager::GlobalObj();
	mng->ReloadTexture(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::GUITextureId>::TableName() {
	return nullptr;
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::GUITextureId>::FuncName() {
	return "GUITexture";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::SanityCheck() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::UnLoad(shok::GUITextureId id) {
	EGUIX::TextureManager::GlobalObj()->FreeTexture(id);
	EGUIX::TextureManager::GlobalObj()->IdManager->RemoveID(static_cast<int>(id));
}
template<>
constexpr bool CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::RegisterFuncPreLoad = false;
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimationId>::Load(shok::AnimationId id, luaext::EState L) {
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	if (m->AnimManager.Map.find(static_cast<int>(id)) != m->AnimManager.Map.end())
		m->FreeAnim(id);
	shok::String s{ CppLogic::GetIdManager<shok::AnimationId>().GetNameByID(id) };
	m->AnimManager.Load(static_cast<int>(id), &s);
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->CreateAnimProps(static_cast<shok::AnimationId>(id));
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimationId>::TableName() {
	return "Animations";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimationId>::FuncName() {
	return "Animation";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId>::SanityCheck() {
	auto* m = (*ED::CGlobalsBaseEx::GlobalObj)->ResManager;
	auto idm = CppLogic::GetIdManager<shok::AnimationId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (m->AnimManager.Map.find(static_cast<int>(id)) == m->AnimManager.Map.end())
			throw lua::LuaException{ std::format("anim {}={} missing", n, static_cast<int>(id)) };
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId>::UnLoad(shok::AnimationId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeAnim(id);
	CppLogic::GetIdManager<shok::AnimationId>().RemoveID(id);
	(*Framework::CMain::GlobalObj)->GluePropsManager->AnimPropsManager->PopAnimPops(id);
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimationId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::UpgradeCategoryId>::Load(shok::UpgradeCategoryId id, luaext::EState L) {

}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::UpgradeCategoryId>::TableName() {
	return "UpgradeCategories";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::UpgradeCategoryId>::FuncName() {
	return nullptr;
}
void CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::Reset()
{
	{
		auto v = (*GGL::CLogicProperties::GlobalObj)->SettlerUpgrades.SaveVector();
		while (ToRemoveSettler.size() != 0) {
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
		while (ToRemoveBuilding.size() != 0) {
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
void CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::RegisterFuncs(luaext::EState L)
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
int CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::Prel(lua::State L)
{
	return Obj.Preload(luaext::EState{ L });
}
int CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::AddSettlerUpgradeCategory(lua::State l)
{
	luaext::EState L{ l };
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
		auto* cty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ctype);
		auto* sty = dynamic_cast<GGL::CGLSettlerProps*>(cty->LogicProps);
		if (sty == nullptr)
			throw lua::LuaException{ "non settler type in settler ucat" };
		sty->Upgrade.Category = id;
		ctype = sty->Upgrade.Type;
	}
	// players are not created yet, so the upgrademanagers in there read the correct data from GGL::CLogicProperties::GlobalObj
	Obj.ToRemoveSettler.push_back(id);
	L.Push(id);
	return 1;
}
int CppLogic::ModLoader::ModLoader::UpgradeCategoriesLoader::AddBuildingUpgradeCategory(lua::State l)
{
	luaext::EState L{ l };
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
		auto* cty = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(ctype);
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

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::WaterTypeId>::Load(shok::WaterTypeId id, luaext::EState L) {
	(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->LoadWaterTypeFromExtraFile(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::WaterTypeId>::TableName() {
	return "WaterTypes";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::WaterTypeId>::FuncName() {
	return "WaterType";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId>::SanityCheck() {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager;
	auto idm = CppLogic::GetIdManager<shok::WaterTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (static_cast<int>(mng->Logic.WaterLogic.size()) <= static_cast<int>(id))
			throw lua::LuaException{ std::format("watertype {}={} missing", n, static_cast<int>(id)) };
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId>::Reset() {
	if (NeedsReload)
		(*Framework::CMain::GlobalObj)->GluePropsManager->WaterPropsManager->ReloadWaterTypes();
	NeedsReload = false;
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::WaterTypeId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::SelectionTextureId>::Load(shok::SelectionTextureId id, luaext::EState L) {
	auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures;
	const char* n = CppLogic::GetIdManager<shok::SelectionTextureId>().GetNameByID(id);
	auto d = mng->Get(n);
	d->FreeCache(); // nop if empty
	d->Get();
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::SelectionTextureId>::TableName() {
	return nullptr;
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::SelectionTextureId>::FuncName() {
	return "SelectionTexture";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId>::SanityCheck() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId>::UnLoad(shok::SelectionTextureId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->SelectionTextures->PopId(static_cast<int>(id));
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::SelectionTextureId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTextureId>::Load(shok::TerrainTextureId id, luaext::EState L) {
	auto idm = CppLogic::GetIdManager<shok::TerrainTextureId>();
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->LoadTexture(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTextureId>::TableName() {
	return nullptr;
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTextureId>::FuncName() {
	return "TerrainTexture";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId>::SanityCheck() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId>::UnLoad(shok::TerrainTextureId id) {
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->PopTexture(id);
	CppLogic::GetIdManager<shok::TerrainTextureId>().RemoveID(id);
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::TerrainTextureId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTypeId>::Load(shok::TerrainTypeId id, luaext::EState L) {
	(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->LoadTerrainTypeFromExtraFile(id);
	(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyTerrainType(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTypeId>::TableName() {
	return "TerrainTypes";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::TerrainTypeId>::FuncName() {
	return "TerrainType";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId>::SanityCheck() {
	auto* mng = (*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager;
	auto idm = CppLogic::GetIdManager<shok::TerrainTypeId>();
	for (auto id : idm) {
		auto n = idm.GetNameByID(id);
		if (static_cast<int>(mng->Logic.LogicProps.size()) <= static_cast<int>(id))
			throw lua::LuaException{ std::format("terraintype {}={} missing", n, static_cast<int>(id)) };
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId>::Reset() {
	if (NeedsReload) {
		(*Framework::CMain::GlobalObj)->GluePropsManager->TerrainPropsManager->ReloadTerrainTypes();
		(*ED::CGlobalsBaseEx::GlobalObj)->TerrainManager->TextureManager->ReApplyAllTerrainTypes();
	}
	NeedsReload = false;
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId> CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::TerrainTypeId>::Obj{};

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityCategory>::Load(shok::EntityCategory id, luaext::EState L) {

}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityCategory>::TableName() {
	return "EntityCategories";
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::EntityCategory>::FuncName() {
	return "EntityCategory";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::SanityCheck() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::Reload() {

}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::UnLoad(shok::EntityCategory id) {
	CppLogic::GetIdManager<shok::EntityCategory>().RemoveID(id);
}
template<>
constexpr bool CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::RegisterFuncLoad = false;
CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory> CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::Obj{};

void CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::Reset()
{
	while (ToRemove.size() != 0) {
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
void CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::RegisterFuncs(luaext::EState L)
{
	L.RegisterFunc<Add>("AddExperienceClass", -3);
	L.RegisterFunc<Add>("ReloadExperienceClass", -3);
}
int CppLogic::ModLoader::ModLoader::ExperienceClassesLoader::Add(lua::State L)
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
	shok::EntityCategory cat = L.IsNumber(2) ? static_cast<shok::EntityCategory>(L.CheckInt(2)) : luaext::EState{ L }.CheckEnum<shok::EntityCategory>(2);
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
	while (ToRemove.size() != 0) {
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
void CppLogic::ModLoader::ModLoader::SoundGroupsLoader::RegisterFuncs(luaext::EState L)
{
	L.RegisterFunc<Add>("AddSounds", -3);
}
int CppLogic::ModLoader::ModLoader::SoundGroupsLoader::Add(lua::State l)
{
	luaext::EState L{ l };
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

void CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimSetId>::Load(shok::AnimSetId id, luaext::EState L) {
	auto* mng = *EGL::AnimSetManager::GlobalObj;
	if (static_cast<int>(id) < static_cast<int>(mng->AnimSets.size()))
		mng->FreeAnimSet(id);
	mng->LoadAnimSet(id);
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimSetId>::TableName() {
	return nullptr;
}
const char* CppLogic::ModLoader::ModLoader::DataTypeLoaderCommon<shok::AnimSetId>::FuncName() {
	return "AnimSet";
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId>::SanityCheck() {
	auto* mng = *EGL::AnimSetManager::GlobalObj;
	auto idm = CppLogic::GetIdManager<shok::AnimSetId>();
	for (auto id : idm) {
		if (static_cast<int>(id) > static_cast<int>(mng->AnimSets.size()) || mng->GetAnimSet(id) == nullptr)
			throw lua::LuaException{ std::format("animset {}={} does not exist", idm.GetNameByID(id), static_cast<int>(id)) };
	}
}
void CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId>::UnLoad(shok::AnimSetId id) {
	(*EGL::AnimSetManager::GlobalObj)->PopAnimSet(id);
	CppLogic::GetIdManager<shok::AnimSetId>().RemoveID(id);
}
CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId> CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::AnimSetId>::Obj{};

void CppLogic::ModLoader::ModLoader::DirectXEffectLoader::Reset()
{
	if (ToRemove.size() > 0) {
		for (int id = 1; id < static_cast<int>((*ED::CGlobalsBaseEx::GlobalObj)->ResManager->ModelManager.ModelIDManager->size()); ++id) {
			(*ED::CGlobalsBaseEx::GlobalObj)->ResManager->FreeModel(static_cast<shok::ModelId>(id));
			// gets reloaded on next use anyway
		}
		auto* mng = (*ED::CGlobalsBaseEx::GlobalObj)->RWEngine->Effects;
		while (ToRemove.size() > 0) {
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
void CppLogic::ModLoader::ModLoader::DirectXEffectLoader::RegisterFuncs(luaext::EState L)
{
	L.RegisterFunc<Add>("LoadDirectXEffect", -3);
}
int CppLogic::ModLoader::ModLoader::DirectXEffectLoader::Add(lua::State L)
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

std::array<CppLogic::ModLoader::ModLoader::DataTypeLoader*, 19> CppLogic::ModLoader::ModLoader::Loaders{ {
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
	} };
std::array<CppLogic::ModLoader::ModLoader::DataTypeLoader*, 1> CppLogic::ModLoader::ModLoader::LoadersIngame{ {
		&CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::GUITextureId>::Obj,
	} };


bool CppLogic::ModLoader::ModLoader::Initialized = false;

int CppLogic::ModLoader::ModLoader::SetEntityTypeToReload(lua::State L)
{
	auto id = luaext::EState{ L }.CheckEnum<shok::EntityTypeId>(1);
	CppLogic::ModLoader::ModLoader::DataTypeLoaderTracking<shok::EntityTypeId>::Obj.OnIdLoaded(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::SetTaskListToReload(lua::State L)
{
	auto id = luaext::EState{ L }.CheckEnum<shok::TaskListId>(1);
	CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::TaskListId>::Obj.OnIdLoaded(id);
	return 0;
}

int CppLogic::ModLoader::ModLoader::SetDamageclassesToReload(lua::State L)
{
	CppLogic::ModLoader::ModLoader::DataTypeLoaderReload<shok::DamageClassId>::Obj.OnIdLoaded(shok::DamageClassId::Invalid);
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

void CppLogic::ModLoader::ModLoader::InitExtraECats()
{
	auto m = CppLogic::GetIdManager<shok::EntityCategory>();
	for (shok::EntityCategory ec : {shok::EntityCategory::TargetFilter_TargetType, shok::EntityCategory::TargetFilter_TargetTypeLeader,
		shok::EntityCategory::TargetFilter_CustomRanged, shok::EntityCategory::TargetFilter_NonCombat}) {
		m.GetIDByNameOrCreate(magic_enum::enum_name(ec).data(), ec);
		CppLogic::ModLoader::ModLoader::DataTypeLoaderHalf<shok::EntityCategory>::Obj.OnIdAllocated(ec);
	}
}


int CppLogic::ModLoader::ModLoader::GetModpackInfo(lua::State L)
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

int CppLogic::ModLoader::ModLoader::LoadModpackBBA(lua::State L)
{
	auto p = GetModPackPath(L.CheckStringView(1));
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	mng->AddArchive(p.c_str());
	if (mng->LoadOrder.size() >= 2) {
		if (auto* ar = dynamic_cast<BB::CBBArchiveFile*>(mng->LoadOrder[1])) {
			if (std::string_view{ ar->ArchiveFile.Filename }.ends_with(".s5x")) {
				mng->LoadOrder[1] = mng->LoadOrder[0];
				mng->LoadOrder[0] = ar;
			}
		}
	}
	L.NewUserClass<ArchivePopHelper>(std::move(p));
	return 1;
}

int CppLogic::ModLoader::ModLoader::InvalidModPackPanic(lua::State L)
{
	auto m = L.CheckStringView(1);
	MessageBoxA(0, m.data(), "ModLoader failure", MB_OK);
	std::exit(0);
	return 0;
}

int CppLogic::ModLoader::ModLoader::GetModpacks(lua::State L)
{
	L.NewTable();
	std::filesystem::directory_iterator it{ std::filesystem::path(ModpackFolder) };
	int k = 1;
	for (const auto& i : it) {
		if (i.is_regular_file()) {
			auto p = i.path();
			if (p.extension().string() == ".bba") {
				L.Push(p.stem().string());
				L.SetTableRaw(-2, k);
				++k;
			}
		}
	}
	return 1;
}

int CppLogic::ModLoader::ModLoader::ReserializeEntityType(lua::State l)
{
	luaext::EState L{ l };
	CppLogic::WinAPI::FileDialog dlg{};
	auto etyid = L.CheckEnum<shok::EntityTypeId>(1);
	auto* ety = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(etyid);
	auto t = std::format("Write {}", CppLogic::GetIdManager<shok::EntityTypeId>().GetNameByID(etyid));
	dlg.Title = t.c_str();
	dlg.Filter = "xml\0*.xml\0";
	dlg.SelectedPath = L.OptStringView(2, "");
	if (dlg.Show()) {
		BB::CFileStreamEx f{};
		if (f.OpenFile(dlg.SelectedPath.c_str(), BB::IStream::Flags::DefaultWrite)) {
			BB::CXmlSerializer::HookWriteXSIType();
			auto s = BB::CXmlSerializer::CreateUnique();
			s->SerializeByData(&f, ety, (*BB::CClassFactory::GlobalObj)->GetSerializationDataForClass(ety->GetClassIdentifier()));
			f.Close();
		}
	}
	return 0;
}

int CppLogic::ModLoader::ModLoader::SetModPackList(lua::State L)
{
	ModPackList = L.CheckStringView(1);
	return 0;

}

int CppLogic::ModLoader::ModLoader::GetModPackList(lua::State L)
{
	L.Push(ModPackList);
	return 1;
}

lua::Reference CppLogic::ModLoader::ModLoader::SavegameValidOverride{};
int CppLogic::ModLoader::ModLoader::OverrideSavegameValid(lua::State L)
{
	if (!L.IsFunction(1))
		throw lua::LuaException{ "no func at 1" };
	L.UnRef(SavegameValidOverride);
	L.PushValue(1);
	SavegameValidOverride = L.Ref();
	Framework::SavegameSystem::IsSaveValidOverride = [](std::string_view save) {
		lua::State L{ shok::LuaStateMainmenu };
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
		lua::State L{ *EScr::CScriptTriggerSystem::GameState };
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

		Log(L, "Done");
	}
}

void CppLogic::ModLoader::ModLoader::InitMainmenu(lua::State L)
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

CppLogic::SerializationListOptions_ForVector<std::string> stringvect{};
const BB::SerializationData CppLogic::ModLoader::ModpackDesc::SerializationData[]{
	BB::SerializationData::FieldData("LoaderPath", MemberSerializationSizeAndOffset(ModpackDesc, LoaderPath), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::FieldData("ScriptPath", MemberSerializationSizeAndOffset(ModpackDesc, ScriptPath), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::FieldData("Version", MemberSerializationSizeAndOffset(ModpackDesc, Version), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::FieldData("Required", MemberSerializationSizeAndOffset(ModpackDesc, Required), &CppLogic::StringSerializer::GlobalObj, &stringvect),
	BB::SerializationData::FieldData("Incompatible", MemberSerializationSizeAndOffset(ModpackDesc, Incompatible), &CppLogic::StringSerializer::GlobalObj, &stringvect),
	BB::SerializationData::FieldData("Override", MemberSerializationSizeAndOffset(ModpackDesc, Override), &CppLogic::StringSerializer::GlobalObj, &stringvect),
	BB::SerializationData::FieldData("DataMod", MemberSerializationFieldData(ModpackDesc, DataMod)),
	BB::SerializationData::FieldData("ScriptMod", MemberSerializationFieldData(ModpackDesc, ScriptMod)),
	BB::SerializationData::FieldData("MainmenuMod", MemberSerializationFieldData(ModpackDesc, MainmenuMod)),
	BB::SerializationData::FieldData("KeepArchive", MemberSerializationFieldData(ModpackDesc, KeepArchive)),
	BB::SerializationData::FieldData("UserRequestable", MemberSerializationFieldData(ModpackDesc, UserRequestable)),
	BB::SerializationData::GuardData(),
};
const BB::SerializationData CppLogic::ModLoader::ModpackDesc::SerializationDataEx[]{
	BB::SerializationData::EmbeddedData(nullptr, 0, sizeof(ModpackDesc), SerializationData),
	BB::SerializationData::FieldData("Name", MemberSerializationSizeAndOffset(ModpackDesc, Name), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::FieldData("BBAPath", MemberSerializationSizeAndOffset(ModpackDesc, BBAPath), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::GuardData(),
};

int CppLogic::ModLoader::ArchivePopHelper::Remove(lua::State L)
{
	auto t = L.CheckUserClass<ArchivePopHelper>(1);
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	auto v = mng->LoadOrder.SaveVector();
	auto it = v.Vector.begin();
	while (it != v.Vector.end()) {
		if (auto* a = dynamic_cast<BB::CBBArchiveFile*>(*it)) {
			if (a->ArchiveFile.Filename == t->Archive) {
				a->Destroy();
				v.Vector.erase(it);
				L.Push(true);
				return 1;
			}
		}
		++it;
	}
	L.Push(false);
	return 1;
}

int CppLogic::ModLoader::ArchivePopHelper::IsLoaded(lua::State L)
{
	auto t = L.CheckUserClass<ArchivePopHelper>(1);
	auto* mng = *BB::CFileSystemMgr::GlobalObj;
	for (const auto* f : mng->LoadOrder) {
		if (const auto* a = dynamic_cast<const BB::CBBArchiveFile*>(f)) {
			if (a->ArchiveFile.Filename == t->Archive) {
				L.Push(true);
				return 1;
			}
		}
	}
	L.Push(false);
	return 1;
}

int CppLogic::ModLoader::ArchivePopHelper::ToString(lua::State L)
{
	auto t = L.CheckUserClass<ArchivePopHelper>(1);
	L.Push(t->Archive);
	return 1;
}

#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_defines.h"
#include "s5_framework.h"
#include "luaext.h"


namespace CppLogic::ModLoader {
	class ModLoader {
		static void Init(lua::State L, const char* mappath, const char* func);
		static void PreMapStart(lua_State* ingame, const char* name, const char* path, bool externalmap);
		static void PreSaveLoad(lua_State* ingame, Framework::GameModeStartMapData* data, bool externalmap);
		static void Log(lua::State L, const char* log);
		static void AddLib(lua::State L);
		static void RemoveLib(lua::State L);

		static bool Initialized;
		static std::vector<shok::EntityTypeId> EntityTypesToRemove;
		static std::vector<shok::EntityTypeId> EntityTypesToReload;
		static bool ReloadEffectTypes;
		static std::vector<shok::TaskListId> TaskListsToRemove;
		static std::vector<shok::TechnologyId> TechsToRemove;
		static std::vector<shok::ModelId> ModelsToRemove;
		static bool ReloadModels;
		static std::vector<shok::GUITextureId> TexturesToRemove;
		static std::vector<shok::GUITextureId> TexturesToReload;
		static std::vector<shok::AnimationId> AnimsToRemove;
		static std::vector<shok::AnimationId> AnimsToReload;
		static std::vector<shok::UpgradeCategoryId> SettlerUCatsToRemove;
		static std::vector<shok::UpgradeCategoryId> BuildingUCatsToRemove;
		static bool ReloadWaterTypes;
		static std::vector<int> SelectionTexturesToRemove;
		static std::vector<int> SelectionTexturesToReload;
		static std::vector<shok::TerrainTextureId> TerrainTexturesToRemove;
		static std::vector<shok::TerrainTextureId> TerrainTexturesToReload;
		static bool ReloadTerrainTypes;
		static std::vector<shok::ExperienceClass> ExperienceClassesToRemove;
		static std::vector<shok::ExperienceClass> ExperienceClassesToReload;
		static std::vector<shok::SoundId> SoundGroupsToRemove;
		static std::vector<shok::AnimSetId> AnimSetsToRemove;
		static std::vector<shok::AnimSetId> AnimSetsToReload;
		static std::vector<int> DirectXEffectsToFree;

		static int PreLoadEntityType(lua::State L);
		static int AddEntityType(lua::State L);
		static int ReloadEntityType(lua::State L);
		static int SetEntityTypeToReload(lua::State L);
		static int PreLoadEffectType(lua::State L);
		static int ReloadEffectType(lua::State L);
		static int AddEffectType(lua::State L);
		static int PreLoadTaskList(lua::State L);
		static int ReloadTaskList(lua::State L);
		static int AddTaskList(lua::State L);
		static int PreLoadTechnology(lua::State L);
		static int ReloadTechnology(lua::State L);
		static int AddTechnology(lua::State L);
		static int AddModel(lua::State L);
		static int ReloadModel(lua::State L);
		static int AddGUITexture(lua::State L);
		static int ReloadGUITexture(lua::State L);
		static int AddAnimation(lua::State L);
		static int ReloadAnimation(lua::State L);
		static int PreLoadUpgradeCategory(lua::State L);
		static int AddSettlerUpgradeCategory(lua::State L);
		static int AddBuildingUpgradeCategory(lua::State L);
		static int AddWaterType(lua::State L);
		static int ReloadWaterType(lua::State L);
		static int AddSelectionTexture(lua::State L);
		static int ReloadSelectionTexture(lua::State L);
		static int AddTerrainTexture(lua::State L);
		static int ReloadTerrainTexture(lua::State L);
		static int AddTerrainType(lua::State L);
		static int ReloadTerrainType(lua::State L);
		static int AddExperienceClass(lua::State L);
		static int ReloadExperienceClass(lua::State L);
		static int AddSounds(lua::State L);
		static int AddAnimSet(lua::State L);
		static int ReloadAnimSet(lua::State L);
		static int LoadDirectXEffect(lua::State L);
		static int RefreshEntityCategoryCache(lua::State L);

		static constexpr std::array<lua::FuncReference, 37> LuaFuncs{ {
				lua::FuncReference::GetRef<PreLoadEntityType>("PreLoadEntityType"),
				lua::FuncReference::GetRef<AddEntityType>("AddEntityType"),
				lua::FuncReference::GetRef<ReloadEntityType>("ReloadEntityType"),
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<ReloadEffectType>("ReloadEffectType"),
				lua::FuncReference::GetRef<PreLoadEffectType>("PreLoadEffectType"),
				lua::FuncReference::GetRef<AddEffectType>("AddEffectType"),
				lua::FuncReference::GetRef<PreLoadTaskList>("PreLoadTaskList"),
				lua::FuncReference::GetRef<ReloadTaskList>("ReloadTaskList"),
				lua::FuncReference::GetRef<AddTaskList>("AddTaskList"),
				lua::FuncReference::GetRef<PreLoadTechnology>("PreLoadTechnology"),
				lua::FuncReference::GetRef<ReloadTechnology>("ReloadTechnology"),
				lua::FuncReference::GetRef<AddTechnology>("AddTechnology"),
				lua::FuncReference::GetRef<AddModel>("AddModel"),
				lua::FuncReference::GetRef<ReloadModel>("ReloadModel"),
				lua::FuncReference::GetRef<AddGUITexture>("AddGUITexture"),
				lua::FuncReference::GetRef<ReloadGUITexture>("ReloadGUITexture"),
				lua::FuncReference::GetRef<AddAnimation>("AddAnimation"),
				lua::FuncReference::GetRef<ReloadAnimation>("ReloadAnimation"),
				lua::FuncReference::GetRef<PreLoadUpgradeCategory>("PreLoadUpgradeCategory"),
				lua::FuncReference::GetRef<AddSettlerUpgradeCategory>("AddSettlerUpgradeCategory"),
				lua::FuncReference::GetRef<AddBuildingUpgradeCategory>("AddBuildingUpgradeCategory"),
				lua::FuncReference::GetRef<AddWaterType>("AddWaterType"),
				lua::FuncReference::GetRef<ReloadWaterType>("ReloadWaterType"),
				lua::FuncReference::GetRef<AddSelectionTexture>("AddSelectionTexture"),
				lua::FuncReference::GetRef<ReloadSelectionTexture>("ReloadSelectionTexture"),
				lua::FuncReference::GetRef<AddTerrainTexture>("AddTerrainTexture"),
				lua::FuncReference::GetRef<ReloadTerrainTexture>("ReloadTerrainTexture"),
				lua::FuncReference::GetRef<AddTerrainType>("AddTerrainType"),
				lua::FuncReference::GetRef<ReloadTerrainType>("ReloadTerrainType"),
				lua::FuncReference::GetRef<AddExperienceClass>("AddExperienceClass"),
				lua::FuncReference::GetRef<ReloadExperienceClass>("ReloadExperienceClass"),
				lua::FuncReference::GetRef<AddSounds>("AddSounds"),
				lua::FuncReference::GetRef<AddAnimSet>("AddAnimSet"),
				lua::FuncReference::GetRef<ReloadAnimSet>("ReloadAnimSet"),
				lua::FuncReference::GetRef<LoadDirectXEffect>("LoadDirectXEffect"),
				lua::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
		} };

		static constexpr std::array<lua::FuncReference, 4> NoLoaderFuncs{ {
				lua::FuncReference::GetRef<SetEntityTypeToReload>("SetEntityTypeToReload"),
				lua::FuncReference::GetRef<AddGUITexture>("AddGUITexture"),
				lua::FuncReference::GetRef<ReloadGUITexture>("ReloadGUITexture"),
				lua::FuncReference::GetRef<RefreshEntityCategoryCache>("RefreshEntityCategoryCache"),
		} };

	public:
		static void Initialize();
		static void Cleanup(Framework::CMain::NextMode n);

		static bool IsInitialized();
		static void AddTaskListToRemove(shok::TaskListId id);
	};
}

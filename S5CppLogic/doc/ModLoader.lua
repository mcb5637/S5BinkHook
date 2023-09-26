CppLogic.API.CreateExtraDataTables()

--- ModLoader tries to load the file ModLoader.lua in your map folder.
-- this happens on starting a new map as well as loading a savegame.
-- make sure, nothing breaks, if this file gets loaded again.
ModLoader = {
	--- if this flag is set, then the s5x archive is kept in the internal filesystem, after the map is loaded.
	--- this is not required for most ModLoader functionality, but you might want to load something out of it at a later time.
	--- you need this, if you have sounds packed in your s5x.
	--- also please set it, if you load another archive (and using a s5x).
	KeepArchive = false,
	--- anything in Manifest gest applied by calling ModLoader.ApplyManifest() from ModLoader.Initialize()
	--- order in which items appear in here do not matter, ModLoader.ApplyManifest() does take care of preloading.
	--- unless otherwise specified, using id or name is supported (obviously only name for adding).
	--- ModLoader now automatically figures out what needs to be added or reloaded.
	Manifest = {
		--- EntityTypes to load or reload.
		EntityTypes = {

		},
		--- EffectTypes to load or reload.
		EffectTypes = {

		},
		--- TaskLists to load or reload.
		TaskLists = {

		},
		--- Technologies to load or reload.
		Technologies = {

		},
		--- Experience classes to load. use [nameOfExperienceClass]=entityCategory.
		ExperienceClasses = {

		},
		--- Models to load or reload.
		Models = {

		},
		--- Animations to load or reload.
		Animations = {

		},
		--- AnimSets to load or reload.
		AnimSets = {

		},
		--- Selection Textures to add or reload.
		SelectionTextures_Add = {

		},
		--- deprecated
		SelectionTextures_Reload = {},
		--- GUI Textures to add or reload.
		GUITextures_Add = {

		},
		--- deprecated
		GUITextures_Reload = {},
		--- Settler Upgradde Categories to add. use in [nameUCat]=nameFirstEntity.
		SettlerUpgradeCategory = {

		},
		--- Building Upgradde Categories to add. use in [nameUCat]=nameFirstEntity.
		BuildingUpgradeCategory = {

		},
		--- Terrain Textures to add or reload.
		TerrainTextures_Add = {

		},
		--- deprecated
		TerrainTextures_Reload = {},
		--- Water Types to load or reload.
		WaterTypes = {

		},
		--- Terrain Types to load or reload.
		TerrainTypes = {

		},
		--- Sound (Groups) to load. use arrays of names to load (requires keeping the bba/s5x that contains them loaded).
		SoundGroups = {

		},
		--- DirectX Effects to load. no difference between load and reload.
		DirectXEffects = {

		},
		-- ArmorClasses to add. (Factors are defined in DamageClasses) (PreLoad only)
		ArmorClasses = {

		},
		-- DamageClasses to add/override.
		DamageClasses = {

		},
		--- EntityCategories to add. (PreLoad only)
		EntityCategories = {

		},
	},
}

--- this function gets called after ModLoader loads this file.
-- called on both map start and save load.
-- this is the place to modify something like EntityTypes, as all entities get created after this method,
-- so even changing behaviors completely works without problems.
-- it is recommended, that you edit data required by others first. so as an example, first add a model, and after that the entitytytpe requiring it (Manifest does that for you).
function ModLoader.Initialize()
	--- applying everything in Manifest
	ModLoader.ApplyManifest()

	--- manual code should go here


	--- refresh caches
	CppLogic.ModLoader.RefreshEntityCategoryCache()
	CppLogic.ModLoader.SanityCheck()
end

--- this function gets called after Initialize, only if the map starts fresh.
function ModLoader.MapStart()

end

--- this function gets called after Initialize, only if a savegame gets loaded.
function ModLoader.LoadSave()

end

--- applying everything in Manifest
function ModLoader.ApplyManifest()
	ModLoader.PreloadManifestType(ModLoader.Manifest.ArmorClasses, CppLogic.ModLoader.PreLoadArmorClass, ArmorClasses)
	ModLoader.PreloadManifestType(ModLoader.Manifest.EffectTypes, CppLogic.ModLoader.PreLoadEffectType, GGL_Effects)
	ModLoader.PreloadManifestType(ModLoader.Manifest.TaskLists, CppLogic.ModLoader.PreLoadTaskList, TaskLists)
	ModLoader.PreloadManifestType(ModLoader.Manifest.EntityTypes, CppLogic.ModLoader.PreLoadEntityType, Entities)
	ModLoader.PreloadManifestType(ModLoader.Manifest.Technologies, CppLogic.ModLoader.PreLoadTechnology, Technologies)
	ModLoader.PreloadManifestType(ModLoader.Manifest.EntityCategories, CppLogic.ModLoader.PreLoadEntityCategory,
		EntityCategories)
	for dc, cfg in pairs(ModLoader.Manifest.DamageClasses) do
		CppLogic.ModLoader.AddDamageClass(dc, cfg)
	end
	for uc, et in pairs(ModLoader.Manifest.SettlerUpgradeCategory) do
		CppLogic.ModLoader.PreLoadUpgradeCategory(uc)
	end
	for uc, et in pairs(ModLoader.Manifest.BuildingUpgradeCategory) do
		CppLogic.ModLoader.PreLoadUpgradeCategory(uc)
	end

	for _, n in ipairs(ModLoader.Manifest.DirectXEffects) do
		CppLogic.ModLoader.LoadDirectXEffect(n)
	end
	for _, n in ipairs(ModLoader.Manifest.TerrainTextures_Add) do
		CppLogic.ModLoader.AddTerrainTexture(n)
	end
	for _, n in ipairs(ModLoader.Manifest.TerrainTextures_Reload) do
		CppLogic.ModLoader.AddTerrainTexture(n)
	end
	ModLoader.HandleManifestType(ModLoader.Manifest.WaterTypes, CppLogic.ModLoader.AddWaterType)
	ModLoader.HandleManifestType(ModLoader.Manifest.TerrainTypes, CppLogic.ModLoader.AddTerrainType)
	for _, n in ipairs(ModLoader.Manifest.SelectionTextures_Add) do
		CppLogic.ModLoader.AddSelectionTexture(n)
	end
	for _, n in ipairs(ModLoader.Manifest.SelectionTextures_Reload) do
		CppLogic.ModLoader.AddSelectionTexture(n)
	end
	ModLoader.HandleManifestType(ModLoader.Manifest.Animations, CppLogic.ModLoader.AddAnimation)
	ModLoader.HandleManifestType(ModLoader.Manifest.AnimSets, CppLogic.ModLoader.AddAnimSet)
	ModLoader.HandleManifestType(ModLoader.Manifest.Models, CppLogic.ModLoader.AddModel)
	ModLoader.HandleManifestType(ModLoader.Manifest.EffectTypes, CppLogic.ModLoader.AddEffectType)
	ModLoader.HandleManifestType(ModLoader.Manifest.TaskLists, CppLogic.ModLoader.AddTaskList)
	ModLoader.HandleManifestType(ModLoader.Manifest.EntityTypes, CppLogic.ModLoader.AddEntityType)
	ModLoader.HandleManifestType(ModLoader.Manifest.Technologies, CppLogic.ModLoader.AddTechnology)
	for _, n in ipairs(ModLoader.Manifest.GUITextures_Add) do
		CppLogic.ModLoader.AddGUITexture(n)
	end
	for _, n in ipairs(ModLoader.Manifest.GUITextures_Reload) do
		CppLogic.ModLoader.AddGUITexture(n)
	end
	for uc, et in pairs(ModLoader.Manifest.SettlerUpgradeCategory) do
		CppLogic.ModLoader.AddSettlerUpgradeCategory(uc, Entities[et])
	end
	for uc, et in pairs(ModLoader.Manifest.BuildingUpgradeCategory) do
		CppLogic.ModLoader.AddBuildingUpgradeCategory(uc, Entities[et])
	end
	for xp, ec in pairs(ModLoader.Manifest.ExperienceClasses) do
		CppLogic.ModLoader.AddExperienceClass(xp, ec)
	end
	for _, a in ipairs(ModLoader.Manifest.SoundGroups) do
		CppLogic.ModLoader.AddSounds(unpack(a))
	end
end

function ModLoader.PreloadManifestType(en, preload, data)
	for _, k in ipairs(en) do
		if type(k) == "string" and not data[k] then
			preload(k)
		end
	end
end

function ModLoader.HandleManifestType(en, add)
	for _, k in ipairs(en) do
		add(k)
	end
end

--- this function gets called when someone leaves a map for any reason.
-- cleanup code should go here.
-- after this method got executed, any extra archives get removed from the filesystem.
-- after that, any data that is marked for cleanup gets removed or reloaded.
-- ModLoader functions automatically mark data for cleanup, but you might have to do some manual cleanup if you use some other CppLogic functions.
-- after the automatic cleanup, ModLoader assumes all data to be back in the original state it was loaded from the games bbas.
function ModLoader.Cleanup()

end

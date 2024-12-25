CppLogic.API.CreateExtraDataTables()

---ModLoader tries to load the file ModLoader.lua in your map folder.
---this happens on starting a new map as well as loading a savegame.
---make sure, nothing breaks, if this file gets loaded again.
---@type ModLoader
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
	--- @type Manifest
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
		SelectionTextures = {

		},
		--- GUI Textures to add or reload.
		GUITextures = {

		},
		--- Settler Upgradde Categories to add. use in [nameUCat]=nameFirstEntity.
		SettlerUpgradeCategory = {

		},
		--- Building Upgradde Categories to add. use in [nameUCat]=nameFirstEntity.
		BuildingUpgradeCategory = {

		},
		--- Terrain Textures to add or reload.
		TerrainTextures = {

		},
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
		--- StringTableTexts to add/override. enter the file name as key, value is the STT prefix, true->same as file
		StringTableTexts = {

		},
	},
	--- the names of all ModPacks requied by this map. a call to ModLoader.RequireModList() in Initialize will then try to load them.
	--- @type string[]
	RequiredMods = {},
}

--- this function gets called after ModLoader loads this file.
-- called on both map start and save load.
-- this is the place to modify something like EntityTypes, as all entities get created after this method,
-- so even changing behaviors completely works without problems.
function ModLoader.Initialize()
	--- search and apply any modpacks requested
	ModLoader.RequireModList()
	--- applying everything in Manifest
	ModLoader.ApplyManifest()

	--- manual code should go here

	--- refresh caches
	CppLogic.ModLoader.RefreshEntityCategoryCache()
	CppLogic.ModLoader.SanityCheck()
	--- cleanup modpacks
	ModLoader.CleanupMods(ModLoader.ModList)
end

--- this function gets called after Initialize, only if the map starts fresh.
function ModLoader.MapStart()

end

--- this function gets called after Initialize, only if a savegame gets loaded.
function ModLoader.LoadSave()

end

--- this function checks if a user requested mod can be added to the map.
--- @param modname string
--- @return boolean
function ModLoader.CheckUserRequestedMod(modname)
	return ModLoader.IsUserRequestedModWhitelisted(modname)
end

--- this function gets called when someone leaves a map for any reason.
-- cleanup code should go here.
-- after this method got executed, any extra archives get removed from the filesystem.
-- after that, any data that is marked for cleanup gets removed or reloaded.
-- ModLoader functions automatically mark data for cleanup, but you might have to do some manual cleanup if you use some other CppLogic functions.
-- after the automatic cleanup, ModLoader assumes all data to be back in the original state it was loaded from the games bbas.
function ModLoader.Cleanup()

end

--- load CppLogic funcs
Script.Load("Data\\Script\\ModLoader\\ModLoader.lua")

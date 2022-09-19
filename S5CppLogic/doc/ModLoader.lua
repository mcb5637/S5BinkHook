CppLogic.API.CreateExtraDataTables()

--- ModLoader tries to load the file ModLoader.lua in your map folder.
-- this happens on starting a new map as well as loading a savegame.
-- make sure, nothing breaks, if this file gets loaded again.
ModLoader = {
	--- if this flag is set, then the s5x archive is kept in the internal filesystem, after the map is loaded.
	-- this is not required for most ModLoader functionality, but you might want to load something out of it at a later time.
	--- you need this, if you have sounds packed in your s5x.
	-- also please set it, if you load another archive (and using a s5x).
	KeepArchive = false,
	--- anything in Manifest gest applied by calling ModLoader.ApplyManifest() from ModLoader.Initialize()
	--- order in which items appear in here do not matter, ModLoader.ApplyManifest() does take care of preloading.
	Manifest = {
		--- EntityTypes to load or reload. put in a string to load, and a number to reload (Entities.Typename).
		EntityTypes = {

		},
		--- EffectTypes to load or reload. put in a string to load, and a number to reload (GGL_Effects.Typename).
		EffectTypes = {

		},
		--- TaskLists to load or reload. put in a string to load, and a number to reload (TaskLists.Typename).
		TaskLists = {

		},
		--- Technologies to load or reload. put in a string to load, and a number to reload (Technologies.Typename).
		Technologies = {

		},
		--- Experience classes to load. use [nameOfExperienceClass]=entityCategory.
		ExperienceClasses = {

		},
		--- Models to load or reload. put in a string to load, and a number to reload (Models.Typename).
		Models = {

		},
		--- Animations to load or reload. put in a string to load, and a number to reload (Animations.Typename).
		Animations = {

		},
		--- AnimSets to load or reload. put in a string to load, and a number to reload (AnimSets.Typename).
		AnimSets = {

		},
		--- Selection Textures to add.
		SelectionTextures_Add = {

		},
		--- Selection Textures to reload.
		SelectionTextures_Reload = {

		},
		--- GUI Textures to add.
		GUITextures_Add = {

		},
		--- GUI Textures to reload.
		GUITextures_Reload = {

		},
		--- Settler Upgradde Categories to add. use in [nameUCat]=nameFirstEntity.
		SettlerUpgradeCategory = {

		},
		--- Building Upgradde Categories to add. use in [nameUCat]=nameFirstEntity.
		BuildingUpgradeCategory = {

		},
		--- Terrain Textures to add.
		TerrainTextures_Add = {

		},
		--- Terrain Textures to reload.
		TerrainTextures_Reload = {

		},
		--- Water Types to load or reload. put in a string to load, and a number to reload (WaterTypes.Typename).
		WaterTypes = {

		},
		--- Terrain Types to load or reload. put in a string to load, and a number to reload (TerrainTypes.Typename).
		TerrainTypes = {

		},
		--- Sound (Groups) to load. use arrays of names to load.
		SoundGroups = {

		},
	},
}

--- this function gets called after ModLoader loads this file.
-- called on both map start and save load.
-- this is the place to modify something like EntityTypes, as all entities get created after this method,
-- so even changing behaviors completely works without problems.
-- it is recommended, that you edit data required by others first. so as an example, first add an model, and after that the entitytytpe requiring it.
function ModLoader.Initialize()
	--- applying everything in Manifest
	ModLoader.ApplyManifest()

	--- manual code should go here
end

--- this function gets called after Initialize, only if the map starts fresh.
function  ModLoader.MapStart()
	
end

--- this function gets called after Initialize, only if a savegame gets loaded.
function ModLoader.LoadSave()
	
end

--- applying everything in Manifest
function ModLoader.ApplyManifest()
	for _,n in ipairs(ModLoader.Manifest.EffectTypes) do
		if type(n)=="string" then
			CppLogic.ModLoader.PreLoadEffectType(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.TaskLists) do
		if type(n)=="string" then
			CppLogic.ModLoader.PreLoadTaskList(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.EntityTypes) do
		if type(n)=="string" then
			CppLogic.ModLoader.PreLoadEntityType(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.Technologies) do
		if type(n)=="string" then
			CppLogic.ModLoader.PreLoadTechnology(n)
		end
	end
	
	for _,n in ipairs(ModLoader.Manifest.TerrainTextures_Add) do
		CppLogic.ModLoader.AddTerrainTexture(n)
	end
	for _,n in ipairs(ModLoader.Manifest.TerrainTextures_Reload) do
		CppLogic.ModLoader.ReloadTerrainTexture(n)
	end
	for _,n in ipairs(ModLoader.Manifest.WaterTypes) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddWaterType(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadWaterType(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.TerrainTypes) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddTerrainType(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadTerrainType(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.SelectionTextures_Add) do
		CppLogic.ModLoader.AddSelectionTexture(n)
	end
	for _,n in ipairs(ModLoader.Manifest.SelectionTextures_Reload) do
		CppLogic.ModLoader.ReloadSelectionTexture(n)
	end
	for _,n in ipairs(ModLoader.Manifest.Animations) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddAnimation(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadAnimation(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.AnimSets) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddAnimSet(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadAnimSet(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.Models) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddModel(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadModel(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.EffectTypes) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddEffectType(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadEffectType(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.TaskLists) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddTaskList(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadTaskList(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.EntityTypes) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddEntityType(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadEntityType(n)
		end
	end
	for _,n in ipairs(ModLoader.Manifest.Technologies) do
		if type(n)=="string" then
			CppLogic.ModLoader.AddTechnology(n)
		elseif type(n)=="number" then
			CppLogic.ModLoader.ReloadTechnology(n)
		end
	end
	
	for _,n in ipairs(ModLoader.Manifest.GUITextures_Add) do
		CppLogic.ModLoader.AddGUITexture(n)
	end
	for _,n in ipairs(ModLoader.Manifest.GUITextures_Reload) do
		CppLogic.ModLoader.ReloadGUITexture(n)
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
	for _,a in ipairs(ModLoader.Manifest.SoundGroups) do
		CppLogic.ModLoader.AddSounds(unpack(a))
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

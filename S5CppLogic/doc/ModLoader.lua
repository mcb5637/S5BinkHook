--- ModLoader tries to load the file ModLoader.lua in your map folder.
-- this happens on starting a new map as well as loading a savegame.
-- make sure, nothing breaks, if this file gets loaded again.
ModLoader = {
	--- if this flag is set, then the s5x archive is kept in the internal filesystem, after the map is loaded.
	-- this is not required for any ModLoader functionality, but you might want to load something out of it at a later time.
	-- also please set it, if you load another archive.
	KeepArchive = false,
}

--- this function gets called after ModLoader loads this file.
-- called on both map start and save load.
-- this is the place to modify something like EntityTypes, as all entities get created after this method,
-- so even changing behaviors completely works without problems.
-- it is recommended, that you edit data required by others first. so as an example, first add an model, and after that the entitytytpe requiring it.
function ModLoader.Initialize()
	
end

--- this function gets called after Initialize, only if the map starts fresh.
function  ModLoader.MapStart()
	
end

--- this function gets called after Initialize, only if a savegame gets loaded.
function ModLoader.LoadSave()
	
end

--- this function gets called when someone leaves a map for any reason.
-- cleanup code should go here.
-- after this method got executed, any extra archives get removed from the filesystem.
-- after that, any data that is marked for cleanup gets removed or reloaded.
-- ModLoader functions automatically mark data for cleanup, but you might have to do some manual cleanup if you use some other CppLogic functions.
-- after the automatic cleanup, ModLoader assumes all data to be back in the original state it was loaded from the games bbas.
function ModLoader.Cleanup()
	
end

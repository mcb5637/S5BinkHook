CppLogic.API.CreateExtraDataTables()

ModLoader = {
	KeepArchive = false,
	Manifest = {
	},
	RequiredMods = {},
}

function ModLoader.Initialize()
	ModLoader.RequireModList()
	ModLoader.ApplyManifest()
	CppLogic.ModLoader.RefreshEntityCategoryCache()
	CppLogic.ModLoader.SanityCheck()
	ModLoader.CleanupMods(ModLoader.ModList)
end

function ModLoader.MapStart()

end

function ModLoader.LoadSave()

end

function ModLoader.CheckUserRequestedMod(modname)
	return ModLoader.IsUserRequestedModWhitelisted(modname)
end

function ModLoader.Cleanup()

end

Script.Load("Data\\Script\\ModLoader\\ModLoader.lua")

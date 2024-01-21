
ModLoader.YourModName = {
	Manifest = {}, -- see ModLoader.lua
}

--- gets called on loading your ModPack.
function ModLoader.YourModName.Init()
	--- merge own manifest into the main mods one
	ModLoader.MergeManifest(ModLoader.Manifest, ModLoader.YourModName.Manifest)
end

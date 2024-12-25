---@diagnostic disable: inject-field
---@type ModPack
ModLoader.YourModName = {
	Manifest = {}, -- see ModLoader.lua
}

---gets called on loading your ModPack.
---@param mp ModpackDesc
function ModLoader.YourModName.Init(mp)
	--- merge own manifest into the main mods one
	ModLoader.MergeManifest(ModLoader.Manifest, ModLoader.YourModName.Manifest)
end

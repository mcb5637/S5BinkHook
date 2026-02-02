---@diagnostic disable: inject-field
---@type ModPack
ModLoader.ExtraPlayers = {
	Manifest = {
	},
}

---gets called on loading your ModPack.
---@param mp ModpackDesc
function ModLoader.ExtraPlayers.Init(mp)
	--- merge own manifest into the main mods one
	ModLoader.MergeManifest(ModLoader.Manifest, ModLoader.ExtraPlayers.Manifest)
end

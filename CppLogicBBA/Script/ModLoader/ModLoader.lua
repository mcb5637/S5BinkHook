ModLoader = ModLoader or {}

--- applying everything in Manifest
function ModLoader.ApplyManifest()
	ModLoader.FillMissingManifestEntries(ModLoader.Manifest)
	ModLoader.PreloadManifestType(ModLoader.Manifest.ArmorClasses, CppLogic.ModLoader.PreLoadArmorClass, ArmorClasses)
	ModLoader.PreloadManifestType(ModLoader.Manifest.EffectTypes, CppLogic.ModLoader.PreLoadEffectType, GGL_Effects)
	ModLoader.PreloadManifestType(ModLoader.Manifest.TaskLists, CppLogic.ModLoader.PreLoadTaskList, TaskLists)
	ModLoader.PreloadManifestType(ModLoader.Manifest.EntityTypes, CppLogic.ModLoader.PreLoadEntityType, Entities)
	ModLoader.PreloadManifestType(ModLoader.Manifest.Technologies, CppLogic.ModLoader.PreLoadTechnology, Technologies)
	ModLoader.PreloadManifestType(ModLoader.Manifest.EntityCategories, CppLogic.ModLoader.PreLoadEntityCategory, EntityCategories)
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
		---@diagnostic disable-next-line: deprecated
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

--- merges all manifest entries from from into into
function ModLoader.MergeManifest(into, from)
	ModLoader.FillMissingManifestEntries(into)
	ModLoader.FillMissingManifestEntries(from)
	local function deref(data, d)
		for k, v in pairs(data) do
			if v == d then
				return k
			end
		end
		return d
	end
	local function contains(t, v)
		for _, d in ipairs(t) do
			if d == v then
				return true
			end
		end
		return false
	end
	for _, k in ipairs {
		"EntityTypes", "EffectTypes", "TaskLists", "Technologies", "Models", "Animations", "AnimSets", "SelectionTextures_Add", "SelectionTextures_Reload",
  "GUITextures_Add", "GUITextures_Reload", "TerrainTextures_Add", "TerrainTextures_Reload", "WaterTypes", "TerrainTypes", "DirectXEffects",
  "ArmorClasses", "EntityCategories"
	} do
		local at = into[k]
		for _, d in ipairs(from[k]) do
			if not contains(at, d) then
				table.insert(at, from)
			end
		end
	end
	for _, k in ipairs {"SettlerUpgradeCategory", "BuildingUpgradeCategory", "DamageClasses"} do
		local at = into[k]
		for u, v in pairs(from[k]) do
			at[u] = v
		end
	end
	do
		local at = into.SoundGroups
		for _, sg in ipairs(from.SoundGroups) do
			local c = false
			for _, s in ipairs(at) do
				if sg[1] == s[1] then
					c = true
				end
			end
			if not c then
				table.insert(at, sg)
			end
		end
	end
end

--- walks a mod list, discovers and adds all requirements
function ModLoader.DiscoverRequired(req, modlist)
	modlist = modlist or {Mods = {}, Incompatible = {}, Missing = {}}
	for _, r in ipairs(req) do
		local found = false
		for _, m in ipairs(modlist.Mods) do
			if m.Name == r then
				found = true
				break
			end
		end
		if not found then
			local m = CppLogic.ModLoader.GetModpackInfo(r)
			if type(m) == "table" then
				table.insert(modlist.Mods, m)
				for _, i in ipairs(m.Incompatible) do
					local f = false
					for _, i2 in ipairs(modlist.Incompatible) do
						if i == i2 then
							f = true
							break
						end
					end
					if not f then
						table.insert(modlist.Incompatible, i)
					end
				end
				ModLoader.DiscoverRequired(m.Required, modlist)
			else
				table.insert(modlist.Missing, r)
			end
		end
	end
	return modlist
end

--- checks a modlist and sorts it
function ModLoader.SortMods(modlist)
	local function contains(t, v)
		for _, d in ipairs(t) do
			if d == v then
				return true
			end
		end
		return false
	end
	if modlist.Missing[1] then
		LuaDebugger.Break()
		CppLogic.ModLoader.InvalidModPackPanic("missing mod")
	end
	for _, m in ipairs(modlist.Mods) do
		if contains(modlist.Incompatible, m.Name) then
			LuaDebugger.Break()
			CppLogic.ModLoader.InvalidModPackPanic("incompatibility found")
		end
	end
	table.sort(modlist.Mods, function(a, b)
		local ab = contains(a.Override, b.Name)
		local ba = contains(b.Override, a.Name)
		if ab and ba then
			LuaDebugger.Break()
			CppLogic.ModLoader.InvalidModPackPanic("circular override")
		end
		if ba then
			return true
		end
		return false
	end)
end

--- loads a ModPack and calls its loader, if present
function ModLoader.LoadMod(mod)
	if type(mod) == "string" then
		mod = CppLogic.ModLoader.GetModpackInfo(mod)
		assert(type(mod) == "table")
	end
	mod.Archive = CppLogic.ModLoader.LoadModpackBBA(mod.Name)
	if mod.LoaderPath ~= "" then
		Script.Load(mod.LoaderPath)
		ModLoader[mod.Name].Init()
	end
	return mod
end

--- loads a modlst
function ModLoader.LoadMods(modlist)
	for _, m in ipairs(modlist.Mods) do
		ModLoader.LoadMod(m)
		if m.LoaderPath ~= "" then
			Script.Load(m.LoaderPath)
		end
	end
end

--- removes no longer needed ModPack bbas
function ModLoader.CleanupMods(modlist, script)
	for _, m in ipairs(modlist.Mods) do
		if m.Archive and not m.KeepArchive and (script or m.ScriptPath == "") then
			m.Archive:Remove()
			m.Archive = nil
		end
	end
end

--- discovers mods and loads them
function ModLoader.RequireModList()
	ModLoader.DiscoverUserRequested(ModLoader.RequiredMods)
	ModLoader.ModList = ModLoader.DiscoverRequired(ModLoader.RequiredMods)
	ModLoader.SortMods(ModLoader.ModList)
	ModLoader.LoadMods(ModLoader.ModList)
end

--- loads script mods
function ModLoader.LoadModScripts()
	for _, m in ipairs(ModLoader.ModList.Mods) do
		if m.ScriptPath ~= "" then
			Script.Load(m.ScriptPath)
		end
	end
	ModLoader.CleanupMods(ModLoader.ModList, true)
end

--- executes ScriptPath of a ModPack, to be used from your mapscript if no ModLoader is present
function ModLoader.LoadScriptMod(name)
	local mod = CppLogic.ModLoader.GetModpackInfo(name)
	assert(mod.ScriptMod)
	assert(not mod.DataMod)
	mod.Archive = CppLogic.ModLoader.LoadModpackBBA(name)
	Script.Load(mod.ScriptPath)
	if not mod.KeepArchive then
		mod.Archive.Remove()
	end
	return mod
end

--- user requested mods allowed by default
function ModLoader.IsUserRequestedModWhitelisted(modname)
	return modname == "test"
end

--- parse user requested mods and add them to the mod list
function ModLoader.DiscoverUserRequested(modlist)
	local str = GDB.GetString("CppLogic\\UserRequestedMods")
	if str and str ~= "" then
		for m in string.gfind(str, "([%w_/\\]+)") do
			if ModLoader.CheckUserRequestedMod(m) then
				table.insert(modlist, m)
			end
		end
	end
end

--- adds empty tables, for compatibility with old modloaders
function ModLoader.FillMissingManifestEntries(manifest)
	if manifest.MissingFilled then
		return
	end
	for _, k in ipairs {
		"EntityTypes", "EffectTypes", "TaskLists", "Technologies", "ExperienceClasses", "Models", "Animations", "AnimSets", "SelectionTextures_Add",
  "SelectionTextures_Reload", "GUITextures_Add", "GUITextures_Reload", "SettlerUpgradeCategory", "BuildingUpgradeCategory", "TerrainTextures_Add",
  "TerrainTextures_Reload", "WaterTypes", "TerrainTypes", "SoundGroups", "DirectXEffects", "ArmorClasses", "DamageClasses", "EntityCategories"
	} do
		if not manifest[k] then
			manifest[k] = {}
		end
	end
	manifest.MissingFilled = true
end

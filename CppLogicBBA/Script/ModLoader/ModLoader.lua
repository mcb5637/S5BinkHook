ModLoader = ModLoader or {}

--- applying everything in Manifest
function ModLoader.ApplyManifest()
	ModLoader.FillMissingManifestEntries(ModLoader.Manifest)
	local mt = ModLoader.ManifestTypes()
	for _, m in ipairs(mt) do
		ModLoader.ManifestType[m.Type or "default"].Preload(m, ModLoader.Manifest)
	end
	for _, m in ipairs(mt) do
		ModLoader.ManifestType[m.Type or "default"].Load(m, ModLoader.Manifest)
	end
end

function ModLoader.ManifestTypes()
	return {
		{Key="ArmorClasses", Preload=CppLogic.ModLoader.PreLoadArmorClass, Table=ArmorClasses, Load=nil},
		{Key="EntityCategories", Preload=CppLogic.ModLoader.PreLoadEntityCategory, Table=EntityCategories, Load=nil},
		{Key="DamageClasses", Preload=nil, Table=DamageClasses, Load=CppLogic.ModLoader.AddDamageClass, Type="kv"},
		{Key="DirectXEffects", Preload=nil, Table=nil, Load=CppLogic.ModLoader.LoadDirectXEffect},
		{Key="TerrainTextures_Add", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddTerrainTexture},
		{Key="TerrainTextures_Reload", Preload=nil, Table=nil, Load=nil, Deprecated="TerrainTextures_Add"},
		{Key="WaterTypes", Preload=nil, Table=WaterTypes, Load=CppLogic.ModLoader.AddWaterType},
		{Key="TerrainTypes", Preload=nil, Table=TerrainTypes, Load=CppLogic.ModLoader.AddTerrainType},
		{Key="SelectionTextures_Add", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddSelectionTexture},
		{Key="SelectionTextures_Reload", Preload=nil, Table=nil, Load=nil, Deprecated="SelectionTextures_Add"},
		{Key="Animations", Preload=nil, Table=Animations, Load=CppLogic.ModLoader.AddAnimation},
		{Key="AnimSets", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddAnimSet},
		{Key="Models", Preload=nil, Table=Models, Load=CppLogic.ModLoader.AddModel},
		{Key="EffectTypes", Preload=CppLogic.ModLoader.PreLoadEffectType, Table=GGL_Effects, Load=CppLogic.ModLoader.AddEffectType},
		{Key="TaskLists", Preload=CppLogic.ModLoader.PreLoadTaskList, Table=TaskLists, Load=CppLogic.ModLoader.AddTaskList},
		{Key="EntityTypes", Preload=CppLogic.ModLoader.PreLoadEntityType, Table=Entities, Load=CppLogic.ModLoader.AddEntityType},
		{Key="Technologies", Preload=CppLogic.ModLoader.PreLoadTechnology, Table=Technologies, Load=CppLogic.ModLoader.AddTechnology},
		{Key="GUITextures_Add", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddGUITexture},
		{Key="GUITextures_Reload", Preload=nil, Table=nil, Load=nil, Deprecated="GUITextures_Add"},
		{Key="SettlerUpgradeCategory", Preload=CppLogic.ModLoader.PreLoadUpgradeCategory, Table=UpgradeCategories, Load=CppLogic.ModLoader.AddSettlerUpgradeCategory, Type="kv"},
		{Key="BuildingUpgradeCategory", Preload=CppLogic.ModLoader.PreLoadUpgradeCategory, Table=UpgradeCategories, Load=CppLogic.ModLoader.AddBuildingUpgradeCategory, Type="kv"},
		{Key="ExperienceClasses", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddExperienceClass, Type="kv"},
		{Key="SoundGroups", Preload=nil, Table=Sounds, Load=CppLogic.ModLoader.AddSounds, Type="sound"},
	}
end

ModLoader.ManifestType = {
	default = {
		Preload = function(t, manifest)
			local preload = t.Preload
			if not preload then
				return
			end
			local entries = manifest[t.Key]
			for _, k in ipairs(entries) do
				if type(k) == "string" then
					preload(k)
				end
			end
		end,
		Load = function(t, manifest)
			local add = t.Load
			if not add then
				return
			end
			local entries = manifest[t.Key]
			for _, k in ipairs(entries) do
				add(k)
			end
		end,
		Merge = function(t, into, from)
			local function contains(t, v)
				for _, d in ipairs(t) do
					if d == v then
						return true
					end
				end
				return false
			end
			local at = into[t.Key]
			for _, d in ipairs(from[t.Key]) do
				if not contains(at, d) then
					table.insert(at, d)
				end
			end
		end,
		Fix = function(t, manifest)
			local data = t.Table
			if not data then
				return
			end
			local entries = manifest[t.Key]
			local function deref(d)
				for k, v in pairs(data) do
					if v == d then
						return k
					end
				end
				return d
			end
			for i, e in ipairs(entries) do
				entries[i] = deref(e)
			end
		end,
	},
	kv = {
		Preload = function(t, manifest)
			local preload = t.Preload
			if not preload then
				return
			end
			local entries = manifest[t.Key]
			local data = t.Table
			for k, _ in pairs(entries) do
				if type(k) == "string" then
					preload(k)
				end
			end
		end,
		Load = function(t, manifest)
			local add = t.Load
			if not add then
				return
			end
			local entries = manifest[t.Key]
			for k, v in pairs(entries) do
				add(k, v)
			end
		end,
		Merge = function(t, into, from)
			local at = into[t.Key]
			for u, v in pairs(from[t.Key]) do
				if not at[u] then
					at[u] = v
				end
			end
		end,
		Fix = function(t, manifest)
			local data = t.Table
			if not data then
				return
			end
			local entries = manifest[t.Key]
			local function deref(d)
				for k, v in pairs(data) do
					if v == d then
						return k
					end
				end
				return d
			end
			local change = {}
			for k, _ in pairs(entries) do
				local kn = deref(k)
				if kn ~= k then
					change[k] = kn
				end
			end
			for k, kn in pairs(change) do
				entries[kn] = entries[k]
				entries[k] = nil
			end
		end,
	},
	sound = {
		Preload = function(t, manifest)
		end,
		Load = function(t, manifest)
			local add = t.Load
			if not add then
				return
			end
			local entries = manifest[t.Key]
			for _, k in ipairs(entries) do
				---@diagnostic disable-next-line: deprecated
				add(unpack(k))
			end
		end,
		Merge = function(t, into, from)
			local at = into[t.Key]
			for _, sg in ipairs(from[t.Key]) do
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
		end,
		Fix = function(t, manifest)

		end,
	},
}

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
	local mt = ModLoader.ManifestTypes()
	for _, m in ipairs(mt) do
		ModLoader.ManifestType[m.Type or "default"].Merge(m, into, from)
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
--- ... [2] overrides [1]
function ModLoader.SortMods(modlist)
	local function contains(t, v)
		for _, d in ipairs(t) do
			if d == v then
				return d
			end
		end
		return nil
	end
	if modlist.Missing[1] then
		LuaDebugger.Break()
		CppLogic.ModLoader.InvalidModPackPanic("missing mod: "..modlist.Missing[1])
	end
	for _, m in ipairs(modlist.Mods) do
		local c = contains(modlist.Incompatible, m.Name)
		if c then
			LuaDebugger.Break()
			CppLogic.ModLoader.InvalidModPackPanic("incompatibility found: "..m.Name.." with "..c)
		end
	end
	table.sort(modlist.Mods, function(a, b)
		local ab = contains(a.Override, b.Name)
		local ba = contains(b.Override, a.Name)
		if ab and ba then
			LuaDebugger.Break()
			CppLogic.ModLoader.InvalidModPackPanic("circular override: "..a.Name.." with "..b.Name)
		end
		if ba then
			return true
		end
		return false
	end)
end

--- loads a ModPack bba, if present
function ModLoader.LoadMod(mod)
	if type(mod) == "string" then
		mod = CppLogic.ModLoader.GetModpackInfo(mod)
		assert(type(mod) == "table")
	end
	mod.Archive = CppLogic.ModLoader.LoadModpackBBA(mod.Name)
	return mod
end

--- initializes mod (load its loader, then call Init)
function ModLoader.InitMod(mod)
	if mod.LoaderPath ~= "" then
		Script.Load(mod.LoaderPath)
		xpcall(function()
			ModLoader[mod.Name].Init()
		end, function(err)
			CppLogic.ModLoader.InvalidModPackPanic("failed to initialize "..mod.Name..": "..err)
		end)
	end
end

--- loads a modlst
function ModLoader.LoadMods(modlist)
	-- [n] ... [2] overrides [1]
	-- always inserted after s5x -> {s5x, n, ..., 2, 1}
	for i, m in ipairs(modlist.Mods) do
		modlist.Mods[i] = ModLoader.LoadMod(m)
	end
	-- merge ignores items already present -> {s5x, n, ..., 2, 1}
	for i=table.getn(modlist.Mods),1,-1 do
		ModLoader.InitMod(modlist.Mods[i])
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
	return modname == "test" or modname == "WideScreenMode"
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
	local mt = ModLoader.ManifestTypes()
	local function getdepr(d)
		for _,m in ipairs(mt) do
			if m.Key == d then
				return m
			end
		end
		assert(false)
	end
	for _, m in ipairs(mt) do
		local t = ModLoader.ManifestType[m.Type or "default"]
		if not manifest[m.Key] then
			manifest[m.Key] = {}
		elseif m.Deprecated then
			local n = getdepr(m.Deprecated)
			ModLoader.ManifestType[n.Type or "default"].Merge(n, manifest, {
				[n.Key] = manifest[m.Key],
			})
		end
		t.Fix(m, manifest)
	end
	manifest.MissingFilled = true
end

---@diagnostic disable: inject-field
ModLoader = ModLoader or {}

if false then
	---@class CArmorDamageMapping
	---@field ArmorClass string
	---@field Factor number
	local CArmorDamageMapping = {
	}
	---@class CDamageClass
	---@field ObjectClassName "CppLogic::Mod::Config::DamageClassExt"|"GGL::CDamageClassProps"
	---@field ArmorClassNoneFactor number
	---@field ArmorClassJerkinFactor number
	---@field ArmorClassLeatherFactor number
	---@field ArmorClassIronFactor number
	---@field ArmorClassFortificationFactor number
	---@field ArmorClassHeroFactor number
	---@field ArmorClassFurFactor number
	---@field ExtraArmorClass CArmorDamageMapping[]|nil
	local DamageClass = {
	}
	---@class Manifest
	---@field MissingFilled boolean|nil
	---@field ArmorClasses string[]|nil
	---@field EntityCategories string[]|nil
	---@field DamageClasses table<string,CDamageClass>|nil
	---@field DirectXEffects string[]|nil
	---@field TerrainTextures string[]|nil
	---@field WaterTypes (string|number)[]|nil
	---@field TerrainTypes (string|number)[]|nil
	---@field SelectionTextures string[]|nil
	---@field Animations (string|number)[]|nil
	---@field AnimSets (string|number)[]|nil
	---@field Models (string|number)[]|nil
	---@field EffectTypes (string|number)[]|nil
	---@field TaskLists (string|number)[]|nil
	---@field EntityTypes (string|number)[]|nil
	---@field Technologies (string|number)[]|nil
	---@field GUITextures string[]|nil
	---@field SettlerUpgradeCategory table<string,string|number>|nil
	---@field BuildingUpgradeCategory table<string,string|number>|nil
	---@field ExperienceClasses table<string,string|number>|nil
	---@field SoundGroups (string|number)[][]|nil
	---@field StringTableTexts table<string,string|true>|nil
	---@field Fonts string[]|nil
	local Manifest = {}
	---@class CManifestEntry
	local CManifestEntry = {
		Key = "",
		---@type function|nil
		Preload = nil,
		---@type function|nil
		Load = nil,
		---@type table|nil
		Table = nil,
		---@type string|nil
		Type = nil,
		---@type string|nil
		Deprecated = nil
	}
	---@class CManifestType
	---@field Preload fun(CManifestEntry, Manifest)
	---@field Load fun(CManifestEntry, Manifest)
	---@field Merge fun(CManifestEntry, into:Manifest, from:Manifest)
	---@field Fix fun(CManifestEntry, Manifest)
	local CManifestType = {
	}
	---@class ModList
	---@field Mods ModpackDesc[]
	---@field Incompatible string[]
	---@field Failed string[]
	local ModList = {
	}
	---@class ModPack
	---@field Manifest Manifest
	---@field Init fun(ModpackDesc)|nil
	local ModPack = {
	}
	---@class ModLoader
	---@field KeepArchive boolean|nil
	---@field Manifest Manifest
	---@field RequiredMods string[]
	---@field Initialize fun()|nil
	---@field MapStart fun()|nil
	---@field LoadSave fun()|nil
	---@field CheckUserRequestedMod nil|fun(string):boolean
	---@field Cleanup fun()|nil
	---@field LanguageOrder string[]|nil
	local ModLoader = {
		MapInfo = {
			MapName = "",
			MapType = 0,
			MapCampagnName = "",
			MapGUID = "",
			IsSavegame = false,
			---@type string|nil
			SaveLoading = "",
		}
	}
end

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

---@return CManifestEntry[]
function ModLoader.ManifestTypes()
	return {
		---@diagnostic disable-next-line: undefined-global
		{Key="ArmorClasses", Preload=CppLogic.ModLoader.PreLoadArmorClass, Table=ArmorClasses, Load=nil},
		{Key="EntityCategories", Preload=CppLogic.ModLoader.PreLoadEntityCategory, Table=EntityCategories, Load=nil},
		{Key="DamageClasses", Preload=nil, Table=DamageClasses, Load=CppLogic.ModLoader.AddDamageClass, Type="kv"},
		{Key="DirectXEffects", Preload=nil, Table=nil, Load=CppLogic.ModLoader.LoadDirectXEffect},
		{Key="TerrainTextures", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddTerrainTexture},
		{Key="TerrainTextures_Add", Preload=nil, Table=nil, Load=nil, Deprecated="TerrainTextures"},
		{Key="TerrainTextures_Reload", Preload=nil, Table=nil, Load=nil, Deprecated="TerrainTextures"},
		---@diagnostic disable-next-line: undefined-global
		{Key="WaterTypes", Preload=nil, Table=WaterTypes, Load=CppLogic.ModLoader.AddWaterType},
		---@diagnostic disable-next-line: undefined-global
		{Key="TerrainTypes", Preload=nil, Table=TerrainTypes, Load=CppLogic.ModLoader.AddTerrainType},
		{Key="SelectionTextures", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddSelectionTexture},
		{Key="SelectionTextures_Add", Preload=nil, Table=nil, Load=nil, Deprecated="SelectionTextures"},
		{Key="SelectionTextures_Reload", Preload=nil, Table=nil, Load=nil, Deprecated="SelectionTextures"},
		{Key="Animations", Preload=nil, Table=Animations, Load=CppLogic.ModLoader.AddAnimation},
		{Key="AnimSets", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddAnimSet},
		{Key="Models", Preload=nil, Table=Models, Load=CppLogic.ModLoader.AddModel},
		{Key="EffectTypes", Preload=CppLogic.ModLoader.PreLoadEffectType, Table=GGL_Effects, Load=CppLogic.ModLoader.AddEffectType},
		{Key="TaskLists", Preload=CppLogic.ModLoader.PreLoadTaskList, Table=TaskLists, Load=CppLogic.ModLoader.AddTaskList},
		{Key="EntityTypes", Preload=CppLogic.ModLoader.PreLoadEntityType, Table=Entities, Load=CppLogic.ModLoader.AddEntityType},
		{Key="Technologies", Preload=CppLogic.ModLoader.PreLoadTechnology, Table=Technologies, Load=CppLogic.ModLoader.AddTechnology},
		{Key="GUITextures", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddGUITexture},
		{Key="GUITextures_Add", Preload=nil, Table=nil, Load=nil, Deprecated="GUITextures"},
		{Key="GUITextures_Reload", Preload=nil, Table=nil, Load=nil, Deprecated="GUITextures"},
		{Key="SettlerUpgradeCategory", Preload=CppLogic.ModLoader.PreLoadUpgradeCategory, Table=UpgradeCategories, Load=CppLogic.ModLoader.AddSettlerUpgradeCategory, Type="kv"},
		{Key="BuildingUpgradeCategory", Preload=CppLogic.ModLoader.PreLoadUpgradeCategory, Table=UpgradeCategories, Load=CppLogic.ModLoader.AddBuildingUpgradeCategory, Type="kv"},
		{Key="ExperienceClasses", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddExperienceClass, Type="kv"},
		{Key="SoundGroups", Preload=nil, Table=Sounds, Load=CppLogic.ModLoader.AddSounds, Type="sound"},
		{Key="StringTableTexts", Preload=nil, Table=nil, Load=ModLoader.LoadSTTOverride, Type="kv"},
		{Key="Fonts", Preload=nil, Table=nil, Load=CppLogic.ModLoader.AddFont},
	}
end

---@type table<string, CManifestType>
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

--- merges all manifest entries from from into into
--- @param into Manifest
--- @param from Manifest
function ModLoader.MergeManifest(into, from)
	ModLoader.FillMissingManifestEntries(into)
	ModLoader.FillMissingManifestEntries(from)
	local mt = ModLoader.ManifestTypes()
	for _, m in ipairs(mt) do
		ModLoader.ManifestType[m.Type or "default"].Merge(m, into, from)
	end
end

--- walks a mod list, discovers and adds all requirements
--- @param req string[]
--- @param modlist ModList|nil
--- @param checkUserRequested boolean|nil
--- @param failuresToList boolean?
--- @return ModList
function ModLoader.DiscoverRequired(req, modlist, checkUserRequested, failuresToList)
	modlist = modlist or {Mods = {}, Incompatible = {}, Failed = {}}
	local function checkfail(cond, msg)
		if failuresToList then
			if not cond then
				table.insert(modlist.Failed, msg)
				return false
			end
		else
			assert(cond, msg)
		end
		return true
	end
	for _, r in ipairs(req) do
		local name, cmp = ModLoader.ParseModString(r)
		local found = false
		for _, m in ipairs(modlist.Mods) do
			if m.Name == name then
				found = true
				break
			end
		end
		if not found then
			local m = CppLogic.ModLoader.GetModpackInfo(name)
			if type(m) == "table" then
				local good = checkfail(cmp(m.Version), "mod version missmatch: requested"..r.." has "..m.Name.."@"..m.Version)
				if checkUserRequested then
					good = checkfail(m.UserRequestable or m.MainmenuMod, name.." is user requested, but not marked as user requestable or mainmenu")
					if not m.UserRequestable then
						good = false
					else
						if not ModLoader.CheckUserRequestedMod(m.Name) then
							good = false
						end
					end
				end
				if good then
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
				end
			else
				checkfail(false, "missing mod: "..name)
			end
		end
	end
	return modlist
end

--- checks a modlist and sorts it
--- ... [2] overrides [1]
--- @param modlist ModList
--- @param failuresToList boolean?
function ModLoader.SortMods(modlist, failuresToList)
	---@generic T
	---@param t T[]
	---@param v T
	---@return T|nil
	local function contains(t, v)
		for _, d in ipairs(t) do
			if d == v then
				return d
			end
		end
		return nil
	end
	local function checkfail(cond, msg)
		if failuresToList then
			if not cond then
				table.insert(modlist.Failed, msg)
			end
		else
			assert(cond, msg)
		end
	end
	for _, m in ipairs(modlist.Mods) do
		local c = contains(modlist.Incompatible, m.Name)
		if c then
			local with = "Map"
			for _,mp in ipairs(modlist.Mods) do
				if contains(mp.Incompatible, m.Name) then
					with = mp.Name
					break
				end
			end
			checkfail(false, "incompatibility found: "..m.Name.." with "..with)
		end
	end
	table.sort(modlist.Mods, function(a, b)
		local ab = contains(a.Override, b.Name)
		local ba = contains(b.Override, a.Name)
		if ab and ba then
			checkfail(false, "circular override: "..a.Name.." with "..b.Name)
			return false
		end
		if ba then
			return true
		end
		return false
	end)
end

--- loads a ModPack bba, if present
--- @param mod ModpackDesc|string
--- @return ModpackDesc
function ModLoader.LoadMod(mod)
	if type(mod) == "string" then
		mod = CppLogic.ModLoader.GetModpackInfo(mod)
		assert(type(mod) == "table")
	end
	mod.Archive = CppLogic.ModLoader.LoadModpackBBA(mod.Name)
	return mod
end

--- initializes mod (load its loader, then call Init)
--- @param mod ModpackDesc
function ModLoader.InitMod(mod)
	if mod.DataMod and mod.LoaderPath ~= "" then
		Script.Load(mod.LoaderPath)
		xpcall(function()
			---@diagnostic disable-next-line: undefined-field
			ModLoader[mod.Name].Init(mod)
		end, function(err)
			CppLogic.ModLoader.InvalidModPackPanic("failed to initialize "..mod.Name..": "..err)
		end)
	end
end

--- @param modlist ModList
function ModLoader.LoadArchives(modlist)
	-- [n] ... [2] overrides [1]
	-- always inserted after s5x -> {s5x, n, ..., 2, 1}
	for i, m in ipairs(modlist.Mods) do
		modlist.Mods[i] = ModLoader.LoadMod(m)
	end
end

--- loads a modlst
--- @param modlist ModList
function ModLoader.LoadMods(modlist)
	-- [n] ... [2] overrides [1]
	ModLoader.LoadArchives(modlist)
	-- merge ignores items already present -> {s5x, n, ..., 2, 1}
	for i=table.getn(modlist.Mods),1,-1 do
		ModLoader.InitMod(modlist.Mods[i])
	end
end

--- removes no longer needed ModPack bbas
--- @param modlist ModList
--- @param script boolean|nil
--- @param lib boolean|nil
--- @param force boolean|nil
function ModLoader.CleanupMods(modlist, script, lib, force)
	for _, m in ipairs(modlist.Mods) do
		if m.Archive then
			if force or (not m.KeepArchive and (script or m.ScriptPath == "") and (lib or not m.ScriptLib)) then
				m.Archive:Remove()
				m.Archive = nil
				if m.RedirectLayer then
					m.RedirectLayer:Remove()
					m.RedirectLayer = nil
				end
			end
		end
	end
end

--- discovers mods and loads them
function ModLoader.RequireModList()
	xpcall(function()
		---@type ModList
		ModLoader.ModList = {Mods = {}, Incompatible = {}, Failed = {}}
		if ModLoader.MapInfo.IsSavegame then
			local _, ml = ModLoader.ParseSaveGUID(ModLoader.MapInfo.MapGUID, ModLoader.MapInfo.MapName,
				ModLoader.MapInfo.MapType, ModLoader.MapInfo.MapCampagnName
			)
			ModLoader.RequiredMods = ml
		else
			local r, i = CppLogic.ModLoader.MapGetModPacks(Framework.GetCurrentMapName(), Framework.GetCurrentMapTypeAndCampaignName())
			if r[1] then
				if not ModLoader.RequiredMods[1] then
					ModLoader.RequiredMods = r
				else
					for _,m in ipairs(r) do
						table.insert(ModLoader.RequiredMods, m)
					end
				end
			end
			ModLoader.ModList.Incompatible = i
		end
		ModLoader.DiscoverRequired(ModLoader.RequiredMods, ModLoader.ModList)
		if not ModLoader.MapInfo.IsSavegame then
			ModLoader.DiscoverUserRequested(ModLoader.ModList)
		end
		ModLoader.SortMods(ModLoader.ModList)
		ModLoader.LoadMods(ModLoader.ModList)
		ModLoader.StoreModList(ModLoader.ModList)
	end, function(msg)
		CppLogic.ModLoader.InvalidModPackPanic(msg)
	end)
end

--- stores modlist for savegames
---@param ml ModList
function ModLoader.StoreModList(ml)
	local s = ""
	for _,m in ipairs(ml.Mods) do
		s = s..";"..m.Name.."@"..m.Version
	end
	if s == "" then
		s = ";"
	end
	CppLogic.ModLoader.SetModPackList(s)
end

---loads modlist
---@param s string|nil
---@return string[]
function ModLoader.LoadModList(s)
	s = s or CppLogic.ModLoader.GetModPackList()
	if not s then
		return {}
	end
	local r = {}
	for m in string.gfind(s, "([%w_/\\@<>=%.]+)") do
		table.insert(r, m)
	end
	return r
end

---parses mod versioned name
---@param s string
---@return string name
---@return fun(v:string):boolean validate
function ModLoader.ParseModString(s)
	local f,_, n, op, v = string.find(s, "^([%w_/\\]+)([@<>=]*)([%w_/\\%.]*)$")
	if not f then
		assert(false, "mod name is invalid: "..s)
	end
	---@param v string
	---@return number[]
	local function parsenumbers(v)
		local t = {}
		for n in string.gfind(v, "(%d+)") do
			table.insert(t, tonumber(n))
		end
		return t
	end
	local cmp = nil
	if op == "@" then
		cmp = function(w)
			return v == w
		end
	elseif op == "==" then
		cmp = function(w)
			return v == string.sub(w, 1, string.len(v))
		end
	elseif op == "<=" then
		cmp = function(w)
			local vn = parsenumbers(v)
			local wn = parsenumbers(w)
			local i = 1
			while vn[i] and wn[i] do
				if vn[i] < wn[i] then
					return false
				elseif vn[i] > wn[i] then
					return true
				end
				i = i + 1
			end
			if not vn[i] and not wn[i] then
				return true
			elseif wn[i] then
				return false
			else
				return true
			end
		end
	elseif op == ">=" then
		cmp = function(w)
			local vn = parsenumbers(v)
			local wn = parsenumbers(w)
			local i = 1
			while vn[i] and wn[i] do
				if vn[i] > wn[i] then
					return false
				elseif vn[i] < wn[i] then
					return true
				end
				i = i + 1
			end
			if not vn[i] and not wn[i] then
				return true
			elseif wn[i] then
				return true
			else
				return false
			end
		end
	elseif op and op ~="" then
		assert(false, "invalid operator: "..s)
	end
	if not cmp then
		cmp = function() return true end
	end
	return n, cmp
end

--- loads script mods
function ModLoader.LoadModScripts()
	if ModLoader.ModScriptsLoaded then
		return
	end
	ModLoader.ModScriptsLoaded = true
	for _, m in ipairs(ModLoader.ModList.Mods) do
		if m.ScriptMod and m.ScriptPath ~= "" then
			Script.Load(m.ScriptPath)
		end
	end
	ModLoader.CleanupMods(ModLoader.ModList, true)
end

--- executes ScriptPath of a ModPack, to be used from your mapscript if no ModLoader is present
--- does not handle modpack dependencies
--- @param name string
--- @return ModpackDesc
function ModLoader.LoadScriptMod(name)
	local mod = CppLogic.ModLoader.GetModpackInfo(name)
	assert(type(mod) ~="string")
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
--- @param modname string
--- @return boolean
function ModLoader.IsUserRequestedModWhitelisted(modname)
	if modname == "test" or modname == "WideScreenMode" or modname == "BugFixes" then
		return true
	end
	if not ModLoader.UserRequestedModWhitelisted then
		local _, _, ur = CppLogic.ModLoader.MapGetModPacks(Framework.GetCurrentMapName(), Framework.GetCurrentMapTypeAndCampaignName())
		ModLoader.UserRequestedModWhitelisted = ur
	end
	for _,s in ipairs(ModLoader.UserRequestedModWhitelisted) do
		if s == modname then
			return true
		end
	end
	return false
end

--- parse user requested mods and add them to the mod list
--- @param modlist ModList
--- @param requested string|string[]|nil
--- @param failuresToList boolean?
function ModLoader.DiscoverUserRequested(modlist, requested, failuresToList)
	if not requested then
		requested = GDB.GetString("CppLogic\\UserRequestedMods")
	end
	---@type string[]
	local l
	if type(requested)=="string" then
		l = ModLoader.LoadModList(requested)
	else
		l = requested
	end
	if not ModLoader.CheckUserRequestedMod then
		return
	end
	ModLoader.DiscoverRequired(l, modlist, true, failuresToList)
end

--- adds empty tables, for compatibility with old modloaders
--- @param manifest Manifest
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

---@param name string
---@param into string|boolean|nil
function ModLoader.LoadSTTOverride(name, into)
	if type(into)~="string" then
		into = nil
	end
	local langs = ModLoader.LanguageOrder or {XNetworkUbiCom.Tool_GetCurrentLanguageShortName(), "en", "de"}
	for _, l in ipairs(langs) do
		if CppLogic.ModLoader.LoadStringTableTextOverrides(name, l, into) >= 0 then
			return
		end
	end
	assert(false, "cannot find STT override file "..name)
end

---parses a savegame guid into map guid and mod list
---returns nil, if map could not be found
---@param guid string
---@param mapname string
---@param maptype number
---@param cname string
---@return string|nil mapguid
---@return string[]|nil modlist
function ModLoader.ParseSaveGUID(guid, mapname, maptype, cname)
	local _, mapguid, _, err = CppLogic.API.MapGetDataPath(mapname, maptype, cname, true)
	if err then
		return nil, nil
	end
	local len = string.len(mapguid)
	local g = string.sub(guid, 1, len)
	if g ~= mapguid then
		return nil, nil
	end
	local ml = ModLoader.LoadModList(string.sub(guid, len+1))
	return g, ml
end

---	unloads all script lib bbas
function ModLoader.UnloadScriptLibs()
	ModLoader.CleanupMods(ModLoader.ModList, true, true)
end

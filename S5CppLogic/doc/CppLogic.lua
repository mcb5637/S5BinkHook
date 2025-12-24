---@diagnostic disable: missing-return

assert(false, "do not load this file, this is documentation only!")

--- CppLogic
--- these changes are always active:
--- - text rendering:
---		- all @ codes do now work after alignment codes (only the last alignment code per line takes effect).
---		- automatic line breaks do now work properly with non la alignment.
--		- added @la alignment code.
---		- checking for @ codes inside words (space before @ no longer needed).
---		- all @ codes can be terminated by | instead of space (space will get rendered, | will not).
---		- added @defaultcolor to reset color to the widgets default.
---		- added @skipWhitespace, which takes on or off as parameter, to turn off condensing multiple spaces to one.
---		- added @@ to print one @ (escape sequence).
---		- added @icon:iconid,texX,texY,texW,texH,sizeX,sizeY,r,g,b,a renders a EGUIX::CMaterial as a char inside the text
---			- iconid is the path to the file, same format as for widgets. (this is a GUITexture, you may use the corresponding ModLoader functions for them).
---			- texX, texY, texW, texH are texture coordinates to display part of the texture only (optional, default 0,0,1,1, either all or none) (easiest to copy them out of the GUIEdito).
---			- sizeX, sizeY size multipliers (optional, default Y=X, X=1, you may specify only one) (if you want to use them, texture coordinates are required).
---			- r, g, b, a material color override (optional, default 255,255,255,255, you may only specify r,g,b) (if you want to use them, texture coordinates and both sizes are required).
---		- for @color each color channel can be replaced by one letter r, g, b or a to use the matching default (widget) color channel (also works in @icon).
--- - music can now be played from bba/s5x archives (make sure the s5x is loaded if you want to use music from it).
--- - removed override for the functions error and pcall, they now work as they should
--- - savegame lua state handling:
---		- completely rewritten, now able to save anything that is not of the following:
---			- lua functions with upvalues
---			- C functions (although they should get readded by SHoK)
---			- userdata (except if it follows a specific protocol, see CppLogic::Serializer::AdvLuaStateSerializer)
---			- threads (coroutines)
---		- note that the following does now work:
---			- function & string size no longer limited
---			- any table keys (not of one of the forbidden types above) (also no size limit) (also no more type changes between numbers <-> strings)
---			- metatables
---			- function identity does now stay as it is. (so if 2 functions are == before saving + loading, they are also after doing so)
---		- please note that savegames made with this change active cannot be loaded without CppLogic. (but you can load old saves, just without any of these features)
--- - arrow and cannonball effect on loaded fix (they now continue flying and hitting after loading a savegame) (this works even for old saves)
--- - s5x maps are no longer blocked while SHoK is running
--- - the mapscript now gets loaded via the same function as all other scripts. (yes, it can be precompiled now)
--- - double click to select all units of a type now works on the entire screen regardless of screen size
--- - GGL::CGLEffectFireProps no longer has a uninitialized member (SoundIDAgain)
--- - more than 24 (non soldier) entities can be selected at the same time
CppLogic = {}
CppLogic.Effect = {}
CppLogic.Effect.Predicates = {}
CppLogic.Memory = {}
CppLogic.API = {}
CppLogic.Combat = {}
--- commands here are not syncronized, they are executed directly
CppLogic.Entity = {}
CppLogic.Entity.Predicates = {}
CppLogic.Entity.Settler = {}
CppLogic.Entity.Leader = {}
CppLogic.Entity.Building = {}
CppLogic.EntityType = {}
CppLogic.EntityType.Settler = {}
CppLogic.EntityType.Building = {}
CppLogic.Technology = {}
CppLogic.Logic = {}
CppLogic.Logic.UICommands = {}
CppLogic.UI = {}
--- commands here are send as NetEvent and synchronized
CppLogic.UI.Commands = {}
CppLogic.UA = {}
--- ModLoader is not available, if Kimichuras dlls are loaded.
--- otherwise the modloader looks for a `ModLoader.lua` file in your map directory. if it is there, this file gets loaded and the ModLoader executes.
--- see default_ModLoader.lua for callbacks and variables.
--- unless otherwise noted, modloader functions are only available during ModLoader callbacks.
CppLogic.ModLoader = {}

--- CppLogic version number.
CppLogic.Version = 2.0005

--- no longer needed.
function CppLogic.OnLeaveMap() end

--- returns settings from CppLogicOptions.txt.
--- @return boolean DisableAdvStringPrinting
--- @return boolean DisableAdvLuaSerializer
--- @return boolean DisableModLoader
--- @return boolean HasSCELoader
function CppLogic.GetOptions() end

--- check if an effect is valid
--- @param id number id
--- @return boolean v true/false
function CppLogic.Effect.IsValidEffect(id) end

--- gets the effect type of an effect
--- @param id number id
--- @return number
function CppLogic.Effect.GetType(id) end

--- gets the position of an effect
--- @param id number id
--- @return Position
function CppLogic.Effect.GetPosition(id) end

--- gets the player of an effect
--- @param id number id
--- @return number
function CppLogic.Effect.GetPlayer(id) end

--- pchecks if the effect is an arrow effect.
--- @param id number id
--- @return boolean
function CppLogic.Effect.IsArrow(id) end

--- checks is the effect is a cannonball effect.
--- @param id number id
--- @return boolean
function CppLogic.Effect.IsCannonBall(id) end

--- gets the start and end position of an cannonball or arrow effect.
--- use CppLogic.Effect.GetPosition to get the current position.
--- @param id number id
--- @return Position start
--- @return Position end
function CppLogic.Effect.GetFlyingEffectStartEnd(id) end

--- gets the combat data of an arrow effect
--- @param id number id
--- @return number attackerId
--- @return number tartegId
--- @return number damage
--- @return boolean misses
function CppLogic.Effect.GetArrowEffectData(id) end

--- gets the combat data of a cannonball effect.
--- @param id number id
--- @return number attackerId
--- @return number damage
--- @return number aoeRange
--- @return number damageClass
--- @return number sourcePlayer
--- @return Position impactPos (note: same as end position of CppLogic.Effect.GetFlyingEffectStartEnd)
function CppLogic.Effect.GetCannonBallEffectData(id) end

--- enables effect triggers: CPPLOGIC_EVENT_ON_EFFECT_CREATED, CPPLOGIC_EVENT_ON_FLYINGEFFECT_HIT, CPPLOGIC_EVENT_ON_EFFECT_DESTROYED
--- status of this gets saved into a savegame.
--- @param enable boolean
function CppLogic.Effect.EnableEffectTriggers(enable) end

--- create a projectile.
--- to deal damage on impact use:
--- - single target: dmg > 0.
--		tarid to target entity id.
--		attid or playerid? set to attacker entity or player.
--- - AOE: dmg > 0, radius > 0.
--		attid or playerid can be set to attacker entity or player (deals damage only to entities hostile to attacker).
--		dmgclass can be set to the attacks damageclass to include it in the calculation.
--- @param effecttype number GGL_Effects.XXX
--- @param startx number start pos x
--- @param starty number start pos y
--- @param tarx number target pos x
--- @param tary number target pos y
--- @param dmg number damage done by projectile (optional, 0/nil if not used)
--- @param radius number damage radius for AOE projectiles (optional, -1/nil if not used)
--- @param tarid number target entity id (optional, 0/nil if not used)
--- @param attid number attacker entity id (optional, 0/nil if not used)
--- @param playerid number attacker player id (optional, 0/nil if not used)
--- @param dmgclass number damageclass (optional, 0/nil if not used)
--- @param callback fun(id:number) function that gets called on hit (optional, nil if not used)
--- @param source number what deals damage, used for triggers (default AdvancedDealDamageSource.Script) max size 8 bit instead of 32
--- @return number effectId id
function CppLogic.Effect.CreateProjectile(effecttype, startx, starty, tarx, tary, dmg, radius, tarid, attid, playerid, dmgclass, callback, source) end

--- enables FXLightning effect to display properly.
--- - no longer changes global lighting conditions
--- - always displayed with full white color (not partially transparent)
---@param enabled boolean
function CppLogic.Effect.EnableLightningFix(enabled) end

--- dumps an effect type to lua. debug only, slow!
--- @param effect number|string
--- @return table
function CppLogic.Effect.DumpEffectType(effect) end

--- dumps effect to lua
--- dev only, slow!
---@param e number|string
---@return table
function CppLogic.Effect.DumpEffect(e) end

---@class EffectPredicate
local EffectPredicate = {}

--- iterates over all effect that match a predicate.
--- you may create/destroy effects while iterating, even the current one.
--- perfect to use with for loop.
--- examples:
--- - for id in CppLogic.Effect.EffectIterator(...) do Message(id) end  
--- - for id, rsqu, prio in CppLogic.Effect.EffectIterator(CppLogic.Entity.Effect.InCircle(...), ...) do Message(id.."   "..r) end  
--- @param ... EffectPredicate
--- @return fun():number,number,number nextEntity
--- @return nil iteratorStatus
--- @return nil
function CppLogic.Effect.EffectIterator(...) end

--- gets all effects that match a predicate as a table.
--- @param ... EffectPredicate
--- @return number[]
function CppLogic.Effect.EffectIteratorTableize(...) end

--- creates a predicate that performs an and of multiple predicates.
--- @param ... EffectPredicate
--- @return EffectPredicate
function CppLogic.Effect.Predicates.And(...) end

--- creates a predicate that performs an or of multiple predicates.
--- @param ... EffectPredicate
--- @return EffectPredicate
function CppLogic.Effect.Predicates.Or(...) end

--- creates a predicate that performs an not of anoher predicate.
--- @param p EffectPredicate
--- @return EffectPredicate
function CppLogic.Effect.Predicates.Not(p) end

--- creates a predicate that always matches, but sets the priority to prio if p matches it.
--- @param p EffectPredicate
--- @param prio number
--- @return EffectPredicate
function CppLogic.Effect.Predicates.SetPriority(p, prio) end

--- creates a predicate that checks for an area.
--- when used in loop iterator, can also return the range squared for each matched entity as 2nd loop parameter (optional).
--- @param p Position center of circle
--- @param r number radius of circle
--- @return EffectPredicate
function CppLogic.Effect.Predicates.InCircle(p, r) end

--- creates a predicate that checks for an rectangular area.
--- order of coordinates does not matter, just have to be grouped as x,y,x,y.
--- @param x1 number x coordinate 1
--- @param y1 number y coordinate 1
--- @param x2 number x coordinate 2
--- @param y2 number y coordinate 2
--- @return EffectPredicate
function CppLogic.Effect.Predicates.InRect(x1, y1, x2, y2) end

--- creates a predicate that checks for an effecttype.
--- @param t number effect type
--- @return EffectPredicate
function CppLogic.Effect.Predicates.OfType(t) end

--- creates a predicate that checks for an effecttype.
--- @param p number
--- @return EffectPredicate
function CppLogic.Effect.Predicates.OfPlayer(p) end

--- creates a predicate that checks for arrow effects
--- @return EffectPredicate
function CppLogic.Effect.Predicates.IsArrow() end

--- creates a predicate that checks for cannonball effects
--- @return EffectPredicate
function CppLogic.Effect.Predicates.IsCannonBall() end

--- creates a predicate that checks for arrow or cannonball effects
--- @return EffectPredicate
function CppLogic.Effect.Predicates.IsArrowOrCannonBall() end

--- sets high precision FPU (gets reset on every API call, so call id directly before your calculations)
function CppLogic.Memory.SetFPU() end

---@enum CppObjectAccessType
CppLogic.Memory.ObjectAccessType = {
	Field = 0,
	Struct = 1,
	BBObject = 2,
	List = 3,
}

---@class CppObjectAccess
CppObjectAccess = {}
--- gets the name of the current seridata
---@return string
function CppObjectAccess:Name()end
--- type of this access object
---@return CppObjectAccessType
function CppObjectAccess:GetType()end
--- typecast, throws if not of the specified type
---@return CppFieldAccess
function CppObjectAccess:AsFieldAccess()end
--- typecast, throws if not of the specified type
---@return CppStructAccess
function CppObjectAccess:AsStructAccess()end
--- typecast, throws if not of the specified type
---@return CppBBObjectAccess
function CppObjectAccess:AsObjectAccess()end
--- typecast, throws if not of the specified type
---@return CppListAccess
function CppObjectAccess:AsListAccess()end

---@class CppFieldAccess : CppObjectAccess
CppFieldAccess = {}
---a description of the datatype of this field
---@return string
function CppFieldAccess:DataType()end
---returns the value of this field
---@return any
function CppFieldAccess:Get()end
---sets the value of this field, including type check, but no semantic check (you can for example set the entity health to -500, but not to "foo")
---@param val any
function CppFieldAccess:Set(val)end
--- type of this access object
---@return `CppLogic.Memory.ObjectAccessType.Field`
function CppFieldAccess:GetType()end

---@class CppStructAccess : CppObjectAccess
---@field [string] CppStructAccess? searches for a field with name and returns it (will return registered functions, if they exist, see Get)
CppStructAccess = {}
--- type of this access object
---@return `CppLogic.Memory.ObjectAccessType.Struct`
function CppStructAccess:GetType()end
---iterates over all fields
---@return fun():CppObjectAccess?
function CppStructAccess:Fields()end
---searches for a field with name and returns it
---@param name string
---@return CppStructAccess?
function CppStructAccess:Get(name)end

---@class CppBBObjectAccess : CppStructAccess
CppBBObjectAccess = {}
--- type of this access object
---@return `CppLogic.Memory.ObjectAccessType.BBObject`
function CppBBObjectAccess:GetType()end
---resturns the type of the pointed to object
---@return string?
function CppBBObjectAccess:ObjectType()end
---allocates a new object and replaces the old one with it
---@param classname string? nil->nullptr
---@param free boolean? nil->true
function CppBBObjectAccess:New(classname, free)end
---checks if the pointer is a nullptr
---@return boolean
function CppBBObjectAccess:IsNullptr()end

---@class CppListAccess : CppObjectAccess
CppListAccess = {}
--- type of this access object
---@return `CppLogic.Memory.ObjectAccessType.List`
function CppListAccess:GetType()end
---iterate over all elements in a list
---@return fun():CppObjectAccess?
function CppListAccess:Elements()end
---number of elements in the list
---@return number
function CppListAccess:Size()end
---finds the first element that matches a predicate
---@param pred fun(element:CppObjectAccess):boolean
---@return CppObjectAccess?
function CppListAccess:First(pred)end
---inserts a new element into the list
---for map-type lists, the writer needs to at least write the key
---@param writer fun(toinsert:CppObjectAccess)
function CppListAccess:Insert(writer)end
---removes elements, if the predicate returs true
---(not supported by all list types)
---@param fun fun(elem:CppObjectAccess):boolean
function CppListAccess:Remove(fun)end
---gets the list type, if known
---@return number
---@return string
function CppListAccess:ListType()end
---inserts a new element into the list at a specific position
---@param writer fun(toinsert:CppObjectAccess)
---@param inFrontOf number|fun(elem:CppObjectAccess):boolean
function CppListAccess:InsertAt(inFrontOf, writer)end

---accesses an entitytypes memory
---warning: has no semantic checks, it is easy to crash your game by modifying memory!
---@param ety string|number
---@return CppBBObjectAccess
function CppLogic.Memory.EntityType(ety)end

---accesses an entities memory
---warning: has no semantic checks, it is easy to crash your game by modifying memory!
---@param id string|number
---@return CppBBObjectAccess
function CppLogic.Memory.Entity(id)end

---accesses an effecttypes memory
---warning: has no semantic checks, it is easy to crash your game by modifying memory!
---@param ety string|number
---@return CppStructAccess
function CppLogic.Memory.EffectType(ety)end

---accesses an effects memory
---warning: has no semantic checks, it is easy to crash your game by modifying memory!
---@param id string|number
---@return CppBBObjectAccess
function CppLogic.Memory.Effect(id)end

---accesses a players memory
---warning: has no semantic checks, it is easy to crash your game by modifying memory!
---@param pid string|number
---@return CppBBObjectAccess
function CppLogic.Memory.Player(pid)end

--- reloads cutscenes from the specified path
--- @param path string|nil optional, default "Maps\\ExternalMap"
function CppLogic.Logic.ReloadCutscene(path) end

--- gets the animation id from a name.
--- @param anim string the name of the animation
--- @return number animation id
function CppLogic.Logic.GetAnimIdFromName(anim) end

--- the damage factor between a damageclass and an armorclass.
--- @param dmgclass number damageclass
--- @param armorclass number armorclass
--- @return number factor
function CppLogic.Logic.GetDamageFactor(dmgclass, armorclass) end
--- the damage factor between a damageclass and an armorclass.
--- @param dmgclass number damageclass
--- @param armorclass number armorclass
--- @param fac number factor
function CppLogic.Logic.SetDamageFactor(dmgclass, armorclass, fac) end

--- the tick where this players payday got startet (-1 if inactive).
--- (you can get the current tick via Logic.GetCurrentTurn() ).
--- @param p number player
--- @return number tick
function CppLogic.Logic.PlayerGetPaydayStartetTick(p) end
--- the tick where this players payday got startet (-1 if inactive).
--- (you can get the current tick via Logic.GetCurrentTurn() ).
--- if you disable it (via -1) it gets immediately restarted has a worker or leader on the map.
--- @param p number player
--- @param t number tick
function CppLogic.Logic.PlayerSetPaydayStartetTick(p, t) end
--- sets the time between paydays. (get via Logic).
--- @param freq number frequency in seconds
function CppLogic.Logic.SetPaydayFrequency(freq) end

--- player kill statistics.
--- @param p number player
--- @return number numSettlerKilled
--- @return number numSettlerLost
--- @return number numBuildingKilled
--- @return number numBuildingLost
function CppLogic.Logic.PlayerGetKillStatistics(p) end

--- checks building placement at a specific position.
--- does not check, if BuildOn type matches.
--- if bon is nil, searches for a buildon entity at the specified position (if the building is buildon).
--- @param ty number entitytype to place
--- @param pl number player
--- @param pos Position target position
--- @param rot number rotation
--- @param bon number|nil BuildOn entity id, or 0, or nil
--- @return boolean
function CppLogic.Logic.CanPlaceBuildingAt(ty, pl, pos, rot, bon) end

--- activates alarm mode.
--- @param pl number player
function CppLogic.Logic.PlayerActivateAlarm(pl) end
--- deactivates alarm mode.
--- @param pl number player
function CppLogic.Logic.PlayerDeactivateAlarm(pl) end

--- upgrades a settler category.
--- @param pl number player
--- @param ucat number
function CppLogic.Logic.PlayerUpgradeSettlerCategory(pl, ucat) end

--- sets a players tax level.
--- @param pl number player
--- @param tl number
function CppLogic.Logic.PlayerSetTaxLevel(pl, tl) end

--- activates a players weathermachine/weathertower.
--- does not check if you are at that weather, or the availability of buildings.
--- @param pl number player
--- @param wt number weather
function CppLogic.Logic.PlayerActivateWeatherMachine(pl, wt) end

--- blesses a players settlers.
--- does not check if you have settlers of that category or a high enough monastery.
--- @param pl number player
--- @param bc number
function CppLogic.Logic.PlayerBlessSettlers(pl, bc) end

--- gets the sector of a position.
--- @param p Position
--- @return number sector
function CppLogic.Logic.LandscapeGetSector(p) end

--- gets the nearest unblocked position in range and in a specific sector.
--- @param p Position
--- @param sector number
--- @param range number
--- @return Position|nil
function CppLogic.Logic.LandscapeGetNearestUnblockedPosInSector(p, sector, range) end

--- gets the terrain type of a position.
--- quads resolution (pos/100/4), input full pos.
--- @param p Position
--- @return number terrainType
function CppLogic.Logic.LandscapeGetTerrainType(p) end
--- gets the water type of a position.
--- quads resolution (pos/100/4), input full pos.
--- @param p Position
--- @return number waterType
function CppLogic.Logic.LandscapeGetWaterType(p) end
--- gets the water height of a position.
--- quads resolution (pos/100/4), input full pos.
--- @param p Position
--- @return number waterHeight
function CppLogic.Logic.LandscapeGetWaterHeight(p) end
--- gets the terrain height of a position.
--- height resolution (pos/100), input full pos.
--- @param p Position
--- @return number terrainHeight
function CppLogic.Logic.LandscapeGetTerrainHeight(p) end
--- gets the terrain vertex color of a position.
--- height resolution (pos/100), input full pos.
--- @param p Position
--- @return number terrainVertexColor
--- @return number alpha
--- @return number red
--- @return number green
--- @return number blue
function CppLogic.Logic.LandscapeGetTerrainVertexColor(p) end
--- gets the blocking of a position.
--- height resolution (pos/100), input full pos.
--- @param p Position
--- @return number blockingInfo bitfield (Blocking -> 1, BridgeArea -> 2, BuildBlock -> 4, TerrainSlope -> 8)
function CppLogic.Logic.LandscapeGetBlocking(p) end
--- gets the bridge height of a position.
--- quads resolution (pos/100/4), input full pos.
--- is 0 if not set by a bridge that was there at some point.
--- bridges set the height in a rect +500, the dont seem to remove the heigth.
--- to check if bridge height is active, use CppLogic.Logic.LandscapeGetBlocking.
--- @param p Position
--- @return number bridgeHeight
function CppLogic.Logic.LandscapeGetBridgeHeight(p) end

--- navigates from from to to and returns a path.
--- a path is a array of positions, with the first position at path[table.getn(path)] and the last position at path[1].
--- if no path is found, returns nil.
--- throws if from or to is blocked.
--- navigation is done via SHoK settler pathfinding.
--- @param from Position
--- @param to Position
--- @return Position[]|nil path
function CppLogic.Logic.Navigate(from, to) end

--- enables Events.LOGIC_EVENT_ENTITY_HURT_ENTITY trigger, even if attacker is 0 and allows access to more info for that event.
--- also sets the hero as attacker on bomb explode.
--- fires trigger CPPLOGIC_EVENT_ON_ENTITY_KILLS_ENTITY on killing an entity.
--- does not work with SCELoader.
--- status of this gets saved into a savegame.
--- @param b boolean enable (optional, default true)
function CppLogic.Logic.EnableAllHurtEntityTrigger(b) end

--- enables entity max hp to be modified by techs.
--- does not work with SCELoader.
--- status of this gets saved into a savegame.
--- use a techs HitpointModifier to set up the boni.
--- then use ModifyHitpoints on settlers and CppLogic.EntityType.Building.AddHPTechMod for buildings to add the techs.
function CppLogic.Logic.EnableMaxHPTechMod() end

--- gets the damage that is going to be dealt in a Events.LOGIC_EVENT_ENTITY_HURT_ENTITY trigger.
--- requires activated CppLogic.Logic.EnableAllHurtEntityTrigger.
--- - CppLogic.Logic.FixSnipeDamage is required for damage source AbilitySnipe.
--- - cannonball damage source override requires CppLogic.Combat.EnableAoEProjectileFix().
--- does not work with SCELoader.
--- @return number dmg
--- @return number damageSource
--- @return number attackerPlayer
function CppLogic.Logic.HurtEntityGetDamage() end
--- sets the damage that is going to be dealt in a Events.LOGIC_EVENT_ENTITY_HURT_ENTITY trigger.
--- requires activated CppLogic.Logic.EnableAllHurtEntityTrigger.
--- does not work with SCELoader.
--- @param dmg number
function CppLogic.Logic.HurtEntitySetDamage(dmg) end

--- gets all loaded archives and folders.
--- @return string[] loadorder
function CppLogic.Logic.GetLoadOrder() end
--- adds a archive on top of your load order.
--- @param arch string archive, relative to current dir, so "extra2\\shr\\maps\\user\\map.s5x for a map file
--- @return ArchivePopHelper
function CppLogic.Logic.AddArchive(arch) end
--- removes the top archive. may only remove archives that you added yourself.
function CppLogic.Logic.RemoveTopArchive() end
--- tries to locate the file in the loadorder and return its archive/folder.
--- requires \\ as path separator and no leading data\\, also requires no absolute path (starting with a drive letter).
---@param file string
---@return string|nil archive
---@return number|nil archiveIndex
function CppLogic.Logic.GetArchiveOfFile(file) end

--- color by color index.
--- does not work with SCELoader.
--- @param index number
--- @return number r
--- @return number g
--- @return number b
--- @return number a
function CppLogic.Logic.GetColorByColorIndex(index) end
--- color by color index. Refreshes the color of players.
--- does not work with SCELoader.
--- @param index number
--- @param r number
--- @param g number
--- @param b number
--- @param a number (optional, default 255)
function CppLogic.Logic.SetColorByColorIndex(index, r, g, b, a) end

--- enables the trigger CPPLOGIC_EVENT_ON_PAYDAY.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
function CppLogic.Logic.SetPaydayCallback() end

--- sets a function to be called for building placement checks.
--- only gets called if all usual conditions for placement are satisfied.
--- set to nil to remove.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param func fun(entityType:number, playerId:number, pos:Position, rotation:number, buildOnID:number):boolean to be called (entitytype, playerId, pos, rotation, buildOnID)->canBuild
function CppLogic.Logic.SetPlaceBuildingAdditionalCheck(func) end

--- sets if leader regenration regenerates troop hp.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param b boolean
function CppLogic.Logic.SetLeadersRegenerateTroopHealth(b) end

--- sets a stringtabletexts accosiated text. automatically resets on leaving the game.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- if called from mainmenu, only affects mainmenu, not ingame.
--- @param key string key to replace
--- @param text string replacement string or nil to restore default
function CppLogic.Logic.SetStringTableText(key, text) end

--- deprecated
--- serf ui place building rotaton.
--- does not work with SCELoader.
--- @return number rotation
function CppLogic.Logic.GetPlaceBuildingRotation() end
--- deprecated
--- serf ui place building rotaton.
--- does not work with SCELoader.
--- @param r number
function CppLogic.Logic.SetPlaceBuildingRotation(r) end

--- fixes snipe task, now it actually shoots a projectile which does damage.
--- status of this gets saved into a savegame.
--- @param override nil|fun(sniper:number, target:number, dmg:number):number (optional, default nil) (sniper, target, dmg)->dmgOverride called before the projectile gets fired, can change the damage)
function CppLogic.Logic.FixSnipeDamage(override) end

--- gets the current weather GFX state (4th param of Logic.AddWeatherElement).
--- @return number weatherGFX
function CppLogic.Logic.GetCurrentWeatherGFXState() end

--- @class WeatherElement
local WeatherElement = {State=1,GFX=1,IsPeriodic=true,Length=1,Forerun=1}

--- gets the current weather queue.
--- @return table<number,WeatherElement> weatherQueue table [startOffset]={State,GFX,IsPeriodic,Length,Forerun}
--- @return number currentOffset
--- @return number nextPeriodicOffset (sum of all periodic element length+forerun == weather loop length)
function CppLogic.Logic.GetWeatherQueue() end

--- clears the weather queue and adds an initial (periodic) first element.
--- @param state number weatherstate
--- @param duration number duration in sec > 5
--- @param forerun number forerun in sec >0 <=10
--- @param gfx number gfxindex >0
--- @param transition number transition in sec >forerun <=200
function CppLogic.Logic.ClearWeatherQueueAndAddInitial(state, duration, forerun, gfx, transition) end

--- sets a func to be called, if a tasklist contains TASK_LUA_FUNC.
--- define the task as <Task classname="EGL::CTaskArgsInteger" classid="0xb3f8356d"><TaskType>TASK_LUA_FUNC</TaskType> in the tasklist to use it.
--- <Value> gets passed to your func, to identify what action should be executed.
--- use the parameter funcs moveto(pos, ignorepathing, noncancelable) and settl(tl) to move the entity or set the task list.
--- if you use moveto with ignorepathing, make sure the tasklist cannot be cancelled and that the target pos is actually not blocked.
--- return true, to call the function again next tick (not available if you have used moveto or settl).
--- do not save&use moveto and settl later, will cause crashes!
--- do not use moveto if you are repeating the call via return true.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param func fun(entityid:number, value:number, moveto:fun(p:Position,ignorePathing:boolean,nonCancelable:boolean), settl:fun(tl:number)):boolean ->repeat
function CppLogic.Logic.SetLuaTaskListFunc(func) end

--- changes all occurences of TASK_WAIT_FOR_ANIM to TASK_WAIT_FOR_ANIM_NON_CANCELABLE in the give tasklist.
--- this prevents new commands canceling the task.
--- does not work with SCELoader.
--- @param tl number tasklist
--- @param tind number task index (optional, default -1) id <0 full tasklist gets searched
function CppLogic.Logic.TaskListMakeWaitForAnimsUnCancelable(tl, tind) end
--- changes all occurences of TASK_WAIT_FOR_ANIM_NON_CANCELABLE to TASK_WAIT_FOR_ANIM in the give tasklist.
--- does not work with SCELoader.
--- @param tl number tasklist
--- @param tind number task index (optional, default -1) id <0 full tasklist gets searched
function CppLogic.Logic.TaskListMakeWaitForAnimsCancelable(tl, tind) end

--- prevents changes in tasklists to cancel TASK_WAIT_FOR_ANIM_NON_CANCELABLE. other states may still be cancelled.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param b boolean|nil (default false)
function CppLogic.Logic.TaskListSetChangeTaskListCheckUncancelable(b) end

--- inserts a TASK_WAIT_FOR_LATEST_ATTACK task into the tasklist at the position tind.
--- @param tl number tasklist
--- @param tind number task index
function CppLogic.Logic.TaskListInsertWaitForLatestAttack(tl, tind) end
--- inserts a TASK_SET_LATEST_ATTACK_TURN task into the tasklist at the position tind.
--- @param tl number tasklist
--- @param tind number task index
function CppLogic.Logic.TaskListInsertSetLatestAttack(tl, tind) end
--- removes all TASK_SET_LATEST_ATTACK_TURN and TASK_WAIT_FOR_LATEST_ATTACK tasks from the tasklist
--- @param tl number tasklist
function CppLogic.Logic.TaskListRemoveLatestAttack(tl) end

--- enables/disables fixing movement tasks when a building is placed on top of a moving entity.
--- with the fix active, issues a new move task to the same position, instead of potentially skipping the move.
--- status of this gets saved into a savegame.
--- @param f boolean enabled
function CppLogic.Logic.EnableBuildOnMovementFix(f) end

--- returns the next position where a building can be build accoring to blocking.
--- position can still be blocked by hostile settlers.
--- for buildon entitiy types, ignores p.r and returns the position+rotation of the nearest free base entity.
--- for non buildon entity types, checks for placement with p.r and returns rotation unchanged.
--- if no valid position can be found, returns {-1,-1,0}.
--- @param ety number entitytype
--- @param p PositionR with rotation
--- @param range number|nil area (optional, default logic.xml/BuildingPlacementSnapDistance)
--- @return PositionR pos with rotation
function CppLogic.Logic.GetNearestFreePosForBuilding(ety, p, range) end

--- performs the same blocking and sector update that a weather change would have done.
function CppLogic.Logic.BlockingUpdateWeatherChange() end

--- enables/disables applying the effects of experience level 1 and applying a negative MissChance on any level.
--- status of this gets saved into a savegame.
--- @param active boolean
function CppLogic.Logic.EnableExperienceClassFix(active) end

--- returns the duration of an animation in seconds.
--- @param anim number
--- @return number seconds
function CppLogic.Logic.GetAnimationDuration(anim) end

--- returns the global trade data.
--- local player trade data is accessible in Logic.
--- @param rt number
--- @return number baseprice (only used at start of map)
--- @return number minprice (respected n price changes)
--- @return number maxprice (respected n price changes)
--- @return number inflation (only used at start of map)
--- @return number deflation (only used at start of map)
--- @return number workamount
function CppLogic.Logic.GetTradeDataForResource(rt) end
--- sets the global trade data. (nil on any value leaves it unmodified).
--- local player trade data is accessible in Logic.
--- @param rt number
--- @param baseprice number|nil (only used at start of map)
--- @param minprice number|nil (respected n price changes)
--- @param maxprice number|nil (respected n price changes)
--- @param inflation number|nil (only used at start of map)
--- @param deflation number|nil (only used at start of map)
--- @param workamount number|nil
function CppLogic.Logic.SetTradeDataForResource(rt, baseprice, minprice, maxprice, inflation, deflation, workamount) end

--- enables/disables resource triggers (Events.CPPLOGIC_EVENT_ON_RESOURCE_REFINED, Events.CPPLOGIC_EVENT_ON_RESOURCE_MINED and Events.CPPLOGIC_EVENT_ON_REFINER_SUPPLY_TAKEN).
--- also fixes 2 mining bugs: almost empty mines mining not existent resources and serfs not putting their resources into the resource mined statistic.
--- the optional refinerFix fixes refiner resource consumption, requires external editing of tasklists and enabled.
--- status of this gets saved into a savegame.
--- @param enabled boolean|nil (default true)
--- @param refinerFix boolean|nil (default false)
function CppLogic.Logic.EnableResourceTriggers(enabled, refinerFix) end

--- enables firing a Events.CPPLOGIC_EVENT_CAN_BUY_SETTLER trigger when:
--- - automatically spawning a worker for some workplace
--- - GUI.BuyLeader (net event handler)
--- - Logic.BarracksBuyLeader
--- - GUI.BuySoldier (net event handler)
--- - GUI.BuySerf (net event handler)
function CppLogic.Logic.EnableSettlerBuyTriggers() end

--- gets data of a Events.CPPLOGIC_EVENT_CAN_BUY_SETTLER
--- @return number typeToBuy
--- @return number targetId workplace or leader or 0
--- @return boolean hasVCSpace
--- @return boolean hasCost
--- @return boolean hasMotivation
--- @return boolean isNotAlarm
--- @return boolean hasHQSpace
function CppLogic.Logic.GetSettlerBuyTriggerData() end
--- sets data of a Events.CPPLOGIC_EVENT_CAN_BUY_SETTLER
--- @param hasVCSpace boolean
--- @param hasCost boolean
--- @param hasMotivation boolean
--- @param isNotAlarm boolean
--- @param hasHQSpace boolean
function CppLogic.Logic.SetSettlerBuyTriggerData(hasVCSpace, hasCost, hasMotivation, isNotAlarm, hasHQSpace) end

--- gets the exploration status of a position.
--- you may need to use Logic.ActivateUpdateOfExplorationForAllPlayers to use it with AI players.
---@param player number
---@param pos Position
---@return boolean currentlyVisible
---@return boolean wasVisible
---@return boolean update
function CppLogic.Logic.IsPositionExplored(player, pos) end

--- sets the exploration status of an area (only the was visible status, currently visible gets updated too often).
--- affects the area inside the AxisAlignedBoundingBox formed by pos1 and pos2 (you may set pos1=pos2 to affect only this one position).
---@param player number
---@param pos1 Position
---@param pos2 Position
---@param explored boolean
function CppLogic.Logic.SetPositionExploration(player, pos1, pos2, explored) end

--- dumps a task list for dev purposes
---@param tl number|string
---@return table
function CppLogic.Logic.DumpTaskList(tl) end

--- queries the attraction usage of all workers of a player
---@param player number
---@return number
function CppLogic.Logic.PlayerGetWorkerAttraction(player) end

--- queries the attraction usage of all leaders and soldiers of a player
---@param player number
---@return number
function CppLogic.Logic.PlayerGetMilitaryAttraction(player) end

--- queries the attraction usage of all serfs of a player
---@param player number
---@return number
function CppLogic.Logic.PlayerGetSerfAttraction(player) end

--- enables foundries constructing a cannon blocking the VC space needed for them.
--- status gets saved into savegame.
---@param e boolean
function CppLogic.Logic.EnableCannonInProgressAttraction(e) end

--- resource doodads wit this category do not get autodestroyed if empty.
--- set to nil to disable.
--- status gets saved into savegame.
---@param e number|string|nil
function CppLogic.Logic.EnableRefillabeMineNoAutoDestroy(e) end

--- gets the player name
---@param pid number
---@return string
---@return boolean isSTTKey
function CppLogic.Logic.GetPlayerName(pid) end

--- dumps game logic
--- dev only, slow!
---@return table
function CppLogic.Logic.DumpGameLogic() end

--- ui command callback.
--- func parameters are (eventId, eventData, writeback).
--- function can return true to skip further event execution.
--- call writeback(eventData) with a modified eventData to change event values.
--- writeback has the event as a upvalue, do not save it and call it later, you will override a random c stack position!
--- status of this gets saved into a savegame.
--- @param f fun(eventId:number, eventData:table, writeBack:fun(eventData:table))
function CppLogic.Logic.UICommands.SetCallback(f) end
--- ui command callback.
--- status of this gets saved into a savegame.
function CppLogic.Logic.UICommands.UnSetCallback() end

--- renders only a model without any entity or effect.
--- automatically gets cleared when the userdata is gc'd / lua state gets closed (gets saved into the lua state).
--- rendering happens regardless of local player FoW and camera pos, use it only when needed.
--- Transform options: Set=0 (removes any other), Multiply=1 (default), ReverseMultiply=2 (as if called with Multiply in opposite order)
--- usual order for transforms is: translate, scale, rotate.
--- remember to set the model before using transforms.
--- should not affect mp in any way (even if not synced), as for gamelogic is concerned, there is nothing there (not tested).
--- @class LogicModel
local LogicModel = {}
--- clears the model and all its data, removes the object from rendering.
function LogicModel:Clear() end
--- sets the model to display. resets transform to identity (position 0/0, no rotation and scale).
--- @param mid number model to display
function LogicModel:SetModel(mid) end
--- translates (moves) the model (if it was not moved, it is at 0/0).
--- @param pos Position position for the center of the model to be
--- @param height number|nil (optional, default 0) height for the model
--- @param tro number|nil (optional, default Multiply) Transform Operation, see LogicModel
--- @param addTerraiHeight boolean|nil (optional, default true) if the terrain height at p should be added to height
--- @param floats boolean|nil (optional, default false) if true uses max of terrain and water height for addTerraiHeight
function LogicModel:Translate(pos, height, tro, addTerraiHeight, floats) end
--- scales the model.
--- scaling all 3 axis differently is currently not supported, but should be easy to implement. ask me if needed.
--- @param s number scale
--- @param tro number|nil (optional, default Multiply) Transform Operation, see LogicModel
function LogicModel:Scale(s, tro) end
--- rotates the model.
--- curretly rotates in the usual way, so the feet stay always on the ground. other rotations should be possible, ask me if needed.
--- @param r number rotation (in degrees)
--- @param tro number|nil (optional, default Multiply) Transform Operation, see LogicModel
function LogicModel:Rotate(r, tro) end
--- resets the transform of the model to identity (position 0/0, no rotation and scale).
function LogicModel:ResetTransform() end
--- sets the player color for this model.
--- @param pl number player id
function LogicModel:SetColorByPlayer(pl) end
--- disables the shadow of a model. reset the model to reenable.
function LogicModel:DisableShadow() end
--- disables all particle effects (smoke...). reset the model to reenable.
function LogicModel:DisableParticleEffects() end
--- disables the terraindecal of a model. reset the model to reenable.
function LogicModel:DisableTerrainDecal() end
--- sets a color to modulate (multiply with texture).
--- @param r number red, [0-255]
--- @param g number green, [0-255]
--- @param b number blue, [0-255]
--- @param a number alpha, [0-255] (optional, default 255) (does not work with all models)
function LogicModel:SetColorModulate(r, g, b, a) end
--- sets the animation to be played. (some models do not work without an animation, and some have no animation).
--- @param anim number animation id
function LogicModel:SetAnim(anim) end
--- sets the time of the animation to be played. may move time forwards or backwards.
--- for normal animations, it is recommended to call this from some GUIUpdate function.
--- if you advance an animation for longer than its duration, it gets looped.
--- @param t number|nil time (optional, default current time)
--- @param absolute boolean|nil if true, subtracts the animation start time (optional, default true)
function LogicModel:SetTimeOfAnim(t, absolute) end

--- creates a new LogicModel.
--- @return LogicModel
function CppLogic.Logic.CreateFreeModel(f) end

--- compiles a lua chunk.
--- asserts if kimichuras dlls are detected.
--- @param code string to compile, sourcecode or binary
--- @return function|string to execute the compiled code, or error message
--- @return boolean successful
function CppLogic.API.Eval(code) end

--- logs a string to the current settlers log.
--- @param str string the string to log
function CppLogic.API.Log(str) end

--- creates a stack trace of the current executing lua functions.
--- @return string trace
function CppLogic.API.StackTrace() end

--- reads a file into a string.
--- can only read files in data\maps\externalmap\
--- @param file string path to the file
--- @return string file content
function CppLogic.API.ReadFileAsString(file) end

--- checks if a file exists.
--- @param file string path to the file
--- @return boolean
function CppLogic.API.DoesFileExist(file) end

--- compiles a lua chunk and executes it. shows the script in the debugger as file with the defined name.
--- asserts if kimichuras dlls are detected.
--- @param code string to compile, sourcecode or binary
--- @param name string name to show in debugger
--- @return any return values of the executed code
function CppLogic.API.DoString(code, name) end

--- gets the path to a map. internal data or s5x file.
--- if nothrow is set, returns dummy variables + error message on invalid map
--- @param mapname string filename of the map
--- @param typ number type code
--- @param campname string campagn name
--- @param nothrow boolean|nil default false
--- @return string path
--- @return string GUID
--- @return number[] keys
--- @return string|nil error
function CppLogic.API.MapGetDataPath(mapname, typ, campname, nothrow) end

--- returns the map a save is accociated with.
--- does not check, if the save is still valid, just if it exists.
--- if nothrow is set, returns dummy variables + false on invalid save
--- @param save string
--- @param nothrow boolean|nil default false
--- @return string mapname
--- @return number typ
--- @return string campname
--- @return string GUID
--- @return boolean valid
function CppLogic.API.SaveGetMapInfo(save, nothrow) end

--- returns the complete GDB as multilayered table.
--- keys are split at every \, and put into a layered map.
--- @return table
function CppLogic.API.GetGDB() end

--- removes a key in GDB.
--- the key gets split at every \ to follow down the layered map. only the last part of the key gets removed, so its neighbors stay.
--- you can remove a node, all its children will automatically get removed as well.
--- does not automatically save GDB.
--- be careful when removing keys used by SHoK, you may break things.
--- example: you have the structure: `data = {root = {a=1, b=2, c=3}}`
--- if you remove "data\\root\\a", the result is `data = {root = {b=2, c=3}}`
--- if you then remove "data\\root", the result is `data = {}`
--- @param key string
function CppLogic.API.RemoveGDBKey(key) end

--- saves a value in the mainmenu lua state (gets only removed on game restart, not on map load/start).
--- @param name string key
--- @param value string|number|nil data to store, only string/number/nil allowed
function CppLogic.API.RuntimeStoreSet(name, value) end
--- loads a value from the mainmenu lua state (gets only removed on game restart, not on map load/start).
--- @param name string key
--- @return string|number|nil value data
function CppLogic.API.RuntimeStoreGet(name) end

--- creates the global tables Animations, TerrainTypes, WaterTypes, ArmorClasses, ExperienceClasses.
--- data gets read directly from the game, so anything you can use is in there.
function CppLogic.API.CreateExtraDataTables() end

--- saves a lua variable into a persistent map file.
--- persistent map files are saved in the folder PersistentMapFiles next to a users savegames.
--- the user has to create that folder manually to enable persistent map files for himself.
--- persistent map files are identified by the GUID of their respective Map.
--- the lua variable to be saved can be anything that adheres to the rules specified for serialization of a lua state.
--- note, that saving _G does not get resolved to the new _G on loading, you will instead gain a new table with all of _Gs previous contents.
---@param var any data to save
---@param mapname string name of map
---@param maptype number map type
---@param cname string|nil campagn name
---@param dataOnly boolean|nil if true, asserts on trying to serialize any function or userdata
---@return string file path to the saved file
function CppLogic.API.SavePersistentMapFile(var, mapname, maptype, cname, dataOnly) end
--- loads a previously saved persistent map file.
--- throws if file does not exist.
---@param mapname string name of map
---@param maptype number map type
---@param cname string|nil campagn name
---@param dataOnly boolean|nil if true, asserts on trying to deserialize any function or userdata
---@return any var loaded data
function CppLogic.API.LoadPersistentMapFile(mapname, maptype, cname, dataOnly) end
--- checks if a persistent map file exists.
--- also checks if the persistent map file system is enabled in this particular user.
---@param mapname string name of map
---@param maptype number map type
---@param cname string|nil campagn name
---@return boolean fileExists
---@return boolean enabled
function CppLogic.API.HasPersistentMapFile(mapname, maptype, cname) end

--- @class FuncData
local FuncData = {name="", namewhat="", nups=1, short_src="", linedefined=1, what=""}

--- returns debug information about a function.
--- @param f function func to check
--- @return table FuncData {name, namewhat, nups, short_src, linedefined, what}, see lua_getinfo for what actually gets returned
function CppLogic.API.GetFuncDebug(f) end

--- returns the main SHoK game thread (the thread that opened the lua state).
--- @return number threadid
function CppLogic.API.GetMainThreadID() end

--- returns the current running thread (this should always be the main thread, or the luadebugger thread).
--- @return number threadid
function CppLogic.API.GetCurrentThreadID() end

--- returns the current time as a single number.
--- useful for random seeds, use Framework.GetSystemTimeDateString if you want somwthing human readable.
--- @return number time
function CppLogic.API.GetCurrentTime() end

--- returns the current active cutscene (empty string if no cutscnene active)
--- @return string name
function CppLogic.API.GetCurrentCutscene() end

--- reloads the externalmaps (s5x and folder) (only in mainmenu)
--- run LoadMap.Init() to refresh the UI afterwards
function CppLogic.API.ReloadExternalmaps() end

---enables triggers to access functions in (nested) tables.
--- for the trigger func x.y checks in this order:
--- - _G["x.y"]
--- - eval("return x.y")
--- 
---affects both condition and action func.
---status gets saved into savegames.
---@param active boolean
function CppLogic.API.EnableScriptTriggerEval(active) end

---@class CPPLMonitorInfo
---@field Name string
---@field H number
---@field W number
local CPPLMonitorInfo = {}

---returns info on all available monitors
---@return CPPLMonitorInfo[]
function CppLogic.API.GetMonitors() end

---@class RNG
local RNG = {}

--- returns a random integer in [min,max] (or [1,min], if max is not set)
--- @param min number
--- @param max number|nil
function RNG:Int(min, max) end

--- returns a random number (double) between [min,max) (or [1,min), if max is not set, or [0,1) if both not set)
--- @param min number|nil
--- @param max number|nil
function RNG:Number(min, max) end

--- creates a RNG object.
--- each RNG object has its own seed (not shared with other RNG objects or Logic/math)
--- serializable
--- @param seed number|nil seed for the RNG, gets derived from current time if nil
--- @return RNG rng
function CppLogic.API.CreateRandomNumberGenerator(seed) end

--- deals damage to a target.
--- calls respective hurt entity trigger.
--- @param target number entity to be damaged
--- @param damage number damage to deal
--- @param attacker number|nil entity dealing the damage (gets xp from kill, statistics...), (default 0)
--- @param attackerPlayerFallback number|nil player fallback for statistics, if attacker not set (default 0)
--- @param uiFeedback boolean|nil send battle feedback (default true)
--- @param xp boolean|nil give attacker xp for kills (default true)
--- @param addStat boolean|nil add kills to statistics (default true)
--- @param source number|nil what deals damage, used for triggers (default AdvancedDealDamageSource.Script)
function CppLogic.Combat.DealDamage(target, damage, attacker, attackerPlayerFallback, uiFeedback, xp, addStat, source) end

--- deals damage in an area.
--- if attackerId (or player) are set, damages only targets hostile to them.
--- if dmgclass not set, it gets ignored in the damage calculation.
--- damage gets reduced, the furher away from impact the target is.
--- @param attackerId number attacking entity (optional, 0 if not used)
--- @param x number X coorinate of center
--- @param y number Y coorinate of center
--- @param r number range of damage dealt
--- @param dmg number damage
--- @param player number|nil attacking player (optional, 0/nil if not used)
--- @param dmgclass number|nil damageclass (optional, 0/nil if not used)
--- @param uiFeedback boolean|nil send battle feedback (default true)
--- @param xp boolean|nil give attacker xp for kills (default true)
--- @param addStat boolean|nil add kills to statistics (default true)
--- @param source number|nil what deals damage, used for triggers (default AdvancedDealDamageSource.Script)
function CppLogic.Combat.DealAoEDamage(attackerId, x, y, r, dmg, player, dmgclass, uiFeedback, xp, addStat, source) end

--- enables AoE projectile fix.
--- when enabled, cannons and similar AoE projectiles use the entitytypes damageclass.
--- (also fixes autocannon rotation)
--- status of this gets saved into a savegame.
function CppLogic.Combat.EnableAoEProjectileFix() end
--- disables AoE projectile fix.
--- when enabled, cannons and similar AoE projectiles use the entitytypes damageclass.
--- status of this gets saved into a savegame.
function CppLogic.Combat.DisableAoEProjectileFix() end

--- enables camoflage projectile fix.
--- when enabled, camoflage does not get reset by ranged attacks, also all attackers get cleared on entering camo.
--- requires and automatically enables CppLogic.Logic.EnableAllHurtEntityTrigger.
--- status of this gets saved into a savegame.
function CppLogic.Combat.EnableCamoFix() end
--- disabes camoflage projectile fix.
--- when enabled, camoflage does not get reset by ranged attacks, also all attackers get cleared on entering camo.
--- status of this gets saved into a savegame.
function CppLogic.Combat.DisableCamoFix() end

--- @class Predicate
local Predicate = {}

--- iterates over all entities that match a predicate.
--- you may create/destroy entities while iterating, even the current one.
--- perfect to use with for loop.
--- needs at least 1 predicate.
--- examples:
--- - for id in CppLogic.Entity.EntityIterator(...) do Message(id) end  
--- - for id, rsqu, prio in CppLogic.Entity.EntityIterator(CppLogic.Entity.Predicates.InCircle(...), ...) do Message(id.."   "..r) end  
--- @param ... Predicate userdata
--- @return fun():number,number,number nextEntity
--- @return nil iteratorStatus
--- @return nil
function CppLogic.Entity.EntityIterator(...) end

--- iterates over all entities of one or more players that match a predicate.
--- cannot iterate over player 0 entities.
--- usually faster than EntityIterator and a player predicate.
--- do not create/destroy entities while iterating.
--- you have to manually create an and predicate.
--- perfect to use with for loop.
--- needs exactly 1 predicate.
--- examples:
--- - for id in CppLogic.Entity.PlayerEntityIterator(...) do Message(id) end  
--- - for id, rsqu, prio in CppLogic.Entity.PlayerEntityIterator(CppLogic.Entity.Predicates.InCircle(...), 1) do Message(id.."   "..r) end  
--- @param pred Predicate userdata
--- @param ... number players
--- @return fun():number,number,number nextEntity
--- @return nil iteratorStatus
--- @return nil
function CppLogic.Entity.PlayerEntityIterator(pred, ...) end

--- iterates over all entities in a circle based on Regions that match a predicate.
--- for small areas faster than EntityIterator and a InCircle predicate.
--- do not create/destroy entities while iterating.
--- automatically creates a InCircle predicate.
--- perfect to use with for loop.
--- examples:
--- - for id in CppLogic.Entity.RegionEntityIteratorCircle(...) do Message(id) end  
--- - for id, rsqu, prio in CppLogic.Entity.RegionEntityIteratorCircle(...) do Message(id.."   "..r) end  
--- @param pos Position center of circle
--- @param range number radius of circle
--- @param accessCategories number accessCategories bitfield (2->settler, 4->animal, 8->building, 16->resource doodad, 32->static, 64->ornamental)
--- @param ... Predicate
--- @return fun():number,number,number nextEntity
--- @return nil iteratorStatus
--- @return nil
function CppLogic.Entity.RegionEntityIteratorCircle(pos, range, accessCategories, ...) end

--- iterates over all entities in a rect based on Regions that match a predicate.
--- for small areas faster than EntityIterator and a InRect predicate.
--- do not create/destroy entities while iterating.
--- automatically creates a InRect predicate.
--- perfect to use with for loop.
--- examples:
--- - for id in CppLogic.Entity.RegionEntityIteratorRect(...) do Message(id) end  
--- - for id, rsqu, prio in CppLogic.Entity.RegionEntityIteratorRect(...) do Message(id.."   "..r) end  
--- @param pos1 Position position 1
--- @param pos2 Position posotion 2
--- @param accessCategories number accessCategories bitfield (2->settler, 4->animal, 8->building, 16->resource doodad, 32->static, 64->ornamental)
--- @param ... Predicate
--- @return fun():number,number,number nextEntity
--- @return nil iteratorStatus
--- @return nil
function CppLogic.Entity.RegionEntityIteratorRect(pos1, pos2, accessCategories, ...) end

--- gets all entities that match a predicate as a table.
--- @param ... Predicate userdata
--- @return number[] ids
function CppLogic.Entity.EntityIteratorTableize(...) end

--- counts all entities that match a predicate as a table.
--- @param ... Predicate userdata
--- @return number c
function CppLogic.Entity.EntityIteratorCount(...) end

--- returns the nearest entity. requires InCircle predicate.
--- @param ... Predicate userdata
--- @return number id may be 0
--- @return number range
function CppLogic.Entity.EntityIteratorGetNearest(...) end

--- checks an entity agains a predicate.
--- @param id number entity
--- @param pred Predicate
--- @return boolean
--- @return number range
--- @return number priority
function CppLogic.Entity.CheckPredicate(id, pred) end

--- creates a predicate that performs an and of multiple predicates.
--- @param ... Predicate
--- @return Predicate
function CppLogic.Entity.Predicates.And(...) end

--- creates a predicate that performs an or of multiple predicates.
--- @param ... Predicate
--- @return Predicate
function CppLogic.Entity.Predicates.Or(...) end

--- creates a predicate that performs an not of anoher predicate.
--- @param p Predicate
--- @return Predicate
function CppLogic.Entity.Predicates.Not(p) end

--- creates a predicate that always matches, but sets the priority to prio if p matches it.
--- @param p Predicate
--- @param prio number
--- @return Predicate
function CppLogic.Entity.Predicates.SetPriority(p, prio) end

--- creates a predicate that checks for one entitytype.
--- @param etyp number entitytype to check against
--- @return Predicate
function CppLogic.Entity.Predicates.OfType(etyp) end

--- creates a predicate that checks for one player.
--- @param pl number player to check against
--- @return Predicate
function CppLogic.Entity.Predicates.OfPlayer(pl) end

--- creates a predicate that checks for an area.
--- when used in loop iterator, can also return the range squared for each matched entity as 2nd loop parameter (optional).
--- @param x number x coordinate of center
--- @param y number y coordinate of center
--- @param r number radius of circle
--- @return Predicate
function CppLogic.Entity.Predicates.InCircle(x, y, r) end

--- creates a predicate that checks for building.
--- (GGL::CConstructionSite entities get filtered out, their attached GGL::CBuilding not.)
--- @return Predicate
function CppLogic.Entity.Predicates.IsBuilding() end

--- creates a predicate that checks for building.
--- matches the GGL::CConstructionSite as well as the GGL::CBuilding under construction.
--- @return Predicate
function CppLogic.Entity.Predicates.PredicateIsBuildingOrConstructionSite() end

--- creates a predicate that checks for settlers.
--- @return Predicate
function CppLogic.Entity.Predicates.IsSettler() end

--- creates a predicate that checks for (player!=0 && (IsBuilding || IsSettler)).
--- @return Predicate
function CppLogic.Entity.Predicates.IsCombatRelevant() end

--- creates a predicate that checks for anything that is not a soldier.
--- @return Predicate
function CppLogic.Entity.Predicates.IsNotSoldier() end

--- creates a predicate that checks for an entitycategory.
--- @param ecat number EntityCategory to check against
--- @return Predicate
function CppLogic.Entity.Predicates.OfEntityCategory(ecat) end

--- creates a predicate that checks for any entitycategory.
--- @param ecat number EntityCategory to check against
--- @return Predicate
function CppLogic.Entity.Predicates.PredicateOfAnyEntityCategory(ecat, ...) end

--- creates a predicate that checks for any class.
--- @param classid number ClassID to check against
--- @return Predicate
function CppLogic.Entity.Predicates.PredicateOfAnyClass(classid, ...) end

--- creates a predicate that checks for a provided resource.
--- @param rty number ResourceType to check against
--- @return Predicate
function CppLogic.Entity.Predicates.ProvidesResource(rty) end

--- creates a predicate that checks for any players.
--- @param ... number players
--- @return Predicate
function CppLogic.Entity.Predicates.OfAnyPlayer(...) end

--- creates a predicate that checks for any entitytypes.
--- @param ... number entitytypes
--- @return Predicate
function CppLogic.Entity.Predicates.OfAnyEntityType(...) end

--- creates a predicate that checks for an rectangular area.
--- order of coordinates does not matter, just have to be grouped as x,y,x,y.
--- @param x1 number x coordinate 1
--- @param y1 number y coordinate 1
--- @param x2 number x coordinate 2
--- @param y2 number y coordinate 2
--- @return Predicate
function CppLogic.Entity.Predicates.InRect(x1, y1, x2, y2) end

--- creates a predicate that checks for ari or thieves invisibility.
--- @return Predicate
function CppLogic.Entity.Predicates.IsVisible() end

--- creates a predicate that checks for an upgrade category (settler or building, as defined in txpe xml).
--- @param ucat number UpgradeCategory to check against
--- @return Predicate
function CppLogic.Entity.Predicates.OfUpgradeCategory(ucat) end

--- creates a predicate that checks dead entities.
--- @return Predicate
function CppLogic.Entity.Predicates.IsAlive() end

--- creates a predicate that checks settlers not inside a building. (includes entering and leaving).
--- includes all sorts of buildings, workplaces/farms/residences/barracks...
--- @return Predicate
function CppLogic.Entity.Predicates.IsNotInBuilding() end

---@alias entity number|string

--- dumps entity to lua
--- dev only, slow!
---@param e entity
---@return table
function CppLogic.Entity.DumpEntity(e) end

--- entity task list index.
--- @param id entity
--- @return number index
function CppLogic.Entity.GetTaskListIndex(id) end
--- entity task list index.
--- @param id entity
--- @param i number index
function CppLogic.Entity.SetTaskListIndex(id, i) end

--- entity TaskList debug info, usefull for debugging TaskLists.
--- @param id entity
--- @return string tasklist
--- @return string|nil task
--- @return string|number|nil state
function CppLogic.Entity.Debug_GetTaskInfo(id) end

--- moving entity speed factor (set via logic).
--- @param id entity
--- @return number factor
function CppLogic.Entity.MovingEntityGetSpeedFactor(id) end

--- entity scale.
--- @param id entity
--- @return number scale
function CppLogic.Entity.GetScale(id) end
--- entity scale.
--- @param id entity
--- @param s number scale
function CppLogic.Entity.SetScale(id, s) end

--- entity check for soldier. (asserts if no entity).
--- @param id entity
--- @return boolean
function CppLogic.Entity.IsSoldier(id) end

--- moving entity target pos (best to change directly after a move command).
--- @param id entity
--- @return Position|PositionR position table, with optional rotation
function CppLogic.Entity.MovingEntityGetTargetPos(id) end
--- moving entity target pos (best to change directly after a move command).
--- @param id entity
--- @param p Position|PositionR position table, with optional rotation
function CppLogic.Entity.MovingEntitySetTargetPos(id, p) end

--- checks if a moving entity is fleing from a position
--- @param id entity
--- @param p Position
--- @return number
function CppLogic.Entity.MovingEntityIsFleeingFrom(id, p) end

--- entity exploration.
--- @param id entity
--- @return number exploration
function CppLogic.Entity.GetExploration(id) end

--- entity speed.
--- @param id entity
--- @return number speed
function CppLogic.Entity.GetSpeed(id) end

--- entity max range (including tech boni).
--- @param id entity
--- @return number max range
function CppLogic.Entity.GetAutoAttackMaxRange(id) end

--- entity model, from model override or entitytype model 0.
--- @param id entity
--- @return number model
function CppLogic.Entity.GetModel(id) end

--- checks if an entity is feared.
--- @param id entity
--- @return boolean|number feared false or fearing entity
function CppLogic.Entity.IsFeared(id) end

--- clears all attackers of an entity.
--- @param id entity
function CppLogic.Entity.ClearAttackers(id) end

--- limited lifespan remaining seconds.
--- @param id entity
--- @return number duration
function CppLogic.Entity.GetLimitedLifespanRemaining(id) end
--- limited lifespan remaining seconds.
--- @param id entity
--- @param t number duration
function CppLogic.Entity.SetLimitedLifespanRemaining(id, t) end

--- replaces a tree with its ResourceTree equivalent.
--- @param id entity
--- @return number|nil id new id or nil
function CppLogic.Entity.ReplaceWithResourceEntity(id) end

--- overrides an entities max hp.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param id entity
--- @param hp number|nil (optional, <0 to disable)
--- @param useBoni boolean|nil (optional)
function CppLogic.Entity.SetMaxHP(id, hp, useBoni) end

--- overrides an entities damage.
--- for autocannons (towers, heroabilities...) apply it to the turret (via Logic.GetFoundationTop(id)).
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param id entity
--- @param dmg number (<0 disable)
function CppLogic.Entity.SetDamage(id, dmg) end

--- overrides an entities armor.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param id entity
--- @param armor number (<0 disable)
function CppLogic.Entity.SetArmor(id, armor) end

--- overrides a settlers/buildings exploration.
--- use Logic for scriptentities.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param id entity
--- @param ex number (<0 disable)
function CppLogic.Entity.SetExploration(id, ex) end

--- overrides a settlers/autocannons max attack range.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param id entity
--- @param ran number (<0 disable)
function CppLogic.Entity.SetAutoAttackMaxRange(id, ran) end

--- overrides an entities display name.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param id entity
--- @param n string ("" disable)
function CppLogic.Entity.SetDisplayName(id, n) end
--- gets the display name of an entity.
--- @param id entity
--- @return string name
function CppLogic.Entity.GetDisplayName(id) end

--- clones all overrides.
--- @param from entity (can be id of last destroyed entity)
--- @param to entity 
function CppLogic.Entity.CloneOverrideData(from, to) end

--- heals an entity. for leaders its own hp is healed, then troop hp if enabled.
--- @param id entity
--- @param heal number
--- @param healTroopHp boolean|nil (optional, default false)
function CppLogic.Entity.PerformHeal(id, heal, healTroopHp) end

--- gets the currently engaged target entity of an entity.
--- (walking towards to engage does not count).
--- @param id entity
--- @return number target
function CppLogic.Entity.GetBattleTarget(id) end

--- gets target assigned to a leader, by direct command or automatically assigned.
--- (might be different from GetBattleTarget, if that is a soldier of this).
--- if id is not currently commanded to attack something, might be garbage (check command state/TL).
--- @param id entity
--- @return number target
function CppLogic.Entity.GetAttackCommandTarget(id) end

--- gets the produced and consumed resources of a entity.
--- requires the entity type having a CppLogic::Mod::ResourceTrackerBehavior behavior and activated CppLogic.Logic.EnableResourceTriggers.
--- @param id entity
--- @return CostInfo|nil produced
--- @return CostInfo|nil used
function CppLogic.Entity.GetTrackedResources(id) end

--- returns the full entityname->id mapping
--- @return table<string, number>
function CppLogic.Entity.GetAllScriptNameMappings() end

--- gets the leader of a soldier.
--- @param id entity
--- @return number id
function CppLogic.Entity.Settler.GetLeaderOfSoldier(id) end

---@alias settler entity

--- base unmodified movementspeed of a settler.
--- @param id settler
--- @return number movement speed
--- @return number rotation speed
function CppLogic.Entity.Settler.GetBaseMovementSpeed(id) end
--- base unmodified movementspeed of a settler.
--- @param id settler
--- @param move number|nil optional new move speed
--- @param rotate number|nil optional new rotation speed
function CppLogic.Entity.Settler.SetBaseMovementSpeed(id, move, rotate) end

--- settler current and max work time.
--- @param id entity
--- @return number workTime
--- @return number maxWorkTime
function CppLogic.Entity.Settler.WorkerGetCurrentWorkTime(id) end
--- settler current work time.
--- @param id entity
--- @param wt number work time
function CppLogic.Entity.Settler.WorkerSetCurrentWorkTime(id, wt) end

--- settler current motivation.
--- @param id entity
--- @return number moti
function CppLogic.Entity.Settler.WorkerGetMotivation(id) end
--- settler current motivation.
--- (if it is higher than the players max motivation, it will quickly be lowered to that)
--- @param id entity
--- @param m number change in motivation
--- @param r number reason (0->unknown (better not use if the worker could leave as a result), 1->Taxes, 2->Overtime, 3->NoWork, 4->NoFood, 5->NoRest, 6->NoPay)
function CppLogic.Entity.Settler.WorkerChangeMotivation(id, m, r) end

--- settler overhead widget.
--- 0->only name, 1->name+bar(anything), 2->worker, 3->name+bar(leader), 4->nothing.
--- @param id settler
--- @return number widgetCode
function CppLogic.Entity.Settler.GetOverheadWidget(id) end
--- settler overhead widget.
--- 0->only name, 1->name+bar(anything), 2->worker, 3->name+bar(leader), 4->nothing.
--- @param id settler
--- @param wid number widget code
function CppLogic.Entity.Settler.SetOverheadWidget(id, wid) end

--- settler experience class
--- @param id settler
--- @return number
function CppLogic.Entity.Settler.GetExperienceClass(id) end

--- camouflage duration left (ari: seconds, thief: invis ? 15 : 0).
--- @param id entity
--- @return number duration
function CppLogic.Entity.Settler.HeroGetCamouflageDurationLeft(id) end
--- camouflage duration left (only ari).
--- @param id entity
--- @param dur number duration
function CppLogic.Entity.Settler.HeroSetCamouflageDurationLeft(id, dur) end

--- time to thief camouflage.
--- @param id entity
--- @return number duration
function CppLogic.Entity.Settler.ThiefGetCamouflageTimeTo(id) end
--- time to thief camouflage.
--- @param id entity
--- @param dur number duration
function CppLogic.Entity.Settler.ThiefSetCamouflageTimeTo(id, dur) end

--- hero resurrection time.
--- @param id entity
--- @return number duration
function CppLogic.Entity.Settler.HeroGetResurrectionTime(id) end
--- hero resurrection time.
--- @param id entity
--- @param t number duration
function CppLogic.Entity.Settler.HeroSetResurrectionTime(id, t) end

--- resurrect a hero.
--- @param id entity
function CppLogic.Entity.Settler.HeroResurrect(id) end

--- set thief stolen resources.
--- @param id entity
--- @param ty number res type or 0
--- @param am number amount (optional)
function CppLogic.Entity.Settler.ThiefSetStolenResourceInfo(id, ty, am) end

--- checks visibility of an entity.
--- @param id entity
--- @return boolean
function CppLogic.Entity.Settler.IsVisible(id) end

--- checks if a settler is idle
--- this is in defend or hold position command, or having the type specific idle tasklist.
--- workers are never idle.
--- @param id entity
--- @return boolean
function CppLogic.Entity.Settler.IsIdle(id) end

--- command to send darios hawk.
--- decreases hawk range automatically, asserts if ability cannot be used.
--- @param id entity
--- @param p Position target
function CppLogic.Entity.Settler.CommandSendHawk(id, p) end

--- get darios hawk from dario. may be 0 (if dario is dead).
--- @param id entity
--- @return number
function CppLogic.Entity.Settler.GetHawkOfHero(id) end

--- command to inflict fear.
--- asserts if ability cannot be used.
--- @param id entity
function CppLogic.Entity.Settler.CommandInflictFear(id) end

--- command to place bomb.
--- asserts if ability cannot be used.
--- @param id entity
--- @param p Position target
function CppLogic.Entity.Settler.CommandPlaceBomb(id, p) end

--- command to place a cannon.
--- asserts if ability cannot be used, checks position.
--- does not check bottom & top entitytype.
--- @param id entity
--- @param p Position target
--- @param bottom number foundation type
--- @param top number cannon type
function CppLogic.Entity.Settler.CommandPlaceCannon(id, p, bottom, top) end

--- command to activate ranged effect.
--- asserts if ability cannot be used.
--- @param id entity
function CppLogic.Entity.Settler.CommandRangedEffect(id) end

--- command to perform circular attack.
--- asserts if ability cannot be used.
--- @param id entity
function CppLogic.Entity.Settler.CommandCircularAttack(id) end

--- command to summon.
--- asserts if ability cannot be used.
--- @param id entity
--- @return number ids of summoned entities
function CppLogic.Entity.Settler.CommandSummon(id) end

--- command to activate camoflage.
--- asserts if ability cannot be used.
--- @param id entity
function CppLogic.Entity.Settler.CommandActivateCamoflage(id) end

--- command to convert.
--- moves into range, asserts if ability cannot be used.
--- @param id entity
--- @param tid entity target
function CppLogic.Entity.Settler.CommandConvert(id, tid) end

--- command to snipe.
--- asserts if ability cannot be used, including out of range.
--- @param id entity
--- @param tid entity target
function CppLogic.Entity.Settler.CommandSnipe(id, tid) end

--- command to use shuriken.
--- asserts if ability cannot be used, including out of range.
--- @param id entity
--- @param tid entity target
function CppLogic.Entity.Settler.CommandShuriken(id, tid) end

--- command to motivate workers.
--- asserts if ability cannot be used, including out of range.
--- @param id entity
function CppLogic.Entity.Settler.CommandMotivateWorkers(id) end

--- command to use sabotage.
--- asserts if ability cannot be used. does not check tech requirement.
--- @param id entity
--- @param tid entity target
function CppLogic.Entity.Settler.CommandSabotage(id, tid) end

--- command to defuse a sabotage.
--- asserts if ability cannot be used. does not check tech requirement.
--- @param id entity
--- @param tid entity target
function CppLogic.Entity.Settler.CommandDefuse(id, tid) end

--- command to use binoculars.
--- asserts if ability cannot be used.
--- @param id entity
--- @param p Position target
function CppLogic.Entity.Settler.CommandBinocular(id, p) end

--- command to place a torch.
--- asserts if ability cannot be used. does not check tech requirement.
--- @param id entity
--- @param p Position target
function CppLogic.Entity.Settler.CommandPlaceTorch(id, p) end

--- command to search resources.
--- asserts if ability cannot be used. does not check tech requirement.
--- @param id entity
function CppLogic.Entity.Settler.CommandPointToRes(id) end

--- command to steal.
--- asserts if ability cannot be used.
--- @param id entity
--- @param tid entity target
function CppLogic.Entity.Settler.CommandStealFrom(id, tid) end

--- command to secure stolen goods.
--- asserts if ability cannot be used.
--- @param id entity
--- @param tid entity target
function CppLogic.Entity.Settler.CommandSecureGoods(id, tid) end

--- command to move.
--- asserts if ability cannot be used.
--- @param id entity
--- @param pos Position target
--- @param r number|nil target rotation (optional, nil if not used)
function CppLogic.Entity.Settler.CommandMove(id, pos, r) end

--- command to construct a building.
--- asserts if ability cannot be used.
--- @param id entity
--- @param tid entity
--- @return boolean s if a slot was free
function CppLogic.Entity.Settler.CommandSerfConstructBuilding(id, tid) end

--- command to construct a building.
--- asserts if ability cannot be used.
--- @param id entity
--- @param tid entity
--- @return boolean s if a slot was free
function CppLogic.Entity.Settler.CommandSerfRepairBuilding(id, tid) end

--- command to extract a resource.
--- automatically tries to convert trees to ResourceTrees, asserts if no resource doodad afterwards.
--- @param id entity
--- @param tid entity
--- @return number resourceEntityId (might be different than tid)
function CppLogic.Entity.Settler.CommandSerfExtract(id, tid) end

--- evables tigger Events.CPPLOGIC_EVENT_ON_CONVERT_ENTITY.
--- targetid is the old converted entity, it is no longer valid.
--- status of this gets saved into a savegame.
function CppLogic.Entity.Settler.EnableConversionHook() end
--- nop.
function CppLogic.Entity.Settler.DisableConversionHook() end

--- command to expell a settler.
--- @param id entity
function CppLogic.Entity.Settler.CommandExpell(id) end

--- command to turn a serf to a battleserf.
--- @param id entity
function CppLogic.Entity.Settler.CommandTurnSerfToBattleSerf(id) end
--- command to turn a battleserf back to a serf.
--- @param id entity
function CppLogic.Entity.Settler.CommandTurnBattleSerfToSerf(id) end

--- lighning strike command
--- @param id entity
--- @param t Position
function CppLogic.Entity.Settler.CommandLightningStrike(id, t) end

--- refill mines command
--- @param id entity
--- @param tid entity
function CppLogic.Entity.Settler.CommandRefillResourceDoodad(id, tid) end

--- shield cover command
--- @param id entity
function CppLogic.Entity.Settler.CommandShieldCover(id) end

--- resurrect command
--- @param id entity
--- @param tid entity
function CppLogic.Entity.Settler.CommandResurrect(id, tid) end

--- bombardment command
--- @param id entity
--- @param p Position
function CppLogic.Entity.Settler.CommandBombardment(id, p) end

--- bomb cannon combo command
--- @param id entity
--- @param p Position
function CppLogic.Entity.Settler.CommandBombCannonCombo(id, p) end

--- teleports a settler to a position. does not change id.
--- @param id entity
--- @param p Position pos
function CppLogic.Entity.Settler.SetPosition(id, p) end

--- enables ranged effect advanced healing. heal gets transfered from solder to leader, first healing the leader, and if something is left, the troop hp pool.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param enabl boolean
function CppLogic.Entity.Settler.EnableRangedEffectSoldierHeal(enabl) end

--- gets the target of a heros shuriken ability.
--- @param id entity
--- @return number tid
function CppLogic.Entity.Settler.ShurikenGetTarget(id) end

--- gets the target of a heros sniper ability.
--- @param id entity
--- @return number tid
function CppLogic.Entity.Settler.SniperGetTarget(id) end

--- gets the building a settler is currently inside, and leaving.
--- @param id entity
--- @return number buildingIn (may be 0)
--- @return number buildingLeaving (may be 0)
function CppLogic.Entity.Settler.GetEnteredBuilding(id) end

--- gets the resources a worker carries.
--- @param id entity
--- @return number amount
--- @return boolean couldConsume
function CppLogic.Entity.Settler.WorkerGetResourceCarried(id) end

--- gets all summoned attached to an entity.
--- @param id entity
--- @return number[] entities
function CppLogic.Entity.Settler.SettlerGetSummoned(id) end

--- sets a special task list (TL_SCRIPT_ANIMATION) to play an animation on this entity.
--- after the animation is done, executes TASK_BATTLE_WAIT_UNTIL to reset animation. (this may block returning to your normal tl for a few seconds).
--- the tasklist TL_SCRIPT_ANIMATION gets created at first use, it may not be there before you call this func.
--- does not work with SCELoader.
--- @param id entity
--- @param animname string name of the animation (can be copied from entity xml) does not check if the anim matches the model!
--- @param backwards boolean play animation backwards, (optional, default false)
--- @param speed number speed factor (optional, default 1)
function CppLogic.Entity.Settler.PlayScriptAnimation(id, animname, backwards, speed) end

--- a leaders experience.
--- @param id entity
--- @return number xp
function CppLogic.Entity.Leader.GetExperience(id) end
--- a leaders experience
--- @param id entity
--- @param xp number
function CppLogic.Entity.Leader.SetExperience(id, xp) end

--- attaches a soldier to a leader.
--- @param leader entity
--- @param soldier entity
function CppLogic.Entity.Leader.AttachSoldier(leader, soldier) end

--- a leaders troop health (hp of all soldiers summed up).
--- -1 when not yet set (calculate in lua as all soldiers at full hp).
--- @param id entity
--- @return number hp
function CppLogic.Entity.Leader.GetTroopHealth(id) end
--- a leaders troop health (does not kill soldiers when decreasing).
--- @param id entity
--- @param hp number
function CppLogic.Entity.Leader.SetTroopHealth(id, hp) end

--- a leaders troop limit.
--- @param id entity
--- @param limit number
function CppLogic.Entity.Leader.SetSoldierLimit(id, limit) end

--- a leaders regeneration.
--- does not work with SCELoader.
--- @param id entity
--- @return number hp hp regenerated
--- @return number sec seconds between refreshes
function CppLogic.Entity.Leader.GetRegeneration(id) end
--- a leaders regeneration.
--- status of this gets saved into a savegame.
--- does not work with SCELoader.
--- @param id entity
--- @param hp number regenerated
--- @param seconds number between refreshes
function CppLogic.Entity.Leader.SetRegeneration(id, hp, seconds) end

--- building height (& construction progress).
--- @param id entity
--- @return number height
function CppLogic.Entity.Building.GetHeight(id) end
--- building height (& construction progress).
--- @param id entity
--- @param h number height
function CppLogic.Entity.Building.SetHeight(id, h) end

--- barracks autofill status.
--- @param id entity
--- @return boolean active
function CppLogic.Entity.Building.GetBarracksAutoFillActive(id) end

---@alias building entity

--- nearest construction slot.
--- @param build building
--- @param p Position to measure from
--- @return number slotIndex -1 if nothing free
function CppLogic.Entity.Building.GetNearestFreeConstructionSlotFor(build, p) end

--- nearest repair slot.
--- @param build building
--- @param p Position to measure from
--- @return number slotIndex -1 if nothing free
function CppLogic.Entity.Building.GetNearestFreeRepairSlotFor(build, p) end

--- market trade data.
--- progress is buy amount + sell amount
--- @param id entity
--- @return number buy type
--- @return number sell type
--- @return number buy amount
--- @return number sell amount
--- @return number progress amount
function CppLogic.Entity.Building.MarketGetCurrentTradeData(id) end
--- market trade data.
--- progress is buy amount + sell amount
--- @param id entity
--- @param bty number buy type (optional)
--- @param sty number sell type (optional)
--- @param bam number buy amount (optional)
--- @param sam number sell amount (optional)
--- @param pam number progress amount (optional)
function CppLogic.Entity.Building.MarketSetCurrentTradeData(id, bty, sty, bam, sam, pam) end

--- starts building a cannon.
--- uses resources, asserts if it cannot build.
--- @param id entity
--- @param ty number cannon type
function CppLogic.Entity.Building.CommandFoundryBuildCannon(id, ty) end

--- starts upgrading a building. does not cost resources.
--- @param id entity
function CppLogic.Entity.Building.StartUpgrade(id) end
--- cancels an upgrade currently in progress. does not refund resources.
--- @param id entity
function CppLogic.Entity.Building.CancelUpgrade(id) end

--- checks if a building is idle.
--- this is not under constrcution, upgrading, researching, in alarm, recruiting or trading.
--- @param id entity
--- @return boolean
function CppLogic.Entity.Building.IsIdle(id) end

--- gets leaders currently getting trainet at a barracks.
--- @param id entity
--- @return number|nil id1
--- @return number|nil id2
--- @return number|nil id3
function CppLogic.Entity.Building.BarracksGetLeadersTrainingAt(id) end

--- gets the entitytype of the cannon in construction.
--- 0 if no entitytype, gets reset when cannon spawns.
--- @param id entity
--- @return number entitytype
function CppLogic.Entity.Building.FoundryGetCannonTypeInConstruction(id) end

--- buys a soldier for a leader at a specific barracks.
--- uses resources.
--- @param bid entity
--- @param lid entity
--- @param noChecktype boolean (optional, default false) set to true to skip the check for matching barracks type.
--- @param checkvcandcost boolean (optional, default true) set to false to skip the check for pop and cost.
function CppLogic.Entity.Building.BarracksBuySoldierForLeader(bid, lid, noChecktype, checkvcandcost) end

--- activates overtime.
--- checks for cooldown.
--- @param id entity
function CppLogic.Entity.Building.ActivateOvertime(id) end
--- deactivates overtime and sets overtime cooldown.
--- @param id entity
function CppLogic.Entity.Building.DeactivateOvertime(id) end

--- sets a barrack to recruit full groups.
--- @param id entity
function CppLogic.Entity.Building.BarracksRecruitGroups(id) end
--- sets a barrack to recruit only leaders.
--- @param id entity
function CppLogic.Entity.Building.BarracksRecruitLeaders(id) end

--- buys a serf at a hq.
--- uses resources.
--- @param id entity
function CppLogic.Entity.Building.HQBuySerf(id) end

--- sells a building.
--- does not give you back resources.
--- @param id entity
function CppLogic.Entity.Building.SellBuilding(id) end

--- starts researching a tech.
--- does not check, if the tech belongs to that building.
--- uses resources.
--- @param id entity
--- @param tech number
function CppLogic.Entity.Building.StartResearch(id, tech) end
--- cancels research.
--- gives back resources.
--- @param id entity
function CppLogic.Entity.Building.CancelResearch(id) end

--- starts a market transaction.
--- uses resources.
--- @param id entity
--- @param sellty number
--- @param buyty number
--- @param buyam number
function CppLogic.Entity.Building.MarketStartTrade(id, sellty, buyty, buyam) end
--- cancels a market transaction.
--- gives back resources.
--- @param id entity
function CppLogic.Entity.Building.MarketCancelTrade(id) end

--- removes the last offer in a mercenary or techtrader tent.
--- @param id entity
function CppLogic.Entity.Building.MercenaryRemoveLastOffer(id) end
--- sets the remaining units and cost in a mercenary or techtrader tent.
--- @param id entity
--- @param ind number index
--- @param remain number|nil offers remaining (optional, default no change)
--- @param cost CostInfo|nil table (optional, default no change)
--- @param ignoreZeroes boolean ignore anything that is zero (optional, default false)
function CppLogic.Entity.Building.MercenarySetOfferData(id, ind, remain, cost, ignoreZeroes) end

--- gets the BuildOn entity this building is constructed on (mine/VC/bridge).
--- @param id entity
--- @return number id id or 0
function CppLogic.Entity.Building.GetBuildOnEntity(id) end
--- gets the building constructed on a BuildOn entity (mine/VC/bridge).
--- @param id entity
--- @return number id id or 0
function CppLogic.Entity.Building.BuildOnEntityGetBuilding(id) end

--- gets the constructonsite of a building.
--- @param id entity
--- @return number id id or 0
function CppLogic.Entity.Building.GetConstructionSite(id) end
--- gets building of a constructionsite.
--- @param id entity
--- @return number id id or 0
function CppLogic.Entity.Building.ConstructionSiteGetBuilding(id) end

--- checks if a building is a GGL::CConstructionSite.
--- (not to confuse with a GGL::CBuilding under construction, use Logic.IsConstructionComplete to check GGL::CBuilding construction state).
--- @param id entity
--- @return boolean
function CppLogic.Entity.Building.IsConstructionSite(id) end

--- buys a leader in a barracks by entitytype.
--- uses resources, asserts if not possible.
--- does not fire a Events.CPPLOGIC_EVENT_CAN_BUY_SETTLER trigger.
--- @param id entity
--- @param ety entity type
--- @param checkType boolean|nil check if leader type can be recruited at this barracks upgradecategory (optional, default false)
--- @param checkVCAndMoti boolean|nil checks VC space, motivation and alarm (optional, default true)
--- @return number id
function CppLogic.Entity.Building.BarracksBuyLeaderByType(id, ety, checkType, checkVCAndMoti) end

--- gets ApproachPosition, LeavePosition and DoorPos of a building.
--- @param id entity
--- @return Position approach
--- @return Position leave
--- @return Position door
function CppLogic.Entity.Building.GetRelativePositions(id) end

--- spawns a worker in a VC or HQ for a specific building.
--- does not check if the target building can have another worker, or if it could spawn normally.
--- also does not check, if spawner is a VC/HQ and of the same player.
--- does not fire a Events.CPPLOGIC_EVENT_CAN_BUY_SETTLER trigger.
--- @param workplace entity
--- @param spawner entity
--- @return number worker
function CppLogic.Entity.Building.SpawnWorkerFor(workplace, spawner) end

--- entity type max health.
--- @param ty number entitytype
--- @return number max health
function CppLogic.EntityType.GetMaxHealth(ty) end
--- entity type max health.
--- @param ty number entitytype
--- @param h number max health
function CppLogic.EntityType.SetMaxHealth(ty, h) end

--- entity type motivation effect.
--- @param ty number entitytype
--- @return number motivation effect
function CppLogic.EntityType.GetAffectMotivation(ty) end
--- entity type motivation effect.
--- @param ty number entitytype
--- @param m number motivation effect
function CppLogic.EntityType.SetAffectMotivation(ty, m) end

--- entity type suspended animation.
--- @param ty number entitytype
--- @return number anim
function CppLogic.EntityType.GetSuspendedAnimation(ty) end
--- entity type suspended animation.
--- @param ty number entitytype
--- @param a number anim
function CppLogic.EntityType.SetSuspendedAnimation(ty, a) end

--- limited lifespan duration seconds.
--- @param ty number entitytype
--- @return number duration
function CppLogic.EntityType.GetLimitedLifespanDuration(ty) end
--- limited lifespan duration seconds.
--- @param ty number entitytype
--- @param t number duration
function CppLogic.EntityType.SetLimitedLifespanDuration(ty, t) end

--- damage and damageclass of autoattacks of an entity type.
--- @param ty number entitytype
--- @return number damageAmount
--- @return number damageclass
function CppLogic.EntityType.GetAutoAttackDamage(ty) end
--- damage and damageclass of autoattacks of an entity type.
--- @param ty number entitytype
--- @param dmg number damage amount
--- @param dc number damageclass
function CppLogic.EntityType.SetAutoAttackDamage(ty, dmg, dc) end

--- adds an entity category to an entitytype.
--- can be tested for with logic/predicate, does not need to be from predefined EntityCategories.
--- @param ty number entitytype
--- @param ecat number entitycategory
function CppLogic.EntityType.AddEntityCategory(ty, ecat) end
--- removes an entity category from an entitytype.
--- can be tested for with logic/predicate, does not need to be from predefined EntityCategories.
--- @param ty number entitytype
--- @param ecat number entitycategory
function CppLogic.EntityType.RemoveEntityCategory(ty, ecat) end

--- miss chance of autoattacks of an entity type (no autocannon).
--- @param ty number entitytype
--- @return number c miss chance
function CppLogic.EntityType.GetAutoAttackMissChance(ty) end
--- miss chance of autoattacks of an entity type (no autocannon).
--- @param ty number entitytype
--- @param mc number miss chance
function CppLogic.EntityType.SetAutoAttackMissChance(ty, mc) end

--- range chance of autoattacks of an entity type.
--- for autocannons only max range, for anything else max and min range.
--- @param ty number entitytype
--- @return number max range
--- @return number min range (nil for autocannons)
function CppLogic.EntityType.GetAutoAttackRange(ty) end
--- range chance of autoattacks of an entity type.
--- for autocannons only max range, for anything else max and min range.
--- @param ty number entitytype
--- @param maxrange number|nil range (optional)
--- @param minrange number|nil range (ignored for autocannons) (optional)
function CppLogic.EntityType.SetAutoAttackRange(ty, maxrange, minrange) end

--- settler or building type armor and armorclass.
--- @param ty number entitytype
--- @return number armor
--- @return number armorclass
function CppLogic.EntityType.GetArmor(ty) end
--- settler or building type armor and armorclass.
--- @param ty number entitytype
--- @param ar number|nil armor
--- @param acl number|nil armorclass
function CppLogic.EntityType.SetArmor(ty, ar, acl) end

--- settler or building type armor modifier techs.
--- @param ty number entitytype
--- @return number[]
function CppLogic.EntityType.GetArmorModifierTechs(ty) end

--- entity type models.
--- most entitytypes use only one model, but some use the others as well.
--- @param ty number entitytype
--- @return number model 0
--- @return number model 1
--- @return number model 2
--- @return number model 3
function CppLogic.EntityType.GetModels(ty) end
--- entity type models.
--- most entitytypes use only one model, but some use the others as well.
--- @param ty number entitytype
--- @param m0 number|nil model 0 (optional)
--- @param m1 number|nil model 1 (optional)
--- @param m2 number|nil model 2 (optional)
--- @param m3 number|nil model 3 (optional)
function CppLogic.EntityType.SetModels(ty, m0, m1, m2, m3) end

--- checks it a entitytype can be replaced with a resourcetree.
--- @param ty number entitytype
--- @return boolean
function CppLogic.EntityType.ResourceTreeTypeHasData(ty) end
--- tree data to replace a tree with a resourcetree.
--- @param ty number entitytype
--- @return number resource entitytype
--- @return number resource amount
function CppLogic.EntityType.ResourceTreeTypeGetData(ty) end
--- tree data to replace a tree with a resourcetree.
--- @param ty number entitytype
--- @param rety number resource entitytype
--- @param ram number resource amount
function CppLogic.EntityType.ResourceTreeTypeSetData(ty, rety, ram) end

--- blocking data of an entitytype.
--- @param ty number entitytype
--- @return Position[][] blocking table (array of {pos,pos})
--- @return number num blocked points (number)
--- @return Position[]|nil buildblock ({pos,pos} or nil)
function CppLogic.EntityType.GetBlocking(ty) end

--- settler or building type exploration modifier techs.
--- @param ty number entitytype
--- @return number[]
function CppLogic.EntityType.GetExplorationModifierTechs(ty) end

--- DeleteWhenBuiltOn flag of an entitytype.
--- @param ty number entitytype
--- @return boolean
function CppLogic.EntityType.GetDeleteWhenBuildOn(ty) end
--- DeleteWhenBuiltOn flag of an entitytype.
--- @param ty number entitytype
--- @param del boolean flag
function CppLogic.EntityType.SetDeleteWhenBuildOn(ty, del) end

--- battlewaituntil (battle behavior) or reloadtime (autocannon behavior) of an entitytype.
--- @param ty number entitytype
--- @return number bwu
function CppLogic.EntityType.GetBattleWaitUntil(ty) end
--- battlewaituntil (battle behavior) or reloadtime (autocannon behavior) of an entitytype.
--- @param ty number entitytype
--- @param bwu number battlewaituntil
function CppLogic.EntityType.SetBattleWaitUntil(ty, bwu) end


--- convertible flag of settlers and buildings (returns nil on other types).
--- @param ty number entitytype
--- @return boolean|nil
function CppLogic.EntityType.IsConvertible(ty) end

--- checks for a building type.
--- @param ty number entitytype
--- @return boolean
function CppLogic.EntityType.IsBuildingType(ty) end
--- checks for a settler type.
--- @param ty number entitytype
--- @return boolean
function CppLogic.EntityType.IsSettlerType(ty) end
--- checks for a leader type.
--- @param ty number entitytype
--- @return boolean
function CppLogic.EntityType.IsLeaderType(ty) end
--- checks for a soldier type.
--- @param ty number entitytype
--- @return boolean
function CppLogic.EntityType.IsSoldierType(ty) end

--- the soldier type of a leader type
--- @param ty number leader type
--- @return number soldier type
function CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(ty) end
--- the soldier type of a leader type
--- @param leaderTy number leader type
--- @param soldierTy number soldier type
function CppLogic.EntityType.Settler.LeaderTypeSetSoldierType(leaderTy, soldierTy) end

--- leader type upkeep per payday.
--- @param ty number entitytype
--- @return number upkeep
function CppLogic.EntityType.Settler.LeaderTypeGetUpkeep(ty) end
--- leader type upkeep per payday.
--- @param ty number entitytype
--- @param u number upkeep
function CppLogic.EntityType.Settler.LeaderTypeSetUpkeep(ty, u) end

--- leader type regeneration.
--- @param ty number entitytype
--- @return number h regen health
--- @return number s regen seconds
function CppLogic.EntityType.Settler.LeaderTypeGetRegeneration(ty) end
--- leader type regeneration.
--- @param ty number entitytype
--- @param hp number regen health (optional)
--- @param t number regen seconds (optional)
function CppLogic.EntityType.Settler.LeaderTypeSetRegeneration(ty, hp, t) end

--- range in which enemies get attacked by this leader type.
--- @param ty number entitytype
--- @return number range
function CppLogic.EntityType.Settler.LeaderTypeGetAutoAggressiveRange(ty) end
--- range in which enemies get attacked by this leader type.
--- @param ty number entitytype
--- @param r number range
function CppLogic.EntityType.Settler.LeaderTypeSetAutoAggressiveRange(ty, r) end

--- stealing time and resource amounts of a thief type.
--- @param ty number entitytype
--- @return number secondsNeeded
--- @return number maxResource
--- @return number minResource
function CppLogic.EntityType.Settler.ThiefTypeGetStealingInfo(ty) end
--- stealing time and resource amounts of a thief type.
--- @param ty number entitytype
--- @param sec number seconds needed
--- @param maxS number max resource
--- @param minS number min resource
function CppLogic.EntityType.Settler.ThiefTypeSetStealingInfo(ty, sec, maxS, minS) end

--- entitytytpe camouflage ability data.
--- for ari duration is invisibility after activation, for thieves its time to invisibility after discovery.
--- @param ty number entitytype
--- @return number duration
--- @return number discoveryRange
--- @return number rechargeTime
function CppLogic.EntityType.Settler.GetAbilityDataCamouflage(ty) end
--- entitytytpe camouflage ability data.
--- for ari duration is invisibility after activation, for thieves its time to invisibility after discovery.
--- @param ty number entitytype
--- @param dur number duration
--- @param dran number discovery range
--- @param rech number recharge time
function CppLogic.EntityType.Settler.SetAbilityDataCamouflage(ty, dur, dran, rech) end

--- entitytytpe circular attack ability data.
--- @param ty number entitytype
--- @return number damage
--- @return number damageClass
--- @return number range
--- @return number rechargeTime
function CppLogic.EntityType.Settler.GetAbilityDataCircularAttack(ty) end
--- entitytytpe circular attack ability data.
--- @param ty number entitytype
--- @param dmg number damage
--- @param dcl number damage class
--- @param ra number range
--- @param rech number recharge time
function CppLogic.EntityType.Settler.SetAbilityDataCircularAttack(ty, dmg, dcl, ra, rech) end

--- entitytytpe shuriken ability data.
--- @param ty number entitytype
--- @return number damage
--- @return number damageClass
--- @return number numberShuriken
--- @return number range
--- @return number maxArcDegree
--- @return number rechargeTime
--- @return number effectTypeID
function CppLogic.EntityType.Settler.GetAbilityDataShuriken(ty) end
--- entitytytpe shuriken ability data.
--- @param ty number entitytype
--- @param dmg number damage
--- @param dcl number damage class
--- @param numSh number number shuriken
--- @param ran number range
--- @param mArc number max arc degree
--- @param rech number recharge time
function CppLogic.EntityType.Settler.SetAbilityDataShuriken(ty, dmg, dcl, numSh, ran, mArc, rech) end

--- entitytytpe sniper ability data.
--- damage is facto * max hp of target.
--- @param ty number entitytype
--- @return number damageFactor
--- @return number range
--- @return number rechargeTime
function CppLogic.EntityType.Settler.GetAbilityDataSniper(ty) end
--- entitytytpe sniper ability data.
--- damage is facto * max hp of target.
--- @param ty number entitytype
--- @param dfac number damage factor
--- @param ran number range
--- @param rech number recharge time
function CppLogic.EntityType.Settler.SetAbilityDataSniper(ty, dfac, ran, rech) end

--- entitytytpe sniper ability data.
--- damage is facto * max hp of target.
--- @param ty number entitytype
--- @return number damageFactor
--- @return number armorFactor
--- @return number healthRecoverFactor
--- @return number range
--- @return number rechargeTime
function CppLogic.EntityType.Settler.GetAbilityDataRangedEffect(ty) end
--- entitytytpe sniper ability data.
--- damage is facto * max hp of target.
--- @param ty number entitytype
--- @param dmgfac number damage factor
--- @param armfac number armor factor
--- @param hpfac number health recover factor
--- @param ran number range
--- @param rech number recharge time
function CppLogic.EntityType.Settler.SetAbilityDataRangedEffect(ty, dmgfac, armfac, hpfac, ran, rech) end

--- entitytytpe resurrect ability data.
--- @param ty number entitytype
--- @return number range
--- @return number progressPerTick
--- @return number rechargeTimeSeconds
function CppLogic.EntityType.Settler.GetAbilityDataResurrect(ty) end

--- entitytytpe lightning strike data.
--- @param ty number entitytype
--- @return number range
--- @return number weatherEnergyCost
--- @return number damage
--- @return number damageClass
--- @return number damageRange
--- @return number effectType
--- @return number rechargeTimeSeconds
function CppLogic.EntityType.Settler.GetAbilityLightningStrikeData(ty) end

--- entitytytpe bombardment data.
--- @param ty number entitytype
--- @return number range
--- @return number effectType
--- @return number damageRange
--- @return number damage
--- @return number damageClass
--- @return number taskList
--- @return number rangeOverride
--- @return number rechargeTimeSeconds
function CppLogic.EntityType.Settler.GetAbilityBombardmentData(ty) end

--- settler type fearless.
--- @param ty number entitytype
--- @return boolean fearless
function CppLogic.EntityType.Settler.GetFearless(ty) end
--- settler type fearless.
--- @param ty number entitytype
--- @param fl boolean fearless
function CppLogic.EntityType.Settler.SetFearless(ty, fl) end

--- settler type cost.
--- @param ty number entitytype
--- @return CostInfo info
function CppLogic.EntityType.Settler.GetCost(ty) end
--- settler type cost.
--- @param ty number entitytype
--- @param c CostInfo cost info
--- @param ignoreZeroes boolean should zeroes get ignored (optional)
function CppLogic.EntityType.Settler.SetCost(ty, c, ignoreZeroes) end

--- settler or building type damage modifier techs.
--- @param ty number entitytype
--- @return number[]
function CppLogic.EntityType.Settler.GetDamageModifierTechs(ty) end

--- settler or building type max range modifier techs.
--- @param ty number entitytype
--- @return number[]
function CppLogic.EntityType.Settler.GetMaxRangeModifierTechs(ty) end

--- settler or building type speed modifier techs.
--- @param ty number entitytype
--- @return number[]
function CppLogic.EntityType.Settler.GetSpeedModifierTechs(ty) end

--- leader type initial max number of soldiers.
--- @param ty number entitytype
--- @return number numSol
function CppLogic.EntityType.Settler.LeaderTypeGetMaxNumberOfSoldiers(ty) end

--- gets the first upgradecategory a settler type is in.
--- @param ty number entitytype
--- @return number ucat
function CppLogic.EntityType.Settler.GetUpgradeCategory(ty) end

--- dumps the entity types GGL::CBattleBehaviorProps to lua
--- likely slow, only intended for development putposes
---@param et number|string
---@return table
function CppLogic.EntityType.Settler.DumpBattleBehavior(et) end

--- number of settlers this building (villagecenter) supports.
--- @param ty number entitytype
--- @return number slots
function CppLogic.EntityType.Building.GetVCAttractionSlotsProvided(ty) end
--- number of settlers this building (villagecenter) supports.
--- @param ty number entitytype
--- @param s number slots
function CppLogic.EntityType.Building.SetVCAttractionSlotsProvided(ty, s) end

--- building type construction cost.
--- @param ty number entitytype
--- @return CostInfo
function CppLogic.EntityType.Building.GetConstructionCost(ty) end
--- building type construction cost.
--- @param ty number entitytype
--- @param c CostInfo
--- @param ignoreZeroes boolean should zeroes get ignored (optional)
function CppLogic.EntityType.Building.SetConstructionCost(ty, c, ignoreZeroes) end

--- building type upgrade cost.
--- @param ty number entitytype
--- @return CostInfo
function CppLogic.EntityType.Building.GetUpradeCost(ty) end
--- building type upgrade cost.
--- @param ty number entitytype
--- @param c CostInfo
--- @param ignoreZeroes boolean should zeroes get ignored (optional)
function CppLogic.EntityType.Building.SetUpradeCost(ty, c, ignoreZeroes) end

--- adds a tech modifier for building hp.
--- does not get saved into savegame.
--- does not work with SCELoader.
--- has no effect without EnableMaxHPTechMod.
--- @param ty number entitytype
--- @param tech number
function CppLogic.EntityType.Building.AddHPTechMod(ty, tech) end

--- gets the BuildOnTypes of a building type.
--- @param ty number entitytype
--- @return number[] (may be empty table)
function CppLogic.EntityType.Building.GetBuildOnTypes(ty) end

--- tech research time and costs.
--- @param tid number tech id
--- @return number t research time needed
--- @return CostInfo
function CppLogic.Technology.GetResearchInfo(tid) end
--- tech research time and costs.
--- @param tid number tech id
--- @param time number research time needed (optional)
--- @param cost CostInfo cost info (optional)
function CppLogic.Technology.SetResearchInfo(tid, time, cost) end

--- tech requirements.
--- @param tid number tech id
--- @return number num required entity cond (0 -> all)
--- @return table<number,number> entity conditions (table [entitytypeId] = amount)
--- @return number num required tech cond (0 -> all)
--- @return number[] tech conditions (array of tech ids)
--- @return number num required ucat cond (0 -> all)
--- @return table<number,number> ucat conditions (table [ucatId] = amount)
function CppLogic.Technology.GetRequirements(tid) end

--- technology exploration modifier.
--- @param tid number tech id
--- @return number operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
--- @return number value
function CppLogic.Technology.GetExplorationModifier(tid) end
--- technology armor modifier.
--- @param tid number tech id
--- @return number operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
--- @return number value
function CppLogic.Technology.GetArmorModifier(tid) end
--- technology damage modifier.
--- @param tid number tech id
--- @return number operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
--- @return number value
function CppLogic.Technology.GetDamageModifier(tid) end
--- technology range modifier.
--- @param tid number tech id
--- @return number operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
--- @return number value
function CppLogic.Technology.GetRangeModifier(tid) end
--- technology speed modifier.
--- @param tid number tech id
--- @return number operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
--- @return number value
function CppLogic.Technology.GetSpeedModifier(tid) end

--- adds a construction time modifier to a technology.
--- default modifier is 1.0, smaller modifiers lead to faster construction.
--- @param tech number tech
--- @param value number modification value
--- @param op string operation to perform, accepted is "+" "-" "*" "/"
function CppLogic.Technology.TechAddConstructionSpeedModifier(tech, value, op) end

--- instantly researchs a technology without showing a feedback flag.
---@param player number
---@param tech number
function CppLogic.Technology.ResearchTechnologyNoFeedback(player, tech) end

--- gets the researched techs of a player (from Statistics). note: because of a BB bug, every tech that gets researched via AutomaticResearch does not show up here.
---@param player number
---@return {Technology : number, Time : number}[]
function CppLogic.Technology.GetResearchedTechs(player) end

---@alias widget string|number

--- gets a widgets position and size.
--- @param wid widget
--- @return number x
--- @return number y
--- @return number w
--- @return number h
function CppLogic.UI.WidgetGetPositionAndSize(wid) end
--- sets a widgets position and size as floats (so you can set fractional coordinates).
--- @param wid widget
--- @param x number|nil (optional, default current)
--- @param y number|nil (optional, default current)
--- @param w number|nil (optional, default current)
--- @param h number|nil (optional, default current)
function CppLogic.UI.WidgetSetPositionAndSize(wid, x, y, w, h) end

--- gets a widgets updatemanual flag.
--- @param wid widget
--- @return boolean
function CppLogic.UI.WidgetGetUpdateManualFlag(wid) end
--- sets a widgets updatemanual flag.
--- @param wid widget
--- @param f boolean
function CppLogic.UI.WidgetSetUpdateManualFlag(wid, f) end

--- gets a widgets update func.
--- @param wid widget
--- @return string command
--- @return fun()|string f func or err msg
function CppLogic.UI.WidgetGetUpdateFunc(wid) end
--- calls a buttons update func.
--- @param wid widget
function CppLogic.UI.WidgetCallUpdateFunc(wid) end
--- overrides a buttons update func to a lua func that gets called without any arguments.
--- @param wid widget
--- @param f fun()
function CppLogic.UI.WidgetOverrideUpdateFunc(wid, f) end

--- gets a buttons action func.
--- @param wid widget
--- @return string command
--- @return fun()|string f func or err msg
function CppLogic.UI.ButtonGetActionFunc(wid) end
--- calls a buttons action func.
--- @param wid widget
function CppLogic.UI.ButtonCallActionFunc(wid) end
--- overrides a buttons action func to a lua func that gets called without any arguments.
--- @param wid widget
--- @param f fun()
function CppLogic.UI.ButtonOverrideActionFunc(wid, f) end

--- gets a widgets tooltip func.
--- @param wid widget
--- @return string command
--- @return fun()|string f func or err msg
function CppLogic.UI.WidgetGetTooltipFunc(wid) end
--- calls a widgets tooltip action func.
--- @param wid widget
function CppLogic.UI.WidgetCallTooltipFunc(wid) end
--- overrides a widgets tooltip action func to a lua func that gets called without any arguments.
--- @param wid widget
--- @param f fun()
function CppLogic.UI.WidgetOverrideTooltipFunc(wid, f) end

--- gets a widgets tooltip data.
--- target widget can be the text widget or the container.
--- @param wid widget
--- @return number tooltipTargetId
--- @return boolean controlTargetWidgetState
--- @return boolean tooltipEnabledFlag
function CppLogic.UI.WidgetGetTooltipData(wid) end
--- sets a widgets tooltip data.
--- target widget can be the text widget or the container.
--- @param wid widget
--- @param tid widget|nil target widget id (nil to disable)
--- @param controlFlag boolean control target widget state flag
--- @param enabledFlag boolean tooltip enabled flag
function CppLogic.UI.WidgetSetTooltipData(wid, tid, controlFlag, enabledFlag) end

--- gets a widgets tooltip string.
--- this string gets shown automatically, if the target widget is a text widget.
--- @param wid widget
--- @return string rawString
--- @return string sttKey
function CppLogic.UI.WidgetGetTooltipString(wid) end
--- sets a widgets tooltip string.
--- this string gets shown automatically, if the target widget is a text widget.
--- @param wid widget
--- @param str string
--- @param isSTTKey boolean|nil (optional, default false) if true, sets this as string table text key
function CppLogic.UI.WidgetSetTooltipString(wid, str, isSTTKey) end

--- returns a table with all child widget ids.
--- @param wid widget
--- @return number[]
function CppLogic.UI.ContainerWidgetGetAllChildren(wid) end

--- returns if the tooltip of this widget is shown.
--- @param wid widget
--- @return boolean
function CppLogic.UI.WidgetIsTooltipShown(wid) end

--- returns the font config.
--- @param fontName string
--- @return number size
--- @return number offset
--- @return number spacing
function CppLogic.UI.FontGetConfig(fontName) end
--- sets the font config.
--- @param fontName string
--- @param size number
--- @param offset number
--- @param spacing number
function CppLogic.UI.FontSetConfig(fontName, size, offset, spacing) end

--- sets the font of a widget.
--- @param wid widget
--- @param fontName string
function CppLogic.UI.WidgetSetFont(wid, fontName) end

--- creates a texture id and loads the texture.
--- useful if a texture is packed into an s5x.
--- without modloader cannot get cleaned up.
--- does nothing if the texture already exists.
--- @param texture string
function CppLogic.UI.PreLoadGUITexture(texture) end

--- basic widget data.
--- @param wid widget
--- @return number zpriority
--- @return boolean forceToHandleMouseEvent
--- @return boolean forceToNeverBeFound
function CppLogic.UI.WidgetGetBaseData(wid) end
--- basic widget data.
--- @param wid widget
--- @param zpriority number|nil (optional, default current value)
--- @param forceToHandleMouseEvent boolean|nil (optional, default current value)
--- @param forceToNeverBeFound boolean|nil (optional, default current value)
function CppLogic.UI.WidgetSetBaseData(wid, zpriority, forceToHandleMouseEvent, forceToNeverBeFound) end

--- string frame distance of a text display. (distance from top of widget to string rendering).
--- @param wid widget
--- @return number frameDistance
function CppLogic.UI.WidgetGetStringFrameDistance(wid) end
--- string frame distance of a text display. (distance from top of widget to string rendering).
--- @param wid widget
--- @param fdis number frame distance
function CppLogic.UI.WidgetSetStringFrameDistance(wid, fdis) end

--- line distance factor (space between lines).
--- @param wid widget
--- @return number distanceFactor
function CppLogic.UI.StaticTextWidgetGetLineDistanceFactor(wid) end
--- line distance factor (space between lines).
--- @param wid widget
--- @param df number distance factor
function CppLogic.UI.StaticTextWidgetSetLineDistanceFactor(wid, df) end

--- gets a widget material textures position and size.  
--- mat:
--- - EGUIX::CButtonWidget (EGUIX::CGfxButtonWidget & EGUIX::CTextButtonWidget): 0->Normal, 1->MouseOver, 2->Clicked, 3->Disabled, 4->Highlighed
--- - EGUIX::CGfxButtonWidget: 10->IconMaterial (always rendered on top)
--- - EGUIX::CStaticWidget (EGUIX::CStaticTextWidget & EGUIX::CProgressBarWidget): ignored (has only one material, set to 0)
--- @param wid widget
--- @param mat number
--- @return number x
--- @return number y
--- @return number w
--- @return number h
function CppLogic.UI.WidgetMaterialGetTextureCoordinates(wid, mat) end
--- sets a widget material textures position and size.  
--- mat:
--- - EGUIX::CButtonWidget (EGUIX::CGfxButtonWidget & EGUIX::CTextButtonWidget): 0->Normal, 1->MouseOver, 2->Clicked, 3->Disabled, 4->Highlighed
--- - EGUIX::CGfxButtonWidget: 10->IconMaterial (always rendered on top)
--- - EGUIX::CStaticWidget (EGUIX::CStaticTextWidget & EGUIX::CProgressBarWidget): ignored (has only one material, set to 0)
--- @param wid widget
--- @param mat number
--- @param x number|nil (optional, default current)
--- @param y number|nil (optional, default current)
--- @param w number|nil (optional, default current)
--- @param h number|nil (optional, default current)
function CppLogic.UI.WidgetMaterialSetTextureCoordinates(wid, mat, x, y, w, h) end

--- a buttons shortcut string.
--- this is not directly a string, its the key code encoded in one char.
--- you may use KeyOf(string.byte(txt, 1), Keys) to decode the return values.
--- @param wid widget
--- @return string rawString
--- @return string sttKey
function CppLogic.UI.ButtonGetShortcutString(wid) end
--- a buttons shortcut string.
--- this is not directly a string, its the key code encoded in one char.
--- you may use string.char(Keys[c.Key]) to get the correct code.
--- @param wid widget
--- @param str string
--- @param isSTTKey boolean|nil (optional, default false) if true, sets this as string table text key
function CppLogic.UI.ButtonSetShortcutString(wid, str, isSTTKey) end

--- sets, if text in a text button is centered vertically. true by default, code for this added by CppLogic.
---@param wid widget
---@param centered boolean
---@diagnostic disable-next-line: duplicate-set-field
function CppLogic.UI.TextButtonSetCenterText(wid, centered) end

--- sets a widgets group.
--- @param wid widget
--- @param group string group name
function CppLogic.UI.WidgetSetGroup(wid, group) end

--- removes a widget.
--- @param wid widget
function CppLogic.UI.RemoveWidget(wid) end

--- moves a widget. after the move, tomove is placed directly before target (at the end, if target is nil).
--- does nothing, if tomove == target, or tomove is directly in front of target.
--- @param tomove widget
--- @param target widget?
function CppLogic.UI.ReorderWidgets(tomove, target) end

--- gets a widgets name.
--- @param wid widget
--- @return string name
function CppLogic.UI.GetWidgetName(wid) end

--- gets the widget at the current position.
--- if ForceToNeverBeFoundFlag is set, that widget gets ignored by this function.
--- can not return container widgets.
--- @param baseWidget widget starts searching at this widget, "root" should make the most sense, x,y is relative to this widget
--- @param x number
--- @param y number
--- @return number widget
function CppLogic.UI.GetWidgetAtPosition(baseWidget, x, y) end

--- EGUIX::CStringInputCustomWidget ignore next char flag.
--- @param wid widget
--- @return boolean
function CppLogic.UI.StringInputWidgetGetIgnoreNextChar(wid) end
--- EGUIX::CStringInputCustomWidget ignore next char flag.
--- @param wid widget
--- @param flag boolean
function CppLogic.UI.StringInputWidgetSetIgnoreNextChar(wid, flag) end

--- EGUIX::CStringInputCustomWidget buffer size.
--- clears current input.
--- @param wid widget
--- @param s number
function CppLogic.UI.StringInputWidgetSetBufferSize(wid, s) end


--- creates a new static widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- @param wid widget
--- @param name string widgetname of the child
--- @param before widget|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @return number id
function CppLogic.UI.ContainerWidgetCreateStaticWidgetChild(wid, name, before) end
--- creates a new static text widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- @param wid widget
--- @param name string widgetname of the child
--- @param before widget|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @return number id
function CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild(wid, name, before) end
--- creates a new pure tooltip widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- @param wid widget
--- @param name string widgetname of the child
--- @param before widget|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @return number id
function CppLogic.UI.ContainerWidgetCreatePureTooltipWidgetChild(wid, name, before) end
--- creates a new gfx button widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- @param wid widget
--- @param name string widgetname of the child
--- @param before widget|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @return number id
function CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild(wid, name, before) end
--- creates a new text button widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- @param wid widget
--- @param name string widgetname of the child
--- @param before widget|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @return number id
function CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild(wid, name, before) end
--- creates a new progress bar widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- @param wid widget
--- @param name string widgetname of the child
--- @param before widget|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @return number id
function CppLogic.UI.ContainerWidgetCreateProgressBarWidgetChild(wid, name, before) end
--- creates a new container widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- @param wid widget
--- @param name string widgetname of the child
--- @param before widget|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @return number id
function CppLogic.UI.ContainerWidgetCreateContainerWidgetChild(wid, name, before) end
--- creates a new container widget and registers it as its child.
--- the widget is initially hidden and all variables at a default value.
--- all attached functions are empty, and the UpdateManualFlag is set.
--- position and size are 0.
--- not all customwidget types were tested by me, use at your own risk. known customwidget types:
--- - EGUIX::CVideoPlaybackCustomWidget
--		on first video start clamps the video to the widget size.
--		if you change size afterwards, it scales the video from the previously clamped size.
--- @param wid widget
--- @param name string widgetname of the child
--- @param customclass string name of the custom class. may not work properly, if not actually a customwidget.
--- @param before string|nil (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
--- @param icv0 number|nil intergerCustomVariable 0 (optional, default 0)
--- @param icv1 number|nil intergerCustomVariable 1 (optional, default 0)
--- @param icv2 number|nil intergerCustomVariable 2 (optional, default 0)
--- @param icv3 number|nil intergerCustomVariable 3 (optional, default 0)
--- @param icv4 number|nil intergerCustomVariable 4 (optional, default 0)
--- @param icv5 number|nil intergerCustomVariable 5 (optional, default 0)
--- @param scv0 string|nil stringCustomVariable 0 (optional, default "")
--- @param scv1 string|nil stringCustomVariable 1 (optional, default "")
--- @return number id
function CppLogic.UI.ContainerWidgetCreateCustomWidgetChild(wid, name, customclass, before, icv0, icv1, icv2, icv3, icv4, icv5, scv0, scv1) end

--- shows a resources gained floating number at a specific entity.
--- entity has to be visible, but settlers or rocks work fine.
--- if the entity moves, the floatie does move with it.
--- @param entity entity defines screen position
--- @param amount number to show (abs of this int gets shown)
function CppLogic.UI.ShowResourceFloatieOnEntity(entity, amount) end

--- shows a resources gained text at a specific position.
--- @param pos Position map position
--- @param txt string text to show (font is onscreenumberssmall, so not all chars can be displayed)
function CppLogic.UI.ShowAdvancedFloatie(pos, txt) end

--- char entered callback. use string.char to get the character.
--- does not work with SCELoader.
--- gets called when a key (or key kombination) is pressed that can be converted to a char.
--- you can return true to skip all additional message handling.
--- set to nil to remove.
--- status of this gets saved into a savegame.
--- @param f fun(c:number):boolean to be called (char)->handeled
function CppLogic.UI.SetCharTrigger(f) end
--- key pressed callback.
--- does not work with SCELoader.
--- use global Keys to check what was pessed.
--- gets called at least twice, when the key gets pressed down, then when the key gets released (only time up is true).
--- if the key keepd pressed, after a short delay the trigger is called in fast succession.
--- you can return true to skip all additional message handling.
--- set to nil to remove.
--- status of this gets saved into a savegame.
--- @param f fun(k:number, up:boolean):boolean to be called (key, up)->handeled
function CppLogic.UI.SetKeyTrigger(f) end
--- mouse event callback.
--- does not work with SCELoader.
--- use global MouseEvents to check what was pessed (from CommunityLib).
--- MouseMove does not get forwarded to lua (cause it is spammed),
--- Double clicks are not generated, you get 2 normal klicks instead.
--- parameters for MouseWheel are (id, x, y, forward)->handeled.
--- parameters for XButtons are (id, x, y, isxb2)->handeled.
--- parameters for all others are (id, x, y)->handeled.
--- x and y cooridates are screen coordinates not processed by SHoK and are equals to what GUI.GetMousePosition returns you (not scaled to widget coordinates).
--- you can return true to skip all additional message handling.
--- set to nil to remove.
--- status of this gets saved into a savegame.
--- @param f fun(id:number, x:number, y:number, forward:boolean):boolean to be called
function CppLogic.UI.SetMouseTrigger(f) end
--- same as CppLogic.UI.SetMouseTrigger, just designed for mainmenu
function CppLogic.UI.SetMouseTriggerMainMenu(f) end

--- sets a GUI State which reroutes clicks to lua functions.
--- onclick gets called on left click and gets x and y coordinates of the mouse cursor and can return a bool to override ending the state (default true).
--- oncancel gets called on right click (or gets canceled any other way).
--- make sure you set the mouse cursor after the call or modify GameCallback_GUI_StateChanged to set it.
--- @param onclick fun(x:number,y:number):boolean ->endstate
--- @param oncancel fun()
function CppLogic.UI.SetGUIStateLuaSelection(onclick, oncancel) end

--- sets a GUI State to place a construction site and choose its rotation.
--- @param ucat number building upgrade category
function CppLogic.UI.SetGUIStatePlaceBuildingEx(ucat) end
--- changes the rotation of a PlaceBuildingEx state.
--- @param deg number
function CppLogic.UI.SetPlaceBuildingRotation(deg) end
--- gets the rotation of a PlaceBuildingEx state.
--- @return number deg
function CppLogic.UI.GetPlaceBuildingRotation() end

--- gets upgrade category of the currently active PlaceBuilding or PlaceBuildingEx state.
--- also gets the foundation and top entitytype of a PlaceCannon state.
--- @return number ucat
--- @return number|nil top
function CppLogic.UI.GetPlaceBuildingUCat() end

--- gets the map position under a given screen position.
--- @param x number
--- @param y number
--- @return Position pos
--- @return number terrainHeigt
function CppLogic.UI.GetLandscapePosAtScreenPos(x, y) end

--- shows a command acknowledgement at a given position. (usually shown on move commands).
--- @param p Position
function CppLogic.UI.ShowCommandAcknowledgementAtPosition(p) end

--- shows a minimap marker at a give position.
--- @param p Position
--- @param pulsing boolean if false, marker is static (like GUI.CreateMinimapMarker), if true marker is animated (like GUI.CreateMinimapPulse)
--- @param r number color 0-255
--- @param g number color 0-255
--- @param b number color 0-255
--- @param timefactor number on pulsing, changes animation speed >1 takes longer for animation to complete (optional, default 1)
--- @param scalefactor number multiplied with scale (optional, default 1) (note, increasing the scale also increases animation speed)
function CppLogic.UI.CreateMiniMapMarker(p, pulsing, r, g, b, timefactor, scalefactor) end

--- shows a script marker at a give position (looks like GUI.ScriptSignal).
--- @param p Position
--- @param r number color 0-255
--- @param g number color 0-255
--- @param b number color 0-255
--- @param scalefactor number multiplied with scale (optional, default 1)
function CppLogic.UI.CreateMiniMapScriptSignal(p, pulsing, r, g, b, timefactor, scalefactor) end

--- gets the far clip plane min and max that are used in cutscenes. similar to Display.SetFarClipPlaneMinAndMax.
--- @return number max
--- @return number min
function CppLogic.UI.GetCutsceneFarClipPlaneMinAndMax() end
--- gets the far clip plane min and max that are used in cutscenes. similar to Display.SetFarClipPlaneMinAndMax.
--- @param max number
--- @param min number
function CppLogic.UI.SetCutsceneFarClipPlaneMinAndMax(max, min) end

--- gets the main windows client size.
--- == GUI.GetScreenSize(), but available in main menu.
--- @return number right
--- @return number bottom
function CppLogic.UI.GetClientSize() end

--- checks if a widget is a container widget.
--- @param wid widget
--- @return boolean
function CppLogic.UI.IsContainerWidget(wid) end

--- reloads the GUI xml.
--- this function is for compatibility with old Hook code specifically, i do recommend using the newer ContainerWidgetCreate... functions with the lua export of the GUIEditor.
--- @param path string
function CppLogic.UI.ReloadGUI(path) end

--- overrides some hardcoded checks for showing wood as resource floaties and mouseover info.
--- @param show boolean
function CppLogic.UI.SetShowWoodInUI(show) end

---@class ShortMessage
local ShortMessage = {
	Type = 1,
	StartTime = 1.0,
	Duration = 1.0,
	Tooltip = "",
	---@type Position
	Pos = {X=0,Y=0},
	Id = 1,
}
--- returns all current ShortMessages and all History entries (these block new ShortMessages depending on type).
--- check doc of CppLogic.UI.CreateShortMessage for type
---@return ShortMessage[] messages visible messages
---@return ShortMessage[] history blocks some short messages
function CppLogic.UI.GetShortMessages() end

--- reinitializes the size of the ShortMessagesListWindow stored inside of the ShortMessagesListWindowController.
function CppLogic.UI.ReInitShortMessagesSize() end

--- creates a ShortMessage. type determines button texture. max 20 messages, oldest gets removed first.
--- 1->ShortMessagesListWindowGfxPrototypeMilitarySettlerAttacked
--- 2->ShortMessagesListWindowGfxPrototypeMilitaryHouseAttacked
--- 3->ShortMessagesListWindowGfxPrototypeSettlerLeft
--- 4,5,6,7,8->ShortMessagesListWindowGfxPrototypeSettlerAngry (taxes, overtime, noWork, noFood, noResi)
--- 10->ShortMessagesListWindowGfxPrototypeUpgradePossible
--- 11->ShortMessagesListWindowGfxPrototypeLimitReached
--- 12->ShortMessagesListWindowGfxPrototypeFarmLimitReached
--- 13->ShortMessagesListWindowGfxPrototypeResidenceLimitReached
--- 14,15->ShortMessagesListWindowGfxPrototypeQuestChanged (quest, market)
--- 9->ShortMessagesListWindowGfxPrototypeNewTech
--- other->ShortMessagesListWindowGfxPrototypeNewTech
---@param type number
---@param duration number
---@param tooltip string
---@param pos Position|nil
function CppLogic.UI.CreateShortMessage(type, duration, tooltip, pos) end

---removes the ShortMessage with the specified id. does nothing, if id invalid.
---check CppLogic.UI.GetShortMessages for the ids.
---@param id number
function CppLogic.UI.RemoveShortMessage(id) end

---initializes a CppLogic::Mod::UI::AutoScrollCustomWidget.
---(calculating max scrollables, cloning scrollables, initializing scrollables, initializing partial scroll element texture from scrollable, initialize slider)
---@param id number|string
---@param numToScroll number number of elements to scroll through
---@return number widCount number of elements to scroll over
---@return number widCount number of widgets active
function CppLogic.UI.InitAutoScrollCustomWidget(id, numToScroll) end

---gets the offset, widget count and element count of a CppLogic::Mod::UI::AutoScrollCustomWidget.
---@param id number|string
---@return number offset offset of rendered widgets (float, floor to get the base offset)
---@return number widgetCount number of widgets active
---@return number elementCount number of elements to scroll
function CppLogic.UI.GetAutoScrollCustomWidgetOffset(id) end

---modifies the offset of a CppLogic::Mod::UI::AutoScrollCustomWidget.
---automatically clamps the resulting offset.
---@param id number|string
---@param off number modify offset by
function CppLogic.UI.AutoScrollCustomWidgetModOffset(id, off) end

---sets the offset of a CppLogic::Mod::UI::AutoScrollCustomWidget.
---@param id number|string
---@param off number set offset to
function CppLogic.UI.AutoScrollCustomWidgetSetOffset(id, off) end

---sets the partial scroll element texture of a CppLogic::Mod::UI::AutoScrollCustomWidget.
---if the scrolled widget has a material itself (button/...) gets initialized by that material.
---if not, can be set with this function.
---@param id number|string
---@param texture string
---@param x number
---@param y number
---@param w number
---@param h number
---@param r number
---@param g number
---@param b number
---@param a number
function CppLogic.UI.AutoScrollCustomWidgetSetMaterial(id, texture, x, y, w, h, r, g, b, a) end

---gets the text of a CppLogic::Mod::UI::TextInputCustomWidget.
---@param id number|string
---@return string
function CppLogic.UI.TextInputCustomWidgetGetText(id) end

---sets the text of a CppLogic::Mod::UI::TextInputCustomWidget.
---@param id number|string
---@param txt string
function CppLogic.UI.TextInputCustomWidgetSetText(id, txt) end

---sets the ignore next char of a CppLogic::Mod::UI::TextInputCustomWidget.
---@param id number|string
---@param ignore boolean
function CppLogic.UI.TextInputCustomWidgetSetIgnoreNextChar(id, ignore) end

---@enum CPLTextInputCustomWidgetEvent
CppLogic.UI.TextInputCustomWidgetEvent = {
	Confirm = 0,
	Cancel = 1,
	Validate = 2,
}

---sets the ignore next char of a CppLogic::Mod::UI::TextInputCustomWidget.
---@param id number|string
---@return boolean
function CppLogic.UI.InputCustomWidgetHasFocus(id) end

---sets the ignore next char of a CppLogic::Mod::UI::TextInputCustomWidget.
---@param id number|string
---@param f boolean
function CppLogic.UI.InputCustomWidgetSetFocus(id, f) end

---gets camera data
---@return number lookAtX
---@return number lookAtY
---@return number lookAtZ
---@return number distance
---@return number yawAngle horizontal
---@return number pitchAngle vertical
function CppLogic.UI.GetCameraData() end

---sets camera data
---@param lookAtX number
---@param lookAtY number
---@param lookAtZ number
---@param distance number
---@param yawAngle number horizontal
---@param pitchAngle number vertical
function CppLogic.UI.SetCameraData(lookAtX, lookAtY, lookAtZ, distance, yawAngle, pitchAngle) end

---gets list of all cutscenes
---@return string[]
function CppLogic.UI.ListCutscenes() end

---exports a cutscene to lua
---@param name string
---@return table|nil
function CppLogic.UI.GetCutscene(name) end

---imports a cutscene from lua
---@param name string
---@param cutscene table
function CppLogic.UI.SetCutscene(name, cutscene) end

---exports all cutscenes currently in memory to xml files.
---uses a save file dialog to query save location, using path as the default.
---@param path string|nil 
function CppLogic.UI.ExportCutscenes(path) end

---gets the video playback size of a EGUIX::CVideoPlaybackCustomWidget.
---@param wid string|number 
---@return number|nil x
---@return number|nil y
function CppLogic.UI.VideoCustomWidgetGetVideoSize(wid) end

---sets the video playback size of a EGUIX::CVideoPlaybackCustomWidget.
---this usually gets set to the widget size on the first call to XGUIEng.StartVideoPlayback.
---needs to match the video files size.
---note: changes after first initializing is expensive, try to avoid it.
---@param wid string|number 
---@param x number
---@param y number
function CppLogic.UI.VideoCustomWidgetSetVideoSize(wid, x, y) end

---sets a callback function for clicks on a minimap overlay.
---funcname is the name of the func to be called, queried via evaulating return.
---(so contained . are possible).
---calling this function will replace a GGUI::CMiniMapOverlayCustomWidget with a CppLogic::Mod::UI::MiniMapOverlayWithCallbackCustomWidget.
---any other customwidget will lead to an assertion.
---@param wid string|number
---@param funcname string
function CppLogic.UI.MiniMapOverlaySetCallbackFuncName(wid, funcname) end

---@class TerrainDecalAccess
local TerrainDecalAccess = {}
---destroys (removes from game world)
function TerrainDecalAccess:Destroy() end
---move
---@param p Position
function TerrainDecalAccess:SetPos(p) end

---creates a selection texture decal at a fixed position
---@param pos Position
---@param texture string|number
---@param sizex number
---@param sizey number
---@return TerrainDecalAccess
function CppLogic.UI.CreateSelectionDecal(texture, pos, sizex, sizey) end

---gives a construct command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Serf_ConstructBuilding(id, tar) end

---gives a repair command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Serf_RepairBuilding(id, tar) end

---gives a extract command (either with a target entity, or with a resource type + position)
---@param id number|string
---@param tar number|string
---@param pos nil|Position
function CppLogic.UI.Commands.Serf_ExtractResource(id, tar, pos) end

---gives a attack move command
---@param id number|string
---@param tar Position
function CppLogic.UI.Commands.Entity_AttackPos(id, tar) end

---gives a guard command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Entity_GuardEntity(id, tar) end

---gives a place bomb command
---@param id number|string
---@param tar Position
function CppLogic.UI.Commands.BombPlacer_PlaceBombAt(id, tar) end

---gives a place cannon command
---@param id number|string
---@param tar Position
---@param foundationType number|string
---@param topType number|string
function CppLogic.UI.Commands.CannonPlacer_HeroPlaceCannonAt(id, tar, foundationType, topType) end

---gives a npc interaction command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Hero_NPCInteraction(id, tar) end

---gives a convert settler command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.ConvertSettler_Convert(id, tar) end

---gives a steal command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Thief_StealFrom(id, tar) end

---gives a return stolen goods command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Thief_CarryStolenStuffToHQ(id, tar) end

---gives a sabotage command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Sabotage_Sabotage(id, tar) end

---gives a defuse command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Sabotage_Defuse(id, tar) end

---gives a binocular command
---@param id number|string
---@param tar Position
function CppLogic.UI.Commands.Binoculars_Observe(id, tar) end

---gives a snipe command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Sniper_Snipe(id, tar) end

---gives a place torch command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.TorchPlacer_Place(id, tar) end

---gives a shuriken command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Shuriken_Activate(id, tar) end

---gives a move command
---@param id number|string
---@param tar number|string
---@param r number|nil
function CppLogic.UI.Commands.Entity_Move(id, tar, r) end

---gives a patrol command
---@param id number|string
---@param tar Position
---@param ... Position
function CppLogic.UI.Commands.Entity_Patrol(id, tar, ...) end

---gives a lighning strike command
---@param id number|string
---@param tar Position
function CppLogic.UI.Commands.LightningStrike_Activate(id, tar) end

---gives a refill mine command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.ResDoodadRefill_Activate(id, tar) end

---gives a shield cover command
---@param id number|string
function CppLogic.UI.Commands.ShieldCover_Activate(id) end

---gives a resurrect command
---@param id number|string
---@param tar number|string
function CppLogic.UI.Commands.Resurrect_Activate(id, tar) end

---gives a bombardment command
---@param id number|string
---@param p Position
function CppLogic.UI.Commands.Bombardment_Activate(id, p) end

---gives a bomb cannon combo ability command
---@param id number|string
---@param p Position
function CppLogic.UI.Commands.BombCannonCombo_Activate(id, p) end

--- loads an entitytype from a xml file (data/config/entities/typename.xml).
--- the entitytype gets automatically removed/reloaded on leaving the map.
--- load any additional models you want to use first.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param typename string|number
--- @return number type_id
function CppLogic.ModLoader.AddEntityType(typename) end
CppLogic.ModLoader.ReloadEntityType = CppLogic.ModLoader.AddEntityType

--- assigns an id in the EntityType IdManager. A call to AddEntityType is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadEntityType(typename) end

--- sets an entitytype to get automatically reloaded on leaving the map (after s5x archives got removed).
--- (for editing it manually via CppLogic).
--- this function is available after ModLoader finishes.
--- @param tid number
function CppLogic.ModLoader.SetEntityTypeToReload(tid) end

--- reloads a feedback event sound data.
--- @param tid number|string
function CppLogic.ModLoader.LoadFeedbackEventSoundData(tid) end

--- accesses an entitytypes memory
--- warning: has no semantic checks, it is easy to crash your game by modifying memory!
--- automatically marks the entitytype for reload on writing
--- @param tid number
--- @return CppBBObjectAccess
function CppLogic.ModLoader.GetEntityTypeMem(tid) end

--- accesses a task lists memory
--- warning: has no semantic checks, it is easy to crash your game by modifying memory!
--- automatically marks the tasklist for reload on writing
--- @param tid number
--- @return CppBBObjectAccess
function CppLogic.ModLoader.GetTaskListMem(tid) end

--- accesses a techs memory
--- warning: has no semantic checks, it is easy to crash your game by modifying memory!
--- automatically marks the tech for reload on writing
--- @param tid number
--- @return CppStructAccess
function CppLogic.ModLoader.GetTechnologyMem(tid) end

--- accesses an effecttype memory
--- warning: has no semantic checks, it is easy to crash your game by modifying memory!
--- automatically marks the effecttype for reload on writing
--- @param tid number
--- @return CppStructAccess
function CppLogic.ModLoader.GetEffectTypeMem(tid) end

--- accesses a feedback event sound data memory
--- warning: has no semantic checks, it is easy to crash your game by modifying memory!
--- gets reloaded by shok automatically
--- @param tid number
--- @return CppStructAccess
function CppLogic.ModLoader.GetFeedbackEventMem(tid) end

--- loads an effecttype from a xml file (data/config/effects/typename.xml) (not default location).
--- the effecttype gets automatically removed/reloaded on leaving the map.
--- load any additional models you want to use first.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param typename string|number
--- @return number type_id
function CppLogic.ModLoader.AddEffectType(typename) end
CppLogic.ModLoader.ReloadEffectType = CppLogic.ModLoader.AddEffectType

--- assigns an id in the EffectType IdManager. A call to AddEffectType is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadEffectType(typename) end

--- loads a task list from a xml file (data/config/tasklists/tlname.xml).
--- the task list gets automatically removed on leaving the map.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param tlname string|number
--- @return number type_id
function CppLogic.ModLoader.AddTaskList(tlname) end
CppLogic.ModLoader.ReloadTaskList = CppLogic.ModLoader.AddTaskList

--- assigns an id in the TaskList IdManager. A call to AddTaskList is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadTaskList(typename) end

--- sets a tasklist to get automatically reloaded on leaving the map (after s5x archives got removed).
--- (for editing it manually via CppLogic).
--- this function is available after ModLoader finishes.
--- @param tid number
function CppLogic.ModLoader.SetTaskListToReload(tid) end

--- loads a technology from a xml file (data/config/technologies/tname.xml).
--- the tech gets automatically removed on leaving the map.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param tname string|number
--- @return number type_id
function CppLogic.ModLoader.AddTechnology(tname) end
CppLogic.ModLoader.ReloadTechnology = CppLogic.ModLoader.AddTechnology

--- assigns an id in the Technologies IdManager. A call to AddTechnology is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadTechnology(typename) end

--- loads a model from a xml and dff file (data/config/models/mname.xml and data/graphics/models/mname.dff).
--- the xml contains the data from Models.xml, the dff is the actual model.
--- also loads all required textures (remember to put the 3 thexture sizes into the correct folders).
--- the model gets automatically removed on leaving the map.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param mname string|number
--- @return number type_id
function CppLogic.ModLoader.AddModel(mname) end
CppLogic.ModLoader.ReloadModel = CppLogic.ModLoader.AddModel

--- assigns an id in the Models IdManager. A call to AddModel is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadModel(typename) end

--- loads a gui texture from a png file (data/graphics/textures/gui/tname.png).
--- the texture gets automatically removed on leaving the map.
--- (different to other reload funcs in ModLoader, this one takes the name (full path) of the texture instead of its id as parameter).
--- note: this is similar to CppLogic.UI.PreLoadGUITexture, just that the texture gets cleaned up.
--- this function is available after ModLoader finishes.
--- @param tname string full path to texture
--- @return number type_id
function CppLogic.ModLoader.AddGUITexture(tname) end
CppLogic.ModLoader.ReloadGUITexture = CppLogic.ModLoader.AddGUITexture

--- loads an animation from an anm file (data/graphics/animations/aname.anm).
--- the animation gets automatically removed on leaving the map.
--- only writes the id to Animations if it exists.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param aname string|number
--- @return number type_id
function CppLogic.ModLoader.AddAnimation(aname) end
CppLogic.ModLoader.ReloadAnimation = CppLogic.ModLoader.AddAnimation

--- assigns an id in the Animations IdManager. A call to AddAnimation is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadAnimation(typename) end

--- adds a settler upgradecategory. to create a upgradecategory, fill in the Upgrade xml entry in the entities, but leave out Category.
--- then call this function with the upgradecategory and the first entitytype. (this will fill in the missing Category).
--- @param ucatname string
--- @param firstid number|string
--- @return number type_id
function CppLogic.ModLoader.AddSettlerUpgradeCategory(ucatname, firstid) end

--- adds a building upgradecategory. to create a upgradecategory, fill in the Upgrade xml entry in the entities, but leave out Category.
--- then call this function with the upgradecategory and the first entitytype. (this will fill in the missing Category).
--- @param ucatname string
--- @param firstid number|string
--- @return number type_id
function CppLogic.ModLoader.AddBuildingUpgradeCategory(ucatname, firstid) end

--- assigns an id in the UpgradeCategory IdManager. A call to AddSettlerUpgradeCategory or AddBuildingUpgradeCategory is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadUpgradeCategory(typename) end

--- loads a selection texture file (data/graphics/textures/tname.dds).
--- the texture name is referenced in the additional model data.
--- remember to put the 3 texture sizes into the correct folders.
--- the texture gets removed on leaving the map.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param tname string
function CppLogic.ModLoader.AddSelectionTexture(tname) end
CppLogic.ModLoader.ReloadSelectionTexture = CppLogic.ModLoader.AddSelectionTexture

--- assigns an id in the SelectionTextures IdManager. A call to AddSelectionTexture is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadSelectionTexture(typename) end

--- loads a water type from a xml file (data/config/watertypes/wname.xml).
--- all water types get reloaded on exiting the map, after using this function.
--- you may request a specific id, or get one automatically by passing 0.
--- if you request a specific id and it does already exist, throws.
--- only writes the id to WaterTypes if it exists.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param wname string|number
--- @return number type_id
function CppLogic.ModLoader.AddWaterType(wname) end
CppLogic.ModLoader.ReloadWaterType = CppLogic.ModLoader.AddWaterType

--- assigns an id in the WaterType IdManager. A call to AddWaterType is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadWaterType(typename) end

--- loads a terrain texture file (data/graphics/textures/tname.xml).
--- the texture name is referenced in water and terrain types.
--- remember to put the 3 texture sizes into the correct folders.
--- the texture gets removed on leaving the map.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param tname string
function CppLogic.ModLoader.AddTerrainTexture(tname) end
CppLogic.ModLoader.ReloadTerrainTexture = CppLogic.ModLoader.AddTerrainTexture

--- assigns an id in the TerrainTexture IdManager. A call to AddTerrainTexture is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadTerrainTexture(typename) end

--- loads a terrain type from a xml file (data/config/terraintypes/tname.xml).
--- all terrain types get reloaded on exiting the map, after using this function.
--- you may request a specific id, or get one automatically by passing 0.
--- if you request a specific id and it does already exist, throws.
--- only writes the id to TerrainTypes if it exists.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param tname string|number
--- @return number type_id
function CppLogic.ModLoader.AddTerrainType(tname) end
CppLogic.ModLoader.ReloadTerrainType = CppLogic.ModLoader.AddTerrainType

--- assigns an id in the TerrainType IdManager. A call to AddTerrainType is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadTerrainType(typename) end

--- loads an ExperienceClass from a xml file (data/config/experiencexpclassname.xml).
--- assigns this experienceclass to every entity that has the category ecategory.
--- the ExperienceClass is removed on exiting the map.
--- (ecategory may be a random int that can get attached to entitytypes via CppLogic.EntityType.AddEntityCategory, just remember to remove it again).
--- only writes the id to ExperienceClasses if it exists.
--- @param xpclassname string
--- @param ecategory number|string
--- @return number type_id
function CppLogic.ModLoader.AddExperienceClass(xpclassname, ecategory) end
CppLogic.ModLoader.ReloadExperienceClass = CppLogic.ModLoader.AddExperienceClass

--- loads multiple sounds from multiple files (data/sounds/soundname.wav).
--- the sounds get reloaded any time they get started, so you have to keep the archive loaded.
--- all sounds get automatically removed on exiting the map.
--- all sounds added in one call of AddSounds form a random group, on playing one of them, one of the group gets randomly played.
--- @param soundname string
--- @param ... string additional sounds for the group
--- @return number type_id id of first sound
function CppLogic.ModLoader.AddSounds(soundname, ...) end

--- loads an AnimSet from a xml file (data/config/animsets/setname.xml).
--- the AnimSet gets automatically removed when exiting the map.
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param setname string|number
--- @return number type_id
function CppLogic.ModLoader.AddAnimSet(setname) end
CppLogic.ModLoader.ReloadAnimSet = CppLogic.ModLoader.AddAnimSet

--- assigns an id in the AnimationSets IdManager. A call to AddAnimSet is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadAnimSet(typename) end

--- loads or reloads a DirectX effect (shader pair) from a fx file (data/graphics/effects/ename.fx and data/graphics/effects/ffp/ename.fx).
--- the effect gets automatically removed when exiting the map.
--- be careful, the ids of these effects cannot be reused.
--- @param ename string
function CppLogic.ModLoader.LoadDirectXEffect(ename) end

--- creates a ArmorClass to use in damageclasses and entitytypes.
--- @param ac string
function CppLogic.ModLoader.PreLoadArmorClass(ac) end

--- creates or replaces a DamageClass with a serialized lua table. (see data_notes.txt for format)
--- for existing ids you may use the id or the name, for new ones you need to use the name.
--- @param dc string|number
--- @param cfg table
function CppLogic.ModLoader.AddDamageClass(dc, cfg) end
CppLogic.ModLoader.ReloadDamageClass = CppLogic.ModLoader.AddDamageClass

--- assigns an id in the DamageClasses IdManager. A call to AddDamageClass is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadDamageClass(typename) end

--- loads a stringtabletxt override file.
--- if called from mainmenu, loads it as mainmenu overrides, not affecting ingame.
--- @param filename string
--- @param language string
--- @param prefixOverride string|nil
--- @return number numLoaded -1 if not found
function CppLogic.ModLoader.LoadStringTableTextOverrides(filename, language, prefixOverride) end

--- sets all damageclasses to get automatically reloaded on leaving the map (after s5x archives got removed).
--- (for editing it manually via CppLogic).
--- this function is available after ModLoader finishes.
function CppLogic.ModLoader.SetDamageclassesToReload() end

--- assigns an id in the EntityCategory IdManager.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadEntityCategory(typename) end

--- loads a font from a met file (data/menu/fonts/fname.xml).
--- @param fname string|number
--- @return number type_id
function CppLogic.ModLoader.AddFont(fname) end
CppLogic.ModLoader.ReloadFont = CppLogic.ModLoader.AddFont

--- assigns an id in the Font IdManager. will get loaded automatically on first use, but using AddFont is neccessary, if the archive is supposed to get popped.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadFont(typename) end

--- refreshes the entitycategory->entitytype cache.
function CppLogic.ModLoader.RefreshEntityCategoryCache() end

--- does some checks for the most common usage errors (as far as reasonable).
function CppLogic.ModLoader.SanityCheck() end

---@class ArchivePopHelper
local ArchivePopHelper = {
}
---@return boolean wasLoaded
function ArchivePopHelper:Remove() end
---@return boolean isLoaded
function ArchivePopHelper:IsLoaded() end
---@return string archivePath
function ArchivePopHelper:ToString() end

---@class RedirectLayerPopHelper : ArchivePopHelper
local RedirectLayerPopHelper = {
}
---@return table<string,string>
function RedirectLayerPopHelper:Get() end
---@param n string
---@param r string|nil
function RedirectLayerPopHelper:Set(n, r) end

---@class ModpackDesc
local ModpackDesc = {
	Name = "", BBAPath = "", LoaderPath = "", ScriptPath = "", MainmenuPath = "", Version = "",
	Description = "",
	---@type string[]
	Required = {},
	---@type string[]
	Incompatible = {},
	---@type string[]
	Override = {},
	DataMod = false, ScriptMod = false, MainmenuMod = false, KeepArchive = false,
	UserRequestable = false, ScriptLib = false,
	Archive = nil|ArchivePopHelper,
	RedirectLayer = nil|RedirectLayerPopHelper
}

--- gets info for a ModPack (mostly from its ModPack.xml).
---@param name string
---@return ModpackDesc|string
function CppLogic.ModLoader.GetModpackInfo(name) end

--- gets map modpack info from its extended info.xml
---@param name string
---@param type number
---@param cname string?
---@return string[] required
---@return string[] incompatible
---@return string[] userRequestable
function CppLogic.ModLoader.MapGetModPacks(name, type, cname) end

--- loads the bba of a ModPack.
---@param name string
---@return ArchivePopHelper
function CppLogic.ModLoader.LoadModpackBBA(name) end

--- creates a redirect layer for a modpack.
---@param name string
---@return RedirectLayerPopHelper
function CppLogic.ModLoader.CreateModpackRedirectLayer(name) end

--- shows a MessageBox with the message, then exits the game.
---@param msg string
function CppLogic.ModLoader.InvalidModPackPanic(msg) end

--- asks for a file to export a entitytype to.
--- the exported entitytype is mostly compliant with my xml schema (some manual cleanup + adding the schema reference is required).
---@param ety number|string
---@param path string|nil hint
function CppLogic.ModLoader.ReserializeEntityType(ety, path) end

--- asks for a file to export a tasklist to.
--- the exported tasklist is mostly compliant with my xml schema (some manual cleanup + adding the schema reference is required).
---@param ety number|string
---@param path string|nil hint
function CppLogic.ModLoader.ReserializeTaskList(ety, path) end

--- asks for a file to export a model data to.
--- the exported model data is mostly compliant with my xml schema (some manual cleanup + adding the schema reference is required).
---@param ety number|string
---@param path string|nil hint
function CppLogic.ModLoader.ReserializeModel(ety, path) end

--- returns a list of every bba in ModPacks.
--- @return string[]
function CppLogic.ModLoader.GetModpacks() end

--- sets the modpack list for savegames.
--- @param l string
function CppLogic.ModLoader.SetModPackList(l) end

--- gets the modpack list for savegames or nil.
--- @return string|nil
function CppLogic.ModLoader.GetModPackList() end

--- resets the global CppLogic.
--- useful if you dont want to use FrameworkWrapper to prevent savegames to override it.
function CppLogic_ResetGlobal() end

--- @class UACore
local UACore = {}
---add leader
---@param id number
function UACore:AddLeader(id) end
---@return Position
function UACore:GetPos() end
---@param s UnlimitedArmy
function UACore:Tick(s) end
---@return fun(state:userdata, index:number):number,number
---@return userdata
---@return number
function UACore:Iterate() end
---@return fun(state:userdata, index:number):number,number
---@return userdata
---@return number
function UACore:IterateTransit() end
---@param ol number
---@param ne number
function UACore:OnIdChanged(ol, ne) end
---@param trans boolean?
---@param deadHero boolean?
---@return number
function UACore:GetSize(trans, deadHero) end
---@param id number
function UACore:RemoveLeader(id) end
---@return boolean
function UACore:IsIdle() end
---@return UAStatus
function UACore:GetStatus() end
---@param a number
function UACore:SetArea(a) end
---@param p Position
function UACore:SetTarget(p) end
function UACore:DumpTable() end
function UACore:ReadTable(t) end
---@param s UAStatus
function UACore:SetStatus(s) end
---@param b boolean?
function UACore:SetReMove(b) end
---@param id number
function UACore:SetCurrentBattleTarget(id) end
---@return number[]
---@return number[]
---@return number[]
function UACore:GetRangedMelee() end
---@param fl boolean?
function UACore:SetIgnoreFleeing(fl) end
---@param ar number
function UACore:SetAutoRotateFormation(ar) end
---@return number?
function UACore:GetFirstDeadHero() end
---@param p boolean?
function UACore:SetPrepDefense(p) end
---@param s boolean?
function UACore:SetSabotageBridges(s) end
---@param n boolean?
function UACore:SetDoNotNormalizeSpeed(n) end


--- creates new ua.
--- @param pl number player
--- @return UACore ua
function CppLogic.UA.New(pl, format, commandqueue, spawner, seed) end

--- gets next enemy in area.
---@param pl number
---@param pos Position
---@param area number
---@param ignoreFleeing boolean?
---@return number
function CppLogic.UA.GetNearestEnemyInArea(pl, pos, area, ignoreFleeing) end

--- adds cannon builder her ability types
--- @param heroTy number|string
--- @param bottomTy number|string
--- @param topTy number|string
function CppLogic.UA.AddCannonBuilderData(heroTy, bottomTy, topTy) end

--- gets number of enemies in area
---@param pl number
---@param pos Position
---@param area number
---@param ignoreFleeing boolean?
---@return number
function CppLogic.UA.CountTargetEntitiesInArea(pl, pos, area, ignoreFleeing) end


assert(false, "do not load this file, this is documentation only!")

--- CppLogic
-- these changes are always active:
-- - text rendering:
--		- all @ codes do now work after alignment codes (only the last alignment code per line takes effect).
--		- automatic line breaks do now work properly with non la alignment.
--		- added @la alignment code.
--		- checking for @ codes inside words (space before @ no longer needed).
--		- all @ codes can be terminated by | instead of space (space will get rendered, | will not).
--		- added @defaultcolor to reset color to the widgets default.
--		- added @skipWhitespace, which takes on or off as parameter, to turn off condensing multiple spaces to one.
--		- added @@ to print one @ (escape sequence)
-- - music can now be played from bba/s5x archives (make sure the s5x is loaded if you want to use music from it).
-- - removed override for the functions error and pcall, they now work as they should
CppLogic = {}
CppLogic.Effect = {}
CppLogic.Memory = {}
CppLogic.API = {}
CppLogic.Combat = {}
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
CppLogic.UA = {}

--- CppLogic version number.
CppLogic.Version = 1.3002

--- call this function to cleanup used hooks.
-- does not reset values in entitytypes.
function CppLogic.OnLeaveMap() end

--- check if an effect is valid
-- @param id effect id
-- @return true/false
function CppLogic.Effect.IsValidEffect(id) end

--- create a projectile.
-- to deal damage on impact use:
-- - single target: dmg > 0.
--		tarid to target entity id.
--		attid or playerid? set to attacker entity or player.
-- - AOE: dmg > 0, radius > 0.
--		attid or playerid can be set to attacker entity or player (deals damage only to entities hostile to attacker).
--		dmgclass can be set to the attacks damageclass to include it in the calculation.
-- @param effecttype GGL_Effects.XXX
-- @param startx start pos x
-- @param starty start pos y
-- @param tarx target pos x
-- @param tary target pos y
-- @param dmg damage done by projectile (optional, 0/nil if not used)
-- @param radius damage radius for AOE projectiles (optional, -1/nil if not used)
-- @param tarid target entity id (optional, 0/nil if not used)
-- @param attid attacker entity id (optional, 0/nil if not used)
-- @param playerid attacker player id (optional, 0/nil if not used)
-- @param dmgclass damageclass (optional, 0/nil if not used)
-- @param callback function that gets called on hit (optional, nil if not used)
-- @param source what deals damage, used for triggers (default shok::AdvancedDealDamageSource.Script) max size 8 bit instead of 32
-- @return effect id
function CppLogic.Effect.CreateProjectile(effecttype, startx, starty, tarx, tary, dmg, radius, tarid, attid, playerid, dmgclass, callback, source) end

--- sets high precision FPU (gets reset on every API call, so call id directly before your calculations)
function CppLogic.Memory.SetFPU() end

--- reloads cutscenes from the specified path
-- @param path optional, default "Maps\\ExternalMap"
function CppLogic.Logic.ReloadCutscene(path) end

--- gets the animation id from a name.
-- @param anim the name of the animation
-- @return animation id
function CppLogic.Logic.GetAnimIdFromName(anim) end

--- the damage factor between a damageclass and an armorclass.
-- @param dmgclass damageclass
-- @param armorclass armorclass
-- @return factor
function CppLogic.Logic.GetDamageFactor(dmgclass, armorclass) end
--- the damage factor between a damageclass and an armorclass.
-- @param dmgclass damageclass
-- @param armorclass armorclass
-- @param fac factor
function CppLogic.Logic.SetDamageFactor(dmgclass, armorclass, fac) end

--- the tick where this players payday got startet (-1 if inactive).
-- (you can get the current tick via Logic.GetCurrentTurn() ).
-- @param p player
-- @return tick
function CppLogic.Logic.PlayerGetPaydayStartetTick(p) end
--- the tick where this players payday got startet (-1 if inactive).
-- (you can get the current tick via Logic.GetCurrentTurn() ).
-- if you disable it (via -1) it gets immediately restarted has a worker or leader on the map.
-- @param p player
-- @param t tick
function CppLogic.Logic.PlayerSetPaydayStartetTick(p, t) end
--- sets the time between paydays. (get via Logic).
-- @param freq frequency in seconds
function CppLogic.Logic.SetPaydayFrequency(freq) end

--- player kill statistics.
-- @param p player
-- @return numSettlerKilled
-- @return numSettlerLost
-- @return numBuildingKilled
-- @return numBuildingLost
function CppLogic.Logic.PlayerGetKillStatistics(p) end

--- checks building placement at a specific position.
-- does not check, if BuildOn type matches.
-- if bon is nil, searches for a buildon entity at the specified position (if the building is buildon).
-- @param ty entitytype to place
-- @param pl player
-- @param pos target position
-- @param rot rotation
-- @param bon BuildOn entity id, or 0, or nil
function CppLogic.Logic.CanPlaceBuildingAt(ty, pl, pos, rot, bon) end

--- activates alarm mode.
-- @param pl player
function CppLogic.Logic.PlayerActivateAlarm(pl) end
--- deactivates alarm mode.
-- @param pl player
function CppLogic.Logic.PlayerDeactivateAlarm(pl) end

--- upgrades a settler category.
-- @param pl player
-- @param ucat
function CppLogic.Logic.PlayerUpgradeSettlerCategory(pl, ucat) end

--- sets a players tax level.
-- @param pl player
-- @param tl
function CppLogic.Logic.PlayerSetTaxLevel(pl, tl) end

--- activates a players weathermachine/weathertower.
-- does not check if you are at that weather, or the availability of buildings.
-- @param pl player
-- @param wt
function CppLogic.Logic.PlayerActivateWeatherMachine(pl, wt) end

--- blesses a players settlers.
-- does not check if you have settlers of that category or a high enough monastery.
-- @param pl player
-- @param bc
function CppLogic.Logic.PlayerBlessSettlers(pl, bc) end

--- gets the sector of a position.
-- @param p
-- @return sector
function CppLogic.Logic.LandscapeGetSector(p) end

--- gets the nearest unblocked position in range and in a specific sector.
-- @param p
-- @param sector
-- @param range
-- @return position or nil
function CppLogic.Logic.LandscapeGetNearestUnblockedPosInSector(p, sector, range) end

--- gets the terrain type of a position.
-- quads resolution (pos/100/4), input full pos.
-- @param p
-- @return terrain type
function CppLogic.Logic.LandscapeGetTerrainType(p) end
--- gets the water type of a position.
-- quads resolution (pos/100/4), input full pos.
-- @param p
-- @return water type
function CppLogic.Logic.LandscapeGetWaterType(p) end
--- gets the water height of a position.
-- quads resolution (pos/100/4), input full pos.
-- @param p
-- @return water height
function CppLogic.Logic.LandscapeGetWaterHeight(p) end
--- gets the terrain height of a position.
-- height resolution (pos/100), input full pos.
-- @param p
-- @return terrain height
function CppLogic.Logic.LandscapeGetTerrainHeight(p) end
--- gets the terrain vertex color of a position.
-- height resolution (pos/100), input full pos.
-- @param p
-- @return terrain vertex color
function CppLogic.Logic.LandscapeGetTerrainVertexColor(p) end
--- gets the blocking of a position.
-- height resolution (pos/100), input full pos.
-- @param p
-- @return blocking info bitfield (Blocking -> 1, BridgeArea -> 2, BuildBlock -> 4, TerrainSlope -> 8)
function CppLogic.Logic.LandscapeGetBlocking(p) end
--- gets the bridge height of a position.
-- quads resolution (pos/100/4), input full pos.
-- is 0 if not set by a bridge that was there at some point.
-- bridges set the height in a rect +500, the dont seem to remove the heigth.
-- to check if bridge height is active, use CppLogic.Logic.LandscapeGetBlocking.
-- @param p
-- @return bridge height
function CppLogic.Logic.LandscapeGetBridgeHeight(p) end

--- enables Events.LOGIC_EVENT_ENTITY_HURT_ENTITY trigger, even if attacker is 0.
-- also sets the hero as attacker on bomb explode.
-- does not work with SCELoader.
-- @param b enable (optional, default true)
-- @param killfunc func(attackerid, killid, attackerplayer, source), (optional, default nil)
function CppLogic.Logic.EnableAllHurtEntityTrigger(b, killfunc) end

--- enables entity max hp to be modified by techs.
-- does not work with SCELoader.
-- use a techs HitpointModifier to set up the boni.
-- then use ModifyHitpoints on settlers and CppLogic.EntityType.Building.AddHPTechMod for buildings to add the techs.
function CppLogic.Logic.EnableMaxHPTechMod() end

--- gets the damage that is going to be dealt in a Events.LOGIC_EVENT_ENTITY_HURT_ENTITY trigger.
-- requires activated CppLogic.Logic.EnableAllHurtEntityTrigger.
-- with SCELoader, returns only the damage.
-- - CppLogic.Logic.FixSnipeDamage is required for damage source AbilitySnipe.
-- - cannonball damage source override requires CppLogic.Combat.EnableAoEProjectileFix().
-- - salims trap shows up as unknown (todo?)
-- does not work with SCELoader.
-- @return dmg
-- @return damageSource
-- @return attackerPlayer
function CppLogic.Logic.HurtEntityGetDamage() end
--- sets the damage that is going to be dealt in a Events.LOGIC_EVENT_ENTITY_HURT_ENTITY trigger.
-- requires activated CppLogic.Logic.EnableAllHurtEntityTrigger.
-- does not work with SCELoader.
-- @param dmg
function CppLogic.Logic.HurtEntitySetDamage(dmg) end

--- gets all loaded archives and folders.
-- @return table of strings
function CppLogic.Logic.GetLoadOrder() end
--- adds a archive on top of your load order.
-- @param map archive, relative to current dir, so "extra2\\shr\\maps\\user\\map.s5x for a map file
function CppLogic.Logic.AddArchive(arch) end
--- removes the top archive. may only remove .s5x map archives.
function CppLogic.Logic.RemoveTopArchive() end

--- color by color index.
-- does not work with SCELoader.
-- @param index
-- @return r
-- @return g
-- @return b
-- @return a
function CppLogic.Logic.GetColorByColorIndex(index) end
--- color by color index. Refreshes the color of players.
-- does not work with SCELoader.
-- @param index
-- @param r
-- @param g
-- @param b
-- @param a (optional, default 255)
function CppLogic.Logic.SetColorByColorIndex(index, r, g, b, a) end

--- sets a function to be called after payday was done.
-- @param func to be called (playerId, taxes)->GoldToAdd
-- does not work with SCELoader.
function CppLogic.Logic.SetPaydayCallback(func) end

--- sets a function to be called for building placement checks.
-- only gets called if all usual conditions for placement are satisfied.
-- set to nil to remove.
-- (with SCELoader player is always -1, and buildOnID is a bool instead of an id).
-- @param func to be called (entitytype, playerId, pos, rotation, buildOnID)->canBuild
function CppLogic.Logic.SetPlaceBuildingAdditionalCheck(func) end

--- sets if leader regenration regenerates troop hp.
-- does not work with SCELoader.
-- @param b bool
function CppLogic.Logic.SetLeadersRegenerateTroopHealth(b) end

--- sets a stringtabletexts accosiated text. automatically resets on leaving the game.
-- (stored in lua registry).
-- does not work with SCELoader.
-- @param key key to replace
-- @param text replacement string or nil to restore default
function CppLogic.Logic.SetStringTableText(key, text) end

--- serf ui place building rotaton.
-- does not work with SCELoader.
-- @return rotation
function CppLogic.Logic.GetPlaceBuildingRotation() end
--- serf ui place building rotaton.
-- does not work with SCELoader.
-- @param r rotation
function CppLogic.Logic.SetPlaceBuildingRotation(r) end

--- fixes snipe task, now it actually shoots a projectile which does damage.
-- @param override func (optional, default nil) (sniper, target, dmg)->dmgOverride called before the projectile gets fired, can change the damage)
function CppLogic.Logic.FixSnipeDamage(override) end

--- gets the current weather GFX state (4th param of Logic.AddWeatherElement).
-- @return weatherGFX
function CppLogic.Logic.GetCurrentWeatherGFXState() end

--- gets the current weather queue.
-- @return weatherQueue table [startOffset]={State,GFX,IsPeriodic,Length,Forerun}
-- @return currentOffset
-- @return nextPeriodicOffset (sum of all periodic element length+forerun == weather loop length)
function CppLogic.Logic.GetWeatherQueue() end

--- clears the weather queue and adds an initial (periodic) first element.
-- @param state weatherstate
-- @param duration duration in sec > 5
-- @param forerun forerun in sec >0 <=10
-- @param gfx gfxindex >0
-- @param transition transition in sec >forerun <=200
function CppLogic.Logic.ClearWeatherQueueAndAddInitial(state, duration, forerun, gfx, transition) end

--- sets a func to be called, if a tasklist CppLogic::ContainsValue TASK_LUA_FUNC.
-- define the task as <Task classname="EGL::CTaskArgsInteger" classid="0xb3f8356d"><TaskType>TASK_LUA_FUNC</TaskType> in the tasklist to use it.
-- <Value> gets passed to your func, to identify what action should be executed.
-- use the parameter funcs moveto(pos, ignorepathing, noncancelable) and settl(tl) to move the entity or set the task list.
-- if you use moveto with ignorepathing, make sure the tasklist cannot be cancelled and that the target pos is actually not blocked.
-- return true, to call the function again next tick (not available if you have used moveto or settl).
-- do not save&use moveto and settl later, will cause crashes!
-- do not use moveto if you are repeating the call via return true.
-- does not work with SCELoader.
-- @param func (entityid, value, moveto(p,i,n), settl(tl))->repeat
function CppLogic.Logic.SetLuaTaskListFunc(func) end

--- changes all occurences of TASK_WAIT_FOR_ANIM to TASK_WAIT_FOR_ANIM_NON_CANCELABLE in the give tasklist.
-- this prevents new commands canceling the task.
-- does not work with SCELoader.
-- @param tl tasklist
-- @param tind task index (optional, default -1) id <0 full tasklist gets searched
function CppLogic.Logic.TaskListMakeWaitForAnimsUnCancelable(tl, tind) end
--- changes all occurences of TASK_WAIT_FOR_ANIM_NON_CANCELABLE to TASK_WAIT_FOR_ANIM in the give tasklist.
-- does not work with SCELoader.
-- @param tl tasklist
-- @param tind task index (optional, default -1) id <0 full tasklist gets searched
function CppLogic.Logic.TaskListMakeWaitForAnimsCancelable(tl, tind) end

--- prevents changes in tasklists to cancel TASK_WAIT_FOR_ANIM_NON_CANCELABLE. other states may still be cancelled.
-- does not work with SCELoader.
-- @param b bool (default false)
function CppLogic.Logic.TaskListSetChangeTaskListCheckUncancelable(b) end

--- enables/disables fixing movement tasks when a building is placed on top of a moving entity.
-- with the fix active, issues a new move task to the same position, instead of potentially skipping the move.
-- @param f enabled
function CppLogic.Logic.EnableBuildOnMovementFix(f) end

--- returns the next position where a building can be build accoring to blocking.
-- position can still be blocked by hostile settlers.
-- for buildon entitiy types, ignores p.r and returns the position+rotation of the nearest free base entity.
-- for non buildon entity types, checks for placement with p.r and returns rotation unchanged.
-- if no valid position can be found, returns {-1,-1,0}.
-- @param ety entitytype
-- @param p position with rotation
-- @param range area (optional, default logic.xml/BuildingPlacementSnapDistance)
-- @return position with rotation
function CppLogic.Logic.GetNearestFreePosForBuilding(ety, p, range) end

--- ui command callback.
-- func parameters are (eventId, eventData, writeback).
-- function can return true to skip further event execution.
-- call writeback(eventData) with a modified eventData to change event values.
-- writeback has the event as a upvalue, do not save it and call it later, you will override a random c stack position!
-- @param f func
function CppLogic.Logic.UICommands.SetCallback(f) end
--- ui command callback.
function CppLogic.Logic.UICommands.UnSetCallback() end

--- renders only a model without any entity or effect. (animations currently not supported).
-- automatically gets cleared when the userdata is gc'd / lua state gets closed (you have to recreate them after save load).
-- rendering happens regardless of local player FoW and camera pos, use it only when needed.
-- Transform options: Set=0 (removes any other), Multiply=1 (default), ReverseMultiply=2 (as if called with Multiply in opposite order)
-- usual order for transforms is: translate, scale, rotate.
-- remember to set the model before using transforms.
-- should not affect mp in any way (even if not synced), as for gamelogic is concerned, there is nothing there (not tested).
--- @class LogicModel
local LogicModel = {}
--- clears the model and all its data, removes the object from rendering.
function LogicModel:Clear() end
--- sets the model to display. resets transform to identity (position 0/0, no rotation and scale).
-- @param mid model to display
function LogicModel:SetModel(mid) end
--- translates (moves) the model (if it was not moved, it is at 0/0).
-- @param pos position for the center of the model to be
-- @param height (optional, default 0) height for the model
-- @param tro (optional, default Multiply) Transform Operation, see LogicModel
-- @param addTerraiHeight (optional, default true) if the terrain height at p should be added to height
-- @param floats (optional, default false) if true uses max of terrain and water height for addTerraiHeight
function LogicModel:Translate(pos, height, tro, addTerraiHeight, floats) end
--- scales the model.
-- scaling all 3 axis differently is currently not supported, but should be easy to implement. ask me if needed.
-- @param s scale
-- @param tro (optional, default Multiply) Transform Operation, see LogicModel
function LogicModel:Scale(s, tro) end
--- rotates the model.
-- curretly rotates in the usual way, so the feet stay always on the ground. other rotations should be possible, ask me if needed.
-- @param r rotation (in degrees)
-- @param tro (optional, default Multiply) Transform Operation, see LogicModel
function LogicModel:Rotate(r, tro) end
--- resets the transform of the model to identity (position 0/0, no rotation and scale).
function LogicModel:ResetTransform() end
--- sets the player color for this model.
-- @param pl player id
function LogicModel:SetColorByPlayer(pl) end
--- disables the shadow of a model. reset the model to reenable.
function LogicModel:DisableShadow() end
--- disables all particle effects (smoke...). reset the model to reenable.
function LogicModel:DisableParticleEffects() end
--- disables the terraindecal of a model. reset the model to reenable.
function LogicModel:DisableTerrainDecal() end
--- sets a color to modulate (multiply with texture).
-- @param r red, [0-255]
-- @param g green, [0-255]
-- @param b blue, [0-255]
-- @param a alpha, [0-255] (optional, default 255) (does not work with all models)
function LogicModel:SetColorModulate(r, g, b, a) end

--- creates a new LogicModel.
--- @return LogicModel
function CppLogic.Logic.CreateFreeModel(f) end

--- compiles a lua chunk.
-- asserts if kimichuras dlls are detected.
-- @param code to compile, sourcecode or binary
-- @return function to execute the compiled code, or error message
-- @return bool successful
function CppLogic.API.Eval(code) end

--- logs a string to the current settlers log.
-- @param str the string to log
function CppLogic.API.Log(str) end

--- creates a stack trace of the current executing lua functions.
-- @return string trace
function CppLogic.API.StackTrace() end

--- reads a file into a string.
-- can only read files in data\maps\externalmap\
-- @param file path to the file
-- @return file content
function CppLogic.API.ReadFileAsString(file) end

--- checks if a file exists.
-- @param file path to the file
-- @return bool
function CppLogic.API.DoesFileExist(file) end

--- compiles a lua chunk and executes it. shows the script in the debugger as file with the defined name.
-- asserts if kimichuras dlls are detected.
-- @param code to compile, sourcecode or binary
-- @param name name to show in debugger
-- @return return values of the executed code
function CppLogic.API.DoString(code, name) end

--- gets the path to a map. internal data or s5x file.
-- @param mapname filename of the map
-- @param typ type code
-- @param campname campagn name
-- @return path
function CppLogic.API.MapGetDataPath(mapname, typ, campname) end

--- returns the map a save is accociated with.
-- does not check, if the save is still valid, just if it exists.
-- @param save
-- @return mapname
-- @return typ
-- @return campname
-- @return GUID
function CppLogic.API.SaveGetMapInfo(save) end

--- returns the complete GDB as multilayered table.
-- keys are split at every \, and put into a layered map.
-- @return table
function CppLogic.API.GetGDB() end

--- saves a value in the mainmenu lua state (gets only removed on game restart, not on map load/start).
-- @param name string key
-- @param value data to store, only string/number/nil allowed
function CppLogic.API.RuntimeStoreSet(name, value) end
--- loads a value from the mainmenu lua state (gets only removed on game restart, not on map load/start).
-- @param name string key
-- @return value data
function CppLogic.API.RuntimeStoreGet(name) end

--- creates the global tables Animations, TerrainTypes, WaterTypes, ArmorClasses.
-- data gets read directly from the game, so anything you can use is in there.
function CppLogic.API.CreateExtraDataTables() end

--- returns debug information about a function.
-- @param f function func to check
-- @return table funcdata {name, namewhat, nups, short_src, linedefined, what}, see lua_getinfo for what actually gets returned
function CppLogic.API.GetFuncDebug(f) end

--- returns the main SHoK game thread (the thread that opened the lua state).
-- @return number threadid
function CppLogic.API.GetMainThreadID(f) end

--- returns the current running thread (this should always be the main thread, or the luadebugger thread).
-- @return number threadid
function CppLogic.API.GetCurrentThreadID(f) end

--- returns the current time as a single number.
-- useful for random seeds, use Framework.GetSystemTimeDateString if you want somwthing human readable.
-- @return number time
function CppLogic.API.GetCurrentTime(f) end

--- deals damage to a target.
-- calls respective hurt entity trigger.
-- @param target entity to be damaged
-- @param damage damage to deal
-- @param attacker entity dealing the damage (gets xp from kill, statistics...), (default 0)
-- @param attackerPlayerFallback player fallback for statistics, if attacker not set (default 0)
-- @param uiFeedback send battle feedback (default true)
-- @param xp give attacker xp for kills (default true)
-- @param addStat add kills to statistics (default true)
-- @param source what deals damage, used for triggers (default shok::AdvancedDealDamageSource.Script)
function CppLogic.Combat.DealDamage(target, damage, attacker, attackerPlayerFallback, uiFeedback, xp, addStat, source) end

--- deals damage in an area.
-- if attackerId (or player) are set, damages only targets hostile to them.
-- if dmgclass not set, it gets ignored in the damage calculation.
-- damage gets reduced, the furher away from impact the target is.
-- @param attackerId attacking entity (optional, 0 if not used)
-- @param x X coorinate of center
-- @param y Y coorinate of center
-- @param r range of damage dealt
-- @param dmg damage
-- @param player attacking player (optional, 0/nil if not used)
-- @param dmgclass damageclass (optional, 0/nil if not used)
-- @param uiFeedback send battle feedback (default true)
-- @param xp give attacker xp for kills (default true)
-- @param addStat add kills to statistics (default true)
-- @param source what deals damage, used for triggers (default shok::AdvancedDealDamageSource.Script)
function CppLogic.Combat.DealAoEDamage(attackerId, x, y, r, dmg, player, dmgclass, uiFeedback, xp, addStat, source) end

--- enables AoE projectile fix.
-- when enabled, cannons and similar AoE projectiles use the entitytypes damageclass.
function CppLogic.Combat.EnableAoEProjectileFix() end
--- disables AoE projectile fix.
-- when enabled, cannons and similar AoE projectiles use the entitytypes damageclass.
function CppLogic.Combat.DisableAoEProjectileFix() end

--- enables camoflage projectile fix.
-- when enabled, camoflage does not get reset by ranged attacks, also all attackers get cleared on entering camo.
function CppLogic.Combat.EnableCamoFix() end
--- disabes camoflage projectile fix.
-- when enabled, camoflage does not get reset by ranged attacks, also all attackers get cleared on entering camo.
function CppLogic.Combat.DisableCamoFix() end

--- iterates over all entities that match a predicate.
-- perfect to use with for loop.
-- examples:
-- - for id in CppLogic.Entity.EntityIterator(...) do Message(id) end  
-- - for id, rsqu, prio in CppLogic.Entity.EntityIterator(CppLogic.Entity.Predicates.InCircle(...), ...) do Message(id.."   "..r) end  
-- @param pred predicate userdata
-- @return nextEntity func
-- @return iteratorStatus
-- @return nil
function CppLogic.Entity.EntityIterator(...) end

--- iterates over all entities of one or more players that match a predicate.
-- cannot iterate over player 0 entities.
-- usually faster than EntityIterator and a player predicate.
-- you have to manually create an and predicate.
-- perfect to use with for loop.
-- examples:
-- - for id in CppLogic.Entity.EntityIterator(...) do Message(id) end  
-- - for id, rsqu, prio in CppLogic.Entity.PlayerEntityIterator(CppLogic.Entity.Predicates.InCircle(...), 1) do Message(id.."   "..r) end  
-- @param pred predicate userdata
-- @param ... players
-- @return nextEntity func
-- @return iteratorStatus
-- @return nil
function CppLogic.Entity.PlayerEntityIterator(pred, ...) end

--- gets all entities that match a predicate as a table.
-- @param pred predicate userdata
-- @return table of ids
function CppLogic.Entity.EntityIteratorTableize(...) end

--- counts all entities that match a predicate as a table.
-- @param pred predicate userdata
-- @return c
function CppLogic.Entity.EntityIteratorCount(...) end

--- returns the nearest entity. requires InCircle predicate.
-- @param pred predicate userdata
-- @return id
function CppLogic.Entity.EntityIteratorGetNearest(...) end

--- checks an entity agains a predicate.
-- @param id entity
-- @param pred predicate
-- @return bool
-- @return number range
-- @return number priority
function CppLogic.Entity.CheckPredicate(id, pred) end

--- creates a predicate that performs an and of multiple predicates.
-- @param ... predicates
-- @return predicate userdata
function CppLogic.Entity.Predicates.And(...) end

--- creates a predicate that performs an or of multiple predicates.
-- @param ... predicates
-- @return predicate userdata
function CppLogic.Entity.Predicates.Or(...) end

--- creates a predicate that performs an not of anoher predicate.
-- @param p predicate
-- @return predicate userdata
function CppLogic.Entity.Predicates.Not(p) end

--- creates a predicate that always matches, but sets the priority to prio if p matches it.
-- @param p predicate
-- @param prio priority
-- @return predicate userdata
function CppLogic.Entity.Predicates.SetPriority(p, prio) end

--- creates a predicate that checks for one entitytype.
-- @param etyp entitytype to check against
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfType(etyp) end

--- creates a predicate that checks for one player.
-- @param pl player to check against
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfPlayer(pl) end

--- creates a predicate that checks for an area.
-- when used in loop iterator, can also return the range squared for each matched entity as 2nd loop parameter (optional).
-- @param x x coordinate of center
-- @param y y coordinate of center
-- @param r radius of circle
-- @return predicate userdata
function CppLogic.Entity.Predicates.InCircle(x, y, r) end

--- creates a predicate that checks for building.
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsBuilding() end

--- creates a predicate that checks for settlers.
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsSettler() end

--- creates a predicate that checks for (player!=0 && (IsBuilding || IsSettler)).
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsCombatRelevant() end

--- creates a predicate that checks for anything that is not a soldier.
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsNotSoldier() end

--- creates a predicate that checks for an entitycategory.
-- @param ecat EntityCategory to check against
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfEntityCategory(ecat) end

--- creates a predicate that checks for a provided resource.
-- @param rty ResourceType to check against
-- @return predicate userdata
function CppLogic.Entity.Predicates.ProvidesResource(rty) end

--- creates a predicate that checks for any players.
-- @param ... players
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfAnyPlayer(...) end

--- creates a predicate that checks for any entitytypes.
-- @param ... entitytypes
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfAnyEntityType(...) end

--- creates a predicate that checks for an rectangular area.
-- order of coordinates does not matter, just have to be grouped as x,y,x,y.
-- @param x1 x coordinate 1
-- @param y1 y coordinate 1
-- @param x2 x coordinate 2
-- @param y2 y coordinate 2
-- @return predicate userdata
function CppLogic.Entity.Predicates.InRect(x1, y1, x2, y2) end

--- creates a predicate that checks for ari or thieves invisibility.
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsVisible() end

--- creates a predicate that checks for an upgrade category (settler or building, as defined in txpe xml).
-- @param ecat UpgradeCategory to check against
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfUpgradeCategory(ecat) end

--- creates a predicate that checks dead entities.
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsAlive() end

--- creates a predicate that checks settlers not inside a building. (includes entering and leaving).
-- includes all sorts of buildings, workplaces/farms/residences/barracks...
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsNotInBuilding() end

--- entity task list index.
-- @param id entity
-- @return index
function CppLogic.Entity.GetTaskListIndex(id) end
--- entity task list index.
-- @param id entity
-- @param i index
function CppLogic.Entity.SetTaskListIndex(id, i) end

--- moving entity speed factor (set via logic).
-- @param id entity
-- @return factor
function CppLogic.Entity.MovingEntityGetSpeedFactor(id) end

--- entity scale.
-- @param id entity
-- @return scale
function CppLogic.Entity.GetScale(id) end
--- entity scale.
-- @param id entity
-- @param s scale
function CppLogic.Entity.SetScale(id, s) end

--- entity check for soldier. (asserts if no entity).
-- @param id entity
-- @return is entity of soldier type
function CppLogic.Entity.IsSoldier(id) end

--- moving entity target pos (best to change directly after a move command).
-- @param id entity
-- @return position table, with optional rotation
function CppLogic.Entity.MovingEntityGetTargetPos(id) end
--- moving entity target pos (best to change directly after a move command).
-- @param id entity
-- @param p position table, with optional rotation
function CppLogic.Entity.MovingEntitySetTargetPos(id, p) end

--- entity exploration.
-- @param id entity
-- @return exploration
function CppLogic.Entity.GetExploration(id) end

--- entity speed.
-- @param id entity
-- @return speed
function CppLogic.Entity.GetSpeed(id) end

--- entity max range (including tech boni).
-- @param id entity
-- @return max range
function CppLogic.Entity.GetAutoAttackMaxRange(id) end

--- entity model, from model override or entitytype model 0.
-- @param id entity
-- @return model
function CppLogic.Entity.GetModel(id) end

--- checks if an entity is feared.
-- @param id entity
-- @return false or fearing entity
function CppLogic.Entity.IsFeared(id) end

--- clears all attackers of an entity.
-- @param id entity
function CppLogic.Entity.ClearAttackers(id) end

--- limited lifespan remaining seconds.
-- @param id entity
-- @return duration
function CppLogic.Entity.GetLimitedLifespanRemaining(id) end
--- limited lifespan remaining seconds.
-- @param id entity
-- @param t duration
function CppLogic.Entity.GetLimitedLifespanRemaining(id, t) end

--- replaces a tree with its ResourceTree equivalent.
-- @param id entity
-- @return new id or nil
function CppLogic.Entity.ReplaceWithResourceEntity(id) end

--- overrides an entities max hp.
-- does not work with SCELoader.
-- @param id entity
-- @param hp (optional, <0 to disable)
-- @param useBoni (optional)
function CppLogic.Entity.SetMaxHP(id, hp, useBoni) end

--- overrides an entities damage.
-- does not work for trapcannon.
-- does not work with SCELoader.
-- @param id entity
-- @param dmg, (<0 disable)
function CppLogic.Entity.SetDamage(id, dmg) end

--- overrides an entities armor.
-- does not work with SCELoader.
-- @param id entity
-- @param armor, (<0 disable)
function CppLogic.Entity.SetArmor(id, armor) end

--- overrides a settlers/buildings exploration.
-- use Logic for scriptentities.
-- does not work with SCELoader.
-- @param id entity
-- @param ex, (<0 disable)
function CppLogic.Entity.SetExploration(id, ex) end

--- overrides a settlers/autocannons max attack range.
-- does not work with SCELoader.
-- @param id entity
-- @param ran, (<0 disable)
function CppLogic.Entity.SetAutoAttackMaxRange(id, ran) end

--- overrides an entities display name.
-- does not work with SCELoader.
-- @param id entity
-- @param n, ("" disable)
function CppLogic.Entity.SetDisplayName(id, n) end
--- gets the display name of an entity.
-- @param id entity
-- @return name
function CppLogic.Entity.GetDisplayName(id) end

--- clones all overrides.
-- @param from entity (can be id of last destroyed entity)
-- @param to entity 
function CppLogic.Entity.CloneOverrideData(from, to) end

--- heals an entity. for leaders its own hp is healed, then troop hp if enabled.
-- @param id entity
-- @param heal
-- @param healTroopHp (optional, default false)
function CppLogic.Entity.PerformHeal(id, heal, healTroopHp) end

--- gets the leader of a soldier.
-- @param id id of the solder
-- @return id of the leader
function CppLogic.Entity.Settler.GetLeaderOfSoldier(id) end

--- base unmodified movementspeed of a settler.
-- @param id settler
-- @return movement speed
-- @return rotation speed
function CppLogic.Entity.Settler.GetBaseMovementSpeed(id) end
--- base unmodified movementspeed of a settler.
-- @param id settler
-- @param move optional new move speed
-- @param rotate optional new rotation speed
function CppLogic.Entity.Settler.SetBaseMovementSpeed(id, move, rotate) end

--- settler current work time.
-- @param id entity
-- @return work time
function CppLogic.Entity.Settler.WorkerGetCurrentWorkTime(id) end
--- settler current work time.
-- @param id entity
-- @param wt work time
function CppLogic.Entity.Settler.WorkerSetCurrentWorkTime(id, wt) end

--- settler overhead widget.
-- 0->only name, 1->name+bar(anything), 2->worker, 3->name+bar(leader), 4->nothing.
-- @param id settler
-- @return widget code
function CppLogic.Entity.Settler.GetOverheadWidget(id) end
--- settler overhead widget.
-- 0->only name, 1->name+bar(anything), 2->worker, 3->name+bar(leader), 4->nothing.
-- @param id settler
-- @param wid widget code
function CppLogic.Entity.Settler.SetOverheadWidget(id, wid) end

--- camouflage duration left (ari: seconds, thief: invis ? 15 : 0).
-- @param id entity
-- @return duration
function CppLogic.Entity.Settler.HeroGetCamouflageDurationLeft(id) end
--- camouflage duration left (only ari).
-- @param id entity
-- @param dur duration
function CppLogic.Entity.Settler.HeroSetCamouflageDurationLeft(id, dur) end

--- time to thief camouflage.
-- @param id entity
-- @return duration
function CppLogic.Entity.Settler.ThiefGetCamouflageTimeTo(id) end
--- time to thief camouflage.
-- @param id entity
-- @param dur duration
function CppLogic.Entity.Settler.ThiefSetCamouflageTimeTo(id, dur) end

--- hero resurrection time.
-- @param id entity
-- @return duration
function CppLogic.Entity.Settler.HeroGetResurrectionTime(id) end
--- hero resurrection time.
-- @param id entity
-- @param t duration
function CppLogic.Entity.Settler.HeroSetResurrectionTime(id, t) end

--- resurrect a hero.
-- @param id entity
function CppLogic.Entity.Settler.HeroResurrect(id) end

--- set thief stolen resources.
-- @param id entity
-- @param ty res type or 0
-- @param am amount (optional)
function CppLogic.Entity.Settler.ThiefSetStolenResourceInfo(id, ty, am) end

--- checks visibility of an entity.
-- @param id entity
function CppLogic.Entity.Settler.IsVisible(id) end

--- checks if a settler is idle
-- this is in defend or hold position command, or having the type specific idle tasklist.
-- workers are never idle.
-- @param id entity
function CppLogic.Entity.Settler.IsIdle(id) end

--- command to send darios hawk.
-- decreases hawk range automatically, asserts if ability cannot be used.
-- @param id entity
-- @param p target
function CppLogic.Entity.Settler.CommandSendHawk(id, p) end

--- command to inflict fear.
-- asserts if ability cannot be used.
-- @param id entity
function CppLogic.Entity.Settler.CommandInflictFear(id) end

--- command to place bomb.
-- asserts if ability cannot be used.
-- @param id entity
-- @param p target
function CppLogic.Entity.Settler.CommandPlaceBomb(id, p) end

--- command to inflict fear.
-- asserts if ability cannot be used, checks position.
-- does not check bottom & top entitytype.
-- @param id entity
-- @param p target
-- @param bottom foundation type
-- @param top cannon type
function CppLogic.Entity.Settler.CommandPlaceCannon(id, p, bottom, top) end

--- command to activate ranged effect.
-- asserts if ability cannot be used.
-- @param id entity
function CppLogic.Entity.Settler.CommandRangedEffect(id) end

--- command to perform circular attack.
-- asserts if ability cannot be used.
-- @param id entity
function CppLogic.Entity.Settler.CommandCircularAttack(id) end

--- command to summon.
-- asserts if ability cannot be used.
-- @param id entity
-- @return ids of summoned entities
function CppLogic.Entity.Settler.CommandSummon(id) end

--- command to activate camoflage.
-- asserts if ability cannot be used.
-- @param id entity
function CppLogic.Entity.Settler.CommandActivateCamoflage(id) end

--- command to convert.
-- moves into range, asserts if ability cannot be used.
-- @param id entity
-- @param tid target
function CppLogic.Entity.Settler.CommandConvert(id, tid) end

--- command to snipe.
-- asserts if ability cannot be used, including out of range.
-- @param id entity
-- @param tid target
function CppLogic.Entity.Settler.CommandSnipe(id, tid) end

--- command to use shuriken.
-- asserts if ability cannot be used, including out of range.
-- @param id entity
-- @param tid target
function CppLogic.Entity.Settler.CommandShuriken(id, tid) end

--- command to motivate workers.
-- asserts if ability cannot be used, including out of range.
-- @param id entity
function CppLogic.Entity.Settler.CommandMotivateWorkers(id) end

--- command to use sabotage.
-- asserts if ability cannot be used. does not check tech requirement.
-- @param id entity
-- @param tid target
function CppLogic.Entity.Settler.CommandSabotage(id, tid) end

--- command to defuse a sabotage.
-- asserts if ability cannot be used. does not check tech requirement.
-- @param id entity
-- @param tid target
function CppLogic.Entity.Settler.CommandDefuse(id, tid) end

--- command to use binoculars.
-- asserts if ability cannot be used.
-- @param id entity
-- @param p target
function CppLogic.Entity.Settler.CommandBinocular(id, p) end

--- command to place a torch.
-- asserts if ability cannot be used. does not check tech requirement.
-- @param id entity
-- @param p target
function CppLogic.Entity.Settler.CommandPlaceTorch(id, p) end

--- command to search resources.
-- asserts if ability cannot be used. does not check tech requirement.
-- @param id entity
function CppLogic.Entity.Settler.CommandPointToRes(id) end

--- command to steal.
-- asserts if ability cannot be used.
-- @param id entity
-- @param tid target
function CppLogic.Entity.Settler.CommandStealFrom(id, tid) end

--- command to secure stolen goods.
-- asserts if ability cannot be used.
-- @param id entity
-- @param tid target
function CppLogic.Entity.Settler.CommandSecureGoods(id, tid) end

--- command to move.
-- asserts if ability cannot be used.
-- @param id entity
-- @param pos target
-- @param r target rotation (optional, nil if not used)
function CppLogic.Entity.Settler.CommandMove(id, pos, r) end

--- command to construct a building.
-- asserts if ability cannot be used.
-- @param id entity
-- @param tid target
-- @return bool if a slot was free
function CppLogic.Entity.Settler.CommandSerfConstructBuilding(id, tid) end

--- command to construct a building.
-- asserts if ability cannot be used.
-- @param id entity
-- @param tid target
-- @return bool if a slot was free
function CppLogic.Entity.Settler.CommandSerfRepairBuilding(id, tid) end

--- command to extract a resource.
-- automatically tries to convert trees to ResourceTrees, asserts if no resource doodad afterwards.
-- @param id entity
-- @param tid target
-- @return resource entity id (might be different than tid)
function CppLogic.Entity.Settler.CommandSerfExtract(id, tid) end

--- enables conversion hook. gets called twice, before and after conversion.
-- the first created entity after pre call is the new converted leader.
-- @param func function(targetId, player, isPost, converterId)
function CppLogic.Entity.Settler.EnableConversionHook(func) end
--- disables conversion hook.
function CppLogic.Entity.Settler.DisableConversionHook() end

--- command to expell a settler.
-- @param id entity
function CppLogic.Entity.Settler.CommandExpell(id) end

--- command to turn a serf to a battleserf.
-- @param id entity
function CppLogic.Entity.Settler.CommandTurnSerfToBattleSerf(id) end
--- command to turn a battleserf back to a serf.
-- @param id entity
function CppLogic.Entity.Settler.CommandTurnBattleSerfToSerf(id) end

--- teleports a settler to a position. does not change id.
-- @param id entity
-- @param p target pos
function CppLogic.Entity.Settler.SetPosition(id, p) end

--- enables ranged effect advanced healing. heal gets transfered from solder to leader, first healing the leader, and if something is left, the troop hp pool.
-- does not work with SCELoader.
-- @param enabl bool
function CppLogic.Entity.Settler.EnableRangedEffectSoldierHeal(enabl) end

--- gets the target of a heros shuriken ability.
-- @param id
-- @return tid
function CppLogic.Entity.Settler.ShurikenGetTarget(id) end

--- gets the target of a heros sniper ability.
-- @param id
-- @return tid
function CppLogic.Entity.Settler.SniperGetTarget(id) end

--- gets the building a settler is currently inside, and leaving.
-- @param id
-- @return buildingIn
-- @return buildingLeaving
function CppLogic.Entity.Settler.GetEnteredBuilding(id) end

--- sets a special task list (TL_SCRIPT_ANIMATION) to play an animation on this entity.
-- after the animation is done, executes TASK_BATTLE_WAIT_UNTIL to reset animation. (this may block returning to your normal tl for a few seconds).
-- the tasklist TL_SCRIPT_ANIMATION gets created at first use, it may not be there before you call this func.
-- requires call to CppLogic.OnLeaveMap() to not crash on map exit (TriggerFixCppLogicExtension does this for you).
-- does not work with SCELoader.
-- @param id
-- @param animname string name of the animation (can be copied from entity xml) does not check if the anim matches the model!
-- @param backwards bool play animation backwards, (optional, default false)
-- @param speed number speed factor (optional, default 1)
function CppLogic.Entity.Settler.PlayScriptAnimation(id, animname, backwards, speed) end

--- a leaders experience.
-- @param id leader
-- @return xp
function CppLogic.Entity.Leader.GetExperience(id) end
--- a leaders experience
-- @param id leader
-- @param xp xp
function CppLogic.Entity.Leader.SetExperience(id, xp) end

--- attaches a soldier to a leader.
-- @param leader
-- @param soldier
function CppLogic.Entity.Leader.AttachSoldier(leader, soldier) end

--- a leaders troop health (hp of all soldiers summed up).
-- -1 when not yet set (calculate in lua as all soldiers at full hp).
-- @param id leader
-- @return hp
function CppLogic.Entity.Leader.GetTroopHealth(id) end
--- a leaders troop health (does not kill soldiers when decreasing).
-- @param id leader
-- @param hp hp
function CppLogic.Entity.Leader.SetTroopHealth(id, hp) end

--- a leaders troop limit.
-- @param id leader
-- @param limit limit
function CppLogic.Entity.Leader.SetSoldierLimit(id, limit) end

--- a leaders regeneration.
-- does not work with SCELoader.
-- @param id leader
-- @return hp regenerated
-- @return seconds between refreshes
function CppLogic.Entity.Leader.GetRegeneration(id) end
--- a leaders regeneration.
-- does not work with SCELoader.
-- @param id leader
-- @param hp regenerated
-- @param seconds between refreshes
function CppLogic.Entity.Leader.SetRegeneration(id, hp, seconds) end

--- building height (& construction progress).
-- @param id entity
-- @return height
function CppLogic.Entity.Building.GetHeight(id) end
--- building height (& construction progress).
-- @param id entity
-- @param h height
function CppLogic.Entity.Building.SetHeight(id, h) end

--- barracks autofill status.
-- @param id barracks entity
-- @return bool active
function CppLogic.Entity.Building.GetBarracksAutoFillActive(id) end

--- nearest construction slot.
-- @param build building
-- @param p position to measure from
-- @return slot index, -1 if nothing free
function CppLogic.Entity.Building.GetNearestFreeConstructionSlotFor(build, p) end

--- nearest repair slot.
-- @param build building
-- @param p position to measure from
-- @return slot index, -1 if nothing free
function CppLogic.Entity.Building.GetNearestFreeRepairSlotFor(build, p) end

--- market trade data.
-- progress is buy amount + sell amount
-- @param id entity
-- @return buy type
-- @return sell type
-- @return buy amount
-- @return sell amount
-- @return progress amount
function CppLogic.Entity.Building.MarketGetCurrentTradeData(id) end
--- market trade data.
-- progress is buy amount + sell amount
-- @param id entity
-- @param bty buy type (optional)
-- @param sty sell type (optional)
-- @param bam buy amount (optional)
-- @param sam sell amount (optional)
-- @param pam progress amount (optional)
function CppLogic.Entity.Building.MarketSetCurrentTradeData(id, bty, sty, bam, sam, pam) end

--- starts building a cannon.
-- uses resources, asserts if it cannot build.
-- @param id foundry entity
-- @param ty cannon type
function CppLogic.Entity.Building.CommandFoundryBuildCannon(id, ty) end

--- starts upgrading a building. does not cost resources.
-- @param id foundry entity
function CppLogic.Entity.Building.StartUpgrade(id) end
--- cancels an upgrade currently in progress. does not refund resources.
-- @param id foundry entity
function CppLogic.Entity.Building.CancelUpgrade(id) end

--- checks if a building is idle.
-- this is not under constrcution, upgrading, researching, in alarm, recruiting or trading.
-- @param id entity
-- @return bool idle status
function CppLogic.Entity.Building.IsIdle(id) end

--- gets leaders currently getting trainet at a barracks.
-- @param id entity
-- @return entities
function CppLogic.Entity.Building.BarracksGetLeadersTrainingAt(id) end

--- gets the entitytype of the cannon in construction.
-- 0 if no entitytype, gets reset when cannon spawns.
-- @param id entity
-- @return entitytype
function CppLogic.Entity.Building.FoundryGetCannonTypeInConstruction(id) end

--- buys a soldier for a leader at a specific barracks.
-- uses resources.
-- @param bid barracks
-- @param lid leader
-- @param noChecktype (optional, default false) set to true to skip the check for matching barracks type.
-- @return entitytype
function CppLogic.Entity.Building.BarracksBuySoldierForLeader(bid, lid, noChecktype) end

--- activates overtime.
-- checks for cooldown.
-- @param id entity
function CppLogic.Entity.Building.ActivateOvertime(id) end
--- deactivates overtime and sets overtime cooldown.
-- @param id entity
function CppLogic.Entity.Building.DeactivateOvertime(id) end

--- sets a barrack to recruit full groups.
-- @param id entity
function CppLogic.Entity.Building.BarracksRecruitGroups(id) end
--- sets a barrack to recruit only leaders.
-- @param id entity
function CppLogic.Entity.Building.BarracksRecruitLeaders(id) end

--- buys a serf at a hq.
-- uses resources.
-- @param id entity
function CppLogic.Entity.Building.HQBuySerf(id) end

--- sells a building.
-- does not give you back resources.
-- @param id entity
function CppLogic.Entity.Building.SellBuilding(id) end

--- starts researching a tech.
-- does not check, if the tech belongs to that building.
-- uses resources.
-- @param id entity
-- @param tech
function CppLogic.Entity.Building.StartResearch(id, tech) end
--- cancels research.
-- gives back resources.
-- @param id entity
function CppLogic.Entity.Building.CancelResearch(id) end

--- starts a market transaction.
-- uses resources.
-- @param id entity
-- @param sellty
-- @param buyty
-- @param buyam
function CppLogic.Entity.Building.MarketStartTrade(id, sellty, buyty, buyam) end
--- cancels a market transaction.
-- gives back resources.
-- @param id entity
function CppLogic.Entity.Building.MarketCancelTrade(id) end

--- removes the last offer in a mercenary or techtrader tent.
-- @param id entity
function CppLogic.Entity.Building.MercenaryRemoveLastOffer(id) end
--- sets the remaining units and cost in a mercenary or techtrader tent.
-- @param id entity
-- @param ind index
-- @param remain offers remaining (optional, default no change)
-- @param cost cost table (optional, default no change)
-- @param ignoreZeroes ignore anything that is zero (optional, default false)
function CppLogic.Entity.Building.MercenarySetOfferData(id, ind, remain, cost, ignoreZeroes) end

--- gets the BuildOn entity this building is constructed on (mine/VC/bridge).
-- @param id entity
-- @return id or 0
function CppLogic.Entity.Building.GetBuildOnEntity(id) end
--- gets the building constructed on a BuildOn entity (mine/VC/bridge).
-- @param id entity
-- @return id or 0
function CppLogic.Entity.Building.BuildOnEntityGetBuilding(id) end

--- gets the constructonsite of a building.
-- @param id entity
-- @return id or 0
function CppLogic.Entity.Building.GetConstructionSite(id) end
--- gets building of a constructionsite.
-- @param id entity
-- @return id or 0
function CppLogic.Entity.Building.ConstructionSiteGetBuilding(id) end

--- buys a leader in a barracks by entitytype.
-- uses resources, asserts if not possible.
-- @param id barracks
-- @param ety leader type
-- @param checkType check if leader type can be recruited at this barracks upgradecategory (optional, default false)
-- @return id
function CppLogic.Entity.Building.BarracksBuyLeaderByType(id, ety, checkType) end


--- entity type max health.
-- @param ty entitytype
-- @return max health
function CppLogic.EntityType.GetMaxHealth(ty) end
--- entity type max health.
-- @param ty entitytype
-- @param h max health
function CppLogic.EntityType.SetMaxHealth(ty, h) end

--- entity type motivation effect.
-- @param ty entitytype
-- @return motivation effect
function CppLogic.EntityType.GetAffectMotivation(ty) end
--- entity type motivation effect.
-- @param ty entitytype
-- @param m motivation effect
function CppLogic.EntityType.SetAffectMotivation(ty, m) end

--- entity type suspended animation.
-- @param ty entitytype
-- @return anim
function CppLogic.EntityType.GetSuspendedAnimation(ty) end
--- entity type suspended animation.
-- @param ty entitytype
-- @param a anim
function CppLogic.EntityType.GetSuspendedAnimation(ty, a) end

--- limited lifespan duration seconds.
-- @param ty entitytype
-- @return duration
function CppLogic.EntityType.GetLimitedLifespanDuration(ty) end
--- limited lifespan duration seconds.
-- @param ty entitytype
-- @param t duration
function CppLogic.EntityType.SetLimitedLifespanDuration(ty, t) end

--- damage and damageclass of autoattacks of an entity type.
-- @param ty entitytype
-- @return damage amount
-- @return damageclass
function CppLogic.EntityType.GetAutoAttackDamage(ty) end
--- damage and damageclass of autoattacks of an entity type.
-- @param ty entitytype
-- @param dmg damage amount
-- @param dc damageclass
function CppLogic.EntityType.SetAutoAttackDamage(ty, dmg, dc) end

--- adds an entity category to an entitytype.
-- can be tested for with logic/predicate, does not need to be from predefined EntityCategories.
-- @param ty entitytype
-- @param ecat entitycategory
function CppLogic.EntityType.AddEntityCategory(ty, ecat) end
--- removes an entity category from an entitytype.
-- can be tested for with logic/predicate, does not need to be from predefined EntityCategories.
-- @param ty entitytype
-- @param ecat entitycategory
function CppLogic.EntityType.RemoveEntityCategory(ty, ecat) end

--- miss chance of autoattacks of an entity type (no autocannon).
-- @param ty entitytype
-- @return miss chance
function CppLogic.EntityType.GetAutoAttackMissChance(ty) end
--- miss chance of autoattacks of an entity type (no autocannon).
-- @param ty entitytype
-- @param mc miss chance
function CppLogic.EntityType.SetAutoAttackMissChance(ty, mc) end

--- range chance of autoattacks of an entity type.
-- for autocannons only max range, for anything else max and min range.
-- @param ty entitytype
-- @return max range
-- @return min range (nil for autocannons)
function CppLogic.EntityType.GetAutoAttackRange(ty) end
--- range chance of autoattacks of an entity type.
-- for autocannons only max range, for anything else max and min range.
-- @param ty entitytype
-- @param maxrange range (optional)
-- @param minrange range (ignored for autocannons) (optional)
function CppLogic.EntityType.SetAutoAttackRange(ty, maxrange, minrange) end

--- settler or building type armor and armorclass.
-- @param ty entitytype
-- @return armor
-- @return armorclass
function CppLogic.EntityType.GetArmor(ty) end
--- settler or building type armor and armorclass.
-- @param ty entitytype
-- @param ar armor
-- @param acl armorclass
function CppLogic.EntityType.SetArmor(ty, ar, acl) end

--- settler or building type armor modifier techs.
-- @param ty entitytype
-- @return array of techs
function CppLogic.EntityType.GetArmorModifierTechs(ty) end

--- entity type models.
-- most entitytypes use only one model, but some use the others as well.
-- @param ty entitytype
-- @return model 0
-- @return model 1
-- @return model 2
-- @return model 3
function CppLogic.EntityType.GetModels(ty) end
--- entity type models.
-- most entitytypes use only one model, but some use the others as well.
-- @param ty entitytype
-- @param m0 model 0 (optional)
-- @param m1 model 1 (optional)
-- @param m2 model 2 (optional)
-- @param m3 model 3 (optional)
function CppLogic.EntityType.SetModels(ty, m0, m1, m2, m3) end

--- checks it a entitytype can be replaced with a resourcetree.
-- @param ty entitytype
-- @return bool
function CppLogic.EntityType.ResourceTreeTypeHasData(ty) end
--- tree data to replace a tree with a resourcetree.
-- @param ty entitytype
-- @return resource entitytype
-- @return resource amount
function CppLogic.EntityType.ResourceTreeTypeGetData(ty) end
--- tree data to replace a tree with a resourcetree.
-- @param ty entitytype
-- @param rety resource entitytype
-- @param ram resource amount
function CppLogic.EntityType.ResourceTreeTypeSetData(ty, rety, ram) end

--- blocking data of an entitytype.
-- @param ty entitytype
-- @return blocking table (array of {pos,pos})
-- @return num blocked points (number)
-- @return buildblock ({pos,pos} or nil)
function CppLogic.EntityType.GetBlocking(ty) end

--- settler or building type exploration modifier techs.
-- @param ty entitytype
-- @return array of techs
function CppLogic.EntityType.GetExplorationModifierTechs(ty) end

--- DeleteWhenBuiltOn flag of an entitytype.
-- @param ty entitytype
-- @return flag
function CppLogic.EntityType.GetDeleteWhenBuildOn(ty) end
--- DeleteWhenBuiltOn flag of an entitytype.
-- @param ty entitytype
-- @param del flag
function CppLogic.EntityType.SetDeleteWhenBuildOn(ty, del) end

--- battlewaituntil (battle behavior) or reloadtime (autocannon behavior) of an entitytype.
-- @param ty entitytype
-- @return bwu
function CppLogic.EntityType.GetBattleWaitUntil(ty) end
--- battlewaituntil (battle behavior) or reloadtime (autocannon behavior) of an entitytype.
-- @param ty entitytype
-- @param bwu battlewaituntil
function CppLogic.EntityType.SetBattleWaitUntil(ty, bwu) end

--- the soldier type of a leader type
-- @param ty leader type
-- @return soldier type
function CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(ty) end
--- the soldier type of a leader type
-- @param leaderTy leader type
-- @param soldierTy soldier type
function CppLogic.EntityType.Settler.LeaderTypeSetSoldierType(leaderTy, soldierTy) end

--- leader type upkeep per payday.
-- @param ty entitytype
-- @return upkeep
function CppLogic.EntityType.Settler.LeaderTypeGetUpkeep(ty) end
--- leader type upkeep per payday.
-- @param ty entitytype
-- @param u upkeep
function CppLogic.EntityType.Settler.LeaderTypeSetUpkeep(ty, u) end

--- leader type regeneration.
-- @param ty entitytype
-- @return regen health
-- @return regen seconds
function CppLogic.EntityType.Settler.LeaderTypeGetRegeneration(ty) end
--- leader type regeneration.
-- @param ty entitytype
-- @param hp regen health (optional)
-- @param t regen seconds (optional)
function CppLogic.EntityType.Settler.LeaderTypeSetRegeneration(ty, hp, t) end

--- range in which enemies get attacked by this leader type.
-- @param ty entitytype
-- @return range
function CppLogic.EntityType.Settler.LeaderTypeGetAutoAggressiveRange(ty) end
--- range in which enemies get attacked by this leader type.
-- @param ty entitytype
-- @param r range
function CppLogic.EntityType.Settler.LeaderTypeSetAutoAggressiveRange(ty, r) end

--- stealing time and resource amounts of a thief type.
-- @param ty entitytype
-- @return seconds needed
-- @return max resource
-- @return min resource
function CppLogic.EntityType.Settler.ThiefTypeGetStealingInfo(ty) end
--- stealing time and resource amounts of a thief type.
-- @param ty entitytype
-- @param sec seconds needed
-- @param maxS max resource
-- @param minS min resource
function CppLogic.EntityType.Settler.ThiefTypeSetStealingInfo(ty, sec, maxS, minS) end

--- entitytytpe camouflage ability data.
-- for ari duration is invisibility after activation, for thieves its time to invisibility after discovery.
-- @param ty entitytype
-- @return duration
-- @return discovery range
-- @return recharge time
function CppLogic.EntityType.Settler.GetAbilityDataCamouflage(ty) end
--- entitytytpe camouflage ability data.
-- for ari duration is invisibility after activation, for thieves its time to invisibility after discovery.
-- @param ty entitytype
-- @param dur duration
-- @param dran discovery range
-- @param rech recharge time
function CppLogic.EntityType.Settler.SetAbilityDataCamouflage(ty, dur, dran, rech) end

--- entitytytpe circular attack ability data.
-- @param ty entitytype
-- @return damage
-- @return damage class
-- @return range
-- @return recharge time
function CppLogic.EntityType.Settler.GetAbilityDataCircularAttack(ty) end
--- entitytytpe circular attack ability data.
-- @param ty entitytype
-- @param dmg damage
-- @param dcl damage class
-- @param ra range
-- @param rech recharge time
function CppLogic.EntityType.Settler.SetAbilityDataCircularAttack(ty, dmg, dcl, ra, rech) end

--- entitytytpe shuriken ability data.
-- @param ty entitytype
-- @return damage
-- @return damage class
-- @return number shuriken
-- @return range
-- @return max arc degree
-- @return recharge time
-- @return effectTypeID
function CppLogic.EntityType.Settler.GetAbilityDataShuriken(ty) end
--- entitytytpe shuriken ability data.
-- @param ty entitytype
-- @param dmg damage
-- @param dcl damage class
-- @param numSh number shuriken
-- @param ran range
-- @param mArc max arc degree
-- @param rech recharge time
function CppLogic.EntityType.Settler.SetAbilityDataShuriken(ty, dmg, dcl, numSh, ran, mArc, rech) end

--- entitytytpe sniper ability data.
-- damage is facto * max hp of target.
-- @param ty entitytype
-- @return damage factor
-- @return range
-- @return recharge time
function CppLogic.EntityType.Settler.GetAbilityDataSniper(ty) end
--- entitytytpe sniper ability data.
-- damage is facto * max hp of target.
-- @param ty entitytype
-- @param dfac damage factor
-- @param ran range
-- @param rech recharge time
function CppLogic.EntityType.Settler.SetAbilityDataSniper(ty, dfac, ran, rech) end

--- entitytytpe sniper ability data.
-- damage is facto * max hp of target.
-- @param ty entitytype
-- @return damage factor
-- @return armor factor
-- @return health recover factor
-- @return range
-- @return recharge time
function CppLogic.EntityType.Settler.GetAbilityDataRangedEffect(ty) end
--- entitytytpe sniper ability data.
-- damage is facto * max hp of target.
-- @param ty entitytype
-- @param dmgfac damage factor
-- @param armfac armor factor
-- @param hpfac health recover factor
-- @param ran range
-- @param rech recharge time
function CppLogic.EntityType.Settler.SetAbilityDataRangedEffect(ty, dmgfac, armfac, hpfac, ran, rech) end

--- settler type fearless.
-- @param ty entitytype
-- @return fearless
function CppLogic.EntityType.Settler.GetFearless(ty) end
--- settler type fearless.
-- @param ty entitytype
-- @param fl fearless
function CppLogic.EntityType.Settler.SetFearless(ty, fl) end

--- settler type cost.
-- @param ty entitytype
-- @return cost info table
function CppLogic.EntityType.Settler.GetCost(ty) end
--- settler type cost.
-- @param ty entitytype
-- @param c cost info table
-- @param ignoreZeroes should zeroes get ignored (optional)
function CppLogic.EntityType.Settler.SetCost(ty, c, ignoreZeroes) end

--- settler or building type damage modifier techs.
-- @param ty entitytype
-- @return array of techs
function CppLogic.EntityType.Settler.GetDamageModifierTechs(ty) end

--- settler or building type max range modifier techs.
-- @param ty entitytype
-- @return array of techs
function CppLogic.EntityType.Settler.GetMaxRangeModifierTechs(ty) end

--- settler or building type speed modifier techs.
-- @param ty entitytype
-- @return array of techs
function CppLogic.EntityType.Settler.GetSpeedModifierTechs(ty) end

--- leader type initial max number of soldiers.
-- @param ty entitytype
-- @return numSol
function CppLogic.EntityType.Settler.LeaderTypeGetMaxNumberOfSoldiers(ty) end

--- gets the first upgradecategory a settler type is in.
-- @param ty entitytype
-- @return ucat
function CppLogic.EntityType.Settler.GetUpgradeCategory(ty) end

--- number of settlers this building (villagecenter) supports.
-- @param ty entitytype
-- @return slots
function CppLogic.EntityType.Building.GetVCAttractionSlotsProvided(ty) end
--- number of settlers this building (villagecenter) supports.
-- @param ty entitytype
-- @param s slots
function CppLogic.EntityType.Building.SetVCAttractionSlotsProvided(ty, s) end

--- building type construction cost.
-- @param ty entitytype
-- @return cost info table
function CppLogic.EntityType.Building.GetConstructionCost(ty) end
--- building type construction cost.
-- @param ty entitytype
-- @param c cost info table
-- @param ignoreZeroes should zeroes get ignored (optional)
function CppLogic.EntityType.Building.SetConstructionCost(ty, c, ignoreZeroes) end

--- building type upgrade cost.
-- @param ty entitytype
-- @return cost info table
function CppLogic.EntityType.Building.GetUpradeCost(ty) end
--- building type upgrade cost.
-- @param ty entitytype
-- @param c cost info table
-- @param ignoreZeroes should zeroes get ignored (optional)
function CppLogic.EntityType.Building.SetUpradeCost(ty, c, ignoreZeroes) end

--- adds a tech modifier for building hp.
-- does not work with SCELoader.
-- has no effect without EnableMaxHPTechMod.
-- @param ty entitytype
-- @param tech
function CppLogic.EntityType.Building.AddHPTechMod(ty, tech) end

--- gets the BuildOnTypes of a building type.
-- @param ty entitytype
-- @return array of types (may be empty table)
function CppLogic.EntityType.Building.GetBuildOnTypes(ty) end

--- tech research time and costs.
-- @param tid tech id
-- @return research time needed
-- @return cost info
function CppLogic.Technology.GetResearchInfo(tid) end
--- tech research time and costs.
-- @param tid tech id
-- @param time research time needed (optional)
-- @param cost cost info (optional)
function CppLogic.Technology.SetResearchInfo(tid, time, cost) end

--- tech requirements.
-- @param tid tech id
-- @return num required entity cond (0 -> all)
-- @return entity conditions (table [entitytypeId] = amount)
-- @return num required tech cond (0 -> all)
-- @return tech conditions (array of tech ids)
-- @return num required ucat cond (0 -> all)
-- @return ucat conditions (table [ucatId] = amount)
function CppLogic.Technology.GetRequirements(tid) end

--- technology exploration modifier.
-- @param tid tech id
-- @return operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
-- @return value
function CppLogic.Technology.GetExplorationModifier(tid) end
--- technology armor modifier.
-- @param tid tech id
-- @return operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
-- @return value
function CppLogic.Technology.GetArmorModifier(tid) end
--- technology damage modifier.
-- @param tid tech id
-- @return operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
-- @return value
function CppLogic.Technology.GetDamageModifier(tid) end
--- technology range modifier.
-- @param tid tech id
-- @return operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
-- @return value
function CppLogic.Technology.GetRangeModifier(tid) end
--- technology speed modifier.
-- @param tid tech id
-- @return operator ( + -> 43, - -> 45, * -> 42, / -> 47, # (off) -> 35)
-- @return value
function CppLogic.Technology.GetSpeedModifier(tid) end

--- adds a construction time modifier to a technology.
-- default modifier is 1.0, smaller modifiers lead to faster construction.
-- @param tech tech
-- @param value modification value
-- @param op string operation to perform, accepted is "+" "-" "*" "/"
function CppLogic.Technology.TechAddConstructionSpeedModifier(tech, value, op) end

--- gets a widgets position and size.
-- @param wid widget
-- @return x
-- @return y
-- @return w
-- @return h
function CppLogic.UI.WidgetGetPositionAndSize(wid) end
--- sets a widgets position and size as floats (so you can set fractional coordinates).
-- @param wid widget
-- @param x (optional, default current)
-- @param y (optional, default current)
-- @param w (optional, default current)
-- @param h (optional, default current)
function CppLogic.UI.WidgetSetPositionAndSize(wid, x, y, w, h) end

--- gets a widgets updatemanual flag.
-- @param wid widget
-- @return bool
function CppLogic.UI.WidgetGetUpdateManualFlag(wid) end
--- sets a widgets updatemanual flag.
-- @param wid widget
-- @param bool
function CppLogic.UI.WidgetSetUpdateManualFlag(wid, f) end

--- gets a widgets update func.
-- @param wid widget
-- @return command string
-- @return func or err msg
function CppLogic.UI.WidgetGetUpdateFunc(wid) end
--- calls a buttons update func.
-- @param wid widget
function CppLogic.UI.WidgetCallUpdateFunc(wid) end
--- overrides a buttons update func to a lua func that gets called without any arguments.
-- @param wid widget
-- @param f func
function CppLogic.UI.WidgetOverrideUpdateFunc(wid, f) end

--- gets a buttons action func.
-- @param wid widget
-- @return command string
-- @return func or err msg
function CppLogic.UI.ButtonGetActionFunc(wid) end
--- calls a buttons action func.
-- @param wid widget
function CppLogic.UI.ButtonCallActionFunc(wid) end
--- overrides a buttons action func to a lua func that gets called without any arguments.
-- @param wid widget
-- @param f func
function CppLogic.UI.ButtonOverrideActionFunc(wid, f) end

--- gets a widgets tooltip func.
-- @param wid widget
-- @return command string
-- @return func or err msg
function CppLogic.UI.WidgetGetTooltipFunc(wid) end
--- calls a widgets tooltip action func.
-- @param wid widget
function CppLogic.UI.WidgetCallTooltipFunc(wid) end
--- overrides a widgets tooltip action func to a lua func that gets called without any arguments.
-- @param wid widget
-- @param f func
function CppLogic.UI.WidgetOverrideTooltipFunc(wid, f) end

--- gets a widgets tooltip data.
-- target widget can be the text widget or the container.
-- @param wid widget
-- @return target widget id
-- @return control target widget state flag
-- @return tooltip enabled flag
function CppLogic.UI.WidgetGetTooltipData(wid) end
--- sets a widgets tooltip data.
-- target widget can be the text widget or the container.
-- @param wid widget
-- @param tid target widget id
-- @param controlFlag control target widget state flag
-- @param enabledFlag tooltip enabled flag
function CppLogic.UI.WidgetSetTooltipData(wid, tid, controlFlag, enabledFlag) end

--- gets a widgets tooltip string.
-- this string gets shown automatically, if the target widget is a text widget.
-- @param wid widget
-- @return raw string
-- @return string table text key
function CppLogic.UI.WidgetGetTooltipString(wid) end
--- sets a widgets tooltip string.
-- this string gets shown automatically, if the target widget is a text widget.
-- @param wid widget
-- @param string
-- @param isSTTKey (optional, default false) if true, sets this as string table text key
function CppLogic.UI.WidgetSetTooltipString(wid, str, isSTTKey) end

--- returns a table with all child widget ids.
-- @param wid widget
-- @param t
function CppLogic.UI.ContainerWidgetGetAllChildren(wid) end

--- returns if the tooltip of this widget is shown.
-- @param wid widget
-- @param bool
function CppLogic.UI.WidgetIsTooltipShown(wid) end

--- returns the font config.
-- @param fontName font
-- @return size
-- @return offset
-- @return spacing
function CppLogic.UI.FontGetConfig(fontName) end
--- sets the font config.
-- @param fontName font
-- @param size
-- @param offset
-- @param spacing
function CppLogic.UI.FontSetConfig(fontName, size, offset, spacing) end

--- sets the font of a widget.
-- @param wid widget
-- @param fontName font
function CppLogic.UI.WidgetSetFont(wid, fontName) end

--- basic widget data.
-- @param wid widget
-- @return zpriority
-- @return forceToHandleMouseEvent
-- @return forceToNeverBeFound
function CppLogic.UI.WidgetGetBaseData(wid) end
--- basic widget data.
-- @param wid widget
-- @param zpriority (optional, default current value)
-- @param forceToHandleMouseEvent (optional, default current value)
-- @param forceToNeverBeFound (optional, default current value)
function CppLogic.UI.WidgetSetBaseData(wid, zpriority, forceToHandleMouseEvent, forceToNeverBeFound) end

--- string frame distance of a text display. (distance from top of widget to string rendering).
-- @param wid widget
-- @return frame distance
function CppLogic.UI.WidgetGetStringFrameDistance(wid) end
--- string frame distance of a text display. (distance from top of widget to string rendering).
-- @param wid widget
-- @param fdis frame distance
function CppLogic.UI.WidgetSetStringFrameDistance(wid, fdis) end

--- line distance factor (space between lines).
-- @param wid widget
-- @return distance factor
function CppLogic.UI.StaticTextWidgetGetLineDistanceFactor(wid) end
--- line distance factor (space between lines).
-- @param wid widget
-- @param df distance factor
function CppLogic.UI.StaticTextWidgetSetLineDistanceFactor(wid, df) end

--- gets a widget material textures position and size
-- @param wid widget
-- @param mat material index, [0..4] for buttons, 0 for statics.
-- @return x
-- @return y
-- @return w
-- @return h
function CppLogic.UI.WidgetMaterialGetTextureCoordinates(wid, mat) end
--- sets a widget material textures position and size
-- @param wid widget
-- @param mat material index, [0..4] for buttons, 0 for statics.
-- @param x (optional, default current)
-- @param y (optional, default current)
-- @param w (optional, default current)
-- @param h (optional, default current)
function CppLogic.UI.WidgetMaterialSetTextureCoordinates(wid, mat, x, y, w, h) end

--- a buttons shortcut string.
-- this is not directly a string, its the key code encoded in one char.
-- you may use KeyOf(string.byte(txt, 1), Keys) to decode the return values.
-- @param wid widget
-- @return raw string
-- @return string table text key
function CppLogic.UI.ButtonGetShortcutString(wid) end
--- a buttons shortcut string.
-- this is not directly a string, its the key code encoded in one char.
-- you may use string.char(Keys[c.Key]) to get the correct code.
-- @param wid widget
-- @param string
-- @param isSTTKey (optional, default false) if true, sets this as string table text key
function CppLogic.UI.ButtonSetShortcutString(wid, str, isSTTKey) end

--- sets a widgets group.
-- @param wid widget
-- @param group group name
function CppLogic.UI.WidgetSetGroup(wid, group) end

--- removes a widget.
-- @param wid widget
function CppLogic.UI.RemoveWidget(wid) end

--- gets a widgets name.
-- @param wid widget
-- @return name
function CppLogic.UI.GetWidgetName(wid) end


--- creates a new static widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- @param wid widget
-- @param name widgetname of the child
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @return id
function CppLogic.UI.ContainerWidgetCreateStaticWidgetChild(wid, name, before) end
--- creates a new static text widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- @param wid widget
-- @param name widgetname of the child
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @return id
function CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild(wid, name, before) end
--- creates a new pure tooltip widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- @param wid widget
-- @param name widgetname of the child
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @return id
function CppLogic.UI.ContainerWidgetCreatePureTooltipWidgetChild(wid, name, before) end
--- creates a new gfx button widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- @param wid widget
-- @param name widgetname of the child
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @return id
function CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild(wid, name, before) end
--- creates a new text button widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- @param wid widget
-- @param name widgetname of the child
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @return id
function CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild(wid, name, before) end
--- creates a new progress bar widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- @param wid widget
-- @param name widgetname of the child
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @return id
function CppLogic.UI.ContainerWidgetCreateProgressBarWidgetChild(wid, name, before) end
--- creates a new container widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- @param wid widget
-- @param name widgetname of the child
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @return id
function CppLogic.UI.ContainerWidgetCreateContainerWidgetChild(wid, name, before) end
--- creates a new container widget and registers it as its child.
-- the widget is initially hidden and all variables at a default value.
-- all attached functions are empty, and the UpdateManualFlag is set.
-- position and size are 0.
-- not all customwidget types were tested by me, use at your own risk. known customwidget types:
-- - EGUIX::CVideoPlaybackCustomWidget
--		on first video start clamps the video to the widget size.
--		if you change size afterwards, it scales the video from the previously clamped size.
-- @param wid widget
-- @param name widgetname of the child
-- @param customclass name of the custom class. may not work properly, if not actually a customwidget.
-- @param before (optional) if set, the new widget gets moved before this widget in the mothers list (rendering on top of it)
-- @param icv0 intergerCustomVariable 0 (optional, default 0)
-- @param icv1 intergerCustomVariable 1 (optional, default 0)
-- @param icv2 intergerCustomVariable 2 (optional, default 0)
-- @param icv3 intergerCustomVariable 3 (optional, default 0)
-- @param icv4 intergerCustomVariable 4 (optional, default 0)
-- @param icv5 intergerCustomVariable 5 (optional, default 0)
-- @param scv0 stringCustomVariable 0 (optional, default "")
-- @param scv1 stringCustomVariable 1 (optional, default "")
-- @return id
function CppLogic.UI.ContainerWidgetCreateCustomWidgetChild(wid, name, customclass, before, icv0, icv1, icv2, icv3, icv4, icv5, scv0, scv1) end

--- shows a resources gained floating number at a specific entity.
-- entity has to be visible, but settlers or rocks work fine.
-- if the entity moves, the floatie does move with it.
-- @param entity entity defines screen position
-- @param amount number to show (abs of this int gets shown)
function CppLogic.UI.ShowResourceFloatieOnEntity(entity, amount) end

--- char entered callback. use string.char to get the character.
-- does not work with SCELoader.
-- gets called when a key (or key kombination) is pressed that can be converted to a char.
-- you can return true to skip all additional message handling.
-- set to nil to remove.
-- @param f func to be called (char)->handeled
function CppLogic.UI.SetCharTrigger(f) end
--- key pressed callback.
-- does not work with SCELoader.
-- use global Keys to check what was pessed.
-- gets called at least twice, when the key gets pressed down, then when the key gets released (only time up is true).
-- if the key keepd pressed, after a short delay the trigger is called in fast succession.
-- you can return true to skip all additional message handling.
-- set to nil to remove.
-- @param f func to be called (key, up)->handeled
function CppLogic.UI.SetKeyTrigger(f) end
--- mouse event callback.
-- does not work with SCELoader.
-- use global MouseEvents to check what was pessed (from CommunityLib).
-- MouseMove does not get forwarded to lua (cause it is spammed),
-- Double clicks are not generated, you get 2 normal klicks instead.
-- parameters for MouseWheel are (id, x, y, forward)->handeled.
-- parameters for XButtons are (id, x, y, isxb2)->handeled.
-- parameters for all others are (id, x, y)->handeled.
-- x and y cooridates are screen coordinates not processed by SHoK and are equals to what GUI.GetMousePosition returns you (not scaled to widget coordinates).
-- you can return true to skip all additional message handling.
-- set to nil to remove.
-- @param f func to be called
function CppLogic.UI.SetMouseTrigger(f) end
--- same as CppLogic.UI.SetMouseTrigger, just designed for mainmenu
function CppLogic.UI.SetMouseTriggerMainMenu(f) end

--- sets a GUI State which reroutes clicks to lua functions.
-- onclick gets called on left click and gets x and y coordinates of the mouse cursor and can return a bool to override ending the state (default true).
-- oncancel gets called on right click (or gets canceled any other way).
-- make sure you set the mouse cursor after the call or modify GameCallback_GUI_StateChanged to set it.
-- @param onclick func(x,y)->endstate
-- @param oncancel func()
function CppLogic.UI.SetGUIStateLuaSelection(onclick, oncancel) end

--- gets the map position under a given screen position.
-- @param x
-- @param y1
-- @return pos
-- @return terrainHeigt
function CppLogic.UI.GetLandscapePosAtScreenPos(x, y) end

--- shows a command acknowledgement at a given position. (usually shown on move commands).
-- @param p
function CppLogic.UI.ShowCommandAcknowledgementAtPosition(p) end

--- gets the main windows client size.
-- == GUI.GetScreenSize(), but available in main menu.
-- @return right
-- @return bottom
function CppLogic.UI.GetClientSize() end

--- checks if a widget is a container widget.
-- @param wid
-- @return true/false
function CppLogic.UI.IsContainerWidget(wid) end

--- resets the global CppLogic.
-- useful if you dont want to use FrameworkWrapper to prevent savegames to override it.
function CppLogic_ResetGlobal() end

--- @class UACore
local UACore = {}
function UACore:AddLeader(id) end
function UACore:GetPos() end
function UACore:Tick() end
function UACore:Iterate() end
function UACore:IterateTransit() end
function UACore:OnIdChanged(ol, ne) end
function UACore:GetSize(trans, deadHero) end
function UACore:RemoveLeader(id) end
function UACore:IsIdle() end
function UACore:GetStatus() end
function UACore:SetArea(a) end
function UACore:SetTarget(p) end
function UACore:DumpTable() end
function UACore:ReadTable(t) end
function UACore:SetStatus(s) end
function UACore:SetReMove(b) end
function UACore:SetCurrentBattleTarget(id) end
function UACore:GetRangedMelee() end
function UACore:SetIgnoreFleeing(fl) end
function UACore:SetAutoRotateFormation(ar) end
function UACore:GetFirstDeadHero() end
function UACore:SetPrepDefense(p) end
function UACore:SetSabotageBridges(s) end


--- creates new ua.
-- @param pl player
--- @return UACore ua
function CppLogic.UA.New(pl, format, commandqueue, spawner, normalize) end

--- gets next enemy in area.
function CppLogic.UA.GetNearestEnemyInArea(pl, pos, area, ignoreFleeing) end

function CppLogic.UA.AddCannonBuilderData(heroTy, bottomTy, topTy) end

function CppLogic.UA.CountTargetEntitiesInArea(pl, pos, area, ignoreFleeing) end

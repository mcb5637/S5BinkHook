
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
---		- added @@ to print one @ (escape sequence)
---		- added @icon:iconid,texX,texY,texW,texH,sizeX,sizeY renders a EGUIX::CMaterial as a char inside the text
---			- iconid is the path to the file, same format as for widgets. (this is a GUITexture, you may use the corresponding  ModLoader functions for them).
---			- texX, texY, texW, texH are texture coordinates to display part of the texture only (optional, default 0,0,1,1, either all or none) (easiest to copy them out of the GUIEdito).
---			- sizeX, sizeY size multipliers (optional, default Y=X, X=1, you may specify only one) (if you want to use them, texture coordinates are required).
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
--- ModLoader is not available, if Kimichuras dlls are loaded.
--- otherwise the modloader looks for a `ModLoader.lua` file in your map directory. if it is there, this file gets loaded and the ModLoader executes.
--- see default_ModLoader.lua for callbacks and variables.
--- unless otherwise noted, modloader functions are only available during ModLoader callbacks.
CppLogic.ModLoader = {}

--- CppLogic version number.
CppLogic.Version = 1.4000

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

--- sets high precision FPU (gets reset on every API call, so call id directly before your calculations)
function CppLogic.Memory.SetFPU() end

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
--- - salims trap shows up as unknown (todo?)
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
function CppLogic.Logic.AddArchive(arch) end
--- removes the top archive. may only remove archives that you added yourself.
function CppLogic.Logic.RemoveTopArchive() end

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
--- @param key string key to replace
--- @param text string replacement string or nil to restore default
function CppLogic.Logic.SetStringTableText(key, text) end

--- serf ui place building rotaton.
--- does not work with SCELoader.
--- @return number rotation
function CppLogic.Logic.GetPlaceBuildingRotation() end
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
--- @param mapname string filename of the map
--- @param typ number type code
--- @param campname string campagn name
--- @return string path
function CppLogic.API.MapGetDataPath(mapname, typ, campname) end

--- returns the map a save is accociated with.
--- does not check, if the save is still valid, just if it exists.
--- @param save string
--- @return string mapname
--- @return number typ
--- @return string campname
--- @return string GUID
function CppLogic.API.SaveGetMapInfo(save) end

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
--- status of this gets saved into a savegame.
function CppLogic.Combat.EnableAoEProjectileFix() end
--- disables AoE projectile fix.
--- when enabled, cannons and similar AoE projectiles use the entitytypes damageclass.
--- status of this gets saved into a savegame.
function CppLogic.Combat.DisableAoEProjectileFix() end

--- enables camoflage projectile fix.
--- when enabled, camoflage does not get reset by ranged attacks, also all attackers get cleared on entering camo.
--- status of this gets saved into a savegame.
function CppLogic.Combat.EnableCamoFix() end
--- disabes camoflage projectile fix.
--- when enabled, camoflage does not get reset by ranged attacks, also all attackers get cleared on entering camo.
--- status of this gets saved into a savegame.
function CppLogic.Combat.DisableCamoFix() end

--- @class Predicate
local Predicate = {}

--- iterates over all entities that match a predicate.
--- perfect to use with for loop.
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
--- you have to manually create an and predicate.
--- perfect to use with for loop.
--- examples:
--- - for id in CppLogic.Entity.EntityIterator(...) do Message(id) end  
--- - for id, rsqu, prio in CppLogic.Entity.PlayerEntityIterator(CppLogic.Entity.Predicates.InCircle(...), 1) do Message(id.."   "..r) end  
--- @param pred Predicate userdata
--- @param ... number players
--- @return fun():number,number,number nextEntity
--- @return nil iteratorStatus
--- @return nil
function CppLogic.Entity.PlayerEntityIterator(pred, ...) end

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
--- @param prio Predicate
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
--- @return Predicate
function CppLogic.Entity.Predicates.IsBuilding() end

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

--- entity task list index.
--- @param id entity
--- @return number index
function CppLogic.Entity.GetTaskListIndex(id) end
--- entity task list index.
--- @param id entity
--- @param i number index
function CppLogic.Entity.SetTaskListIndex(id, i) end

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
--- does not work for trapcannon.
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

--- settler current work time.
--- @param id entity
--- @return number workTime
function CppLogic.Entity.Settler.WorkerGetCurrentWorkTime(id) end
--- settler current work time.
--- @param id entity
--- @param wt number work time
function CppLogic.Entity.Settler.WorkerSetCurrentWorkTime(id, wt) end

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
function CppLogic.Entity.Building.BarracksBuySoldierForLeader(bid, lid, noChecktype) end

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

--- buys a leader in a barracks by entitytype.
--- uses resources, asserts if not possible.
--- @param id entity
--- @param ety entity type
--- @param checkType boolean|nil check if leader type can be recruited at this barracks upgradecategory (optional, default false)
--- @return number id
function CppLogic.Entity.Building.BarracksBuyLeaderByType(id, ety, checkType) end

--- gets ApproachPosition, LeavePosition and DoorPos of a building.
--- @param id entity
--- @return Position approach
--- @return Position leave
--- @return Position door
function CppLogic.Entity.Building.GetRelativePositions(id) end


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
function CppLogic.EntityType.GetSuspendedAnimation(ty, a) end

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
--- @param tid widget target widget id
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

--- gets a widget material textures position and size
--- @param wid widget
--- @param mat number material index, [0..4] for buttons, 0 for statics.
--- @return number x
--- @return number y
--- @return number w
--- @return number h
function CppLogic.UI.WidgetMaterialGetTextureCoordinates(wid, mat) end
--- sets a widget material textures position and size
--- @param wid widget
--- @param mat number material index, [0..4] for buttons, 0 for statics.
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

--- sets a widgets group.
--- @param wid widget
--- @param group string group name
function CppLogic.UI.WidgetSetGroup(wid, group) end

--- removes a widget.
--- @param wid widget
function CppLogic.UI.RemoveWidget(wid) end

--- gets a widgets name.
--- @param wid widget
--- @return string name
function CppLogic.UI.GetWidgetName(wid) end


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

--- loads an entitytype from a xml file (data/config/entities/typename.xml).
--- the entitytype gets automatically removed on leaving the map.
--- load any additional models you want to use first.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.AddEntityType(typename) end

--- reloads an entitytype from a xml file (data/config/entities/typename.xml).
--- the entitytype gets automatically reloaded on leaving the map (after s5x archives got removed).
--- load any additional models you want to use first.
--- @param tid number
function CppLogic.ModLoader.ReloadEntityType(tid) end

--- assigns an id in the EntityType IdManager. A call to AddEntityType is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadEntityType(typename) end

--- sets an entitytype to get automatically reloaded on leaving the map (after s5x archives got removed).
--- (for editing it manually via CppLogic).
--- this function is available after ModLoader finishes.
--- @param tid number
function CppLogic.ModLoader.SetEntityTypeToReload(tid) end

--- loads an effecttype from a xml file (data/config/effects/typename.xml) (not default location).
--- the effecttype gets automatically removed on leaving the map.
--- load any additional models you want to use first.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.AddEffectType(typename) end

--- reloads an effecttype from a xml file (data/config/effects/typename.xml).
--- if you use it, all effect types get reloaded on leaving the map (after s5x archives got removed).
--- load any additional models you want to use first.
--- @param tid number
function CppLogic.ModLoader.ReloadEffectType(tid) end

--- assigns an id in the EffectType IdManager. A call to AddEffectType is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadEffectType(typename) end

--- loads a task list from a xml file (data/config/tasklists/tlname.xml).
--- the task list gets automatically removed on leaving the map.
--- @param tlname string
--- @return number type_id
function CppLogic.ModLoader.AddTaskList(tlname) end

--- reloads an task list from a xml file (data/config/tasklists/tlname.xml).
--- all tasklists get reloaded in any case on starting a new map.
--- @param tid number
function CppLogic.ModLoader.ReloadTaskList(tid) end

--- assigns an id in the TaskList IdManager. A call to AddTaskList is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadTaskList(typename) end

--- loads a technology from a xml file (data/config/technologies/tname.xml).
--- the tech gets automatically removed on leaving the map.
--- @param tname string
--- @return number type_id
function CppLogic.ModLoader.AddTechnology(tname) end

--- reloads a technology from a xml file (data/config/technologies/tname.xml).
--- all technologies get reloaded in any case on starting a new map.
--- @param tid number
function CppLogic.ModLoader.ReloadTechnology(tid) end

--- assigns an id in the Technologies IdManager. A call to AddTechnology is required before leaving ModLoader.
--- @param typename string
--- @return number type_id
function CppLogic.ModLoader.PreLoadTechnology(typename) end

--- loads a model from a xml and dff file (data/config/models/mname.xml and data/graphics/models/mname.dff).
--- the xml contains the data from Models.xml, the dff is the actual model.
--- also loads all required textures (remember to put the 3 thexture sizes into the correct folders).
--- the model gets automatically removed on leaving the map.
--- @param mname string
--- @return number type_id
function CppLogic.ModLoader.AddModel(mname) end

--- reloads a model from a xml and dff file (data/config/models/mname.xml and data/graphics/models/mname.dff).
--- the xml contains the data from Models.xml, the dff is the actual model.
--- also loads all required textures (remember to put the 3 thexture sizes into the correct folders).
--- the model gets automatically reloaded on leaving the map (after s5x archives got removed).
--- @param mid number
function CppLogic.ModLoader.ReloadModel(mid) end

--- loads a gui texture from a png file (data/graphics/textures/gui/tname.png).
--- the texture gets automatically removed on leaving the map.
--- note: this is similar to CppLogic.UI.PreLoadGUITexture, just that the texture gets cleaned up.
--- this function is available after ModLoader finishes.
--- @param tname string full path to texture
--- @return number type_id
function CppLogic.ModLoader.AddGUITexture(tname) end

--- reloads a gui texture from a png file (data/graphics/textures/gui/tname.png).
--- the texture gets automatically reloaded on leaving the map (after s5x archives got removed).
--- (different to other reload funcs in ModLoader, this one takes the name (full path) of the texture instead of its id as parameter).
--- this function is available after ModLoader finishes.
--- @param tname string full path to texture
function CppLogic.ModLoader.ReloadGUITexture(tname) end

--- loads an animation from an anm file (data/graphics/animations/aname.anm).
--- the animation gets automatically removed on leaving the map.
--- only writes the id to Animations if it exists.
--- @param aname string
--- @return number type_id
function CppLogic.ModLoader.AddAnimation(aname) end

--- reloads an animation from an anm file (data/graphics/animations/aname.anm).
--- the animation gets automatically reloaded on leaving the map (after s5x archives got removed).
--- use CppLogic.API.CreateExtraDataTables to get the Animations table with all ids.
--- @param aid number
function CppLogic.ModLoader.ReloadAnimation(aid) end

--- adds a settler upgradecategory. to create a upgradecategory, fill in the Upgrade xml entry in the entities, but leave out Category.
--- then call this function with the upgradecategory and the first entitytype. (this will fill in the missing Category).
--- @param ucatname string
--- @param firstid number
--- @return number type_id
function CppLogic.ModLoader.AddSettlerUpgradeCategory(ucatname, firstid) end

--- adds a building upgradecategory. to create a upgradecategory, fill in the Upgrade xml entry in the entities, but leave out Category.
--- then call this function with the upgradecategory and the first entitytype. (this will fill in the missing Category).
--- @param ucatname string
--- @param firstid number
--- @return number type_id
function CppLogic.ModLoader.AddBuildingUpgradeCategory(ucatname, firstid) end

--- loads a selection texture file (data/graphics/textures/tname.dds).
--- the texture name is referenced in the additional model data.
--- remember to put the 3 texture sizes into the correct folders.
--- the texture gets removed on leaving the map.
--- @param tname string
function CppLogic.ModLoader.AddSelectionTexture(tname) end

--- reloads a selection texture file (data/graphics/textures/tname.dds).
--- remember to put the 3 texture sizes into the correct folders.
--- the texture gets reloaded on leaving the map.
--- @param tname string
function CppLogic.ModLoader.ReloadSelectionTexture(tname) end

--- loads a water type from a xml file (data/config/watertypes/wname.xml).
--- all water types get reloaded on exiting the map, after using this function.
--- you may request a specific id, or get one automatically by passing 0.
--- if you request a specific id and it does already exist, throws.
--- only writes the id to WaterTypes if it exists.
--- @param wname string
--- @param id number|nil optional id, default == 0
--- @return number type_id
function CppLogic.ModLoader.AddWaterType(wname, id) end

--- reloads a water type from a xml file (data/config/watertypes/wname.xml).
--- all water types get reloaded on exiting the map, after using this function.
--- @param id number
function CppLogic.ModLoader.ReloadWaterType(id) end

--- loads a terrain texture file (data/graphics/textures/tname.xml).
--- the texture name is referenced in water and terrain types.
--- remember to put the 3 texture sizes into the correct folders.
--- the texture gets removed on leaving the map.
--- @param tname string
function CppLogic.ModLoader.AddTerrainTexture(tname) end

--- reloads a terrain texture file (data/graphics/textures/tname.xml).
--- remember to put the 3 texture sizes into the correct folders.
--- the texture gets reloaded on leaving the map.
--- @param tname string
function CppLogic.ModLoader.ReloadTerrainTexture(tname) end

--- loads a terrain type from a xml file (data/config/terraintypes/tname.xml).
--- all terrain types get reloaded on exiting the map, after using this function.
--- you may request a specific id, or get one automatically by passing 0.
--- if you request a specific id and it does already exist, throws.
--- only writes the id to TerrainTypes if it exists.
--- @param tname string
--- @param id number|nil optional id, default == 0
--- @return number type_id
function CppLogic.ModLoader.AddTerrainType(tname, id) end

--- reloads a terrain type from a xml file (data/config/terraintypes/tname.xml).
--- all terrain types get reloaded on exiting the map, after using this function.
--- @param id number
function CppLogic.ModLoader.ReloadTerrainType(id) end

--- loads an ExperienceClass from a xml file (data/config/experiencexpclassname.xml).
--- assigns this experienceclass to every entity that has the category ecategory.
--- the ExperienceClass is removed on exiting the map.
--- (ecategory may be a random int that can get attached to entitytypes via CppLogic.EntityType.AddEntityCategory, just remember to remove it again).
--- only writes the id to ExperienceClasses if it exists.
--- @param xpclassname string
--- @param ecategory number
--- @return number type_id
function CppLogic.ModLoader.AddExperienceClass(xpclassname, ecategory) end

--- reloads an ExperienceClass from a xml file (data/config/experiencexpclassname.xml).
--- the ExperienceClass is reloaded on exiting the map.
--- @param id number
function CppLogic.ModLoader.ReloadExperienceClass(id) end

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
--- @param setname string
--- @return number type_id
function CppLogic.ModLoader.AddAnimSet(setname) end

--- reloads an AnimSet from a xml file (data/config/animsets/setname.xml).
--- the AnimSet is reloaded on exiting the map.
--- @param id number
function CppLogic.ModLoader.ReloadAnimSet(id) end

--- loads or reloads a DirectX effect (shader pair) from a fx file (data/graphics/effects/ename.fx and data/graphics/effects/ffp/ename.fx).
--- the effect gets automatically removed when exiting the map.
--- be careful, the ids of these effects cannot be reused.
--- @param ename string
function CppLogic.ModLoader.LoadDirectXEffect(ename) end

--- resets the global CppLogic.
--- useful if you dont want to use FrameworkWrapper to prevent savegames to override it.
function CppLogic_ResetGlobal() end

--- @class UACore
local UACore = {}
function UACore:AddLeader(id) end
---@return Position
function UACore:GetPos() end
function UACore:Tick(self) end
---@return fun(state:userdata, index:number):number,number
---@return userdata
---@return number
function UACore:Iterate() end
---@return fun(state:userdata, index:number):number,number
---@return userdata
---@return number
function UACore:IterateTransit() end
function UACore:OnIdChanged(ol, ne) end
---@return number
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
---@return number[]
---@return number[]
---@return number[]
function UACore:GetRangedMelee() end
function UACore:SetIgnoreFleeing(fl) end
function UACore:SetAutoRotateFormation(ar) end
function UACore:GetFirstDeadHero() end
function UACore:SetPrepDefense(p) end
function UACore:SetSabotageBridges(s) end
function UACore:SetDoNotNormalizeSpeed(n) end


--- creates new ua.
--- @param pl number player
--- @return UACore ua
function CppLogic.UA.New(pl, format, commandqueue, spawner, seed) end

--- gets next enemy in area.
function CppLogic.UA.GetNearestEnemyInArea(pl, pos, area, ignoreFleeing) end

function CppLogic.UA.AddCannonBuilderData(heroTy, bottomTy, topTy) end

function CppLogic.UA.CountTargetEntitiesInArea(pl, pos, area, ignoreFleeing) end

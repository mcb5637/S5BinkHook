--- CppLogic
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

--- check if an effect is valid
-- @param id effect id
-- @return true/false
function CppLogic.Effect.IsValidEffect(id) end

--- create a projectile.
-- to deal damage on impact use:
-- - AOE: dmg > 0, radius > 0, attid or playerid set to attacker entity or player (deals damage only to entities hostile to attacker).
-- - single target: dmg > 0, tarid to target entity id, attid or playerid set to attacker entity or player.
-- @param effecttype GGL_Effects.XXX
-- @param startx start pos x
-- @param starty start pos y
-- @param tarx target pos x
-- @param tary target pos y
-- @param dmg damage done by projectile, default 0
-- @param radius damage radius for AOE projectiles, default -1
-- @param tarid target entity id, default 0
-- @param attid attacker entity id, default 0
-- @param playerid attacker player id, default 0
-- @return effect id
function CppLogic.Effect.CreateProjectile(effecttype, startx, starty, tarx, tary, dmg, radius, tarid, attid, playerid) end


--- sets high precision FPU (gets reset on every API call, so call id directly before your calculations)
function CppLogic.Memory.SetFPU() end

--- reloads cutscenes from the specified path
-- @param path optional, default "Maps\\ExternalMap"
function CppLogic.Memory.ReloadCutscene(path) end

--- gets the animation id from a name.
-- @param anim the name of the animation
-- @return animation id
function CppLogic.Memory.GetAnimIdFromName(anim) end


--- compiles a lua chunk
-- @param code to compile, sourcecode or binary
-- @return function to execute the compiled code, or error message
-- @return bool successful
function CppLogic.API.Eval(code) end

--- logs a string to the current settlers log.
-- @param str the string to log
function CppLogic.API.Log(str) end


--- deals damage to a target.
-- calls respective hurt entity trigger.
-- @param target entity to be damaged
-- @param damage damage to deal
-- @param attacker entity dealing the damage (gets xp from kill, statistics...), default 0
function CppLogic.Combat.DealDamage(target, damage, attacker) end


--- iterates over all entities that match a predicate.
-- perfect to use with for loop.
-- @param pred predicate userdata
-- @return nextEntity func
-- @return iteratorStatus
-- @return nil
function CppLogic.Entity.EntityIterator(pred) end

--- gets all entities that match a predicate as a table.
-- @param pred predicate userdata
-- @return table of ids
function CppLogic.Entity.EntityIteratorTableize(pred) end

--- checks an entity agains a predicate.
-- @param id entity
-- @param pred predicate
-- @return bool
function CppLogic.Entity.CheckPredicate(id, pred) end


--- creates a predicate that performs an and of multiple predicates.
-- @param ... predicates
-- @return predicate userdata
function CppLogic.Entity.Predicates.And(...) end

--- creates a predicate that performs an or of multiple predicates.
-- @param ... predicates
-- @return predicate userdata
function CppLogic.Entity.Predicates.Or(...) end

--- creates a predicate that checks for one entitytype.
-- @param etyp entitytype to check against
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfType(etyp) end

--- creates a predicate that checks for one player.
-- @param pl player to check against
-- @return predicate userdata
function CppLogic.Entity.Predicates.OfPlayer(pl) end

--- creates a predicate that checks for an area.
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

--- creates a predicate that checks for settlers or buildings.
-- @return predicate userdata
function CppLogic.Entity.Predicates.IsSettlerOrBuilding() end

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

--- entity scale.
-- @param id entity
-- @return scale
function CppLogic.Entity.GetScale(id) end
--- entity scale.
-- @param id entity
-- @param s scale
function CppLogic.Entity.SetScale(id, s) end

--- moving entity target pos (best to change directly after a move command).
-- @param id entity
-- @return position table, with optional rotation
function CppLogic.Entity.MovingEntityGetTargetPos(id) end
--- moving entity target pos (best to change directly after a move command).
-- @param id entity
-- @param p position table, with optional rotation
function CppLogic.Entity.MovingEntitySetTargetPos(id, p) end


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


--- a leaders experience.
-- @param id leader
-- @return xp
function CppLogic.Entity.Leader.GetExperience(id) end
--- a leaders experience
-- @param id leader
-- @param xp xp
function CppLogic.Entity.Leader.SetExperience(id, xp) end

--- a leaders troop health (hp of all soldiers summed up).
-- -1 when not yet set (calculate in lua as all soldiers at full hp).
-- @param id leader
-- @return hp
function CppLogic.Entity.Leader.GetTroopHealth(id) end
--- a leaders troop health (does not kill soldiers when decreasing).
-- @param id leader
-- @param hp hp
function CppLogic.Entity.Leader.SetTroopHealth(id, hp) end


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


--- entity type max range (standard for melee is 250).
-- @param ty entity type
-- @return range
function CppLogic.EntityType.GetMaxRange(ty) end
--- entity type max range (standard for melee is 250).
-- @param ty entity type
-- @param r range
function CppLogic.EntityType.SetMaxRange(ty, r) end


--- the soldier type of a leader type
-- @param ty leader type
-- @return soldier type
function CppLogic.EntityType.Settler.LeaderTypeGetSoldierType(ty) end
--- the soldier type of a leader type
-- @param leaderTy leader type
-- @param soldierTy soldier type
function CppLogic.EntityType.Settler.LeaderTypeSetSoldierType(leaderTy, soldierTy) end

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

--- limited lifespan remaining seconds.
-- @param id entity
-- @return duration
function CppLogic.Entity.GetLimitedLifespanRemaining(id) end
--- limited lifespan remaining seconds.
-- @param id entity
-- @param t duration
function CppLogic.Entity.GetLimitedLifespanRemaining(id, t) end

--- limited lifespan duration seconds.
-- @param ty entitytype
-- @return duration
function CppLogic.EntityType.GetLimitedLifespanDuration(ty) end
--- limited lifespan duration seconds.
-- @param ty entitytype
-- @param t duration
function CppLogic.EntityType.SetLimitedLifespanDuration(ty, t) end

--- leader type upkeep per payday.
-- @param ty entitytype
-- @return upkeep
function CppLogic.EntityType.Settler.LeaderTypeGetUpkeep(ty) end
--- leader type upkeep per payday.
-- @param ty entitytype
-- @param u upkeep
function CppLogic.EntityType.Settler.LeaderTypeSetUpkeep(ty, u) end

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

--- damageclass of autoattacks of an entity type.
-- @param ty entitytype
-- @return damageclass
function CppLogic.EntityType.GetAutoAttackDamageClass(ty) end
--- damageclass of autoattacks of an entity type.
-- @param ty entitytype
-- @param dc damageclass
function CppLogic.EntityType.SetAutoAttackDamageClass(ty, dc) end

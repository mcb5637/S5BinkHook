
assert(false, "do not load this file, this is documentation only!")

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
-- examples:
-- - for id in CppLogic.Entity.EntityIterator(...) do Message(id) end  
-- - for id, rsqu in CppLogic.Entity.EntityIterator(CppLogic.Entity.Predicates.InCircle(...), ...) do Message(id.."   "..r) end  
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
function CppLogic.EntityType.GetAutoAttackMissChance(ty) end
--- range chance of autoattacks of an entity type.
-- for autocannons only max range, for anything else max and min range.
-- @param ty entitytype
-- @return max range (optional)
-- @return min range (ignored for autocannons) (optional)
function CppLogic.EntityType.SetAutoAttackMissChance(ty, maxrange, minrange) end

--- range in which enemies get attacked by this leader type.
-- @param ty entitytype
-- @return range
function CppLogic.EntityType.Settler.LeaderTypeGetAutoAggressiveRange(ty) end
--- range in which enemies get attacked by this leader type.
-- @param ty entitytype
-- @param r range
function CppLogic.EntityType.Settler.LeaderTypeSetAutoAggressiveRange(ty, r) end

--- number of settlers this building (villagecenter) supports.
-- @param ty entitytype
-- @return slots
function CppLogic.EntityType.Building.GetVCAttractionSlotsProvided(ty) end
--- number of settlers this building (villagecenter) supports.
-- @param ty entitytype
-- @param s slots
function CppLogic.EntityType.Building.SetVCAttractionSlotsProvided(ty, s) end

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

--- settler or building type armor and armorclass.
-- @param ty entitytype
-- @return armor
-- @return armorclass
function CppLogic.EntityType.GetArmor(ty) end
--- settler or building type armor and armorclass.
-- @param ty entitytype
-- @param ar armor
-- @param acl armorclass
function CppLogic.EntityType.GetArmor(ty, ar, acl) end

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
function CppLogic.EntityType.GetModels(ty, m0, m1, m2, m3) end

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

--- entitytytpe shuriken ability data.
-- @param ty entitytype
-- @return damage
-- @return damage class
-- @return number shuriken
-- @return range
-- @return max arc degree
-- @return recharge time
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
function CppLogic.EntityType.Settler.GetAbilityDataSniper(ty, dfac, ran, rech) end

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

--- settler current work time.
-- @param id entity
-- @return work time
function CppLogic.Entity.Settler.WorkerGetCurrentWorkTime(id) end
--- settler current work time.
-- @param id entity
-- @param wt work time
function CppLogic.Entity.Settler.WorkerSetCurrentWorkTime(id, wt) end

--- settler type fearless.
-- @param ty entitytype
-- @return fearless
function CppLogic.EntityType.Settler.GetFearless(ty) end
--- settler type fearless.
-- @param ty entitytype
-- @param fl fearless
function CppLogic.EntityType.Settler.SetFearless(ty, fl) end

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

--- entity check for soldier. (asserts if no entity).
-- @param id entity
-- @return is entity of soldier type
function CppLogic.Entity.IsSoldier(id) end

--- settler type cost.
-- @param ty entitytype
-- @return cost info table
function CppLogic.EntityType.Settler.GetCost(ty) end
--- settler type cost.
-- @param ty entitytype
-- @param c cost info table
-- @param ignoreZeroes should zeroes get ignored (optional)
function CppLogic.EntityType.Settler.SetCost(ty, c, ignoreZeroes) end

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

--- resurrect a hero.
-- @param ty entitytype
function CppLogic.Entity.Settler.HeroResurrect(id) end

--- set thief stolen resources.
-- @param ty entitytype
-- @param ty res type or 0
-- @param am amount (optional)
function CppLogic.Entity.Settler.ThiefSetStolenResourceInfo(id, ty, am) end

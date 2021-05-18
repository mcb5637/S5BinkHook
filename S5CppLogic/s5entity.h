#pragma once
#include "s5data.h"
#include <map>


struct entityAddonData {
	int EntityId = 0;
	int HealthOverride = -1;
	bool HealthUseBoni = true;
	int DamageOverride = -1;
	int ArmorOverride = -1;
	float ExplorationOverride = -1.0f;
	int RegenHPOverride = -1;
};

struct shok_EGL_CGLEEntity : shok_object {
	PADDINGI(1);
	int EntityId;
	PADDINGI(1);
	int EntityType;
	int ModelOverride;
	int PlayerId;
	int attachmentvt;
	shok_set<shok_attachment> ObserverEntities;
	shok_set<shok_attachment> ObserverEffects;
	shok_set<shok_attachment> ObservedEntities;
	shok_set<shok_attachment> ObservedEffects;
	byte SendEvent; // 20
	PADDING(3);
	int SetNewTaskListDepth;
	shok_positionRot Position; // 22
	float Scale; // 25
	PADDINGI(1);
	byte StandardBehaviorActive, f1, ValidPosition, IsOnWater;
	byte UserSelectableFlag, UserControlableFlag, IsVisible, OnlySetTaskListWhenAlive;
	int TaskListToSet; // 29
	vector_padding;
	std::vector<shok_EGL_CGLEBehavior*, shok_allocator<shok_EGL_CGLEBehavior*>> Behaviours; // 30, first field in 31
	int CurrentState, EntityState, CurrentTaskListID, CurrentTaskIndex; // 34 la37
	PADDINGI(12); // la49
	int Health; // 50
	char* ScriptName; // "Name" in loader
	char* ScriptCommandLine;
	float Exploration;
	int TaskListStart; // 54
	PADDINGI(2);
	byte InvulnerabilityFlag, Suspended; // 57
	PADDING(2);
	int SuspensionTurn;
	int ScriptingValue[4]; // 59
	int StateChangeCounter; // 63
	int TaskListChangeCounter;
	int NumberOfAuras; // 65

	static inline constexpr int vtp = 0x783E74;

private:
	shok_EGL_CGLEBehavior* SearchBehavior(int vt);
	shok_EGL_CGLEBehavior* SearchBehavior(int* vts, int num);
public:
	shok_GGL_CSoldierBehavior* GetSoldierBehavior();
	shok_GGL_CLeaderBehavior* GetLeaderBehavior();
	shok_GGL_CBehaviorDefaultMovement* GetMovementBehavior();
	shok_GGL_CBarrackBehavior* GetBarrackBehavior();
	shok_GGL_CCamouflageBehavior* GetCamoAbilityBehavior();
	shok_GGL_CThiefCamouflageBehavior* GetCamoThiefBehavior();
	shok_GGL_CHeroBehavior* GetHeroBehavior();
	shok_GGL_CLimitedLifespanBehavior* GetLimitedLifespanBehavior();
	shok_GGL_CWorkerBehavior* GetWorkerBehavior();
	shok_GGL_CMarketBehavior* GetMarketBehavior();
	shok_GGL_CThiefBehavior* GetThiefBehavior();
	shok_GGL_CBattleBehavior* GetBattleBehavior();
	shok_GGL_CHeroHawkBehavior* GetHeroHawkBehavior();
	shok_GGL_CInflictFearAbility* GetInflictFearBehavior();
	shok_GGL_CBombPlacerBehavior* GetBombPlacerBehavior();
	shok_GGL_CCannonBuilderBehavior* GetCannonBuilderBehavior();
	shok_GGL_CRangedEffectAbility* GetRangedEffectBehavior();
	shok_GGL_CCircularAttack* GetCircularAttackBehavior();
	shok_GGL_CSummonBehavior* GetSummonBehavior();
	shok_GGL_CConvertSettlerAbility* GetConvertSettlerBehavior();
	shok_GGL_CSniperAbility* GetSniperBehavior();
	shok_GGL_CShurikenAbility* GetShurikenBehavior();
	shok_GGL_CKegPlacerBehavior* GetKegPlacerBehavior();
	shok_GGL_CAbilityScoutBinocular* GetBinocularBehavior();
	shok_GGL_CTorchPlacerBehavior* GetTorchPlacerBehavior();
	shok_GGL_CPointToResourceBehavior* GetPointToResBehavior();
	shok_GGL_CKegBehavior* GetKegBehavior();
	shok_GGL_CFoundryBehavior* GetFoundryBehavior();
	shok_GGL_CSerfBehavior* GetSerfBehavior();
	shok_GGL_CBattleSerfBehavior* GetBattleSerfBehavior();
	shok_GGL_CLimitedAttachmentBehavior* GetLimitedAttachmentBehavior();
	shok_GGL_CBuildingMerchantBehavior* GetBuildingMerchantBehavior();
	shok_GGL_CBuildingMercenaryBehavior* GetMercenaryBehavior();



	bool IsEntityInCategory(int cat);
	bool IsMovingEntity();
	bool IsSettler();
	bool IsBuilding();
	bool IsResourceDoodad();
	int GetResourceProvided();
	shok_GGlue_CGlueEntityProps* GetEntityType();

	int EventGetDamage();
	int EventGetArmor();
	int EventGetMaxWorktime();
	int EventLeaderGetCurrentCommand();
	float GetExploration();
	int GetMaxHealth();
	int LimitedAttachmentGetMaximum(int attachType);

	void SetHealth(int h);

	void Destroy();

	int GetFirstAttachedToMe(int attachmentId);
	int GetFirstAttachedEntity(int attachmentId);

	void ClearAttackers();

	entityAddonData* GetAdditionalData(bool create);
	void CloneAdditionalDataFrom(entityAddonData* other);

protected:
	int EventGetIntById(int id);
};

struct shok_GGL_CBuilding;
struct shok_EGL_CMovingEntity : shok_EGL_CGLEEntity {
	shok_position TargetPosition; // la67
	byte TargetRotationValid;
	PADDING(3);
	float TargetRotation;
	int MovementState; // 70

	static inline constexpr int vtp = 0x783F84;

	void AttackMove(shok_position& p);
	void AttackEntity(int targetId);
	void HoldPosition();
	void Defend();
	void Move(shok_position& p);
	void LeaderAttachSoldier(int soldierId);
	void SettlerExpell();
	void HeroAbilitySendHawk(shok_position& p);
	void HeroAbilityInflictFear();
	void HeroAbilityPlaceBomb(shok_position& p);
	void HeroAbilityPlaceCannon(shok_position& p, int FoundationType, int CannonType);
	void HeroAbilityRangedEffect();
	void HeroAbilityCircularAttack();
	void HeroAbilitySummon();
	void HeroAbilityConvert(int target);
	void HeroAbilitySnipe(int tid);
	void HeroAbilityShuriken(int tid);
	void HeroAbilityActivateCamoflage();
	void ThiefSabotage(int tid);
	void ThiefDefuse(int tid);
	void ThiefStealFrom(int tid);
	void ThiefSecureGoods(int tid);
	void ScoutBinoculars(shok_position& p);
	void ScoutFindResource();
	void ScoutPlaceTorch(shok_position& p);
	bool SerfConstructBuilding(shok_GGL_CBuilding* build);
	bool SerfRepairBuilding(shok_GGL_CBuilding* build);
	void SerfExtractResource(int id);
	void SerfTurnToBattleSerf();
	void BattleSerfTurnToSerf();

	void SetPosition(shok_position& p);

	int LeaderGetNearbyBarracks();
	bool IsMoving();
};

struct shok_GGL_CEvadingEntity : shok_EGL_CMovingEntity {
	int EvaderWaitObject[4];

	static inline constexpr int vtp = 0x770A7C;
};

struct shok_GGL_CSettler : shok_GGL_CEvadingEntity {
	int TimeToWait, HeadIndex, HeadParams; // la77
private:
	int u2[7];
public:
	float ExperiencePoints; // 85
	int ExperienceLevel;
private:
	int u3[2];
public:
	float MovingSpeed; //89
private:
	int u4;
public:
	float Damage;
private:
	int u5;
public:
	float Dodge;
private:
	int u6;
public:
	float Motivation;
private:
	int u7;
public:
	float Armor;
private:
	int u8;
public:
	float CurrentAmountSoldiers;
private:
	int u9;
public:
	float MaxAmountSoldiers;
private:
	int u10;
public:
	float DamageBonus;
private:
	int u11;
public:
	float ExplorationCache;
private:
	int u12;
public:
	float MaxAttackRange;
private:
	int u13;
public:
	float AutoAttackRange;
private:
	int u14;
public:
	float HealingPoints;
private:
	int u15;
public:
	float MissChance;
private:
	int u16;
public:
	float Healthbar; // 115
private:
	int u17[7];
public:
	float SomeIntRegardingTaskLists; // 123
private:
	int u18[3];
public:
	int LeaderId;
private:
	int u19[2];
public:
	int OverheadWidget;
	int ExperienceClass;
private:
	int u20[2];
public:
	int BlessBuff, NPCMarker, LeaveBuildingTurn; //la136

	static inline constexpr int vtp = 0x76E3CC;

	bool IsIdle();

	int LeaderGetRegenHealth();
};

struct shok_GGL_CAnimal : shok_EGL_CMovingEntity {
	shok_position TerritoryCenter; // 71
	float TerritoryRadius; // int?
	shok_position DangerPosition;

	static inline constexpr int vtp = 0x778F7C;
};

struct shok_GGL_CResourceDoodad : shok_EGL_CGLEEntity {
	int ResourceType, ResourceAmount, ResourceAmountAdd;

	static inline constexpr int vtp = 0x76FEA4;
};

struct shok_GGL_CBuilding : shok_EGL_CGLEEntity {
	shok_position ApproachPosition, LeavePosition; // fi 66
	byte IsActive, IsRegistered, IsUpgrading, IsOvertimeActive, HQAlarmActive;
private:
	byte u[3];
public:
	int MaxNumWorkers, CurrentTechnology, LatestAttackTurn, MostRecentDepartureTurn;
	float BuildingHeight, Helathbar, UpgradeProgress; //la78
private:
	int u2[2]; // list Slots with NumberOfRepairingSerfs?
public:
	int NumberOfRepairingSerfs;
	int OvertimeCooldown;
	int ConstructionSiteType; // 83

	static inline constexpr int vtp = 0x76EB94;

	int GetConstructionSite();
	int GetNearestFreeConstructionSlotFor(shok_position* p);
	int GetNearestFreeRepairSlotFor(shok_position* p);
	bool IsConstructionFinished();
	bool IsIdle();
	int GetTechnologyInResearch();
	int GetCannonProgress();
	float GetMarketProgress();

	void StartUpgrade();
	void CancelUpgrade();
	void CommandBuildCannon(int entitytype);
	void CommandRecruitSoldierForLeader(int id);
	void ActivateOvertime();
	void DeactivateOvertime();
	void BarracksRecruitGroups();
	void BarracksRecruitLeaders();
	void HQBuySerf();
	void SellBuilding();
	void StartResearch(int tech);
	void CancelResearch();
	void MarketStartTrade(int ResourceTypeSell, int ResourceTypeBuy, float BuyAmount);
	void MarketCancelTrade();
};

struct shok_GGL_CBridgeEntity : shok_GGL_CBuilding {
	static inline constexpr int vtp = 0x77805C;
};

struct shok_EGL_CAmbientSoundEntity : shok_EGL_CGLEEntity {
private:
	int u;
public:
	int AmbientSoundType;

	static inline constexpr int vtp = 0x78568C;
};

struct shok_EGL_CGLEEntityCreator : shok_object {
	int EntityType = 0;
	shok_positionRot Pos = { 0,0,0 };
	int Flags = 0; // 5
	int PlayerId = 0;
	int Health = 0; // 7
	char* ScriptName = nullptr;
	char* ScriptCommandLine = nullptr;
	int AmbientSoundType = 0;
	float Scale = 0; // 11
	
	shok_EGL_CGLEEntityCreator();
	shok_EGL_CGLEEntityCreator(bool _);
	~shok_EGL_CGLEEntityCreator();
};
struct shok_GGL_CGLConstructionSiteCreator : shok_EGL_CGLEEntityCreator {


	shok_GGL_CGLConstructionSiteCreator();
};
struct shok_GGL_CResourceDoodadCreator : shok_EGL_CGLEEntityCreator {
	int ResourceAmount = 0;

	shok_GGL_CResourceDoodadCreator();
};

static inline shok_EGL_CGLEEntity* (__stdcall* const shok_eid2obj)(int id) = (shok_EGL_CGLEEntity * (__stdcall*)(int)) 0x5825B4;
static inline void(__cdecl* const shok_entityHurtEntity)(shok_EGL_CGLEEntity* attackerObj, shok_EGL_CGLEEntity* targetObj, int damage) = (void(__cdecl*)(shok_EGL_CGLEEntity*, shok_EGL_CGLEEntity*, int)) 0x49F358;
static inline void(__cdecl* const shok_entityDealAOEDamage)(shok_EGL_CGLEEntity* attacker, shok_position* center, float range, int damage, int player, int damageclass) = (void(__cdecl*)(shok_EGL_CGLEEntity*, shok_position*, float, int, int, int))0x49F82A;
static inline int(__cdecl* const shok_getEntityIdByScriptName)(const char* str) = (int(__cdecl*)(const char* str)) 0x576624;
static inline int(__cdecl* const shok_EntityGetProvidedResourceByID)(int id) = (int(__cdecl*)(int id)) 0x4B8489;
static inline bool(__cdecl* const shok_entityIsDead)(int id) = (bool(__cdecl*)(int)) 0x44B096;
static inline int(__cdecl* const shok_entityChangePlayer)(int entityid, int player) = (int(__cdecl*)(int, int)) 0x49A6A7;

shok_EGL_CGLEEntity* ReplaceEntityWithResourceEntity(shok_EGL_CGLEEntity* e);

void ActivateEntityChangePlayerFix();

extern void (*Hero6ConvertHookCb)(int id, int pl, int nid, int converter);
void HookHero6Convert();
void HookResetCamo();
extern int ResetCamoIgnoreIfNotEntity;
extern void (*CamoActivateCb)(shok_GGL_CCamouflageBehavior* th);
void HookCamoActivate();

extern int* HurtEntityDamagePointer;
void HookHurtEntity();

extern std::multimap<int, int> BuildingMaxHpBoni; // entitytype -> tech id
void EnableMaxHealthTechBoni();

extern entityAddonData LastRemovedEntityAddonData;
void HookDestroyEntity();

void EnableEntityDamageMod();
void EnableEntityArmorMod();
void EnableEntityExplorationMod();
extern bool LeaderRegenRegenerateSoldiers;
void HookLeaderRegen();

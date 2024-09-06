#include "pch.h"
#include <cmath>
#include "s5_player.h"
#include "s5_defines.h"
#include "s5_maplogic.h"
#include "s5_events.h"
#include "s5_scriptsystem.h"
#include "s5_entity.h"
#include "hooks.h"

static inline int(__thiscall* const plattracthandlerGetAttLimit)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall*)(GGL::CPlayerAttractionHandler*)>(0x4C216F);
int GGL::CPlayerAttractionHandler::GetAttractionLimit()
{
	return plattracthandlerGetAttLimit(this);
}

static inline int(__thiscall* const plattracthandlerGetAttUsage)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall*)(GGL::CPlayerAttractionHandler*)>(0x4C278B);
int GGL::CPlayerAttractionHandler::GetAttractionUsage()
{
	return plattracthandlerGetAttUsage(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getplayersettlerpayday)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1CDE);
int GGL::CPlayerAttractionHandler::GetWorkerPaydayIncome()
{
	return shok_playerattractionhandler_getplayersettlerpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumattractedworkers)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1BAA);
int GGL::CPlayerAttractionHandler::GetNumberOfAttractedWorkers()
{
	return shok_playerattractionhandler_getnumattractedworkers(this);
}
static inline float(__thiscall* const shok_playerattractionhandler_getplayerleaderpayday)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<float(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1D1E);
float GGL::CPlayerAttractionHandler::GetLeaderPaydayCost()
{
	return shok_playerattractionhandler_getplayerleaderpayday(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumleaders)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1B28);
int GGL::CPlayerAttractionHandler::GetNumberOfLeaders()
{
	return shok_playerattractionhandler_getnumleaders(this);
}
static inline bool(__thiscall* const playerattractionhandler_isslotavail)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<bool(__thiscall*)(GGL::CPlayerAttractionHandler*)>(0x4C28D0);
bool GGL::CPlayerAttractionHandler::IsAttractionsSlotAvailable()
{
	return playerattractionhandler_isslotavail(this);
}
static inline bool(__thiscall* const playerattractionhandler_ismotilocked)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<bool(__thiscall*)(GGL::CPlayerAttractionHandler*)>(0x4C1BEC);
bool GGL::CPlayerAttractionHandler::IsMotivationLocked()
{
	return playerattractionhandler_ismotilocked(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnumsettlers)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1B62);
int GGL::CPlayerAttractionHandler::GetNumberOfSettlers()
{
	return shok_playerattractionhandler_getnumsettlers(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getnummilitary)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1BCB);
int GGL::CPlayerAttractionHandler::GetNumberOfMilitaryUnits()
{
	return shok_playerattractionhandler_getnummilitary(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getsleepplacelimit)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C21AA);
int GGL::CPlayerAttractionHandler::GetSleepPlaceLimit()
{
	return shok_playerattractionhandler_getsleepplacelimit(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getfreesleepplaces)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C2226);
int GGL::CPlayerAttractionHandler::GetFreeSleepPlaces()
{
	return shok_playerattractionhandler_getfreesleepplaces(this);
}
int GGL::CPlayerAttractionHandler::GetUsedSleepPlaces()
{
	return GetSleepPlaceLimit() - GetFreeSleepPlaces(); // 0x4C22A2 does the same
}
static inline int(__thiscall* const shok_playerattractionhandler_getnummissingsleeplaces)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C2D62);
int GGL::CPlayerAttractionHandler::GetNumberOfWorkersWithoutSleepPlace()
{
	return shok_playerattractionhandler_getnummissingsleeplaces(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getfarmplacelimit)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C22B9);
int GGL::CPlayerAttractionHandler::GetFarmPlaceLimit()
{
	return shok_playerattractionhandler_getfarmplacelimit(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_getfreefarmplaces)(GGL::CPlayerAttractionHandler* th, bool uk) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*, bool)>(0x4C3C4C);
int GGL::CPlayerAttractionHandler::GetFreeFarmPlace()
{
	return shok_playerattractionhandler_getfreefarmplaces(this, true);
}
int GGL::CPlayerAttractionHandler::GetUsedFarmPlaces()
{
	return GetFarmPlaceLimit() - GetFreeFarmPlace(); // 0x4C3D90 does the same
}
static inline int(__thiscall* const shok_playerattractionhandler_getnummissingfarmlaces)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C2D8B);
int GGL::CPlayerAttractionHandler::GetNumberOfWorkersWithoutFarmPlace()
{
	return shok_playerattractionhandler_getnummissingfarmlaces(this);
}
static inline int(__thiscall* const shok_playerattractionhandler_gettimetopayday)(GGL::CPlayerAttractionHandler* th) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*)>(0x4C1AF4);
int GGL::CPlayerAttractionHandler::GetTimeToNextPayday()
{
	return shok_playerattractionhandler_gettimetopayday(this);
}
static inline float(__thiscall* const shok_playerattractionhandler_getavmoti)(GGL::CPlayerAttractionHandler* th, int z) = reinterpret_cast<float(__thiscall* const)(GGL::CPlayerAttractionHandler*, int)>(0x4B5494);
float GGL::CPlayerAttractionHandler::GetAverageMotivation()
{
	return shok_playerattractionhandler_getavmoti(this, 0);
}

static inline int(__thiscall* const shok_playerattractionhandler_attachworkers)(GGL::CPlayerAttractionHandler* th, bool force) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*, bool)>(0x4C4302);
void GGL::CPlayerAttractionHandler::CheckWorkerAttachment(bool forceReAttach)
{
	shok_playerattractionhandler_attachworkers(this, forceReAttach);
}

inline bool(__thiscall* const playerattractionhand_attachwork)(GGL::CPlayerAttractionHandler* th, shok::EntityId w, shok::EntityId b) = reinterpret_cast<bool(__thiscall*)(GGL::CPlayerAttractionHandler*, shok::EntityId, shok::EntityId)>(0x4C383E);
bool GGL::CPlayerAttractionHandler::AttachWorker(shok::EntityId worker, shok::EntityId building)
{
	return playerattractionhand_attachwork(this, worker, building);
}

shok::EntityId GGL::CPlayerAttractionHandler::PerformSpawnWorker(GGL::CBuilding* workplace, GGL::CBuilding* spawner)
{
	shok::Position spawnPos{};
	spawner->GetDoorPosAbs(&spawnPos);
	EGL::CGLEEntityCreator ct{};
	ct.EntityType = workplace->GetWorkerType();
	ct.Pos = spawnPos;
	ct.PlayerId = workplace->PlayerId;
	shok::EntityId worker = (*EGL::CGLEGameLogic::GlobalObj)->CreateEntity(&ct);
	if (worker == shok::EntityId::Invalid)
		return shok::EntityId::Invalid;
	EGL::CGLEEntity::GetEntityByID(worker)->AttachEntity(shok::AttachmentType::SETTLER_ENTERED_BUILDING, spawner->EntityId, shok::EventIDs::NoDetachEvent, shok::EventIDs::NoDetachEvent);
	if (AttachWorker(worker, workplace->EntityId))
		return worker;
	return shok::EntityId::Invalid;
}

inline int(__thiscall* const playerattractionhand_getusageofvect)(const GGL::CPlayerAttractionHandler* th, const shok::Vector<shok::EntityId>* v) = reinterpret_cast<int(__thiscall*)(const GGL::CPlayerAttractionHandler*, const shok::Vector<shok::EntityId>*)>(0x4C2335);
int GGL::CPlayerAttractionHandler::GetAttractionUsageOfVector(const shok::Vector<shok::EntityId>& v) const
{
	return playerattractionhand_getusageofvect(this, &v);
}

void (*GGL::CPlayerAttractionHandler::OnCheckPayDayCallback)(GGL::CPlayerAttractionHandler* th) = nullptr;
void __thiscall GGL::CPlayerAttractionHandler::CheckPaydayHook()
{
	if (GGL::CPlayerAttractionHandler::OnCheckPayDayCallback)
		GGL::CPlayerAttractionHandler::OnCheckPayDayCallback(this);
	GGL::CEventGoodsTraded ev{ shok::EventIDs::CppLogicEvent_OnPayday, shok::ResourceType::Gold, shok::ResourceType::GoldRaw,
		static_cast<float>(GetWorkerPaydayIncome()), static_cast<shok::EntityId>(static_cast<int>(PlayerID)), static_cast<float>(GetLeaderPaydayCost()) };
	(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
}
void __declspec(naked) hookedcheckpayday() {
	__asm {
		mov ecx, esi;
		call GGL::CPlayerAttractionHandler::CheckPaydayHook;

		mov esi, [esi + 4];
		mov[ebp + 0x18], 0x1300E;
		push 0x4C275E;
		ret;
	}
}
bool HookCheckPayday_Hooked = false;
void GGL::CPlayerAttractionHandler::HookCheckPayday()
{
	if (HookCheckPayday_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		throw 0;
	HookCheckPayday_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4C2754), 0x4C275E - 0x4C2754 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4C2754), &hookedcheckpayday, reinterpret_cast<void*>(0x4C275E));
}

inline shok::EntityId(__thiscall* const playerattractionhandl_getnearestreachable)(GGL::CPlayerAttractionHandler* th, shok::Vector<shok::EntityId>* v, shok::Position* p) = reinterpret_cast<shok::EntityId(__thiscall*)(GGL::CPlayerAttractionHandler*, shok::Vector<shok::EntityId>*, shok::Position*)>(0x4C205C);
void __thiscall GGL::CPlayerAttractionHandler::CheckWorkerSpawnHook()
{
	struct Data {
		shok::EntityTypeId WorkerType;
		shok::EntityId Workplace;
		float Weight;
	};

	EGL::CGLEGameLogic* gl = *EGL::CGLEGameLogic::GlobalObj;
	if ((gl->GetTick() % (1000 / 100 * (*GGL::CPlayerAttractionProps::GlobalObj)->AttractionFrequency)) != 0) {
		return;
	}
	if (VillageCenterArray.size() == 0 && HeadquarterArray.size() == 0)
		return;
	shok::List<Data> data{};
	reinterpret_cast<void(__thiscall*)(GGL::CPlayerAttractionHandler*, shok::List<Data>*)>(0x4C46BB)(this, &data);
	for (const auto& d : data) {
		auto* wp = BB::IdentifierCast<GGL::CBuilding>(EGL::CGLEEntity::GetEntityByID(d.Workplace));
		shok::Position approachpos = wp->GetAbsoluteApproachPos();
		shok::EntityId spawner = playerattractionhandl_getnearestreachable(this, &VillageCenterArray, &approachpos);
		if (spawner == shok::EntityId::Invalid)
			spawner = playerattractionhandl_getnearestreachable(this, &HeadquarterArray, &approachpos);
		if (spawner == shok::EntityId::Invalid)
			continue;
		auto* sp = BB::IdentifierCast<GGL::CBuilding>(EGL::CGLEEntity::GetEntityByID(spawner));
		CppLogic::Events::CanBuySettlerEvent ev{ shok::EventIDs::CppLogicEvent_CanBuySettler, spawner, d.Workplace, d.WorkerType,
			AIPlayerFlag || (GetAttractionUsage() < GetAttractionLimit()), true, !IsMotivationLocked(), !sp->WorkerAlarmModeActive };
		(*EScr::CScriptTriggerSystem::GlobalObj)->RunTrigger(&ev);
		if (ev.Motivation && ev.VCPop && ev.Alarm)
			PerformSpawnWorker(wp, sp);
		return;
	}
}
bool HookWorkerSpawn_Hooked = false;
void GGL::CPlayerAttractionHandler::HookWorkerSpawn()
{
	if (HookWorkerSpawn_Hooked)
		return;
	if (CppLogic::HasSCELoader())
		throw 0;
	HookWorkerSpawn_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4C4AB7), 10 };
	CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x4C4AB7), CppLogic::Hooks::MemberFuncPointerToVoid(&GGL::CPlayerAttractionHandler::CheckWorkerSpawnHook, 0));
}

int __thiscall GGL::CPlayerAttractionHandler::CannonsInProgressAttraction() const
{
	if (!AttractionCannonInProgress)
		return 0;
	int i = 0;
	for (shok::EntityId id : WorkBuildingsArray) {
		const EGL::CGLEEntity* e = EGL::CGLEEntity::GetEntityByID(id);
		auto* f = e->GetBehavior<GGL::CFoundryBehavior>();
		if (f->CannonType != shok::EntityTypeId::Invalid) {
			const auto* et = (*EGL::CGLEEntitiesProps::GlobalObj)->GetEntityType(f->CannonType);
			const auto* sp = BB::IdentifierCast<GGL::CGLSettlerProps>(et->LogicProps);
			if (sp != nullptr)
				i += sp->AttractionSlots;
		}
	}
	return i;
}
void __declspec(naked) CannonsInProgressAttraction_asm() {
	__asm {
		mov eax, 0x4C2335;
		call eax;
		add edi, eax;
		mov ecx, esi;
		call GGL::CPlayerAttractionHandler::CannonsInProgressAttraction;
		add eax, edi;

		push 0x4C27DD;
		ret;
	};
}
bool GGL::CPlayerAttractionHandler::AttractionCannonInProgress = true;
bool AttractionCannonInProgress_Hooked = false;
void GGL::CPlayerAttractionHandler::HookAttractionCannonInProgress()
{
	if (AttractionCannonInProgress_Hooked)
		return;
	AttractionCannonInProgress_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4C27D6), 10 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4C27D6), &CannonsInProgressAttraction_asm, reinterpret_cast<void*>(0x4C27DD));
}

static inline shok::UpgradeCategoryId(__thiscall* const upgrademanager_getucatbybuilding)(GGL::CUpgradeManager* th, shok::EntityTypeId id) = reinterpret_cast<shok::UpgradeCategoryId(__thiscall*)(GGL::CUpgradeManager*, shok::EntityTypeId)>(0x4B3CA6);
shok::UpgradeCategoryId GGL::CUpgradeManager::GetUpgradeCategoryOfEntityType(shok::EntityTypeId etype)
{
	return upgrademanager_getucatbybuilding(this, etype);
}

static inline shok::EntityTypeId(__thiscall* const upmanager_getetybyucat)(GGL::CUpgradeManager* th, shok::UpgradeCategoryId ucat) = reinterpret_cast<shok::EntityTypeId(__thiscall*)(GGL::CUpgradeManager*, shok::UpgradeCategoryId)>(0x4B3280);
shok::EntityTypeId GGL::CUpgradeManager::GetTypeByUCat(shok::UpgradeCategoryId ucat)
{
	return upmanager_getetybyucat(this, ucat);
}

static inline void(__thiscall* const upmanager_addcat)(GGL::CUpgradeManager* th, shok::UpgradeCategoryId ucat, shok::EntityTypeId ety) = reinterpret_cast<void(__thiscall*)(GGL::CUpgradeManager*, shok::UpgradeCategoryId, shok::EntityTypeId)>(0x4B3D42);
void GGL::CSettlerUpgradeManager::AddCategory(shok::UpgradeCategoryId ucat, shok::EntityTypeId firstEntity)
{
	upmanager_addcat(this, ucat, firstEntity);
}

static inline GGL::CBuildingUpgradeManager::ScholarInfo* (__thiscall* const buildupmanager_getscolarinfo)(void* th, shok::UpgradeCategoryId* ucat) = reinterpret_cast<GGL::CBuildingUpgradeManager::ScholarInfo * (__thiscall*)(void*, shok::UpgradeCategoryId*)>(0x4CA5A2);
void GGL::CBuildingUpgradeManager::AddCategory(shok::UpgradeCategoryId ucat, shok::EntityTypeId firstEntity, int maxscholar)
{
	upmanager_addcat(this, ucat, firstEntity);
	auto* i = buildupmanager_getscolarinfo(&ScholarInfoElement, &ucat);
	i->CurrentAmount = 0;
	i->MaxAmount = maxscholar;
}

static inline void(__thiscall* const trademng_resdata_adjust)(GGL::CTradeManager::ResData* th, float a, bool b) = reinterpret_cast<void(__thiscall*)(GGL::CTradeManager::ResData*, float, bool)>(0x4C866C);
void GGL::CTradeManager::ResData::AdjustPrice(float amount, bool buyed)
{
	trademng_resdata_adjust(this, amount, buyed);
}

static inline void(__thiscall* const tradeorder_setdata)(GGL::CTradeManager::TradeOrder* th, shok::PlayerId pl, shok::ResourceType s, shok::ResourceType bu, float am) = reinterpret_cast<void(__thiscall*)(GGL::CTradeManager::TradeOrder*, shok::PlayerId, shok::ResourceType, shok::ResourceType, float)>(0x4E7CFF);
void GGL::CTradeManager::TradeOrder::SetData(shok::PlayerId player, shok::ResourceType sellTy, shok::ResourceType buyTy, float buyAm)
{
	tradeorder_setdata(this, player, sellTy, buyTy, buyAm);
}
static inline bool(__thiscall* const tradeorder_isvalid)(GGL::CTradeManager::TradeOrder* th) = reinterpret_cast<bool(__thiscall*)(GGL::CTradeManager::TradeOrder*)>(0x516E6E);
bool GGL::CTradeManager::TradeOrder::IsTradeValid()
{
	return tradeorder_isvalid(this);
}
static inline float(__thiscall* const tradeorder_getpmax)(GGL::CTradeManager::TradeOrder* th) = reinterpret_cast<float(__thiscall*)(GGL::CTradeManager::TradeOrder*)>(0x516EFD);
float GGL::CTradeManager::TradeOrder::GetProgressMax()
{
	return tradeorder_getpmax(this);
}
bool GGL::CTradeManager::TradeOrder::IsFinished()
{
	return GetProgressMax() <= ProgressAmount; // also 0x5170B3
}
static inline bool(__thiscall* const tradeorder_remsell)(GGL::CTradeManager::TradeOrder* th) = reinterpret_cast<bool(__thiscall*)(GGL::CTradeManager::TradeOrder*)>(0x516F45);
bool GGL::CTradeManager::TradeOrder::RemoveSellRes()
{
	return tradeorder_remsell(this);
}
static inline bool(__thiscall* const tradeorder_complete)(GGL::CTradeManager::TradeOrder* th, bool rs) = reinterpret_cast<bool(__thiscall*)(GGL::CTradeManager::TradeOrder*, bool)>(0x517027);
void GGL::CTradeManager::TradeOrder::Complete(bool removeSell)
{
	tradeorder_complete(this, removeSell);
}
static inline bool(__thiscall* const tradeorder_adv)(GGL::CTradeManager::TradeOrder* th, float p) = reinterpret_cast<bool(__thiscall*)(GGL::CTradeManager::TradeOrder*, float)>(0x517080);
void GGL::CTradeManager::TradeOrder::AdvanceProgress(float p)
{
	tradeorder_adv(this, p);
}

static inline void(__thiscall* const trademng_applytrade)(GGL::CTradeManager* th, const GGL::CTradeManager::TradeOrder* o) = reinterpret_cast<void(__thiscall*)(GGL::CTradeManager*, const GGL::CTradeManager::TradeOrder*)>(0x4C8903);
void GGL::CTradeManager::ApplyPriceChangesOfTrade(const TradeOrder* o)
{
	trademng_applytrade(this, o);
}

static inline GGL::CTradeManager::ResData* (__thiscall* const trademng_getres)(GGL::CTradeManager* th, shok::ResourceType rt) = reinterpret_cast<GGL::CTradeManager::ResData * (__thiscall*)(GGL::CTradeManager*, shok::ResourceType)>(0x4C8732);
GGL::CTradeManager::ResData* GGL::CTradeManager::GetResource(shok::ResourceType rt)
{
	return trademng_getres(this, rt);
}

GGL::CTechConditionPredicate::CTechConditionPredicate()
{
	*reinterpret_cast<int*>(this) = vtp;
}
GGL::CPreConditionPredicate::CPreConditionPredicate()
{
	*reinterpret_cast<int*>(this) = vtp;
}

inline bool(__thiscall* const techmng_addprograw)(GGL::PlayerTechManager* th, shok::TechnologyId t, float a) = reinterpret_cast<bool(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId, float)>(0x4A1A29);
bool GGL::PlayerTechManager::AddTechProgressRaw(shok::TechnologyId techId, float amount)
{
	return techmng_addprograw(this, techId, amount);
}
inline void(__thiscall* const techmng_researched)(GGL::PlayerTechManager* th, shok::TechnologyId t, shok::EntityId id) = reinterpret_cast<void(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId, shok::EntityId)>(0x4A1C6D);
void GGL::PlayerTechManager::TechResearched(shok::TechnologyId techId, shok::EntityId researcherId)
{
	techmng_researched(this, techId, researcherId);
}
inline void(__thiscall* const techmng_addprogworker)(GGL::PlayerTechManager* th, shok::TechnologyId t, float a) = reinterpret_cast<void(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId, float)>(0x4A1D02);
void GGL::PlayerTechManager::AddTechProgressWorker(shok::TechnologyId techId, float amount)
{
	techmng_addprogworker(this, techId, amount);
}
inline bool(__thiscall* const techmng_checktechreq)(GGL::PlayerTechManager* th, shok::TechnologyId t, const GGL::CTechConditionPredicate* p) = reinterpret_cast<bool(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId, const GGL::CTechConditionPredicate*)>(0x4A2640);
bool GGL::PlayerTechManager::CheckTechnologyRequirementsFor(shok::TechnologyId tech, const CTechConditionPredicate& p)
{
	return techmng_checktechreq(this, tech, &p);
}
inline bool(__thiscall* const techmng_checkallreq)(GGL::PlayerTechManager* th, shok::TechnologyId t, const GGL::CTechConditionPredicate* p) = reinterpret_cast<bool(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId, const GGL::CTechConditionPredicate*)>(0x4A2772);
bool GGL::PlayerTechManager::CheckAllRequirementsFor(shok::TechnologyId tech, const CTechConditionPredicate& p)
{
	return techmng_checkallreq(this, tech, &p);
}
inline shok::TechState(__thiscall* const techmng_getstate)(GGL::PlayerTechManager* th, shok::TechnologyId t) = reinterpret_cast<shok::TechState(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId)>(0x4A2A8D);
shok::TechState GGL::PlayerTechManager::GetTechState(shok::TechnologyId tech)
{
	return techmng_getstate(this, tech);
}
inline void(__thiscall* const techmng_forceresearch)(GGL::PlayerTechManager* th, shok::TechnologyId t) = reinterpret_cast<void(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId)>(0x4A1CE0);
void GGL::PlayerTechManager::ForceResearch(shok::TechnologyId tech)
{
	return techmng_forceresearch(this, tech);
}

void GGL::PlayerTechManager::ForceResearchNoFeedback(shok::TechnologyId tech)
{
	int te = static_cast<int>(tech);
	TechnologyState[te].TechStatus = shok::TechState::Researched;
	TechnologyState[te].ResearchProgress = 0;

	lua::State L{ *EScr::CScriptTriggerSystem::GameState };
	int t = L.GetTop();
	L.Push("GameCallback_OnTechnologyResearched");
	L.GetGlobal();
	L.Push(static_cast<int>(PlayerID));
	L.Push(te);
	L.Push(0);
	L.PCall(3, 0);
	L.SetTop(t);

	(*GGL::CGLGameLogic::GlobalObj)->GetPlayer(PlayerID)->Statistics.AddTechResearched(tech);
}

static inline void(__thiscall* const tributemanager_setdata)(GGL::PlayerTributesManager* th, int tid, const shok::CostInfo* c, shok::EntityId ownerent, shok::PlayerId offeringpl, const char* txt) = reinterpret_cast<void(__thiscall*)(GGL::PlayerTributesManager*, int, const shok::CostInfo*, shok::EntityId, shok::PlayerId, const char*)>(0x4BE63E);
void GGL::PlayerTributesManager::SetTributeData(int tid, const shok::CostInfo& c, shok::EntityId ownerEntityId, shok::PlayerId offeringPlayerId, const char* text)
{
	tributemanager_setdata(this, tid, &c, ownerEntityId, offeringPlayerId, text);
}
static inline bool(__thiscall* const tributemanager_remove)(GGL::PlayerTributesManager* th, int tid) = reinterpret_cast<bool(__thiscall*)(GGL::PlayerTributesManager*, int)>(0x4BE520);
bool GGL::PlayerTributesManager::RemoveTribute(int tid)
{
	return tributemanager_remove(this, tid);
}

static inline void(__thiscall* const questmanager_setdata)(GGL::PlayerQuestManager* th, int qid, GGL::PlayerQuestManager::QuestStatus qty, const char* name, const char* txt, const shok::Position* p, int info) = reinterpret_cast<void(__thiscall*)(GGL::PlayerQuestManager*, int, GGL::PlayerQuestManager::QuestStatus, const char*, const char*, const shok::Position*, int)>(0x4C8474);
void GGL::PlayerQuestManager::SetQuestData(int questId, QuestStatus questType, const char* name, const char* text, const shok::Position* pos, bool info)
{
	questmanager_setdata(this, questId, questType, name, text, pos, info);
}
static inline void(__thiscall* const questmanager_settype)(GGL::PlayerQuestManager* th, int qid, GGL::PlayerQuestManager::QuestStatus qty, int info) = reinterpret_cast<void(__thiscall*)(GGL::PlayerQuestManager*, int, GGL::PlayerQuestManager::QuestStatus, int)>(0x4C8564);
void GGL::PlayerQuestManager::SetQuestType(int questId, QuestStatus questType, bool info)
{
	questmanager_settype(this, questId, questType, info);
}
static inline void(__thiscall* const questmanager_setpos)(GGL::PlayerQuestManager* th, int qid, const shok::Position* p, int info) = reinterpret_cast<void(__thiscall*)(GGL::PlayerQuestManager*, int, const shok::Position*, int)>(0x4C80B4);
void GGL::PlayerQuestManager::SetQuestPosition(int questId, const shok::Position* pos, bool info)
{
	questmanager_setpos(this, questId, pos, info);
}
static inline bool(__thiscall* const questmanager_removequest)(GGL::PlayerQuestManager* th, int qid, int info) = reinterpret_cast<bool(__thiscall*)(GGL::PlayerQuestManager*, int, int)>(0x4C818E);
bool GGL::PlayerQuestManager::RemoveQuest(int questId, bool info)
{
	return questmanager_removequest(this, questId, info);
}

unsigned int __stdcall GGL::CResourceStatistics::GetClassIdentifier() const
{
	return 0xC1488787;
}

unsigned int __stdcall GGL::CGameStatistics::GetClassIdentifier() const
{
	return 0x964BEA57;
}

inline void(__thiscall* const gamestatistics_addtech)(GGL::CGameStatistics* th, shok::TechnologyId t) = reinterpret_cast<void(__thiscall*)(GGL::CGameStatistics*, shok::TechnologyId)>(0x4C15A1);
void GGL::CGameStatistics::AddTechResearched(shok::TechnologyId tech)
{
	gamestatistics_addtech(this, tech);
}
inline void(__thiscall* const gamestatistics_resrefined)(GGL::CGameStatistics* th, shok::ResourceType rt, float am) = reinterpret_cast<void(__thiscall*)(GGL::CGameStatistics*, shok::ResourceType, float)>(0x4E101E);
void GGL::CGameStatistics::OnResRefined(shok::ResourceType rt, float am)
{
	gamestatistics_resrefined(this, rt, am);
}
inline void(__thiscall* const gamestatistics_resmined)(GGL::CGameStatistics* th, shok::ResourceType rt, float am) = reinterpret_cast<void(__thiscall*)(GGL::CGameStatistics*, shok::ResourceType, float)>(0x4E6544);
void GGL::CGameStatistics::OnResMined(shok::ResourceType rt, float am)
{
	gamestatistics_resmined(this, rt, am);
}

static inline shok::DiploState(__thiscall* const shok_GGL_CPlayerStatus_getDiploState)(int* d, shok::PlayerId p) = reinterpret_cast<shok::DiploState(__thiscall*)(int* d, shok::PlayerId p)>(0x4B4D5B);
shok::DiploState GGL::CPlayerStatus::GetDiploStateTo(shok::PlayerId p)
{
	return shok_GGL_CPlayerStatus_getDiploState(DiplomacyData, p);
}

static inline shok::TechState(__thiscall* const playerstatus_gettechstatus)(GGL::PlayerTechManager* th, shok::TechnologyId tech) = reinterpret_cast<shok::TechState(__thiscall*)(GGL::PlayerTechManager*, shok::TechnologyId)>(0x4A2A8D);
shok::TechState GGL::CPlayerStatus::GetTechStatus(shok::TechnologyId tech)
{
	return playerstatus_gettechstatus(&TechnologyStates, tech);
}

static inline int(__thiscall* const playerstatus_gettaxperworker)(GGL::CPlayerStatus* th) = reinterpret_cast<int(__thiscall*)(GGL::CPlayerStatus*)>(0x4B4AC5);
int GGL::CPlayerStatus::GetTaxPerWorker()
{
	return playerstatus_gettaxperworker(this);
}

static inline int(__thiscall* const playerstatus_getlevytaxperworker)(GGL::CPlayerStatus* th) = reinterpret_cast<int(__thiscall*)(GGL::CPlayerStatus*)>(0x4B4A93);
int GGL::CPlayerStatus::GetLevyTaxPerWorker()
{
	return playerstatus_getlevytaxperworker(this);
}
static inline int(__thiscall* const playerstatus_getlevytax)(GGL::CPlayerStatus* th) = reinterpret_cast<int(__thiscall*)(GGL::CPlayerStatus*)>(0x4B4AA9);
int GGL::CPlayerStatus::GetLevyTaxAmount()
{
	return playerstatus_getlevytax(this);
}
inline float(__thiscall* const playerstatus_getmaxmoti)(GGL::CPlayerStatus* th) = reinterpret_cast<float(__thiscall*)(GGL::CPlayerStatus*)>(0x4B4AE9);
float GGL::CPlayerStatus::GetMaxMotivation()
{
	return playerstatus_getmaxmoti(this);
}
inline void(__thiscall* const playerstatus_changemoti)(GGL::CPlayerStatus* th, float delta, shok::WorkerReason reason) = reinterpret_cast<void(__thiscall*)(GGL::CPlayerStatus*, float, shok::WorkerReason)>(0x4B5401);
void GGL::CPlayerStatus::ChangeMotivationForAllWorkers(float delta, shok::WorkerReason reason)
{
	playerstatus_changemoti(this, delta, reason);
}
inline void(__thiscall* const playerstatus_changemaxmoti)(GGL::CPlayerStatus* th, float deltan) = reinterpret_cast<void(__thiscall*)(GGL::CPlayerStatus*, float)>(0x4B587F);
void GGL::CPlayerStatus::ChangeMaxMotivation(float delta)
{
	playerstatus_changemaxmoti(this, delta);
}

static inline bool(__thiscall* const playerstatus_setstatus)(GGL::CPlayerStatus* th, GGL::CPlayerStatus::PlayerStatus s) = reinterpret_cast<bool(__thiscall*)(GGL::CPlayerStatus*, GGL::CPlayerStatus::PlayerStatus)>(0x4B4B38);
bool GGL::CPlayerStatus::SetState(PlayerStatus s)
{
	return playerstatus_setstatus(this, s);
}

inline bool(__thiscall* const playerstatus_hasresfeedback)(GGL::CPlayerStatus* th, const shok::CostInfo* c, bool fb) = reinterpret_cast<bool(__thiscall*)(GGL::CPlayerStatus*, const shok::CostInfo*, bool)>(0x4B619D);
bool GGL::CPlayerStatus::HasResourcesFeedback(const shok::CostInfo& c, bool feedback)
{
	return playerstatus_hasresfeedback(this, &c, feedback);
}

bool GGL::CPlayerStatus::ArePlayersHostile(shok::PlayerId p1, shok::PlayerId p2)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p1);
	if (!ps)
		return false;
	return ps->GetDiploStateTo(p2) == shok::DiploState::Hostile;
}
bool GGL::CPlayerStatus::ArePlayersFriendly(shok::PlayerId p1, shok::PlayerId p2)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p1);
	if (!ps)
		return false;
	return ps->GetDiploStateTo(p2) == shok::DiploState::Friendly;
}

bool (*GGL::CPlayerStatus::CanPlaceBuildingCallback)(shok::EntityTypeId entitytype, shok::PlayerId player, shok::Position* pos, float rotation, shok::EntityId buildOnId) = nullptr;
int __stdcall GGL::CPlayerStatus::CanPlaceBuildingHook(shok::EntityTypeId entitytype, shok::PlayerId player, shok::Position* pos, float rotation, shok::EntityId buildOnId)
{
	if (GGL::CPlayerStatus::CanPlaceBuildingCallback)
		return GGL::CPlayerStatus::CanPlaceBuildingCallback(entitytype, player, pos, rotation, buildOnId);
	return true;
}
void __declspec(naked) canplacebuildingasm() {
	__asm {

		test bl, bl;
		jz retu;

		push[ebp + 0x18];
		push[ebp + 0x14];
		push[ebp + 0x10];
		push[ebp + 0xC];
		push[ebp + 0x8];
		call GGL::CPlayerStatus::CanPlaceBuildingHook;
		jmp modif;

	retu:
		mov al, bl;

	modif:
		mov ecx, [ebp + 0xC];
		pop ebx;
		push 0x4B45BF;
		ret;
	}
}
bool HookCanPlaceBuilding_Hooked = false;
void GGL::CPlayerStatus::HookCanPlaceBuilding()
{
	if (HookCanPlaceBuilding_Hooked)
		return;
	HookCanPlaceBuilding_Hooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4B45B9), 20 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x4B45B9), &canplacebuildingasm, reinterpret_cast<void*>(0x4B45BF));
}

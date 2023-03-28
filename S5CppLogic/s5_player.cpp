#include "pch.h"
#include <cmath>
#include "s5_player.h"
#include "s5_defines.h"
#include "s5_maplogic.h"
#include "s5_events.h"
#include "s5_scriptsystem.h"
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
static inline int(__thiscall* const shok_playerattractionhandler_getfreefarmplaces)(GGL::CPlayerAttractionHandler* th, bool uk) = reinterpret_cast<int(__thiscall* const)(GGL::CPlayerAttractionHandler*,bool)>(0x4C3C4C);
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

void (*GGL::CPlayerAttractionHandler::OnCheckPayDayCallback)(GGL::CPlayerAttractionHandler* th) = nullptr;
void __thiscall GGL::CPlayerAttractionHandler::CheckPaydayHook()
{
	if (GGL::CPlayerAttractionHandler::OnCheckPayDayCallback)
		GGL::CPlayerAttractionHandler::OnCheckPayDayCallback(this);
	GGL::CEventGoodsTraded ev{ shok::EventIDs::CppLogicEvent_OnPayday, shok::ResourceType::Gold, shok::ResourceType::GoldRaw,
		static_cast<float>(GetWorkerPaydayIncome()), PlayerID, static_cast<float>(GetLeaderPaydayCost())};
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

static inline int(__thiscall* const upgrademanager_getucatbybuilding)(GGL::CUpgradeManager* th, int id) = reinterpret_cast<int(__thiscall*)(GGL::CUpgradeManager*, int)>(0x4B3CA6);
int GGL::CUpgradeManager::GetUpgradeCategoryOfEntityType(int etype)
{
	return upgrademanager_getucatbybuilding(this, etype);
}

static inline int(__thiscall* const upmanager_getetybyucat)(GGL::CUpgradeManager* th, int ucat) = reinterpret_cast<int(__thiscall*)(GGL::CUpgradeManager*, int)>(0x4B3280);
int GGL::CUpgradeManager::GetTypeByUCat(int ucat)
{
	return upmanager_getetybyucat(this, ucat);
}

static inline void(__thiscall* const upmanager_addcat)(GGL::CUpgradeManager* th, int ucat, int ety) = reinterpret_cast<void(__thiscall*)(GGL::CUpgradeManager*, int, int)>(0x4B3D42);
void GGL::CSettlerUpgradeManager::AddCategory(int ucat, int firstEntity)
{
	upmanager_addcat(this, ucat, firstEntity);
}

static inline GGL::CBuildingUpgradeManager::ScholarInfo* (__thiscall* const buildupmanager_getscolarinfo)(void* th, int* ucat) = reinterpret_cast<GGL::CBuildingUpgradeManager::ScholarInfo * (__thiscall*)(void*, int*)>(0x4CA5A2);
void GGL::CBuildingUpgradeManager::AddCategory(int ucat, int firstEntity, int maxscholar)
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

static inline void(__thiscall* const tradeorder_setdata)(GGL::CTradeManager::TradeOrder* th, int pl, shok::ResourceType s, shok::ResourceType bu, float am) = reinterpret_cast<void(__thiscall*)(GGL::CTradeManager::TradeOrder*, int, shok::ResourceType, shok::ResourceType, float)>(0x4E7CFF);
void GGL::CTradeManager::TradeOrder::SetData(int player, shok::ResourceType sellTy, shok::ResourceType buyTy, float buyAm)
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

inline bool(__thiscall* const techmng_addprograw)(GGL::PlayerTechManager* th, int t, float a) = reinterpret_cast<bool(__thiscall*)(GGL::PlayerTechManager*, int, float)>(0x4A1A29);
bool GGL::PlayerTechManager::AddTechProgressRaw(int techId, float amount)
{
	return techmng_addprograw(this, techId, amount);
}
inline void(__thiscall* const techmng_researched)(GGL::PlayerTechManager* th, int t, int id) = reinterpret_cast<void(__thiscall*)(GGL::PlayerTechManager*, int, int)>(0x4A1C6D);
void GGL::PlayerTechManager::TechResearched(int techId, int researcherId)
{
	techmng_researched(this, techId, researcherId);
}
inline void(__thiscall* const techmng_addprogworker)(GGL::PlayerTechManager* th, int t, float a) = reinterpret_cast<void(__thiscall*)(GGL::PlayerTechManager*, int, float)>(0x4A1D02);
void GGL::PlayerTechManager::AddTechProgressWorker(int techId, float amount)
{
	techmng_addprogworker(this, techId, amount);
}

static inline void(__thiscall* const tributemanager_setdata)(GGL::PlayerTributesManager* th, int tid, const shok::CostInfo* c, int ownerent, int offeringpl, const char* txt) = reinterpret_cast<void(__thiscall*)(GGL::PlayerTributesManager*, int, const shok::CostInfo*, int, int, const char*)>(0x4BE63E);
void GGL::PlayerTributesManager::SetTributeData(int tid, const shok::CostInfo& c, int ownerEntityId, int offeringPlayerId, const char* text)
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

static inline shok::DiploState(__thiscall* const shok_GGL_CPlayerStatus_getDiploState)(int* d, int p) = reinterpret_cast<shok::DiploState(__thiscall*)(int* d, int p)>(0x4B4D5B);
shok::DiploState GGL::CPlayerStatus::GetDiploStateTo(int p)
{
	return shok_GGL_CPlayerStatus_getDiploState(DiplomacyData, p);
}

static inline shok::TechState(__thiscall* const playerstatus_gettechstatus)(GGL::PlayerTechManager* th, int tech) = reinterpret_cast<shok::TechState(__thiscall*)(GGL::PlayerTechManager*, int)>(0x4A2A8D);
shok::TechState GGL::CPlayerStatus::GetTechStatus(int tech)
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

static inline bool(__thiscall* const playerstatus_setstatus)(GGL::CPlayerStatus* th, GGL::CPlayerStatus::PlayerStatus s) = reinterpret_cast<bool(__thiscall*)(GGL::CPlayerStatus*, GGL::CPlayerStatus::PlayerStatus)>(0x4B4B38);
bool GGL::CPlayerStatus::SetState(PlayerStatus s)
{
	return playerstatus_setstatus(this, s);
}


bool GGL::CPlayerStatus::ArePlayersHostile(int p1, int p2)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p1);
	if (!ps)
		return false;
	return ps->GetDiploStateTo(p2) == shok::DiploState::Hostile;
}
bool GGL::CPlayerStatus::ArePlayersFriendly(int p1, int p2)
{
	GGL::CPlayerStatus* ps = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(p1);
	if (!ps)
		return false;
	return ps->GetDiploStateTo(p2) == shok::DiploState::Friendly;
}

bool (*GGL::CPlayerStatus::CanPlaceBuildingCallback)(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId) = nullptr;
int __stdcall GGL::CPlayerStatus::CanPlaceBuildingHook(int entitytype, int player, shok::Position* pos, float rotation, int buildOnId)
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

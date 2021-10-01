#include "pch.h"
#include "s5data.h"
#include <stdexcept>


struct shok_vtable_ECS_CManager {
	PADDINGI(2);
	void(__thiscall* ReloadCutscene)(shok_ECS_CManager* th, const char* path);
};

struct shok_vtable_EGL_CGLEGameLogic {
	PADDINGI(22);
	int(__thiscall* CreateEntity)(shok_EGL_CGLEGameLogic* th, shok_EGL_CGLEEntityCreator* data, int i); // 22
	int(__thiscall* CreateEffect)(shok_EGL_CGLEGameLogic* th, shok_EGL_CGLEEffectCreator* data); // 23
};

struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(shok_BB_IPostEvent* th, shok_BB_CEvent* ev);
};

struct shok_vtable_GGL_CWeatherHandler {
	PADDINGI(1);
	int(__thiscall* GetCurrentWeather)(shok_GGL_CWeatherHandler* th);
};

struct shok_vtable_shok_BB_CFileSystemMgr {
	PADDINGI(4);
	void* (__thiscall* OpenFileAsStream)(shok_BB_CFileSystemMgr* th, const char* path, int mode); // use via shok_BB_CFileStreamEx
	bool(__thiscall* OpenAsFileHandle)(shok_BB_CFileSystemMgr* th, const char* path, int* pHandle, size_t* pSize);
	void(__thiscall* AddArchive)(shok_BB_CFileSystemMgr* th, const char* Path, bool OnTop); // 6
	PADDINGI(1);
	void(__thiscall* AddFolder)(shok_BB_CFileSystemMgr* th, const char* path, bool OnTop, int unknown); // 8
	PADDINGI(1);
	void(__thiscall* RemoveTop)(shok_BB_CFileSystemMgr* th); // 10
	PADDINGI(1);
	void(__thiscall* MakeAbsolute)(shok_BB_CFileSystemMgr* th, char* absol, char* relati); // 12
};
//constexpr int i = offsetof(shok_vtable_shok_BB_CFileSystemMgr, OpenAsFileHandle);

struct shok_vtable_ED_CLandscape {
	void(__thiscall* Destroy)(shok_ED_CLandscape* th, bool free);
	PADDINGI(1); // concurrency ref counter destroy?
	float(__thiscall* GetTerrainHeightAtPos)(shok_ED_CLandscape* th, float x, float y);
	float(__thiscall* GetWaterHeightAtPos)(shok_ED_CLandscape* th, float x, float y);
	float(__thiscall* GetHigherTerrainOrWaterHeightAtPos)(shok_ED_CLandscape* th, float x, float y);
	bool(__thiscall* GetLandscapePosFromMousePos)(shok_ED_CLandscape* th, void* cam, float* mousepos, shok_positionRot* outpos, int flag);
	PADDINGI(1); // ret 0.0f, thiscall 3 args
};
//constexpr int i = offsetof(shok_vtable_ED_CLandscape, GetLandscapePosFromMousePos) / 4;

struct shok_vtable_ED_CCommandAcknowledgements {
	PADDINGI(3);
	void(__thiscall* ShowAck)(shok_ED_CCommandAcknowledgements* th, float x, float y);
};

struct shok_vtable_ED_CResourceManager {
	PADDINGI(6);
	shok_modeldata* (__thiscall* GetModelDataByEntityType)(shok_ED_CResourceManager* th, int ety); // 6
	shok_modeldata* (__thiscall* GetModelDataByModelID)(shok_ED_CResourceManager* th, int mid);
};
//constexpr int i = offsetof(shok_vtable_ED_CResourceManager, GetModelDataByModelType) / 4;

shok_GGlue_CGlueEntityProps* shok_EGL_CGLEEntitiesProps::GetEntityType(int i)
{
	if (i <= 0 || i >= (int)EntityTypes.size())
		return nullptr;
	return EntityTypes.data() + i;
}

const char* (__stdcall* const getentitydisplayname)(int i) = reinterpret_cast<const char* (__stdcall* const)(int i)>(0x52EFCF);
const char* shok_EGL_CGLEEntitiesProps::GetEntityTypeDisplayName(int i)
{
	return getentitydisplayname(i);
}

void shok_ECS_CManager::ReloadCutscene(const char* path)
{
	reinterpret_cast<shok_vtable_ECS_CManager*>(vtable)->ReloadCutscene(this, path);
}

static inline bool(__thiscall* const shok_EGL_CGLEEffectManager_IsEffectValid)(shok_EGL_CGLEEffectManager* th, int id) = reinterpret_cast<bool(__thiscall*)(shok_EGL_CGLEEffectManager*, int)>(0x4FAABD);
bool shok_EGL_CGLEEffectManager::IsEffectValid(int id)
{
	return shok_EGL_CGLEEffectManager_IsEffectValid(this, id);
}

static inline shok_EGL_CEffect* (__thiscall* const shok_effid2obj)(shok_EGL_CGLEEffectManager* th, int id) = reinterpret_cast<shok_EGL_CEffect * (__thiscall*)(shok_EGL_CGLEEffectManager*, int)>(0x4FAAE3);
shok_EGL_CEffect* shok_EGL_CGLEEffectManager::GetEffectById(int id)
{
	return shok_effid2obj(this, id);
}

static inline int(__thiscall* const shok_getAnimIdByName)(shok_BB_CIDManagerEx* th, const char* name) = reinterpret_cast<int(__thiscall*)(shok_BB_CIDManagerEx * th, const char* name)>(0x54F19E);
static inline int(__thiscall* const shok_BB_CIDManager_getidbyname)(void* th, const char* name, int nid) = reinterpret_cast<int(__thiscall*)(void*, const char*, int)>(0x54F656);
int shok_BB_CIDManagerEx::GetIdByName(const char* name) {
	return shok_getAnimIdByName(this, name);
}
const char* shok_BB_CIDManagerEx::GetNameByID(int id)
{
	if (id > 0 && id < static_cast<int>(TypeNames.size()))
		return TypeNames[id].Name;
	return nullptr;
}
int shok_BB_CIDManagerEx::GetIDByNameOrCreate(const char* name, int newid)
{
	return shok_BB_CIDManager_getidbyname(this, name, newid);
}
int shok_BB_CIDManagerEx::GetIDByNameOrCreate(const char* name)
{
	return GetIDByNameOrCreate(name, 0);
}

void shok_BB_CIDManagerEx::RemoveID(int id)
{
	shok_saveVector<shok_BB_CIDManagerEx_data>(&TypeNames, [id](std::vector<shok_BB_CIDManagerEx_data, shok_allocator<shok_BB_CIDManagerEx_data>>& v) {
		shok_free(v.at(id).Name);
		v[id].Name = nullptr;
		v[id].Hash = 0;
		if (id == static_cast<int>(v.size()-1))
			v.pop_back();
		});
}
static inline void(__stdcall* const idmanager_writemngtoglobal)(lua_State* L, const char* global, shok_BB_CIDManagerEx* mng) = reinterpret_cast<void(__stdcall*)(lua_State*, const char*, shok_BB_CIDManagerEx*)>(0x59C128);
void shok_BB_CIDManagerEx::DumpManagerToLuaGlobal(lua_State* L, const char* global)
{
	idmanager_writemngtoglobal(L, global, this);
}

int shok_BB_CIDManager::GetIDByName(const char* name, int newid)
{
	return shok_BB_CIDManager_getidbyname(this, name, newid);
}
int shok_BB_CIDManager::GetIDByName(const char* name)
{
	return GetIDByName(name, 0);
}

void shok_EGL_CTerrainVertexColors::ToTerrainCoord(shok_position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool shok_EGL_CTerrainVertexColors::IsCoordValid(int* out)
{
	return out[0] >= 0 && out[1] >= 0 && (out[0] + 1) < ArraySizeX && (out[1] + 1) < ArraySizeY;
}
int shok_EGL_CTerrainVertexColors::GetTerrainVertexColor(shok_position& p)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return VertexColors[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)];
}
void(__thiscall* const terrvertexcol_setvcol)(shok_EGL_CTerrainVertexColors* th, int x, int y, int* col) = reinterpret_cast<void(__thiscall*)(shok_EGL_CTerrainVertexColors*, int, int, int*)>(0x580582);
void shok_EGL_CTerrainVertexColors::SetTerrainVertexColor(shok_position& p, int col)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	terrvertexcol_setvcol(this, qp[0], qp[1], &col);
}

void shok_EGL_CGLETerrainHiRes::ToTerrainCoord(shok_position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool shok_EGL_CGLETerrainHiRes::IsCoordValid(int* out)
{
	return out[0] >= 0 && out[1] >= 0 && out[0] < MaxSizeX && out[1] < MaxSizeY;
}
int shok_EGL_CGLETerrainHiRes::GetTerrainHeight(shok_position& p)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return TerrainHeights[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)];
}
void(__thiscall* const terrhires_setheight)(shok_EGL_CGLETerrainHiRes* th, int* qp, int16_t h) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLETerrainHiRes*, int*, int16_t)>(0x591B53);
void shok_EGL_CGLETerrainHiRes::SetTerrainHeight(shok_position& p, int h)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	terrhires_setheight(this, qp, h);
}

void shok_EGL_CGLETerrainLowRes::ToQuadCoord(const shok_position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100) / 4);
	out[1] = static_cast<int>(std::lroundf(p.Y / 100) / 4);
}
bool shok_EGL_CGLETerrainLowRes::IsCoordValid(const int* out)
{
	return out[0] >= 0 && out[1] >= 0 && out[0] < MaxSizeX && out[1] < MaxSizeY;
}
int shok_EGL_CGLETerrainLowRes::GetTerrainTypeAt(const shok_position& p)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return Data[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] & 0xFF;
}
void(__thiscall* const terrlores_settty)(shok_EGL_CGLETerrainLowRes* th, int* qp, int h) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLETerrainLowRes*, int*, int)>(0x58BBCE);
void shok_EGL_CGLETerrainLowRes::SetTerrainTypeAt(const shok_position& p, int tty)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	terrlores_settty(this, qp, tty);
}
int shok_EGL_CGLETerrainLowRes::GetWaterTypeAt(const shok_position& p)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return (Data[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] & 0x3F00) >> 8;
}
void(__thiscall* const terrlores_setwty)(shok_EGL_CGLETerrainLowRes* th, int* qp, int h) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLETerrainLowRes*, int*, int)>(0x591BB9);
void shok_EGL_CGLETerrainLowRes::SetWaterTypeAt(const shok_position& p, int wty)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	terrlores_setwty(this, qp, wty);
}
int shok_EGL_CGLETerrainLowRes::GetWaterHeightAt(const shok_position& p)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return (Data[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] & 0x3FFFC000) >> 14;
}
void(__thiscall* const terrlores_setwh)(shok_EGL_CGLETerrainLowRes* th, int* qp, int h) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLETerrainLowRes*, int*, int)>(0x591B82);
void shok_EGL_CGLETerrainLowRes::SetWaterHeightAt(const shok_position& p, int wh)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	terrlores_setwh(this, qp, wh);
}
int shok_EGL_CGLETerrainLowRes::GetBridgeHeight(const shok_position& p)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return BridgeHeights[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)];
}
void shok_EGL_CGLETerrainLowRes::SetBridgeHeight(const shok_position& p, int bh)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	BridgeHeights[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] = bh;
}
inline int* shok_EGL_CGLETerrainLowRes::GetBridgeHeightP(const int* qp)
{
	return &BridgeHeights[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)];
}

static inline int(__thiscall* const shokLandscape_getSector)(shok_EGL_CGLELandscape* th, const shok_position* p) = reinterpret_cast<int(__thiscall*)(shok_EGL_CGLELandscape*, const shok_position*)>(0x5778BE);
int shok_EGL_CGLELandscape::GetSector(const shok_position* p)
{
	return shokLandscape_getSector(this, p);
}

static inline void(__thiscall* const shoklandscape_getnearestcon)(int* pred, bool* one, int sector, shok_EGL_CGLELandscape* l) = reinterpret_cast<void(__thiscall*)(int*, bool*, int, shok_EGL_CGLELandscape*)>(0x57F253);
static inline bool(__thiscall* const shoklandscape_getnearest)(shok_EGL_CGLELandscape* th, const shok_position* pIn, float range, int* pred, shok_position* pOut) = reinterpret_cast<bool(__thiscall*)(shok_EGL_CGLELandscape*, const shok_position*, float, int*, shok_position*)>(0x5775AF);
bool shok_EGL_CGLELandscape::GetNearestPositionInSector(const shok_position* pIn, float range, int sector, shok_position* pOut)
{
	int pred[5];
	bool one = true;
	shoklandscape_getnearestcon(pred, &one, sector, this);
	pOut->X = 0.0f;
	pOut->Y = 0.0f;
	return shoklandscape_getnearest(this, pIn, range, pred, pOut);
}
static inline void(__thiscall* const shoklandscape_getnearestfreepos)(shok_EGL_CGLELandscape* ls, shok_position* ou, const shok_position* in, float r, int* d) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLELandscape*, shok_position*, const shok_position*, float, int*)>(0x577661);
shok_position shok_EGL_CGLELandscape::GetNearestFreePos(const shok_position* p, float range)
{
	shok_position r;
	int i = 1;
	shoklandscape_getnearestfreepos(this, &r, p, range, &i);
	return r;
}
static inline bool(__thiscall* const shok_EGL_CGLELandscape_isvalidpos)(shok_EGL_CGLELandscape* ls, shok_position* p) = reinterpret_cast<bool(__thiscall*)(shok_EGL_CGLELandscape*, shok_position*)>(0x5785BC);
bool shok_EGL_CGLELandscape::IsValidPos(shok_position* p)
{
	return shok_EGL_CGLELandscape_isvalidpos(this, p);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_getsize)(shok_EGL_CGLELandscape* ls, shok_position* p) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLELandscape*, shok_position*)>(0x578589);
shok_position shok_EGL_CGLELandscape::GetMapSize()
{
	shok_position r;
	shok_EGL_CGLELandscape_getsize(this, &r);
	return r;
}
static inline bool(__thiscall* const shok_EGL_CGLELandscape_isposblockedinmode)(shok_EGL_CGLELandscape* ls, const shok_position* p, shok_EGL_CGLELandscape::BlockingMode* mode) = reinterpret_cast<bool(__thiscall*)(shok_EGL_CGLELandscape*, const shok_position*, shok_EGL_CGLELandscape::BlockingMode*)>(0x57772E);
bool shok_EGL_CGLELandscape::IsPosBlockedInMode(const shok_position* p, BlockingMode mode)
{
	return shok_EGL_CGLELandscape_isposblockedinmode(this, p, &mode);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_flattenpos)(shok_EGL_CGLELandscape* th, const shok_position* p, const shok_position* a, const shok_position* b, float r) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLELandscape*, const shok_position*, const shok_position*, const shok_position*, float)>(0x579AFA);
void shok_EGL_CGLELandscape::FlattenPosForBuilding(const shok_position& p, const shok_AARect& area, float rot)
{
	shok_EGL_CGLELandscape_flattenpos(this, &p, &area.low, &area.high, rot);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_applyblocking)(shok_EGL_CGLELandscape* th, const shok_position* p, const shok_position* a, const shok_position* b, float r, shok_EGL_CGLELandscape::BlockingMode* m) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLELandscape*, const shok_position*, const shok_position*, const shok_position*, float, shok_EGL_CGLELandscape::BlockingMode*)>(0x577A82);
void shok_EGL_CGLELandscape::ApplyBlocking(const shok_position& p, const shok_AARect& area, float rot, BlockingMode blockingmode)
{
	shok_EGL_CGLELandscape_applyblocking(this, &p, &area.low, &area.high, rot, &blockingmode);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_remblocking)(shok_EGL_CGLELandscape* th, const shok_position* p, const shok_position* a, const shok_position* b, float r, shok_EGL_CGLELandscape::BlockingMode* m) = reinterpret_cast<void(__thiscall*)(shok_EGL_CGLELandscape*, const shok_position*, const shok_position*, const shok_position*, float, shok_EGL_CGLELandscape::BlockingMode*)>(0x577B41);
void shok_EGL_CGLELandscape::RemoveBlocking(const shok_position& p, const shok_AARect& area, float rot, BlockingMode blockingmode)
{
	shok_EGL_CGLELandscape_remblocking(this, &p, &area.low, &area.high, rot, &blockingmode);
}
void shok_EGL_CGLELandscape::AdvancedApplyBridgeHeight(const shok_position& p, const shok_AARect& area, float rot, int height)
{
	shok_position pl = area.low;
	shok_position ph = area.high;
	// todo rotate positions
	if (pl.X > ph.X)
		std::swap(pl.X, ph.X);
	if (pl.Y > ph.Y)
		std::swap(pl.Y, ph.Y);
	pl = p - pl;
	ph = p + ph;

	int low[2] = { 0,0 };
	int high[2] = { 0,0 };
	LowRes->ToQuadCoord(pl, low);
	LowRes->ToQuadCoord(ph, high);

	int curr[2];
	for (curr[1] = low[1] - 4; curr[1] < high[1] + 4; curr[1]++) {
		for (curr[0] = low[0] - 4; curr[0] < high[0] + 4; curr[0]++) {
			if (!LowRes->IsCoordValid(curr))
				continue;
			shok_position bch = { static_cast<float>(curr[1] * 100 * 4), static_cast<float>(curr[0] * 100 * 4) };
			bool before = IsPosBlockedInMode(&bch, BlockingMode::BridgeArea);
			bool toadd = !(curr[1] < low[1] || curr[1] > high[1] || curr[0] < low[0] || curr[0] > high[0]);
			int* h = LowRes->GetBridgeHeightP(curr);
			if (!before) {
				*h = height;
			}
			else if (toadd) {
				*h = max(*h, height);
			}
		}
	}

	ApplyBlocking(p, area, 0, BlockingMode::BridgeArea); // set r after implememting rotation
	RemoveBlocking(p, area, 0, BlockingMode::Blocked);
}

int shok_EGL_CGLEGameLogic::CreateEffect(shok_EGL_CGLEEffectCreator* data) {
	return reinterpret_cast<shok_vtable_EGL_CGLEGameLogic*>(vtable)->CreateEffect(this, data);
}

int shok_EGL_CGLEGameLogic::CreateEntity(shok_EGL_CGLEEntityCreator* cr)
{
	return reinterpret_cast<shok_vtable_EGL_CGLEGameLogic*>(vtable)->CreateEntity(this, cr, 1);
}

int shok_EGL_CGLEGameLogic::GetTimeMS()
{
	return InGameTime[0] * InGameTime[1];
}

int shok_EGL_CGLEGameLogic::GetTick()
{
	return InGameTime[0];
}

int(__thiscall* CreateEffectHookedOrig)(shok_EGL_CGLEGameLogic* th, shok_EGL_CGLEEffectCreator* data) = nullptr;
void(*shok_EGL_CGLEGameLogic::CreateEffectHookCallback)(int id, void* ret) = nullptr;
int __fastcall CreateEffectHook(shok_EGL_CGLEGameLogic* th, int _, shok_EGL_CGLEEffectCreator* data)
{
	void** ebp_var = (void**)1;
	_asm {
		mov ebp_var, ebp;
	}
	int id = CreateEffectHookedOrig(th, data);
	if (shok_EGL_CGLEGameLogic::CreateEffectHookCallback)
		shok_EGL_CGLEGameLogic::CreateEffectHookCallback(id, ebp_var[1]);
	return id;
}
void shok_EGL_CGLEGameLogic::HookCreateEffect()
{
	if (CreateEffectHookedOrig)
		return;
	shok_vtable_EGL_CGLEGameLogic* vt = reinterpret_cast<shok_vtable_EGL_CGLEGameLogic*>(vtable);
	shok_saveVirtualProtect vp{ vt, 25 * 4 };
	CreateEffectHookedOrig = vt->CreateEffect;
	vt->CreateEffect = reinterpret_cast<int(__thiscall*)(shok_EGL_CGLEGameLogic * th, shok_EGL_CGLEEffectCreator * data)>(&CreateEffectHook);
}


void shok_ED_CGlobalsLogicEx::ToTerrainCoord(const shok_position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool shok_ED_CGlobalsLogicEx::IsCoordValid(int* out)
{
	return out[0] >= 0 && out[1] >= 0 && out[0] < Blocking->ArraySizeXY && out[1] < Blocking->ArraySizeXY;
}
shok_EGL_CGLELandscape::BlockingMode shok_ED_CGlobalsLogicEx::GetBlocking(const shok_position& p)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return static_cast<shok_EGL_CGLELandscape::BlockingMode>(Blocking->data[qp[1] * Blocking->ArraySizeXY + qp[0]]);
}
bool shok_ED_CLandscape::GetTerrainPosAtScreenCoords(shok_positionRot& outpos, int x, int y)
{
	float mp[] = { static_cast<float>(x), static_cast<float>(y) };
	return reinterpret_cast<shok_vtable_ED_CLandscape*>(vtable)->GetLandscapePosFromMousePos(this, (*shok_ED_CGlobalsBaseEx::GlobalObj)->Camera->SomeCameraData, mp, &outpos, 3);
}

shok_EGL_CGLEEntity* shok_EGL_CGLEEntityManager::GetEntityByNum(int num)
{
	if (EntityCount <= num)
		return nullptr;
	return Entities[num].entity;
}

static inline shok_GGL_CPlayerStatus* (__thiscall* const shok_GGL_CGLGameLogic_GetPlayer)(shok_GGL_CPlayerStatus** pl, int p) = reinterpret_cast<shok_GGL_CPlayerStatus * (__thiscall*) (shok_GGL_CPlayerStatus**, int)>(0x4A91BC);
shok_GGL_CPlayerStatus* shok_GGL_CGLGameLogic::GetPlayer(int i)
{
	return shok_GGL_CGLGameLogic_GetPlayer(this->players, i);
}

shok_technology* shok_GGL_CGLGameLogic::GetTech(int i)
{
	try {
		return TechList->TechList.at(i - 1);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

static inline void(__thiscall* const cglgamelogic_enablealarm)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventPlayerID* d) = reinterpret_cast<void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventPlayerID*)>(0x49FA14);
void shok_GGL_CGLGameLogic::EnableAlarmForPlayer(int pl)
{
	shok_EGL_CNetEventPlayerID ev{ shok_NetEventIds::CommandPlayerActivateAlarm, pl };
	cglgamelogic_enablealarm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_disablealarm)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventPlayerID* d) = reinterpret_cast<void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventPlayerID*)>(0x49FAD7);
void shok_GGL_CGLGameLogic::DisableAlarmForPlayer(int pl)
{
	shok_EGL_CNetEventPlayerID ev{ shok_NetEventIds::CommandPlayerDeactivateAlarm, pl };
	cglgamelogic_disablealarm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_upgradesettlercat)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventIntegerAndPlayerID*)>(0x4985C5);
void shok_GGL_CGLGameLogic::UpgradeSettlerCategory(int pl, int ucat)
{
	shok_EGL_CNetEventIntegerAndPlayerID ev{ shok_NetEventIds::PlayerUpgradeSettlerCategory, pl, ucat };
	cglgamelogic_upgradesettlercat(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_activateweatherm)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventIntegerAndPlayerID*)>(0x49BF7A);
void shok_GGL_CGLGameLogic::PlayerActivateWeathermachine(int player, int weathertype)
{
	shok_EGL_CNetEventIntegerAndPlayerID ev{ shok_NetEventIds::CommandWeathermachineChangeWeather, player, weathertype };
	cglgamelogic_activateweatherm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_blesssettlers)(shok_GGL_CGLGameLogic* th, shok_EGL_CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(shok_GGL_CGLGameLogic*, shok_EGL_CNetEventIntegerAndPlayerID*)>(0x49B7E6);
void shok_GGL_CGLGameLogic::PlayerBlessSettlers(int player, int blessCat)
{
	shok_EGL_CNetEventIntegerAndPlayerID ev{ shok_NetEventIds::CommandMonasteryBlessSettlerGroup, player, blessCat };
	cglgamelogic_blesssettlers(this, &ev);
}

static inline int(__thiscall* const weatherdata_getnext)(shok_GGL_CWeatherHandler* th) = reinterpret_cast<int(__thiscall*)(shok_GGL_CWeatherHandler*)>(0x4B93BD);
int shok_GGL_CWeatherHandler::GetNextWeatherState()
{
	return weatherdata_getnext(this);
}

int shok_GGL_CWeatherHandler::GetCurrentWeatherState()
{
	return ((shok_vtable_GGL_CWeatherHandler*)vtable)->GetCurrentWeather(this);
}

static inline int(__thiscall* const weatherdata_gettimetonext)(shok_GGL_CWeatherHandler* th) = reinterpret_cast<int(__thiscall*)(shok_GGL_CWeatherHandler*)>(0x4B9309);
int shok_GGL_CWeatherHandler::GetTicksToNextPeriodicWeatherChange()
{
	return weatherdata_gettimetonext(this);
}

static inline void(__thiscall* const weatherdata_addelement)(shok_GGL_CWeatherHandler* th, int state, int dur, bool peri, int fore, int gfx, int trans) = reinterpret_cast<void(__thiscall*)(shok_GGL_CWeatherHandler*, int, int, bool, int, int, int)>(0x4B95A8);
void shok_GGL_CWeatherHandler::AddWeatherElement(int state, int dur, bool peri, int forerun, int gfx, int transition)
{
	weatherdata_addelement(this, state, dur, peri, forerun, gfx, transition);
}

static inline void(__thiscall* const weatherdata_elements_remove)(shok_set<shok_GGL_CWeatherHandler_KeyAndWeatherElement>* th, int* ind) = reinterpret_cast<void(__thiscall*)(shok_set<shok_GGL_CWeatherHandler_KeyAndWeatherElement>*, int*)>(0x513D32);
static inline void(__thiscall* const weatherdata_elements_add)(shok_set<shok_GGL_CWeatherHandler_KeyAndWeatherElement>* th, shok_GGL_CWeatherHandler_weatherElement* e) = reinterpret_cast<void(__thiscall*)(shok_set<shok_GGL_CWeatherHandler_KeyAndWeatherElement>*, shok_GGL_CWeatherHandler_weatherElement*)>(0x513CB6);
void shok_GGL_CWeatherHandler::ClearQueue(int state, int dur, int forerun, int gfx, int transition)
{
	// save data
	shok_GGL_CWeatherHandler_weatherElement *current;
	shok_GGL_CWeatherHandler_weatherElement *changeto;
	std::vector<int> win_toremove{};
	Elements.ForAll([this, &win_toremove, &current, &changeto](shok_GGL_CWeatherHandler_KeyAndWeatherElement* kae) {
		if (this->CurrentWeatherIndex == kae->WeatherIndex)
			current = &kae->WeatherElement;
		else if (this->WeatherChange.WIndexToChangeTo == kae->WeatherIndex)
			changeto = &kae->WeatherElement;
		else
			win_toremove.push_back(kae->WeatherIndex);
		});
	// remove all elements
	for (int i : win_toremove) {
		int a = i;
		weatherdata_elements_remove(&Elements, &a);
	}
	// initalize first element
	NextPeriodicWeatherStartTimeOffset = 0;
	shok_GGL_CWeatherHandler_weatherElement initial{};
	initial.State = state;
	initial.Length = dur;
	initial.Forerun = forerun;
	initial.GfxSet = gfx;
	initial.Transition = transition;
	initial.IsPeriodic = true;
	initial.StartTimeOffset = 0;
	initial.WeatherIndex = NextWeatherIndex;
	NextWeatherIndex++;
	weatherdata_elements_add(&Elements, &initial);
	// add transition nonperiodics
	if (WeatherChange.State == 1) { // weatherchange in progress
		current->IsPeriodic = false; // always delete
		if (changeto->IsPeriodic) { // remove asap
			changeto->IsPeriodic = false;
			changeto->Length = changeto->Transition;
			changeto->StartTimeOffset = 1;
		}
		// keep changeto nonperiodic
	}
	else {
		if (current->IsPeriodic) {
			current->IsPeriodic = false;
			current->Length = current->Transition;
			current->StartTimeOffset = 1;
		}
		// keep nonperiodic as is
	}
}

void shok_EScr_CScriptTriggerSystem::RunTrigger(shok_BB_CEvent* ev)
{
	reinterpret_cast<shok_vtable_BB_IPostEvent*>(PostEvent.vtable)->PostEvent(&PostEvent, ev);
}

static inline void(__cdecl* const filesystem_addfolder)(const char* p, char* d) = reinterpret_cast<void(__cdecl*)(const char*, char*)>(0x546514);
void shok_BB_CFileSystemMgr::AddFolder(const char* path)
{
	filesystem_addfolder(path, nullptr);
	size_t l = LoadOrder.size();
	shok_BB_IFileSystem* last = LoadOrder[l - 1];
	for (int i = l - 1; i > 0; i--) {
		LoadOrder[i] = LoadOrder[i - 1];
	}
	LoadOrder[0] = last;
}

void shok_BB_CFileSystemMgr::AddArchive(const char* path)
{
	reinterpret_cast<shok_vtable_shok_BB_CFileSystemMgr*>(vtable)->AddArchive(this, path, 1);
}

void shok_BB_CFileSystemMgr::RemoveTopArchive()
{
	reinterpret_cast<shok_vtable_shok_BB_CFileSystemMgr*>(vtable)->RemoveTop(this);
}

bool shok_BB_CFileSystemMgr::OpenFileAsHandle(const char* path, int& handle, size_t& size)
{
	return reinterpret_cast<shok_vtable_shok_BB_CFileSystemMgr*>(vtable)->OpenAsFileHandle(this, path, &handle, &size);
}
static inline bool(__cdecl* const file_closehandle)(int h) = reinterpret_cast<bool(__cdecl*)(int)>(0x54E9EC);
bool shok_BB_CFileSystemMgr::CloseHandle(int handle)
{
	return file_closehandle(handle);
}

int shok_ED_CPlayerColors::GetColorByIndex(int i)
{
	// todo 16 player
	if (i < 0 || i > 16)
		return 0;
	return Colors[i];
}
void shok_ED_CPlayerColors::SetColorByIndex(int i, int c)
{
	if (i < 0 || i > 16)
		return;
	Colors[i] = c;
	(*shok_ED_CGlobalsBaseEx::GlobalObj)->DisplayProps->MiniMapColor.data()[i] = c;
}
static inline void(__thiscall* const playercolors_refresh)(shok_ED_CPlayerColors* th) = reinterpret_cast<void(__thiscall*)(shok_ED_CPlayerColors*)>(0x4699B9);
void shok_ED_CPlayerColors::RefreshPlayerColors()
{
	playercolors_refresh(this);
}

void shok_ED_CCommandAcknowledgements::ShowAck(const shok_position& pos)
{
	reinterpret_cast<shok_vtable_ED_CCommandAcknowledgements*>(vtable)->ShowAck(this, pos.X, pos.Y);
}

static inline void(__cdecl* const registermodelinst)(void* robj, shok_modelinstance* i) = reinterpret_cast<void(__cdecl*)(void*, shok_modelinstance*)>(0x627130);
void shok_modelinstance::Register()
{
	registermodelinst((*shok_ED_CGlobalsBaseEx::GlobalObj)->DisplayWorld->SomeRenderObj, this);
}
static inline void*(__cdecl* const modelinst_getsomethingdtor)(shok_modelinstance* i) = reinterpret_cast<void*(__cdecl*)(shok_modelinstance*)>(0x626E00);
static inline void(__cdecl* const modelinst_deregister)(void* d, shok_modelinstance* i) = reinterpret_cast<void(__cdecl*)(void*, shok_modelinstance*)>(0x6271C0);
static inline void(__cdecl* const modelinst_dest)(shok_modelinstance* i) = reinterpret_cast<void(__cdecl*)(shok_modelinstance*)>(0x6294A0);
void shok_modelinstance::Destroy()
{
	void* d = modelinst_getsomethingdtor(this);
	if (d) {
		modelinst_deregister(d, this);
		modelinst_dest(this);
	}
}
static inline void(__cdecl* const modelinst_setrot)(void* l, float* f, float r, int i) = reinterpret_cast<void(__cdecl*)(void*, float*, float, int)>(0x4141D0);
void shok_modelinstance::SetRotation(float r)
{
	float f[] = { 0,0,1 };
	modelinst_setrot(Loc, f, r, 0);
}
static inline void(__cdecl* const modelinst_setscale)(void* l, float* f, int i) = reinterpret_cast<void(__cdecl*)(void*, float*, int)>(0x414170);
void shok_modelinstance::SetScale(float* s)
{
	modelinst_setscale(Loc, s, 2);
}
void shok_modelinstance::SetScale(float s)
{
	float f[] = { s,s,s };
	SetScale(f);
}
static inline void(__cdecl* const modelinst_setpos)(void* l, float* f, int i) = reinterpret_cast<void(__cdecl*)(void*, float*, int)>(0x414140);
void shok_modelinstance::SetPosition(const shok_position& p, float height)
{
	float f[] = { p.X, p.Y, height };
	modelinst_setpos(Loc, f, 2);
}

static inline shok_modelinstance* (__thiscall* const modeldata_instanciate)(const shok_modeldata* d) = reinterpret_cast<shok_modelinstance*(__thiscall*)(const shok_modeldata*)> (0x472742);
shok_modelinstance* shok_modeldata::Instanciate() const
{
	return modeldata_instanciate(this);
}
const shok_modeldata* shok_ED_CResourceManager::GetModelData(int modelid)
{
	return reinterpret_cast<shok_vtable_ED_CResourceManager*>(vtable)->GetModelDataByModelID(this, modelid);
}

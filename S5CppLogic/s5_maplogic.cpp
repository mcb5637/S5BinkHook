#include "pch.h"
#include <stdexcept>
#include "s5_maplogic.h"
#include "s5_netevents.h"
#include "s5_mapdisplay.h"
#include "s5_entitytype.h"
#include "s5_entity.h"
#include "s5_mem.h"
#include "s5_defines.h"
#include "s5_netevents.h"
#include "entityiterator.h"
#include "hooks.h"


struct shok_vtable_EGL_CGLEGameLogic {
	PADDINGI(22);
	int(__thiscall* CreateEntity)(EGL::CGLEGameLogic* th, EGL::CGLEEntityCreator* data, int i); // 22
	int(__thiscall* CreateEffect)(EGL::CGLEGameLogic* th, EGL::CGLEEffectCreator* data); // 23
};



void EGL::CTerrainVertexColors::ToTerrainCoord(shok::Position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool EGL::CTerrainVertexColors::IsCoordValid(int* out)
{
	return out[0] >= 0 && out[1] >= 0 && (out[0] + 1) < ArraySizeX && (out[1] + 1) < ArraySizeY;
}
int EGL::CTerrainVertexColors::GetTerrainVertexColor(shok::Position& p)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "GetTerrainVertexColor out of range" };
	return VertexColors[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)];
}
void(__thiscall* const terrvertexcol_setvcol)(EGL::CTerrainVertexColors* th, int x, int y, int* col) = reinterpret_cast<void(__thiscall*)(EGL::CTerrainVertexColors*, int, int, int*)>(0x580582);
void EGL::CTerrainVertexColors::SetTerrainVertexColor(shok::Position& p, int col)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "SetTerrainVertexColor out of range" };
	terrvertexcol_setvcol(this, qp[0], qp[1], &col);
}

void EGL::CGLETerrainHiRes::ToTerrainCoord(const shok::Position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool EGL::CGLETerrainHiRes::IsCoordValid(int x, int y)
{
	return x >= 0 && y >= 0 && x < MaxSizeX&& y < MaxSizeY;
}
bool EGL::CGLETerrainHiRes::IsCoordValid(int* out)
{
	return IsCoordValid(out[0], out[1]);
}
int EGL::CGLETerrainHiRes::GetTerrainHeight(const shok::Position& p)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "SetTerrainVertexColor out of range" };
	return TerrainHeights[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)];
}
void(__thiscall* const terrhires_setheight)(EGL::CGLETerrainHiRes* th, int* qp, int16_t h) = reinterpret_cast<void(__thiscall*)(EGL::CGLETerrainHiRes*, int*, int16_t)>(0x591B53);
void EGL::CGLETerrainHiRes::SetTerrainHeight(const shok::Position& p, int h)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "SetTerrainHeight out of range" };
	terrhires_setheight(this, qp, h);
}
int EGL::CGLETerrainHiRes::GetTerrainHeight(int x, int y) {
	return TerrainHeights[(y + 1) * ArraySizeY + (x + 1)];
}

void EGL::CGLETerrainLowRes::ToQuadCoord(const shok::Position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100) / 4);
	out[1] = static_cast<int>(std::lroundf(p.Y / 100) / 4);
}
bool EGL::CGLETerrainLowRes::IsCoordValid(int x, int y)
{
	return x >= 0 && y >= 0 && x < MaxSizeX&& y < MaxSizeY;
}
bool EGL::CGLETerrainLowRes::IsCoordValid(const int* out)
{
	return IsCoordValid(out[0], out[1]);
}
bool EGL::CGLETerrainLowRes::IsBridgeHeightCoordValid(int x, int y)
{
	if (HiResBridgeHeightEnabled) {
		return x >= 0 && y >= 0 && x < (MaxSizeX * 4) && y < (MaxSizeY * 4);
	}
	return x >= 0 && y >= 0 && x < MaxSizeX&& y < MaxSizeY;
}
int EGL::CGLETerrainLowRes::GetTerrainTypeAt(const shok::Position& p)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "GetTerrainTypeAt out of range" };
	return Data[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] & 0xFF;
}
void(__thiscall* const terrlores_settty)(EGL::CGLETerrainLowRes* th, int* qp, int h) = reinterpret_cast<void(__thiscall*)(EGL::CGLETerrainLowRes*, int*, int)>(0x58BBCE);
void EGL::CGLETerrainLowRes::SetTerrainTypeAt(const shok::Position& p, int tty)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "SetTerrainTypeAt out of range" };
	terrlores_settty(this, qp, tty);
}
int EGL::CGLETerrainLowRes::GetWaterTypeAt(const shok::Position& p)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "GetWaterTypeAt out of range" };
	return (Data[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] & 0x3F00) >> 8;
}
void(__thiscall* const terrlores_setwty)(EGL::CGLETerrainLowRes* th, int* qp, int h) = reinterpret_cast<void(__thiscall*)(EGL::CGLETerrainLowRes*, int*, int)>(0x591BB9);
void EGL::CGLETerrainLowRes::SetWaterTypeAt(const shok::Position& p, int wty)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "SetWaterTypeAt out of range" };
	terrlores_setwty(this, qp, wty);
}
int EGL::CGLETerrainLowRes::GetWaterHeightAt(const shok::Position& p)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "GetWaterHeightAt out of range" };
	return (Data[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] & 0x3FFFC000) >> 14;
}
void(__thiscall* const terrlores_setwh)(EGL::CGLETerrainLowRes* th, int* qp, int h) = reinterpret_cast<void(__thiscall*)(EGL::CGLETerrainLowRes*, int*, int)>(0x591B82);
void EGL::CGLETerrainLowRes::SetWaterHeightAt(const shok::Position& p, int wh)
{
	int qp[2] = { 0,0 };
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "SetWaterHeightAt out of range" };
	terrlores_setwh(this, qp, wh);
}
int EGL::CGLETerrainLowRes::GetBridgeHeight(const shok::Position& p)
{
	int qp[2] = { 0,0 };
	if (HiResBridgeHeightEnabled) {
		CheckBridgeHeightSize();
		EGL::CGLETerrainHiRes::ToTerrainCoord(p, qp);
		if (!IsBridgeHeightCoordValid(qp[0], qp[1]))
			throw std::out_of_range{ "GetBridgeHeight out of range" };
		return *GetBridgeHeightP(qp[0], qp[1]);
	}
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "GetBridgeHeight out of range" };
	return BridgeHeights[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)];
}
void EGL::CGLETerrainLowRes::SetBridgeHeight(const shok::Position& p, int bh)
{
	int qp[2] = { 0,0 };
	if (HiResBridgeHeightEnabled) {
		CheckBridgeHeightSize();
		EGL::CGLETerrainHiRes::ToTerrainCoord(p, qp);
		if (!IsBridgeHeightCoordValid(qp[0], qp[1]))
			throw std::out_of_range{ "SetBridgeHeight out of range" };
		*GetBridgeHeightP(qp[0], qp[1]) = bh;
		return;
	}
	ToQuadCoord(p, qp);
	if (!IsCoordValid(qp))
		throw std::out_of_range{ "SetBridgeHeight out of range" };
	BridgeHeights[(qp[1] + 1) * ArraySizeY + (qp[0] + 1)] = bh;
}
inline int* EGL::CGLETerrainLowRes::GetBridgeHeightP(int x, int y)
{
	if (HiResBridgeHeightEnabled) {
		CheckBridgeHeightSize();
		if (!IsBridgeHeightCoordValid(x, y))
			throw std::out_of_range{ "GetBridgeHeightP out of range" };
		return &Dbg_bh[(y + 1) * (MaxSizeY * 4 + 2) + (x + 1)];
	}
	if (!IsCoordValid(x, y))
		throw std::out_of_range{ "GetBridgeHeightP out of range" };
	return &BridgeHeights[(y + 1) * ArraySizeY + (x + 1)];
}
int EGL::CGLETerrainLowRes::GetWaterHeightAt(int x, int y)
{
	return (Data[(y + 1) * ArraySizeY + (x + 1)] & 0x3FFFC000) >> 14;
}
float EGL::CGLETerrainLowRes::GetBridgeHeightFloat(float x, float y)
{
	shok::Position p{ x,y };
	return static_cast<float>(GetBridgeHeight(p));
}


float __fastcall shok_ED_CLandscape_overridegetwaterheightatpos(ED::CLandscape* th, int _, float x, float y) { // this func breaks its arguments, so i have to rewrite it instead of patching a few instructions
	shok::Position p{ min(max(0, x), th->WorldSizeX),min(max(0, y), th->WorldSizeY) };
	if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->IsPosBlockedInMode(&p, EGL::CGLELandscape::BlockingMode::BridgeArea)) {
		return static_cast<float>(th->TerrainLowRes->GetBridgeHeight(p));
	}
	else {
		return static_cast<float>(th->TerrainLowRes->GetWaterHeightAt(p));
	}
}
void __declspec(naked) hiresbridgearea_somewaterregionfunc() {
	__asm {
		push[ebp - 0x4]; // y
		push[ebp + 0x8]; // x
		mov ecx, [ebp - 0xC];
		call EGL::CGLETerrainLowRes::GetBridgeHeightP;
		fild[eax];

		push 0x47D318; // should be 47D30A, but thats just a unconditional jmp
		ret;
	};
}
void __fastcall shok_bridge_applyheight(GGL::CBridgeEntity* th) {
	GGL::CBridgeProperties* p = static_cast<GGL::CBridgeProperties*>(th->GetEntityType()->LogicProps);
	auto* lr = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->LowRes;
	int h = (*EGL::CGLEGameLogic::GlobalObj)->Landscape->HiRes->GetTerrainHeight(th->Position) + p->Height;
	for (const shok::AARect& area : p->BridgeArea) {
		EGL::CGLELandscape::AdvancedAARectIterator it{ th->Position, area, th->Position.r, !EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled, true };
		for (const auto& c : it) {
			if (!lr->IsBridgeHeightCoordValid(c.x, c.y))
				continue;
			*lr->GetBridgeHeightP(c.x, c.y) = h;
		}
	}
}

bool EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled = false;
int* EGL::CGLETerrainLowRes::Dbg_bh = nullptr;
void EGL::CGLETerrainLowRes::EnableHiResBridgeHeight()
{
	if (EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled)
		return;
	EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x76A410), 10 };
	CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x47D301), 10 };
	CppLogic::Hooks::SaveVirtualProtect vp3{ reinterpret_cast<void*>(0x503C50), 10 };
	*reinterpret_cast<float(__fastcall**)(ED::CLandscape*, int, float, float)>(0x76A410) = &shok_ED_CLandscape_overridegetwaterheightatpos;
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x47D301), &hiresbridgearea_somewaterregionfunc, reinterpret_cast<void*>(0x47D30A));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x503C50), &shok_bridge_applyheight, reinterpret_cast<void*>(0x503C58));
}

void EGL::CGLETerrainLowRes::CheckBridgeHeightSize()
{
	if (!HiResBridgeHeightEnabled)
		return;
	if (Dbg_bh)
		return;
	int ary = (MaxSizeY * 4 + 2);
	int arx = (MaxSizeX * 4 + 2);
	int size = arx * ary;
	Dbg_bh = new int[size];
	memset(Dbg_bh, 0, size * sizeof(int));
	for (int y = 0; y < MaxSizeY; y++) {
		for (int x = 0; x < MaxSizeX; x++) {
			int h = BridgeHeights[(y + 1) * ArraySizeY + (x + 1)];
			int x2 = x * 4 + 1;
			int y2 = y * 4 + 1;
			for (int dy = 0; dy < 4; dy++) {
				for (int dx = 0; dx < 4; dx++) {
					Dbg_bh[(y2 + dy) * ary + (x2 + dx)] = h;
				}
			}
		}
	}
	int** vec = reinterpret_cast<int**>(&BridgeHeights);
	shok::Free(vec[1]);
	for (int i = 0; i < 4; i++)
		vec[i] = nullptr;
}
void EGL::CGLETerrainLowRes::ClearBridgeArea()
{
	if (Dbg_bh) {
		delete[] Dbg_bh;
		Dbg_bh = nullptr;
	}
}


EGL::CGLELandscape::AdvancedAARectIterator::AdvancedAARectIterator(const shok::Position& p, const shok::AARect& area, float rot, bool LowRes) : AdvancedAARectIterator(p, area, rot, LowRes, false)
{
}
EGL::CGLELandscape::AdvancedAARectIterator::AdvancedAARectIterator(const shok::Position& p, const shok::AARect& area, float rot, bool LowRes, bool AddOne)
{
	// TODO implement rotation
	shok::AARect rotated = area;
	rotated.low = area.low.Rotate(rot);
	rotated.high = area.high.Rotate(rot);
	shok::Position p2{ min(rotated.low.X, rotated.high.X) + p.X, min(rotated.low.Y, rotated.high.Y) + p.Y };
	int low[2] = { 0,0 };
	int high[2] = { 0,0 };
	if (LowRes)
		EGL::CGLETerrainLowRes::ToQuadCoord(p2, low);
	else
		EGL::CGLETerrainHiRes::ToTerrainCoord(p2, low);
	p2 = { max(rotated.low.X, rotated.high.X) + p.X, max(rotated.low.Y, rotated.high.Y) + p.Y };
	if (LowRes)
		EGL::CGLETerrainLowRes::ToQuadCoord(p2, high);
	else
		EGL::CGLETerrainHiRes::ToTerrainCoord(p2, high);
	Low = { low[0], low[1] };
	High = { high[0], high[1] };
	if (AddOne) {
		High.x++;
		High.y++;
	}
}
bool EGL::CGLELandscape::AdvancedAARectIterator::HasNext(const Coord& Curr) const
{
	return Curr.x < High.x&& Curr.y < High.y;
}
void EGL::CGLELandscape::AdvancedAARectIterator::ToNext(Coord& Curr) const
{
	Curr.x++;
	if (Curr.x >= High.x) {
		Curr.x = Low.x;
		Curr.y++;
	}
}
EGL::CGLELandscape::AdvancedAARectIterator::Iter EGL::CGLELandscape::AdvancedAARectIterator::begin() const {
	if (High.x == Low.x || High.y == Low.y)
		return end();
	return EGL::CGLELandscape::AdvancedAARectIterator::Iter(*this, Low);
}
EGL::CGLELandscape::AdvancedAARectIterator::Iter EGL::CGLELandscape::AdvancedAARectIterator::end() const
{
	return EGL::CGLELandscape::AdvancedAARectIterator::Iter(*this, { Low.x, High.y }); // {x-1, y-1}.next()
}
const EGL::CGLELandscape::AdvancedAARectIterator::Coord& EGL::CGLELandscape::AdvancedAARectIterator::Iter::operator*() const
{
	return Curr;
}
bool EGL::CGLELandscape::AdvancedAARectIterator::Iter::operator==(const Iter& o) const
{
	return this->Curr.x == o.Curr.x && this->Curr.y == o.Curr.y;
}
bool EGL::CGLELandscape::AdvancedAARectIterator::Iter::operator!=(const Iter& o) const
{
	return !(*this == o);
}
EGL::CGLELandscape::AdvancedAARectIterator::Iter& EGL::CGLELandscape::AdvancedAARectIterator::Iter::operator++()
{
	I->ToNext(Curr);
	return *this;
}
EGL::CGLELandscape::AdvancedAARectIterator::Iter EGL::CGLELandscape::AdvancedAARectIterator::Iter::operator++(int)
{
	Iter r = *this;
	++(*this);
	return r;
}
EGL::CGLELandscape::AdvancedAARectIterator::Iter::Iter(const AdvancedAARectIterator& i, const Coord& c)
{
	I = &i;
	Curr = c;
}

static inline int(__thiscall* const shokLandscape_getSector)(EGL::CGLELandscape* th, const shok::Position* p) = reinterpret_cast<int(__thiscall*)(EGL::CGLELandscape*, const shok::Position*)>(0x5778BE);
int EGL::CGLELandscape::GetSector(const shok::Position* p)
{
	return shokLandscape_getSector(this, p);
}

static inline void(__thiscall* const shoklandscape_getnearestcon)(int* pred, bool* one, int sector, EGL::CGLELandscape* l) = reinterpret_cast<void(__thiscall*)(int*, bool*, int, EGL::CGLELandscape*)>(0x57F253);
static inline bool(__thiscall* const shoklandscape_getnearest)(EGL::CGLELandscape* th, const shok::Position* pIn, float range, int* pred, shok::Position* pOut) = reinterpret_cast<bool(__thiscall*)(EGL::CGLELandscape*, const shok::Position*, float, int*, shok::Position*)>(0x5775AF);
bool EGL::CGLELandscape::GetNearestPositionInSector(const shok::Position* pIn, float range, int sector, shok::Position* pOut)
{
	int pred[5];
	bool one = true;
	shoklandscape_getnearestcon(pred, &one, sector, this);
	pOut->X = 0.0f;
	pOut->Y = 0.0f;
	return shoklandscape_getnearest(this, pIn, range, pred, pOut);
}
static inline void(__thiscall* const shoklandscape_getnearestfreepos)(EGL::CGLELandscape* ls, shok::Position* ou, const shok::Position* in, float r, int* d) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, shok::Position*, const shok::Position*, float, int*)>(0x577661);
shok::Position EGL::CGLELandscape::GetNearestFreePos(const shok::Position* p, float range)
{
	shok::Position r;
	int i = 1;
	shoklandscape_getnearestfreepos(this, &r, p, range, &i);
	return r;
}
static inline bool(__thiscall* const shok_EGL_CGLELandscape_isvalidpos)(EGL::CGLELandscape* ls, shok::Position* p) = reinterpret_cast<bool(__thiscall*)(EGL::CGLELandscape*, shok::Position*)>(0x5785BC);
bool EGL::CGLELandscape::IsValidPos(shok::Position* p)
{
	return shok_EGL_CGLELandscape_isvalidpos(this, p);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_getsize)(EGL::CGLELandscape* ls, shok::Position* p) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, shok::Position*)>(0x578589);
shok::Position EGL::CGLELandscape::GetMapSize()
{
	shok::Position r;
	shok_EGL_CGLELandscape_getsize(this, &r);
	return r;
}
static inline bool(__thiscall* const shok_EGL_CGLELandscape_isposblockedinmode)(EGL::CGLELandscape* ls, const shok::Position* p, EGL::CGLELandscape::BlockingMode* mode) = reinterpret_cast<bool(__thiscall*)(EGL::CGLELandscape*, const shok::Position*, EGL::CGLELandscape::BlockingMode*)>(0x57772E);
bool EGL::CGLELandscape::IsPosBlockedInMode(const shok::Position* p, BlockingMode mode)
{
	return shok_EGL_CGLELandscape_isposblockedinmode(this, p, &mode);
}
EGL::CGLELandscape::BlockingMode EGL::CGLELandscape::GetBlocking(const shok::Position& p)
{
	int i[] = { 0,0 };
	HiRes->ToTerrainCoord(p, i);
	if (!HiRes->IsCoordValid(i))
		throw std::out_of_range{ "GetBlocking out of range" };
	return BlockingData->GetBlockingData(i[0], i[1]);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_flattenpos)(EGL::CGLELandscape* th, const shok::Position* p, const shok::Position* a, const shok::Position* b, float r) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, const shok::Position*, const shok::Position*, const shok::Position*, float)>(0x579AFA);
void EGL::CGLELandscape::FlattenPosForBuilding(const shok::Position& p, const shok::AARect& area, float rot)
{
	shok_EGL_CGLELandscape_flattenpos(this, &p, &area.low, &area.high, rot);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_applyblocking)(EGL::CGLELandscape* th, const shok::Position* p, const shok::Position* a, const shok::Position* b, float r, EGL::CGLELandscape::BlockingMode* m) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, const shok::Position*, const shok::Position*, const shok::Position*, float, EGL::CGLELandscape::BlockingMode*)>(0x577A82);
void EGL::CGLELandscape::ApplyBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode)
{
	shok_EGL_CGLELandscape_applyblocking(this, &p, &area.low, &area.high, rot, &blockingmode);
}
static inline void(__thiscall* const shok_EGL_CGLELandscape_remblocking)(EGL::CGLELandscape* th, const shok::Position* p, const shok::Position* a, const shok::Position* b, float r, EGL::CGLELandscape::BlockingMode* m) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, const shok::Position*, const shok::Position*, const shok::Position*, float, EGL::CGLELandscape::BlockingMode*)>(0x577B41);
void EGL::CGLELandscape::RemoveBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode)
{
	shok_EGL_CGLELandscape_remblocking(this, &p, &area.low, &area.high, rot, &blockingmode);
}
static inline void(__thiscall* const landscape_updateblocking)(EGL::CGLELandscape* th, int* low, int* high) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, int*, int*)>(0x5796A2);
void EGL::CGLELandscape::UpdateBlocking(const shok::AARect& area)
{
	shok::Position p{ min(area.low.X, area.high.X), min(area.low.Y, area.high.Y) };
	int low[2] = { 0,0 };
	int high[2] = { 0,0 };
	HiRes->ToTerrainCoord(p, low);
	p = { max(area.low.X, area.high.X), max(area.low.Y, area.high.Y) };
	HiRes->ToTerrainCoord(p, high);
	if (!HiRes->IsCoordValid(low))
		throw std::out_of_range{ "UpdateBlocking low out of range" };
	if (!HiRes->IsCoordValid(high))
		throw std::out_of_range{ "UpdateBlocking high out of range" };
	landscape_updateblocking(this, low, high);
}
static inline void(__thiscall* const lanscape_weatherchangeupdateblockig)(EGL::CGLELandscape* th, int z) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, int)>(0x579613);
void EGL::CGLELandscape::WeatherChangeBlockingUpdate()
{
	lanscape_weatherchangeupdateblockig(this, 0);
}
void EGL::CGLELandscape::AdvancedRemoveBridgeHeight(const shok::Position& p, const shok::AARect& area, float rot)
{
	shok::AARect ar = area.Rotate(rot).Sort();
	CppLogic::Iterator::PredicateInRect<EGL::CGLEEntity> rec{ p.X + ar.low.X, p.Y + ar.low.Y, p.X + ar.high.X - 100, p.Y + ar.high.Y - 100 };
	CppLogic::Iterator::GlobalEntityIterator it{ &rec };
	for (auto* ent : it) {
		if (GGL::CSettler* s = dynamic_cast<GGL::CSettler*>(ent)) {
			s->KillSettlerByEnvironment();
		}
		else if (GGL::CAnimal* a = dynamic_cast<GGL::CAnimal*>(ent)) {
			if (!a->GetFirstAttachedToMe(shok::AttachmentType::HERO_HAWK))
				a->Destroy();
		}
	}
	AdvancedRemoveBlocking(p, area, rot, BlockingMode::BridgeArea);
	AdvancedApplyBlocking(p, area, rot, BlockingMode::Blocked);
}
static inline void(__thiscall* const landscape_setsingleblockingpoint)(EGL::CGLELandscape* th, int x, int y, EGL::CGLELandscape::BlockingMode* m) = reinterpret_cast<void(__thiscall*)(EGL::CGLELandscape*, int, int, EGL::CGLELandscape::BlockingMode*)>(0x57738A);
void EGL::CGLELandscape::AdvancedApplyBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode)
{
	AdvancedAARectIterator iter{ p, area, rot, false };
	for (auto& curr : iter) {
		if (!HiRes->IsCoordValid(curr.x, curr.y))
			continue;
		landscape_setsingleblockingpoint(this, curr.x, curr.y, &blockingmode);
	}
}
static inline void(__thiscall* const lsblocking_remvoveblockingpoint)(EGL::LandscapeBlockingData* th, int x, int y, EGL::CGLELandscape::BlockingMode* m) = reinterpret_cast<void(__thiscall*)(EGL::LandscapeBlockingData*, int, int, EGL::CGLELandscape::BlockingMode*)>(0x57EEE1);
void EGL::CGLELandscape::AdvancedRemoveBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode)
{
	AdvancedAARectIterator iter{ p, area, rot, false };
	for (auto& curr : iter) {
		if (!HiRes->IsCoordValid(curr.x, curr.y))
			continue;
		RemoveSingleBlockingPoint(curr.x, curr.y, blockingmode);
	}
}
void EGL::CGLELandscape::RemoveSingleBlockingPoint(int x, int y, BlockingMode mode)
{
	if (static_cast<int>(mode) & static_cast<int>(BlockingMode::Blocked) && Tiling->GetSomeGlobal())
		Tiling->OnPreBlockingMode1Removed(x, y);
	lsblocking_remvoveblockingpoint(BlockingData, x, y, &mode);
	if (static_cast<int>(mode) & static_cast<int>(BlockingMode::BridgeArea) && Tiling->GetSomeGlobal())
		Tiling->OnPostBlockingMode2Removed(x, y);
}
void EGL::CGLELandscape::AdvancedApplyBridgeHeight(const shok::Position& p, const shok::AARect& area, float rot, int height)
{
	AdvancedAARectIterator it{ p, area, rot, !EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled, false };
	for (auto& c : it) {
		if (!LowRes->IsBridgeHeightCoordValid(c.x, c.y))
			continue;
		int* h = LowRes->GetBridgeHeightP(c.x, c.y);
		*h = height;
	}

	height += 10;

	AdvancedAARectIterator iter{ p, area, rot, false };
	BlockingMode m = BlockingMode::BridgeArea;
	BlockingMode m2 = BlockingMode::Blocked;
	for (auto& curr : iter) {
		if (!HiRes->IsCoordValid(curr.x, curr.y))
			continue;
		if (HiRes->GetTerrainHeight(curr.x, curr.y) < height) {
			landscape_setsingleblockingpoint(this, curr.x, curr.y, &m);
			RemoveSingleBlockingPoint(curr.x, curr.y, BlockingMode::Blocked);
		}
		else {
			landscape_setsingleblockingpoint(this, curr.x, curr.y, &m2);
			RemoveSingleBlockingPoint(curr.x, curr.y, BlockingMode::BridgeArea);
		}
	}
}
bool EGL::CGLELandscape::IsAreaUnblockedInMode(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode mode, bool AddOne)
{
	AdvancedAARectIterator iter{ p, area, rot, false, AddOne };
	for (auto& curr : iter) {
		if (!HiRes->IsCoordValid(curr.x, curr.y))
			return false;
		if ((BlockingData->GetBlockingData(curr.x, curr.y) & mode) != BlockingMode::None)
			return false;
	}
	return true;
}
bool EGL::CGLELandscape::IsAreaNotUnderWater(const shok::Position& p, const shok::AARect& area, float rot, bool AddOne)
{
	AdvancedAARectIterator iter{ p, area, rot, false, AddOne };
	for (auto& curr : iter) {
		if (!HiRes->IsCoordValid(curr.x, curr.y))
			return false;
		if (HiRes->GetTerrainHeight(curr.x, curr.y) < LowRes->GetWaterHeightAt(curr.x / 4, curr.y / 4))
			return false;
	}
	return true;
}

void(__thiscall* const buildblock_gethigh)(const GGL::CGLBuildingProps* t, shok::Position* out, float r) = reinterpret_cast<void(__thiscall*)(const GGL::CGLBuildingProps * t, shok::Position*, float)>(0x4C7B8B);
void(__thiscall* const buildblock_getlow)(const GGL::CGLBuildingProps* t, shok::Position* out, float r) = reinterpret_cast<void(__thiscall*)(const GGL::CGLBuildingProps * t, shok::Position*, float)>(0x4C7B1E);
struct shok_GGL_CFreeBuildingPosPredicate {
	PADDINGI(20);
};
void(__thiscall* const freebuildpred_ctor)(shok_GGL_CFreeBuildingPosPredicate* p, shok::Position* lo, shok::Position* hi, int uk, float r) = reinterpret_cast<void(__thiscall*)(shok_GGL_CFreeBuildingPosPredicate*, shok::Position*, shok::Position*, int, float)>(0x4C7E84);
void(__thiscall* const freebuildpred_calc)(EGL::LandscapeBlockingData* bl, int x, int y, int* xo, int* yo, int ra, shok_GGL_CFreeBuildingPosPredicate* pred) = reinterpret_cast<void(__thiscall*)(EGL::LandscapeBlockingData*, int, int, int*, int*, int, shok_GGL_CFreeBuildingPosPredicate*)>(0x57F726);
void(__thiscall* const freebuildpred_dtor)(shok_GGL_CFreeBuildingPosPredicate* p) = reinterpret_cast<void(__thiscall*)(shok_GGL_CFreeBuildingPosPredicate*)>(0x4C7E07);
shok::Position EGL::LandscapeBlockingData::GetFreeBuildingPlacementPos(const GGL::CGLBuildingProps* bprops, const shok::PositionRot& pos, float range)
{
	int x = 0, y = 0;
	shok::Position hi, lo;
	buildblock_gethigh(bprops, &hi, pos.r);
	buildblock_getlow(bprops, &lo, pos.r);
	shok_GGL_CFreeBuildingPosPredicate pred;
	freebuildpred_ctor(&pred, &lo, &hi, reinterpret_cast<int>(bprops) + 136, pos.r); // blocing area vector*
	freebuildpred_calc((*EGL::CGLEGameLogic::GlobalObj)->Landscape->BlockingData, static_cast<int>(std::roundf(pos.X / 100)), static_cast<int>(std::roundf(pos.Y / 100)), &x, &y, static_cast<int>(std::roundf(range / 100)), &pred);
	freebuildpred_dtor(&pred);

	if (IsCoordValid(x, y))
		return { static_cast<float>(x) * 100, static_cast<float>(y) * 100 };

	return { -1,-1 };
}
EGL::CGLELandscape::BlockingMode EGL::LandscapeBlockingData::GetBlockingData(int x, int y)
{
	return static_cast<EGL::CGLELandscape::BlockingMode>(data[x + y * *EGL::CGLEGameLogic::MapSize]);
}


bool EGL::LandscapeBlockingData::IsCoordValid(int x, int y)
{
	return x >= 0 && x < ArraySizeXY&& y >= 0 && y < ArraySizeXY;
}

static inline bool(__thiscall* explohandler_isexplored)(EGL::CPlayerExplorationHandler* th, int x, int y) = reinterpret_cast<bool(__thiscall*)(EGL::CPlayerExplorationHandler*, int, int)>(0x4BD183);
bool EGL::CPlayerExplorationHandler::IsPositionExplored(const shok::Position& p)
{
	int i[2] = {};
	EGL::CGLETerrainHiRes::ToTerrainCoord(p, i);
	return explohandler_isexplored(this, i[0], i[1]);
}

static inline int(__thiscall* somegamelogicstuff_getexplomin)(EGL::GameLogicExplorationStuff* th) = reinterpret_cast<int(__thiscall*)(EGL::GameLogicExplorationStuff*)>(0x5758FB);
static inline int(__thiscall* somegamelogicstuff_getexplomax)(EGL::GameLogicExplorationStuff* th) = reinterpret_cast<int(__thiscall*)(EGL::GameLogicExplorationStuff*)>(0x575904);
static inline EGL::CPlayerExplorationHandler* (__thiscall* somegamelogicstuff_getexplo)(EGL::GameLogicExplorationStuff* th, int pl) = reinterpret_cast<EGL::CPlayerExplorationHandler * (__thiscall*)(EGL::GameLogicExplorationStuff*, int)>(0x575895);
EGL::CPlayerExplorationHandler* EGL::GameLogicExplorationStuff::GetExplorationHandlerByPlayer(int pl)
{
	if (pl >= somegamelogicstuff_getexplomin(this) && pl >= somegamelogicstuff_getexplomax(this)) {
		return somegamelogicstuff_getexplo(this, pl);
	}
	return nullptr;
}
static inline void(__thiscall* const gamelogicexplo_setshare)(EGL::GameLogicExplorationStuff* th, int p1, int p2, bool f) = reinterpret_cast<void(__thiscall*)(EGL::GameLogicExplorationStuff*, int, int, bool)>(0x57584B);
void EGL::GameLogicExplorationStuff::SetShareExplorationFlag(int pl1, int pl2, bool share)
{
	gamelogicexplo_setshare(this, pl1, pl2, share);
}
static inline void(__thiscall* const gamelogicexplo_activateexploforall)(EGL::GameLogicExplorationStuff* th) = reinterpret_cast<void(__thiscall*)(EGL::GameLogicExplorationStuff*)>(0x575884);
void EGL::GameLogicExplorationStuff::ActivateUpdateOfExplorationForAllPlayers()
{
	gamelogicexplo_activateexploforall(this);
}

int EGL::CGLEGameLogic::CreateEntity(EGL::CGLEEntityCreator* cr)
{
	return CreateEntity(cr, 1);
}

int EGL::CGLEGameLogic::GetTimeMS()
{
	return InGameTime->Tick * InGameTime->TicksPerMS;
}

int EGL::CGLEGameLogic::GetTick()
{
	return InGameTime->Tick;
}

int(__thiscall* CreateEffectHookedOrig)(EGL::CGLEGameLogic* th, EGL::CGLEEffectCreator* data) = nullptr;
void(*EGL::CGLEGameLogic::CreateEffectHookCallback)(int id, void* ret) = nullptr;
int __fastcall CreateEffectHook(EGL::CGLEGameLogic* th, int _, EGL::CGLEEffectCreator* data)
{
	void** ebp_var = (void**)1;
	_asm {
		mov ebp_var, ebp;
	}
	int id = CreateEffectHookedOrig(th, data);
	if (EGL::CGLEGameLogic::CreateEffectHookCallback)
		EGL::CGLEGameLogic::CreateEffectHookCallback(id, ebp_var[1]);
	return id;
}
void EGL::CGLEGameLogic::HookCreateEffect()
{
	if (CreateEffectHookedOrig)
		return;
	shok_vtable_EGL_CGLEGameLogic* vt = *reinterpret_cast<shok_vtable_EGL_CGLEGameLogic**>(this);
	CppLogic::Hooks::SaveVirtualProtect vp{ vt, 25 * 4 };
	CreateEffectHookedOrig = vt->CreateEffect;
	vt->CreateEffect = reinterpret_cast<int(__thiscall*)(EGL::CGLEGameLogic * th, EGL::CGLEEffectCreator * data)>(&CreateEffectHook);
}

static inline int(__thiscall* const weatherdata_getnext)(GGL::CWeatherHandler* th) = reinterpret_cast<int(__thiscall*)(GGL::CWeatherHandler*)>(0x4B93BD);
int GGL::CWeatherHandler::GetNextWeatherState()
{
	return weatherdata_getnext(this);
}

static inline int(__thiscall* const weatherdata_gettimetonext)(GGL::CWeatherHandler* th) = reinterpret_cast<int(__thiscall*)(GGL::CWeatherHandler*)>(0x4B9309);
int GGL::CWeatherHandler::GetTicksToNextPeriodicWeatherChange()
{
	return weatherdata_gettimetonext(this);
}

static inline void(__thiscall* const weatherdata_addelement)(GGL::CWeatherHandler* th, int state, int dur, bool peri, int fore, int gfx, int trans) = reinterpret_cast<void(__thiscall*)(GGL::CWeatherHandler*, int, int, bool, int, int, int)>(0x4B95A8);
void GGL::CWeatherHandler::AddWeatherElement(int state, int dur, bool peri, int forerun, int gfx, int transition)
{
	weatherdata_addelement(this, state, dur, peri, forerun, gfx, transition);
}
static inline void(__thiscall* const weatherdata_setoffset)(GGL::CWeatherHandler* th, int o) = reinterpret_cast<void(__thiscall*)(GGL::CWeatherHandler*, int)>(0x4B9635);
void GGL::CWeatherHandler::SetOffset(int o)
{
	weatherdata_setoffset(this, o);
}

static inline void(__thiscall* const weatherdata_elements_remove)(shok::Map<int, GGL::CWeatherHandler::WeatherElementData>* th, int* ind) = reinterpret_cast<void(__thiscall*)(shok::Map<int, GGL::CWeatherHandler::WeatherElementData>*, int*)>(0x513D32);
static inline void(__thiscall* const weatherdata_elements_add)(shok::Map<int, GGL::CWeatherHandler::WeatherElementData>* th, GGL::CWeatherHandler::WeatherElementData* e) = reinterpret_cast<void(__thiscall*)(shok::Map<int, GGL::CWeatherHandler::WeatherElementData>*, GGL::CWeatherHandler::WeatherElementData*)>(0x513CB6);
void GGL::CWeatherHandler::ClearQueue(int state, int dur, int forerun, int gfx, int transition)
{
	// save data
	GGL::CWeatherHandler::WeatherElementData* current = nullptr;
	GGL::CWeatherHandler::WeatherElementData* changeto = nullptr;
	std::vector<int> win_toremove{};
	for (auto& kae : Elements) {
		if (this->CurrentWeatherIndex == kae.first)
			current = &kae.second;
		else if (this->WeatherChange.WIndexToChangeTo == kae.first)
			changeto = &kae.second;
		else
			win_toremove.push_back(kae.first);
	}
	if (!current)
		throw std::out_of_range("somehow we got no valid current");
	// remove all elements
	for (int i : win_toremove) {
		int a = i;
		weatherdata_elements_remove(&Elements, &a);
	}
	// initalize first element
	NextPeriodicWeatherStartTimeOffset = 0;
	GGL::CWeatherHandler::WeatherElementData initial{};
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
		if (!changeto)
			throw std::out_of_range("somehow we got no valid changeto");
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

static inline GGL::CPlayerStatus* (__thiscall* const gamelogic_getplayer)(GGL::CPlayerStatus** pl, int p) = reinterpret_cast<GGL::CPlayerStatus * (__thiscall*) (GGL::CPlayerStatus**, int)>(0x4A91BC);
GGL::CPlayerStatus* GGL::CGLGameLogic::GetPlayer(int i)
{
	return gamelogic_getplayer(this->players, i);
}

shok::Technology* GGL::CGLGameLogic::GetTech(int i)
{
	--i;
	if (i >= static_cast<int>(TechManager->Techs.size()))
		return nullptr;
	return TechManager->Techs[i];
}

static inline void(__thiscall* const cglgamelogic_enablealarm)(GGL::CGLGameLogic* th, EGL::CNetEventPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventPlayerID*)>(0x49FA14);
void GGL::CGLGameLogic::EnableAlarmForPlayer(int pl)
{
	EGL::CNetEventPlayerID ev{ shok::NetEventIds::CommandPlayerActivateAlarm, pl };
	cglgamelogic_enablealarm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_disablealarm)(GGL::CGLGameLogic* th, EGL::CNetEventPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventPlayerID*)>(0x49FAD7);
void GGL::CGLGameLogic::DisableAlarmForPlayer(int pl)
{
	EGL::CNetEventPlayerID ev{ shok::NetEventIds::CommandPlayerDeactivateAlarm, pl };
	cglgamelogic_disablealarm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_upgradesettlercat)(GGL::CGLGameLogic* th, EGL::CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventIntegerAndPlayerID*)>(0x4985C5);
void GGL::CGLGameLogic::UpgradeSettlerCategory(int pl, int ucat)
{
	EGL::CNetEventIntegerAndPlayerID ev{ shok::NetEventIds::PlayerUpgradeSettlerCategory, pl, ucat };
	cglgamelogic_upgradesettlercat(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_activateweatherm)(GGL::CGLGameLogic* th, EGL::CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventIntegerAndPlayerID*)>(0x49BF7A);
void GGL::CGLGameLogic::PlayerActivateWeathermachine(int player, int weathertype)
{
	EGL::CNetEventIntegerAndPlayerID ev{ shok::NetEventIds::CommandWeathermachineChangeWeather, player, weathertype };
	cglgamelogic_activateweatherm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_blesssettlers)(GGL::CGLGameLogic* th, EGL::CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventIntegerAndPlayerID*)>(0x49B7E6);
void GGL::CGLGameLogic::PlayerBlessSettlers(int player, int blessCat)
{
	EGL::CNetEventIntegerAndPlayerID ev{ shok::NetEventIds::CommandMonasteryBlessSettlerGroup, player, blessCat };
	cglgamelogic_blesssettlers(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_setdiplo)(GGL::CGLGameLogic* th, EGL::CNetEvent2PlayerIDsAndInteger* ev) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEvent2PlayerIDsAndInteger*)>(0x49872D);
void GGL::CGLGameLogic::SetDiplomacy(int p1, int p2, shok::DiploState state)
{
	EGL::CNetEvent2PlayerIDsAndInteger ev{ shok::NetEventIds::SetDiplomacy, p1, p2, static_cast<int>(state) };
	cglgamelogic_setdiplo(this, &ev);
}

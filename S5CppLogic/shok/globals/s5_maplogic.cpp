#include "pch.h"
#include <stdexcept>
#include <shok/globals/s5_maplogic.h>
#include <shok/globals/s5_mapdisplay.h>
#include <shok/entitytype/s5_entitytype.h>
#include <shok/entity/s5_entity.h>
#include <shok/s5_mem.h>
#include <shok/s5_defines.h>
#include <shok/events/s5_netevents.h>
#include <shok/s5_scriptsystem.h>
#include <shok/globals/s5_classfactory.h>
#include <utility/entityiterator.h>
#include <utility/hooks.h>

void EGL::CTerrainVertexColors::ToTerrainCoord(shok::Position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool EGL::CTerrainVertexColors::IsCoordValid(const int* out) const
{
	return out[0] >= 0 && out[1] >= 0 && (out[0] + 1) < ArraySizeX && (out[1] + 1) < ArraySizeY;
}
int EGL::CTerrainVertexColors::GetTerrainVertexColor(shok::Position& p) const
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
bool EGL::CGLETerrainHiRes::IsCoordValid(int x, int y) const
{
	return x >= 0 && y >= 0 && x < MaxSizeX&& y < MaxSizeY;
}
bool EGL::CGLETerrainHiRes::IsCoordValid(const int* out) const {
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
	terrhires_setheight(this, qp, static_cast<int16_t>(h));
}
int EGL::CGLETerrainHiRes::GetTerrainHeight(int x, int y) {
	return TerrainHeights[(y + 1) * ArraySizeY + (x + 1)];
}

void EGL::CGLETerrainLowRes::ToQuadCoord(const shok::Position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100) / 4);
	out[1] = static_cast<int>(std::lroundf(p.Y / 100) / 4);
}
bool EGL::CGLETerrainLowRes::IsCoordValid(int x, int y) const
{
	return x >= 0 && y >= 0 && x < MaxSizeX&& y < MaxSizeY;
}
bool EGL::CGLETerrainLowRes::IsCoordValid(const int* out) const {
	return IsCoordValid(out[0], out[1]);
}
bool EGL::CGLETerrainLowRes::IsBridgeHeightCoordValid(int x, int y) const
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
int* EGL::CGLETerrainLowRes::GetBridgeHeightP(int x, int y)
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

int * __thiscall EGL::CGLETerrainLowRes::GetBridgeHeightPStatic(CGLETerrainLowRes *th, int x, int y) {
	return th->GetBridgeHeightP(x, y);
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



void __declspec(naked) hiresbridgearea_somewaterregionfunc() {
	__asm {
		push[ebp - 0x4]; // y
		push[ebp + 0x8]; // x
		mov ecx, [ebp - 0xC];
		call EGL::CGLETerrainLowRes::GetBridgeHeightPStatic;
		fild dword ptr [eax];

		push 0x47D318; // should be 47D30A, but thats just a unconditional jmp
		ret;
	};
}
bool EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled = false;
int* EGL::CGLETerrainLowRes::Dbg_bh = nullptr;
void EGL::CGLETerrainLowRes::EnableHiResBridgeHeight()
{
	if (EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled)
		return;
	EGL::CGLETerrainLowRes::HiResBridgeHeightEnabled = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ 0x10, { reinterpret_cast<void*>(0x76A410),
		reinterpret_cast<void*>(0x47D301),
		reinterpret_cast<void*>(0x503C50),
	} };
	*reinterpret_cast<void**>(0x76A410) = CppLogic::Hooks::MemberFuncPointerToVoid(&ED::CLandscape::GetWaterHeightAtPosOverride, 0);
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x47D301), &hiresbridgearea_somewaterregionfunc, reinterpret_cast<void*>(0x47D30A));
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x503C50), CppLogic::Hooks::MemberFuncPointerToVoid(&GGL::CBridgeEntity::ApplyHeightOverride, 0), reinterpret_cast<void*>(0x503C58));
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
	shok::Position p2{ std::min(rotated.low.X, rotated.high.X) + p.X, std::min(rotated.low.Y, rotated.high.Y) + p.Y };
	int low[2] = { 0,0 };
	int high[2] = { 0,0 };
	if (LowRes)
		EGL::CGLETerrainLowRes::ToQuadCoord(p2, low);
	else
		EGL::CGLETerrainHiRes::ToTerrainCoord(p2, low);
	p2 = { std::max(rotated.low.X, rotated.high.X) + p.X, std::max(rotated.low.Y, rotated.high.Y) + p.Y };
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
	return {*this, Low};
}
EGL::CGLELandscape::AdvancedAARectIterator::Iter EGL::CGLELandscape::AdvancedAARectIterator::end() const
{
	return {*this, { Low.x, High.y }};
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

static inline shok::SectorId(__thiscall* const shokLandscape_getSector)(EGL::CGLELandscape* th, const shok::Position* p) = reinterpret_cast<shok::SectorId(__thiscall*)(EGL::CGLELandscape*, const shok::Position*)>(0x5778BE);
shok::SectorId EGL::CGLELandscape::GetSector(const shok::Position* p)
{
	return shokLandscape_getSector(this, p);
}

static inline void(__thiscall* const shoklandscape_getnearestcon)(int* pred, bool* one, shok::SectorId sector, EGL::CGLELandscape* l) = reinterpret_cast<void(__thiscall*)(int*, bool*, shok::SectorId, EGL::CGLELandscape*)>(0x57F253);
static inline bool(__thiscall* const shoklandscape_getnearest)(EGL::CGLELandscape* th, const shok::Position* pIn, float range, int* pred, shok::Position* pOut) = reinterpret_cast<bool(__thiscall*)(EGL::CGLELandscape*, const shok::Position*, float, int*, shok::Position*)>(0x5775AF);
bool EGL::CGLELandscape::GetNearestPositionInSector(const shok::Position* pIn, float range, shok::SectorId sector, shok::Position* pOut)
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
EGL::CGLELandscape::BlockingMode EGL::CGLELandscape::GetBlocking(const shok::Position& p) const
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
	shok::Position p{ std::min(area.low.X, area.high.X), std::min(area.low.Y, area.high.Y) };
	int low[2] = { 0,0 };
	int high[2] = { 0,0 };
	HiRes->ToTerrainCoord(p, low);
	p = { std::max(area.low.X, area.high.X), std::max(area.low.Y, area.high.Y) };
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
		if (auto* s = dynamic_cast<GGL::CSettler*>(ent)) {
			s->KillSettlerByEnvironment();
		}
		else if (auto* a = dynamic_cast<GGL::CAnimal*>(ent)) {
			if (a->GetFirstAttachedToMe(shok::AttachmentType::HERO_HAWK) != static_cast<shok::EntityId>(0))
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
void EGL::CGLELandscape::AdvancedRemoveBlocking(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode blockingmode) const {
	AdvancedAARectIterator iter{ p, area, rot, false };
	for (auto& curr : iter) {
		if (!HiRes->IsCoordValid(curr.x, curr.y))
			continue;
		RemoveSingleBlockingPoint(curr.x, curr.y, blockingmode);
	}
}
void EGL::CGLELandscape::RemoveSingleBlockingPoint(int x, int y, BlockingMode mode) const
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
bool EGL::CGLELandscape::IsAreaUnblockedInMode(const shok::Position& p, const shok::AARect& area, float rot, BlockingMode mode, bool AddOne) const
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
bool EGL::CGLELandscape::IsAreaNotUnderWater(const shok::Position& p, const shok::AARect& area, float rot, bool AddOne) const
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

bool(__thiscall* const landscape_isposunderwater)(EGL::CGLELandscape* th, const shok::Position* p) = reinterpret_cast<bool(__thiscall*)(EGL::CGLELandscape*, const shok::Position*)>(0x578F18);
bool EGL::CGLELandscape::IsPosUnderwater(const shok::Position& p)
{
	return landscape_isposunderwater(this, &p);
}

bool(__thiscall* const landscape_iswaterposfreezable)(EGL::CGLELandscape* th, const shok::Position* p) = reinterpret_cast<bool(__thiscall*)(EGL::CGLELandscape*, const shok::Position*)>(0x578FA1);
bool EGL::CGLELandscape::IsWaterAtPosFreezable(const shok::Position& p)
{
	return landscape_iswaterposfreezable(this, &p);
}

void(__thiscall* const buildblock_gethigh)(const GGL::CGLBuildingProps* t, shok::Position* out, float r) = reinterpret_cast<void(__thiscall*)(const GGL::CGLBuildingProps * t, shok::Position*, float)>(0x4C7B8B);
void(__thiscall* const buildblock_getlow)(const GGL::CGLBuildingProps* t, shok::Position* out, float r) = reinterpret_cast<void(__thiscall*)(const GGL::CGLBuildingProps * t, shok::Position*, float)>(0x4C7B1E);
struct shok_GGL_CFreeBuildingPosPredicate {
	PADDINGI(20);
};
void(__thiscall* const freebuildpred_ctor)(shok_GGL_CFreeBuildingPosPredicate* p, shok::Position* lo, shok::Position* hi, int uk, float r) = reinterpret_cast<void(__thiscall*)(shok_GGL_CFreeBuildingPosPredicate*, shok::Position*, shok::Position*, int, float)>(0x4C7E84);
void(__thiscall* const freebuildpred_calc)(EGL::LandscapeBlockingData* bl, int x, int y, int* xo, int* yo, int ra, shok_GGL_CFreeBuildingPosPredicate* pred) = reinterpret_cast<void(__thiscall*)(EGL::LandscapeBlockingData*, int, int, int*, int*, int, shok_GGL_CFreeBuildingPosPredicate*)>(0x57F726);
void(__thiscall* const freebuildpred_dtor)(shok_GGL_CFreeBuildingPosPredicate* p) = reinterpret_cast<void(__thiscall*)(shok_GGL_CFreeBuildingPosPredicate*)>(0x4C7E07);
shok::Position EGL::LandscapeBlockingData::GetFreeBuildingPlacementPos(const GGL::CGLBuildingProps* bprops, const shok::PositionRot& pos, float range) const
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
EGL::CGLELandscape::BlockingMode EGL::LandscapeBlockingData::GetBlockingData(int x, int y) const
{
	return static_cast<EGL::CGLELandscape::BlockingMode>(data[x + y * *EGL::CGLEGameLogic::MapSize]);
}


bool EGL::LandscapeBlockingData::IsCoordValid(int x, int y) const
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

inline void(__thiscall* const playerexploupdate_setplayers)(EGL::CPlayerExplorationUpdate* th, int f, int l) = reinterpret_cast<void(__thiscall*)(EGL::CPlayerExplorationUpdate*, int, int)>(0x58C724);
void EGL::CPlayerExplorationUpdate::SetPlayersToUpdate(int first, int last)
{
	playerexploupdate_setplayers(this, first, last);
}

static inline shok::PlayerId(__thiscall* somegamelogicstuff_getexplomin)(EGL::PlayerManager* th) = reinterpret_cast<shok::PlayerId(__thiscall*)(EGL::PlayerManager*)>(0x5758FB);
static inline shok::PlayerId(__thiscall* somegamelogicstuff_getexplomax)(EGL::PlayerManager* th) = reinterpret_cast<shok::PlayerId(__thiscall*)(EGL::PlayerManager*)>(0x575904);
static inline EGL::CPlayerExplorationHandler* (__thiscall* somegamelogicstuff_getexplo)(EGL::PlayerManager* th, shok::PlayerId pl) = reinterpret_cast<EGL::CPlayerExplorationHandler * (__thiscall*)(EGL::PlayerManager*, shok::PlayerId)>(0x575895);
EGL::CPlayerExplorationHandler* EGL::PlayerManager::GetExplorationHandlerByPlayer(shok::PlayerId pl)
{
	if (pl >= somegamelogicstuff_getexplomin(this) && pl >= somegamelogicstuff_getexplomax(this)) {
		return somegamelogicstuff_getexplo(this, pl);
	}
	return nullptr;
}
static inline void(__thiscall* const gamelogicexplo_setshare)(EGL::PlayerManager* th, shok::PlayerId p1, shok::PlayerId p2, bool f) = reinterpret_cast<void(__thiscall*)(EGL::PlayerManager*, shok::PlayerId, shok::PlayerId, bool)>(0x57584B);
void EGL::PlayerManager::SetShareExplorationFlag(shok::PlayerId pl1, shok::PlayerId pl2, bool share)
{
	gamelogicexplo_setshare(this, pl1, pl2, share);
}
static inline void(__thiscall* const gamelogicexplo_activateexploforall)(EGL::PlayerManager* th) = reinterpret_cast<void(__thiscall*)(EGL::PlayerManager*)>(0x575884);
void EGL::PlayerManager::ActivateUpdateOfExplorationForAllPlayers()
{
	gamelogicexplo_activateexploforall(this);
}
inline EGL::CPlayerExplorationUpdate* (__thiscall* const playermng_getupdate)(EGL::PlayerManager* th) = reinterpret_cast<EGL::CPlayerExplorationUpdate * (__thiscall*)(EGL::PlayerManager*)>(0x5757A8);
EGL::CPlayerExplorationUpdate* EGL::PlayerManager::GetUpdate()
{
	return playermng_getupdate(this);
}

const BB::SerializationData EGL::PlayerManager::SerializationData[3] {
	reinterpret_cast<const BB::SerializationData*(__stdcall*)()>(0x575B2B)()[0],
	reinterpret_cast<const BB::SerializationData* (__stdcall*)()>(0x575B2B)()[1],
	BB::SerializationData::GuardData(),
};

shok::Vector<EGL::CGLEEntity*>& EGL::RegionDataEntity::Entry::GetByAccessCategory(shok::AccessCategory ac)
{
	return EntitiesByAccessCategory[static_cast<unsigned int>(ac)];
}

EGL::RegionDataEntity::Entry* EGL::RegionDataEntity::GetEntry(const shok::Position& p)
{
	auto id = GetEntryId(p);
	if (id == *reinterpret_cast<size_t*>(0x898990))
		return nullptr;
	return &Data[id];
}

inline size_t(__thiscall* const regiondataent_getentry)(EGL::RegionDataEntity* th, const shok::Position* p) = reinterpret_cast<size_t(__thiscall*)(EGL::RegionDataEntity*, const shok::Position*)>(0x58763C);
size_t EGL::RegionDataEntity::GetEntryId(const shok::Position& p)
{
	return regiondataent_getentry(this, &p);
}
inline void(__thiscall* const regiondataent_getcomp)(EGL::RegionDataEntity* th, const shok::Position* p, int* x, int* y) = reinterpret_cast<void(__thiscall*)(EGL::RegionDataEntity*, const shok::Position*, int*, int*)>(0x5875B7);
void EGL::RegionDataEntity::GetEntryComponents(const shok::Position& p, int& x, int& y)
{
	regiondataent_getcomp(this, &p, &x, &y);
}
inline size_t(__thiscall* const regiondataent_getidbycomp)(EGL::RegionDataEntity* th, int x, int y) = reinterpret_cast<size_t(__thiscall*)(EGL::RegionDataEntity*, int, int)>(0x57A237);
size_t EGL::RegionDataEntity::GetEntryIdByComponents(int x, int y)
{
	return regiondataent_getidbycomp(this, x, y);
}
EGL::RegionDataEntity::Entry* EGL::RegionDataEntity::GetEntry(int x, int y)
{
	auto id = GetEntryIdByComponents(x, y);
	if (id == *reinterpret_cast<size_t*>(0x898990))
		return nullptr;
	return &Data[id];
}
int EGL::RegionDataEntity::GetSingleEntryComponent(float x)
{
	return static_cast<int>(std::floor(x * *reinterpret_cast<float*>(0x784A18)));
}

shok::EntityId EGL::CGLEGameLogic::CreateEntity(EGL::CGLEEntityCreator* cr)
{
	return CreateEntity(cr, 1);
}

int EGL::CGLEGameLogic::GetTimeMS() const
{
	return InGameTime->Tick * InGameTime->TicksPerMS;
}

int EGL::CGLEGameLogic::GetTick() const
{
	return InGameTime->Tick;
}

float EGL::CGLEGameLogic::GetTimeSeconds() const {
	return static_cast<float>(GetTimeMS()) / 1000.0f;
}

inline void(__thiscall* const egl_gamelogic_cleardest)(EGL::CGLEGameLogic* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEGameLogic*)>(0x5720FE);
void EGL::CGLEGameLogic::ClearToDestroy()
{
	egl_gamelogic_cleardest(this);
}

inline shok::EffectId(__thiscall* const CreateEffectHookedOrig)(EGL::CGLEGameLogic* th, EGL::CGLEEffectCreator* data) = reinterpret_cast<shok::EffectId(__thiscall*)(EGL::CGLEGameLogic*, EGL::CGLEEffectCreator*)>(0x571BBB);
void(*EGL::CGLEGameLogic::CreateEffectHookCallback)(shok::EffectId id) = nullptr;
shok::EffectId EGL::CGLEGameLogic::CreateEffectOverride(EGL::CGLEEffectCreator* data)
{
	shok::EffectId id = CreateEffectHookedOrig(this, data);
	if (EGL::CGLEGameLogic::CreateEffectHookCallback)
		EGL::CGLEGameLogic::CreateEffectHookCallback(id);
	return id;
}
bool CreateEffectHooked = false;
void EGL::CGLEGameLogic::HookCreateEffect()
{
	if (CreateEffectHooked)
		return;
	CreateEffectHooked = true;
	CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x783A28), 4 };
	*reinterpret_cast<void**>(0x783A28) = CppLogic::Hooks::MemberFuncPointerToVoid(&CGLEGameLogic::CreateEffectOverride, 0);
}

void EGL::CGLEGameLogic::LoadMapscriptOverrideASM() {
	__asm {
		mov ecx, esi;
		lea eax, [ebp - 0x264];
		push eax;
		call EGL::CGLEGameLogic::LoadMapscriptOverride;

		push 0x573705;
		ret;
	}
}

void(*EGL::CGLEGameLogic::OnMapscriptLoaded)() = nullptr;
bool HookOnMapscriptLoadedHooked = false;
void EGL::CGLEGameLogic::HookOnMapscriptLoaded()
{
	if (HookOnMapscriptLoadedHooked)
		return;
	HookOnMapscriptLoadedHooked = true;
	{
		CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x57368B), 0x573697-0x57368B };
		CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x57368B), &LoadMapscriptOverrideASM, reinterpret_cast<void*>(0x573697));
	}
	EScr::CScriptTriggerSystem::HookLoadFileToLuaState();
}
void __stdcall EGL::CGLEGameLogic::OnMapscriptLoadedCaller()
{
	if (OnMapscriptLoaded)
		OnMapscriptLoaded();
}
void __thiscall EGL::CGLEGameLogic::LoadMapscriptOverride(CGLEGameLogic* th, const char* path)
{
	EScr::CScriptTriggerSystem::LoadFileToLuaState(th->Scripting->Ingame, path);
	EGL::CGLEGameLogic::OnMapscriptLoadedCaller();
}

static inline shok::WeatherState(__thiscall* const weatherdata_getnext)(GGL::CWeatherHandler* th) = reinterpret_cast<shok::WeatherState(__thiscall*)(GGL::CWeatherHandler*)>(0x4B93BD);
shok::WeatherState GGL::CWeatherHandler::GetNextWeatherState()
{
	return weatherdata_getnext(this);
}

static inline int(__thiscall* const weatherdata_gettimetonext)(GGL::CWeatherHandler* th) = reinterpret_cast<int(__thiscall*)(GGL::CWeatherHandler*)>(0x4B9309);
int GGL::CWeatherHandler::GetTicksToNextPeriodicWeatherChange()
{
	return weatherdata_gettimetonext(this);
}

static inline void(__thiscall* const weatherdata_addelement)(GGL::CWeatherHandler* th, shok::WeatherState state, int dur, bool peri, int fore, shok::WeatherGFXSet gfx, int trans) = reinterpret_cast<void(__thiscall*)(GGL::CWeatherHandler*, shok::WeatherState, int, bool, int, shok::WeatherGFXSet, int)>(0x4B95A8);
void GGL::CWeatherHandler::AddWeatherElement(shok::WeatherState state, int dur, bool peri, int forerun, shok::WeatherGFXSet gfx, int transition)
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
void GGL::CWeatherHandler::ClearQueue(shok::WeatherState state, int dur, int forerun, shok::WeatherGFXSet gfx, int transition)
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

static inline GGL::CPlayerStatus* (__thiscall* const gglplayermng_getplayer)(GGL::PlayerManager* pl, shok::PlayerId p) = reinterpret_cast<GGL::CPlayerStatus * (__thiscall*) (GGL::PlayerManager*, shok::PlayerId)>(0x4A91BC);
GGL::CPlayerStatus* GGL::PlayerManager::GetPlayer(shok::PlayerId p)
{
	return gglplayermng_getplayer(this, p);
}

GGL::CPlayerStatus* GGL::CGLGameLogic::GetPlayer(shok::PlayerId i) const
{
	return Players->GetPlayer(i);
}

static inline void(__thiscall* const cglgamelogic_enablealarm)(GGL::CGLGameLogic* th, EGL::CNetEventPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventPlayerID*)>(0x49FA14);
void GGL::CGLGameLogic::EnableAlarmForPlayer(shok::PlayerId pl)
{
	EGL::CNetEventPlayerID ev{ shok::NetEventIds::Player_ActivateAlarm, pl };
	cglgamelogic_enablealarm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_disablealarm)(GGL::CGLGameLogic* th, EGL::CNetEventPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventPlayerID*)>(0x49FAD7);
void GGL::CGLGameLogic::DisableAlarmForPlayer(shok::PlayerId pl)
{
	EGL::CNetEventPlayerID ev{ shok::NetEventIds::Player_DeactivateAlarm, pl };
	cglgamelogic_disablealarm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_upgradesettlercat)(GGL::CGLGameLogic* th, EGL::CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventIntegerAndPlayerID*)>(0x4985C5);
void GGL::CGLGameLogic::UpgradeSettlerCategory(shok::PlayerId pl, shok::UpgradeCategoryId ucat)
{
	EGL::CNetEventIntegerAndPlayerID ev{ shok::NetEventIds::Player_UpgradeSettlerCategory, pl, static_cast<int>(ucat) };
	cglgamelogic_upgradesettlercat(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_activateweatherm)(GGL::CGLGameLogic* th, EGL::CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventIntegerAndPlayerID*)>(0x49BF7A);
void GGL::CGLGameLogic::PlayerActivateWeathermachine(shok::PlayerId player, shok::WeatherState weathertype)
{
	EGL::CNetEventIntegerAndPlayerID ev{ shok::NetEventIds::Weathermachine_Activate, player, static_cast<int>(weathertype) };
	cglgamelogic_activateweatherm(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_blesssettlers)(GGL::CGLGameLogic* th, EGL::CNetEventIntegerAndPlayerID* d) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEventIntegerAndPlayerID*)>(0x49B7E6);
void GGL::CGLGameLogic::PlayerBlessSettlers(shok::PlayerId player, shok::BlessCategoryId blessCat)
{
	EGL::CNetEventIntegerAndPlayerID ev{ shok::NetEventIds::Player_BlessGroup, player, static_cast<int>(blessCat) };
	cglgamelogic_blesssettlers(this, &ev);
}

static inline void(__thiscall* const cglgamelogic_setdiplo)(GGL::CGLGameLogic* th, EGL::CNetEvent2PlayerIDsAndInteger* ev) = reinterpret_cast<void(__thiscall*)(GGL::CGLGameLogic*, EGL::CNetEvent2PlayerIDsAndInteger*)>(0x49872D);
void GGL::CGLGameLogic::SetDiplomacy(shok::PlayerId p1, shok::PlayerId p2, shok::DiploState state)
{
	EGL::CNetEvent2PlayerIDsAndInteger ev{ shok::NetEventIds::Player_SetDiplomacy, p1, p2, static_cast<int>(state) };
	cglgamelogic_setdiplo(this, &ev);
}

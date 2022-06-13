#include "pch.h"
#include <stdexcept>
#include "s5_mapdisplay.h"
#include "s5_config.h"


bool ED::CLandscape::GetTerrainPosAtScreenCoords(shok::PositionRot& outpos, int x, int y)
{
	float mp[] = { static_cast<float>(x), static_cast<float>(y) };
	return GetLandscapePosFromMousePos((*ED::CGlobalsBaseEx::GlobalObj)->Camera->SomeCameraData, mp, &outpos, 3);
}
float ED::CLandscape::GetTerrainHeightAtPos(const shok::Position& p)
{
	return GetTerrainHeightAtPosF(p.X, p.Y);
}
float ED::CLandscape::GetWaterHeightAtPos(const shok::Position& p)
{
	return GetWaterHeightAtPosF(p.X, p.Y);
}

void ED::CGlobalsLogicEx::ToTerrainCoord(const shok::Position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool ED::CGlobalsLogicEx::IsCoordValid(int* out)
{
	return out[0] >= 0 && out[1] >= 0 && out[0] < Blocking->ArraySizeXY && out[1] < Blocking->ArraySizeXY;
}
EGL::CGLELandscape::BlockingMode ED::CGlobalsLogicEx::GetBlocking(const shok::Position& p)
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return static_cast<EGL::CGLELandscape::BlockingMode>(Blocking->data[qp[1] * Blocking->ArraySizeXY + qp[0]]);
}

shok::Color ED::CPlayerColors::GetColorByIndex(int i)
{
	if (i < 0 || i > 16)
		return { 0,0,0,0 };
	return Colors[i];
}
void ED::CPlayerColors::SetColorByIndex(int i, shok::Color c)
{
	if (i < 0 || i > 16)
		throw std::out_of_range("invalid color index");
	Colors[i] = c;
	(*ED::CGlobalsBaseEx::GlobalObj)->DisplayProps->MiniMapColor.data()[i] = c;
}
static inline void(__thiscall* const playercolors_refresh)(ED::CPlayerColors* th) = reinterpret_cast<void(__thiscall*)(ED::CPlayerColors*)>(0x4699B9);
void ED::CPlayerColors::RefreshPlayerColors()
{
	playercolors_refresh(this);
}

void ED::CCommandAcknowledgements::ShowAck(const shok::Position& pos)
{
	ShowAck(pos.X, pos.Y);
}

static inline void(__cdecl* const registermodelinst)(void* robj, ED::ModelInstance* i) = reinterpret_cast<void(__cdecl*)(void*, ED::ModelInstance*)>(0x627130);
void ED::ModelInstance::Register()
{
	registermodelinst((*ED::CGlobalsBaseEx::GlobalObj)->DisplayWorld->SomeRenderObj, this);
}
static inline void* (__cdecl* const modelinst_getsomethingdtor)(ED::ModelInstance* i) = reinterpret_cast<void* (__cdecl*)(ED::ModelInstance*)>(0x626E00);
static inline void(__cdecl* const modelinst_deregister)(void* d, ED::ModelInstance* i) = reinterpret_cast<void(__cdecl*)(void*, ED::ModelInstance*)>(0x6271C0);
static inline void(__cdecl* const modelinst_dest)(ED::ModelInstance* i) = reinterpret_cast<void(__cdecl*)(ED::ModelInstance*)>(0x6294A0);
void ED::ModelInstance::Destroy()
{
	void* d = modelinst_getsomethingdtor(this);
	if (d) {
		modelinst_deregister(d, this);
		modelinst_dest(this);
	}
}
static inline void(__cdecl* const modelinst_setrot)(void* l, float* f, float r, ED::ModelInstance::TransformOperation i) = reinterpret_cast<void(__cdecl*)(void*, float*, float, ED::ModelInstance::TransformOperation)>(0x4141D0);
void ED::ModelInstance::Rotate(float r, float* axis, TransformOperation op)
{
	modelinst_setrot(Transform, axis, r, op);
}
void ED::ModelInstance::Rotate(float r, TransformOperation op)
{
	float f[] = { 0,0,1 };
	Rotate(r, f, op);
}
static inline void(__cdecl* const modelinst_setscale)(void* l, float* f, ED::ModelInstance::TransformOperation i) = reinterpret_cast<void(__cdecl*)(void*, float*, ED::ModelInstance::TransformOperation)>(0x414170);
void ED::ModelInstance::Scale(float* s, TransformOperation op)
{
	modelinst_setscale(Transform, s, op);
}
void ED::ModelInstance::Scale(float s, TransformOperation op)
{
	float f[] = { s,s,s };
	Scale(f, op);
}
static inline void(__cdecl* const modelinst_setpos)(void* l, float* f, ED::ModelInstance::TransformOperation i) = reinterpret_cast<void(__cdecl*)(void*, float*, ED::ModelInstance::TransformOperation)>(0x414140);
void ED::ModelInstance::Translate(const shok::Position& p, float height, TransformOperation op)
{
	float f[] = { p.X, p.Y, height };
	modelinst_setpos(Transform, f, op);
}
static inline void(__cdecl* const modelinst_iteratefuncoversomething)(ED::ModelInstance* m, int(__cdecl* op)(void* m, void* data), void* data) = reinterpret_cast<void(__cdecl*)(ED::ModelInstance*, int(__cdecl*)(void*, void*), void*)>(0x628E30);
static inline int(__cdecl* const modelinst_iter_setcolorbyplayer)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x48F361);
void ED::ModelInstance::SetColorByPlayerID(int pl)
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_setcolorbyplayer, &pl);
}
static inline int(__cdecl* const modelinst_iter_disableshadow)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x721FD8);
void ED::ModelInstance::DisableShadow()
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_disableshadow, nullptr);
}
static inline int(__cdecl* const modelinst_iter_disableparticleeffects)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x721F87);
void ED::ModelInstance::DisableParticleEffects()
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_disableparticleeffects, nullptr);
}
static inline int(__cdecl* const modelinst_iter_disableterraindecal)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x721FB2);
void ED::ModelInstance::DisableTerrainDecal()
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_disableterraindecal, nullptr);
}
static inline int(__cdecl* const modelinst_iter_setcolormodulate)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x47B6E3);
void ED::ModelInstance::SetColorModulate(shok::Color argb)
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_setcolormodulate, &argb);
}
void ED::ModelInstance::SetColorModulate(int a, int r, int g, int b)
{
	a &= 0xFF;
	r &= 0xFF;
	g &= 0xFF;
	b &= 0xFF;
	SetColorModulate({ r, g, b, a });
}

static inline ED::ModelInstance* (__thiscall* const modeldata_instanciate)(const ED::ModelData* d) = reinterpret_cast<ED::ModelInstance * (__thiscall*)(const ED::ModelData*)> (0x472742);
ED::ModelInstance* ED::ModelData::Instanciate() const
{
	return modeldata_instanciate(this);
}

unsigned int __stdcall GD::CBuildingEffectsProps::GetClassIdentifier() const
{
	return Identifier;
}

#include "pch.h"
#include <stdexcept>
#include "s5_mapdisplay.h"
#include "s5_config.h"
#include "s5_idmanager.h"
#include "s5_classfactory.h"


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

void ED::CModelsProps::LoadModelDataFromExtraFile(int id)
{
	if (id > static_cast<int>(Model.size()))
		throw std::logic_error{ "somehow the id is too big" };
	if (id == static_cast<int>(Model.size())) {
		auto v = Model.SaveVector();
		v.Vector.emplace_back();
	}
	auto& t = Model[id];
	std::string filename = "Data/Config/Models/";
	filename.append(ModelIdManager->GetNameByID(id));
	filename.append(".xml");
	(*BB::CClassFactory::GlobalObj)->LoadObject(&t, filename.c_str(), t.SerializationData);
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

static inline RWE::RpClump* (__thiscall* const modeldata_instanciate)(const ED::ModelData* d) = reinterpret_cast<RWE::RpClump * (__thiscall*)(const ED::ModelData*)>(0x472742);
RWE::RpClump* ED::ModelData::Instanciate() const
{
	return modeldata_instanciate(this);
}
static inline void(__thiscall* const modeldata_dtor)(ED::ModelData* d) = reinterpret_cast<void(__thiscall*)(ED::ModelData * d)>(0x475D83);
ED::ModelData::~ModelData()
{
	modeldata_dtor(this);
}
ED::ModelData::ModelData(ModelData&& o) noexcept
{
	std::memcpy(this, &o, sizeof(ModelData));
	std::memset(&o, 0, sizeof(ModelData));
}
void* ED::ModelData::operator new(size_t s)
{
	return shok::Malloc(s);
}
void ED::ModelData::operator delete(void* p) {
	shok::Free(p);
}

static inline ED::ModelData* (__cdecl* const resmng_loadmodel)(const char* n) = reinterpret_cast<ED::ModelData* (__cdecl*)(const char*)>(0x472D71);
ED::ModelData* ED::CResourceManager::LoadModel(const char* name)
{
	return resmng_loadmodel(name);
}

unsigned int __stdcall GD::CBuildingEffectsProps::GetClassIdentifier() const
{
	return Identifier;
}

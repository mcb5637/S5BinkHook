#include <stdexcept>
#include <map>
#include "s5_mapdisplay.h"
#include <shok/globals/s5_config.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/globals/s5_classfactory.h>
#include <shok/globals/s5_glue.h>
#include <shok/engine/s5_RWE_anim.h>


bool ED::CLandscape::GetTerrainPosAtScreenCoords(shok::PositionRot& outpos, int x, int y)
{
	float mp[] = { static_cast<float>(x), static_cast<float>(y) };
	return GetLandscapePosFromMousePos((*ED::CGlobalsBaseEx::GlobalObj)->Camera->Camera, mp, &outpos, 3);
}
float ED::CLandscape::GetTerrainHeightAtPos(const shok::Position& p)
{
	return GetTerrainHeightAtPosF(p.X, p.Y);
}
float ED::CLandscape::GetWaterHeightAtPos(const shok::Position& p)
{
	return GetWaterHeightAtPosF(p.X, p.Y);
}

float __thiscall ED::CLandscape::GetWaterHeightAtPosOverride(float x, float y) const { // this func breaks its arguments, so i have to rewrite it instead of patching a few instructions
	shok::Position p{ std::min(std::max(0.0f, x), WorldSizeX),std::min(std::max(0.0f, y), WorldSizeY) };
	if ((*EGL::CGLEGameLogic::GlobalObj)->Landscape->IsPosBlockedInMode(&p, EGL::CGLELandscape::BlockingMode::BridgeArea)) {
		return static_cast<float>(TerrainLowRes->GetBridgeHeight(p));
	}
	else {
		return static_cast<float>(TerrainLowRes->GetWaterHeightAt(p));
	}
}

void ED::CGlobalsLogicEx::ToTerrainCoord(const shok::Position& p, int* out)
{
	out[0] = static_cast<int>(std::lroundf(p.X / 100));
	out[1] = static_cast<int>(std::lroundf(p.Y / 100));
}
bool ED::CGlobalsLogicEx::IsCoordValid(const int* out) const
{
	return out[0] >= 0 && out[1] >= 0 && out[0] < Blocking->ArraySizeXY && out[1] < Blocking->ArraySizeXY;
}
EGL::CGLELandscape::BlockingMode ED::CGlobalsLogicEx::GetBlocking(const shok::Position& p) const
{
	int qp[2] = { 0,0 };
	ToTerrainCoord(p, qp);
	if (!IsCoordValid(qp))
		DEBUGGER_BREAK;
	return static_cast<EGL::CGLELandscape::BlockingMode>(Blocking->data[qp[1] * Blocking->ArraySizeXY + qp[0]]);
}

ED::CModelsProps::ModelData& ED::CModelsProps::Get(shok::ModelId id)
{
	int i = static_cast<int>(id);
	return Model.at(i);
}
void ED::CModelsProps::LoadModelDataFromExtraFile(shok::ModelId id)
{
	int i = static_cast<int>(id);
	if (i > static_cast<int>(Model.size()))
		throw std::logic_error{ "somehow the id is too big" };
	if (i == static_cast<int>(Model.size())) {
		auto v = Model.SaveVector();
		v.Vector.emplace_back();
	}
	else {
		Model[i] = ModelData{};
	}
	std::string filename = "Data/Config/Models/";
	filename.append(ModelIdManager->GetNameByID(i));
	filename.append(".xml");
	(*BB::CClassFactory::GlobalObj)->LoadObject(&Model[i], filename.c_str(), ModelData::SerializationData);
}
void ED::CModelsProps::PopModel(shok::ModelId id)
{
	int i = static_cast<int>(id);
	if (i + 1 != static_cast<int>(Model.size()))
		throw std::out_of_range("invalid id");
	auto v = Model.SaveVector();
	v.Vector.pop_back();
}
void ED::CModelsProps::ReloadAllModels()
{
	{
		auto v = Model.SaveVector();
		v.Vector.clear();
		v.Vector.resize(ModelIdManager->size());
	}
	auto** m = reinterpret_cast<BB::CIDManagerEx**>(0xA19F8C);
	*m = ModelIdManager;
	(*BB::CClassFactory::GlobalObj)->LoadObject(this, "Data\\Config\\Models.xml", SerializationData);
	*m = nullptr;
}

inline void(__thiscall* const edcamera_clear)(ED::CCamera* th) = reinterpret_cast<void(__thiscall*)(ED::CCamera*)>(0x467B94);
void ED::CCamera::Clear()
{
	edcamera_clear(this);
}

inline bool(__thiscall* const edcamera_begin)(ED::CCamera* th) = reinterpret_cast<bool(__thiscall*)(ED::CCamera*)>(0x467BAB);
bool ED::CCamera::BeginRender()
{
	return edcamera_begin(this);
}
inline int(__thiscall* const edcamera_end)(ED::CCamera* th) = reinterpret_cast<int(__thiscall*)(ED::CCamera*)>(0x467BE1);
bool ED::CCamera::EndRender()
{
	return edcamera_end(this);
}
inline void(__thiscall* const edcamera_show)(ED::CCamera* th, HWND w) = reinterpret_cast<void(__thiscall*)(ED::CCamera*, HWND)>(0x467BF0);
void ED::CCamera::Show(HWND window)
{
	edcamera_show(this, window);
}

shok::Color ED::CPlayerColors::GetColorByIndex(int i)
{
	if (i < 0 || i > 16)
		return { 0,0,0,0 };
	return ConfigColors[i];
}
void ED::CPlayerColors::SetColorByIndex(int i, shok::Color c)
{
	if (i < 0 || i > 16)
		throw std::out_of_range("invalid color index");
	ConfigColors[i] = c;
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
void* ED::ModelData::operator new(size_t s)
{
	return shok::Malloc(s);
}
void ED::ModelData::operator delete(void* p) {
	shok::Free(p);
}

void ED::CResourceManager::FreeModel(shok::ModelId id)
{
	int i = static_cast<int>(id);
	delete ModelManager.Models[i];
	ModelManager.Models[i] = nullptr;
}
void ED::CResourceManager::PopModel(shok::ModelId id)
{
	int i = static_cast<int>(id);
	if (i + 1 != static_cast<int>(ModelManager.Models.size()))
		throw std::out_of_range("invalid id");
	FreeModel(id);
	auto v = ModelManager.Models.SaveVector();
	v.Vector.pop_back();
}
const ED::ModelData* ED::CResourceManager::LoadModel(shok::ModelId id)
{
	int i = static_cast<int>(id);
	if (i >= static_cast<int>(ModelManager.Models.size())) {
		auto v = ModelManager.Models.SaveVector();
		v.Vector.resize(i + 1, nullptr);
	}
	return GetModelData(id);
}


void ED::CResourceManager::FreeAnim(shok::AnimationId id)
{
	int i = static_cast<int>(id);
	RWE::RtAnimAnimation* a = AnimManager.Get(i);
	a->Destroy();
	AnimManager.Map.erase(AnimManager.Map.find(i));
}

static inline ED::ModelData* (__cdecl* const resmng_loadmodel)(const char* n) = reinterpret_cast<ED::ModelData* (__cdecl*)(const char*)>(0x472D71);
ED::ModelData* ED::CResourceManager::LoadModel(const char* name)
{
	return resmng_loadmodel(name);
}

static inline void(__thiscall* const terraintexturemng_reloadall)(ED::TerrainTextureManager* th) = reinterpret_cast<void(__thiscall*)(ED::TerrainTextureManager*)>(0x470927);
void ED::TerrainTextureManager::ReloadAllTextures()
{
	terraintexturemng_reloadall(this);
}
static inline void(__thiscall* const terraintexturemng_applyquality)(ED::TerrainTextureManager* th) = reinterpret_cast<void(__thiscall*)(ED::TerrainTextureManager*)>(0x4716C5);
void ED::TerrainTextureManager::ApplyTextureQuality()
{
	terraintexturemng_applyquality(this);
}

void ED::TerrainTextureManager::PopTexture(shok::TerrainTextureId id)
{
	int i = static_cast<int>(id);
	if (Textures[i]) {
		Textures[i]->Destroy();
		Textures[i] = nullptr;
	}
	if (i + 1 == static_cast<int>(Textures.size())) {
		auto v = Textures.SaveVector();
		v.Vector.pop_back();
	}
}
void ED::TerrainTextureManager::LoadTexture(shok::TerrainTextureId id)
{
	int i = static_cast<int>(id);
	auto v = Textures.SaveVector();
	if (i + 2 == static_cast<int>(Textures.size())) {
		v.Vector.push_back(RWE::RwTexture::Read(DisplayProps->TerrainTextureManager->GetNameByID(i), nullptr));
	}
	else {
		if (v.Vector.at(i))
			v.Vector[i]->Destroy();
		v.Vector[i] = RWE::RwTexture::Read(DisplayProps->TerrainTextureManager->GetNameByID(i), nullptr);
	}
}
void ED::TerrainTextureManager::ReApplyTerrainType(shok::TerrainTypeId id)
{
	int i = static_cast<int>(id);
	TerrainType t = CreateTerrainType(id);

	std::map<int, int> PriorityToTerrainid{};
	for (unsigned int id2 = 0; id2 < DisplayProps->TerrainTypeManager->size(); ++id2) {
		int prio = DisplayProps->DisplayProps[id2].Priority;
		if (id2 != i && DisplayProps->TerrainTypeManager->GetNameByID(static_cast<int>(id2)) != nullptr) {
			while (PriorityToTerrainid.contains(prio))
				++prio;
			PriorityToTerrainid[prio] = static_cast<int>(id2);
		}
	}
	while (PriorityToTerrainid.contains(t.Priority))
		++t.Priority;
	PriorityToTerrainid[t.Priority] = static_cast<int>(t.Id);
	{
		auto ottv = OriginalTerrainTypes.SaveVector();
		while (static_cast<int>(ottv.Vector.size()) <= i)
			ottv.Vector.emplace_back();
		ottv.Vector[i] = t;
		auto ttv = TerrainTypes.SaveVector();
		while (static_cast<int>(ttv.Vector.size()) <= i)
			ttv.Vector.emplace_back();
		ttv.Vector[i] = t;
		auto trv = TextureReplacement.SaveVector();
		while (static_cast<int>(trv.Vector.size()) <= i)
			trv.Vector.emplace_back();
		trv.Vector[i] = i;
		auto tidsord = TextureIdsOrderedByPriority.SaveVector();
		tidsord.Vector.clear();
		tidsord.Vector.push_back(static_cast<shok::TerrainTypeId>(0));
		for (const auto& kv : PriorityToTerrainid) {
			int s = static_cast<int>(tidsord.Vector.size());
			tidsord.Vector.push_back(static_cast<shok::TerrainTypeId>(kv.second));
			ttv.Vector[kv.second].Priority = s;
			ottv.Vector[kv.second].Priority = s;
		}
	}
	if (TextureQualityOption != 2) {
		ApplyTextureQuality();
		--TextureQualityOptionChangedCounter;
	}
}
void ED::TerrainTextureManager::ReApplyAllTerrainTypes()
{
	{
		auto ottv = OriginalTerrainTypes.SaveVector();
		ottv.Vector.clear();
		auto ttv = TerrainTypes.SaveVector();
		ttv.Vector.clear();
		auto tidsord = TextureIdsOrderedByPriority.SaveVector();
		tidsord.Vector.clear();
		std::map<int, int> PriorityToTerrainid{};
		for (unsigned int id = 0; id < DisplayProps->TerrainTypeManager->size(); ++id) {
			TerrainType t = CreateTerrainType(static_cast<shok::TerrainTypeId>(id));
			if (DisplayProps->TerrainTypeManager->GetNameByID(static_cast<int>(id)) != nullptr) {
				while (PriorityToTerrainid.contains(t.Priority))
					++t.Priority;
				PriorityToTerrainid[t.Priority] = static_cast<int>(t.Id);
			}
			ottv.Vector.push_back(t);
			ttv.Vector.push_back(t);
		}
		tidsord.Vector.push_back(static_cast<shok::TerrainTypeId>(0));
		for (const auto& kv : PriorityToTerrainid) {
			int s = static_cast<int>(tidsord.Vector.size());
			tidsord.Vector.push_back(static_cast<shok::TerrainTypeId>(kv.second));
			ttv.Vector[kv.second].Priority = s;
			ottv.Vector[kv.second].Priority = s;
		}
	}
	BlackInternalUseOnlyId = static_cast<shok::TerrainTypeId>(DisplayProps->TerrainTypeManager->GetIdByName("BlackInternalUseOnly"));
	Transitions01Id = static_cast<shok::TerrainTextureId>(DisplayProps->TerrainTextureManager->GetIdByName("Transitions01"));
	ApplyTextureQuality();
	--TextureQualityOptionChangedCounter;
}
ED::TerrainTextureManager::TerrainType ED::TerrainTextureManager::CreateTerrainType(shok::TerrainTypeId id) const
{
	int i = static_cast<int>(id);
	TerrainType t{};
	const auto& dis = DisplayProps->DisplayProps.at(i);
	t.Id = id;
	t.Priority = dis.Priority;
	t.BaseTextureId = dis.BaseTexture;
	t.SnowTextureId = dis.SnowTexture;
	t.TransitionTextureId = dis.TransitionsTexture;
	t.OneDiv4TimesQuads = 1.0f / static_cast<float>(4 * (dis.Quads <= 0 ? 4 : dis.Quads));
	t.ReplacementTerrainType = static_cast<shok::TerrainTypeId>(DisplayProps->TerrainTypeManager->GetIdByName(dis.ReplacementTerrainType.c_str()));
	t.TransitionsColorModulate = dis.TransitionsColorModulate;
	t.TransitionTextureIs_Transitions01 = t.TransitionTextureId == Transitions01Id;
	return t;
}

static inline void(__thiscall* const entitestypeflags_ctor)(ED::CEntitiesTypeFlags* th, ED::EntityTypeDisplayProps* p) = reinterpret_cast<void(__thiscall*)(ED::CEntitiesTypeFlags*, ED::EntityTypeDisplayProps*)>(0x46FE7E);
ED::CEntitiesTypeFlags::CEntitiesTypeFlags(ED::EntityTypeDisplayProps* entityTypeDisplays)
{
	entitestypeflags_ctor(this, entityTypeDisplays);
}

void* ED::CEntitiesTypeFlags::operator new(size_t s)
{
	return shok::Malloc(s);
}
void ED::CEntitiesTypeFlags::operator delete(void* p) {
	shok::Free(p);
}

inline void(__thiscall* const light_gfxdata_interpolate)(ED::CLight::GFXDataHandler* th, ED::CGfxSetTransitionStatus* status, ED::CLight::LightData* output) = reinterpret_cast<void(__thiscall*)(ED::CLight::GFXDataHandler*, ED::CGfxSetTransitionStatus*, ED::CLight::LightData*)>(0x46D9B6);
void ED::CLight::GFXDataHandler::Interpolate(CGfxSetTransitionStatus* status, LightData* output)
{
	light_gfxdata_interpolate(this, status, output);
}

inline void(__thiscall* const light_update)(ED::CLight* th) = reinterpret_cast<void(__thiscall*)(ED::CLight*)>(0x46D685);
void ED::CLight::Update()
{
	light_update(this);
}

shok::ClassId __stdcall GD::CBuildingEffectsProps::GetClassIdentifier() const
{
	return Identifier;
}

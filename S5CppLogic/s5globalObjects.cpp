#include "pch.h"
#include "s5data.h"
#include <stdexcept>
#include "entityiterator.h"
#include "s5_netevents.h"


struct shok_vtable_BB_IPostEvent {
	void(__stdcall* PostEvent)(shok_BB_IPostEvent* th, BB::CEvent* ev);
};

struct shok_vtable_GGL_CWeatherHandler {
	PADDINGI(1);
	int(__thiscall* GetCurrentWeather)(GGL::CWeatherHandler* th);
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
	void(__thiscall* Destroy)(EGL::CGLEGameLogic* th, bool free);
	PADDINGI(1); // concurrency ref counter destroy?
	float(__thiscall* GetTerrainHeightAtPos)(EGL::CGLEGameLogic* th, float x, float y);
	float(__thiscall* GetWaterHeightAtPos)(EGL::CGLEGameLogic* th, float x, float y);
	float(__thiscall* GetHigherTerrainOrWaterHeightAtPos)(EGL::CGLEGameLogic* th, float x, float y);
	bool(__thiscall* GetLandscapePosFromMousePos)(EGL::CGLEGameLogic* th, void* cam, float* mousepos, shok::PositionRot* outpos, int flag);
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


void shok_EScr_CScriptTriggerSystem::RunTrigger(BB::CEvent* ev)
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

shok_color::shok_color(int r, int g, int b, int a)
{
	R = r & 0xFF;
	G = g & 0xFF;
	B = b & 0xFF;
	A = a & 0xFF;
}

shok_color shok_ED_CPlayerColors::GetColorByIndex(int i)
{
	// todo 16 player
	if (i < 0 || i > 16)
		return { 0,0,0,0 };
	return Colors[i];
}
void shok_ED_CPlayerColors::SetColorByIndex(int i, shok_color c)
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

void shok_ED_CCommandAcknowledgements::ShowAck(const shok::Position& pos)
{
	reinterpret_cast<shok_vtable_ED_CCommandAcknowledgements*>(vtable)->ShowAck(this, pos.X, pos.Y);
}

static inline void(__cdecl* const registermodelinst)(void* robj, shok_modelinstance* i) = reinterpret_cast<void(__cdecl*)(void*, shok_modelinstance*)>(0x627130);
void shok_modelinstance::Register()
{
	registermodelinst((*shok_ED_CGlobalsBaseEx::GlobalObj)->DisplayWorld->SomeRenderObj, this);
}
static inline void* (__cdecl* const modelinst_getsomethingdtor)(shok_modelinstance* i) = reinterpret_cast<void* (__cdecl*)(shok_modelinstance*)>(0x626E00);
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
static inline void(__cdecl* const modelinst_setrot)(void* l, float* f, float r, shok_modelinstance::TransformOperation i) = reinterpret_cast<void(__cdecl*)(void*, float*, float, shok_modelinstance::TransformOperation)>(0x4141D0);
void shok_modelinstance::Rotate(float r, float* axis, TransformOperation op)
{
	modelinst_setrot(Transform, axis, r, op);
}
void shok_modelinstance::Rotate(float r, TransformOperation op)
{
	float f[] = { 0,0,1 };
	Rotate(r, f, op);
}
static inline void(__cdecl* const modelinst_setscale)(void* l, float* f, shok_modelinstance::TransformOperation i) = reinterpret_cast<void(__cdecl*)(void*, float*, shok_modelinstance::TransformOperation)>(0x414170);
void shok_modelinstance::Scale(float* s, TransformOperation op)
{
	modelinst_setscale(Transform, s, op);
}
void shok_modelinstance::Scale(float s, TransformOperation op)
{
	float f[] = { s,s,s };
	Scale(f, op);
}
static inline void(__cdecl* const modelinst_setpos)(void* l, float* f, shok_modelinstance::TransformOperation i) = reinterpret_cast<void(__cdecl*)(void*, float*, shok_modelinstance::TransformOperation)>(0x414140);
void shok_modelinstance::Translate(const shok::Position& p, float height, TransformOperation op)
{
	float f[] = { p.X, p.Y, height };
	modelinst_setpos(Transform, f, op);
}
static inline void(__cdecl* const modelinst_iteratefuncoversomething)(shok_modelinstance* m, int(__cdecl* op)(void* m, void* data), void* data) = reinterpret_cast<void(__cdecl*)(shok_modelinstance*, int(__cdecl*)(void*, void*), void*)>(0x628E30);
static inline int(__cdecl* const modelinst_iter_setcolorbyplayer)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x48F361);
void shok_modelinstance::SetColorByPlayerID(int pl)
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_setcolorbyplayer, &pl);
}
static inline int(__cdecl* const modelinst_iter_disableshadow)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x721FD8);
void shok_modelinstance::DisableShadow()
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_disableshadow, nullptr);
}
static inline int(__cdecl* const modelinst_iter_disableparticleeffects)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x721F87);
void shok_modelinstance::DisableParticleEffects()
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_disableparticleeffects, nullptr);
}
static inline int(__cdecl* const modelinst_iter_disableterraindecal)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x721FB2);
void shok_modelinstance::DisableTerrainDecal()
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_disableterraindecal, nullptr);
}
static inline int(__cdecl* const modelinst_iter_setcolormodulate)(void* m, void* data) = reinterpret_cast<int(__cdecl*)(void*, void*)>(0x47B6E3);
void shok_modelinstance::SetColorModulate(shok_color argb)
{
	modelinst_iteratefuncoversomething(this, modelinst_iter_setcolormodulate, &argb);
}
void shok_modelinstance::SetColorModulate(int a, int r, int g, int b)
{
	a &= 0xFF;
	r &= 0xFF;
	g &= 0xFF;
	b &= 0xFF;
	SetColorModulate({ r, g, b, a });
}

static inline shok_modelinstance* (__thiscall* const modeldata_instanciate)(const shok_modeldata* d) = reinterpret_cast<shok_modelinstance * (__thiscall*)(const shok_modeldata*)> (0x472742);
shok_modelinstance* shok_modeldata::Instanciate() const
{
	return modeldata_instanciate(this);
}
const shok_modeldata* shok_ED_CResourceManager::GetModelData(int modelid)
{
	return reinterpret_cast<shok_vtable_ED_CResourceManager*>(vtable)->GetModelDataByModelID(this, modelid);
}

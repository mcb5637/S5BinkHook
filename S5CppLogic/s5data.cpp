#include "pch.h"

#include "s5data.h"
#include <stdexcept>
#include <libloaderapi.h>


static inline void(__thiscall* str_ctor)(shok_string* th, const char* s) = (void(__thiscall*)(shok_string*, const char*))0x4018C6;
shok_string::shok_string(const char* s)
{
	str_ctor(this, s);
}
const char* shok_string::c_str()
{
	if (size < 16)
		return (const char*)&data;
	else
		return (const char*)data;
}

void shok_position::FloorToBuildingPlacement()
{
	X = std::floorf(X / 100) * 100;
	Y = std::floorf(Y / 100) * 100;
}

float shok_position::GetDistanceSquaredTo(shok_position& p)
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	return (dx * dx + dy * dy);
}

bool shok_position::IsInRange(shok_position& p, float range)
{
	return GetDistanceSquaredTo(p) <= (range * range);
}

float shok_position::GetAngleBetween(shok_position& p)
{
	float dx = X - p.X;
	float dy = Y - p.Y;
	if (dx == 0 && dy == 0)
		return 0;
	float a = std::asinf(std::fabsf(dx) / (std::sqrtf(dx * dx + dy * dy)));
	a = (float)rad2deg((double)a);
	if (dx >= 0 && dy > 0)
		a = 270 - a;
	else if (dx < 0 && dy > 0)
		a = 270 + a;
	else if (dx < 0 && dy <= 0)
		a = 90 - a;
	else if (dx >= 0 && dy <= 0)
		a = 90 + a;
	return a;
}

void RedirectCall(void* call, void* redirect) {
	byte* opcode = (byte*)call;
	if (*opcode != 0xE8) { // call
		DEBUGGER_BREAK
	}
	int* adr = (int*)(opcode+1);
	*adr = ((int)redirect) - ((int)(adr + 1)); // address relative to next instruction
}
void WriteJump(void* adr, void* toJump) {
	byte* opcode = (byte*)adr;
	*opcode = 0xE9; // jmp
	int* a = (int*)(opcode + 1);
	*a = ((int)toJump) - ((int)(a + 1)); // address relative to next instruction
}

bool operator<(shok_attachment a, shok_attachment b) {
	if (a.AttachmentType == b.AttachmentType)
		return a.EntityId < b.EntityId;
	return a.AttachmentType < b.AttachmentType;
}

struct shok_BB_CFileStreamEx {
	int vtable = 0x761C60;
	int x = 0;
};
static inline bool(__thiscall* shok_BB_CFileStreamEx_OpenFile)(shok_BB_CFileStreamEx* th, const char* name, int unk) = (bool(__thiscall*)(shok_BB_CFileStreamEx*, const char*, int)) 0x54924D;
static inline size_t(__stdcall* shok_BB_CFileStreamEx_GetSize)(shok_BB_CFileStreamEx* th) = (size_t(__stdcall*)(shok_BB_CFileStreamEx*)) 0x549140;
static inline int(__stdcall* shok_BB_CFileStreamEx_ReadToBuffer)(shok_BB_CFileStreamEx* th, void* buff, size_t buffsiz) = (int(__stdcall*)(shok_BB_CFileStreamEx*, void*, size_t)) 0x5491A8;
static inline void(__thiscall* shok_BB_CFileStreamEx_Close)(shok_BB_CFileStreamEx* th) = (void(__thiscall*)(shok_BB_CFileStreamEx*))0x54920A;
static inline int(__thiscall* shok_BB_CFileStreamEx_dtor)(shok_BB_CFileStreamEx* th) = (int(__thiscall*)(shok_BB_CFileStreamEx*))0x549215;

const char* ReadFileToString(const char* name, size_t* size)
{
	char* buff = nullptr;
	try
	{
		shok_BB_CFileStreamEx filestr = shok_BB_CFileStreamEx();
		if (shok_BB_CFileStreamEx_OpenFile(&filestr, name, 0x10113)) {
			size_t s = shok_BB_CFileStreamEx_GetSize(&filestr);
			if (size)
				*size = s;
			if (s > 0) {
				buff = new char[s + 1];
				memset(buff, 0, s + 1);
				shok_BB_CFileStreamEx_ReadToBuffer(&filestr, buff, s);
			}
			shok_BB_CFileStreamEx_Close(&filestr);
		}
		shok_BB_CFileStreamEx_dtor(&filestr);
	}
	catch (...)
	{
		if (buff)
			delete[] buff;
		return nullptr;
	}
	return buff;
}

bool DoesFileExist(const char* name)
{

	bool r = false;
	try
	{
		shok_BB_CFileStreamEx filestr = shok_BB_CFileStreamEx();
		if (shok_BB_CFileStreamEx_OpenFile(&filestr, name, 0x10113)) {
			size_t s = shok_BB_CFileStreamEx_GetSize(&filestr);
			if (s > 0) {
				r = true;
			}
			shok_BB_CFileStreamEx_Close(&filestr);
		}
		shok_BB_CFileStreamEx_dtor(&filestr);
	}
	catch (...)
	{
		return false;
	}
	return r;
}

shok_EGL_CGLEEntity* ReplaceEntityWithResourceEntity(shok_EGL_CGLEEntity* e)
{
	shok_EGL_CGLEEntityProps* ty = e->GetEntityType()->LogicProps;
	if (ty->vtable != shok_vtp_GGL_CEntityProperties)
		return nullptr;
	shok_GGL_CEntityProperties* t = (shok_GGL_CEntityProperties*)ty;
	shok_GGL_CResourceDoodadCreator c = shok_GGL_CResourceDoodadCreator();
	c.EntityType = t->ResourceEntity;
	c.PlayerId = e->PlayerId;
	c.Pos = e->Position;
	c.ResourceAmount = t->ResourceAmount;
	c.Scale = e->Scale;
	if (e->ScriptName) {
		size_t len = strlen(e->ScriptName) + 1;
		c.ScriptName = (char*)shok_malloc(sizeof(char) * len);
		strcpy_s(c.ScriptName, len, e->ScriptName);
	}
	else {
		c.ScriptName = nullptr;
	}
	int id = (*shok_EGL_CGLEGameLogicObject)->CreateEntity(&c);
	shok_EGL_CGLEEntity* r = shok_eid2obj(id);
	shok_event_data_EGL_CEventValue_int_27574121 ev = shok_event_data_EGL_CEventValue_int_27574121();
	ev.id = 0x1000C;
	ev.i = e->EntityType;
	((shok_vtable_EGL_CGLEEntity*)r->vtable)->FireEvent(r, &ev);
	e->Destroy();
	return r;
}

static inline float(__thiscall* costinfo_getres)(shok_costInfo* th, int ty, bool addRaw) = (float(__thiscall*)(shok_costInfo*, int, bool))0x4A9606;
float shok_costInfo::GetResourceAmountFromType(int ty, bool addRaw)
{
	return costinfo_getres(this, ty, addRaw);
}

static inline void(__thiscall* costinfo_add)(shok_costInfo* th, int ty, float a) = (void(__thiscall*)(shok_costInfo*, int, float))0x4A9774;
void shok_costInfo::AddToType(int ty, float toadd)
{
	costinfo_add(this, ty, toadd);
}

static inline void(__thiscall* costinfo_sub)(shok_costInfo* th, int ty, float a, float b) = (void(__thiscall*)(shok_costInfo*, int, float, float))0x4A963D;
void shok_costInfo::SubFromType(int ty, float tosub)
{
	costinfo_sub(this, ty, tosub, 0.0f);
}

static inline bool(__thiscall* constinfo_hasres)(shok_costInfo* th, shok_costInfo* has) = (bool(__thiscall*)(shok_costInfo*, shok_costInfo*))0x4A96D3;
bool shok_costInfo::HasResources(shok_costInfo* has)
{
	return constinfo_hasres(this, has);
}

bool HasSCELoader()
{
	return GetModuleHandle(L"SCELoader.dll");
}

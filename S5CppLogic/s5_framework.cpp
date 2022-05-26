#include "pch.h"
#include "s5_framework.h"
#include "hooks.h"

void ECore::IReplayStreamExtension::unknown0()
{
}

int(__thiscall* const cinfo_getindex)(Framework::CampagnInfo* th, const char* s) = reinterpret_cast<int(__thiscall* const)(Framework::CampagnInfo*, const char*)>(0x51A23B);
int Framework::CampagnInfo::GetMapIndexByName(const char* s)
{
    return cinfo_getindex(this, s);
}

Framework::MapInfo* Framework::CampagnInfo::GetMapInfoByName(const char* n)
{
    int i = GetMapIndexByName(n);
    if (i < 0)
        return nullptr;
    return &Maps[i];
}

bool(__thiscall* const savedata_load)(Framework::SaveData* th, const char* s) = reinterpret_cast<bool(__thiscall* const)(Framework::SaveData*, const char*)>(0x403253);
bool Framework::SaveData::LoadSaveData(const char* name)
{
    return savedata_load(this, name);
}

bool(__thiscall* const gdb_iskeyvalid)(GDB::CList* th, const char* k) = reinterpret_cast<bool(__thiscall* const)(GDB::CList*, const char*)>(0x40E038);
bool GDB::CList::IsKeyValid(const char* key)
{
    return gdb_iskeyvalid(this, key);
}
const char* (__thiscall* const gdb_getstring)(GDB::CList* th, const char* k) = reinterpret_cast<const char* (__thiscall* const)(GDB::CList*, const char*)>(0x40E01C);
const char* GDB::CList::GetString(const char* key)
{
    return gdb_getstring(this, key);
}
void(__thiscall* const gdb_setstring)(GDB::CList* th, const char* k, const char* v) = reinterpret_cast<void(__thiscall* const)(GDB::CList*, const char*, const char*)>(0x40E1AD);
void GDB::CList::SetString(const char* key, const char* value)
{
    return gdb_setstring(this, key, value);
}
float(__thiscall* const gdb_getval)(GDB::CList* th, const char* k) = reinterpret_cast<float(__thiscall* const)(GDB::CList*, const char*)>(0x40E000);
float GDB::CList::GetValue(const char* key)
{
    return gdb_getval(this, key);
}
void(__thiscall* const gdb_setval)(GDB::CList* th, const char* k, float v) = reinterpret_cast<void(__thiscall* const)(GDB::CList*, const char*, float)>(0x40E18A);
void GDB::CList::SetValue(const char* key, float value)
{
    gdb_setval(this, key, value);
}

unsigned int __stdcall GDB::CList::GetClassIdentifier() const
{
    return Identifier;
}

static inline bool(__thiscall* const gamemodebase_addarchiveifexternalmap)(Framework::AGameModeBase* th, Framework::GameModeStartMapData* data, GS3DTools::CMapData* map, const char* path) = reinterpret_cast<bool(__thiscall*)(Framework::AGameModeBase*, Framework::GameModeStartMapData*, GS3DTools::CMapData*, const char*)>(0x40F109);
bool Framework::AGameModeBase::AddArchiveIfExternalmap(GameModeStartMapData* data, GS3DTools::CMapData* map, const char* path)
{
    return gamemodebase_addarchiveifexternalmap(this, data, map, path);
}
static inline void(__thiscall* const gamemodebase_removearchiveifexternalmap)(Framework::AGameModeBase* th) = reinterpret_cast<void(__thiscall*)(Framework::AGameModeBase*)>(0x40E47B);
void Framework::AGameModeBase::RemoveArchiveIfExternalmap()
{
    gamemodebase_removearchiveifexternalmap(this);
}

void (*Framework::AGameModeBase::PreStartMap)(lua_State* ingame, const char* name, const char* path, bool externalmap) = nullptr;
static inline bool(__thiscall* const gamemodebase_startmap)(Framework::AGameModeBase* mode, const char* name, const char* path) = reinterpret_cast<bool(__thiscall*)(Framework::AGameModeBase*, const char*, const char*)>(0x40E5F0);
bool __fastcall gamemodebase_startmapoverride(Framework::AGameModeBase* mode, int _, const char* name, const char* path) {
    if (Framework::AGameModeBase::PreStartMap)
        Framework::AGameModeBase::PreStartMap(mode->IngameLuaState, name, path, mode->IsExternalMap);
    return gamemodebase_startmap(mode, name, path);
}
bool HookStartMap_Hooked = false;
void Framework::AGameModeBase::HookStartMap()
{
    if (HookStartMap_Hooked)
        return;
    HookStartMap_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x40F851), 10 };
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F851), &gamemodebase_startmapoverride);
    CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x40F566), 10 };
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F566), &gamemodebase_startmapoverride);
}

bool Framework::AGameModeBase::DoNotRemoveNextArchive = false;
void __fastcall gamemodebase_removearchiveifexternalmap_override(Framework::AGameModeBase* th) {
    if (!Framework::AGameModeBase::DoNotRemoveNextArchive)
        th->RemoveArchiveIfExternalmap();
    Framework::AGameModeBase::DoNotRemoveNextArchive = false;
}
bool HookRemoveArchive_Hooked = false;
void Framework::AGameModeBase::HookRemoveArchive()
{
    if (HookRemoveArchive_Hooked)
        return;
    HookRemoveArchive_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ 0x20, {
        reinterpret_cast<void*>(0x40F8CD),
        reinterpret_cast<void*>(0x40F604)
    } };
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F8CD), &gamemodebase_removearchiveifexternalmap_override);
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F604), &gamemodebase_removearchiveifexternalmap_override);
}


void (*Framework::AGameModeBase::PreLoadSave)(lua_State* ingame, GameModeStartMapData* data, bool externalmap) = nullptr;
void __fastcall gamemodebase_onsaveload(Framework::AGameModeBase* th, Framework::GameModeStartMapData* d) {
    if (Framework::AGameModeBase::PreLoadSave)
        Framework::AGameModeBase::PreLoadSave(th->IngameLuaState, d, th->IsExternalMap);
}
void __declspec(naked) gamemodebase_onloadsave_asm() {
    __asm {
        mov ecx, esi;
        mov edx, [ebp + 8];
        call gamemodebase_onsaveload;

        mov edi, [esi + 0x10];
        mov eax, 0x403158;
        call eax;

        push 0x40FB76;
        ret;
    };
}
bool HookLoadSave_Hooked = false;
void Framework::AGameModeBase::HookLoadSave()
{
    if (HookLoadSave_Hooked)
        return;
    HookLoadSave_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x40FB6E), 0x40FB76 - 0x40FB6E };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x40FB6E), &gamemodebase_onloadsave_asm, reinterpret_cast<void*>(0x40FB76));
}

Framework::CampagnInfo* Framework::CMain::GetCampagnInfo(int i, const char* n)
{
    Framework::CampagnInfo* r = nullptr;
    int* th = &CampagnInfoHandler;
    __asm {
        push n;
        mov ecx, th;
        mov eax, i;
        mov edx, 0x40BB16; // this thing has first param in eax, no known calling convention, so i have to improvise
        call edx;
        pop edx;
        mov r, eax;
    }
    return r;
}
Framework::CampagnInfo*(__thiscall* const framew_getcinfo)(int* th, const GS3DTools::CMapData* s) = reinterpret_cast<Framework::CampagnInfo*(__thiscall* const)(int*, const GS3DTools::CMapData*)>(0x5190D5);
Framework::CampagnInfo* Framework::CMain::GetCampagnInfo(GS3DTools::CMapData* d)
{
    static_assert(offsetof(Framework::CMain, CampagnInfoHandler) == 174 * 4);
    return framew_getcinfo(&CampagnInfoHandler, d);
}

inline void(__thiscall* const framew_savegdb)(Framework::CMain* th) = reinterpret_cast<void(__thiscall* const)(Framework::CMain*)>(0x40AED0);
void Framework::CMain::SaveGDB()
{
    framew_savegdb(this);
}

void (*Framework::CMain::OnModeChange)(NextMode mode) = nullptr;
inline void(__thiscall* const cmain_startmapsp)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40A916);
inline void(__thiscall* const cmain_gotomainmenu)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40AA1B);
inline void(__thiscall* const cmain_loadsave)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40AA76);
inline void(__thiscall* const cmain_startmapmp)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40ACFE);
void __fastcall cmain_checktodo_hooked(Framework::CMain* th) {
    if (th->ToDo != Framework::CMain::NextMode::NoChange)
        if (Framework::CMain::OnModeChange)
            Framework::CMain::OnModeChange(th->ToDo);
    switch (th->ToDo)
    {
    case Framework::CMain::NextMode::StartMapSP:
        cmain_startmapsp(th);
        break;
    case Framework::CMain::NextMode::ToMainMenu:
        cmain_gotomainmenu(th);
        break;
    case Framework::CMain::NextMode::LoadSaveSP:
        cmain_loadsave(th);
        break;
    case Framework::CMain::NextMode::StartMapMP:
        cmain_startmapmp(th);
        break;
    case Framework::CMain::NextMode::RestartMapSP:
        cmain_gotomainmenu(th);
        cmain_startmapsp(th);
        break;
    case Framework::CMain::NextMode::LeaveGame:
        PostQuitMessage(0);
        break;
    }
    th->ToDo = Framework::CMain::NextMode::NoChange;
}


bool HookModeChange_Hooked = false;
void Framework::CMain::HookModeChange()
{
    if (HookModeChange_Hooked)
        return;
    HookModeChange_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x40B3BE), 0x40B3C7 - 0x40B3BE };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x40B3BE), &cmain_checktodo_hooked, reinterpret_cast<void*>(0x40B3C7));
}

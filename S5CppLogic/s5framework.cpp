#include "pch.h"
#include "s5data.h"

int(__thiscall* const cinfo_getindex)(shok_framework_campagnInfo* th, const char* s) = reinterpret_cast<int(__thiscall* const)(shok_framework_campagnInfo*, const char*)>(0x51A23B);
int shok_framework_campagnInfo::GetMapIndexByName(const char* s)
{
    return cinfo_getindex(this, s);
}

shok_framework_mapinfo* shok_framework_campagnInfo::GetMapInfoByName(const char* n)
{
    int i = GetMapIndexByName(n);
    if (i < 0)
        return nullptr;
    return &Maps[i];
}

bool(__thiscall* const savedata_load)(shok_framework_saveData* th, const char* s) = reinterpret_cast<bool(__thiscall* const)(shok_framework_saveData*, const char*)>(0x403253);
bool shok_framework_saveData::LoadSaveData(const char* name)
{
    return savedata_load(this, name);
}

bool(__thiscall* const gdb_iskeyvalid)(shok_GDB_CList* th, const char* k) = reinterpret_cast<bool(__thiscall* const)(shok_GDB_CList*, const char*)>(0x40E038);
bool shok_GDB_CList::IsKeyValid(const char* key)
{
    return gdb_iskeyvalid(this, key);
}
const char* (__thiscall* const gdb_getstring)(shok_GDB_CList* th, const char* k) = reinterpret_cast<const char* (__thiscall* const)(shok_GDB_CList*, const char*)>(0x40E01C);
const char* shok_GDB_CList::GetString(const char* key)
{
    return gdb_getstring(this, key);
}
void(__thiscall* const gdb_setstring)(shok_GDB_CList* th, const char* k, const char* v) = reinterpret_cast<void(__thiscall* const)(shok_GDB_CList*, const char*, const char*)>(0x40E1AD);
void shok_GDB_CList::SetString(const char* key, const char* value)
{
    return gdb_setstring(this, key, value);
}
float(__thiscall* const gdb_getval)(shok_GDB_CList* th, const char* k) = reinterpret_cast<float(__thiscall* const)(shok_GDB_CList*, const char*)>(0x40E000);
float shok_GDB_CList::GetValue(const char* key)
{
    return gdb_getval(this, key);
}
void(__thiscall* const gdb_setval)(shok_GDB_CList* th, const char* k, float v) = reinterpret_cast<void(__thiscall* const)(shok_GDB_CList*, const char*, float)>(0x40E18A);
void shok_GDB_CList::SetValue(const char* key, float value)
{
    gdb_setval(this, key, value);
}

shok_framework_campagnInfo* shok_Framework_CMain::GetCampagnInfo(int i, const char* n)
{
    shok_framework_campagnInfo* r = nullptr;
    __asm {
        push n;
        mov ecx, this;
        add ecx, 0x2B8;
        mov eax, i;
        mov edx, 0x40BB16; // this thing has first param in eax, no known calling convention, so i have to improvise
        call edx;
        pop edx;
        mov r, eax;
    }
    return r;
}
shok_framework_campagnInfo*(__thiscall* const framew_getcinfo)(shok_Framework_CMain* th, const shok_GS3DTools_CMapData* s) = reinterpret_cast<shok_framework_campagnInfo*(__thiscall* const)(shok_Framework_CMain*, const shok_GS3DTools_CMapData*)>(0x5190D5);
shok_framework_campagnInfo* shok_Framework_CMain::GetCampagnInfo(shok_GS3DTools_CMapData* d)
{
    return framew_getcinfo(this, d);
}

void(__thiscall* const framew_savegdb)(shok_Framework_CMain* th) = reinterpret_cast<void(__thiscall* const)(shok_Framework_CMain*)>(0x40AED0);
void shok_Framework_CMain::SaveGDB()
{
    framew_savegdb(this);
}

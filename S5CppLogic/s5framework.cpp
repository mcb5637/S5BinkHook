#include "pch.h"
#include "s5data.h"

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

Framework::CampagnInfo* Framework::CMain::GetCampagnInfo(int i, const char* n)
{
    Framework::CampagnInfo* r = nullptr;
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
Framework::CampagnInfo*(__thiscall* const framew_getcinfo)(Framework::CMain* th, const GS3DTools::CMapData* s) = reinterpret_cast<Framework::CampagnInfo*(__thiscall* const)(Framework::CMain*, const GS3DTools::CMapData*)>(0x5190D5);
Framework::CampagnInfo* Framework::CMain::GetCampagnInfo(GS3DTools::CMapData* d)
{
    return framew_getcinfo(this, d);
}

void(__thiscall* const framew_savegdb)(Framework::CMain* th) = reinterpret_cast<void(__thiscall* const)(Framework::CMain*)>(0x40AED0);
void Framework::CMain::SaveGDB()
{
    framew_savegdb(this);
}

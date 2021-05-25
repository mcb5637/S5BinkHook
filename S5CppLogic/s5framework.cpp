#include "pch.h"
#include "s5data.h"

int(__thiscall* const cinfo_getindex)(shok_framework_campagnInfo* th, const char* s) = (int(__thiscall* const)(shok_framework_campagnInfo*, const char*)) 0x51A23B;
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

shok_framework_campagnInfo* shok_Framework_CMain::GetCampagnInfo(int i, const char* n)
{
    shok_framework_campagnInfo* r = nullptr;
    __asm {
        push n
        mov ecx, this
        add ecx, 0x2B8
        mov eax, i
        mov edx, 0x40BB16 // this thing has first param in eax, no known calling convention, so i have to improvise
        call edx
        pop edx
        mov r, eax
    }
    return r;
}

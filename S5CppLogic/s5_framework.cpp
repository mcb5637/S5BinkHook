#include "pch.h"
#include "s5_framework.h"

#include <format>

#include "s5_mapdisplay.h"
#include "s5_classfactory.h"
#include "s5_exception.h"
#include "hooks.h"

void ECore::IReplayStreamExtension::unknown0()
{
}

void __stdcall GS3DTools::CGUIReplaySystem::PostEvent(BB::CEvent* ev)
{
}
void __stdcall GS3DTools::CGUIReplaySystem::CPlayingReplay::PostEvent(BB::CEvent* ev)
{
}

inline bool(__thiscall* skeys_checknorm)(const Framework::SKeys* th, const Framework::SKeys* map) = reinterpret_cast<bool(__thiscall*)(const Framework::SKeys*, const Framework::SKeys*)>(0x51B9F7);
bool Framework::SKeys::Check(const SKeys& map) const
{
    return skeys_checknorm(this, &map);
}

inline GS3DTools::CMapData* (__thiscall* const mapdata_assign)(GS3DTools::CMapData* th, const GS3DTools::CMapData* o) = reinterpret_cast<GS3DTools::CMapData * (__thiscall*)(GS3DTools::CMapData*, const GS3DTools::CMapData*)>(0x4029B8);
GS3DTools::CMapData& GS3DTools::CMapData::operator=(const GS3DTools::CMapData& o)
{
    if (this == &o)
        return *this;
    return *mapdata_assign(this, &o);
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

void Framework::CampagnInfo::HookLoad()
{
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x51AA04), 0x10 };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x51AA04), CppLogic::Hooks::MemberFuncPointerToVoid(&CampagnInfo::LoadOverride, 0), reinterpret_cast<void*>(0x51AA0E));
}
void __thiscall Framework::CampagnInfo::LoadOverride(const char* path, const SKeys* keys, bool isSp)
{
    auto* fs = *BB::CFileSystemMgr::GlobalObj;
    auto ar = std::unique_ptr<BB::CBBArchiveFile, CppLogic::DestroyCaller<BB::CBBArchiveFile>>(BB::CBBArchiveFile::Create());
    auto seri = std::unique_ptr<BB::CXmlSerializer, CppLogic::DestroyCaller<BB::CXmlSerializer>>(BB::CXmlSerializer::Create());
    CampagnName = strrchr(path, '\\') + 1;
    BB::SerializationData* data = reinterpret_cast<BB::SerializationData * (*)()>(0x51A41D)();
    shok::Set<shok::String> maps{};
    fs->FillFilesInDirectory(&maps, path, BB::IFileSystem::SearchOptions::None);
    auto v = Maps.SaveVector();
    v.Vector.clear();
    for (const shok::String& m : maps) {
        Framework::MapInfo inf{};
        inf.MapFileName = m;
        inf.MapFilePath = std::format("{}\\{}", path, static_cast<std::string_view>(m));
        BB::IFileSystem::FileInfo finf{};
        char abspath[2001]{};
        fs->GetFileInfo(&finf, inf.MapFilePath.c_str(), 0);
        fs->MakeAbsolute(abspath, inf.MapFilePath.c_str());
        if (!finf.IsDirectory) {
            if (!static_cast<std::string_view>(m).ends_with(".s5x"))
                continue;
            try {
                ar->OpenArchive(abspath);
                auto file = std::unique_ptr<BB::IStream>(ar->OpenFileStream("Maps\\ExternalMap\\Info.xml", BB::IStream::Flags::DefaultRead));
                seri->DeserializeByData(file.get(), &inf, data);
                inf.MapFilePath = abspath;
                inf.MapFileName = static_cast<std::string_view>(m).substr(0, m.size() - 4);
                ar->CloseArchive();
            }
            catch (const BB::CFileException&) {
                ar->CloseArchive();
                continue;
            }
            inf.IsExternalmap = true;
        }
        else {
            auto infoxml = std::format("{}\\{}", abspath[0] ? abspath : inf.MapFilePath.c_str(), "Info.xml");
            BB::CFileStreamEx file{};
            try {
                file.OpenFile(infoxml.c_str(), BB::IStream::Flags::DefaultRead);
                seri->DeserializeByData(&file, &inf, data);
                file.Close();
            }
            catch (const BB::CFileException&) {
                file.Close();
                continue;
            }
            inf.IsExternalmap = false;
        }
        if (keys->Check(inf.Keys))
            v.Vector.push_back(inf);
    }
}

inline bool(__thiscall* const savedata_load)(Framework::SavegameSystem* th, const char* s) = reinterpret_cast<bool(__thiscall* const)(Framework::SavegameSystem*, const char*)>(0x403253);
bool Framework::SavegameSystem::LoadSaveData(const char* name)
{
    return savedata_load(this, name);
}

inline void(__thiscall* const savesys_save)(Framework::SavegameSystem* th, const char* slot, GS3DTools::CMapData* mapdata, const char* name, bool debugSave) = reinterpret_cast<void(__thiscall*)(Framework::SavegameSystem*, const char*, GS3DTools::CMapData*, const char*, bool)>(0x4031C4);
void Framework::SavegameSystem::SaveGame(const char* slot, GS3DTools::CMapData* mapdata, const char* name, bool debugSave)
{
    savesys_save(this, slot, mapdata, name, debugSave);
}

void (*Framework::SavegameSystem::OnGameSavedTo)(const char* folder, const char* savename) = nullptr;
inline void(__thiscall* const savedata_save)(Framework::SaveData* th, const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name) = reinterpret_cast<void(__thiscall*)(Framework::SaveData*, const char*, GGL::CGLGameLogic*, GS3DTools::CMapData*, const char*)>(0x402DAD);

void __thiscall Framework::SaveData::SaveGameOverride(const char* path, GGL::CGLGameLogic* gl, GS3DTools::CMapData* mapdata, const char* name)
{
    savedata_save(this, path, gl, mapdata, name);
    if (Framework::SavegameSystem::OnGameSavedTo)
        Framework::SavegameSystem::OnGameSavedTo(path, name);
}
void Framework::SavegameSystem::HookSaveGame()
{
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x403235), 0x10 };
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x403235), CppLogic::Hooks::MemberFuncPointerToVoid(&SaveData::SaveGameOverride, 0));
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

void GDB::CList::RemoveKey(const std::string& k)
{
    size_t off = k.find('\\');
    if (off == std::string::npos) {
        shok::String s{ k.c_str() };
        auto i = Entries.find(s);
        if (i != Entries.end()) {
            delete i->second;
            Entries.erase(i);
        }
    }
    else {
        std::string key = k.substr(0, off);
        std::string next = k.substr(off + 1);
        shok::String s{ key.c_str() };
        auto i = Entries.find(s);
        if (i != Entries.end()) {
            if (auto* n = dynamic_cast<GDB::CList*>(i->second)) {
                n->RemoveKey(next);
            }
        }
    }
}

inline void(__thiscall* const timemng_setfixedupdate)(CTimeManager* th, double t) = reinterpret_cast<void(__thiscall*)(CTimeManager*, double)>(0x406D5E);
void CTimeManager::SetFixedUpdateTime(double t)
{
    timemng_setfixedupdate(this, t);
}
inline void(__thiscall* const timemng_settimefactor)(CTimeManager* th, double t) = reinterpret_cast<void(__thiscall*)(CTimeManager*, double)>(0x406E23);
void CTimeManager::SetTimeFactor(double f)
{
    timemng_settimefactor(this, f);
}

void __stdcall Framework::CEventTimeManager::PostEvent(BB::CEvent* ev)
{
    throw 0;
}
void __stdcall Framework::CCheckSumCalculator::PostEvent(BB::CEvent* ev)
{
    throw 0;
}
void __stdcall Framework::CEscapeEventHandler::PostEvent(BB::CEvent* ev)
{
    throw 0;
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
void (*Framework::AGameModeBase::PreStartMap2)(lua_State* ingame, const char* name, const char* path, bool externalmap) = nullptr;
static inline bool(__thiscall* const gamemodebase_startmap)(Framework::AGameModeBase* mode, const char* name, const char* path) = reinterpret_cast<bool(__thiscall*)(Framework::AGameModeBase*, const char*, const char*)>(0x40E5F0);
bool __thiscall Framework::AGameModeBase::StartMapOverride(const char* name, const char* path)
{
    if (Framework::AGameModeBase::PreStartMap2)
        Framework::AGameModeBase::PreStartMap2(IngameLuaState, name, path, IsExternalMap);
    if (Framework::AGameModeBase::PreStartMap)
        Framework::AGameModeBase::PreStartMap(IngameLuaState, name, path, IsExternalMap);
    return gamemodebase_startmap(this, name, path);
}
bool HookStartMap_Hooked = false;
void Framework::AGameModeBase::HookStartMap()
{
    if (HookStartMap_Hooked)
        return;
    HookStartMap_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ 0x10, {reinterpret_cast<void*>(0x40F851),
        reinterpret_cast<void*>(0x40F566)
    } };
    // sp
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F851), CppLogic::Hooks::MemberFuncPointerToVoid(&AGameModeBase::StartMapOverride, 0));
    // mp
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F566), CppLogic::Hooks::MemberFuncPointerToVoid(&AGameModeBase::StartMapOverride, 0));
}

bool Framework::AGameModeBase::DoNotRemoveNextArchive = false; void __thiscall Framework::AGameModeBase::RemoveArchiveIfExternalmapOverride()
{
    if (!Framework::AGameModeBase::DoNotRemoveNextArchive)
        RemoveArchiveIfExternalmap();
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
        reinterpret_cast<void*>(0x40F604),
        reinterpret_cast<void*>(0x40FC68)
    } };
    //sp load map
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F8CD), CppLogic::Hooks::MemberFuncPointerToVoid(&AGameModeBase::RemoveArchiveIfExternalmapOverride, 0));
    // mp
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40F604), CppLogic::Hooks::MemberFuncPointerToVoid(&AGameModeBase::RemoveArchiveIfExternalmapOverride, 0));
    // sp load save
    CppLogic::Hooks::RedirectCall(reinterpret_cast<void*>(0x40FC68), CppLogic::Hooks::MemberFuncPointerToVoid(&AGameModeBase::RemoveArchiveIfExternalmapOverride, 0));
}


void (*Framework::AGameModeBase::PreLoadSave)(lua_State* ingame, GameModeStartMapData* data, bool externalmap) = nullptr;
void (*Framework::AGameModeBase::PreLoadSave2)(lua_State* ingame, GameModeStartMapData* data, bool externalmap) = nullptr;
void __fastcall Framework::AGameModeBase::OnSaveLoadedEx(Framework::GameModeStartMapData* d)
{
    if (Framework::AGameModeBase::PreLoadSave2)
        Framework::AGameModeBase::PreLoadSave2(IngameLuaState, d, IsExternalMap);
    if (Framework::AGameModeBase::PreLoadSave)
        Framework::AGameModeBase::PreLoadSave(IngameLuaState, d, IsExternalMap);
}
void __declspec(naked) gamemodebase_onloadsave_asm() {
    __asm {
        mov ecx, esi;
        mov edx, [ebp + 8];
        call Framework::AGameModeBase::OnSaveLoadedEx;

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

void __stdcall Framework::CSinglePlayerMode::CNetworkEvent::PostEvent(BB::CEvent* ev)
{
    throw 0;
}
void __stdcall Framework::CMultiPlayerMode::CNetworkEvent::PostEvent(BB::CEvent* ev)
{
    throw 0;
}

Framework::CampagnInfo* Framework::CMain::CIH::GetCampagnInfo(shok::MapType i, const char* n)
{
    int i2 = static_cast<int>(i);
    if (i2 < -1 || i2 >= 4)
        return nullptr;
    Framework::CampagnInfo* r = nullptr;
    auto* th = this;
    __asm {
        push n;
        mov ecx, th;
        mov eax, i2;
        mov edx, 0x40BB16; // this thing has first param in eax, no known calling convention, so i have to improvise
        call edx;
        pop edx;
        mov r, eax;
    }
    return r;
}
Framework::CampagnInfo*(__thiscall* const framew_getcinfo)(void* th, const GS3DTools::CMapData* s) = reinterpret_cast<Framework::CampagnInfo*(__thiscall* const)(void*, const GS3DTools::CMapData*)>(0x5190D5);
Framework::CampagnInfo* Framework::CMain::CIH::GetCampagnInfo(GS3DTools::CMapData* d)
{
    return framew_getcinfo(this, d);
}

inline void(__thiscall* const framew_savegdb)(Framework::CMain* th) = reinterpret_cast<void(__thiscall* const)(Framework::CMain*)>(0x40AED0);
void Framework::CMain::SaveGDB()
{
    framew_savegdb(this);
}
inline ED::CGUICamera* (__thiscall* const framew_getcam)(Framework::CMain* th) = reinterpret_cast<ED::CGUICamera * (__thiscall*)(Framework::CMain*)>(0x4088AF);
ED::CGUICamera* Framework::CMain::GetCamera()
{
    return framew_getcam(this);
}

void (*Framework::CMain::OnModeChange)(NextMode mode) = nullptr;
void (*Framework::CMain::OnSaveLoaded)() = nullptr;
inline void(__thiscall* const cmain_startmapsp)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40A916);
inline void(__thiscall* const cmain_gotomainmenu)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40AA1B);
inline void(__thiscall* const cmain_loadsave)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40AA76);
inline void(__thiscall* const cmain_startmapmp)(Framework::CMain* th) = reinterpret_cast<void(__thiscall*)(Framework::CMain*)>(0x40ACFE);
void __thiscall Framework::CMain::CheckToDoOverride()
{
    if (ToDo != Framework::CMain::NextMode::NoChange)
        if (Framework::CMain::OnModeChange)
            Framework::CMain::OnModeChange(ToDo);
    switch (ToDo)
    {
    case Framework::CMain::NextMode::StartMapSP:
        cmain_startmapsp(this);
        break;
    case Framework::CMain::NextMode::ToMainMenu:
        cmain_gotomainmenu(this);
        break;
    case Framework::CMain::NextMode::LoadSaveSP:
        cmain_loadsave(this);
        if (Framework::CMain::OnSaveLoaded)
            Framework::CMain::OnSaveLoaded();
        break;
    case Framework::CMain::NextMode::StartMapMP:
        cmain_startmapmp(this);
        break;
    case Framework::CMain::NextMode::RestartMapSP:
        cmain_gotomainmenu(this);
        cmain_startmapsp(this);
        break;
    case Framework::CMain::NextMode::LeaveGame:
        PostQuitMessage(0);
        break;
    }
    ToDo = Framework::CMain::NextMode::NoChange;
}


bool HookModeChange_Hooked = false;
void Framework::CMain::HookModeChange()
{
    if (HookModeChange_Hooked)
        return;
    HookModeChange_Hooked = true;
    CppLogic::Hooks::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x40B3BE), 0x40B3C7 - 0x40B3BE };
    CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x40B3BE), CppLogic::Hooks::MemberFuncPointerToVoid(&CMain::CheckToDoOverride, 0), reinterpret_cast<void*>(0x40B3C7));
}

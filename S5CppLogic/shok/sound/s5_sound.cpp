#include "pch.h"
#include "s5_sound.h"
#include <shok/globals/s5_filesystem.h>
#include <shok/globals/s5_idmanager.h>
#include <utility/hooks.h>

void ESnd::ISoEMusic::StartMusic(const char* path, int vol, bool loop)
{
	const auto f = reinterpret_cast<void(__thiscall*)(ESnd::ISoEMusic*, const char*, int, bool)>(0x4965a3);
	f(this, path, vol, loop);
}
void ESnd::ISoEMusic::StopMusic()
{
	const auto f = reinterpret_cast<void(__thiscall*)(ESnd::ISoEMusic*)>(0x496721);
	f(this);
}

static inline void(__thiscall* const music_pause)(ESnd::CSoEMusic* th, bool p) = reinterpret_cast<void(__thiscall*)(ESnd::CSoEMusic*, bool)>(0x4964DF);
void ESnd::CSoEMusic::PauseMusic(bool p)
{
	music_pause(this, p);
}

static inline void(__thiscall* const music_setvoladj)(ESnd::CSoEMusic* th, float f) = reinterpret_cast<void(__thiscall*)(ESnd::CSoEMusic*, float)>(0x4963CE);
void ESnd::CSoEMusic::SetVolumeAdjustment(float ad)
{
	music_setvoladj(this, ad);
}

static inline int(__stdcall** const AIL_openstream)(int th, const char* path, int ze) = reinterpret_cast<int(__stdcall**)(int, const char*, int)>(0x761488);
static inline void(__stdcall** const AIL_setsreamuserdata)(int streamh, int idx, int data) = reinterpret_cast<void(__stdcall**)(int, int, int)>(0x761514);
static inline int(__stdcall** const AIL_getstreamuserdata)(int streamh, int idx) = reinterpret_cast<int(__stdcall**)(int, int)>(0x761520);
static inline void(__stdcall** const AIL_closestream)(int streamh) = reinterpret_cast<void(__stdcall**)(int)>(0x7614A4);
void(__stdcall* AIL_closestreamBackup)(int streamh) = nullptr;
void __stdcall ail_closestream_override(int streamh) {
	int h = (*AIL_getstreamuserdata)(streamh, 2); // check handle and close it
	if (h) {
		BB::CFileSystemMgr::CloseHandle(h);
	}
	AIL_closestreamBackup(streamh);
}
int __stdcall startmusic_patch(const char* path, int th) {
	int handle = 0;
	size_t s = 0;
	std::string_view pathview = path;
	if (pathview._Starts_with("data") || pathview._Starts_with("Data")) {
		path += 4;
		if (path[0] == '/' || path[0] == '\\')
			path++;
	}
	if (!(*BB::CFileSystemMgr::GlobalObj)->OpenFileAsHandle(path, handle, s)) {
		return 0;
	}
	char tmp[255] = {};
	// this string tells openstream the handle and filesize
	snprintf(tmp, 254, "\\\\\\\\%i,%i%s", handle, s, BB::CFileSystemMgr::PathGetExtension(path));
	int h = (*AIL_openstream)(th, tmp, 0); // first param is some string, maybe output format?
	if (!h) {
		BB::CFileSystemMgr::CloseHandle(handle);
		return 0;
	}
	(*AIL_setsreamuserdata)(h, 2, handle); // remember handle, so we can close it later
	return h;
}
void __declspec(naked) startmusic_patchasm() {
	__asm {
		mov eax, 0x859F0C; // mov eax, [0x859F0C] does for some reason generate mov eax, 0x859F0C
		mov eax, [eax];
		push [eax + 0xC];
		push[ebp + 8];
		call startmusic_patch;

		push 0x49669E;
		ret;
	};
}


void ESnd::CSoEMusic::HookStartMusicFilesystem()
{
	CppLogic::Hooks::SaveVirtualProtect vp{ AIL_closestream ,4 };
	AIL_closestreamBackup = *AIL_closestream;
	*AIL_closestream = &ail_closestream_override;

	CppLogic::Hooks::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x496677), 0x30 };
	CppLogic::Hooks::WriteJump(reinterpret_cast<void*>(0x496677), &startmusic_patchasm, reinterpret_cast<void*>(0x49669E));
}

static inline void(__thiscall* const sound_play2d)(ESnd::CSoESound* th, int* id, shok::SoundId sound, int vol, bool looped, float x, float y, float z) = reinterpret_cast<void(__thiscall*)(ESnd::CSoESound*, int*, shok::SoundId, int, bool, float, float, float)>(0x494B5B);
int ESnd::CSoESound::Play2DSound(shok::SoundId sound, int vol, bool looped)
{
	int id;
	sound_play2d(this, &id, sound, vol, looped, 0, 0, 0);
	return id;
}

static inline void(__thiscall* const sound_pauseall)(ESnd::CSoESound* th, bool p) = reinterpret_cast<void(__thiscall*)(ESnd::CSoESound*, bool)>(0x49399F);
void ESnd::CSoESound::PauseAll(bool pause)
{
	sound_pauseall(this, pause);
}

static inline void(__thiscall* const sound_pause3d)(ESnd::CSoESound* th, bool p) = reinterpret_cast<void(__thiscall*)(ESnd::CSoESound*, bool)>(0x4939DE);
void ESnd::CSoESound::Pause3d(bool pause)
{
	sound_pause3d(this, pause);
}

shok::SoundId ESnd::CSoESound::AddSoundToNewGroup(const char* name)
{
	int id = (*BB::CIDManagerEx::SoundsManager)->GetIDByNameOrCreate(name);
	{
		auto v = UnknownVector.SaveVector();
		v.Vector.push_back(0);
	}
	{
		auto v = RandomData.SaveVector();
		IdRandomData d{reinterpret_cast<void*>(0x19F1B4), static_cast<shok::SoundId>(id), static_cast<shok::SoundId>(id)};
		v.Vector.push_back(d);
	}
	return static_cast<shok::SoundId>(id);
}
shok::SoundId ESnd::CSoESound::AddSoundToLastGroup(const char* name)
{
	int id = (*BB::CIDManagerEx::SoundsManager)->GetIDByNameOrCreate(name);
	{
		auto v = UnknownVector.SaveVector();
		v.Vector.push_back(0);
	}
	{
		auto v = RandomData.SaveVector();
		v.Vector.back().MaxRan = static_cast<shok::SoundId>(id);
	}
	return static_cast<shok::SoundId>(id);
}
void ESnd::CSoESound::PopSoundGroup(shok::SoundId firstsound)
{
	auto rdv = RandomData.SaveVector();
	auto& rd = rdv.Vector.back();
	if (rd.MinRan > firstsound || rd.MaxRan < firstsound)
		throw std::out_of_range{ "id not in last group" };
	auto ukv = UnknownVector.SaveVector();
	for (int i = static_cast<int>(rd.MaxRan);i >= static_cast<int>(rd.MinRan); --i) {
		ukv.Vector.pop_back();
		(*BB::CIDManagerEx::SoundsManager)->RemoveID(i);
	}
	rdv.Vector.pop_back();
}

#include "pch.h"
#include "s5data.h"

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
	shok::SaveVirtualProtect vp{ AIL_closestream ,4 };
	AIL_closestreamBackup = *AIL_closestream;
	*AIL_closestream = &ail_closestream_override;

	shok::SaveVirtualProtect vp2{ reinterpret_cast<void*>(0x496677) ,10 };
	WriteJump(reinterpret_cast<void*>(0x496677), &startmusic_patchasm);
}

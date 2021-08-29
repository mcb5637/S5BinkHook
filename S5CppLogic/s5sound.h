#pragma once
#include "s5data.h"

struct shok_ESnd_CSoEMusic : shok_object {
	PADDINGI(18);
	float VolumeAdjustment;


	void StartMusic(const char* path, int vol, bool loop);
	void PauseMusic(bool p);
	void SetVolumeAdjustment(float ad);

	static inline constexpr int vtp = 0x76D37C;
	static inline constexpr int TypeDesc = 0x80D3C0;

	static inline shok_ESnd_CSoEMusic** const GlobalObj = reinterpret_cast<shok_ESnd_CSoEMusic**>(0x859FD4);

	static void HookStartMusicFilesystem();
};
//constexpr int i = offsetof(shok_ESnd_CSoEMusic, VolumeAdjustment)/4;

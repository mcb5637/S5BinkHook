#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace ESnd {
	class IAmbientSoundInfo {
		virtual void unknowno() = 0;
	};

	class ISoEMusic {
	public:
		virtual void StartMusic(const char* path, int vol, bool loop) = 0;
		virtual void StopMusic() = 0;
		// 2 more vfuncs
	};

	class CSoEMusic : public ISoEMusic {
		PADDINGI(18);
		float VolumeAdjustment;


		void PauseMusic(bool p);
		void SetVolumeAdjustment(float ad);

		static inline constexpr int vtp = 0x76D37C;
		static inline constexpr int TypeDesc = 0x80D3C0;

		static inline ESnd::CSoEMusic** const GlobalObj = reinterpret_cast<ESnd::CSoEMusic**>(0x859FD4);

		static void HookStartMusicFilesystem();
	};
	//constexpr int i = offsetof(ESnd::CSoEMusic, VolumeAdjustment)/4;
}

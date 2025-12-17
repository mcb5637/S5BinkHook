#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>

namespace ESnd {
	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class IAmbientSoundInfo {
		virtual void unknowno() = 0;
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class ISoEMusic {
	public:
		virtual void StartMusic(const char* path, int vol, bool loop);
		virtual void StopMusic();
		// 2 more vfuncs
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CSoEMusic : public ISoEMusic {
		PADDINGI(18);
		float VolumeAdjustment = false; // 19
		PADDINGI(1); // a bool?


		void PauseMusic(bool p);
		void SetVolumeAdjustment(float ad);

		static inline constexpr int vtp = 0x76D37C;
		static inline constexpr int TypeDesc = 0x80D3C0;

		static inline ESnd::CSoEMusic** const GlobalObj = reinterpret_cast<ESnd::CSoEMusic**>(0x859FD4);

		static void HookStartMusicFilesystem();
	};
	//constexpr int i = offsetof(ESnd::CSoEMusic, VolumeAdjustment)/4;
	static_assert(sizeof(CSoEMusic) == 21 * 4);

	class AmbientSoundManager {
	public:
		struct AmbientSoundData {
			int AmbientSoundType = 0;
			struct {
				int SoundNormalName = 0;
				int SoundRainName = 0;
				int SoundSnowName = 0;
				bool Looped = false;
				PADDING(3);
				int Probability = 0;
				int MaxVolume = 0;
				float FallOffDistance = 0.0f;
			} PropertyData;
		};


		BB::CIDManagerEx* AmbientSoundIdManager = nullptr;
		shok::Vector<AmbientSoundData> AmbientSound;
	};
	static_assert(sizeof(AmbientSoundManager::AmbientSoundData) == 8 * 4);

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class ISoESoundPlayBack {
	public:
		virtual BB::CIDManagerEx* __stdcall GetSoundManager() = 0;
		// 3 more, then dtor

		static inline constexpr int vtp = 0x76D2CC;
	};
	class ISoESound : public BB::IPostEvent {
	public:
		virtual ISoESoundPlayBack* __stdcall GetPlayback() = 0;
		virtual CSoEMusic* __stdcall GetMusic() = 0;
	private:
		virtual void __stdcall unknown(float) = 0; // update?, param is current time float
	public:
		virtual void __stdcall Set3DPosition(float x, float y, float z) = 0;
		virtual void __stdcall Set3DOrientation(float, float, float, float, float, float) = 0; // 5 parameters??
	private:
		virtual void __stdcall uk1(int, int) = 0;
		virtual void __stdcall uk2(int) = 0;
		virtual int __stdcall uk3() = 0;
	public:
		virtual void __stdcall SetVolumeAdjustment(int t, float v) = 0;
		virtual ~ISoESound() = default; // 10



		static inline constexpr int vtp = 0x76D2E4;

	};
	class ISoESoundEx : public ISoESound {
	public:
		virtual void __stdcall Destroy() = 0;
		virtual void __stdcall LoadIds(const char* filename) = 0;
		virtual void __stdcall LoadAmbientSounds() = 0;
		virtual void __stdcall InitLuaState(lua_State L) = 0;
	};
	class CSoESound : public ISoESoundEx,  public ISoESoundPlayBack { // 15 v funcs
	public:
		// 12 load ids
		static inline constexpr int vtp = 0x76D334;

		static inline CSoESound** const GlobalObj = reinterpret_cast<CSoESound**>(0x859F10);

		struct IdRandomData {
			void* Str; // points to stack, not useable
			shok::SoundId MinRan;
			shok::SoundId MaxRan;
		};

		PADDINGI(32);
		shok::Vector<int> UnknownVector; // 34
		shok::Vector<IdRandomData> RandomData; // 38
		PADDINGI(4); // another vector?
		int RandomSeed = 0; // 46
		CSoEMusic Music; // 47
		PADDINGI(7);
		AmbientSoundManager AmbientSound; // 75

		int Play2DSound(shok::SoundId sound, int vol, bool looped);
		void PauseAll(bool pause);
		void Pause3d(bool pause);

		shok::SoundId AddSoundToNewGroup(const char* name);
		shok::SoundId AddSoundToLastGroup(const char* name);
		void PopSoundGroup(shok::SoundId firstsound);
	};
	static_assert(offsetof(CSoESound, UnknownVector) == 34 * 4);
	static_assert(offsetof(CSoESound, AmbientSound) == 75 * 4);
	//constexpr int i = offsetof(CSoESound, Music) / 4;
}

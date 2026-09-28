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
		[[nodiscard]] virtual const char* GetCurrentlyPlaying() const;
		[[nodiscard]] virtual int GetVolume() const;
	};

	// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
	class CSoEMusic : public ISoEMusic {
		PADDINGI(10);
		// next 2 only set when looped
		shok::String CurrentlyPlaying;
		int Volume;
		float VolumeAdjustment = false; // 19
		PADDINGI(1); // a bool?


		void PauseMusic(bool p);
		void SetVolumeAdjustment(float ad);

		static inline constexpr int vtp = 0x76D37C;
		static inline constexpr int TypeDesc = 0x80D3C0;

		static inline ESnd::CSoEMusic** const GlobalObj = reinterpret_cast<ESnd::CSoEMusic**>(0x859FD4);

		static void HookStartMusicFilesystem();

		// ctor 49654b

		struct SavegameData {
			shok::String Music;
			int Volume;

			static inline auto* const SerializationData = reinterpret_cast<const BB::SerializationData*(__stdcall*)()>(0x40276e);
		};
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
		// no idea what 1st param is, int* success or some id?, returns it
		virtual void* __stdcall Start3DSound(void*, shok::SoundId sound, float x, float y, float z, int volume, bool looped) = 0;
		// 2 more, then dtor

		static inline constexpr int vtp = 0x76D2CC;
	};
	class ISoESound : public BB::IPostEvent {
	public:
		enum class VolumeAdjustType : int {
			Main = 0,
			SoundEffect = 1,
			Music = 2,
			Voice = 3,
			Feedback = 4,
		};
		enum class LuaAPIVolumeAdjustType : int {
			Main = 0,
			Ambient = 5,
			Voice = 4,
		};

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
		virtual void __stdcall SetVolumeAdjustment(VolumeAdjustType t, float v) = 0;
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
	class CSoESound : public ISoESoundEx, public ISoESoundPlayBack { // 15 v funcs
	public:
		// 12 load ids
		static inline constexpr int vtp = 0x76D334;

		static inline CSoESound** const GlobalObj = reinterpret_cast<CSoESound**>(0x859F10);
		// get globalobj 493322

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
		float MainVolumeAdjustment = 0.0f;
		float UnknownEffectVolumeAdjustment = 0.0f;
		float FeedbackVolumeAdjustment = 0.0f;
		float EffectVolumeAdjustment = 0.0f;
		float Voice1VolumeAdjustment = 0.0f;
		PADDINGI(1);
		float Voice2VolumeAdjustment = 0.0f;
		AmbientSoundManager AmbientSound; // 75
		PADDINGI(8);
		float AmbientVolumeAdjustment = 0.0f;
		PADDING(40);
		bool PausedAll = false;
		bool Paused3D = false;
		PADDING(2);

		int Play2DSound(shok::SoundId sound, int vol, bool looped);
		void PauseAll(bool pause);
		void Pause3d(bool pause);

		shok::SoundId AddSoundToNewGroup(const char* name);
		shok::SoundId AddSoundToLastGroup(const char* name);
		void PopSoundGroup(shok::SoundId firstsound);

		// ctor 49535c
	};
	static_assert(offsetof(CSoESound, UnknownVector) == 34 * 4);
	static_assert(offsetof(CSoESound, AmbientSound) == 75 * 4);
	//constexpr int i = offsetof(CSoESound, Music) / 4;
}

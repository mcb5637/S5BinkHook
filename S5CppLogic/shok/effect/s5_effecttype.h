#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>

namespace EGL {
	class CGLEEffectProps : public BB::IObject, public ECore::ICheckData {
	public:
		shok::ClassId Class{};
		bool Global = false;
		PADDING(3);
		float Duration = 0.0f;
		PADDINGI(1); // probably some modified duration
		shok::SoundId SoundID{};
		int SoundVolume = 0;
		bool SoundLooped = false; // 8
		PADDING(3);


		static inline constexpr int vtp = 0x777120;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1E9EDB9D);
	};

	class CFlyingEffectProps : public CGLEEffectProps {
	public:
		float Speed = 1.0f; // 9
		float GravityFactor = 0.0f;
		float HeightOffsetStart = 0.0f;
		float HeightOffsetTarget = 0.0f; // 12

		static inline constexpr int vtp = 0x777590;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9D656D6D);
	};
}

namespace GGL {
	class CGLEffectFireProps : public EGL::CGLEEffectProps {
	public:
		int SpreadTime = 300;
		int SpreadDelay = 300;
		float SpreadRadius = 12.5f;
		shok::SoundId SoundIDAgain{}; // 12

		static inline constexpr int vtp = 0x77713C;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8BA99D0);

		static void HookFixCtor();
	};

	class CCannonBallEffectProps : public EGL::CFlyingEffectProps {
	public:
		int Damage = 0;
		float Radius = 1.0f;
		shok::EffectTypeId ImpactEffectID{}; // 15

		static inline constexpr int vtp = 0x777620;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xC46E396D);
	};

	class CArrowEffectProps : public EGL::CFlyingEffectProps {
	public:
		int Damage = 0;
		shok::EffectTypeId ImpactEffectID{}; // 14


		static inline constexpr int vtp = 0x778DD0;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x696E8EED);
	};

	class CGLEffectLightningProps : public EGL::CGLEEffectProps {
	public:
		int IgniteDelay = 1; // 9
		int DestructionDelay = 5;
		float SpawnRadius = 3.0f;

		static inline constexpr int vtp = 0x779D20;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x322AE630);
	};
}

namespace ED {
	class CEffectProps : public BB::IObject {
	public:
		shok::ClassId Class{};
		shok::Vector<int> Texture; // weathereffecttexture

		static inline constexpr int vtp = 0x76ABA4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1B2E185D);
	};

	class CEffectParticleSystemProps : public ED::CEffectProps {
	public:
		shok::ModelId Model{}; // 6

		static inline constexpr int vtp = 0x7AE7AC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x124ACAED);
	};

	class CHAnimEffectProps : public ED::CEffectProps {
	public:
		shok::ModelId Model{}; // 6
		shok::AnimationId Anim{};
		bool UsePlayerID = false;
		PADDING(3);

		static inline constexpr int vtp = 0x7AE9A8;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x8D2B3BE7);
	};

	class CFlyingEffectProps : public ED::CEffectProps {
	public:
		shok::ModelId Model{}; // 6

		static inline constexpr int vtp = 0x7AE9EC;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFEB81B27);
	};
}

namespace GD {
	class CEffectSnowProps : public ED::CEffectProps { // NOLINT(*-pro-type-member-init)
	public:
		unsigned int MaxParticlesNum;
		unsigned int NewParticlesPerSecond;
		unsigned int DeletedParticlesPerSecond;
		float ParticleSize;
		float ParticleZSpeedMin;
		float ParticleZSpeedMax;
		float CylinderRadius;
		float CylinderZMin;
		float CylinderZMax;
		float FlowSimulationDeltaTime;
		float FlowSimulationTimeScale;
		float WindSpeedScale;
		float WindViscosity;
		float AppliedForceMin;
		float AppliedForceMax;
		float ForceTimeMin;
		float ForceTimeMax; // 22

		static inline constexpr int vtp = 0x76ABB4;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x3101C3A3);
	};

	class CDisplayEffectRainProps : public ED::CEffectProps { // NOLINT(*-pro-type-member-init)
	public:
		int MaxParticlesNumber;
		float NewParticlesPerSecond;
		float MinLength;
		float MaxLength;
		shok::Color HeadColor;
		shok::Color TailColor;
		float CylinderRadius;
		float CylinderHeightUp;
		float CylinderHeightDown;
		float FallSpeed;
		float DirectionRandomRatio; // 16

		static inline constexpr int vtp = 0x76AD54;
		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x2C96B613);
	};
}

namespace EGL {
	class CGLEEffectsProps : public ECore::ICheckData {
	public:
		BB::CIDManagerEx* EffectTypeManager;
		shok::Vector<CGLEEffectProps*> EffectTypes;

		inline CGLEEffectProps* GetLogic(shok::EffectTypeId id) {
			return EffectTypes.at(static_cast<int>(id));
		}


		static inline constexpr int vtp = 0x7888F8;
		static inline CGLEEffectsProps** const GlobalObj = reinterpret_cast<CGLEEffectsProps**>(0x895DB4);
	};
	class EffectsDisplayProps {
	public:
		BB::CIDManagerEx* EffectTypeManager = nullptr;
		shok::Vector<ED::CEffectProps*> EffectTypes;
	};
	struct EffectType {
		PADDINGI(1);
		CGLEEffectProps* Logic = nullptr;
		ED::CEffectProps* Display = nullptr;


		static inline BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA0CCD8);
	};
	class EffectsProps {
	public:
		using EffectType = EffectType;
		shok::Vector<EffectType> Effects;
	};
}

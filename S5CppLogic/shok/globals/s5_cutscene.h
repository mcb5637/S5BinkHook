#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/engine/s5_RWEngine.h>

namespace ECS {
	class ICallbacks {
	public:
		virtual void OnStart(const char* cutsceneName) = 0;
		virtual void OnFinished(const char* cutsceneName) = 0;
		virtual void OnTick(RWE::RwV3d*, RWE::RwV3d*) = 0; // probably cam position, lookat?
	};
	// Framework::CCutsceneCallBackReceiver : ICallbacks

	class ICutsceneManager {
	public:
		static inline constexpr int vtp = 0x786084;
		virtual ~ICutsceneManager() = default;
		virtual bool IsCutsceneRunning() = 0;
		virtual bool SetCallback(ICallbacks* cb) = 0;
		virtual void ReloadCutscene(const char* path) = 0; // 3
		virtual void SaveCutscenesTo(const char* path) = 0;
		virtual void Cancel() = 0; // 5
		virtual void Start(const char* name) = 0;
		virtual void Tick(float, float) = 0; // params from framework time mng
	};

	struct Cutscene {
		struct PosWithTangents {
			RWE::RwV3d Position, InTangent, OutTangent;

			static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA05E38);
		};

		struct PositionFlight {
			shok::Map<float, PosWithTangents> Element; // as Time, Data::Positios

			static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA05FD8);
		};
		struct PositionFlightList {
			shok::Map<float, PositionFlight> Element; // as Time, Data::Positios

			static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA06260);
		};

		struct FuncFlight {
			shok::Map<float, shok::String> Element; // as Time, FuncName

			static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA06108);
		};
		struct FuncFlightList {
			shok::Map<float, FuncFlight> Element;

			static inline const BB::SerializationData* SerializationData = reinterpret_cast<BB::SerializationData*>(0xA06320);
		};

		shok::String Name;
		PositionFlightList CameraPos; // 7 CameraPos::Flights
		PositionFlightList CameraLookAt; // CameraLookAt::Flights
		FuncFlightList LuaFunctions; // LuaFunctions::Flights
		int Status = 0; // 16, 2->normal?, 1->finished?, 0->paused?
		float StartTime1 = 0;
		float StartTime2 = 0;
		float Time1Last = 0;
		float Time1Current = 0; // 20
		float Time2Last = 0;
		float Time2Currrent = 0;
		float Duration = 0; // 23 just a guess

		static inline const BB::SerializationData* (__stdcall * const SerializationData)() = reinterpret_cast<const BB::SerializationData*(__stdcall*)()>(0x59A344);

		// start 0x59AC07 (this,float,float)
		// update timers 0x59AC62 (this,float,float)
		// get cam pos 0x59AC9C (this,RWE::RwV3d*)
		// get lookat pos 0x59ACB9 (this,RWE::RwV3d*)
		// get update funcs 0x59ACFE (this,shok::Vector<shok::String>*)
	};
	static_assert(sizeof(Cutscene) == 24 * 4);
	static_assert(sizeof(Cutscene::PosWithTangents) == 9 * 4);
	//constexpr int i = offsetof(Cutscene, i) / 4;

	// cutscene manager
	class CManager : public ICutsceneManager { 
	public:
		shok::Map<shok::String, Cutscene> Cutscenes;
		shok::String ActiveCutscene; // 4
		PADDINGI(2); // 11, 2 floats, current tick, last tick? from framework time mng
		ICallbacks* Callbacks; // on cutscnene start?
		lua_State* LuaState; // 14

		static inline constexpr int vtp = 0x7860B4;

		static inline ECS::CManager*** const GlobalObj = reinterpret_cast<ECS::CManager***>(0xA0344C);
	};
	static_assert(sizeof(CManager) == 15 * 4);
	//constexpr int i = offsetof(CManager, t) / 4;
}

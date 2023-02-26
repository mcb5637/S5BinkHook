#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace ECS {
	class ICallbacks {
	public:
		virtual void OnStart(const char* cutsceneName) = 0;
		virtual void OnFinished(const char* cutsceneName) = 0;
		virtual void OnTick(float[3], float[3]) = 0; // probably cam position, lookat?
	};
	// Framework::CCutsceneCallBackReceiver : ICallbacks

	class ICutsceneManager {
	public:
		static inline constexpr int vtp = 0x786084;
		virtual ~ICutsceneManager() = default;
		virtual bool IsCutsceneRunning() = 0;
	protected:
		virtual bool setmember13(int) = 0;
	public:
		virtual void ReloadCutscene(const char* path) = 0; // 3
	protected:
		virtual void unknownFailedAnalys() = 0;
	public:
		virtual void Cancel() = 0; // 5
		virtual void Start(const char* name) = 0;
		virtual void Tick(float, float) = 0; // params from framework time mng
	};

	struct Cutscene {
		shok::String Name;
		PADDINGI(3); // cam pos
		PADDINGI(3); // cam lookat
		PADDINGI(3); // luafunc
		PADDINGI(8); // single ints/floats?
	};
	static_assert(sizeof(Cutscene) == 24 * 4);

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

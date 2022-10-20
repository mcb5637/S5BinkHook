#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace ECS {
	class ICutsceneManager {
	public:
		static inline constexpr int vtp = 0x786084;
		virtual ~ICutsceneManager() = default;
	protected:
		virtual bool member9nonzero() = 0; // is cutscene running?
		virtual bool setmember13(int) = 0;
	public:
		virtual void ReloadCutscene(const char* path) = 0; // 3
	protected:
		virtual void unknownFailedAnalys() = 0;
	public:
		virtual void Cancel() = 0; // 5
		virtual void Start(const char* name) = 0;
	protected:
		virtual void Finish(float, int) = 0; // at least t calls finished in lua
	};

	// c manager
	class CManager : public ICutsceneManager { 
	public:
		// map string->cutscene

		static inline constexpr int vtp = 0x7860B4;

		static inline ECS::CManager*** const GlobalObj = reinterpret_cast<ECS::CManager***>(0xA0344C);
	};
}

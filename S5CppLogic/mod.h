#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entitydisplay.h"
#include "s5_ui.h"
#include "s5_widget.h"

namespace CppLogic::Mod {
	class OnScreenInfoDisplayBehavior : public ED::IBehavior {
	public:
		virtual bool RenderUI(GGUI::OnScreenInfoRenderer* renderer, shok::Position* screenPos, GGL::IGLGUIInterface::UIData* data, bool* active) = 0;
	};

	// note that you need to override CastToIdentifier for this
	class IFormationBehaviorExtProvider {
	public:
		virtual shok::Position GetPosExt(EGL::CGLEEntity* leader) = 0;


		static inline constexpr unsigned int Identifier = 0x1008;
	};
}

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
}

#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entitydisplay.h"
#include "s5_ui.h"
#include "s5_widget.h"

namespace CppLogic::Mod {
	// note that you need to override CastToIdentifier for this
	class OnScreenInfoDisplayBehavior : public ED::IBehavior {
	public:
		virtual bool RenderUI(GGUI::OnScreenInfoRenderer* renderer, shok::Position* screenPos, GGL::IGLGUIInterface::UIData* data, bool* active) = 0;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x100A);
	};

	// note that you need to override CastToIdentifier for this
	class IFormationBehaviorExtProvider {
	public:
		virtual shok::Position GetPosExt(EGL::CGLEEntity* leader) = 0;


		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1008);
	};
}

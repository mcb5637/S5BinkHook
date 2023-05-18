#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_widget.h"

#include "mod.h"


namespace CppLogic::Mod::UI {
	class CustomWidgetRenderTest : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(unsigned int id) override;

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords) override;
		virtual bool HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain) override;

		bool a = false;

		static constexpr unsigned int Identifier = 0x100C;
		static constexpr BB::SerializationData* SerializationData = nullptr;

		void* operator new(size_t s);
		void operator delete(void* p);

		static void Register();
	};
}

#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_widget.h"

#include "mod.h"


namespace CppLogic::Mod::UI {
	void RegisterClasses();

	// string user var 0 scrollbar handle name
	// string user var 1 to scroll widget blueprint
	// int user var 0 spacing (space between scrolled widgets)
	// int user var 1 slider spacing (space between slider and top/bottom)
	// current item can be read out on the scrollables by XGUIEng.GetBaseWidgetUserVariable(XGUIEng.GetCurrentWidgetID(), 0) [0-ElementCount)
	class AutoScrollCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(unsigned int id) override;

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords) override;
		virtual bool HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain) override;

		float Offset = 0;
		int ElementCount = 0;
		int WidgetCount = 0;
		std::vector<EGUIX::CBaseWidget*> Widgets;
		EGUIX::CBaseWidget* Slider = nullptr;
		bool Dragging = false;

		static constexpr unsigned int Identifier = 0x100C;
		static constexpr BB::SerializationData* SerializationData = nullptr;

		void* operator new(size_t s);
		void operator delete(void* p);

		void ReInit();
		void Update(EGUIX::CBaseWidget* w);
		void Clamp(EGUIX::CBaseWidget* cw);
	private:
		void UpdateBySlider(EGUIX::CBaseWidget* cw, int x, int y);
		bool ClickedOnSlider(EGUIX::CBaseWidget* cw, int x, int y);
	};
}

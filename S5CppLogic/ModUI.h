#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_widget.h"

#include "mod.h"


namespace CppLogic::Mod::UI {
	void RegisterClasses();

	// string user var 0 scrollbar handle name (may be empty)
	// string user var 1 to scroll widget blueprint
	// int user var 0 spacing (space between scrolled widgets)
	// current item can be read out on the scrollables by XGUIEng.GetBaseWidgetUserVariable(XGUIEng.GetCurrentWidgetID(), 0) [0-ElementCount)
	// set up EGUIX::CContainerWidget with AutoScrollCustomWidget. add one toScroll widget (may be EGUIX::CContainerWidget) and set its name in AutoScrollCustomWidget.
	//		(this widget will get cloned to fill the space of the Container).
	//		(you may put a EGUIX::CContainerWidget between the toScroll widget and the AutoScrollCustomWidget mother widget to make the mouse sensitive area bigger than the toScroll list).
	// if you want to use the bar (and not just the mousewheel) add a Container where the scrollbar travels (has to be directly in the same
	//		Container as the AutoScrollCustomWidget) and a EGUIX::CStaticWidget as the scroll handle into it. then just set the handles name in AutoScrollCustomWidget).
	//		You also might want to add up and down Buttons over and under the scrollbar travel container.
	//		The scrollbar handle will be set visible only if it is needed, so you might want to do the same to any background and button elements.
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
		EGUIX::CBaseWidget* Slider = nullptr, * SliderTravel = nullptr;
		EGUIX::CContainerWidget* WidgetContainer = nullptr;
		bool Dragging = false;
		EGUIX::CMaterial PartialWidget;

		static constexpr unsigned int Identifier = 0x100C;
		static constexpr BB::SerializationData* SerializationData = nullptr;

		void* operator new(size_t s);
		void operator delete(void* p);

		void ReInit();
		void Update();
		void Clamp();
	private:
		void UpdateBySlider(int x, int y);
		bool ClickedOnSlider(int x, int y);
		float UIOffset() const;
	};
}

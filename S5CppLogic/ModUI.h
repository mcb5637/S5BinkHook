#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_widget.h"

#include "mod.h"


namespace CppLogic::Mod::UI {
	void RegisterClasses();

	class AutoScrollCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

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

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x100C);
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

	class InputFocusWidget {
		bool Active = false;
	protected:

		bool CheckFocusEvent(BB::CEvent* ev);

	public:
		void GetFocus();
		void ClearFocus();
		bool HasFocus();
	};

	class TextInputCustomWidget : public BB::IObject, public EGUIX::ICustomWidget, public InputFocusWidget {
	public:
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords) override;
		virtual bool HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain) override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x100E);
		static constexpr BB::SerializationData* SerializationData = nullptr;

		void* operator new(size_t s);
		void operator delete(void* p);

		std::string CurrentTextRaw;
		std::string CurrentTextDisplay;
		EGUIX::CFontIDHandler Font;
		size_t CurrentPos = 0;
		bool IgnoreNextChar = false;

		std::string ClearTextOutput() const;
		void RefreshDisplayText();

	private:
		bool CharValid(char c) const;
		void CallFunc(std::string_view funcname, int ev);
	};

	class FreeCamCustomWidget : public BB::IObject, public EGUIX::ICustomWidget, public InputFocusWidget {
	public:
		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords) override;
		virtual bool HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* ev, BB::CEvent* evAgain) override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1011);
		static constexpr BB::SerializationData* SerializationData = nullptr;

		void* operator new(size_t s);
		void operator delete(void* p);

		bool Forward = false, Backward = false, Right = false, Left = false;
		bool RotateLeft = false, RotateRight = false, Up = false, Down = false;
		int LastTick = 0;
	};
}

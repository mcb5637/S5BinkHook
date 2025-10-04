#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_widget.h"
#include "EnumIdManagerMagic.h"

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

		inline const shok::String& SliderName() const {
			return StringUserVariable[0];
		}
		inline const shok::String& ScollableName() const {
			return StringUserVariable[1];
		}
		inline int ScollableSpacing() const {
			return IntegerUserVariable0;
		}
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
		enum class Event : int {
			Confirm = 0,
			Cancel = 1,
			Validate = 2,
		};
		enum class Modes : int {
			Normal = 0,
			Password = 1,
			Int = 2,
			Double = 3,
			UInt = 4,
			UDouble = 5,
		};
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
		size_t CurrentPosInDisplay = 0;
		bool IgnoreNextChar = false;

		std::pair<std::string, size_t> ClearTextOutput() const;
		void RefreshDisplayText();

		inline shok::String& EventFunc() {
			return StringUserVariable[0];
		}
		inline const shok::String& FontName() const {
			return StringUserVariable[1];
		}
		inline Modes Mode() const {
			return static_cast<Modes>(IntegerUserVariable0);
		}
		inline Event Flags() const {
			return static_cast<Event>(IntegerUserVariable1);
		}
		inline bool HasFlag(Event f) const {
			return static_cast<int>(Flags()) & static_cast<int>(f);
		}
		inline shok::Color TextColor() const {
			return std::bit_cast<shok::Color>(IntegerUserVariable2);
		}
		inline shok::Color BlinkColor() const {
			return std::bit_cast<shok::Color>(IntegerUserVariable3);
		}
		inline shok::Color BackgroundColor() const {
			return std::bit_cast<shok::Color>(IntegerUserVariable4);
		}
		inline int ScrollDelta() const {
			return IntegerUserVariable5;
		}
	private:
		bool CharValid(char c) const;
		bool NegativeNumberValid() const;
		bool CallFunc(std::string_view funcname, Event ev);
		void TryPaste();
		bool Validate();
		void TryCopy() const;
	};
	template<>
	class ::enum_is_flags<TextInputCustomWidget::Event> : public std::true_type {};

	class FreeCamCustomWidget : public BB::IObject, public EGUIX::ICustomWidget, public InputFocusWidget {
		enum class MouseStatus : uint8_t {
			None,
			Forward,
			Backward,
			Rotate,
		};

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

	private:
		bool Forward = false, Backward = false, Right = false, Left = false;
		bool RotateLeft = false, RotateRight = false, Up = false, Down = false;
		bool RotateUp = false, RotateDown = false;
		MouseStatus Status = MouseStatus::None;
		float LastTick = 0;
		int MouseStartX = 0, MouseStartY = 0;
		int MouseX = 0, MouseY = 0;

		inline float MouseInverted(const EGUIX::CCustomWidget* cw) const {
			return cw->UserVariable[1] ? 1.0f : -1.0f;
		}
		inline int& CurrentSensitivity(EGUIX::CCustomWidget* cw) {
			return cw->UserVariable[0];
		}
		inline int DefaultScrollSpeed() const {
			return IntegerUserVariable0;
		}

		static void ClampCamera(ERwTools::CRwCameraHandler* cam);
		static void ClampLookAt(ERwTools::CRwCameraHandler* cam);
	};
}

template<>
inline auto CppLogic::GetIdManager<CppLogic::Mod::UI::TextInputCustomWidget::Event>() {
	return CppLogic::MagicEnum::EnumIdManager<CppLogic::Mod::UI::TextInputCustomWidget::Event>();
}

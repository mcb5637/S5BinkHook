#pragma once
#include "s5data.h"
#include <list>

namespace EGUIX {
	struct Rect { // size 4
		float X, Y, W, H;
	};

	struct Color { // size 4
		int Red, Green, Blue, Alpha; // >=0 && <=0xFF
	};

	class TextureManager { // no vtable, does this thing do only gui textures, or something else too?
	public:
		BB::CIDManager* IdManager;
		PADDINGI(4);

		int GetTextureID(const char* name);

		static inline TextureManager* (__stdcall* const GlobalObj)() = reinterpret_cast<TextureManager * (__stdcall*)()>(0x556644);
	};


	class CFontIDHandler : public BB::IObject { // size 2
	public:
		int FontID;

		static inline constexpr int vtp = 0x780B0C;
		static inline constexpr int TypeDesc = 0x833B40;
		static inline constexpr unsigned int Identifier = 0x34D5D406;

		virtual unsigned int __stdcall GetClassIdentifier() const override;

		void LoadFont(const char* name);
	};
	static_assert(sizeof(CFontIDHandler) == 2 * 4);

	class CSingleStringHandler : public BB::IObject { // size 15
	public:
		shok::String StringTableKey;
		shok::String RawString;

		const char* GetString();

		static inline constexpr int vtp = 0x7809A4;
		static inline constexpr int TypeDesc = 0x833530;
		static inline constexpr unsigned int Identifier = 0x1B070026;

		virtual unsigned int __stdcall GetClassIdentifier() const override;
	};
	static_assert(sizeof(EGUIX::CSingleStringHandler) == 15 * 4);

	class CWidgetStringHelper : public BB::IObject { // size 23
	public:
		EGUIX::CFontIDHandler FontHandler;
		EGUIX::CSingleStringHandler SingleStringHandler;
		float StringFrameDistance;
		EGUIX::Color Color;

		static inline constexpr int vtp = 0x781518;
		static inline constexpr int TypeDesc = 0x834FDC;
		static inline constexpr unsigned int Identifier = 0x53185D06;

		virtual unsigned int __stdcall GetClassIdentifier() const override;
	};
	static_assert(sizeof(EGUIX::CWidgetStringHelper) == 23 * 4);

	class CMaterial : public BB::IObject {
	public:
		int Texture; // type? // 1
		EGUIX::Rect TextureCoordinates; // la 5
		EGUIX::Color Color; // la 10

		static inline constexpr int vtp = 0x780890;
		static inline constexpr int TypeDesc = 0x832A90;
		static inline constexpr unsigned int Identifier = 0xF1CAFF46;

		void SetTexture(const char* name);

		virtual unsigned int __stdcall GetClassIdentifier() const override;
	};
	static_assert(sizeof(EGUIX::CMaterial) == 10 * 4);

	class CLuaFunctionHelper : public::BB::IObject { // size 20
	public:
		shok::String LuaCommand;
		EScr::CLuaFuncRefCommand FuncRefCommand;

		static inline constexpr int vtp = 0x780994;
		static inline constexpr int TypeDesc = 0x8333B8;
		static inline constexpr unsigned int Identifier = 0xCBC5B326;

		virtual unsigned int __stdcall GetClassIdentifier() const override;

		void Call(int widgetID);
	};
	static_assert(sizeof(EGUIX::CLuaFunctionHelper) == 20 * 4);

	class CButtonHelper : public BB::IObject { // size 38
	public:
		int CurrentState; // probably current model disabled 3, highlighted 4, klicked 2, mouseover 1, normal 0
		bool DisabledFlag, HighLightedFlag;
		PADDING(2);
		EGUIX::CLuaFunctionHelper ActionFunction;
		EGUIX::CSingleStringHandler ShortCutString;

		void PressButton(int widgetID);

		static void HookShortcutSignExtend();
		static void HookShortcutComparison();

		static inline constexpr int vtp = 0x780818;
		static inline constexpr int TypeDesc = 0x832918;
		static inline constexpr unsigned int Identifier = 0x813DE3B6;

		virtual unsigned int __stdcall GetClassIdentifier() const override;
	};
	static_assert(sizeof(EGUIX::CButtonHelper) == 38 * 4);

	class IWidgetRegistrationCallback {
	public:
		virtual void OnRegistrationChanged(int id, int mode) = 0; // mode ==2 sem to be unregister
	};

	class CToolTipHelper : public BB::IObject, public IWidgetRegistrationCallback { // size 41
	public:
		bool ToolTipEnabledFlag;
		PADDING(3);
		EGUIX::CSingleStringHandler ToolTipString;
		int TargetWidget;
		bool ControlTargetWidgetDisplayState;
		PADDING(3);
		EGUIX::CLuaFunctionHelper UpdateFunction;
		bool IsToolTipShown;
		PADDING(3);

		static inline constexpr int vtp = 0x780B30;
		static inline constexpr int TypeDesc = 0x833CB8;
		static inline constexpr unsigned int Identifier = 0x102E66C6;

		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void OnRegistrationChanged(int id, int mode) override;
	};
	static_assert(sizeof(EGUIX::CToolTipHelper) == 41 * 4);

	class IOnEvent {
	public:
		virtual ~IOnEvent() = default;
		virtual int HandleEvent(BB::CEvent* ev, int uk) = 0; // return id of wid that handeled event or 0?

		static inline constexpr unsigned int Identifier = 0xF284ED96;
	};
	class IRender {
	public:
		virtual ~IRender() = default;
	private:
		virtual bool Render(float* uk) = 0; // possibly possize rect?
	};
	class IMaterialAccess {
	public:
		virtual ~IMaterialAccess() = default;
		virtual bool SetTexture(int state, const char* name) = 0;
		virtual bool SetColor(int state, int r, int g, int b, int a) = 0;
		virtual CMaterial* GetMaterial(int state) = 0;

		static inline constexpr unsigned int Identifier = 0x55F6D0B6;
	};
	class ITextAccess {
	public:
		virtual ~ITextAccess() = default;
		virtual const char* GetText() = 0;
		virtual bool SetTextRaw(const char* txt, int one) = 0; // 2nd param encoding?
		virtual bool SetTextKey(const char* key) = 0;
		virtual bool AddRawTextAtEnd(const char* txt, int one) = 0;
		virtual bool InsertRawTextInFront(const char* txt, int one) = 0;
		virtual bool LimitTextLines(int max, int num) = 0;
		virtual bool SetLinesToPrint(int first, int num) = 0;
		virtual void SetColor(int r, int g, int b, int a) = 0;

		static inline constexpr unsigned int Identifier = 0x7B4C9DB6;
	};

	class CBaseWidget : public BB::IObject, public IOnEvent { // size 14
	public:
		int UserVariable[2];
		int WidgetID; // 4
		EGUIX::Rect PosAndSize;
		bool IsShown; //9
		PADDING(3);
		float ZPriority;
		int MotherWidgetID;
		int Group; // 12
		bool ForceToHandleMouseEventsFlag, ForceToNeverBeFoundFlag;
		PADDING(2); // 13

		static inline constexpr int vtp = 0x780768;
		static inline constexpr int TypeDesc = 0x82D93C;
		static inline constexpr unsigned int Identifier = 0x18736A06;
		// 5598E3 ctor

		virtual CBaseWidget* Clone() = 0; // 3
		virtual void Initialize() = 0;
	private:
		virtual void unknown0() = 0; // does something on wid geristr cb on mother wid? 5
	public:
		virtual bool SetName(const char* name) = 0; // no idea what return is
		virtual int SetPosAndSize(float x, float y, float w, float h) = 0;
		virtual bool SetVisibility(bool vis) = 0; // returns changed
		virtual int GetMouseOverWidget(float* mousePosrelativeToWidget, int uk1, int uk2) = 0; // 10
		virtual bool IsWidgetID(int id) = 0; // not sure what this is exactly about
	private:
		virtual bool retzero() = 0;
	public:

		bool* GetUpdateManualFlag();
		EGUIX::CLuaFunctionHelper* GetUpdateFunc();
		EGUIX::CMaterial* GetMaterials(int* count);
		EGUIX::CButtonHelper* GetButtonHelper();
		EGUIX::CToolTipHelper* GetTooltipHelper();
		EGUIX::CWidgetStringHelper* GetStringHelper();
	};
	static_assert(sizeof(CBaseWidget) == 14 * 4);

	class CWidgetListHandler {
	public:
		virtual ~CWidgetListHandler() = default;
		shok::List<EGUIX::CBaseWidget*> SubWidgets;

		static inline constexpr int vtp = 0x78098C;
		static inline constexpr int TypeDesc = 0x833240;
	};
	static_assert(sizeof(CWidgetListHandler) == 4 * 4);

	class CStaticWidget : public EGUIX::CBaseWidget, public IRender, public IMaterialAccess { // irenderer vt 14
	public:
		EGUIX::CMaterial BackgroundMaterial; // 16 la 24

		static inline constexpr int vtp = 0x780F84;
		static inline constexpr int TypeDesc = 0x82DF64;
		static inline constexpr unsigned int Identifier = 0x213A8776;

		static EGUIX::CStaticWidget* Create();
	};
	static_assert(offsetof(CStaticWidget, BackgroundMaterial) == 16 * 4);

	class CStaticTextWidget : public EGUIX::CStaticWidget, public ITextAccess {
	public:
		EGUIX::CWidgetStringHelper StringHelper; // 27
		EGUIX::CLuaFunctionHelper UpdateFunction; // 50
		bool UpdateManualFlag;
		PADDING(3);
		int FirstLineToPrint, NumberOfLinesToPrint;
		float LineDistanceFactor;

		static inline constexpr int vtp = 0x780EE4;
		static inline constexpr int TypeDesc = 0x82D95C;
		static inline constexpr unsigned int Identifier = 0x86E3BC06;

		static EGUIX::CStaticTextWidget* Create();
	};
	static_assert(offsetof(CStaticTextWidget, StringHelper) == 27 * 4);

	class CButtonWidget : public EGUIX::CBaseWidget, public IRender, public IMaterialAccess { // irender 14
	public:
		EGUIX::CButtonHelper ButtonHelper; // 16 la 54
		EGUIX::CMaterial Materials[5];
		int NumMaterials; // ? why after materials
		EGUIX::CToolTipHelper ToolTipHelper; // 105 la 146

		static inline constexpr int vtp = 0x780E5C;
		static inline constexpr int TypeDesc = 0x834148;
		static inline constexpr unsigned int Identifier = 0x0FE028496;
		// 55F782 ctor
	};
	static_assert(sizeof(EGUIX::CButtonWidget) == 146 * 4);
	static_assert(offsetof(CButtonWidget, ButtonHelper) == 16 * 4);

	class CGfxButtonWidget : public EGUIX::CButtonWidget {
	public:
		EGUIX::CMaterial IconMaterial;
		EGUIX::CLuaFunctionHelper UpdateFunction;
		bool UpdateManualFlag;
		PADDING(3);

		static inline constexpr int vtp = 0x780CD0;
		static inline constexpr int TypeDesc = 0x82DB14;
		static inline constexpr unsigned int Identifier = 0x56DDA656;

		static EGUIX::CGfxButtonWidget* Create();
	};

	class CTextButtonWidget : public EGUIX::CButtonWidget, public ITextAccess { // textacc 147
	public:
		EGUIX::CWidgetStringHelper StringHelper; // la 170
		EGUIX::CLuaFunctionHelper UpdateFunction;
		bool UpdateManualFlag;
		PADDING(3);

		static inline constexpr int vtp = 0x780DB0;
		static inline constexpr int TypeDesc = 0x82DF3C;
		static inline constexpr unsigned int Identifier = 0x5DD085A6;

		static EGUIX::CTextButtonWidget* Create();
	};

	class CContainerWidget : public EGUIX::CBaseWidget, public IRender, public IWidgetRegistrationCallback { // irender 15
	public:
		EGUIX::CWidgetListHandler WidgetListHandler;

		static inline constexpr int vtp = 0x78114C;
		static inline constexpr int TypeDesc = 0x8321D8;
		static inline constexpr unsigned int Identifier = 0x55F9D1F6;

	protected:
		virtual void AddChild(CBaseWidget* add) = 0;
		// remove child?

	public:
		void AddWidget(EGUIX::CBaseWidget* toAdd, const char* name, const EGUIX::CBaseWidget* before);
		static EGUIX::CContainerWidget* Create();
	};
	static_assert(sizeof(CContainerWidget) == 20 * 4);

	class CPureTooltipWidget : public EGUIX::CBaseWidget {
	public:
		EGUIX::CToolTipHelper ToolTipHelper;

		static inline constexpr int vtp = 0x780BB0;
		static inline constexpr int TypeDesc = 0x833E30;
		static inline constexpr unsigned int Identifier = 0x82CC8876;

		static EGUIX::CPureTooltipWidget* Create();
	};

	class CProgressBarWidget : public EGUIX::CStaticWidget {
	public:
		EGUIX::CLuaFunctionHelper UpdateFunction;
		bool UpdateManualFlag;
		float ProgressBarValue, ProgressBarLimit;

		static inline constexpr int vtp = 0x780C20;
		static inline constexpr int TypeDesc = 0x833FCC;
		static inline constexpr unsigned int Identifier = 0x72633416;

		static EGUIX::CProgressBarWidget* Create();
	};

	class ICustomWidget {
	public:
		virtual ~ICustomWidget() = default;
		// 5 more funcs
	};

	class CCustomWidget : public EGUIX::CBaseWidget, public IRender {
	public:
		shok::String CustomClassName; // 15
		ICustomWidget* CustomWidget; // 22
		int IntegerUserVariable0DefaultValue, IntegerUserVariable1DefaultValue, IntegerUserVariable2DefaultValue, IntegerUserVariable3DefaultValue,
			IntegerUserVariable4DefaultValue, IntegerUserVariable5DefaultValue;
		shok::String StringUserVariable0DefaultValue, StringUserVariable1DefaultValue; //29

		void InitializeCustomWidget();

		static inline constexpr int vtp = 0x7810F8;
		static inline constexpr int TypeDesc = 0x834820;
		static inline constexpr unsigned int Identifier = 0x7656DB56;
	};
	static_assert(sizeof(EGUIX::CCustomWidget) == 43 * 4);

}

namespace GGUI {
	class C3DOnScreenInformationCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		static inline constexpr int vtp = 0x77D490;
		static inline constexpr int TypeDesc = 0x82E1E8;

		void ShowResourceFloatieOnEntity(int eid, int amount);

		static void HookResourceFloatieShowWood(bool showwood); // unfortunately there is something else that prevents this...

		static inline GGUI::C3DOnScreenInformationCustomWidget** const GlobalObj = reinterpret_cast<GGUI::C3DOnScreenInformationCustomWidget**>(0x882F54);
	};
}


namespace EGUIX {
	class WidgetManager { // this thing has no vtable...
	public:
		BB::CIDManagerEx* WidgetNameManager;
		shok::Vector<EGUIX::CBaseWidget*> Widgets;

		int GetIdByName(const char* name);
		EGUIX::CBaseWidget* GetWidgetByID(int id);
		void RemoveWidget(EGUIX::CBaseWidget* w);

		static inline EGUIX::WidgetManager* (* const GlobalObj)() = reinterpret_cast<EGUIX::WidgetManager * (*)()>(0x558473);
	};

	class CWidgetGroupManager : public IWidgetRegistrationCallback {
	public:
		int GetGroupId(const char* s);
		int CreateGroup(const char* s);

		static inline constexpr int vtp = 0x780978;
		static inline constexpr int TypeDesc = 0x832F70;

		static inline EGUIX::CWidgetGroupManager* (__stdcall* const GlobalObj)() = reinterpret_cast<EGUIX::CWidgetGroupManager * (__stdcall*)()>(0x55B688);
	};

	class Font {
	public:
		PADDINGI(1);
		float Size, Offset, Spacing;
	};

	class FontManager { // no vtable either
	public:
		static void LoadFont(int* outFontID, const char* fontName);
		EGUIX::Font* GetFontObj(int id);

		static inline EGUIX::FontManager* (* const GlobalObj)() = reinterpret_cast<EGUIX::FontManager * (*)()>(0x5593AD);
	};

	extern bool (*UIInput_Char_Callback)(int c);
	extern bool (*UIInput_Key_Callback)(int c, win_mouseEvents ev);
	extern bool (*UIInput_Mouse_Callback)(win_mouseEvents id, int w, int l);
	extern bool (*UIInput_Mouse_CallbackMainMenu)(win_mouseEvents id, int w, int l);
	void HookUIInput();

	class FeedbackEventHandler { // another no vtable
	public:
		void FireEvent(BB::CEvent* ev);

		static inline EGUIX::FeedbackEventHandler* (* const GlobalObj)() = reinterpret_cast<EGUIX::FeedbackEventHandler * (*)()>(0x582EE1);
	};
}

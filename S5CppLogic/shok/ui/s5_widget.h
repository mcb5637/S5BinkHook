#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/ui/s5_ui.h>
#include <shok/s5_scriptsystem.h>
#include <shok/globals/s5_idmanager.h>
#include <shok/ui/s5_video.h>
#include <shok/engine/s5_RWEngine.h>
#include <span>
#include <optional>

namespace EGUIX {
	struct Rect { // size 4
		float X = 0.0f, Y = 0.0f, W = 1.0f, H = 1.0f;
	};

	struct Color { // size 4
		int Red = 0xFF, Green = 0xFF, Blue = 0xFF, Alpha = 0xFF; // >=0 && <=0xFF

		shok::Color ToShokColor() const;
		Color() = default;
		explicit Color(shok::Color c);
		Color(int r, int g, int b, int a);
	};

	class TextureManager { // no vtable, only gui textures in here
	public:
		BB::CIDManager* IdManager;
		shok::Vector<RWE::RwTexture*> Textures; // loads texture lazily, vector seems to be bigger than number of ids

		shok::GUITextureId GetTextureID(const char* name);
		shok::GUITextureId GetTextureIDNoAdd(const char* name);
		RWE::RwTexture* GetTextureByID(shok::GUITextureId id);
		RWE::RwTexture* ReloadTexture(shok::GUITextureId id);

		// last id first, remember to clear idmanager too
		void FreeTexture(shok::GUITextureId id);

		static inline TextureManager* (__stdcall* const GlobalObj)() = reinterpret_cast<TextureManager * (__stdcall*)()>(0x556644);
	};


	class CFontIDHandler : public BB::IObject { // size 2
	public:
		shok::FontId FontID = shok::FontId::Invalid;

		static inline constexpr int vtp = 0x780B0C;
		static inline constexpr int TypeDesc = 0x833B40;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x34D5D406);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

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
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1B070026);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
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
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x53185D06);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	};
	static_assert(sizeof(EGUIX::CWidgetStringHelper) == 23 * 4);

	class CMaterial : public BB::IObject {
	public:
		shok::GUITextureId Texture = {}; // 1
		EGUIX::Rect TextureCoordinates; // la 5
		EGUIX::Color Color; // la 10

		static inline constexpr int vtp = 0x780890;
		static inline constexpr int TypeDesc = 0x832A90;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF1CAFF46);
		// ctor that sets data 55A978

		CMaterial();
		void SetTexture(const char* name);
		shok::Position GetSize();
		RWE::RwTexture* GetTexture();

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	};
	static_assert(sizeof(EGUIX::CMaterial) == 10 * 4);

	class CLuaFunctionHelper : public::BB::IObject { // size 20
	public:
		shok::String LuaCommand;
		EScr::CLuaFuncRefCommand FuncRefCommand;

		static inline constexpr int vtp = 0x780994;
		static inline constexpr int TypeDesc = 0x8333B8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xCBC5B326);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		void Call(shok::WidgetId widgetID);
	};
	static_assert(sizeof(EGUIX::CLuaFunctionHelper) == 20 * 4);

	class CButtonHelper : public BB::IObject { // size 38
	public:
		int CurrentState; // probably current model disabled 3, highlighted 4, klicked 2, mouseover 1, normal 0
		bool DisabledFlag, HighLightedFlag;
		PADDING(2);
		EGUIX::CLuaFunctionHelper ActionFunction;
		EGUIX::CSingleStringHandler ShortCutString;

		void PressButton(shok::WidgetId widgetID);

		static void HookShortcutSignExtend();
		static void HookShortcutComparison();

		static inline constexpr int vtp = 0x780818;
		static inline constexpr int TypeDesc = 0x832918;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x813DE3B6);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
	};
	static_assert(sizeof(EGUIX::CButtonHelper) == 38 * 4);

	class IWidgetRegistrationCallback {
	public:
		virtual void OnRegistrationChanged(shok::WidgetId id, int mode) = 0; // mode ==2 sem to be unregister
	};

	class CToolTipHelper : public BB::IObject, public IWidgetRegistrationCallback { // size 41
	public:
		bool ToolTipEnabledFlag;
		PADDING(3);
		EGUIX::CSingleStringHandler ToolTipString;
		shok::WidgetId TargetWidget;
		bool ControlTargetWidgetDisplayState;
		PADDING(3);
		EGUIX::CLuaFunctionHelper UpdateFunction;
		bool IsToolTipShown;
		PADDING(3);

		static inline constexpr int vtp = 0x780B30;
		static inline constexpr int TypeDesc = 0x833CB8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x102E66C6);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void OnRegistrationChanged(shok::WidgetId id, int mode) override;
	};
	static_assert(sizeof(EGUIX::CToolTipHelper) == 41 * 4);

	class IOnEvent {
	public:
		virtual ~IOnEvent() = default;
		// evLocalCoords uses coordinates of mother container, evUnmodified uses global coordinates
		virtual int HandleEvent(BB::CEvent* evLocalCoords, BB::CEvent* evUnmodified) = 0; // return id of wid that handeled event or 0?

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xF284ED96);
	};
	class IRender {
	public:
		virtual ~IRender() = default;
		virtual bool Render(const Rect* screenCoords) = 0;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x876EC256);
	};
	class IMaterialAccess {
	public:
		virtual ~IMaterialAccess() = default;
		virtual bool SetTexture(int state, const char* name) = 0;
		virtual bool SetColor(int state, int r, int g, int b, int a) = 0;
		virtual CMaterial* GetMaterial(int state) = 0;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x55F6D0B6);
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

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7B4C9DB6);
	};

	class CBaseWidget : public BB::IObject, public IOnEvent { // size 14
	public:
		int UserVariable[2];
		shok::WidgetId WidgetID; // 4
		EGUIX::Rect PosAndSize;
		bool IsShown; //9
		PADDING(3);
		float ZPriority; // 10 CContainerWidget sorts widgets based on this, lower value gets bumped down to lower list pos
		shok::WidgetId MotherWidgetID;
		shok::WidgetGroupId Group; // 12
		bool ForceToHandleMouseEventsFlag, ForceToNeverBeFoundFlag;
		PADDING(2); // 13

		static inline constexpr int vtp = 0x780768;
		static inline constexpr int TypeDesc = 0x82D93C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x18736A06);
		// 5598E3 ctor

		virtual CBaseWidget* Clone() = 0; // 3
		virtual void Initialize() = 0;
	private:
		virtual void unknown0() = 0; // does something on wid geristr cb on mother wid? unused? 5
	public:
		virtual bool SetName(const char* name) = 0; // no idea what return is
		virtual int SetPosAndSize(float x, float y, float w, float h) = 0;
		virtual bool SetVisibility(bool vis) = 0; // returns changed
		virtual int GetMouseOverWidget(float* mousePosrelativeToWidget, bool checkSubwidgets, bool containerWidget) = 0; // 10
		virtual bool IsWidgetID(shok::WidgetId id) = 0; // not sure what this is exactly about
	private:
		virtual bool retzero() = 0; // button & containerwid true, customwid forward
	public:

		bool* GetUpdateManualFlag();
		EGUIX::CLuaFunctionHelper* GetUpdateFunc();
		EGUIX::CMaterial* GetMaterial(int m);
		EGUIX::CButtonHelper* GetButtonHelper();
		EGUIX::CToolTipHelper* GetTooltipHelper();
		EGUIX::CWidgetStringHelper* GetStringHelper();

		EGUIX::Rect CalcGlobalPosAndSize() const;
	};
	static_assert(sizeof(CBaseWidget) == 14 * 4);
	//constexpr int i = offsetof(CBaseWidget, PosAndSize.H) / 4;

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
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x213A8776);

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
		float LineDistanceFactor; // 73

		static inline constexpr int vtp = 0x780EE4;
		static inline constexpr int TypeDesc = 0x82D95C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x86E3BC06);

		static EGUIX::CStaticTextWidget* Create();
	};
	static_assert(offsetof(CStaticTextWidget, StringHelper) == 27 * 4);
	//constexpr int i = offsetof(CStaticTextWidget, StringHelper.Color) / 4;

	class CButtonWidget : public EGUIX::CBaseWidget, public IRender, public IMaterialAccess { // irender 14
	public:
		EGUIX::CButtonHelper ButtonHelper; // 16 la 54
		shok::Array<EGUIX::CMaterial, 5> Materials;
		EGUIX::CToolTipHelper ToolTipHelper; // 105 la 146

		static inline constexpr int vtp = 0x780E5C;
		static inline constexpr int TypeDesc = 0x834148;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFE028496);
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
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x56DDA656);

		static EGUIX::CGfxButtonWidget* Create();
	};

	class CTextButtonWidget : public EGUIX::CButtonWidget, public ITextAccess { // textacc 147
	public:
		EGUIX::CWidgetStringHelper StringHelper; // la 170
		EGUIX::CLuaFunctionHelper UpdateFunction;
		bool UpdateManualFlag, CppLogic_CenterText;
		PADDING(2);

		static inline constexpr int vtp = 0x780DB0;
		static inline constexpr int TypeDesc = 0x82DF3C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5DD085A6);

		static EGUIX::CTextButtonWidget* Create();
		static void HookFixTextRender();
	private:
		static void __thiscall CenterRenderArea(const CTextButtonWidget* th, Rect* render);
		static void __attribute((naked)) CenterRenderAreaASM();
	};
	static_assert(offsetof(CTextButtonWidget, StringHelper.StringFrameDistance) == (14+151) * 4);
	static_assert(offsetof(CTextButtonWidget, UpdateManualFlag) == 190 * 4);
	static_assert(sizeof(CTextButtonWidget) == 191 * 4);

	class CContainerWidget : public EGUIX::CBaseWidget, public IRender, public IWidgetRegistrationCallback { // irender 15
	public:
		EGUIX::CWidgetListHandler WidgetListHandler;

		static inline constexpr int vtp = 0x78114C;
		static inline constexpr int TypeDesc = 0x8321D8;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x55F9D1F6);

	protected:
		virtual void AddChild(CBaseWidget* add) = 0;
		// remove child?

	public:
		void AddWidget(EGUIX::CBaseWidget* toAdd, const char* name, const EGUIX::CBaseWidget* before);
		EGUIX::CBaseWidget* CloneAsChild(EGUIX::CBaseWidget* toClone, std::function<std::string(const char* n, EGUIX::CBaseWidget* oldWid)> nameGen, EGUIX::CBaseWidget* before = nullptr);
		static EGUIX::CContainerWidget* Create();
	};
	static_assert(sizeof(CContainerWidget) == 20 * 4);

	class CProjectWidget : public CContainerWidget {
	public:
		shok::WidgetId CurrentRootWidget;

		static inline constexpr int vtp = 0x780910;
		static inline constexpr int TypeDesc = 0x832DFC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x5CA15E96);

		// render all 0x55B056 __thiscall()
	};
	static_assert(sizeof(CProjectWidget) == 21 * 4);

	class CPureTooltipWidget : public EGUIX::CBaseWidget {
	public:
		EGUIX::CToolTipHelper ToolTipHelper;

		static inline constexpr int vtp = 0x780BB0;
		static inline constexpr int TypeDesc = 0x833E30;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x82CC8876);

		static EGUIX::CPureTooltipWidget* Create();
	};

	class CProgressBarWidget : public EGUIX::CStaticWidget {
	public:
		EGUIX::CLuaFunctionHelper UpdateFunction;
		bool UpdateManualFlag;
		float ProgressBarValue, ProgressBarLimit;

		static inline constexpr int vtp = 0x780C20;
		static inline constexpr int TypeDesc = 0x833FCC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x72633416);

		static EGUIX::CProgressBarWidget* Create();
	};

	class CCustomWidget;
	class ICustomWidget { // size 36
	public:
		virtual ~ICustomWidget() = default;
		virtual void Initialize() = 0;
		virtual void Destroy() = 0;
		virtual void Render(CCustomWidget* widget, const Rect* screenCoords) = 0;
		// evLocalCoords uses coordinates of mother container, evUnmodified uses global coordinates
		virtual bool HandleEvent(CCustomWidget* widget, BB::CEvent* evLocalCoords, BB::CEvent* evUnmodified) = 0;
	private:
		virtual int uk3(); // 5
	public:

		int IntegerUserVariable0 = 0, IntegerUserVariable1 = 0, IntegerUserVariable2 = 0, IntegerUserVariable3 = 0,
			IntegerUserVariable4 = 0, IntegerUserVariable5 = 0; // ctor memset 0
		shok::String StringUserVariable[4]; // 7
		shok::WidgetId WidgetId = {}; // 35

		void RenderLine(const Rect* screenCoords, const EGUIX::Color* c, float x1, float y1, float x2, float y2); // coordinates relative to widget

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x156D9BF6);
	};
	static_assert(sizeof(ICustomWidget)==36*4);
	static_assert(offsetof(ICustomWidget, WidgetId) == 35 * 4);

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
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7656DB56);
	};
	static_assert(sizeof(EGUIX::CCustomWidget) == 43 * 4);

	// int user variable0 bool always visible, if false ignores first char after reset and auto hides first mother container widget on enter/esc
	// int user variable1 bool keep content on close
	// int user variable2 mode, 2->cd-key, 1->password/cheat, 0->chat
	// int user variable3 bool no confirm call
	// int user variable4 buffer size
	// string user var 0 confirm lua func in _G (inputString, widgetId)
	class CStringInputCustomWidget : public BB::IObject, public ICustomWidget {
	public:
		bool AlwaysVisible = false; // 37
		shok::String OnConfirm; // initialized with StringUserVariable0
		bool KeepContentOnClose = true, ModePassword = false, ModeCDKeyInput = false, DoNotCallConfirm = false; // 45
		int BufferSize = 0;
		bool IgnoreNextChar = false;
		struct Buffer { // 48
			bool Allocated = 0;
			PADDING(3);
			size_t BuffSize = 0;
			size_t CurrentEditPos = 0;
			char* ActualText = nullptr;
			char* OutputText = nullptr;
			bool PasswordMode = false, CDKeyMode = false; // 53

			// 55CF81(this, keyCode, keyChar) handle key -> enter->3, escape->4, 1->handeled, 2->not handeled
		} Buff;
		shok::FontId FontID = {}; // Data\\Menu\\Fonts\\standard10.met

		const char* GetBuffer(bool passwordMask = false);
		void Reserve(size_t s); // also clears buffer
		void CallOnConfirm(CCustomWidget* wid, const char* content = nullptr);
		void Assign(const char* buff);
		void Close(CCustomWidget* wid);
		bool HandleInput(CCustomWidget* wid, shok::Keys keyCode, char keyChar); // remove modifier on keyCode

		static inline constexpr int vtp = 0x780AFC;
		static inline constexpr int TypeDesc = 0x833B10;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xBF22E2D6);
	};
	static_assert(offsetof(CStringInputCustomWidget, AlwaysVisible) == 4 * 37);
	static_assert(sizeof(CStringInputCustomWidget) == 55 * 4);
	//constexpr int i = offsetof(CStringInputCustomWidget, Buff.PasswordMode) / 4;
}

namespace GGUI {
	class OnScreenInfoRenderer;
	class COnScreenElement {
	public:
		OnScreenInfoRenderer* OSIR;
		PADDINGI(64);

		virtual ~COnScreenElement() = default;

		static inline constexpr int vtp = 0x77E518;
	};
	class COnScreenElementMotivation : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E590;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // moti bubble
	};
	class COnScreenElementBuilding : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E530;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // alarm, unknown playercolor+worker?, moti locked, overtime, progress perc
	};
	class COnScreenElementTask : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E598;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // work cycle icon
	};
	class COnScreenElementExperience : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E580;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // starts, leader type icon (does nothing, if not one of the hardcoded ecats)
	};
	class COnScreenElementHealthbar : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E558;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // health bar, limited lifespan bar
	};
	class COnScreenElementWorkers : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E578;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // worker icons
	};
	class COnScreenElementConstructionSite : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E528;
	};
	class COnScreenElementResources : public COnScreenElement {
	public:
		bool u;
		static inline constexpr int vtp = 0x77E588;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // res icon and amount for mines and similar
	};
	class COnScreenElementSleepers : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E568;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // sleeper icons
	};
	class COnScreenElementChomping : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E570;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // eater icons
	};
	class COnScreenElementType : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E5A0;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data, bool centered); // type name

		static void HookDisplayName();
	private:
		static const char* __stdcall GetDisplayName(const GGL::IGLGUIInterface::UIData* data);
		static void __attribute((naked)) GetDisplayNameASM();
	};
	class COnScreenElementWorktime : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E5A8;
	};
	class COnScreenElementRefiner : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E520;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // res icon and res count
	};
	class COnScreenElementSoldiers : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E560;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // solder amount boxes
	};
	class COnScreenElementNpc : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E5B0;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // npc marker
	};
	class COnScreenElementHero : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E5B8;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // resurrect bar/icon, camo bar
	};
	class COnScreenElementMerchantBuilding : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E5C0;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // text
	};
	class COnScreenElementBridge : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E538;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // progress bar
	};
	class COnScreenElementThief : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E540;

		void Render(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // sabotage/defuse bar
	};
	class COnScreenElementBombTick : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E550;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // countdown
	};
	class COnScreenElementPlunder : public COnScreenElement {
	public:
		static inline constexpr int vtp = 0x77E548;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data); // thief steal bar
	};

	class OnScreenInfoRenderer {
		friend class C3DOnScreenInformationCustomWidget;
	public:
		struct {
			EGUIX::CMaterial Texture_DeadHero; // 46
			EGUIX::CMaterial Texture_NPCMarker;
			EGUIX::CMaterial Texture_NoWorker; // 66
			EGUIX::CMaterial Texture_Alarm;
			EGUIX::CMaterial Texture_Overtime;
			EGUIX::CMaterial Texture_Locked;
			EGUIX::CMaterial Texture_Healthbar0; // color to 0,0,0,125
			EGUIX::CMaterial Texture_Healthbar0_again; // color to 0,180,0,255
			EGUIX::CMaterial Texture_EmoteHappy;
			EGUIX::CMaterial Texture_EmoteAngry;
			EGUIX::CMaterial Texture_EmoteGood;
			EGUIX::CMaterial Texture_EmoteSad;
			EGUIX::CMaterial Texture_EmoteLeave; // 166
			EGUIX::CMaterial Texture_EmoteFlee; // 176
			EGUIX::CMaterial Texture_StatusEat; // 186
			EGUIX::CMaterial Texture_StatusRest;
			EGUIX::CMaterial Texture_StatusLeave;
			EGUIX::CMaterial Texture_StatusIdle;
			EGUIX::CMaterial Texture_StatusWork;
			EGUIX::CMaterial Texture_StatusFlee;
			EGUIX::CMaterial Texture_EmblemSword; // 246
			EGUIX::CMaterial Texture_EmblemArcher;
			EGUIX::CMaterial Texture_EmblemSpear;
			EGUIX::CMaterial Texture_EmblemCavalry;
			EGUIX::CMaterial Texture_EmblemCannon;
			EGUIX::CMaterial Texture_EmblemThief;
			EGUIX::CMaterial Texture_EmblemRifle;
			EGUIX::CMaterial Texture_EmblemScout;
			EGUIX::CMaterial Texture_RefineGold;
			EGUIX::CMaterial Texture_RefineIron;
			EGUIX::CMaterial Texture_RefineMud;
			EGUIX::CMaterial Texture_RefineStone;
			EGUIX::CMaterial Texture_RefineSulfur;
			EGUIX::CMaterial Texture_RefineWood;
			EGUIX::CMaterial Texture_ResGold;
			EGUIX::CMaterial Texture_ResIron;
			EGUIX::CMaterial Texture_ResMud;
			EGUIX::CMaterial Texture_ResStone;
			EGUIX::CMaterial Texture_ResSulfur;
			EGUIX::CMaterial Texture_ResWood;
			EGUIX::CMaterial Texture_OnscreenWorker;
			EGUIX::CMaterial Texture_OnscreenSleeper;
			EGUIX::CMaterial Texture_OnscreenChomping;
			EGUIX::CMaterial Texture_OnscreenHearts;
			EGUIX::CMaterial Texture_OnscreenStars1;
			EGUIX::CMaterial Texture_OnscreenStars2;
			EGUIX::CMaterial Texture_OnscreenStars3;
			EGUIX::CMaterial Texture_OnscreenStars4;
			EGUIX::CMaterial Texture_OnscreenStars5;
			EGUIX::CMaterial Texture_MOEmpty;
			shok::FontId FontID_OnscreenNumbersSmall; // 500 in struct
			shok::FontId FontID_Onscreen10;
			PADDINGI(3); // map? of entitytype -> texture gui/MO_NAME
		} TextureData;
		COnScreenElementMotivation OSEMotivation; // 551
		COnScreenElementBuilding OSEBuilding; // 617
		COnScreenElementTask OSETask; // 683
		COnScreenElementExperience OSEExperience; // 749
		COnScreenElementHealthbar OSEHealthbar; // 815
		COnScreenElementWorkers OSEWorkers; // 881
		COnScreenElementConstructionSite OSEConstructionSite;
		COnScreenElementResources OSEResources; // 1013
		COnScreenElementSleepers OSESleepers; // 1080
		COnScreenElementChomping OSEChomping; // 1146
		COnScreenElementType OSEType; // 1212
		COnScreenElementWorktime OSEWorktime;
		COnScreenElementRefiner OSERefiner; // 1344
		COnScreenElementSoldiers OSESoldiers; // 1410
		COnScreenElementNpc OSENpc; // 1476
		COnScreenElementHero OSEHero; // 1542
		COnScreenElementMerchantBuilding OSEMerchantBuilding; // 1608
		COnScreenElementBridge OSEBridge; // 1674
		COnScreenElementThief OSEThief; // 1740
		COnScreenElementBombTick OSEBombTick; // 1806
		COnScreenElementPlunder OSEPlunder; // 1872
		struct {
			struct Floatie {
				float StartTime;
				OnScreenInfoRenderer* OSIR;
				shok::EntityId EntityId;
				int Amount;
			};

			shok::Vector<Floatie> Floaties;
			OnScreenInfoRenderer* OSIR;
		} FloatieManager;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data);
		void RenderActive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data);
		void RenderFloaties();
		EGUIX::CMaterial* GetResourceIcon(shok::ResourceType rt); // does not return wood icon without patching
		EGUIX::CMaterial* GetRefinerResourceIcon(shok::ResourceType rt);

	private:
		EGUIX::CMaterial* __thiscall GetResourceIconOverride(shok::ResourceType rt);
		void __thiscall RenderFloatiesHooked();
	};

	class C3DOnScreenInformationCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		bool ShowAllInformationFlag, ShowEffects; // 37
		EGUIX::Rect r1, r2; // not sure, but its 2 4 float objects, second 42 probably something with mouseover entity
		OnScreenInfoRenderer Renderer;


		static inline constexpr int vtp = 0x77D490;
		static inline constexpr int TypeDesc = 0x82E1E8;

		void ShowResourceFloatieOnEntity(shok::EntityId eid, int amount);

		static void HookResourceFloatieShowWood(bool showwood);
		static void HookResourceElementWood(bool showwood);
		static void HookOnScreenInfoDisplayBehavior();
		static void HookAdditionalFloaties();

		static inline GGUI::C3DOnScreenInformationCustomWidget** const GlobalObj = reinterpret_cast<GGUI::C3DOnScreenInformationCustomWidget**>(0x882F54);

	private:
		static void __stdcall RenderHooked(C3DOnScreenInformationCustomWidget* th, shok::Position* screenPos, GGL::IGLGUIInterface::UIData* data, bool* active);
		static void __attribute((naked)) RenderHookedASM();
	};
	static_assert(offsetof(C3DOnScreenInformationCustomWidget, ShowAllInformationFlag) == 37 * 4);
	static_assert(offsetof(C3DOnScreenInformationCustomWidget, Renderer.TextureData) == 46 * 4);
	static_assert(offsetof(C3DOnScreenInformationCustomWidget, Renderer.TextureData.FontID_OnscreenNumbersSmall) == 46 * 4 + 500 * 4);
	static_assert(offsetof(C3DOnScreenInformationCustomWidget, Renderer.OSEMotivation) == 46 * 4 + 505 * 4);
	static_assert(offsetof(C3DOnScreenInformationCustomWidget, Renderer.OSEBuilding) == 46 * 4 + 571 * 4);
	static_assert(offsetof(C3DOnScreenInformationCustomWidget, Renderer.FloatieManager) == 46 * 4 + 1892 * 4);
	static_assert(sizeof(C3DOnScreenInformationCustomWidget) == 1943 * 4);

	class AdvancedFloatieManager {
	public:
		struct Floatie {
			shok::Position Pos;
			float Height;
			float StartTime;
			std::string Text;
		};
		std::vector<Floatie> Floaties;

		void RenderFloaties();
		void AddFloatie(const shok::Position& pos, std::string_view txt);
		void AddFloatie(const shok::Position& pos, float height, std::string_view txt);
		void AddFloatie(const shok::Position& pos, std::string&& txt);
		void AddFloatie(const shok::Position& pos, float height, std::string&& txt);

		static AdvancedFloatieManager GlobalObj;
	};

	class CShortMessagesWindowControllerCustomWidget : public BB::IObject, public EGUIX::ICustomWidget { // size 91
	public:
		enum class MessageType : int { // button texture is hardcoded from this
			SettlerAttacked = 1,
			BuildingAttacked = 2,
			WorkerLeft = 3,
			WorkerAngryTaxes = 4,
			WorkerAngryTooMuchWork = 5,
			WorkerAngryNoWork = 6,
			WorkerAngryNoFood = 7,
			WorkerAngryNoRest = 8,
			UpgradePossible = 10,
			AttractionLimitReached = 11,
			FarmLimitReached = 12,
			ResidenceLimitReached = 13,
			QuestChanged = 14,
			TransactionComplete = 15,
		};

		struct Message {
			int MessageId; // 0x82DA54 source
			MessageType Type;
			BB::CEvent* ev; // source?
			float StartTime;
			float Duration; // guessed?
			shok::String Tooltip;
			shok::Position Pos;
		};

		struct Entries {
			shok::List<Message*> Messages;
			shok::Position LastPos;

			void Add(MessageType type, BB::CEvent* ev, float duration, const shok::Position* pos, const char* tooltip);
			void ClearOldMessages(); // removes depending on duration+starttime
			Message* GetMessageWithId(int id);
			void RemoveMessage(int id);
			// limit to 0x531BD5 __thiscall(uint limit) (removes oldest messages first)
		};

		struct Button {
			EGUIX::CButtonWidget* Widget;
			int MessageId; // matches id of displayed StandardMessage
		};


		Entries StandardMessage; // 37
		Entries History; // blocks new messages 42
		Button Buttons[20]; //47
		EGUIX::Rect ShortMessagesListWindowPosAndSize; // 87

		static constexpr int vtp = 0x77C718;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x6E6AE2C6);

		// show tooltip for 0x53192F __thiscall(int idx) (ShortMessagesOutputWindowInfoString and ShortMessagesOutputWindow are hardcoded)
		// button clicked 0x531C8E __thiscall(int idx)
		// init buttons 0x531573 __thiscall (nop on already initialized)
		// update buttons 0x531803 __thiscall
		// feedbackEvent 0x5320DF __thiscall(event*)

		static inline CShortMessagesWindowControllerCustomWidget** const GlobalObj = reinterpret_cast<CShortMessagesWindowControllerCustomWidget**>(0x882C34);
	};
	static_assert(sizeof(CShortMessagesWindowControllerCustomWidget::Message) == 14 * 4);
	static_assert(sizeof(CShortMessagesWindowControllerCustomWidget) == 91 * 4);
	constexpr int i = offsetof(CShortMessagesWindowControllerCustomWidget, Buttons) / 4;

	// IntegerUserVariable0 bool is mainmenu
	class CStatisticsRendererCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		enum class StatType : int {
			Houses = 0,
			Military = 1,
			Resources = 2,
			Settlers = 3,
			Technologies = 4,
			Scores = 5,
			SettlersSerfs = 10,
			SettlersWorkers = 11,
			SettlersMilitary = 12,
			SettlersMotivation = 13,
			MilitaryUnitsBuildingsKilled = 20,
			MilitaryUnitsKilled = 21,
			MilitaryUnitsBuildingsLost = 22,
			MilitaryUnitsLost = 23,
			HousesAll = 30,
			HousesFarms = 31,
			HousesResidences = 32,
			HousesWorkers = 33,
			ResourcesMoney = 40,
			ResourcesClay = 41,
			ResourcesIron = 22,
			ResourcesStone = 43,
			ResourcesSulphur = 44,
			ResourcesWood = 45,
		};
		enum class TimeScale : int {
			Min60 = 1,
			Min120 = 2,
			Min240 = 3,
			All = 4,
			Min30 = 5,
		};

		struct StatToRender {
			StatType Selected;
			TimeScale Scale;
			shok::PlayerId PlayerId;

			void ToggleTimeScale();
			int GetMinutes(shok::PlayerId pl, CStatisticsRendererCustomWidget* cw);

			static inline StatToRender** const GlobalObj = reinterpret_cast<StatToRender**>(0x882B00);
		};

		bool IsMainMenu; // 37


		static constexpr int vtp = 0x77C3D0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9A90FE66);

		shok::PlayerId GetPlayer() const;
		bool IsHumanPlayer(shok::PlayerId pl) const;
		GGL::CGameStatistics* GetStatistics(shok::PlayerId player);
	};
	static_assert(offsetof(CStatisticsRendererCustomWidget, IsMainMenu) == 37 * 4);
	static_assert(sizeof(CStatisticsRendererCustomWidget) == 38 * 4);

	class CMiniMapSignal {
	public:
		virtual ~CMiniMapSignal() = default;
		virtual bool Update() = 0;

		float StartTime = 0; // ret of 548B64, float 126?
		float X = 0, Y = 0;
		float Scale = 0;
		int R = 0, G = 0, B = 0, A = 0;
		float Rotation = 0; // zero
		PADDINGI(1); // int 4 // 10

		static inline constexpr int vtp = 0x77BA00;
		// ctor 53D45F(int, float x, float y)
	};
	class CMiniMapSignalPulse : public CMiniMapSignal {
	public:
		float Time = 2.5;
		float Scale_Multiplier = 0.125; // caclulates Scale from this in update
		int Mode = 1; // 1 pulsing, 2 static
		float Time2 = 2.0;

	public:
		static inline constexpr int vtp = 0x77BA3C;
		CMiniMapSignalPulse(float x, float y, bool pulsing, int r, int g, int b, float timeFactor, float scaleFactor);

		virtual bool Update() override;
	};
	class CMiniMapSignalDefault : public CMiniMapSignal {
	public:
		float Time = 0;
		static inline constexpr int vtp = 0x77BA0C;
		CMiniMapSignalDefault(float x, float y, int r, int g, int b, float scaleFactor);

		virtual bool Update() override;
	};
	class CMiniMapSignalBattleShort : public CMiniMapSignal {
	public:

		static inline constexpr int vtp = 0x77BA30;
		CMiniMapSignalBattleShort(float x, float y);

		virtual bool Update() override;
	};
	class CMiniMapSignalBattleDetail : public CMiniMapSignal {
	public:
		float Time = 0;

		static inline constexpr int vtp = 0x77BA24;
		CMiniMapSignalBattleDetail(float x, float y, int colorCode);

		virtual bool Update() override;
	};
	class CMiniMapSignalBattleRough : public CMiniMapSignal {
	public:
		float Time = 0;

		static inline constexpr int vtp = 0x77BA18;
		CMiniMapSignalBattleRough(float x, float y, float t);

		virtual bool Update() override;
	};

	class MiniMapHandler { // size 251
	public:
		enum class Mode : int {
			Normal = 0,
			Tactical = 1,
			Resource = 2,
		};

		struct MiniMapRenderer {
			EGUIX::Rect SomePos;
			int NumVerticies;

			// ctor 53D0C5
			// there are 100 rects that can be rendered at once
			// set color 53CDD0(EGUIX::Color*, rectind), 53CE0B(shok::Color, rectind)
			// set render rect 53CFE1(EGUIX::Rect*, rectind)
			// set render rect vert pos 53CE5D(vertind, f x, f y, rectind)
			// render current raster 53D143()
			// reset verticies to render 53CDCB()
			
			static inline std::span<RWE::RwIm2DVertex, 400> VerticiesToRender{ reinterpret_cast<RWE::RwIm2DVertex*>(0x883C40), 400 };
			static inline std::span<uint16_t, 100> VertexIndices{ reinterpret_cast<uint16_t*>(0x883790), 100 };
		};
		struct MiniMapTexture {
			RWE::RwRaster* Raster;
			RWE::RwTexture* Texture;
			BBRw::CDynTexRef* Dyn;
			PADDINGI(1);

			// ctor texture 53D7BC (shok::string*)
			// ctor CDynTexRef 53D75A (x, y)
			// ctor empty 53D6E3()
			// dtor 53D79F
			void SetToRender();
		};


		// no vtable
		PADDING(1);
		bool RenderFoW;
		bool BattleMarkersEnabled;
		Mode CurrentMode; // 1
		EGUIX::Rect MiniMapRenderPos;
		struct TerrainHandler {
			bool TerrainRasterDone;
			Mode CurrentMode;
			EGUIX::Rect* RenderPos;
			MiniMapRenderer Render;
			MiniMapTexture NormalTexture, TacticalResourceModeTexture;

			// setcurrentmode 5269CB
			// render 543C54()
			// build terrain raster 5434D8()
		} Terrain; // 6
		struct EntitiesHandler {
			EGUIX::Rect* RenderPos;
			MiniMapRenderer Render;

			MiniMapTexture minimap_sphere, minimap_quad;
			int IsResourceMode;
			bool RenderIgnoreFoW;

			// set render entity 543D19(f x, f y, f sx, f sy, shok::Color c, rectind)
			// render entities of accesscat 543E22(f scale, AccessCategoryFlags f)
			// render heroes 54414F(f scale)
			// render selected entities 54402C(f scale)
			// render VCs 544467(f scale)
			// render HQs 5442DB(f scale)
			// render debug stuff 5446AF()
			// render 544951()
		} Entities; // 22
		struct FoWTerrainHiderHandler {
			EGUIX::Rect* RenderPos;
			MiniMapRenderer Render;

			MiniMapTexture ExplorationOverlay;

			float LastUpdateTime;
			PADDINGI(1);

			// build fow raster 53E54A(bool =false)
			// render 53E77B()
		} FoWTerrainHider; // 38 just renders black over existing terrain
		struct CameraHandler {
			EGUIX::Rect* RenderPos;
			struct {
				MiniMapRenderer Render;
				std::array<RWE::RwIm2DVertex, 4> VertToRender;
				MiniMapTexture Empty;

				// store vert 54521F()
				// render stored 545180()
			} CamRenderer;

			// render 544A8E(bool prepare)
		} Camera; // 50
		struct MarkerHandler {
			MiniMapRenderer Render;
			MiniMapTexture minimap_signal_0, minimap_signal_1; // 5 (dashed circle), 9 (rotating squares)

			shok::Vector<CMiniMapSignalDefault> Defaults; // 13 renders minimap_signal_1
			shok::Vector<CMiniMapSignalBattleRough> BattleRoughs; // 17 renders minimap_signal_0
			shok::Vector<CMiniMapSignalBattleDetail> BattleDetails; // 21 renders minimap_signal_1
			shok::Vector<CMiniMapSignalBattleShort> ShortBattles; // 25 renders minimap_signal_0
			shok::Vector<CMiniMapSignalPulse> Pulses; // 29 renders minimap_signal_0
			EGUIX::Rect* RenderPos; // 33

			// render signal with active texture 5294C4(CMiniMapSignal*)
			// render 52A015()
			// add battle markers 52B237(x, y, int allied) (pos modified see CreateMarker)
		} Markers; // 88
		struct DebugTextHandler {
			bool Active;
			char Text[128 * 4];

			// render 543316()
		} DebugText; // 112

		void CreateMarker(const shok::Position& p, bool pulsing, int r, int g, int b, float timeFactor, float scaleFactor);
		void CreateSignalDefault(const shok::Position& p, int r, int g, int b, float scaleFactor);

		static inline MiniMapHandler* (__cdecl* const GlobalObj)() = reinterpret_cast<MiniMapHandler * (__cdecl*)()>(0x52FE1C); // ret 0x882AB8
		// ctor 0x52FDBB
		// dtor 52FDCD
		// set render rect 0x53D9E0 __thiscall(EGUIX::Rect*)
		// set render fow 53D8BC(bool)
		// set minimap mode 53D8E6(Mode)
		// render minimap 53DD07()
		// render overlay 53E496()
		// on battle event 53DA5D(x, y, bool allied) same 52FE22 
	};
	static_assert(offsetof(MiniMapHandler, MiniMapRenderPos) == 2 * 4);
	static_assert(offsetof(MiniMapHandler, Terrain) == 6 * 4);
	static_assert(offsetof(MiniMapHandler, Terrain.Render) == (6 + 3) * 4);
	static_assert(offsetof(MiniMapHandler, Terrain.NormalTexture) == (6 + 8) * 4);
	static_assert(offsetof(MiniMapHandler, Entities) == 22 * 4);
	static_assert(offsetof(MiniMapHandler, Entities.minimap_quad) == (22 + 10) * 4);
	static_assert(offsetof(MiniMapHandler, Entities.RenderIgnoreFoW) == (22 + 15) * 4);
	static_assert(offsetof(MiniMapHandler, FoWTerrainHider) == 38 * 4);
	static_assert(offsetof(MiniMapHandler, FoWTerrainHider.LastUpdateTime) == (38 + 10) * 4);
	static_assert(offsetof(MiniMapHandler, Camera) == 50 * 4);
	static_assert(offsetof(MiniMapHandler, Camera.CamRenderer.Empty) == (50+34) * 4);
	static_assert(offsetof(MiniMapHandler, Markers) == 88 * 4);
	static_assert(offsetof(MiniMapHandler, Markers.minimap_signal_1) == (88 + 9) * 4);
	static_assert(offsetof(MiniMapHandler, Markers.Pulses) == 117 * 4);
	static_assert(offsetof(MiniMapHandler, DebugText) == 122 * 4);
	static_assert(sizeof(MiniMapHandler) == 251 * 4);
	//constexpr int i = offsetof(MiniMapHandler, Uk1.Uk) / 4;

	// renders the minimap
	class CMiniMapCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77BF54;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x98C68876);
	};

	// mouse up: nop
	// mouse down:
	//	mouse l -> if GGUI::CPositionCommandState, execute at clicked pos, else jump to clicked pos
	//	mouse r -> if shift, something MP, elseif GGUI::CSelectionState, execute at clicked pos, else nop 
	// also renders signals (and something more if in minimap debug mode...)
	class CMiniMapOverlayCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77CDCC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x6157C1AE);

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Render(EGUIX::CCustomWidget* widget, const EGUIX::Rect* screenCoords) override;
		virtual bool HandleEvent(EGUIX::CCustomWidget* widget, BB::CEvent* evLocalCoords, BB::CEvent* evUnmodified) override;

		// buggy? calculates global pos and compares to local event?
		// seems to work fine, at least with the current setup
		std::optional<shok::Position> MapPosFromMouseEvent(const EGUIX::CBaseWidget* wid, const BB::CEvent*) const;
	};
	static_assert(sizeof(CMiniMapOverlayCustomWidget) == 0x94);

	// renders some network info on screen
	class CNetworkInfoCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77CB14;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x6D7F7F36);
	};

	// on autoupdate, updates NetworkWindowPlayerX and NetworkWindowPlayerXKickButton widgets (by hardcoded names)
	class CNetworkWindowControllerCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77CC90;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x9915CE26);
	};

	// renders some debug info
	// what exactly?
	class C3DOnScreenDebugCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77CEA4;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x20BFE706);
	};

	// sets some text to (hardcoded) NotesWindowOutput
	// no way to set the text displayed???
	class CNotesWindowControllerCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77CF14;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x30118F6);
	};

	// updates (hardcoded) DiplomacyWindowPlayerX widgets
	// player colors, names, diplomacy and mp trade
	// also sets action/toottip funcs
	class CDiplomacyWindowControllerCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77D1EC;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x947925C6);
	};

	// updates (hardcoded) QuestWindowQuestX widgets
	// quest title, types
	// seems to store selected quest in own widget int user var 0
	class CQuestWindowControllerCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77D250;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xEDF05506);
	};

	// renders text from (hardcoded) QuestWindowController
	// also does something with QuestWindowQuestOutputBG?
	class CQuestWindowQuestOutputCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77D27C;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7E0B4136);
	};

	// updates TradeWindowOffer (hardcoded) widgets
	// text, player and action func
	// also sets TradeWindowInfoWidget text on show
	class CTradeWindowControllerCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77D398;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x69246616);
	};

	// forwards events to guistate
	// renders move hold direction line
	class C3DViewCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {

	public:
		static inline constexpr int vtp = 0x77D4A0;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x7FA7DB06);
	};
}


namespace EGUIX {
	//string user var0: lua callback (in _G) (value, widgetId)
	//string user var1: slider widget name (not actually set to visible, just the material stolen from)
	//ScrollVertical is set, if height > width
	class CScrollBarButtonCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		bool RMBDown = false, ScrollVertical = true; //37
		int SliderValue; // 38
		shok::AARect CurrentScreenPos; // 39
		shok::String Callback, SliderName; // 43, 50
		shok::WidgetId SliderWidgetId; // 57


		static constexpr int vtp = 0x780A60;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0xFF947B86);

		void UpdateSlider(int value, bool callback);
		// set value to mouse pos 0x55CB97 __thiscall(EGUIX::CCustomWidget* wid)
	};

	// the Collection has its own video size, this needs to match the files size
	// if set correctly, scales the video
	// gets initialized to widget size on first video start
	class CVideoPlaybackCustomWidget : public BB::IObject, public ICustomWidget {
	public:
		EVid::CVideo* Video;
		EVid::CVideoCollectionRW* Collection;

		// stop video 55C4A6 thiscall
		// open video 55C735 thiscall(const char*)
		// start video 55C47B thiscall(const char*, bool)

		bool Init(CBaseWidget* wid = nullptr);

		static constexpr int vtp = 0x780A18;
		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x687793A6);
	};
	static_assert(sizeof(CVideoPlaybackCustomWidget) == 39 * 4);

	class WidgetManager { // this thing has no vtable...
	public:
		BB::CIDManagerEx* WidgetNameManager;
		shok::Vector<EGUIX::CBaseWidget*> Widgets;

		shok::WidgetId GetIdByName(const char* name);
		EGUIX::CBaseWidget* GetWidgetByID(shok::WidgetId id);
		void RemoveWidget(EGUIX::CBaseWidget* w); // does not delete

		static inline EGUIX::WidgetManager* (* const GlobalObj)() = reinterpret_cast<EGUIX::WidgetManager * (*)()>(0x558473);

	private:
		friend class EGUIX::CContainerWidget;
		void AddWidget(EGUIX::CBaseWidget* w, shok::WidgetId id); // widget still needs id and mother
		shok::WidgetId RegisterName(const char* name);
	};

	class CWidgetGroupManager : public IWidgetRegistrationCallback {
	public:
		BB::CIDManager* Manager;

		shok::WidgetGroupId GetGroupId(const char* s);
		shok::WidgetGroupId CreateGroup(const char* s);

		static inline constexpr int vtp = 0x780978;
		static inline constexpr int TypeDesc = 0x832F70;

		static inline EGUIX::CWidgetGroupManager* (__stdcall* const GlobalObj)() = reinterpret_cast<EGUIX::CWidgetGroupManager * (__stdcall*)()>(0x55B688);
	};

	class FontManager { // no vtable either
	public:
		BB::CIDManager* Manager;
		shok::Vector<RWE::P2D::Rt2dFont*> Fonts;

		static void LoadFont(shok::FontId* outFontID, const char* fontName);
		RWE::P2D::Rt2dFont* GetFontObj(shok::FontId id);

		// removes from vector
		void ClearFont(shok::FontId id);
		// removes, then pops id
		void PopFont(shok::FontId id);

		static inline EGUIX::FontManager* (* const GlobalObj)() = reinterpret_cast<EGUIX::FontManager * (*)()>(0x5593AD);
	};

	extern bool (*UIInput_Char_Callback)(int c);
	extern bool (*UIInput_Key_Callback)(int c, win_mouseEvents ev);
	extern bool (*UIInput_Mouse_Callback)(win_mouseEvents id, int w, int l);
	extern bool (*UIInput_Mouse_CallbackMainMenu)(win_mouseEvents id, int w, int l);
	void HookUIInput();

	class FeedbackEventHandler { // another no vtable
	public:
		shok::List<BB::TSlot1<BB::CEvent const&>> Handlers;

		// add feedback event handler thiscall 0x525022(GGUI::CManager* obj, void* handler func) (creates BB::TSlotEx1<GGUI::CManager,BB::CEvent const &>)

		void FireEvent(BB::CEvent* ev);

		static inline EGUIX::FeedbackEventHandler* (* const GlobalObj)() = reinterpret_cast<EGUIX::FeedbackEventHandler * (*)()>(0x582EE1);
	};
	static_assert(sizeof(FeedbackEventHandler) == 3 * 4);

	class WidgetLoader {
	public:
		shok::String PathToGame;
		CProjectWidget* RootWid; // 7
		bool Unknown; // seems to be always false?

		void LoadGUI(const char* file);

		static inline WidgetLoader* (* const GlobalObj)() = reinterpret_cast<WidgetLoader * (*)()>(0x5563CC);

		// update tome event 0x556489 __thiscall(float)
		// render all 0x556409 __thiscall()

		static inline void(__cdecl* const KeyStrokeLuaCallback)() = reinterpret_cast<void(__cdecl*)()>(0x55C445);
	};

	class CEventManager : public IWidgetRegistrationCallback {
	public:
		shok::Position CurrentMousePos;
		PADDINGI(4);
		shok::Keys CurrentModifiers; // 7

		static inline CEventManager* (__cdecl* const GlobalObj)() = reinterpret_cast<CEventManager * (__cdecl*)()>(0x558C16);

		bool IsModifierPressed(shok::Keys m);
		// fire event on widget 558C2E __thiscall(CProjectWidget*, BB::CEvent*)
	};
	static_assert(offsetof(CEventManager, CurrentModifiers) == 7 * 4);

	// no vtable
	class WidgetFuncManager {
	public:
		lua_State* ActiveState;
		shok::WidgetId CurrentWidget;

		static inline WidgetFuncManager* (__cdecl* const GlobalObj)() = reinterpret_cast<WidgetFuncManager* (__cdecl*)()>(0x55ADE0);
		// ctor 55AE9D
	};
}

namespace CppLogic {
	class TextureIdManager {
		EGUIX::TextureManager* Manager;

	public:
		inline TextureIdManager(EGUIX::TextureManager* mng) : Manager(mng) {

		}

		// does not add
		inline shok::GUITextureId GetIdByName(const char* name) {
			return Manager->GetTextureIDNoAdd(name);
		}
		inline const char* GetNameByID(shok::GUITextureId id) {
			return Manager->IdManager->GetNameByID(static_cast<int>(id));
		}
		// throws if id invalid
		inline shok::GUITextureId GetIDByNameOrCreate(const char* name) {
			return Manager->GetTextureID(name);
		}
		// remove highest id first, cause that way the vector gets shrunk. ids get reused, use this only for cleanup
		inline void RemoveID(shok::GUITextureId id) {
			Manager->IdManager->RemoveID(static_cast<int>(id));
		}
		inline void DumpManagerToLuaGlobal(lua_State* L, const char* global) {
			Manager->IdManager->DumpManagerToLuaGlobal(L, global);
		}
		inline size_t size() {
			return Manager->IdManager->size();
		}
		inline void clear() {
			Manager->IdManager->clear();
		}
	};
}

template<>
inline auto CppLogic::GetIdManager<shok::GUITextureId>() {
	auto mng = EGUIX::TextureManager::GlobalObj();
	return CppLogic::TextureIdManager{mng};
}
template<>
inline auto CppLogic::GetIdManager<shok::WidgetGroupId>() {
	auto mng = EGUIX::CWidgetGroupManager::GlobalObj();
	return CppLogic::EnumIdManager<shok::WidgetGroupId>{ mng->Manager };
}

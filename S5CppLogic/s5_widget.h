#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_ui.h"
#include "s5_scriptsystem.h"

namespace EGUIX {
	struct Rect { // size 4
		float X = 0.0f, Y = 0.0f, W = 1.0f, H = 1.0f;
	};

	struct Color { // size 4
		int Red = 0xFF, Green = 0xFF, Blue = 0xFF, Alpha = 0xFF; // >=0 && <=0xFF

		shok::Color ToShokColor() const;
	};

	class TextureManager { // no vtable, only gui textures in here
	public:
		BB::CIDManager* IdManager;
		shok::Vector<RWE::RwTexture*> Textures; // loads texture lazily, vector seems to be bigger than number of ids

		int GetTextureID(const char* name);
		int GetTextureIDNoAdd(const char* name);
		RWE::RwTexture* GetTextureByID(int id);
		RWE::RwTexture* ReloadTexture(int id);

		// last id first, remember to clear idmanager too
		void FreeTexture(int id);

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
		int Texture = 0; // type? // 1
		EGUIX::Rect TextureCoordinates; // la 5
		EGUIX::Color Color; // la 10

		static inline constexpr int vtp = 0x780890;
		static inline constexpr int TypeDesc = 0x832A90;
		static inline constexpr unsigned int Identifier = 0xF1CAFF46;
		// ctor that sets data 55A978

		CMaterial();
		void SetTexture(const char* name);
		shok::Position GetSize();

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
	//constexpr int i = offsetof(CStaticTextWidget, StringHelper.Color) / 4;

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

	class CProjectWidget : public CContainerWidget {
	public:
		PADDINGI(1);

		static inline constexpr int vtp = 0x780910;
		static inline constexpr int TypeDesc = 0x832DFC;
		static inline constexpr unsigned int Identifier = 0x5CA15E96;
	};
	static_assert(sizeof(CProjectWidget) == 21 * 4);

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

	class ICustomWidget { // size 35
	public:
		virtual ~ICustomWidget() = default;
	private:
		virtual void uk() = 0;
	public:
		virtual void Destroy() = 0;
		virtual void Render(CBaseWidget widget, float* zero) = 0;
	private:
		virtual void uk2(int, int) = 0;
		virtual int uk3() = 0;

		PADDINGI(6); // ctor memset 0
		shok::String Strings[4];
	};
	static_assert(sizeof(ICustomWidget)==35*4);

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
			int FontID_OnscreenNumbersSmall; // 500 in struct
			int FontID_Onscreen10;
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
				int EntityId, Amount;
			};

			shok::Vector<Floatie> Floaties;
			OnScreenInfoRenderer* OSIR;
		} FloatieManager;

		void RenderInactive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data);
		void RenderActive(const shok::Position* screenPos, const GGL::IGLGUIInterface::UIData* data);
		void RenderFloaties();
		EGUIX::CMaterial* GetResourceIcon(shok::ResourceType rt); // does not return wood icon without patching
		EGUIX::CMaterial* GetRefinerResourceIcon(shok::ResourceType rt);
	};

	class C3DOnScreenInformationCustomWidget : public BB::IObject, public EGUIX::ICustomWidget {
	public:
		PADDINGI(1);
		bool ShowAllInformationFlag, ShowEffects; // 37
		EGUIX::Rect r1, r2; // not sure, but its 2 4 float objects, second 42 probably something with mouseover entity
		OnScreenInfoRenderer Renderer;


		static inline constexpr int vtp = 0x77D490;
		static inline constexpr int TypeDesc = 0x82E1E8;

		void ShowResourceFloatieOnEntity(int eid, int amount);

		static void HookResourceFloatieShowWood(bool showwood);
		static void HookResourceElementWood(bool showwood);
		static void HookOnScreenInfoDisplayBehavior();
		static void HookAdditionalFloaties();

		static inline GGUI::C3DOnScreenInformationCustomWidget** const GlobalObj = reinterpret_cast<GGUI::C3DOnScreenInformationCustomWidget**>(0x882F54);
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
		void AddFloatie(const shok::Position& pos, const char* txt);
		void AddFloatie(const shok::Position& pos, float height, const char* txt);

		static AdvancedFloatieManager GlobalObj;
	};

	class CMiniMapSignal {
	public:
		virtual ~CMiniMapSignal() = default;
		// update

		float StartTime = 0; // ret of 548B64, float 126?
		float X = 0, Y = 0;
		float Scale = 0;
		int R = 0, G = 0, B = 0, A = 0;
		float Rotation = 0; // zero
		PADDINGI(1); // int 4 // 10

		static inline constexpr int vtp = 0x77BA00;
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
	};
	class CMiniMapSignalDefault : public CMiniMapSignal {
	public:
		float Time = 0;
		static inline constexpr int vtp = 0x77BA0C;
		CMiniMapSignalDefault(float x, float y, int r, int g, int b, float scaleFactor);
	};

	class MiniMapMarkerHandler {
	public:
		// no vtable
		PADDINGI(88);
		PADDINGI(13);
		shok::Vector<CMiniMapSignalDefault> Defaults;
		PADDINGI(12);
		shok::Vector<CMiniMapSignalPulse> Pulses;

		void CreateMarker(const shok::Position& p, bool pulsing, int r, int g, int b, float timeFactor, float scaleFactor);
		void CreateSignalDefault(const shok::Position& p, int r, int g, int b, float scaleFactor);

		static inline MiniMapMarkerHandler* (__cdecl* const GlobalObj)() = reinterpret_cast<MiniMapMarkerHandler * (__cdecl*)()>(0x52FE1C);
	};
	static_assert(offsetof(MiniMapMarkerHandler, Pulses) == 117 * 4);
	//constexpr int i = offsetof(MiniMapMarkerHandler, Pulses) / 4;
	class CMiniMapCustomWidget : public EGUIX::CCustomWidget {

	public:
		static inline constexpr int vtp = 0x77BF54;
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

	class FontManager { // no vtable either
	public:
		static void LoadFont(int* outFontID, const char* fontName);
		RWE::P2D::Rt2dFont* GetFontObj(int id);

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

	class WidgetLoader {
	public:
		shok::String PathToGame;
		CProjectWidget* RootWid;
		bool Unknown; // seems to be always false?

		void LoadGUI(const char* file);

		static inline WidgetLoader* (* const GlobalObj)() = reinterpret_cast<WidgetLoader * (*)()>(0x5563CC);
	};
}

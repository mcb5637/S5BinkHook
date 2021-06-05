#pragma once
#include "s5data.h"
#include <list>

struct shok_widget_rect { // size 4
	float X, Y, W, H;
};

struct shok_widget_color { // size 4
	int Red, Green, Blue, Alpha; // >=0 && <=0xFF
};


struct shok_EGUIX_CFontIDHandler : shok_object { // size 2
	int FontID;

	static inline constexpr int vtp = 0x780B0C;

	void LoadFont(const char* name);
};

struct shok_EGUIX_CSingleStringHandler : shok_object { // size 15
	shok_string StringTableKey;
	shok_string RawString;

	static inline constexpr int vtp = 0x7809A4;
};
static_assert(sizeof(shok_EGUIX_CSingleStringHandler) == 15 * 4);

struct shok_EGUIX_CWidgetStringHelper : shok_object { // size 23
	shok_EGUIX_CFontIDHandler FontHandler;
	shok_EGUIX_CSingleStringHandler SingleStringHandler;
	float StringFrameDistance;
	shok_widget_color Color;

	static inline constexpr int vtp = 0x781518;
};
static_assert(sizeof(shok_EGUIX_CWidgetStringHelper) == 23 * 4);

struct shok_EGUIX_CMaterial : shok_object {
	int Texture; // type? // 1
	shok_widget_rect TextureCoordinates; // la 5
	shok_widget_color Color; // la 10

	static inline constexpr int vtp = 0x780890;
};
static_assert(sizeof(shok_EGUIX_CMaterial) == 10 * 4);

struct shoc_EScr_CLuaFuncRefCommand : shok_object {
	lua_State* L;
	int NeedsCompile;
	int Ref;
	shok_string LuaCommand;
	PADDINGI(1);

	static inline constexpr int vtp = 0x786BE0;
};

struct shok_EGUIX_CLuaFunctionHelper : shok_object { // size 20
	shok_string LuaCommand;
	shoc_EScr_CLuaFuncRefCommand FuncRefCommand;

	static inline constexpr int vtp = 0x780994;

	void Call(int widgetID);
};
static_assert(sizeof(shok_EGUIX_CLuaFunctionHelper) == 20 * 4);

struct shok_EGUIX_CButtonHelper : shok_object { // size 38
	PADDINGI(1);
	byte DisabledFlag, HighLightedFlag;
	PADDING(2);
	shok_EGUIX_CLuaFunctionHelper ActionFunction;
	shok_EGUIX_CSingleStringHandler ShortCutString;

	static inline constexpr int vtp = 0x780818;
};
static_assert(sizeof(shok_EGUIX_CButtonHelper) == 38 * 4);

struct shok_EGUIX_CToolTipHelper : shok_object { // size 41
	void* vtable_EGUIX_IWidgetRegistrationCallback;
	byte ToolTipEnabledFlag;
	PADDING(3);
	shok_EGUIX_CSingleStringHandler ToolTipString;
	int TargetWidget;
	byte ControlTargetWidgetDisplayState;
	PADDING(3);
	shok_EGUIX_CLuaFunctionHelper UpdateFunction;
	byte IsToolTipShown;
	PADDING(3);

	static inline constexpr int vtp = 0x780B30;
};
static_assert(sizeof(shok_EGUIX_CToolTipHelper) == 41 * 4);

struct shok_EGUIX_CBaseWidget : shok_object {
	void* vtable_EGUIX_IOnEvent;
	int UserVariable[2];
	int WidgetID;
	shok_widget_rect PosAndSize;
	byte IsShown;
	PADDING(3)
	float ZPriority;
	int MotherWidgetID;
	int Group; // 12
	byte ForceToHandleMouseEventsFlag, ForceToNeverBeFoundFlag;
	PADDING(2); // 13

	static inline constexpr int vtp = 0x780768;

	int SetPosAndSize(float x, float y, float w, float h);
	byte* GetUpdateManualFlag();
	shok_EGUIX_CLuaFunctionHelper* GetUpdateFunc();
	bool IsContainerWidget();
	bool IsStaticTextWidget();
	shok_EGUIX_CMaterial* GetMaterials(int* count);
	shok_EGUIX_CButtonHelper* GetButtonHelper();
	shok_EGUIX_CToolTipHelper* GetTooltipHelper();
	shok_EGUIX_CWidgetStringHelper* GetStringHelper();
};

struct shok_EGUIX_CWidgetListHandler : shok_object {
	vector_padding;
	std::list<shok_EGUIX_CBaseWidget*, shok_allocator<shok_EGUIX_CBaseWidget*>> SubWidgets;

	static inline constexpr int vtp = 0x78098C;
};

struct shok_EGUIX_CStaticWidget : shok_EGUIX_CBaseWidget {
	void* vtable_EGUIX_IRender; // 14
	void* vtable_EGUIX_IMaterialAccess;
	shok_EGUIX_CMaterial BackgroundMaterial; // la 24

	static inline constexpr int vtp = 0x780F84;

	static shok_EGUIX_CStaticWidget* Create();
};

struct shok_EGUIX_CStaticTextWidget : shok_EGUIX_CStaticWidget {
	void* vtable_EGUIX_ITextAccess;
	shok_EGUIX_CWidgetStringHelper StringHelper; // 27
	shok_EGUIX_CLuaFunctionHelper UpdateFunction; // 50
	byte UpdateManualFlag;
	PADDING(3);
	int FirstLineToPrint, NumberOfLinesToPrint;
	float LineDistanceFactor;

	static inline constexpr int vtp = 0x780EE4;

	static shok_EGUIX_CStaticTextWidget* Create();
};

struct shok_EGUIX_CButtonWidget : shok_EGUIX_CBaseWidget {
	void* vtable_EGUIX_IRender; // 14
	void* vtable_EGUIX_IMaterialAccess; // 15
	shok_EGUIX_CButtonHelper ButtonHelper; // la 54
	shok_EGUIX_CMaterial Materials[5];
	int NumMaterials; // ? why after materials
	shok_EGUIX_CToolTipHelper ToolTipHelper; // 105 la 146

	static inline constexpr int vtp = 0x780E5C;
};
static_assert(sizeof(shok_EGUIX_CButtonWidget) == 146 * 4);

struct shok_EGUIX_CGfxButtonWidget : shok_EGUIX_CButtonWidget {
	shok_EGUIX_CMaterial IconMaterial;
	shok_EGUIX_CLuaFunctionHelper UpdateFunction;
	byte UpdateManualFlag;
	PADDING(3);

	static inline constexpr int vtp = 0x780CD0;

	static shok_EGUIX_CGfxButtonWidget* Create();
};

struct shok_EGUIX_CTextButtonWidget : shok_EGUIX_CButtonWidget {
	void* vtable_EGUIX_ITextAccess; // 147
	shok_EGUIX_CWidgetStringHelper StringHelper; // la 170
	shok_EGUIX_CLuaFunctionHelper UpdateFunction;
	byte UpdateManualFlag;
	PADDING(3);

	static inline constexpr int vtp = 0x780DB0;

	static shok_EGUIX_CTextButtonWidget* Create();
};

struct shok_EGUIX_CContainerWidget : shok_EGUIX_CBaseWidget {
	void* vtable_EGUIX_IRender; // 15
	void* vtable_EGUIX_IWidgetRegistrationCallback;
	shok_EGUIX_CWidgetListHandler WidgetListHandler;

	static inline constexpr int vtp = 0x78114C;

	void AddWidget(shok_EGUIX_CBaseWidget* toAdd, const char* name, const shok_EGUIX_CBaseWidget* before);
	static shok_EGUIX_CContainerWidget* Create();
};

struct shok_EGUIX_CPureTooltipWidget : shok_EGUIX_CBaseWidget {
	shok_EGUIX_CToolTipHelper ToolTipHelper;

	static inline constexpr int vtp = 0x780BB0;

	static shok_EGUIX_CPureTooltipWidget* Create();
};

struct shok_EGUIX_CProgressBarWidget : shok_EGUIX_CStaticWidget {
	shok_EGUIX_CLuaFunctionHelper UpdateFunction;
	byte UpdateManualFlag;
	float ProgressBarValue, ProgressBarLimit;

	static inline constexpr int vtp = 0x780C20;

	static shok_EGUIX_CProgressBarWidget* Create();
};

struct shok_widgetManager { // this thing has no vtable...
	int u;
	vector_padding;
	std::vector<shok_EGUIX_CBaseWidget*, shok_allocator<shok_EGUIX_CBaseWidget*>> Widgets;

	int GetIdByName(const char* name);
	shok_EGUIX_CBaseWidget* GetWidgetByID(int id);
};
static inline shok_widgetManager* (*const shok_getWidgetManagerObj)() = (shok_widgetManager * (*)())0x558473;

struct shok_EGUIX_CWidgetGroupManager : shok_object {

	int GetGroupId(const char* s);
	int CreateGroup(const char* s);

	static inline constexpr int vtp = 0x780978;

	static inline shok_EGUIX_CWidgetGroupManager* (__stdcall* const GlobalObj)() = reinterpret_cast<shok_EGUIX_CWidgetGroupManager * (__stdcall*)()>(0x55B688);
};

struct shok_font {
	PADDINGI(1);
	float Size, Offset, Spacing;
};

struct shok_fontManager { // no vtable either

	static void LoadFont(int* outFontID, const char* fontName);
	shok_font* GetFontObj(int id);
};
static inline shok_fontManager* (*const shok_getFontMangerObj)() = (shok_fontManager * (*)())0x5593AD;

extern void (*UIInput_Char_Callback)(int c);
extern void (*UIInput_Key_Callback)(int c, int ev);
extern void (*UIInput_Mouse_Callback)(win_mouseEvents id, int w, int l);
void HookUIInput();

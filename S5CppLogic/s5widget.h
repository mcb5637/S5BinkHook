#pragma once
#include "s5data.h"

struct shok_widget_rect { // size 4
	float X, Y, W, H;
};

struct shok_widget_color { // size 4
	float Red, Green, Blue, Alpha; // type float or int?
};

struct shok_widget_string { // size 7
	PADDINGI(1)
	char* string;
	PADDINGI(5)
};
static_assert(sizeof(shok_widget_string) == 7 * 4);

#define shok_vtp_EGUIX_CFontIDHandler (void*)0x780B0C
struct shok_EGUIX_CFontIDHandler : shok_object { // size 2
	int FontID;
};

#define shok_vtp_EGUIX_CSingleStringHandler (void*)0x7809A4
struct shok_EGUIX_CSingleStringHandler : shok_object { // size 15
	shok_widget_string StringTableKey, RawString;
};
static_assert(sizeof(shok_EGUIX_CSingleStringHandler) == 15 * 4);

#define shok_vtp_EGUIX_CWidgetStringHelper (void*)0x781518
struct shok_EGUIX_CWidgetStringHelper : shok_object { // size 23
	shok_EGUIX_CFontIDHandler FontHandler;
	shok_EGUIX_CSingleStringHandler SingleStringHandler;
	int StringFrameDistance; //float?
	shok_widget_color Color;
};
static_assert(sizeof(shok_EGUIX_CWidgetStringHelper) == 23 * 4);

#define shok_vtp_EGUIX_CMaterial (void*)0x780890
struct shok_EGUIX_CMaterial : shok_object {
	int Texture; // type? // 1
	shok_widget_rect TextureCoordinates; // la 5
	shok_widget_color Color; // la 10
};
static_assert(sizeof(shok_EGUIX_CMaterial) == 10 * 4);

#define shok_vtp_EScr_CLuaFuncRefCommand (void*)0x786BE0
struct shoc_EScr_CLuaFuncRefCommand : shok_object {
	lua_State* L;
	PADDINGI(1)
	int Ref;
	PADDINGI(1)
	char* LuaCommand;
	PADDINGI(6)
};

#define shok_vtp_EGUIX_CLuaFunctionHelper (void*)0x780994
struct shok_EGUIX_CLuaFunctionHelper : shok_object { // size 20
	PADDINGI(1)
	char* LuaCommand;
	PADDINGI(5)
	shoc_EScr_CLuaFuncRefCommand FuncRefCommand;
};
static_assert(sizeof(shok_EGUIX_CLuaFunctionHelper) == 20 * 4);

struct shok_EGUIX_IMaterialAccess : shok_object { // multiinheritance
	shok_EGUIX_CMaterial Material; // la 10
};

struct shok_EGUIX_ITextAccess : shok_object { // multiinheritance

};

#define shok_vtp_EGUIX_CButtonHelper (void*)0x780818
struct shok_EGUIX_CButtonHelper : shok_object { // size 38
	PADDINGI(1);
	byte DisabledFlag, HighLightedFlag;
	PADDING(2);
	shok_EGUIX_CLuaFunctionHelper ActionFunction;
	shok_EGUIX_CSingleStringHandler ShortCutString;
};
static_assert(sizeof(shok_EGUIX_CButtonHelper) == 38 * 4);

#define shok_vtp_EGUIX_CToolTipHelper (void*)0x780B30
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
};
static_assert(sizeof(shok_EGUIX_CToolTipHelper) == 41 * 4);

#define shok_vtp_EGUIX_CBaseWidget (void*)0x780768
struct shok_EGUIX_CBaseWidget : shok_object {
	void* vtable_EGUIX_IOnEvent;
	PADDINGI(2)
	int WidgetID;
	shok_widget_rect PosAndSize;
	byte IsShown;
	PADDING(3)
	float ZPriority;
	int MotherWidgetID;
	int Group; // 12
	byte ForceToHandleMouseEventsFlag, ForceToNeverBeFoundFlag;
	PADDING(2); // 13

	int SetPosAndSize(float x, float y, float w, float h);
	byte* GetUpdateManualFlag();
	shok_EGUIX_CLuaFunctionHelper* GetUpdateFunc();
};

#define shok_vtp_EGUIX_CStaticWidget (void*)0x780F84
struct shok_EGUIX_CStaticWidget : shok_EGUIX_CBaseWidget {
	void* vtable_EGUIX_IRender; // 14
	shok_EGUIX_IMaterialAccess MaterialAcces; // la 24
};

#define shok_vtp_EGUIX_CStaticTextWidget (void*)0x780EE4
struct shok_EGUIX_CStaticTextWidget : shok_EGUIX_CStaticWidget {
	shok_EGUIX_ITextAccess ITextAccess;
	shok_EGUIX_CWidgetStringHelper StringHelper; // 27
	shok_EGUIX_CLuaFunctionHelper UpdateFunction; // 50
	byte UpdateManualFlag;
	PADDING(3)
	int FirstLineToPrint, NumberOfLinesToPrint, LineDistanceFactor;
};

#define shok_vtp_EGUIX_CButtonWidget (void*)0x780E5C
struct shok_EGUIX_CButtonWidget : shok_EGUIX_CBaseWidget {
	void* vtable_EGUIX_IRender; // 14
	void* vtable_EGUIX_IMaterialAccess; // 15
	shok_EGUIX_CButtonHelper ButtonHelper; // la 54
	shok_EGUIX_CMaterial Materials[5];
	int NumMaterials; // ? why after materials
	shok_EGUIX_CToolTipHelper ToolTipHelper; // 105 la 146
};
static_assert(sizeof(shok_EGUIX_CButtonWidget) == 146 * 4);

#define shok_vtp_EGUIX_CGfxButtonWidget (void*)0x780CD0
struct shok_EGUIX_CGfxButtonWidget : shok_EGUIX_CButtonWidget {
	shok_EGUIX_CMaterial IconMaterial;
	shok_EGUIX_CLuaFunctionHelper UpdateFunction;
	byte UpdateManualFlag;
	PADDING(3);
};

#define shok_vtp_EGUIX_CTextButtonWidget (void*)0x780DB0
struct shok_EGUIX_CTextButtonWidget : shok_EGUIX_CButtonWidget {
	void* vtable_EGUIX_ITextAccess; // 147
	shok_EGUIX_CWidgetStringHelper StringHelper; // la 170
	shok_EGUIX_CLuaFunctionHelper UpdateFunction;
	byte UpdateManualFlag;
	PADDING(3);
};

struct shok_widgetManager { // this thing has no vtable...


	int GetIdByName(const char* name);
	shok_EGUIX_CBaseWidget* GetWidgetByID(int id);
};
static inline shok_widgetManager* (*shok_getWidgetManagerObj)() = (shok_widgetManager * (*)())0x558473;

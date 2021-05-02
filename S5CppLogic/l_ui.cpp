#include "pch.h"
#include "l_ui.h"

shok_EGUIX_CBaseWidget* l_uiCheckWid(lua_State* L, int i) {
	int id;
	shok_widgetManager* wm = shok_getWidgetManagerObj();
	if (lua_isnumber(L, i)) {
		id = luaL_checkint(L, i);
	}
	else {
		id = wm->GetIdByName(luaL_checkstring(L, i));
	}
	shok_EGUIX_CBaseWidget* r = wm->GetWidgetByID(id);
	luaext_assertPointer(L, r, "no widget");
	return r;
}

void l_uiOverrideFunc(lua_State* L, shok_EGUIX_CLuaFunctionHelper* f, int i) {
	lua_pushvalue(L, i);
	if (f->FuncRefCommand.L) {
		lua_rawseti(L, LUA_REGISTRYINDEX, f->FuncRefCommand.Ref);
	}
	else {
		f->FuncRefCommand.L = L;
		f->FuncRefCommand.Ref = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	f->FuncRefCommand.NeedsCompile = 0;
}

void l_uiSetString(lua_State* L, shok_EGUIX_CSingleStringHandler& h, int i) {
	const char* s = luaL_checkstring(L, i);
	if (lua_toboolean(L, i + 1)) {
		h.StringTableKey.assign(s);
		h.RawString.assign("");
	}
	else {
		h.RawString.assign(s);
		h.StringTableKey.assign("");
	}
}

int l_uiGetWidAdr(lua_State* L) {
	shok_EGUIX_CBaseWidget* w = l_uiCheckWid(L, 1);
	lua_pushnumber(L, (int)w);
	return 1;
}

int l_uiGetWidPosAndSize(lua_State* L) {
	shok_EGUIX_CBaseWidget* w = l_uiCheckWid(L, 1);
	lua_pushnumber(L, w->PosAndSize.X);
	lua_pushnumber(L, w->PosAndSize.Y);
	lua_pushnumber(L, w->PosAndSize.W);
	lua_pushnumber(L, w->PosAndSize.H);
	return 4;
}

int l_uiSetWidPosAndSize(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	float x, y, w, h;
	if (lua_isnumber(L, 2))
		x = luaL_checkfloat(L, 2);
	else
		x = wid->PosAndSize.X;
	if (lua_isnumber(L, 3))
		y = luaL_checkfloat(L, 3);
	else
		y = wid->PosAndSize.Y;
	if (lua_isnumber(L, 4))
		w = luaL_checkfloat(L, 4);
	else
		w = wid->PosAndSize.W;
	if (lua_isnumber(L, 5))
		h = luaL_checkfloat(L, 5);
	else
		h = wid->PosAndSize.H;
	wid->SetPosAndSize(x, y, w, h);
	return 0;
}

int l_uiGetUpdateManualFlag(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	byte* f = wid->GetUpdateManualFlag();
	if (f) {
		lua_pushboolean(L, *f);
		return 1;
	}
	return luaL_error(L, "widget has no known updatemanual flag");
}
int l_uiSetUpdateManualFlag(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	bool flg = lua_toboolean(L, 2);
	byte* f = wid->GetUpdateManualFlag();
	if (f) {
		*f = flg;
		return 0;
	}
	return luaL_error(L, "widget has no known updatemanual flag");
}

int l_uiGetUpdateFunc(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CLuaFunctionHelper* fh = wid->GetUpdateFunc();
	luaext_assertPointer(L, fh, "widget has no known update func");
	lua_pushstring(L, fh->LuaCommand.c_str());
	if (fh->FuncRefCommand.L == L)
		lua_rawgeti(L, LUA_REGISTRYINDEX, fh->FuncRefCommand.Ref);
	else
		lua_pushstring(L, "no compiled func found");
	return 2;
}
int l_uiCallUpdateFunc(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CLuaFunctionHelper* fh = wid->GetUpdateFunc();
	luaext_assertPointer(L, fh, "widget has no known update func");
	fh->Call(wid->WidgetID);
	return 0;
}
int l_uiOverrideUpdateFunc(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CLuaFunctionHelper* fh = wid->GetUpdateFunc();
	luaext_assertPointer(L, fh, "widget has no known update func");
	l_uiOverrideFunc(L, fh, 2);
	return 0;
}

int l_uiGetAllSubWidgets(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	lua_newtable(L);
	int i = 1;
	for (shok_EGUIX_CBaseWidget* p : c->WidgetListHandler.SubWidgets) {
		lua_pushnumber(L, p->WidgetID);
		lua_rawseti(L, -2, i);
		i++;
	}
	return 1;
}

int l_uiGetMaterialTexCoord(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	int c = 0;
	shok_EGUIX_CMaterial* m = wid->GetMaterials(&c);
	luaext_assertPointer(L, m, "no known materials");
	int min = luaL_checkint(L, 2);
	luaext_assert(L, min >= 0 && min < c, "invalid index");
	lua_pushnumber(L, m[min].TextureCoordinates.X);
	lua_pushnumber(L, m[min].TextureCoordinates.Y);
	lua_pushnumber(L, m[min].TextureCoordinates.H);
	lua_pushnumber(L, m[min].TextureCoordinates.W);
	return 4;
}
int l_uiSetMaterialTexCoord(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	int c = 0;
	shok_EGUIX_CMaterial* m = wid->GetMaterials(&c);
	luaext_assertPointer(L, m, "no known materials");
	int min = luaL_checkint(L, 2);
	luaext_assert(L, min >= 0 && min < c, "invalid index");
	if (lua_isnumber(L, 3))
		m[min].TextureCoordinates.X = luaL_checkfloat(L, 3);
	if (lua_isnumber(L, 4))
		m[min].TextureCoordinates.Y = luaL_checkfloat(L, 4);
	if (lua_isnumber(L, 5))
		m[min].TextureCoordinates.H = luaL_checkfloat(L, 5);
	if (lua_isnumber(L, 6))
		m[min].TextureCoordinates.W = luaL_checkfloat(L, 6);
	return 0;
}

int l_uiGetTooltipData(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	lua_pushnumber(L, tt->TargetWidget);
	lua_pushboolean(L, tt->ControlTargetWidgetDisplayState);
	lua_pushboolean(L, tt->ToolTipEnabledFlag);
	return 3;
}
int l_uiSetTooltipData(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	if (!lua_isnoneornil(L, 2)) {
		tt->TargetWidget = l_uiCheckWid(L, 2)->WidgetID;
	}
	if (!lua_isnoneornil(L, 3))
		tt->ControlTargetWidgetDisplayState = lua_toboolean(L, 3);
	if (!lua_isnoneornil(L, 4))
		tt->ToolTipEnabledFlag = lua_toboolean(L, 4);
	return 0;
}

int l_uiGetTooltipString(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	lua_pushstring(L, tt->ToolTipString.RawString.c_str());
	lua_pushstring(L, tt->ToolTipString.StringTableKey.c_str());
	return 2;
}
int l_uiSetTooltipString(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	l_uiSetString(L, tt->ToolTipString, 2);
	return 0;
}

int l_uiTooltipGetFunc(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	lua_pushstring(L, tt->UpdateFunction.LuaCommand.c_str());
	if (tt->UpdateFunction.FuncRefCommand.L == L)
		lua_rawgeti(L, LUA_REGISTRYINDEX, tt->UpdateFunction.FuncRefCommand.Ref);
	else
		lua_pushstring(L, "no compiled func found");
	return 2;
}
int l_uiTooltipCallFunc(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	tt->UpdateFunction.Call(wid->WidgetID);
	return 0;
}
int l_uiTooltipOverrideFunc(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	l_uiOverrideFunc(L, &tt->UpdateFunction, 2);
	return 0;
}

int l_uiButtonGetAction(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CButtonHelper* fh = wid->GetButtonHelper();
	luaext_assertPointer(L, fh, "widget has no known action func");
	lua_pushstring(L, fh->ActionFunction.LuaCommand.c_str());
	if (fh->ActionFunction.FuncRefCommand.L == L)
		lua_rawgeti(L, LUA_REGISTRYINDEX, fh->ActionFunction.FuncRefCommand.Ref);
	else
		lua_pushstring(L, "no compiled func found");
	return 2;
}
int l_uiButtonCallAction(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CButtonHelper* fh = wid->GetButtonHelper();
	luaext_assertPointer(L, fh, "widget has no known action func");
	fh->ActionFunction.Call(wid->WidgetID);
	return 0;
}
int l_uiButtonOverrideAction(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CButtonHelper* fh = wid->GetButtonHelper();
	luaext_assertPointer(L, fh, "widget has no known action func");
	l_uiOverrideFunc(L, &fh->ActionFunction, 2);
	return 0;
}

int l_uiIsTooltipOfWidgetShown(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper();
	luaext_assertPointer(L, tt, "no known tooltip");
	lua_pushboolean(L, tt->IsToolTipShown);
	return 1;
}

int l_uiSetFont(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CWidgetStringHelper* s = wid->GetStringHelper();
	luaext_assertPointer(L, s, "no known stringhelper");
	const char* font = luaL_checkstring(L, 2);
	luaext_assert(L, DoesFileExist(font), "file doesnt exist");
	s->FontHandler.LoadFont(font);
	return 0;
}

int l_uiCreateStaticWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_getWidgetManagerObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CStaticWidget* ne = shok_EGUIX_CStaticWidget::Create();
	c->AddWidget(ne, name);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateStaticTextWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_getWidgetManagerObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CStaticTextWidget* ne = shok_EGUIX_CStaticTextWidget::Create();
	c->AddWidget(ne, name);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreatePureTooltipWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_getWidgetManagerObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CPureTooltipWidget* ne = shok_EGUIX_CPureTooltipWidget::Create();
	c->AddWidget(ne, name);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateGFXButtonWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_getWidgetManagerObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CGfxButtonWidget* ne = shok_EGUIX_CGfxButtonWidget::Create();
	c->AddWidget(ne, name);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateTextButtonWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_getWidgetManagerObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CTextButtonWidget* ne = shok_EGUIX_CTextButtonWidget::Create();
	c->AddWidget(ne, name);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateProgessBarWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_getWidgetManagerObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CProgressBarWidget* ne = shok_EGUIX_CProgressBarWidget::Create();
	c->AddWidget(ne, name);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateContainerWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_getWidgetManagerObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CContainerWidget* ne = shok_EGUIX_CContainerWidget::Create();
	c->AddWidget(ne, name);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}

int l_uiGetFontValues(lua_State* L) {
	const char* font = luaL_checkstring(L, 1);
	luaext_assert(L, DoesFileExist(font), "file doesnt exist");
	int id = 0;
	shok_fontManager::LoadFont(&id, font);
	shok_font* f = shok_getFontMangerObj()->GetFontObj(id);
	lua_pushnumber(L, f->Size);
	lua_pushnumber(L, f->Offset);
	lua_pushnumber(L, f->Spacing);
	return 3;
}
int l_uiSetFontValues(lua_State* L) {
	const char* font = luaL_checkstring(L, 1);
	luaext_assert(L, DoesFileExist(font), "file doesnt exist");
	int id = 0;
	shok_fontManager::LoadFont(&id, font);
	shok_font* f = shok_getFontMangerObj()->GetFontObj(id);
	if (lua_isnumber(L, 2))
		f->Size = luaL_checkfloat(L, 2);
	if (lua_isnumber(L, 3))
		f->Offset = luaL_checkfloat(L, 3);
	if (lua_isnumber(L, 4))
		f->Spacing = luaL_checkfloat(L, 4);
	return 0;
}

int l_uiTest(lua_State* L) {
	const char* s = luaL_checkstring(L, 1);
	int i = 0;
	shok_fontManager::LoadFont(&i, s);
	lua_pushnumber(L, (int)shok_getFontMangerObj()->GetFontObj(i));
	return 1;
}

void l_ui_init(lua_State* L)
{
	luaext_registerFunc(L, "WidgetGetPositionAndSize", &l_uiGetWidPosAndSize);
	luaext_registerFunc(L, "WidgetSetPositionAndSize", &l_uiSetWidPosAndSize);
	luaext_registerFunc(L, "WidgetGetAddress", &l_uiGetWidAdr);
	luaext_registerFunc(L, "WidgetGetUpdateManualFlag", &l_uiGetUpdateManualFlag);
	luaext_registerFunc(L, "WidgetSetUpdateManualFlag", &l_uiSetUpdateManualFlag);
	luaext_registerFunc(L, "WidgetGetUpdateFunc", &l_uiGetUpdateFunc);
	luaext_registerFunc(L, "WidgetCallUpdateFunc", &l_uiCallUpdateFunc);
	luaext_registerFunc(L, "WidgetOverrideUpdateFunc", &l_uiOverrideUpdateFunc);
	luaext_registerFunc(L, "ContainerWidgetGetAllChildren", &l_uiGetAllSubWidgets);
	luaext_registerFunc(L, "WidgetMaterialGetTextureCoordinates", &l_uiGetMaterialTexCoord);
	luaext_registerFunc(L, "WidgetMaterialSetTextureCoordinates", &l_uiSetMaterialTexCoord);
	luaext_registerFunc(L, "ButtonGetActionFunc", &l_uiButtonGetAction);
	luaext_registerFunc(L, "ButtonCallActionFunc", &l_uiButtonCallAction);
	luaext_registerFunc(L, "ButtonOverrideActionFunc", &l_uiButtonOverrideAction);
	luaext_registerFunc(L, "WidgetGetTooltipData", &l_uiGetTooltipData);
	luaext_registerFunc(L, "WidgetSetTooltipData", &l_uiSetTooltipData);
	luaext_registerFunc(L, "WidgetGetTooltipString", &l_uiGetTooltipString);
	luaext_registerFunc(L, "WidgetSetTooltipString", &l_uiSetTooltipString);
	luaext_registerFunc(L, "WidgetGetTooltipFunc", &l_uiTooltipGetFunc);
	luaext_registerFunc(L, "WidgetCallTooltipFunc", &l_uiTooltipCallFunc);
	luaext_registerFunc(L, "WidgetOverrideTooltipFunc", &l_uiTooltipOverrideFunc);
	luaext_registerFunc(L, "WidgetIsTooltipShown", &l_uiIsTooltipOfWidgetShown);
	luaext_registerFunc(L, "WidgetSetFont", &l_uiSetFont);
	luaext_registerFunc(L, "FontGetConfig", &l_uiGetFontValues);
	luaext_registerFunc(L, "FontSetConfig", &l_uiSetFontValues);
	luaext_registerFunc(L, "Test", &l_uiTest);
	luaext_registerFunc(L, "ContainerWidgetCreateStaticWidgetChild", &l_uiCreateStaticWidget);
	luaext_registerFunc(L, "ContainerWidgetCreateStaticTextWidgetChild", &l_uiCreateStaticTextWidget);
	luaext_registerFunc(L, "ContainerWidgetCreatePureTooltipWidgetChild", &l_uiCreatePureTooltipWidget);
	luaext_registerFunc(L, "ContainerWidgetCreateGFXButtonWidgetChild", &l_uiCreateGFXButtonWidget);
	luaext_registerFunc(L, "ContainerWidgetCreateTextButtonWidgetChild", &l_uiCreateTextButtonWidget);
	luaext_registerFunc(L, "ContainerWidgetCreateProgressBarWidgetChild", &l_uiCreateProgessBarWidget);
	luaext_registerFunc(L, "ContainerWidgetCreateContainerWidgetChild", &l_uiCreateContainerWidget);
}

// CppLogic.UI.WidgetGetAddress("StartMenu00_EndGame")
// StartMenu00_VersionNumber
// StartMenu00_EndGame
// StartMenu00
// GoldTooltipController
// CppLogic.UI.ButtonOverrideActionFunc("StartMenu00_EndGame", function() LuaDebugger.Log(XGUIEng.GetCurrentWidgetID()) end)
// CppLogic.UI.Test("StartMenu00") XGUIEng.SetMaterialTexture("test", 0, "data\\graphics\\textures\\gui\\hero_sel_dario.png")
// CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("test", "test1"); XGUIEng.SetMaterialTexture("test", 0, "data\\graphics\\textures\\gui\\hero_sel_dario.png"); CppLogic.UI.WidgetSetPositionAndSize("test", 5, 5, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetMaterialColor("test", 0, 0,0,0,0); XGUIEng.SetTextColor("test",255,255,255,255); XGUIEng.SetText("test","tst"); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreatePureTooltipWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); CppLogic.UI.WidgetSetTooltipData("test", "StartMenu_TooltipText", true, true); CppLogic.UI.WidgetSetTooltipString("test", "tst"); CppLogic.UI.WidgetOverrideTooltipFunc("test", function() LuaDebugger.Log(1) end); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetText("test","tst"); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateProgressBarWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateContainerWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 100, 100); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.WidgetSetFont("StartMenu00_EndGame", "data\\menu\\fonts\\medium11bold.met") --"data\\menu\\fonts\\mainmenularge.met"
// CppLogic.UI.WidgetSetFont("StartMenu00_EndGame", "data\\menu\\fonts\\standard10.met")

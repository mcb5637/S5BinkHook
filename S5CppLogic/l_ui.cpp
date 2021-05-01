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

int l_uiTest(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	luaext_assert(L, wid->IsContainerWidget(), "no container widget");
	shok_EGUIX_CContainerWidget* c = (shok_EGUIX_CContainerWidget*)wid;
	shok_EGUIX_CStaticWidget* ne = shok_EGUIX_CStaticWidget::Create();
	c->AddWidget(ne, "test");
	ne->BackgroundMaterial.Color = { 0xFF, 0xFF, 0xFF ,0xFF };
	ne->BackgroundMaterial.TextureCoordinates = { 0, 0, 1 ,1 };
	ne->SetPosAndSize(10, 10, 32, 32);
	ne->IsShown = 1;
	return 0;
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
	luaext_registerFunc(L, "Test", &l_uiTest);
	luaext_registerFunc(L, "ContainerWidgetCreateStaticWidgetChild", &l_uiCreateStaticWidget);
	luaext_registerFunc(L, "ContainerWidgetCreateStaticTextWidgetChild", &l_uiCreateStaticTextWidget);
}

// CppLogic.UI.WidgetGetAddress("StartMenu00_EndGame")
// StartMenu00_VersionNumber
// StartMenu00_EndGame
// StartMenu00
// GoldTooltipController
// CppLogic.UI.ButtonOverrideActionFunc("StartMenu00_EndGame", function() LuaDebugger.Log(XGUIEng.GetCurrentWidgetID()) end)
// CppLogic.UI.Test("StartMenu00") XGUIEng.SetMaterialTexture("test", 0, "data\\graphics\\textures\\gui\\hero_sel_dario.png")
// CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("StartMenu00", "test"); XGUIEng.SetMaterialTexture("test", 0, "data\\graphics\\textures\\gui\\hero_sel_dario.png"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); CppLogic.UI.WidgetMaterialSetTextureCoordinates("test", 0, 0, 0, 1, 1); XGUIEng.SetMaterialColor("test", 0, 255,255,255,255); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetMaterialColor("test", 0, 0,0,0,0);  XGUIEng.SetTextColor("test",255,255,255,255); XGUIEng.SetText("test","tst"); XGUIEng.ShowWidget("test", 1);

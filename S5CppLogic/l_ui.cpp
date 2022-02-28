#include "pch.h"
#include "l_ui.h"
#include <WinUser.h>

shok_EGUIX_CBaseWidget* l_uiCheckWid(lua_State* L, int i) {
	int id;
	shok_widgetManager* wm = shok_widgetManager::GlobalObj();
	if (lua_isnumber(L, i)) {
		id = luaL_checkint(L, i);
	}
	else {
		id = wm->GetIdByName(luaL_checkstring(L, i));
	}
	shok_EGUIX_CBaseWidget* r = wm->GetWidgetByID(id);
	if (!r)
		luaL_error(L, "no widget at %i", i);
	return r;
}

void l_uiClearFunc(lua_State* L, shok_EGUIX_CLuaFunctionHelper* f) {
	if (f->FuncRefCommand.L) {
		luaL_unref(f->FuncRefCommand.L, LUA_REGISTRYINDEX, f->FuncRefCommand.Ref);
		f->FuncRefCommand.L = nullptr;
		f->FuncRefCommand.Ref = LUA_NOREF;
	}
}
void l_uiOverrideFunc(lua_State* L, shok_EGUIX_CLuaFunctionHelper* f, int i) {
	lua_pushvalue(L, i);
	l_uiClearFunc(L, f);
	f->FuncRefCommand.L = L;
	f->FuncRefCommand.Ref = luaL_ref(L, LUA_REGISTRYINDEX);
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

const char* l_uiCheckFontString(const char* font) {
	if (!BB::CFileSystemMgr::DoesFileExist(font))
		return "file doesnt exist";
	if (!str_ends_with(font, ".met"))
		return "wrong file extension";
	std::string_view str{ font };
	if ((str.rfind("data\\maps\\externalmap\\", 0) == std::string::npos) && (str.rfind("data\\menu\\fonts\\", 0) == std::string::npos))
		return "incorrect folder";
	return nullptr;
}

int l_uiGetWidAdr(lua_State* L) {
	shok_EGUIX_CBaseWidget* w = l_uiCheckWid(L, 1);
	lua_pushnumber(L, reinterpret_cast<int>(w));
	shok_EGUIX_CLuaFunctionHelper* bh = w->GetUpdateFunc();
	if (bh) {
		lua_pushnumber(L, (int)bh);
		return 2;
	}
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
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	lua_newtable(L);
	int i = 1;
	for (shok_EGUIX_CBaseWidget* p : c->WidgetListHandler.SubWidgets) {
		lua_pushnumber(L, p->WidgetID);
		lua_rawseti(L, -2, i);
		i++;
	}
	return 1;
}

int l_ui_IsContainerWid(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	if (shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid))
		lua_pushboolean(L, true);
	else
		lua_pushboolean(L, false);
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
	lua_pushnumber(L, m[min].TextureCoordinates.W);
	lua_pushnumber(L, m[min].TextureCoordinates.H);
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
		m[min].TextureCoordinates.W = luaL_checkfloat(L, 5);
	if (lua_isnumber(L, 6))
		m[min].TextureCoordinates.H = luaL_checkfloat(L, 6);
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
	const char* err = l_uiCheckFontString(font);
	if (err)
		luaL_error(L, err);
	s->FontHandler.LoadFont(font);
	return 0;
}

int l_uiGetBaseWidgetData(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	lua_pushnumber(L, wid->ZPriority);
	lua_pushboolean(L, wid->ForceToHandleMouseEventsFlag);
	lua_pushboolean(L, wid->ForceToNeverBeFoundFlag);
	return 3;
}
int l_uiSetBaseWidgetData(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	if (lua_isnumber(L, 2))
		wid->ZPriority = luaL_checkfloat(L, 2);
	if (lua_isboolean(L, 3))
		wid->ForceToHandleMouseEventsFlag = lua_toboolean(L, 3);
	if (lua_isboolean(L, 4))
		wid->ForceToNeverBeFoundFlag = lua_toboolean(L, 4);
	return 0;
}

int l_uiGetWidgetStringFrameDistance(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CWidgetStringHelper* s = wid->GetStringHelper();
	luaext_assertPointer(L, s, "no string helper");
	lua_pushnumber(L, s->StringFrameDistance);
	return 1;
}
int l_uiSetWidgetStringFrameDistance(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CWidgetStringHelper* s = wid->GetStringHelper();
	luaext_assertPointer(L, s, "no string helper");
	s->StringFrameDistance = luaL_checkfloat(L, 2);
	return 0;
}

int l_uiGetStaticTextWidgetLineDistanceFactor(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CStaticTextWidget* tw = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CStaticTextWidget>(wid);
	luaext_assertPointer(L, tw, "no static text widget");
	lua_pushnumber(L, tw->LineDistanceFactor);
	return 1;
}
int l_uiSetStaticTextWidgetLineDistanceFactor(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CStaticTextWidget* tw = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CStaticTextWidget>(wid);
	luaext_assertPointer(L, tw, "no static text widget");
	tw->LineDistanceFactor = luaL_checkfloat(L, 2);
	return 0;
}

int l_uiGetButtonShortCut(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CButtonHelper* b = wid->GetButtonHelper();
	luaext_assertPointer(L, b, "no button");
	lua_pushstring(L, b->ShortCutString.RawString.c_str());
	lua_pushstring(L, b->ShortCutString.StringTableKey.c_str());
	return 2;
}
int l_uiSetButtonShortCut(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CButtonHelper* b = wid->GetButtonHelper();
	luaext_assertPointer(L, b, "no button");
	l_uiSetString(L, b->ShortCutString, 2);
	return 0;
}

int l_uiSetWidgetGroup(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	const char* s = luaL_checkstring(L, 2);
	shok_EGUIX_CWidgetGroupManager* wgm = shok_EGUIX_CWidgetGroupManager::GlobalObj();
	int g = wgm->GetGroupId(s);
	if (!g)
		g = wgm->CreateGroup(s);
	wid->Group = g;
	return 0;
}

int l_uiCreateStaticWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	if (!lua_isnoneornil(L, 3))
		bef = l_uiCheckWid(L, 3);
	shok_EGUIX_CStaticWidget* ne = shok_EGUIX_CStaticWidget::Create();
	c->AddWidget(ne, name, bef);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateStaticTextWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	if (!lua_isnoneornil(L, 3))
		bef = l_uiCheckWid(L, 3);
	shok_EGUIX_CStaticTextWidget* ne = shok_EGUIX_CStaticTextWidget::Create();
	c->AddWidget(ne, name, bef);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreatePureTooltipWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	if (!lua_isnoneornil(L, 3))
		bef = l_uiCheckWid(L, 3);
	shok_EGUIX_CPureTooltipWidget* ne = shok_EGUIX_CPureTooltipWidget::Create();
	c->AddWidget(ne, name, bef);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateGFXButtonWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	if (!lua_isnoneornil(L, 3))
		bef = l_uiCheckWid(L, 3);
	shok_EGUIX_CGfxButtonWidget* ne = shok_EGUIX_CGfxButtonWidget::Create();
	c->AddWidget(ne, name, bef);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateTextButtonWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	if (!lua_isnoneornil(L, 3))
		bef = l_uiCheckWid(L, 3);
	shok_EGUIX_CTextButtonWidget* ne = shok_EGUIX_CTextButtonWidget::Create();
	c->AddWidget(ne, name, bef);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateProgessBarWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	if (!lua_isnoneornil(L, 3))
		bef = l_uiCheckWid(L, 3);
	shok_EGUIX_CProgressBarWidget* ne = shok_EGUIX_CProgressBarWidget::Create();
	c->AddWidget(ne, name, bef);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_uiCreateContainerWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	if (!lua_isnoneornil(L, 3))
		bef = l_uiCheckWid(L, 3);
	shok_EGUIX_CContainerWidget* ne = shok_EGUIX_CContainerWidget::Create();
	c->AddWidget(ne, name, bef);
	lua_pushnumber(L, c->WidgetID);
	return 1;
}
int l_ui_CreateCustomWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* c = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	luaext_assertPointer(L, c, "no container widget");
	const char* name = luaL_checkstring(L, 2);
	luaext_assert(L, shok_widgetManager::GlobalObj()->GetIdByName(name) == 0, "name already in use");
	shok_EGUIX_CBaseWidget* bef = nullptr;
	const char* customname = luaL_check_string(L, 3);
	if (!lua_isnoneornil(L, 4))
		bef = l_uiCheckWid(L, 4);
	shok_EGUIX_CCustomWidget* ne = (*shok_BB_CClassFactory::GlobalObj)->CreateObject<shok_EGUIX_CCustomWidget>();
	c->AddWidget(ne, name, bef);
	ne->CustomClassName.assign(customname);
	ne->IntegerUserVariable0DefaultValue = luaL_optint(L, 5, 0);
	ne->IntegerUserVariable1DefaultValue = luaL_optint(L, 6, 0);
	ne->IntegerUserVariable2DefaultValue = luaL_optint(L, 7, 0);
	ne->IntegerUserVariable3DefaultValue = luaL_optint(L, 8, 0);
	ne->IntegerUserVariable4DefaultValue = luaL_optint(L, 9, 0);
	ne->IntegerUserVariable5DefaultValue = luaL_optint(L, 10, 0);
	ne->StringUserVariable0DefaultValue.assign(luaL_optstring(L, 11, ""));
	ne->StringUserVariable1DefaultValue.assign(luaL_optstring(L, 12, ""));
	ne->InitializeCustomWidget();
	lua_pushnumber(L, c->WidgetID);
	return 1;
}

int l_uiGetFontValues(lua_State* L) {
	const char* font = luaL_checkstring(L, 1);
	const char* err = l_uiCheckFontString(font);
	if (err)
		luaL_error(L, err);
	int id = 0;
	shok_fontManager::LoadFont(&id, font);
	shok_font* f = shok_fontManager::GlobalObj()->GetFontObj(id);
	lua_pushnumber(L, f->Size);
	lua_pushnumber(L, f->Offset);
	lua_pushnumber(L, f->Spacing);
	return 3;
}
int l_uiSetFontValues(lua_State* L) {
	const char* font = luaL_checkstring(L, 1);
	const char* err = l_uiCheckFontString(font);
	if (err)
		luaL_error(L, err);
	int id = 0;
	shok_fontManager::LoadFont(&id, font);
	shok_font* f = shok_fontManager::GlobalObj()->GetFontObj(id);
	if (lua_isnumber(L, 2))
		f->Size = luaL_checkfloat(L, 2);
	if (lua_isnumber(L, 3))
		f->Offset = luaL_checkfloat(L, 3);
	if (lua_isnumber(L, 4))
		f->Spacing = luaL_checkfloat(L, 4);
	return 0;
}

int l_uiRemoveWidget(lua_State* L) {
	shok_EGUIX_CBaseWidget* wid = l_uiCheckWid(L, 1);
	shok_EGUIX_CContainerWidget* cw = shok_DynamicCast<shok_EGUIX_CBaseWidget, shok_EGUIX_CContainerWidget>(wid);
	if (cw)
		luaext_assert(L, cw->WidgetListHandler.SubWidgets.size() == 0, "container widget has to be empty");
	//shok_widgetManager::GlobalObj()->RemoveWidget(wid);
	wid->Destructor(true); // destructor calls removewidget
	return 0;
}

int l_uiTest(lua_State* L) {
	const char* s = luaL_checkstring(L, 1);
	int g = shok_EGUIX_CWidgetGroupManager::GlobalObj()->GetGroupId(s);
	if (!g)
		g = shok_EGUIX_CWidgetGroupManager::GlobalObj()->CreateGroup(s);
	lua_pushnumber(L, g);
	return 1;
}

int l_ui_SetCharTrigger(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	if (lua_isnil(L, 1)) {
		UIInput_Char_Callback = nullptr;
		lua_pushlightuserdata(L, &l_ui_SetCharTrigger);
		lua_pushnil(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		return 0;
	}

	luaext_assert(L, lua_isfunction(L, 1), "no func at 1");
	lua_pushlightuserdata(L, &l_ui_SetCharTrigger);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);

	if (!UIInput_Char_Callback) {
		HookUIInput();
		UIInput_Char_Callback = [](int c) {
			lua_State* L = *EScr::CScriptTriggerSystem::GameState;
			int t = lua_gettop(L);
			bool r = false;

			lua_pushlightuserdata(L, &l_ui_SetCharTrigger);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, c);
			if (!lua_pcall(L, 1, 1, 0)) {
				r = luaext_optbool(L, -1, false);
			}


			lua_settop(L, t);
			return r;
		};
	}
	return 0;
}
int l_ui_SetKeyTrigger(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	if (lua_isnil(L, 1)) {
		UIInput_Key_Callback = nullptr;
		lua_pushlightuserdata(L, &l_ui_SetKeyTrigger);
		lua_pushnil(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		return 0;
	}

	luaext_assert(L, lua_isfunction(L, 1), "no func at 1");
	lua_pushlightuserdata(L, &l_ui_SetKeyTrigger);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);

	if (!UIInput_Key_Callback) {
		HookUIInput();
		UIInput_Key_Callback = [](int c, win_mouseEvents id) {
			lua_State* L = *EScr::CScriptTriggerSystem::GameState;
			int t = lua_gettop(L);
			bool r = false;

			lua_pushlightuserdata(L, &l_ui_SetKeyTrigger);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, c);
			lua_pushboolean(L, id == win_mouseEvents::KeyUp || id == win_mouseEvents::SysKeyUp);
			if (!lua_pcall(L, 2, 1, 0)) {
				r = luaext_optbool(L, -1, false);
			}

			lua_settop(L, t);
			return r;
		};
	}
	return 0;
}
int l_ui_SetMouseTrigger(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	if (lua_isnil(L, 1)) {
		UIInput_Mouse_Callback = nullptr;
		lua_pushlightuserdata(L, &l_ui_SetMouseTrigger);
		lua_pushnil(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		return 0;
	}

	luaext_assert(L, lua_isfunction(L, 1), "no func at 1");
	lua_pushlightuserdata(L, &l_ui_SetMouseTrigger);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);

	if (!UIInput_Mouse_Callback) {
		HookUIInput();
		UIInput_Mouse_Callback = [](win_mouseEvents id, int w, int l) {
			if (id == win_mouseEvents::MouseMove)
				return false;

			lua_State* L = *EScr::CScriptTriggerSystem::GameState;
			int t = lua_gettop(L);
			bool r = false;

			lua_pushlightuserdata(L, &l_ui_SetMouseTrigger);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, static_cast<int>(id));
			lua_pushnumber(L, l & 0xFFFF);
			lua_pushnumber(L, (l >> 16) & 0xFFFF);
			int res = 1;

			if (id >= win_mouseEvents::LButtonDown && id <= win_mouseEvents::MButtonDBl) {
				res = lua_pcall(L, 3, 1, 0);
			}
			else if (id == win_mouseEvents::MouseWheel) {
				lua_pushboolean(L, w > 0);
				res = lua_pcall(L, 4, 1, 0);
			}
			else if (id >= win_mouseEvents::XButtonDown && id <= win_mouseEvents::XButtonDBl) {
				lua_pushboolean(L, ((w >> 16) & 0xFFFF) - 1);
				res = lua_pcall(L, 4, 1, 0);
			}
			if (!res) {
				r = luaext_optbool(L, -1, false);
			}

			lua_settop(L, t);
			return r;
		};
	}
	return 0;
}
int l_ui_SetMouseTriggerMainMenu(lua_State* L) {
	if (HasSCELoader())
		luaL_error(L, "not supported with SCELoader");
	if (lua_isnil(L, 1)) {
		UIInput_Mouse_CallbackMainMenu = nullptr;
		lua_pushlightuserdata(L, &l_ui_SetMouseTriggerMainMenu);
		lua_pushnil(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		return 0;
	}

	luaext_assert(L, lua_isfunction(L, 1), "no func at 1");
	lua_pushlightuserdata(L, &l_ui_SetMouseTriggerMainMenu);
	lua_pushvalue(L, 1);
	lua_rawset(L, LUA_REGISTRYINDEX);

	if (!UIInput_Mouse_CallbackMainMenu) {
		HookUIInput();
		UIInput_Mouse_CallbackMainMenu = [](win_mouseEvents id, int w, int l) {
			if (id == win_mouseEvents::MouseMove)
				return false;

			if ((*shok_Framework_CMain::GlobalObj)->CurrentMode != 1)
				return false;

			lua_State* L = mainmenu_state;
			int t = lua_gettop(L);
			bool r = false;

			lua_pushlightuserdata(L, &l_ui_SetMouseTriggerMainMenu);
			lua_rawget(L, LUA_REGISTRYINDEX);
			lua_pushnumber(L, static_cast<int>(id));
			lua_pushnumber(L, l & 0xFFFF);
			lua_pushnumber(L, (l >> 16) & 0xFFFF);
			int res = 1;

			if (id >= win_mouseEvents::LButtonDown && id <= win_mouseEvents::MButtonDBl) {
				res = lua_pcall(L, 3, 1, 0);
			}
			else if (id == win_mouseEvents::MouseWheel) {
				lua_pushboolean(L, w > 0);
				res = lua_pcall(L, 4, 1, 0);
			}
			else if (id >= win_mouseEvents::XButtonDown && id <= win_mouseEvents::XButtonDBl) {
				lua_pushboolean(L, ((w >> 16) & 0xFFFF) - 1);
				res = lua_pcall(L, 4, 1, 0);
			}
			if (!res) {
				r = luaext_optbool(L, -1, false);
			}

			lua_settop(L, t);
			return r;
		};
	}
	return 0;
}

int l_ui_ShowResFloatie(lua_State* L) {
	(*shok_GGUI_C3DOnScreenInformationCustomWidget::GlobalObj)->ShowResourceFloatieOnEntity(luaext_checkEntityId(L, 1), luaL_checkint(L, 2));
	return 0;
}

int l_ui_GetClientSize(lua_State* L) {
	RECT r;
	if (GetClientRect(*shok_mainWindowHandle, &r)) {
		lua_pushnumber(L, r.right);
		lua_pushnumber(L, r.bottom);
		return 2;
	}
	return 0;
}

int l_ui_GetWidgetName(lua_State* L) {
	shok_EGUIX_CBaseWidget* w = l_uiCheckWid(L, 1);
	lua_pushstring(L, shok_widgetManager::GlobalObj()->WidgetNameManager->GetNameByID(w->WidgetID));
	return 1;
}

int l_ui_GetLandscapePosAtScreenPos(lua_State* L) {
	int x = luaL_checkint(L, 1);
	int y = luaL_checkint(L, 2);
	shok::PositionRot p;
	if ((*ED::CGlobalsLogicEx::GlobalObj)->Landscape->GetTerrainPosAtScreenCoords(p, x, y)) {
		luaext_pushPos(L, p);
		lua_pushnumber(L, p.r);
		return 2;
	}
	return 0;
}

int l_ui_ShowCommandAckAtPos(lua_State* L) {
	shok::Position p;
	luaext_checkPos(L, p, 1);
	(*ED::CGlobalsBaseEx::GlobalObj)->CommandAcks->ShowAck(p);
	return 0;
}

int l_ui_SetGUIState_LuaSelection(lua_State* L) {
	luaext_assert(L, lua_isfunction(L, 1), "no confirm func");
	auto* vh = shok_GGUI_CManager::GlobalObj()->C3DViewHandler;
	vh->StateIdManager->GetIDByNameOrCreate("LuaSelectionState", 27); // make sure the state id exists
	vh->SetGUIState<CppL_GUIState_LuaSelection>();
	CppL_GUIState_LuaSelection* s = shok_DynamicCast<shok_GGUI_CState, CppL_GUIState_LuaSelection>(vh->CurrentState);
	lua_pushvalue(L, 1);
	s->RefOnKlick = luaL_ref(L, LUA_REGISTRYINDEX);
	if (!lua_isnoneornil(L, 2)) {
		luaext_assert(L, lua_isfunction(L, 2), "no cancel func");
		lua_pushvalue(L, 2);
		s->RefOnCancel = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	return 0;
}


// creator func, used by class factory
shok_BB_IObject* __stdcall CppL_GUIState_LuaSelection_ctor() {
	// allocate an object and then call the constructor for it manually
	return new (shok::Malloc(sizeof(CppL_GUIState_LuaSelection))) CppL_GUIState_LuaSelection();
}
void CppL_GUIState_LuaSelection::Initialize()
{
	// add class to factory, best called on mainmenu lua state initialization (will be called before anything serious gets loaded)
	// map lua state initialization will be to late, as most stuff is already loaded there
	(*shok_BB_CClassFactory::GlobalObj)->AddClassToFactory(CppL_GUIState_LuaSelection::Identifier, "class CppLogic::GuiState_LuaSelection", &CppL_GUIState_LuaSelection_ctor, nullptr);
}
CppL_GUIState_LuaSelection::CppL_GUIState_LuaSelection()
{
	// make sure to initialize all vtables of the object manually
	vtable = CppL_GUIState_LuaSelection::vtp;
}
CppL_GUIState_LuaSelection::~CppL_GUIState_LuaSelection()
{
	if (RefOnKlick != LUA_NOREF)
		luaL_unref(*EScr::CScriptTriggerSystem::GameState, LUA_REGISTRYINDEX, RefOnKlick);
	if (RefOnCancel != LUA_NOREF)
		luaL_unref(*EScr::CScriptTriggerSystem::GameState, LUA_REGISTRYINDEX, RefOnCancel);
}
bool CppL_GUIState_LuaSelection::OnMouseEvent(BB::CEvent* ev)
{
	BB::CMouseEvent* mev = shok_DynamicCast<BB::CEvent, BB::CMouseEvent>(ev);
	if (mev && mev->IsEvent(shok::InputEventIds::MouseButtonDown)) {
		if (mev->IsKey(shok::Keys::MouseLButton)) {
			bool r = true;
			if (RefOnKlick != LUA_NOREF) {
				lua_State* L = *EScr::CScriptTriggerSystem::GameState;
				int i = lua_gettop(L);
				lua_rawgeti(L, LUA_REGISTRYINDEX, RefOnKlick);
				lua_pushnumber(L, mev->X);
				lua_pushnumber(L, mev->Y);
				lua_pcall(L, 2, 1, 0);
				if (lua_isboolean(L, -1))
					r = lua_toboolean(L, -1);
				lua_settop(L, i);
			}
			if (r)
				Cancel(false);
			return r;
		}
		else if (mev->IsKey(shok::Keys::MouseRButton)) { // rmb
			Cancel(true);
			return true;
		}
	}
	return false;
}
void CppL_GUIState_LuaSelection::Cancel(bool calllua)
{
	if (calllua && RefOnCancel != LUA_NOREF) {
		lua_State* L = *EScr::CScriptTriggerSystem::GameState;
		int i = lua_gettop(L);
		lua_rawgeti(L, LUA_REGISTRYINDEX, RefOnCancel);
		lua_pcall(L, 0, 0, 0);
		lua_settop(L, i);
	}
	C3DViewHandler->SetGUIStateByIdentfierOnNextUpdate<shok_GGUI_CSelectionState>();
}

// funcs that will be placed in the vtable (fastcall with an ignored 2nd parameter to emulate thiscall)
// "fake" destructor, call the real destructor, then deallocate if needed
void __fastcall CppL_GUIState_LuaSelection_dtor(CppL_GUIState_LuaSelection* th, int _, bool free) {
	th->~CppL_GUIState_LuaSelection();
	if (free)
		shok::Free(th);
}
unsigned int __stdcall CppL_GUIState_LuaSelection_GetClassID(const shok_BB_IObject* th) {
	return CppL_GUIState_LuaSelection::Identifier;
}
bool __fastcall CppL_GUIState_LuaSelection_OnMouseEvent(CppL_GUIState_LuaSelection* th, int _, BB::CEvent* ev) {
	return th->OnMouseEvent(ev);
}
int __fastcall CppL_GUIState_LuaSelection_Cancel(CppL_GUIState_LuaSelection* th) {
	th->Cancel(true);
	return 1;
}
const char* __fastcall CppL_GUIState_LuaSelection_GetName(const shok_GGUI_CState* th) {
	return "LuaSelectionState";
}
// generate a typedesc for it (you need to write a custom one if you want type_info::name to return something useful
RTTI_TypeDescriptor CppL_GUIState_LuaSelection_TypeDesc {};
const int CppL_GUIState_LuaSelection::TypeDesc = reinterpret_cast<int>(&CppL_GUIState_LuaSelection_TypeDesc);
// generate base class array (have a look at array of other classes with the same ihneritance)
RTTI_BaseClassDescriptor CppL_GUIState_LuaSelection_BaseClass1{ &CppL_GUIState_LuaSelection_TypeDesc, 2, 0, -1, 0, 0 };
RTTI_BaseClassDescriptor CppL_GUIState_LuaSelection_BaseClass2{ RTTI_TypeDescriptor::TypeID<shok_GGUI_CState>(), 1, 0, -1, 0, 0 };
RTTI_BaseClassDescriptor CppL_GUIState_LuaSelection_BaseClass3{ RTTI_TypeDescriptor::TypeID<shok_BB_IObject>(), 0, 0, -1, 0, 0 };
RTTI_BaseClassDescriptor* CppL_GUIState_LuaSelection_BaseClasses[] = { &CppL_GUIState_LuaSelection_BaseClass1, &CppL_GUIState_LuaSelection_BaseClass2, &CppL_GUIState_LuaSelection_BaseClass3 };
// generate class hierarchy (remember to set the length of the array)
RTTI_ClassHierarchyDescriptor CppL_GUIState_LuaSelection_ClassHierarchy{ 0, 0, 3, CppL_GUIState_LuaSelection_BaseClasses };
// finally the object locator
RTTI_CompleteObjectLocator CppL_GUIState_LuaSelection_ObjectLocator{ 0, 0, 0, &CppL_GUIState_LuaSelection_TypeDesc, &CppL_GUIState_LuaSelection_ClassHierarchy };
// manually generate the vtable, important: add the objectlocator to -1 (like here, at 0 in the struct and then use the dtor as the vtable)
// you can reuse shok funcs, as it is done here
struct vtable_CppL_GUIState_LuaSelection {
	RTTI_CompleteObjectLocator* ObLoc = &CppL_GUIState_LuaSelection_ObjectLocator;
	void(__fastcall* dtor)(CppL_GUIState_LuaSelection* th, int _, bool free) = &CppL_GUIState_LuaSelection_dtor;
	unsigned int(__stdcall* GetClassIdentifier)(const shok_BB_IObject* th) = &CppL_GUIState_LuaSelection_GetClassID;
	int retzero = 0x55336A;
	bool(__fastcall* OnMouseEvent)(CppL_GUIState_LuaSelection* th, int _, BB::CEvent* ev) = &CppL_GUIState_LuaSelection_OnMouseEvent;
	int SetStateParameters = 0x52B509;
	int(__fastcall* Cancel)(CppL_GUIState_LuaSelection* th) = &CppL_GUIState_LuaSelection_Cancel;
	const char* (__fastcall* GetName)(const shok_GGUI_CState* th) = &CppL_GUIState_LuaSelection_GetName;
	int OnSelectionChanged = 0x526A15;
};
vtable_CppL_GUIState_LuaSelection CppL_GUIState_LuaSelectionVtable{};
// the vtp that will be set into the object, as discussed earlier this is & of element 1 in the struct
const int CppL_GUIState_LuaSelection::vtp = reinterpret_cast<int>(&CppL_GUIState_LuaSelectionVtable.dtor);

void l_ui_cleanup(lua_State* L) {
	UIInput_Char_Callback = nullptr;
	UIInput_Key_Callback = nullptr;
	UIInput_Mouse_Callback = nullptr;
	if (shok_widgetManager* wm = shok_widgetManager::GlobalObj()) {
		for (shok_EGUIX_CBaseWidget* wid : wm->Widgets) {
			if (shok_EGUIX_CToolTipHelper* tt = wid->GetTooltipHelper()) {
				l_uiClearFunc(L, &tt->UpdateFunction);
			}
			if (shok_EGUIX_CLuaFunctionHelper* fh = wid->GetUpdateFunc()) {
				l_uiClearFunc(L, fh);
			}
			if (shok_EGUIX_CButtonHelper* bh = wid->GetButtonHelper()) {
				l_uiClearFunc(L, &bh->ActionFunction);
			}
		}
	}
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
	luaext_registerFunc(L, "WidgetGetBaseData", &l_uiGetBaseWidgetData);
	luaext_registerFunc(L, "WidgetSetBaseData", &l_uiSetBaseWidgetData);
	luaext_registerFunc(L, "WidgetGetStringFrameDistance", &l_uiGetWidgetStringFrameDistance);
	luaext_registerFunc(L, "WidgetSetStringFrameDistance", &l_uiSetWidgetStringFrameDistance);
	luaext_registerFunc(L, "StaticTextWidgetGetLineDistanceFactor", &l_uiGetStaticTextWidgetLineDistanceFactor);
	luaext_registerFunc(L, "StaticTextWidgetSetLineDistanceFactor", &l_uiSetStaticTextWidgetLineDistanceFactor);
	luaext_registerFunc(L, "ButtonGetShortcutString", &l_uiGetButtonShortCut);
	luaext_registerFunc(L, "ButtonSetShortcutString", &l_uiSetButtonShortCut);
	luaext_registerFunc(L, "WidgetSetGroup", &l_uiSetWidgetGroup);
	luaext_registerFunc(L, "RemoveWidget", &l_uiRemoveWidget);
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
	luaext_registerFunc(L, "ContainerWidgetCreateCustomWidgetChild", &l_ui_CreateCustomWidget);
	luaext_registerFunc(L, "SetCharTrigger", &l_ui_SetCharTrigger);
	luaext_registerFunc(L, "SetKeyTrigger", &l_ui_SetKeyTrigger);
	luaext_registerFunc(L, "SetMouseTrigger", &l_ui_SetMouseTrigger);
	luaext_registerFunc(L, "ShowResourceFloatieOnEntity", &l_ui_ShowResFloatie);
	luaext_registerFunc(L, "GetClientSize", &l_ui_GetClientSize);
	luaext_registerFunc(L, "IsContainerWidget", &l_ui_IsContainerWid);
	luaext_registerFunc(L, "GetWidgetName", &l_ui_GetWidgetName);
	luaext_registerFunc(L, "SetGUIStateLuaSelection", &l_ui_SetGUIState_LuaSelection);
	luaext_registerFunc(L, "GetLandscapePosAtScreenPos", &l_ui_GetLandscapePosAtScreenPos);
	luaext_registerFunc(L, "ShowCommandAcknowledgementAtPosition", &l_ui_ShowCommandAckAtPos);

	if (L == mainmenu_state) {
		luaext_registerFunc(L, "SetMouseTriggerMainMenu", &l_ui_SetMouseTriggerMainMenu);
		CppL_GUIState_LuaSelection::Initialize();
	}
}

// CppLogic.UI.WidgetGetAddress("StartMenu00_EndGame")
// CppLogic.UI.WidgetGetAddress("AutoAddSerfJobs")
// StartMenu00_VersionNumber
// StartMenu00_EndGame
// StartMenu00
// GoldTooltipController
// CppLogic.UI.ButtonOverrideActionFunc("StartMenu00_EndGame", function() LuaDebugger.Log(XGUIEng.GetCurrentWidgetID()) end)
// CppLogic.UI.ContainerWidgetCreateStaticWidgetChild("StartMenu00", "test", "StartMenu00_EndGame"); XGUIEng.SetMaterialTexture("test", 0, "data\\graphics\\textures\\gui\\hero_sel_dario.png"); CppLogic.UI.WidgetSetPositionAndSize("test", 783, 500, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateStaticTextWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetMaterialColor("test", 0, 0,0,0,0); XGUIEng.SetTextColor("test",255,255,255,255); CppLogic.UI.WidgetSetFont("test", "data\\menu\\fonts\\standardinvert10.met"); XGUIEng.SetText("test","tst"); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreatePureTooltipWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); CppLogic.UI.WidgetSetTooltipData("test", "StartMenu_TooltipText", true, true); CppLogic.UI.WidgetSetTooltipString("test", "tst"); CppLogic.UI.WidgetOverrideTooltipFunc("test", function() LuaDebugger.Log(1) end); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateGFXButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateTextButtonWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.SetText("test","tst"); CppLogic.UI.WidgetSetFont("test", "data\\menu\\fonts\\standardinvert10.met"); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateProgressBarWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 32, 32); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.ContainerWidgetCreateContainerWidgetChild("StartMenu00", "test"); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 100, 100); XGUIEng.ShowWidget("test", 1);
// CppLogic.UI.WidgetSetFont("StartMenu00_EndGame", "data\\menu\\fonts\\medium11bold.met") --"data\\menu\\fonts\\mainmenularge.met"
// CppLogic.UI.FontGetConfig("data\\menu\\fonts\\standard10.met")
// XGUIEng.SetText("StartMenu00_EndGame", "@center @color:0,255,0 test")
// CppLogic.UI.RemoveWidget("StartMenu00_VersionNumber")
// CppLogic.UI.SetGUIStateLuaSelection(function(x, y) LuaDebugger.Log(x.."/"..y) end)
// CppLogic.UI.ContainerWidgetCreateCustomWidgetChild("StartMenu00", "test", "EGUIX::CVideoPlaybackCustomWidget"); XGUIEng.ShowWidget("test", 1); CppLogic.UI.WidgetSetPositionAndSize("test", 0, 0, 250, 250); XGUIEng.StartVideoPlayback("test", "data\\graphics\\videos\\pu_farmer.bik", 1);

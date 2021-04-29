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
	luaext_assertPointer(L, fh, "widget ha no known update func");
	luaext_assert(L, fh->FuncRefCommand.L == L, "lua state doesnt match");
	lua_rawgeti(L, LUA_REGISTRYINDEX, fh->FuncRefCommand.Ref);
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
}

// CppLogic.UI.WidgetGetAddress("")
// StartMenu00_VersionNumber
// StartMenu00_EndGame

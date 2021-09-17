#pragma once
#include "s5data.h"
#include "luaext.h"

void l_ui_init(lua_State* L);
void l_ui_cleanup(lua_State* L);

struct CppL_GUIState_LuaSelection : shok_GGUI_CState {
	PADDINGI(2); // not sure how big the base object is;
	int RefOnKlick = LUA_NOREF;
	int RefOnCancel = LUA_NOREF;

	static void Initialize();
	CppL_GUIState_LuaSelection();
	~CppL_GUIState_LuaSelection();
	bool OnMouseEvent(shok_BB_CEvent* ev);
	void Cancel(bool calllua);

	// the usual vtable, typedec and identifier
	// vtable is used for manual class identification, typedesc for dynamic_cast and identifier for classfactory
	static const int vtp;
	static const int TypeDesc;
	static constexpr unsigned int Identifier = 0x1000;
};

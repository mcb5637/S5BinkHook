#include "pch.h"
#include "l_entity.h"
#include "pch.h"
#include "s5data.h"
#include "luaext.h"

int l_entity_getNum(lua_State* L) {
	lua_pushnumber(L, (*shok_EGL_CGLEEntityManagerObj)->EntityCount);
	return 1;
}

int l_entity_get(lua_State* L) {
	int i = luaL_checkint(L, 1);
	shok_EGL_CGLEEntity* p = shok_EGL_CGLEEntityManager_GetEntityByNum(*shok_EGL_CGLEEntityManagerObj, i);
	if (p == nullptr)
		lua_pushnumber(L, 0);
	else
		lua_pushnumber(L, p->EntityId);
	return 1;
}

void l_entity_init(lua_State* L)
{
	luaext_registerFunc(L, "GetNumberOfAllocatedEntities", &l_entity_getNum);
	luaext_registerFunc(L, "Get", &l_entity_get);
}

// CppLogic.Entity.GetNumberOfAllocatedEntities()
// CppLogic.Entity.Get(1)
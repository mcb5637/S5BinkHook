#pragma once
#include "s5data.h"
#include "luaext.h"

void l_ui_init(lua_State* L);
void l_ui_cleanup(lua_State* L);

namespace CppLogic::UI {
	class GUIState_LuaSelection : public GGUI::CState {
	public:
		PADDINGI(2); // not sure how big the base object is;
		int RefOnKlick = LUA_NOREF;
		int RefOnCancel = LUA_NOREF;

		virtual ~GUIState_LuaSelection() override;
		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(unsigned int id) override;
		virtual bool OnMouseEvent(BB::CEvent* ev) override;
		virtual void SetStateParameters(GGUI::SStateParameters* p) override;
		virtual bool Cancel() override;
		virtual const char* GetName() override;
		virtual int OnSelectionChanged(int z) override;

		static void Initialize();
		void Cancel(bool calllua);

		void* operator new(size_t s);
		void operator delete(void* p);
		static BB::IObject* __stdcall Create(); // todo: template when done with classfactory

		static constexpr unsigned int Identifier = 0x1000;
	};
}



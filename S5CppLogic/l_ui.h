#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_guistates.h"
#include "luaext.h"

namespace CppLogic::UI {
	void Init(lua::State L);
	void Cleanup(lua::State L);

	class GUIState_LuaSelection : public GGUI::CState {
	public:
		PADDINGI(2); // not sure how big the base object is;
		lua::Reference RefOnKlick = lua::State::NoRef;
		lua::Reference RefOnCancel = lua::State::NoRef;

		virtual ~GUIState_LuaSelection() override;
		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual bool OnMouseEvent(BB::CEvent* ev) override;
		virtual void SetStateParameters(GGUI::SStateParameters* p) override;
		virtual bool Cancel() override;
		virtual const char* GetName() override;
		virtual int OnSelectionChanged(int z) override;

		static void Initialize();
		void Cancel(bool calllua);

		void* operator new(size_t s);
		void operator delete(void* p);

		static constexpr unsigned int Identifier = 0x1000;
		static constexpr BB::SerializationData* SerializationData = nullptr;
	};
}

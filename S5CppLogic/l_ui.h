#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_guistates.h"
#include "luaext.h"

namespace CppLogic::UI {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnSaveLoaded(lua::State L);


	constexpr const char* CharTriggerRegKey = "CppLogic::UI::CharTrigger";
	constexpr const char* KeyTriggerRegKey = "CppLogic::UI::KeyTrigger";
	constexpr const char* MouseTriggerRegKey = "CppLogic::UI::MouseTrigger";

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

		static constexpr const char* Name = "LuaSelectionState";
		static constexpr int Id = 27;

		void* operator new(size_t s);
		void operator delete(void* p);

		static constexpr unsigned int Identifier = 0x1000;
		static constexpr BB::SerializationData* SerializationData = nullptr;
	};

	class GUIState_PlaceBuildingEx : public GGUI::CPlaceBuildingState {
	public:
		int CurrentStep = 0;

		static constexpr int NumSteps = 4;
		static constexpr float StepToDegrees = 360 / NumSteps;

		static void Initialize();

		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual const char* GetName() override;

		virtual bool OnMouseEvent(BB::CEvent* ev) override;
		virtual bool CheckCommandValid(TargetData* d, int z) override;
		virtual void ExecuteCommand(TargetData* d, ExecuteData* selectedID) override;
		virtual TargetData* GetTargetData(TargetData* d, int x, int y) override;
		virtual void OnMouseMove(int x, int y) override;

		void UpdateModel(int x, int y);
		float GetRotation();
		void SetRotation(float deg);

		static constexpr const char* Name = "PlaceBuildingStateEx";
		static constexpr int Id = 28;

		void* operator new(size_t s);
		void operator delete(void* p);

		static constexpr unsigned int Identifier = 0x1005;
		static constexpr BB::SerializationData* SerializationData = nullptr;
	};
}

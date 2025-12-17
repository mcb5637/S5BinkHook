#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/ui/s5_guistates.h>
#include <luaext.h>

namespace CppLogic::UI {
	void Init(lua::State L);
	void Cleanup(lua::State L);
	void OnSaveLoaded(lua::State L);


	constexpr std::string_view CharTriggerRegKey = "CppLogic::UI::CharTrigger";
	constexpr std::string_view KeyTriggerRegKey = "CppLogic::UI::KeyTrigger";
	constexpr std::string_view MouseTriggerRegKey = "CppLogic::UI::MouseTrigger";

	class GUIState_LuaSelection : public GGUI::CState {
	public:
		PADDINGI(2); // not sure how big the base object is;
		lua::Reference RefOnKlick = lua::State::NoRef;
		lua::Reference RefOnCancel = lua::State::NoRef;

		virtual ~GUIState_LuaSelection() override;
		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual bool OnMouseEvent(BB::CEvent* ev) override;
		virtual bool Cancel() override;
		virtual const char* GetName() override;

		static void Initialize();
		void Cancel(bool calllua);

		static constexpr const char* Name = "LuaSelectionState";
		static constexpr int Id = 28;

		void* operator new(size_t s);
		void operator delete(void* p);

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1000);
		static constexpr BB::SerializationData* SerializationData = nullptr;
	};

	class GUIState_PlaceBuildingEx : public GGUI::CPlaceBuildingState {
	public:
		int CurrentStep = 0;

		static constexpr int NumSteps = 4;
		static constexpr float StepToDegrees = 360.0f / NumSteps;

		static void Initialize();

		[[nodiscard]] virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual const char* GetName() override;

		virtual void SetStateParameters(GGUI::SStateParameters* p) override;
		virtual bool OnMouseEvent(BB::CEvent* ev) override;
		virtual bool CheckCommandValid(TargetData* d, int z) override;
		virtual void ExecuteCommand(TargetData* d, ExecuteData* selectedID) override;
		virtual TargetData* GetTargetData(TargetData* d, int x, int y) override;
		virtual void OnMouseMove(int x, int y) override;
		virtual bool OnCancel() override;

		void UpdateModel(int x, int y);
		float GetRotation() const;
		void SetRotation(float deg);
		void OnRotationChanged();

		static constexpr const char* Name = "PlaceBuildingStateEx";
		static constexpr int Id = 29;

		void* operator new(size_t s);
		void operator delete(void* p);

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1005);
		static constexpr BB::SerializationData* SerializationData = nullptr;
	};

	class TerrainDecalAccess {
		std::unique_ptr<ED::CTerrainDecalBase, CppLogic::DestroyCaller<ED::CTerrainDecalBase>> Decal;

		static int Destroy(lua::State L);
		static int SetPos(lua::State L);

		void Destroy();

		static std::set<TerrainDecalAccess*> Actives;

	public:
		TerrainDecalAccess(ED::CTerrainDecalBase* d);

		TerrainDecalAccess(const TerrainDecalAccess &) = delete;
		TerrainDecalAccess(TerrainDecalAccess&&) = delete;
		void operator=(const TerrainDecalAccess&) = delete;
		void operator=(TerrainDecalAccess&&) = delete;
		~TerrainDecalAccess();

		static constexpr const std::array LuaMethods {
				lua::FuncReference::GetRef<Destroy>("Destroy"),
				lua::FuncReference::GetRef<SetPos>("SetPos"),
			};

		static void Cleanup();
	};
}

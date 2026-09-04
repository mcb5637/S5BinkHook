#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <luaext.h>

namespace CppLogic::UI {
	void Init(luaext::State L);
	void Cleanup(luaext::State L);
	void OnSaveLoaded(luaext::State L);


	constexpr std::string_view CharTriggerRegKey = "CppLogic::UI::CharTrigger";
	constexpr std::string_view KeyTriggerRegKey = "CppLogic::UI::KeyTrigger";
	constexpr std::string_view MouseTriggerRegKey = "CppLogic::UI::MouseTrigger";

	class TerrainDecalAccess {
		std::unique_ptr<ED::CTerrainDecalBase, CppLogic::DestroyCaller<ED::CTerrainDecalBase>> Decal;

		static int SetPos(luaext::State L);

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
				luaext::FuncReference::GetUCRef<&TerrainDecalAccess::Destroy>("Destroy"),
				luaext::FuncReference::GetRef<SetPos>("SetPos"),
			};

		static void Cleanup();
	};
}

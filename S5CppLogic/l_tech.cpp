#include "pch.h"
#include "l_tech.h"
#include "luaext.h"

namespace CppLogic::Tech {
	int GetResearchInfo(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		L.Push(tech->TimeToResearch);
		L.PushCostInfo(tech->ResourceCosts);
		return 2;
	}
	int SetResearchInfo(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		if (L.IsNumber(2))
			tech->TimeToResearch = L.CheckFloat(2);
		bool ig = L.OptBool(4, false);
		if (L.IsTable(3))
			L.ReadCostInfo(3, tech->ResourceCosts, ig);
		return 0;
	}

	int GetRequirements(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		L.Push(tech->RequiredEntityConditions);
		L.NewTable();
		for (shok::Technology::ETypeReq ec : tech->EntityConditions) {
			L.Push(ec.EntityType);
			L.Push(ec.Amount);
			L.SetTableRaw(-3);
		}
		L.Push(tech->RequiredTecConditions);
		L.NewTable();
		int i = 1;
		for (shok::Technology::TechReq ec : tech->TecConditions) {
			L.Push(ec.TecType);
			L.SetTableRawI(-2, i);
			i++;
		}
		L.Push(tech->RequiredUpgradeCategoryConditions);
		L.NewTable();
		for (shok::Technology::UCatReq ec : tech->UpgradeCategoryConditions) {
			L.Push(ec.UpgradeCategory);
			L.Push(ec.Amount);
			L.SetTableRaw(-3);
		}
		return 6;
	}

	void inline TechPushMod(luaext::EState L, shok::Technology::Modifier& mod) {
		L.Push(mod.Operator.c_str()[0]);
		L.Push(mod.Value);
	}

	int GetExplorationModifier(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->ExplorationModifier);
		return 2;
	}
	int GetArmorModifier(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->ArmorModifier);
		return 2;
	}
	int GetDamageModifier(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->DamageModifier);
		return 2;
	}
	int GetRangeModifier(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->RangeModifier);
		return 2;
	}
	int GetSpeedModifier(lua::State ls) {
		luaext::EState L{ ls };
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->SpeedModifier);
		return 2;
	}

	int TechAddConstructionSpeedModifier(lua::State ls) {
		luaext::EState L{ ls };
		GGL::CBuilding::EnableConstructionSpeedTechs();
		shok::Technology* tech = L.CheckTech(1);
		char op = L.CheckString(3)[0];
		GGL::CBuilding::ConstructionSpeedModifiers.push_back({ L.CheckInt(1), L.CheckFloat(2), op });
		return 0;
	}

	constexpr std::array<lua::FuncReference, 9> Techs{ {
			lua::FuncReference::GetRef<GetResearchInfo>("GetResearchInfo"),
			lua::FuncReference::GetRef<SetResearchInfo>("SetResearchInfo"),
			lua::FuncReference::GetRef<GetRequirements>("GetRequirements"),
			lua::FuncReference::GetRef<GetExplorationModifier>("GetExplorationModifier"),
			lua::FuncReference::GetRef<GetArmorModifier>("GetArmorModifier"),
			lua::FuncReference::GetRef<GetDamageModifier>("GetDamageModifier"),
			lua::FuncReference::GetRef<GetRangeModifier>("GetRangeModifier"),
			lua::FuncReference::GetRef<GetSpeedModifier>("GetSpeedModifier"),
			lua::FuncReference::GetRef<TechAddConstructionSpeedModifier>("TechAddConstructionSpeedModifier"),
	} };

	void Cleanup(lua::State L) {
		GGL::CBuilding::ConstructionSpeedModifiers.clear();
	}

	void Init(lua::State L)
	{
		L.RegisterFuncs(Techs, -3);
	}
}
// CppLogic.Technology.TechAddConstructionSpeedModifier(Technologies.T_ScoutFindResources, 0.01, "*")

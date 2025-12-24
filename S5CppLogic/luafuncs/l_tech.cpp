#include "pch.h"
#include "l_tech.h"
#include <shok/s5_baseDefs.h>
#include <shok/tech/s5_tech.h>
#include <shok/entity/s5_entity.h>
#include <shok/globals/s5_maplogic.h>

namespace CppLogic::Tech {
	int GetResearchInfo(luaext::State L) {
		shok::Technology* tech = L.CheckTech(1);
		L.Push(tech->TimeToResearch);
		L.Push(tech->ResourceCosts);
		return 2;
	}
	int SetResearchInfo(luaext::State L) {
		shok::Technology* tech = L.CheckTech(1);
		if (L.IsNumber(2))
			tech->TimeToResearch = L.CheckFloat(2);
		bool ig = L.OptBool(4, false);
		if (L.IsTable(3))
			L.ReadCostInfo(3, tech->ResourceCosts, ig);
		return 0;
	}

	int GetRequirements(luaext::State L) {
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
			L.SetTableRaw(-2, i);
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

	void inline TechPushMod(luaext::State L, shok::Technology::Modifier& mod) {
		L.Push(mod.Operator.c_str()[0]);
		L.Push(mod.Value);
	}

	int GetExplorationModifier(luaext::State L) {
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->ExplorationModifier);
		return 2;
	}
	int GetArmorModifier(luaext::State L) {
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->ArmorModifier);
		return 2;
	}
	int GetDamageModifier(luaext::State L) {
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->DamageModifier);
		return 2;
	}
	int GetRangeModifier(luaext::State L) {
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->RangeModifier);
		return 2;
	}
	int GetSpeedModifier(luaext::State L) {
		shok::Technology* tech = L.CheckTech(1);
		TechPushMod(L, tech->SpeedModifier);
		return 2;
	}

	int TechAddConstructionSpeedModifier(luaext::State L) {
		GGL::CBuilding::EnableConstructionSpeedTechs();
		char op = L.CheckString(3)[0];
		GGL::CBuilding::ConstructionSpeedModifiers.push_back({ L.CheckEnum<shok::TechnologyId>(1), L.CheckFloat(2), op });
		return 0;
	}

	int ResearchTechnologyNoFeedback(luaext::State L) {
		auto i = L.CheckPlayerId(1, false);
		GGL::CPlayerStatus* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(i);
		L.CheckTech(2);
		pl->TechnologyStates.ForceResearchNoFeedback(L.CheckEnum<shok::TechnologyId>(2));
		return 0;
	}

	int GetResearchedTechs(luaext::State L) {
		auto pid = L.CheckPlayerId(1, false);
		GGL::CPlayerStatus* pl = (*GGL::CGLGameLogic::GlobalObj)->GetPlayer(pid);
		L.NewTable();
		int i = 1;
		for (const auto& r : pl->Statistics.ResearchedTechnologies) {
			L.NewTable();
			L.Push("Technology");
			L.Push(r.TechnologyType);
			L.SetTableRaw(-3);
			L.Push("Time");
			L.Push(r.Time);
			L.SetTableRaw(-3);
			L.SetTableRaw(-2, i);
			++i;
		}
		return 1;
	}

	constexpr std::array Techs{
			luaext::FuncReference::GetRef<GetResearchInfo>("GetResearchInfo"),
			luaext::FuncReference::GetRef<SetResearchInfo>("SetResearchInfo"),
			luaext::FuncReference::GetRef<GetRequirements>("GetRequirements"),
			luaext::FuncReference::GetRef<GetExplorationModifier>("GetExplorationModifier"),
			luaext::FuncReference::GetRef<GetArmorModifier>("GetArmorModifier"),
			luaext::FuncReference::GetRef<GetDamageModifier>("GetDamageModifier"),
			luaext::FuncReference::GetRef<GetRangeModifier>("GetRangeModifier"),
			luaext::FuncReference::GetRef<GetSpeedModifier>("GetSpeedModifier"),
			luaext::FuncReference::GetRef<TechAddConstructionSpeedModifier>("TechAddConstructionSpeedModifier"),
			luaext::FuncReference::GetRef<ResearchTechnologyNoFeedback>("ResearchTechnologyNoFeedback"),
			luaext::FuncReference::GetRef<GetResearchedTechs>("GetResearchedTechs"),
	};

	void Cleanup(luaext::State L) {
		GGL::CBuilding::ConstructionSpeedModifiers.clear();
	}

	void Init(luaext::State L)
	{
		L.RegisterFuncs(Techs, -3);
	}
}
// CppLogic.Technology.TechAddConstructionSpeedModifier(Technologies.T_ScoutFindResources, 0.01, "*")

#pragma once
#include <map>
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>

namespace CppLogic::SavegameExtra {
	class SerializedMapdata {
		static constexpr std::string_view SaveGameFile = "\\CppLogic.xml";
	public:

		std::string SavegameName;
		bool HurtEntityCallWithNoAttacker = false;
		bool UseMaxHPTechBoni = false;
		bool PaydayTrigger = false;
		bool LeaderRegenRegenerateSoldiers = false;
		bool ChangeTaskListCheckUncancelable = false;
		bool BuildOnMovementFix = false;
		bool ExperienceClassFix = false;
		bool AoEDamageFix = false;
		bool CamoFix = false;
		bool ConversionTrigger = false;
		bool HookMaxHP = false;
		bool HookDamage = false;
		bool HookArmor = false;
		bool HookExploration = false;
		bool HookRegen = false;
		bool HookMaxRange = false;
		bool HookDisplayName = false;
		bool RangedEffectSoldierHeal = false;
		bool EffectTriggers = false;
		bool ResourceTriggers = false;
		bool RefinerFix = false;
		bool SettlerBuyTriggers = false;
		bool ScriptTriggerEval = false;
		bool CannonInProgressAttraction = false;
		bool LightningEffectFix = false;
		shok::EntityCategory ResDoodad_RefillableCategory = {};

		std::map<std::string, std::string, CppLogic::CaselessStringComparator> StringTableTextOverride;
		static std::map<std::string, std::string, CppLogic::CaselessStringComparator> StringTableTextOverrideMainmenu;

		void SerializeTo(const char* path, const char* savename);
		void DeserializeFrom(const char* path, const char* savename);
		void Clear();

		static void STTToMainmenu();
		static void STTToIngame();
		static std::map<std::string, std::string, CppLogic::CaselessStringComparator>& GetActiveOverrides(lua_State* s);
		static void STTHasChanged(lua_State* s);

		static SerializedMapdata GlobalObj;
		static BB::SerializationData SerializationData[];
	};

	struct StringTableTextOverride {

		std::map<std::string, std::string, CppLogic::CaselessStringComparator> StringTableTextOverrideData;

		void Merge(std::string_view prefix, lua_State* s) const;

		static BB::SerializationData SerializationData[];
	};
}

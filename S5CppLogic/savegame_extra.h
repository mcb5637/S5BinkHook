#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_entity.h"

namespace CppLogic::SavegameExtra {
	class SerializedMapdata {
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
		std::map<std::string, std::string, CppLogic::CaselessStringComparator> StringTableTextOverride;

		void SerializeTo(const char* path, const char* savename);
		void DeserializeFrom(const char* path, const char* savename);
		void Clear();

		static SerializedMapdata GlobalObj;
		static BB::SerializationData SerializationData[];
	};
}

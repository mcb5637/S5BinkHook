#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"

namespace CppLogic::SavegameExtra {
	class SerializedMapdata {
	public:
		std::string SavegameName;
		bool HurtEntityCallWithNoAttacker = false;
		bool UseMaxHPTechBoni = false;
		bool PaydayTrigger = false;
		bool LeaderRegenRegenerateSoldiers = false;
		std::map<const std::string, std::string> StringTableTextOverride;

		void SerializeTo(const char* path, const char* savename);
		void DeserializeFrom(const char* path, const char* savename);

		static SerializedMapdata GlobalObj;
		static BB::SerializationData SerializationData[];

	private:
		void Clear();
	};
}

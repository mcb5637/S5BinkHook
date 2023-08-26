#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_behaviors.h"
#include "s5_behaviorProps.h"
#include "s5_config.h"

#include "mod.h"

namespace CppLogic::Mod::Config {
	void RegisterClasses();

	class DamageClassExt : public GGL::CDamageClassProps {
	public:
		std::map<shok::ArmorClassId, float> ExtraArmorClasses;

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x100D);
		static const BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);
	};
}

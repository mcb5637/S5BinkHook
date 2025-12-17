#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/entity/s5_behaviors.h>
#include <shok/globals/s5_config.h>

#include <utility/mod.h>

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

#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_behaviors.h"
#include "s5_behaviorProps.h"

#include "mod.h"

namespace CppLogic::Mod {
	void RegisterClasses();

	class FormationSpacedBehaviorProps : public GGL::CFormationBehaviorProperties {
	public:
		float SpaceFactor = 0;

		virtual unsigned int __stdcall GetClassIdentifier() const override;

		static inline constexpr unsigned int Identifier = 0x1009;
		static const BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	class FormationSpacedBehavior : public GGL::CFormationBehavior, public IFormationBehaviorExtProvider {
	public:

		virtual unsigned int __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(unsigned int id) override;

		virtual shok::Position GetPosExt(EGL::CGLEEntity* leader) override;

		static inline constexpr unsigned int Identifier = 0x1010;
		static const BB::SerializationData SerializationData[];

		FormationSpacedBehavior();

		void* operator new(size_t s);
		void operator delete(void* p);
	};
}

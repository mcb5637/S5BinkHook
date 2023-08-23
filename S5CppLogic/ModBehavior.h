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

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1009);
		static const BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);
	};
	class FormationSpacedBehavior : public GGL::CFormationBehavior, public IFormationBehaviorExtProvider {
	public:

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void* __stdcall CastToIdentifier(shok::ClassId id) override;

		virtual shok::Position GetPosExt(EGL::CGLEEntity* leader) override;

		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1010);
		static const BB::SerializationData SerializationData[];

		FormationSpacedBehavior();

		void* operator new(size_t s);
		void operator delete(void* p);
	};

	class ResourceTrackerBehavior : public EGL::CGLEBehavior {
	public:
		shok::CostInfo Produced, Used;

		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void __thiscall AddHandlers(shok::EntityId id) override;
		virtual void __thiscall OnEntityCreate(EGL::CGLEBehaviorProps* p) override;
		virtual void __thiscall OnEntityLoad(EGL::CGLEBehaviorProps* p) override;
		virtual void __thiscall OnEntityUpgrade(EGL::CGLEEntity* old) override;


		static constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x100B);
		static const BB::SerializationData SerializationData[];

		void* operator new(size_t s);
		void operator delete(void* p);

	private:
		void EventMinedOrRefined(GGL::CEventGoodsTraded* ev);
		void EventSupplied(GGL::CEventGoodsTraded* ev);
	};
}

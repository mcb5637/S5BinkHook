#pragma once
#include "s5_forwardDecls.h"
#include "s5_baseDefs.h"
#include "s5_effects.h"

#include "mod.h"
#include "s5_staticlist.h"
#include "s5_netevents.h"


namespace CppLogic::Mod::Effect {
	void RegisterClasses();


	class EntityPlacerEffect : public EGL::CFlyingEffect {
	public:
		shok::EntityId Attacker = shok::EntityId::Invalid;
		shok::EntityTypeId ToCreate = shok::EntityTypeId::Invalid;
		shok::PlayerId AttackerPlayer = shok::PlayerId::P0;
		shok::AttachmentType AttachCreated = shok::AttachmentType::INVALID;
		shok::EventIDs OnDetachAttacker = shok::EventIDs::NoDetachEvent, OnDetachCreated = shok::EventIDs::NoDetachEvent;


		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void FromCreator(EGL::CGLEEffectCreator* ct) override;
		virtual void OnLoaded() override;
		virtual void OnHit() override;

		void* operator new(size_t s);
		void operator delete(void* p);


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x101D);
		static BB::SerializationData SerializationData[];
	};
}

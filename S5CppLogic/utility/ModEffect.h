#pragma once
#include <shok/s5_forwardDecls.h>
#include <shok/s5_baseDefs.h>
#include <shok/effect/s5_effects.h>

#include <utility/mod.h>
#include <shok/globals/s5_staticlist.h>
#include <shok/events/s5_netevents.h>


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


	class PiercingShotEffect : public EGL::CFlyingEffect {
	public:
		shok::EntityId AttackerID = shok::EntityId::Invalid;
		shok::PlayerId SourcePlayer = shok::PlayerId::Invalid;
		int DamageAmount = 0;
		float AoERange = -1;
		shok::DamageClassId DamageClass = shok::DamageClassId::Invalid;
		shok::AdvancedDealDamageSource AdvancedDamageSourceOverride = shok::AdvancedDealDamageSource::Cannonball;
		std::vector<shok::EntityId> AlreadyAffectedEntities;


		virtual shok::ClassId __stdcall GetClassIdentifier() const override;
		virtual void FromCreator(EGL::CGLEEffectCreator* ct) override;
		virtual void OnLoaded() override;
		virtual void OnTick() override;

		void* operator new(size_t s);
		void operator delete(void* p);


		static inline constexpr shok::ClassId Identifier = static_cast<shok::ClassId>(0x1020);
		static BB::SerializationData SerializationData[];

	private:
		void CheckForDamage();
	};
}

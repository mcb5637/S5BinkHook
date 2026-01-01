#include "ModEffect.h"
#include <shok/globals/s5_classfactory.h>
#include <utility/entityiterator.h>

void CppLogic::Mod::Effect::RegisterClasses()
{
	BB::CClassFactory* f = *BB::CClassFactory::GlobalObj;
	f->AddClassToFactory<EntityPlacerEffect>();
	f->AddClassToFactory<PiercingShotEffect>();
}

shok::ClassId __stdcall CppLogic::Mod::Effect::EntityPlacerEffect::GetClassIdentifier() const
{
    return Identifier;
}

void* CppLogic::Mod::Effect::EntityPlacerEffect::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::Effect::EntityPlacerEffect::operator delete(void* p) {
	shok::Free(p);
}

BB::SerializationData CppLogic::Mod::Effect::EntityPlacerEffect::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<EntityPlacerEffect, EGL::CFlyingEffect>(),
	AutoMemberSerialization(EntityPlacerEffect, Attacker),
	AutoMemberSerialization(EntityPlacerEffect, ToCreate),
	AutoMemberSerialization(EntityPlacerEffect, AttackerPlayer),
	AutoMemberSerialization(EntityPlacerEffect, AttachCreated),
	AutoMemberSerialization(EntityPlacerEffect, OnDetachAttacker),
	AutoMemberSerialization(EntityPlacerEffect, OnDetachCreated),
	BB::SerializationData::GuardData(),
};

void CppLogic::Mod::Effect::EntityPlacerEffect::FromCreator(EGL::CGLEEffectCreator* ct)
{
	EGL::CFlyingEffect::FromCreator(ct);

	if (auto* c = BB::IdentifierCast<CProjectileEffectCreator>(ct)) {
		Attacker = c->AttackerID;
		AttackerPlayer = c->SourcePlayer;
	}
}

void CppLogic::Mod::Effect::EntityPlacerEffect::OnLoaded()
{
	EGL::CFlyingEffect::FixOnLoad();
}

void CppLogic::Mod::Effect::EntityPlacerEffect::OnHit()
{
	
	if (ToCreate != shok::EntityTypeId::Invalid)
	{
		EGL::CGLEEntityCreator cr{};
		cr.Pos = TargetPosition;
		cr.EntityType = ToCreate;
		cr.PlayerId = AttackerPlayer;
		auto eid = (*EGL::CGLEGameLogic::GlobalObj)->CreateEntity(&cr);

		if (EGL::CGLEEntity* a = EGL::CGLEEntity::GetEntityByID(Attacker)) {
			if (AttachCreated != shok::AttachmentType::INVALID) {
				a->AttachEntity(AttachCreated, eid, OnDetachAttacker, OnDetachCreated);
			}
		}
	}

	EGL::CFlyingEffect::OnHit();
}

shok::ClassId __stdcall CppLogic::Mod::Effect::PiercingShotEffect::GetClassIdentifier() const
{
	return Identifier;
}

void* CppLogic::Mod::Effect::PiercingShotEffect::operator new(size_t s)
{
	return shok::Malloc(s);
}
void CppLogic::Mod::Effect::PiercingShotEffect::operator delete(void* p) {
	shok::Free(p);
}

CreateSerializationListFor(CppLogic::Mod::Effect::PiercingShotEffect, AlreadyAffectedEntities);
BB::SerializationData CppLogic::Mod::Effect::PiercingShotEffect::SerializationData[] = {
	BB::SerializationData::AutoBaseClass<PiercingShotEffect, EGL::CFlyingEffect>(),
	AutoMemberSerialization(PiercingShotEffect, AttackerID),
	AutoMemberSerialization(PiercingShotEffect, SourcePlayer),
	AutoMemberSerialization(PiercingShotEffect, DamageAmount),
	AutoMemberSerialization(PiercingShotEffect, AoERange),
	AutoMemberSerialization(PiercingShotEffect, DamageClass),
	AutoMemberSerialization(PiercingShotEffect, AdvancedDamageSourceOverride),
	AutoMemberSerialization(PiercingShotEffect, AlreadyAffectedEntities),
	BB::SerializationData::GuardData(),
};

void CppLogic::Mod::Effect::PiercingShotEffect::FromCreator(EGL::CGLEEffectCreator* ct)
{
	EGL::CFlyingEffect::FromCreator(ct);

	if (auto* c = BB::IdentifierCast<CProjectileEffectCreator>(ct)) {
		AttackerID = c->AttackerID;
		SourcePlayer = c->SourcePlayer;
		DamageAmount = c->Damage;
		AoERange = c->DamageRadius;
		DamageClass = c->DamageClass;
		AdvancedDamageSourceOverride = c->AdvancedDamageSourceOverride;
	}
}

void CppLogic::Mod::Effect::PiercingShotEffect::OnLoaded()
{
	EGL::CFlyingEffect::FixOnLoad();
}

void CppLogic::Mod::Effect::PiercingShotEffect::OnTick()
{
	FlyingEffectSlot.LastPosition = FlyingEffectSlot.Position;
	bool hit = OnTickMove();
	CheckForDamage();
	if (hit)
		OnHit();
}

void CppLogic::Mod::Effect::PiercingShotEffect::CheckForDamage()
{
	if ((*GGL::CGLGameLogic::GlobalObj)->GlobalInvulnerability)
		return;
	if (AoERange <= 0 || DamageAmount <= 0)
		return;
	EGL::CGLEEntity* attacker = EGL::CGLEEntity::GetEntityByID(AttackerID);

	auto pl = attacker ? attacker->PlayerId : SourcePlayer;
	CppLogic::Iterator::EntityPredicateIsCombatRelevant irel{};
	CppLogic::Rect rect{ FlyingEffectSlot.Position, FlyingEffectSlot.LastPosition, AoERange };
	CppLogic::Iterator::PredicateInArbitrayRect<EGL::CGLEEntity> icircl{ rect };
	CppLogic::Iterator::EntityPredicateIsAlive iali{};

	auto lam = [this, attacker, pl](EGL::CGLEEntity* curr) {
			if (std::find(AlreadyAffectedEntities.begin(), AlreadyAffectedEntities.end(), curr->EntityId) != AlreadyAffectedEntities.end())
				return;

			float dmg = curr->CalculateDamageAgainstMe(DamageAmount, DamageClass, 1.0f);

			if (dmg < 1)
				dmg = 1;

			curr->AdvancedHurtEntityBy(attacker, static_cast<int>(dmg), pl, true, true, true, this->AdvancedDamageSourceOverride);
			AlreadyAffectedEntities.push_back(curr->EntityId);
		};

	static constexpr auto acflags = shok::AccessCategoryFlags::AccessCategorySettler
		| shok::AccessCategoryFlags::AccessCategoryAnimal | shok::AccessCategoryFlags::AccessCategoryBuilding
		| shok::AccessCategoryFlags::AccessCategoryResourceDoodad | shok::AccessCategoryFlags::AccessCategoryStatic
		| shok::AccessCategoryFlags::AccessCategoryOrnamental;
	if (pl != shok::PlayerId::P0) {
		CppLogic::Iterator::EntityPredicateOfAnyPlayer ipl{};
		CppLogic::Iterator::EntityPredicateOfAnyPlayer::FillHostilePlayers(ipl.players, pl);
		CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 4> p{ &irel, &iali, &ipl, &icircl };
		CppLogic::Iterator::MultiRegionEntityIterator it{ rect.BoundingBox(), acflags, &p};
		for (auto* ei : it) {
			lam(ei);
		}
	}
	else {
		CppLogic::Iterator::PredicateStaticAnd<EGL::CGLEEntity, 3> p{ &irel, &icircl, &iali };
		CppLogic::Iterator::MultiRegionEntityIterator it{ rect.BoundingBox(), acflags, &p };
		for (auto* ei : it) {
			lam(ei);
		}
	}
}

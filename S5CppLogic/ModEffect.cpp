#include "pch.h"
#include "ModEffect.h"
#include "s5_classfactory.h"

void CppLogic::Mod::Effect::RegisterClasses()
{
	BB::CClassFactory* f = *BB::CClassFactory::GlobalObj;
	f->AddClassToFactory<EntityPlacerEffect>();
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

void CppLogic::Mod::Effect::EntityPlacerEffect::OnHit()
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

	EGL::CFlyingEffect::OnHit();
}

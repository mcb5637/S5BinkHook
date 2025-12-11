#include "pch.h"
#include "EntityAddonData.h"
#include <shok/globals/s5_classfactory.h>
#include <shok/entity/s5_entity.h>

void CppLogic::EntityAddon::EntityAddonData::operator=(const EntityAddonData& other)
{
	EntityId = other.EntityId;
	HealthOverride = other.HealthOverride;
	HealthUseBoni = other.HealthUseBoni;
	DamageOverride = other.DamageOverride;
	ArmorOverride = other.ArmorOverride;
	ExplorationOverride = other.ExplorationOverride;
	RegenHPOverride = other.RegenHPOverride;
	RegenSecondsOverride = other.RegenSecondsOverride;
	MaxRangeOverride = other.MaxRangeOverride;
	NameOverride = other.NameOverride;
	FakeTaskValue = other.FakeTaskValue;
}

shok::ClassId __stdcall CppLogic::EntityAddon::EntityAddonData::GetClassIdentifier() const
{
	return Identifier;
}

void CppLogic::EntityAddon::EntityAddonData::AddHandlers(shok::EntityId id)
{
	EntityId = id;
}

void CppLogic::EntityAddon::EntityAddonData::OnEntityCreate(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void CppLogic::EntityAddon::EntityAddonData::OnEntityLoad(EGL::CGLEBehaviorProps* p)
{
	PropPointer = p;
}

void CppLogic::EntityAddon::EntityAddonData::OnEntityDestroy(bool ev)
{
	EGL::CGLEEntity::LastRemovedEntityAddonData = *this;
}

void* CppLogic::EntityAddon::EntityAddonData::operator new(size_t s)
{
	return shok::Malloc(s);
}

void CppLogic::EntityAddon::EntityAddonData::operator delete(void* p)
{
	shok::Free(p);
}

void CppLogic::EntityAddon::EntityAddonData::Init()
{
	(*BB::CClassFactory::GlobalObj)->AddClassToFactory<EntityAddonData>();
}

const BB::SerializationData CppLogic::EntityAddon::EntityAddonData::SerializationData[]{
	BB::SerializationData::AutoBaseClass<EntityAddonData, EGL::CGLEBehavior>(),
	AutoMemberSerialization(EntityAddonData, HealthOverride),
	AutoMemberSerialization(EntityAddonData, HealthUseBoni),
	AutoMemberSerialization(EntityAddonData, DamageOverride),
	AutoMemberSerialization(EntityAddonData, ArmorOverride),
	AutoMemberSerialization(EntityAddonData, ExplorationOverride),
	AutoMemberSerialization(EntityAddonData, RegenHPOverride),
	AutoMemberSerialization(EntityAddonData, RegenSecondsOverride),
	AutoMemberSerialization(EntityAddonData, MaxRangeOverride),
	AutoMemberSerialization(EntityAddonData, NameOverride),
	AutoMemberSerialization(EntityAddonData, FakeTaskValue),
	BB::SerializationData::GuardData(),
};

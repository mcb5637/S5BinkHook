#include "pch.h"
#include "EntityAddonData.h"
#include "s5_classfactory.h"
#include "s5_entity.h"

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

BB::SerializationData CppLogic::EntityAddon::EntityAddonData::SerializationData[]{
	BB::SerializationData::FieldData("HealthOverride", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, HealthOverride)),
	BB::SerializationData::FieldData("HealthUseBoni", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, HealthUseBoni)),
	BB::SerializationData::FieldData("DamageOverride", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, DamageOverride)),
	BB::SerializationData::FieldData("ArmorOverride", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, ArmorOverride)),
	BB::SerializationData::FieldData("ExplorationOverride", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, ExplorationOverride)),
	BB::SerializationData::FieldData("RegenHPOverride", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, RegenHPOverride)),
	BB::SerializationData::FieldData("RegenSecondsOverride", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, RegenSecondsOverride)),
	BB::SerializationData::FieldData("MaxRangeOverride", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, MaxRangeOverride)),
	BB::SerializationData::FieldData("NameOverride", MemberSerializationSizeAndOffset(CppLogic::EntityAddon::EntityAddonData, NameOverride), &CppLogic::StringSerializer::GlobalObj),
	BB::SerializationData::FieldData("FakeTaskValue", MemberSerializationFieldData(CppLogic::EntityAddon::EntityAddonData, FakeTaskValue)),
	BB::SerializationData::GuardData(),
};

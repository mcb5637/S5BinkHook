#include "pch.h"
#include "s5_entitytype.h"
#include <shok/globals/s5_framework.h>

static inline void(__thiscall* const ety_initblock)(EGL::CGLEEntityProps* th) = reinterpret_cast<void(__thiscall*)(EGL::CGLEEntityProps*)>(0x58466C);
void EGL::CGLEEntityProps::InitializeBlocking() {
	ety_initblock(this);
}

static inline float(__thiscall* const modeprops_mod)(EGL::CGLEEntityProps::ModifyEntityProps* th, int p, float i) = reinterpret_cast<float(__thiscall*)(EGL::CGLEEntityProps::ModifyEntityProps*, int, float)>(0x4C797D);
float EGL::CGLEEntityProps::ModifyEntityProps::ModifyValue(int player, float initial) {
	return modeprops_mod(this, player, initial);
}

GGlue::CGlueEntityProps::CGlueEntityProps() {
	*reinterpret_cast<int*>(this) = vtp;
	LogicProps = nullptr;
	DisplayProps = nullptr;
}
GGlue::CGlueEntityProps::CGlueEntityProps(const CGlueEntityProps& o) {
	*reinterpret_cast<int*>(this) = vtp;
	LogicProps = o.LogicProps;
	DisplayProps = o.DisplayProps;
	BehaviorProps = o.BehaviorProps;
}
GGlue::CGlueEntityProps::CGlueEntityProps(CGlueEntityProps&& o) noexcept {
	*reinterpret_cast<int*>(this) = vtp;
	LogicProps = o.LogicProps;
	DisplayProps = o.DisplayProps;
	BehaviorProps = std::move(o.BehaviorProps);
}

bool GGlue::CGlueEntityProps::IsSettlerType() const
{
	return dynamic_cast<GGL::CGLSettlerProps*>(LogicProps);
}

bool GGlue::CGlueEntityProps::IsBuildingType() const
{
	return dynamic_cast<GGL::CGLBuildingProps*>(LogicProps);
}

bool GGlue::CGlueEntityProps::IsCEntityProperties() const
{
	return dynamic_cast<GGL::CEntityProperties*>(LogicProps);
}

bool GGlue::CGlueEntityProps::IsOfCategory(shok::EntityCategory cat) const
{
	for (shok::EntityCategory i : LogicProps->Categories)
		if (i == cat)
			return true;
	return false;
}

shok::ClassId __stdcall GGlue::CGlueEntityProps::GetClassIdentifier() const {
	return Identifier;
}

GGlue::CGlueEntityProps* CppLogic::GetEntityType(shok::EntityTypeId id)
{
	int i = static_cast<int>(id);
	auto* m = (*Framework::CMain::GlobalObj)->GluePropsManager->EntitiesPropsManager;
	if (i > 0 && i <= static_cast<int>(m->EntityTypes.size()))
		return &m->EntityTypes[i];
	return nullptr;
}

const char* (__stdcall* const getentitydisplayname)(shok::EntityTypeId i) = reinterpret_cast<const char* (__stdcall* const)(shok::EntityTypeId i)>(0x52EFCF);
const char* EGL::CGLEEntitiesProps::GetEntityTypeDisplayName(shok::EntityTypeId i)
{
	return getentitydisplayname(i);
}


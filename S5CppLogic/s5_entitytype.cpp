#include "pch.h"
#include "s5_entitytype.h"

GGlue::CGlueEntityProps::CGlueEntityProps() {
	*reinterpret_cast<int*>(this) = vtp;
	LogicProps = nullptr;
	DisplayProps = nullptr;
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

unsigned int __stdcall GGlue::CGlueEntityProps::GetClassIdentifier() const {
	return Identifier;
}

GGlue::CGlueEntityProps* EGL::CGLEEntitiesProps::GetEntityType(int i)
{
	if (i <= 0 || i >= static_cast<int>(EntityTypes.size()))
		return nullptr;
	return EntityTypes.data() + i;
}

const char* (__stdcall* const getentitydisplayname)(int i) = reinterpret_cast<const char* (__stdcall* const)(int i)>(0x52EFCF);
const char* EGL::CGLEEntitiesProps::GetEntityTypeDisplayName(int i)
{
	return getentitydisplayname(i);
}


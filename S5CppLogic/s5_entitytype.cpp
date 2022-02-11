#include "pch.h"
#include "s5data.h"

bool GGlue::CGlueEntityProps::IsSettlerType() const
{
	return shok_DynamicCast<EGL::CGLEEntityProps, GGL::CGLSettlerProps>(LogicProps);
}

bool GGlue::CGlueEntityProps::IsBuildingType() const
{
	return shok_DynamicCast<EGL::CGLEEntityProps, GGL::CGLBuildingProps>(LogicProps);
}

bool GGlue::CGlueEntityProps::IsCEntityProperties() const
{
	return shok_DynamicCast<EGL::CGLEEntityProps, GGL::CEntityProperties>(LogicProps);
}

bool GGlue::CGlueEntityProps::IsOfCategory(shok::EntityCategory cat) const
{
	for (shok::EntityCategory i : LogicProps->Categories)
		if (i == cat)
			return true;
	return false;
}

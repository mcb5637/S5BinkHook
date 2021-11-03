#include "pch.h"
#include "s5data.h"

bool shok_GGlue_CGlueEntityProps::IsSettlerType() const
{
	return shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLSettlerProps>(LogicProps);
}

bool shok_GGlue_CGlueEntityProps::IsBuildingType() const
{
	return shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CGLBuildingProps>(LogicProps);
}

bool shok_GGlue_CGlueEntityProps::IsCEntityProperties() const
{
	return shok_DynamicCast<shok_EGL_CGLEEntityProps, shok_GGL_CEntityProperties>(LogicProps);
}

bool shok_GGlue_CGlueEntityProps::IsOfCategory(shok_EntityCategory cat) const
{
	for (shok_EntityCategory i : LogicProps->Categories)
		if (i == cat)
			return true;
	return false;
}

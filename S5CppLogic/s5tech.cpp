#include "pch.h"
#include "s5data.h"

float shok_technologyModifier::ModifyValue(float i)
{
	switch (Operator) {
	case '+':
		i += Value;
		break;
	case '-':
		i -= Value;
		break;
	case '*':
		i *= Value;
		break;
	case '/':
		i /= Value;
		break;
	}
    return i;
}

float additionalTechModifier::ModifyValue(float i)
{
	switch (Operator) {
	case '+':
		i += Value;
		break;
	case '-':
		i -= Value;
		break;
	case '*':
		i *= Value;
		break;
	case '/':
		i /= Value;
		break;
	}
	return i;
}

std::vector<additionalTechModifier> ConstructionSpeedModifiers = std::vector<additionalTechModifier>();
float __fastcall constructionsite_getprogresspertick_hook(shok_GGL_CBuilding* th) { // param is constructionsite, just not done yet ;)
	shok_GGL_CGLSettlerProps* serf = (shok_GGL_CGLSettlerProps*)(*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(*shok_entityTypeIDSerf)->LogicProps;
	shok_GGL_CGLBuildingProps* bty = (shok_GGL_CGLBuildingProps*)(*shok_EGL_CGLEEntitiesPropsObj)->GetEntityType(th->ConstructionSiteType)->LogicProps;
	float constructionfactor = serf->BuildFactor;
	for (additionalTechModifier& tmod : ConstructionSpeedModifiers) {
		if ((*shok_GGL_CGLGameLogicObj)->GetPlayer(th->PlayerId)->GetTechStatus(tmod.TechID) != TechState::Researched)
			continue;
		constructionfactor = tmod.ModifyValue(constructionfactor);
	}
	constructionfactor = constructionfactor * bty->ConstructionInfo.Time * 10;
	if (constructionfactor <= 0.0f)
		return 0.0f;
	else
		return 1.0f / constructionfactor;
}
void EnableConstructionSpeedTechs()
{
	WriteJump((void*)0x4B8EAD, &constructionsite_getprogresspertick_hook);
}

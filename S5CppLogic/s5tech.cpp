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
float __fastcall constructionsite_getprogresspertick_hook(GGL::CBuilding* th) { // param is constructionsite, just not done yet ;)
	GGL::CGLSettlerProps* serf = (GGL::CGLSettlerProps*)(*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(*GGlue::CGlueEntityProps::EntityTypeIDSerf)->LogicProps;
	GGL::CGLBuildingProps* bty = (GGL::CGLBuildingProps*)(*shok_EGL_CGLEEntitiesProps::GlobalObj)->GetEntityType(th->ConstructionSiteType)->LogicProps;
	float constructionfactor = serf->BuildFactor;
	for (additionalTechModifier& tmod : ConstructionSpeedModifiers) {
		if ((*shok_GGL_CGLGameLogic::GlobalObj)->GetPlayer(th->PlayerId)->GetTechStatus(tmod.TechID) != shok::TechState::Researched)
			continue;
		constructionfactor = tmod.ModifyValue(constructionfactor);
	}
	constructionfactor = constructionfactor * bty->ConstructionInfo.Time * 10;
	if (constructionfactor <= 0.0f)
		return 0.0f;
	else
		return 1.0f / constructionfactor;
}
bool EnableConstructionSpeedTechs_Hooked = false;
void EnableConstructionSpeedTechs()
{
	if (EnableConstructionSpeedTechs_Hooked)
		return;
	EnableConstructionSpeedTechs_Hooked = true;
	shok::SaveVirtualProtect vp{ reinterpret_cast<void*>(0x4B8EAD), 10 };
	WriteJump(reinterpret_cast<void*>(0x4B8EAD), &constructionsite_getprogresspertick_hook);
}

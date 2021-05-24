#include "pch.h"
#include "s5data.h"


shok_EGL_CGLEBehaviorProps* shok_GGlue_CGlueEntityProps::SearchBehaviorProp(int* vts, int num)
{
	for (shok_EGL_CGLEBehaviorProps* b : BehaviorProps) {
		if (b != nullptr && contains(vts, b->vtable, num))
			return b;
	}
	return nullptr;
}

shok_EGL_CGLEBehaviorProps* shok_GGlue_CGlueEntityProps::SearchBehaviorProp(int vt)
{
	return SearchBehaviorProp(&vt, 1);
}



shok_GGL_CLeaderBehaviorProps* shok_GGlue_CGlueEntityProps::GetLeaderBehaviorProps()
{
	int data[2] = { shok_GGL_CLeaderBehaviorProps::vtp, shok_GGL_CBattleSerfBehaviorProps::vtp };
	return (shok_GGL_CLeaderBehaviorProps*)SearchBehaviorProp(data, 2);
}

shok_GGL_CSoldierBehaviorProps* shok_GGlue_CGlueEntityProps::GetSoldierBehaviorProps()
{
	return (shok_GGL_CSoldierBehaviorProps*)SearchBehaviorProp(shok_GGL_CSoldierBehaviorProps::vtp);
}

shok_GGL_CBattleBehaviorProps* shok_GGlue_CGlueEntityProps::GetBattleBehaviorProps()
{
	int data[5] = { shok_GGL_CBattleBehaviorProps::vtp, shok_GGL_CLeaderBehaviorProps::vtp, shok_GGL_CSoldierBehaviorProps::vtp, shok_GGL_CSerfBattleBehaviorProps::vtp, shok_GGL_CBattleSerfBehaviorProps::vtp };
	return (shok_GGL_CBattleBehaviorProps*)SearchBehaviorProp(data, 5);
}

shok_GGL_CLimitedLifespanBehaviorProps* shok_GGlue_CGlueEntityProps::GetLimitedLifespanBehaviorProps()
{
	return (shok_GGL_CLimitedLifespanBehaviorProps*)SearchBehaviorProp(shok_GGL_CLimitedLifespanBehaviorProps::vtp);
}

shok_GGL_CAffectMotivationBehaviorProps* shok_GGlue_CGlueEntityProps::GetAffectMotivationBehaviorProps()
{
	return (shok_GGL_CAffectMotivationBehaviorProps*)SearchBehaviorProp(shok_GGL_CAffectMotivationBehaviorProps::vtp);
}

shok_GGL_CGLAnimationBehaviorExProps* shok_GGlue_CGlueEntityProps::GetAnimationExProps()
{
	return (shok_GGL_CGLAnimationBehaviorExProps*)SearchBehaviorProp(shok_GGL_CGLAnimationBehaviorExProps::vtp);
}

shok_GGL_CAutoCannonBehaviorProps* shok_GGlue_CGlueEntityProps::GetAutoCannonProps()
{
	return (shok_GGL_CAutoCannonBehaviorProps*)SearchBehaviorProp(shok_GGL_CAutoCannonBehaviorProps::vtp);
}

shok_GGL_CThiefBehaviorProperties* shok_GGlue_CGlueEntityProps::GetThiefBehaviorProps()
{
	return (shok_GGL_CThiefBehaviorProperties*)SearchBehaviorProp(shok_GGL_CThiefBehaviorProperties::vtp);
}

shok_GGL_CCamouflageBehaviorProps* shok_GGlue_CGlueEntityProps::GetCamouflageBehaviorProps()
{
	return (shok_GGL_CCamouflageBehaviorProps*)SearchBehaviorProp(shok_GGL_CCamouflageBehaviorProps::vtp);
}

shok_GGL_CCircularAttackProps* shok_GGlue_CGlueEntityProps::GetCircularAttackBehaviorProps()
{
	return (shok_GGL_CCircularAttackProps*)SearchBehaviorProp(shok_GGL_CCircularAttackProps::vtp);
}

shok_GGL_CShurikenAbilityProps* shok_GGlue_CGlueEntityProps::GetShurikenBehaviorProps()
{
	return (shok_GGL_CShurikenAbilityProps*)SearchBehaviorProp(shok_GGL_CShurikenAbilityProps::vtp);
}

shok_GGL_CSniperAbilityProps* shok_GGlue_CGlueEntityProps::GetSniperBehaviorProps()
{
	return (shok_GGL_CSniperAbilityProps*)SearchBehaviorProp(shok_GGL_CSniperAbilityProps::vtp);
}

shok_GGL_CRangedEffectAbilityProps* shok_GGlue_CGlueEntityProps::GetRangedEffectBehaviorProps()
{
	return (shok_GGL_CRangedEffectAbilityProps*)SearchBehaviorProp(shok_GGL_CRangedEffectAbilityProps::vtp);
}

shok_GGL_CWorkerBehaviorProps* shok_GGlue_CGlueEntityProps::GetWorkerBehaviorProps()
{
	return (shok_GGL_CWorkerBehaviorProps*)SearchBehaviorProp(shok_GGL_CWorkerBehaviorProps::vtp);
}

shok_GGL_CHeroHawkBehaviorProps* shok_GGlue_CGlueEntityProps::GetHeroHawkBehaviorProps()
{
	return (shok_GGL_CHeroHawkBehaviorProps*)SearchBehaviorProp(shok_GGL_CHeroHawkBehaviorProps::vtp);
}

shok_GGL_CInflictFearAbilityProps* shok_GGlue_CGlueEntityProps::GetInflictFearBehaviorProps()
{
	return (shok_GGL_CInflictFearAbilityProps*)SearchBehaviorProp(shok_GGL_CInflictFearAbilityProps::vtp);
}

shok_GGL_CHeroAbilityProps* shok_GGlue_CGlueEntityProps::GetBombPlacerBehaviorProps()
{
	return (shok_GGL_CHeroAbilityProps*)SearchBehaviorProp(shok_GGL_CHeroAbilityProps::vtp);
}

shok_GGL_CCannonBuilderBehaviorProps* shok_GGlue_CGlueEntityProps::GetCannonBuilderBehaviorProps()
{
	return (shok_GGL_CCannonBuilderBehaviorProps*)SearchBehaviorProp(shok_GGL_CCannonBuilderBehaviorProps::vtp);
}

shok_GGL_CSummonBehaviorProps* shok_GGlue_CGlueEntityProps::GetSummonBehaviorProps()
{
	return (shok_GGL_CSummonBehaviorProps*)SearchBehaviorProp(shok_GGL_CSummonBehaviorProps::vtp);
}

shok_GGL_CConvertSettlerAbilityProps* shok_GGlue_CGlueEntityProps::GetConvertSettlersBehaviorProps()
{
	return (shok_GGL_CConvertSettlerAbilityProps*)SearchBehaviorProp(shok_GGL_CConvertSettlerAbilityProps::vtp);
}

shok_GGL_CKegPlacerBehaviorProperties* shok_GGlue_CGlueEntityProps::GetKegPlacerBehaviorProps()
{
	return (shok_GGL_CKegPlacerBehaviorProperties*)SearchBehaviorProp(shok_GGL_CKegPlacerBehaviorProperties::vtp);
}

shok_GGL_CAbilityScoutBinocularProps* shok_GGlue_CGlueEntityProps::GetBinocularBehaviorProps()
{
	return (shok_GGL_CAbilityScoutBinocularProps*)SearchBehaviorProp(shok_GGL_CAbilityScoutBinocularProps::vtp);
}

shok_GGL_CTorchPlacerBehaviorProperties* shok_GGlue_CGlueEntityProps::GetTorchPlacerBehaviorProps()
{
	return (shok_GGL_CTorchPlacerBehaviorProperties*)SearchBehaviorProp(shok_GGL_CTorchPlacerBehaviorProperties::vtp);
}

shok_GGL_CPointToResourceBehaviorProperties* shok_GGlue_CGlueEntityProps::GetPointToResBehaviorProps()
{
	return (shok_GGL_CPointToResourceBehaviorProperties*)SearchBehaviorProp(shok_GGL_CPointToResourceBehaviorProperties::vtp);
}

shok_GGL_CSerfBehaviorProps* shok_GGlue_CGlueEntityProps::GetSerfBehaviorProps()
{
	return (shok_GGL_CSerfBehaviorProps*)SearchBehaviorProp(shok_GGL_CSerfBehaviorProps::vtp);
}

shok_GGL_CResourceRefinerBehaviorProperties* shok_GGlue_CGlueEntityProps::GetResourceRefinerBehaviorProps()
{
	return (shok_GGL_CResourceRefinerBehaviorProperties*)SearchBehaviorProp(shok_GGL_CResourceRefinerBehaviorProperties::vtp);
}

shok_GGL_CBarrackBehaviorProperties* shok_GGlue_CGlueEntityProps::GetBarrackBehaviorProps()
{
	return (shok_GGL_CBarrackBehaviorProperties*)SearchBehaviorProp(shok_GGL_CBarrackBehaviorProperties::vtp);
}



bool shok_GGlue_CGlueEntityProps::IsSettlerType()
{
	return LogicProps->vtable == shok_GGL_CGLSettlerProps::vtp;
}

bool shok_GGlue_CGlueEntityProps::IsBuildingType()
{
	int data[2] = { shok_GGL_CGLBuildingProps::vtp, shok_GGL_CBridgeProperties::vtp };
	return contains(data, LogicProps->vtable, 2);
}

bool shok_GGlue_CGlueEntityProps::IsCEntityProperties()
{
	return LogicProps->vtable == shok_GGL_CEntityProperties::vtp;
}

bool shok_GGlue_CGlueEntityProps::IsOfCategory(int cat)
{
	for (int i : LogicProps->Categories)
		if (i == cat)
			return true;
	return false;
}

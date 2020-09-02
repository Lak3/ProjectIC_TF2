#pragma once
#include "../../../SDK/SDK.h"

class C_TriggerShoot
{
private:
	bool ShouldRun(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon);
	bool IsTraceEntityValid(C_BaseEntity *pLocal, C_BaseEntity *pEntity);

public:
	bool bActive		= false;
	int nHitbox			= 0; //0 all 1 head only
	float fHeadScale	= 0.9f;
	bool bIgnoreCloaked = false;

	void Run(C_UserCmd *pCmd);
};

extern C_TriggerShoot gTriggerShoot;
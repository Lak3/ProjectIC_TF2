#pragma once
#include "../../../SDK/SDK.h"

class C_TriggerStab
{
private:
	bool ShouldRun(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon);
	bool IsEntityValid(C_BaseEntity *pLocal, C_BaseEntity *pEntity);
	bool CanBackstab(const Vec3 &vecSrc, const Vec3 &vecDst, Vec3 vecWSCDelta);
	bool TraceMelee(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon, const Vec3 &vecViewAngles, C_BaseEntity **pEntityOut = nullptr);

public:
	bool bActive = false;
	bool bRage = false;

	void Run(C_UserCmd *pCmd);
};

extern C_TriggerStab gTriggerStab;
#include "TriggerShoot.h"

bool C_TriggerShoot::ShouldRun(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon)
{
	if (!bActive)
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	if (!pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsBonked() || pLocal->IsStunned())
		return false;

	int nDamageType = pLocalWeapon->GetDamageType();

	//basically a filter for bullet only weapons
	if (!(nDamageType & DMG_BULLET) && !(nDamageType & DMG_BUCKSHOT))
		return false;

	//if (!pLocalWeapon->CanShoot(pLocal))
	//	return false;

	//...

	return true;
}

bool C_TriggerShoot::IsTraceEntityValid(C_BaseEntity *pLocal, C_BaseEntity *pEntity)
{
	if (!pEntity || pEntity->GetTeamNum() == pLocal->GetTeamNum())
		return false;

	if (!pEntity->IsPlayer() && !pEntity->IsBuilding())
		return false;

	if (pEntity->IsPlayer())
	{
		if (!pEntity->IsVulnerable())
			return false;

		if (bIgnoreCloaked && pEntity->IsCloaked())
			return false;
	}

	return true;
}

void C_TriggerShoot::Run(C_UserCmd *pCmd)
{
	C_BaseEntity *pLocal = gEntCache.pLocal;
	C_BaseCombatWeapon *pLocalWeapon = gEntCache.pLocalWeapon;

	if (pLocal && pLocalWeapon)
	{
		if (!ShouldRun(pLocal, pLocalWeapon))
			return;

		Vec3 vecForward = Vec3();
		Math::AngleVectors(gInts.Engine->GetViewAngles(), &vecForward);

		Vec3 vecTraceStart = pLocal->GetShootPos();
		Vec3 vecTraceEnd = (vecTraceStart + (vecForward * 8192.0f));

		Ray_t ray;
		ray.Init(vecTraceStart, vecTraceEnd);
		C_TraceFilterHitscan filter;
		filter.pSkip = pLocal;
		C_GameTrace trace;
		gInts.EngineTrace->TraceRay(ray, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

		if (!IsTraceEntityValid(pLocal, trace.entity))
			return;

		if (trace.entity->IsPlayer())
		{
			if (nHitbox == 1 && trace.hitbox != HITBOX_HEAD)
				return;

			if (trace.hitbox == HITBOX_HEAD)
			{
				if (fHeadScale < 1.0f)
				{
					Vec3 vecMins		= Vec3();
					Vec3 vecMaxs		= Vec3();
					Vec3 vecCenter		= Vec3();
					matrix3x4 Matrix	= {};

					if (!trace.entity->GetHitboxMinsAndMaxsAndMatrix(HITBOX_HEAD, vecMins, vecMaxs, Matrix, &vecCenter))
						return;

					vecMins *= fHeadScale;
					vecMaxs *= fHeadScale;

					if (!Math::RayToOBB(vecTraceStart, vecForward, vecCenter, vecMins, vecMaxs, Matrix))
						return;
				}
			}
		}

		if (gGlobalInfo.bNoInterpolation && trace.entity->IsPlayer()) {
			pCmd->tick_count = TIME_TO_TICKS(trace.entity->flSimulationTime() + std::max(gConVars.cl_interp->GetFloat(),
				gConVars.cl_interp_ratio->GetFloat() / gConVars.cl_updaterate->GetFloat()));
		}

		pCmd->buttons |= IN_ATTACK;
	}
}

C_TriggerShoot gTriggerShoot;
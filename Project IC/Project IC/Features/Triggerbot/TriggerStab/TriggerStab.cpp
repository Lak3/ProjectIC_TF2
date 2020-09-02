#include "TriggerStab.h"
#include "../../Aimbot/Aimbot.h"

bool C_TriggerStab::ShouldRun(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon)
{
	if (!bActive)
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	if (!pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsBonked() || pLocal->IsStunned())
		return false;

	if (pLocalWeapon->GetWeaponID() != TF_WEAPON_KNIFE)
		return false;

	if (!gGlobalInfo.bWeaponCanShoot)
		return false;

	//...

	return true;
}

bool C_TriggerStab::IsEntityValid(C_BaseEntity *pLocal, C_BaseEntity *pEntity)
{
	if (!pEntity || pEntity->GetTeamNum() == pLocal->GetTeamNum())
		return false;

	if (!pEntity->IsPlayer() || !pEntity->IsVulnerable())
		return false;

	return true;
}

bool C_TriggerStab::CanBackstab(const Vec3 &vecSrc, const Vec3 &vecDst, Vec3 vecWSCDelta)
{
	vecWSCDelta.z = 0;
	vecWSCDelta.NormalizeInPlace();

	Vec3 vecEyeSpy = Vec3();
	Math::AngleVectors(vecSrc, &vecEyeSpy);
	vecEyeSpy.z = 0;
	vecEyeSpy.NormalizeInPlace();

	Vec3 vecEyeVictim = Vec3();
	Math::AngleVectors(vecDst, &vecEyeVictim);
	vecEyeVictim.z = 0;
	vecEyeVictim.NormalizeInPlace();

	if (vecWSCDelta.Dot(vecEyeVictim) <= 0.01f)
		return false;

	if (vecWSCDelta.Dot(vecEyeSpy) <= 0.5f)
		return false;

	if (vecEyeSpy.Dot(vecEyeVictim) <= -0.3f)
		return false;

	return true;
}

bool C_TriggerStab::TraceMelee(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon, const Vec3 &vecViewAngles, C_BaseEntity **pEntityOut)
{
	static Vec3 vecSwingMins = { -18, -18, -18 };
	static Vec3 vecSwingMaxs = { 18, 18, 18 };

	float flRange = (pLocalWeapon->GetSwingRange(pLocal) * 0.98f); //a little smaller for a bit more accuracy

	if (flRange <= 0.0f)
		return false;

	Vec3 vecForward = Vec3();
	Math::AngleVectors(vecViewAngles, &vecForward);

	Vec3 vecTraceStart = pLocal->GetShootPos();
	Vec3 vecTraceEnd = (vecTraceStart + (vecForward * flRange));

	C_TraceFilterHitscan filter;
	filter.pSkip = pLocal;
	C_GameTrace trace;
	Util::TraceHull(vecTraceStart, vecTraceEnd, vecSwingMins, vecSwingMaxs, MASK_SOLID, &filter, &trace);
	
	if (trace.entity)
	{
		if (pEntityOut && !*pEntityOut)
			*pEntityOut = trace.entity;

		return true;
	}

	return false;
}

void C_TriggerStab::Run(C_UserCmd *pCmd)
{
	C_BaseEntity *pLocal				= gEntCache.pLocal;
	C_BaseCombatWeapon *pLocalWeapon	= gEntCache.pLocalWeapon;

	if (pLocal && pLocalWeapon)
	{
		if (!ShouldRun(pLocal, pLocalWeapon))
			return;

		if (bRage)
		{
			Vec3 vecLocalPos = pLocal->GetShootPos();

			const auto Enemies = gEntCache.GetGroup(GroupType_t::PLAYERS_ENEMIES);

			for (const auto &Enemy : Enemies)
			{
				if (!IsEntityValid(pLocal, Enemy))
					continue;

				Vec3 vecAngleTo = Math::CalcAngle(vecLocalPos, Enemy->GetHitboxPos(HITBOX_BODY));

				if (!CanBackstab(vecAngleTo, Enemy->GetEyeAngles(), (Enemy->GetWorldSpaceCenter() - pLocal->GetWorldSpaceCenter())))
					continue;

				C_BaseEntity *pTraceEntity = nullptr;

				if (!TraceMelee(pLocal, pLocalWeapon, vecAngleTo, &pTraceEntity))
					continue;

				if (!IsEntityValid(pLocal, pTraceEntity) || pTraceEntity != Enemy)
					continue;

				if (gAimbot.nAimMethod == 3)
					gAimbot.FixMovement(pCmd, vecAngleTo);

				pCmd->viewangles = vecAngleTo;

				if (gGlobalInfo.bNoInterpolation) {
					pCmd->tick_count = TIME_TO_TICKS(Enemy->flSimulationTime() + std::max(gConVars.cl_interp->GetFloat(),
						gConVars.cl_interp_ratio->GetFloat() / gConVars.cl_updaterate->GetFloat()));
				}

				pCmd->buttons |= IN_ATTACK;
				break;
			}
		}

		else
		{
			C_BaseEntity *pEntity = nullptr;

			if (!TraceMelee(pLocal, pLocalWeapon, gInts.Engine->GetViewAngles(), &pEntity))
				return;

			if (!IsEntityValid(pLocal, pEntity))
				return;

			if (!CanBackstab(gInts.Engine->GetViewAngles(), pEntity->GetEyeAngles(), (pEntity->GetWorldSpaceCenter() - pLocal->GetWorldSpaceCenter())))
				return;

			if (gGlobalInfo.bNoInterpolation) {
				pCmd->tick_count = TIME_TO_TICKS(pEntity->flSimulationTime() + std::max(gConVars.cl_interp->GetFloat(),
					gConVars.cl_interp_ratio->GetFloat() / gConVars.cl_updaterate->GetFloat()));
			}

			pCmd->buttons |= IN_ATTACK;
		}
	}
}

C_TriggerStab gTriggerStab;
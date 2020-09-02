#include "Aimbot.h"

#include "../Menu/Menu.h"
#include "../Visuals/Visuals.h"
#include "../Triggerbot/TriggerBot.h"

bool bSortingByFov = false; //for FOV circle

bool C_Aimbot::ShouldRun(C_BaseEntity* pLocal, C_BaseCombatWeapon *pLocalWeapon)
{
	if (!bActive)
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	//stunned is also when you receive knockback from force-a-nature
	if (!pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsBonked()/* || pLocal->IsStunned()*/)
		return false;

	if (pLocalWeapon->GetSlot() == 2)
	{
		if (bNoMelee)
			return false;

		if (pLocalWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
		{
			//don't let the aimbot mess with the viewangles when triggerstab's rage option is enabled
			if (gTriggerBot.bActive && gTriggerStab.bActive && gTriggerStab.bRage)
				return false;
		}
	}

	//TODO: remove us when/if functionality for us is added!
	{
		int nItemDefIdx = pLocalWeapon->GetItemDefinitionIndex();

		switch (nItemDefIdx)
		{
			case Soldier_m_RocketJumper:
			case Demoman_s_StickyJumper: {
				return false;
			}

			default: break;
		}

		switch (pLocalWeapon->GetWeaponID())
		{
			case TF_WEAPON_PDA:
			case TF_WEAPON_PDA_ENGINEER_BUILD:
			case TF_WEAPON_PDA_ENGINEER_DESTROY:
			case TF_WEAPON_PDA_SPY:
			case TF_WEAPON_PDA_SPY_BUILD:
			case TF_WEAPON_BUILDER:
			case TF_WEAPON_INVIS: 
			case TF_WEAPON_LUNCHBOX:
			case TF_WEAPON_BUFF_ITEM:
			case TF_WEAPON_GRAPPLINGHOOK: {
				return false;
			}

			default: break;
		}
	}

	//...
	
	return true;
}

bool C_Aimbot::IsAimKeyDown()
{
	switch (nAimKey)
	{
		case 0: {
			return (GetAsyncKeyState(VK_SHIFT) & 0x8000);
		}
		case 1: {
			return (GetAsyncKeyState(VK_XBUTTON2) & 0x8000);
		}

			  //...

		default: {
			return false;
		}
	}

	return false;
}

int C_Aimbot::GetAimHitbox(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon)
{
	switch (nAimHitbox)
	{
		case 0: {
			return HITBOX_HEAD;
		}

		case 1: {
			return HITBOX_PELVIS;
		}

		case 2:
		{
			int nWeaponSlot = pLocalWeapon->GetSlot();

			if (nWeaponSlot == 2)
				return HITBOX_BODY; //it feels awkward to aim at pelvis with melee

			int nClassNum = pLocal->GetClassNum();

			if (nClassNum == CLASS_SNIPER)
			{
				//all sniper's primary weapons can headshot
				return (nWeaponSlot == 0 ? HITBOX_HEAD : HITBOX_PELVIS);
			}

			else if (nClassNum == CLASS_SPY)
			{
				int nWeaponIndex = pLocalWeapon->GetItemDefinitionIndex();
				bool bIsAmbassador = (nWeaponIndex == Spy_m_TheAmbassador || nWeaponIndex == Spy_m_FestiveAmbassador);

				return (bIsAmbassador ? HITBOX_HEAD : HITBOX_PELVIS);
			}

			else return HITBOX_PELVIS;
		}
	}

	return 0;
}

bool C_Aimbot::GetTargets(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon)
{
	Vec3 vecLocalShootPos = pLocal->GetShootPos();
	Vec3 vecLocalAngles = gInts.Engine->GetViewAngles();
	int nHitbox = GetAimHitbox(pLocal, pLocalWeapon);
	ESortType SortType = GetSortType(pLocalWeapon);

	bSortingByFov = (SortType == ESortType::FOV);

	Targets.clear();

	if (bAimPlayers)
	{
		bool bIsCrossbow = (pLocalWeapon->GetWeaponID() == TF_WEAPON_CROSSBOW);

		auto Players = (bIsCrossbow ? gEntCache.GetGroup(GroupType_t::PLAYERS_ALL) : gEntCache.GetGroup(GroupType_t::PLAYERS_ENEMIES));

		for (const auto& Player : Players)
		{
			if (!Player->IsAlive())
				continue;

			if (!bIsCrossbow)
			{
				if (!Player->IsVulnerable())
					continue;

				if (bIgnoreFriends && pLocal->IsPlayerOnSteamFriendList(Player))
					continue;

				if (bIgnoreCloaked && Player->IsCloaked())
					continue;
			}

			Target_t Target = {};

			Target.pEntity = Player;
			Target.eType = ETargetType::PLAYER;
			Target.vecPos = Player->GetHitboxPos(nHitbox);
			Target.vecAngleTo = Math::CalcAngle(vecLocalShootPos, Target.vecPos);
			Target.fFovTo = Math::CalcFov(vecLocalAngles, Target.vecAngleTo);
			Target.fDistTo = vecLocalShootPos.DistTo(Target.vecPos);
			Target.nTargetedHitbox = nHitbox;

			if (SortType == ESortType::FOV && Target.fFovTo > fAimFov)
				continue;

			Targets.push_back(Target);
		}
	}

	if (bAimBuildings)
	{
		auto Buildings = gEntCache.GetGroup(GroupType_t::BUILDINGS_ENEMIES);

		for (const auto& Building : Buildings)
		{
			if (!Building->IsAlive())
				continue;

			Target_t Target = {};

			Target.pEntity = Building;
			Target.eType = ETargetType::BUILDING;
			Target.vecPos = Building->GetWorldSpaceCenter();
			Target.vecAngleTo = Math::CalcAngle(vecLocalShootPos, Target.vecPos);
			Target.fFovTo = Math::CalcFov(vecLocalAngles, Target.vecAngleTo);
			Target.fDistTo = vecLocalShootPos.DistTo(Target.vecPos);

			if (SortType == ESortType::FOV && Target.fFovTo > fAimFov)
				continue;

			Targets.push_back(Target);
		}
	}

	return !Targets.empty();
}

void C_Aimbot::SortTargets(C_BaseCombatWeapon* pLocalWeapon)
{
	std::sort(Targets.begin(), Targets.end(), [&](const Target_t& a, const Target_t& b) -> bool
		{
			switch (GetSortType(pLocalWeapon))
			{
			case ESortType::FOV: {
				return (a.fFovTo < b.fFovTo);
			}

			case ESortType::DISTANCE: {
				return (a.fDistTo < b.fDistTo);
			}

			default: { return false; };
			}
		});
}

bool C_Aimbot::GetTarget(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon, C_UserCmd *pCmd, Target_t& out)
{
	if (!GetTargets(pLocal, pLocalWeapon))
		return false;

	SortTargets(pLocalWeapon);

	Vec3 vecLocalShootPos = pLocal->GetShootPos();

	ProjectileInfo_t ProjInfo = {};
	bool bIsProjectile = GetProjectileInfo(pLocalWeapon, ProjInfo);
	bIsCurWeaponProjectile = bIsProjectile;

	for (auto& Target : Targets)
	{
		if (bIsProjectile)
		{
			Vec3 vecAimPos = Target.pEntity->GetWorldSpaceCenter();

			if (Target.eType == ETargetType::PLAYER)
			{
				switch (pLocal->GetClassNum())
				{
					case CLASS_SOLDIER: {
						vecAimPos = (Target.pEntity->GetAbsOrigin() + Vec3(0.0f, 0.0f, 15.0f));
						break;
					}

					case CLASS_SNIPER: {
						vecAimPos = Target.pEntity->GetHitboxPos(HITBOX_HEAD);
						break;
					}

					default: break;
				}
			}

			//setup the predictor
			Predictor_t Predictor = {
				Target.pEntity,
				vecAimPos,
				Target.eType == ETargetType::PLAYER ? Target.pEntity->GetVelocity() : Vec3(),
				Target.eType == ETargetType::PLAYER ? Vec3(0.0f, 0.0f, gConVars.sv_gravity->GetFloat() * ((Target.pEntity->GetCondEx2() & TFCondEx2_Parachute) ? 0.224f : 1.0f)) : Vec3()
			};

			Solution_t Solution = {};

			if (!SolveProjectile(pLocal, pLocalWeapon, pCmd, Predictor, ProjInfo, Solution))
				return false; //we don't want to continue this...

			Target.vecAngleTo = { -RAD2DEG(Solution.fPitch), RAD2DEG(Solution.fYaw), 0.0f };
		}

		else
		{
			if (pLocalWeapon->GetSlot() == 2)
			{
				if (bRangeCheck ? !CanMeleeHit(pLocal, pLocalWeapon, Target.vecAngleTo, Target.pEntity->GetIndex()) :
					!gVisCheck.Pos(pLocal, Target.pEntity, vecLocalShootPos, Target.vecPos)) {
					continue;
				}
			}

			else
			{
				if (Target.nTargetedHitbox == HITBOX_HEAD)
				{
					int nHitboxOut = -1;

					if (!gVisCheck.PosHitboxIdOut(pLocal, Target.pEntity, vecLocalShootPos, Target.vecPos, nHitboxOut))
						continue;

					if (nHitboxOut != HITBOX_HEAD)
						DoHeadCorrection(pLocal, Target); //try correcting and if we don't find a correction then fuck it keep aiming center
				}

				else
				{
					if (!gVisCheck.Pos(pLocal, Target.pEntity, vecLocalShootPos, Target.vecPos))
					{
						if (!DoBodyCorrection(pLocal, Target))
							continue;
					}

					else
					{
						if (nOldHitScan != -1)
							ResetSmooth();

						nOldHitScan = -1;
					}
				}
			}
		}

		out = Target;
		return true;
	}

	return false;
}

void C_Aimbot::SetAngles(const Target_t& Target, C_UserCmd* pCmd, C_BaseEntity* pLocal, C_BaseCombatWeapon* pWeapon)
{
	Vec3 vecAngle = Target.vecAngleTo /*- (pLocal->GetPunchAngles())*/;
	Math::ClampAngles(vecAngle);

	switch (nAimMethod)
	{
		case 0: //normal
		{
		pCmd->viewangles = vecAngle;
		break;
	}

		case 1: //smooth
		{
			float fProgress = std::clamp(((gInts.GlobalVars->curtime - fSmoothStartTime) / fSmoothDuration), 0.0f, 1.0f);
			bSmoothFinished = (fProgress > 0.99f);

			pCmd->viewangles = vecSmoothStartAngle;

			Vec3 vecAngleDelta = (vecAngle - vecSmoothStartAngle);
			Math::ClampAngles(vecAngleDelta);

			pCmd->viewangles += (vecAngleDelta * Math::EaseInOutSine(fProgress));
			break;
		}

		case 2: //smooth simple
		{
			Vec3 vecDelta = (vecAngle - pCmd->viewangles);
			Math::ClampAngles(vecDelta);

			pCmd->viewangles += (vecDelta / fSmoothSimple);
			break;
		}

		case 3: //silent
		{
			FixMovement(pCmd, vecAngle);
			pCmd->viewangles = vecAngle;
			break;
		}
	}
}

bool C_Aimbot::ShouldShoot(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon, C_UserCmd* pCmd, const Target_t& Target)
{
	//at the end of this function we return true, so add checks that will return false

	if (!bAutoShoot)
		return false;

	if (pLocalWeapon->GetSlot() == 2)
	{
		if (pLocalWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
		{
			//let the triggerstab do the shooting for us
			//the rage option of triggerstab disables the aimbot altogether
			if (gTriggerBot.bActive && gTriggerStab.bActive && !gTriggerStab.bRage)
				return false;
		}

		//CanMeleeHit is ran earlier if RangeCheck is enabled so no point in doing it again
		if (!bRangeCheck && !CanMeleeHit(pLocal, pLocalWeapon, Target.vecAngleTo, Target.pEntity->GetIndex()))
			return false;
	}

	else
	{
		if (bIsCurWeaponProjectile)
		{
			//we do not know when we'll be able to shoot with the other smooth method so do not.
			if ((nAimMethod == 1 && !bSmoothFinished) || nAimMethod == 2)
				return false;
		}

		int nClassNum		= pLocal->GetClassNum();
		int nWeaponIndex	= pLocalWeapon->GetItemDefinitionIndex();
		int nWeaponSlot		= pLocalWeapon->GetSlot();

		switch (nClassNum)
		{
			case CLASS_SNIPER:
			{
				if (bWaitForCharge && !gGlobalInfo.bWeaponCanHeadShot)
				{
					if (pLocal->IsScoped() && nWeaponSlot == 0)
						return false;
				}

				break;
			}

			case CLASS_SPY:
			{
				if (bWaitForCharge && !gGlobalInfo.bWeaponCanHeadShot)
				{
					if (nWeaponIndex == Spy_m_TheAmbassador || nWeaponIndex == Spy_m_FestiveAmbassador)
						return false;
				}

				break;
			}

			default: {
				break;
			}
		}

		if ((nAimMethod == 1 && !bSmoothFinished) || nAimMethod == 2)
		{
			Vec3 vecForward = Vec3();
			Math::AngleVectors(pCmd->viewangles, &vecForward);

			Vec3 vecTraceStart = pLocal->GetShootPos();
			Vec3 vecTraceEnd = (vecTraceStart + (vecForward * 8192.0f));

			Ray_t ray;
			ray.Init(vecTraceStart, vecTraceEnd);
			C_TraceFilterHitscan filter;
			filter.pSkip = pLocal;
			C_GameTrace trace;
			gInts.EngineTrace->TraceRay(ray, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

			if (trace.entity != Target.pEntity)
				return false;

			//for the head let's do some extra checking for accuracy reasons
			if (Target.nTargetedHitbox == HITBOX_HEAD)
			{
				if (trace.hitbox != HITBOX_HEAD)
					return false;

				Vec3 vecMins		= Vec3();
				Vec3 vecMaxs		= Vec3();
				Vec3 vecCenter		= Vec3();
				matrix3x4 Matrix	= {};

				if (!trace.entity->GetHitboxMinsAndMaxsAndMatrix(HITBOX_HEAD, vecMins, vecMaxs, Matrix, &vecCenter))
					return false;

				vecMins *= 0.3f;
				vecMaxs *= 0.3f;

				if (!Math::RayToOBB(vecTraceStart, vecForward, vecCenter, vecMins, vecMaxs, Matrix))
					return false;
			}
		}
	}

	return true;
}

void C_Aimbot::FixMovement(C_UserCmd* pCmd, const Vec3& vecTargetAngle)
{
	Vec3 vecMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
	Vec3 vecMoveAng = Vec3();

	Math::VectorAngles(vecMove, vecMoveAng);

	float fSpeed = Math::FastSqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);
	float fYaw = DEG2RAD(vecTargetAngle.y - pCmd->viewangles.y + vecMoveAng.y);

	pCmd->forwardmove = (cos(fYaw) * fSpeed);
	pCmd->sidemove = (sin(fYaw) * fSpeed);
}

C_Aimbot::ESortType C_Aimbot::GetSortType(C_BaseCombatWeapon* pLocalWeapon)
{
	if (nSortMethod == 0)
		return ESortType::FOV;

	else if (nSortMethod == 1)
		return ESortType::DISTANCE;

	else return (pLocalWeapon->GetSlot() == 2 ? ESortType::DISTANCE : ESortType::FOV);

	return ESortType::UNKNOWN;
}

bool C_Aimbot::CanMeleeHit(C_BaseEntity *pLocal, C_BaseCombatWeapon* pLocalWeapon, const Vec3& vecViewAngles, int nTargetIndex)
{
	if (!pLocalWeapon || pLocalWeapon->GetSlot() != 2)
		return false;

	static Vec3 vecSwingMins = { -18, -18, -18 };
	static Vec3 vecSwingMaxs = { 18, 18, 18 };

	float flRange = (pLocalWeapon->GetSwingRange(pLocal));

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

	//if the normal one didn't hit we can try predicting the swing
	if (!(trace.entity && trace.entity->GetIndex() == nTargetIndex))
	{
		if (!bPredictSwing)
			return false;

		Vec3 vecTraceStart = pLocal->GetShootPos() + (pLocal->GetVelocity() * pLocalWeapon->GetWeaponData().m_flSmackDelay);
		Vec3 vecTraceEnd = (vecTraceStart + (vecForward * flRange));

		Util::TraceHull(vecTraceStart, vecTraceEnd, vecSwingMins, vecSwingMaxs, MASK_SOLID, &filter, &trace);

		return (trace.entity && trace.entity->GetIndex() == nTargetIndex);
	}

	else return true;
}

void C_Aimbot::DoHeadCorrection(C_BaseEntity* pLocal, Target_t& Target)
{
	model_t* pModel = Target.pEntity->GetModel();
	if (!pModel)
		return;

	studiohdr_t* pHDR = reinterpret_cast<studiohdr_t*>(gInts.ModelInfo->GetStudioModel(pModel));
	if (!pHDR)
		return;

	matrix3x4 BoneMatrix[128];
	if (!Target.pEntity->SetupBones(BoneMatrix, 128, 0x100, gInts.GlobalVars->curtime))
		return;

	mstudiohitboxset_t* pSet = pHDR->pHitboxSet(Target.pEntity->GetHitboxSet());
	if (!pSet)
		return;

	mstudiobbox_t* pBox = pSet->hitbox(HITBOX_HEAD);
	if (!pBox)
		return;

	Vec3 vecLocalShootPos = pLocal->GetShootPos();

	Vec3 vecMins = pBox->bbmin;
	Vec3 vecMaxs = pBox->bbmax;

	Vec3 vecTopCenter = Vec3(
		(vecMins.x + vecMaxs.x) * 0.5f,
		(vecMins.y * 0.7f),
		(vecMins.z + vecMaxs.z) * 0.5f
	);

	Vec3 vecTopCenterTransformed = Vec3();

	Math::VectorTransform(vecTopCenter, BoneMatrix[pBox->bone], vecTopCenterTransformed);

	if (gVisCheck.PosHitboxId(pLocal, Target.pEntity, vecLocalShootPos, vecTopCenterTransformed, HITBOX_HEAD)) {
		Target.vecPos = vecTopCenterTransformed;
		Target.vecAngleTo = Math::CalcAngle(vecLocalShootPos, vecTopCenterTransformed);
	}
}

bool C_Aimbot::DoBodyCorrection(C_BaseEntity* pLocal, Target_t& Target)
{
	Vec3 vecLocalShootPos = pLocal->GetShootPos();

	for (int n = 0; n < Target.pEntity->GetNumOfHitboxes(); n++)
	{
		if (n == HITBOX_PELVIS)
			continue; //we got here because PELVIS wasn't visible, let's continue

		Vec3 vecScannedHitbox = Target.pEntity->GetHitboxPos(n);

		if (gVisCheck.Pos(pLocal, Target.pEntity, vecLocalShootPos, vecScannedHitbox))
		{
			Target.vecPos = vecScannedHitbox;
			Target.vecAngleTo = Math::CalcAngle(vecLocalShootPos, vecScannedHitbox);

			if (nOldHitScan != n) {
				nOldHitScan = n;
				ResetSmooth();
			}

			return true;
		}
	}

	return false;
}


void C_Aimbot::ResetSmooth()
{
	vecSmoothStartAngle = gInts.Engine->GetViewAngles();
	fSmoothStartTime = gInts.GlobalVars->curtime;
}

bool C_Aimbot::TargetChanged()
{
	static int nOldTarget = gGlobalInfo.nCurrentTargetIdx;

	if (gGlobalInfo.nCurrentTargetIdx != nOldTarget) {
		nOldTarget = gGlobalInfo.nCurrentTargetIdx;
		return true;
	}

	return false;
}

bool C_Aimbot::WeaponChanged()
{
	if (!gEntCache.pLocalWeapon)
		return false;

	int nIndex = gEntCache.pLocalWeapon->GetItemDefinitionIndex();
	static int nOldIndex = nIndex;

	if (nIndex != nOldIndex) {
		nOldIndex = nIndex;
		return true;
	}

	return false;
}

bool C_Aimbot::Clip1Changed()
{
	if (!gEntCache.pLocalWeapon)
		return false;

	int nClip1 = gEntCache.pLocalWeapon->GetClip1();
	static int nOlcClip1 = nClip1;

	if (nClip1 != nOlcClip1) {
		nOlcClip1 = nClip1;
		return true;
	}

	return false;
}

bool C_Aimbot::Clip2Changed()
{
	if (!gEntCache.pLocalWeapon)
		return false;

	int nClip2 = gEntCache.pLocalWeapon->GetClip2();
	static int nOldClip2 = nClip2;

	if (nClip2 != nOldClip2) {
		nOldClip2 = nClip2;
		return true;
	}

	return false;
}

bool C_Aimbot::AmmoChanged()
{
	if (!gEntCache.pLocal)
		return false;

	int nAmmo = gEntCache.pLocal->GetAmmo();
	static int nOldAmmo = nAmmo;

	if (nAmmo != nOldAmmo) {
		nOldAmmo = nAmmo;
		return true;
	}

	return false;
}


bool C_Aimbot::GetProjectileInfo(C_BaseCombatWeapon *pWeapon, ProjectileInfo_t &out)
{
	switch (pWeapon->GetItemDefinitionIndex())
	{
		case Soldier_m_RocketLauncher:
		case Soldier_m_RocketLauncherR:
		case Soldier_m_TheBlackBox:
		case Soldier_m_TheCowMangler5000:
		case Soldier_m_TheOriginal:
		case Soldier_m_FestiveRocketLauncher:
		case Soldier_m_TheBeggarsBazooka:
		case Soldier_m_SilverBotkillerRocketLauncherMkI:
		case Soldier_m_GoldBotkillerRocketLauncherMkI:
		case Soldier_m_RustBotkillerRocketLauncherMkI:
		case Soldier_m_BloodBotkillerRocketLauncherMkI:
		case Soldier_m_CarbonadoBotkillerRocketLauncherMkI:
		case Soldier_m_DiamondBotkillerRocketLauncherMkI:
		case Soldier_m_SilverBotkillerRocketLauncherMkII:
		case Soldier_m_GoldBotkillerRocketLauncherMkII:
		case Soldier_m_FestiveBlackBox:
		case Soldier_m_TheAirStrike:
		case Soldier_m_WoodlandWarrior:
		case Soldier_m_SandCannon:
		case Soldier_m_AmericanPastoral:
		case Soldier_m_SmalltownBringdown:
		case Soldier_m_ShellShocker:
		case Soldier_m_AquaMarine:
		case Soldier_m_Autumn:
		case Soldier_m_BlueMew:
		case Soldier_m_BrainCandy:
		case Soldier_m_CoffinNail:
		case Soldier_m_HighRollers:
		case Soldier_m_Warhawk: {
			out = { 1100.0f, 0.0f };
			break;
		}

		case Soldier_m_TheDirectHit: {
			out = { 1980.0f, 0.0f };
			break;
		}

		case Soldier_m_TheLibertyLauncher: {
			out = { 1540.0f, 0.0f };
			break;
		}

		case Demoman_m_GrenadeLauncher:
		case Demoman_m_GrenadeLauncherR:
		case Demoman_m_FestiveGrenadeLauncher:
		case Demoman_m_TheIronBomber:
		case Demoman_m_Autumn:
		case Demoman_m_MacabreWeb:
		case Demoman_m_Rainbow:
		case Demoman_m_SweetDreams:
		case Demoman_m_CoffinNail:
		case Demoman_m_TopShelf:
		case Demoman_m_Warhawk:
		case Demoman_m_ButcherBird: {
			out = { 1217.0f, 0.4f };
			break;
		}

		case Soldier_s_TheRighteousBison:
		case Engi_m_ThePomson6000: {
			out = { 1200.0f, 0.0f };
			break;
		}

		case Demoman_m_TheLooseCannon: {
			out = { 1453.9f, 0.4f };
			break;
		}

		case Demoman_m_TheLochnLoad: {
			out = { 1513.3f, 0.4f };
			break;
		}

		case Engi_m_TheRescueRanger:
		case Medic_m_FestiveCrusadersCrossbow:
		case Medic_m_CrusadersCrossbow: {
			out = { 2400.0f, 0.2f };
			break;
		}

		case Pyro_m_DragonsFury: {
			out = { 3000.0f, 0.0f };
			break;
		}

		case Pyro_m_FlameThrower:
		case Pyro_m_ThePhlogistinator: 
		case Pyro_m_TheBackburner: 
		case Pyro_m_TheDegreaser: {
			out = { 1000.0f, 0.0f, 0.4f};
			break;
		}

		case Pyro_s_TheDetonator:
		case Pyro_s_TheFlareGun:
		case Pyro_s_FestiveFlareGun:
		case Pyro_s_TheScorchShot: {
			out = { 2000.0f, 0.3f };
			break;
		}

		case Pyro_s_TheManmelter: {
			out = { 3000.0f, 0.2f };
			break;
		}

		case Medic_m_SyringeGun:
		case Medic_m_SyringeGunR:
		case Medic_m_TheBlutsauger:
		case Medic_m_TheOverdose: {
			out = { 1000.0f, 0.2f };
			break;
		}

		case Sniper_m_TheHuntsman:
		case Sniper_m_FestiveHuntsman:
		case Sniper_m_TheFortifiedCompound: {
			float charge = (gInts.GlobalVars->curtime - pWeapon->GetChargeTime());
			out = { ((fminf(fmaxf(charge, 0.0f), 1.0f) * 800.0f) + 1800.0f), ((fminf(fmaxf(charge, 0.0f), 1.0f) * -0.4f) + 0.5f) };
			break;
		}
	}

	return out.fSpeed;
}

Vec3 C_Aimbot::Predictor_t::Extrapolate(float time)
{
	Vec3 vecOut = {};

	if (pEntity->IsOnGround())
	{
		vecOut = (vecPosition + (vecVelocity * time));

		//TODO: StayOnGround
	}

	else vecOut = (vecPosition + (vecVelocity * time) - vecAcceleration * 0.5f * time * time + std::max(1.0f, time * 0.4f));

	return vecOut;
}

bool C_Aimbot::CalcProjAngle(const Vec3 &vecLocalPos, const Vec3 &vecPredictedPos, const ProjectileInfo_t &ProjInfo, Solution_t &out)
{
	const Vec3 v	= (vecPredictedPos - vecLocalPos);
	const float dx	= sqrt(v.x * v.x + v.y * v.y);
	const float dy	= v.z;
	const float v0	= ProjInfo.fSpeed;
	const float g	= (gConVars.sv_gravity->GetFloat() * ProjInfo.fGravity);

	if (g > 0.0f)
	{
		const float root = v0 * v0 * v0 * v0 - g * (g * dx * dx + 2.0f * dy * v0 * v0);

		if (root < 0.0f)
			return false;

		out.fPitch = atan((v0 * v0 - sqrt(root)) / (g * dx));
		out.fYaw = atan2(v.y, v.x);
	}

	else
	{
		Vec3 vecAngle	= Math::CalcAngle(vecLocalPos, vecPredictedPos);
		out.fPitch		= -DEG2RAD(vecAngle.x);
		out.fYaw		= DEG2RAD(vecAngle.y);
	}

	out.fTime = dx / (cos(out.fPitch) * v0);

	return true;
}

void C_Aimbot::GetProjectileFireSetup(C_BaseEntity *pPlayer, const Vec3 &vecViewAngles, Vec3 vecOffset, Vec3 *vecSrc)
{
	if (gConVars.cl_flipviewmodels->GetInt())
		vecOffset.y *= -1.0f;

	Vec3 vecForward = Vec3(), vecRight = Vec3(), vecUp = Vec3();
	Math::AngleVectors(vecViewAngles, &vecForward, &vecRight, &vecUp);

	*vecSrc = pPlayer->GetShootPos() + (vecForward * vecOffset.x) + (vecRight * vecOffset.y) + (vecUp * vecOffset.z);
}

bool C_Aimbot::SolveProjectile(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon, C_UserCmd *pCmd, Predictor_t &Predictor, const ProjectileInfo_t &ProjInfo, Solution_t &out)
{
	C_NetChannelInfo *pNetChannel = reinterpret_cast<C_NetChannelInfo *>(gInts.Engine->GetNetChannelInfo());

	if (!pNetChannel)
		return false;

	Ray_t Ray = {};
	C_TraceFilterWorldAndPropsOnly TraceFilter = {};
	C_GameTrace Trace = {};
	
	TraceFilter.pSkip = Predictor.pEntity;

	Vec3 vecLocalPos = pLocal->GetShootPos();

	float fInterp = gConVars.cl_interp->GetFloat();
	float fLatency = (pNetChannel->GetLatency(FLOW_OUTGOING) + pNetChannel->GetLatency(FLOW_INCOMING));

	float MAX_TIME = ProjInfo.fMaxTime;
	float TIME_STEP = (MAX_TIME / 192.0f);

	for (float fPredTime = 0.0f; fPredTime < MAX_TIME; fPredTime += TIME_STEP)
	{
		float fCorrectTime = (fPredTime + fInterp + fLatency);
		Vec3 vecPredictedPos = Predictor.Extrapolate(fCorrectTime);

		if (!Predictor.vecVelocity.IsZero())
		{
			//collision trace
			//Ray.Init(Predictor.vecPosition, vecPredictedPos);
			//gInts.EngineTrace->TraceRay(Ray, MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);
			Util::TraceHull(Predictor.vecPosition, vecPredictedPos, Vec3(-2, -2, -2), Vec3(2, 2, 2), MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);

			if (Trace.DidHit())
			{
				//TODO: handle collisions
				vecPredictedPos.z = Trace.vEndPos.z;
			}
		}

		//need to actually offset the shootpos for pipes
		switch (pLocalWeapon->GetWeaponID())
		{
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				Vec3 vecOffset(16.0f, 8.0f, -6.0f);
				GetProjectileFireSetup(pLocal, pCmd->viewangles, vecOffset, &vecLocalPos);

				//let's elevate the pos here

				Vec3 vecDelta = (vecPredictedPos - vecLocalPos);
				float fRange = Math::VectorNormalize(vecDelta);

				float fElevationAngle = (fRange * (pLocalWeapon->GetItemDefinitionIndex() == Demoman_m_TheLochnLoad ? 0.0075f : 0.013f));

				if (fElevationAngle > 45.0f)
					fElevationAngle = 45.0f;

				float s = 0.0f, c = 0.0f;
				Math::SinCos((fElevationAngle * PI / 180.0f), &s, &c);

				float fElevation = (fRange * (s / c));
				vecPredictedPos.z += (c > 0.0f ? fElevation : 0.0f);

				break;
			}

			default: break;
		}

		if (!CalcProjAngle(vecLocalPos, vecPredictedPos, ProjInfo, out))
			return false;

		if (out.fTime < fCorrectTime)
		{
			Vec3 vecVisCheck = vecLocalPos;

			//only vischeck from these offsets, don't offset the pos!
			switch (pLocalWeapon->GetWeaponID())
			{
				case TF_WEAPON_ROCKETLAUNCHER:
				case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
				case TF_WEAPON_FLAREGUN:
				case TF_WEAPON_FLAREGUN_REVENGE:
				{
					Vec3 vecOffset(23.5f, 12.0f, -3.0f);

					if (pLocal->IsDucking())
						vecOffset.z = 8.0f;

					GetProjectileFireSetup(pLocal, pCmd->viewangles, vecOffset, &vecVisCheck);
					break;
				}

				case TF_WEAPON_GRENADELAUNCHER:
				case TF_WEAPON_PIPEBOMBLAUNCHER:
				{
					Vec3 vecOffset(16.0f, 8.0f, -6.0f);
					GetProjectileFireSetup(pLocal, pCmd->viewangles, vecOffset, &vecVisCheck);

					//while we're here let's also correct vecUp

					Vec3 vecAngle = Vec3(), vecForward = Vec3(), vecRight = Vec3(), vecUp = Vec3();
					Math::AngleVectors({ -RAD2DEG(out.fPitch), RAD2DEG(out.fYaw), 0.0f }, &vecForward, &vecRight, &vecUp);
					Vec3 vecVelocity = ((vecForward * ProjInfo.fSpeed) - (vecUp * 200.0f));
					Math::VectorAngles(vecVelocity, vecAngle);
					out.fPitch = -DEG2RAD(vecAngle.x);

					break;
				}

				case TF_WEAPON_SYRINGEGUN_MEDIC:
				{
					Vec3 vecOffset(16.0f, 6.0f, -8.0f);
					GetProjectileFireSetup(pLocal, pCmd->viewangles, vecOffset, &vecVisCheck);
					break;
				}

				case TF_WEAPON_COMPOUND_BOW:
				{
					Vec3 vecOffset(23.5f, -8.0f, -3.0f);
					GetProjectileFireSetup(pLocal, pCmd->viewangles, vecOffset, &vecVisCheck);
					break;
				}

				default: break;
			}

			//vischeck trace
			//Ray.Init(vecVisCheck, vecPredictedPos);
			//gInts.EngineTrace->TraceRay(Ray, MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);
			Util::TraceHull(vecVisCheck, vecPredictedPos, Vec3(-2, -2, -2), Vec3(2, 2, 2), MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);

			if (Trace.DidHit())
				return false;

			gGlobalInfo.vecPredictedPos = vecPredictedPos;

			return true;
		}
	}

	return false;
}

Vec3 C_Aimbot::EstimateProjectileImpactPosition(C_BaseEntity *pLocal, float pitch, float yaw, float initVel)
{
	Vec3 vecForward = Vec3(), vecRight = Vec3(), vecUp = Vec3();
	Vec3 vecAngles = Vec3(pitch, yaw, 0.0f);
	Math::AngleVectors(vecAngles, &vecForward, &vecRight, &vecUp);

	Vec3 vecSrc = pLocal->GetShootPos();
	vecSrc += vecForward * 16.0f + vecRight * (8.0f * gConVars.cl_flipviewmodels->GetInt() ? -1.0f : 1.0f) + vecUp * -6.0f;

	const float fInitVelScale = 0.9f;
	Vec3 vecVelocity = ((vecForward * initVel) - (vecUp * 200.0f)) * fInitVelScale;

	Vec3 vecPos = vecSrc;
	Vec3 vecLastPos = vecPos;

	Vec3 vecAlongDir = vecForward;
	vecAlongDir.z = 0.0f;
	vecAlongDir.NormalizeInPlace();

	float fAlongVel = Math::FastSqrt(vecVelocity.x * vecVelocity.x + vecVelocity.y * vecVelocity.y);

	C_GameTrace Trace;
	C_TraceFilterWorldAndPropsOnlyOther TraceFilter;
	TraceFilter.pSkip = pLocal;

	const float fGravity = gConVars.sv_gravity->GetFloat();
	const float fTimeStep = 0.01f;
	const float fMaxTime = 1.5f;

	for (float fTime = 0.0f; fTime < fMaxTime; fTime += fTimeStep)
	{
		float fAlong = fAlongVel * fTime;
		float fHeight = vecVelocity.z * fTime - 0.5f * fGravity * fTime * fTime;

		vecPos.x = vecSrc.x + vecAlongDir.x * fAlong;
		vecPos.y = vecSrc.y + vecAlongDir.y * fAlong;
		vecPos.z = vecSrc.z + fHeight;

		Util::TraceHull(vecLastPos, vecPos, Vec3(-2.0f, -2.0f, -2.0f), Vec3(2.0f, 2.0f, 2.0f), MASK_SOLID_BRUSHONLY, &TraceFilter, &Trace);

		if (Trace.DidHit())
			break;

		vecLastPos = vecPos;
	}

	return Trace.vEndPos;
}


void C_Aimbot::Run(C_UserCmd* pCmd)
{
	bSortingByFov = false;

	gGlobalInfo.nCurrentTargetIdx = 0;
	gGlobalInfo.vecPredictedPos = Vec3();

	C_BaseEntity* pLocal = gEntCache.pLocal;
	C_BaseCombatWeapon* pLocalWeapon = gEntCache.pLocalWeapon;
	
	if (pLocal && pLocalWeapon)
	{
		if (!ShouldRun(pLocal, pLocalWeapon)) {
			ResetSmooth();
			return;
		}

		Target_t Target = {};

		if (GetTarget(pLocal, pLocalWeapon, pCmd, Target) && IsAimKeyDown())
		{
			gGlobalInfo.nCurrentTargetIdx = Target.pEntity->GetIndex();

			if (TargetChanged() || WeaponChanged())
				ResetSmooth();

			SetAngles(Target, pCmd, pLocal, pLocalWeapon);

			if (ShouldShoot(pLocal, pLocalWeapon, pCmd, Target))
				pCmd->buttons |= IN_ATTACK;

			if ((pCmd->buttons & IN_ATTACK) && gGlobalInfo.bWeaponCanShoot)
			{
				if (gGlobalInfo.bNoInterpolation && Target.eType == ETargetType::PLAYER)
				{
					pCmd->tick_count = TIME_TO_TICKS(Target.pEntity->flSimulationTime() +
						std::max(gConVars.cl_interp->GetFloat(), gConVars.cl_interp_ratio->GetFloat() / gConVars.cl_updaterate->GetFloat()));
				}

				if (fDrawTracers)
				{
					Vec3 vecPos = bIsCurWeaponProjectile ? gGlobalInfo.vecPredictedPos : Target.vecPos;
					Color_t Color = (bTracerRandomColor ? Util::RandColor(100) : Color_t{ 255, 255, 255, 255 });
					Color.a = static_cast<byte>(fTracersAlpha * 255.0f);
					gInts.DebugOverlay->AddLineOverlayAlpha(pLocal->GetShootPos(), vecPos, Color.r, Color.g, Color.b, Color.a, true, fDrawTracers);
				}
			}
		}

		else ResetSmooth();
	}
}

void C_Aimbot::Paint()
{
	if (!gGlobalInfo.vecPredictedPos.IsZero())
	{
		if (!gInts.Engine->Con_IsVisible() && !gInts.EngineVGui->IsGameUIVisible())
		{
			static const int size = 10;
			Vec3 vecScreen = Vec3();

			if (Math::W2S(gGlobalInfo.vecPredictedPos, vecScreen))
				gDraw.OutlinedRect(static_cast<int>(vecScreen.x - (size / 2)), static_cast<int>(vecScreen.y - (size / 2)), size, size, { 0, 255, 0, 255 });
		}
	}

	if (fDrawAimFov)
	{
		if (auto pLocal{ gEntCache.pLocal })
		{
			if (!gMenu.m_OnAimbotFOV)
			{
				if (!bSortingByFov || !bActive)
					return;

				if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
					return;

				if (gInts.EngineVGui->IsGameUIVisible())
					return;

				if (!pLocal->IsAlive() || pLocal->IsTaunting() || pLocal->IsBonked() || pLocal->IsStunned())
					return;
			}

			else
			{
				if (!gMenu.m_Open)//I broke this shit sorry!!
					return;
			}

			float flR = tanf(DEG2RAD(fAimFov) / 2) / tanf(DEG2RAD((pLocal->IsScoped() && !gVisuals.bNoZoom) ? 30.0f : gVisuals.nCustomFov) / 2) * gScreenSize.w;
			gDraw.OutlinedCircle(gScreenSize.w / 2, gScreenSize.h / 2, flR, 64, { 255, 255, 255, static_cast<byte>(fDrawAimFov * 255.0f) });
		}
	}
}

C_Aimbot gAimbot;
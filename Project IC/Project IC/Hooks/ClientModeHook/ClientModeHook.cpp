#include "ClientModeHook.h"

#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Triggerbot/TriggerBot.h"
#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Features/Misc/Misc.h"

void __stdcall ClientModeHook::OverrideView::Hook(C_ViewSetup* pView)
{
	Table.Original<fn>(index)(gInts.ClientMode, pView);

	gVisuals.DoFov(pView);
}

bool __stdcall ClientModeHook::ShouldDrawViewModel::Hook()
{
	C_BaseEntity* pLocal = gEntCache.pLocal;

	if (pLocal)
	{
		if (pLocal->IsScoped() && gVisuals.bNoScope && gVisuals.bNoZoom)
			return true;
	}

	return Table.Original<fn>(index)(gInts.ClientMode);
}

bool __stdcall ClientModeHook::CreateMove::Hook(float input_sample_frametime, C_UserCmd* pCmd)
{
	fn original = Table.Original<fn>(index);

	if (!pCmd || !pCmd->command_number)
		return original(gInts.ClientMode, input_sample_frametime, pCmd);

	uintptr_t _bp; __asm mov _bp, ebp;
	bool* pSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

	{
		C_BaseEntity* pLocal = gEntCache.pLocal;
		C_BaseCombatWeapon* pLocalWeapon = gEntCache.pLocalWeapon;

		if (pLocal && pLocalWeapon)
		{
			gGlobalInfo.bWeaponCanHeadShot = pLocalWeapon->CanWeaponHeadShot();
			gGlobalInfo.bWeaponCanShoot = pLocalWeapon->CanShoot(pLocal);
			gGlobalInfo.fChargeTime = (gInts.GlobalVars->curtime - pLocalWeapon->GetChargeTime());
		}
	}

	//back these up for pSilent
	Vec3 vecOldAngles = pCmd->viewangles;
	float fOldSideMove = pCmd->sidemove;
	float fOldForwardMove = pCmd->forwardmove;

	gMisc.DoBunnyhop(pCmd);
	gMisc.DoStrafe(pCmd);

	gPrediction.Start(pCmd);
	{
		gMisc.DoEdgeJump(pCmd, gPrediction.nOldFlags);
		gAimbot.Run(pCmd);
		gTriggerBot.Run(pCmd);
	}
	gPrediction.End(pCmd);

	gGlobalInfo.vecUCMDAngles = pCmd->viewangles;

	if (gAimbot.nAimMethod == 3)
	{
		if (gAimbot.bIsCurWeaponProjectile || (gEntCache.pLocalWeapon && gEntCache.pLocalWeapon->GetWeaponID() == TF_WEAPON_KNIFE))
		{
			if ((pCmd->buttons & IN_ATTACK) && gGlobalInfo.bWeaponCanShoot)
				*pSendPacket = false;

			else
			{
				*pSendPacket = true;
				pCmd->viewangles = vecOldAngles;
				pCmd->sidemove = fOldSideMove;
				pCmd->forwardmove = fOldForwardMove;
			}
		}

		static int nChoked = 0;

		if (!*pSendPacket)
		{
			nChoked++;

			if (nChoked > 14) {
				nChoked = 0;
				*pSendPacket = true;
			}
		}

		return false;
	}

	return original(gInts.ClientMode, input_sample_frametime, pCmd);
}

VMT::Table ClientModeHook::Table;
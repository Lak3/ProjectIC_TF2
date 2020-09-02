#include "Misc.h"

void C_Misc::DoBunnyhop(C_UserCmd *pCmd)
{
	if (auto pLocal{ gEntCache.pLocal })
	{
		if (!bBunnyhop              ||
			!pLocal->IsAlive()      ||
			 pLocal->IsSwimming())
			 return;

		static bool bJumpState = false;

		if (pCmd->buttons & IN_JUMP)
		{
			if (!bJumpState && !pLocal->IsOnGround())
				pCmd->buttons &= ~IN_JUMP;

			else if (bJumpState)
				bJumpState = false;
		}

		else if (!bJumpState)
			bJumpState = true;
	}
}

void C_Misc::DoStrafe(C_UserCmd* pCmd)
{
	//I hope this aint too complicated
	if (auto pLocal{ gEntCache.pLocal })
	{
		if (!bAutoStrafe ||
			!pLocal->IsAlive() || 
			 pLocal->IsSwimming() ||
			 pLocal->IsOnGround())
			 return;

		if (pCmd->mousedx > 1 || pCmd->mousedx < -1)
			pCmd->sidemove = pCmd->mousedx > 1 ? 450.f : -450.f;
	}
}

//Very simple, comparing if we are on ground currently and predicted.
void C_Misc::DoEdgeJump(C_UserCmd* pCmd, int nOldFlags)
{
	if (auto pLocal{ gEntCache.pLocal })
	{
		if (!bEdgeJump || !pLocal->IsAlive() || pLocal->IsSwimming())
			return;

		if (!(pLocal->GetFlags() & FL_ONGROUND) && nOldFlags & FL_ONGROUND)
			pCmd->buttons |= IN_JUMP;
	}
}

C_Misc gMisc;
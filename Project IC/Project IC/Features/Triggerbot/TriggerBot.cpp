#include "TriggerBot.h"

bool C_TriggerBot::IsTriggerKeyDown()
{
	switch (nTriggerKey)
	{
		case 0: {
			return true;
		}
		case 1: {
			return (GetAsyncKeyState(VK_SHIFT) & 0x8000);
		}
		case 2: {
			return (GetAsyncKeyState(VK_XBUTTON1) & 0x8000);
		}
		case 3: {
			return (GetAsyncKeyState(VK_XBUTTON2) & 0x8000);
		}

		//...

		default: {
			return false;
		}
	}

	return false;
}

void C_TriggerBot::Run(C_UserCmd *pCmd)
{
	if (!bActive || !IsTriggerKeyDown())
		return;

	gTriggerShoot.Run(pCmd);
	gTriggerStab.Run(pCmd);
}

C_TriggerBot gTriggerBot;
#pragma once
#include "TriggerShoot/TriggerShoot.h"
#include "TriggerStab/TriggerStab.h"

class C_TriggerBot
{
private:
	bool IsTriggerKeyDown();

public:
	bool bActive = false;
	int nTriggerKey = 0; //0 always 1 lshift 2 xbutton1 3 xbutton2

	void Run(C_UserCmd *pCmd);
};

extern C_TriggerBot gTriggerBot;
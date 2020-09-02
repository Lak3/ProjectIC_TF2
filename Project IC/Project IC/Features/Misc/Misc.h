#pragma once
#include "../../SDK/SDK.h"

class C_Misc
{
public:
	bool bBunnyhop = true;
	bool bEdgeJump = false;
	bool bAutoStrafe = false;

	void DoBunnyhop(C_UserCmd *pCmd);
	void DoEdgeJump(C_UserCmd* pCmd, int nOldFlags);
	void DoStrafe(C_UserCmd* pCmd);
};

extern C_Misc gMisc;
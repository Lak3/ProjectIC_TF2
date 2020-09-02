#pragma once
#include "../../SDK/SDK.h"

class C_LocalPrediction
{
private:
	C_MoveData moveData		= { };
	float fOldCurrentTime	= 0.0f;
	float fOldFrameTime		= 0.0f;
	int nOldTickCount		= 0;

private:
	int GetTickbase(C_UserCmd *pCmd, C_BaseEntity *pLocal);

public:
	void Start(C_UserCmd *pCmd);
	void End(C_UserCmd *pCmd);

	int nOldFlags = 0;
};

extern C_LocalPrediction gPrediction;
#include "PredictionHook.h"

void __stdcall PredictionHook::RunCommand::Hook(C_BaseEntity *pEntity, C_UserCmd *pCmd, C_MoveHelper *pMoveHelper)
{
	Table.Original<fn>(index)(gInts.Prediction, pEntity, pCmd, pMoveHelper);

	if (pMoveHelper && !gInts.MoveHelper)
		gInts.MoveHelper = pMoveHelper;

	//...
}

VMT::Table PredictionHook::Table;
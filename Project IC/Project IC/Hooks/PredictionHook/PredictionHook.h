#pragma once
#include "../../SDK/SDK.h"

namespace PredictionHook
{
	extern VMT::Table Table;

	namespace RunCommand
	{
		const int index = 17;
		using fn = void(__thiscall *)(C_Prediction *, C_BaseEntity *, C_UserCmd *, C_MoveHelper *);
		void __stdcall Hook(C_BaseEntity *pEntity, C_UserCmd *pCmd, C_MoveHelper *pMoveHelper);
	}
}
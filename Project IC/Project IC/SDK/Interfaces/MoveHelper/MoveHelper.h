#pragma once
#include "../../Includes/Includes.h"

class C_MoveHelper
{
public:
	void SetHost(C_BaseEntity *host) 
	{
		typedef void(__thiscall *FN)(PVOID, C_BaseEntity *);
		GetVFunc<FN>(this, 0)(this, host);
	}
};
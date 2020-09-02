#pragma once
#include "../../Includes/Includes.h"

class C_BaseClientDLL
{
public:
	C_ClientClass *GetAllClasses()
	{
		typedef C_ClientClass *(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 8)(this);
	}

	bool GetPlayerView(C_ViewSetup &playerView)
	{
		typedef bool (__thiscall *FN)(PVOID, C_ViewSetup &);
		return GetVFunc<FN>(this, 59)(this, playerView);
	}
};

#define CLIENT_DLL_INTERFACE_VERSION "VClient017"
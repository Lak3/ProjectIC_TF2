#pragma once
#include "../../Includes/Includes.h"

class C_Panel
{
public:
	const char *GetName(unsigned int vgui_panel)
	{
		typedef const char *(__thiscall *FN)(PVOID, unsigned int);
		return GetVFunc<FN>(this, 36)(this, vgui_panel);
	}

	void SetMouseInputEnabled(unsigned int panel, bool state)
	{
		typedef void(__thiscall *FN)(void *, int, bool);
		GetVFunc<FN>(this, 32)(this, panel, state);
	}

	void SetTopmostPopup(unsigned int panel, bool state)
	{
		typedef void(__thiscall *FN)(void *, int, bool);
		GetVFunc<FN>(this, 59)(this, panel, state);
	}
};

#define VGUI_PANEL_INTERFACE_VERSION "VGUI_Panel009"
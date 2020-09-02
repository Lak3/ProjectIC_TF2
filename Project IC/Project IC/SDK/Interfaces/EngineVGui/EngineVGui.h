#pragma once
#include "../../Includes/Includes.h"

class C_EngineVGui
{
public:
	virtual					~C_EngineVGui(void) { }
	virtual unsigned int	GetPanel(VGuiPanel_t type) = 0;
	virtual bool			IsGameUIVisible() = 0;
};

#define VENGINE_VGUI_VERSION "VEngineVGui001"
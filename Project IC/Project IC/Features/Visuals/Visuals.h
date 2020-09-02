#pragma once
#include "../../SDK/SDK.h"

class C_Visuals
{
public:
	bool bNoZoom			= true;
	bool bNoScope			= true;
	float nCustomFov		= 110;

	bool DoRemoveScope(int iCurPanel);
	void DoFov(C_ViewSetup *pViewSetup);
};

extern C_Visuals gVisuals;
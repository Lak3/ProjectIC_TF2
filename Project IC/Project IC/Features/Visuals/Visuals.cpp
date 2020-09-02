#include "Visuals.h"
#include "../Menu/Menu.h"

bool C_Visuals::DoRemoveScope(int iCurPanel)
{
	if (!bNoScope) return false;
	return iCurPanel == Hash::HudScope;
}

void C_Visuals::DoFov(C_ViewSetup *pViewSetup)
{
	C_BaseEntity *pLocal = gEntCache.pLocal;

	if (pLocal && pViewSetup)
	{
		if (pLocal->GetObserverMode() == OBS_MODE_FIRSTPERSON)
			return;

		if (!bNoZoom && pLocal->IsScoped())
			return;

		pViewSetup->fov = nCustomFov;

		if (pLocal->IsAlive())
			pLocal->SetFov(nCustomFov);
	}
}

C_Visuals gVisuals;
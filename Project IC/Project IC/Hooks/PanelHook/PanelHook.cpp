#include "PanelHook.h"

#include "../../Features/Visuals/Visuals.h"

void __stdcall PanelHook::PaintTraverse::Hook(unsigned int vgui_panel, bool force_repaint, bool allow_force)
{
	int nCurPanel = Hash::String(gInts.Panel->GetName(vgui_panel));

	if (gVisuals.DoRemoveScope(nCurPanel)) 
		return;

	Table.Original<fn>(index)(gInts.Panel, vgui_panel, force_repaint, allow_force);

	if (gGlobalInfo.m_nFocusOverlay == 0) {
		if (nCurPanel == Hash::FocusOverlayPanel)
			gGlobalInfo.m_nFocusOverlay = vgui_panel;
	}

	//...
}

VMT::Table PanelHook::Table;
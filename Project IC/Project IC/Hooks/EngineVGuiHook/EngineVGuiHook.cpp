#include "EngineVGuiHook.h"

#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Menu/Menu.h"
#include "../../Features/Visuals/ESP/ESP.h"
#include "../../Features/Visuals/SpectatorList/SpectatorList.h"
#include "../../Features/Visuals/SpyWarning/SpyWarning.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Features/Visuals/Radar/Radar.h"

#include "../../SDK/Includes/icons.h"


void __stdcall EngineVGuiHook::Paint::Hook(int mode)
{
	if (!gScreenSize.w || !gScreenSize.h)
		gScreenSize.Update();

	static auto StartDrawing = reinterpret_cast<void(__thiscall*)(void*)>(gPattern.Find("vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14"));
	static auto FinishDrawing = reinterpret_cast<void(__thiscall*)(void*)>(gPattern.Find("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00"));

	//HACK: for some reason we need to do this
	{
		static bool bInitIcons = false;

		if (!bInitIcons)
		{
			for (int nIcon = 0; nIcon < ICONS::TEXTURE_AMOUNT; nIcon++)
				gDraw.Texture(-200, 0, 18, 18, nIcon);

			bInitIcons = true;
		}
	}

	Table.Original<fn>(index)(gInts.EngineVGui, mode);

	if (mode & PAINT_UIPANELS)
	{
		//let's update WorldToProjection VMatrix here
		{
			C_ViewSetup ViewSetup = {};

			if (gInts.Client->GetPlayerView(ViewSetup))
			{
				VMatrix WorldToView = {}, ViewToProjection = {}, WorldToPixels = {};
				gInts.RenderView->GetMatricesForView(ViewSetup, &WorldToView, &ViewToProjection, &gGlobalInfo.WorldToProjection, &WorldToPixels);
			}
		}

		StartDrawing(gInts.Surface);
		{
			gESP.Run();
			gSpectatorList.Run();
			gSpyWarning.Run();
			gAimbot.Paint();
			gMenu.Run();
			gRadar.Paint();
		}
		FinishDrawing(gInts.Surface);
	}
}

VMT::Table EngineVGuiHook::Table;
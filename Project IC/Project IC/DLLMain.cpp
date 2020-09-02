#include "Hooks/Hooks.h"

#include "Features/Visuals/Chams/Chams.h"

DWORD WINAPI MainThread(LPVOID lpParam)
{
	gSteam.Init();
	gInts.Init();
	gNetVars.Init();
	gHooks.Init();
	gConVars.Init();
	gChams.InitMaterials();
	gDraw.InitFonts
	({
		//FONT_ESP
		{ 0x0, "Tahoma", 12, 0, FONTFLAG_NONE },
		//FONT_ESP_OUTLINED
		{ 0x0, "Tahoma", 12, 0, FONTFLAG_OUTLINE },

		//FONT_ESP_NAME
		{ 0x0, "Tahoma", 13, 0, FONTFLAG_NONE },
		//FONT_ESP_NAME_OUTLINED
		{ 0x0, "Tahoma", 13, 0, FONTFLAG_OUTLINE },

		//FONT_ESP_COND
		{ 0x0, "Consolas", 10, 0, FONTFLAG_NONE },
		//FONT_ESP_COND_OUTLINED
		{ 0x0, "Consolas", 10, 0, FONTFLAG_OUTLINE },

		//FONT_ESP_PICKUPS
		{ 0x0, "Tahoma", 11, 0, FONTFLAG_NONE },
		//FONT_ESP_PICKUPS_OUTLINED
		{ 0x0, "Tahoma", 11, 0, FONTFLAG_OUTLINE },

		//FONT_DEBUG
		{ 0x0, "Arial", 16, 0, FONTFLAG_OUTLINE },
		//FONT_MENU
		{ 0x0, "Segoe UI", 13, 0, FONTFLAG_ANTIALIAS },
		//FONT_MENU_NAME
		{ 0x0, "Segoe UI", 20, 0, FONTFLAG_ANTIALIAS }
	});

	//CSteamID csLocal = gSteam.User->GetSteamID();
	//csLocal.Render() = Steam ID
	//csLocal.SteamRender() = Steam ID3

	//Stuck at this, as long as "panic" key is not pressed.
	while (!(GetAsyncKeyState(VK_F11) & 0x8000))
		std::this_thread::sleep_for(std::chrono::seconds(1));
	
	//releasing them in DLL DETACH just crashed the game on a normal exit
	//when our dll unloads after the game and baseclass is inaccessible
	gHooks.Release();
	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH: {
			if (HANDLE hMain = CreateThread(0, 0, MainThread, hinstDLL, 0, 0)) CloseHandle(hMain);
			break;
		}

		default: break;
	}

	return TRUE;
}
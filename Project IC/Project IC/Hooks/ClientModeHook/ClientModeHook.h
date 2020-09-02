#pragma once
#include "../../SDK/SDK.h"

namespace ClientModeHook
{
	extern VMT::Table Table;

	namespace OverrideView
	{
		const int index = 16;
		using fn = void(__thiscall *)(C_ClientModeShared *, C_ViewSetup *);
		void __stdcall Hook(C_ViewSetup *pView);
	}

	namespace ShouldDrawViewModel
	{
		const int index = 24;
		using fn = bool(__thiscall *)(C_ClientModeShared *);
		bool __stdcall Hook();
	}

	namespace CreateMove
	{
		const int index = 21;
		using fn = bool(__thiscall *)(C_ClientModeShared *, float, C_UserCmd*);
		bool __stdcall Hook(float input_sample_frametime, C_UserCmd *pCmd);
	}
}
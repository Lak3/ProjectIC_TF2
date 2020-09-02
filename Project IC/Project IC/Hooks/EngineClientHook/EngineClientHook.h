#pragma once
#include "../../SDK/SDK.h"

namespace EngineClientHook
{
	extern VMT::Table Table;

	namespace IsPlayingTimeDemo
	{
		const int index = 78;
		using fn = bool(__thiscall *)(C_EngineClient *);
		bool __stdcall Hook();
	}
}
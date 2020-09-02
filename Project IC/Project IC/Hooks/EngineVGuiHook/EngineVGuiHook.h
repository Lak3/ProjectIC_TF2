#pragma once

#include "../../SDK/SDK.h"

namespace EngineVGuiHook
{
	extern VMT::Table Table;

	namespace Paint
	{
		using fn = void(__thiscall *)(C_EngineVGui *, int);
		constexpr int index = 13;
		void __stdcall Hook(int mode);
	}
}
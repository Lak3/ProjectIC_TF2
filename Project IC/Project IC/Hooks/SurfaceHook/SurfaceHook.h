#pragma once
#include "../../SDK/SDK.h"

namespace SurfaceHook
{
	extern VMT::Table Table;

	namespace OnScreenSizeChanged
	{
		const int index = 111;
		using fn = void(__thiscall *)(C_Surface *, int, int);
		void __stdcall Hook(int OldWidht, int OldHeight);
	}

	namespace LockCursor
	{
		const int index = 62;
		using fn = void(__thiscall *)(C_Surface *);
		void __stdcall Hook();
	}
}
#include "SurfaceHook.h"

#include "../../Features/Menu/Menu.h"

void __stdcall SurfaceHook::OnScreenSizeChanged::Hook(int OldWidht, int OldHeight)
{
	Table.Original<fn>(index)(gInts.Surface, OldWidht, OldHeight);

	gScreenSize.Update();
	gDraw.ReloadFonts();
}

void __stdcall SurfaceHook::LockCursor::Hook()
{
	gMenu.m_Open ? gInts.Surface->UnlockCursor() : Table.Original<fn>(index)(gInts.Surface);
}

VMT::Table SurfaceHook::Table;
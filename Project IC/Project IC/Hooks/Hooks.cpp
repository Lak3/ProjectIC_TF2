#include "Hooks.h"

void C_Hooks::Init()
{
	if (gInts.Client)
	{
		using namespace ClientHook;

		Table.Init(gInts.Client);
		Table.Hook(PreEntity::index, &PreEntity::Hook);
		Table.Hook(PostEntity::index, &PostEntity::Hook);
		Table.Hook(ShutDown::index, &ShutDown::Hook);
		Table.Hook(FrameStageNotify::index, &FrameStageNotify::Hook);
	}

	if (gInts.ClientMode)
	{
		using namespace ClientModeHook;

		Table.Init(gInts.ClientMode);
		Table.Hook(OverrideView::index, &OverrideView::Hook);
		Table.Hook(ShouldDrawViewModel::index, &ShouldDrawViewModel::Hook);
		Table.Hook(CreateMove::index, &CreateMove::Hook);
	}

	if (gInts.Prediction)
	{
		using namespace PredictionHook;

		Table.Init(gInts.Prediction);
		Table.Hook(RunCommand::index, &RunCommand::Hook);
	}

	if (gInts.Surface)
	{
		using namespace SurfaceHook;

		Table.Init(gInts.Surface);
		Table.Hook(OnScreenSizeChanged::index, &OnScreenSizeChanged::Hook);
		Table.Hook(LockCursor::index, &LockCursor::Hook);
	}

	if (gInts.Panel)
	{
		using namespace PanelHook;
		
		Table.Init(gInts.Panel);
		Table.Hook(PaintTraverse::index, &PaintTraverse::Hook);
	}

	if (gInts.EngineVGui)
	{
		using namespace EngineVGuiHook;

		Table.Init(gInts.EngineVGui);
		Table.Hook(Paint::index, &Paint::Hook);
	}

	if (gInts.Engine)
	{
		using namespace EngineClientHook;

		Table.Init(gInts.Engine);
		Table.Hook(IsPlayingTimeDemo::index, &IsPlayingTimeDemo::Hook);
	}

	if (gInts.ModelRender)
	{
		using namespace ModelRenderHook;

		Table.Init(gInts.ModelRender);
		Table.Hook(DrawModelExecute::index, &DrawModelExecute::Hook);
	}

  //if (gInts.WinApi)
	{
		while (!Window)
			Window = FindWindowA(0, "Team Fortress 2");

		WndProcHook::WndProc = (WNDPROC)SetWindowLongPtr(Window, GWL_WNDPROC, (LONG_PTR)WndProcHook::Hook);
	}
}

void C_Hooks::Release()
{
	ClientHook::Table.RestoreTable();
	ClientModeHook::Table.RestoreTable();
	PredictionHook::Table.RestoreTable();
	SurfaceHook::Table.RestoreTable();
	PanelHook::Table.RestoreTable();
	EngineVGuiHook::Table.RestoreTable();
	EngineClientHook::Table.RestoreTable();
	ModelRenderHook::Table.RestoreTable();

	SetWindowLongPtr(Window, GWL_WNDPROC, (LONG_PTR)WndProcHook::WndProc);
}

C_Hooks gHooks;
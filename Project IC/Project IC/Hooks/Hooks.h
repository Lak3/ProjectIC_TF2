#pragma once

#include "ClientHook/ClientHook.h"
#include "ClientModeHook/ClientModeHook.h"
#include "PredictionHook/PredictionHook.h"
#include "SurfaceHook/SurfaceHook.h"
#include "PanelHook/PanelHook.h"
#include "EngineVGuiHook/EngineVGuiHook.h"
#include "EngineClientHook/EngineClientHook.h"
#include "ModelRenderHook/ModelRenderHook.h"
#include "WndProcHook/WndProcHook.h"

class C_Hooks
{
private:
	HWND Window = 0;

public:
	void Init();
	void Release();
};

extern C_Hooks gHooks;
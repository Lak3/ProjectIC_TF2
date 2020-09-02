#pragma once

#include "EngineClient/EngineClient.h"
#include "BaseClientDLL/BaseClientDLL.h"
#include "ClientDLLSharedAppSystems/ClientDLLSharedAppSystems.h"
#include "ClientModeShared/ClientModeShared.h"
#include "GlobalVarsBase/GlobalVarsBase.h"
#include "ClientEntityList/ClientEntityList.h"
#include "ModelInfo/ModelInfo.h"
#include "DebugOverlay/DebugOverlay.h"
#include "EngineTrace/EngineTrace.h"
#include "GameMovement/GameMovement.h"
#include "MoveHelper/MoveHelper.h"
#include "ConVar/ConVar.h"
#include "Prediction/Prediction.h"
#include "Panel/Panel.h"
#include "Surface/Surface.h"
#include "EngineVGui/EngineVGui.h"
#include "NetChannel/NetChannel.h"
#include "Steam/Steamworks.h"
#include "Steam/SteamTypes.h"
#include "GameEvent/GameEvent.h"
#include "Material/Material.h"
#include "ModelRender/ModelRender.h"
#include "ViewRender/ViewRender.h"
#include "RenderView/RenderView.h"

class C_SteamInterfaces
{
public:
	ISteamFriends002* Friends002 = nullptr;
	ISteamFriends015* Friends015 = nullptr;
	ISteamApps006* SteamApps = nullptr;
	ISteamUserStats011* UserStats = nullptr;
	ISteamClient017* Client = nullptr;
	ISteamUser017* User = nullptr;
	ISteamUtils007* Utils007 = nullptr;

	void Init();
};

class C_Render {
public:
	const VMatrix &WorldToScreenMatrix() {
		typedef const VMatrix &(__thiscall *FN)(PVOID);
		return GetVFunc<FN>(this, 13)(this);
	}
};

class C_RenderView
{
public:
	void SetBlend(float flBlend) {
		typedef void(__thiscall* FN)(PVOID, float);
		return GetVFunc<FN>(this, 4)(this, flBlend);
	}
	void SetColorModulation(const float* flColor) {
		typedef void(__thiscall* FN)(PVOID, const float*);
		return GetVFunc<FN>(this, 6)(this, flColor);
	}
	void GetMatricesForView(const C_ViewSetup &view, VMatrix *pWorldToView, VMatrix *pViewToProjection, VMatrix *pWorldToProjection, VMatrix *pWorldToPixels) {
		typedef void(__thiscall *FN)(PVOID, const C_ViewSetup &, VMatrix *, VMatrix *, VMatrix *, VMatrix *);
		GetVFunc<FN>(this, 50)(this, view, pWorldToView, pViewToProjection, pWorldToProjection, pWorldToPixels);
	}
};

#define VENGINE_RENDERVIEW_INTERFACE_VERSION "VEngineRenderView014"

class C_Interfaces
{
public:
	C_BaseClientDLL* Client						= nullptr;
	C_ClientDLLSharedAppSystems* ClientShared	= nullptr;
	C_ClientModeShared* ClientMode				= nullptr;
	C_EngineClient* Engine						= nullptr;
	C_Panel* Panel								= nullptr;
	C_Surface* Surface							= nullptr;
	C_ClientEntityList* EntityList				= nullptr;
	C_ModelInfoClient* ModelInfo				= nullptr;
	C_EngineTrace* EngineTrace					= nullptr;
	C_Prediction* Prediction					= nullptr;
	C_GameMovement* GameMovement				= nullptr;
	C_MoveHelper* MoveHelper					= nullptr;
	ICvar* CVars								= nullptr; //hello, I am ICvar and I do not associate with C_ niggas
	C_GlobalVarsBase* GlobalVars				= nullptr;
	C_EngineVGui *EngineVGui					= nullptr;
	int32_t *RandomSeed							= nullptr; //What about me?!?!
	void *DemoPlayer							= nullptr; //hHaha we're getting STRONG!!1
	C_Render *EngineRenderer					= nullptr;
	//C_RenderView *RenderView					= nullptr;
	IDebugOverlay* DebugOverlay                 = nullptr;
	C_GameEventManager* GameEvent               = nullptr;
	C_ModelRender* ModelRender					= nullptr;
	C_MaterialSystem* MatSystem					= nullptr;
	IViewRender* ViewRender						= nullptr;
	IVRenderView* RenderView					= nullptr;

	void Init();
};

extern C_Interfaces gInts;
extern C_SteamInterfaces gSteam;
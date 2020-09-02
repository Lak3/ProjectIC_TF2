#include "ModelRenderHook.h"

#include "../../Features/Visuals/Chams/Chams.h"

constexpr float flDefaultColor[3] = { 1, 1, 1 };

void __stdcall ModelRenderHook::DrawModelExecute::Hook(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
{
	const char* szModel = gInts.ModelInfo->GetModelName(pInfo.m_pModel);

	if (!szModel) 
		return;

	if (gChams.bRemoveHands && strstr(szModel, "arms"))
		return;

	gChams.Paint(pState, pInfo, szModel);

	Table.Original<fn>(index)(gInts.ModelRender, pState, pInfo, pBoneToWorld);

	gInts.RenderView->SetBlend(1);
	gInts.RenderView->SetColorModulation(flDefaultColor);
	gInts.ModelRender->ForcedMaterialOverride(nullptr);
}

VMT::Table ModelRenderHook::Table;
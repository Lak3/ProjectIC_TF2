#pragma once

#include "../../Includes/Includes.h"

class C_ModelRender
{
public:
	void ForcedMaterialOverride(IMaterial* mat, OverrideType_t type = OverrideType_t::OVERRIDE_NORMAL)
	{
		typedef void(__thiscall* FN)(PVOID, IMaterial*, OverrideType_t);
		return GetVFunc<FN>(this, 1)(this, mat, type);
	}

	void DrawModelExecute(const DrawModelState_t* pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld)
	{
		typedef void(__thiscall* FN)(PVOID, const DrawModelState_t*, const ModelRenderInfo_t&, matrix3x4*);
		return GetVFunc<FN>(this, 19)(this, pState, pInfo, pBoneToWorld);
	}
};

#define VENGINE_MODELRENDER_INTERFACE "VEngineModel016"
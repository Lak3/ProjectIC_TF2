#pragma once

#include "../../SDK/SDK.h"

namespace ModelRenderHook
{
	extern VMT::Table Table;

	namespace DrawModelExecute
	{
		using fn = void(__thiscall*)(C_ModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*);
		constexpr int index = 19;
		void __stdcall Hook(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4* pBoneToWorld);
	}
}
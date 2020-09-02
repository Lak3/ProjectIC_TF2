#pragma once

#include "Main/BaseEntity/BaseEntity.h"
#include "Main/BaseCombatWeapon/BaseCombatWeapon.h"
#include "Main/BaseObject/BaseObject.h"

#include "Main/DrawUtils/DrawUtils.h"
#include "Main/EntityCache/EntityCache.h"
#include "Main/VisCheck/VisCheck.h"
#include "Main/GlobalInfo/GlobalInfo.h"
#include "Main/ConVars/ConVars.h"

#define TIME_TO_TICKS( dt )	( (int)( 0.5f + (float)(dt) / gInts.GlobalVars->interval_per_tick ) )
#define TICKS_TO_TIME( t )	( gInts.GlobalVars->interval_per_tick * ( t ) )
#define TICK_INTERVAL		( gInts.GlobalVars->interval_per_tick )

namespace Util
{
	inline void TraceHull(const Vec3 &vecStart, const Vec3 &vecEnd, const Vec3 &vecHullMin, const Vec3 &vecHullMax,
		unsigned int nMask, C_TraceFilter *pFilter, C_GameTrace *pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd, vecHullMin, vecHullMax);
		gInts.EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

	inline int RandInt(int min, int max) {
		std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distr(min, max);
		return distr(gen);
	}

	inline Color_t RandColor(byte min = 0, bool bRandAlpha = false)
	{
		byte r = static_cast<byte>(RandInt(0, 255));
		byte g = static_cast<byte>(RandInt(0, 255));
		byte b = static_cast<byte>(RandInt(0, 255));
		byte a = static_cast<byte>(bRandAlpha ? RandInt(0, 255) : 255);

		if (min)
		{
			r = std::clamp(r, min, static_cast<byte>(255));
			g = std::clamp(g, min, static_cast<byte>(255));
			b = std::clamp(b, min, static_cast<byte>(255));
			a = std::clamp(a, min, static_cast<byte>(255));
		}

		return Color_t{r, g, b, a};
	}
}
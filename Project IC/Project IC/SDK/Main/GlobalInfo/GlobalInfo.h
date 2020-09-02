#pragma once
#include "../BaseEntity/BaseEntity.h"

struct GlobalInfo_t
{
	//vars
	int nCurrentTargetIdx = 0;
	bool bWeaponCanHeadShot = false;
	bool bWeaponCanShoot = false;
	float fChargeTime = 0.0f;
	Vec3 vecUCMDAngles = Vec3();

	//temporary place for some features I guess
	bool bNoInterpolation = true;
	Vec3 vecPredictedPos = Vec3();

	//Fuck
	bool m_bDrawMenu = false;
	int m_nFocusOverlay = 0;
	bool m_bPPUCP = true;

	VMatrix WorldToProjection = {};
};

inline GlobalInfo_t gGlobalInfo;
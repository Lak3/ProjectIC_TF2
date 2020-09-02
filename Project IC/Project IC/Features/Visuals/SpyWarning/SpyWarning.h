#pragma once
#include "../../../SDK/SDK.h"

class C_SpyWarning
{
private:
	bool ShouldRun(C_BaseEntity *pLocal);
	bool IsEntityValid(C_BaseEntity *pEntity);
	bool CanBackstab(const Vec3 &vecSrc, const Vec3 &vecDst, Vec3 vecWSCDelta);
	void DrawArrowTo(const Vec3 &vecFromPos, const Vec3 &vecToPos);

	std::vector<Vec3> Spies;

public:
	bool bActive = true;
	void Run();
};

extern C_SpyWarning gSpyWarning;
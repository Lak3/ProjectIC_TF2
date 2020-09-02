#include "SpyWarning.h"

Color_t ColorSpyWarning = { 199, 236, 238, 255 };
int nOffsetUp = 100;

#define MAX_DISTANCE 400.0f
#define MAX_DISTANCE_HEAT 200.0f

bool C_SpyWarning::ShouldRun(C_BaseEntity *pLocal)
{
	if (!bActive)
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	if (!pLocal->IsAlive() || pLocal->IsStunned())
		return false;

	return true;
}

bool C_SpyWarning::IsEntityValid(C_BaseEntity *pEntity)
{
	if (!pEntity || !pEntity->IsAlive() || pEntity->GetClassNum() != CLASS_SPY)
		return false;

	if (pEntity->IsCloaked())
		return false;

	return true;
}

bool C_SpyWarning::CanBackstab(const Vec3 &vecSrc, const Vec3 &vecDst, Vec3 vecWSCDelta)
{
	vecWSCDelta.z = 0;
	vecWSCDelta.NormalizeInPlace();

	Vec3 vecEyeSpy = Vec3();
	Math::AngleVectors(vecSrc, &vecEyeSpy);
	vecEyeSpy.z = 0;
	vecEyeSpy.NormalizeInPlace();

	Vec3 vecEyeVictim = Vec3();
	Math::AngleVectors(vecDst, &vecEyeVictim);
	vecEyeVictim.z = 0;
	vecEyeVictim.NormalizeInPlace();

	if (vecWSCDelta.Dot(vecEyeVictim) <= 0.01f)
		return false;

	if (vecWSCDelta.Dot(vecEyeSpy) <= 0.5f)
		return false;

	if (vecEyeSpy.Dot(vecEyeVictim) <= -0.3f)
		return false;

	return true;
}

void C_SpyWarning::DrawArrowTo(const Vec3 &vecFromPos, const Vec3 &vecToPos)
{
	//credits to pleb

	auto GetClockwiseAngle = [&](const Vec3 &vecViewAngle, const Vec3 &vecAimAngle) -> float
	{
		Vec3 vecAngle = Vec3();
		Math::AngleVectors(vecViewAngle, &vecAngle);
		Vec3 vecAim = Vec3();
		Math::AngleVectors(vecAimAngle, &vecAim);
		return -atan2(vecAngle.x * vecAim.y - vecAngle.y * vecAim.x, vecAngle.x * vecAim.x + vecAngle.y * vecAim.y);
	};

	auto MapFloat = [&](float x, float in_min, float in_max, float out_min, float out_max) -> float {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	};

	Vec3 vecAngleTo = Math::CalcAngle(vecFromPos, vecToPos);
	Vec3 vecViewAngle = gInts.Engine->GetViewAngles();

	const float deg = GetClockwiseAngle(vecViewAngle, vecAngleTo);
	const float xrot = cos(deg - PI / 2);
	const float yrot = sin(deg - PI / 2);
	
	const float x1 = (20.0f + 5.0f) * xrot;
	const float y1 = (20.0f + 5.0f) * yrot;
	const float x2 = (20.0f + 15.0f) * xrot;
	const float y2 = (20.0f + 15.0f) * yrot;
	
	constexpr float arrow_angle = DEG2RAD(90.0f);
	constexpr float arrow_lenght = 10.0f;
	
	const Vec3 line{ x2 - x1, y2 - y1, 0.0f };
	const float length = line.Lenght();
	
	const float fpoint_on_line = arrow_lenght / (atanf(arrow_angle) * length);
	const Vec3 point_on_line = Vec3(x2, y2, 0) + (line * fpoint_on_line * -1.0f);
	const Vec3 normal_vector{ -line.y, line.x, 0.0f };
	const Vec3 normal = Vec3(arrow_lenght, arrow_lenght, 0.0f) / (length * 2);
	
	const Vec3 rotation = normal * normal_vector;
	const Vec3 left = point_on_line + rotation;
	const Vec3 right = point_on_line - rotation;

	float cx = static_cast<float>(gScreenSize.w / 2);
	float cy = static_cast<float>((gScreenSize.h / 2) - nOffsetUp);

	gDraw.Line(cx + x2, cy + y2, cx + left.x, cy + left.y, ColorSpyWarning);
	gDraw.Line(cx + x2, cy + y2, cx + right.x, cy + right.y, ColorSpyWarning);

	float fMap = std::clamp(MapFloat(vecFromPos.DistTo(vecToPos), MAX_DISTANCE, MAX_DISTANCE_HEAT, 0.0f, 1.0f), 0.0f, 1.0f);
	Color_t HeatColor = { 255, 0, 0, static_cast<byte>(fMap * 255.0f) };

	gDraw.Line(cx + x2, cy + y2, cx + left.x, cy + left.y, HeatColor);
	gDraw.Line(cx + x2, cy + y2, cx + right.x, cy + right.y, HeatColor);
}

void C_SpyWarning::Run()
{
	static bool bPlayed = false;

	if (const auto pLocal = gEntCache.pLocal)
	{
		if (!ShouldRun(pLocal))
			return;
		
		Vec3 vecLocalPos = pLocal->GetWorldSpaceCenter();
		const auto Enemies = gEntCache.GetGroup(GroupType_t::PLAYERS_ENEMIES);

		Spies.clear();

		for (const auto Enemy : Enemies)
		{
			//spy with a knife out only filter
			if (!IsEntityValid(Enemy))
				continue;

			Vec3 vecEnemyPos = Enemy->GetWorldSpaceCenter();

			if (vecLocalPos.DistTo(vecEnemyPos) > MAX_DISTANCE)
				continue;

			Vec3 vecAngleToEnemy = Math::CalcAngle(vecLocalPos, vecEnemyPos);
			float fFovToEnemy = Math::CalcFov(gInts.Engine->GetViewAngles(), vecAngleToEnemy);

			if (fFovToEnemy < 70.0f)
				continue;

			Spies.push_back(vecEnemyPos);
		}

		if (Spies.empty()) {
			bPlayed = false;
			return;
		}

		if (!bPlayed)
		{
			gInts.Engine->ClientCmd_Unrestricted("voicemenu 1 1");
			bPlayed = true;
		}

		static const int size = 40;
		gDraw.Texture(((gScreenSize.w / 2) - (size / 2)), (((gScreenSize.h / 2) - nOffsetUp) - (size / 2)), size, size, 8);

		for (const auto &Spy : Spies)
		{
			DrawArrowTo(vecLocalPos, Spy);
		}
	}
}

C_SpyWarning gSpyWarning;
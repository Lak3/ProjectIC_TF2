#include "VisCheck.h"

bool C_TraceFilterHitscan::ShouldHitEntity(void* pEntityHandle, int nContentsMask)
{
	C_BaseEntity* pEntity = reinterpret_cast<C_BaseEntity*>(pEntityHandle);

	switch (pEntity->GetClientClass()->iClassID)
	{
	case 55:
	case 64:
	case 117:
	case 225: {
		return false;
	}
	}

	return (pEntityHandle != pSkip);
}

TraceType_t C_TraceFilterHitscan::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool C_TraceFilterWorldAndPropsOnly::ShouldHitEntity(void *pEntityHandle, int nContentsMask)
{
	C_BaseEntity *pEntity = reinterpret_cast<C_BaseEntity *>(pEntityHandle);

	switch (pEntity->GetClientClass()->iClassID)
	{
		case 55:
		case 64:
		case 117:
		case 225:
		case CTFPlayer:
		case CTFAmmoPack:
		case CTFProjectile_Arrow:
		case CTFProjectile_BallOfFire:
		case CTFProjectile_Cleaver:
		case CTFProjectile_Jar:
		case CTFProjectile_JarMilk:
		case CTFProjectile_EnergyBall:
		case CTFProjectile_EnergyRing:
		case CTFProjectile_Flare:
		case CTFProjectile_HealingBolt:
		case CTFProjectile_MechanicalArmOrb:
		case CTFProjectile_Rocket:
		case CTFProjectile_SentryRocket:
		case CTFGrenadePipebombProjectile: {
			return false;
		}
	}

	return !(pEntityHandle == pSkip);
}

TraceType_t C_TraceFilterWorldAndPropsOnly::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool C_TraceFilterWorldAndPropsOnlyOther::ShouldHitEntity(void *pEntityHandle, int nContentsMask)
{
	C_BaseEntity *pEntity = reinterpret_cast<C_BaseEntity *>(pEntityHandle);

	switch (pEntity->GetClientClass()->iClassID)
	{
		case 55:
		case 64:
		case 117:
		case 225:
		//case CTFPlayer:
		case CTFAmmoPack:
		case CTFProjectile_Arrow:
		case CTFProjectile_BallOfFire:
		case CTFProjectile_Cleaver:
		case CTFProjectile_Jar:
		case CTFProjectile_JarMilk:
		case CTFProjectile_EnergyBall:
		case CTFProjectile_EnergyRing:
		case CTFProjectile_Flare:
		case CTFProjectile_HealingBolt:
		case CTFProjectile_MechanicalArmOrb:
		case CTFProjectile_Rocket:
		case CTFProjectile_SentryRocket:
		case CTFGrenadePipebombProjectile: {
			return false;
		}
	}

	return !(pEntityHandle == pSkip);
}

TraceType_t C_TraceFilterWorldAndPropsOnlyOther::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

//=======================================================================================================

bool C_VisCheck::Pos(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vec3& from, const Vec3& to)
{
	Ray_t ray;
	ray.Init(from, to);

	const uint32_t mask = (MASK_SHOT | CONTENTS_GRATE);

	C_TraceFilterHitscan filter;
	filter.pSkip = pSkip;

	C_GameTrace trace;
	gInts.EngineTrace->TraceRay(ray, mask, &filter, &trace);

	return ((trace.entity && trace.entity == pEntity) || trace.flFraction > 0.99f);
}

bool C_VisCheck::PosHitboxId(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vec3& from, const Vec3& to, int nHitbox)
{
	Ray_t ray;
	ray.Init(from, to);

	const uint32_t mask = (MASK_SHOT | CONTENTS_GRATE);

	C_TraceFilterHitscan filter;
	filter.pSkip = pSkip;

	C_GameTrace trace;
	gInts.EngineTrace->TraceRay(ray, mask, &filter, &trace);

	return (trace.entity && trace.entity == pEntity && trace.hitbox == nHitbox);
}

bool C_VisCheck::PosHitboxIdOut(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vec3& from, const Vec3& to, int& nHitboxOut)
{
	Ray_t ray;
	ray.Init(from, to);

	const uint32_t mask = (MASK_SHOT | CONTENTS_GRATE);

	C_TraceFilterHitscan filter;
	filter.pSkip = pSkip;

	C_GameTrace trace;
	gInts.EngineTrace->TraceRay(ray, mask, &filter, &trace);

	if (trace.entity && trace.entity == pEntity)
	{
		nHitboxOut = trace.hitbox;
		return true;
	}

	return false;
}

bool C_VisCheck::PosFraction(C_BaseEntity *pSkip, const Vec3 &from, const Vec3 &to)
{
	Ray_t ray;
	ray.Init(from, to);

	const uint32_t mask = (MASK_SHOT | CONTENTS_GRATE);

	C_TraceFilterWorldAndPropsOnlyOther filter;
	filter.pSkip = pSkip;

	C_GameTrace trace;
	gInts.EngineTrace->TraceRay(ray, mask, &filter, &trace);

	return (trace.flFraction > 0.99f);
}

C_VisCheck gVisCheck;
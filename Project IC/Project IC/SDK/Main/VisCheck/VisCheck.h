#pragma once
#include "../BaseEntity/BaseEntity.h"

class C_TraceFilterHitscan : public C_TraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntityHandle, int nContentsMask);
	virtual TraceType_t	GetTraceType() const;
	C_BaseEntity* pSkip = nullptr;
};

class C_TraceFilterWorldAndPropsOnly : public C_TraceFilter
{
public:
	virtual bool ShouldHitEntity(void *pEntityHandle, int nContentsMask);
	virtual TraceType_t	GetTraceType() const;
	C_BaseEntity *pSkip = nullptr;
};

class C_TraceFilterWorldAndPropsOnlyOther : public C_TraceFilter
{
public:
	virtual bool ShouldHitEntity(void *pEntityHandle, int nContentsMask);
	virtual TraceType_t	GetTraceType() const;
	C_BaseEntity *pSkip = nullptr;
};

class C_VisCheck
{
public:
	bool Pos(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vec3& from, const Vec3& to);
	bool PosHitboxId(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vec3& from, const Vec3& to, int nHitbox);
	bool PosHitboxIdOut(C_BaseEntity* pSkip, C_BaseEntity* pEntity, const Vec3& from, const Vec3& to, int& nHitboxOut);
	bool PosFraction(C_BaseEntity *pSkip, const Vec3 &from, const Vec3 &to);
};

extern C_VisCheck gVisCheck;
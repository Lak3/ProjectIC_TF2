#pragma once
#include "../BaseEntity/BaseEntity.h"

class C_BaseCombatWeapon : public C_BaseEntity
{
public:
	WeaponData_t GetWeaponData();
	int GetWeaponID();
	int GetClip1();
	int GetClip2();
	int GetSlot();
	float GetChargeTime();
	float GetChargeDamage();
	int GetItemDefinitionIndex();
	float GetLastFireTime();
	float GetSwingRange(C_BaseEntity *pLocal);
	bool DoSwingTrace(C_GameTrace &Trace);
	Vec3 &GetBulletSpread();
	int GetDamageType();
	bool CanFireCriticalShot(bool bHeadShot = false);
	bool CanWeaponHeadShot();
	bool WillCrit();
	float GetNextPrimaryAttack();
	float GetNextSecondaryAttack();
	bool CanShoot(C_BaseEntity *pLocal);
	bool CanSecondaryAttack(C_BaseEntity* pLocal);
};
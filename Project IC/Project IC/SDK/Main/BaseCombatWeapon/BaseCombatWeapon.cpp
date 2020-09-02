#include "BaseCombatWeapon.h"
#include "../ConVars/ConVars.h"

WeaponData_t C_BaseCombatWeapon::GetWeaponData()
{
	using Data = CTFWeaponInfo * (__cdecl *)(int);
	static int offset = gPattern.Find("client.dll", "55 8B EC 66 8B ? ? 66 3B 05 ? ? ? ? 73");
	static auto get_tf_weapon_data_fn = reinterpret_cast<Data>(offset);
	return get_tf_weapon_data_fn(GetWeaponID())->m_WeaponData[0];
}

int C_BaseCombatWeapon::GetWeaponID()
{
	typedef int(__thiscall *FN)(void *);
	return GetVFunc<FN>(this, 377)(this);
}

int C_BaseCombatWeapon::GetClip1()
{
	DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip1");
}

int C_BaseCombatWeapon::GetClip2()
{
	DYNVAR_RETURN(int, this, "DT_BaseCombatWeapon", "LocalWeaponData", "m_iClip2");
}

int C_BaseCombatWeapon::GetSlot()
{
	typedef int(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 327)(this);
}

float C_BaseCombatWeapon::GetChargeTime()
{
	DYNVAR_RETURN(float, this, "DT_WeaponPipebombLauncher", "PipebombLauncherLocalData", "m_flChargeBeginTime");
}

float C_BaseCombatWeapon::GetChargeDamage()
{
	DYNVAR_RETURN(float, this, "DT_TFSniperRifle", "SniperRifleLocalData", "m_flChargedDamage");
}

int C_BaseCombatWeapon::GetItemDefinitionIndex()
{
	DYNVAR_RETURN(int, this, "DT_EconEntity", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
}

float C_BaseCombatWeapon::GetLastFireTime()
{
	DYNVAR_RETURN(float, this, "DT_TFWeaponBase", "LocalActiveTFWeaponData", "m_flLastFireTime");
}

float C_BaseCombatWeapon::GetSwingRange(C_BaseEntity *pLocal)
{
	typedef int(__thiscall *FN)(C_BaseEntity *);
	return (static_cast<float>(GetVFunc<FN>(this, 451)(pLocal)));
}

bool C_BaseCombatWeapon::DoSwingTrace(C_GameTrace &Trace)
{
	typedef int(__thiscall *FN)(C_GameTrace &);
	return GetVFunc<FN>(this, 453)(Trace);
}

bool C_BaseCombatWeapon::WillCrit()
{
	typedef bool(__thiscall* FN)(C_BaseCombatWeapon*);

	static DWORD dwFN = gPattern.Find("client.dll",
	"55 8B EC 83 EC 18 56 57 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B F0 83 C4 14 89 75 EC");

	return ((FN)dwFN)(this);
}

Vec3 &C_BaseCombatWeapon::GetBulletSpread()
{
	typedef Vec3 &(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 286)(this);
}

int C_BaseCombatWeapon::GetDamageType()
{
	typedef int(__thiscall *FN)(void *);
	return GetVFunc<FN>(this, 378)(this);
}

bool C_BaseCombatWeapon::CanFireCriticalShot(bool bHeadShot)
{
	typedef bool(__thiscall *FN)(void *, bool, C_BaseEntity *);
	return GetVFunc<FN>(this, 421)(this, bHeadShot, nullptr);
}

bool C_BaseCombatWeapon::CanWeaponHeadShot()
{
	C_BaseEntity *owner = gInts.EntityList->GetClientEntityFromHandle(this->GetOwner());

	if (!owner)
		return false;

	switch (owner->GetClassNum())
	{
		case CLASS_SNIPER:
		{
			static float timer = gInts.GlobalVars->curtime;

			if (owner->IsScoped())
			{
				if ((gInts.GlobalVars->curtime - timer) >= 0.2f)
					return true;
			}

			else timer = gInts.GlobalVars->curtime;
		}

		case CLASS_SPY:
		{
			return (this->GetDamageType() & DMG_USE_HITLOCATIONS && this->CanFireCriticalShot(true)); //credits to bertti
		}
	}

	return false;
}

float C_BaseCombatWeapon::GetNextSecondaryAttack()
{
	DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextSecondaryAttack");
}

float C_BaseCombatWeapon::GetNextPrimaryAttack()
{
	DYNVAR_RETURN(float, this, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
}

bool C_BaseCombatWeapon::CanShoot(C_BaseEntity *pLocal)
{
	if (!pLocal || pLocal->IsCloaked() || pLocal->IsFeignDeathReady() || pLocal->IsTaunting() || pLocal->IsBonked())
		return false;

	C_NetChannelInfo *pNetChannel = reinterpret_cast<C_NetChannelInfo *>(gInts.Engine->GetNetChannelInfo());

	if (!pNetChannel)
		return false;

	float fInterp = gConVars.cl_interp->GetFloat();
	float fLatency = (pNetChannel->GetLatency(FLOW_OUTGOING) + pNetChannel->GetLatency(FLOW_INCOMING));

	return (GetNextPrimaryAttack() < ((fInterp + fLatency + static_cast<float>(pLocal->GetTickBase())) * gInts.GlobalVars->interval_per_tick));
}

bool C_BaseCombatWeapon::CanSecondaryAttack(C_BaseEntity* pLocal)
{
	return (GetNextSecondaryAttack() < (static_cast<float>(pLocal->GetTickBase()) * gInts.GlobalVars->interval_per_tick));
}
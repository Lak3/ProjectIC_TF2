#pragma once

#include "../../SDK/SDK.h"

class C_Aimbot
{
private:
	enum struct ETargetType
	{
		DEFAULT = -1,
		PLAYER,
		BUILDING
	};

	struct Target_t
	{
		C_BaseEntity* pEntity = nullptr;
		ETargetType eType = ETargetType::DEFAULT;
		Vec3 vecPos = Vec3();
		Vec3 vecAngleTo = Vec3();
		float fFovTo = 0.0f;
		float fDistTo = 0.0f;

		//player specific
		int nTargetedHitbox = -1;
	};

	std::vector<Target_t> Targets;

	enum struct ESortType
	{
		UNKNOWN,
		FOV,
		DISTANCE
	};

public:
	bool bActive		= true;
	int nAimKey			= 1;
	int nAimMethod		= 3; //0 normal 1 smooth 2 simple smooth 3 silent
	bool bAutoShoot		= true;
	bool bWaitForCharge = true;
	
	bool bAimPlayers	= true;
	bool bAimBuildings	= true;

	bool bIgnoreCloaked = false;
	bool bIgnoreFriends = false;
	int nAimHitbox		= 2; //0 head 1 body 2 auto

	int nSortMethod		= 2; //0 fov 1 dist 2 auto
	float fAimFov		= 45.0f;

	bool bNoMelee		= false;
	bool bPredictSwing	= true;
	bool bRangeCheck	= true;

	float fSmoothDuration	= 0.2f;
	float fSmoothSimple		= 5.0f;

	float fDrawAimFov		= 0.1f;
	float fDrawTracers		= 1.0f;
	float fTracersAlpha		= 0.5f;
	bool bTracerRandomColor = false;

private://main funcs
	bool ShouldRun(C_BaseEntity* pLocal, C_BaseCombatWeapon *pLocalWeapon);
	bool IsAimKeyDown();
	int GetAimHitbox(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon);
	bool GetTargets(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon);
	void SortTargets(C_BaseCombatWeapon* pLocalWeapon);
	bool GetTarget(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon, C_UserCmd *pCmd, Target_t& out);
	void SetAngles(const Target_t& Target, C_UserCmd* pCmd, C_BaseEntity* pLocal, C_BaseCombatWeapon* pWeapon);
	bool ShouldShoot(C_BaseEntity* pLocal, C_BaseCombatWeapon* pLocalWeapon, C_UserCmd* pCmd, const Target_t& Target);
	ESortType GetSortType(C_BaseCombatWeapon* pLocalWeapon);
	bool CanMeleeHit(C_BaseEntity *pLocal, C_BaseCombatWeapon* pLocalWeapon, const Vec3& vecViewAngles, int nTargetIndex);
	void DoHeadCorrection(C_BaseEntity* pLocal, Target_t& Target);
	bool DoBodyCorrection(C_BaseEntity* pLocal, Target_t& Target);

private://smooth shit
	Vec3 vecSmoothStartAngle = Vec3();
	float fSmoothStartTime = 0.0f;
	int nOldHitScan = -1;
	bool bSmoothFinished = false;

	void ResetSmooth();
	bool TargetChanged();
	bool WeaponChanged();
	bool Clip1Changed();
	bool Clip2Changed();
	bool AmmoChanged();

private://projectile shit
	struct ProjectileInfo_t
	{
		float fSpeed = 0.0f;
		float fGravity = 0.0f;
		float fMaxTime = 2.5f;
	};

	bool GetProjectileInfo(C_BaseCombatWeapon *pWeapon, ProjectileInfo_t &out);

	struct Predictor_t
	{
		C_BaseEntity *pEntity = nullptr;

		Vec3 vecPosition		= Vec3();
		Vec3 vecVelocity		= Vec3();
		Vec3 vecAcceleration	= Vec3();

		Vec3 Extrapolate(float time);
	};

	struct Solution_t
	{
		float fPitch	= 0.0f;
		float fYaw		= 0.0f;
		float fTime		= 0.0f;
	};

	bool CalcProjAngle(const Vec3 &vecLocalPos, const Vec3 &vecPredictedPos, const ProjectileInfo_t &ProjInfo, Solution_t &out);
	void GetProjectileFireSetup(C_BaseEntity *pPlayer, const Vec3 &vecViewAngles, Vec3 vecOffset, Vec3 *vecSrc);
	bool SolveProjectile(C_BaseEntity *pLocal, C_BaseCombatWeapon *pLocalWeapon, C_UserCmd *pCmd, Predictor_t &Predictor, const ProjectileInfo_t &ProjInfo, Solution_t &out);
	Vec3 EstimateProjectileImpactPosition(C_BaseEntity *pLocal, float pitch, float yaw, float initVel);

public:
	void Run(C_UserCmd* pCmd);
	void Paint();

	//We kinda need this everywhere, shouldn't we just run this @ the end of CMove??
	void FixMovement(C_UserCmd* pCmd, const Vec3& angs);

	bool bIsCurWeaponProjectile = false;
};

extern C_Aimbot gAimbot;
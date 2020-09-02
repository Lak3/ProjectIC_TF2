#include "BaseEntity.h"

bool C_BaseEntity::IsFeignDeathReady()
{
	DYNVAR_RETURN(bool, this, "DT_TFPlayer", "m_Shared", "m_bFeignDeathReady");
}

float C_BaseEntity::GetStepSize()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_flStepSize");
}

float C_BaseEntity::GetConveyorSpeed()
{
	DYNVAR_RETURN(float, this, "DT_FuncConveyor", "m_flConveyorSpeed");
}

C_BaseEntity *C_BaseEntity::GetGroundEntity()
{
	DYNVAR(pHandle, int, "DT_BasePlayer", "m_hGroundEntity");
	return reinterpret_cast<C_BaseEntity *>(gInts.EntityList->GetClientEntityFromHandle(pHandle.GetValue(this)));
}

CTFPlayerAnimState *C_BaseEntity::GetAnimState()
{
	return *reinterpret_cast<CTFPlayerAnimState **>(this + 0x1BF8);
}

std::array<float, MAXSTUDIOPOSEPARAM> C_BaseEntity::GetPoseParam()
{
	static DWORD dwOffset = gNetVars.get_offset("DT_BaseAnimating", "m_flPoseParameter");
	return *reinterpret_cast<std::array<float, MAXSTUDIOPOSEPARAM> *>(this + dwOffset);
}

bool C_BaseEntity::GetHitboxMinsAndMaxsAndMatrix(int nHitbox, Vec3 &mins, Vec3 &maxs, matrix3x4& matrix, Vec3 *center)
{
	model_t *pModel = GetModel();
	if (!pModel)
		return false;

	studiohdr_t *pHdr = (studiohdr_t *)gInts.ModelInfo->GetStudioModel(pModel);
	if (!pHdr)
		return false;

	matrix3x4 BoneMatrix[128];
	if (!this->SetupBones(BoneMatrix, 128, 0x100, gInts.GlobalVars->curtime))
		return false;

	mstudiohitboxset_t *pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return false;

	mstudiobbox_t *pBox = pSet->hitbox(nHitbox);
	if (!pBox)
		return false;

	mins = pBox->bbmin;
	maxs = pBox->bbmax;
	memcpy(matrix, BoneMatrix[pBox->bone], sizeof(matrix3x4));

	if (center)
		Math::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), BoneMatrix[pBox->bone], *center);

	return true;
}

bool C_BaseEntity::GetHitboxMinsAndMaxs(int nHitbox, Vec3& mins, Vec3& maxs, Vec3* center)
{
	model_t* pModel = GetModel();
	if (!pModel)
		return false;

	studiohdr_t* pHdr = (studiohdr_t*)gInts.ModelInfo->GetStudioModel(pModel);
	if (!pHdr)
		return false;

	matrix3x4 BoneMatrix[128];
	if (!this->SetupBones(BoneMatrix, 128, 0x100, gInts.GlobalVars->curtime))
		return false;

	mstudiohitboxset_t* pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return false;

	mstudiobbox_t* pBox = pSet->hitbox(nHitbox);
	if (!pBox)
		return false;

	mins = pBox->bbmin;
	maxs = pBox->bbmax;

	if (center)
		Math::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), BoneMatrix[pBox->bone], *center);

	return true;
}

MoveType_t C_BaseEntity::GetMoveType()
{
	DYNVAR_RETURN(MoveType_t, this, "DT_BaseEntity", "movetype");
}

bool C_BaseEntity::IsStunned()
{
	return (this->GetCond() & TFCond_Stunned);
}

float C_BaseEntity::GetSurfaceFriction()
{
	return *reinterpret_cast<float *>(this + 0x12D4);
}

float C_BaseEntity::GetMaxSpeed()
{
	DYNVAR_RETURN(float, this, "DT_BasePlayer", "m_flMaxspeed");
}

float C_BaseEntity::GetWaterJumpTime()
{
	return *reinterpret_cast<float *>(this + 0x10FC);

	//reference
	/*
	float			m_flMaxspeed;
	int				m_iBonusProgress;
	int				m_iBonusChallenge;
	CInterpolatedVar< Vector >	m_iv_vecViewOffset;
	Vector			m_vecWaterJumpVel;
	float			m_flWaterJumpTime;
	*/
}

int C_BaseEntity::GetState()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerState");
}

bool C_BaseEntity::IsSlowed()
{
	return (this->GetCond() & TFCond_Slowed);
}

bool C_BaseEntity::IsSentrygun()
{
	return (this->GetClientClass()->iClassID == CObjectSentrygun);
}

bool C_BaseEntity::IsDispenser()
{
	return (this->GetClientClass()->iClassID == CObjectDispenser);
}

bool C_BaseEntity::IsTeleporter()
{
	return (this->GetClientClass()->iClassID == CObjectTeleporter);
}

void C_BaseEntity::SetTickBase(int tickbase)
{
	DYNVAR_SET(int, this, tickbase, "DT_BasePlayer", "localdata", "m_nTickBase");
}

bool C_BaseEntity::IsInValidTeam()
{
	int Team = this->GetTeamNum();
	return (Team == 2 || Team == 3);
}

C_BaseEntity *C_BaseEntity::FirstMoveChild() {
	return gInts.EntityList->GetClientEntity(*reinterpret_cast<int *>(this + 0x1B0) & 0xFFF);
}

C_BaseEntity *C_BaseEntity::NextMovePeer() {
	return gInts.EntityList->GetClientEntity(*reinterpret_cast<int *>(this + 0x1B4) & 0xFFF);
}

void C_BaseEntity::UpTickBase()
{
	static CDynamicNetvar<int> n("DT_BasePlayer", "localdata", "m_nTickBase");
	n.SetValue(this, n.GetValue(this) + 1);
}

void C_BaseEntity::UpdateClientSideAnimation()
{
	void *renderable = (PVOID)(this + 0x4);
	typedef void(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(renderable, 3)(renderable);
}

const Vec3 &C_BaseEntity::GetRenderAngles()
{
	void *renderable = (PVOID)(this + 0x4);
	typedef const Vec3 &(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(renderable, 2)(renderable);
}

void C_BaseEntity::SetRenderAngles(const Vec3 &v)
{
	Vec3 *pRenderAngles = const_cast<Vec3 *>(&this->GetRenderAngles());
	*pRenderAngles = v;
}

void C_BaseEntity::SetAbsOrigin(const Vec3 &v)
{
	typedef void(__thiscall *FN)(C_BaseEntity *, const Vec3 &);
	static DWORD dwFN = gPattern.Find("client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07");
	FN func = (FN)dwFN;
	func(this, v);
}

void C_BaseEntity::SetAbsAngles(const Vec3 &v)
{
	/*typedef void(__thiscall *FN)(C_BaseEntity *, const Vec3 &);
	static DWORD dwFN = gPattern.FindInClient("55 8B EC 83 EC 60 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07 0F 2E 86 ? ? ? ? 9F F6 C4 44 7A 28 F3 0F 10 47 ?");
	FN func = (FN)dwFN;
	func(this, v);*/

	Vec3 *pAbsAngles = const_cast<Vec3 *>(&this->GetAbsAngles());
	*pAbsAngles = v;
}

const Vec3 &C_BaseEntity::GetAbsOrigin()
{
	typedef Vec3 &(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 9)(this);
}

//Recreated from "C_TFPlayer"
//Supposed to be used from local.
bool C_BaseEntity::IsPlayerOnSteamFriendList(C_BaseEntity* pEntity)
{
	PlayerInfo_t pi;
	if (gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pi) && pi.friendsID)
	{
		CSteamID steamID{ pi.friendsID, 1, k_EUniversePublic, k_EAccountTypeIndividual };
		if (gSteam.Friends002->HasFriend(steamID, k_EFriendFlagImmediate))
			return true;
	}

	return false;
}

const Vec3 &C_BaseEntity::GetAbsAngles()
{
	typedef Vec3 &(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 10)(this);
}

Vec3 C_BaseEntity::GetViewOffset() {
	DYNVAR_RETURN(Vec3, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
}

Vec3 C_BaseEntity::GetEyePosition() {
	DYNVAR_RETURN(Vec3, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]") + GetAbsOrigin();
}

C_UserCmd *C_BaseEntity::GetCurrentCommand()
{
	DYNVAR_RETURN(C_UserCmd *, (this - 0x4), "DT_BasePlayer", "localdata", "m_hConstraintEntity");
}

void C_BaseEntity::SetCurrentCmd(C_UserCmd *cmd)
{
	DYNVAR_SET(C_UserCmd *, (this - 0x4), cmd, "DT_BasePlayer", "localdata", "m_hConstraintEntity");
}

Vec3 C_BaseEntity::GetPunchAngles()
{
	return *reinterpret_cast<Vec3*>(this + 0xE8C);
}

Vec3 C_BaseEntity::GetVecOrigin()
{
	DYNVAR_RETURN(Vec3, this, "DT_BaseEntity", "m_vecOrigin");
}

void C_BaseEntity::SetVecOrigin(Vec3 &vOrigin)
{
	DYNVAR_SET(Vec3, this, vOrigin, "DT_BaseEntity", "m_vecOrigin");
}

bool C_BaseEntity::GetTouched() {
	return *reinterpret_cast<bool*>(this + 0x8F8);
}

int C_BaseEntity::GetType() {
	return *reinterpret_cast<int*>(this + 0x8FC);
}

Vec3 C_BaseEntity::GetShootPos()
{
	static auto Weapon_ShootPositionFn = reinterpret_cast<float *(__thiscall *)(PVOID, Vec3 *)>(gPattern.Find("client.dll", "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90"));
	Vec3 vOut = Vec3();
	Weapon_ShootPositionFn(this, &vOut);
	return vOut;
}

const char *C_BaseEntity::GetModelName()
{
	return gInts.ModelInfo->GetModelName(this->GetModel());
}

int C_BaseEntity::GetAmmo()
{
	DYNVAR_RETURN(int, (this + 4), "DT_BasePlayer", "localdata", "m_iAmmo");
}

bool C_BaseEntity::IsBaseCombatWeapon()
{
	typedef bool(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 137)(this);
}

bool C_BaseEntity::IsWearable()
{
	typedef bool(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 138)(this);
}

int C_BaseEntity::GetHitboxSet()
{
	DYNVAR_RETURN(int, this, "DT_BaseAnimating", "m_nHitboxSet");
}

int C_BaseEntity::GetTickBase()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_nTickBase");
}

void C_BaseEntity::IncreaseTickBase()
{
	static CDynamicNetvar<int>n("DT_BasePlayer", "localdata", "m_nTickBase");
	n.SetValue(this, n.GetValue(this) + 1);
}

float C_BaseEntity::flSimulationTime()
{
	DYNVAR_RETURN(float, this, "DT_BaseEntity", "m_flSimulationTime");
}

int C_BaseEntity::GetOwner()
{
	DYNVAR_RETURN(int, this, "DT_BaseEntity", "m_hOwnerEntity");
}

Vec3 C_BaseEntity::GetWorldSpaceCenter()
{
	Vec3 vMin, vMax;

	this->GetRenderBounds(vMin, vMax);
	Vec3 vWorldSpaceCenter = this->GetAbsOrigin();
	vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2.0f;

	return vWorldSpaceCenter;
}

model_t *C_BaseEntity::GetModel()
{
	PVOID pRenderable = (PVOID)(this + 0x4);
	typedef model_t *(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(pRenderable, 9)(pRenderable);
}

int C_BaseEntity::DrawModel(int flags)
{
	void *renderable = (PVOID)(this + 0x4);
	typedef int(__thiscall *FN)(PVOID, int);
	return GetVFunc<FN>(renderable, 10)(renderable, flags);
}

bool C_BaseEntity::SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int nBoneMask, float fCurrentTime)
{
	PVOID pRenderable = (PVOID)(this + 0x4);
	typedef bool(__thiscall *FN)(PVOID, matrix3x4 *, int, int, float);
	return GetVFunc<FN>(pRenderable, 16)(pRenderable, pBoneToWorldOut, nMaxBones, nBoneMask, fCurrentTime);
}

C_ClientClass *C_BaseEntity::GetClientClass()
{
	PVOID pNetworkable = (PVOID)(this + 0x8);
	typedef C_ClientClass *(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(pNetworkable, 2)(pNetworkable);
}

bool C_BaseEntity::IsDormant()
{
	PVOID pNetworkable = (PVOID)(this + 0x8);
	typedef bool(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(pNetworkable, 8)(pNetworkable);
}

int C_BaseEntity::GetIndex()
{
	PVOID pNetworkable = (PVOID)(this + 0x8);
	typedef int(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(pNetworkable, 9)(pNetworkable);
}

void C_BaseEntity::GetRenderBounds(Vec3 &vMins, Vec3 &vMaxs)
{
	PVOID pRenderable = (PVOID)(this + 0x4);
	typedef void(__thiscall *FN)(PVOID, Vec3 &, Vec3 &);
	GetVFunc<FN>(pRenderable, 20)(pRenderable, vMins, vMaxs);
}

matrix3x4 &C_BaseEntity::GetRgflCoordinateFrame()
{
	PVOID pRenderable = (PVOID)(this + 0x4);
	typedef matrix3x4 &(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(pRenderable, 34)(pRenderable);
}

Vec3 C_BaseEntity::GetVelocity()
{
	typedef void(__thiscall *EstimateAbsVelocityFn)(C_BaseEntity *, Vec3 &);
	static DWORD dwFn = gPattern.Find("client.dll", "E8 ? ? ? ? F3 0F 10 4D ? 8D 85 ? ? ? ? F3 0F 10 45 ? F3 0F 59 C9 56 F3 0F 59 C0 F3 0F 58 C8 0F 2F 0D ? ? ? ? 76 07") + 0x1;
	static DWORD dwEstimate = ((*(PDWORD)(dwFn)) + dwFn + 0x4);
	EstimateAbsVelocityFn vel = (EstimateAbsVelocityFn)dwEstimate;
	Vec3 v;
	vel(this, v);
	return v;
}

int C_BaseEntity::GetMaxHealth()
{
	typedef int(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 107)(this);
}

int C_BaseEntity::GetHealth()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iHealth");
}

int C_BaseEntity::GetTeamNum()
{
	DYNVAR_RETURN(int, this, "DT_BaseEntity", "m_iTeamNum");
}

int C_BaseEntity::GetFlags()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_fFlags");
}

void C_BaseEntity::SetFlags(int nFlags)
{
	DYNVAR_SET(int, this, nFlags, "DT_BasePlayer", "m_fFlags");
}

BYTE C_BaseEntity::GetLifeState()
{
	DYNVAR_RETURN(BYTE, this, "DT_BasePlayer", "m_lifeState");
}

int C_BaseEntity::GetClassId()
{
	return this->GetClientClass()->iClassID;
}

int C_BaseEntity::GetClassNum()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_PlayerClass", "m_iClass");
}

const char *C_BaseEntity::szGetClass()
{
	int nClassNum = this->GetClassNum();

	switch (nClassNum)
	{
		case CLASS_SCOUT:
			return "scout";

		case CLASS_SOLDIER:
			return "soldier";

		case CLASS_PYRO:
			return "pyro";

		case CLASS_DEMOMAN:
			return "demoman";

		case CLASS_HEAVY:
			return "heavy";

		case CLASS_ENGINEER:
			return "engineer";

		case CLASS_MEDIC:
			return "medic";

		case CLASS_SNIPER:
			return "sniper";

		case CLASS_SPY:
			return "spy";

		default: return "unknown";
	}
}

int C_BaseEntity::GetCond()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
}

int C_BaseEntity::GetCondEx()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCondEx");
}

int C_BaseEntity::GetCondEx2()
{
	DYNVAR_RETURN(int, this, "DT_TFPlayer", "m_Shared", "m_nPlayerCondEx2");
}

void C_BaseEntity::RemoveCond(int cond)
{
	static DWORD offset = gNetVars.get_offset("DT_TFPlayer", "m_Shared", "m_nPlayerCond");

	if (*reinterpret_cast<int *>(this + offset) &cond)
		*reinterpret_cast<DWORD *>(this + offset) &= ~cond;
}

Vec3 C_BaseEntity::GetCollideableMins()
{
	DYNVAR_RETURN(Vec3, this, "DT_BaseEntity", "m_Collision", "m_vecMins");
}

Vec3 C_BaseEntity::GetCollideableMaxs()
{
	DYNVAR_RETURN(Vec3, this, "DT_BaseEntity", "m_Collision", "m_vecMaxs");
}

Vec3 C_BaseEntity::GetEyeAngles()
{
	DYNVAR_RETURN(Vec3, this, "DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
}

void C_BaseEntity::SetEyeAngles(const Vec3 &v)
{
	DYNVAR_SET(Vec3, this, v, "DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
}

byte C_BaseEntity::GetWaterLevel()
{
	DYNVAR_RETURN(byte, this, "DT_TFPlayer", "m_nWaterLevel");
}

bool C_BaseEntity::IsOnGround()
{
	return (this->GetFlags() & FL_ONGROUND);
}

bool C_BaseEntity::IsInWater()
{
	return (this->GetFlags() & FL_INWATER); //it's better to use IsSwimming method cuz this one doesn't work iirc.
}

bool C_BaseEntity::IsSwimming()
{
	return (this->GetWaterLevel() > 1);
}

Vec3 C_BaseEntity::GetHitboxPos(int nHitbox)
{
	model_t *pModel = GetModel();
	if (!pModel)
		return Vec3();

	studiohdr_t *pHdr = (studiohdr_t *)gInts.ModelInfo->GetStudioModel(pModel);
	if (!pHdr)
		return Vec3();

	matrix3x4 BoneMatrix[128];
	if (!this->SetupBones(BoneMatrix, 128, 0x100, gInts.GlobalVars->curtime))
		return Vec3();

	mstudiohitboxset_t *pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return Vec3();

	mstudiobbox_t *pBox = pSet->hitbox(nHitbox);
	if (!pBox)
		return Vec3();

	Vec3 vPos = (pBox->bbmin + pBox->bbmax) * 0.5f;
	Vec3 vOut = Vec3();

	Math::VectorTransform(vPos, BoneMatrix[pBox->bone], vOut);

	return vOut;
}

int C_BaseEntity::GetNumOfHitboxes()
{
	model_t *pModel = GetModel();
	if (!pModel)
		return 0;

	studiohdr_t *pHdr = (studiohdr_t *)gInts.ModelInfo->GetStudioModel(pModel);
	if (!pHdr)
		return 0;

	mstudiohitboxset_t *pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return 0;

	return pSet->numhitboxes;
}

Vec3 C_BaseEntity::GetBonePos(int nBone)
{
	matrix3x4 matrix[128];

	if (this->SetupBones(matrix, 128, 0x100, GetTickCount64()))
		return Vec3(matrix[nBone][0][3], matrix[nBone][1][3], matrix[nBone][2][3]);

	return Vec3(0.0f, 0.0f, 0.0f);
}

C_BaseCombatWeapon *C_BaseEntity::GetActiveWeapon()
{
	DYNVAR(pHandle, int, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	return reinterpret_cast<C_BaseCombatWeapon *>(gInts.EntityList->GetClientEntityFromHandle(pHandle.GetValue(this)));
}

float C_BaseEntity::GetNextAttack()
{
	DYNVAR_RETURN(float, this, "DT_BaseCombatCharacter", "bcc_localdata", "m_flNextAttack");
}

void C_BaseEntity::ForceTauntCam(bool b)
{
	DYNVAR_SET(bool, this, b, "DT_TFPlayer", "m_nForceTauntCam");
}

bool C_BaseEntity::IsAlive()
{
	return (this->GetLifeState() == LIFE_ALIVE);
}

bool C_BaseEntity::IsVulnerable()
{
	return !(this->GetCond() & TFCond_Ubercharged || this->GetCond() & TFCond_Bonked);
}

bool C_BaseEntity::IsCritBoosted()
{
	int nCond = this->GetCond();
	int nCondEx = this->GetCondEx();

	return (nCond & TFCond_Kritzkrieged
		|| nCond & TFCond_MiniCrits
		|| nCondEx & TFCondEx_CritCanteen
		|| nCondEx & TFCondEx_CritOnFirstBlood
		|| nCondEx & TFCondEx_CritOnWin
		|| nCondEx & TFCondEx_HalloweenCritCandy
		|| nCondEx & TFCondEx_PyroCrits);
}

bool C_BaseEntity::IsPlayer()
{
	return (this->GetClientClass()->iClassID == CTFPlayer);
}

bool C_BaseEntity::IsBuilding()
{
	return (this->GetClientClass()->iClassID == CObjectDispenser
		|| this->GetClientClass()->iClassID == CObjectSentrygun
		|| this->GetClientClass()->iClassID == CObjectTeleporter);
}

bool C_BaseEntity::IsPickup()
{
	return (this->GetClientClass()->iClassID == ETFClassID_t::CBaseAnimating && this->GetModelName()[24] != 'h' || this->GetClientClass()->iClassID == CTFAmmoPack);
}

bool C_BaseEntity::IsScoped()
{
	return (this->GetCond() & TFCond_Zoomed);
}

int C_BaseEntity::GetFov()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iFOV");
}

void C_BaseEntity::SetFov(int nFov)
{
	DYNVAR_SET(int, this, nFov, "DT_BasePlayer", "m_iFOV");
}

bool C_BaseEntity::IsTaunting()
{
	return (this->GetCond() & TFCond_Taunting);
}

bool C_BaseEntity::IsDucking()
{
	return (this->GetFlags() & FL_DUCKING);
}

bool C_BaseEntity::IsUbered()
{
	return (this->GetCond() & TFCond_Ubercharged);
}

bool C_BaseEntity::IsBonked()
{
	return (this->GetCond() & TFCond_Bonked);
}

bool C_BaseEntity::IsCloaked()
{
	return (this->GetCond() & TFCond_Cloaked);
}

int C_BaseEntity::GetObserverTarget()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_hObserverTarget");
}

int C_BaseEntity::GetObserverMode()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iObserverMode");
}

void C_BaseEntity::RemoveEffect(byte Effect)
{
	*reinterpret_cast<byte *>(this + 0x7C) &= ~Effect;

	if (Effect == EF_NODRAW) {
		static auto AddToLeafSystemFn = reinterpret_cast<int(__thiscall *)(PVOID, int)>(gPattern.Find("client.dll", "55 8B EC 56 FF 75 08 8B F1 B8"));

		if (AddToLeafSystemFn)
			AddToLeafSystemFn(this, RENDER_GROUP_OPAQUE_ENTITY);
	}
}

void C_BaseEntity::SetGlowEnabled(bool bState) {
	DYNVAR_SET(bool, this, bState, "DT_TFPlayer", "m_bGlowEnabled");
}

bool C_BaseEntity::IsGlowEnabled() {
	DYNVAR_RETURN(bool, this, "DT_TFPlayer", "m_bGlowEnabled");
}

void C_BaseEntity::UpdateGlowEffect() {
	typedef void(__thiscall *FN)(PVOID);
	return GetVFunc<FN>(this, 226)(this);
}

void *C_BaseEntity::GetThrower()
{
	DYNVAR_RETURN(C_BaseEntity *, this, "DT_BaseGrenade", "m_hThrower");
}

float C_BaseEntity::GetDamageRadius()
{
	DYNVAR_RETURN(float, this, "DT_BaseGrenade", "m_DmgRadius");
}

void C_BaseEntity::SetCond(int cond)
{
	DYNVAR_SET(int, this, cond, "DT_TFPlayer", "m_Shared", "m_nPlayerCond");
}
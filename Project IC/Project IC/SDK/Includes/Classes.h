#pragma once
#include "../../Utils/Utils.h"

#include "Enums.h"
#include "Structs.h"

#include "dt_common.h"
#include "dt_recv.h"

class IMaterialVar;
class MorphFormat_t;
class MaterialPropertyTypes_t;
class VertexFormat_t;

class VMatrix {
private:
	Vec3 m[4][4];

public:
	const matrix3x4 &As3x4() const {
		return *((const matrix3x4 *)this);
	}
};

class C_ViewSetup
{
public:
	// left side of view window
	int			x;
	int			m_nUnscaledX;
	// top side of view window
	int			y;
	int			m_nUnscaledY;
	// width of view window
	int			width;
	int			m_nUnscaledWidth;
	// height of view window
	int			height;
	// which eye are we rendering?
	StereoEye_t m_eStereoEye;
	int			m_nUnscaledHeight;

	// the rest are only used by 3D views

		// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vec3		origin;

	// heading of camera (pitch, yaw, roll)
	Vec3		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState;

	// If using VR, the headset calibration will feed you a projection matrix per-eye.
	// This does NOT override the Z range - that will be set up as normal (i.e. the values in this matrix will be ignored).
	bool        m_bViewToProjectionOverride;
	VMatrix     m_ViewToProjection;
};

class C_MoveData
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;

	void* m_nPlayerHandle; // edict index on server, client entity handle on client

	int    m_nImpulseCommand; // Impulse command issued.
	Vec3 m_vecViewAngles; // Command view angles (local space)
	Vec3 m_vecAbsViewAngles; // Command view angles (world space)
	int    m_nButtons; // Attack buttons.
	int    m_nOldButtons; // From host_client->oldbuttons;
	float  m_flForwardMove;
	float  m_flSideMove;
	float  m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vec3 m_vecVelocity; // edict::velocity		// Current movement direction.
	Vec3 m_vecAngles; // edict::angles
	Vec3 m_vecOldAngles;

	// Output only
	float  m_outStepHeight; // how much you climbed this move
	Vec3 m_outWishVel; // This is where you tried
	Vec3 m_outJumpVel; // This is your jump velocity

					   // Movement constraints	(radius 0 means no constraint)
	Vec3 m_vecConstraintCenter;
	float  m_flConstraintRadius;
	float  m_flConstraintWidth;
	float  m_flConstraintSpeedFactor;

	void        SetAbsOrigin(const Vec3& vec);
	const Vec3& GetAbsOrigin() const;

	// private:
	Vec3 m_vecAbsOrigin; // edict::origin
};

class CEntityGroundContact
{
public:
	int		entindex;
	float	minheight;
	float	maxheight;
};

class C_UserCmd
{
public:
	virtual ~C_UserCmd() {};

	// For matching server and client commands for debugging
	int		command_number;

	// the tick the client created this command
	int		tick_count;

	// Player instantaneous view angles.
	Vec3	viewangles;
	// Intended velocities
	//	forward velocity.
	float	forwardmove;
	//  sideways velocity.
	float	sidemove;
	//  upward velocity.
	float	upmove;
	// Attack button states
	int		buttons;
	// Impulse command issued.
	byte    impulse;
	// Current weapon id
	int		weaponselect;
	int		weaponsubtype;

	int		random_seed;	// For shared random functions

	//int		server_random_seed; // Only the server populates this seed

	short	mousedx;		// mouse accum in x from create move
	short	mousedy;		// mouse accum in y from create move

	// Client only, tracks whether we've predicted this command at least once
	bool	hasbeenpredicted;
};

class C_ClientClass
{
public:
	BYTE Pad[8];
	char *szNetworkName;
	RecvTable *pRecvTable;
	C_ClientClass *pNextClass;
	int iClassID;
};

class C_BaseEntity; //forward declare

class C_TraceFilter
{
public:
	virtual bool ShouldHitEntity(void *entity, int contents_mask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class C_BaseTrace
{
public:
	bool IsDispSurface(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceSurfProp1(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceSurfProp2(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vec3			vStartPos = Vec3();
	Vec3			vEndPos = Vec3();
	cplane_t		Plane = {};
	float			flFraction = 0.0f;
	int				iContents = 0;
	unsigned short	DispFlags = 0;
	bool			bAllSolid = false;
	bool			bStartSolid = false;

	C_BaseTrace()
	{

	}

private:
	C_BaseTrace(const C_BaseTrace &othr);
};

class C_GameTrace : public C_BaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int  GetEntityIndex() const;

	bool DidHit() const {
		return (flFraction < 1 || bAllSolid || bStartSolid);
	}

public:
	float			fraction_left_solid;
	csurface_t		surface;
	int				hit_group;
	short			physics_bone;
	C_BaseEntity	*entity;
	int				hitbox;

	C_GameTrace() { }
	C_GameTrace(const C_GameTrace &othr);
};

struct CHudTexture;
struct KeyValues;

class FileWeaponInfo_t
{
public:

	FileWeaponInfo_t();

	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse(KeyValues *pKeyValuesData, const char *szWeaponName);


public:
	bool					bParsedScript;
	bool					bLoadedHudElements;

	// SHARED
	char					szClassName[MAX_WEAPON_STRING];
	char					szPrintName[MAX_WEAPON_STRING];			// Name for showing in HUD, etc.

	char					szViewModel[MAX_WEAPON_STRING];			// View model of this weapon
	char					szWorldModel[MAX_WEAPON_STRING];		// Model of this weapon seen carried by the player
	char					szAnimationPrefix[MAX_WEAPON_PREFIX];	// Prefix of the animations that should be used by the player carrying this weapon
	int						iSlot;									// inventory slot.
	int						iPosition;								// position in the inventory slot.
	int						iMaxClip1;								// max primary clip size (-1 if no clip)
	int						iMaxClip2;								// max secondary clip size (-1 if no clip)
	int						iDefaultClip1;							// amount of primary ammo in the gun when it's created
	int						iDefaultClip2;							// amount of secondary ammo in the gun when it's created
	int						iWeight;								// this value used to determine this weapon's importance in autoselection.
	int						iRumbleEffect;							// Which rumble effect to use when fired? (xbox)
	bool					bAutoSwitchTo;							// whether this weapon should be considered for autoswitching to
	bool					bAutoSwitchFrom;						// whether this weapon can be autoswitched away from when picking up another weapon or ammo
	int						iFlags;									// miscellaneous weapon flags
	char					szAmmo1[MAX_WEAPON_AMMO_NAME];			// "primary" ammo type
	char					szAmmo2[MAX_WEAPON_AMMO_NAME];			// "secondary" ammo type

	// Sound blocks
	char					aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

	int						iAmmoType;
	int						iAmmo2Type;
	bool					m_bMeleeWeapon;		// Melee weapons can always "fire" regardless of ammo.

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool					m_bBuiltRightHanded;
	bool					m_bAllowFlipping;	// False to disallow flipping the model, regardless of whether
												// it is built left or right handed.

// CLIENT DLL
	// Sprite data, read from the data file
	int						iSpriteCount;
	CHudTexture *iconActive;
	CHudTexture *iconInactive;
	CHudTexture *iconAmmo;
	CHudTexture *iconAmmo2;
	CHudTexture *iconCrosshair;
	CHudTexture *iconAutoaim;
	CHudTexture *iconZoomedCrosshair;
	CHudTexture *iconZoomedAutoaim;
	CHudTexture *iconSmall;

	// TF2 specific
	bool					bShowUsageHint;							// if true, then when you receive the weapon, show a hint about it

// SERVER DLL

};

struct WeaponData_t
{
	int		m_nDamage;
	int		m_nBulletsPerShot;
	float	m_flRange;
	float	m_flSpread;
	float	m_flPunchAngle;
	float	m_flTimeFireDelay;				// Time to delay between firing
	float	m_flTimeIdle;					// Time to idle after firing
	float	m_flTimeIdleEmpty;				// Time to idle after firing last bullet in clip
	float	m_flTimeReloadStart;			// Time to start into a reload (ie. shotgun)
	float	m_flTimeReload;					// Time to reload
	bool	m_bDrawCrosshair;				// Should the weapon draw a crosshair
	int		m_iProjectile;					// The type of projectile this mode fires
	int		m_iAmmoPerShot;					// How much ammo each shot consumes
	float	m_flProjectileSpeed;			// Start speed for projectiles (nail, etc.); NOTE: union with something non-projectile
	float	m_flSmackDelay;					// how long after swing should damage happen for melee weapons
	bool	m_bUseRapidFireCrits;

	void Init(void)
	{
		m_nDamage = 0;
		m_nBulletsPerShot = 0;
		m_flRange = 0.0f;
		m_flSpread = 0.0f;
		m_flPunchAngle = 0.0f;
		m_flTimeFireDelay = 0.0f;
		m_flTimeIdle = 0.0f;
		m_flTimeIdleEmpty = 0.0f;
		m_flTimeReloadStart = 0.0f;
		m_flTimeReload = 0.0f;
		m_iProjectile = 0;
		m_iAmmoPerShot = 0;
		m_flProjectileSpeed = 0.0f;
		m_flSmackDelay = 0.0f;
		m_bUseRapidFireCrits = false;
	};
};

class CTFWeaponInfo : public FileWeaponInfo_t
{
public:

	CTFWeaponInfo();
	~CTFWeaponInfo();

	virtual void Parse(::KeyValues *pKeyValuesData, const char *szWeaponName);

	WeaponData_t const &GetWeaponData(int iWeapon) const { return m_WeaponData[iWeapon]; }

public:

	WeaponData_t	m_WeaponData[2];

	int		m_iWeaponType;

	// Grenade.
	bool	m_bGrenade;
	float	m_flDamageRadius;
	float	m_flPrimerTime;
	bool	m_bLowerWeapon;
	bool	m_bSuppressGrenTimer;

	// Skins
	bool	m_bHasTeamSkins_Viewmodel;
	bool	m_bHasTeamSkins_Worldmodel;

	// Muzzle flash
	char	m_szMuzzleFlashModel[128];
	float	m_flMuzzleFlashModelDuration;
	char	m_szMuzzleFlashParticleEffect[128];

	// Tracer
	char	m_szTracerEffect[128];

	// Eject Brass
	bool	m_bDoInstantEjectBrass;
	char	m_szBrassModel[128];

	// Explosion Effect
	char	m_szExplosionSound[128];
	char	m_szExplosionEffect[128];
	char	m_szExplosionPlayerEffect[128];
	char	m_szExplosionWaterEffect[128];

	bool	m_bDontDrop;
};

class IMaterial
{
public:
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;

	virtual PreviewImageRetVal_t GetPreviewImageProperties(int* width, int* height,
		ImageFormat* imageFormat, bool* isTranslucent) const = 0;

	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char* data,
		int width, int height,
		ImageFormat imageFormat) const = 0;

	virtual int				GetMappingWidth() = 0;
	virtual int				GetMappingHeight() = 0;
	virtual int				GetNumAnimationFrames() = 0;
	virtual bool			InMaterialPage(void) = 0;
	virtual	void			GetMaterialOffset(float* pOffset) = 0;
	virtual void			GetMaterialScale(float* pScale) = 0;
	virtual IMaterial*      GetMaterialPage(void) = 0;
	virtual IMaterialVar*   FindVar(const char* varName, bool* found, bool complain = true) = 0;
	virtual void			IncrementReferenceCount(void) = 0;
	virtual void			DecrementReferenceCount(void) = 0;
	virtual int 			GetEnumerationID(void) const = 0;
	virtual void			GetLowResColorSample(float s, float t, float* color) const = 0;
	virtual void			RecomputeStateSnapshots() = 0;
	virtual bool			IsTranslucent() = 0;
	virtual bool			IsAlphaTested() = 0;
	virtual bool			IsVertexLit() = 0;
	virtual VertexFormat_t	GetVertexFormat() const = 0;
	virtual bool			HasProxy(void) const = 0;
	virtual bool			UsesEnvCubemap(void) = 0;
	virtual bool			NeedsTangentSpace(void) = 0;
	virtual bool			NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsSoftwareSkinning(void) = 0;
	virtual void			AlphaModulate(float alpha) = 0;
	virtual void			ColorModulate(float r, float g, float b) = 0;
	virtual void			SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool			GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;
	virtual void			GetReflectivity(Vec3& reflect) = 0;
	virtual bool			GetPropertyFlag(MaterialPropertyTypes_t type) = 0;
	virtual bool			IsTwoSided() = 0;
	virtual void			SetShader(const char* pShaderName) = 0;
	virtual int				GetNumPasses(void) = 0;
	virtual int				GetTextureMemoryBytes(void) = 0;
	virtual void			Refresh() = 0;
	virtual bool			NeedsLightmapBlendAlpha(void) = 0;
	virtual bool			NeedsSoftwareLighting(void) = 0;
	virtual int				ShaderParamCount() const = 0;
	virtual IMaterialVar**  GetShaderParams(void) = 0;
	virtual bool			IsErrorMaterial() const = 0;
	virtual void			SetUseFixedFunctionBakedLighting(bool bEnable) = 0;
	virtual float			GetAlphaModulation() = 0;
	virtual void			GetColorModulation(float* r, float* g, float* b) = 0;
	virtual MorphFormat_t	GetMorphFormat() const = 0;
	virtual IMaterialVar*   FindVarFast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void			SetShaderAndParams(KeyValues* pKeyValues) = 0;
	virtual const char*     GetShaderName() const = 0;
	virtual void			DeleteIfUnreferenced() = 0;
	virtual bool			IsSpriteCard() = 0;
	virtual void			CallBindProxy(void* proxyData) = 0;
	virtual IMaterial*      CheckProxyReplacement(void* proxyData) = 0;
	virtual void			RefreshPreservingMaterialVars() = 0;
	virtual bool			WasReloadedFromWhitelist() = 0;
	virtual bool			IsPrecached() const = 0;
};
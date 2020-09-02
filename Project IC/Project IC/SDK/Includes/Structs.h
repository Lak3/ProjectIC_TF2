#pragma once
#include "../../Utils/Utils.h"

#include "Const.h"

struct model_t;
class IClientRenderable;
typedef unsigned short ModelInstanceHandle_t;
struct StudioData_t;
struct StudioDecalHandle_t;

struct mstudiobbox_t
{
	int bone;
	int group;
	Vec3 bbmin;
	Vec3 bbmax;
	int szhitboxnameindex;
	int unused[8];
};

struct mstudiohitboxset_t
{
	int sznameindex;
	inline char* const name(void) const { return ((char*)this) + sznameindex; }
	int numhitboxes;
	int hitboxindex;
	mstudiobbox_t* hitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};

struct studiohdr_t
{
	unsigned char		pad00[12];
	char				name[64];
	unsigned char		pad01[80];
	int					numbones;
	int					boneindex;
	unsigned char		pad02[12];
	int					hitboxsetindex;
	unsigned char		pad03[228];

	mstudiohitboxset_t* pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((unsigned char*)this) + hitboxsetindex) + i;
	};

	mstudiobbox_t* pHitbox(int i, int set) const
	{
		const mstudiohitboxset_t* s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->hitbox(i);
	};

	int			iHitboxCount(int set) const
	{
		const mstudiohitboxset_t* s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};
};

struct Ray_t
{
	VectorAligned vaStartP;
	VectorAligned vaDelta;
	VectorAligned vaStartOffset;
	VectorAligned vaExtents;

	bool IsRay;
	bool IsSwept;

	void Init(const Vec3 &source, const Vec3 &destination)
	{
		vaDelta = destination - source;

		IsSwept = (vaDelta.LenghtSqr() != 0);

		vaExtents.Set();
		IsRay = true;

		vaStartOffset.Set();
		vaStartP = source;
	}

	void Init(const Vec3& source, const Vec3& destination, const Vec3& min, const Vec3& max)
	{
		vaDelta = destination - source;

		IsSwept = (vaDelta.LenghtSqr() != 0);

		vaExtents = max - min;
		vaExtents *= 0.5f;
		IsRay = (vaExtents.LenghtSqr() < 1e-6);

		vaStartOffset = min + max;

		vaStartOffset *= 0.5f;
		vaStartP = source - vaStartOffset;
		vaStartOffset *= -1.0f;
	}
};

struct cplane_t
{
	Vec3 normal;
	float dist;
	BYTE type;
	BYTE sign_bits;
	BYTE pad[2];
};

struct csurface_t
{
	const char* name;
	short surface_props;
	unsigned short flags;
};

struct PlayerInfo_t
{
	// scoreboard information
	char			name[MAX_PLAYER_NAME_LENGTH];
	// local server user ID, unique while server is running
	int				userID;
	// global unique player identifer
	char			guid[SIGNED_GUID_LEN + 1];
	// friends identification number
	uint32_t		friendsID;
	// friends name
	char			friendsName[MAX_PLAYER_NAME_LENGTH];
	// true, if player is a bot controlled by game.dll
	bool			fakeplayer;
	// true if player is the HLTV proxy
	bool			ishltv;
	// custom files CRC for this player
	unsigned long	customFiles[MAX_CUSTOM_FILES];
	// this counter increases each time the server downloaded a new file
	unsigned char	filesDownloaded;
};

struct DrawModelState_t
{
	studiohdr_t* m_pStudioHdr;
	StudioData_t* m_pStudioData;
	IClientRenderable* m_pRenderable;
	const matrix3x4* m_pModelToWorld;
	StudioDecalHandle_t* m_hDecals;
	int					 m_DrawFlags;
	int					 m_Lod;
};

struct ModelRenderInfo_t
{
	Vec3			      m_vOrigin;
	Vec3			      m_vAngles;
	IClientRenderable*    m_pRenderable;
	const model_t*		  m_pModel;
	const matrix3x4*      m_pModelToWorld;
	const matrix3x4*      m_pLightingOffset;
	const Vec3*           m_pLightingOrigin;
	int				      m_nFlags;
	int				      m_nEntIndex;
	int				      m_nSkin;
	int					  m_nBody;
	int				      m_nHitboxSet;
	ModelInstanceHandle_t m_hMdlInstance;

	ModelRenderInfo_t()
	{
		m_pModelToWorld = NULL;
		m_pLightingOffset = NULL;
		m_pLightingOrigin = NULL;
	}
};

struct StaticPropRenderInfo_t
{
	const matrix3x4*        m_pModelToWorld;
	const model_t*          m_pModel;
	IClientRenderable*      m_pRenderable;
	Vec3*                   m_pLightOrigin;
	short					m_sSkin;
	ModelInstanceHandle_t	m_hMdlInstance;
};
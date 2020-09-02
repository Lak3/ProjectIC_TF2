#pragma once

#include "../../../SDK/SDK.h"

struct RadarInfo_t
{
	int m_nTeam;
	int m_nClass;
	int m_nHealth;
	int m_nMaxHealth;
	int m_nTexture;
	float m_flDistance;
	bool m_bIsFriend;
	Vec3 m_vOrigin;
	Color_t m_clrColor;
	Color_t m_clrHealth;
};

class C_Radar
{
public:
	void Paint();

	bool bActive = false;
	int nNoTeammatePlayers = 2; //0 off 1 all 2 keep friends
	int nNoCloakedPlayers = 0; //0 off 1 all 2 enemies only
	float flRadarSize = 90;
	float flRange = 2000.0f;

private:
	void Drag(int& x, int& y, int w, int h);
	bool ShouldRun();
	bool GetPlayers(C_BaseEntity* pLocal, Vec3 vLocal);

	Color_t GetDrawColor(C_BaseEntity* pLocal, C_BaseEntity* pEntity, bool bIsFriend);

	const Color_t OutlineColor = { 0, 0, 0, 180 };
	const Color_t CondColor = { 254, 202, 87, 255 };
	const Color_t TargetColor = { 153, 128, 250, 255 };
	const Color_t InvulnColor = { 59, 59, 152, 255 };
	const Color_t CloakColor = { 165, 177, 194, 255 };
	const Color_t FriendColor = { 152, 255, 152, 255 };
	const Color_t OverhealColor = { 84, 160, 255, 255 };

	std::vector<RadarInfo_t> m_vecInfo;
};

extern C_Radar gRadar;
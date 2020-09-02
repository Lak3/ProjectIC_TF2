#pragma once
#include "../../../SDK/SDK.h"

class C_ESP
{
public:
	bool bActive = true;
	int nOutline = 1; //0 off 1 text only 2 all

	bool bPlayers				= true;
	int nNoTeammatePlayers		= 2; //0 off 1 all 2 keep friends
	int nNoCloakedPlayers		= 0; //0 off 1 all 2 enemies only
	bool bPlayerName			= true;
	bool bPlayerClass			= true;
	bool bPlayerHealth			= false;
	bool bPlayerCond			= true;
	bool bPlayerHealthBar		= true;
	int nPlayerBox				= 2; //0 off 1 simple 2 corners

	bool bBuildings				= true;
	bool bNoTeammateBuildings	= true;
	bool bBuildingName			= true;
	bool bBuildingHealth		= false;
	bool bBuildingLevel			= true;
	bool bBuildingCond			= true;
	bool bBuildingHealthBar		= true;
	int nBuildingBox			= 2; //0 off 1 simple 2 corners

	bool bPickups	= true;
	bool bHealth	= true;
	bool bAmmo		= true;

private:
	std::wstring ConvertUtf8ToWide(const std::string &str);
	std::string szGetFlag(int nHash);

private:
	bool ShouldRun();
	bool GetDrawBounds(C_BaseEntity *pEntity, int &x, int &y, int &w, int &h);
	Color_t GetDrawColor(C_BaseEntity *pLocal, C_BaseEntity *pEntity);

public:
	void Run();

private:
	Color_t OutlineColor = { 0, 0, 0, 180 };
	Color_t CondColor = { 254, 202, 87, 255 };
	Color_t TargetColor = { 153, 128, 250, 255 };
	Color_t InvulnColor = { 59, 59, 152, 255 };
	Color_t CloakColor = { 165, 177, 194, 255 };
	Color_t FriendColor = { 152, 255, 152, 255 };
	Color_t OverhealColor = { 84, 160, 255, 255 };

	Color_t HealthColor = { 0, 230, 64, 255 };
	Color_t AmmoColor = { 191, 191, 191, 255 };
};

extern C_ESP gESP;
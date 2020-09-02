#pragma once

#include "../../../SDK/SDK.h"

class C_Chams
{
public:
	void Paint(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, const char* szModel);
	void InitMaterials();

	bool bRemoveHands = false;
	int nNoTeammatePlayers = 2; //0 off 1 all 2 keep friends
	int nNoCloakedPlayers = 0; //0 off 1 all 2 enemies only
	int nHandMat = 0;		  //0 off ...
	int nPlayerMat = 0;		 //0 off ...

private:
	IMaterial* GetMat(int nIndex);

	bool ShouldRun();
	Color_t GetDrawColor(C_BaseEntity* pLocal, C_BaseEntity* pEntity, bool bIsFriend);

	const Color_t TargetColor = { 153, 128, 250, 255 };
	const Color_t InvulnColor = { 59, 59, 152, 255 };
	const Color_t CloakColor = { 165, 177, 194, 255 };
	const Color_t FriendColor = { 152, 255, 152, 255 };

private: //Some mats I was testing dont look here
	IMaterial* m_matGlow = nullptr;
	IMaterial* m_matGold = nullptr;
	IMaterial* m_matIce = nullptr;
	IMaterial* m_matHolo = nullptr;
	IMaterial* m_matRobo = nullptr;
	IMaterial* m_matGoldW = nullptr;
	IMaterial* m_matBlur = nullptr;
	IMaterial* m_matMotion = nullptr;
};

extern C_Chams gChams;
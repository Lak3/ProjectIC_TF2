#include "Chams.h"

C_Chams gChams;

//If using, you will notice lots of major changes that should be done.

void C_Chams::Paint(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, const char* szModel)
{
	if (!ShouldRun())
		return;

	if (const auto pLocal = gEntCache.pLocal)
	{
		if (nHandMat && strstr(szModel, "arms"))
		{
			if (pLocal->IsAlive())
			{
				if (auto pHandsMat = GetMat(nHandMat))
				{
					Color_t cTeam = gDraw.GetTeamColor(pLocal->GetTeamNum());

					float flColor[3] =
					{
						Color::TOFLOAT(cTeam.r),
						Color::TOFLOAT(cTeam.g),
						Color::TOFLOAT(cTeam.b)
					};

					pHandsMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					pHandsMat->ColorModulate(flColor[0], flColor[1], flColor[2]);
					pHandsMat->IncrementReferenceCount();

					gInts.RenderView->SetBlend(Color::TOFLOAT(cTeam.a));
					gInts.RenderView->SetColorModulation(flColor);
					gInts.ModelRender->ForcedMaterialOverride(pHandsMat);
				}
			}
		}

		//Switch this up to check if the index is within the indexes that are stored for players
		//To remove the need of gay ass string comparison.
		if (nPlayerMat && strstr(szModel, "models/player"))
		{
			if (auto pEntity = gInts.EntityList->GetClientEntity(pInfo.m_nEntIndex))
			{
				if (!pEntity->IsPlayer() || !pEntity->IsAlive() || pEntity == pLocal)
					return;

				int nLocalTeam = pLocal->GetTeamNum();
				int nEntTeam = pEntity->GetTeamNum();
				bool bIsFriend = pLocal->IsPlayerOnSteamFriendList(pEntity);

				switch (nNoTeammatePlayers)
				{
				case 0: { break; }
				case 1: { if (nEntTeam == nLocalTeam) { return; } break; }
				case 2: { if (nEntTeam == nLocalTeam && !bIsFriend) { return; } break; }
				}

				switch (nNoCloakedPlayers)
				{
				case 0: { break; }
				case 1: { if (pEntity->IsCloaked()) { return; } break; }
				case 2: { if (pEntity->IsCloaked() && nEntTeam != nLocalTeam) { return; } break; }
				}

				if (auto pPlayerMat = GetMat(nPlayerMat))
				{
					Color_t clrColor = GetDrawColor(pLocal, pEntity, bIsFriend);

					float flColor[3] =
					{
						Color::TOFLOAT(clrColor.r),
						Color::TOFLOAT(clrColor.g),
						Color::TOFLOAT(clrColor.b)
					};

					pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					pPlayerMat->ColorModulate(flColor[0], flColor[1], flColor[2]);
					pPlayerMat->IncrementReferenceCount();

					gInts.RenderView->SetBlend(Color::TOFLOAT(clrColor.a));
					gInts.RenderView->SetColorModulation(flColor);
					gInts.ModelRender->ForcedMaterialOverride(pPlayerMat);
				}
			}
		}
	}
}

bool C_Chams::ShouldRun()
{
	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	return true;
}

IMaterial* C_Chams::GetMat(int nIndex)
{
	switch (nIndex)
	{
	case 1: return m_matGlow; break;
	case 2: return m_matGold; break;
	case 3: return m_matIce; break;
	case 4: return m_matHolo; break;
	case 5: return m_matRobo; break;
	case 6: return m_matGoldW; break;
	case 7: return m_matBlur; break;
	case 8: return m_matMotion; break;
	default: return nullptr; break;
	}
}

//Straight from ESP
//Added it here incase we want some specific colors on the chams
Color_t C_Chams::GetDrawColor(C_BaseEntity* pLocal, C_BaseEntity* pEntity, bool bIsFriend)
{
	Color_t clrOut = gDraw.GetTeamColor(pEntity->GetTeamNum());

	if (bIsFriend)
		clrOut = FriendColor;

	if (pEntity->IsPlayer())
	{
		if (pEntity->IsCloaked())
			clrOut = CloakColor;

		if (!pEntity->IsVulnerable())
			clrOut = InvulnColor;
	}

	if (pEntity->GetIndex() == gGlobalInfo.nCurrentTargetIdx)
		clrOut = TargetColor;

	return clrOut;
}

void C_Chams::InitMaterials()
{
	if (!m_matGlow)
	{
		m_matGlow = gInts.MatSystem->Find("dev/glow_color",                          "Model textures");
		m_matGold = gInts.MatSystem->Find("models/player/shared/gold_player",        "Model textures");
		m_matIce = gInts.MatSystem->Find("models/player/shared/ice_player",          "Model textures");
		m_matHolo = gInts.MatSystem->Find("models/effects/solidier_hologram",        "Model textures");
		m_matRobo = gInts.MatSystem->Find("models/effects/robo_marker",              "Model textures");
		m_matGoldW = gInts.MatSystem->Find("models/effects/goldenwrench",            "Model textures");
		m_matBlur = gInts.MatSystem->Find("models/effects/muzzleflash/blurmuzzle",   "Model textures");
		m_matMotion = gInts.MatSystem->Find("models/effects/pyro/pilotlight_motion", "Model textures");
	}
}